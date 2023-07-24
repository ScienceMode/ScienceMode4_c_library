/***************************************************************************//**
* \file smpt_ll_packet_general.c
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

#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "smpt_messages.h"
#include "smpt_packet_validity.h"
#include <string.h>

void smpt_clear_buffer(uint8_t buffer[], uint32_t buffer_length)
{
    /*uint32_t i = 0;

    for (i = 0; i < buffer_length; i++)
    {
        buffer[i] = 0;
    }
    */
    memset(buffer, 0, buffer_length * sizeof(*buffer));
}

bool smpt_find_packet(uint32_t *const packet_start_index, uint32_t *const packet_length,
                      uint32_t search_start_index,
                      const uint8_t buffer[], uint32_t buffer_length)
{
    bool ignore_next_byte = false;
    bool active_start_byte = false;
    bool run_loop = true;
    bool found_packet = false;

    uint32_t i = 0;
    *packet_length = 0;
    while (run_loop && (i < buffer_length)) /* search at maximum once through the buffer */
    {
        uint32_t index = (search_start_index + i) % buffer_length;
        uint8_t byte = buffer[index];

        if (ignore_next_byte)
        {
            ignore_next_byte = false;
            (*packet_length)++;
        }
        else if (byte == Smpt_Packet_Stuffing_Byte)
        {
            ignore_next_byte = true;
            (*packet_length)++;
        }
        else if (active_start_byte &&
                (byte != Smpt_Packet_Start_Byte) &&
                (byte != Smpt_Packet_Stop_Byte))
        {
            (*packet_length)++;
        }
        else if ((!active_start_byte) && (byte == Smpt_Packet_Stop_Byte))
        {
            /* do nothing */
            ;
        }
        else if ((!active_start_byte) && (byte == Smpt_Packet_Start_Byte))
        {
            *packet_start_index = index;
            (*packet_length)++;
            active_start_byte = true;
        }
        else if (active_start_byte && (byte == Smpt_Packet_Stop_Byte))
        {
            (*packet_length)++;
            active_start_byte = false;
            found_packet = true;
            run_loop = false;
        }
        else if (active_start_byte && (byte == Smpt_Packet_Start_Byte))
        {
            smpt_error("smpt_find_packet(): Start byte found while waiting for stop byte.");
            *packet_start_index = index;
            (*packet_length)++;
        }
        else { ; }

        i++;
    }

    if (!found_packet)
    {
        (*packet_length) = 0;
        (*packet_start_index) = 0;
    }

    return found_packet;
}

bool smpt_check_length(const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;

    if ((packet[Smpt_Index_Length_Msb_Stuff] == Smpt_Packet_Stuffing_Byte) &&
        (packet[Smpt_Index_Length_Lsb_Stuff] == Smpt_Packet_Stuffing_Byte))
    {
        uint8_t msb = smpt_unstuff_byte(packet[Smpt_Index_Length_Msb]);
        uint8_t lsb = smpt_unstuff_byte(packet[Smpt_Index_Length_Lsb]);

        uint16_t transfered_packet_length = smpt_convert_to_uint16_t(msb, lsb);

        if (transfered_packet_length == (uint16_t)(packet_length))
        {
            result = true;
        }
        else
        {
            smpt_error("smpt_check_length(): Transfer error (Length) \
                       transfered: %u, packet length: %u", transfered_packet_length, packet_length);
        }
    }

    return result;
}

bool smpt_check_checksum(const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;

    if ((packet[Smpt_Index_Checksum_Msb_Stuff] == Smpt_Packet_Stuffing_Byte) &&
        (packet[Smpt_Index_Checksum_Lsb_Stuff] == Smpt_Packet_Stuffing_Byte))
    {
        uint8_t msb = smpt_unstuff_byte(packet[Smpt_Index_Checksum_Msb]);
        uint8_t lsb = smpt_unstuff_byte(packet[Smpt_Index_Checksum_Lsb]);

        uint16_t transfered_checksum = smpt_convert_to_uint16_t(msb, lsb);
        uint16_t checksum = (uint16_t) (smpt_compute_checksum(packet, 9, packet_length - 1));

        if (transfered_checksum == checksum)
        {
            result = true;
        }
    }

    return result;
}

