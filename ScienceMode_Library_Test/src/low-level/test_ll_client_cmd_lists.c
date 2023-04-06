/***************************************************************************
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2022, MPL and LGPL HASOMED GmbH
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
#include "test_ll_client_cmd_lists.h"

#include "smpt_client_cmd_lists.h"
#include "smpt_ll_definitions.h"

void test_ll_client_cmd_lists(void)
{
    Smpt_cmd_list cmd_list = {0};

    smpt_add_request(Smpt_Cmd_Ll_Init, 1, &cmd_list);
    smpt_add_request(Smpt_Cmd_Ll_Init, 2, &cmd_list);
    smpt_add_request(Smpt_Cmd_Ll_Init, 3, &cmd_list);

    smpt_add_ack(Smpt_Cmd_Ll_Init_Ack, Smpt_Result_Successful, 1, &cmd_list);

    smpt_add_ack(Smpt_Cmd_Ll_Init_Ack, Smpt_Result_Successful, 2, &cmd_list);

    smpt_add_ack(Smpt_Cmd_Ll_Init_Ack, Smpt_Result_Successful, 3, &cmd_list);

    smpt_add_request(Smpt_Cmd_Ll_Init, 4, &cmd_list);

    smpt_add_ack(Smpt_Cmd_Ll_Init_Ack, Smpt_Result_Successful, 4, &cmd_list);

    smpt_add_request(Smpt_Cmd_Ll_Init, 5, &cmd_list);

    smpt_add_ack(Smpt_Cmd_Ll_Init_Ack, Smpt_Result_Successful, 5, &cmd_list);
}
