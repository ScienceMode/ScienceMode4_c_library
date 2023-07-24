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
#include "smpt_ml_definitions_data_types.h"
#include "smpt_ll_definitions_data_types.h"

void smpt_clear_ml_init(Smpt_ml_init *const ml_init)
{
    Smpt_ml_init empty = {0};
    *ml_init = empty;
}

void smpt_clear_ml_update(Smpt_ml_update *const ml_update)
{
    Smpt_ml_update empty = {0};
    *ml_update = empty;
}

void smpt_clear_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data)
{
    Smpt_ml_get_current_data empty = {0};
    *ml_get_current_data = empty;
}

void smpt_clear_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                        *const ml_get_current_data_ack) {
    Smpt_ml_get_current_data_ack empty = {0};
    *ml_get_current_data_ack = empty;
}

void smpt_clear_ml_ack(Smpt_ml_ack *const ml_ack)
{
    Smpt_ml_ack empty = {0};
    *ml_ack = empty;
}
