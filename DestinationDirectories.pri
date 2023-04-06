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

CONFIG(debug, debug|release) {
    DESTDIR_TEMP = "debug"
} else {
    DESTDIR_TEMP = "release"
}

LVL_DIR = "smpt"

smpt_low-level:        DEFINES += SMPT_LOW_LEVEL
smpt_mid-level:        DEFINES += SMPT_MID_LEVEL SMPT_LOW_LEVEL
smpt_dyscom-level:     DEFINES += SMPT_DYSCOM_LEVEL

!smpt_all {
  smpt_low-level:        LVL_DIR=$${LVL_DIR}_low-level
  smpt_mid-level:        LVL_DIR=$${LVL_DIR}_mid-level
  smpt_dyscom-level:     LVL_DIR=$${LVL_DIR}_dyscom-level
}

DESTDIR_TEMP = $${DESTDIR_TEMP}/$$LVL_DIR

linux_static|linux_shared {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/linux_x86
}

linux_x86_amd64_static {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/linux_x86_amd64/static
}

linux_arm {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/linux_arm
}

win_arm {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/win_arm
}

linux_arm_64 {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/linux_arm_64
}

windows_static_x86 {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/windows_x86/static
}

windows_static_amd64 {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/windows_amd64/static
}

mingw {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/mingw
}

windows_shared_x86 {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/windows_x86/shared
}

windows_shared_amd64 {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/windows_amd64/shared
}

macos {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/mac_os
}

android {
    SMPT_LIB_DESTDIR = $${DESTDIR_TEMP}/android
}

#############################
# TARGET name
CONFIG(debug, debug|release) {
    SMPT_LIB_NAME = smptd
} else {
    SMPT_LIB_NAME = smpt
}

*msvc*{
    SMPT_LIB_NAME = lib$${SMPT_LIB_NAME}
}
*g++*{
#
}

OBJECTS_DIR = $$SMPT_LIB_DESTDIR/.obj
MOC_DIR = $$SMPT_LIB_DESTDIR/.moc
RCC_DIR = $$SMPT_LIB_DESTDIR/.qrc
UI_DIR = $$SMPT_LIB_DESTDIR/.ui
