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
#ifndef SMPT_DEFINITIONS_FILE_TRANSFER_H
#define SMPT_DEFINITIONS_FILE_TRANSFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "smpt_definitions_data_types.h"

typedef enum
{
    Smpt_Length_File_Block_Data = 512
    /*Smpt_Length_File_Name       =  60*/
} Smpt_File_Transfer_Length;

typedef struct /* file_block */
{
    uint16_t number_of_bytes;
    uint8_t  data[Smpt_Length_File_Block_Data];
    uint32_t block_number;
    uint8_t  packet_number;  /**< [0 - 63] Packet number */

} Smpt_file_block; /**< \struct Smpt_file_block
                      Struct for the file_block data. \see e.g. smpt_al_get_file()*/

typedef struct /* get_file */
{
    char    file_name[Smpt_Length_File_Name];
    uint8_t packet_number;  /**< [0 - 63] Packet number */
} Smpt_get_file;

typedef struct /* get_file_ack */
{
    uint8_t  packet_number;  /**< [0 - 63] Packet number */
    Smpt_Result result;
    char     file_name[Smpt_Length_File_Name];
    uint32_t number_of_blocks;
} Smpt_get_file_ack;


typedef struct /* remove_file */
{
    char     file_name[Smpt_Length_File_Name];
    uint8_t  packet_number;  /**< [0 - 63] Packet number */
} Smpt_remove_file;

void smpt_clear_get_file(Smpt_get_file *const get_file);
void smpt_clear_get_file_ack(Smpt_get_file_ack *const get_file_ack);
void smpt_clear_file_block(Smpt_file_block *const file_block);
void smpt_clear_remove_file(Smpt_remove_file *const remove_file);


#ifdef __cplusplus
}
#endif

#endif /* SMPT_DEFINITIONS_FILE_TRANSFER_H */

