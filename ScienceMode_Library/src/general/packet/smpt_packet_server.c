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
#include "smpt_packet_server.h"

#include "smpt_packet_general.h"
#include "smpt_packet_internal.h"
#include "smpt_definitions_internal.h"
#include "smpt_packet_validity.h"
#include "smpt_messages.h"
#include "smpt_packet_utils.h"

static void extract_get_file(Smpt_get_file *const get_file,
                             const uint8_t buffer_unstuffed[]);
static void extract_remove_file(Smpt_remove_file *const remove_file,
                                const uint8_t buffer_unstuffed[]);
static void extract_set_power(Smpt_set_power *const set_power,
                              const uint8_t buffer_unstuffed[]);

static uint32_t insert_file_block(uint8_t buffer_unstuffed[],
                                  const Smpt_file_block *const file_block,
                                  uint32_t index);

static uint32_t insert_get_file_ack(uint8_t buffer_unstuffed[],
                                    const Smpt_get_file_ack *const get_file_ack,
                                    uint32_t index);

static uint32_t insert_get_device_id_ack(uint8_t buffer_unstuffed[],
                                         const Smpt_get_device_id_ack *const get_device_id_ack,
                                         uint32_t index);

static uint32_t insert_get_battery_status_ack(uint8_t buffer_unstuffed[],
                                              const Smpt_get_battery_status_ack * const get_battery_status_ack,
                                              uint32_t index);


static uint32_t insert_get_extended_version_ack(uint8_t buffer_unstuffed[],
                                const Smpt_get_extended_version_ack *const get_extended_version_ack,
                                uint32_t index);

static uint32_t insert_get_version_ack(uint8_t buffer_unstuffed[],
                                const Smpt_get_version_ack *const get_version_ack,
                                uint32_t index);

static uint32_t insert_version(uint8_t buffer_unstuffed[], const Smpt_version *const version,
                               uint32_t index);

static uint32_t insert_get_stim_status_ack(uint8_t buffer_unstuffed[],
                                           const Smpt_get_stim_status_ack *const get_stim_status_ack,
                                           uint32_t index);

static uint32_t insert_get_main_status_ack(uint8_t buffer_unstuffed[],
                                           const Smpt_get_main_status_ack *const get_main_status_ack,
                                           uint32_t index);


bool smpt_extract_get_file(Smpt_get_file *const get_file,
                              const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_get_file(get_file);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Get_File)
    {
        extract_get_file(get_file, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_get_file(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Get_File);
    }

    result = result && (smpt_is_valid_get_file(get_file));

    return result;
}

void extract_get_file(Smpt_get_file * const get_file, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    get_file->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    index = smpt_extract_array_char(get_file->file_name, Smpt_Length_File_Name,
                                    buffer_unstuffed, index);
}

uint32_t smpt_build_file_block(uint8_t buffer[], uint32_t buffer_length, Smpt_Cmd cmd,
                                  const Smpt_file_block * const file_block)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_file_block(buffer_unstuffed, file_block,
                                   Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd, file_block->packet_number);

    return packet_length;
}

uint32_t insert_file_block(uint8_t buffer_unstuffed[], const Smpt_file_block *const file_block,
                           uint32_t index)
{

    index = smpt_insert_uint16_t(buffer_unstuffed, &file_block->number_of_bytes, index);
    index = smpt_insert_uint32_t(buffer_unstuffed, &file_block->block_number, index);

    index = smpt_insert_array_char(buffer_unstuffed,
                                   (char*)file_block->data, file_block->number_of_bytes,
                                   index);

    return index;
}

uint32_t smpt_build_get_file_ack(uint8_t buffer[], uint32_t buffer_length, Smpt_Cmd cmd,
                                 const Smpt_get_file_ack *const get_file_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);

    last_index = insert_get_file_ack(buffer_unstuffed, get_file_ack,
                                     Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd, get_file_ack->packet_number);
    return packet_length;
}


uint32_t insert_get_file_ack(uint8_t buffer_unstuffed[],
                             const Smpt_get_file_ack *const get_file_ack,
                             uint32_t index)
{
    buffer_unstuffed[index++] = get_file_ack->result;
    index = smpt_insert_uint32_t(buffer_unstuffed, &get_file_ack->number_of_blocks, index);
    index = smpt_insert_array_char(buffer_unstuffed,
                                   get_file_ack->file_name, Smpt_Length_File_Name,
                                   index);

    return index;
}

