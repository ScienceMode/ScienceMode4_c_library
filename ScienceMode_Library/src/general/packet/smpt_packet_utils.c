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
#include "smpt_packet_utils.h"
#include "smpt_messages.h"
#include "string.h"


union Double_container
{
    double   value;
    uint64_t uint_bytes;
};

union Float_container
{
    float    value;
    uint32_t uint_bytes;
};

static uint32_t copy_array_char(char destination[], const char source[], uint32_t array_length,
                         uint32_t index);

uint32_t smpt_insert_uint16_t(uint8_t buffer[], const uint16_t* const value, uint32_t index)
{
    uint8_t byte_index = 0;
    uint8_t bit_shift = 0;
    for (; byte_index < 2; byte_index++)
    {
        bit_shift = (2 - 1 - byte_index) * 8;
        buffer[index++] = ((*value) >> bit_shift) & 0xFF;
    }

    return index;
}

uint32_t smpt_extract_uint16_t(uint16_t* const value, const uint8_t buffer[],
                                  uint32_t index)
{
    uint8_t byte_index = 0;
    uint8_t bit_shift = 0;
    *value = 0;
    for (; byte_index < 2; byte_index++)
    {
        bit_shift = (2 - 1 - byte_index) * 8;
        *value |= ((uint16_t)buffer[index++]) << bit_shift;
    }

    return index;
}

uint32_t smpt_insert_uint32_t(uint8_t buffer[], const uint32_t* const value, uint32_t index)
{
    uint8_t byte_index = 0;
    uint8_t bit_shift = 0;
    for (; byte_index < 4; byte_index++)
    {
        bit_shift = (4 - 1 - byte_index) * 8;
        buffer[index++] = ((*value) >> bit_shift) & 0xFF;
    }

    return index;
}

uint32_t smpt_extract_uint32_t(uint32_t* const value, const uint8_t buffer[],
                                  uint32_t index)
{
    uint8_t byte_index = 0;
    uint8_t bit_shift = 0;
    *value = 0;
    for (; byte_index < 4; byte_index++)
    {
        bit_shift = (4 - 1 - byte_index) * 8;
        *value |= ((uint32_t)buffer[index++]) << bit_shift;
    }

    return index;
}

uint32_t smpt_insert_uint64_t(uint8_t buffer[], const uint64_t* const value, uint32_t index)
{
    uint8_t byte_index = 0;
    uint8_t bit_shift = 0;
    for (; byte_index < 8; byte_index++)
    {
        bit_shift = (8 - 1 - byte_index) * 8;
        buffer[index++] = ((*value) >> bit_shift) & 0xFF;
    }

    return index;
}

uint32_t smpt_extract_uint64_t(uint64_t* const value, const uint8_t buffer[],
                                  uint32_t index)
{
    uint8_t byte_index = 0;
    uint8_t bit_shift = 0;
    *value = 0;
    for (; byte_index < 8; byte_index++)
    {
        bit_shift = (8 - 1 - byte_index) * 8;
        *value |= ((uint64_t)buffer[index++]) << bit_shift;
    }

    return index;
}


uint32_t smpt_insert_double(uint8_t buffer[], const double *const value, uint32_t index)
{
    union Double_container dc;

    if (sizeof(double) != 8)
    {
        smpt_error("smpt_insert_double(): Size of double not 64-bit.");
    }

    dc.value = *value;

    return smpt_insert_uint64_t(buffer, &dc.uint_bytes, index);
}

uint32_t smpt_extract_double(double *const value, const uint8_t buffer[],
                             uint32_t index)
{
    union Double_container dc;
    dc.value = 0;

    if (sizeof(double) != 8)
    {
        smpt_error("smpt_insert_double(): Size of double not 64-bit.");
    }

    index = smpt_extract_uint64_t(&dc.uint_bytes, buffer, index);
    *value =  dc.value;

    return index;
}

uint32_t smpt_insert_float(uint8_t buffer[], const float *const value, uint32_t index)
{
    union Float_container dc;

    if (sizeof(float) != 4)
    {
        smpt_error("smpt_insert_float(): Size of double not 64-bit.");
    }

    dc.value = *value;

    return smpt_insert_uint32_t(buffer, &dc.uint_bytes, index);
}

uint32_t smpt_extract_float(float *const value, const uint8_t buffer[],
                             uint32_t index)
{
    union Float_container dc;
    dc.value = 0;

    if (sizeof(float) != 4)
    {
        smpt_error("smpt_insert_float(): Size of float not 64-bit.");
    }

    index = smpt_extract_uint32_t(&dc.uint_bytes, buffer, index);
    *value =  dc.value;

    return index;
}

uint32_t smpt_insert_array_char(uint8_t buffer[], const char array[], uint32_t array_length,
                                uint32_t index)
{
    return copy_array_char((char*)(&(buffer[index])), array, array_length, index);
}


uint32_t smpt_extract_array_char(char array[], uint32_t array_length, const uint8_t buffer[],
                                 uint32_t index)
{
    return copy_array_char(array, (char*)(&(buffer[index])), array_length, index);
}

uint32_t copy_array_char(char destination[], const char source[], uint32_t array_length,
                         uint32_t index)
{
    memcpy(destination, source, array_length * sizeof(*destination));
    return index + array_length;
}
