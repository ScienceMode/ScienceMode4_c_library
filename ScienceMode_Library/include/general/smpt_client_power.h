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
#ifndef SMPT_CLIENT_POWER_H
#define SMPT_CLIENT_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

#include "smpt_definitions.h"
#include "smpt_definitions_power.h"

/***************************************************************************//**
* \brief Builds the set_power packet and sends it to the serial port.
* \param[in,out] device
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_set_power(Smpt_device *const device, const Smpt_set_power *const set_power);

#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */

#ifdef __cplusplus
}
#endif

#endif /* SMPT_CLIENT_POWER_H*/

