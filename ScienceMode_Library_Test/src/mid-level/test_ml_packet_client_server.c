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
#include "test_ml_packet_client_server.h"
#include "test_general_smpt.h"

#include "smpt_packet_general.h"
#include "smpt_ml_packet_server.h"
#include "smpt_ml_packet_client.h"
#include "smpt_ll_definitions_data_types.h"

#include <assert.h>

/* File scope variables */
static uint8_t buffer_[Smpt_Limit_Max_Packet_Size];
static uint32_t buffer_length_ = Smpt_Limit_Max_Packet_Size;

/* File scope functions */
static void test_ml_init(void);
static void fill_ml_init(Smpt_ml_init *const ml_init);
static bool compare_ml_init(const Smpt_ml_init *const ml_init_a,
                            const Smpt_ml_init *const ml_init_b);

static void test_ml_update_internal(void (*fill_function_ptr)(Smpt_ml_update *const));
static void test_ml_update(void);
static void fill_ml_update_full(Smpt_ml_update *const ml_update);
static void fill_ml_update_one_channel(Smpt_ml_update *const ml_update);
static void fill_ml_update_minimal(Smpt_ml_update *const ml_update);
static bool compare_ml_update(const Smpt_ml_update *const ml_update_a,
                              const Smpt_ml_update *const ml_update_b);

static void test_ml_get_current_data(void);
static void fill_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data);
static bool compare_ml_get_current_data(const Smpt_ml_get_current_data
                                       *const ml_get_current_data_a,
                                        const Smpt_ml_get_current_data
                                       *const ml_get_current_data_b);

static void test_ml_get_current_data_ack(void);
static void fill_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                         *const ml_get_current_data_ack);
static void fill_state_ml_get_current_data_ack(Smpt_ml_get_current_data_ack
                                         *const ml_get_current_data_ack);
static void fill_ml_get_current_data_eight_channels_ack(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack);
static bool compare_ml_get_current_data_ack(const Smpt_ml_get_current_data_ack
                                            *const ml_get_current_data_ack_a,
                                            const Smpt_ml_get_current_data_ack
                                            *const ml_get_current_data_ack_b);

void test_ml_packet_client_server(void)
{
    test_ml_init();
    test_ml_update();
    test_ml_get_current_data();
    test_ml_get_current_data_ack();
}

void test_ml_init(void)
{
    uint32_t packet_length;
    Smpt_ml_init ml_init;
    Smpt_ml_init ml_init_out;
    smpt_clear_ml_init(&ml_init);
    smpt_clear_ml_init(&ml_init_out);


    smpt_clear_buffer(buffer_, buffer_length_);
    fill_ml_init(&ml_init);

    packet_length = smpt_build_ml_init(buffer_, buffer_length_, &ml_init);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Ml_Init);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_ml_init(&ml_init_out, buffer_, packet_length));

    assert(compare_ml_init(&ml_init, &ml_init_out));
}

void fill_ml_init(Smpt_ml_init *const ml_init)
{
    ml_init->stop_all_channels_on_error = 1;
    ml_init->packet_number = 45;
}

bool compare_ml_init(const Smpt_ml_init *const ml_init_a,
                     const Smpt_ml_init *const ml_init_b)
{
    return (ml_init_a->stop_all_channels_on_error  == ml_init_b->stop_all_channels_on_error) &&
           (ml_init_a->packet_number == ml_init_b->packet_number);
}

void test_ml_update(void)
{
    test_ml_update_internal(&fill_ml_update_full);
    test_ml_update_internal(&fill_ml_update_one_channel);
    test_ml_update_internal(&fill_ml_update_minimal);
}

void test_ml_update_internal(void (*fill_function_ptr)(Smpt_ml_update *const))
{
    uint32_t packet_length;
    Smpt_ml_update ml_update;
    Smpt_ml_update ml_update_out;

    smpt_clear_ml_update(&ml_update);
    smpt_clear_ml_update(&ml_update_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    (*fill_function_ptr)(&ml_update);

    packet_length = smpt_build_ml_update(buffer_, buffer_length_, &ml_update);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Ml_Update);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_ml_update(&ml_update_out, buffer_, packet_length));

    assert(compare_ml_update(&ml_update, &ml_update_out));
}


