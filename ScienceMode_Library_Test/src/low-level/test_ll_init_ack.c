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
#include "test_ll_init_ack.h"
#include "test_ll_definitions.h"

static void fill_ll_init_ack(Smpt_ll_init_ack *const ll_init_ack);
static bool compare_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack_a,
                                const Smpt_ll_init_ack *const ll_init_ack_b);

void test_ll_init_ack(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_ll_init_ack ll_init_ack;
    Smpt_ll_init_ack ll_init_ack_out;
    smpt_clear_ll_init_ack(&ll_init_ack);
    smpt_clear_ll_init_ack(&ll_init_ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_ll_init_ack(&ll_init_ack);

    packet_length = smpt_build_ll_init_ack(buffer, buffer_length, &ll_init_ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Ll_Init_Ack);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_ll_init_ack(&ll_init_ack_out, buffer, packet_length));

    assert(compare_ll_init_ack(&ll_init_ack, &ll_init_ack_out));
}

void fill_ll_init_ack(Smpt_ll_init_ack *const ll_init_ack)
{

    ll_init_ack->packet_number = 56;
    ll_init_ack->result = Smpt_Result_Parameter_Error;

}

bool compare_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack_a,
                         const Smpt_ll_init_ack *const ll_init_ack_b)
{
    bool result = true;

    return (result &&
           (ll_init_ack_a->packet_number == ll_init_ack_b->packet_number) &&
           (ll_init_ack_a->result == ll_init_ack_b->result));
}
