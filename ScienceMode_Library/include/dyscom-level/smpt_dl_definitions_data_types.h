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
#ifndef SMPT_DL_DEFINITIONS_DATA_TYPES_H
#define SMPT_DL_DEFINITIONS_DATA_TYPES_H

/***************************************************************************//**
* \file smpt_dl_definitions_data_types.h
*
* This file defines the data types of the protocol, which are related to the
* dyscom-level.
*******************************************************************************/

#include "smpt_definitions_data_types.h"

#define SMPT_DL_MAX_STRING_LENGTH 128

/**< maximum file name length not including terminating null character */
#define SMPT_DL_MAX_FILE_NAME_LENGTH SMPT_DL_MAX_STRING_LENGTH

/**< maximum patient name length not including terminating null character */
#define SMPT_DL_MAX_PATIENT_NAME_LENGTH SMPT_DL_MAX_STRING_LENGTH

/**< maximum investigator name length not including terminating null character */
#define SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH SMPT_DL_MAX_STRING_LENGTH

/**< maximum GUID length without terminating null character */
#define SMPT_DL_GUID_STRING_LENGTH 36

/**< number of characters used for a file name id */
#define SMPT_DL_MAX_FILE_ID_LENGTH 60

/**< number of bytes used to represent a file size [0, 64 ExaByte] */
#define SMPT_DL_FILE_SIZE_BYTES 8

/**< maximum number of bytes per block used in file transfer */
#define SMPT_DL_MAX_BLOCK_BYTES_LENGTH 512

/**< maximum number of channels for one sample tranfer by @see send_live_data */
#define SMPT_DL_MAX_CHANNELS 8

/**< maximum number of electrode value 24 Bit  with sign */
#define SMPT_DL_MAX_SAMPLE_VALUE ((1 << 23) - 1)

/**< 2^34 - 1 should be 16 GB. this is a large file */
#define SMPT_DL_MAX_FILE_SIZE ((1 << 33) - 1)

/** maximum number of measurements */
#define SMPT_DL_MAX_N_MEASUREMENTS 8

/** sampling frequencies */
#define SMPT_DL_4KHZ 4000
#define SMPT_DL_2KHZ 2000
#define SMPT_DL_1KHZ 1000

typedef enum /* Smpt_Dl_Signal_Type */
{
    Smpt_Dl_Signal_Type_Unused      = 0,    /**< this channel is used */
    Smpt_Dl_Signal_Type_Unkown      = 1,    /**< at this time, this channel isn't defined */
    Smpt_Dl_Signal_Type_BI          = 2,    /**< bio imendance in milli ohm */
    Smpt_Dl_Signal_Type_EMG1        = 3,    /**< EMG1 in milli volt */
    Smpt_Dl_Signal_Type_Op_Voltage  = 4,    /**< operation voltage in volt */
    Smpt_Dl_Signal_Type_Test_Signal = 5,    /**< test signal in raw adc */
    Smpt_Dl_Signal_Type_Ground      = 6,    /**< ground signal in milli volt */
    Smpt_Dl_Signal_Type_Temperature = 7,    /**< temperature in ??? °C */
    Smpt_Dl_Signal_Type_Internal_SC = 8,    /**< chip internal short circuit */
    Smpt_Dl_Signal_Type_EMG2        = 9,    /**< EMG2 in milli volt */
    Smpt_Dl_Signal_Type_Time        = 10,   /**< time in us */
    Smpt_Dl_Signal_Type_PushButton  = 11,   /**< anlog Voltage in milli volt */
    Smpt_Dl_Signal_Type_Breathing   = 12,   /**< anlog Voltage in milli volt */
    Smpt_Dl_Signal_Type_Last_Item   = 12
    /**< don't forget to adjust the last item */
}Smpt_Dl_Signal_Type;

