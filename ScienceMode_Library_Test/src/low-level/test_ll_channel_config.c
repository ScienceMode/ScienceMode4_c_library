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
#include "test_ll_channel_config.h"
#include "test_ll_definitions.h"

static void test_ll_channel_config_internal(void (*fill_function_ptr)(Smpt_ll_channel_config *const));
static void fill_ll_channel_config_complete(Smpt_ll_channel_config *const ll_channel_config);
static void fill_ll_channel_config_minimal(Smpt_ll_channel_config *const ll_channel_config);

static bool compare_ll_channel_config(const Smpt_ll_channel_config *const ll_channel_config_a,
                                      const Smpt_ll_channel_config *const ll_channel_config_b);

void test_ll_channel_config(void)
{
    test_ll_channel_config_internal(&fill_ll_channel_config_complete);
    test_ll_channel_config_internal(&fill_ll_channel_config_minimal);

}

void test_ll_channel_config_internal(void (*fill_function_ptr)(Smpt_ll_channel_config *const))
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_ll_channel_config ll_channel_config;
    Smpt_ll_channel_config ll_channel_config_out;
    smpt_clear_ll_channel_config(&ll_channel_config);
    smpt_clear_ll_channel_config(&ll_channel_config_out);

    smpt_clear_buffer(buffer, buffer_length);
    (*fill_function_ptr)(&ll_channel_config);

    packet_length = smpt_build_ll_channel_config(buffer, buffer_length, &ll_channel_config);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Ll_Channel_Config);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_ll_channel_config(&ll_channel_config_out, buffer, packet_length));
    assert(compare_ll_channel_config(&ll_channel_config, &ll_channel_config_out));
}

void fill_ll_channel_config_complete(Smpt_ll_channel_config *const ll_channel_config)
{
    uint32_t i;
    uint8_t number_of_points;

    ll_channel_config->packet_number = 46;

    ll_channel_config->enable_stimulation = 1;
    ll_channel_config->channel = 2;
    ll_channel_config->connector = 1;
    ll_channel_config->number_of_points = 16;

    number_of_points = ll_channel_config->number_of_points;
    for (i = 0; i < number_of_points; i++)
    {
        ll_channel_config->points[i].time = (uint16_t)(i * 2);
        ll_channel_config->points[i].current = (float)(i * -2.5);
        ll_channel_config->points[i].interpolation_mode = Smpt_Ll_Interpolation_Linear;
    }

}

void fill_ll_channel_config_minimal(Smpt_ll_channel_config *const ll_channel_config)
{
    ll_channel_config->packet_number = 24;

    ll_channel_config->enable_stimulation = 0;
    ll_channel_config->channel = 3;
    ll_channel_config->connector = 1;
    ll_channel_config->number_of_points = 16;
}

bool compare_ll_channel_config(const Smpt_ll_channel_config *const ll_channel_config_a,
                               const Smpt_ll_channel_config *const ll_channel_config_b)
{
    bool result = true;

    int i;
    for (i = 0; i < Smpt_Length_Points; i++)
    {
        result = result &&
                 ((ll_channel_config_a->points[i].time ==
                   ll_channel_config_b->points[i].time) &&
                  (ll_channel_config_a->points[i].current ==
                   ll_channel_config_b->points[i].current) &&
                  (ll_channel_config_a->points[i].interpolation_mode ==
                   ll_channel_config_b->points[i].interpolation_mode));
    }

    return result &&
           (ll_channel_config_a->packet_number      == ll_channel_config_b->packet_number) &&
           (ll_channel_config_a->enable_stimulation == ll_channel_config_b->enable_stimulation) &&
           (ll_channel_config_a->channel            == ll_channel_config_b->channel) &&
           (ll_channel_config_a->connector            == ll_channel_config_b->connector) &&
           (ll_channel_config_a->number_of_points   == ll_channel_config_b->number_of_points);
}
