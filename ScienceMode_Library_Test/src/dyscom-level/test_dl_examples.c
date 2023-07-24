/***************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2023, MPL and LGPL HASOMED GmbH
*
* Alternatively, the contents of this file may be used under the terms
* of the GNU Lesser General Public License Version 3.0, as described below:
*
* This file is free software: you may copy, redistribute and/or modify
* it under the terms of the GNU Lesser General Public License as published by the
* Free Software Foundation, either version 3.0 of the License, or (at your
* option) any later version.
*
* This file is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
* Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program. If not, see http://www.gnu.org/licenses/.
*
*******************************************************************************/
/***************************************************************************//**
 * \file test_dl_examples.h
 *
 * Minimal example for using smpt protocol with RehaIngest measurement hardware.
 *
 * The example does the following:
 *
 *  1. Open a serial connection to the measurement device (struct Smpt_device)
 *  2. Preparing and processing a measurement of 100 values.
 */

#include "smpt_dl_client.h"
#include "smpt_packet_number_generator.h"
#include <stdio.h>

#define LIVE_DATA_TIME_OFFSET_EXPECTED  500
#define LIVE_DATA_TIME_OFFSET_EPS       10
#define MAX_LOG_ERRORS                  20

#define MAX_LIVE_DATA_OFFSET  (LIVE_DATA_TIME_OFFSET_EXPECTED + LIVE_DATA_TIME_OFFSET_EPS)
#define MIN_LIVE_DATA_OFFSET  (LIVE_DATA_TIME_OFFSET_EXPECTED - LIVE_DATA_TIME_OFFSET_EPS)
/*
 * EDIT: Change to the com port of your device COM0, COM1,...
 * or CNCA0 for com0com virtual com port
 */
static const char* DEFAULT_COM_PORT_NAME = "COM20";

static int test_simple_measurement(void);
static void measurement(Smpt_device *const device);
static void fill_dl_init(Smpt_device *const device, Smpt_dl_init *const dl_init);

static void fill_dl_init_test_signal(Smpt_dl_init *const dl_init);
static void fill_dl_init_bi_simple(Smpt_dl_init *const dl_init);
static void fill_dl_power_module(Smpt_device *const device, Smpt_dl_power_module* const dl_power_module);

static void wait_for_response(Smpt_device *const device, Smpt_Cmd cmd);

static void get_dl_live_data(Smpt_device *const device,
                             Smpt_dl_send_live_data *const dl_live_data);


void test_dl_examples(void)
{
    test_simple_measurement();
}

int test_simple_measurement(void)
{    
    const char *port_name = DEFAULT_COM_PORT_NAME;

    Smpt_device device;
    smpt_clear_device(&device);

    if (!smpt_open_serial_port(&device, port_name))
    {
        printf("Could not open serial port: %s.\n", port_name);
        return -1;
    }
    smpt_packet_number_generator_reset(&device); /* The packet number can be used
                                                    for debugging purposes. */
    measurement(&device);

    if (!smpt_close_serial_port(&device))
    {
        printf("Could not close serial port.\n");
        return -1;
    }

    return 0;
}

