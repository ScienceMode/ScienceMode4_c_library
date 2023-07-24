/***************************************************************************//**
* \file smpt_ll_client_cmd_lists.c
* \copyright MPL and LGPL HASOMED GmbH 2013-2022
*
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

#include "smpt_client_cmd_lists.h"
#include "smpt_messages.h"
#include "smpt_packet_general.h"

/*
 *
 *  * req_current_index         |
 * [x][x][x]                    | Initial state (x means not defined)
 *  * req_expected_index        |
 *
 *     * req_current_index      |
 * [0][x][x]                    | A packet with packet number 0 has been sent
 *  * req_expected_index        |
 *
 *        * req_current_index   |
 * [0][1][x]                    | A packet with packet number 1 has been sent
 *  * req_expected_index        |
 *
 *        * req_current_index   |
 * [0][1][x]                    | A packet with packet number 0 was received
 *     * req_expected_index     |
 *
 *        * req_current_index   |
 * [0][1][x]                    | A packet with packet number 1 was received
 *        * req_expected_index  |
 *
 */

static bool compare_acks(const Smpt_ack *const ack_a,
                  const Smpt_ack *const ack_b);

static void smpt_check_expected_ack(const Smpt_ack *const ack, Smpt_cmd_list * const cmd_list);

/**
 * @brief has_request returns true if the cmd is a direct response (ack) of a
 * corresponding request which had been sent before otherwise if the cmd is just
 * a single direction cmd without direct request response meaning false is
 * returned
 * @param cmd
 */
static bool has_request(const Smpt_Cmd cmd);

void smpt_clear_acks_and_requests(Smpt_cmd_list *const cmd_list)
{
    uint32_t i;
    Smpt_cmd empty_cmd;
    Smpt_ack empty_ack;

    smpt_clear_ack(&empty_ack);
    smpt_clear_cmd(&empty_cmd);

    cmd_list->acks_length             = 0;
    cmd_list->acks_current_index      = 0;
    cmd_list->requests_current_index  = 0;
    cmd_list->requests_expected_index = 0;
    cmd_list->number_of_expected      = 0;

    for (i = 0; i < Smpt_Length_Number_Of_Acks; i++)
    {
        cmd_list->acks[i]     = empty_ack;
        cmd_list->requests[i] = empty_cmd;
    }
}

void smpt_add_request(Smpt_Cmd cmd, uint8_t packet_number, Smpt_cmd_list *const cmd_list)
{
    cmd_list->requests[cmd_list->requests_current_index].command_number = cmd;
    cmd_list->requests[cmd_list->requests_current_index].packet_number  = packet_number;

    cmd_list->requests_current_index++;
    cmd_list->requests_current_index = cmd_list->requests_current_index %
                                       Smpt_Length_Number_Of_Acks;
    cmd_list->number_of_expected++;
}

void smpt_add_ack(Smpt_Cmd cmd, Smpt_Result result, uint8_t packet_number,
                  Smpt_cmd_list *const cmd_list)
{
    Smpt_ack ack;
    ack.command_number = cmd;
    ack.packet_number = packet_number;
    ack.result = result;

    cmd_list->acks[cmd_list->acks_current_index] = ack;

    cmd_list->acks_current_index++;
    cmd_list->acks_current_index = cmd_list->acks_current_index % Smpt_Length_Number_Of_Acks;

    if (cmd_list->acks_length < Smpt_Length_Number_Of_Acks)
    {
        cmd_list->acks_length++;
    }

    if (has_request(cmd))
    {
        if (cmd_list->number_of_expected == 0)
        {
            smpt_error("smpt_add_response(): Received response without request.");
        }
        else
        {
            smpt_check_expected_ack(&ack, cmd_list);
        }
    }
}

void smpt_get_last_ack(Smpt_ack *const ack, uint32_t new_acks_available,
                       const Smpt_cmd_list *const cmd_list)
{
    uint32_t last_response_index = 0;

    /**
     * what about the cases
     *
     * new_acks_available > Smpt_Saveable_Acks_Size
     *
     * ? Is there an other point in the lib where we test this case or do we
     * have to print out an error message here like
     *
     * if(new_acks_available > Smpt_Saveable_Acks_Size)
     * {
     *     smpt_error("smpt_get_last_ack(): Error: maximum number of acks reached: %d", new_acks_available);
     * }
     */
    last_response_index = ((cmd_list->acks_current_index + Smpt_Length_Number_Of_Acks)
                           - new_acks_available)
                          % Smpt_Length_Number_Of_Acks;

    *ack = cmd_list->acks[last_response_index];
}

void smpt_check_expected_ack(const Smpt_ack *const ack, Smpt_cmd_list *const cmd_list)
{
    uint32_t i;
    bool run_loop = true;
    uint32_t number_of_comparisons = cmd_list->number_of_expected;
    for (i = 0; (i < number_of_comparisons) && run_loop; i++)
    {
        Smpt_ack expected;
        expected.command_number = cmd_list->requests[cmd_list->requests_expected_index].command_number;
        expected.packet_number = cmd_list->requests[cmd_list->requests_expected_index].packet_number;
        expected.command_number += 1; /* + 1 to get the ack cmd */

        if (!compare_acks(&expected, ack))
        {
            smpt_error("smpt_check_expected_ack(): Expected another ack. \n \
                       (expected: cmd: %d, no: %d \n \
                        received: cmd: %d, no: %d)",
                        expected.command_number, expected.packet_number,
                        ack->command_number, ack->packet_number);
        }
        else
        {
            run_loop = false;
        }

        cmd_list->number_of_expected--;
        cmd_list->requests_expected_index = (cmd_list->requests_expected_index + 1) %
                                            Smpt_Length_Number_Of_Acks;
    }

    if (run_loop)
    {
        smpt_error("smpt_check_expected_ack(): Unexpected ack received.\n \
                   (received: cmd: %d no: %d)",
                   ack->command_number, ack->packet_number);
    }

}

bool compare_acks(const Smpt_ack *const ack_a,
                  const Smpt_ack *const ack_b)
{
    return (ack_a->command_number == ack_b->command_number) &&
           (ack_a->packet_number  == ack_b->packet_number);
}

bool has_request(const Smpt_Cmd cmd)
{
    bool result = true;
    switch(cmd)
    {
        case Smpt_Cmd_Dl_Send_Live_Data:
            result = false;
        break;

        case Smpt_Cmd_Dl_Send_File:
            result = false;
        break;

        case Smpt_Cmd_Dl_Send_MMI:
            result = false;
        break;

        case Smpt_Cmd_General_Error:
            result = false;
        break;

        case Smpt_Cmd_Sl_Debug_Message:
            result = false;
        break;

        default:
            result = true;
    }
    return result;
}

/* void print_values()
{
    smpt_debug("responses_length_ = %d", responses_length_);
    smpt_debug("responses_current_index_ = %d, packet no: %d", responses_current_index_,
               responses_[responses_current_index_].packet_number);
    smpt_debug("requests_current_index_ = %d, packet no: %d", requests_current_index_,
               requests_[requests_current_index_].packet_number);
    smpt_debug("requests_expected_index_ = %d, expected packet no: %d", requests_expected_index_,
               requests_[requests_expected_index_].packet_number);
    smpt_debug("number_of_expected_ = %d\n", number_of_expected_);
} */
