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
#include "smpt_dl_packet_client.h"

#include "smpt_dl_packet_validity.h"
#include "smpt_dl_packet_utils.h"
#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "string.h"

#include "smpt_messages.h"

static void     extract_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack,
                                const uint8_t buffer_unstuffed[]);

static void     extract_dl_power_module_ack(Smpt_dl_power_module_ack *const dl_power_module_ack,
                                            const uint8_t buffer_unstuffed[]);

static void     extract_dl_start_ack(Smpt_dl_start_ack *const dl_start_ack,
                                    const uint8_t buffer_unstuffed[]);

static void     extract_dl_stop_ack(Smpt_dl_stop_ack *const dl_stop_ack,
                                    const uint8_t buffer_unstuffed[]);

static void     extract_dl_get_ack(Smpt_dl_get_ack *const dl_get_ack,
                                    const uint8_t buffer_unstuffed[]);

static void     extract_dl_send_file(Smpt_dl_send_file *const dl_send_file,
                                     const uint8_t buffer_unstuffed[]);

static void     extract_dl_send_live_data(Smpt_dl_send_live_data *const dl_send_live_data,
                                     const uint8_t buffer_unstuffed[]);

static void     extract_dl_send_mmi(Smpt_dl_send_mmi *const dl_send_mmi,
                                    const uint8_t buffer_unstuffed[]);

static void     extract_dl_mmi(Smpt_dl_mmi *const dl_mmi,
                               const uint8_t buffer_unstuffed[]);

static void     extract_dl_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack,
                                   const uint8_t buffer_unstuffed[]);

static uint32_t insert_dl_get(uint8_t buffer_unstuffed[], const Smpt_dl_get *const dl_get,
                               uint32_t index);

