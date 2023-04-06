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

TEMPLATE = subdirs
CONFIG += ordered


win_arm {
SUBDIRS = ScienceMode_Library #\
          # ScienceMode_Library_Test
}

!win_arm {
SUBDIRS = ScienceMode_Library \
          ScienceMode_Library_Test
}
# ScienceMode_Test.depends = ScienceMode_Library

# -----------
# Subprojects
# -----------
#
# The main project ScienceMode contains 2 sub projects.
#   * ScienceMode_Lib         - Main library project that contains all important source files
#   *            _Lib_Test    - (Unit)-Test functions for the library
#
# You need to specify the compiler, protocol level, and release/debug-type in the build steps
# using the additional arguments. You need the quotation marks!
#      "CONFIG+=COMPILER, PROTOCOL_LEVEL, RELEASE-DEBUG"
# e.g. "CONFIG+=mingw, smpt_mid-level, myRelease"
#
# See below for the different flag options.
#
# --------
# Compiler
# --------
#
# Use one of the following compiler flags:
#   * mingw
#   * linux_static
#   * linux_x86_amd64_static
#   * linux_shared
#   * win_arm
#   * linux_arm
#   * linux_arm_64
#   * windows_static_x86
#   * windows_static_amd64
#
# Example: "CONFIG+=mingw"
#
# --------------
# Protocol Level
# --------------
#
# Use one or more of the the following flags to enable the protocol levels:
#   * smpt_low-level
#   * smpt_mid-level
#   * smpt_dyscom-level
#   * smpt_all
#
# Example: "CONFIG+=smpt_mid_level"
#
# Caution!: Do not use CONFIG-= in combination with smpt_* options.
#
# ------------------------
# Debug and Release Builds
# ------------------------
#
# Qt's debug/release setting, especially for subprojects is
# poor or not documented properly. This is why we use our own CONFIG option
# settings for a flexible targt build configuration
#
# Use one of the following flags:
#   * myRelease
#   * myDebug
#
# ----------------------------------
# Full Build Configuration Examples
# ---------------------------------
#
# Windows:
#
# Build-Name            qmake arguments   additional arguments
# all (Release)         "CONFIG=release"  "CONFIG+=myRelease smpt_all mingw"
# all (Debug)           "CONFIG=debug"    "CONFIG+=myDebug smpt_all mingw"
# low-level (Debug)     "CONFIG=debug"    "CONFIG+=myDebug smpt_low-level mingw"
# mid-level (Debug)     "CONFIG=debug"    "CONFIG+=myDebug smpt_mid-level mingw"
# dyscom-level (Debug)  "CONFIG=debug"    "CONFIG+=myDebug smpt_dyscom-level mingw"
# "CONFIG+=myRelease smpt_low-level smpt_mid-level smpt_dyscom-level mingw"
#
#
# The customized option handling is done in the include project files:
#   * DestinationDirectories.pri and
#   * BuildConfiguration.pri
#
# The file BuildConfigurationOutput.pri outputs some Qmake variables
#
