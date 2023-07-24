/***************************************************************************//**
* \file smpt_ll_packet_internal.c
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

#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "smpt_packet_general.h"
#include "smpt_messages.h"

/***************************************************************************//**
* \brief Inserts the cmd number and packet number into the packet
*
* \param[in,out] buffer
* \param[in] cmd
* \param[in] packet_number
*******************************************************************************/
static void smpt_insert_cmd_number(uint8_t buffer[], Smpt_Cmd cmd, uint8_t packet_number);

/***************************************************************************//**
* \brief Inserts the header (start byte, checksum, packet length) into the packet
*
* \param[in,out] buffer
* \param[in] packet_length
*******************************************************************************/
static void smpt_insert_header(uint8_t buffer[], uint32_t packet_length);

/***************************************************************************//**
* \brief Inserts the stop byte into the packet
*
* \param[in,out] buffer
* \param[in] index current index, where the stop byte should be inserted
* \returns Packet length after the stop byte was inserted
*******************************************************************************/
static uint32_t smpt_insert_stop_byte(uint8_t buffer[], uint32_t index);

/***************************************************************************//**
* \brief Stuffs the unstuffed packet
*
* \param[out] packet_stuffed
* \param[in] packet_unstuffed
* \param[in] packet_unstuffed_length
* \returns Packet length of the stuffed packet
*******************************************************************************/
static uint32_t smpt_stuff_packet(uint8_t packet_stuffed[], const uint8_t packet_unstuffed[],
                                  uint32_t packet_unstuffed_length);

/***************************************************************************//**
* \brief Checks if a byte is a special (start, stop or stuffing) byte

* \param[in] byte
* \returns True if the byte is a start, stop, or stuffing byte
*******************************************************************************/
static bool smpt_is_special_byte(uint8_t byte);

/***************************************************************************//**
* \brief Stuffs a byte
*
* \param[in] unstuffed byte
* \returns Stuffed byte
*******************************************************************************/
static uint8_t smpt_stuff_byte(uint8_t byte);

static const uint32_t smpt_crc_ccitt_16_table_[256] =
{
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};


void smpt_insert_cmd_number(uint8_t buffer[], Smpt_Cmd cmd, uint8_t packet_number)
{
    uint32_t index = Smpt_Limit_Header_Size - 2;
    uint8_t cmd_number_2_bit_msb;

    /*if ((uint16_t)cmd > 1024)
    {
       smpt_error("smpt_insert_cmd_number(): Command number to large (%d).", cmd);
        cmd = Smpt_Cmd_Ll_Init;
    }*/

    cmd_number_2_bit_msb = (uint8_t) ((((uint32_t) cmd) >> 8) & 0x03);
    buffer[index] = ((packet_number << 2) & 0xfc) | cmd_number_2_bit_msb;
    index++;

    buffer[index] = (uint8_t) cmd;
}

uint32_t smpt_build_packet(uint8_t buffer[], uint32_t buffer_length,
                           uint8_t buffer_unstuffed[],
                           uint32_t index, Smpt_Cmd cmd, uint8_t packet_number)
{
    /* This function has more than one exit points. */

    uint32_t packet_length;

    if (buffer_length < Smpt_Limit_Max_Packet_Size_No_Emg)
    {
        smpt_error("smpt_build_packet(): Buffer length to short (%d, expected: %d).",
                   buffer_length, Smpt_Limit_Max_Packet_Size_No_Emg);
        return 0;
    }

    smpt_insert_cmd_number(buffer_unstuffed, cmd, packet_number);
    index = smpt_stuff_packet(buffer, buffer_unstuffed, index);
    index = smpt_insert_stop_byte(buffer, index);

    packet_length = index;
    smpt_insert_header(buffer, packet_length);

    return packet_length;
}

uint32_t smpt_stuff_packet(uint8_t packet_stuffed[], const uint8_t packet_unstuffed[],
                           uint32_t packet_unstuffed_length)
{
    uint32_t stuffed_i = 0;
    uint32_t i;

    for (i = 0; i < packet_unstuffed_length; i++)
    {        
        uint8_t byte = packet_unstuffed[i];
        if (smpt_is_special_byte(byte))
        {
            packet_stuffed[stuffed_i] = Smpt_Packet_Stuffing_Byte;
            stuffed_i++;
            packet_stuffed[stuffed_i] = smpt_stuff_byte(byte);
            stuffed_i++;
        }
        else
        {
            packet_stuffed[stuffed_i] = byte;
            stuffed_i++;
        }
    }

    return stuffed_i; /* packet_stuffed_length */
}

uint32_t smpt_unstuff_packet(uint8_t packet_unstuffed[], const uint8_t packet_stuffed[],
                        uint32_t packet_stuffed_length)
{
    uint32_t i;
    uint32_t unstuffed_i = 0;
    bool last_byte_is_stuffing_byte = false;

    for (i = 0; i < (Smpt_Limit_Header_Size - 2); i++)
    {
        packet_unstuffed[unstuffed_i] = packet_stuffed[i];
        unstuffed_i++;
    }

    for (i = (Smpt_Limit_Header_Size - 2); i < packet_stuffed_length; i++)
    {
        uint8_t byte = packet_stuffed[i];

        if (last_byte_is_stuffing_byte)
        {
            packet_unstuffed[unstuffed_i] = smpt_unstuff_byte(byte);
            unstuffed_i++;
            last_byte_is_stuffing_byte = false;
        }
        else if (byte == Smpt_Packet_Stuffing_Byte)
        {
            last_byte_is_stuffing_byte = true;
        }
        else
        {
            packet_unstuffed[unstuffed_i] = byte;
            unstuffed_i++;
        }
    }

    return unstuffed_i; /* packet_unstuffed_length */
}

