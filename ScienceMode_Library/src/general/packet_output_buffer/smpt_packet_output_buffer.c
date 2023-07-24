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
#include "smpt_packet_output_buffer.h"
#include "smpt_definitions.h"
#include "smpt_messages.h"
#include <string.h>

enum Packet_Output_Buffer_Consts
{
    Packet_Output_Buffer_Size = Smpt_Length_Packet_Output_Buffer
};

static uint8_t buffer_[Packet_Output_Buffer_Size];
static uint32_t last_pos_;
static bool ready_;
static bool (*send_function_)(uint8_t data[], uint32_t data_length);

static void reset_buffer(void);
/******************************************************************************/

void smpt_packet_output_buffer_init(bool (*send_function)(uint8_t data[], uint32_t data_length))
{
    smpt_packet_output_buffer_set_callback(send_function);
    ready_ = true;
    reset_buffer();
}

void smpt_packet_output_buffer_set_callback(bool (*send_function)(uint8_t data[], uint32_t data_length))
{
    send_function_ = send_function;
}

void smpt_packet_output_buffer_write(const uint8_t packet[], uint32_t packet_length)
{
    uint32_t new_last_pos;
    ready_ = false;
    new_last_pos = last_pos_ + packet_length;

    if (new_last_pos < Packet_Output_Buffer_Size)
    {
        memcpy(&(buffer_[last_pos_]), packet, packet_length);
        last_pos_ = new_last_pos;
    }
    else
    {
        smpt_error("smpt_packet_output_buffer(): Buffer full");
    }
    ready_ = true;
}

void smpt_packet_output_buffer_send(void)
{
    if ((last_pos_ > 0) && ready_)
    {
        if (send_function_(buffer_, last_pos_))
        {
            reset_buffer();
        }
    }
}

void reset_buffer(void)
{
    memset(buffer_, 0, Packet_Output_Buffer_Size);
    last_pos_ = 0;
}
