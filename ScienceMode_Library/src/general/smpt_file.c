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
#include "smpt_file.h"
#include "smpt_messages.h"

#include <string.h>
#include <stdio.h>

bool smpt_open_file(char file_name[], Smpt_file *const file)
{
    bool result = false;

    memset(file->file_name, 0, Smpt_File_File_Name_Length);
    strcpy(file->file_name, file_name);

    file->file_handler = fopen(file->file_name, "w");
    if (file->file_handler == NULL)
    {
        smpt_error("smpt_file.c: Error open file");
        file->file_open = false;
    }
    else
    {
        file->file_open = true;
        result = true;
    }

    return result;
}

bool smpt_close_file(Smpt_file *const file)
{
    bool result = false;
    if (fclose(file->file_handler) != 0)
    {
          smpt_error("smpt_file.c: Error close file");
    }
    else
    {
        file->file_open = false;
        result = true;
    }

    return result;
}

bool smpt_write_block(uint8_t buffer[], uint32_t buffer_length, Smpt_file *const file)
{
    bool result = false;

    if (file->file_open == true)
    {
        if ((fwrite(buffer, sizeof(uint8_t), buffer_length, file->file_handler)) != 0)
        {
            result = true;
        }
        else
        {
            smpt_error("smpt_file.c: Write error");
        }
    }
    else
    {
        smpt_error("smpt_file.c: Write error. File not opened");
    }
    return result;
}

void smpt_clear_file(Smpt_file *const file)
{
    file->file_handler = 0;
    memset(file->file_name, 0, Smpt_File_File_Name_Length);
    file->file_open = false;
}
