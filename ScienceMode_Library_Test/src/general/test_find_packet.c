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
#include "test_find_packet.h"
#include "test_definitions.h"

static void test_find_packet_a(void);
static void test_find_packet_b(void);
static void test_find_packet_c(void);
static void test_find_packet_d(void);
static void test_find_packet_e(void);
static void test_find_packet_f(void);
static void test_find_packet_g(void);

void test_find_packet(void)
{
    /* { } - buffer
       [ ] - packet
        X  - Start of search

       Packet found
       a) { [ .. ] } -> The start of the packet is the first index == start of search
       b) { .. X .. [ .. ] .. }
       c) { .. [ .. ] .. X .. }
       d) { .. ] .. X .. [ .. }

       Packet missed
       e) { .. [ .. X .. ] .. }
       f) { .. ] .. [ .. X .. }
       g) { .. X .. ] .. [ .. }
     */

    test_find_packet_a();
    test_find_packet_b();
    test_find_packet_c();
    test_find_packet_d();
    test_find_packet_e();
    test_find_packet_f();
    test_find_packet_g();
}

void test_find_packet_a(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = Smpt_Packet_Start_Byte;
    buffer[1] = 'a';
    buffer[2] = 'a';
    buffer[3] = Smpt_Packet_Stop_Byte;

    assert(smpt_find_packet(&packet_start_index, &packet_length, 0, buffer, buffer_length));
    assert(packet_start_index == 0);
    assert(packet_length == 4);
}

void test_find_packet_b(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = 'b';
    buffer[1] = 'b';
    buffer[2] = 'b';
    buffer[3] = Smpt_Packet_Start_Byte;
    buffer[4] = 'b';
    buffer[5] = 'b';
    buffer[6] = 'b';
    buffer[7] = 'b';
    buffer[8] = Smpt_Packet_Stop_Byte;

    assert(smpt_find_packet(&packet_start_index, &packet_length, 1, buffer, buffer_length));
    assert(packet_start_index == 3);
    assert(packet_length == 6);
}

void test_find_packet_c(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = 'c';
    buffer[1] = 'c';
    buffer[2] = Smpt_Packet_Stop_Byte;
    buffer[3] = 'c';
    buffer[4] = 'c';
    buffer[5] = 'c';
    buffer[6] = Smpt_Packet_Start_Byte;
    buffer[7] = 'c';
    buffer[8] = 'c';

    assert(smpt_find_packet(&packet_start_index, &packet_length, 4, buffer, buffer_length));
    assert(packet_start_index == 6);
    assert(packet_length == Smpt_Limit_Max_Packet_Size - 3);
}

void test_find_packet_d(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = 'd';
    buffer[1] = 'd';
    buffer[2] = Smpt_Packet_Start_Byte;
    buffer[3] = 'd';
    buffer[4] = 'd';
    buffer[5] = 'd';
    buffer[6] = Smpt_Packet_Stop_Byte;
    buffer[7] = 'd';
    buffer[8] = 'd';

    assert(smpt_find_packet(&packet_start_index, &packet_length, 8, buffer, buffer_length));
    assert(packet_start_index == 2);
    assert(packet_length == 5);
}

void test_find_packet_e(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = 'e';
    buffer[1] = 'e';
    buffer[2] = Smpt_Packet_Start_Byte;
    buffer[3] = 'e';
    buffer[4] = 'e';
    buffer[5] = 'e';
    buffer[6] = Smpt_Packet_Stop_Byte;
    buffer[7] = 'e';
    buffer[8] = 'e';

    assert(!smpt_find_packet(&packet_start_index, &packet_length, 4, buffer, buffer_length));
    assert(packet_start_index == 0);
    assert(packet_length == 0);
}

void test_find_packet_f(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = 'f';
    buffer[1] = 'f';
    buffer[2] = Smpt_Packet_Stop_Byte;
    buffer[3] = 'f';
    buffer[4] = 'f';
    buffer[5] = 'f';
    buffer[6] = Smpt_Packet_Start_Byte;
    buffer[7] = 'f';
    buffer[8] = 'f';

    assert(!smpt_find_packet(&packet_start_index, &packet_length, 8, buffer, buffer_length));
    assert(packet_start_index == 0);
    assert(packet_length == 0);
}

void test_find_packet_g(void)
{
    uint8_t buffer[Smpt_Limit_Max_Packet_Size];
    uint32_t buffer_length = Smpt_Limit_Max_Packet_Size;
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    smpt_clear_buffer(buffer, buffer_length);

    buffer[0] = 'g';
    buffer[1] = 'g';
    buffer[2] = Smpt_Packet_Stop_Byte;
    buffer[3] = 'g';
    buffer[4] = 'g';
    buffer[5] = 'g';
    buffer[6] = Smpt_Packet_Start_Byte;
    buffer[7] = 'g';
    buffer[8] = 'g';

    assert(!smpt_find_packet(&packet_start_index, &packet_length, 1, buffer, buffer_length));
    assert(packet_start_index == 0);
    assert(packet_length == 0);
}

