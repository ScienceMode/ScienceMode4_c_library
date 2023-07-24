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
#ifndef SMPT_LL_DEFINITIONS_DATA_TYPES_H
#define SMPT_LL_DEFINITIONS_DATA_TYPES_H

/***************************************************************************//**
* \file smpt_ll_definitions_data_types.h
*
* This file defines the data types for the low level protocol.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions_data_types.h"

typedef enum /* Ll_Interpolation_Mode */
{
    Smpt_Ll_Interpolation_Jump   = 0,   /**< No interpolation between the points */
    Smpt_Ll_Interpolation_Linear = 1,   /**< Not yet implemented */
    /* Adjust the following item if another enum item is added */
    Smpt_Ll_Interpolation_Last_Item = Smpt_Ll_Interpolation_Linear

} Smpt_Ll_Interpolation_Mode; /**< \enum Smpt_Ll_Interpolation_Mode The interpolation modes in
                                         Ll_channel_config. \see Smpt_Ll_channel_config */

typedef struct /* Ll_init */
{
    Smpt_Measurement  measurement_type;        /**< Not yet implemented */
    Smpt_High_Voltage high_voltage_level; /**< Sets the high voltage level \see Smpt_High_Voltage */
    uint8_t packet_number;                /**< [0 - 63] Packet number */

} Smpt_ll_init; /**< \struct Smpt_ll_init
                     Struct for the Ll_init command. \see smpt_send_ll_init() */

typedef struct /* Ll_init_ack */
{
    uint8_t packet_number;    /**< Packet number echo */
    Smpt_Result result;       /**< \see Smpt_Result */

} Smpt_ll_init_ack; /**< \struct Smpt_ll_init_ack
                     *   Struct for the Ll_init_ack response.
                     *   \see smpt_get_ll_init_ack() */


typedef Smpt_point Smpt_ll_point; /**< \struct Smpt_ll_init_ack
                                       \see Smpt_point */


typedef struct /* Ll_channel_config */
{
    bool enable_stimulation;   /**< Value | Description
                                    ----- | -----------
                                    False | Do not transfer stimulation points, no stimulation
                                    True  | Transfer points, the points are executed.  */
    Smpt_Channel channel;      /**< [0 - 3] Channel number \n \see Smpt_Channel */
    Smpt_Connector connector;  /**< [0 - 1] Connector number \n \see Smpt_Connector */
    uint8_t number_of_points;  /**< [1 - 16] Number of points */
    Smpt_ll_point points[Smpt_Length_Points]; /**< Stimulation points \see Smpt_point */
    uint8_t packet_number;     /**< [0 - 63] Packet number */

} Smpt_ll_channel_config; /**< \struct Smpt_ll_channel_config
                               Struct for the Ll_channel_config command.
                               \see smpt_send_ll_channel_config() */


typedef struct /* Smpt_Ll_channel_config_ack */
{
    uint8_t packet_number;      /**< Packet number echo */
    Smpt_Result result;         /**< \see Smpt_Result */
    Smpt_Channel channel;       /**< Contains the channel. \see Smpt_Channel*/
    Smpt_Connector connector;   /**< Contains the connector. \see Smpt_Connector*/
    Smpt_Measurement  measurement_type;        /**< Echo of the measurement type \see Smpt_Measurement*/
    uint16_t measurement_sampling_time_us;     /**< Sampling time in microseconds */
    uint16_t measurement[128];                 /**< Measurement data, needs to be mulitplied with 100*/
} Smpt_ll_channel_config_ack;  /**< \struct Smpt_ll_channel_config_ack
                                *   Struct for the Ll_channel_config_ack response.
                                *   \see smpt_get_ll_channel_config_ack() */


typedef Smpt_ack Smpt_ll_ack;  /**< \struct Smpt_ll_ack
                                    \see Smpt_ack */



/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in,out] ll_init
*******************************************************************************/
SMPT_API void smpt_clear_ll_init(Smpt_ll_init *const ll_init);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in,out] ll_init_ack
*******************************************************************************/
SMPT_API void smpt_clear_ll_init_ack(Smpt_ll_init_ack *const ll_init_ack);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in,out] ll_channel_config
*******************************************************************************/
SMPT_API void smpt_clear_ll_channel_config(Smpt_ll_channel_config *const ll_channel_config);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in,out] ll_channel_config_ack
*******************************************************************************/
SMPT_API void smpt_clear_ll_channel_config_ack(Smpt_ll_channel_config_ack *const ll_channel_config_ack);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in,out] ll_ack
*******************************************************************************/
SMPT_API void smpt_clear_ll_ack(Smpt_ll_ack *const ll_ack);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in,out] ll_point
*******************************************************************************/
SMPT_API void smpt_clear_ll_point(Smpt_ll_point *const ll_point);


#ifdef __cplusplus
}
#endif

#endif /* SMPT_LL_DEFINITIONS_DATA_TYPES_H */