typedef enum/* Smpt_Dl_Dsp_Filter */
{
    Smpt_Dl_Dsp_Filter_off = 0, /* no filter is activated*/
    Smpt_Dl_Dsp_Filter_001 = 1, /* predefined Filter casade or block for this notation*/
    Smpt_Dl_Dsp_Filter_002 = 2, /* predefined Filter casade or block for this notation*/
    Smpt_Dl_Dsp_Filter_003 = 3,  /* predefined Filter casade or block for this notation*/
    Smpt_Dl_Dsp_Filter_Last = 3
}Smpt_Dl_Dsp_Filter;

typedef enum{
    Smpt_Dl_Output_Sample_Freq_Invalid              = 0,
    Smpt_Dl_Output_Sample_Freq_32kSPS               = 1,
    Smpt_Dl_Output_Sample_Freq_16kSPS               = 2,
    Smpt_Dl_Output_Sample_Freq_8kSPS                = 3,
    Smpt_Dl_Output_Sample_Freq_4kSPS                = 4,
    Smpt_Dl_Output_Sample_Freq_2kSPS                = 5,
    Smpt_Dl_Output_Sample_Freq_1kSPS                = 6,
    Smpt_Dl_Output_Sample_Freq_500SPS               = 7,
    Smpt_Dl_Output_Sample_Freq_250SPS               = 8,
    Smpt_Dl_Output_Sample_Freq_Lowpower_Last_Item   = 8
}Smpt_Dl_Output_Sample_Freq;

typedef enum /* Smpt_Dl_Electrode_Status, used as bit mask bit positions for a 8 bit bit mask */
{
    Smpt_Dl_Electrode_Status_Positive       = 1, /**< electrode status (1 adhesive, 0 off) for positive electrode */
    Smpt_Dl_Electrode_Status_Negative       = 2, /**< electrode status (1 adhesive, 0 off) for negative electrode */
    Smpt_Dl_Electrode_Status_Maximum_Valid  = 3, /**< electrode status (1, 1) means both electrodes are adhesive. */
    Smpt_Dl_Electrode_Status_Last_Item      = 3
    /**< don't forget to adjust the last item */
} Smpt_Dl_Electrode_Status;

typedef enum /* Smpt_Dl_Get_Type */
{
    Smpt_Dl_Get_Type_Battery_Status      = 0, /**< battery status in absolute voltage and relative percent */
    Smpt_Dl_Get_Type_File_System_Status  = 1, /**< file system status in available and used SD-Card-Memory */
    Smpt_Dl_Get_Type_List_of_MMI         = 2, /**< list of measurement meta information. The corresponding
                                                   dl_get_ack sends the number of measurements. If it is
                                                   greater than zero dl_send_mmi commands will follow.*/
    Smpt_Dl_Get_Type_Operation_Mode      = 3, /**< operation mode */
    Smpt_Dl_Get_Type_File_By_Name        = 4, /**< file by name */
    Smpt_Dl_Get_Type_Device_Id           = 5, /**< device id of the measurement hardware */
    Smpt_Dl_Get_Type_Firmware_Version    = 6, /**< firmware version of the firmware of the measurement hardware */
    Smpt_Dl_Get_Type_File_Info           = 7, /**< get Info of size, checksum, name */
    Smpt_Dl_Get_Type_Last_Item           = 7
    /* don't forget to adjust the last item */
} Smpt_Dl_Get_Type;


typedef enum /* Smpt_Dl_Op_Mode stands for operation mode */
{
    Smpt_Dl_Op_Mode_Undefined     = 0, /**< */
    Smpt_Dl_Op_Mode_Idle   = 1, /**< when hardware is on and nothing is to do)*/
    Smpt_Dl_Op_Mode_Live_Measurement_Pre     = 2, /**< when measurement is initialized but not started */
    Smpt_Dl_Op_Mode_Live_Measurement  = 3, /**< when measurement is startet  */
    Smpt_Dl_Op_Mode_Record_Pre = 4, /** when Recording is initialized */
    Smpt_Dl_Op_Mode_Record = 5, /** when Recording ist startet*/
    Smpt_Dl_Op_Mode_Data_Transfer_Pre = 6, /** when transfering to PC is initialized **/
    Smpt_Dl_Op_Mode_Data_Transfer = 7,  /** when File is transfering to PC **/
    Smpt_Dl_Op_Mode_Last_Item     = Smpt_Dl_Op_Mode_Data_Transfer

} Smpt_Dl_Op_Mode;

