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
#include "test_ll_init.h"
#include "test_ll_definitions.h"

static void fill_ll_init(Smpt_ll_init *const ll_init);
static bool compare_ll_init(const Smpt_ll_init *const ll_init_a,
                            const Smpt_ll_init *const ll_init_b);

void test_ll_init(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_ll_init ll_init;
    Smpt_ll_init ll_init_out;
    smpt_clear_ll_init(&ll_init);
    smpt_clear_ll_init(&ll_init_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_ll_init(&ll_init);

    packet_length = smpt_build_ll_init(buffer, buffer_length, &ll_init);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Ll_Init);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_ll_init(&ll_init_out, buffer, packet_length));

    assert(compare_ll_init(&ll_init, &ll_init_out));
}

void fill_ll_init(Smpt_ll_init *const ll_init)
{
    ll_init->measurement_type = Smpt_Measurement_High_Voltage;
    ll_init->packet_number = 50;
    ll_init->high_voltage_level = Smpt_High_Voltage_30V;
}

bool compare_ll_init(const Smpt_ll_init *const ll_init_a, const Smpt_ll_init *const ll_init_b)
{
    bool result = true;

    return result &&
           (ll_init_a->measurement_type ==
            ll_init_b->measurement_type) &&
           (ll_init_a->packet_number ==
            ll_init_b->packet_number) &&
           (ll_init_a->high_voltage_level ==
            ll_init_b->high_voltage_level);
}

