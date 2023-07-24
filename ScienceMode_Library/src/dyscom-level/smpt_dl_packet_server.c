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
#include "smpt_dl_packet_server.h"

#include "smpt_dl_packet_validity.h"
#include "smpt_dl_packet_utils.h"
#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "smpt_messages.h"
#include "string.h"

static uint32_t insert_dl_init_ack(uint8_t buffer[],
                                   const Smpt_dl_init_ack *const dl_init_ack,
                                   uint32_t index);

static uint32_t insert_dl_power_module_ack(uint8_t buffer[],
                                   const Smpt_dl_power_module_ack *const dl_power_module_ack,
                                   uint32_t index);

static uint32_t insert_dl_start_ack(uint8_t buffer[],
                                   const Smpt_dl_start_ack *const dl_start_ack,
                                   uint32_t index);

static uint32_t insert_dl_stop_ack(uint8_t buffer[],
                                   const Smpt_dl_stop_ack *const dl_stop_ack,
                                   uint32_t index);

static uint32_t insert_dl_get_ack(uint8_t buffer[],
                                   const Smpt_dl_get_ack *const dl_get_ack,
                                   uint32_t index);

static uint32_t insert_dl_sys_ack(uint8_t buffer[],
                                  const Smpt_dl_sys_ack *const dl_sys_ack, uint32_t index);

static void extract_dl_get(Smpt_dl_get *const dl_get,
                    const uint8_t buffer_unstuffed[]);

static uint32_t insert_dl_send_file(uint8_t buffer[],
                                   const Smpt_dl_send_file *const dl_send_file,
                                   uint32_t index);
static void extract_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack,
                    const uint8_t buffer_unstuffed[]);

static void extract_dl_sys(Smpt_dl_sys *const dl_sys,
                    const uint8_t buffer_unstuffed[]);

static uint32_t insert_dl_send_live_data(uint8_t buffer[],
                                   const Smpt_dl_send_live_data *const dl_send_live_data,
                                   uint32_t index);

static uint32_t insert_dl_send_mmi(uint8_t buffer[],
                                   const Smpt_dl_send_mmi *const dl_send_mmi,
                                   uint32_t index);

static uint32_t insert_dl_mmi(uint8_t buffer[],
                              const Smpt_dl_mmi *const dl_mmi,
                              uint32_t index);


bool smpt_extract_dl_init(Smpt_dl_init *dl_init, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_dl_init(dl_init);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Init)
    {
        smpt_dl_extract_init_(dl_init, buffer_unstuffed);
        dl_init->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_init(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Init);
    }

    result = result && (smpt_is_valid_dl_init(dl_init));

    return result;
}

bool smpt_extract_dl_get(Smpt_dl_get *const dl_get, const uint8_t packet[],
                          uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_dl_get(dl_get);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Get)
    {
        extract_dl_get(dl_get, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_get(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Get);
    }

    result = result && (smpt_is_valid_dl_get(dl_get));

    return result;
}

bool smpt_extract_dl_power_module(Smpt_dl_power_module *const dl_power_module,
                                  const uint8_t packet[],
                                  uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_dl_power_module(dl_power_module);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Power_Module)
    {
        smpt_dl_extract_power_module(dl_power_module, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_dl_extract_power_module(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Power_Module);
    }

    result = result && (smpt_is_valid_dl_power_module(dl_power_module));

    return result;
}

void extract_dl_get(Smpt_dl_get *const dl_get,
                    const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_get->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    dl_get->get_type      = buffer_unstuffed[index++];

    /*
     * Always expect name payload at the end!
     */
    if((dl_get->get_type == Smpt_Dl_Get_Type_File_By_Name))
    {
        index = smpt_dl_extract_file_by_name(&dl_get->file_by_name, buffer_unstuffed, index);
    }
    else if((dl_get->get_type == Smpt_Dl_Get_Type_File_Info))
    {
        index = smpt_dl_extract_file_info(&dl_get->file_info, buffer_unstuffed, index);
    }
}

uint32_t smpt_build_dl_init_ack(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_init_ack * const dl_init_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_init_ack(buffer_unstuffed, dl_init_ack,
                                                Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Init_Ack,
                                      dl_init_ack->packet_number);
    return packet_length;
}


