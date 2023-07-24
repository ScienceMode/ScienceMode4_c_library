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
#include "test_ack.h"
#include "test_definitions.h"

static void fill_ack(Smpt_ack *const ack);
static bool compare_ack(const Smpt_ack *const ack_a,
                           const Smpt_ack *const ack_b);

void test_ack(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_ack ack;
    Smpt_ack ack_out;
    smpt_clear_ack(&ack);
    smpt_clear_ack(&ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_ack(&ack);

    packet_length = smpt_build_ack(buffer, buffer_length, &ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_ack(&ack_out, buffer, packet_length));

    assert(compare_ack(&ack, &ack_out));
}

void fill_ack(Smpt_ack *const ack)
{
    ack->command_number = 3;
    ack->packet_number = 1;
    ack->result = Smpt_Result_Transfer_Error;
}

static bool compare_ack(const Smpt_ack *const ack_a,
                        const Smpt_ack *const ack_b)
{
    return (ack_a->command_number == ack_b->command_number) &&
           (ack_a->packet_number == ack_b->packet_number) &&
           (ack_a->result == ack_b->result);
}