Smpt_Cmd smpt_get_cmd(const uint8_t packet[], uint32_t packet_length)
{
    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_unstuff_packet(buffer_unstuffed, packet, Smpt_Limit_Header_Size + 1);

    return smpt_get_cmd_packet_unstuffed(buffer_unstuffed, packet_length);
}

uint8_t smpt_get_packet_number(const uint8_t packet[])
{
    uint8_t packet_number;
    if (packet[9] == Smpt_Packet_Stuffing_Byte)
    {
        packet_number = smpt_unstuff_byte(packet[10]) >> 2;
    }
    else
    {
        packet_number = (packet[9] >> 2);
    }
    return packet_number;
}

uint32_t smpt_build_cmd(uint8_t buffer[], uint32_t buffer_length,
                           const Smpt_cmd *const cmd)
{
    uint32_t last_index;
    uint32_t packet_length;
    Smpt_Cmd cmd_number = (Smpt_Cmd) cmd->command_number;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = Smpt_Limit_Header_Size;

    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd_number, cmd->packet_number);
    return packet_length;
}

bool smpt_extract_cmd(Smpt_cmd *const cmd,
                      const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_cmd(cmd);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Ll_Cmd)
    {
        cmd->command_number = smpt_get_cmd_packet_unstuffed(buffer_unstuffed,
                                                               packet_unstuffed_length);
        cmd->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_cmd(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Ll_Cmd);
    }

    result = result && (smpt_is_valid_cmd(cmd));

    return result;
}


uint32_t smpt_build_ack(uint8_t buffer[], uint32_t buffer_length,
                           const Smpt_ack *const ack)
{
    uint32_t index = Smpt_Limit_Header_Size;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    buffer_unstuffed[index] = ack->result;
    index++;
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, index,
                                      (Smpt_Cmd)ack->command_number, ack->packet_number);
    return packet_length;
}


bool smpt_extract_ack(Smpt_ack *const ack, const uint8_t buffer[],
                      uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    /* Only extract the needed packet size, not the whole packet */
    /* maximum length of result byte ->  15 = 9 Byte (start byte + packet length + checksum)   +
                                              4 Byte (2 Byte packet number + cmd + 2 stuffing) +
                                              2 Byte (1 Byte result + 1 possible stuffing byte) */
    uint32_t max_length = Smpt_Length_Header_Size + 4; /* 15 */
    uint32_t length = (packet_length < max_length) ? packet_length : max_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, buffer, length);

    if (packet_unstuffed_length >= Smpt_Length_Min_Packet_Size)
    {
        ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
        ack->command_number = smpt_get_cmd_packet_unstuffed(buffer_unstuffed, packet_length);
        if (packet_unstuffed_length >= (Smpt_Length_Min_Packet_Size + 1))
        {
            /* KU: Maybe result in wrong <<result>> value, if the packet has no result */
            ack->result = smpt_get_result_unstuffed(buffer_unstuffed);
        }
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Limit_Min_Packet_Size + 1);
    }

    if (ack->command_number == Smpt_Cmd_Sl_Debug_Message) /* no result in cmds */
    {
        ack->result = Smpt_Result_Successful;
    }

    result = result && (smpt_is_valid_ack(ack));

    return result;
}

void smpt_clear_cmd(Smpt_cmd *const cmd)
{
    cmd->command_number = 0;
    cmd->packet_number = 0;
}

void smpt_clear_char_array(char char_array[], uint32_t array_length)
{
    memset(char_array, 0, array_length * sizeof(*char_array));
}
