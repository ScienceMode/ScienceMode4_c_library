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
#include "test_set_power.h"
#include "test_definitions.h"

static void fill_set_power(Smpt_set_power *const set_power);
static bool compare_set_power(const Smpt_set_power *const set_power_a,
                              const Smpt_set_power *const set_power_b);

void test_set_power(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_set_power set_power;
    Smpt_set_power set_power_out;
    smpt_clear_set_power(&set_power);
    smpt_clear_set_power(&set_power_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_set_power(&set_power);

    packet_length = smpt_build_set_power(buffer, buffer_length, &set_power);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Set_Power);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_set_power(&set_power_out, buffer, packet_length));

    assert(compare_set_power(&set_power, &set_power_out));
}

void fill_set_power(Smpt_set_power *const set_power)
{
    set_power->packet_number = 12;
    set_power->modules[Smpt_Power_Bluetooth] = true;
    set_power->modules[Smpt_Power_Emg] = true;
    set_power->modules[Smpt_Power_Memory_Card] = true;
    set_power->modules[Smpt_Power_Uc_Stim] = true;
}

bool compare_set_power(const Smpt_set_power *const set_power_a,
                       const Smpt_set_power *const set_power_b)
{
    bool equal = true;
    uint32_t i;

    for (i = 0; i < Smpt_Length_Power_Modules; i++)
    {
        equal = equal && set_power_a->modules[i] ==
                         set_power_b->modules[i];
    }

    return  (equal &&
            (set_power_a->packet_number    ==
             set_power_b->packet_number));
}
