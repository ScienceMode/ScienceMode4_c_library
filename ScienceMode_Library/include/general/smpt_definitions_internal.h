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
#ifndef SMPT_DEFINITIONS_INTERNAL_H
#define SMPT_DEFINITIONS_INTERNAL_H

/***************************************************************************//**
* \file smpt_ll_definitions_internal.h
*
* This file defines the low level data types for internal implementation.
* They should not be visible when using the SMPT.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum /* Special packet bytes */
{
    Smpt_Packet_Start_Byte    = 0xf0,
    Smpt_Packet_Stop_Byte     = 0x0f,
    Smpt_Packet_Stuffing_Byte = 0x81,
    Smpt_Packet_Stuffing_Key  = 0x55
} Smpt_Packet_Bytes;

typedef enum
{
    Smpt_Min_Size_Get_Status_Ack      = 12, /* TODO */
    Smpt_Min_Size_File_Block          = 12,
    Smpt_Min_Size_Get_File            = 12,
    Smpt_Min_Size_Get_File_Ack        = 12,
    Smpt_Min_Size_Remove_File         = 12,
    Smpt_Min_Size_Get_Device_Id_Ack   = 12,
    Smpt_Min_Size_Get_Battery_Status_Ack = 12,
    Smpt_Min_Size_Get_Version_Ack     = 12,
    Smpt_Min_Size_Get_Extended_Version_Ack = 18,
    Smpt_Min_Size_Set_Power           = 12,
    Smpt_Min_Size_Get_Stim_Status_Ack = 15,
    Smpt_Min_Size_Get_Main_Status_Ack = 14,

    Smpt_Min_Size_Ll_Init             = 13,
    Smpt_Min_Size_Ll_Init_Ack         = 13,
    Smpt_Min_Size_Ll_Channel_Config   = 13,
    Smpt_Min_Size_Ll_Channel_Config_Ack = 14,
    Smpt_Min_Size_Ll_Cmd              = 12,
    Smpt_Min_Size_Ll_Ack              = 12,
    Smpt_Min_Size_Ll_Get_Emg_Reg      = 13,
    Smpt_Min_Size_Ll_Get_Emg_Reg_Ack  = 14,
    Smpt_Min_Size_Ll_Get_Emg_Data     = 13,
    Smpt_Min_Size_Ll_Get_Status_Ack   = 17,
    Smpt_Min_Size_Ll_Emg_Switches     = 17,

    Smpt_Min_Size_Ml_Init                 = 13,
    Smpt_Min_Size_Ml_Init_Ack             = 13,
    Smpt_Min_Size_Ml_Update               = 13,
    Smpt_Min_Size_Ml_Update_Ack           = 13,
    Smpt_Min_Size_Ml_Get_Current_Data     = 13,
    Smpt_Min_Size_Ml_Get_Current_Data_Ack = 14,

    /* TODO correct the minimal sizes for Apero-Layer*/
    Smpt_Min_Size_Al_Init                 = 12,
    Smpt_Min_Size_Al_Init_Ack             = 12,
    Smpt_Min_Size_Al_Update               = 12,
    Smpt_Min_Size_Al_Current_Data         = 12,

    Smpt_Min_Size_Dl_Init                 = 180+12,
    Smpt_Min_Size_Dl_Init_Ack             = 35+12,
    Smpt_Min_Size_Dl_Start                = 12,
    Smpt_Min_Size_Dl_Start_Ack            = 12+1,
    Smpt_Min_Size_Dl_Stop                 = 12,
    Smpt_Min_Size_Dl_Stop_Ack             = 12+1,
    Smpt_Min_Size_Dl_Send_Live_Data       = 6+12,
    Smpt_Min_Size_Dl_Send_File            = 6+12,
    Smpt_Min_Size_Dl_Send_File_Ack        = 12+1,
    Smpt_Min_Size_Dl_Get                  = 12+1,
    Smpt_Min_Size_Dl_Get_Ack              = 14,
    Smpt_Min_Size_Dl_Send_MMI             = 234,
    Smpt_Min_Size_Dl_Power_Module         = 12+2,
    Smpt_Min_Size_Dl_Power_Module_Ack     = 12+1,
    Smpt_Min_Size_Dl_Sys                  = 12,
    Smpt_Min_Size_Dl_Sys_Ack              = 12,

    Smpt_Min_Size_Bl_Init_Ack             = 12,
    Smpt_Min_Size_Bl_Update_Init          = 12,

    Smpt_Min_Size_Dm_Init                 = 12,
    Smpt_Min_Size_Dm_Init_Ack             = 12,
    Smpt_Min_Size_Dm_Data                 = 12,
    Smpt_Min_Size_Minimal                 = 12

} Smpt_Min_Size_Cmd;

typedef enum /* Bitmasks */
{
    Smpt_Bitmask_Use_Emg            = 0x10,
    Smpt_Bitmask_Emg_Register       = 0x60,
    Smpt_Bitmask_Use_Module         = 0x80,
    Smpt_Bitmask_Channel            = 0x60,
    Smpt_Bitmask_Connector          = 0x10,
    Smpt_Bitmask_Number_Of_Points   = 0x0f,
    Smpt_Bitmask_Time               = 0xf0,
    Smpt_Bitmask_Current            = 0x0f,
    Smpt_Bitmask_Control_Mode       = 0x03,
    Smpt_Bitmask_Interpolation_Mode = 0x60,
    Smpt_Bitmask_Emg_Point          = 0x10
} Smpt_Bitmask;

typedef enum /* Header Bytes */
{
    Smpt_Index_Start_Byte         = 0,
    Smpt_Index_Length_Msb_Stuff   = 1,
    Smpt_Index_Length_Msb         = 2,
    Smpt_Index_Length_Lsb_Stuff   = 3,
    Smpt_Index_Length_Lsb         = 4,
    Smpt_Index_Checksum_Msb_Stuff = 5,
    Smpt_Index_Checksum_Msb       = 6,
    Smpt_Index_Checksum_Lsb_Stuff = 7,
    Smpt_Index_Checksum_Lsb       = 8,
    Smpt_Index_Cmd_Number_Msb     = 9,
    Smpt_Index_Cmd_Number_Lsb     = 10
} Smpt_Header_Index_Byte;

typedef enum
{
    Smpt_Checksum_Start_Index = 9
} Smpt_Checksum;

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_DEFINITIONS_INTERNAL_H */
