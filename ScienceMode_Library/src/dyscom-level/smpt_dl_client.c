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
#include "smpt_dl_client.h"

#include "smpt_client_cmd_lists.h"
#include "smpt_dl_packet_client.h"
#include "smpt_serial_port.h"

#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"

#include "smpt_messages.h"

/*******************************************************************************
 Section Windows and Linux
*******************************************************************************/
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

bool smpt_send_dl_init(Smpt_device *const device, const Smpt_dl_init *const dl_init)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_init(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, dl_init);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Init, dl_init->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_dl_start(Smpt_device *const device, uint8_t packet_number)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_start(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, packet_number);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Start, packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_dl_stop(Smpt_device *const device, uint8_t packet_number)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_stop(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, packet_number);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Stop, packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_dl_get(Smpt_device *const device, const Smpt_dl_get *const dl_get)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_get(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, dl_get);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Get, dl_get->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_get_dl_get_ack(Smpt_device *const device, Smpt_dl_get_ack *const dl_get_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Get_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_dl_get_ack(dl_get_ack, device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_send_live_data(Smpt_device *const device,
                                Smpt_dl_send_live_data *const dl_send_live_data)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Send_Live_Data;
    if (cmd == expected)
    {
        if (smpt_extract_dl_send_live_data(dl_send_live_data,
                                           device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_send_file(Smpt_device *const device, Smpt_dl_send_file *const dl_send_file)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Send_File;
    if (cmd == expected)
    {
        if (smpt_extract_dl_send_file(dl_send_file,
                                      device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_send_mmi(Smpt_device *const device, Smpt_dl_send_mmi *const dl_send_mmi)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Send_MMI;
    if (cmd == expected)
    {
        if (smpt_extract_dl_send_mmi(dl_send_mmi,
                                     device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_init_ack(Smpt_device *const device, Smpt_dl_init_ack *const dl_init_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Init_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_dl_init_ack(dl_init_ack,
                                     device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_power_module_ack(Smpt_device *const device, Smpt_dl_power_module_ack *const dl_power_module_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Power_Module_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_dl_power_module_ack(dl_power_module_ack,
                                     device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_stop_ack(Smpt_device *const device, Smpt_dl_stop_ack *const dl_stop_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Stop_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_dl_stop_ack(dl_stop_ack,
                                     device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_get_dl_sys_ack(Smpt_device *const device, Smpt_dl_sys_ack *const dl_sys_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Dl_Sys_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_dl_sys_ack(dl_sys_ack, device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_dl_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

bool smpt_send_dl_power_module(Smpt_device *const device, const Smpt_dl_power_module *const dl_power_module)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_power_module(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, dl_power_module);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Power_Module, dl_power_module->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_dl_send_file_ack(Smpt_device *const device, const Smpt_dl_send_file_ack *const dl_send_file_ack)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_send_file_ack(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, dl_send_file_ack);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Send_File_Ack, dl_send_file_ack->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_dl_sys(Smpt_device * const device, const Smpt_dl_sys * const dl_sys)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_dl_sys(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, dl_sys);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Dl_Sys, dl_sys->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}
#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */


