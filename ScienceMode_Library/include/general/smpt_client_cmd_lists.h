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
#ifndef SMPT_CLIENT_CMD_LISTS_H
#define SMPT_CLIENT_CMD_LISTS_H

/***************************************************************************//**
* \file smpt_client_cmd_lists.h
*
* This file contains functions to track the sent commands and the received
* responses. It is automatically checked, if the incoming packet is a
* corresponding response to a command sent before.
*
* There are two lists:
*      - the commands (packages) that have sent (requests) to the stimulator and
*      - the responses (acks) that we got from the stimulator.
* The cmd/acks have to be added using the add_* functions.*
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions.h"

/***************************************************************************//**
* \brief Clears the acks and requests
*
* \param[in,out] cmd_list
*******************************************************************************/
void smpt_clear_acks_and_requests(Smpt_cmd_list *const cmd_list);


/***************************************************************************//**
* \brief Adds a command to the list of sent commands
*
* \param[in] cmd
* \param[in] packet_number
* \param[in,out] cmd_list
*******************************************************************************/
void smpt_add_request(Smpt_Cmd cmd, uint8_t packet_number,
                      Smpt_cmd_list *const cmd_list);


/***************************************************************************//**
* \brief Adds an acknowledge to the list of received responses
*
* \param[in] cmd
* \param[in] result
* \param[in] packet_number
* \param[in,out] cmd_list
*******************************************************************************/
void smpt_add_ack(Smpt_Cmd cmd, Smpt_Result result, uint8_t packet_number,
                  Smpt_cmd_list *const cmd_list);


/***************************************************************************//**
* \brief Gets the last received response (ack)
*
* \param[out] ack
* \param[in] new_acks_available
* \param[in,out] cmd_list
*******************************************************************************/
void smpt_get_last_ack(Smpt_ack *const ack, uint32_t new_acks_available,
                       const Smpt_cmd_list *const cmd_list);


/***************************************************************************//**
* \brief Gets the next expected response (ack) according the last send cmd (request)
*
* \param[out] ll_ack
* \param[in,out] cmd_list
*******************************************************************************/
void smpt_get_expected_ack_cmd(Smpt_ack *const ll_ack, Smpt_cmd_list *const cmd_list);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_CLIENT_CMD_LISTS_H */
