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
#ifndef SMPT_CLIENT_UTILS_H
#define SMPT_CLIENT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

bool smpt_send_cmd(Smpt_device *const device, uint8_t packet_number, Smpt_Cmd cmd_type);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SMPT_CLIENT_UTILS_H */
