/***************************************************************************//**
* \file smpt_ml_packet_server.c
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

#include "smpt_ml_packet_server.h"
#include "smpt_ml_packet_validity.h"
#include "smpt_ml_packet_utils.h"

#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"

#include "smpt_ll_packet_server.h"
#include "smpt_messages.h"

static void extract_ml_init(Smpt_ml_init *const ml_init, const uint8_t buffer_unstuffed[]);

static void extract_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data,
                                        const uint8_t buffer_unstuffed[]);

static uint32_t insert_ml_get_current_data_ack(uint8_t buffer_unstuffed[],
                                               const Smpt_ml_get_current_data_ack
                                               *const ml_get_current_data_ack,
                                               uint32_t index);

static uint32_t insert_stimulation_data(uint8_t buffer_unstuffed[],
                                        const Smpt_ml_get_current_data_ack
                                        *const ml_get_current_data_ack, uint32_t index);

static uint32_t insert_stimulation_data_eight_channels(uint8_t buffer_unstuffed[], const Smpt_ml_get_current_data_ack
                             *const ml_get_current_data_ack, uint32_t index);

static uint32_t insert_channels_data(uint8_t buffer_unstuffed[], const Smpt_ml_get_current_data_ack
                                     *const ml_get_current_data_ack, uint32_t index);

bool smpt_extract_ml_init(Smpt_ml_init *const ml_init, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_ml_init(ml_init);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ml_Init)
    {
        extract_ml_init(ml_init, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ml_init(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ml_Init);
    }

    result = result && (smpt_is_valid_ml_init(ml_init));

    return result;
}

void extract_ml_init(Smpt_ml_init *const ml_init, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    ml_init->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    ml_init->stop_all_channels_on_error = buffer_unstuffed[index] & 0x1;
    index++;
}

bool smpt_extract_ml_update(Smpt_ml_update *const ml_update,
                            const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_ml_update(ml_update);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ml_Update)
    {
        smpt_utils_extract_ml_update(ml_update, buffer_unstuffed, Smpt_Length_Header_Size);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ml_update(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ml_Update);
    }

    result = result && (smpt_is_valid_ml_update(ml_update));

    return result;
}

bool smpt_extract_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data,
                                      const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_ml_get_current_data(ml_get_current_data);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ml_Get_Current_Data)
    {
        extract_ml_get_current_data(ml_get_current_data, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ml_get_current_data(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ml_Get_Current_Data);
    }

    result = result && (smpt_is_valid_ml_get_current_data(ml_get_current_data));

    return result;
}

void extract_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data,
                                 const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    ml_get_current_data->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    ml_get_current_data->data_selection = (Smpt_Ml_Data) buffer_unstuffed[index];
    index++;

}

uint32_t smpt_build_ml_get_current_data_ack(uint8_t buffer[], uint32_t buffer_length,
                                            const Smpt_ml_get_current_data_ack
                                            *const ml_get_current_data_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_ml_get_current_data_ack(buffer_unstuffed, ml_get_current_data_ack,
                                                Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Ml_Get_Current_Data_Ack,
                                      ml_get_current_data_ack->packet_number);

    return packet_length;
}

uint32_t insert_ml_get_current_data_ack(uint8_t buffer_unstuffed[],
                                        const Smpt_ml_get_current_data_ack
                                        *const ml_get_current_data_ack,
                                        uint32_t index)
{

    buffer_unstuffed[index] = ml_get_current_data_ack->result;
    index++;

    buffer_unstuffed[index] = ml_get_current_data_ack->data_selection & 0xff;
    index++;

    if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Stimulation)
    {
        index = insert_stimulation_data(buffer_unstuffed, ml_get_current_data_ack, index);
    }
    else if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Stimulation_Eight_Channels)
    {
        index = insert_stimulation_data_eight_channels(buffer_unstuffed, ml_get_current_data_ack, index);
    }
    else if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Channels)
    {
        index = insert_channels_data(buffer_unstuffed, ml_get_current_data_ack, index);
    }

    return index;
}

uint32_t insert_stimulation_data(uint8_t buffer_unstuffed[], const Smpt_ml_get_current_data_ack
                             *const ml_get_current_data_ack, uint32_t index)
{
    uint8_t i;
    uint8_t electrode_error = 0;

    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (i <= 4)
        {
            electrode_error |=
                    (ml_get_current_data_ack->stimulation_data.electrode_error[i] & 0x01) << i;
        }
    }

    buffer_unstuffed[index] =
                ((ml_get_current_data_ack->stimulation_data.stimulation_state & 0x01) << 4) |
                ((electrode_error & 0x0f));
    index++;

    return index;
}


uint32_t insert_stimulation_data_eight_channels(uint8_t buffer_unstuffed[], const Smpt_ml_get_current_data_ack
                             *const ml_get_current_data_ack, uint32_t index)
{
    uint8_t i;
    uint8_t electrode_error = 0;
    uint8_t electrode_error2 = 0;

    for (i = 0; i < 4; i++)
    {
        electrode_error |=
                (ml_get_current_data_ack->stimulation_data.electrode_error[i] & 0x01) << i;
    }
    if (Smpt_Length_Number_Of_Channels > 4)
    {
        for (i = 4; i < Smpt_Length_Number_Of_Channels; i++)
        {
            electrode_error2 |=
                    (ml_get_current_data_ack->stimulation_data.electrode_error[i] & 0x01) << (i - 4);
        }
    }

    buffer_unstuffed[index] =
                ((ml_get_current_data_ack->stimulation_data.stimulation_state & 0x01) << 4) |
                ((electrode_error & 0x0f));
    index++;
    if (Smpt_Length_Number_Of_Channels > 4)
    {
        buffer_unstuffed[index] =
            ((ml_get_current_data_ack->stimulation_data.stimulation_state & 0x01) << 4) |
            ((electrode_error2 & 0x0f));
        index++;
    }

    return index;
}


uint32_t insert_channels_data(uint8_t buffer_unstuffed[], const Smpt_ml_get_current_data_ack
                             *const ml_get_current_data_ack, uint32_t index)
{
    uint8_t i;
    uint8_t stimulation_state = 0;

    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        stimulation_state |=
                (ml_get_current_data_ack->channel_data.stimulation_state[i] & 0x01) << i;
    }

    buffer_unstuffed[index] = stimulation_state;
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        if (i%2 == 0)
        {
            index++;
            buffer_unstuffed[index] = (ml_get_current_data_ack->channel_data.channel_state[i] & 0x0f);
        }
        else
        {
            buffer_unstuffed[index] |= (ml_get_current_data_ack->channel_data.channel_state[i] & 0x0f) << 4;

        }
    }
    index++;

    return index;
}
