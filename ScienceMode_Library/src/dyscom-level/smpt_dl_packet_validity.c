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
#include "smpt_dl_packet_validity.h"
#include "smpt_packet_validity.h"
#include "smpt_messages.h"


static bool is_valid_ads129x(const Smpt_dl_ads129x *const ads129x);
static bool is_valid_tm(const Smpt_dl_tm *const tm);
static bool is_valid_battery_status(const Smpt_dl_battery_status *const battery_status);
static bool is_valid_file_system_status(const Smpt_dl_file_system_status *const file_system_status);
static bool is_valid_electrode_samples(const Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS], uint8_t n_samples);

bool smpt_is_valid_dl_init(const Smpt_dl_init *const dl_init)
{
    bool valid = true;
    valid &= is_valid_ads129x(&dl_init->ads129x);


    valid &= dl_init->system_time_flag ?
                         is_valid_tm(&dl_init->system_time) : 1;

    valid &= dl_init->timed_start_flag ?
                         is_valid_tm(&dl_init->planned_start_time) : 1;

    valid &= smpt_is_valid_packet_number(dl_init->packet_number, "dl");

    if (!valid)
        smpt_error("smpt_is_valid_dl_init(): error, see above for details");

    return valid;
}

bool smpt_is_valid_dl_init_ack(const Smpt_dl_init_ack *const dl_init_ack)
{
    bool valid = true;
    valid &= is_valid_ads129x(&dl_init_ack->ads129x);
    valid &= smpt_is_valid_packet_number(dl_init_ack->packet_number, "dl");
    if (!valid)
        smpt_error("smpt_is_valid_dl_init_ack(): error, see above for details");

    if (dl_init_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_init_ack(): wrong result code.");
        valid = false;
    }
    /*
    int dont_know__ = dl_init_ack->measurement_file_id;
    int dont_know___ = dl_init_ack->result;
    */
    return valid;
}

bool smpt_is_valid_dl_power_module_ack(const Smpt_dl_power_module_ack *const dl_power_module_ack)
{
    bool valid = true;
    valid &= smpt_is_valid_packet_number(dl_power_module_ack->packet_number, "dl");
    if (!valid)
        smpt_error("smpt_is_valid_dl_power_module_ack(): error, see above for details");

    if (dl_power_module_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_power_module_ack(): wrong result code.");
        valid = false;
    }
    if (dl_power_module_ack->hardware_module > Smpt_Dl_Hardware_Module_Last)
    {
        smpt_error("smpt_is_valid_dl_power_module_ack(): wrong hardware module code: %d",
                   dl_power_module_ack->hardware_module);
        valid = false;
    }

    return valid;
}

bool smpt_is_valid_dl_start_ack(const Smpt_dl_start_ack *const dl_start_ack)
{
    bool valid = true;
    valid &= smpt_is_valid_packet_number(dl_start_ack->packet_number, "dl");
    if (dl_start_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_start_ack(): wrong result code.");
        valid = false;
    }
    return valid;
}

bool smpt_is_valid_dl_stop_ack(const Smpt_dl_stop_ack *const dl_stop_ack)
{
    bool valid = true;
    valid &= smpt_is_valid_packet_number(dl_stop_ack->packet_number, "dl");
    if (dl_stop_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_stop_ack(): wrong result code.");
        valid = false;
    }
    /*
    dont check
    dl_stop_ack->packet_number
    dl_stop_ack->time
    */
    return valid;
}
bool smpt_is_valid_dl_get(const Smpt_dl_get *const dl_get)
{
    bool valid = true;
    valid &= smpt_is_valid_packet_number(dl_get->packet_number, "dl");
    if (dl_get->get_type > Smpt_Dl_Get_Type_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_get(): wrong get_type number: %d", dl_get->get_type);
        valid = false;
    }
    /*
     * Check for file system friendly string or not?
     *
     * dl_get->file_name
     *
     */
    return valid;
}

bool smpt_is_valid_dl_power_module(const Smpt_dl_power_module*const dl_power_module)
{
    bool valid = true;

    if(dl_power_module->hardware_module > Smpt_Dl_Hardware_Module_Last)
    {
        smpt_error("smpt_is_valid_dl_power_module(): wrong hardware module number: %d", dl_power_module->hardware_module);
        smpt_error("Allowed hardware module numbers: [%d - %d]",
                   Smpt_Dl_Hardware_Module_Undefined,
                   Smpt_Dl_Hardware_Module_Last);
        valid = false;
    }

    return valid;
}

bool smpt_is_valid_dl_get_ack(const Smpt_dl_get_ack *const dl_get_ack)
{
    bool valid = true;
    valid &= smpt_is_valid_packet_number(dl_get_ack->packet_number, "dl");
    if (!is_valid_battery_status(&dl_get_ack->battery_status))
    {
        smpt_error("smpt_is_valid_dl_get_ack(): wrong battery status.");
        valid = false;
    }
    if (!is_valid_file_system_status(&dl_get_ack->file_system_status))
    {
        smpt_error("smpt_is_valid_dl_get_ack(): file system status.");
        valid = false;
    }
    if (dl_get_ack->get_type > Smpt_Dl_Get_Type_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_get_ack(): wrong get_type number: %d", dl_get_ack->get_type);
        valid = false;
    }
    if (dl_get_ack->operation_mode > Smpt_Dl_Op_Mode_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_get_ack(): wrong operation mode.");
        valid = false;
    }
    if (dl_get_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_dl_get_ack(): wrong result code.");
        valid = false;
    }
    return valid;
}

