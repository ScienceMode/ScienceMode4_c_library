/***************************************************************************//**
* \file smpt_ll_definitions_data_types.c
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

#include "smpt_ll_definitions_data_types.h"
#include "smpt_packet_general.h"
#include "smpt_ll_packet_validity.h"
#include "smpt_messages.h"
#include "string.h"

/******************************************************************************/


void smpt_clear_ll_init(Smpt_ll_init *const ll_init)
{
    Smpt_ll_init empty = {0};
    *ll_init = empty;
}

void smpt_clear_ll_init_ack(Smpt_ll_init_ack *const ll_init_ack)
{
    Smpt_ll_init_ack empty = {0};
    *ll_init_ack = empty;
}

void smpt_clear_ll_channel_config(Smpt_ll_channel_config *const ll_channel_config)
{
    Smpt_ll_channel_config empty = {0};
    *ll_channel_config = empty;
}

void smpt_clear_ll_channel_config_ack(Smpt_ll_channel_config_ack *const ll_channel_config_ack)
{
    Smpt_ll_channel_config_ack empty = {0};
    *ll_channel_config_ack = empty;
}

void smpt_clear_ll_ack(Smpt_ll_ack *const ll_ack)
{
    Smpt_ll_ack empty = {0};
    *ll_ack = empty;
}

void smpt_clear_ll_point(Smpt_ll_point *const ll_point)
{
    Smpt_ll_point empty = {0};
    *ll_point = empty;
}


