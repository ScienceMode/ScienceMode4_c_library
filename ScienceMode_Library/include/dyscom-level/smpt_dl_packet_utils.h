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
#ifndef SMPT_DL_PACKET_UTILS_H
#define SMPT_DL_PACKET_UTILS_H
#include "smpt_dl_definitions.h"
#include "smpt_packet_utils.h"

/**<
 * This file contains utility functions for inserting bytes into a buffer and
 * extracting bytes from a buffer. Here is the scheme for each use case:
 *
 * The string 'ds' stands for data structure. Type and type refer to the data
 * type of the data structure the insert/ectract function corresponds to.
 *
 * Inserting
 * ---------
 *
 * uint32_t smpt_dl_insert_type(uint8_t buffer[],
 *                            const Type *const ds,
 *                            uint32_t index);
 *
 * Buffer is the target buffer where bytes will be writte. The insert
 * function takes the data from ds and puts it into the buffer byte by byte
 * starting at index index in the buffer and returning the incremented index
 * which points to the next position to last inserted byte position.
 *
 * Extracting
 * ----------
 *
 * uint32_t smpt_dl_extract_type(Type *const ds,
 *                             const uint8_t buffer[],
 *                             uint32_t index);
 *
 * The extract functions take the bytes from the buffer starting at position
 * index and fill the data structure ds with these bytes.
 *
 */

uint32_t smpt_dl_insert_ads129x(uint8_t buffer[], const Smpt_dl_ads129x *const dl_ads129x,
                               uint32_t index);
uint32_t smpt_dl_extract_ads129x(Smpt_dl_ads129x *const dl_ads129x, const uint8_t buffer_unstuffed[],
                               uint32_t index);

uint32_t smpt_dl_insert_tm(uint8_t buffer[], const Smpt_dl_tm *const dl_tm,
                               uint32_t index);
uint32_t smpt_dl_extract_tm(Smpt_dl_tm *const dl_tm, const uint8_t buffer_unstuffed[],
                               uint32_t index);

uint32_t smpt_dl_insert_init(uint8_t buffer_unstuffed[],
                             const Smpt_dl_init *const dl_init,
                             uint32_t index);

uint32_t smpt_dl_extract_init_(Smpt_dl_init *const dl_init,
                              const uint8_t buffer_unstuffed[]);

uint32_t smpt_dl_insert_string(uint8_t buffer[],
                               const char* const string,
                               uint32_t index,
                               uint8_t str_length);
uint32_t smpt_dl_extract_string(char* const string,
                                const uint8_t buffer_unstuffed[],
                                uint32_t index,
                                uint8_t str_length);

uint32_t smpt_dl_insert_patient_name(uint8_t buffer[], const char* const patient_name,
                                     uint32_t index);
uint32_t smpt_dl_extract_patient_name(char* const patient_name, const uint8_t buffer_unstuffed[],
                                     uint32_t index);

uint32_t smpt_dl_insert_investigator_name(uint8_t buffer[], const char* const investigator_name,
                                     uint32_t index);
uint32_t smpt_dl_extract_investigator_name(char* const investigator_name, const uint8_t buffer_unstuffed[],
                                     uint32_t index);

uint32_t smpt_dl_insert_patient_number(uint8_t buffer[],
                                       const char* const patient_number,
                                       uint32_t index);
uint32_t smpt_dl_extract_patient_number(char* const patient_number,
                                        const uint8_t buffer_unstuffed[],
                                        uint32_t index);

uint32_t smpt_dl_insert_measurement_number(uint8_t buffer[],
                                       const uint16_t* const measurement_number,
                                       uint32_t index);
uint32_t smpt_dl_extract_measurement_number(uint16_t* const measurement_number,
                                        const uint8_t buffer_unstuffed[],
                                        uint32_t index);

uint32_t smpt_dl_insert_battery_status(uint8_t buffer[],
                                       const Smpt_dl_battery_status* const battery_status,
                                       uint32_t index);
uint32_t smpt_dl_extract_battery_status(Smpt_dl_battery_status* const battery_status,
                                        const uint8_t buffer[], uint32_t index);

uint32_t smpt_dl_insert_file_sizes (uint8_t buffer[],
                                    const uint64_t *size,
                                    const uint32_t *n_blocks, uint32_t index);
uint32_t smpt_dl_extract_file_sizes (uint64_t *size,
                                     uint32_t *n_blocks,
                                     const uint8_t buffer[], uint32_t index);

uint32_t smpt_dl_insert_block_offset (uint8_t buffer[],
                                      const uint32_t *block_offset,
                                      uint32_t index);
uint32_t smpt_dl_extract_block_offset (uint32_t *block_offset,
                                       const uint8_t buffer[],
                                       uint32_t index);

uint32_t smpt_dl_insert_file_id(uint8_t buffer[],
                                const char file_id[SMPT_DL_MAX_FILE_ID_LENGTH],
                                  uint32_t index);