bool smpt_is_valid_dl_send_file(const Smpt_dl_send_file *const dl_send_file)
{
    bool valid = true;
    if (dl_send_file->n_bytes_per_block > SMPT_DL_MAX_BLOCK_BYTES_LENGTH)
    {
        smpt_error("smpt_is_valid_dl_send_file(): "
                   "dl_send_file->n_bytes_per_block = %d "
                   "is too large. Maximum allowed is"
                   "SMPT_DL_MAX_BLOCK_BYTES_LENGTH = %d.",
                   dl_send_file->n_bytes_per_block,
                   SMPT_DL_MAX_BLOCK_BYTES_LENGTH);
        valid = false;
    }

    return valid;
}

bool smpt_is_valid_dl_send_live_data(const Smpt_dl_send_live_data *const live_data)
{
    bool valid = true;
    if (live_data->n_channels > SMPT_DL_MAX_CHANNELS)
    {
        smpt_error("smpt_is_valid_dl_send_live_data(): "
                   "live_data->n_channels = %d ist too large. "
                   "Maximum is Smpt_Length_N_Dl_Channels = %d",
                   live_data->n_channels,
                   SMPT_DL_MAX_CHANNELS);
        valid = false;
    }
    else
    {
        valid = is_valid_electrode_samples(live_data->electrode_samples,
                                 live_data->n_channels);
    }
    return valid;
}

bool is_valid_electrode_samples(const Smpt_dl_electrode_sample
                                electrode_samples[SMPT_DL_MAX_CHANNELS],
                                uint8_t n_samples)
{
    bool valid = true;
    int i = 0;
    while (i < n_samples)
    {
        if (electrode_samples[i].status > Smpt_Dl_Electrode_Status_Maximum_Valid)
        {
            smpt_error("is_valid_electrode_samples(): "
                       "electrode_samples[%d].status = %d"
                       "is an invalid status.",
                       i,
                       electrode_samples[i].status);
            valid = false;
        }
        if (electrode_samples[i].signal_type > Smpt_Dl_Signal_Type_Last_Item)
        {
            smpt_error("is_valid_electrode_samples(): "
                       "electrode_samples[%d].unit_type = %d"
                       "is an invalid unit type.",
                       i,
                       electrode_samples[i].signal_type);
            valid = false;
        }
        if ((electrode_samples[i].value >= SMPT_DL_MAX_SAMPLE_VALUE) || (electrode_samples[i].value <= (-1*SMPT_DL_MAX_SAMPLE_VALUE)))
        {
            valid  = false;
            smpt_error("is_valid_electrode_samples(): "
                       "electrode_samples[%d].value = %f"
                       "is out of bounds: [%d, %d]",
                       i,
                       electrode_samples[i].value,
                       SMPT_DL_MAX_SAMPLE_VALUE,
                       (-1*SMPT_DL_MAX_SAMPLE_VALUE));
        }
        i++;
    }
    return valid;
}

bool is_valid_ads129x(const Smpt_dl_ads129x *const ads129x)
{
    bool is_valid = true;
    int nothing = ads129x->ch1set;
    nothing++;
    /*
    int dont_know_what_to_check_here;
    */
    return is_valid;
}

bool is_valid_tm(const Smpt_dl_tm *const tm)
{
    bool is_valid = true;
    is_valid &= (tm->tm_hour <= 24);

    is_valid &= (tm->tm_mday <= 31) && (tm->tm_mday > 0);
    is_valid &= (tm->tm_min <= 59);
    is_valid &= (tm->tm_sec <= 59);
    is_valid &= (tm->tm_mon <= 12);
    is_valid &= (tm->tm_wday <= 7);
    is_valid &= (tm->tm_yday <= 365);
    if (!is_valid)
        smpt_error("smpt_is_valid_dl_init(): Invalid \"dl_tm struct\": %d, %d, %d, %d, %d, %d, %d, %d",
                   tm->tm_yday,
                   tm->tm_wday,
                   tm->tm_year,
                   tm->tm_mon,
                   tm->tm_mday,
                   tm->tm_hour,
                   tm->tm_min,
                   tm->tm_sec);
    return is_valid;
}

bool is_valid_battery_status(const Smpt_dl_battery_status *const battery_status)
{
    return  (battery_status->percentage <= 100);
}

bool is_valid_file_system_status(const Smpt_dl_file_system_status *const file_system_status)
{
    bool valid = true;
    int nothing = file_system_status->free_size;
    nothing++;
    /* file_system_status->  ??  maybe add sum of free_size + used_size ? */
    return valid;
}

bool smpt_is_valid_dl_send_mmi(const Smpt_dl_send_mmi *const dl_send_mmi)
{
    bool valid = true;
    int nothing = dl_send_mmi->dl_init.packet_number;
    nothing++;
    return valid;
}

bool smpt_is_valid_dl_sys(const Smpt_dl_sys * const dl_sys)
{
    bool valid = true;
    if (dl_sys->packet_number > 63)
    {
        valid = false;
    }

    return valid;
}

bool smpt_is_valid_dl_sys_ack(const Smpt_dl_sys_ack * const dl_sys_ack)
{
    bool valid = true;
    if (dl_sys_ack->packet_number > 63)
    {
        valid = false;
    }
    return valid;

}
