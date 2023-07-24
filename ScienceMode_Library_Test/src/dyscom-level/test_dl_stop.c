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
#include "test_dl_stop.h"
#include "test_dl_common.h"

void test_dl_stop_ack(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_stop_ack dl_stop_ack;
    Smpt_dl_stop_ack dl_stop_ack_out;
    smpt_clear_dl_stop_ack(&dl_stop_ack);
    smpt_clear_dl_stop_ack(&dl_stop_ack_out);


    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_stop_ack(&dl_stop_ack);

    packet_length = smpt_build_dl_stop_ack(buffer_, buffer_length_, &dl_stop_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Stop_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_stop_ack(&dl_stop_ack_out, buffer_, packet_length));
    assert(compare_dl_stop_ack(&dl_stop_ack, &dl_stop_ack_out));
}

void fill_dl_stop_ack(Smpt_dl_stop_ack *const dl_stop_ack)
{
    dl_stop_ack->packet_number  = 42;
    dl_stop_ack->result         = 0;
    dl_stop_ack->time           = 40000;
}
