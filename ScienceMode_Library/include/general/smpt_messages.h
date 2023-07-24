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
#ifndef SMPT_MESSAGES_H
#define SMPT_MESSAGES_H

/***************************************************************************//**
* \file smpt_messages.h
*
* This file defines functions for error messages. The error function needs to
* be initialized with a callback function using smpt_init_error_callback().
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

/***************************************************************************//**
* \brief Sets the callback function for the error message
*
* \param[in] error_function The callback function
*******************************************************************************/
SMPT_API void smpt_init_error_callback(void (*error_function)(const char* format));

/***************************************************************************//**
* \brief Calls the error callback function with the error message.
*
* \param[in] format A variable number of arguments (usage like printf()))
*******************************************************************************/
SMPT_API void smpt_error(const char* format, ...);

/***************************************************************************//**
* \brief Prints a debug message to stdout. This function is for internal usage
* only.
*
* \param[in] format A variable number of arguments (usage like printf())
*******************************************************************************/
void smpt_debug(const char* format, ...);

/***************************************************************************//**
* \brief Prints the array to stdout. This function is for internal usage only.
*
* \param[in] array
* \param[in] array_length
*******************************************************************************/
void smpt_print_array(const uint8_t array[], uint32_t array_length);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_MESSAGES_H */
