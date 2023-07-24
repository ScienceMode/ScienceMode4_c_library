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
#include "test_ll_packet_client_server.h"
#include "test_ll_client_cmd_lists.h"

#include "test_packet_client_server.h"
#include "test_ml_packet_client_server.h"
#include "test_dl_packet_client_server.h"

#include "test_ll_init_ping.h"
#include "test_ll_examples.h"
#include "test_ml_examples.h"


#include "smpt_messages.h"
#include <stdio.h>
#include "string.h"

static void error(const char* format);

void uint64_printf_scanf_test(void)
{
    /*
    uint64_t a = 1073741824;
    uint64_t val = a*a;
    uint32_t low_byte = val & 0xFFFFFFFF;
    uint32_t high_byte = (val >> 32) & 0xFFFFFFFF;


    uint64_t high_uint64 = 0;
    uint64_t low_uint64 = 0;

    printf("low byte: %u\n", low_byte);
    printf("high byte: %u\n", high_byte);
    */

    /* the next three statements will not compile with -Werror but they work
     *
    sscanf("456 123", "%u %u", &high_uint64, &low_uint64);
    printf("%u\n", low_uint64);
    printf("%u\n", high_uint64);
    */

    /*
     * the follwing printf statement will not work with mingw 32bit c89
     *
     * format '%u' expects argument of type 'unsigned int', but argument 2 has type 'uint64_t' [-Werror=format=]
     * printf("%ull\n", val);
     *
     *
     * printf("%ull\n", val);
    */
}


typedef struct {
    char text[7];
} BStruct;

typedef struct {
    char text[12];
    uint64_t x;
    BStruct sub;
} AStruct;

/*
void test_struct_copy()
{
    AStruct a,b;
    strncpy(a.text, "123456789", 12);
    strncpy(a.sub.text, "654321", 7);
    a.x = 42;

    b = a;
    b.x++;
    printf("%s\n", a.text);
    printf("%s %s \n", b.text, b.sub.text);
} */

int main(void)
{
    /* uint64_printf_scanf_test(); */
    smpt_init_error_callback(&error);

    /* general */
    printf("test_packet_client_server()\n");
    test_packet_client_server();


#ifdef SMPT_LOW_LEVEL
    printf("test_ll_packet_client_server()\n");
    test_ll_packet_client_server();
    printf("test_ll_client_cmd_lists()\n\n");
    test_ll_client_cmd_lists();
    /* Works only when a virtual comport is created*/
    /*test_ll_init_ping();*/
    /*test_single_pulse_main();*/
#endif

#ifdef SMPT_MID_LEVEL
    printf("test_ml_packet_client_server()\n\n");
    test_ml_packet_client_server();
    /* Works only when a virtual comport is created*/
    /*test_mid_level_examples();*/
#endif

#ifdef SMPT_DYSCOM_LEVEL
    printf("test_dl_packet_client_server()\n\n");
    test_dl_packet_client_server();
#endif

    return 0;
}

/*
 * The debug messages are realized with a printf wrapper (see following website
 * for more details) The ** need to be replaced with two slashs.
 * http:**www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
 */
void error(const char* format)
{
    printf("%s\n", format);
}








