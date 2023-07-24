/***************************************************************************//**
* \file smpt_ll_packet_server.c
* \copyright MPL and LGPL HASOMED GmbH 2013-2022
*
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
*******************************************************************************/

#include "smpt_ll_packet_server.h"
#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "smpt_ll_packet_validity.h"
#include "smpt_messages.h"
#include "smpt_packet_server.h"
#include "smpt_packet_utils.h"

/* File scope functions */
static void extract_ll_init(Smpt_ll_init *const ll_init, const uint8_t buffer_unstuffed[]);

static void extract_ll_channel_config(Smpt_ll_channel_config *const ll_channel_config,
                                     const uint8_t buffer_unstuffed[]);

static uint32_t extract_points(Smpt_ll_channel_config *const ll_channel_config,
                               const uint8_t buffer_unstuffed[], uint32_t index);

static uint32_t insert_ll_init_ack(uint8_t buffer_unstuffed[],
                                   const Smpt_ll_init_ack *const ll_init_ack, uint32_t index);

static uint32_t insert_ll_channel_config_ack(uint8_t buffer_unstuffed[],
                                             const Smpt_ll_channel_config_ack
                                             *const ll_channel_config_ack, uint32_t index);


bool smpt_extract_ll_init(Smpt_ll_init *const ll_init, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_ll_init(ll_init);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ll_Init)
    {
        extract_ll_init(ll_init, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ll_init(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ll_Init);
    }

    result = result && (smpt_is_valid_ll_init(ll_init));

    return result;
}


void extract_ll_init(Smpt_ll_init *const ll_init, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    uint8_t temp_byte;
    ll_init->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    temp_byte = buffer_unstuffed[index++];

    ll_init->measurement_type = (Smpt_Measurement) ((temp_byte & 0x70) >> 4);
    ll_init->high_voltage_level = (Smpt_High_Voltage) ((temp_byte & 0x0E) >> 1);
}


bool smpt_extract_ll_channel_config(Smpt_ll_channel_config *const ll_channel_config,
                                    const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);
    smpt_clear_ll_channel_config(ll_channel_config);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ll_Channel_Config)
    {
        extract_ll_channel_config(ll_channel_config, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ll_channel_config(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ll_Channel_Config);
    }

    result = result && (smpt_is_valid_ll_channel_config(ll_channel_config));

    return result;
}


void extract_ll_channel_config(Smpt_ll_channel_config *const ll_channel_config,
                              const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    uint8_t temp_byte;
    uint8_t number_of_points;

    ll_channel_config->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    temp_byte = buffer_unstuffed[index];
    ll_channel_config->enable_stimulation = (temp_byte & Smpt_Bitmask_Use_Module) >> 7;
    ll_channel_config->channel = (Smpt_Channel) ((temp_byte & Smpt_Bitmask_Channel) >> 5);
    ll_channel_config->connector = (Smpt_Connector)(temp_byte & Smpt_Bitmask_Connector) >> 4;
    number_of_points = (temp_byte & Smpt_Bitmask_Number_Of_Points);
    index++;

    ll_channel_config->number_of_points = number_of_points + 1;

    index = extract_points(ll_channel_config, buffer_unstuffed, index);
}


uint32_t extract_points(Smpt_ll_channel_config *const ll_channel_config,
                        const uint8_t buffer_unstuffed[], uint32_t index)
{
    if (ll_channel_config->enable_stimulation)
    {
        uint8_t i;
        const uint8_t number_of_points = ll_channel_config->number_of_points;
        for (i = 0; i < number_of_points; i++)
        {
            index = smpt_extract_ll_point(&ll_channel_config->points[i], buffer_unstuffed, index);
        }
    }

    return index;
}


uint32_t smpt_extract_ll_point(Smpt_ll_point *const ll_point,
                               const uint8_t buffer_unstuffed[], uint32_t index)
{
    ll_point->time = ((uint16_t)(buffer_unstuffed[index]) << 4) |
                     ((buffer_unstuffed[index + 1] & Smpt_Bitmask_Time) >> 4);
    index++;

    ll_point->current = (float)(
                        (((uint16_t)(buffer_unstuffed[index] & Smpt_Bitmask_Current)) << 6) |
                        (buffer_unstuffed[index + 1] >> 2));
    index++;
    ll_point->current = (ll_point->current - 300) / 2;
    index++;

    ll_point->interpolation_mode = (buffer_unstuffed[index] & Smpt_Bitmask_Interpolation_Mode) >> 5;
    index++;

    return index;
}


uint32_t smpt_build_ll_init_ack(uint8_t buffer[], uint32_t buffer_length,
                                const Smpt_ll_init_ack *const ll_init_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_ll_init_ack(buffer_unstuffed, ll_init_ack, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ll_Init_Ack, ll_init_ack->packet_number);
    return packet_length;
}


uint32_t insert_ll_init_ack(uint8_t buffer_unstuffed[],
                            const Smpt_ll_init_ack *const ll_init_ack, uint32_t index)
{
    buffer_unstuffed[index++] = ll_init_ack->result;

    return index;
}


uint32_t smpt_build_ll_channel_config_ack(uint8_t buffer[], uint32_t buffer_length,
                                          const Smpt_ll_channel_config_ack *const ll_channel_config_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_ll_channel_config_ack(buffer_unstuffed, ll_channel_config_ack,
                                              Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ll_Channel_Config_Ack,
                                      ll_channel_config_ack->packet_number);
    return packet_length;
}


uint32_t insert_ll_channel_config_ack(uint8_t buffer_unstuffed[],
                                      const Smpt_ll_channel_config_ack
                                      *const ll_channel_config_ack,
                                      uint32_t index)
{
    buffer_unstuffed[index] = ll_channel_config_ack->result;
    index++;

    buffer_unstuffed[index] = (((ll_channel_config_ack->connector & 0x0f) << 4) |
            (ll_channel_config_ack->channel & 0x0f));
    index++;
    buffer_unstuffed[index] = ll_channel_config_ack->measurement_type & 0xFF;
    index++;
    if ((ll_channel_config_ack->measurement_type != Smpt_Measurement_None))
    {
        buffer_unstuffed[index] = (ll_channel_config_ack->measurement_sampling_time_us >> 8) & 0xFF;
        index++;
        buffer_unstuffed[index] = ll_channel_config_ack->measurement_sampling_time_us & 0xFF;
        index++;
        for (uint8_t i = 0; i<128; i++)
        {
            buffer_unstuffed[index] = (ll_channel_config_ack->measurement[i] >> 8) & 0xFF;
            index++;
            buffer_unstuffed[index] = ll_channel_config_ack->measurement[i] & 0xFF;
            index++;
        }

    }

    return index;
}
