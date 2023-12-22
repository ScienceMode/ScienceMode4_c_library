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
#include "smpt_client.h"

#include "smpt_serial_port.h"
#include "smpt_client_data.h"
#include "smpt_packet_general.h"
#include "smpt_packet_client.h"
#include "smpt_client_cmd_lists.h"
#include "smpt_messages.h"
#include "smpt_client_utils.h"
#include "string.h"

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
#ifdef ANDROID
#else

bool send_general_cmd(Smpt_device *const device, uint8_t packet_number, Smpt_Cmd cmd_type);

bool smpt_open_serial_port(Smpt_device *const device, const char *const device_name)
{
    uint32_t length = strlen(device_name);
    if (length > Smpt_Length_Serial_Port_Chars)
    {
        smpt_error("smpt_open_serial_port: Device name too long.");
        return false;
    }

    memcpy(device->serial_port_name, device_name, length + 1);

    smpt_client_data_init(device);
    return smpt_open_serial_port_internal(device);
}

bool smpt_close_serial_port(Smpt_device *const device)
{
    return smpt_close_serial_port_internal(device);
}

bool smpt_check_serial_port(const char *const device_name)
{
    return smpt_check_serial_port_internal(device_name);
}

bool smpt_new_packet_received(Smpt_device *const device)
{
    return smpt_client_data_new_packet_received(device);
}

void smpt_last_ack(Smpt_device *const device, Smpt_ack *const ack)
{
    smpt_client_data_last_ack(device, ack);
}

bool smpt_send_get_version_main(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Version_Main);
}

bool smpt_send_get_version_stim(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Version_Stim);
}

bool smpt_send_get_extended_version(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Extended_Version);
}

bool smpt_send_get_device_id(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Device_Id);
}

bool smpt_send_get_battery_status(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Battery_Status);
}

bool smpt_send_reset(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Reset);
}

bool smpt_send_get_main_status(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Main_Status);
}

bool smpt_send_get_stim_status(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Get_Stim_Status);
}

bool smpt_get_get_version_main_ack(Smpt_device *const device,
                                   Smpt_get_version_ack *const get_version_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Version_Main_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_version_ack(get_version_ack,
                                         device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_get_version_stim_ack(Smpt_device *const device,
                                   Smpt_get_version_ack *const get_version_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Version_Stim_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_version_ack(get_version_ack,
                                         device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_get_extended_version_ack(Smpt_device *const device,
                                            Smpt_get_extended_version_ack *const get_extended_version_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Extended_Version_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_extended_version_ack(get_extended_version_ack,
                                         device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_get_device_id_ack(Smpt_device *const device,
                                Smpt_get_device_id_ack *const get_device_id_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Device_Id_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_device_id_ack(get_device_id_ack,
                                           device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_get_battery_status_ack(Smpt_device *const device,
                                     Smpt_get_battery_status_ack
                                     *const get_battery_status_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Battery_Status_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_battery_status_ack(get_battery_status_ack,
                                                device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_get_stim_status_ack(Smpt_device * const device,
                                  Smpt_get_stim_status_ack *const get_stim_status_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Stim_Status_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_stim_status_ack(get_stim_status_ack,
                                             device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_get_main_status_ack(Smpt_device * const device,
                                  Smpt_get_main_status_ack *const get_main_status_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Get_Main_Status_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_get_main_status_ack(get_main_status_ack,
                                             device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

Smpt_version smpt_library_version()
{
    Smpt_version version = {0};
    version.major    = Smpt_Library_Version_Major;
    version.minor    = Smpt_Library_Version_Minor;
    version.revision = Smpt_Library_Version_Revision;

    return version;
}

#endif /* ANDROID */
#endif /* defined(_WIN32) || defined(__linux__) */


