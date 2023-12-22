/***************************************************************************//**
* \file smpt_ll_messages.c
* \copyright MPL and LGPL HASOMED GmbH 2013-2022
*
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

#include "smpt_ll_messages.h"

/* REMOVE FOR RELEASE VERSION */
/* #define SMPT_DEBUG */

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

#include <stdio.h>
#include <stdarg.h>
#include "smpt_messages.h"

/***************************************************************************//**
* \brief Prints the parameters to stdout.
*
* \param[in] yellow set to true if the color of the point is yellow
* \param[in] i current point index
* \param[in] ll_point
*******************************************************************************/
static void smpt_print_ll_point(bool yellow, int i, const Smpt_ll_point *const ll_point);

void smpt_print_ll_init(const Smpt_ll_init * const ll_init)
{
    printf("Ll_init - \n");
    printf("   packet_number: %u\n", ll_init->packet_number);
    printf("   measurement: %d\n", ll_init->measurement_type);
    fflush(stdout);
}

void smpt_print_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack)
{
    printf("Ll_init_ack - \n");
    printf("   packet_number: %u\n", ll_init_ack->packet_number);
    printf("   result: %u\n", ll_init_ack->result);
    fflush(stdout);
}

void smpt_print_ll_channel_config(const Smpt_ll_channel_config *const ll_channel_config)
{
    uint8_t i;

    printf("Ll_channel_config - \n");
    printf("   packet_number: %u\n", ll_channel_config->packet_number);
    printf("   enable_stimulation: %u\n", ll_channel_config->enable_stimulation);
    printf("   channel: %u\n", ll_channel_config->channel);
    printf("   connector: %u\n", ll_channel_config->connector);
    printf("   number_of_points: %u\n", ll_channel_config->number_of_points);

    for (i = 0; i < ll_channel_config->number_of_points; i++)
    {
        Smpt_ll_point point = ll_channel_config->points[i];
        smpt_print_ll_point(true, i, &point);
    }

    fflush(stdout);
}

void smpt_print_ll_channel_config_ack(const Smpt_ll_channel_config_ack *const ll_channel_config_ack)
{
    printf("Ll_channel_config_ack - \n");
    printf("   packet_number: %u\n", ll_channel_config_ack->packet_number);
    printf("   result: %u\n", ll_channel_config_ack->result);
    printf("   channel: %u\n", ll_channel_config_ack->channel);
    printf("   connector: %u\n", ll_channel_config_ack->connector);
    fflush(stdout);
}

void smpt_print_ll_point(bool yellow, int i, const Smpt_ll_point *const ll_point)
{
    char color = (yellow) ? 'y' : 'g';
    printf("   point_%c: %02u time: %04u; ", color, i+1, ll_point->time);
    printf("current: %04.1f; ", ll_point->current);
    printf("interpolation_mode: %02u; ", ll_point->interpolation_mode);
    fflush(stdout);
}

/* void smpt_print_ll_cmd(const Smpt_ll_cmd *const ll_cmd)
{
    printf("Ll_cmd - command_number: %u\n", ll_cmd->command_number);
    printf("   packet_number: %u\n", ll_cmd->packet_number);
    fflush(stdout);
}*/

void smpt_print_ll_ack(const Smpt_ll_ack *const ll_ack)
{
    printf("Ll_ack - command_number: %u\n", ll_ack->command_number);
    printf("   packet_number: %u\n", ll_ack->packet_number);
    printf("   result: %u\n", ll_ack->result);
    fflush(stdout);
}

/*void smpt_time_stamp(const char message[])
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    printf("time: %d:%d:%d %s", time.wMinute, time.wSecond, time.wMilliseconds, message);
    fflush(stdout);
}*/


/* End defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */
#else /* µC */

/* dummy implementations to trick the compiler */


void smpt_print_ll_init(const Smpt_ll_init * const ll_init)
{
    uint8_t c = ll_init->packet_number;
    c++;
}


void smpt_print_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack)
{
    uint8_t c = ll_init_ack->packet_number;
    c++;
}


#endif /* End µC */
