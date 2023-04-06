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
#include "test_packet_client_server.h"

#include "test_get_file.h"
#include "test_get_file_ack.h"
#include "test_file_block.h"
#include "test_remove_file.h"
#include "test_file.h"
#include "test_ack.h"
#include "test_find_packet.h"
#include "test_get_battery_status_ack.h"
#include "test_get_device_id_ack.h"
#include "test_get_version_ack.h"
#include "test_get_main_status_ack.h"
#include "test_get_stim_status_ack.h"
#include "test_set_power.h"

void test_packet_client_server()
{
    test_get_file();
    test_get_file_ack();
    test_file_block();
    //test_remove_file();

    test_get_battery_status_ack();
    test_get_device_id_ack();
    test_get_version_ack();
    test_get_extended_version_ack();
    test_set_power();
    test_get_main_status_ack();
    test_get_stim_status_ack();

    test_file();

    test_ack();
    test_find_packet();
}
