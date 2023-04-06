/***************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2022, MPL and LGPL HASOMED GmbH
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
*******************************************************************************/
#include "test_ll_examples.h"

#include "smpt_ll_client.h"

static void test_single_pulse(const char *port_name);
static void test_wait_for_ll_ack(const char *port_name);

int test_single_pulse_main(void)
{
    /* EDIT: Change to the virtual com port of your device */
    /* const char *port_name = "COM5"; */
    const char *port_name = "CNCA0";

    test_wait_for_ll_ack(port_name);
    test_single_pulse(port_name);
    return 0;
}


void test_single_pulse(const char *port_name)
{
    uint8_t packet_number = 0;  /* The packet_number can be used for debugging purposes */
    Smpt_ll_init ll_init = {0};       /* Struct for ll_init command */
    Smpt_ll_channel_config ll_channel_config = {0};   /* Struct for ll_channel_config command */

    Smpt_device device = {0};
    smpt_open_serial_port(&device, port_name);

    /* Clear ll_init struct and set the data */
    smpt_clear_ll_init(&ll_init);
    ll_init.packet_number = packet_number;

    /* Send the ll_init command to stimulation unit */
    smpt_send_ll_init(&device, &ll_init);

    packet_number++;

    /* Set the data */
    ll_channel_config.enable_stimulation = true;
    ll_channel_config.channel = Smpt_Channel_Blue;  /* Use blue channel */
    ll_channel_config.number_of_points = 3;         /* Set the number of points*/
    ll_channel_config.packet_number = packet_number;

    /* Set the stimulation pulse */
    /* First point, current: 20 mA, positive, pulse width: 200 µs */
    ll_channel_config.points[0].current =  20;
    ll_channel_config.points[0].time    = 200;

    /* Second point, pause 100 µs */
    ll_channel_config.points[1].time = 100;

    /* Third point, current: -20 mA, negative, pulse width: 200 µs */
    ll_channel_config.points[2].current = -20;
    ll_channel_config.points[2].time    = 200;

    /* Send the ll_channel_list command to the stimulation unit */
    smpt_send_ll_channel_config(&device, &ll_channel_config);

    packet_number++;

    /* Send the ll_stop command to the stimulation unit */
    smpt_send_ll_stop(&device, packet_number);

    smpt_close_serial_port(&device);
}

void test_wait_for_ll_ack(const char *port_name)
{
    uint8_t packet_number = 0;  /* The packet_number can be used for debugging purposes */
    Smpt_ll_init ll_init = {0};          /* Struct for ll_init command */
    Smpt_ll_init_ack ll_init_ack = {0};  /* Struct for ll_init_ack response */
    Smpt_ack ack = {0};            /* Struct for general response */

    Smpt_device device = {0};
    smpt_open_serial_port(&device, port_name);

    /* Clear ll_init struct and set the data */
    smpt_clear_ll_init(&ll_init);
    ll_init.packet_number = packet_number;

    /* Send the ll_init command to the stimulation unit */
    smpt_send_ll_init(&device, &ll_init);

    packet_number++;

    while (!smpt_new_packet_received(&device)) { /* busy waits for Ll_init_ack response */}

    smpt_clear_ack(&ack);
    smpt_last_ack(&device, &ack);
    if (ack.command_number == Smpt_Cmd_Ll_Init_Ack)
    {
        smpt_get_ll_init_ack(&device, &ll_init_ack);  /* Writes the received data into ll_init_ack */
    }

    /* Send the Ll_stop command to stimulation unit */
    smpt_send_ll_stop(&device, packet_number);

    while (!smpt_new_packet_received(&device)) { /* busy waits for ll_stop_ack */ }

    smpt_clear_ack(&ack);
    smpt_last_ack(&device, &ack);
    if (ack.command_number == Smpt_Cmd_Ll_Stop_Ack)
    {
        /* Ll_stop_ack has been received */
    }

    smpt_close_serial_port(&device);
}

void test_ll_demux(const char *port_name)
{
    uint8_t packet_number = 0;  /* The packet_number can be used for debugging purposes */
    Smpt_ll_init ll_init = {0};       /* Struct for ll_init command */
    Smpt_ll_channel_config ll_channel_config = {0};   /* Struct for ll_channel_config command */
    Smpt_ll_demux ll_demux = {0};     /* Struct for demux configuration */

    Smpt_device device = {0};
    smpt_open_serial_port(&device, port_name);

    /* Clear ll_init struct and set the data */
    smpt_clear_ll_init(&ll_init);
    ll_init.packet_number = packet_number;

    /* Enable demux */
    ll_init.enable_demux  = true;

    /* Send the ll_init command to stimulation unit */
    smpt_send_ll_init(&device, &ll_init);

    packet_number++;

    /* Configure the demux and put it into ll_channel_config */
    ll_demux.demux_id = Smpt_Demux_Rt_72;
    ll_demux.electrodes[0]  = Smpt_Ll_Demux_Positive;
    ll_demux.electrodes[60] = Smpt_Ll_Demux_Negative;
    smpt_ll_demux_convert(&ll_channel_config, &ll_demux);

    /* Set the variable for transmitting the demux configuration */
    ll_channel_config.modify_demux = true;

    /* Set the stimulation data */
    ll_channel_config.enable_stimulation = true;
    ll_channel_config.channel = Smpt_Channel_Blue;  /* Use blue channel */
    ll_channel_config.number_of_points = 3;         /* Set the number of points*/
    ll_channel_config.packet_number = packet_number;

    /* Set the stimulation pulse */
    /* First point, current: 20 mA, positive, pulse width: 200 µs */
    ll_channel_config.points[0].current =  20;
    ll_channel_config.points[0].time    = 200;

    /* Second point, pause 100 µs */
    ll_channel_config.points[1].time = 100;

    /* Third point, current: -20 mA, negative, pulse width: 200 µs */
    ll_channel_config.points[2].current = -20;
    ll_channel_config.points[2].time    = 200;

    /* Send the ll_channel_list command to the stimulation unit */
    smpt_send_ll_channel_config(&device, &ll_channel_config);

    packet_number++;

    /* Send the ll_stop command to the stimulation unit */
    smpt_send_ll_stop(&device, packet_number);

    smpt_close_serial_port(&device);
}
