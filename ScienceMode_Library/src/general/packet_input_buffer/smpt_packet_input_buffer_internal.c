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
#include "smpt_packet_input_buffer_internal.h"
#include "smpt_definitions_internal.h"
#include <string.h>
#include "smpt_messages.h"

static void clear_data_line(uint32_t row_number, Packet_input_buffer *const input_buffer);
static void set_row_state_to_full(Packet_input_buffer *const input_buffer);

void smpt_packet_input_buffer_init(Packet_input_buffer *const input_buffer)
{
    uint32_t i;
    for (i = 0; i < input_buffer->number_of_rows; i++)
    {
        clear_data_line(i, input_buffer);
    }
    input_buffer->write_row_length_count = 0;
    input_buffer->write_row_count = 0;
    input_buffer->read_row_count = 0;
    input_buffer->ignore_next_byte = false;
}

bool smpt_packet_input_buffer_nearly_full(const Packet_input_buffer *const input_buffer)
{
    uint32_t successor = input_buffer->write_row_count + 1;
    if (successor == input_buffer->number_of_rows)
    {
        successor = 0;
    }
    return input_buffer->buffer_state[successor] == Packet_Buffer_Row_State_Full;
}

void smpt_packet_input_buffer_write(uint8_t byte, Packet_input_buffer *const input_buffer)
{
    /* This function has more than one exit points. */

    uint32_t index = input_buffer->write_row_count * input_buffer->row_length +
                     input_buffer->write_row_length_count;

    if (input_buffer->buffer_state[input_buffer->write_row_count] == Packet_Buffer_Row_State_Full)
    {
        static long count = 0;
        if ((++count % 10000) == 1)
        {
            smpt_error("smpt_packet_intput_buffer_write_buffer(): "
                       "Full buffer error. Could not write byte. Discarded bytes (%ld)", count);
        }
        return;
    }

    if (((input_buffer->write_row_length_count) == 0) && (byte != Smpt_Packet_Start_Byte))
    {
        return; /* do not write until a start byte has been found. */
    }

    input_buffer->buffer[index] = byte;
    input_buffer->write_row_length_count++;

    if (input_buffer->ignore_next_byte)
    {
        input_buffer->ignore_next_byte = false;

        if (input_buffer->write_row_length_count == input_buffer->row_length)
        {
            smpt_error("smpt_packet_intput_buffer_write(): End of column. No stop byte found. (1)");
            /* smpt_print_array(&input_buffer->buffer[input_buffer->write_row_count *
                                                   input_buffer->row_length],
                                                   input_buffer->row_length);*/
            set_row_state_to_full(input_buffer);
        }
    }
    else
    {
        if (byte == Smpt_Packet_Stuffing_Byte)
        {
            input_buffer->ignore_next_byte = true;
        }

        if (byte == Smpt_Packet_Stop_Byte)
        {
            set_row_state_to_full(input_buffer);
        }

        if (input_buffer->write_row_length_count == input_buffer->row_length)
        {
            smpt_error("smpt_packet_intput_buffer_write(): End of column. No stop byte found. (2)");
            /* smpt_print_array(&input_buffer->buffer[input_buffer->write_row_count *
                                                   input_buffer->row_length],
                                                   input_buffer->row_length); */
            set_row_state_to_full(input_buffer);
        }
    }
}


void set_row_state_to_full(Packet_input_buffer *const input_buffer)
{
    input_buffer->buffer_state[input_buffer->write_row_count] = Packet_Buffer_Row_State_Full;

    input_buffer->write_row_count++;
    if (input_buffer->write_row_count == input_buffer->number_of_rows)
    {
        input_buffer->write_row_count = 0;
    }
    input_buffer->write_row_length_count = 0;
}

bool smpt_packet_input_buffer_write_buffer(uint8_t *buffer, uint32_t buffer_length,
                                           Packet_input_buffer *const input_buffer)
{
    /* This function has more than one exit points. */
    uint32_t index = input_buffer->write_row_count * input_buffer->row_length;

    if (input_buffer->buffer_state[input_buffer->write_row_count] == Packet_Buffer_Row_State_Full)
    {
        static long count = 0;
        if ((++count % 1000) == 1)
        {
            smpt_error("smpt_packet_intput_buffer_write_buffer(): "
                       "Full buffer error. Could not write byte. Discarded bytes (%ld)", count);
        }
        return false;
    }

    if (buffer_length > input_buffer->row_length)
    {
        smpt_error("smpt_packet_input_buffer_write_buffer(): Buffer length mismatch.");
        return false;
    }

    memcpy(&(input_buffer->buffer[index]), buffer, buffer_length);

    set_row_state_to_full(input_buffer);

    return true;
}


bool smpt_packet_input_buffer_new_data_available(Packet_input_buffer *const input_buffer)
{
    return (input_buffer->buffer_state[input_buffer->read_row_count] ==
            Packet_Buffer_Row_State_Full);
}


void smpt_packet_input_buffer_read_data_line(uint8_t **buffer,
                                             Packet_input_buffer *const input_buffer)
{
    *buffer = &input_buffer->buffer[input_buffer->read_row_count * input_buffer->row_length];
}


void smpt_packet_input_buffer_clear_data_line(Packet_input_buffer *const input_buffer)
{
    /* This function has more than one return points */
    if (input_buffer->buffer_state[input_buffer->read_row_count] == Packet_Buffer_Row_State_Empty)
    {
        smpt_error("smpt_packet_intput_buffer_clear_data_line(): Attempt to delete empty row.");
        return;
    }

    clear_data_line(input_buffer->read_row_count, input_buffer);

    input_buffer->read_row_count++;
    if (input_buffer->read_row_count == input_buffer->number_of_rows)
    {
        input_buffer->read_row_count = 0;
    }
}


void clear_data_line(uint32_t row_number, Packet_input_buffer *const input_buffer)
{
    memset(&(input_buffer->buffer[row_number * input_buffer->row_length]), 0x00,
           input_buffer->row_length);
    input_buffer->buffer_state[row_number] = Packet_Buffer_Row_State_Empty;
}
