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
#ifndef SMPT_DL_CLIENT_H
#define SMPT_DL_CLIENT_H

/***************************************************************************//**
* \file smpt_dl_client.h
*
* This file contains dyscom level ScienceMode functions to control the dyscom hw
* with an external device. There are simple request response commands and there
* are one way commands which are sometimes used in combination with an initial
* request response command.
*
* Example for simple request response pattern:
*
*     PC <-------------------------> DysCom-HW
*
*         dl_get       ---->  (battery status)
*         dl_get_ack   <----  (3V, 50%)
*
* Example for request response in combination with one way pattern:
*
*     PC <-------------------------> DysCom-HW
*
*         dl_get       ----> (file name)
*         dl_get_ack   <---- (file size = n)
*                      <---- (file block 1)
*                      <---- (file block 2)
*                          .
*                          .
*                          .
*                      <---- (file block n)
*
* Check the Smpt_Result result enumeration member of the *_ack command structs
* of the answers to find out if it is Smpt_Result_Successful.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 Section Windows and Linux
*******************************************************************************/
#if defined(_WIN32) || defined(__linux__)

#include "smpt_definitions.h"
#include "smpt_dl_definitions_data_types.h"
#include "smpt_dl_packet_validity.h"
#include "smpt_client.h"

/***************************************************************************//**
* \brief Builds the dl_init packet and sends it to the serial port.
* dl_init is used to:
*     - initialize a planned measurement (start date and duration)
*     - initialize a live measurement
*     - set the system time of the DysCom-HW
*     - enable/disable storing or live sending of measurement data
*     - configure the ads1294 measurement chip
* \param[in,out] device
* \param[in] @see: dl_init
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_init(Smpt_device *const device, const Smpt_dl_init *const dl_init);

/***************************************************************************//**
* \brief Builds the dl_start packet and sends it to the serial port.
* dl_start is used to start an initialized measurement.
* \param[in,out] device
* \param[in] @see: dl_start
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_start(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the dl_stop packet and sends it to the serial port.
* dl_stop is used to:
*     - abort a running measuremen
*     - abort a running file transfer
*
* * \param[in,out] device
* \param[in] @see: dl_stop
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_stop(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the dl_get packet and sends it to the serial port.
* dl_get is used to get the:
*     - battery status (absolute voltage, percentage value)
*     - file system status (available used SD-Card-Memory)
*     - list of measurement meta information
*     - Operation mode
*     - file by name
*
* \param[in,out] device
* \param[in] @see: dl_get
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_get(Smpt_device *const device, const Smpt_dl_get *const dl_get);

/**** Get data from acks *************************************************/

/***************************************************************************//**
* \brief Writes the last received dl_init_ack response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_init_ack
*******************************************************************************/
SMPT_API bool smpt_get_dl_init_ack(Smpt_device *const device, Smpt_dl_init_ack *const dl_init_ack);

/***************************************************************************//**
* \brief Writes the last received dl_power_module_ack response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_power_module_ack
*******************************************************************************/
SMPT_API bool smpt_get_dl_power_module_ack(Smpt_device *const device, Smpt_dl_power_module_ack *const dl_power_module_ack);

/***************************************************************************//**
* \brief Writes the last received dl_stop_ack response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_init_ack
*******************************************************************************/
SMPT_API bool smpt_get_dl_stop_ack(Smpt_device *const device, Smpt_dl_stop_ack *const dl_stop_ack);

/***************************************************************************//**
* \brief Writes the last received dl_get_ack response in the given struct.
*
* Important! Only the data fields of the corresponding dl_get_ack->get_type
* will contain values different from 0. For instance when you have
*
* dl_get_ack->get_type == Smpt_Dl_Get_Type_Battery_Status
*
* dl_get_ack->battery_status will be filled with data but
* dl_get_ack->deviceId will be 0
*
* \param[in,out] device
* \param[in,out] Smpt_dl_get_ack
*******************************************************************************/
SMPT_API bool smpt_get_dl_get_ack(Smpt_device *const device, Smpt_dl_get_ack *const dl_get_ack);

/***************************************************************************//**
* \brief Writes the last received dl_send_live_data response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_send_live_data
*******************************************************************************/
SMPT_API bool smpt_get_dl_send_live_data(Smpt_device *const device,
                                         Smpt_dl_send_live_data *const dl_send_live_data);

/***************************************************************************//**
* \brief Writes the last received dl_send_file response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_send_file
*******************************************************************************/
SMPT_API bool smpt_get_dl_send_file(Smpt_device *const device,
                                    Smpt_dl_send_file *const dl_send_file);

/***************************************************************************//**
* \brief Writes the last received dl_send_mmi response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_send_mmi
*******************************************************************************/
SMPT_API bool smpt_get_dl_send_mmi(Smpt_device *const device,
                                   Smpt_dl_send_mmi *const dl_send_mmi);

/***************************************************************************//**
* \brief Builds the dl_power_module packet and sends it to the serial port.
* dl_power_module is used to:
*     - power on/off some hardware modules
*       > measurement unit
*       > bluetooth
*       > memory card
*       > research unit
*       > system
* \param[in,out] device
* \param[in] @see: dl_module_power
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_power_module(Smpt_device *const device,
                                        const Smpt_dl_power_module *const dl_power_module);

/***************************************************************************//**
* \brief Builds the dl_send_file_ack packet and sends it to the serial port.
*
* \param[in,out] device
* \param[in] @see: dl_send_file_ack
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_send_file_ack(Smpt_device *const device,
                                         const Smpt_dl_send_file_ack *const dl_send_file_ack);


/***************************************************************************//**
* \brief Builds the dl_sys packet and sends it to the serial port.
* dl_sys is used to make some actions on the remote device, delete, file etc
* \param[in,out] device
* \param[in] @see: dl_sys
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_dl_sys(Smpt_device *const device, const Smpt_dl_sys *const dl_sys);

/***************************************************************************//**
* \brief Writes the last received dl_sys_ack response in the given struct
*
* \param[in,out] device
* \param[in,out] Smpt_dl_sys_ack
*******************************************************************************/
SMPT_API bool smpt_get_dl_sys_ack(Smpt_device *const device, Smpt_dl_sys_ack *const dl_sys_ack);

#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__)*/

#ifdef __cplusplus
}
#endif

#endif /* SMPT_DL_CLIENT_H */