uint32_t smpt_build_dl_start_ack(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_start_ack * const dl_start_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_start_ack(buffer_unstuffed, dl_start_ack,
                                                Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Start_Ack,
                                      dl_start_ack->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_stop_ack(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_stop_ack * const dl_stop_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_stop_ack(buffer_unstuffed, dl_stop_ack,
                                    Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Stop_Ack,
                                      dl_stop_ack->packet_number);
    return packet_length;
}


uint32_t smpt_build_dl_get_ack(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_get_ack * const dl_get_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_get_ack(buffer_unstuffed, dl_get_ack,
                                    Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Get_Ack,
                                      dl_get_ack->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_send_file(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_send_file * const dl_send_file)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_dl_send_file(buffer_unstuffed, dl_send_file,
                                    Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Send_File,
                                      dl_send_file->packet_number);
    return packet_length;
}

uint32_t smpt_build_dl_send_live_data(uint8_t buffer[], uint32_t buffer_length,
                                      const Smpt_dl_send_live_data * const dl_send_live_data)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_dl_send_live_data(buffer_unstuffed, dl_send_live_data,
                                    Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Send_Live_Data,
                                      dl_send_live_data->packet_number);
    return packet_length;
}

uint32_t insert_dl_init_ack(uint8_t buffer[], const Smpt_dl_init_ack *const dl_init_ack,
                            uint32_t index)
{
    buffer[index++] = dl_init_ack->result;
    index = smpt_dl_insert_ads129x(buffer, &dl_init_ack->ads129x, index);
    index = smpt_dl_insert_file_id(buffer, dl_init_ack->measurement_file_id, index);
    buffer[index++] = dl_init_ack->init_state;
    buffer[index++] = dl_init_ack->freq_out;

    return index;
}

uint32_t insert_dl_power_module_ack(uint8_t buffer[], const Smpt_dl_power_module_ack *const dl_power_module_ack,
                            uint32_t index)
{
    buffer[index++] = dl_power_module_ack->result;
    buffer[index++] = (uint8_t) dl_power_module_ack->hardware_module;
    buffer[index++] = dl_power_module_ack->switch_on_off;

    return index;
}

uint32_t insert_dl_start_ack(uint8_t buffer[],
                                   const Smpt_dl_start_ack *const dl_start_ack,
                                   uint32_t index)
{
    buffer[index++] = dl_start_ack->result;

    return index;
}

uint32_t insert_dl_stop_ack(uint8_t buffer[],
                            const Smpt_dl_stop_ack *const dl_stop_ack,
                            uint32_t index)
{
    buffer[index++] = dl_stop_ack->result;
    index = smpt_insert_uint64_t(buffer, &dl_stop_ack->time, index);

    return index;
}

uint32_t insert_dl_get_ack(uint8_t buffer[],
                           const Smpt_dl_get_ack *const dl_get_ack,
                           uint32_t index)
{
    buffer[index++] = dl_get_ack->result;
    buffer[index++] = dl_get_ack->get_type;

    switch (dl_get_ack->get_type)
    {
        case Smpt_Dl_Get_Type_Battery_Status:
            index = smpt_dl_insert_battery_status(buffer,
                                                  &dl_get_ack->battery_status,
                                                  index);
        break;
        case Smpt_Dl_Get_Type_File_By_Name:
            index = smpt_dl_insert_file_by_name(buffer, &dl_get_ack->file_by_name, index);

        break;
        case Smpt_Dl_Get_Type_File_System_Status:
            index = smpt_dl_insert_file_system_status(buffer,
                                                      &dl_get_ack->file_system_status,
                                                      index);
        break;
        case Smpt_Dl_Get_Type_List_of_MMI:
            index = smpt_insert_uint16_t(buffer,
                                         &dl_get_ack->mmi.n_measurements,
                                         index);
        break;
        case Smpt_Dl_Get_Type_Operation_Mode:
            buffer[index++] = dl_get_ack->operation_mode;
        break;
        case Smpt_Dl_Get_Type_Device_Id:
            index = smpt_dl_insert_string(buffer,
                                          dl_get_ack->deviceId,
                                          index,
                                          SMPT_DL_MAX_STRING_LENGTH);
        break;
        case Smpt_Dl_Get_Type_Firmware_Version:
            index = smpt_dl_insert_string(buffer,
                                          dl_get_ack->firmwareVersion,
                                          index,
                                          SMPT_DL_MAX_STRING_LENGTH);
        break;
        case Smpt_Dl_Get_Type_File_Info:
            index = smpt_dl_insert_file_info(buffer, &dl_get_ack->file_info, index);

        break;
    }

    return index;
}

uint32_t insert_dl_send_file(uint8_t buffer[],
                             const Smpt_dl_send_file *const dl_send_file,
                             uint32_t index)
{
    index = smpt_insert_uint32_t(buffer, &dl_send_file->block_number, index);
    index = smpt_insert_uint16_t(buffer, &dl_send_file->n_bytes_per_block, index);
    if (dl_send_file->n_bytes_per_block > SMPT_DL_MAX_BLOCK_BYTES_LENGTH)
    {
        smpt_error("insert_dl_send_file(): too large number in dl_send_file->n_bytes: %d."
                   "Max %d is allowed", dl_send_file->n_bytes_per_block, SMPT_DL_MAX_BLOCK_BYTES_LENGTH) ;
    }
    else
    {
        memcpy(&buffer[index], dl_send_file->data, dl_send_file->n_bytes_per_block);
        index += dl_send_file->n_bytes_per_block;
    }

    return index;
}

uint32_t insert_dl_send_live_data(uint8_t buffer[],
                                   const Smpt_dl_send_live_data *const dl_send_live_data,
                                   uint32_t index)
{
    if (dl_send_live_data->n_channels > SMPT_DL_MAX_CHANNELS)
    {
        smpt_error("insert_dl_send_live_data(): dl_send_live_data->n_channels = %d is too large. "
                   "Maximum is Smpt_Length_N_Dl_Channels = %d.",
                   dl_send_live_data->n_channels,
                   SMPT_DL_MAX_CHANNELS);
    }
    else
    {
        buffer[index++] = dl_send_live_data->n_channels;
        index = smpt_dl_insert_time_length(buffer, &(dl_send_live_data->time_offset), index);
        index = smpt_dl_insert_electrode_samples(buffer,
                               dl_send_live_data->electrode_samples,
                               index, dl_send_live_data->n_channels);
    }

    return index;
}

uint32_t insert_dl_mmi(uint8_t buffer[], const Smpt_dl_mmi * const dl_mmi, uint32_t index)
{
    index = smpt_dl_insert_file_id  (buffer, dl_mmi->measurement_id, index);
    index = smpt_dl_insert_file_size(buffer, &dl_mmi->file_size, index);
    index = smpt_dl_insert_measurement_number(buffer, &dl_mmi->measurement_number, index);
    index = smpt_dl_insert_patient_number(buffer, dl_mmi->patient_number, index);
    index = smpt_dl_insert_tm       (buffer, &dl_mmi->start_time,      index);
    index = smpt_dl_insert_time_length(buffer, &dl_mmi->time_length,   index);

    return index;
}

uint32_t insert_dl_send_mmi(uint8_t buffer[], const Smpt_dl_send_mmi * const dl_send_mmi, uint32_t index)
{
    index = smpt_dl_insert_init     (buffer, &dl_send_mmi->dl_init, index);
    index = insert_dl_mmi(buffer, &dl_send_mmi->mmi, index);

    return index;
}

uint32_t smpt_build_dl_send_mmi(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_send_mmi * const dl_send_mmi)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_dl_send_mmi(buffer_unstuffed, dl_send_mmi,
                                    Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Send_MMI,
                                      dl_send_mmi->packet_number);
    return packet_length;
}


uint32_t smpt_build_dl_power_module_ack(uint8_t buffer[], uint32_t buffer_length,
                                        const Smpt_dl_power_module_ack * const dl_power_module_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_power_module_ack(buffer_unstuffed, dl_power_module_ack,
                                                Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Power_Module_Ack,
                                      dl_power_module_ack->packet_number);
    return packet_length;
}

bool smpt_extract_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack,
                                  const uint8_t packet[],
                                  uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);
    smpt_clear_dl_send_file_ack(dl_send_file_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Send_File_Ack)
    {
        extract_dl_send_file_ack(dl_send_file_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_send_file_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Send_File_Ack);
    }

    return result;
}

