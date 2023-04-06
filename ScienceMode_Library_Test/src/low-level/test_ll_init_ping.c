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
#include "test_ll_init_ping.h"
#include "smpt_ll_client.h"

static void fill_ll_init(Smpt_ll_init* const ll_init);
static void wait_for_response(Smpt_device * const device);

void test_ll_init_ping(void)
{
    Smpt_ll_init ll_init = {0};
    /*const char port_name[] = "\\\\.\\COM4";*/
    /*const char port_name[] = "\\\\.\\CNCA0";*/
    /*const char port_name[] = "\\\\.\\COM8";*/

    const char *port_name = "/dev/ttyS0";
    Smpt_device device = {0};

    smpt_open_serial_port(&device, port_name);

    fill_ll_init(&ll_init);

    while(true)
    {
        smpt_debug("send_init()");
        smpt_send_ll_init(&device, &ll_init);

        wait_for_response(&device);
    }

    smpt_close_serial_port(&device);
}

void wait_for_response(Smpt_device *const device)
{
    bool run_loop = true;
    uint8_t counter = 0;

    while (run_loop)
    {
        if (smpt_new_packet_received(device))
        {
            Smpt_ack ack;
            smpt_last_ack(device, &ack);
            smpt_debug("--------------------------------------------------------------",
                       ack.command_number, ack.packet_number);
        }

        counter++;
        if (counter > 10)
        {
            run_loop = false;
        }
    }
}

void fill_ll_init(Smpt_ll_init* const ll_init)
{
    ll_init->measurement_type = Smpt_Measurement_High_Voltage;
    ll_init->packet_number = 50;
}

