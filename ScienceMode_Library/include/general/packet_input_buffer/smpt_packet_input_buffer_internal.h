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
#ifndef SMPT_PACKET_INPUT_BUFFER_INTERNAL_H
#define SMPT_PACKET_INPUT_BUFFER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_packet_input_buffer_definitions.h"

void smpt_packet_input_buffer_init(Packet_input_buffer *const input_buffer);

void smpt_packet_input_buffer_write(uint8_t byte, Packet_input_buffer *const input_buffer);

bool smpt_packet_input_buffer_write_buffer(uint8_t *buffer, uint32_t buffer_length,
                                           Packet_input_buffer *const input_buffer);

bool smpt_packet_input_buffer_nearly_full(const Packet_input_buffer *const input_buffer);

bool smpt_packet_input_buffer_new_data_available(Packet_input_buffer *const memory);

void smpt_packet_input_buffer_read_data_line(uint8_t **buffer,
                                                Packet_input_buffer *const input_buffer);

void smpt_packet_input_buffer_clear_data_line(Packet_input_buffer *const input_buffer);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_INPUT_BUFFER_INTERNAL_H */
