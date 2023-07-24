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
#ifndef SMPT_ML_DEFINITIONS_DATA_TYPES_H
#define SMPT_ML_DEFINITIONS_DATA_TYPES_H

/***************************************************************************//**
* \file smpt_ml_definitions_data_types.h
*
* This file defines the data types of the protocol, which are related to the
* mid-level.
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "smpt_definitions_data_types.h"

typedef struct /* Ml_init */
{
    bool stop_all_channels_on_error; /**< False - Only the channel with an error is stopped, \n
                                          True  - All channels are stopped in case of an error */
    uint8_t packet_number;           /**< [0-63] Packet number */

} Smpt_ml_init; /**< \struct Smpt_ml_init
                     Struct for the Ml_init command. \see smpt_send_ml_init() */

typedef Smpt_ack Smpt_ml_ack; /**< \struct Smpt_ml_ack
                                   \see Smpt_ack */

typedef Smpt_point Smpt_ml_point; /**< \struct Smpt_ml_point
                                       \see Smpt_point */

typedef struct /* Ml_channel_config */
{
    uint8_t ramp;                      /**< [0-15] pulses \n
                                            Number of linear increasing lower current pulse pattern
                                            until the full current is reached\n
                                            The ramp is excecuted if the channel is enabled. */
    float period;                      /**< [0,5–16383] ms \n ([<0.1-2000] Hz)
                                            Time between two pulse patterns */
    bool double_period_accuracy;       /**< False - Period accuracy is 0.5 ms with range  [0,5–16383] ms,\n
                                            True - Period accuracy is 0.25 ms with range  [0,25–8191,5] ms*/
    uint8_t number_of_points;          /**< [1 - 16] Number of points */
    Smpt_ml_point points[Smpt_Length_Points];
                                       /**< Stimulation points \see Smpt_point */


} Smpt_ml_channel_config; /**< \struct Smpt_ml_channel_config
                               Channel configuration for one channel. */

typedef struct /* Ml_update */
{
    bool enable_channel[Smpt_Length_Number_Of_Channels];
                                        /**< Enable/Disable each channel  \n
                                          *  e.g. enable_channel[Smpt_Channel_Red] = true; \n
                                          *  to enable channel red \n
                                          *  False - The channel is disabled, \n
                                          *  True - The channel is enabled */
    Smpt_ml_channel_config channel_config[Smpt_Length_Number_Of_Channels];
                                          /**< Channel configuration for each channel */
    uint8_t packet_number;                /**< [0 - 63] Packet number */

} Smpt_ml_update; /**< \struct Smpt_ml_update
                       Struct for the Ml_update command. \see smpt_send_ml_update() */

typedef enum /* Ml_Data */
{
    Smpt_Ml_Data_Stimulation                  = 2,  /**< Returns electrode error for the first 4 stimulation channels*/
    Smpt_Ml_Data_Stimulation_Eight_Channels   = 3,  /**< Returns electrode error for up to 8 stimulation channels*/
    Smpt_Ml_Data_Channels                     = 4,  /**< Returns all errors for up to 8 stimulation channels*/
} Smpt_Ml_Data; /**< \enum Smpt_Ml_Data */

typedef struct /* Ml_get_current_data */
{
    uint8_t packet_number;           /**< [0 - 63] Packet number */
    Smpt_Ml_Data data_selection;     /**< Selection of the requested data, \see Smpt_Ml_Data */

} Smpt_ml_get_current_data; /**< \struct Smpt_ml_get_current_data
                                 Struct for the Ml_get_current_data command.
                                 \see smpt_send_ml_get_current_data() */

typedef enum /* Ml_Stimulation_State */
{
    Smpt_Ml_Stimulation_Stopped = 0,
    Smpt_Ml_Stimulation_Running = 1,

    Smpt_Ml_Stimulation_Last_Item = 1
} Smpt_Ml_Stimulation_State; /**< \enum Smpt_Ml_Stimulation_State */


typedef enum /* Smpt_Ml_Channel_State */
{
    Smpt_Ml_Channel_State_Ok                  = 0,   /**<  No Error occured*/
    Smpt_Ml_Channel_State_Electrode_Error     = 1,   /**< Measured load indicates that the electrode has falled off*/
    Smpt_Ml_Channel_State_Timeout_Error       = 2,   /**< Period of the pulse cannot be achieved*/
    Smpt_Ml_Channel_State_Low_Current_Error   = 3, /**< Selected pulse current cannot be achived*/
    Smpt_Ml_Channel_State_Last_Item      = Smpt_Ml_Channel_State_Low_Current_Error

} Smpt_Ml_Channel_State; /**< \enum Smpt_Ml_Channel_State  */

typedef struct /* Smpt_ml_stimulation_data */
{
    bool is_valid; /**< Is true when valid (Smpt_Ml_Data_Stimulation and Smpt_Ml_Data_Stimulation_Eight_Channels mode)*/
    bool electrode_error[Smpt_Length_Number_Of_Channels]; /**< False - No electrode error \n
                                                               True - Electrode error \n
                                                               or channels. */
    Smpt_Ml_Stimulation_State stimulation_state; /**< \see Current stimulation state */

} Smpt_ml_stimulation_data;    /**< \struct Smpt_ml_stimulation_data
                                    Contains the current stimulation data. */

typedef struct /* Smpt_ml_stimulation_channels_data */
{
    bool is_valid; /**< Is true when valid (Smpt_Ml_Data_Channels)*/
    Smpt_Ml_Channel_State channel_state[Smpt_Length_Number_Of_Channels];
    Smpt_Ml_Stimulation_State stimulation_state[Smpt_Length_Number_Of_Channels]; /**< \see Current stimulation state */

} Smpt_ml_stimulation_channels_data;    /**< \struct Smpt_ml_stimulation_channels_data
                                    Contains the current stimulation data. */

typedef struct /* Ml_get_current_data_ack */
{
    uint8_t packet_number;              /**< [0 - 63] Echo packet number */
    /* general */
    Smpt_Result result;                 /**< \see Smpt_Result */
    Smpt_Ml_Data data_selection;        /**< \see Smpt_ml_get_current_data */

    /* optional */
    Smpt_ml_stimulation_data stimulation_data;  /**< \see Smpt_ml_stimulation_data */
    Smpt_ml_stimulation_channels_data channel_data;

} Smpt_ml_get_current_data_ack; /**< \struct Smpt_ml_get_current_data_ack
                                     Struct for the Ml_get_current_data_ack command.
                                     \see smpt_get_ml_get_current_data_ack() */


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in] ml_init
*******************************************************************************/
SMPT_API void smpt_clear_ml_init(Smpt_ml_init *const ml_init);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in] ml_update
*******************************************************************************/
SMPT_API void smpt_clear_ml_update(Smpt_ml_update *const ml_update);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in] ml_get_current_data
*******************************************************************************/
SMPT_API void smpt_clear_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data);


/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in] ml_get_current_data_ack
*******************************************************************************/
SMPT_API void smpt_clear_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                                 *const ml_get_current_data_ack);

/***************************************************************************//**
* \brief Clears the struct (sets all values to 0).
*
* \param[in] ml_ack
*******************************************************************************/
SMPT_API void smpt_clear_ml_ack(Smpt_ml_ack *const ml_ack);

#ifdef __cplusplus
}
#endif

#endif /* SMPT_ML_DEFINITIONS_DATA_TYPES_H */
