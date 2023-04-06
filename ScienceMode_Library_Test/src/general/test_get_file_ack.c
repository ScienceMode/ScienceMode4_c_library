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
#include "test_get_file_ack.h"
#include "test_definitions.h"

static void fill_get_file_ack(Smpt_get_file_ack *const get_file_ack);
static bool compare_get_file_ack(const Smpt_get_file_ack *const get_file_ack_a,
                                 const Smpt_get_file_ack *const get_file_ack_b);

void test_get_file_ack()
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_get_file_ack get_file_ack;
    Smpt_get_file_ack get_file_ack_out;
    smpt_clear_get_file_ack(&get_file_ack);
    smpt_clear_get_file_ack(&get_file_ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_get_file_ack(&get_file_ack);

    packet_length = smpt_build_get_file_ack(buffer, buffer_length,
                                            Smpt_Cmd_Dl_Send_File_Ack, &get_file_ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Dl_Send_File_Ack);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_get_file_ack(&get_file_ack_out, buffer, packet_length));

    assert(compare_get_file_ack(&get_file_ack, &get_file_ack_out));
}

void fill_get_file_ack(Smpt_get_file_ack *const get_file_ack)
{
    uint32_t i;
    get_file_ack->packet_number = 14;
    get_file_ack->number_of_blocks = 1000;

    for (i = 0; i < Smpt_Length_File_Name; i++)
    {
        get_file_ack->file_name[i] = '1';
    }
}

bool compare_get_file_ack(const Smpt_get_file_ack *const get_file_ack_a,
                          const Smpt_get_file_ack *const get_file_ack_b)
{
    bool valid = true;
    uint32_t i;

    for (i = 0; i < Smpt_Length_File_Name; i++)
    {
        valid = valid && get_file_ack_a->file_name[i] == get_file_ack_b->file_name[i];
    }

    return ((get_file_ack_a->packet_number == get_file_ack_b->packet_number) && valid);
}

