/***************************************************************************//**
* \file smpt_messages.c
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

/* REMOVE FOR RELEASE VERSION */
/* #define SMPT_DEBUG */

#include <stdarg.h>
#include <stdio.h>
#include "smpt_messages.h"

static void (*error_function_)(const char* format) = 0;

SMPT_API void smpt_init_error_callback(void (*error_function)(const char* format))
{
    if (error_function != 0)
    {
        error_function_ = error_function;
    }
}

SMPT_API void smpt_error(const char* format, ...)
{
    if (error_function_ != 0)
    {
        char temp_buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(temp_buffer, format, args);
        va_end(args);
        error_function_(temp_buffer);
    }
}

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

#include "smpt_messages.h"


void smpt_print_array(const uint8_t array[], uint32_t array_length)
{
    uint32_t i = 0;

    printf("0x [");
    for (i = 0; (i < array_length); i++)
    {
        printf("%x", array[i]);
        if (i != array_length - 1)
        {
            printf(" ");
        }
    }
    printf("]\n");
    fflush(stdout);
}

#ifdef SMPT_DEBUG
void smpt_debug(const char* format, ...)
{
    va_list args;
    fprintf(stdout, "Debug(Smpt): ");
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fprintf(stdout, "\n");
    fflush(stdout);
}
#else
void smpt_debug(const char* format, ...)
{
    char c = format[0]; /* dummy commands to satisfy the compiler */
    c++;
}
#endif /* SMPT_DEBUG */
/*void smpt_time_stamp(const char message[])
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    printf("time: %d:%d:%d %s", time.wMinute, time.wSecond, time.wMilliseconds, message);
    fflush(stdout);
}*/


/* End defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */
#else /* µC */

#include "smpt_definitions.h"

void smpt_debug(const char* format, ...)
{
    char c = format[0];
    c++;
}


void smpt_print_array(const uint8_t array[], uint32_t buffer_length)
{
    if (buffer_length > 0)
    {
        uint8_t c = array[0];
        c += buffer_length;
    }
}

#endif /* End µC */
