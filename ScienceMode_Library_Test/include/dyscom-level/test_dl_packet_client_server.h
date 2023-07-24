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
#ifndef TEST_DL_PACKET_CLIENT_SERVER_H
#define TEST_DL_PACKET_CLIENT_SERVER_H

void test_dl_packet_client_server(void);

/*
#define generate_test_function(smallName,largeName) \
void test_##smallName (void)                                            \
{                                                                       \
    uint32_t packet_length;                                             \
    Smpt_##smallName smallName;                                         \
    Smpt_##smallName smallName##_out;                                   \
    smpt_clear_##smallName (&smallName);                                \
    smpt_clear_##smallName (&smallName##_out);                          \
    smpt_clear_buffer(buffer_, buffer_length_);                         \
    fill_##smallName (&smallName);                                      \
    packet_length = smpt_build_##smallName (buffer_, buffer_length_, &smallName); \
    assert(test_check_header_and_footer(buffer_, packet_length));               \
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_##largeName);       \
    assert(smpt_check_length(buffer_, packet_length));                          \
    assert(smpt_check_checksum(buffer_, packet_length));                        \
    assert(smpt_extract_##smallName (&smallName##_out, buffer_, packet_length));\
    assert(compare_##smallName (&smallName, &smallName##_out));                 \
}

#define generate
*/
#endif /* TEST_DL_PACKET_CLIENT_SERVER_H */
