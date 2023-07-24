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
#include "smpt_packet_validity.h"
#include "smpt_messages.h"

bool smpt_is_valid_packet_number(const uint8_t packet_number, const char* smpt_level)
{
    bool valid = true;
    if (packet_number > 63)
    {
        smpt_error("smpt_%s_is_valid_packet_number(): Invalid \"packet_number\": %d",
                   packet_number, smpt_level);
        valid = false;
    }
    return valid;
}

bool smpt_is_valid_cmd(const Smpt_cmd *const cmd)
{
    bool valid = true;

    if (cmd->command_number > Smpt_Cmd_Last_Item)
    {
        smpt_error("smpt_is_valid_cmd(): Invalid \"command_number\": %u",
                   cmd->command_number);
        valid = false;
    }

    else if (cmd->packet_number > 63)
    {
        smpt_error("smpt_is_valid_cmd(): Invalid \"packet_number\": %u",
                   cmd->packet_number);
        valid = false;
    }

    else { ; }

    return valid;
}


bool smpt_is_valid_ack(const Smpt_ack *const ack)
{
    bool valid = true;

    if (ack->command_number > Smpt_Cmd_Last_Item)
    {
        smpt_error("smpt_is_valid_ll_ack(): Invalid \"command_number\": %u",
                   ack->command_number);
        valid = false;
    }

    else if (ack->packet_number > 63)
    {
        smpt_error("smpt_is_valid_ll_ack(): Invalid \"packet_number\": %u",
                   ack->packet_number);
        valid = false;
    }

    else if (ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_ll_ack(): Invalid \"result\": %u",
                   ack->result);
        valid = false;
    }

    else { ; }

    return valid;
}

bool smpt_is_valid_file_block(const Smpt_file_block *const file_block)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(file_block->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_file(const Smpt_get_file *const get_file)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_file->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_remove_file(const Smpt_remove_file *const remove_file)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(remove_file->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_file_ack(const Smpt_get_file_ack *const get_file_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_file_ack->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_device_id_ack(const Smpt_get_device_id_ack *const get_device_id_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_device_id_ack->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_battery_status_ack(const Smpt_get_battery_status_ack *const get_battery_status_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_battery_status_ack->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_version_ack(const Smpt_get_version_ack * const get_version_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_version_ack->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_extended_version_ack(const Smpt_get_extended_version_ack * const get_extended_version_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_extended_version_ack->packet_number, "");

    if (get_extended_version_ack->fw_hash_type > Smpt_Fw_Hash_Type_Last_Item)
    {
        smpt_error("smpt_is_valid_get_extended_version_ack(): Invalid \"fw_hash_type\": %u",
                   get_extended_version_ack->fw_hash_type);
        valid = false;
    }
    return valid;
}


bool smpt_is_valid_set_power(const Smpt_set_power *const set_power)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(set_power->packet_number, "");

    /* No valid check for other parameters */
    return valid;
}

bool smpt_is_valid_get_stim_status_ack(const Smpt_get_stim_status_ack *const get_stim_status_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_stim_status_ack->packet_number, "");

    if (get_stim_status_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_get_stim_status_ack(): Invalid \"result\": %u",
                   get_stim_status_ack->result);
        valid = false;
    }

    else if (get_stim_status_ack->stim_status > Smpt_Stim_Status_Last_Item)
    {
        smpt_error("smpt_is_valid_get_stim_status_ack(): Invalid \"stim_status\": %u",
                   get_stim_status_ack->stim_status);
        valid = false;
    }

    else if (get_stim_status_ack->high_voltage_level > Smpt_High_Voltage_Last_Item)
    {
        smpt_error("smpt_is_valid_get_stim_status_ack(): Invalid \"high_voltage_level\": %u",
                   get_stim_status_ack->high_voltage_level);
        valid = false;
    }

    return valid;
}

bool smpt_is_valid_get_main_status_ack(const Smpt_get_main_status_ack *const get_main_status_ack)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(get_main_status_ack->packet_number, "");

    if (get_main_status_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_get_main_status_ack(): Invalid \"result\": %u",
                   get_main_status_ack->result);
        valid = false;
    }

    else if (get_main_status_ack->main_status > Smpt_Main_Status_Last_Item)
    {
        smpt_error("smpt_is_valid_get_main_status_ack(): Invalid \"main_status\": %u",
                   get_main_status_ack->main_status);
        valid = false;
    }

    return valid;
}