void fill_ml_update_full(Smpt_ml_update *const ml_update)
{
    ml_update->packet_number = 45;
    {
        uint8_t i = 0;
        for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
        {
            ml_update->enable_channel[i] = 1;
            // ml_update->softstart = 1;

            ml_update->channel_config[i].number_of_points = 16;
            ml_update->channel_config[i].period = 2345.25;
            ml_update->channel_config[i].double_period_accuracy = true;
            ml_update->channel_config[i].ramp = 15;

            {
                uint8_t j = 0;
                for (j = 0; j < ml_update->channel_config[i].number_of_points; j++)
                {
                    ml_update->channel_config[i].points[j].interpolation_mode =
                                                                         Smpt_Ll_Interpolation_Jump;
                    ml_update->channel_config[i].points[j].current = 124.5;
                    ml_update->channel_config[i].points[j].time = 20;
                }
            }
        }
    }
}


void fill_ml_update_one_channel(Smpt_ml_update *const ml_update)
{
    ml_update->packet_number = 44;

    {
        ml_update->enable_channel[2] = 1;

        ml_update->channel_config[2].number_of_points = 2;
        ml_update->channel_config[2].period = 16000;
        ml_update->channel_config[2].double_period_accuracy = false;
        ml_update->channel_config[2].ramp = 3;

        {
            uint8_t j = 0;
            for (j = 0; j < ml_update->channel_config[2].number_of_points; j++)
            {
                ml_update->channel_config[2].points[j].interpolation_mode =
                        Smpt_Ll_Interpolation_Jump;
                ml_update->channel_config[2].points[j].current = -122.5;
                ml_update->channel_config[2].points[j].time = 2000;
            }
        }
    }
}

void fill_ml_update_minimal(Smpt_ml_update *const ml_update)
{
    ml_update->packet_number = 40;
}

bool compare_ml_update(const Smpt_ml_update *const ml_update_a,
                       const Smpt_ml_update *const ml_update_b)
{
    bool valid = true;
    uint8_t i = 0;

    //valid = valid && (ml_update_a->softstart ==
    //                  ml_update_b->softstart);

    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        valid = valid && (ml_update_a->enable_channel[i] ==
                          ml_update_b->enable_channel[i]);

        valid = valid && (ml_update_a->channel_config[i].number_of_points ==
                          ml_update_b->channel_config[i].number_of_points) &&
                         (ml_update_a->channel_config[i].period ==
                          ml_update_b->channel_config[i].period) &&
                        (ml_update_a->channel_config[i].double_period_accuracy ==
                         ml_update_b->channel_config[i].double_period_accuracy) &&
                         (ml_update_a->channel_config[i].ramp ==
                          ml_update_b->channel_config[i].ramp);

        {
            uint8_t j = 0;
            for (j = 0; j < Smpt_Length_Points; j++)
            {
                valid = valid &&
                        (ml_update_a->channel_config[i].points[i].time ==
                         ml_update_b->channel_config[i].points[i].time) &&
                        (ml_update_a->channel_config[i].points[i].current ==
                         ml_update_b->channel_config[i].points[i].current) &&
                        (ml_update_a->channel_config[i].points[i].interpolation_mode ==
                         ml_update_b->channel_config[i].points[i].interpolation_mode);
            }
        }
    }

    return valid &&
           (ml_update_a->packet_number == ml_update_b->packet_number);
}

void test_ml_get_current_data(void)
{
    uint32_t packet_length;
    Smpt_ml_get_current_data ml_get_current_data;
    Smpt_ml_get_current_data ml_get_current_data_out;
    smpt_clear_ml_get_current_data(&ml_get_current_data);
    smpt_clear_ml_get_current_data(&ml_get_current_data_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_ml_get_current_data(&ml_get_current_data);

    packet_length = smpt_build_ml_get_current_data(buffer_, buffer_length_, &ml_get_current_data);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Ml_Get_Current_Data);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_ml_get_current_data(&ml_get_current_data_out, buffer_, packet_length));

    assert(compare_ml_get_current_data(&ml_get_current_data, &ml_get_current_data_out));
}

