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
#ifndef SMPT_ML_PACKET_SERVER_H
#define SMPT_ML_PACKET_SERVER_H

/***************************************************************************//**
* \file smpt_ml_packet_server.h
*
* This file contains functions for building and extracting mid-level packets
* as server (stimulator).
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ml_definitions.h"

/***************************************************************************//**
* \brief Extracts the packet to the Ml_init struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] ml_init Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_ml_init(Smpt_ml_init *const ml_init, const uint8_t packet[],
                          uint32_t packet_length);


/***************************************************************************//**
* \brief Extracts the packet to the Ml_update struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] ml_update Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_ml_update(Smpt_ml_update *const ml_update, const uint8_t packet[],
                            uint32_t packet_length);


/***************************************************************************//**
* \brief Extracts the packet to the Ml_get_current_data struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] ml_get_current_data Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
* *******************************************************************************/
bool smpt_extract_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data,
                                      const uint8_t packet[], uint32_t packet_length);


/***************************************************************************//**
* \brief Builds the Ml_get_current_data_ack packet in the buffer using the passed
* struct. The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_ll_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] ml_get_current_data_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_ml_get_current_data_ack(uint8_t buffer[], uint32_t buffer_length,
                                            const Smpt_ml_get_current_data_ack
                                            *const ml_get_current_data_ack);
#ifdef __cplusplus
}
#endif

#endif /* SMPT_ML_PACKET_SERVER_H */
