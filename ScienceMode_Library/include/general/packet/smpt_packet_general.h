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
#ifndef SMPT_PACKET_GENERAL_H
#define SMPT_PACKET_GENERAL_H

/***************************************************************************//**
* \file smpt_packet_general.h
* \author Bjoern Kuberski
* \copyright Hasomed GmbH 2013-2016
*
* This file contains functions for working with commands as client or server.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

/***************************************************************************//**
* \brief Set all buffer values to zero.

* \param[in] buffer
* \param[in] buffer_length
*******************************************************************************/
void smpt_clear_buffer(uint8_t buffer[], uint32_t buffer_length);

/***************************************************************************//**
* \brief Set all buffer values to zero.

* \param[in] char_array
* \param[in] array_length
*******************************************************************************/
void smpt_clear_char_array(char char_array[], uint32_t array_length);


/***************************************************************************//**
* \brief Finds the packet in a buffer
*
* \param[out] packet_start_index Start index of the found packet (only if function returns true)
* \param[out] packet_length Packet length (only if function returns true)
* \param[in] search_start_index
* \param[in] buffer Buffer, which contains byte stream
* \param[in] buffer_length
* \returns True if a packet was found (also sets start_pos and packet_length)
*******************************************************************************/
bool smpt_find_packet(uint32_t *const packet_start_index, uint32_t *const packet_length,
                      uint32_t search_start_index,
                      const uint8_t buffer[], uint32_t buffer_length);


/***************************************************************************//**
* \brief Checks the length of a packet.
*
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the packet length inside the packet matches the length of packet
*******************************************************************************/
bool smpt_check_length(const uint8_t packet[], uint32_t packet_length);


/***************************************************************************//**
* \brief Checks the checksum of a packet
* Calculates the checksum of the packet and compares it to the transfered
* checksum in the packet.

* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if both checksums (transfered, calculated) are equal
*******************************************************************************/
bool smpt_check_checksum(const uint8_t packet[], uint32_t packet_length);


/***************************************************************************//**
* \brief Extracts the command from a packet.
*
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns Cmd enum from the packet
*******************************************************************************/
Smpt_Cmd smpt_get_cmd(const uint8_t packet[], uint32_t packet_length);


/***************************************************************************//**
* \brief Extracts the packet number from a packet.
*
* \param[in] packet Contains the packet, [0] = start byte
* \returns Packet number from the packet
*******************************************************************************/
uint8_t smpt_get_packet_number(const uint8_t packet[]);


uint32_t smpt_build_cmd(uint8_t buffer[], uint32_t buffer_length,
                        const Smpt_cmd *const cmd);


/***************************************************************************//**
* \brief Extracts the packet to the Ll_cmd struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] ll_cmd Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_cmd(Smpt_cmd *const cmd,
                         const uint8_t packet[], uint32_t packet_length);

/***************************************************************************//**
* \brief Builds a general ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ack(uint8_t buffer[], uint32_t buffer_length,
                        const Smpt_ack *const ack);

/***************************************************************************//**
* \brief Extracts a general ack packet from buffer to the struct.
* The function expects a stuffed complete packet in the buffer.
*
* \param[out] ack
* \param[in] buffer Contains the raw packet
* \param[in] packet_length
* returns True if the packet was successfully extracted and the parameters are valid.
*******************************************************************************/
bool smpt_extract_ack(Smpt_ack *const ack,
                      const uint8_t buffer[], uint32_t packet_length);

/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in] cmd
*******************************************************************************/
void smpt_clear_cmd(Smpt_cmd *const cmd);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_GENERAL_H */
