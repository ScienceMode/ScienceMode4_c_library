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
#include "smpt_dl_packet_utils.h"
#include "smpt_messages.h"
#include "smpt_packet_internal.h"
uint32_t smpt_dl_insert_ads129x(uint8_t buffer[], const Smpt_dl_ads129x *const dl_ads129x,
                               uint32_t index)
{
    buffer[index++] = dl_ads129x->ch1set;
    buffer[index++] = dl_ads129x->ch2set;
    buffer[index++] = dl_ads129x->ch3set;
    buffer[index++] = dl_ads129x->ch4set;
    buffer[index++] = dl_ads129x->ch5set;
    buffer[index++] = dl_ads129x->ch6set;
    buffer[index++] = dl_ads129x->ch7set;
    buffer[index++] = dl_ads129x->ch8set;
    buffer[index++] = dl_ads129x->config1;
    buffer[index++] = dl_ads129x->config2;
    buffer[index++] = dl_ads129x->config3;
    buffer[index++] = dl_ads129x->config4;
    buffer[index++] = dl_ads129x->gpio;
    buffer[index++] = dl_ads129x->id;
    buffer[index++] = dl_ads129x->loff;
    buffer[index++] = dl_ads129x->loff_flip;
    buffer[index++] = dl_ads129x->loff_sensn;
    buffer[index++] = dl_ads129x->loff_sensp;
    buffer[index++] = dl_ads129x->loff_statn;
    buffer[index++] = dl_ads129x->loff_statp;
    buffer[index++] = dl_ads129x->pace;
    buffer[index++] = dl_ads129x->resp;
    buffer[index++] = dl_ads129x->rld_sensn;
    buffer[index++] = dl_ads129x->rld_sensp;
    buffer[index++] = dl_ads129x->wct1;
    buffer[index++] = dl_ads129x->wct2;

    return index;
}

uint32_t smpt_dl_extract_ads129x(Smpt_dl_ads129x *const dl_ads129x, const uint8_t buffer[],
                               uint32_t index)
{
    dl_ads129x->ch1set      = buffer[index++];
    dl_ads129x->ch2set      = buffer[index++];
    dl_ads129x->ch3set      = buffer[index++];
    dl_ads129x->ch4set      = buffer[index++];
    dl_ads129x->ch5set      = buffer[index++];
    dl_ads129x->ch6set      = buffer[index++];
    dl_ads129x->ch7set      = buffer[index++];
    dl_ads129x->ch8set      = buffer[index++];
    dl_ads129x->config1     = buffer[index++];
    dl_ads129x->config2     = buffer[index++];
    dl_ads129x->config3     = buffer[index++];
    dl_ads129x->config4     = buffer[index++];
    dl_ads129x->gpio        = buffer[index++];
    dl_ads129x->id          = buffer[index++];
    dl_ads129x->loff        = buffer[index++];
    dl_ads129x->loff_flip   = buffer[index++];
    dl_ads129x->loff_sensn  = buffer[index++];
    dl_ads129x->loff_sensp  = buffer[index++];
    dl_ads129x->loff_statn  = buffer[index++];
    dl_ads129x->loff_statp  = buffer[index++];
    dl_ads129x->pace        = buffer[index++];
    dl_ads129x->resp        = buffer[index++];
    dl_ads129x->rld_sensn   = buffer[index++];
    dl_ads129x->rld_sensp   = buffer[index++];
    dl_ads129x->wct1        = buffer[index++];
    dl_ads129x->wct2        = buffer[index++];

    return index;
}

uint32_t smpt_dl_insert_tm(uint8_t buffer[], const Smpt_dl_tm *const dl_tm,
                               uint32_t index)
{
    uint16_t yday = dl_tm->tm_yday;
    uint16_t year = dl_tm->tm_year;
    buffer[index++] = dl_tm->tm_hour;
    buffer[index++] = dl_tm->tm_isdst;
    buffer[index++] = dl_tm->tm_mday;
    buffer[index++] = dl_tm->tm_min;
    buffer[index++] = dl_tm->tm_mon;
    buffer[index++] = dl_tm->tm_sec;
    buffer[index++] = dl_tm->tm_wday;
    index = smpt_insert_uint16_t(buffer, &yday, index);
    index = smpt_insert_uint16_t(buffer, &year, index);

    return index;
}