bool smpt_extract_remove_file(Smpt_remove_file *const remove_file,
                              const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_remove_file(remove_file);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Remove_File)
    {
        extract_remove_file(remove_file, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_remove_file(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Remove_File);
    }

    result = result && (smpt_is_valid_remove_file(remove_file));

    return result;
}

void extract_remove_file(Smpt_remove_file *const remove_file, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    remove_file->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    index = smpt_extract_array_char(remove_file->file_name, Smpt_Length_File_Name,
                                    buffer_unstuffed, index);
}


bool smpt_extract_set_power(Smpt_set_power *const set_power,
                            const uint8_t packet[], uint32_t packet_length)
{
    bool result = false;
    uint32_t packet_unstuffed_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size_No_Emg];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size_No_Emg);
    smpt_clear_set_power(set_power);

    packet_unstuffed_length = smpt_unstuff_packet(buffer_unstuffed, packet, packet_length);

    if (packet_unstuffed_length >= Smpt_Min_Size_Set_Power)
    {
        extract_set_power(set_power, buffer_unstuffed);
        result = true;
    }
    else
    {
        smpt_error("smpt_extract_set_power(): Packet length too short (%d, expected: %d)",
                   packet_unstuffed_length, Smpt_Min_Size_Set_Power);
    }

    result = result && (smpt_is_valid_set_power(set_power));

    return result;
}

void extract_set_power(Smpt_set_power *const set_power, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    int b, i;

    set_power->packet_number = smpt_get_packet_number_unstuffed(buffer_unstuffed);

    for (b = 0; b < Smpt_Length_Power_Modules / 8; b++)
    {
        uint8_t temp = buffer_unstuffed[index++];

        for (i = 0; i < 8; i++)
        {
            set_power->modules[b * 8 + i] = (temp >> i) & 1;
        }
    }
}


uint32_t smpt_build_get_device_id_ack(uint8_t buffer[], uint32_t buffer_length,
                                      const Smpt_get_device_id_ack *const get_device_id_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_get_device_id_ack(buffer_unstuffed, get_device_id_ack,
                                                Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Get_Device_Id_Ack,
                                      get_device_id_ack->packet_number);
    return packet_length;
}

uint32_t insert_get_device_id_ack(uint8_t buffer_unstuffed[],
                                  const Smpt_get_device_id_ack *const get_device_id_ack,
                                  uint32_t index)
{
    buffer_unstuffed[index++] = get_device_id_ack->result;
    index = smpt_insert_array_char(buffer_unstuffed, get_device_id_ack->device_id,
                                   Smpt_Length_Device_Id, index);

    return index;
}

uint32_t smpt_build_get_battery_status_ack(uint8_t buffer[], uint32_t buffer_length,
                                           const Smpt_get_battery_status_ack *const get_battery_status_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_get_battery_status_ack(buffer_unstuffed, get_battery_status_ack,
                                                Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Get_Battery_Status_Ack,
                                      get_battery_status_ack->packet_number);
    return packet_length;
}

uint32_t insert_get_battery_status_ack(uint8_t buffer_unstuffed[],
                                const Smpt_get_battery_status_ack *const get_battery_status_ack,
                                uint32_t index)
{
    buffer_unstuffed[index++] = get_battery_status_ack->result;
    buffer_unstuffed[index++] = get_battery_status_ack->battery_level;
    index = smpt_insert_uint16_t(buffer_unstuffed, &get_battery_status_ack->battery_voltage, index);

    return index;
}


uint32_t smpt_build_get_extended_version_ack(uint8_t buffer[], uint32_t buffer_length,
                                    const Smpt_get_extended_version_ack *const get_extended_version_ack,
                                    Smpt_Cmd cmd)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_get_extended_version_ack(buffer_unstuffed, get_extended_version_ack,
                                        Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd, get_extended_version_ack->packet_number);

    return packet_length;
}