void extract_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack,
                    const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_send_file_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    index = smpt_dl_extract_send_file_ack(dl_send_file_ack, buffer_unstuffed, index);
}

bool smpt_extract_dl_sys(Smpt_dl_sys * const dl_sys, const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);
    smpt_clear_dl_sys(dl_sys);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);


    if (packet_unstuffed_length >= Smpt_Min_Size_Dl_Sys)
    {
        extract_dl_sys(dl_sys, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_dl_sys(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Dl_Sys);
    }

    return result;
}

void extract_dl_sys(Smpt_dl_sys *const dl_sys,
                    const uint8_t buffer_unstuffed[])
{
     uint32_t index = Smpt_Limit_Header_Size;
     dl_sys->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
     index = smpt_dl_extract_sys(dl_sys, buffer_unstuffed, index);
}

uint32_t smpt_build_dl_sys_ack(uint8_t buffer[], uint32_t buffer_length, const Smpt_dl_sys_ack * const dl_sys_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_dl_sys_ack(buffer_unstuffed, dl_sys_ack,
                                    Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Dl_Sys_Ack,
                                      dl_sys_ack->packet_number);
    return packet_length;
}

uint32_t insert_dl_sys_ack(uint8_t buffer[],
                                  const Smpt_dl_sys_ack *const dl_sys_ack, uint32_t index)
{
   buffer[index++] = dl_sys_ack->result;
   index = smpt_dl_insert_sys_ack(buffer, dl_sys_ack, index);

   return index;
}
