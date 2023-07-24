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
#ifndef SMPT_LL_PACKET_CLIENT_H
#define SMPT_LL_PACKET_CLIENT_H

/***************************************************************************//**
* \file smpt_ll_packet_client.h
*
* This file contains functions for building and extracting low-level packets
* as client (external master).
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ll_definitions.h"
#include "smpt_packet_general.h"

/* Build methods */

/***************************************************************************//**
* \brief Builds the Ll_init packet in the buffer using the passed struct.
* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ll_init
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ll_init(uint8_t buffer[], uint32_t buffer_length,
                            const Smpt_ll_init *const ll_init);


/***************************************************************************//**
* \brief Builds the Ll_channel_config packet in the buffer using the passed struct.
* The command is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ll_channel_config
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ll_channel_config(uint8_t buffer[], uint32_t buffer_length,
                                      const Smpt_ll_channel_config *const ll_channel_config);


/* Extract methods */

/***************************************************************************//**
* \brief Extracts the Ll_init_ack packet from buffer to the struct.
* The function expects a stuffed complete packet in the buffer.
*
* \param[out] ll_init_ack result
* \param[in] buffer Contains the raw packet
* \param[in] packet_length
* returns True if the packet was successfully extracted and the parameters are valid.
*******************************************************************************/
bool smpt_extract_ll_init_ack(Smpt_ll_init_ack *const ll_init_ack, const uint8_t buffer[],
                              uint32_t packet_length);


/***************************************************************************//**
* \brief Extracts the Ll_channel_config_ack packet from buffer to the struct.
* The function expects a stuffed complete packet in the buffer.
*
* \param[out] ll_channel_config_ack
* \param[in] buffer Contains the raw packet
* \param[in] packet_length
* returns True if the packet was successfully extracted and the parameters are valid.
*******************************************************************************/
bool smpt_extract_ll_channel_config_ack(Smpt_ll_channel_config_ack *const ll_channel_config_ack,
                                        const uint8_t buffer[], uint32_t packet_length);


/***************************************************************************//**
* \brief Inserts a Ll_point into the packet starting from index. This function
* is "public", because it is also used by ml_packet_client.h. You do want to
* use this function.
*
* \param[out] buffer_unstuffed
* \param[in] ll_point
* \param[in] index
* returns index of the last insertion + 1
*******************************************************************************/
uint32_t smpt_insert_ll_point(uint8_t buffer_unstuffed[], const Smpt_ll_point *const ll_point,
                              uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_PACKET_CLIENT_H */
