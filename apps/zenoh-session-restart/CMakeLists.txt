# SPDX-License-Identifier: Apache-2.0


include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(zenoh_session_restart)

cmake_print_variables(CMAKE_VERBOSE_MAKEFILE)

# TODO Make the library configurable and should be moved at the top level CMakeLists.txt idealy
if(CONFIG_ZENOH_PICO_LIB)
zephyr_include_directories(../../lib/zenoh-pico/include)
add_definitions(-DZENOH_ZEPHYR=1)
endif()

FILE(GLOB app_sources
    PRIVATE
    src/*.c
)

target_sources(app PRIVATE ${app_sources})