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
#ifndef SMPT_PACKET_INPUT_BUFFER_H
#define SMPT_PACKET_INPUT_BUFFER_H

/***************************************************************************//**
* \file smpt_packet_input_buffer.h
*
* This modul is used to manage the input data from communication to build
* ScienceMode2+ packets. There are three independent input buffers.
* Before the first use, the each buffer needs to be initialized (*_init()).
* You add to the buffer using the *_write() function. This byte is saved in an
* internal 2-dimensional array, where each line is reserved for one packet
* (except if an overflow occurs). You can check if a complete packet is
* available using *_new_data_available(). To get the pointer to the line in the
* buffer, use * *_read_data_line(). After you have processed the data, you need
* to clear the last read buffer line using *_clear_data_line().
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

/****************************************************************************//*
* \brief Initializes the internal ring structure (Buffer 1).
*******************************************************************************/
void smpt_packet_input_buffer1_init(void);


/****************************************************************************//*
* \brief Writes a byte into the internal structure (Buffer 1).
*
* \param[in] data
*******************************************************************************/
void smpt_packet_input_buffer1_write(uint8_t data);


/****************************************************************************//*
* \brief Reads a complete line of received RX data (Buffer 1).
*
* \param[out] buffer
*******************************************************************************/
void smpt_packet_input_buffer1_read_data_line(uint8_t **buffer, uint32_t *buffer_length);


void smpt_packet_input_buffer1_clear_data_line(void);

/****************************************************************************//*
* \brief Checks if new data (a complete line) is ready for read out (Buffer 1).
*
* \returns True if a new line is ready (stop byte has been found before)
*******************************************************************************/
bool smpt_packet_input_buffer1_new_data_available(void);


/****************************************************************************//*
* \brief Checks if the buffer can only buffer on more packet
* \returns true, if the buffer is full when the next packet is completed
*******************************************************************************/
bool smpt_packet_input_buffer1_nearly_full(void);

/****************************************************************************//*
* \brief Initializes the internal ring structure (Buffer 2).
*******************************************************************************/
void smpt_packet_input_buffer2_init(void);


/****************************************************************************//*
* \brief Writes a byte into the internal structure (Buffer 2).
*
* \param[in] data
*******************************************************************************/
void smpt_packet_input_buffer2_write(uint8_t data);


/****************************************************************************//*
* \brief Reads a complete line of received RX data (Buffer 2).
*
* \param[out] buffer
*******************************************************************************/
void smpt_packet_input_buffer2_read_data_line(uint8_t **buffer, uint32_t *buffer_length);


void smpt_packet_input_buffer2_clear_data_line(void);

/****************************************************************************//*
* \brief Checks if new data (a complete line) is ready for read out (Buffer 2).
*
* \returns True if a new line is ready (stop byte has been found before)
*******************************************************************************/
bool smpt_packet_input_buffer2_new_data_available(void);


/****************************************************************************//*
* \brief Initializes the internal ring structure (Buffer 3).
*******************************************************************************/
void smpt_packet_input_buffer3_init(void);


/****************************************************************************//*
* \brief Writes a byte into the internal structure (Buffer 3).
*
* \param[in] data
*******************************************************************************/
void smpt_packet_input_buffer3_write(uint8_t data);


/****************************************************************************//*
* \brief Reads a complete line of received RX data (Buffer 3).
*
* \param[out] buffer
*******************************************************************************/
void smpt_packet_input_buffer3_read_data_line(uint8_t **buffer, uint32_t *buffer_length);


void smpt_packet_input_buffer3_clear_data_line(void);


/****************************************************************************//*
* \brief Checks if new data (a complete line) is ready for read out (Buffer 3).
*
* \returns True if a new line is ready (stop byte has been found before)
*******************************************************************************/
bool smpt_packet_input_buffer3_new_data_available(void);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_INPUT_BUFFER_H */
