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
#include "test_ll_channel_config_ack.h"
#include "test_ll_definitions.h"

static void fill_ll_channel_config_ack(Smpt_ll_channel_config_ack *const ll_channel_config_ack);
static bool compare_ll_channel_config_ack(const Smpt_ll_channel_config_ack
                                          *const ll_channel_config_ack_a,
                                          const Smpt_ll_channel_config_ack
                                          *const ll_channel_config_ack_b);

void test_ll_channel_config_ack(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_ll_channel_config_ack ll_channel_config_ack;
    Smpt_ll_channel_config_ack ll_channel_config_ack_out;
    smpt_clear_ll_channel_config_ack(&ll_channel_config_ack);
    smpt_clear_ll_channel_config_ack(&ll_channel_config_ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_ll_channel_config_ack(&ll_channel_config_ack);

    packet_length = smpt_build_ll_channel_config_ack(buffer, buffer_length, &ll_channel_config_ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Ll_Channel_Config_Ack);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_ll_channel_config_ack(&ll_channel_config_ack_out, buffer, packet_length));

    assert(compare_ll_channel_config_ack(&ll_channel_config_ack, &ll_channel_config_ack_out));
}

void fill_ll_channel_config_ack(Smpt_ll_channel_config_ack *const ll_channel_config_ack)
{
    ll_channel_config_ack->result = Smpt_Result_Successful;
    ll_channel_config_ack->packet_number = 16;
    ll_channel_config_ack->connector = 1;
    ll_channel_config_ack->channel = 1;
    ll_channel_config_ack->measurement_sampling_time_us = 1;
    ll_channel_config_ack->measurement_type = Smpt_Measurement_Stim_Voltage;
    for (uint8_t i=0; i<128; i++)
    {
        ll_channel_config_ack->measurement[i] = i;
    }
}

bool compare_ll_channel_config_ack(const Smpt_ll_channel_config_ack
                                          *const ll_channel_config_ack_a,
                                          const Smpt_ll_channel_config_ack
                                          *const ll_channel_config_ack_b)
{
    bool measurement_check = true;
    for (uint8_t i=0; i<128; i++)
    {
        measurement_check &= (ll_channel_config_ack_a->measurement[i] == ll_channel_config_ack_b->measurement[i]);
    }
    return (ll_channel_config_ack_a->result == ll_channel_config_ack_b->result) &&
           (ll_channel_config_ack_a->packet_number == ll_channel_config_ack_b->packet_number) &&
           (ll_channel_config_ack_a->channel ==
            ll_channel_config_ack_b->channel) &&
            (ll_channel_config_ack_a->connector ==
             ll_channel_config_ack_b->connector) &&
            (ll_channel_config_ack_a->measurement_type ==
             ll_channel_config_ack_b->measurement_type) &&
            (ll_channel_config_ack_a->measurement_sampling_time_us ==
             ll_channel_config_ack_b->measurement_sampling_time_us) &&
            measurement_check;
}
