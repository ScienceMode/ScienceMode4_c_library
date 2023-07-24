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
#ifndef SMPT_PACKET_INPUT_BUFFER_DEFINITIONS_H
#define SMPT_PACKET_INPUT_BUFFER_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

typedef enum /* General modul constants */
{
#ifdef UC_MAIN /* Also used for uc_stim and other controllers */
    Packet_Buffer_Number_Of_Rows_1 = 8,
    Packet_Buffer_Number_Of_Rows_2 = 1,
    Packet_Buffer_Number_Of_Rows_3 = 1,
    Packet_Buffer_Row_Length_1 = Smpt_Limit_Max_Packet_Size_No_Emg,   /* uc_main: buffer from ext_master, uc_stim: buffer from uc_main */
    Packet_Buffer_Row_Length_2 = 1,
    Packet_Buffer_Row_Length_3 = 1,

#else /* External Master */
    Packet_Buffer_Number_Of_Rows_1 = 8,
    Packet_Buffer_Number_Of_Rows_2 = 1,
    Packet_Buffer_Number_Of_Rows_3 = 1,
    Packet_Buffer_Row_Length_1 = Smpt_Limit_Max_Packet_Size_No_Emg,
    Packet_Buffer_Row_Length_2 = 1,
    Packet_Buffer_Row_Length_3 = 1,

#endif
    Packet_Buffer_Row_State_Empty = 0,
    Packet_Buffer_Row_State_Full = 1
} Packet_Buffer_Constants;

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_INPUT_BUFFER_DEFINITIONS_H */
