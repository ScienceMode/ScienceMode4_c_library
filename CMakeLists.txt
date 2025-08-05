cmake_minimum_required(VERSION 3.10)
project(ScienceMode 
    VERSION 4.0.0
    DESCRIPTION "ScienceMode communication library for FES devices"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)

# Options to enable different layers
option(SMPT_LOW_LEVEL "Enable Low-Level module" ON)
option(SMPT_MID_LEVEL "Enable Mid-Level module" ON)
option(SMPT_DYSCOM_LEVEL "Enable Dyscom-Level module" ON)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)

# Library source files
file(GLOB_RECURSE LIB_SOURCES
    "ScienceMode_Library/src/general/*.c"
    "ScienceMode_Library/src/general/packet/*.c"
    "ScienceMode_Library/src/general/packet_input_buffer/*.c"
    "ScienceMode_Library/src/general/packet_output_buffer/*.c"
    "ScienceMode_Library/src/general/serial_port/*.c"
    "ScienceMode_Library/src/low-level/*.c"
    "ScienceMode_Library/src/mid-level/*.c"
    "ScienceMode_Library/src/dyscom-level/*.c"
)

# Library include directories (only used internally)
set(LIBRARY_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/packet
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/packet_input_buffer
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/packet_output_buffer
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/serial_port
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/low-level
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/mid-level
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/dyscom-level
)

# Use BUILD_SHARED_LIBS option to determine library type
if(BUILD_SHARED_LIBS)
    add_library(smpt SHARED ${LIB_SOURCES})
    set_target_properties(smpt PROPERTIES 
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        OUTPUT_NAME "smpt"  # Will create libsmpt.so
    )
    # When building shared libraries on Windows, export symbols
    if(WIN32)
        target_compile_definitions(smpt PRIVATE SMPT_EXPORTS)
        target_compile_definitions(smpt PUBLIC SMPT_DLL)
    endif()
else()
    add_library(smpt STATIC ${LIB_SOURCES})
    set_target_properties(smpt PROPERTIES 
        OUTPUT_NAME "smpt"  # Will create libsmpt.a
    )
    if(WIN32)
        target_compile_definitions(smpt PUBLIC SMPT_STATIC)
    endif()
endif()

# Use proper include directories for both building and installing
target_include_directories(smpt 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/packet>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/packet_input_buffer>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/packet_output_buffer>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/general/serial_port>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/low-level>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/mid-level>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library/include/dyscom-level>
        $<INSTALL_INTERFACE:include/ScienceMode4>
)
target_link_libraries(smpt PUBLIC m)  # Link with math library

# Add compile definitions based on options
if(SMPT_LOW_LEVEL)
    target_compile_definitions(smpt PUBLIC SMPT_LOW_LEVEL)
endif()
if(SMPT_MID_LEVEL)
    target_compile_definitions(smpt PUBLIC SMPT_MID_LEVEL)
endif()
if(SMPT_DYSCOM_LEVEL)
    target_compile_definitions(smpt PUBLIC SMPT_DYSCOM_LEVEL)
endif()

# Test source files
file(GLOB_RECURSE TEST_SOURCES
    "ScienceMode_Library_Test/src/*.c"
)

# Test include directories
set(TEST_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library_Test/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library_Test/include/general
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library_Test/include/low-level
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library_Test/include/mid-level
    ${CMAKE_CURRENT_SOURCE_DIR}/ScienceMode_Library_Test/include/dyscom-level
    # Do not include LIBRARY_INCLUDE_DIRS directly, as they'll be included via the target dependency
)

add_executable(sciencemode_tests ${TEST_SOURCES})
target_include_directories(sciencemode_tests PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(sciencemode_tests PRIVATE smpt)

# Propagate the same compile definitions to the test executable
if(SMPT_LOW_LEVEL)
    target_compile_definitions(sciencemode_tests PRIVATE SMPT_LOW_LEVEL)
endif()
if(SMPT_MID_LEVEL)
    target_compile_definitions(sciencemode_tests PRIVATE SMPT_MID_LEVEL)
endif()
if(SMPT_DYSCOM_LEVEL)
    target_compile_definitions(sciencemode_tests PRIVATE SMPT_DYSCOM_LEVEL)
endif()

# Configure a ScienceModeConfig.cmake file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ScienceModeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ScienceModeConfig.cmake
    @ONLY
)

# Install the config file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/ScienceModeConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ScienceMode
)

# Add export targets for proper installation
include(GNUInstallDirs)

# Install targets
install(TARGETS smpt 
    EXPORT ScienceModeTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header files
install(DIRECTORY ScienceMode_Library/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ScienceMode4
)

# Install the export set
install(EXPORT ScienceModeTargets
    FILE ScienceModeTargets.cmake
    NAMESPACE ScienceMode::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ScienceMode
)

# Add testing support
enable_testing()
add_test(NAME ScienceModeTest COMMAND sciencemode_tests)
