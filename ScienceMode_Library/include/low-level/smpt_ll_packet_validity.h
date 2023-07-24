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
#ifndef SMPT_LL_PACKET_VALIDITY_H
#define SMPT_LL_PACKET_VALIDITY_H

/***************************************************************************//**
* \file smpt_ll_packet_validity.h
*
* This file contains functions for validity checks of the command structs.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ll_definitions.h"

/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ll_init
* \returns True if all parameters are valid.
*******************************************************************************/
SMPT_API bool smpt_is_valid_ll_init(const Smpt_ll_init *const ll_init);


/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ll_init_ack
* \returns True if all parameters are valid.
*******************************************************************************/
SMPT_API bool smpt_is_valid_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack);


/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ll_channel_config
* \returns True if all parameters are valid.
*******************************************************************************/
SMPT_API bool smpt_is_valid_ll_channel_config(const Smpt_ll_channel_config
                                              *const ll_channel_config);

/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ll_channel_config_ack
* \returns True if all parameters are valid.
*******************************************************************************/
SMPT_API bool smpt_is_valid_ll_channel_config_ack(const Smpt_ll_channel_config_ack
                                                  *const ll_channel_config_ack);


/***************************************************************************//**
* \brief Checks the validity of the passed struct.
*
* \param[in] ll_point
* \param[in] index index of the point for debugging purposes
* \returns True if all parameters are valid.
*******************************************************************************/
SMPT_API bool smpt_is_valid_point(const Smpt_ll_point *const ll_point, uint8_t index);


#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_PACKET_VALIDITY_H */