typedef struct
{
    char        file_name[SMPT_DL_MAX_FILE_NAME_LENGTH+1]; /** FileName **/
    uint32_t    file_size; /** Size of File in kilobyte **/
    uint16_t    file_checksum; /** CRC16 Checksum **/
}Smpt_dl_file_info;


int smpt_dl_compare_int(int a, int b);

/*
#include <time.h>
typedef struct tm Smpt_dl_tm ;
*/
typedef struct
{
    uint8_t tm_sec;
    uint8_t tm_min;
    uint8_t tm_hour;
    uint8_t tm_mday;
    uint8_t tm_mon;
    uint16_t tm_year;
    uint8_t tm_wday;
    uint16_t tm_yday;
    uint8_t tm_isdst;
} Smpt_dl_tm;

/**
 * @brief smpt_dl_compare_tm compares to struct tm pointer contents
 * year, month, month day, hour, minute, seconds lexicografically.
 * @param a
 * @param b
 * @return  0,  if a == b
 *          -1, if a <  b
 *          1,  if a >  b
 */
int smpt_dl_compare_tm(const Smpt_dl_tm *a, const Smpt_dl_tm *b);

bool smpt_dl_tm_is_null(Smpt_dl_tm* tm);




/**
 * @brief smpt_clear_dl_tm initializes the dl_tm struct
 * @param dl_tm
 */
void smpt_clear_dl_tm(Smpt_dl_tm *const dl_tm);

typedef struct /* Smpt_dl_ads129x */
{
    uint8_t id;             /**< read only */
    uint8_t config1;        /**< */
    uint8_t config2;        /**< */
    uint8_t config3;        /**< */
    uint8_t loff;        /**< */
    uint8_t ch1set;        /**< */
    uint8_t ch2set;        /**< */
    uint8_t ch3set;        /**< */
    uint8_t ch4set;        /**< */
    uint8_t ch5set;        /**< */
    uint8_t ch6set;        /**< */
    uint8_t ch7set;        /**< */
    uint8_t ch8set;        /**< */
    uint8_t rld_sensp;        /**< */
    uint8_t rld_sensn;        /**< */
    uint8_t loff_sensp;        /**< */
    uint8_t loff_sensn;        /**< */
    uint8_t loff_flip;        /**< */
    uint8_t loff_statp;        /**< */
    uint8_t loff_statn;     /**< read only */
    uint8_t gpio;        /**< */
    uint8_t pace;        /**< */
    uint8_t resp;        /**< */
    uint8_t config4;        /**< */
    uint8_t wct1;        /**< */
    uint8_t wct2;        /**< */
} Smpt_dl_ads129x;

void smpt_copy_dl_ads129x (Smpt_dl_ads129x* const target, const Smpt_dl_ads129x* const source);
/**
 * @brief smpt_clear_dl_ads129x initializes the dl_ads129x struct
 * @param dl_ads129x
 */
void smpt_clear_dl_ads129x(Smpt_dl_ads129x *const dl_ads129x);

typedef enum
{
    Smpt_Dl_Init_Sync_Signal_Undifined          = 0,
    Smpt_Dl_Init_Sync_Signal_Rect_Pulse_0_5V    = 1,
    Smpt_Dl_Init_Sync_Signal_Rect_Pulse_0_200mV = 2,
    Smpt_Dl_Init_Sync_Signal_Sinus_0_200mV      = 3,
    Smpt_Dl_Init_Sync_Signal_Last_Item          = 3

}
Smpt_Dl_Init_Sync_Signal;

