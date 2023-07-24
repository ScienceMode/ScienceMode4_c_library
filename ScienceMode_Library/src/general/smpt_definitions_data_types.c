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
#include "smpt_definitions_data_types.h"

void smpt_clear_ack(Smpt_ack *const ack)
{
    Smpt_ack empty = {0};
    *ack = empty;
}

void smpt_clear_get_device_id_ack(Smpt_get_device_id_ack *const get_device_id_ack)
{
    Smpt_get_device_id_ack empty = {0};
    *get_device_id_ack = empty;
}

void smpt_clear_get_battery_status_ack(Smpt_get_battery_status_ack *const get_battery_status_ack)
{
    Smpt_get_battery_status_ack empty = {0};
    *get_battery_status_ack = empty;
}

void smpt_clear_get_version_ack(Smpt_get_version_ack *const get_version_ack)
{
    Smpt_get_version_ack empty = {0};
    *get_version_ack = empty;
}

void smpt_clear_get_extended_version_ack(Smpt_get_extended_version_ack *const get_extended_version_ack)
{
    Smpt_get_extended_version_ack empty = {0};
    *get_extended_version_ack = empty;
}

void smpt_clear_get_stim_status_ack(Smpt_get_stim_status_ack *const get_stim_status_ack)
{
    Smpt_get_stim_status_ack empty = {0};
    *get_stim_status_ack = empty;
}

void smpt_clear_get_main_status_ack(Smpt_get_main_status_ack *const get_main_status_ack)
{
    Smpt_get_main_status_ack empty = {0};
    *get_main_status_ack = empty;
}

void smpt_clear_cmd_list(Smpt_cmd_list *const cmd_list)
{
    Smpt_cmd_list empty = {0};
    *cmd_list = empty;
}

void smpt_clear_device(Smpt_device *const device)
{
    Smpt_device empty = {0};
    *device = empty;
}


bool smpt_is_general(Smpt_Cmd cmd)
{
    return ((uint32_t)(cmd) >= Smpt_Cmd_Numbers_General_Level_Min &&
            (uint32_t)(cmd) <= Smpt_Cmd_Numbers_General_Level_Max);
}

bool smpt_is_low_level(Smpt_Cmd cmd)
{
    return ((uint32_t)(cmd) <= Smpt_Cmd_Numbers_Low_Level_Max);
}

bool smpt_is_mid_level(Smpt_Cmd cmd)
{
    return ((uint32_t)(cmd) >= Smpt_Cmd_Numbers_Mid_Level_Min &&
            (uint32_t)(cmd) <= Smpt_Cmd_Numbers_Mid_Level_Max);
}

bool smpt_is_dyscom_level(Smpt_Cmd cmd)
{
    return ((uint32_t)(cmd) >= Smpt_Cmd_Numbers_DysCom_Level_Min &&
            (uint32_t)(cmd) <= Smpt_Cmd_Numbers_DysCom_Level_Max);
}


bool smpt_is_boot_level(Smpt_Cmd cmd)
{
    return ((uint32_t)(cmd) >= Smpt_Cmd_Numbers_Boot_Level_Min &&
            (uint32_t)(cmd) <= Smpt_Cmd_Numbers_Boot_Level_Max);
}

bool smpt_is_service_level(Smpt_Cmd cmd)
{
    return ((uint32_t)(cmd) >= Smpt_Cmd_Numbers_Service_Level_Min &&
            (uint32_t)(cmd) <= Smpt_Cmd_Numbers_Service_Level_Max);
}
