/***************************************************************************//**
* \file smpt_ml_packet_client.c
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

#include "smpt_ml_packet_client.h"
#include "smpt_ml_packet_validity.h"
#include "smpt_ml_packet_utils.h"

#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "smpt_packet_validity.h"

#include "smpt_ll_packet_client.h"
#include "smpt_messages.h"

static uint32_t insert_ml_init(uint8_t buffer_unstuffed[], const Smpt_ml_init *const ml_init,
                               uint32_t index);

static uint32_t insert_ml_get_current_data(uint8_t buffer_unstuffed[],
                                               const Smpt_ml_get_current_data
                                               *const ml_get_current_data,
                                               uint32_t index);

static void extract_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                            *const ml_get_current_data_ack,
                                            const uint8_t buffer_unstuffed[]);

static uint32_t extract_stimulation_data(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                     const uint8_t buffer_unstuffed[], uint32_t index);
static uint32_t extract_stimulation_data_eight_channels(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                  const uint8_t buffer_unstuffed[], uint32_t index);
static uint32_t extract_channel_state(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                     const uint8_t buffer_unstuffed[], uint32_t index);


uint32_t smpt_build_ml_init(uint8_t buffer[], uint32_t buffer_length,
                            const Smpt_ml_init *const ml_init)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_ml_init(buffer_unstuffed, ml_init, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ml_Init, ml_init->packet_number);

    return packet_length;
}


uint32_t insert_ml_init(uint8_t buffer_unstuffed[], const Smpt_ml_init *const ml_init,
                        uint32_t index)
{
    buffer_unstuffed[index] = ml_init->stop_all_channels_on_error;
    index++;

    return index;
}


uint32_t smpt_build_ml_update(uint8_t buffer[], uint32_t buffer_length,
                              const Smpt_ml_update *const ml_update)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = smpt_utils_insert_ml_update(buffer_unstuffed, ml_update, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ml_Update, ml_update->packet_number);

    return packet_length;
}


uint32_t smpt_build_ml_get_current_data(uint8_t buffer[], uint32_t buffer_length,
                                        const Smpt_ml_get_current_data *const ml_get_current_data)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_ml_get_current_data(buffer_unstuffed, ml_get_current_data,
                                            Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ml_Get_Current_Data,
                                      ml_get_current_data->packet_number);

    return packet_length;
}

uint32_t insert_ml_get_current_data(uint8_t buffer_unstuffed[],
                                    const Smpt_ml_get_current_data *const ml_get_current_data,
                                    uint32_t index)
{

    buffer_unstuffed[index] = ml_get_current_data->data_selection & 0xff;
    index++;

    return index;
}

uint32_t smpt_build_ml_stop(uint8_t buffer[], uint32_t buffer_length, uint8_t packet_number)
{
    Smpt_cmd cmd;
    cmd.command_number = Smpt_Cmd_Ml_Stop;
    cmd.packet_number = packet_number;

    return smpt_build_cmd(buffer, buffer_length, &cmd);
}

bool smpt_extract_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                          *const ml_get_current_data_ack,
                                          const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_ml_get_current_data_ack(ml_get_current_data_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ml_Get_Current_Data_Ack)
    {
        extract_ml_get_current_data_ack(ml_get_current_data_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ml_get_current_data_ack(): Packet length too short"
                                                                "(%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ml_Get_Current_Data_Ack);
    }

    result = result && (smpt_is_valid_ml_get_current_data_ack(ml_get_current_data_ack));

    return result;
}

void extract_ml_get_current_data_ack(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                     const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    ml_get_current_data_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    ml_get_current_data_ack->result = (Smpt_Result) buffer_unstuffed[index];
    index++;

    ml_get_current_data_ack->data_selection = (Smpt_Ml_Data)buffer_unstuffed[index];

    index++;

    if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Stimulation)
    {
        index = extract_stimulation_data(ml_get_current_data_ack, buffer_unstuffed, index);
    }
    else if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Stimulation_Eight_Channels)
    {
        index = extract_stimulation_data_eight_channels(ml_get_current_data_ack, buffer_unstuffed, index);
    }
    else if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Channels)
    {
        index = extract_channel_state(ml_get_current_data_ack, buffer_unstuffed, index);
    }

}

uint32_t extract_stimulation_data(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                  const uint8_t buffer_unstuffed[], uint32_t index)
{
    uint8_t i;
    ml_get_current_data_ack->stimulation_data.is_valid=true;
    ml_get_current_data_ack->channel_data.is_valid=false;
    ml_get_current_data_ack->stimulation_data.stimulation_state = (Smpt_Ml_Stimulation_State)
                                                        ((buffer_unstuffed[index] & 0x10) >> 4);
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (i <= 4)
            ml_get_current_data_ack->stimulation_data.electrode_error[i] =
                                                  ((buffer_unstuffed[index] & 0x0f) >> i) & 0x01;
        else
            ml_get_current_data_ack->stimulation_data.electrode_error[i] = false;
    }

    index++;


    return index;
}

uint32_t extract_stimulation_data_eight_channels(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                  const uint8_t buffer_unstuffed[], uint32_t index)
{
    uint8_t i;
    ml_get_current_data_ack->stimulation_data.is_valid=true;
    ml_get_current_data_ack->channel_data.is_valid=false;
    ml_get_current_data_ack->stimulation_data.stimulation_state = (Smpt_Ml_Stimulation_State)
                                                   ((buffer_unstuffed[index] & 0x10) >> 4);

    for (i = 0; i < 4; i++)
    {
        ml_get_current_data_ack->stimulation_data.electrode_error[i] =
                                                  ((buffer_unstuffed[index] & 0x0f) >> i) & 0x01;
    }
    index++;
    if (Smpt_Length_Number_Of_Channels > 4)
    {
        for (i = 4; i < Smpt_Length_Number_Of_Channels; i++)
        {
            ml_get_current_data_ack->stimulation_data.electrode_error[i] =
                                                      ((buffer_unstuffed[index] & 0x0f) >> (i-4)) & 0x01;
        }
        index++;
    }

    return index;
}

uint32_t extract_channel_state(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack,
                                  const uint8_t buffer_unstuffed[], uint32_t index)
{
    uint8_t i, temp_byte;
    ml_get_current_data_ack->channel_data.is_valid=true;
    ml_get_current_data_ack->stimulation_data.is_valid=false;
    temp_byte = buffer_unstuffed[index];

    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        ml_get_current_data_ack->channel_data.stimulation_state[i] = (Smpt_Ml_Stimulation_State)((temp_byte & (1 << i)) >> i);
    }
    index++;
    temp_byte = buffer_unstuffed[index];
    index++;
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (i%2==0)
            ml_get_current_data_ack->channel_data.channel_state[i] = (Smpt_Ml_Channel_State)(temp_byte & 0x0f);
        else
        {
            ml_get_current_data_ack->channel_data.channel_state[i] = (Smpt_Ml_Channel_State)((temp_byte & 0xf0) >> 4);
            temp_byte = buffer_unstuffed[index];
            index++;
        }

    }


    return index;
}