uint32_t smpt_dl_extract_tm(Smpt_dl_tm *const dl_tm, const uint8_t buffer[],
                               uint32_t index)
{
    uint16_t yday;
    uint16_t year;
    dl_tm->tm_hour  = buffer[index++];
    dl_tm->tm_isdst = buffer[index++];
    dl_tm->tm_mday  = buffer[index++];
    dl_tm->tm_min   = buffer[index++];
    dl_tm->tm_mon   = buffer[index++];
    dl_tm->tm_sec   = buffer[index++];
    dl_tm->tm_wday  = buffer[index++];
    index = smpt_extract_uint16_t(&yday, buffer, index);
    index = smpt_extract_uint16_t(&year, buffer, index);
    dl_tm->tm_yday = yday;
    dl_tm->tm_year = year;

    return index;
}

uint32_t smpt_dl_insert_init(uint8_t buffer_unstuffed[], const Smpt_dl_init *const dl_init,
                        uint32_t index)
{

    index = smpt_dl_insert_ads129x(buffer_unstuffed, &dl_init->ads129x, index);
    index = smpt_dl_insert_tm(buffer_unstuffed, &dl_init->planned_start_time, index);
    index = smpt_dl_insert_tm(buffer_unstuffed, &dl_init->system_time, index);
    index = smpt_dl_insert_patient_name(buffer_unstuffed, dl_init->patient_name, index);

    index = smpt_dl_insert_investigator_name(buffer_unstuffed, dl_init->investigator_name, index);
    index = smpt_dl_insert_patient_number(buffer_unstuffed, dl_init->patient_number, index);
    index = smpt_dl_insert_number_of_channels(buffer_unstuffed, &dl_init->number_of_channels, index);
    index = smpt_dl_insert_duration_of_planned_m(buffer_unstuffed, &dl_init->duration_of_planned_m, index);
    index = smpt_dl_insert_channel_type(buffer_unstuffed, dl_init->channel_type, SMPT_DL_MAX_CHANNELS, index);

    buffer_unstuffed[index++] = 0;
    buffer_unstuffed[index++] = ((uint8_t)dl_init->sync_signal);
    buffer_unstuffed[index++] = ((uint8_t)dl_init->filter);

    buffer_unstuffed[index] = 0;
    buffer_unstuffed[index] |= (dl_init->live_data_mode_flag    << 0);
    buffer_unstuffed[index] |= (dl_init->sd_storage_mode_flag   << 1);
    buffer_unstuffed[index] |= (dl_init->timed_start_flag       << 2);
    buffer_unstuffed[index] |= (dl_init->system_time_flag       << 3);
    buffer_unstuffed[index] |= (dl_init->mute_flag              << 4);

    index++;

    return index;
}


uint32_t smpt_dl_extract_init_(Smpt_dl_init *const dl_init, const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;

    index = smpt_dl_extract_ads129x(&dl_init->ads129x, buffer_unstuffed, index);
    index = smpt_dl_extract_tm(&dl_init->planned_start_time, buffer_unstuffed, index);
    index = smpt_dl_extract_tm(&dl_init->system_time, buffer_unstuffed, index);
    index = smpt_dl_extract_patient_name(dl_init->patient_name, buffer_unstuffed, index);

    index = smpt_dl_extract_investigator_name(dl_init->investigator_name, buffer_unstuffed, index);
    index = smpt_dl_extract_patient_number(dl_init->patient_number, buffer_unstuffed, index);
    index = smpt_dl_extract_number_of_channels(&dl_init->number_of_channels, buffer_unstuffed, index);
    index = smpt_dl_extract_duration_of_planned_m(&dl_init->duration_of_planned_m, buffer_unstuffed, index);
    index = smpt_dl_extract_channel_type(dl_init->channel_type, buffer_unstuffed, SMPT_DL_MAX_CHANNELS, index);

    index++;
    dl_init->sync_signal            = (Smpt_Dl_Init_Sync_Signal) buffer_unstuffed[index++];
    dl_init->filter                 = (Smpt_Dl_Dsp_Filter) buffer_unstuffed[index++];

    dl_init->live_data_mode_flag    = buffer_unstuffed[index] & 1;
    dl_init->sd_storage_mode_flag   = buffer_unstuffed[index] & (1 << 1);
    dl_init->timed_start_flag       = buffer_unstuffed[index] & (1 << 2);
    dl_init->system_time_flag       = buffer_unstuffed[index] & (1 << 3);
    dl_init->mute_flag              = buffer_unstuffed[index] & (1 << 4);

    index++;

    return index;
}

