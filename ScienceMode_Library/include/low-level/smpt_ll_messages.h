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
#ifndef SMPT_LL_MESSAGES_H
#define SMPT_LL_MESSAGES_H

/***************************************************************************//**
* \file smpt_ll_messages.h
*
* This file defines functions to print he low-level cmd structs to stdout.
* messages. The functions do nothing empty if the compiled system is not
* Windows or Linux.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_ll_definitions_data_types.h"

/***************************************************************************//**
* \brief Prints the struct parameters to stdout.
*
* \param[in] ll_init
*******************************************************************************/
SMPT_API void smpt_print_ll_init(const Smpt_ll_init *const ll_init);


/***************************************************************************//**
* \brief Prints the struct parameters to stdout.
*
* \param[in] ll_init_ack
*******************************************************************************/
SMPT_API void smpt_print_ll_init_ack(const Smpt_ll_init_ack *const ll_init_ack);


/***************************************************************************//**
* \brief Prints the struct parameters to stdout.
*
* \param[in] ll_channel_config
*******************************************************************************/
SMPT_API void smpt_print_ll_channel_config(const Smpt_ll_channel_config *const ll_channel_config);


/***************************************************************************//**
* \brief Prints the struct parameters to stdout.
*
* \param[in] ll_channel_config_ack
*******************************************************************************/
SMPT_API void smpt_print_ll_channel_config_ack(const Smpt_ll_channel_config_ack *const ll_channel_config_ack);


/***************************************************************************//**
* \brief Prints the struct parameters to stdout.
*
* \param[in] ll_ack
*******************************************************************************/
SMPT_API void smpt_print_ll_ack(const Smpt_ll_ack *const ll_ack);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_MESSAGES_H */