uint32_t insert_get_extended_version_ack(uint8_t buffer_unstuffed[],
                                const Smpt_get_extended_version_ack *const get_version_ack,
                                uint32_t index)
{
    buffer_unstuffed[index++] = get_version_ack->result;

    index = insert_version(buffer_unstuffed, &get_version_ack->uc_version.fw_version, index);
    index = insert_version(buffer_unstuffed, &get_version_ack->uc_version.smpt_version, index);
    Smpt_uc_version tmp_version;
    tmp_version.fw_version.major = (get_version_ack->fw_hash >> 24) & 0xff;
    tmp_version.fw_version.minor = (get_version_ack->fw_hash >> 16) & 0xff;
    tmp_version.fw_version.revision = (get_version_ack->fw_hash >> 8) & 0xff;
    tmp_version.smpt_version.major = (get_version_ack->fw_hash) & 0xff;
    tmp_version.smpt_version.minor = (get_version_ack->fw_hash_type) & 0xff;
    tmp_version.smpt_version.revision = (get_version_ack->fw_hash_state) & 0xff;

    index = insert_version(buffer_unstuffed, &tmp_version.fw_version, index);
    index = insert_version(buffer_unstuffed, &tmp_version.smpt_version, index);
    return index;
}

uint32_t smpt_build_get_version_ack(uint8_t buffer[], uint32_t buffer_length,
                                    const Smpt_get_version_ack *const get_version_ack,
                                    Smpt_Cmd cmd)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_get_version_ack(buffer_unstuffed, get_version_ack,
                                        Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      cmd, get_version_ack->packet_number);

    return packet_length;
}

uint32_t insert_get_version_ack(uint8_t buffer_unstuffed[],
                                const Smpt_get_version_ack *const get_version_ack,
                                uint32_t index)
{
    buffer_unstuffed[index++] = get_version_ack->result;

    index = insert_version(buffer_unstuffed, &get_version_ack->uc_version.fw_version, index);
    index = insert_version(buffer_unstuffed, &get_version_ack->uc_version.smpt_version, index);

    return index;
}

uint32_t insert_version(uint8_t buffer_unstuffed[], const Smpt_version *const version,
                        uint32_t index)
{
    buffer_unstuffed[index++] = version->major;
    buffer_unstuffed[index++] = version->minor;
    buffer_unstuffed[index++] = version->revision;

    return index;
}

uint32_t smpt_build_get_stim_status_ack(uint8_t buffer[], uint32_t buffer_length,
                                        const Smpt_get_stim_status_ack *const get_stim_status_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_get_stim_status_ack(buffer_unstuffed, get_stim_status_ack,
                                            Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Get_Stim_Status_Ack,
                                      get_stim_status_ack->packet_number);

    return packet_length;
}

uint32_t insert_get_stim_status_ack(uint8_t buffer_unstuffed[],
                                    const Smpt_get_stim_status_ack *const get_stim_status_ack,
                                    uint32_t index)
{
    buffer_unstuffed[index++] = get_stim_status_ack->result;
    buffer_unstuffed[index++] = get_stim_status_ack->stim_status;
    buffer_unstuffed[index++] = get_stim_status_ack->high_voltage_level;

    return index;
}

uint32_t smpt_build_get_main_status_ack(uint8_t buffer[], uint32_t buffer_length,
                                        const Smpt_get_main_status_ack *const get_main_status_ack)
{
    uint32_t last_index;
    uint32_t packet_length;

    uint8_t buffer_unstuffed[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_unstuffed, Smpt_Limit_Max_Packet_Size);

    last_index = insert_get_main_status_ack(buffer_unstuffed, get_main_status_ack,
                                            Smpt_Limit_Header_Size);
    packet_length = smpt_build_packet(buffer, buffer_length, buffer_unstuffed, last_index,
                                      Smpt_Cmd_Get_Main_Status_Ack,
                                      get_main_status_ack->packet_number);

    return packet_length;
}

uint32_t insert_get_main_status_ack(uint8_t buffer_unstuffed[],
                                    const Smpt_get_main_status_ack *const get_main_status_ack,
                                    uint32_t index)
{
    buffer_unstuffed[index++] = get_main_status_ack->result;
    buffer_unstuffed[index++] = get_main_status_ack->main_status;

    return index;
}
