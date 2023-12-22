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
#ifndef SMPT_CLIENT_H
#define SMPT_CLIENT_H

/***************************************************************************//**
* \file smpt_client.h
*
* This file contains general functions needed to control the stimulator with an
* external device using ScienceMode.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

#ifdef ANDROID
#else

#include "smpt_definitions.h"
#include "smpt_packet_number_generator.h"

/* Serial communication */

/***************************************************************************//**
* \brief Opens the serial port.
*
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] device_name eg. "COM1", "/dev/ttyS0"
*
* \returns True if the port has been opened
*******************************************************************************/
SMPT_API bool smpt_open_serial_port(Smpt_device *const device,
                                    const char *const device_name);


/***************************************************************************//**
* \brief Closes the serial port
*
* \param[in,out] device The device struct is needed for internal handling.
* \returns True if the port has been closed
*******************************************************************************/
SMPT_API bool smpt_close_serial_port(Smpt_device *const device);


/***************************************************************************//**
* \brief Checks if a serial port is available (Windows only)
*
* \param[in] device_name eg. "COM1", "/dev/ttyS0"
* \returns True if the port was opened and closed successfully.
*******************************************************************************/
SMPT_API bool smpt_check_serial_port(const char *const device_name);


/***************************************************************************//**
* \brief Checks if one or more packets have been received since the last call
*
* This function triggers the read of the serial device. The packets are buffered
* internally. If a new packet (ack) is received, use smpt_last_ack() to get more
* information about the packet.
*
* \param[in,out] device The device struct is needed for internal handling.
* \returns True if one or more packets have been received
*******************************************************************************/
SMPT_API bool smpt_new_packet_received(Smpt_device *const device);


/***************************************************************************//**
* \brief Writes the last received command response (ack) in the given struct

* \param[in,out] device The device struct is needed for internal handling.
* \param[in,out] ack
*******************************************************************************/
SMPT_API void smpt_last_ack(Smpt_device *const device, Smpt_ack *const ack);


/***************************************************************************//**
* \brief Builds the get_version packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_version_main(Smpt_device *const device, uint8_t packet_number);


/***************************************************************************//**
* \brief Builds the get_version packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_version_stim(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the get_extended_version packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_extended_version(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the get_device_id packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_device_id(Smpt_device *const device, uint8_t packet_number);


/***************************************************************************//**
* \brief Builds the get_battery_status packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_battery_status(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the send_reset packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_reset(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the get_stim_status packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_stim_status(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Builds the get_main_status packet and sends it to the serial port.
* \param[in,out] device The device struct is needed for internal handling.
* \param[in] packet_number
* \returns True if the command was send successfully
*******************************************************************************/
SMPT_API bool smpt_send_get_main_status(Smpt_device *const device, uint8_t packet_number);

/***************************************************************************//**
* \brief Writes Smpt_get_version_ack response in the given struct.
*
* \param[in,out] device The device struct is needed for internal handling.
* \param[in,out] get_version_ack
*******************************************************************************/
SMPT_API bool smpt_get_get_version_main_ack(Smpt_device *const device,
                                            Smpt_get_version_ack *const get_version_ack);

SMPT_API bool smpt_get_get_version_stim_ack(Smpt_device *const device,
                                            Smpt_get_version_ack *const get_version_ack);
SMPT_API bool smpt_get_get_extended_version_ack(Smpt_device *const device,
                                            Smpt_get_extended_version_ack *const get_extended_version_ack);

/***************************************************************************//**
* \brief Writes Smpt_get_device_id_ack response in the given struct.
*
* \param[in,out] device The device struct is needed for internal handling.
* \param[in,out] get_device_id_ack
*******************************************************************************/
SMPT_API bool smpt_get_get_device_id_ack(Smpt_device *const device,
                                         Smpt_get_device_id_ack *const get_device_id_ack);

/***************************************************************************//**
* \brief Writes Smpt_get_battery_status_ack response in the given struct.
*
* \param[in,out] device The device struct is needed for internal handling.
* \param[in,out] get_battery_status_ack
*******************************************************************************/
SMPT_API bool smpt_get_get_battery_status_ack(Smpt_device *const device,
                                              Smpt_get_battery_status_ack
                                              *const get_battery_status_ack);

/***************************************************************************//**
* \brief Writes Smpt_get_stim_status_ack response in the given struct.
*
* \param[in,out] device The device struct is needed for internal handling.
* \param[in,out] get_stim_status_ack
*******************************************************************************/
SMPT_API bool smpt_get_get_stim_status_ack(Smpt_device *const device,
                                           Smpt_get_stim_status_ack *const get_stim_status_ack);

/***************************************************************************//**
* \brief Writes Smpt_get_main_status_ack response in the given struct.
*
* \param[in,out] device The device struct is needed for internal handling.
* \param[in,out] get_main_status_ack
*******************************************************************************/
SMPT_API bool smpt_get_get_main_status_ack(Smpt_device *const device,
                                           Smpt_get_main_status_ack *const get_main_status_ack);

/***************************************************************************//**
* \brief Returns the library version of the client (not the library version of the device).
*
*******************************************************************************/
SMPT_API Smpt_version smpt_library_version();

#endif /* ANDROID */
#endif /* defined(_WIN32) || defined(__linux__) || defined(__APPLE__)*/

#ifdef __cplusplus
}
#endif

#endif /* SMPT_CLIENT_H */
