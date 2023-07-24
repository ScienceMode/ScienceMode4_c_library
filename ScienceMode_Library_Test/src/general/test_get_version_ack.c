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
#include "test_get_version_ack.h"
#include "test_definitions.h"

static void fill_get_version_ack(Smpt_get_version_ack *const get_version_ack);
static bool compare_get_version_ack(const Smpt_get_version_ack *const get_version_ack_a,
                                    const Smpt_get_version_ack *const get_version_ack_b);
static void fill_get_extended_version_ack(Smpt_get_extended_version_ack *const get_version_ack);
static bool compare_get_extended_version_ack(const Smpt_get_extended_version_ack *const get_version_ack_a,
                                    const Smpt_get_extended_version_ack *const get_version_ack_b);

void test_get_version_ack(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_get_version_ack get_version_ack;
    Smpt_get_version_ack get_version_ack_out;
    smpt_clear_get_version_ack(&get_version_ack);
    smpt_clear_get_version_ack(&get_version_ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_get_version_ack(&get_version_ack);

    packet_length = smpt_build_get_version_ack(buffer, buffer_length, &get_version_ack,
                                               Smpt_Cmd_Get_Version_Main_Ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Get_Version_Main_Ack);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_get_version_ack(&get_version_ack_out, buffer, packet_length));

    assert(compare_get_version_ack(&get_version_ack, &get_version_ack_out));
}

void fill_get_version_ack(Smpt_get_version_ack *const get_version_ack)
{
    get_version_ack->result = Smpt_Result_Parameter_Error;
    get_version_ack->packet_number = 16;

    get_version_ack->uc_version.fw_version.major    = 1;
    get_version_ack->uc_version.fw_version.minor    = 2;
    get_version_ack->uc_version.fw_version.revision = 3;

    get_version_ack->uc_version.smpt_version.major    = 4;
    get_version_ack->uc_version.smpt_version.minor    = 5;
    get_version_ack->uc_version.smpt_version.revision = 6;
}

bool compare_get_version_ack(const Smpt_get_version_ack *const get_version_ack_a,
                             const Smpt_get_version_ack *const get_version_ack_b)
{
    return (get_version_ack_a->result == get_version_ack_b->result) &&
           (get_version_ack_a->packet_number == get_version_ack_b->packet_number) &&

           (get_version_ack_a->uc_version.fw_version.major ==
            get_version_ack_b->uc_version.fw_version.major) &&
           (get_version_ack_a->uc_version.fw_version.minor ==
            get_version_ack_b->uc_version.fw_version.minor) &&
           (get_version_ack_a->uc_version.fw_version.revision ==
            get_version_ack_b->uc_version.fw_version.revision) &&
           (get_version_ack_a->uc_version.smpt_version.major ==
            get_version_ack_b->uc_version.smpt_version.major) &&
           (get_version_ack_a->uc_version.smpt_version.minor ==
            get_version_ack_b->uc_version.smpt_version.minor) &&
           (get_version_ack_a->uc_version.smpt_version.revision ==
            get_version_ack_b->uc_version.smpt_version.revision);
}





void test_get_extended_version_ack(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_get_extended_version_ack get_version_ack;
    Smpt_get_extended_version_ack get_version_ack_out;
    smpt_clear_get_extended_version_ack(&get_version_ack);
    smpt_clear_get_extended_version_ack(&get_version_ack_out);

    smpt_clear_buffer(buffer, buffer_length);
    fill_get_extended_version_ack(&get_version_ack);

    packet_length = smpt_build_get_extended_version_ack(buffer, buffer_length, &get_version_ack,
                                               Smpt_Cmd_Get_Extended_Version_Ack);

    assert(test_check_header_and_footer(buffer, packet_length));
    assert(smpt_get_cmd(buffer, packet_length) == Smpt_Cmd_Get_Extended_Version_Ack);
    assert(smpt_check_length(buffer, packet_length));
    assert(smpt_check_checksum(buffer, packet_length));
    assert(smpt_extract_get_extended_version_ack(&get_version_ack_out, buffer, packet_length));

    assert(compare_get_extended_version_ack(&get_version_ack, &get_version_ack_out));
}

void fill_get_extended_version_ack(Smpt_get_extended_version_ack *const get_version_ack)
{
    get_version_ack->result = Smpt_Result_Parameter_Error;
    get_version_ack->packet_number = 16;

    get_version_ack->uc_version.fw_version.major    = 1;
    get_version_ack->uc_version.fw_version.minor    = 2;
    get_version_ack->uc_version.fw_version.revision = 3;

    get_version_ack->uc_version.smpt_version.major    = 4;
    get_version_ack->uc_version.smpt_version.minor    = 5;
    get_version_ack->uc_version.smpt_version.revision = 6;
    get_version_ack->fw_hash = 12345;
    get_version_ack->fw_hash_type = Smpt_Fw_Hash_Type_Git;
    get_version_ack->fw_hash_state = Smpt_Fw_Hash_State_Valid;
}

bool compare_get_extended_version_ack(const Smpt_get_extended_version_ack *const get_version_ack_a,
                             const Smpt_get_extended_version_ack *const get_version_ack_b)
{
    return (get_version_ack_a->result == get_version_ack_b->result) &&
           (get_version_ack_a->packet_number == get_version_ack_b->packet_number) &&

           (get_version_ack_a->uc_version.fw_version.major ==
            get_version_ack_b->uc_version.fw_version.major) &&
           (get_version_ack_a->uc_version.fw_version.minor ==
            get_version_ack_b->uc_version.fw_version.minor) &&
           (get_version_ack_a->uc_version.fw_version.revision ==
            get_version_ack_b->uc_version.fw_version.revision) &&
           (get_version_ack_a->uc_version.smpt_version.major ==
            get_version_ack_b->uc_version.smpt_version.major) &&
           (get_version_ack_a->uc_version.smpt_version.minor ==
            get_version_ack_b->uc_version.smpt_version.minor) &&
           (get_version_ack_a->uc_version.smpt_version.revision ==
            get_version_ack_b->uc_version.smpt_version.revision) &&
           (get_version_ack_a->fw_hash ==
            get_version_ack_b->fw_hash) &&
           (get_version_ack_a->fw_hash_type ==
            get_version_ack_b->fw_hash_type) &&
           (get_version_ack_a->fw_hash_state ==
            get_version_ack_b->fw_hash_state);
}
