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
#include "test_file_block.h"

#include "test_definitions.h"

static void fill_file_block(Smpt_file_block *const file_block);
static bool compare_file_block(const Smpt_file_block *const file_block_a,
                               const Smpt_file_block *const file_block_b);

void test_file_block()
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_file_block file_block;
    Smpt_file_block file_block_out;
    smpt_clear_file_block(&file_block);
    smpt_clear_file_block(&file_block_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_file_block(&file_block);

    packet_length = smpt_build_file_block(buffer, buffer_length, Smpt_Cmd_Dl_Send_File,
                                          &file_block);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Dl_Send_File);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_file_block(&file_block_out, buffer, packet_length));

    assert(compare_file_block(&file_block, &file_block_out));
}

void fill_file_block(Smpt_file_block * const file_block)
{
    uint32_t i;

    file_block->packet_number = 12;
    file_block->block_number = 8;

    file_block->number_of_bytes = 400;

    for (i = 0; i < file_block->number_of_bytes; i++)
    {
        file_block->data[i] = 23;
    }
}

bool compare_file_block(const Smpt_file_block *const file_block_a,
                        const Smpt_file_block *const file_block_b)
{
    bool valid = true;
    uint32_t i;

    for (i = 0; i < file_block_a->number_of_bytes; i++)
    {
        valid = valid && file_block_a->data[i] == file_block_b->data[i];
    }

    return (valid &&
            (file_block_a->packet_number   == file_block_b->packet_number) &&
            (file_block_a->number_of_bytes == file_block_b->number_of_bytes) &&
            (file_block_a->block_number    == file_block_b->block_number));
}

