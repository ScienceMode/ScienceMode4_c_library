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
#include "smpt_client_power.h"

#include "smpt_packet_general.h"
#include "smpt_packet_client.h"
#include "smpt_client_cmd_lists.h"
#include "smpt_messages.h"
#include "smpt_client_utils.h"
#include "smpt_serial_port.h"

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

bool smpt_send_set_power(Smpt_device *const device, const Smpt_set_power *const set_power)
{
    bool result = false;
    uint32_t packet_length = 0;

    uint8_t buffer_out[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg);

    packet_length = smpt_build_set_power(buffer_out, Smpt_Limit_Max_Packet_Size_No_Emg,
                                         set_power);
    if (smpt_write_to_serial_port_internal(device, buffer_out, packet_length))
    {
        smpt_add_request(Smpt_Cmd_Set_Power, set_power->packet_number, &device->cmd_list);
        result = true;
    }

    return result;
}

#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */
