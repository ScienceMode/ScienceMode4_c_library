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
#include "smpt_packet_client.h"

#include "smpt_definitions_internal.h"
#include "smpt_packet_general.h"
#include "smpt_definitions.h"
#include "smpt_packet_internal.h"
#include "smpt_packet_validity.h"
#include "smpt_messages.h"
#include "smpt_packet_utils.h"

static uint32_t insert_get_file(uint8_t buffer_unstuffed[], const Smpt_get_file *const get_file,
                                uint32_t index);
static uint32_t insert_remove_file(uint8_t buffer_unstuffed[],
                                   const Smpt_remove_file *const remove_file,
                                   uint32_t index);

static uint32_t insert_set_power(uint8_t buffer_unstuffed[], const Smpt_set_power *const set_power,
                                 uint32_t index);

static void extract_file_block(Smpt_file_block *const file_block, const uint8_t buffer_unstuffed[]);

static void extract_get_file_ack(Smpt_get_file_ack *const get_file_ack,
                                 const uint8_t buffer_unstuffed[]);
static void extract_get_device_id_ack(Smpt_get_device_id_ack *const get_device_id_ack,
                                      const uint8_t buffer_unstuffed[]);
static void extract_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack,
                                           const uint8_t buffer_unstuffed[]);
static void extract_get_version_ack(Smpt_get_version_ack *const get_version_ack,
                                    const uint8_t buffer_unstuffed[]);
static uint32_t extract_version(Smpt_version *const version, const uint8_t buffer_unstuffed[],
                                uint32_t index);
static void extract_get_extended_version_ack(Smpt_get_extended_version_ack *const get_extended_version_ack,
                                    const uint8_t buffer_unstuffed[]);
static void extract_get_stim_status_ack(Smpt_get_stim_status_ack *const get_stim_status_ack,
                                        const uint8_t buffer_unstuffed[]);
static void extract_get_main_status_ack(Smpt_get_main_status_ack *const get_main_status_ack,
                                        const uint8_t buffer_unstuffed[]);

uint32_t smpt_build_remove_file(uint8_t buffer[], uint32_t buffer_length, Smpt_Cmd cmd,
                                const Smpt_remove_file *const remove_file)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_remove_file(buffer_unstuffed, remove_file, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd, remove_file->packet_number);

    return packet_length;
}

uint32_t insert_remove_file(uint8_t buffer_unstuffed[], const Smpt_remove_file *const remove_file,
                            uint32_t index)
{
    index = smpt_insert_array_char(buffer_unstuffed, remove_file->file_name, Smpt_Length_File_Name,
                                   index);

    return index;
}

uint32_t smpt_build_set_power(uint8_t buffer[], uint32_t buffer_length,
                              const Smpt_set_power *const set_power)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_set_power(buffer_unstuffed, set_power, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Set_Power, set_power->packet_number);

    return packet_length;
}

uint32_t insert_set_power(uint8_t buffer_unstuffed[], const Smpt_set_power *const set_power,
                          uint32_t index)
{
    uint8_t i, b;

    for (b = 0; b < Smpt_Length_Power_Modules/8; b++)
    {
        uint8_t temp = 0;

        for (i = 0; i < 8; i++)
        {
            temp |= (set_power->modules[b * 8 + i] & 1) << i;
        }

        buffer_unstuffed[index++] = temp;
    }

    return index;
}


uint32_t smpt_build_get_file(uint8_t buffer[], uint32_t buffer_length, Smpt_Cmd cmd,
                                const Smpt_get_file *const get_file)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_buffer(buffer, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_get_file(buffer_unstuffed, get_file, Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd, get_file->packet_number);

    return packet_length;
}

uint32_t insert_get_file(uint8_t buffer_unstuffed[], const Smpt_get_file * const get_file,
                         uint32_t index)
{
    index = smpt_insert_array_char(buffer_unstuffed, get_file->file_name, Smpt_Length_File_Name,
                                   index);

    return index;
}

bool smpt_extract_file_block(Smpt_file_block *const file_block,
                             const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);
    smpt_clear_file_block(file_block);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_File_Block)
    {
        extract_file_block(file_block, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_al_current_data(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_File_Block);
    }

    result = result && (smpt_is_valid_file_block(file_block));

    return result;
}

void extract_file_block(Smpt_file_block *const file_block, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    file_block->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    index = smpt_extract_uint16_t(&file_block->number_of_bytes, buffer_unstuffed, index);
    index = smpt_extract_uint32_t(&file_block->block_number, buffer_unstuffed, index);

    index = smpt_extract_array_char((char*)file_block->data, file_block->number_of_bytes,
                                    buffer_unstuffed, index);
}

bool smpt_extract_get_file_ack(Smpt_get_file_ack *const get_file_ack,
                               const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_file_ack(get_file_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_File_Block)
    {
        extract_get_file_ack(get_file_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_file_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_File_Ack);
    }

    result = result && (smpt_is_valid_get_file_ack(get_file_ack));

    return result;
}

void extract_get_file_ack(Smpt_get_file_ack *const get_file_ack, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_file_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    get_file_ack->result = (Smpt_Result) buffer_unstuffed[index++];
    index = smpt_extract_uint32_t(&get_file_ack->number_of_blocks, buffer_unstuffed, index);

    index = smpt_extract_array_char((char*)get_file_ack->file_name, Smpt_Length_File_Name,
                                    buffer_unstuffed, index);
}