typedef enum{
    Smpt_Dl_Init_Ack_State_Undefined = 0,
    Smpt_Dl_Init_Ack_State_Successfull = 1,
    Smpt_Dl_Init_Ack_State_Error_Storage_Init = 2,
    Smpt_Dl_Init_Ack_State_Error_Storage_Write = 3,
    Smpt_Dl_Init_Ack_State_Error_Storage_Full = 4,
    Smpt_Dl_Init_Ack_State_Warning_Battery_Level = 5,
    Smpt_Dl_Init_Ack_State_Error_Ads129x_Register = 6,
    Smpt_Dl_Init_Ack_State_Last = 6
}
Smpt_Dl_Init_Ack_State;

typedef struct /* Smpt_dl_init */
{
    uint8_t         packet_number;
    Smpt_dl_ads129x ads129x; /**< The registry map of the ADS129X, 26 bytes */

    Smpt_dl_tm planned_start_time;  /**< Struct with date and time for scheduled start */
    Smpt_dl_tm system_time;  /**< The current system time of the client */
    char patient_name[SMPT_DL_MAX_PATIENT_NAME_LENGTH+1];
    char patient_number[SMPT_DL_GUID_STRING_LENGTH+1];
    char investigator_name[SMPT_DL_MAX_INVESTIGATOR_NAME_LENGTH+1];
    /* Untersucher */
    /* Typ oder Art der Messung */

    Smpt_Dl_Signal_Type channel_type[SMPT_DL_MAX_CHANNELS]; /**< enum for channel type, BI, EMG,
                                                            Breathing, Trigger etc. */

    uint32_t duration_of_planned_m; /**< duration of planned measuremnt in seconds , max 8h */

    uint16_t number_of_channels;    /**< number of channels used, this is just a
                                    redundant value which can be used for the
                                    DysCom-HW to check if the ads1294r
                                    configuration is correct */

    bool live_data_mode_flag;   /**< Flag which determines if live data will be
                                    sent directly after gathering measurements */

    bool sd_storage_mode_flag;  /**< Flag which determines whether the
                                     measurements will be saved on sd-card */

    bool timed_start_flag;      /**< If this flag is set the dyscom hw will
                                     wait intil time timed_start is reached
                                     and then starts the measurements */

    bool system_time_flag;      /**< If this flag is set, the system time will
                                  be used for synchroniziation with the
                                  dyscom hw clock */

    bool mute_flag;             /**< If this flag is set, the system will
                                     listen to a hardware wire mute signal.
                                     Whenever the mute
                                     signal is received the measurement hardware
                                     mutes its own measurement activities to
                                     protect circuits and measurement results
                                     from external electricity input. */

    Smpt_Dl_Init_Sync_Signal sync_signal;
    /**< The sync_signal variable is used to specify a synchronization signal
     * which may be used to synchronize the measurement hardware with other
     * medical devices */

    Smpt_Dl_Dsp_Filter filter;      /**< filter setting spefic blocks of filters
                                    for different channels and application*/
} Smpt_dl_init; /**< \struct Smpt_dl_init
                     Struct for the Dl_init command. \see smpt_send_dl_init() */
/**
 * @brief smpt_clear_dl_init initializes the dl_init struct
 * @param dl_init
 */
void smpt_clear_dl_init(Smpt_dl_init *const dl_init);

void smpt_clear_dl_patient_name(char *const patient_name);
void smpt_clear_dl_investigator_name(char *const investigator_name);
void smpt_clear_dl_patient_number(char *const patient_number);

void smpt_clear_dl_string(char *const string, uint16_t str_length);

typedef struct /* Smpt_dl_init_ack */
{
    uint8_t         packet_number;  /**< */
    Smpt_Result     result;         /**< */
    Smpt_dl_ads129x ads129x;        /**< should be equal to the ads129x sent by init*/
    char            measurement_file_id[SMPT_DL_MAX_FILE_ID_LENGTH+1];
    /**< this id will be used for the file names used to store date to the
     * SD-Card of the DysCom-HW for the corresponding measurement */
    Smpt_Dl_Init_Ack_State init_state;
    Smpt_Dl_Output_Sample_Freq freq_out;
} Smpt_dl_init_ack;

