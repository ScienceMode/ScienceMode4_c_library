/***************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2022, MPL and LGPL HASOMED GmbH
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
#include "test_dl_init.h"
#include "test_dl_common.h"

#include "string.h"

void test_dl_init()
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_init dl_init;
    Smpt_dl_init dl_init_out;
    smpt_clear_dl_init(&dl_init);
    smpt_clear_dl_init(&dl_init_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_init(&dl_init);

    packet_length = smpt_build_dl_init(buffer_, buffer_length_, &dl_init);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Init);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_init(&dl_init_out, buffer_, packet_length));
    assert(compare_dl_init(&dl_init, &dl_init_out));
}

void test_dl_init_ack(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_init_ack dl_init_ack;
    Smpt_dl_init_ack dl_init_ack_out;
    smpt_clear_dl_init_ack(&dl_init_ack);
    smpt_clear_dl_init_ack(&dl_init_ack_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_init_ack(&dl_init_ack);

    packet_length = smpt_build_dl_init_ack(buffer_, buffer_length_, &dl_init_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Init_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_init_ack(&dl_init_ack_out, buffer_, packet_length));
    assert(compare_dl_init_ack(&dl_init_ack, &dl_init_ack_out));
}

void fill_dl_tm(Smpt_dl_tm *const tm);

bool compare_dl_stop_ack(const Smpt_dl_stop_ack *const a,
                            const Smpt_dl_stop_ack *const b)
{
    return ((a->packet_number   == b->packet_number) &&
            (a->result          == b->result) &&
            (a->time            == b->time));
}

void fill_dl_init(Smpt_dl_init *const dl_init)
{
    fill_dl_ads129x(&dl_init->ads129x);
    fill_dl_tm(&dl_init->system_time);
    fill_dl_tm(&dl_init->planned_start_time);

    strcpy(dl_init->patient_name,
            "Hans-Dieter Romeo Peter Julius Schlagmichtot");
    strcpy(dl_init->patient_number, "21EC2020-3AEA-4069-A2DD-08002B30309D");

    dl_init->number_of_channels     = 2;
    dl_init->live_data_mode_flag    = 1;
    dl_init->packet_number          = 42;
    dl_init->sd_storage_mode_flag   = 1;
    dl_init->system_time_flag       = 1;
    dl_init->timed_start_flag       = 1;
    dl_init->mute_flag              = 0;
    dl_init->sync_signal            = Smpt_Dl_Init_Sync_Signal_Rect_Pulse_0_200mV;

    dl_init->channel_type[0] = Smpt_Dl_Signal_Type_BI;
    dl_init->channel_type[1] = Smpt_Dl_Signal_Type_EMG1;
    dl_init->channel_type[2] = Smpt_Dl_Signal_Type_Unused;
    dl_init->channel_type[3] = Smpt_Dl_Signal_Type_Unused;


    dl_init->filter = Smpt_Dl_Dsp_Filter_003;
}

void fill_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack)
{
    dl_init_ack->packet_number = 42;
    fill_dl_ads129x(&dl_init_ack->ads129x);
    strcpy(dl_init_ack->measurement_file_id,
            "xyzasdfg");
    dl_init_ack->result = 0;
    dl_init_ack->init_state = Smpt_Dl_Init_Ack_State_Error_Storage_Full;
    dl_init_ack->freq_out = Smpt_Dl_Output_Sample_Freq_32kSPS;
}

void fill_dl_ads129x (Smpt_dl_ads129x *const ads129x)
{
    ads129x->ch1set = 1;
    ads129x->ch2set = 2;
    ads129x->ch3set = 3;
    ads129x->ch4set = 4;
    ads129x->ch5set = 5;
    ads129x->ch6set = 6;
    ads129x->ch7set = 7;
    ads129x->ch8set = 8;
    ads129x->config1 = 9;
    ads129x->config2 = 10;
    ads129x->config3 = 11;
    ads129x->config4 = 12;
    ads129x->gpio = 13;
    ads129x->id = 14;
    ads129x->loff = 15;
    ads129x->loff_flip = 16;
    ads129x->loff_sensn = 17;
    ads129x->loff_sensp = 18;
    ads129x->loff_statn = 19;
    ads129x->loff_statp = 20;
    ads129x->pace = 21;
    ads129x->resp = 22;
    ads129x->rld_sensn = 23;
    ads129x->rld_sensp = 24;
    ads129x->wct1 = 25;
    ads129x->wct2 = 26;
}

void fill_dl_tm(Smpt_dl_tm *const tm)
{
    tm->tm_hour = 12;
    tm->tm_mday = 1;
    tm->tm_min = 59;
    tm->tm_sec = 29;
    tm->tm_mon = 6;
    tm->tm_year = 142;
    tm->tm_yday = 33;
    tm->tm_isdst = 0xFF;
    tm->tm_wday = 0;
}

bool compare_dl_init(const Smpt_dl_init *const a,
                     const Smpt_dl_init *const b)
{
    return  compare_dl_ads129x(&a->ads129x, &b->ads129x) &&
            compare_dl_tm(&a->system_time, &b->system_time) &&
            compare_dl_tm(&a->planned_start_time, &b->planned_start_time) &&
            compare_dl_patient_name(a->patient_name, b->patient_name) &&
            compare_dl_patient_number(a->patient_number, b->patient_number) &&
            compare_dl_investigator_name(a->investigator_name, b->investigator_name) &&
            compare_dl_channel_type((char*)a->channel_type, (char*)b->channel_type)&&

            (a->duration_of_planned_m   == b->duration_of_planned_m) &&
            (a->number_of_channels      == b->number_of_channels) &&
            (a->live_data_mode_flag     == b->live_data_mode_flag) &&
            (a->sd_storage_mode_flag    == b->sd_storage_mode_flag) &&
            (a->timed_start_flag        == b->timed_start_flag) &&
            (a->system_time_flag        == b->system_time_flag) &&
            (a->mute_flag               == b->mute_flag) &&
            (a->sync_signal             == b->sync_signal);
}

bool compare_dl_ads129x(const Smpt_dl_ads129x *const a,
                        const Smpt_dl_ads129x *const b)
{
    return  (a->id           == b->id)        && /* 1 */
            (a->config1      == b->config1)   && /* 2 */
            (a->config2      == b->config2)   && /* 3 */
            (a->config3      == b->config3)   && /* 4 */
            (a->loff         == b->loff)      && /* 5 */
            (a->ch1set       == b->ch1set)    && /* 6 */
            (a->ch2set       == b->ch2set)    && /* 7 */
            (a->ch3set       == b->ch3set)    && /* 8 */
            (a->ch4set       == b->ch4set)    && /* 9 */
            (a->ch5set       == b->ch5set)    && /* 10*/
            (a->ch6set       == b->ch6set)    && /* 11 */
            (a->ch7set       == b->ch7set)    && /* 12 */
            (a->ch8set       == b->ch8set)    && /* 13 */
            (a->rld_sensp    == b->rld_sensp) && /* 14 */
            (a->rld_sensn    == b->rld_sensn) && /* 15 */
            (a->loff_sensp   == b->loff_sensp)&& /* 16 */
            (a->loff_sensn   == b->loff_sensn)&& /* 17 */
            (a->loff_flip    == b->loff_flip) && /* 18 */
            (a->loff_statp   == b->loff_statp)&& /* 19 */
            (a->loff_statn   == b->loff_statn)&& /* 20 */
            (a->gpio         == b->gpio)      && /* 21 */
            (a->pace         == b->pace)      && /* 22 */
            (a->resp         == b->resp)      && /* 23 */
            (a->config4      == b->config4)   && /* 24 */
            (a->wct1         == b->wct1)      && /* 25 */
            (a->wct2         == b->wct2);        /* 27 */
}

