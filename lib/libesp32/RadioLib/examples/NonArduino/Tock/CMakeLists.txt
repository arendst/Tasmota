# RadioLib Non-Arduino Tock Library CMake script
#
# Licensed under the MIT License
#
# Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.18)

# create the project
project(tock-sx1261)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c/userland_generic.ld)

include("tock.cmake")

# when using debuggers such as gdb, the following line can be used
#set(CMAKE_BUILD_TYPE Debug)

# if you did not build RadioLib as shared library (see wiki),
# you will have to add it as source directory
# the following is just an example, yours will likely be different
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../../RadioLib" "${CMAKE_CURRENT_BINARY_DIR}/RadioLib")

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# link with RadioLib and libtock-c
target_link_libraries(${PROJECT_NAME} PUBLIC
    RadioLib
    ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c/libtock/build/cortex-m4/libtock.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c/libc++/cortex-m/libgcc.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c/libc++/cortex-m/libstdc++.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c/newlib/cortex-m/v7-m/libc.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c/newlib/cortex-m/v7-m/libm.a
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libtock-c
)

# you can also specify RadioLib compile-time flags here
#target_compile_definitions(${PROJECT_NAME} PUBLIC RADIOLIB_DEBUG RADIOLIB_VERBOSE)
