/***************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2022, MPL and LGPL HASOMED GmbH
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
#include "test_get_battery_status_ack.h"
#include "test_definitions.h"

static void fill_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack);
static bool compare_get_battery_status_ack(const Smpt_get_battery_status_ack
                                           *const get_battery_status_ack_a,
                                           const Smpt_get_battery_status_ack
                                           *const get_battery_status_ack_b);

void test_get_battery_status_ack()
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_get_battery_status_ack get_battery_status_ack;
    Smpt_get_battery_status_ack get_battery_status_ack_out;
    smpt_clear_get_battery_status_ack(&get_battery_status_ack);
    smpt_clear_get_battery_status_ack(&get_battery_status_ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_get_battery_status_ack(&get_battery_status_ack);

    packet_length = smpt_build_get_battery_status_ack(buffer, buffer_length,
                                                      &get_battery_status_ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Get_Battery_Status_Ack);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_get_battery_status_ack(&get_battery_status_ack_out, buffer, packet_length));

    assert(compare_get_battery_status_ack(&get_battery_status_ack, &get_battery_status_ack_out));
}

void fill_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack)
{
    get_battery_status_ack->packet_number = 12;
    get_battery_status_ack->result = Smpt_Result_Busy;
    get_battery_status_ack->battery_level = 100;
    get_battery_status_ack->battery_voltage = 65000;
}

bool compare_get_battery_status_ack(const Smpt_get_battery_status_ack *const get_battery_status_ack_a,
                                    const Smpt_get_battery_status_ack *const get_battery_status_ack_b)
{
    return ((get_battery_status_ack_a->packet_number    ==
             get_battery_status_ack_b->packet_number)   &&
            (get_battery_status_ack_a->battery_level    ==
             get_battery_status_ack_b->battery_level)   &&
            (get_battery_status_ack_a->battery_voltage  ==
             get_battery_status_ack_b->battery_voltage) &&
            (get_battery_status_ack_a->result           ==
             get_battery_status_ack_b->result));
}