bool smpt_extract_get_device_id_ack(Smpt_get_device_id_ack *const get_device_id_ack,
                                    const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_device_id_ack(get_device_id_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_Device_Id_Ack)
    {
        extract_get_device_id_ack(get_device_id_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_device_id_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_Device_Id_Ack);
    }

    result = result && (smpt_is_valid_get_device_id_ack(get_device_id_ack));

    return result;
}

void extract_get_device_id_ack(Smpt_get_device_id_ack *const get_device_id_ack,
                               const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_device_id_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    get_device_id_ack->result = (Smpt_Result) buffer_unstuffed[index++];

    index = smpt_extract_array_char(get_device_id_ack->device_id, Smpt_Length_Device_Id,
                                    buffer_unstuffed, index);
}


bool smpt_extract_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack,
                                         const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_battery_status_ack(get_battery_status_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_Battery_Status_Ack)
    {
        extract_get_battery_status_ack(get_battery_status_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_device_id_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_Device_Id_Ack);
    }

    result = result && (smpt_is_valid_get_battery_status_ack(get_battery_status_ack));

    return result;
}

void extract_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack,
                                    const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_battery_status_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    get_battery_status_ack->result = (Smpt_Result) buffer_unstuffed[index++];

    get_battery_status_ack->battery_level = buffer_unstuffed[index++];
    index = smpt_extract_uint16_t(&get_battery_status_ack->battery_voltage, buffer_unstuffed, index);
}


bool smpt_extract_get_extended_version_ack(Smpt_get_extended_version_ack *const get_extended_version_ack,
                                            const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_extended_version_ack(get_extended_version_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_Extended_Version_Ack)
    {
        extract_get_extended_version_ack(get_extended_version_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_extended_version_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_Extended_Version_Ack);
    }

    result = result && (smpt_is_valid_get_extended_version_ack(get_extended_version_ack));

    return result;
}

void extract_get_extended_version_ack(Smpt_get_extended_version_ack *const get_extended_version_ack,
                             const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_extended_version_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    get_extended_version_ack->result = (Smpt_Result) buffer_unstuffed[index++];

    index = extract_version(&get_extended_version_ack->uc_version.fw_version,   buffer_unstuffed, index);
    index = extract_version(&get_extended_version_ack->uc_version.smpt_version, buffer_unstuffed, index);

    Smpt_uc_version tmp_version;

    index = extract_version(&tmp_version.fw_version,   buffer_unstuffed, index);
    index = extract_version(&tmp_version.smpt_version, buffer_unstuffed, index);

    get_extended_version_ack->fw_hash = tmp_version.fw_version.major << 24 | tmp_version.fw_version.minor << 16 | tmp_version.fw_version.revision << 8 | tmp_version.smpt_version.major;
    get_extended_version_ack->fw_hash_type = (Smpt_Fw_Hash_Type)tmp_version.smpt_version.minor & 0xff;
    get_extended_version_ack->fw_hash_state = (Smpt_Fw_Hash_State_Type)tmp_version.smpt_version.revision & 0xff;

}


bool smpt_extract_get_version_ack(Smpt_get_version_ack *const get_version_ack,
                                  const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_version_ack(get_version_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_Version_Ack)
    {
        extract_get_version_ack(get_version_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_version_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_Version_Ack);
    }

    result = result && (smpt_is_valid_get_version_ack(get_version_ack));

    return result;
}

void extract_get_version_ack(Smpt_get_version_ack *const get_version_ack,
                             const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_version_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    get_version_ack->result = (Smpt_Result) buffer_unstuffed[index++];

    index = extract_version(&get_version_ack->uc_version.fw_version,   buffer_unstuffed, index);
    index = extract_version(&get_version_ack->uc_version.smpt_version, buffer_unstuffed, index);
}

uint32_t extract_version(Smpt_version *const version, const uint8_t buffer_unstuffed[],
                         uint32_t index)
{
    version->major    = buffer_unstuffed[index++];
    version->minor    = buffer_unstuffed[index++];
    version->revision = buffer_unstuffed[index++];

    return index;
}


bool smpt_extract_get_stim_status_ack(Smpt_get_stim_status_ack *const get_stim_status_ack,
                                      const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_stim_status_ack(get_stim_status_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_Stim_Status_Ack)
    {
        extract_get_stim_status_ack(get_stim_status_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_stim_status_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_Stim_Status_Ack);
    }

    result = result && (smpt_is_valid_get_stim_status_ack(get_stim_status_ack));

    return result;
}

void extract_get_stim_status_ack(Smpt_get_stim_status_ack *const get_stim_status_ack,
                                 const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_stim_status_ack->packet_number      = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    get_stim_status_ack->result             = (Smpt_Result) buffer_unstuffed[index++];
    get_stim_status_ack->stim_status        = (Smpt_Stim_Status) buffer_unstuffed[index++];
    get_stim_status_ack->high_voltage_level = (Smpt_High_Voltage) buffer_unstuffed[index++];
}

bool smpt_extract_get_main_status_ack(Smpt_get_main_status_ack *const get_main_status_ack,
                                      const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_main_status_ack(get_main_status_ack);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_Main_Status_Ack)
    {
        extract_get_main_status_ack(get_main_status_ack, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_main_status_ack(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_Main_Status_Ack);
    }

    result = result && (smpt_is_valid_get_main_status_ack(get_main_status_ack));

    return result;
}

void extract_get_main_status_ack(Smpt_get_main_status_ack *const get_main_status_ack,
                                 const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_main_status_ack->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);
    get_main_status_ack->result        = (Smpt_Result) buffer_unstuffed[index++];
    get_main_status_ack->main_status   = (Smpt_Main_Status) buffer_unstuffed[index++];
}
