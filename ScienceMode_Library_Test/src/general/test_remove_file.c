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
#include "test_remove_file.h"

#include "test_definitions.h"

static void fill_remove_file(Smpt_remove_file *const remove_file);
static bool compare_remove_file(const Smpt_remove_file *const remove_file_a,
                             const Smpt_remove_file *const remove_file_b);

void test_remove_file(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_remove_file remove_file;
    Smpt_remove_file remove_file_out;
    smpt_clear_remove_file(&remove_file);
    smpt_clear_remove_file(&remove_file_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_remove_file(&remove_file);

    packet_length = smpt_build_remove_file(buffer, buffer_length,
                                           Smpt_Cmd_Dl_Get, &remove_file);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Dl_Get);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_remove_file(&remove_file_out, buffer, packet_length));

    assert(compare_remove_file(&remove_file, &remove_file_out));
}

void fill_remove_file(Smpt_remove_file *const remove_file)
{
    uint32_t i;
    remove_file->packet_number = 14;

    for (i = 0; i < Smpt_Length_File_Name; i++)
    {
        remove_file->file_name[i] = '1';
    }
}

bool compare_remove_file(const Smpt_remove_file *const remove_file_a,
                      const Smpt_remove_file *const remove_file_b)
{
    bool valid = true;
    uint32_t i;

    for (i = 0; i < Smpt_Length_File_Name; i++)
    {
        valid = valid && remove_file_a->file_name[i] == remove_file_b->file_name[i];
    }

    return ((remove_file_a->packet_number == remove_file_b->packet_number) && valid);
}

