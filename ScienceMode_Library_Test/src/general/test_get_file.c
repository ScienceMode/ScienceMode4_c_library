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
#include "test_get_file.h"

#include "test_definitions.h"

static void fill_get_file(Smpt_get_file *const get_file);
static bool compare_get_file(const Smpt_get_file *const get_file_a,
                             const Smpt_get_file *const get_file_b);

void test_get_file(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_get_file get_file;
    Smpt_get_file get_file_out;
    smpt_clear_get_file(&get_file);
    smpt_clear_get_file(&get_file_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_get_file(&get_file);

    packet_length = smpt_build_get_file(buffer, buffer_length, Smpt_Cmd_Dl_Get, &get_file);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Dl_Get);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_get_file(&get_file_out, buffer, packet_length));

    assert(compare_get_file(&get_file, &get_file_out));
}

void fill_get_file(Smpt_get_file *const get_file)
{
    uint32_t i;
    get_file->packet_number = 14;

    for (i = 0; i < Smpt_Length_File_Name; i++)
    {
        get_file->file_name[i] = '1';
    }
}

bool compare_get_file(const Smpt_get_file *const get_file_a,
                      const Smpt_get_file *const get_file_b)
{
    bool equal = true;
    uint32_t i;

    for (i = 0; i < Smpt_Length_File_Name; i++)
    {
        equal = equal && get_file_a->file_name[i] == get_file_b->file_name[i];
    }

    return ((get_file_a->packet_number == get_file_b->packet_number) && equal);
}

