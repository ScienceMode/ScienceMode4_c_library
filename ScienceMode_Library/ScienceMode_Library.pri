#/***************************************************************************
#* This Source Code Form is subject to the terms of the Mozilla Public
#* License, v. 2.0. If a copy of the MPL was not distributed with this
#* file, You can obtain one at https://mozilla.org/MPL/2.0/.
#*
#* Copyright (c) 2013-2022, MPL and LGPL HASOMED GmbH
#*
#* Alternatively, the contents of this file may be used under the terms
#* of the GNU Lesser General Public License Version 3.0, as described below:
#*
#* This file is free software: you may copy, redistribute and/or modify
#* it under the terms of the GNU Lesser General Public License as published by the
#* Free Software Foundation, either version 3.0 of the License, or (at your
#* option) any later version.
#*
#* This file is distributed in the hope that it will be useful, but
#* WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
#* Public License for more details.
#*
#* You should have received a copy of the GNU Lesser General Public License
#* along with this program. If not, see http://www.gnu.org/licenses/.
#*******************************************************************************/

TEMPLATE = lib
CONFIG -= qt

#------------------------------------------------------
# general
#----------
message("Using smpt-level: general-level")

INCLUDEPATH += $$PWD                                      \
               $$PWD/include                              \
               $$PWD/include/general                      \
               $$PWD/include/general/packet               \
               $$PWD/include/general/packet_input_buffer  \
               $$PWD/include/general/serial_port          \
               $$PWD/include/general/packet_output_buffer

HEADERS += \
    $$PWD/include/general/smpt_client.h                          \
    $$PWD/include/general/smpt_client_data.h                     \
    $$PWD/include/general/smpt_client_cmd_lists.h                \
    $$PWD/include/general/smpt_definitions.h                     \
    $$PWD/include/general/smpt_definitions_internal.h            \
    $$PWD/include/general/smpt_definitions_data_types.h          \
    $$PWD/include/general/smpt_messages.h                        \
                                                                 \
    $$PWD/include/general/packet/smpt_packet_general.h           \
    $$PWD/include/general/packet/smpt_packet_internal.h          \
    $$PWD/include/general/packet/smpt_packet_validity.h          \
    $$PWD/include/general/packet/smpt_packet_utils.h             \
    $$PWD/include/general/packet/smpt_packet_client.h            \
    $$PWD/include/general/packet/smpt_packet_server.h            \
                                                                                     \
    $$PWD/include/general/packet_input_buffer/smpt_packet_input_buffer.h             \
    $$PWD/include/general/packet_input_buffer/smpt_packet_input_buffer_definitions.h \
    $$PWD/include/general/packet_input_buffer/smpt_packet_input_buffer_internal.h    \
                                                                                     \
    $$PWD/include/general/packet_output_buffer/smpt_packet_output_buffer.h           \
                                                                                     \
    $$PWD/include/general/serial_port/smpt_serial_port.h         \
    $$PWD/include/general/serial_port/smpt_serial_port_windows.h \
    $$PWD/include/general/serial_port/smpt_serial_port_linux.h   \
    $$PWD/include/general/smpt_definitions_file_transfer.h       \
    $$PWD/include/general/smpt_file.h                            \
    $$PWD/include/general/smpt_packet_number_generator.h         \
    $$PWD/include/general/smpt_definitions_power.h               \
    $$PWD/include/general/smpt_client_power.h                    \
    $$PWD/include/general/smpt_client_utils.h

SOURCES += \
     $$PWD/src/general/smpt_messages.c                        \
     $$PWD/src/general/smpt_client.c                          \
     $$PWD/src/general/smpt_client_data.c                     \
     $$PWD/src/general/smpt_client_cmd_lists.c                \
     $$PWD/src/general/smpt_definitions_data_types.c          \
                                                              \
     $$PWD/src/general/packet/smpt_packet_general.c           \
     $$PWD/src/general/packet/smpt_packet_internal.c          \
     $$PWD/src/general/packet/smpt_packet_validity.c          \
     $$PWD/src/general/packet/smpt_packet_utils.c             \
     $$PWD/src/general/packet/smpt_packet_client.c            \
     $$PWD/src/general/packet/smpt_packet_server.c            \
                                                                                  \
     $$PWD/src/general/packet_input_buffer/smpt_packet_input_buffer.c             \
     $$PWD/src/general/packet_input_buffer/smpt_packet_input_buffer_internal.c    \
                                                                                  \
     $$PWD/src/general/packet_output_buffer/smpt_packet_output_buffer.c           \
                                                                                  \
     $$PWD/src/general/serial_port/smpt_serial_port.c         \
     $$PWD/src/general/serial_port/smpt_serial_port_windows.c \
     $$PWD/src/general/serial_port/smpt_serial_port_linux.c   \
     $$PWD/src/general/smpt_definitions_file_transfer.c       \
     $$PWD/src/general/smpt_file.c                            \
     $$PWD/src/general/smpt_packet_number_generator.c         \
     $$PWD/src/general/smpt_definitions_power.c               \
     $$PWD/src/general/smpt_client_utils.c                    \
     $$PWD/src/general/smpt_client_power.c