uint32_t smpt_dl_extract_file_id(char file_id[SMPT_DL_MAX_FILE_ID_LENGTH],
                                 const uint8_t buffer_unstuffed[],
                                 uint32_t index);

uint32_t smpt_dl_insert_file_size (uint8_t buffer[],
                                   const uint64_t *size,
                                   uint32_t index);
uint32_t smpt_dl_extract_file_size (uint64_t *size,
                                    const uint8_t buffer[],
                                    uint32_t index);

uint32_t smpt_dl_insert_file_system_status(uint8_t buffer[],
                                       const Smpt_dl_file_system_status* const file_system_status,
                                       uint32_t index);
uint32_t smpt_dl_extract_file_system_status(Smpt_dl_file_system_status* const file_system_status,
                                        const uint8_t buffer[], uint32_t index);

uint32_t smpt_dl_insert_electrode_samples(uint8_t buffer[],
                                       const Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS],
                                       uint32_t index, uint8_t n_channels);
uint32_t smpt_dl_extract_electrode_samples(Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS],
                                        const uint8_t buffer[], uint32_t index, uint8_t n_channels);


uint32_t smpt_dl_insert_time_length (uint8_t buffer[],
                                     const uint32_t *length,
                                     uint32_t index);
uint32_t smpt_dl_extract_time_length (uint32_t *length,
                                      const uint8_t buffer[],
                                      uint32_t index);

uint32_t smpt_dl_insert_number_of_channels (uint8_t buffer[],
                                     const uint16_t *n_channels,
                                     uint32_t index);
uint32_t smpt_dl_extract_number_of_channels (uint16_t *n_channels,
                                      const uint8_t buffer[],
                                      uint32_t index);

uint32_t smpt_dl_insert_duration_of_planned_m (uint8_t buffer[],
                                     const uint32_t *duration,
                                     uint32_t index);
uint32_t smpt_dl_extract_duration_of_planned_m (uint32_t *duration,
                                      const uint8_t buffer[],
                                      uint32_t index);

uint32_t smpt_dl_insert_power_module(uint8_t buffer_unstuffed[],
                             const Smpt_dl_power_module *const dl_power_module,
                             uint32_t index);

uint32_t smpt_dl_extract_power_module(Smpt_dl_power_module *const dl_power_module,
                                      const uint8_t buffer_unstuffed[]);

uint32_t smpt_dl_insert_file_by_name(uint8_t buffer[],
                                     const Smpt_dl_file_by_name * const file_by_name,
                                     uint32_t index);

uint32_t smpt_dl_extract_file_by_name(Smpt_dl_file_by_name * const file_by_name,
                                      const uint8_t buffer[],
                                      uint32_t index);
uint32_t smpt_dl_insert_file_name(uint8_t buffer[],
                                const char file_name[SMPT_DL_MAX_FILE_NAME_LENGTH],
                                uint32_t index);
uint32_t smpt_dl_extract_file_name(char file_name[SMPT_DL_MAX_FILE_NAME_LENGTH],
                                 const uint8_t buffer_unstuffed[],
                                 uint32_t index);
uint32_t smpt_dl_insert_send_file_ack(uint8_t buffer_unstuffed[],
                                      const Smpt_dl_send_file_ack * const dl_send_file_ack,
                                      uint32_t index);

uint32_t smpt_dl_extract_send_file_ack(Smpt_dl_send_file_ack * const dl_send_file_ack,
                                      const uint8_t buffer[], uint32_t index);

uint32_t smpt_dl_insert_file_info(uint8_t buffer[],
                                     const Smpt_dl_file_info * const dl_file_info,
                                     uint32_t index);

uint32_t smpt_dl_extract_file_info(Smpt_dl_file_info * const dl_file_by_info,
                                      const uint8_t buffer[],
                                      uint32_t index);

uint32_t smpt_dl_insert_sys(uint8_t buffer_unstuffed[],
                            const Smpt_dl_sys * const dl_sys, uint32_t index);

uint32_t smpt_dl_extract_sys(Smpt_dl_sys * const dl_sys,
                             const uint8_t buffer[],
                             uint32_t index);

uint32_t smpt_dl_insert_sys_ack(uint8_t buffer_unstuffed[],
                                const Smpt_dl_sys_ack * const dl_sys_ack,
                                uint32_t index);

uint32_t smpt_dl_extract_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack,
                                   const uint8_t buffer[], uint32_t index);

uint32_t smpt_dl_insert_channel_type(uint8_t buffer_unstuffed[], const Smpt_Dl_Signal_Type dl_channel_type[], uint32_t dl_channel_type_length, uint32_t index);

uint32_t smpt_dl_extract_channel_type(Smpt_Dl_Signal_Type dl_channel_type[], const uint8_t buffer_unstuffed[], uint32_t dl_channel_type_length, uint32_t index);

#endif /* SMPT_DL_PACKET_UTILS_H */