uint32_t smpt_dl_insert_string(uint8_t buffer[], const char * const string, uint32_t index, uint8_t str_length)
{
    int i = 0;
    int end = str_length;
    if (str_length > SMPT_DL_MAX_STRING_LENGTH+1)
    {
        smpt_error("smpt_dl_insert_string(...): str_length too large: %d:%d",
                   str_length, SMPT_DL_MAX_STRING_LENGTH+1);
        end = SMPT_DL_MAX_STRING_LENGTH + 1;
    }
    while(i < end)
    {
        buffer[index + i] = string[i];
        i++;
    }

    return index + i;
}

uint32_t smpt_dl_extract_string(char * const string,
                                const uint8_t buffer_unstuffed[],
                                uint32_t index, uint8_t str_length)
{
    int i = 0;
    int end = str_length;
    if (str_length > SMPT_DL_MAX_STRING_LENGTH+1)
    {
        smpt_error("smpt_dl_extract_string(...): str_length too large: %d:%d",
                   str_length, SMPT_DL_MAX_STRING_LENGTH+1);
        end = SMPT_DL_MAX_STRING_LENGTH+1;
    }
    while(i < end)
    {
        string[i] = buffer_unstuffed[index + i];
        i++;
    }

    return index + i;
}

uint32_t smpt_dl_insert_patient_name(uint8_t buffer[], const char* const patient_name,
                                     uint32_t index)
{
    return smpt_dl_insert_string(buffer, patient_name,
                                 index, SMPT_DL_MAX_PATIENT_NAME_LENGTH+1);
}

uint32_t smpt_dl_extract_patient_name(char* const patient_name, const uint8_t buffer_unstuffed[],
                                     uint32_t index)
{
    return smpt_dl_extract_string(patient_name, buffer_unstuffed,
                                  index, SMPT_DL_MAX_PATIENT_NAME_LENGTH+1);
}

uint32_t smpt_dl_insert_investigator_name(uint8_t buffer[], const char* const investigator_name,
                                     uint32_t index)
{
    return smpt_dl_insert_string(buffer, investigator_name,
                                 index, SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1);
}

uint32_t smpt_dl_extract_investigator_name(char* const investigator_name, const uint8_t buffer_unstuffed[],
                                     uint32_t index)
{
    return smpt_dl_extract_string(investigator_name, buffer_unstuffed,
                                  index, SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1);
}

uint32_t smpt_dl_insert_patient_number(uint8_t buffer[], const char* const patient_number,
                                     uint32_t index)
{
    return smpt_dl_insert_string(buffer, patient_number,
                                 index, SMPT_DL_GUID_STRING_LENGTH+1);
}

uint32_t smpt_dl_extract_patient_number(char* const patient_number, const uint8_t buffer_unstuffed[],
                                     uint32_t index)
{
    return smpt_dl_extract_string(patient_number, buffer_unstuffed,
                                  index, SMPT_DL_GUID_STRING_LENGTH+1);
}