/**
 * @brief smpt_clear_dl_init_ack initializes the dl_init_ack struct
 * @param dl_init
 */
void smpt_clear_dl_init_ack(Smpt_dl_init_ack *const dl_init_ack);

/* smpt_send_dl_start and smpt_send_dl_stop are parameterless */
typedef enum /* Smpt_dl_get_block_flag, used as bit mask bit positions for a 8 bit bit mask */
{
    Smpt_Dl_Send_File_By_Name_Mode_Undefined = 0,
    Smpt_Dl_Send_File_By_Name_Mode_Multiblock = 1,
    Smpt_Dl_Send_File_By_Name_Mode_Singleblock = 2
}
Smpt_Dl_Send_File_By_Name_Mode;

typedef struct /* Smpt_Dl_File_By_Name  */
{
    uint64_t    file_size;
    uint32_t    n_blocks;
    char        file_name[SMPT_DL_MAX_FILE_NAME_LENGTH+1];
    uint32_t    block_offset;
    Smpt_Dl_Send_File_By_Name_Mode mode;
}Smpt_dl_file_by_name;

typedef struct /* Smpt_dl_mmi */
{
    uint16_t    n_measurements;
}Smpt_dl_get_mmi;

typedef enum /* Smpt_Dl_EnergyState, used as bit mask bit positions for a 8 bit bit mask */
{
    Smpt_Dl_EnergyState_CableConnected = 1,
    Smpt_Dl_EnergyState_DeviceIsLoading = 2
}
Smpt_Dl_EnergyState;

typedef struct /* Smpt_dl_get */
{
    uint8_t                 packet_number;                              /**< */
    Smpt_Dl_Get_Type        get_type;                              /**< */
    Smpt_dl_file_by_name    file_by_name;
    Smpt_dl_get_mmi         mmi;
    Smpt_dl_file_info       file_info;
} Smpt_dl_get;
/**
 * @brief smpt_clear_dl_get initializes the dl_get struct
 * @param dl_get
 */
void smpt_clear_dl_get(Smpt_dl_get *const dl_get);

typedef struct /* Smpt_dl_battery_status */
{
    uint32_t voltage;       /* [0, 65535] in milli volt*/
    int32_t current;        /* [-327675, 327675] Current*/
    uint8_t percentage;    /* [0, 100] in percent */
    int8_t temperature;    /* [-128, 127]Temperature*/
    uint8_t energystate;
} Smpt_dl_battery_status;

typedef struct /* Smpt_dl_file_system_status */
{
    bool file_system_ready;     /**< 1 if SD-Card with valid file system detected */
    uint64_t used_size;         /**< */
    uint64_t free_size;         /**< */
} Smpt_dl_file_system_status;

/*
 * Smpt_dl_mmi : Smpt_dl_mmi_
 * here we need a work around because we like to use this structure in a
 * sorted list with sglib.h. Smpt_dl_mmi_ is just a helper struct
 */
struct Smpt_dl_mmi_
{
    uint16_t measurement_number;        /**< the number of the item in relation
                                            to the number of all measurements */
    char measurement_id[SMPT_DL_MAX_FILE_ID_LENGTH+1];  /**< the measurement file name */
    char patient_number[SMPT_DL_GUID_STRING_LENGTH+1];
    uint64_t file_size;
    Smpt_dl_tm planned_start_time;     /**< time when a planned measurement
                                            should be started. This is the same
                                            time as in dl_init */
    Smpt_dl_tm start_time;                 /**< time when measurement started
                                            may differ from time in dl_init */
    uint32_t time_length;              /**< the duration of the
                                            measurement in seconds */
    /*
     * struct Smpt_dl_mmi_ *next_ptr;
     * struct Smpt_dl_mmi_ *prev_ptr;
     */
};

typedef struct Smpt_dl_mmi_ Smpt_dl_mmi;

void smpt_clear_dl_mmi(Smpt_dl_mmi *const dl_mmi);

