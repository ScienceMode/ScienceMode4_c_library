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
#include "test_dl_send_file.h"
#include "test_dl_common.h"

void test_dl_send_file()
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_send_file dl_send_file;
    Smpt_dl_send_file dl_send_file_out;
    smpt_clear_dl_send_file(&dl_send_file);
    smpt_clear_dl_send_file(&dl_send_file_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_send_file(&dl_send_file);

    packet_length = smpt_build_dl_send_file(buffer_, buffer_length_, &dl_send_file);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Send_File);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_send_file(&dl_send_file_out, buffer_, packet_length));
    assert(compare_dl_send_file(&dl_send_file, &dl_send_file_out));
}

void fill_dl_send_file(Smpt_dl_send_file * const dl_send_file)
{
    int n_blocks = SMPT_DL_MAX_BLOCK_BYTES_LENGTH;
    uint8_t i;
    dl_send_file->block_number = 33;
    dl_send_file->n_bytes_per_block = n_blocks;
    for (i = 0; i < n_blocks; i++)
    {
        dl_send_file->data[i] = i;
    }
    dl_send_file->packet_number = 42;
}

bool compare_dl_send_file(const Smpt_dl_send_file *const a,
                          const Smpt_dl_send_file *const b)
{
    bool valid = true;
    int i;
    if ((a->block_number != b->block_number) ||
        (a->n_bytes_per_block != b->n_bytes_per_block) ||
        (a->packet_number != b->packet_number))
    {
        valid = false;
    }
    if (a->n_bytes_per_block > SMPT_DL_MAX_BLOCK_BYTES_LENGTH)
    {
        valid = false;
    }
    else
    {
        for (i = 0; i < a->n_bytes_per_block; i++)
        {
            if (a->data[i] != b->data[i])
                valid = false;
        }
    }
    return valid;
}

void test_dl_send_file_ack()
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_send_file_ack dl_send_file_ack;
    Smpt_dl_send_file_ack dl_send_file_ack_out;
    smpt_clear_dl_send_file_ack(&dl_send_file_ack);
    smpt_clear_dl_send_file_ack(&dl_send_file_ack_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_send_file_ack(&dl_send_file_ack);

    packet_length = smpt_build_dl_send_file_ack(buffer_, buffer_length_, &dl_send_file_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Send_File_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_send_file_ack(&dl_send_file_ack_out, buffer_, packet_length));
    assert(compare_dl_send_file_ack(&dl_send_file_ack, &dl_send_file_ack_out));
}

void fill_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack)
{
    dl_send_file_ack->block_number = 33;
    dl_send_file_ack->packet_number = 63;
}

bool compare_dl_send_file_ack(const Smpt_dl_send_file_ack *const a,
                              const Smpt_dl_send_file_ack *const b)
{
    bool valid = true;
    if ((a->block_number != b->block_number) ||
       (a->packet_number != b->packet_number))
    {
        valid = false;
    }
    return valid;
}
