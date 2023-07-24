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
#include "test_dl_send_live_data.h"
#include "test_dl_common.h"

#include <stdlib.h>
void test_dl_send_live_data(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_send_live_data dl_send_live_data;
    Smpt_dl_send_live_data dl_send_live_data_out;


    smpt_clear_dl_send_live_data(&dl_send_live_data);
    smpt_clear_dl_send_live_data(&dl_send_live_data_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_send_live_data(&dl_send_live_data);

    packet_length = smpt_build_dl_send_live_data(buffer_, buffer_length_, &dl_send_live_data);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Send_Live_Data);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_send_live_data(&dl_send_live_data_out, buffer_, packet_length));
    assert(compare_dl_send_live_data(&dl_send_live_data, &dl_send_live_data_out));
}


void fill_dl_send_live_data(Smpt_dl_send_live_data * const dl_send_live_data)
{
    int i = 0;
    dl_send_live_data->packet_number = 42;
    dl_send_live_data->n_channels = 8;
    dl_send_live_data->time_offset = 42;
    srand(42);
    for (i = 0; i < dl_send_live_data->n_channels; i++)
    {
        dl_send_live_data->electrode_samples[i].status = Smpt_Dl_Electrode_Status_Positive || Smpt_Dl_Electrode_Status_Negative;
        dl_send_live_data->electrode_samples[i].signal_type = Smpt_Dl_Signal_Type_BI;
        dl_send_live_data->electrode_samples[i].value = ((uint64_t)rand()) * 255 - 1;
    }
}

bool compare_dl_send_live_data(const Smpt_dl_send_live_data *const a,
                               const Smpt_dl_send_live_data *const b)
{
    bool valid = true;
    int i = 0;
    if((a->packet_number != b->packet_number) ||
       (a->n_channels != b->n_channels)       ||
       (a->time_offset != b->time_offset))
    {
        valid = false;
    }
    for (i = 0; i < a->n_channels; i++)
    {
        if ((a->electrode_samples[i].status != b->electrode_samples[i].status) ||
            (a->electrode_samples[i].signal_type != b->electrode_samples[i].signal_type) ||
            (a->electrode_samples[i].value != b->electrode_samples[i].value))
        {
            valid = false;
        }
    }
    return valid;
}
