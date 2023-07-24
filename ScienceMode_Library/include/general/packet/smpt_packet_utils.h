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
#ifndef SMPT_PACKET_UTILS_H
#define SMPT_PACKET_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint32_t smpt_insert_uint16_t(uint8_t buffer[],
                              const uint16_t* const value,
                              uint32_t index);
uint32_t smpt_extract_uint16_t(uint16_t* const value, const uint8_t buffer[],
                               uint32_t index);

uint32_t smpt_insert_uint32_t(uint8_t buffer[],
                              const uint32_t* const value,
                              uint32_t index);
uint32_t smpt_extract_uint32_t(uint32_t* const value, const uint8_t buffer[],
                               uint32_t index);

uint32_t smpt_insert_uint64_t(uint8_t buffer[],
                              const uint64_t* const value,
                              uint32_t index);
uint32_t smpt_extract_uint64_t(uint64_t* const value, const uint8_t buffer[],
                               uint32_t index);

uint32_t smpt_insert_double(uint8_t buffer[], const double *const value,
                            uint32_t index);
uint32_t smpt_extract_double(double *const value, const uint8_t buffer[],
                             uint32_t index);

uint32_t smpt_insert_float(uint8_t buffer[], const float *const value,
                            uint32_t index);
uint32_t smpt_extract_float(float *const value, const uint8_t buffer[],
                             uint32_t index);

uint32_t smpt_insert_array_char(uint8_t buffer[],
                                const char array[], uint32_t array_length,
                                uint32_t index);
uint32_t smpt_extract_array_char(char array[], uint32_t array_length,
                                 const uint8_t buffer[], uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_UTILS_H */