/*
 *  This measurement process can be parted into 5 parts
 *
 *  1. Power on the measurement circuits of the measurement device
 *  2. Initializing measurement parameters
 *  3. Starting measurement
 *  4. Receiving measurement results
 *  5. Stopping measurement
 *
 * Notes:
 * This is a minimal example. The purpose is to get a rough understanding of
 * how the science mode protocol works. There are things you should not do in
 * production code. For instance in this example we use busy waiting in a way
 * that will create infinite loops because we wait for commands that may never
 * arrive. Don't do this in production code. Use time outs for such situations.
 *
 * The smpt protocol has two main goals. One is the pure communication, sending
 * and receiving commands. The other goal is to give the user some special
 * commands with meaning for a certain use case. For instance the dyscom-level
 * of the smpt protocl was created for the RehaIngest project. In this project
 * we use a measurement hardware that measures EMG(Electromyografy) and
 * BI(Bioimpendance) values and sends it to a PC.
 *
 * For sending commands from the pc to the measurement device u use
 * API-functions smpt_send_dl_{command name} where {command name} must be
 * replaced by one of the following: power_module, init, get, start, stop
 *
 * For receiving commands from the measurement device you have to call
 * smpt_new_packet_received(). It returns true if a new packet has been found
 * in the input buffer of the serial port otherwise false is returned. If a
 * new packet was found you should call smpt_last_ack to find out which command
 * is wrapped in this packet. Call smpt_get_dl_{command name) to get the command
 * parameters of the command where {command name} must be replaced by one of the
 * following: get_ack, init_ack, start_ack, stop_ack, send_live_data.
 *
 * Most of the commands use parameters which are organized in structs whose
 * type name is similar to the command name itself. For instance there is a
 * struct Smpt_dl_init definition which must be used as a argument type
 * in the smpt_send_dl_init function.
 *
 * Each command that can be sent from the pc to the measurement hardware
 * has a corresponding ack (acknowledge) command which should be sent as
 * an answer of the measurement device to the pc. I say should because there
 * may be transfer errors. If we assume there are now transfer errors then you
 * have a response (ack) for each call.
 * The smpt_get_dl_send_live_data is a function which is used by the pc to
 * get an Smpt_dl_send_live_data dl_send_live_data struct from the measurement
 * device. The sending of live data from the measurement device is a one way
 * communication. Here is a sequence diagram:
 *
 * PC                               Measurement Device
 *
 * smpt_send_dl_power_module    -->
 *                                  receive command
 *                              <-- send answer, switching on/off power module
 * smpt_get_dl_power_module_ack
 *
 * smpt_send_dl_init            -->
 *                                  receive command
 *                              <-- send answer
 * smpt_get_dl_init_ack
 * smpt_send_dl_start           -->
 *                                  receive command
 *                              <-- send answer*
 * smpt_get_dl_start_ack        <-- send live data
 * smpt_get_dl_send_live_data   <-- send live data
 * smpt_get_dl_send_live_data   <-- send live data
 * smpt_get_dl_send_live_data   <-- send live data
 * smpt_get_dl_send_live_data   <-- send live data
 * smpt_get_dl_send_live_data
 * ...
 *
 * smpt_send_dl_stop            -->
 *                                  receive command, stopping measurement
 * smpt_get_dl_send_live_data   <-- send live data
 * (some live data packets may still be sent before the stop answer)
 *                              <-- send stop answer
 * smpt_get_dl_stop_ack
 *
 * The measurement device sends out live data until it receives a dl_stop
 * command from the pc.
 *
 * You can check some parameters of each ack command struct to find out if there
 * have been errors (transfer errors or command parameter interpretation or
 * command execution errors).
 * One common parameter is Smpt_Result result which should be equal to
 * Smpt_Result_Successful in normal case. Another one is uint8_t packet_number.
 * The packet number should be incremented from 0 to 63 and then start from 0
 * again.
 *
 * You have to read the specification and/or the source code of each protocol
 * command carefully to find out which parameters are there and how to use them.
 *
 * todo: use com port and expected time offset as arguments
 */
void measurement(Smpt_device *const device)
{
    /* Init communication */
    Smpt_dl_init                dl_init             = {0};
    Smpt_dl_power_module        dl_power_module     = {0};
    Smpt_dl_send_live_data      dl_live_data        = {0};

    printf("Minimal example for using science mode protocol dyscom level\n");
    printf("------------------------------------------------------------\n");
    printf("1. send dl_power_module to enable the power supply for the "
           "\nmeasurement circuits\n\n");
    fill_dl_power_module(device, &dl_power_module);
    smpt_send_dl_power_module(device, &dl_power_module);
    wait_for_response(device, Smpt_Cmd_Dl_Power_Module_Ack);

    printf("You have to wait 2 seconds after receiving the dl_power_module_ack "
           "\nbecause the circuits need time to get the power\n");
    Sleep(2000);
    printf("Waiting done\n\n");

    printf("2. Send dl_init to initialize the measurement circuits with "
           "\nmeasurement parameters\n");
    fill_dl_init(device, &dl_init);
    smpt_send_dl_init(device, &dl_init);
    wait_for_response(device, Smpt_Cmd_Dl_Init_Ack);

    printf("3. Send dl_start to start the measurement. The measurement device "
           "\nis going to send dl_live_data commands immediately\n");
    smpt_send_dl_start(device, smpt_packet_number_generator_next(device));
    wait_for_response(device, Smpt_Cmd_Dl_Start_Ack);

    printf("4. Receiving live data commands\n");
    /*
     * Firmware with time_offset 0, 250, 250, 250, ...
     * get_dl_live_data_new(device, &dl_live_data);
     */
    get_dl_live_data(device, &dl_live_data);

    printf("5. Sending dl_stop to stop measurement at 100 "
           "received dl_live_data commands.\n");
    smpt_send_dl_stop(device, smpt_packet_number_generator_next(device));
    wait_for_response(device, Smpt_Cmd_Dl_Stop_Ack);

    printf("------------------------- END -----------------------------------\n");

}

void fill_dl_power_module(Smpt_device *const device, Smpt_dl_power_module* const dl_power_module)
{
    dl_power_module->hardware_module = Smpt_Dl_Hardware_Module_Measurement;
    dl_power_module->packet_number = smpt_packet_number_generator_next(device);
    dl_power_module->switch_on_off = 1;
}

