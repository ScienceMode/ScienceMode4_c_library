/***************************************************************************//**
* \file smpt_ml_packet_validity.c
* \copyright MPL and LGPL HASOMED GmbH 2013-2022
*
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

#include "smpt_ml_packet_validity.h"
#include "smpt_messages.h"
#include "smpt_ll_packet_validity.h"
#include "smpt_packet_validity.h"
#include <math.h>

static bool is_valid_channel(const Smpt_ml_channel_config *const ml_channel_config, uint8_t i);

static bool check_parameters(const Smpt_ml_update *const ml_update);
static float get_duration_ms(const Smpt_ml_channel_config *const ch_config);

bool smpt_is_valid_ml_init(const Smpt_ml_init *const ml_init)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(ml_init->packet_number, "ml");

    return valid;
}


bool smpt_is_valid_ml_update(const Smpt_ml_update *const ml_update)
{
    bool valid = true;
    uint8_t i;

    valid &= smpt_is_valid_packet_number(ml_update->packet_number, "ml");


    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {

        if (ml_update->enable_channel[i])
        {
            valid = valid && is_valid_channel(&ml_update->channel_config[i], i);
        }
    }

    valid = valid && check_parameters(ml_update);

    return valid;
}

bool is_valid_channel(const Smpt_ml_channel_config *const ml_channel_config, uint8_t i)
{
    bool valid = true;
    uint8_t j;

    if (ml_channel_config->number_of_points > 16)
    {
        smpt_error("smpt_is_valid_ml_update(): Invalid \"number_of_points\"(%d): %d",
                   i, ml_channel_config->number_of_points);
        valid = false;
    }

    if (ml_channel_config->ramp > 15)
    {
        smpt_error("smpt_is_valid_ml_update(): Invalid \"ramp\"(%d): %d",
                   i, ml_channel_config->ramp);
        valid = false;
    }

    if (ml_channel_config->period <= 0 ||
        ml_channel_config->period > 16383)
    {
        smpt_error("smpt_is_valid_ml_update(): Invalid \"period\"(%d): %f",
                   i, ml_channel_config->period);
        valid = false;
    }

    for (j = 0; j < ml_channel_config->number_of_points; j++)
    {
        Smpt_ll_point point = ml_channel_config->points[j];
        valid = valid && (smpt_is_valid_point(&point, j));
    }

    return valid;
}

bool check_parameters(const Smpt_ml_update *const ml_update)
{
    bool result = true;
    uint8_t ml_index;
    float tges_ms = 0.0;
    uint32_t duration_ms = 0;
    const uint16_t ML_DURATION_1S_LIMIT = 1000; /* allowed duration/ms (incl. extra time) during 1s */

    for (ml_index = 0; ml_index < Smpt_Length_Number_Of_Channels; ml_index++)
    {
        if (ml_update->enable_channel[ml_index])
        {
            tges_ms += get_duration_ms(&ml_update->channel_config[ml_index]);
        }
    }

    duration_ms = (uint32_t) ceil(tges_ms);     /* round up */
    if (duration_ms >= ML_DURATION_1S_LIMIT)
    {
        result = false;
    }

    return result;
}

float get_duration_ms(const Smpt_ml_channel_config *const ch_config)
{
    uint32_t t_us = 0;
    //const uint16_t ML_DURATION_MUTE_US = 200;    /* Mute prestart time µs */
    //const uint16_t ML_DURATION_TEST_US = 30;     /* biphasic testpulse time/µs */
    //const uint16_t ML_DURATION_RESERVE_US = 100; /* extra time as reserve/µs */

    const uint16_t ML_DURATION_MUTE_US = 40;    /* Mute prestart time µs */
    const uint16_t ML_DURATION_TEST_US = 0;     /* biphasic testpulse time/µs */
    const uint16_t ML_DURATION_RESERVE_US =20; /* extra time as reserve/µs */

    float tg_ms;

    uint8_t i;
    for (i = 0; i < Smpt_Length_Points; i++)
    {
        t_us += ch_config->points[i].time;
    }
    /* t_us += tMute + tTest + tReserve; */
    t_us += ML_DURATION_MUTE_US + ML_DURATION_TEST_US + ML_DURATION_RESERVE_US;

    /* tg_ms = t_us / period_ms; */
    tg_ms = t_us / ch_config->period;

    return tg_ms;
}

bool smpt_is_valid_ml_get_current_data(const Smpt_ml_get_current_data *const ml_get_current_data)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(ml_get_current_data->packet_number, "ml");

    if (!valid)
        smpt_error("smpt_is_valid_ml_get_current_data(): error, see above for details");

    return valid;
}


bool smpt_is_valid_ml_get_current_data_ack(const Smpt_ml_get_current_data_ack
                                           *const ml_get_current_data_ack)
{
    bool valid = true;

    /* general */
    if (ml_get_current_data_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_get_current_data_ack(): Invalid \"result\": %u",
                   ml_get_current_data_ack->result);
        valid = false;
    }

    /* optional */

    if (ml_get_current_data_ack->data_selection == Smpt_Ml_Data_Stimulation && ml_get_current_data_ack->stimulation_data.stimulation_state > Smpt_Ml_Stimulation_Last_Item)
    {
        smpt_error("smpt_is_valid_get_current_data_ack(): Invalid \"stimulation_state\": %u",
                   ml_get_current_data_ack->stimulation_data.stimulation_state);
        valid = false;
    }

    valid &= smpt_is_valid_packet_number(ml_get_current_data_ack->packet_number, "ml");

    return valid;
}
