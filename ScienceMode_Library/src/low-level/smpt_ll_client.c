/***************************************************************************//**
* \file smpt_ll_client.c
* \copyright MPL and LGPL HASOMED GmbH 2013-2022
*
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

#include "smpt_ll_client.h"
#include "smpt_ll_packet_client.h"
#include "smpt_client_cmd_lists.h"
#include "smpt_definitions_internal.h"
#include "smpt_packet_input_buffer.h"
#include "smpt_client_data.h"
#include "smpt_serial_port.h"
#include "smpt_client.h"
#include "smpt_client_utils.h"
#include "string.h"

/*******************************************************************************
 Section Windows and Linux
*******************************************************************************/
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

bool smpt_send_ll_init(Smpt_device *const device,
                       const Smpt_ll_init *const ll_init)
{
    bool result = false;
    uint32_t packet_length = 0;

    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_ll_init(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, ll_init);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Ll_Init, ll_init->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_ll_channel_config(Smpt_device *const device,
                                 const Smpt_ll_channel_config *const ll_channel_config)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size);

    packet_length = smpt_build_ll_channel_config(buffer_out, Smpt_Limit_Max_Packet_Size,
                                                 ll_channel_config);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Ll_Channel_Config, ll_channel_config->packet_number,
                         &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_ll_stop(Smpt_device *const device, uint8_t packet_number)
{   
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Ll_Stop);
}


bool smpt_get_ll_init_ack(const Smpt_device *const device, Smpt_ll_init_ack *const ll_init_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Ll_Init_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_ll_init_ack(ll_init_ack, device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_ll_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}


bool smpt_get_ll_channel_config_ack(const Smpt_device *const device,
                                    Smpt_ll_channel_config_ack *const ll_channel_config_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Ll_Channel_Config_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_ll_channel_config_ack(ll_channel_config_ack,
                                          device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_ll_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}

#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */
/*******************************************************************************
 End Section Windows and Linux
*******************************************************************************/

