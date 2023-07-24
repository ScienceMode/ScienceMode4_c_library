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
#ifndef SMPT_ML_PACKET_UTILS_H
#define SMPT_ML_PACKET_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ml_definitions.h"

uint32_t smpt_utils_insert_ml_update(uint8_t buffer_unstuffed[],
                                     const Smpt_ml_update *const ml_update, uint32_t index);

void smpt_utils_extract_ml_update(Smpt_ml_update *const ml_update,
                                  const uint8_t buffer_unstuffed[], uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_ML_PACKET_UTILS_H */
