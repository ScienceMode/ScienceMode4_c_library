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
#include "test_general_smpt.h"

bool test_check_header_and_footer(const uint8_t packet[], uint32_t packet_length)
{
    return ((packet[Smpt_Index_Start_Byte] == Smpt_Packet_Start_Byte) &&
            (packet[Smpt_Index_Length_Msb_Stuff] == Smpt_Packet_Stuffing_Byte) &&
            (packet[Smpt_Index_Length_Lsb_Stuff] == Smpt_Packet_Stuffing_Byte) &&
            (packet[Smpt_Index_Checksum_Msb_Stuff] == Smpt_Packet_Stuffing_Byte) &&
            (packet[Smpt_Index_Checksum_Lsb_Stuff] == Smpt_Packet_Stuffing_Byte) &&
            (packet[packet_length - 1] == Smpt_Packet_Stop_Byte));
}

