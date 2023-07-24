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
*******************************************************************************/
#include "smpt_ml_packet_utils.h"
#include "smpt_ll_packet_client.h"
#include "smpt_ll_packet_server.h"
#include "smpt_packet_internal.h"

static uint32_t insert_points(uint8_t buffer_unstuffed[], const Smpt_ml_update *const ml_update,
                       uint32_t index, uint8_t channel);

static uint32_t insert_channels(uint8_t buffer_unstuffed[], const Smpt_ml_update *const ml_update,
                                uint32_t index);

static uint32_t extract_points(Smpt_ml_update *const ml_update, const uint8_t buffer_unstuffed[],
                               uint32_t index, uint8_t channel);

uint32_t smpt_utils_insert_ml_update(uint8_t buffer_unstuffed[], const Smpt_ml_update *const ml_update,
                          uint32_t index)
{
    uint8_t enable_channels = 0;

    uint8_t i = 0;
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (ml_update->enable_channel[i])
        {
            enable_channels |= 1 << i;
        }
    }

    // enable_channels |=  (ml_update->softstart & 0x1) << 4;

    buffer_unstuffed[index] = enable_channels;
    index++;

    index = insert_channels(buffer_unstuffed, ml_update, index);

    return index;
}


uint32_t insert_channels(uint8_t buffer_unstuffed[], const Smpt_ml_update *const ml_update,
                         uint32_t index)
{
    uint8_t i = 0;
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (ml_update->enable_channel[i])
        {
            uint16_t period;
            uint8_t number_of_points = ml_update->channel_config[i].number_of_points - 1;
            buffer_unstuffed[index] = (((number_of_points << 4) & 0xf0) |
                                       (ml_update->channel_config[i].ramp & 0x0f));
            index++;
            if (ml_update->channel_config[i].double_period_accuracy)
                period = (uint16_t)(ml_update->channel_config[i].period * 4);
            else
                period = (uint16_t)(ml_update->channel_config[i].period * 2);
            buffer_unstuffed[index] = (uint8_t)((period << 1) >> 8);
            index++;

            buffer_unstuffed[index] = ((uint8_t)((period << 1) & 0xfe)) |
                                      (ml_update->channel_config[i].double_period_accuracy & 0x01);
            index++;

            index = insert_points(buffer_unstuffed, ml_update, index, i);
        }
    }

    return index;
}


uint32_t insert_points(uint8_t buffer_unstuffed[], const Smpt_ml_update *const ml_update,
                       uint32_t index, uint8_t channel)
{
    const uint8_t number_of_points = ml_update->channel_config[channel].number_of_points;
    uint8_t i = 0;
    for (i = 0; i < number_of_points; i++)
    {
        index = smpt_insert_ll_point(buffer_unstuffed,
                                     &ml_update->channel_config[channel].points[i],
                                     index);
    }

    return index;
}

void smpt_utils_extract_ml_update(Smpt_ml_update *const ml_update, const uint8_t buffer_unstuffed[],
                                  uint32_t index)
{
    uint8_t temp_byte;
    uint8_t i;

    ml_update->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    temp_byte = buffer_unstuffed[index];
    index++;
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        ml_update->enable_channel[i] = ((temp_byte & (1 << i)) >> i);
    }

    //ml_update->softstart = (temp_byte & (1 << 4) >> 4);

    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (ml_update->enable_channel[i])
        {
            uint8_t high;
            uint8_t low;

            temp_byte = buffer_unstuffed[index];
            index++;
            ml_update->channel_config[i].number_of_points = ((temp_byte & 0xf0) >> 4) + 1;
            ml_update->channel_config[i].ramp             =  (temp_byte & 0x0f);

            high = buffer_unstuffed[index];
            index++;

            low = buffer_unstuffed[index];
            index++;
            ml_update->channel_config[i].double_period_accuracy = (low & 0x01);
            if (ml_update->channel_config[i].double_period_accuracy)
                ml_update->channel_config[i].period = (float)
                                                      ((((uint16_t)high << 8) | (low & 0xfe)) >> 1) / 4;
            else
                ml_update->channel_config[i].period = (float)
                                                      ((((uint16_t)high << 8) | (low & 0xfe)) >> 1) / 2;

            index = extract_points(ml_update, buffer_unstuffed, index, i);
        }
    }
}

uint32_t extract_points(Smpt_ml_update *const ml_update, const uint8_t buffer_unstuffed[],
                        uint32_t index, uint8_t channel)
{
    uint8_t i;
    const uint8_t number_of_points = ml_update->channel_config[channel].number_of_points;
    for (i = 0; i < number_of_points; i++)
    {
        index = smpt_extract_ll_point(&ml_update->channel_config[channel].points[i],
                                      buffer_unstuffed, index);
    }

    return index;
}

