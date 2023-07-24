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
#ifndef SMPT_ML_PACKET_CLIENT_H
#define SMPT_ML_PACKET_CLIENT_H

/***************************************************************************//**
* \file smpt_ml_packet_client.h
*
* This file contains functions for building and extracting mid-level packets
* as client (external master).
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ml_definitions_data_types.h"

/* Build methods */

/***************************************************************************//**
* \brief Builds the Ml_init packet in the buffer using the passed struct.
* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ml_init
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ml_init(uint8_t buffer[], uint32_t buffer_length,
                            const Smpt_ml_init *const ml_init);


/***************************************************************************//**
* \brief Builds the Ml_update packet in the buffer using the passed struct.
* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ml_update
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ml_update(uint8_t buffer[], uint32_t buffer_length,
                              const Smpt_ml_update *const ml_update);


/***************************************************************************//**
* \brief Builds the Ml_get_current_data packet in the buffer using the passed
* struct. The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ml_get_current_data
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ml_get_current_data(uint8_t buffer[], uint32_t buffer_length,
                                        const Smpt_ml_get_current_data *const ml_get_current_data);


/***************************************************************************//**
* \brief Builds the Ml_stop packet in the buffer using the passed
* struct. The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] packet_number
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ml_stop(uint8_t buffer[], uint32_t buffer_length, uint8_t packet_number);


/***************************************************************************//**
* \brief Extracts the Ml_get_current_data_ack packet from buffer to the struct.
* The function expects a stuffed complete packet in the buffer.
*
* \param[out] ml_get_current_data_ack result
* \param[in] buffer Contains the raw packet
* \param[in] packet_length
* returns True if the packet was successfully extracted and the parameters are valid.
*******************************************************************************/
bool smpt_extract_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                          *const ml_get_current_data_ack, const uint8_t buffer[],
                                          uint32_t packet_length);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_ML_PACKET_CLIENT_H */