uint32_t smpt_build_dl_init(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_init * const dl_init)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = smpt_dl_insert_init(buffer_unstuffed, dl_init, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Init, dl_init->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_start(uint8_t buffer[], uint32_t buffer_length, uint8_t packet_number)
{
    Smpt_cmd cmd;
    cmd.command_number = Smpt_Cmd_Dl_Start;
    cmd.packet_number = packet_number;
    return smpt_build_cmd(buffer, buffer_length, &cmd);
}

uint32_t smpt_build_dl_stop(uint8_t buffer[], uint32_t buffer_length, uint8_t packet_number)
{
    Smpt_cmd cmd;
    cmd.command_number = Smpt_Cmd_Dl_Stop;
    cmd.packet_number = packet_number;
    return smpt_build_cmd(buffer, buffer_length, &cmd);
}

uint32_t smpt_build_dl_get(uint8_t buffer[], uint32_t buffer_length,
                           const Smpt_dl_get * const dl_get)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_get(buffer_unstuffed, dl_get, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Get, dl_get->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_power_module(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_power_module * const dl_power_module)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = smpt_dl_insert_power_module(buffer_unstuffed, dl_power_module, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Power_Module, dl_power_module->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_send_file_ack(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_send_file_ack * const dl_send_file_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = smpt_dl_insert_send_file_ack(buffer_unstuffed, dl_send_file_ack, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Send_File_Ack, dl_send_file_ack->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_sys(uint8_t buffer[], uint32_t buffer_length,
                                     const Smpt_dl_sys *const dl_sys)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = smpt_dl_insert_sys(buffer_unstuffed, dl_sys, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Sys, dl_sys->packet_number);
    return packet_length;
}


bool smpt_extract_dl_stop_ack(Smpt_dl_stop_ack *const dl_stop_ack, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_clear_dl_stop_ack(dl_stop_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Stop_Ack)
    {
        extract_dl_stop_ack(dl_stop_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_stop_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Stop_Ack);
    }

    result = result && (smpt_is_valid_dl_stop_ack(dl_stop_ack));

    return result;
}

bool smpt_extract_dl_get_ack(Smpt_dl_get_ack *const dl_get_ack, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_clear_dl_get_ack(dl_get_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Get_Ack)
    {
        extract_dl_get_ack(dl_get_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_get_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Get_Ack);
    }

    result = result && (smpt_is_valid_dl_get_ack(dl_get_ack));

    return result;
}


bool smpt_extract_dl_send_file(Smpt_dl_send_file *const dl_send_file, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    smpt_clear_dl_send_file(dl_send_file);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Send_File)
    {
        extract_dl_send_file(dl_send_file, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_send_file(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Send_File);
    }

    result = result && (smpt_is_valid_dl_send_file(dl_send_file));

    return result;
}

bool smpt_extract_dl_send_live_data(Smpt_dl_send_live_data *const dl_send_live_data, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    smpt_clear_dl_send_live_data(dl_send_live_data);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Send_Live_Data)
    {
        extract_dl_send_live_data(dl_send_live_data, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_send_live_data(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Send_Live_Data);
    }

    result = result && (smpt_is_valid_dl_send_live_data(dl_send_live_data));

    return result;
}

bool smpt_extract_dl_send_mmi(Smpt_dl_send_mmi *const dl_send_mmi,
                              const uint8_t packet[],
                              uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    smpt_clear_dl_send_mmi(dl_send_mmi);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Send_MMI)
    {
        extract_dl_send_mmi(dl_send_mmi, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_send_mmi(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Send_MMI);
    }

    result = result && (smpt_is_valid_dl_send_mmi(dl_send_mmi));

    return result;
}

bool smpt_extract_dl_start_ack(Smpt_dl_start_ack *const dl_start_ack, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_clear_dl_start_ack(dl_start_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Start_Ack)
    {
        extract_dl_start_ack(dl_start_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_start_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Start_Ack);
    }

    result = result && (smpt_is_valid_dl_start_ack(dl_start_ack));

    return result;
}

bool smpt_extract_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_clear_dl_init_ack(dl_init_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Init_Ack)
    {
        extract_dl_init_ack(dl_init_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_init_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Init_Ack);
    }

    result = result && (smpt_is_valid_dl_init_ack(dl_init_ack));

    return result;
}

bool smpt_extract_dl_power_module_ack(Smpt_dl_power_module_ack *const dl_power_module_ack,
                                      const uint8_t packet[],
                                      uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_clear_dl_power_module_ack(dl_power_module_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Power_Module_Ack)
    {
        extract_dl_power_module_ack(dl_power_module_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_power_module_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Power_Module_Ack);
    }

    result = result && (smpt_is_valid_dl_power_module_ack(dl_power_module_ack));

    return result;
}

bool smpt_extract_dl_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack,
                             const uint8_t packet[],
                             uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    smpt_clear_dl_sys_ack(dl_sys_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Sys_Ack)
    {
        extract_dl_sys_ack(dl_sys_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_sys_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Sys_Ack);
    }

    result = result && (smpt_is_valid_dl_sys_ack(dl_sys_ack));

    return result;

}

/*******************************************************************************
 * help functions follow
 ******************************************************************************/

uint32_t insert_dl_get(uint8_t buffer_unstuffed[], const Smpt_dl_get *const dl_get,
                        uint32_t index)
{
    buffer_unstuffed[index++] = dl_get->get_type;
    if((dl_get->get_type == Smpt_Dl_Get_Type_File_By_Name))
    {
        index = smpt_dl_insert_file_by_name(buffer_unstuffed, &dl_get->file_by_name,
                                      index);
    }
    else if((dl_get->get_type == Smpt_Dl_Get_Type_File_Info))
    {
        index = smpt_dl_insert_file_info(buffer_unstuffed, &dl_get->file_info,
                                      index);
    }

    return index;
}

void extract_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack, const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_init_ack->packet_number = smpt_get_packet_number_unstuffed(buffer);

    dl_init_ack->result = (Smpt_Result) buffer[index++];
    index = smpt_dl_extract_ads129x(&dl_init_ack->ads129x,
                                    buffer, index);
    index = smpt_dl_extract_file_id(dl_init_ack->measurement_file_id,
                                    buffer, index);
    dl_init_ack->init_state = buffer[index++];
    dl_init_ack->freq_out = buffer[index++];
}

void extract_dl_power_module_ack(Smpt_dl_power_module_ack *const dl_power_module_ack, const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_power_module_ack->packet_number = smpt_get_packet_number_unstuffed(buffer);
    dl_power_module_ack->result = (Smpt_Result) buffer[index++];
    dl_power_module_ack->hardware_module    = buffer[index++];
    dl_power_module_ack->switch_on_off      = buffer[index++];
}

void extract_dl_start_ack(Smpt_dl_start_ack *const dl_start_ack, const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_start_ack->result = (Smpt_Result) buffer[index++];
    dl_start_ack->packet_number = smpt_get_packet_number_unstuffed(buffer);
}

void extract_dl_stop_ack(Smpt_dl_stop_ack *const dl_stop_ack, const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_stop_ack->packet_number = smpt_get_packet_number_unstuffed(buffer);
    dl_stop_ack->result = (Smpt_Result) buffer[index++];
    index = smpt_extract_uint64_t(&dl_stop_ack->time, buffer, index);
}

void extract_dl_get_ack(Smpt_dl_get_ack *const dl_get_ack, const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_get_ack->packet_number = smpt_get_packet_number_unstuffed(buffer);
    dl_get_ack->result        = (Smpt_Result) buffer[index++];
    dl_get_ack->get_type      = buffer[index++];

    switch (dl_get_ack->get_type) {
        case Smpt_Dl_Get_Type_Battery_Status:
            index = smpt_dl_extract_battery_status(&dl_get_ack->battery_status,
                                                   buffer, index);
        break;
        case Smpt_Dl_Get_Type_File_By_Name:
            index = smpt_dl_extract_file_by_name(&dl_get_ack->file_by_name,
                                               buffer, index);
        break;
        case Smpt_Dl_Get_Type_File_System_Status:
            index = smpt_dl_extract_file_system_status(&dl_get_ack->file_system_status,
                                                       buffer, index);
        break;
        case Smpt_Dl_Get_Type_List_of_MMI:
            index = smpt_extract_uint16_t(&dl_get_ack->mmi.n_measurements, buffer, index);
        break;
        case Smpt_Dl_Get_Type_Operation_Mode:
            dl_get_ack->operation_mode= buffer[index++];
        break;
        case Smpt_Dl_Get_Type_Device_Id:
            index = smpt_dl_extract_string(dl_get_ack->deviceId, buffer, index, SMPT_DL_MAX_STRING_LENGTH);
        break;
        case Smpt_Dl_Get_Type_Firmware_Version:
            index = smpt_dl_extract_string(dl_get_ack->firmwareVersion, buffer, index, SMPT_DL_MAX_STRING_LENGTH);
        break;
        case Smpt_Dl_Get_Type_File_Info:
            index = smpt_dl_extract_file_info(&dl_get_ack->file_info, buffer, index);
        break;
    }
}

void extract_dl_send_file(Smpt_dl_send_file *const dl_send_file,
                          const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_send_file->packet_number = smpt_get_packet_number_unstuffed(buffer);

    index = smpt_extract_uint32_t(&dl_send_file->block_number, buffer, index);
    index = smpt_extract_uint16_t(&dl_send_file->n_bytes_per_block, buffer, index);
    if (dl_send_file->n_bytes_per_block > SMPT_DL_MAX_BLOCK_BYTES_LENGTH)
    {
        smpt_error("extract_dl_send_file(): too many block bytes, "
                   "max is %d. dl_send_file->n_bytes is %d",
                   SMPT_DL_MAX_BLOCK_BYTES_LENGTH,
                   dl_send_file->n_bytes_per_block);
    }
    else
    {
        memcpy(dl_send_file->data, &buffer[index], dl_send_file->n_bytes_per_block);
        index += dl_send_file->n_bytes_per_block;
    }
}

void extract_dl_send_live_data(Smpt_dl_send_live_data *const live_data,
                               const uint8_t buffer[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    live_data->packet_number = smpt_get_packet_number_unstuffed(buffer);

    live_data->n_channels = buffer[index++];

    index = smpt_dl_extract_time_length(&(live_data->time_offset), buffer, index);

    if (live_data->n_channels > SMPT_DL_MAX_CHANNELS)
    {
        smpt_error("extract_dl_send_live_data(): "
                   "dl_send_live_data->n_channels = %d is too large. "
                   "Maximum is Smpt_Length_N_Dl_Channels = %d",
                   live_data->n_channels,
                   SMPT_DL_MAX_CHANNELS);
    }
    else
    {
        index = smpt_dl_extract_electrode_samples(live_data->electrode_samples,
                                                  buffer, index,
                                                  live_data->n_channels);
    }
}

void extract_dl_send_mmi(Smpt_dl_send_mmi *const send_mmi,
                         const uint8_t buffer[])
{
    int32_t index = Smpt_Limit_Header_Size;
    send_mmi->packet_number = smpt_get_packet_number_unstuffed(buffer);
    index = smpt_dl_extract_init_(&send_mmi->dl_init, buffer);
    extract_dl_mmi(&send_mmi->mmi, buffer + index);
}

void extract_dl_mmi(Smpt_dl_mmi *const mmi,
                         const uint8_t buffer[])
{
    int32_t index = 0;
    index = smpt_dl_extract_file_id(mmi->measurement_id, buffer, index);
    index = smpt_dl_extract_file_size(&mmi->file_size, buffer, index);
    index = smpt_dl_extract_measurement_number(&mmi->measurement_number,
                                               buffer,index);
    index = smpt_dl_extract_patient_number(mmi->patient_number, buffer, index);
    index = smpt_dl_extract_tm(&mmi->start_time, buffer, index);
    index = smpt_dl_extract_time_length(&mmi->time_length, buffer, index);
}

void extract_dl_sys_ack(Smpt_dl_sys_ack * const dl_sys_ack, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    dl_sys_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    dl_sys_ack->result        = (Smpt_Result) buffer_unstuffed[index++];
    index = smpt_dl_extract_sys_ack(dl_sys_ack, buffer_unstuffed, index);
}





