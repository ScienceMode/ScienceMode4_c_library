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
#ifndef SMPT_PACKET_VALIDITY_H
#define SMPT_PACKET_VALIDITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"
#include "smpt_definitions_file_transfer.h"
#include "smpt_definitions_power.h"
#include "smpt_packet_general.h"

bool smpt_is_valid_packet_number (const uint8_t packet_number, const char* smpt_level);

/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] cmd
* \returns True if all parameters are valid.
*******************************************************************************/
bool smpt_is_valid_cmd(const Smpt_cmd *const cmd);

/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ll_ack
* \returns True if all parameters are valid.
*******************************************************************************/
bool smpt_is_valid_ack(const Smpt_ack *const ack);

bool smpt_is_valid_file_block(const Smpt_file_block *const file_block);
bool smpt_is_valid_get_file(const Smpt_get_file *const get_file);
bool smpt_is_valid_get_file_ack(const Smpt_get_file_ack *const get_file_ack);
bool smpt_is_valid_remove_file(const Smpt_remove_file *const remove_file);

SMPT_API bool smpt_is_valid_get_device_id_ack(const Smpt_get_device_id_ack *const get_device_id_ack);
SMPT_API bool smpt_is_valid_get_battery_status_ack(const Smpt_get_battery_status_ack *const get_device_id_ack);
SMPT_API bool smpt_is_valid_get_version_ack(const Smpt_get_version_ack *const get_version_ack);
SMPT_API bool smpt_is_valid_get_extended_version_ack(const Smpt_get_extended_version_ack * const get_extended_version_ack);
SMPT_API bool smpt_is_valid_set_power(const Smpt_set_power *const set_power);
SMPT_API bool smpt_is_valid_get_stim_status_ack(const Smpt_get_stim_status_ack
                                                *const get_stim_status_ack);
SMPT_API bool smpt_is_valid_get_main_status_ack(const Smpt_get_main_status_ack
                                                *const get_main_status_ack);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_VALIDITY_H */