typedef struct /* Smpt_dl_send_mmi - mmi = measurement_meta_info */
{
    uint8_t  packet_number;
    Smpt_dl_mmi mmi;
    Smpt_dl_init dl_init;
} Smpt_dl_send_mmi;
void smpt_clear_dl_send_mmi(Smpt_dl_send_mmi* const dl_send_mmi);
void smpt_clear_dl_measurement_id(char file_id[SMPT_DL_MAX_FILE_ID_LENGTH+1]);

typedef struct /* Smpt_dl_get_ack */
{
    uint8_t                     packet_number;
    Smpt_Result                 result;
    Smpt_Dl_Get_Type            get_type;
    Smpt_Dl_Op_Mode             operation_mode;
    Smpt_dl_battery_status      battery_status;
    Smpt_dl_file_system_status  file_system_status;
    Smpt_dl_file_by_name        file_by_name;
    Smpt_dl_get_mmi             mmi;
    char                        deviceId[SMPT_DL_MAX_STRING_LENGTH + 1];
    char                        firmwareVersion[SMPT_DL_MAX_STRING_LENGTH + 1];
    Smpt_dl_file_info           file_info;
} Smpt_dl_get_ack;
void smpt_clear_dl_get_ack(Smpt_dl_get_ack* const dl_get_ack);

typedef struct /* Smpt_dl_send_file */
{
    uint8_t    packet_number;
    uint32_t    block_number;
    uint16_t    n_bytes_per_block;
    uint8_t     data[SMPT_DL_MAX_BLOCK_BYTES_LENGTH];

} Smpt_dl_send_file;

/**
 * @brief smpt_clear_dl_send_file initializes the dl_send_file struct
 * @param dl_send_file
 */
void smpt_clear_dl_send_file(Smpt_dl_send_file *const dl_send_file);

typedef struct /* Smpt_dl_electrode_sample */
{
    float                       value;
    Smpt_Dl_Signal_Type         signal_type;
    Smpt_Dl_Electrode_Status    status;
} Smpt_dl_electrode_sample;

typedef struct /* Smpt_dl_send_live_data */
{
    uint8_t                     packet_number;
    uint8_t                     n_channels;
    uint32_t                    time_offset;
    Smpt_dl_electrode_sample    electrode_samples[SMPT_DL_MAX_CHANNELS];
                                /**< n_channels items SMPT_DL_MAX_CHANNELS */

} Smpt_dl_send_live_data;

/**
 * @brief smpt_clear_dl_send_live_data initializes the dl_send_live_data struct
 * @param dl_send_live_data
 */
void smpt_clear_dl_send_live_data(Smpt_dl_send_live_data *const dl_send_live_data);
void smpt_clear_dl_electrode_samples(Smpt_dl_electrode_sample electrode_samples[SMPT_DL_MAX_CHANNELS]);

typedef struct /* Smpt_dl_send_file_ack this is not used yet maybe we do not need an answer to the server */
{
    uint8_t     packet_number;
    Smpt_Result result;
    uint32_t    block_number;
} Smpt_dl_send_file_ack;

typedef struct /* Smpt_dl_start_ack */
{
    uint8_t     packet_number;
    Smpt_Result result;
} Smpt_dl_start_ack;

/**
 * @brief smpt_clear_dl_start_ack initializes the dl_start_ack struct
 * @param dl_start_ack
 */
void smpt_clear_dl_start_ack(Smpt_dl_start_ack *const dl_start_ack);

typedef struct /* Smpt_dl_stop_ack */
{
    uint8_t     packet_number;
    Smpt_Result result;
    uint64_t    time; /**< time in milli seconds */
} Smpt_dl_stop_ack;

/**
 * @brief smpt_clear_dl_stop_ack initializes the dl_stop_ack struct
 * @param dl_stop_ack
 */
void smpt_clear_dl_stop_ack(Smpt_dl_stop_ack *const dl_stop_ack);

