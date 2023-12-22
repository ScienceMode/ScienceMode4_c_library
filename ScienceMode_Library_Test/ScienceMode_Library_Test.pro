#/***************************************************************************
#* This Source Code Form is subject to the terms of the Mozilla Public
#* License, v. 2.0. If a copy of the MPL was not distributed with this
#* file, You can obtain one at https://mozilla.org/MPL/2.0/.
#*
#* Copyright (c) 2013-2023, MPL and LGPL HASOMED GmbH
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

message(*** ScienceMode_Library_Test.pro ***)
TEMPLATE = app
CONFIG += console
CONFIG -= qt


TARGET = smptlus_test

CONFIG -= app_bundle

include(../BuildConfiguration.pri)
include(../DestinationDirectories.pri)

INCLUDEPATH += ../ScienceMode_Library/include/general \
               ../ScienceMode_Library/include/general/packet \
               ../ScienceMode_Library/include/general/packet_input_buffer \
               ../ScienceMode_Library/include/general/serial_port \
               ../ScienceMode_Library/include/low-level \
               ../ScienceMode_Library/include/mid-level \
               ../ScienceMode_Library/include/dyscom-level \
               include/general   \
               include/low-level \
               include/mid-level \
               include/dyscom-level

CONFIG(debug, debug|release) {
    SM_LIB = libsmptd
} else {
    SM_LIB = libsmpt
}

windows_*{
    LIBS += ../ScienceMode_Library/$${SMPT_LIB_DESTDIR}/$${SM_LIB}.lib
    PRE_TARGETDEPS += ../ScienceMode_Library/$${SMPT_LIB_DESTDIR}/$${SM_LIB}.lib
}
linux*|mingw{
    LIBS += ../ScienceMode_Library/$${SMPT_LIB_DESTDIR}/$${SM_LIB}.a
    PRE_TARGETDEPS += ../ScienceMode_Library/$${SMPT_LIB_DESTDIR}/$${SM_LIB}.a
}
macos{
    LIBS += ../ScienceMode_Library/$${SMPT_LIB_DESTDIR}/$${SM_LIB}.a
    PRE_TARGETDEPS += ../ScienceMode_Library/$${SMPT_LIB_DESTDIR}/$${SM_LIB}.a
}

HEADERS += \
    include/general/test_general_smpt.h \
    include/general/test_definitions.h \
    include/general/test_packet_client_server.h \
    include/general/test_get_file.h \
    include/general/test_get_file_ack.h \
    include/general/test_file_block.h \
    include/general/test_file.h \
    include/general/test_serial_port.h \
    include/general/test_find_packet.h \
    include/general/test_ack.h \
    include/general/test_get_version_ack.h \
    include/general/test_get_battery_status_ack.h \
    include/general/test_get_device_id_ack.h \
    include/general/test_set_power.h \
    include/general/test_get_main_status_ack.h \
    include/general/test_get_stim_status_ack.h \
    include/general/test_remove_file.h

smpt_low-level {
    HEADERS += \
        include/low-level/test_ll_packet_client_server.h \
        include/low-level/test_ll_init_ping.h \
        include/low-level/test_ll_client_cmd_lists.h \
        include/low-level/test_ll_init.h \
        include/low-level/test_ll_definitions.h \
        include/low-level/test_ll_init_ack.h \
    	include/low-level/test_ll_channel_config.h \
        include/low-level/test_ll_channel_config_ack.h \
        include/low-level/test_ll_examples.h
}

smpt_mid-level {
    HEADERS += \
        include/mid-level/test_ml_packet_client_server.h \
        include/mid-level/test_ml_examples.h
}

smpt_dyscom-level {
    HEADERS += \
        include/dyscom-level/test_dl_packet_client_server.h \
        include/dyscom-level/test_dl_common.h \
        include/dyscom-level/test_dl_init.h \
        include/dyscom-level/test_dl_stop.h \
        include/dyscom-level/test_dl_get.h \
        include/dyscom-level/test_dl_send_file.h \
        include/dyscom-level/test_dl_send_live_data.h \
        include/dyscom-level/test_dl_send_mmi.h \
        include/dyscom-level/test_dl_power_module.h \
        include/dyscom-level/test_dl_sys.h \
        include/test_dl_examples.h
}


SOURCES += \
    src/main.c \
    src/general/test_general_smpt.c \
    src/general/test_packet_client_server.c \
    src/general/test_file_block.c \
    src/general/test_get_file.c \
    src/general/test_get_file_ack.c \
    src/general/test_file.c \
    src/general/test_find_packet.c \
    src/general/test_ack.c \
    src/general/test_serial_port.c \
    src/general/test_get_version_ack.c \
    src/general/test_get_device_id_ack.c \
    src/general/test_get_battery_status_ack.c \
    src/general/test_set_power.c \
    src/general/test_get_main_status_ack.c \
    src/general/test_get_stim_status_ack.c \
    src/general/test_remove_file.c

smpt_low-level {
    SOURCES += \
        src/low-level/test_ll_packet_client_server.c \
        src/low-level/test_ll_init_ping.c \
        src/low-level/test_ll_client_cmd_lists.c \
        src/low-level/test_ll_init.c \
        src/low-level/test_ll_init_ack.c \
        src/low-level/test_ll_channel_config.c \
        src/low-level/test_ll_channel_config_ack.c \
        src/low-level/test_ll_examples.c
}

smpt_mid-level {
    SOURCES += \
        src/mid-level/test_ml_packet_client_server.c \
        src/mid-level/test_ml_examples.c
}

smpt_dyscom-level {
    SOURCES += \
        src/dyscom-level/test_dl_packet_client_server.c \
        src/dyscom-level/test_dl_init.c \
        src/dyscom-level/test_dl_stop.c \
        src/dyscom-level/test_dl_get.c \
        src/dyscom-level/test_dl_send_file.c \
        src/dyscom-level/test_dl_send_live_data.c \
        src/dyscom-level/test_dl_send_mmi.c \
        src/dyscom-level/test_dl_power_module.c \
        src/dyscom-level/test_dl_examples.c \
        src/dyscom-level/test_dl_sys.c
}



message("--------------------------------------------------------------------------------")
message("DESTDIR = "$${DESTDIR})
message("TARGET  = $${TARGET}")
message("CONFIG  = $${CONFIG}")
message("LIBS    = "$${LIBS})
