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
#include "smpt_client_data.h"

#include "smpt_client_cmd_lists.h"
#include "smpt_definitions_internal.h"
#include "smpt_packet_input_buffer_internal.h"
#include "smpt_serial_port.h"
#include "smpt_messages.h"
#include "smpt_packet_general.h"
#include "string.h"

/*******************************************************************************
 Section Windows and Linux
*******************************************************************************/
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

static void update_incoming_packets(Smpt_device *const device);
static bool read_packet(Smpt_device *const device, uint8_t **buffer, uint32_t *buffer_length);
/*static bool extract_packet(const uint8_t packet[], uint32_t packet_length,
                           void *const device, Smpt_cmd_list *const cmd_list);*/
static void packet_error(const uint8_t packet[], uint32_t packet_length,
                         const char *const message);

void smpt_client_data_init(Smpt_device *const device)
{
    device->packet_input_buffer.number_of_rows = Smpt_Length_Packet_Input_Buffer_Rows;
    device->packet_input_buffer.row_length     = Smpt_Length_Max_Packet_Size;
    device->packet_input_buffer.buffer         = device->packet_input_buffer_data;
    device->packet_input_buffer.buffer_state   = device->packet_input_buffer_state;
    smpt_packet_input_buffer_init(&device->packet_input_buffer);

    smpt_clear_acks_and_requests(&device->cmd_list);
}

void update_incoming_packets(Smpt_device *const device)
{
    uint32_t packet_start_index = 0;
    uint32_t packet_length = 0;

    uint8_t *buffer_in;
    uint32_t buffer_length = 0;

    if (read_packet(device, &buffer_in, &buffer_length))
    {
        if (smpt_find_packet(&packet_start_index, &packet_length, 0, buffer_in, buffer_length))
        {
            const uint8_t *packet = &buffer_in[packet_start_index];

            /* smpt_error("input: %d", smpt_get_packet_number(packet)); */

            if (smpt_check_length(packet, packet_length) &&
                smpt_check_checksum(packet, packet_length))
            {
                Smpt_ack ack = {0};
                smpt_extract_ack(&ack, packet, packet_length);

                smpt_add_ack(ack.command_number, ack.result, ack.packet_number,
                             &device->cmd_list);

                smpt_clear_buffer(device->packet, device->packet_length);
                memcpy(device->packet, packet, packet_length);
                device->packet_length = packet_length;

                device->cmd_list.new_ack_available++;
            }
            else
            {
                packet_error(packet, packet_length, "Transfer error incoming packet");
            }
        }
        smpt_packet_input_buffer_clear_data_line(&device->packet_input_buffer);
    }
}

void packet_error(const uint8_t packet[], uint32_t packet_length, const char *const message)
{
    uint8_t packet_number = 0;
    Smpt_Cmd cmd = Smpt_Cmd_Ll_Init;
    if (packet_length > Smpt_Length_Min_Packet_Size)
    {
        packet_number = smpt_get_packet_number(packet);
        cmd = smpt_get_cmd(packet, packet_length);
    }

    smpt_error("%s cmd: %d, packet_number: %d", message, cmd, packet_number);
}

bool read_packet(Smpt_device *const device, uint8_t **buffer, uint32_t *buffer_length)
{
    bool packet_available = false;
    uint32_t bytes_read = 0;
    uint32_t i = 0;

    uint8_t buffer_in[Smpt_Limit_Max_Packet_Size];
    smpt_clear_buffer(buffer_in, Smpt_Limit_Max_Packet_Size);
    *buffer_length = Smpt_Limit_Max_Packet_Size;

    smpt_read_from_serial_port_internal(device, &bytes_read, buffer_in, Smpt_Limit_Max_Packet_Size);

    for (i = 0; i < bytes_read; i++)
    {
        smpt_packet_input_buffer_write(buffer_in[i], &device->packet_input_buffer);
    }

    if (smpt_packet_input_buffer_new_data_available(&device->packet_input_buffer))
    {
        smpt_packet_input_buffer_read_data_line(buffer, &device->packet_input_buffer);
        packet_available = true;
    }

    return packet_available;
}


void smpt_client_data_last_ack(Smpt_device *const device, Smpt_ack *const ack)
{
    if (device->cmd_list.new_ack_available > 0)
    {
        smpt_get_last_ack(ack, device->cmd_list.new_ack_available, &device->cmd_list);
        device->cmd_list.new_ack_available--;
    }
}

bool smpt_client_data_new_packet_received(Smpt_device *const device)
{
    update_incoming_packets(device);
    return (device->cmd_list.new_ack_available > 0);
}

#else

bool smpt_new_packet_received_internal(void)
{
    return false;
}

#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */
/*******************************************************************************
 End Section Windows and Linux
*******************************************************************************/