void fill_ml_get_current_data(Smpt_ml_get_current_data *const ml_get_current_data)
{
    ml_get_current_data->data_selection = Smpt_Ml_Data_Stimulation;
    ml_get_current_data->packet_number = 40;
}

bool compare_ml_get_current_data(const Smpt_ml_get_current_data *const ml_get_current_data_a,
                                 const Smpt_ml_get_current_data *const ml_get_current_data_b)
{
    uint8_t result = true;

    result = result &&
            ml_get_current_data_a->data_selection ==
            ml_get_current_data_b->data_selection;


    return (result &&
           (ml_get_current_data_a->packet_number == ml_get_current_data_b->packet_number));
}

void test_ml_get_current_data_ack(void)
{
    uint32_t packet_length;
    Smpt_ml_get_current_data_ack ml_get_current_data_ack;
    Smpt_ml_get_current_data_ack ml_get_current_data_ack_out;
    smpt_clear_ml_get_current_data_ack(&ml_get_current_data_ack);
    smpt_clear_ml_get_current_data_ack(&ml_get_current_data_ack_out);

    smpt_clear_buffer(buffer_, buffer_length_);
    fill_ml_get_current_data_ack(&ml_get_current_data_ack);

    packet_length = smpt_build_ml_get_current_data_ack(buffer_, buffer_length_,
                                                       &ml_get_current_data_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Ml_Get_Current_Data_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_ml_get_current_data_ack(&ml_get_current_data_ack_out,
                                                buffer_, packet_length));

    assert(compare_ml_get_current_data_ack(&ml_get_current_data_ack, &ml_get_current_data_ack_out));

    smpt_clear_buffer(buffer_, buffer_length_);

    fill_ml_get_current_data_eight_channels_ack(&ml_get_current_data_ack);

    packet_length = smpt_build_ml_get_current_data_ack(buffer_, buffer_length_,
                                                       &ml_get_current_data_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Ml_Get_Current_Data_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_ml_get_current_data_ack(&ml_get_current_data_ack_out,
                                                buffer_, packet_length));

    assert(compare_ml_get_current_data_ack(&ml_get_current_data_ack, &ml_get_current_data_ack_out));

    fill_state_ml_get_current_data_ack(&ml_get_current_data_ack);

    packet_length = smpt_build_ml_get_current_data_ack(buffer_, buffer_length_,
                                                       &ml_get_current_data_ack);

    assert(test_check_header_and_footer(buffer_, packet_length));
    assert(smpt_get_cmd(buffer_, packet_length) == Smpt_Cmd_Ml_Get_Current_Data_Ack);
    assert(smpt_check_length(buffer_, packet_length));
    assert(smpt_check_checksum(buffer_, packet_length));
    assert(smpt_extract_ml_get_current_data_ack(&ml_get_current_data_ack_out,
                                                buffer_, packet_length));

    assert(compare_ml_get_current_data_ack(&ml_get_current_data_ack, &ml_get_current_data_ack_out));

}

void fill_ml_get_current_data_ack(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack)
{
    uint8_t i = 0;

    ml_get_current_data_ack->result = 2;

    ml_get_current_data_ack->data_selection = Smpt_Ml_Data_Stimulation;

    /* optional */
    ml_get_current_data_ack->stimulation_data.is_valid = true;
    ml_get_current_data_ack->stimulation_data.stimulation_state = 1;
    for (i = 0; i < 4; i++)
    {
        ml_get_current_data_ack->stimulation_data.electrode_error[i] = 1;
    }

    ml_get_current_data_ack->packet_number = 0;
}

void fill_state_ml_get_current_data_ack(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack)
{
    uint8_t i = 0;

    ml_get_current_data_ack->result = 2;

    ml_get_current_data_ack->data_selection = Smpt_Ml_Data_Channels;

    /* optional */
    ml_get_current_data_ack->channel_data.is_valid = true;
    ml_get_current_data_ack->stimulation_data.is_valid = false;

    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        ml_get_current_data_ack->channel_data.stimulation_state[i] = 1;
        ml_get_current_data_ack->channel_data.channel_state[i] = 1;
    }

    ml_get_current_data_ack->packet_number = 0;
}

