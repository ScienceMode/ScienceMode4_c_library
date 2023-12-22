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
#include "smpt_ml_client.h"

#include "smpt_client_cmd_lists.h"
#include "smpt_serial_port.h"
#include "smpt_ml_packet_client.h"
#include "smpt_packet_general.h"
#include "smpt_client_data.h"
#include "smpt_client_utils.h"

/*******************************************************************************
 Section Windows and Linux
*******************************************************************************/
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

bool smpt_send_ml_init(Smpt_device *const device, const Smpt_ml_init *const ml_init)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_ml_init(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, ml_init);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Ml_Init, ml_init->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_ml_update(Smpt_device *const device, const Smpt_ml_update *const ml_update)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_ml_update(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg, ml_update);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Ml_Update, ml_update->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}


bool smpt_send_ml_get_current_data(Smpt_device *const device,
                                   const Smpt_ml_get_current_data *const ml_get_current_data)
{
    bool result = false;
    uint32_t packet_length = 0;
    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_ml_get_current_data(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg,
                                                   ml_get_current_data);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Ml_Get_Current_Data, ml_get_current_data->packet_number,
                         &device->cmd_list);
        result = true;
    }

    return result;
}

bool smpt_send_ml_stop(Smpt_device *const device, uint8_t packet_number)
{
    return smpt_send_cmd(device, packet_number, Smpt_Cmd_Ml_Stop);
}

bool smpt_get_ml_get_current_data_ack(Smpt_device *const device,
                                      Smpt_ml_get_current_data_ack * const ml_get_current_data_ack)
{
    bool extracted = false;

    Smpt_Cmd cmd = smpt_get_cmd(device->packet, device->packet_length);
    Smpt_Cmd expected = Smpt_Cmd_Ml_Get_Current_Data_Ack;
    if (cmd == expected)
    {
        if (smpt_extract_ml_get_current_data_ack(ml_get_current_data_ack,
                                                 device->packet, device->packet_length))
        {
            extracted = true;
        }
    }
    else
    {
        smpt_error("smpt_get_ml_<<cmd>>: wrong cmd (expected: %d, got: %d)", expected, cmd);
    }

    return extracted;
}


#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */
