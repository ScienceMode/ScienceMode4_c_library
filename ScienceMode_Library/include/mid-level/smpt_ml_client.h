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
#ifndef SMPT_ML_CLIENT_H
#define SMPT_ML_CLIENT_H

/***************************************************************************//**
* \file smpt_ml_client.h
*
* This file contains mid level ScienceMode functions to control the stimulator
* with an external device.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 Section Windows and Linux
*******************************************************************************/
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

#include "smpt_definitions.h"
#include "smpt_ml_definitions_data_types.h"
#include "smpt_ml_packet_validity.h"
#include "smpt_messages.h"
#include "smpt_client.h"

/***************************************************************************//**
* \brief Builds the Ml_init packet and sends it to the serial port.
* This command switches on the high voltage for both modules (execution duration
* 2 x 40 ms).
*
* \param[in,out] device
* \param[in] ml_init
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ml_init(Smpt_device *const device, const Smpt_ml_init *const ml_init);


/***************************************************************************//**
* \brief Builds the Ml_update packet and sends it to the serial port.
*
* This command starts or updates the stimulation using the parameters.
*
* \param[in,out] device
* \param[in] ml_update
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ml_update(Smpt_device *const device,
                                  const Smpt_ml_update *const ml_update);


/***************************************************************************//**
* \brief Builds the Ml_get_current_data packet and sends it to the serial port.
*
* This command is used as a keep-alive signal. After the start of the
* stimulation (Ml_update) you need to send this command at least every 2 seconds.
* The response contains general and requested status information.
*
* \param[in,out] device
* \param[in] ml_get_current_data
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ml_get_current_data(Smpt_device *const device,
                                            const Smpt_ml_get_current_data
                                            *const ml_get_current_data);


/***************************************************************************//**
* \brief Builds the Ml_stop packet and sends it to the serial port.
* This command switches off the high voltage for both modules (execution duration
* 40 ms).
*
* \param[in,out] device
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_ml_stop(Smpt_device *const device, uint8_t packet_number);


/***************************************************************************//**
* \brief Write the last received ml_get_current_data_ack in the given struct
*
* \param[in,out] device
* \param[in,out] ml_get_current_data_ack
*******************************************************************************/
SMPT_API bool smpt_get_ml_get_current_data_ack(Smpt_device *const device,
                                               Smpt_ml_get_current_data_ack *const
                                               ml_get_current_data_ack);

#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__) */

#ifdef __cplusplus
}
#endif

#endif /* SMPT_ML_CLIENT_H */
