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
#ifndef SMPT_ML_PACKET_VALIDITY_H
#define SMPT_ML_PACKET_VALIDITY_H

/***************************************************************************//**
* \file smpt_ml_packet_validity.h
*
* This file contains functions for validity checks of the mid-level command
* structs.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ml_definitions_data_types.h"

/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ml_init
* \returns True if all parameters are valid.
*******************************************************************************/
bool smpt_is_valid_ml_init(const Smpt_ml_init *const ml_init);


/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ml_update
* \returns True if all parameters are valid.
*******************************************************************************/
bool smpt_is_valid_ml_update(const Smpt_ml_update *const ml_update);


/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ml_get_current_data
* \returns True if all parameters are valid.
*******************************************************************************/
bool smpt_is_valid_ml_get_current_data(const Smpt_ml_get_current_data *const ml_get_current_data);


/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ml_get_current_data_ack
* \returns True if all parameters are valid.
*******************************************************************************/
bool smpt_is_valid_ml_get_current_data_ack(const Smpt_ml_get_current_data_ack
                                           *const ml_get_current_data_ack);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_ML_PACKET_VALIDITY_H */