#------------------------------------------------------
# low-level
#----------
smpt_low-level {
message("Using smpt-level: low-level")
INCLUDEPATH += $$PWD/include/low-level

HEADERS += \
    $$PWD/include/low-level/smpt_ll_client.h                 \
    $$PWD/include/low-level/smpt_ll_definitions.h            \
    $$PWD/include/low-level/smpt_ll_packet_client.h          \
    $$PWD/include/low-level/smpt_ll_packet_server.h          \
    $$PWD/include/low-level/smpt_ll_packet_validity.h        \
    $$PWD/include/low-level/smpt_ll_definitions_data_types.h \
    $$PWD/include/low-level/smpt_ll_messages.h

SOURCES += \
    $$PWD/src/low-level/smpt_ll_packet_client.c              \
    $$PWD/src/low-level/smpt_ll_packet_server.c              \
    $$PWD/src/low-level/smpt_ll_client.c                     \
    $$PWD/src/low-level/smpt_ll_packet_validity.c            \
    $$PWD/src/low-level/smpt_ll_messages.c                   \
    $$PWD/src/low-level/smpt_ll_definitions_data_types.c
}
#-----------------------------------------------------
# mid-level
#----------
smpt_mid-level {
message("Using smpt-level: mid-level")
INCLUDEPATH += $$PWD/include/mid-level

HEADERS += \
    $$PWD/include/mid-level/smpt_ml_packet_client.h          \
    $$PWD/include/mid-level/smpt_ml_definitions_data_types.h \
    $$PWD/include/mid-level/smpt_ml_packet_server.h          \
    $$PWD/include/mid-level/smpt_ml_packet_validity.h        \
    $$PWD/include/mid-level/smpt_ml_client.h                 \
    $$PWD/include/mid-level/smpt_ml_definitions.h            \
    $$PWD/include/mid-level/smpt_ml_packet_utils.h

SOURCES += \
    $$PWD/src/mid-level/smpt_ml_packet_client.c              \
    $$PWD/src/mid-level/smpt_ml_definitions_data_types.c     \
    $$PWD/src/mid-level/smpt_ml_packet_server.c              \
    $$PWD/src/mid-level/smpt_ml_packet_validity.c            \
    $$PWD/src/mid-level/smpt_ml_client.c                     \
    $$PWD/src/mid-level/smpt_ml_packet_utils.c
}


#-----------------------------------------------------
# dyscom-level
#-------------
smpt_dyscom-level {
message("Using smpt-level: dyscom-level")
INCLUDEPATH += $$PWD/include/dyscom-level

HEADERS += \
    $$PWD/include/dyscom-level/smpt_dl_packet_client.h          \
    $$PWD/include/dyscom-level/smpt_dl_definitions_data_types.h \
    $$PWD/include/dyscom-level/smpt_dl_packet_server.h          \
    $$PWD/include/dyscom-level/smpt_dl_packet_validity.h        \
    $$PWD/include/dyscom-level/smpt_dl_client.h                 \
    $$PWD/include/dyscom-level/smpt_dl_definitions.h            \
    $$PWD/include/dyscom-level/smpt_dl_packet_utils.h           \
    $$PWD/include/dyscom-level/smpt_dl_client.h                 \
    $$PWD/include/dyscom-level/smpt_dl_definitions_data_types.h

SOURCES += \
    $$PWD/src/dyscom-level/smpt_dl_packet_client.c              \
    $$PWD/src/dyscom-level/smpt_dl_definitions_data_types.c     \
    $$PWD/src/dyscom-level/smpt_dl_packet_server.c              \
    $$PWD/src/dyscom-level/smpt_dl_packet_validity.c            \
    $$PWD/src/dyscom-level/smpt_dl_client.c                     \
    $$PWD/src/dyscom-level/smpt_dl_packet_utils.c
}

