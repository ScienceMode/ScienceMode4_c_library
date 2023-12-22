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
#ifndef SMPT_LL_CLIENT_H
#define SMPT_LL_CLIENT_H

/***************************************************************************//**
* \file smpt_ll_client.h
*
* This file contains low level ScienceMode functions to control the stimulator
* with an external device.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

#include "smpt_ll_definitions.h"
#include "smpt_messages.h"
#include "smpt_ll_packet_validity.h"
#include "smpt_client.h"

/* Send commands */
/***************************************************************************//**
* \brief Builds the Ll_init packet and sends it to the device.
* This command switches on the high voltage (execution duration 40 ms).
*
* \param[in,out] device
* \param[in] ll_init
*
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ll_init(Smpt_device *const device,
                                const Smpt_ll_init *const ll_init);


/***************************************************************************//**
* \brief Builds the Ll_channel_config packet and sends it to the device.
*
* Send this command periodically to set the frequency of your choice. You can also
* send commands shortly after another. The stimulator will internally buffer the
* command and execute it as fast as possible.
* It is recommended, but not necessary to wait for the response of the command.
*
* \param[in,out] device
* \param[in] ll_channel_config
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ll_channel_config(Smpt_device *const device,
                                          const Smpt_ll_channel_config *const ll_channel_config);


/***************************************************************************//**
* \brief Builds the Ll_stop packet and sends it to the device.
* This command switches off the high voltage (execution duration 40 ms).
*
* \param[in,out] device
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ll_stop(Smpt_device *const device,
                                uint8_t packet_number);


/**** Acks ***************************************************************/

/**** Get data from acks *************************************************/
/***************************************************************************//**
* \brief Writes the received ll_init_ack response in the given struct.
*
* \param[in,out] device
* \param[in,out] ll_init_ack
*******************************************************************************/
SMPT_API bool smpt_get_ll_init_ack(const Smpt_device *const device,
                                   Smpt_ll_init_ack *const ll_init_ack);

/***************************************************************************//**
* \brief Write the received ll_channel_config_ack in the given struct

* \param[in,out] device
* \param[in,out] ll_channel_config_ack
*******************************************************************************/
SMPT_API bool smpt_get_ll_channel_config_ack(const Smpt_device *const device,
                 Smpt_ll_channel_config_ack *const ll_channel_config_ack);


#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */

#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_CLIENT_H */
