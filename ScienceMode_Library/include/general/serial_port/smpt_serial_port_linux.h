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
#ifndef SMPT_LL_SERIAL_PORT_LINUX_H
#define SMPT_LL_SERIAL_PORT_LINUX_H

/***************************************************************************//**
* \file smpt_ll_serial_port_linux.h
*
* This file provides the implementation for serial port communication in Linux
* based operation systems.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include "smpt_ll_definitions.h"

bool smpt_open_serial_port_linux(int *file_descriptor, const char port_name[],
                                 unsigned int baud_rate, bool debug_error);


bool smpt_write_to_serial_port_linux(int *const fd, const uint8_t output_data[],
                                     uint32_t output_data_length);


bool smpt_read_from_serial_port_linux(uint32_t *bytes_read, int fd,
                                      uint8_t input_data[], uint32_t input_data_length);


bool smpt_close_serial_port_linux(int fd);

#endif /* __linux__ */

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_SERIAL_PORT_LINUX_H */
