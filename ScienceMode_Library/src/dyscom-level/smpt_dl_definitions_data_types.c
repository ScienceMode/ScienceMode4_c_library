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
#include "smpt_dl_definitions_data_types.h"
#include "smpt_packet_general.h"

void smpt_clear_dl_init(Smpt_dl_init *const dl_init)
{
    int i = 0;
    smpt_clear_dl_ads129x(&dl_init->ads129x);
    smpt_clear_dl_patient_name(dl_init->patient_name);
    smpt_clear_dl_investigator_name(dl_init->investigator_name);
    smpt_clear_dl_patient_number(dl_init->patient_number);
    smpt_clear_dl_tm(&dl_init->system_time);
    smpt_clear_dl_tm(&dl_init->planned_start_time);
    /*smpt_clear_buffer((uint8_t*)(&dl_init->channel_type), SMPT_DL_MAX_CHANNELS);*/
    /*smpt_clear_buffer((uint8_t*)(&dl_init->channel_type), 32);*/

    while(i < SMPT_DL_MAX_CHANNELS)
    {
        dl_init->channel_type[i] = 0;
        i++;
    }

    dl_init->duration_of_planned_m  = 0;
    dl_init->number_of_channels     = 0;
    dl_init->packet_number          = 0;
    dl_init->live_data_mode_flag    = false;
    dl_init->sd_storage_mode_flag   = false;
    dl_init->system_time_flag       = false;
    dl_init->timed_start_flag       = false;
    dl_init->mute_flag              = false;
    dl_init->sync_signal            = Smpt_Dl_Init_Sync_Signal_Undifined;

    dl_init->filter = Smpt_Dl_Dsp_Filter_off;
}

void smpt_clear_dl_tm(Smpt_dl_tm *const dl_tm)
{
    dl_tm->tm_hour  = 0;
    dl_tm->tm_isdst = 0;
    dl_tm->tm_mday  = 0;
    dl_tm->tm_min   = 0;
    dl_tm->tm_mon   = 0;
    dl_tm->tm_sec   = 0;
    dl_tm->tm_wday  = 0;
    dl_tm->tm_yday  = 0;
    dl_tm->tm_year  = 0;
}

int smpt_dl_compare_int(int a, int b)
{
    int res;
    if (a < b)
    {
        res = -1;
    }
    else if (a > b)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    return res;
}

int smpt_dl_compare_tm(const Smpt_dl_tm *a, const Smpt_dl_tm *b)
{
    int res;
    if ((res = smpt_dl_compare_int(a->tm_year, b->tm_year)) == 0)
    {
    if ((res = smpt_dl_compare_int(a->tm_mon, b->tm_mon)) == 0)
    {
    if ((res = smpt_dl_compare_int(a->tm_mday, b->tm_mday)) == 0)
    {
    if ((res = smpt_dl_compare_int(a->tm_hour, b->tm_hour)) == 0)
    {
    if ((res = smpt_dl_compare_int(a->tm_min, b->tm_min)) == 0)
    {
    if ((res = smpt_dl_compare_int(a->tm_sec, b->tm_sec)) == 0)
    {}}}}}};
    /*
    if (res = smpt_dl_compare_int(a->tm_yday, b->tm_yday) == 0)
    if (res = smpt_dl_compare_int(a->tm_wday, b->tm_wday) == 0)
    if (res = smpt_dl_compare_int(a->tm_isdst, b->tm_isdst) == 0);
    */
    return res;
}

bool smpt_dl_tm_is_null(Smpt_dl_tm* tm)
{
    return ((tm->tm_hour == 0) &&
            (tm->tm_isdst == 0) &&
            (tm->tm_mday == 0) &&
            (tm->tm_min == 0) &&
            (tm->tm_mon == 0) &&
            (tm->tm_sec == 0) &&
            (tm->tm_wday == 0) &&
            (tm->tm_yday == 0) &&
            (tm->tm_year == 0));
}

void smpt_clear_dl_ads129x(Smpt_dl_ads129x *const dl_ads129x)
{
    dl_ads129x->ch1set = 0;
    dl_ads129x->ch2set = 0;
    dl_ads129x->ch3set = 0;
    dl_ads129x->ch4set = 0;
    dl_ads129x->ch5set = 0;
    dl_ads129x->ch6set = 0;
    dl_ads129x->ch7set = 0;
    dl_ads129x->ch8set = 0;
    dl_ads129x->config1 = 0;
    dl_ads129x->config2 = 0;
    dl_ads129x->config3 = 0;
    dl_ads129x->config4 = 0;
    dl_ads129x->gpio    = 0;
    dl_ads129x->id      = 0;
    dl_ads129x->loff    = 0;
    dl_ads129x->loff_flip = 0;
    dl_ads129x->loff_sensn  = 0;
    dl_ads129x->loff_sensp  = 0;
    dl_ads129x->loff_statn  = 0;
    dl_ads129x->loff_statp  = 0;
    dl_ads129x->pace        = 0;
    dl_ads129x->resp        = 0;
    dl_ads129x->rld_sensn   = 0;
    dl_ads129x->rld_sensp   = 0;
    dl_ads129x->wct1        = 0;
    dl_ads129x->wct2        = 0;
}

