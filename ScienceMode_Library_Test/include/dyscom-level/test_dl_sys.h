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
#ifndef TEST_DL_SYS_H
#define TEST_DL_SYS_H

#include "smpt_dl_definitions.h"

void test_dl_sys(void);
void fill_dl_sys(Smpt_dl_sys *const dl_get);
bool compare_dl_sys(const Smpt_dl_sys *const a,
                    const Smpt_dl_sys *const b);

void test_dl_sys_ack(void);
void fill_dl_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack);
bool compare_dl_sys_ack(const Smpt_dl_sys_ack *const a,
                        const Smpt_dl_sys_ack *const b);

bool smpt_compare_string(const char *a, const char *b, uint8_t len);

#endif
