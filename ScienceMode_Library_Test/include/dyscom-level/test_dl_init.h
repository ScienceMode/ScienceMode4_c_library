/***************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2022, MPL and LGPL HASOMED GmbH
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
#ifndef TEST_DL_INIT_H
#define TEST_DL_INIT_H

#include "smpt_dl_definitions.h"

void test_dl_init(void);
void fill_dl_init(Smpt_dl_init *const dl_init);
void fill_dl_ads129x(Smpt_dl_ads129x *const ads129x);
void fill_dl_tm(Smpt_dl_tm *const tm);

/*
 *   (a->packet_number == b->packet_number)
 * the packet_number is evil because it is handled differently from
 * the other struct parameters.
 */
bool compare_dl_init(const Smpt_dl_init *const dl_init_a,
                     const Smpt_dl_init *const dl_init_b);
bool compare_dl_ads129x(const Smpt_dl_ads129x *const dl_ads129x_a,
                        const Smpt_dl_ads129x *const dl_ads129x_b);

bool compare_dl_tm(const Smpt_dl_tm *const dl_tm_a,
                   const Smpt_dl_tm *const dl_tm_b);


void test_dl_init_ack(void);
void fill_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack);
bool compare_dl_init_ack(const Smpt_dl_init_ack *const a,
                         const Smpt_dl_init_ack *const b);

bool compare_dl_patient_name(const char a[SMPT_DL_MAX_PATIENT_NAME_LENGTH+1],
                             const char b[SMPT_DL_MAX_PATIENT_NAME_LENGTH+1]);

bool compare_dl_investigator_name(const char a[SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1],
                                  const char b[SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1]);

bool compare_dl_file_id(const char a[SMPT_DL_MAX_FILE_ID_LENGTH],
                        const char b[SMPT_DL_MAX_FILE_ID_LENGTH]);

bool compare_dl_patient_number (const char a[SMPT_DL_GUID_STRING_LENGTH+1],
                                const char b[SMPT_DL_GUID_STRING_LENGTH+1]);

bool compare_dl_channel_type (const char a[SMPT_DL_MAX_CHANNELS],
                              const char b[SMPT_DL_MAX_CHANNELS]);

#endif /* TEST_DL_INIT_H */