void smpt_clear_dl_init_ack(Smpt_dl_init_ack * const dl_init_ack)
{
    smpt_clear_dl_ads129x(&dl_init_ack->ads129x);
    dl_init_ack->packet_number = 0;
    dl_init_ack->result = 0;
    smpt_clear_dl_string(dl_init_ack->measurement_file_id, SMPT_DL_MAX_FILE_ID_LENGTH);
    dl_init_ack->init_state = 0;
    dl_init_ack->freq_out = 0;
}

void smpt_clear_dl_string(char * const string, uint16_t str_length)
{
    int i = 0;
    while (i < str_length)
    {
        string[i++] = 0;
    }
}

void smpt_clear_dl_patient_name(char * const patient_name)
{
    smpt_clear_dl_string(patient_name, SMPT_DL_MAX_PATIENT_NAME_LENGTH+1);
}

void smpt_clear_dl_investigator_name(char * const investigator_name)
{
    smpt_clear_dl_string(investigator_name, SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1);
}

void smpt_clear_dl_patient_number(char * const patient_number)
{
    smpt_clear_dl_string(patient_number, SMPT_DL_GUID_STRING_LENGTH+1);
}

void smpt_clear_dl_stop_ack(Smpt_dl_stop_ack * const dl_stop_ack)
{
    dl_stop_ack->time = 0;
    dl_stop_ack->result = 0;
    dl_stop_ack->packet_number = 0;
}

void smpt_clear_dl_start_ack(Smpt_dl_start_ack * const dl_start_ack)
{
    dl_start_ack->packet_number = 0;
    dl_start_ack->result = 0;
}

void smpt_clear_dl_get(Smpt_dl_get * const dl_get)
{
    dl_get->get_type            = 0;
    dl_get->packet_number       = 0;
    dl_get->file_by_name.block_offset   = 0;
    dl_get->file_by_name.file_size      = 0;
    dl_get->file_by_name.mode          = 0;
    dl_get->file_by_name.n_blocks       = 0;

    smpt_clear_dl_string(dl_get->file_by_name.file_name, SMPT_DL_MAX_FILE_NAME_LENGTH+1);

    dl_get->mmi.n_measurements = 0;
}

void smpt_clear_dl_get_ack(Smpt_dl_get_ack * const dl_get_ack)
{
    int i = 0;
    dl_get_ack->battery_status.percentage   = 0;
    dl_get_ack->battery_status.voltage      = 0;
    dl_get_ack->battery_status.current      = 0;
    dl_get_ack->battery_status.temperature  = 0;

    dl_get_ack->file_system_status.file_system_ready = 0;
    dl_get_ack->file_system_status.free_size = 0;
    dl_get_ack->file_system_status.used_size = 0;

    dl_get_ack->get_type        = 0;
    dl_get_ack->operation_mode  = 0;
    dl_get_ack->packet_number   = 0;
    dl_get_ack->result          = 0;

    dl_get_ack->file_by_name.block_offset   = 0;
    smpt_clear_dl_string(dl_get_ack->file_by_name.file_name, SMPT_DL_MAX_FILE_NAME_LENGTH+1);
    dl_get_ack->file_by_name.file_size      = 0;
    dl_get_ack->file_by_name.mode           = 0;
    dl_get_ack->file_by_name.n_blocks       = 0;

    dl_get_ack->mmi.n_measurements = 0;

    smpt_clear_dl_string(dl_get_ack->file_info.file_name, SMPT_DL_MAX_FILE_NAME_LENGTH+1);
    dl_get_ack->file_info.file_size = 0;
    dl_get_ack->file_info.file_checksum = 0;


    for (; i < (SMPT_DL_MAX_STRING_LENGTH + 1); i++)
    {
        dl_get_ack->deviceId[i] = 0;
        dl_get_ack->firmwareVersion[i] = 0;
    }
}


void smpt_clear_dl_send_file(Smpt_dl_send_file * const dl_send_file)
{
    int i = 0;
    dl_send_file->packet_number = 0;
    dl_send_file->block_number = 0;
    dl_send_file->n_bytes_per_block = 0;
    while (i < SMPT_DL_MAX_BLOCK_BYTES_LENGTH)
    {
        dl_send_file->data[i++] = 0;
    }
}