uint16_t smpt_convert_to_uint16_t(uint8_t msb, uint8_t lsb)
{
    return (((uint16_t) msb) << 8) | ((uint16_t) lsb);
}

bool smpt_is_special_byte(uint8_t byte)
{
    bool result = false;
    if ((byte == Smpt_Packet_Start_Byte) ||
        (byte == Smpt_Packet_Stuffing_Byte) ||
        (byte == Smpt_Packet_Stop_Byte))
    {
        result = true;
    }
    return result;
}

uint8_t smpt_stuff_byte(uint8_t byte)
{
    return byte ^ Smpt_Packet_Stuffing_Key;
}

uint8_t smpt_unstuff_byte(uint8_t byte)
{
    return byte ^ Smpt_Packet_Stuffing_Key;
}

uint32_t smpt_insert_stop_byte(uint8_t buffer[], uint32_t index)
{
    buffer[index] = Smpt_Packet_Stop_Byte;
    index++;
    return index;
}

void smpt_insert_header(uint8_t buffer[], uint32_t packet_length)
{
    uint32_t checksum;

    uint8_t packet_length_msb;
    uint8_t packet_length_lsb;
    uint8_t checksum_msb;
    uint8_t checksum_lsb;

    /* Subtract stopp byte (1 byte) for checksum calculation */
    checksum = smpt_compute_checksum(buffer, Smpt_Checksum_Start_Index, packet_length - 1);

    buffer[Smpt_Index_Start_Byte] = Smpt_Packet_Start_Byte;

    packet_length_msb = (uint8_t) (packet_length >> 8);
    packet_length_lsb = (uint8_t) packet_length;

    buffer[Smpt_Index_Length_Msb_Stuff] = Smpt_Packet_Stuffing_Byte;
    buffer[Smpt_Index_Length_Msb] = smpt_stuff_byte(packet_length_msb);
    buffer[Smpt_Index_Length_Lsb_Stuff] = Smpt_Packet_Stuffing_Byte;
    buffer[Smpt_Index_Length_Lsb] = smpt_stuff_byte(packet_length_lsb);

    checksum_msb = (uint8_t) (checksum >> 8);
    checksum_lsb = (uint8_t) checksum;

    buffer[Smpt_Index_Checksum_Msb_Stuff] = Smpt_Packet_Stuffing_Byte;
    buffer[Smpt_Index_Checksum_Msb] = smpt_stuff_byte(checksum_msb);
    buffer[Smpt_Index_Checksum_Lsb_Stuff] = Smpt_Packet_Stuffing_Byte;
    buffer[Smpt_Index_Checksum_Lsb] = smpt_stuff_byte(checksum_lsb);
}

/*  Polynomial 0x1021,
    CRC CCITT 16 Bit
    Online-check: http:**depa.usst.edu.cn/chenjq/www2/SDesign/JavaScript/CRCcalculation.htm */
uint32_t smpt_compute_checksum(const uint8_t buffer[], uint32_t start_index, uint32_t length)
{
    /* seed */
    uint32_t crc = 0x0000;
    uint32_t i;

    for (i = start_index; i < length; i++)
    {
        uint32_t temp = (buffer[i] ^ (crc >> 8)) & 0xff;
        crc = smpt_crc_ccitt_16_table_[temp] ^ (crc << 8);
    }

    return crc;
}

uint8_t smpt_to_bytes(uint8_t number_of_switches)
{
    uint8_t result;

    if (number_of_switches == 0)
    {
        result = 0;
    }
    else
    {
        result = 1 + ((number_of_switches - 1) / 8);
    }

    return result;
}

Smpt_Cmd smpt_get_cmd_packet_unstuffed(const uint8_t packet[], uint32_t packet_length)
{
    uint16_t msb;
    uint16_t lsb;
    uint16_t cmd_number;
    Smpt_Cmd cmd;

    if (packet_length < Smpt_Limit_Min_Packet_Size)
    {
        smpt_error("smpt_get_cmd_packet_unstuffed(): Packet length too short (%d, expected: %d)",
                   packet_length, Smpt_Limit_Min_Packet_Size);
    }

    msb = (packet[9] & 0x03);
    lsb = packet[10];

    cmd_number = (msb << 8) | lsb;

    if (cmd_number <= Smpt_Cmd_Last_Item)
    {
        cmd = (Smpt_Cmd)(cmd_number);
    }
    else
    {
        cmd = Smpt_Cmd_Unknown_Cmd;
    }

    return cmd;
}

uint8_t smpt_get_packet_number_unstuffed(const uint8_t packet[])
{
    return (packet[9] >> 2);
}

Smpt_Result smpt_get_result_unstuffed(const uint8_t packet[])
{
    return (Smpt_Result) packet[Smpt_Length_Header_Size];
}
