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

CONFIG -= debug_and_release

# adding all smpt protocol levels if smpt is defined
smpt_all: CONFIG+= smpt_low-level smpt_mid-level \
                   smpt_dyscom-level

# adding low-level when mid-level is defined
!smpt_all: {
  !smpt_low-level: {
    smpt_mid-level: CONFIG+=smpt_low-level
  }
}

# remove some of Qt's default compiler settings
#QMAKE_CFLAGS_RELEASE  = ""
#QMAKE_CFLAGS_DEBUG    = ""
#QMAKE_CFLAGS_WARN_OFF = ""
#QMAKE_CFLAGS_WARN_ON  = ""

mingw {
    CONFIG += staticlib
    CONFIG += shared

    CONFIG(debug, debug|release) {
        QMAKE_CFLAGS += -std=c99 -pedantic-errors -Werror -Wall -g -D_BSD_SOURCE
    }
    else {
        QMAKE_CFLAGS += -std=c99 -pedantic-errors -Werror -Wall -O2 -D_BSD_SOURCE
    }
}

# If you cross-compile 32/64-Bit on Linux, you will probably need the following libraries:
# gcc-multilib and g++-multilib.
linux_static|linux_shared {
#    CONFIG += staticlib
    QMAKE_CFLAGS = -std=c99 -pedantic-errors -Werror -Wall -O2 -D_BSD_SOURCE -pipe -fPIC -m32
    QMAKE_LINK   = gcc
    QMAKE_LFLAGS = -m32
}

linux_x86_amd64_static {
    QMAKE_CFLAGS = -std=c99 -pedantic-errors -Werror -Wall -O2 -D_BSD_SOURCE -pipe -fPIC -m64
    QMAKE_LINK   = gcc
    QMAKE_LFLAGS = -m64
#    CONFIG += staticlib
}

android {
    QMAKE_CC     = arm-linux-androideabi-gcc
    QMAKE_CFLAGS_RELEASE =
    QMAKE_CFLAGS = -O2 -D_BSD_SOURCE -pipe -fPIC -Wall #-Werror
    QMAKE_LINK   = arm-linux-androideabi-gcc
    QMAKE_LFLAGS =
    INCLUDEPATH += %appdata%/Local/Android/sdk/ndk-bundle/platforms/android-23/arch-arm/usr/include/
}

win_arm {
#    CONFIG += staticlib
    QMAKE_CC     = C:/arm/bin/arm-none-eabi-gcc
    QMAKE_CFLAGS = -mcpu=cortex-m4  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -mthumb  -pedantic-errors -Werror -Wall -std=c99 -D_BSD_SOURCE -pipe -fPIC -DUC_MAIN  #  -std=gnu11   # -std=gnu11
    QMAKE_CFLAGS_RELEASE =
    INCLUDEPATH += C:/arm/arm-none-eabi/include
    QMAKE_LFLAGS =
    QMAKE_CXX = C:/arm/bin/arm-none-eabi-g++
    QMAKE_LINK = C:/arm/bin/arm-none-eabi-gcc
}

linux_arm {
#    CONFIG += staticlib
    QMAKE_CC     = arm-linux-gnueabihf-gcc
    QMAKE_CFLAGS = -std=c99 -pedantic-errors -Werror -Wall -O2 -D_BSD_SOURCE -pipe -fPIC
    QMAKE_CFLAGS_RELEASE =
    QMAKE_LINK   = arm-linux-gnueabihf-gcc
    QMAKE_LFLAGS =
}


linux_arm_64 {
#    CONFIG += staticlib
    QMAKE_CC     = aarch64-linux-gnu-gcc
    QMAKE_CFLAGS = -std=c99 -pedantic-errors -Werror -Wall -O2 -D_BSD_SOURCE -pipe -fPIC
    QMAKE_CFLAGS_RELEASE =
    QMAKE_LINK   = aarch64-linux-gnu-gcc
    QMAKE_LFLAGS =
}

# Windows MSVC static
windows_static_x86|windows_static_amd64 {
#    CONFIG += staticlib
    QMAKE_CFLAGS = /TC
    QMAKE_CFLAGS_RELEASE = -O2 -MD
}

# Windows MSVC shared
windows_shared_x86|windows_shared_amd64 {
#    CONFIG += shared
    QMAKE_CFLAGS += /TC
    QMAKE_CFLAGS_RELEASE = -O2 -MT -DSMPT_DLL -DSMPT_EXPORTS
}

macos {
    #TARGET_EXT = .bc
    #QMAKE_EXT_OBJ = .bc
    CONFIG += staticlib
    #CONFIG =
    QMAKE_CC     = /home/entwickler/Entwicklung/osxcross/target/bin/x86_64h-apple-darwin14-cc
    QMAKE_CFLAGS = -g3 -Wall -fmessage-length=0 -I/home/entwickler/Entwicklung/osxcross/target/SDK/MacOSX10.10.sdk/System/Library/Frameworks/IOKit.framework/Headers/ -I/home/entwickler/Entwicklung/osxcross/target/SDK/MacOSX10.10.sdk/System/Library/Frameworks/CoreFoundation.framework/Headers/
    QMAKE_CFLAGS_RELEASE =
    QMAKE_CFLAGS_DEBUG =
    QMAKE_LINK   = /home/entwickler/Entwicklung/osxcross/target/bin/x86_64h-apple-darwin14-cc
    #QMAKE_LFLAGS = -m64 -Wl -lpthread -L/home/entwickler/Entwicklung/osxcross/target/SDK/MacOSX10.10.sdk/usr/lib/
    QMAKE_LFLAGS = -L/home/entwickler/Entwicklung/osxcross/target/SDK/MacOSX10.10.sdk/usr/lib/
    QMAKE_LFLAGS_RELEASE =

    #TARGET_EXT = .bc
    #QMAKE_EXT_OBJ = .bc
    #QMAKE_CFLAGS += -emit-llvm
    #QMAKE_LIB = llvm-ld -link-as-library -o
    #QMAKE_RUN_CC = $(CC) $(CFLAGS) $(INCPATH) -c $src -o $obj
    #QMAKE_RUN_CC = $(CC) $(CFLAGS) $(INCPATH) -c $src -o $obj
}

!mingw:!linux_static:!linux_shared:!windows_static_x86:!windows_static_amd64:!linux_arm:!linux_arm_64:!win_arm:!linux_x86_amd64_static:!macos:!android {
  message("!!!Project error!!!")
  message("You must set exactly one of the following Config variable options for this project.")
  message("")
  message("          mingw")
  message("          linux_static")
  message("          linux_x86_amd64_static")
  message("          linux_shared")
  message("          linux_arm")
  message("          linux_arm_64")
  message("          win_arm")
  message("          windows_static_x86")
  message("          windows_static_amd64")
  message("          android")
  message("")
  message("You can do this in the Build Configuration of this Qt-Project in Qt-Creator")
}
