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
#ifndef SMPT_DEFINITIONS_POWER_H
#define SMPT_DEFINITIONS_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

typedef enum
{
    Smpt_Power_Uc_Stim      = 0,
    Smpt_Power_Bluetooth    = 1,
    Smpt_Power_Emg          = 2,
    Smpt_Power_Memory_Card  = 3,
    Smpt_Power_Demux        = 4

} Smpt_Power; /**< \enum Smpt_Power
                *   Modules that can be switched on and off */

typedef struct /* Smpt_set_power */
{
    uint8_t packet_number;
    bool    modules[Smpt_Length_Power_Modules];

} Smpt_set_power; /**< \struct Smpt_set_power */

SMPT_API void smpt_clear_set_power(Smpt_set_power *const set_power);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_DEFINITIONS_POWER_H*/

