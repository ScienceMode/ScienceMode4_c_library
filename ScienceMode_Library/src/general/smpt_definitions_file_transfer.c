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
#include "smpt_definitions_file_transfer.h"
#include "smpt_definitions_data_types.h"
#include <string.h>

void smpt_clear_get_file(Smpt_get_file *const get_file)
{
    uint32_t i;

    get_file->packet_number = 0;

    get_file->file_name[0] = '\0';
    for (i = 1; i < Smpt_Length_File_Name; i++)
    {
        get_file->file_name[i] = 0;
    }
}

void smpt_clear_get_file_ack(Smpt_get_file_ack *const get_file_ack)
{
    uint32_t i;

    get_file_ack->packet_number = 0;
    get_file_ack->number_of_blocks = 0;

    get_file_ack->file_name[0] = '\0';
    for (i = 1; i < Smpt_Length_File_Name; i++)
    {
        get_file_ack->file_name[i] = 0;
    }
}

void smpt_clear_file_block(Smpt_file_block *const file_block)
{
    file_block->packet_number = 0;
    file_block->number_of_bytes = 0;
    file_block->block_number = 0;
    memset(file_block->data, 0, Smpt_Length_File_Block_Data * sizeof(file_block->data[0]));
}

void smpt_clear_remove_file(Smpt_remove_file *const remove_file)
{
    uint32_t i;

    remove_file->packet_number = 0;

    remove_file->file_name[0] = '\0';
    for (i = 1; i < Smpt_Length_File_Name; i++)
    {
        remove_file->file_name[i] = 0;
    }
}
