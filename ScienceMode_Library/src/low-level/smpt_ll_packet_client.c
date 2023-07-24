/***************************************************************************//**
* \file smpt_ll_packet_client.c
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

#include "smpt_ll_packet_client.h"
#include "smpt_packet_internal.h"
#include "smpt_ll_packet_validity.h"
#include "smpt_definitions_internal.h"
#include "smpt_messages.h"
#include "smpt_packet_client.h"
#include "smpt_packet_utils.h"

static uint32_t insert_ll_init(uint8_t buffer_unstuffed[], const Smpt_ll_init *const ll_init,
                               uint32_t index);

static uint32_t insert_ll_channel_config(uint8_t buffer_unstuffed[],
                                    const Smpt_ll_channel_config *const ll_channel_config,
                                    uint32_t index);

static uint32_t insert_points(uint8_t buffer_unstuffed[],
                              const Smpt_ll_channel_config *const ll_channel_config,
                              uint32_t index);

uint32_t smpt_build_ll_init(uint8_t buffer[], uint32_t buffer_length,
                            const Smpt_ll_init *const ll_init)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer,           Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_ll_init(buffer_unstuffed, ll_init, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ll_Init, ll_init->packet_number);

    return packet_length;
}


uint32_t insert_ll_init(uint8_t buffer_unstuffed[], const Smpt_ll_init *const ll_init,
                        uint32_t index)
{
    buffer_unstuffed[index++] = ((ll_init->measurement_type & 0x07) << 4) |
                                ((ll_init->high_voltage_level & 0x07) << 1);

    return index;
}


uint32_t smpt_build_ll_channel_config(uint8_t buffer[], uint32_t buffer_length,
                                      const Smpt_ll_channel_config *const ll_channel_config)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);
    smpt_clear_buffer(buffer,           Smpt_Limit_Max_Packet_Size);

    last_index = insert_ll_channel_config(buffer_unstuffed, ll_channel_config,
                                          Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ll_Channel_Config, ll_channel_config->packet_number);
    return packet_length;
}


uint32_t insert_ll_channel_config(uint8_t buffer_unstuffed[],
                                  const Smpt_ll_channel_config *const ll_channel_config,
                                  uint32_t index)
{
    uint8_t number_of_points = (ll_channel_config->number_of_points == 0) ?
                                0 : ll_channel_config->number_of_points - 1;

    uint8_t channel = ll_channel_config->channel;
    uint8_t connector = ll_channel_config->connector;
    buffer_unstuffed[index++] = ((ll_channel_config->enable_stimulation << 7)    & 0x80) |
                                ((channel << 5)                                  & 0x60) |
                                ((connector << 4)          & 0x10) |
                                 (number_of_points & 0x0f);

    index = insert_points(buffer_unstuffed, ll_channel_config, index);

    return index;
}


uint32_t insert_points(uint8_t buffer_unstuffed[],
                       const Smpt_ll_channel_config *const ll_channel_config,
                       uint32_t index)
{
    if (ll_channel_config->enable_stimulation)
    {
        uint8_t i;
        const uint8_t number_of_points = ll_channel_config->number_of_points;
        for (i = 0; i < number_of_points; i++)
        {
            Smpt_ll_point point = ll_channel_config->points[i];
            index = smpt_insert_ll_point(buffer_unstuffed, &point, index);
        }
    }

    return index;
}


uint32_t smpt_insert_ll_point(uint8_t buffer_unstuffed[], const Smpt_ll_point *const ll_point,
                      uint32_t index)
{
    uint16_t current = (uint16_t)(ll_point->current * 2 + 300);

    buffer_unstuffed[index] = (uint8_t) (ll_point->time >> 4);
    index++;
    buffer_unstuffed[index] = (((uint8_t) (ll_point->time << 4)) & 0xf0) |
                              (((uint8_t) (current >> 6)         & 0x0f));
    index++;
    buffer_unstuffed[index] = ((uint8_t) (current << 2)      & 0xfc);
    index++;
    buffer_unstuffed[index] = ((ll_point->interpolation_mode << 5) & 0x60);
    index++;

    return index;
}


bool smpt_extract_ll_init_ack(Smpt_ll_init_ack *const ll_init_ack, const uint8_t buffer[],
                              uint32_t packet_length)
{
    bool result = false;
    uint32_t index = Smpt_Limit_Header_Size;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, buffer, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ll_Init_Ack)
    {
        ll_init_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
        ll_init_ack->result = (Smpt_Result) buffer_unstuffed[index++];
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ll_init_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ll_Init_Ack);
    }

    result = result && (smpt_is_valid_ll_init_ack(ll_init_ack));

    return result;
}


bool smpt_extract_ll_channel_config_ack(Smpt_ll_channel_config_ack
                                    *const ll_channel_config_ack, const uint8_t buffer[],
                                    uint32_t packet_length)
{
    bool result = false;
    uint32_t index = Smpt_Limit_Header_Size;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, buffer, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ll_Channel_Config_Ack)
    {
        ll_channel_config_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

        ll_channel_config_ack->result = (Smpt_Result) buffer_unstuffed[index];
        index++;

        ll_channel_config_ack->channel = (Smpt_Channel) (buffer_unstuffed[index] & 0x0f);
        ll_channel_config_ack->connector = (Smpt_Connector) ((buffer_unstuffed[index] & 0xf0) >> 4);
        index++;
        ll_channel_config_ack->measurement_type = (Smpt_Measurement)buffer_unstuffed[index];
        index++;

        if ((ll_channel_config_ack->measurement_type != Smpt_Measurement_None))
        {

            ll_channel_config_ack->measurement_sampling_time_us = buffer_unstuffed[index] << 8 | buffer_unstuffed[index + 1];
            index += 2;
            for (uint8_t i = 0; i<128; i++)
            {
                ll_channel_config_ack->measurement[i] = buffer_unstuffed[index] << 8 | buffer_unstuffed[index + 1];
                index += 2;
            }
        }
        else
        {
            ll_channel_config_ack->measurement_sampling_time_us = 0;
            for (uint8_t i = 0; i<128; i++)
                ll_channel_config_ack->measurement[i] = 0;
        }

        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ll_channel_config_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ll_Channel_Config_Ack);
    }

    result = result && (smpt_is_valid_ll_channel_config_ack(ll_channel_config_ack));

    return result;
}

