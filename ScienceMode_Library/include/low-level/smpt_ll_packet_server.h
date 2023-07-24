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
#ifndef SMPT_LL_SERVER_H
#define SMPT_LL_SERVER_H

/***************************************************************************//**
* \file smpt_ll_packet_server.h
*
* This file contains functions for building and extracting packets as server.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ll_definitions.h"
#include "smpt_packet_general.h"
#include "smpt_ll_packet_validity.h"

/***************************************************************************//**
* \brief Extracts the packet to the Ll_init struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] ll_init Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_ll_init(Smpt_ll_init *const ll_init, const uint8_t packet[],
                          uint32_t packet_length);


/***************************************************************************//**
* \brief Extracts the packet to the Ll_channel_config struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] ll_channel_config Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_ll_channel_config(Smpt_ll_channel_config *const ll_channel_config,
                                    const uint8_t packet[], uint32_t packet_length);


/***************************************************************************//**
* \brief Builds the Ll_init_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ll_init_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ll_init_ack(uint8_t buffer[], uint32_t buffer_length,
                                const Smpt_ll_init_ack *const ll_init_ack);


/***************************************************************************//**
* \brief Builds the Ll_channel_config_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ll_channel_config_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ll_channel_config_ack(uint8_t buffer[], uint32_t buffer_length,
                                          const Smpt_ll_channel_config_ack *const ll_channel_config_ack);


/***************************************************************************//**
* \brief Builds the Ll_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ll_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ll_ack(uint8_t buffer[], uint32_t buffer_length,
                           const Smpt_ll_ack *const ll_ack);


/***************************************************************************//**
* \brief Extracts the Ll_point from the packet and writes it in the struct.
*        This function is "public", because ml_packet_server.h needs to extract
*        points, too. You do not want to use this function.
*
* \param[out] ll_point
* \param[in] buffer_unstuffed
* \param[in] index
* \returns last index
*******************************************************************************/
uint32_t smpt_extract_ll_point(Smpt_ll_point *const ll_point,
                               const uint8_t buffer_unstuffed[], uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_SERVER_H */
