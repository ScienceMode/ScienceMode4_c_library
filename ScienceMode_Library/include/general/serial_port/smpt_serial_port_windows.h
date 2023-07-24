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
#ifndef SMPT_LL_SERIAL_PORT_WINDOWS_H
#define SMPT_LL_SERIAL_PORT_WINDOWS_H

/***************************************************************************//**
* \file smpt_ll_serial_port_windows.h
*
* This file provides the implementation for serial port communication in 32-Bit
* Windows operation systems.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

#include "windows.h"
#include "smpt_definitions.h"

/**
 * For windows serial communication see
 * https://msdn.microsoft.com/en-us/library/windows/desktop/aa363214%28v=vs.85%29.aspx
 * and maybe
 * http:**www.codeproject.com/Articles/3061/Creating-a-Serial-communication-on-Win32
 */

/***************************************************************************//**
* \brief Opens the serial port in windows based operation systems
*
* \param[out] serial_port_handle
* \param[in] port_name
* \param[in] baud_rate
* \param[in] debug_error if true, an error is written to stderr if the port could not be opened
* \returns True if the port has been opened
*******************************************************************************/
bool smpt_open_serial_port_win(HANDLE *serial_port_handle, const char port_name[],
                               uint32_t baud_rate, bool debug_error);

/***************************************************************************//**
* \brief Writes data to the serial port in windows based operation systems
*
* \param[in] serial_port_handle
* \param[in] output_data
* \param[in] output_data_length
* \returns True if all data has been written
*******************************************************************************/
bool smpt_write_to_serial_port_win(HANDLE serial_port_handle, const uint8_t output_data[],
                                   uint32_t output_data_length);

/***************************************************************************//**
* \brief Reads data from the serial port in windows based operation systems
*
* \param[out] bytes_read bytes that have been read
* \param[in] serial_port_handle
* \param[in] input_data
* \param[in] input_data_length
* \returns True if at least one byte was read
*******************************************************************************/
bool smpt_read_from_serial_port_win(uint32_t *bytes_read, HANDLE serial_port_handle,
                                    uint8_t input_data[], uint32_t input_data_length);

/***************************************************************************//**
* \brief Reads data from the serial port in windows based operation systems
*
* \param[in] serial_port_handle
* \returns True if the serial was closed
*******************************************************************************/
bool smpt_close_serial_port_win(HANDLE serial_port_handle);

#endif /* _WIN32 */

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_SERIAL_PORT_WINDOWS_H */
