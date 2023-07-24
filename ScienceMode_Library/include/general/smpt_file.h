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
#ifndef SMPT_FILE_H
#define SMPT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"
#include <stdint.h>
#include <stdio.h>

typedef enum
{
    Smpt_File_File_Name_Length = 512
} Smpt_File_Definitions;

typedef struct /* Smpt_file */
{
    char  file_name[Smpt_File_File_Name_Length];
    FILE* file_handler;
    bool  file_open;
} Smpt_file; /**< \struct Smpt_file */

/* max file_name length 511 characters */
bool smpt_open_file(char file_name[], Smpt_file *const file);
bool smpt_close_file(Smpt_file *const file);
bool smpt_write_block(uint8_t buffer[], uint32_t buffer_length, Smpt_file *const file);

void smpt_clear_file(Smpt_file *const file);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_FILE_H */

