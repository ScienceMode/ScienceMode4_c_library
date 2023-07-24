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
#ifndef SMPT_PACKET_OUTPUT_BUFFER_H
#define SMPT_PACKET_OUTPUT_BUFFER_H

/***************************************************************************//**
* \file smpt_packet_output_buffer.h
*
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "smpt_definitions.h"

void smpt_packet_output_buffer_init(bool (*send_function)(uint8_t data[], uint32_t data_length));

void smpt_packet_output_buffer_write(const uint8_t packet[], uint32_t packet_length);

void smpt_packet_output_buffer_set_callback(bool (*send_function)(uint8_t data[], uint32_t data_length));

/***************************************************************************//**
* \brief If pending packets are available, this function tries to send them to external master.
*******************************************************************************/
void smpt_packet_output_buffer_send(void);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_INPUT_BUFFER_H */
