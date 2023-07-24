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
#include "test_dl_get.h"
#include "test_dl_common.h"

#include "string.h"

void test_dl_get(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_get dl_get;
    Smpt_dl_get dl_get_out;
    smpt_clear_dl_get(&dl_get);
    smpt_clear_dl_get(&dl_get_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_get(&dl_get);

    packet_length = smpt_build_dl_get(buffer_, buffer_length_, &dl_get);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Get);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_get(&dl_get_out, buffer_, packet_length));
    assert(compare_dl_get(&dl_get, &dl_get_out));
}


void fill_dl_get(Smpt_dl_get * const dl_get)
{
    char file_name[] = "ThisIs-my_file_name.xyz"; /* 23 + \0 */
    strcpy(dl_get->file_by_name.file_name, file_name);
    dl_get->get_type = Smpt_Dl_Get_Type_File_By_Name;
    dl_get->packet_number   = 42;
    dl_get->file_by_name.block_offset    = 0;
    strcpy(dl_get->file_info.file_name, file_name);
}

bool compare_dl_get(const Smpt_dl_get *const a,
                    const Smpt_dl_get *const b)
{
    bool valid = true;

    if (a->packet_number != b->packet_number)
    {
        valid = false;
    }
    switch(a->get_type)
    {
        case Smpt_Dl_Get_Type_File_By_Name:
            if (strncmp(a->file_by_name.file_name, b->file_by_name.file_name,
                        SMPT_DL_MAX_FILE_NAME_LENGTH) != 0)
            {
                valid = false;
            }
            break;
        case Smpt_Dl_Get_Type_File_Info:
            if ((strncmp(a->file_info.file_name, b->file_info.file_name,
                        SMPT_DL_MAX_FILE_NAME_LENGTH) != 0) || (a->file_info.file_name[0] == 0))
            {
                valid = false;
            }
        break;
        default: valid = true;
    }

    if (a->get_type != b->get_type)
    {
        valid = false;
    }

    if (a->file_by_name.block_offset != b->file_by_name.block_offset)
    {
        valid = false;
    }
    return valid;
}

void test_dl_get_ack(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_get_ack dl_get_ack;
    Smpt_dl_get_ack dl_get_ack_out;
    smpt_clear_dl_get_ack(&dl_get_ack);
    smpt_clear_dl_get_ack(&dl_get_ack_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_get_ack(&dl_get_ack);

    packet_length = smpt_build_dl_get_ack(buffer_, buffer_length_, &dl_get_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Get_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_get_ack(&dl_get_ack_out, buffer_, packet_length));
    assert(compare_dl_get_ack(&dl_get_ack, &dl_get_ack_out));
}


void fill_dl_get_ack(Smpt_dl_get_ack * const dl_get_ack)
{
    char deviceId[] = "1.0.42";
    char firmwareVersion[] = "0.58";

    dl_get_ack->battery_status.percentage = 33;
    dl_get_ack->battery_status.voltage = 3000;
    dl_get_ack->battery_status.current = 3867;
    dl_get_ack->battery_status.temperature = -12;

    /* 2^40
    dl_get_ack->file_size = 1024*1024*1024*1024;
    dl_get_ack->file_size = 1099511627776;
    dl_get_ack->file_size = 1099511627776ULL;
    */
    dl_get_ack->file_by_name.file_size = 4294967295U;
    dl_get_ack->file_system_status.file_system_ready = 1;

    dl_get_ack->file_system_status.free_size = 4294967295U;
    dl_get_ack->file_system_status.used_size = 4294967295U;

    dl_get_ack->get_type = Smpt_Dl_Get_Type_File_By_Name;

    dl_get_ack->file_by_name.n_blocks = 99;

    dl_get_ack->operation_mode = Smpt_Dl_Op_Mode_Live_Measurement;

    dl_get_ack->result = 0;

    dl_get_ack->packet_number = 42;

    memcpy(dl_get_ack->deviceId, deviceId, sizeof(deviceId));
    memcpy(dl_get_ack->firmwareVersion, deviceId, sizeof(firmwareVersion));

}

bool compare_dl_get_ack(const Smpt_dl_get_ack *const a,
                        const Smpt_dl_get_ack *const b)
{
    bool valid = true;

    if ((a->packet_number != b->packet_number)||
        (a->result != b->result) ||
        (a->get_type != b->get_type))
    {
        valid = false;
    }
    else {
        switch (a->get_type)
        {
            case Smpt_Dl_Get_Type_Battery_Status:
            {
                if ((a->battery_status.percentage != b->battery_status.percentage)  ||
                    (a->battery_status.voltage != b->battery_status.voltage)        ||
                    (a->battery_status.current != b->battery_status.current)        ||
                    (a->battery_status.temperature != b->battery_status.temperature))
                {
                    valid = false;
                }
            }
            break;
            case Smpt_Dl_Get_Type_File_By_Name:
            {
                if ((a->file_by_name.file_size != b->file_by_name.file_size) ||
                    (a->file_by_name.n_blocks != b->file_by_name.n_blocks))
                    valid = false;
            }
            break;
            case Smpt_Dl_Get_Type_File_System_Status:
            {
                if ((a->file_system_status.file_system_ready != b->file_system_status.file_system_ready) ||
                    (a->file_system_status.free_size != b->file_system_status.free_size) ||
                    (a->file_system_status.used_size != b->file_system_status.used_size))
                {
                    valid = false;
                }
            }
            break;
            case Smpt_Dl_Get_Type_List_of_MMI:
                valid = (a->mmi.n_measurements == b->mmi.n_measurements);
            break;
            case Smpt_Dl_Get_Type_Operation_Mode:
                valid = (a->operation_mode == b->operation_mode);
            break;
            case Smpt_Dl_Get_Type_Device_Id:
                valid = smpt_compare_string(a->deviceId, b->deviceId, SMPT_DL_MAX_STRING_LENGTH);
            break;
            case Smpt_Dl_Get_Type_Firmware_Version:
                valid = smpt_compare_string(a->firmwareVersion, b->firmwareVersion, SMPT_DL_MAX_STRING_LENGTH);
            break;
            case Smpt_Dl_Get_Type_File_Info:
            {
                valid = smpt_compare_string(a->file_info.file_name, b->file_info.file_name, SMPT_DL_MAX_STRING_LENGTH);
                if ((a->file_info.file_checksum) != (b->file_info.file_checksum) ||
                    (a->file_info.file_size) != (b->file_info.file_size))
                {
                    valid = false;
                }
            }
        }
    }

    return valid;
}

bool smpt_compare_string(const char *a, const char *b, uint8_t len)
{
    uint8_t i = 0;
    while(i++ < len)
    {
        if(a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}