uint32_t smpt_dl_insert_file_id(uint8_t buffer[],
                                const char file_id[SMPT_DL_MAX_FILE_ID_LENGTH],
                                uint32_t index)
{
    return smpt_dl_insert_string(buffer, file_id, index, SMPT_DL_MAX_FILE_ID_LENGTH);
}

uint32_t smpt_dl_extract_file_id(char file_id[SMPT_DL_MAX_FILE_ID_LENGTH],
                                 const uint8_t buffer_unstuffed[],
                                 uint32_t index)
{
    return smpt_dl_extract_string(file_id, buffer_unstuffed,
                                  index, SMPT_DL_MAX_FILE_ID_LENGTH);
}

uint32_t smpt_dl_insert_battery_status(uint8_t buffer[], const Smpt_dl_battery_status * const battery_status, uint32_t index)
{
    buffer[index++] = battery_status->energystate;
    buffer[index++] = battery_status->percentage;
    buffer[index++] = battery_status->temperature;
    index = smpt_insert_uint32_t(buffer, (uint32_t*)(&battery_status->current),index);
    index = smpt_insert_uint32_t(buffer, &battery_status->voltage, index);

    return index;
}

uint32_t smpt_dl_extract_battery_status(Smpt_dl_battery_status * const battery_status, const uint8_t buffer[], uint32_t index)
{
    battery_status->energystate = buffer[index++];
    battery_status->percentage = buffer[index++];
    battery_status->temperature = buffer[index++];
    index = smpt_extract_uint32_t((uint32_t*)(&battery_status->current), buffer, index);
    index = smpt_extract_uint32_t(&battery_status->voltage, buffer, index);

    return index;
}

uint32_t smpt_dl_insert_file_system_status(uint8_t buffer[], const Smpt_dl_file_system_status * const file_system_status, uint32_t index)
{
    /* if more flags are added bit shift logic has to be implemented! */
    buffer[index++] = file_system_status->file_system_ready;
    index = smpt_insert_uint64_t(buffer, &file_system_status->free_size, index);
    index = smpt_insert_uint64_t(buffer, &file_system_status->used_size, index);

    return index;
}

uint32_t smpt_dl_extract_file_system_status(Smpt_dl_file_system_status * const file_system_status, const uint8_t buffer[], uint32_t index)
{
    file_system_status->file_system_ready = buffer[index++] & 1;
    index = smpt_extract_uint64_t(&file_system_status->free_size, buffer, index);
    index = smpt_extract_uint64_t(&file_system_status->used_size, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_electrode_samples(uint8_t buffer[], const Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS], uint32_t index, uint8_t n_channels)
{
    int i = 0;
    while (i < n_channels)
    {
        index = smpt_insert_float(buffer, &(electrode_samples[i].value), index);
        buffer[index++] = electrode_samples[i].signal_type;
        buffer[index++] = electrode_samples[i].status;
        i++;
    }

    return index;
}


uint32_t smpt_dl_extract_electrode_samples(Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS], const uint8_t buffer[], uint32_t index, uint8_t n_channels)
{
    int i = 0;

    while (i < n_channels)
    {
        index = smpt_extract_float(&(electrode_samples[i].value), buffer, index);
        electrode_samples[i].signal_type = buffer[index++];
        electrode_samples[i].status = buffer[index++];
        i++;
    }

    return index;
}


uint32_t smpt_dl_insert_file_sizes(uint8_t buffer[], const uint64_t *size, const uint32_t *n_blocks, uint32_t index)
{
    index = smpt_insert_uint64_t(buffer, size, index);
    index = smpt_insert_uint32_t(buffer, n_blocks, index);

    return index;
}


