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
#ifndef TEST_DL_EXAMPLES_H
#define TEST_DL_EXAMPLES_H

#include "smpt_dl_definitions.h"
#include "smpt_dl_definitions_data_types.h"

int test_simple_measurement(void);
void measurement(Smpt_device *const device);
void fill_dl_init_examples(Smpt_device *const device, Smpt_dl_init *const dl_init);

void fill_dl_init_test_signal(Smpt_dl_init *const dl_init);
void fill_dl_init_bi_simple(Smpt_dl_init *const dl_init);
void fill_dl_power_module_examples(Smpt_device *const device, Smpt_dl_power_module* const dl_power_module);

void wait_for_response(Smpt_device *const device, Smpt_Cmd cmd);

void get_dl_live_data(Smpt_device *const device,
                             Smpt_dl_send_live_data *const dl_live_data);

#endif /* TEST_DL_EXAMPLES_H */

