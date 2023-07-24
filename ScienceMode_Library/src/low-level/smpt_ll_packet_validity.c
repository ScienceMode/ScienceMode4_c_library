/***************************************************************************//**
* \file smpt_ll_packet_validity.c
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

#include "smpt_ll_packet_validity.h"
#include "smpt_packet_validity.h"
#include "smpt_messages.h"

bool smpt_is_valid_ll_init(const Smpt_ll_init *const ll_init)
{
    bool valid = true;

    valid &= smpt_is_valid_packet_number(ll_init->packet_number, "ll");

    if (ll_init->high_voltage_level > Smpt_High_Voltage_150V)
    {
        smpt_error("smpt_is_valid_ll_init(): Invalid \"high_voltage_level\": %d",
                   ll_init->high_voltage_level);
        valid = false;
    }

    else { ; }

    /* The emg_registers have no restrictions */    
    return valid;
}


bool smpt_is_valid_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack)
{
    bool valid = true;

    if (ll_init_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_ll_init_ack(): Invalid \"result\": %d",
                   ll_init_ack->result);
        valid = false;
    }

    return valid;
}


bool smpt_is_valid_ll_channel_config(const Smpt_ll_channel_config *const ll_channel_config)
{
    bool valid = true;
    uint8_t i;

    if (ll_channel_config->packet_number > 63)
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"packet_number\": %u",
                   ll_channel_config->packet_number);
        valid = false;
    }

    else if (ll_channel_config->channel > 3)
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"channel\": %u",
                   ll_channel_config->channel);
        valid = false;
    }
    else if (ll_channel_config->connector > 1)
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"connector\": %u",
                   ll_channel_config->connector);
        valid = false;
    }
    else if (ll_channel_config->number_of_points > 16)
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"number_of_points\": %u",
                   ll_channel_config->number_of_points);
        valid = false;
    }

    else { ; }

    for (i = 0; i < Smpt_Length_Points; i++)
    {
        Smpt_ll_point point = ll_channel_config->points[i];
        valid = valid && (smpt_is_valid_point(&point, i));
    }

    return valid;
}

bool smpt_is_valid_point(const Smpt_ll_point *const ll_point, uint8_t index)
{
    bool valid = true;

    if (ll_point->time > 4095)
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"time\": %d (index: %u)",
                   ll_point->time, index);
        valid = false;
    }

    if ((ll_point->current > 130.f) || (ll_point->current < -130.f))
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"current\": %04.1f (index: %u)",
                   ll_point->current, index);
        valid = false;
    }

    else if (ll_point->interpolation_mode > Smpt_Ll_Interpolation_Last_Item)
    {
        smpt_error("smpt_is_valid_ll_channel_config(): Invalid \"interpolation_mode\": %u (index: %u)",
                   ll_point->interpolation_mode, index);
        valid = false;
    }

    else { ; }

    return valid;
}


bool smpt_is_valid_ll_channel_config_ack(const Smpt_ll_channel_config_ack
                                         *const ll_channel_config_ack)
{
    bool valid = true;

    if (ll_channel_config_ack->packet_number > 63)
    {
        smpt_error("smpt_is_valid_ll_channel_config_ack(): Invalid \"packet_number\": %u",
                   ll_channel_config_ack->packet_number);
        valid = false;
    }

    else if (ll_channel_config_ack->result > Smpt_Result_Last_Item)
    {
        smpt_error("smpt_is_valid_ll_channel_config_ack(): Invalid \"result\": %u",
                   ll_channel_config_ack->result);
        valid = false;
    }
    else if (ll_channel_config_ack->connector > 1)
    {
        smpt_error("smpt_is_valid_ll_channel_config_ack(): Invalid \"connector\": %u",
                   ll_channel_config_ack->connector);
        valid = false;
    }

    else if (ll_channel_config_ack->channel > 3)
    {
        smpt_error("smpt_is_valid_ll_channel_config_ack(): Invalid \"channel\": %u",
                   ll_channel_config_ack->channel);
        valid = false;
    }

    else { ; }

    /* no restrictions for electrode_error_channel and timestamps */

    return valid;
}