uint32_t smpt_dl_extract_file_sizes(uint64_t *size, uint32_t *n_blocks, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint64_t(size, buffer, index);
    index = smpt_extract_uint32_t(n_blocks, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_file_size(uint8_t buffer[], const uint64_t *size, uint32_t index)
{
    index = smpt_insert_uint64_t(buffer, size, index);

    return index;
}


uint32_t smpt_dl_extract_file_size(uint64_t *size, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint64_t(size, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_measurement_number(uint8_t buffer[], const uint16_t * const measurement_number, uint32_t index)
{
    index = smpt_insert_uint16_t(buffer, measurement_number, index);

    return index;
}


uint32_t smpt_dl_extract_measurement_number(uint16_t *const measurement_number, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint16_t(measurement_number, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_time_length(uint8_t buffer[], const uint32_t *length, uint32_t index)
{
    index = smpt_insert_uint32_t(buffer, length, index);

    return index;
}


uint32_t smpt_dl_extract_time_length(uint32_t *length, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint32_t(length, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_number_of_channels(uint8_t buffer[], const uint16_t *n_channels, uint32_t index)
{
    index = smpt_insert_uint16_t(buffer, n_channels, index);

    return index;
}


uint32_t smpt_dl_extract_number_of_channels(uint16_t *n_channels, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint16_t(n_channels, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_duration_of_planned_m(uint8_t buffer[], const uint32_t *duration, uint32_t index)
{
    index = smpt_insert_uint32_t(buffer, duration, index);

    return index;
}


uint32_t smpt_dl_extract_duration_of_planned_m(uint32_t *duration, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint32_t(duration, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_block_offset(uint8_t buffer[], const uint32_t *block_offset, uint32_t index)
{
    index = smpt_insert_uint32_t(buffer, block_offset, index);

    return index;
}


uint32_t smpt_dl_extract_block_offset(uint32_t *block_offset, const uint8_t buffer[], uint32_t index)
{
    index = smpt_extract_uint32_t(block_offset, buffer, index);

    return index;
}


uint32_t smpt_dl_insert_power_module(uint8_t buffer_unstuffed[], const Smpt_dl_power_module * const dl_power_module, uint32_t index)
{

    buffer_unstuffed[index++] = ((uint8_t)dl_power_module->hardware_module);
    buffer_unstuffed[index++] = dl_power_module->switch_on_off;

    return index;
}


uint32_t smpt_dl_extract_power_module(Smpt_dl_power_module *const dl_power_module,
                                      const uint8_t buffer_unstuffed[])
{
    uint32_t index = Smpt_Limit_Header_Size;
    dl_power_module->hardware_module = (Smpt_Dl_Hardware_Module)(buffer_unstuffed[index++]);
    dl_power_module->switch_on_off   = buffer_unstuffed[index++];

    return index;
}

uint32_t smpt_dl_insert_file_by_name(uint8_t buffer[], const Smpt_dl_file_by_name * const file_by_name, uint32_t index)
{
    index = smpt_dl_insert_file_name(buffer, file_by_name->file_name, index);
    index = smpt_insert_uint32_t(buffer, &file_by_name->block_offset, index);
    index = smpt_insert_uint64_t(buffer, &file_by_name->file_size, index);
    index = smpt_insert_uint32_t(buffer, &file_by_name->n_blocks, index);

    buffer[index++] = file_by_name->mode;

    return index;
}

uint32_t smpt_dl_extract_file_by_name(Smpt_dl_file_by_name * const file_by_name, const uint8_t buffer[], uint32_t index)
{
    index = smpt_dl_extract_file_name(file_by_name->file_name, buffer, index);
    index = smpt_extract_uint32_t(&file_by_name->block_offset, buffer, index);
    index = smpt_extract_uint64_t(&file_by_name->file_size, buffer, index);
    index = smpt_extract_uint32_t(&file_by_name->n_blocks, buffer, index);

    file_by_name->mode = buffer[index++];

    return index;
}

uint32_t smpt_dl_insert_file_name(uint8_t buffer[],
                                const char file_name[SMPT_DL_MAX_FILE_NAME_LENGTH],
                                uint32_t index)
{
    return smpt_dl_insert_string(buffer, file_name, index, SMPT_DL_MAX_FILE_NAME_LENGTH);
}

uint32_t smpt_dl_extract_file_name(char file_name[SMPT_DL_MAX_FILE_NAME_LENGTH],
                                 const uint8_t buffer_unstuffed[],
                                 uint32_t index)
{
    return smpt_dl_extract_string(file_name, buffer_unstuffed,
                                  index, SMPT_DL_MAX_FILE_NAME_LENGTH);
}

uint32_t smpt_dl_insert_send_file_ack(uint8_t buffer_unstuffed[],
                                      const Smpt_dl_send_file_ack * const dl_send_file_ack,
                                      uint32_t index)
{
    return smpt_insert_uint32_t(buffer_unstuffed, &dl_send_file_ack->block_number, index);
}
uint32_t smpt_dl_extract_send_file_ack(Smpt_dl_send_file_ack * const dl_send_file_ack,
                                      const uint8_t buffer[], uint32_t index)
{
    return smpt_extract_uint32_t(&dl_send_file_ack->block_number, buffer, index);
}

uint32_t smpt_dl_insert_file_info(uint8_t buffer[], const Smpt_dl_file_info * const dl_file_info, uint32_t index)
{
    index = smpt_dl_insert_file_name(buffer, dl_file_info->file_name, index);
    index = smpt_insert_uint32_t(buffer, &dl_file_info->file_size, index);
    index = smpt_insert_uint16_t(buffer, &dl_file_info->file_checksum, index);

    return index;
}

uint32_t smpt_dl_extract_file_info(Smpt_dl_file_info * const dl_file_by_info, const uint8_t buffer[], uint32_t index)
{
    index = smpt_dl_extract_file_name(dl_file_by_info->file_name, buffer,index);
    index = smpt_extract_uint32_t(&dl_file_by_info->file_size, buffer, index);
    index = smpt_extract_uint16_t(&dl_file_by_info->file_checksum, buffer, index);

    return index;
}

uint32_t smpt_dl_insert_sys(uint8_t buffer_unstuffed[], const Smpt_dl_sys * const dl_sys, uint32_t index)
{

    index = smpt_dl_insert_file_name(buffer_unstuffed, dl_sys->file_name, index);
    buffer_unstuffed[index++] = (uint8_t)dl_sys->type;

    return index;
}

uint32_t smpt_dl_extract_sys(Smpt_dl_sys * const dl_sys, const uint8_t buffer[], uint32_t index)
{
    index = smpt_dl_extract_file_name(dl_sys->file_name, buffer, index);
    dl_sys->type = buffer[index++];

    return index;
}

uint32_t smpt_dl_insert_sys_ack(uint8_t buffer_unstuffed[], const Smpt_dl_sys_ack * const dl_sys_ack, uint32_t index)
{

    buffer_unstuffed[index++] = (uint8_t)dl_sys_ack->type;
    buffer_unstuffed[index++] = (uint8_t)dl_sys_ack->state;
    index     = smpt_dl_insert_file_name(buffer_unstuffed, dl_sys_ack->file_name, index);

    return index;
}

uint32_t smpt_dl_extract_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack,
                                   const uint8_t buffer[], uint32_t index)
{
   dl_sys_ack->type     =   buffer[index++];
   dl_sys_ack->state    =   buffer[index++];
   index = smpt_dl_extract_file_name(dl_sys_ack->file_name, buffer, index);

   return index;
}

uint32_t smpt_dl_insert_channel_type(uint8_t buffer_unstuffed[], const Smpt_Dl_Signal_Type dl_channel_type[], uint32_t dl_channel_type_length, uint32_t index)
{
    uint32_t i = 0;
    for(; i < dl_channel_type_length; i++)
    {
        buffer_unstuffed[index++] = (uint8_t)dl_channel_type[i];
    }
    return  index;
}

uint32_t smpt_dl_extract_channel_type(Smpt_Dl_Signal_Type dl_channel_type[], const uint8_t buffer_unstuffed[], uint32_t dl_channel_type_length, uint32_t index)
{
    uint32_t i = 0;
    for(; i < dl_channel_type_length; i++)
    {
        dl_channel_type[i] = (Smpt_Dl_Signal_Type)buffer_unstuffed[index++];
    }
    return  index;
}