void fill_dl_init(Smpt_device *const device, Smpt_dl_init *const dl_init)
{
    smpt_clear_dl_init(dl_init);



    /* the call of the next line overwrites the dl_init parameters of the
       call of the previous line. I must do this because out compiler options
       force me to have no unused functions. Feel free to uncomment in your
       code as needed. */

     fill_dl_init_bi_simple(dl_init);

      fill_dl_init_test_signal(dl_init);
    dl_init->packet_number = smpt_packet_number_generator_next(device);

}

void fill_dl_init_test_signal(Smpt_dl_init *const dl_init)
{                                   /* 76543210  One byte */
    dl_init->ads129x.ch1set = 21;   /* 00010101 */
    dl_init->ads129x.ch2set = 21;   /* 00010101 */
    dl_init->ads129x.ch3set = 128;  /* 10000000 */
    dl_init->ads129x.ch4set = 128;  /* 10000000 */
    dl_init->ads129x.ch5set = 0;
    dl_init->ads129x.ch6set = 0;
    dl_init->ads129x.ch7set = 0;
    dl_init->ads129x.config1 = 3;   /* 00000011 */
    dl_init->ads129x.config2 = 16;  /* 00010000 */
    dl_init->ads129x.config3 = 192; /* 11000000 */
    dl_init->ads129x.config4 = 0;
    dl_init->ads129x.gpio    = 0;
    dl_init->ads129x.id      = 0;

    dl_init->ads129x.gpio       = 0;
    dl_init->ads129x.id         = 0;
    dl_init->ads129x.loff       = 0;
    dl_init->ads129x.loff_flip  = 0;
    dl_init->ads129x.loff_sensn = 0;
    dl_init->ads129x.loff_sensp = 0;
    dl_init->ads129x.loff_statn = 0;
    dl_init->ads129x.loff_statp = 0;
    dl_init->ads129x.pace       = 0;
    dl_init->ads129x.resp       = 0; /* 00000000 */
    dl_init->ads129x.rld_sensn  = 0;
    dl_init->ads129x.rld_sensp  = 0;
    dl_init->ads129x.wct1       = 0;
    dl_init->ads129x.wct2       = 0;

    dl_init->sync_signal            = 0;
    dl_init->mute_flag              = 0;
    dl_init->sd_storage_mode_flag   = false;
    dl_init->live_data_mode_flag    = true;
}

void fill_dl_init_bi_simple(Smpt_dl_init *const dl_init)
{                                   /* 76543210  One byte */

    dl_init->ads129x.ch1set = 16;   /* 00010000 */
    dl_init->ads129x.ch2set = 96;   /* 01100000 */
    dl_init->ads129x.ch3set = 128;  /* 10000000 */
    dl_init->ads129x.ch4set = 0;
    dl_init->ads129x.ch5set = 0;
    dl_init->ads129x.ch6set = 0;
    dl_init->ads129x.ch7set = 0;
    dl_init->ads129x.config1 = 3;   /* 0000011 */
    dl_init->ads129x.config2 = 0;
    dl_init->ads129x.config3 = 192; /* 1100000 */
    dl_init->ads129x.config4 = 0;
    dl_init->ads129x.gpio    = 0;
    dl_init->ads129x.id      = 0;

    dl_init->ads129x.gpio       = 0;
    dl_init->ads129x.id         = 0;
    dl_init->ads129x.loff       = 0;
    dl_init->ads129x.loff_flip  = 0;
    dl_init->ads129x.loff_sensn = 0;
    dl_init->ads129x.loff_sensp = 0;
    dl_init->ads129x.loff_statn = 0;
    dl_init->ads129x.loff_statp = 0;
    dl_init->ads129x.pace       = 0;
    dl_init->ads129x.resp       = 246; /* 11110110 */
    dl_init->ads129x.rld_sensn  = 0;
    dl_init->ads129x.rld_sensp  = 0;
    dl_init->ads129x.wct1       = 0;
    dl_init->ads129x.wct2       = 0;

    dl_init->sync_signal            = 0;
    dl_init->mute_flag              = 0;
    dl_init->sd_storage_mode_flag   = false;
    dl_init->live_data_mode_flag    = true;

}

char* get_command_response_text(Smpt_Cmd cmd)
{
    char* command_text  = "";
    switch (cmd)
    {
        case Smpt_Cmd_Dl_Init_Ack :
            command_text = "Smpt_Cmd_Dl_Init_Ack";
        break;
        case Smpt_Cmd_Dl_Power_Module_Ack :
            command_text = "Smpt_Cmd_Dl_Power_Module_Ack";
        break;
        case Smpt_Cmd_Dl_Start_Ack :
            command_text = "Smpt_Cmd_Dl_Start_Ack";
        break;
        case Smpt_Cmd_Dl_Stop_Ack :
            command_text = "Smpt_Cmd_Dl_Stop_Ack";
        break;
        case Smpt_Cmd_Dl_Send_Live_Data:
            command_text = "Smpt_Cmd_Dl_Send_Live_Data";
        break;
        default:
            command_text = "Unknown command";
    }

    return command_text;
}

