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
#ifndef SMPT_DL_SERVER_H
#define SMPT_DL_SERVER_H

/***************************************************************************//**
* \file smpt_dl_packet_server.h
*
* This file contains functions for building and extracting packets as server.
*******************************************************************************/

#include "smpt_dl_definitions.h"
#include "smpt_packet_general.h"
#include "smpt_dl_packet_validity.h"

/***************************************************************************//**
* \brief Extracts the packet to the dl_init struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] dl_init Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_dl_init(Smpt_dl_init *dl_init, const uint8_t packet[],
                          uint32_t packet_length);

/**
 * dl_start and dl_stop do not have any payload. If the server needs the
 * packet number it may call smpt_get_packet_number(packet)
 */

/***************************************************************************//**
* \brief Extracts the packet to the dl_get struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] dl_get Contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet Contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_dl_get(Smpt_dl_get *const dl_get, const uint8_t packet[],
                          uint32_t packet_length);

/***************************************************************************//**
* \brief Builds the dl_init_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_init_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_init_ack(uint8_t buffer[], uint32_t buffer_length,
                                const Smpt_dl_init_ack *const dl_init_ack);

/***************************************************************************//**
* \brief Builds the dl_start_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_start_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_start_ack(uint8_t buffer[], uint32_t buffer_length,
                                 const Smpt_dl_start_ack *const dl_start_ack);

/***************************************************************************//**
* \brief Builds the dl_stop_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_stop_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_stop_ack(uint8_t buffer[], uint32_t buffer_length,
                                 const Smpt_dl_stop_ack *const dl_stop_ack);

/***************************************************************************//**
* \brief Builds the dl_get_ack packet in the buffer using the passed struct.
*
* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* Only the data fields of the corresponding dl_get_ack->get_type
* will contain values different from 0. For instance when you have
*
* dl_get_ack->get_type == Smpt_Dl_Get_Type_Battery_Status
*
* dl_get_ack->battery_status will be filled with data but
* dl_get_ack->deviceId will be 0
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_get_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_get_ack(uint8_t buffer[], uint32_t buffer_length,
                               const Smpt_dl_get_ack *const dl_get_ack);

/***************************************************************************//**
* \brief Builds the dl_send_file packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer (must have Smpt_Limit_Max_Packet_Size size!)
* \param[in] buffer_length
* \param[in] dl_send_file
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_send_file(uint8_t buffer[], uint32_t buffer_length,
                               const Smpt_dl_send_file *const dl_send_file);

/***************************************************************************//**
* \brief Builds the dl_send_live_data packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
*
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_send_live_data
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_send_live_data(uint8_t buffer[], uint32_t buffer_length,
                                      const Smpt_dl_send_live_data *const dl_send_live_data);

/***************************************************************************//**
* \brief Builds the smpt_build_dl_send_mmi packet in the buffer using
* the passed struct.
*
* The packet is stuffed and ready to transmit.
*
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] Smpt_build_dl_send_mmi
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_send_mmi(uint8_t buffer[], uint32_t buffer_length,
                                      const Smpt_dl_send_mmi *const dl_send_mmi);

/***************************************************************************//**
* \brief Builds the dl_power_module_ack packet in the buffer using the passed struct.

* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_power_module_ack
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_power_module_ack(uint8_t buffer[], uint32_t buffer_length,
                                const Smpt_dl_power_module_ack *const dl_power_module_ack);

/***************************************************************************//**
* \brief Builds the smpt_build_dl_sys_ack packet in the buffer using
* the passed struct.
*
* The packet is stuffed and ready to transmit.
*
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] Smpt_build_dl_send_mmi
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_sys_ack(uint8_t buffer[], uint32_t buffer_length,
                               const Smpt_dl_sys_ack *const dl_sys_ack);

/***************************************************************************//**
* \brief Extracts the packet to the dl_power_module struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] dl_power_module contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_dl_power_module(Smpt_dl_power_module *const dl_power_module,
                                  const uint8_t packet[],
                                  uint32_t packet_length);

/***************************************************************************//**
* \brief Extracts the packet to the dl_send_file_ack struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] dl_send_file_ack
* \param[in] packet contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack,
                                  const uint8_t packet[],
                                  uint32_t packet_length);

/***************************************************************************//**
* \brief Extracts the packet to the dl_sys struct.
*
* The checksum or length is not checked. This has to be done before calling
* this function.
*
* \param[out] dl_power_module contains the extracted parameter from the packet,
*             if function returns true
* \param[in] packet contains the packet, [0] = start byte
* \param[in] packet_length
* \returns True if the extraction was successful \n
*          False, if there was a parameter error (to few parameter, wrong range)
*******************************************************************************/
bool smpt_extract_dl_sys(Smpt_dl_sys *const dl_sys,
                                  const uint8_t packet[],
                                  uint32_t packet_length);


#endif /* SMPT_DL_SERVER_H */
