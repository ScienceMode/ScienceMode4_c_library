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
#ifndef SMPT_DL_PACKET_CLIENT_H
#define SMPT_DL_PACKET_CLIENT_H

/***************************************************************************//**
* \file smpt_dl_packet_client.h
*
* This file contains functions for building and extracting dyscom-level packets
* as client (external master).
*******************************************************************************/

#include "smpt_dl_definitions.h"
#include "smpt_packet_general.h"

/* Build methods */

/***************************************************************************//**
* \brief Builds the Dl_init packet in the buffer using the passed struct.
* The packet is stuffed and ready to transmit.
* Caution: Be sure to check validity of the struct before calling this function
* (see smpt_is_valid_dl_*()).
*
* \param[out] buffer
* \param[in] buffer_length
* \param[in] dl_init
* \returns The number of bytes written in buffer (packet_length).
*******************************************************************************/
uint32_t smpt_build_dl_init(uint8_t buffer[], uint32_t buffer_length,
                            const Smpt_dl_init *const dl_init);


bool smpt_extract_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack,
                              const uint8_t packet[],
                              uint32_t packet_length);

bool smpt_extract_dl_power_module_ack(Smpt_dl_power_module_ack *const dl_power_module_ack,
                                      const uint8_t packet[],
                                      uint32_t packet_length);

bool smpt_extract_dl_start_ack(Smpt_dl_start_ack *const dl_start_ack,
                               const uint8_t packet[],
                               uint32_t packet_length);

bool smpt_extract_dl_stop_ack(Smpt_dl_stop_ack *const dl_stop_ack,
                              const uint8_t packet[],
                              uint32_t packet_length);

bool smpt_extract_dl_get_ack(Smpt_dl_get_ack *const dl_get_ack,
                              const uint8_t packet[],
                              uint32_t packet_length);

bool smpt_extract_dl_send_file(Smpt_dl_send_file *const dl_send_file,
                              const uint8_t packet[],
                              uint32_t packet_length);

bool smpt_extract_dl_send_live_data(Smpt_dl_send_live_data *const dl_send_live_data,
                              const uint8_t packet[],
                              uint32_t packet_length);

bool smpt_extract_dl_send_mmi(Smpt_dl_send_mmi *const dl_send_mmi,
                              const uint8_t packet[],
                              uint32_t packet_length);
bool smpt_extract_dl_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack,
                             const uint8_t packet[],
                             uint32_t packet_length);

uint32_t smpt_build_dl_start(uint8_t buffer[],
                             uint32_t buffer_length,
                             uint8_t packet_number);

uint32_t smpt_build_dl_stop(uint8_t buffer[],
                            uint32_t buffer_length,
                            uint8_t packet_number);

uint32_t smpt_build_dl_get(uint8_t buffer[],
                            uint32_t buffer_length,
                            const Smpt_dl_get * const dl_get);

uint32_t smpt_build_dl_power_module(uint8_t buffer[], uint32_t buffer_length,
                            const Smpt_dl_power_module *const dl_power_module);

uint32_t smpt_build_dl_send_file_ack(uint8_t buffer[], uint32_t buffer_length,
                                     const Smpt_dl_send_file_ack *const dl_send_file_ack);

uint32_t smpt_build_dl_sys(uint8_t buffer[],
                            uint32_t buffer_length,
                            const Smpt_dl_sys * const dl_sys);


#endif /* SMPT_DL_PACKET_CLIENT_H */