void wait_for_response(Smpt_device *const device, Smpt_Cmd cmd)
{
    while (!smpt_new_packet_received(device)) { /* busy waits for response */}

    do
    {
        Smpt_ack ack = {0};
        smpt_last_ack(device, &ack);

        if (ack.command_number == cmd)
        {
            printf("Answer cmd found, cmd: %d %s\n\n", cmd, get_command_response_text(cmd));
            break;
        }
        else
        {
            printf("Unexpected cmd found, cmd: %d %s, \n\n",
                   ack.command_number,
                   get_command_response_text(ack.command_number));
            printf("Expected cmd was, cmd: %d %s, \n\n",
                   cmd,
                   get_command_response_text(cmd));
            Sleep(2);
        }
    }
    while(smpt_new_packet_received(device));
}

void print_out_live_data(Smpt_dl_send_live_data *const dl_live_data)
{
    float values[4];
    int i;
    int n_channels = dl_live_data->n_channels;
    if ((n_channels < 1) || (n_channels > 4))
    {
        /* we expect the number of channels to be between 1 and 4 */
        return;
    }

    for(i = 0; i < n_channels; i++)
    {
        values[i] =   dl_live_data->electrode_samples[i].value;
    }
    printf("(%f, %f, %f, %f, %d, %d)\n",
           values[0],
           values[1],
           values[2],
           values[3],
           dl_live_data->time_offset,
           dl_live_data->packet_number);
}

#include <time.h>

void handle_live_data(int *packetCounter,
                      uint8_t *lastPacketNumber,
                      int32_t *countSampleTimeOffsetErrors,
                      int32_t *countPacketNumberErrors,
                      Smpt_dl_send_live_data *const dl_live_data)
{
    int32_t sampleTimeOffset    = dl_live_data->time_offset;
    uint8_t packetNumber        = dl_live_data->packet_number;

    if ((*packetCounter) > 1)
    {
        if (packetNumber != (((*lastPacketNumber) + 1) % Smpt_Packet_Number_Modulo))
        {
            if ((*countPacketNumberErrors) < MAX_LOG_ERRORS)
            {
                printf("Wrong packet number: last, current, count: %d, %d, %d\n",
                       *lastPacketNumber, packetNumber, *packetCounter);
            }
            countPacketNumberErrors++;

        }
        if ((sampleTimeOffset < MIN_LIVE_DATA_OFFSET) ||
            (sampleTimeOffset > MAX_LIVE_DATA_OFFSET))
        {
            if (*countSampleTimeOffsetErrors < MAX_LOG_ERRORS)
            {
                printf("Live data offset out of bounds: %d\n", sampleTimeOffset);
            }
            (*countSampleTimeOffsetErrors)++;
        }
    }
    *lastPacketNumber = packetNumber;
}

/*
 * Firmware with time_offset 0, 250, 250, ...
 */
void get_dl_live_data(Smpt_device *const device, Smpt_dl_send_live_data *const dl_live_data)
{
    Smpt_ack ack = {0};
    int packetCounter = 0;
    clock_t beginTime, endTime;
    double timeSpent;
    int n = 1000;

    int32_t countSampleTimeOffsetErrors = 0;
    uint8_t lastPacketNumber = 0;
    int32_t countPacketNumberErrors = 0;

    beginTime = clock();
    while( packetCounter < n)
    {
        while (!smpt_new_packet_received(device)) { }
        packetCounter++;
        smpt_last_ack(device, &ack);

        if (ack.command_number == Smpt_Cmd_Dl_Send_Live_Data)
        {
            smpt_get_dl_send_live_data(device, dl_live_data);
            handle_live_data(&packetCounter,
                             &lastPacketNumber,
                             &countSampleTimeOffsetErrors,
                             &countPacketNumberErrors,
                             dl_live_data);

        }
    }
    endTime = clock();
    timeSpent = (double)(endTime - beginTime) / CLOCKS_PER_SEC;
    printf("\nReceived %d live data samples in %f seconds\n"
           "Frequency is about %f Hz\n"
           "Number of wrong time offsets: %d \n"
           "Number of wrong packet numbers: %d\n\n",
           packetCounter, timeSpent,
           ((double)packetCounter) / timeSpent,
           countSampleTimeOffsetErrors,
           countPacketNumberErrors);   
}