void fill_ml_get_current_data_eight_channels_ack(Smpt_ml_get_current_data_ack *const ml_get_current_data_ack)
{
    uint8_t i = 0;

    ml_get_current_data_ack->result = 2;

    ml_get_current_data_ack->data_selection = Smpt_Ml_Data_Stimulation_Eight_Channels;

    /* optional */
    ml_get_current_data_ack->stimulation_data.is_valid = true;
    for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
    {
        ml_get_current_data_ack->stimulation_data.electrode_error[i] = 1;
    }

    ml_get_current_data_ack->packet_number = 0;
}

bool compare_ml_get_current_data_ack(const Smpt_ml_get_current_data_ack
                                     *const ml_get_current_data_ack_a,
                                     const Smpt_ml_get_current_data_ack
                                     *const ml_get_current_data_ack_b)
{
    bool valid = true;
    uint8_t i;

    /* general */
    valid = valid && (ml_get_current_data_ack_a->result == ml_get_current_data_ack_b->result) &&
                     (ml_get_current_data_ack_a->packet_number ==
                      ml_get_current_data_ack_b->packet_number);



    valid = valid && (ml_get_current_data_ack_a->data_selection ==
                      ml_get_current_data_ack_b->data_selection);


    /* optional */
    if (ml_get_current_data_ack_a->data_selection == Smpt_Ml_Data_Stimulation)
    {
        valid = valid && (ml_get_current_data_ack_a->stimulation_data.stimulation_state ==
                          ml_get_current_data_ack_b->stimulation_data.stimulation_state);
        valid = valid && (ml_get_current_data_ack_a->stimulation_data.is_valid ==
                          ml_get_current_data_ack_b->stimulation_data.is_valid);
        valid = valid && (ml_get_current_data_ack_a->channel_data.is_valid ==
                          ml_get_current_data_ack_b->channel_data.is_valid);
        for (i = 0; i < 4; i++)
        {
            valid = valid && (ml_get_current_data_ack_a->stimulation_data.electrode_error[i] ==
                              ml_get_current_data_ack_b->stimulation_data.electrode_error[i]);
        }
    } else if (ml_get_current_data_ack_a->data_selection == Smpt_Ml_Data_Stimulation_Eight_Channels)
    {
        valid = valid && (ml_get_current_data_ack_a->stimulation_data.stimulation_state ==
                          ml_get_current_data_ack_b->stimulation_data.stimulation_state);
        valid = valid && (ml_get_current_data_ack_a->stimulation_data.is_valid ==
                          ml_get_current_data_ack_b->stimulation_data.is_valid);
        valid = valid && (ml_get_current_data_ack_a->channel_data.is_valid ==
                          ml_get_current_data_ack_b->channel_data.is_valid);
        for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
        {
            valid = valid && (ml_get_current_data_ack_a->stimulation_data.electrode_error[i] ==
                              ml_get_current_data_ack_b->stimulation_data.electrode_error[i]);
        }
    } else if (ml_get_current_data_ack_a->data_selection == Smpt_Ml_Data_Channels)
    {
        valid = valid && (ml_get_current_data_ack_a->channel_data.is_valid ==
                          ml_get_current_data_ack_b->channel_data.is_valid);
        valid = valid && (ml_get_current_data_ack_a->stimulation_data.is_valid ==
                          ml_get_current_data_ack_b->stimulation_data.is_valid);
        for (i = 0; i < Smpt_Length_Number_Of_Channels; i++)
        {
            valid = valid && (ml_get_current_data_ack_a->channel_data.stimulation_state[i] ==
                              ml_get_current_data_ack_b->channel_data.stimulation_state[i]) &&
                             (ml_get_current_data_ack_a->channel_data.channel_state[i] ==
                              ml_get_current_data_ack_b->channel_data.channel_state[i]);
        }
    }

    return valid;
}

