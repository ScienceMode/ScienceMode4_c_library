message(*** ScienceMode_Library.pro ***)

CONFIG -= qt

include(../BuildConfiguration.pri)

linux_static|linux_shared|linux_x86_amd64_static|linux_arm|win_arm|linux_arm_64|windows_static_x86|windows_static_amd64|android {
    CONFIG += staticlib
}

# Windows MSVC shared
windows_shared_x86|windows_shared_amd64{
    CONFIG += shared
}
include(../DestinationDirectories.pri)

DESTDIR = $${SMPT_LIB_DESTDIR}
TARGET = $${SMPT_LIB_NAME}

message(------------------------- Science Mode Library Levels -----------------)
include(ScienceMode_Library.pri)

message("--------------------------------------------------------------------------------")
message("DESTDIR = "$${DESTDIR})
message("TARGET  = $${TARGET}")
message("CONFIG  = $${CONFIG}")
message("LIBS    = "$${LIBS})
