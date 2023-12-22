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
#include "smpt_definitions.h"
#include "smpt_packet_input_buffer.h"
#include "smpt_client_cmd_lists.h"

/*******************************************************************************
 Section Windows only
*******************************************************************************/
#ifdef _WIN32

#include "smpt_serial_port_windows.h"
#include "smpt_client_data.h"
#include "smpt_messages.h"

bool smpt_open_serial_port_internal(Smpt_device *const device)
{
    char device_name_win[259] = {0};

    if (device->serial_port_name[0] != '\\')
    {
        strcpy(device_name_win, "\\\\.\\");

    }
    strcat(device_name_win, device->serial_port_name);
    strcpy(device->serial_port_name, device_name_win);

    return smpt_open_serial_port_win(&device->serial_port_handle_,
                                     device_name_win, 3000000, true);
}

bool smpt_close_serial_port_internal(Smpt_device *const device)
{
    return smpt_close_serial_port_win(device->serial_port_handle_);
}

bool smpt_write_to_serial_port_internal(Smpt_device *const device,
                                        const uint8_t buffer[], uint32_t packet_length)
{
    return smpt_write_to_serial_port_win(device->serial_port_handle_,
                                         buffer, packet_length);
}

bool smpt_read_from_serial_port_internal(Smpt_device *const device,
                                         uint32_t *bytes_read,
                                         uint8_t buffer[], uint32_t buffer_length)
{
    return smpt_read_from_serial_port_win(bytes_read, device->serial_port_handle_, buffer,
                                          buffer_length);
}

bool smpt_check_serial_port_internal(const char *const device_name)
{
    bool result;
    Smpt_device device = {0};

    uint32_t length = strlen(device_name);
    if (length > Smpt_Length_Serial_Port_Chars)
    {
        smpt_error("smpt_check_serial_port_internal: Device name too long.");
        return false;
    }

    memcpy(device.serial_port_name, device_name, length + 1);

    result = smpt_open_serial_port_internal(&device);

    if (result)
    {
        smpt_close_serial_port_internal(&device);
    }

    return result;
}

#endif /* _WIN32 */

/*******************************************************************************
 Section Linux only
*******************************************************************************/
#ifdef __linux__

#ifdef ANDROID
#else

#include "smpt_serial_port_linux.h"
#include "smpt_packet_input_buffer.h"
#include "smpt_client_data.h"
#include <termios.h>

bool smpt_open_serial_port_internal(Smpt_device *const device)
{
    return smpt_open_serial_port_linux(&device->serial_port_descriptor,
                                       device->serial_port_name, B3000000, true);
}


bool smpt_close_serial_port_internal(Smpt_device *const device)
{
    return smpt_close_serial_port_linux(device->serial_port_descriptor);
}


bool smpt_write_to_serial_port_internal(Smpt_device *const device,
                                        const uint8_t buffer[], uint32_t packet_length)
{
    return smpt_write_to_serial_port_linux(&device->serial_port_descriptor, buffer, packet_length);
}


bool smpt_read_from_serial_port_internal(Smpt_device *const device, uint32_t *bytes_read,
                                         uint8_t buffer[], uint32_t buffer_length)
{
    return smpt_read_from_serial_port_linux(bytes_read, device->serial_port_descriptor,
                                            buffer, buffer_length);
}

bool smpt_check_serial_port_internal(Smpt_device *const device)
{
    bool result;

    result = smpt_open_serial_port_linux(&device->serial_port_descriptor, device->serial_port_name,
                                         B3000000, false);

    if (result)
    {
        smpt_close_serial_port_internal(device);
    }

    return result;
}

#endif /* __ANDROID_API__ */

#endif /* __linux__ */

/*******************************************************************************
 Section Macos only
*******************************************************************************/
#ifdef __APPLE__

#include "smpt_serial_port_macos.h"
#include "smpt_packet_input_buffer.h"
#include "smpt_client_data.h"
#include <termios.h>

bool smpt_open_serial_port_internal(Smpt_device *const device)
{
    return smpt_open_serial_port_macos(&device->serial_port_descriptor,
                                       device->serial_port_name, B230400, true);
}


bool smpt_close_serial_port_internal(Smpt_device *const device)
{
    return smpt_close_serial_port_macos(device->serial_port_descriptor);
}


bool smpt_write_to_serial_port_internal(Smpt_device *const device,
                                        const uint8_t buffer[], uint32_t packet_length)
{
    return smpt_write_to_serial_port_macos(&device->serial_port_descriptor, buffer, packet_length);
}


bool smpt_read_from_serial_port_internal(Smpt_device *const device, uint32_t *bytes_read,
                                         uint8_t buffer[], uint32_t buffer_length)
{
    return smpt_read_from_serial_port_macos(bytes_read, device->serial_port_descriptor,
                                            buffer, buffer_length);
}

bool smpt_check_serial_port_internal(Smpt_device *const device)
{
    bool result;

    result = smpt_open_serial_port_macos(&device->serial_port_descriptor, device->serial_port_name,
                                         B230400, false);

    if (result)
    {
        smpt_close_serial_port_internal(device);
    }

    return result;
}

#endif /* __APPLE__ */
