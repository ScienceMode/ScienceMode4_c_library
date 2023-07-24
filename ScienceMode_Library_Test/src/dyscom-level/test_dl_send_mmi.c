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
#include "test_dl_send_mmi.h"
#include "test_dl_common.h"
#include "test_dl_init.h"
#include "string.h"

void test_dl_send_mmi(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_send_mmi dl_send_mmi;
    Smpt_dl_send_mmi dl_send_mmi_out;
    smpt_clear_dl_send_mmi(&dl_send_mmi);
    smpt_clear_dl_send_mmi(&dl_send_mmi_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_send_mmi(&dl_send_mmi);

    packet_length = smpt_build_dl_send_mmi(buffer_, buffer_length_, &dl_send_mmi);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Send_MMI);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_send_mmi(&dl_send_mmi_out, buffer_, packet_length));
    assert(compare_dl_send_mmi(&dl_send_mmi, &dl_send_mmi_out));
}

void fill_dl_send_mmi(Smpt_dl_send_mmi * const dl_send_mmi)
{
    fill_dl_init(&dl_send_mmi->dl_init);
    strncpy(dl_send_mmi->mmi.measurement_id, "12345678", SMPT_DL_MAX_FILE_ID_LENGTH);
    dl_send_mmi->mmi.file_size = 123456789;
    dl_send_mmi->mmi.measurement_number = 26;
    dl_send_mmi->packet_number = 13;
    fill_dl_tm(&dl_send_mmi->mmi.start_time);
    dl_send_mmi->mmi.time_length = 1024;
}

bool compare_dl_send_mmi(const Smpt_dl_send_mmi *const a,
                         const Smpt_dl_send_mmi *const b)
{
    return compare_dl_init(&a->dl_init, &b->dl_init) &&
           compare_dl_file_id(a->mmi.measurement_id, b->mmi.measurement_id) &&
           (a->mmi.file_size == b->mmi.file_size) &&
           (a->mmi.measurement_number == b->mmi.measurement_number) &&
           (a->packet_number == b->packet_number) &&
           (smpt_dl_compare_tm(&a->mmi.start_time, &b->mmi.start_time) == 0) &&
           (smpt_dl_compare_tm(&a->mmi.planned_start_time, &b->mmi.planned_start_time) == 0) &&
           (a->mmi.time_length == b->mmi.time_length);
}