void smpt_clear_dl_send_live_data(Smpt_dl_send_live_data * const dl_send_live_data)
{
    dl_send_live_data->packet_number    = 0;
    dl_send_live_data->n_channels       = 0;
    dl_send_live_data->time_offset      = 0;
    smpt_clear_dl_electrode_samples(dl_send_live_data->electrode_samples);
}


void smpt_clear_dl_electrode_samples(Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS])
{
    int i = 0;
    while (i < SMPT_DL_MAX_CHANNELS)
    {
        electrode_samples[i].value          = 0;
        electrode_samples[i].signal_type    = 0;
        electrode_samples[i].status         = 0;
        i++;
    }
}


void smpt_copy_dl_ads129x(Smpt_dl_ads129x * const target, const Smpt_dl_ads129x * const source)
{
    target->ch1set      = source->ch1set;
    target->ch2set      = source->ch2set;
    target->ch3set      = source->ch3set;
    target->ch4set      = source->ch4set;
    target->ch5set      = source->ch5set;
    target->ch6set      = source->ch6set;
    target->ch7set      = source->ch7set;
    target->ch8set      = source->ch8set;
    target->config1     = source->config1;
    target->config2     = source->config2;
    target->config3     = source->config3;
    target->config4     = source->config4;
    target->gpio        = source->gpio;
    target->id          = source->id;
    target->loff        = source->loff;
    target->loff_flip   = source->loff_flip;
    target->loff_sensn  = source->loff_sensn;
    target->loff_sensp  = source->loff_sensp;
    target->loff_statn  = source->loff_statn;
    target->loff_statp  = source->loff_statp;
    target->rld_sensn   = source->rld_sensn;
    target->rld_sensp   = source->rld_sensp;
    target->pace        = source->pace;
    target->resp        = source->resp;
    target->wct1        = source->wct1;
    target->wct2        = source->wct2;
}


void smpt_clear_dl_mmi(Smpt_dl_mmi * const dl_mmi)
{
    smpt_clear_dl_measurement_id(dl_mmi->measurement_id);
    dl_mmi->measurement_number = 0;
    dl_mmi->file_size = 0;
    smpt_clear_dl_patient_number(dl_mmi->patient_number);
    smpt_clear_dl_tm(&dl_mmi->start_time);
    smpt_clear_dl_tm(&dl_mmi->planned_start_time);
    dl_mmi->time_length = 0;
}

void smpt_clear_dl_send_mmi(Smpt_dl_send_mmi * const dl_send_mmi)
{
    dl_send_mmi->packet_number = 0;
    smpt_clear_dl_mmi(&dl_send_mmi->mmi);
    smpt_clear_dl_init(&dl_send_mmi->dl_init);
}

void smpt_clear_dl_measurement_id(char file_id[SMPT_DL_MAX_FILE_ID_LENGTH+1])
{
    smpt_clear_char_array(file_id, SMPT_DL_MAX_FILE_ID_LENGTH+1);
}


void smpt_clear_dl_power_module(Smpt_dl_power_module *dl_power_module)
{
    dl_power_module->packet_number      = 0;
    dl_power_module->hardware_module    = Smpt_Dl_Hardware_Module_Undefined;
    dl_power_module->switch_on_off      = 0;
}


void smpt_clear_dl_power_module_ack(Smpt_dl_power_module_ack *dl_power_module_ack)
{
    dl_power_module_ack->packet_number  = 0;
    dl_power_module_ack->result         = 0;
    dl_power_module_ack->hardware_module= 0;
    dl_power_module_ack->switch_on_off  = 0;
}

void smpt_clear_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack)
{
    dl_send_file_ack->block_number = 0;
    dl_send_file_ack->packet_number = 0;
    dl_send_file_ack->result = 0;
}

void smpt_clear_dl_sys(Smpt_dl_sys *const dl_sys)
{
    dl_sys->packet_number           = 0;
    dl_sys->result                  = 0;
    smpt_clear_dl_string(dl_sys->file_name, SMPT_DL_MAX_FILE_NAME_LENGTH+1);
    dl_sys->type                    = 0;
}

void smpt_clear_dl_sys_ack(Smpt_dl_sys_ack * const dl_sys_ack)
{
    dl_sys_ack->packet_number           = 0;
    dl_sys_ack->result                  = 0;
    smpt_clear_dl_string(dl_sys_ack->file_name, SMPT_DL_MAX_FILE_NAME_LENGTH+1);
    dl_sys_ack->state                   = 0;
    dl_sys_ack->type                    = 0;
}
