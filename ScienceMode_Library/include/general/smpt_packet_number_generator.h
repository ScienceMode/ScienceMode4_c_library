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
#ifndef SMPT_PACKET_NUMBER_GENERATOR_H
#define SMPT_PACKET_NUMBER_GENERATOR_H

/***************************************************************************//**
* \file smpt_packet_number_generator.h
*
* This file defines a simple packet number generator. It generates packet
* numbers 0-63 in ascending order.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

/***************************************************************************//**
* \brief Resets the packet number generator. The next packet number is to 0.
*
* \param[in,out] device The device struct is needed for internal handling.
*******************************************************************************/
void smpt_packet_number_generator_reset(Smpt_device *const device);

/***************************************************************************//**
* \brief Returns the next packet number
*
* \param[in,out] device The device struct is needed for internal handling.
* \returns The next packet number (0-63)
*******************************************************************************/
uint8_t smpt_packet_number_generator_next(Smpt_device *const device);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_PACKET_NUMBER_GENERATOR_H */

