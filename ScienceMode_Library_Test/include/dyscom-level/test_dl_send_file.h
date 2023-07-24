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
#ifndef TEST_DL_SEND_FILE_H
#define TEST_DL_SEND_FILE_H

#include "smpt_dl_definitions.h"

void test_dl_send_file(void);
void fill_dl_send_file(Smpt_dl_send_file *const dl_send_file);
bool compare_dl_send_file(const Smpt_dl_send_file *const a,
                          const Smpt_dl_send_file *const b);
void test_dl_send_file_ack();
void fill_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack);
bool compare_dl_send_file_ack(const Smpt_dl_send_file_ack *const a,
                              const Smpt_dl_send_file_ack *const b);

#endif
