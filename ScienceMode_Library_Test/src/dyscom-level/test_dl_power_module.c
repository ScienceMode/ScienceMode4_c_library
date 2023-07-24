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
#include "test_dl_power_module.h"
#include "test_dl_common.h"

#include "string.h"

void test_dl_power_module(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_power_module dl_power_module;
    Smpt_dl_power_module dl_power_module_out;
    smpt_clear_dl_power_module(&dl_power_module);
    smpt_clear_dl_power_module(&dl_power_module_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_power_module(&dl_power_module);

    packet_length = smpt_build_dl_power_module(buffer_, buffer_length_, &dl_power_module);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Power_Module);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_power_module(&dl_power_module_out, buffer_, packet_length));
    assert(compare_dl_power_module(&dl_power_module, &dl_power_module_out));
}

void test_dl_power_module_ack(void)
{
    uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_length;
    Smpt_dl_power_module_ack dl_power_module_ack;
    Smpt_dl_power_module_ack dl_power_module_ack_out;
    smpt_clear_dl_power_module_ack(&dl_power_module_ack);
    smpt_clear_dl_power_module_ack(&dl_power_module_ack_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_dl_power_module_ack(&dl_power_module_ack);

    packet_length = smpt_build_dl_power_module_ack(buffer_, buffer_length_, &dl_power_module_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Dl_Power_Module_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_dl_power_module_ack(&dl_power_module_ack_out, buffer_, packet_length));
    assert(compare_dl_power_module_ack(&dl_power_module_ack, &dl_power_module_ack_out));
}

void fill_dl_power_module_ack(Smpt_dl_power_module_ack *const dl_power_module_ack)
{
    dl_power_module_ack->packet_number = 42;
    dl_power_module_ack->result = 0;
    dl_power_module_ack->hardware_module = 1;
    dl_power_module_ack->switch_on_off = 1;
}

bool compare_dl_power_module(const Smpt_dl_power_module *const a,
                             const Smpt_dl_power_module *const b)
{
    return  (a->hardware_module == b->hardware_module) &&
            (a->switch_on_off == b->switch_on_off);
}

bool compare_dl_power_module_ack(const Smpt_dl_power_module_ack *const a,
                                 const Smpt_dl_power_module_ack *const b)
{
    return (a->packet_number    == b->packet_number) &&
           (a->result           == b->result);
}


void fill_dl_power_module(Smpt_dl_power_module * const dl_power_module)
{
    dl_power_module->hardware_module = Smpt_Dl_Hardware_Module_Measurement;
    dl_power_module->switch_on_off = 1;
    dl_power_module->packet_number = 40;
}
