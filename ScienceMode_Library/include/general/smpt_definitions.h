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
#ifndef SMPT_DEFINITIONS_H
#define SMPT_DEFINITIONS_H

/***************************************************************************//**
* \file smpt_definitions.h
*
* This is the main science mode include file.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions_data_types.h"

typedef enum /* Smpt_General */
{
    Smpt_Library_Version_Major     = 4,  /**< Used for interface changes that breaks backward
                                              compatibility */
    Smpt_Library_Version_Minor     = 0,  /**< Used for adding commands, parameters\n
                                              The interface is backward compatible. */
    Smpt_Library_Version_Revision  = 0   /**< Used for bugfixes, no interface changes */

} Smpt_General; /**< \enum Smpt_General General protocol and library constants */

#ifdef __cplusplus
}
#endif

#endif /* SMPT_DEFINITIONS_H */
