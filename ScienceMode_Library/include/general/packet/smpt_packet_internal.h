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
#ifndef SMPT_LL_PACKET_INTERNAL_H
#define SMPT_LL_PACKET_INTERNAL_H

/***************************************************************************//**
* \file smpt_ll_packet_internal.h
* \author Bjoern Kuberski
* \copyright Hasomed GmbH 2013-2016
*
* This file contains the basic functions for building the packets. It includes
* inserting header, footer, stuffing, unstuffing and checksum, length calculations.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

/***************************************************************************//**
* \brief Unstuffs the stuffed packet
*
* \param[out] packet_unstuffed
* \param[in] packet_stuffed
* \param[in] packet_stuffed_length
* \returns Packet length of the unstuffed packet
*******************************************************************************/
uint32_t smpt_unstuff_packet(uint8_t packet_unstuffed[], const uint8_t packet_stuffed[],
                        uint32_t packet_stuffed_length);

/***************************************************************************//**
* \brief Unstuffs a byte
*
* \param[in] stuffed byte
* \returns Unstuffed byte
*******************************************************************************/
uint8_t smpt_unstuff_byte(uint8_t byte);

/***************************************************************************//**
* \brief Converts two bytes to a 2-byte word
*
* \param[in] msb most significant byte
* \param[in] lsb least significant byte
* \returns 2 byte word (msb | lsb)
*******************************************************************************/
uint16_t smpt_convert_to_uint16_t(uint8_t msb, uint8_t lsb);

/***************************************************************************//**
* \brief Computes the checksum of a buffer
*
* \param[in] buffer
* \param[in] start_index start index of the buffer
* \param[in] length length of the packet (not the length of start index to end)
* \returns Checksum
*******************************************************************************/
uint32_t smpt_compute_checksum(const uint8_t buffer[], uint32_t start_index, uint32_t length);

/***************************************************************************//**
* \brief Builds the packet
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] buffer_unstuffed should contain the unstuffed data without header/footer
* \returns packet length
*******************************************************************************/
uint32_t smpt_build_packet(uint8_t buffer[], uint32_t buffer_length, uint8_t buffer_unstuffed[],
                           uint32_t index, Smpt_Cmd cmd, uint8_t packet_number);

/***************************************************************************//**
* \brief Converts the number of switches to a multiple of 8
*
* \param[int] number of switches
* \returns Number of switches as a multiple of 8
*******************************************************************************/
uint8_t smpt_to_bytes(uint8_t number_of_switches);

/***************************************************************************//**
* \brief Gets the cmd of an unstuffed packet
*
* \param[in] packet
* \param[in] packet_length
* \returns Command number of the packet
*******************************************************************************/
Smpt_Cmd smpt_get_cmd_packet_unstuffed(const uint8_t packet[], uint32_t packet_length);

/***************************************************************************//**
* \brief Gets the packet_number of an unstuffed packet
*
* \param[in] packet
* \returns Packet number of the packet
*******************************************************************************/
uint8_t smpt_get_packet_number_unstuffed(const uint8_t packet[]);

/***************************************************************************//**
* \brief Gets the result of an unstuffed packet
*
* \param[in] packet
* \returns Smpt_Result of the packet
*******************************************************************************/
Smpt_Result smpt_get_result_unstuffed(const uint8_t packet[]);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_PACKET_INTERNAL_H */
