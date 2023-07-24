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
#include "test_dl_packet_client_server.h"

#include "test_dl_init.h"
/*#include "test_dl_start.h"*/
#include "test_dl_stop.h"
#include "test_dl_get.h"
#include "test_dl_send_file.h"
#include "test_dl_send_live_data.h"
#include "test_dl_send_mmi.h"
#include "test_dl_power_module.h"
#include "test_dl_sys.h"
#include "test_dl_examples.h"
#include <stdio.h>

/* File scope variables */
/*static uint8_t buffer_[Smpt_Limit_Max_Packet_Size];*/
/*static uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;*/

/* File scope functions */

void test_dl_packet_client_server(void)
{
    printf("test_dl_init()\n");
    test_dl_init();

    printf("test_dl_init_ack()\n");
    test_dl_init_ack();

    printf("test_dl_stop_ack()\n");
    test_dl_stop_ack();

    printf("test_dl_get()\n");
    test_dl_get();

    printf("test_dl_get_ack()\n");
    test_dl_get_ack();

    /*printf("test_dl_send_file()");*/
    test_dl_send_file();

    printf("test_dl_send_live_data()\n");
    test_dl_send_live_data();

    test_dl_send_mmi();

    printf("test_dl_power_module()\n");
    test_dl_power_module();

    printf("test_dl_power_module_ack()\n");
    test_dl_power_module_ack();

    printf("test_dl_send_file_ack()\n");
    test_dl_send_file_ack();

    printf("test_dl_sys()\n");
    test_dl_sys();

    printf("test_dl_sys_ack()\n");
    test_dl_sys_ack();
    /*printf("test_dl_examples()\n");*/
    /*Only when virtual comport is connected*/
    /*test_dl_examples();*/
}