typedef struct /* Smpt_dl_channel_measurement */
{
    Smpt_Dl_Signal_Type signal_type;            /**<  */
    uint32_t        value;              /**< 24 bit value */
    uint8_t         electrode_status;   /**< bit mask

    - 0               connection    [0 - no, 1 - yes]
    - 1               short circuit [0 - no, 1 - yes]
    - 2               unused                  0
    - 3               unused                  0
    - 4               unused                  0
    - 5               unused                  0
    - 6               unused                  0
    - 7               unused                  0 */
} Smpt_dl_channel_measurement;

typedef enum /* Smpt_Dl_Hardware_Module */
{
    Smpt_Dl_Hardware_Module_Undefined   = 0,
    Smpt_Dl_Hardware_Module_Bluetooth   = 1,
    Smpt_Dl_Hardware_Module_Memory_Card = 2,
    Smpt_Dl_Hardware_Module_Measurement = 3, /**< the bio impendance and electro myography measurement circuits (BI/EMG) */
    Smpt_Dl_Hardware_Module_Research    = 4, /**< research power supply for sync and mute signal */
    Smpt_Dl_Hardware_Module_Last        = 4
}
Smpt_Dl_Hardware_Module;
/* Caution: do not exceed 255 because
 * smpt_dl_insert_power_module and  smpt_dl_extract_power_module uses uint8_t
 */

typedef struct /* Smpt_dl_power_module */
{
    uint8_t                 packet_number;     /**< */
    Smpt_Dl_Hardware_Module hardware_module;
    bool                    switch_on_off;
}
Smpt_dl_power_module;
void smpt_clear_dl_power_module(Smpt_dl_power_module *dl_power_module);

typedef struct /* Smpt_dl_power_module_ack */
{
    uint8_t                 packet_number;
    Smpt_Result             result;
    Smpt_Dl_Hardware_Module hardware_module;
    bool                    switch_on_off;
}
Smpt_dl_power_module_ack;

void smpt_clear_dl_power_module_ack(Smpt_dl_power_module_ack *dl_power_module_ack);

/**
 * @brief smpt_clear_dl_send_file_ack initializes the dl_send_file_ack struct
 * @param dl_send_file_ack
 */
void smpt_clear_dl_send_file_ack(Smpt_dl_send_file_ack *const dl_send_file_ack);

typedef enum
{
    Smpt_Dl_Sys_Type_Undefined          = 0,
    Smpt_Dl_Sys_Type_Deletefile         = 1,
    Smpt_Dl_Sys_Type_Device_Sleep       = 2,
    Smpt_Dl_Sys_Type_Device_Storage     = 3,
    Smpt_Dl_Sys_Type_Device_Last_Item   = 3
}Smpt_Dl_Sys_Type;

typedef enum
{
    Smpt_Dl_Sys_State_Undefined         = 0,
    Smpt_Dl_Sys_State_Successful        = 1,
    Smpt_Dl_Sys_State_Last_Item         = 1
}Smpt_Dl_Sys_State;

typedef struct /* Smpt_dl_sys */
{
    uint8_t             packet_number;
    Smpt_Result         result;
    Smpt_Dl_Sys_Type    type;
    char                file_name[SMPT_DL_MAX_FILE_NAME_LENGTH+1];
} Smpt_dl_sys;

/**
 * @brief smpt_clear_dl_sys initializes the dl_start_ack struct
 * @param dl_start_ack
 */
void smpt_clear_dl_sys(Smpt_dl_sys *const dl_sys);

typedef struct /* Smpt_dl_sys_ack */
{
    uint8_t             packet_number;
    Smpt_Result         result;
    Smpt_Dl_Sys_Type    type;
    char                file_name[SMPT_DL_MAX_FILE_NAME_LENGTH+1];
    Smpt_Dl_Sys_State   state;
} Smpt_dl_sys_ack;

/**
 * @brief smpt_clear_dl_sys_ack initializes the dl_start_ack struct
 * @param dl_start_ack
 */
void smpt_clear_dl_sys_ack(Smpt_dl_sys_ack *const dl_sys_ack);

#endif
