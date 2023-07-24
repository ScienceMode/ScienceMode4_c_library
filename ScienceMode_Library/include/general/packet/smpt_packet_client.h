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
#ifndef SMPT_PACKET_CLIENT_H
#define SMPT_PACKET_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"
#include "smpt_definitions_file_transfer.h"
#include "smpt_definitions_power.h"

uint32_t smpt_build_get_file(uint8_t buffer[], uint32_t buffer_length, Smpt_Cmd cmd,
                             const Smpt_get_file * const get_file);

uint32_t smpt_build_remove_file(uint8_t buffer[], uint32_t buffer_length, Smpt_Cmd cmd,
                                const Smpt_remove_file * const remove_file);

uint32_t smpt_build_set_power(uint8_t buffer[], uint32_t buffer_length,
                                const Smpt_set_power * const set_power);

bool smpt_extract_file_block(Smpt_file_block *const file_block,
                             const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_file_ack(Smpt_get_file_ack *const get_file_ack,
                               const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_version_ack(Smpt_get_version_ack *const get_version_ack,
                                  const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_extended_version_ack(Smpt_get_extended_version_ack *const get_extended_version_ack,
                                  const uint8_t packet[], uint32_t packet_length);


bool smpt_extract_get_version_main_ack(Smpt_get_version_ack *const get_version_ack,
                                       const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_device_id_ack(Smpt_get_device_id_ack *const get_device_id_ack,
                                    const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack,
                                         const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_stim_status_ack(Smpt_get_stim_status_ack *const get_stim_status_ack,
                                         const uint8_t packet[], uint32_t packet_length);

bool smpt_extract_get_main_status_ack(Smpt_get_main_status_ack *const get_main_status_ack,
                                         const uint8_t packet[], uint32_t packet_length);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_CLIENT_H */

