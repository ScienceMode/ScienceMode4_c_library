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
#include "smpt_packet_input_buffer.h"

#include "smpt_packet_input_buffer_definitions.h"
#include "smpt_packet_input_buffer_internal.h"

static Packet_input_buffer packet_input_buffer_1_;
static uint8_t packet_input_buffer_data_1_[Packet_Buffer_Number_Of_Rows_1 *
                                           Packet_Buffer_Row_Length_1];
static uint8_t packet_input_buffer_state_1_[Packet_Buffer_Number_Of_Rows_1];

static Packet_input_buffer packet_input_buffer_2_;
static uint8_t packet_input_buffer_data_2_[Packet_Buffer_Number_Of_Rows_2 *
                                           Packet_Buffer_Row_Length_2];
static uint8_t packet_input_buffer_state_2_[Packet_Buffer_Number_Of_Rows_2];

static Packet_input_buffer packet_input_buffer_3_;
static uint8_t packet_input_buffer_data_3_[Packet_Buffer_Number_Of_Rows_3 *
                                           Packet_Buffer_Row_Length_3];
static uint8_t packet_input_buffer_state_3_[Packet_Buffer_Number_Of_Rows_3];

void smpt_packet_input_buffer1_init(void)
{
    packet_input_buffer_1_.number_of_rows = Packet_Buffer_Number_Of_Rows_1;
    packet_input_buffer_1_.row_length = Packet_Buffer_Row_Length_1;
    packet_input_buffer_1_.buffer = packet_input_buffer_data_1_;
    packet_input_buffer_1_.buffer_state = packet_input_buffer_state_1_;
    smpt_packet_input_buffer_init(&packet_input_buffer_1_);
}


void smpt_packet_input_buffer1_write(uint8_t byte)
{
    smpt_packet_input_buffer_write(byte, &packet_input_buffer_1_);
}


void smpt_packet_input_buffer1_read_data_line(uint8_t **buffer, uint32_t *buffer_length)
{
    smpt_packet_input_buffer_read_data_line(buffer, &packet_input_buffer_1_);
    *buffer_length = packet_input_buffer_1_.row_length;
}


bool smpt_packet_input_buffer1_new_data_available(void)
{
    return smpt_packet_input_buffer_new_data_available(&packet_input_buffer_1_);
}


void smpt_packet_input_buffer1_clear_data_line(void)
{
    smpt_packet_input_buffer_clear_data_line(&packet_input_buffer_1_);
}

bool smpt_packet_input_buffer1_nearly_full()
{
    return smpt_packet_input_buffer_nearly_full(&packet_input_buffer_1_);
}

void smpt_packet_input_buffer2_init(void)
{
    packet_input_buffer_2_.number_of_rows = Packet_Buffer_Number_Of_Rows_2;
    packet_input_buffer_2_.row_length = Packet_Buffer_Row_Length_2;
    packet_input_buffer_2_.buffer = packet_input_buffer_data_2_;
    packet_input_buffer_2_.buffer_state = packet_input_buffer_state_2_;
    smpt_packet_input_buffer_init(&packet_input_buffer_2_);
}


void smpt_packet_input_buffer2_write(uint8_t byte)
{
    smpt_packet_input_buffer_write(byte, &packet_input_buffer_2_);
}


void smpt_packet_input_buffer2_read_data_line(uint8_t **buffer, uint32_t* buffer_length)
{
    smpt_packet_input_buffer_read_data_line(buffer, &packet_input_buffer_2_);
    *buffer_length = packet_input_buffer_2_.row_length;
}


bool smpt_packet_input_buffer2_new_data_available(void)
{
    return smpt_packet_input_buffer_new_data_available(&packet_input_buffer_2_);
}


void smpt_packet_input_buffer2_clear_data_line(void)
{
    smpt_packet_input_buffer_clear_data_line(&packet_input_buffer_2_);
}


void smpt_packet_input_buffer3_init(void)
{
    packet_input_buffer_3_.number_of_rows = Packet_Buffer_Number_Of_Rows_3;
    packet_input_buffer_3_.row_length = Packet_Buffer_Row_Length_3;
    packet_input_buffer_3_.buffer = packet_input_buffer_data_3_;
    packet_input_buffer_3_.buffer_state = packet_input_buffer_state_3_;
    smpt_packet_input_buffer_init(&packet_input_buffer_3_);
}


void smpt_packet_input_buffer3_write(uint8_t byte)
{
    smpt_packet_input_buffer_write(byte, &packet_input_buffer_3_);
}


void smpt_packet_input_buffer3_read_data_line(uint8_t **buffer, uint32_t *buffer_length)
{
    smpt_packet_input_buffer_read_data_line(buffer, &packet_input_buffer_3_);
    *buffer_length = packet_input_buffer_3_.row_length;
}


bool smpt_packet_input_buffer3_new_data_available(void)
{
    return smpt_packet_input_buffer_new_data_available(&packet_input_buffer_3_);
}


void smpt_packet_input_buffer3_clear_data_line(void)
{
    smpt_packet_input_buffer_clear_data_line(&packet_input_buffer_3_);
}
