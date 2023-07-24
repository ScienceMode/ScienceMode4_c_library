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
#include "test_ml_examples.h"
#include "smpt_ml_client.h"

static void test_mid_level_stimulation(const char *port_name);

void test_mid_level_examples(void)
{
    /* EDIT: Change to the virtual com port of your device */
    /* const char *port_name = "COM5"; */
    const char *port_name = "CNCA0";

    test_mid_level_stimulation(port_name);
}

void test_mid_level_stimulation(const char *port_name)
{
    uint8_t packet_number = 0;  /* The packet_number can be used for debugging purposes */
    Smpt_ml_init ml_init = {0};       /* Struct for ml_init command */
    Smpt_ml_update ml_update = {0};   /* Struct for ml_update command */

    Smpt_device device = {0};

    smpt_open_serial_port(&device, port_name);

    /* Clear ml_init struct and set the data */
    smpt_clear_ml_init(&ml_init);
    ml_init.packet_number = packet_number;

    /* Send the ml_init command to the stimulation unit */
    smpt_send_ml_init(&device, &ml_init);

    packet_number++;

    /* Clear ml_update and set the data */
    smpt_clear_ml_update(&ml_update);
    ml_update.enable_channel[Smpt_Channel_Red] = true;  /* Enable channel red */
    ml_update.packet_number = packet_number;

    ml_update.channel_config[Smpt_Channel_Red].number_of_points = 3;  /* Set the number of points */
    ml_update.channel_config[Smpt_Channel_Red].ramp = 3;              /* Three lower pre-pulses   */
    ml_update.channel_config[Smpt_Channel_Red].period = 20;           /* Frequency: 50 Hz */

    /* Set the stimulation pulse */
    /* First point, current: 20 mA, positive, pulse width: 200 µs */
    ml_update.channel_config[Smpt_Channel_Red].points[0].current = 20;
    ml_update.channel_config[Smpt_Channel_Red].points[0].time = 200;

    /* Second point, pause 100 µs */
    ml_update.channel_config[Smpt_Channel_Red].points[1].time = 100;

    /* Third point, current: -20 mA, negative, pulse width: 200 µs */
    ml_update.channel_config[Smpt_Channel_Red].points[2].current = -20;
    ml_update.channel_config[Smpt_Channel_Red].points[2].time = 200;

    /* Send the ml_update command to the stimulation unit */
    smpt_send_ml_update(&device, &ml_update);

    packet_number++;

    /* Send the ml_stop command to the stimulation unit */
    smpt_send_ml_stop(&device, packet_number);

    smpt_close_serial_port(&device);
}
