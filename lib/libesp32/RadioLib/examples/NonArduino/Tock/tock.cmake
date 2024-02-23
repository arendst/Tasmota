# Tock target specific CMake file
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
#
# This is copied from https://github.com/Lora-net/LoRaMac-node/pull/1390
# and has been relicensed by the original author

include("toolchain-arm-none-eabi.cmake")

if(NOT DEFINED LINKER_SCRIPT)
message(FATAL_ERROR "No linker script defined")
endif(NOT DEFINED LINKER_SCRIPT)
message("Linker script: ${LINKER_SCRIPT}")

#---------------------------------------------------------------------------------------
# Set compiler/linker flags
#---------------------------------------------------------------------------------------

set(STACK_SIZE 4096)
set(APP_HEAP_SIZE 2048)
set(KERNEL_HEAP_SIZE 2048)

# Object build options
set(OBJECT_GEN_FLAGS "-mthumb -g2 -fno-builtin -mcpu=cortex-m4 -Wall -Wextra -pedantic -Wno-unused-parameter -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize -frecord-gcc-switches -gdwarf-2 -Os -fdata-sections -ffunction-sections -fstack-usage -Wl,--emit-relocs -fPIC -mthumb -mfloat-abi=soft -msingle-pic-base -mpic-register=r9 -mno-pic-data-is-text-relative -D__TOCK__ -DSVCALL_AS_NORMAL_FUNCTION -DSOFTDEVICE_s130")

set(CMAKE_C_FLAGS "${OBJECT_GEN_FLAGS} -std=gnu99 " CACHE INTERNAL "C Compiler options")
set(CMAKE_CXX_FLAGS "${OBJECT_GEN_FLAGS} -std=c++20 " CACHE INTERNAL "C++ Compiler options")
set(CMAKE_ASM_FLAGS "${OBJECT_GEN_FLAGS} -x assembler-with-cpp " CACHE INTERNAL "ASM Compiler options")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections --specs=nano.specs --specs=nosys.specs -mthumb -g2 -mcpu=cortex-m4 -mabi=aapcs -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Xlinker --defsym=STACK_SIZE=${STACK_SIZE} -Xlinker --defsym=APP_HEAP_SIZE=${APP_HEAP_SIZE} -Xlinker --defsym=KERNEL_HEAP_SIZE=${KERNEL_HEAP_SIZE} -nostdlib -Wl,--start-group" CACHE INTERNAL "Linker options")