bool compare_dl_tm(const Smpt_dl_tm *const dl_tm_a,
                   const Smpt_dl_tm *const dl_tm_b)
{
    return  (dl_tm_a->tm_sec == dl_tm_b->tm_sec) &&
            (dl_tm_a->tm_min == dl_tm_b->tm_min) &&
            (dl_tm_a->tm_hour == dl_tm_b->tm_hour) &&
            (dl_tm_a->tm_mday == dl_tm_b->tm_mday) &&
            (dl_tm_a->tm_mon == dl_tm_b->tm_mon) &&
            (dl_tm_a->tm_year == dl_tm_b->tm_year) &&
            (dl_tm_a->tm_wday == dl_tm_b->tm_wday) &&
            (dl_tm_a->tm_yday == dl_tm_b->tm_yday) &&
            (dl_tm_a->tm_isdst == dl_tm_b->tm_isdst);
}
bool compare_dl_patient_name(const char a[SMPT_DL_MAX_PATIENT_NAME_LENGTH + 1],
                             const char b[SMPT_DL_MAX_PATIENT_NAME_LENGTH + 1])
{
    return (strncmp(a, b, SMPT_DL_MAX_PATIENT_NAME_LENGTH) == 0);
}

bool compare_dl_investigator_name(const char a[SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1],
                                  const char b[SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1])
{
    return (strncmp(a, b, SMPT_DL_MAX_PATIENT_NAME_LENGTH) == 0);
}

bool compare_dl_patient_number (const char a[SMPT_DL_GUID_STRING_LENGTH+1],
                                const char b[SMPT_DL_GUID_STRING_LENGTH+1])
{
    return (strncmp(a, b, SMPT_DL_GUID_STRING_LENGTH) == 0);
}

bool compare_dl_init_ack(const Smpt_dl_init_ack *const a,
                     const Smpt_dl_init_ack *const b)
{
    return  (compare_dl_ads129x(&a->ads129x, &b->ads129x)) &&
            (compare_dl_file_id(a->measurement_file_id, b->measurement_file_id)) &&
            (a->packet_number   == b->packet_number) &&
            (a->result          == b->result) && (a->init_state == b->init_state);
}

bool compare_dl_file_id(const char a[SMPT_DL_MAX_FILE_ID_LENGTH],
                        const char b[SMPT_DL_MAX_FILE_ID_LENGTH])
{
    bool result = true;
    uint8_t i = 0;
    while ((i < SMPT_DL_MAX_FILE_ID_LENGTH) && result)
    {
        if (a[i] != b[i])
            result = false;
        i++;
    }
    return result;
}

bool compare_dl_channel_type (const char a[SMPT_DL_MAX_CHANNELS],
                              const char b[SMPT_DL_MAX_CHANNELS])
{
    bool result = true;
    uint8_t i = 0;
    while ((i < SMPT_DL_MAX_CHANNELS) && result)
    {
        if (a[i] != b[i])
            result = false;
        i++;
    }
    return result;
}
