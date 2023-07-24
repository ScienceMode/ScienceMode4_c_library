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
#include "test_dl_sys.h"
#include "test_dl_common.h"
#include "test_dl_get.h"


void test_dl_sys(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_sys dl_sys;
    Smpt_dl_sys dl_sys_out;
    smpt_clear_dl_sys(&dl_sys);
    smpt_clear_dl_sys(&dl_sys_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_sys(&dl_sys);

    packet_length = smpt_build_dl_sys(buffer_, buffer_length_, &dl_sys);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Sys);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_sys(&dl_sys_out, buffer_, packet_length));
    assert(compare_dl_sys(&dl_sys, &dl_sys_out));
}


void fill_dl_sys(Smpt_dl_sys * const dl_sys)
{
    char file_name[] = "ThisIs-my_file_name.xyz"; /* 23 + \0 */
    strcpy(dl_sys->file_name, file_name);
    dl_sys->type = Smpt_Dl_Sys_Type_Deletefile;
    dl_sys->packet_number   = 42;
}

bool compare_dl_sys(const Smpt_dl_sys *const a,
                    const Smpt_dl_sys *const b)
{
    bool valid = true;

    if (a->packet_number != b->packet_number)
    {
        valid = false;
    }
    switch(a->type)
    {
        case Smpt_Dl_Sys_Type_Deletefile:
            if (strncmp(a->file_name, b->file_name,
                        SMPT_DL_MAX_FILE_NAME_LENGTH) != 0)
            {
                valid = false;
            }
            break;
        case Smpt_Dl_Sys_Type_Device_Sleep:
            {
                valid = true;
            }
            break;
        default: valid = true;
    }

    if (a->type != b->type)
    {
        valid = false;
    }
    return valid;
}

void test_dl_sys_ack()
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_sys_ack dl_sys_ack;
    Smpt_dl_sys_ack dl_sys_ack_out;
    smpt_clear_dl_sys_ack(&dl_sys_ack);
    smpt_clear_dl_sys_ack(&dl_sys_ack_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_sys_ack(&dl_sys_ack);

    packet_length = smpt_build_dl_sys_ack(buffer_, buffer_length_, &dl_sys_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Sys_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_sys_ack(&dl_sys_ack_out, buffer_, packet_length));
    assert(compare_dl_sys_ack(&dl_sys_ack, &dl_sys_ack_out));
}


void fill_dl_sys_ack(Smpt_dl_sys_ack * const dl_sys_ack)
{
    char file_name[] = "ThisIs-my_file_name_ack.xyz"; /* 23 + \0 */
    strcpy(dl_sys_ack->file_name, file_name);
    dl_sys_ack->type = Smpt_Dl_Sys_Type_Deletefile;
    dl_sys_ack->packet_number   = 42;
    dl_sys_ack->state = Smpt_Dl_Sys_State_Successful;
}

bool compare_dl_sys_ack(const Smpt_dl_sys_ack *const a,
                        const Smpt_dl_sys_ack *const b)
{
    bool valid = true;

    if ((a->packet_number != b->packet_number)||
        (a->result != b->result) ||
        (a->type != b->type))
    {
        valid = false;
    }
    else {
        switch (a->type)
        {
            case Smpt_Dl_Sys_Type_Undefined:
            {
                valid = true;
            }
            break;
            case Smpt_Dl_Sys_Type_Deletefile:
            {
                valid = smpt_compare_string(a->file_name, b->file_name, SMPT_DL_MAX_STRING_LENGTH);
            }
            break;
            case Smpt_Dl_Sys_Type_Device_Sleep:
            {
                valid = true;
            }
            break;
            case Smpt_Dl_Sys_Type_Device_Storage:
            {
                valid = true;
            }
            break;
         }
    }
    return valid;
}
