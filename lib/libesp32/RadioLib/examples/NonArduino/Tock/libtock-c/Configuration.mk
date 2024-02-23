################################################################################
##
## libtock-c build system configuration.
##
## This sets all of the parameters and flags required to build libtock-c
## applications for the architectures Tock supports.
##
## Included by AppMakefile.mk and TockLibrary.mk.
##
################################################################################

# Ensure that this file is only included once.
ifndef CONFIGURATION_MAKEFILE
CONFIGURATION_MAKEFILE = 1

# Remove built-in rules and variables
# n.b. no-op for make --version < 4.0
MAKEFLAGS += -r
MAKEFLAGS += -R

# Toolchain programs.
AR := -ar
AS := -as
CXX := -g++
OBJDUMP := -objdump
RANLIB := -ranlib
READELF := -readelf
SIZE := -size

# Set default region sizes for process memory requirements.
STACK_SIZE       ?= 2048
APP_HEAP_SIZE    ?= 1024
KERNEL_HEAP_SIZE ?= 1024

# Set default required kernel version.
KERNEL_MAJOR_VERSION     ?= 2
KERNEL_MINOR_VERSION     ?= 0

# PACKAGE_NAME is used to identify the application for IPC and for error
# reporting. This can be overwritten per-app to customize the name, otherwise we
# default to the name of the directory the app is in.
PACKAGE_NAME ?= $(shell basename "$(shell pwd)")

# Tock app targets.
#
# This is a list of all of the different targets to build an app for which will
# all be bundled into a TAB. This allows us to build an app for any board Tock
# supports, and wait until a TAB is installed onto a board to figure out which
# specific binary that hardware platform needs.
#
# Each entry is itself a list:
#
# 1. The name of the architecture. This is used for naming generated files and
#    variables in the Makefiles. It is generally just a human-readable name.
# 2. (Optional) The name to use when creating the output file.
# 3. (Optional) The address to use as the fixed start of flash.
# 4. (Optional) The address to use as the fixed start of RAM.
#
# By default we currently only build the Cortex-M targets. To enable the RISC-V
# targets, set the RISCV variable like so:
#
#     $ make RISCV=1
#
# Once the RV32 toolchain distribution stabilizes (as of June 2020 the toolchain
# isn't as easily obtained as we would like), we intend to make the RISC-V
# targets build by default as well.
ifeq ($(RISCV),)
TOCK_TARGETS ?= cortex-m0 cortex-m3 cortex-m4 cortex-m7
else
# Specific addresses useful for the OpenTitan hardware memory map.
OPENTITAN_TOCK_TARGETS := rv32imc|rv32imc.0x20030080.0x10005000|0x20030080|0x10005000\
                          rv32imc|rv32imc.0x20030880.0x10008000|0x20030880|0x10008000\
                          rv32imc|rv32imc.0x20032080.0x10008000|0x20032080|0x10008000\
                          rv32imc|rv32imc.0x20034080.0x10008000|0x20034080|0x10008000

# Specific addresses useful for the ARTY-E21 FPGA softcore hardware memory map.
ARTY_E21_TOCK_TARGETS := rv32imac|rv32imac.0x40430060.0x80004000|0x40430060|0x80004000\
                         rv32imac|rv32imac.0x40440060.0x80007000|0x40440060|0x80007000

# Include the RISC-V targets.
#  rv32imac|rv32imac.0x20040060.0x80002800 # RISC-V for HiFive1b
#  rv32imac|rv32imac.0x403B0060.0x3FCC0000 # RISC-V for ESP32-C3
#  rv32imc|rv32imc.0x41000060.0x42008000   # RISC-V for LiteX Arty-A7
#  rv32imc|rv32imc.0x00080060.0x40008000   # RISC-V for LiteX Simulator
TOCK_TARGETS ?= cortex-m0\
                cortex-m3\
                cortex-m4\
                cortex-m7\
                rv32imac|rv32imac.0x20040060.0x80002800|0x20040060|0x80002800\
                rv32imac|rv32imac.0x403B0060.0x3FCC0000|0x403B0060|0x3FCC0000\
                rv32imc|rv32imc.0x41000060.0x42008000|0x41000060|0x42008000\
                rv32imc|rv32imc.0x00080060.0x40008000|0x00080060|0x40008000\
                $(OPENTITAN_TOCK_TARGETS) \
                $(ARTY_E21_TOCK_TARGETS)
endif

# Generate `TOCK_ARCH_FAMILIES`, the set of architecture families which will be
# used to determine toolchains to use in the build process.
TOCK_ARCH_FAMILIES := $(sort $(foreach target, $(TOCK_TARGETS), $(strip \
  $(findstring rv32i,$(target)) \
  $(findstring cortex-m,$(target)))))

# Generate `TOCK_ARCHS`, the set of architectures listed in `TOCK_TARGETS`.
#
# The architecture name is used extensively to create the correct build commands
# for each architecture. Make targets are automatically generated in
# `AppMakefile.mk` based on the list of `TOCK_TARGETS`. The remainder of this
# file uses the architecture name to pull the correct flags for each stage in
# the build process.
TOCK_ARCHS := $(sort $(foreach target, $(TOCK_TARGETS), $(firstword $(subst |, ,$(target)))))

# Check if elf2tab exists, if not, install it using cargo.
ELF2TAB ?= elf2tab
ELF2TAB_REQUIRED_VERSION := 0.7.0
ELF2TAB_EXISTS := $(shell $(SHELL) -c "command -v $(ELF2TAB)")
ELF2TAB_VERSION := $(shell $(SHELL) -c "$(ELF2TAB) --version | cut -d ' ' -f 2")

# Check elf2tab version.
UPGRADE_ELF2TAB := $(shell $(SHELL) -c "printf '%s\n%s\n' '$(ELF2TAB_REQUIRED_VERSION)' '$(ELF2TAB_VERSION)' | sort --check=quiet --version-sort || echo yes")

ifeq ($(UPGRADE_ELF2TAB),yes)
  $(info Trying to update elf2tab to >= $(ELF2TAB_REQUIRED_VERSION))
  ELF2TAB_EXISTS =
endif

ifndef ELF2TAB_EXISTS
  $(shell cargo install elf2tab)
  # Check elf2tab version after install
  ELF2TAB_VERSION := $(shell $(SHELL) -c "$(ELF2TAB) --version | cut -d ' ' -f 2")
  UPGRADE_ELF2TAB := $(shell $(SHELL) -c "printf '%s\n%s\n' '$(ELF2TAB_REQUIRED_VERSION)' '$(ELF2TAB_VERSION)' | sort --check=quiet --version-sort || echo yes")
  ifeq ($(UPGRADE_ELF2TAB),yes)
    $(error Failed to automatically update elf2tab, please update manually elf2tab to >= $(ELF2TAB_REQUIRED_VERSION))
  endif
endif

################################################################################
##
## Shared build flags for all architectures in libtock-c.
##
################################################################################

# elf2tab flags.
#
# Provide the name, memory sizes, and required kernel version as arguments to
# elf2tab so it can include the parameters in the TBF header.
ELF2TAB_ARGS += -n $(PACKAGE_NAME)
ELF2TAB_ARGS += --stack $(STACK_SIZE) --app-heap $(APP_HEAP_SIZE) --kernel-heap $(KERNEL_HEAP_SIZE)
ELF2TAB_ARGS += --kernel-major $(KERNEL_MAJOR_VERSION) --kernel-minor $(KERNEL_MINOR_VERSION)

# Flags for building app Assembly, C, and C++ files used by all architectures.
# n.b. CPPFLAGS are shared for C and C++ sources (it's short for C PreProcessor,
# and C++ uses the C preprocessor). To specify flags for only C or C++, use
# CFLAGS for C only and CXXFLAGS for C++ only. [While we're on the trivia
# lesson, CXX is shorthand for C++ because folks on the unix/gnu side of history
# needed a valid letter rather than a symbol (an X is a rotated +). Confusingly,
# the dos/microsoft lineage chose `.cpp` to address this same issue, leading to
# confusion nowadays about the meaning of 'cpp'.]
override ASFLAGS += -mthumb
override CFLAGS  += -std=gnu11
override CPPFLAGS += \
      -frecord-gcc-switches\
      -gdwarf-2\
      -Os\
      -fdata-sections -ffunction-sections\
      -fstack-usage\
      -D_FORTIFY_SOURCE=2\
      -Wall\
      -Wextra
override WLFLAGS += \
      -Wl,--warn-common\
      -Wl,--gc-sections\
      -Wl,--build-id=none

# Flags to improve the quality and information in listings (debug target)
OBJDUMP_FLAGS += --disassemble-all --source -C --section-headers

# Use a generic linker script for all libtock-c apps.
LAYOUT ?= $(TOCK_USERLAND_BASE_DIR)/userland_generic.ld

# Various flags for a specific toolchain. Different compilers may have different
# supported features. For GCC we warn if the compiler estimates the stack usage
# will be greater than the allocated stack size.
override CPPFLAGS_gcc += -Wstack-usage=$(STACK_SIZE)

# Generic PIC flags for architectures with compiler support for FDPIC. Note!
# These flags are not sufficient for full PIC support as Tock requires. The
# `-fPIC` flag generally only allows the .text and .data sections to be at
# different relative addresses. However, the .text and RAM sections are not
# fully relocatable. Therefore, just including these flags is not sufficient to
# build a full PIC app for Tock. So, we split these out, and only include them
# for architectures where we have full PIC support.
override CPPFLAGS_PIC += \
      -Wl,--emit-relocs\
      -fPIC

################################################################################
##
## RISC-V compiler/linker flags
##
################################################################################

# RISC-V toolchains, irrespective of their name-tuple, can compile for
# essentially any target. Thus, try a few known names and choose the one for
# which a compiler is found.
ifneq (,$(shell which riscv64-none-elf-gcc 2>/dev/null))
  TOOLCHAIN_rv32i := riscv64-none-elf
else ifneq (,$(shell which riscv32-none-elf-gcc 2>/dev/null))
  TOOLCHAIN_rv32i := riscv32-none-elf
else ifneq (,$(shell which riscv64-elf-gcc 2>/dev/null))
  TOOLCHAIN_rv32i := riscv64-elf
else ifneq (,$(shell which riscv64-unknown-elf-clang 2>/dev/null))
  TOOLCHAIN_rv32i := riscv64-unknown-elf
else ifneq (,$(shell which riscv32-unknown-elf-clang 2>/dev/null))
  TOOLCHAIN_rv32i := riscv32-unknown-elf
else
  # Fallback option. We don't particularly want to throw an error (even if
  # RISCV=1 is set) as this configuration makefile can be useful without a
  # proper toolchain.
  TOOLCHAIN_rv32i := riscv64-unknown-elf
endif
TOOLCHAIN_rv32imac := $(TOOLCHAIN_rv32i)
TOOLCHAIN_rv32imc := $(TOOLCHAIN_rv32i)

# For RISC-V we default to GCC, but can support clang as well. Eventually, one
# or both toolchains might support the PIC we need, at which point we would
# default to that.
ifeq ($(CLANG),)
  # Default to GCC
  CC_rv32     := -gcc
else
  # If `CLANG=1` on command line, use -clang.
  CC_rv32     := -clang
endif
CC_rv32i    := $(CC_rv32)
CC_rv32imc  := $(CC_rv32)
CC_rv32imac := $(CC_rv32)

# Set the toolchain specific flags.
#
# Note: There are no non-gcc, clang-specific flags currently in use, so there is
# no equivalent CPPFLAGS_clang currently. If there are clang-only flags in the
# future, one can/should be added.
ifeq ($(findstring -gcc,$(CC_rv32)),-gcc)
  override CPPFLAGS_toolchain_rv32 += $(CPPFLAGS_gcc)
  override CFLAGS_toolchain_rv32 += $(CFLAGS_gcc)
endif

# Set the toolchain specific `CFLAGS` for RISC-V. We use the same generic
# toolchain flags for each RISC-V variant.
override CFLAGS_rv32 += \
      $(CFLAGS_toolchain_rv32)

override CFLAGS_rv32i    += $(CFLAGS_rv32)
override CFLAGS_rv32imc  += $(CFLAGS_rv32)
override CFLAGS_rv32imac += $(CFLAGS_rv32)

# Set the base `CPPFLAGS` for all RISC-V variants based on the toolchain family.
override CPPFLAGS_rv32 += \
      $(CPPFLAGS_toolchain_rv32)

# Set the `CPPFLAGS` for RISC-V. Here we need different flags for different
# variants.
override CPPFLAGS_rv32i += $(CPPFLAGS_rv32) \
      -march=rv32i\
      -mabi=ilp32\
      -mcmodel=medlow

override CPPFLAGS_rv32imc += $(CPPFLAGS_rv32) \
      -march=rv32imc\
      -mabi=ilp32\
      -mcmodel=medlow

override CPPFLAGS_rv32imac += $(CPPFLAGS_rv32) \
      -march=rv32imac\
      -mabi=ilp32\
      -mcmodel=medlow

# Set the base `WLFLAGS` linker flags for all RISC-V variants.
override WLFLAGS_rv32 += \
      -Wl,--no-relax   # Prevent use of global_pointer for RISC-V.

# Use the base linker flags for each RISC-V variant.
override WLFLAGS_rv32i    += $(WLFLAGS_rv32)
override WLFLAGS_rv32imc  += $(WLFLAGS_rv32)
override WLFLAGS_rv32imac += $(WLFLAGS_rv32)

# Set the system libraries we link against for RISC-V. We support C++ apps by
# default.
override LINK_LIBS_rv32 += \
      -lgcc -lstdc++ -lsupc++

override LINK_LIBS_rv32i    += $(LINK_LIBS_rv32)
override LINK_LIBS_rv32imc  += $(LINK_LIBS_rv32)
override LINK_LIBS_rv32imac += $(LINK_LIBS_rv32)

# Use precompiled libaries we provide to link against.
override LEGACY_LIBS_rv32i += \
      $(TOCK_USERLAND_BASE_DIR)/newlib/rv32/rv32i/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/rv32/rv32i/libm.a

override LEGACY_LIBS_rv32im += \
      $(TOCK_USERLAND_BASE_DIR)/newlib/rv32/rv32im/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/rv32/rv32im/libm.a

override LEGACY_LIBS_rv32imc += $(LEGACY_LIBS_rv32im)

override LEGACY_LIBS_rv32imac += \
      $(TOCK_USERLAND_BASE_DIR)/newlib/rv32/rv32imac/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/rv32/rv32imac/libm.a


################################################################################
##
## Cortex-M compiler/linker flags
##
################################################################################

# Setup the correct toolchain for each architecture. ARM has a standard
# toolchain we can use for every variant.
TOOLCHAIN_cortex-m  := arm-none-eabi
TOOLCHAIN_cortex-m0 := $(TOOLCHAIN_cortex-m)
TOOLCHAIN_cortex-m3 := $(TOOLCHAIN_cortex-m)
TOOLCHAIN_cortex-m4 := $(TOOLCHAIN_cortex-m)
TOOLCHAIN_cortex-m7 := $(TOOLCHAIN_cortex-m)

# Setup the correct compiler. For cortex-m we only support GCC as it is the only
# toolchain with the PIC support we need for Tock userspace apps.
CC_cortex-m  := -gcc
CC_cortex-m0 := $(CC_cortex-m)
CC_cortex-m3 := $(CC_cortex-m)
CC_cortex-m4 := $(CC_cortex-m)
CC_cortex-m7 := $(CC_cortex-m)

# Based on the toolchain used by each architecture, add in toolchain-specific
# flags. We assume that each architecture family uses the same toolchain.
ifeq ($(findstring -gcc,$(CC_cortex-m)),-gcc)
  override CPPFLAGS_toolchain_cortex-m += $(CPPFLAGS_gcc)
  override CFLAGS_toolchain_cortex-m += $(CFLAGS_gcc)
endif

override CFLAGS_cortex-m += \
      $(CFLAGS_toolchain_cortex-m)

override CFLAGS_cortex-m0 += $(CFLAGS_cortex-m)
override CFLAGS_cortex-m3 += $(CFLAGS_cortex-m)
override CFLAGS_cortex-m4 += $(CFLAGS_cortex-m)
override CFLAGS_cortex-m7 += $(CFLAGS_cortex-m)

override CPPFLAGS_cortex-m += \
      $(CPPFLAGS_toolchain_cortex-m)\
      $(CPPFLAGS_PIC)\
      -mthumb\
      -mfloat-abi=soft\
      -msingle-pic-base\
      -mpic-register=r9\
      -mno-pic-data-is-text-relative

# Work around https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85606
override CPPFLAGS_cortex-m0 += $(CPPFLAGS_cortex-m) \
      -mcpu=cortex-m0\
      -march=armv6s-m

override CPPFLAGS_cortex-m3 += $(CPPFLAGS_cortex-m) \
      -mcpu=cortex-m3

override CPPFLAGS_cortex-m4 += $(CPPFLAGS_cortex-m) \
      -mcpu=cortex-m4

override CPPFLAGS_cortex-m7 += $(CPPFLAGS_cortex-m) \
      -mcpu=cortex-m7

# Single-arch libraries, to be phased out
override LEGACY_LIBS_cortex-m += \
      $(TOCK_USERLAND_BASE_DIR)/libc++/cortex-m/libstdc++.a\
      $(TOCK_USERLAND_BASE_DIR)/libc++/cortex-m/libsupc++.a\
      $(TOCK_USERLAND_BASE_DIR)/libc++/cortex-m/libgcc.a

override LEGACY_LIBS_cortex-m0 += $(LEGACY_LIBS_cortex-m) \
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v6-m/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v6-m/libm.a

override LEGACY_LIBS_cortex-m3 += $(LEGACY_LIBS_cortex-m) \
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v7-m/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v7-m/libm.a

override LEGACY_LIBS_cortex-m4 += $(LEGACY_LIBS_cortex-m) \
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v7-m/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v7-m/libm.a

override LEGACY_LIBS_cortex-m7 += $(LEGACY_LIBS_cortex-m) \
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v7-m/libc.a\
      $(TOCK_USERLAND_BASE_DIR)/newlib/cortex-m/v7-m/libm.a

# Cortex-M needs an additional OBJDUMP flag.
override OBJDUMP_FLAGS_cortex-m  += --disassembler-options=force-thumb
override OBJDUMP_FLAGS_cortex-m7 += $(OBJDUMP_FLAGS_cortex-m)
override OBJDUMP_FLAGS_cortex-m4 += $(OBJDUMP_FLAGS_cortex-m)
override OBJDUMP_FLAGS_cortex-m3 += $(OBJDUMP_FLAGS_cortex-m)
override OBJDUMP_FLAGS_cortex-m0 += $(OBJDUMP_FLAGS_cortex-m)


################################################################################
# Extra warning flags not enabled by Wall or Wextra.
#
# I read through the gcc manual and grabbed the ones that I thought might be
# interesting / useful. Then I grabbed that snippet below to find other things
# that were left out of the manual that may be worth adding. Below are all
# warnings and a short description supported by (arm-none-eabi)-gcc as of
# v6.2.1.
#
# http://stackoverflow.com/questions/11714827/
# List all supported warnings and their status:
#   gcc -Wall -Wextra -Q --help=warning
# Below are all warnings produced in an un-merged set of sorted lists
# broken into C/C++, C only, C++ only, other languages
#
# TODO(Pat) libnrfserialization noise with these, but I think they're useful
# and I want them back when I get a chance to clean that up.
#CPPFLAGS += -Wcast-qual #                # const char* -> char*
#CPPFLAGS += -Wswitch-default #           # switch w/out default (doesn't cover all cases) (maybe annoying?)
#CFLAGS += -Wstrict-prototypes #          # function defined w/out specifying argument types

override CPPFLAGS += -Wdate-time #                # warn if __TIME__, __DATE__, or __TIMESTAMP__ used
                                                  # ^on b/c flashing assumes same code => no flash, these enforce
override CPPFLAGS += -Wfloat-equal #              # floats used with '=' operator, likely imprecise
override CPPFLAGS += -Wformat-nonliteral #        # can't check format string (maybe disable if annoying)
override CPPFLAGS += -Wformat-security #          # using untrusted format strings (maybe disable)
override CPPFLAGS += -Wformat-y2k #               # use of strftime that assumes two digit years
override CPPFLAGS += -Winit-self #                # { int i = i }
override CPPFLAGS += -Wmissing-declarations #     # ^same? not sure how these differ
override CPPFLAGS += -Wmissing-field-initializers # if init'ing struct w/out field names, warn if not all used
override CPPFLAGS += -Wmissing-format-attribute # # something looks printf-like but isn't marked as such
override CPPFLAGS += -Wmissing-noreturn #         # __attribute__((noreturn)) like -> ! in Rust, should use it
override CPPFLAGS += -Wmultichar #                # use of 'foo' instead of "foo" (surpised not on by default?)
override CPPFLAGS += -Wpointer-arith #            # sizeof things not define'd (i.e. sizeof(void))
override CPPFLAGS += -Wredundant-decls #          # { int i; int i; } (a lint)
override CPPFLAGS += -Wshadow #                   # int foo(int a) { int a = 1; } inner a shadows outer a
override CPPFLAGS += -Wunused-macros #            # macro defined in this file not used
override CPPFLAGS += -Wunused-parameter #         # function parameter is unused aside from its declaration
override CPPFLAGS += -Wwrite-strings #            # { char* c = "foo"; c[0] = 'b' } <-- "foo" should be r/o

override CPPFLAGS_gcc += -Wlogical-op #           # "suspicious use of logical operators in expressions" (a lint)
override CPPFLAGS_gcc += -Wtrampolines #          # attempt to generate a trampoline on the NX stack

#CPPFLAGS += -Wabi -Wabi-tag              # inter-compiler abi issues
#CPPFLAGS += -Waggregate-return           # warn if things return struct's
#CPPFLAGS += -Wcast-align                 # { char *c; int *i = (int*) c}, 1 byte -> 4 byte align
#CPPFLAGS += -Wconversion                 # implicit conversion that may unexpectedly alter value
#                                         ^ A ton of these from syscalls I think, XXX look later
#CPPFLAGS += -Wdisabled-optimization      # gcc skipped an optimization for any of a thousand reasons
#CPPFLAGS += -Wdouble-promotion           # warn if float -> double implicitly XXX maybe?
#CPPFLAGS += -Wformat-signedness #        # { int i; printf("%d %u", i, i) } second bad (maybe annoying?)
#                                         ^ Too obnoxious when you want hex of an int
#CPPFLAGS += -Wfloat-conversion           # subset of -Wconversion
#CPPFLAGS += -Winline                     # something marked `inline` wasn't inlined
#CPPFLAGS += -Winvalid-pch                # bad precompiled header found in an include dir
#CPPFLAGS += -Wmissing-include-dirs -- XXX Didn't try, afriad could be annoying
#CPPFLAGS += -Woverlength-strings         # complier compat: strings > [509 C90, 4095 C99] chars
#CPPFLAGS += -Wpacked                     # struct with __attribute__((packed)) that does nothing
#CPPFLAGS += -Wpadded                     # padding added to a struct. Noisy for argument structs
#CPPFLAGS += -Wpedantic                   # strict ISO C/C++
#CPPFLAGS += -Wsign-conversion            # implicit integer sign conversions, part of -Wconversion
#CPPFLAGS += -Wstack-protector            # only if -fstack-protector, on by default, warn fn not protect
#CPPFLAGS += -Wsuggest-attribute=const    # does what it sounds like - removed due to noise
#CPPFLAGS += -Wsuggest-attribute=pure     # does what it sounds like - removed due to noise
#CPPFLAGS += -Wswitch-enum #              # switch of enum doesn't explicitly cover all cases
#                                         ^ annoying in practice, let default: do its job
#CPPFLAGS += -Wsystem-headers             # warnings from system headers
#CPPFLAGS += -Wtraditional                # stuff gcc allows that "traditional" C doesn't
#CPPFLAGS += -Wundef                      # undefined identifier is evaluated in an `#if' directive
#                                         ^ Lots of library #if SAMD || SMAR21 stuff
#                                           Should probably be ifdef, but too much noise
#CPPFLAGS += -Wunsafe-loop-optimizations  # compiler can't divine loop bounds XXX maybe interesting?
#CPPFLAGS += -Wvariadic-macros            # can't be used in ISO C
#CPPFLAGS += -Wvector-operation-performance # perf option not appropriate for these systems
#CPPFLAGS += -Wvla                  -- XXX Didn't try, but interested

# C-only warnings
override CFLAGS += -Wbad-function-cast #          # not obvious when this would trigger, could drop if annoying
override CFLAGS += -Wmissing-prototypes #         # global fn defined w/out prototype (should be static or in .h)
override CFLAGS += -Wnested-externs #             # mis/weird-use of extern keyword
override CFLAGS += -Wold-style-definition #       # this garbage: void bar (a) int a; { }

override CFLAGS_gcc += -Wjump-misses-init #       # goto or switch skips over a variable initialization

#CFLAGS += -Wunsuffixed-float-constants # # { float f=0.67; if(f==0.67) printf("y"); else printf("n"); } => n
#                                         ^ doesn't seem to work right? find_north does funny stuff

#CFLAGS += -Wtraditional-conversion #     # prototype causes a conversion different than w/o prototype (?)
#                                         ^ real noisy

# CXX-only warnings
override CXXFLAGS += -Wctor-dtor-privacy #        # unusable class b/c everything private and no friends
override CXXFLAGS += -Wdelete-non-virtual-dtor #  # catches undefined behavior
override CXXFLAGS += -Wold-style-cast #           # C-style cast in C++ code
override CXXFLAGS += -Woverloaded-virtual #       # subclass shadowing makes parent impl's unavailable
override CXXFLAGS += -Wsign-promo #               # gcc did what spec requires, but probably not what you want
override CXXFLAGS += -Wstrict-null-sentinel #     # seems like a not-very-C++ thing to do? very unsure
override CXXFLAGS += -Wsuggest-final-methods #    # does what it sounds like
override CXXFLAGS += -Wsuggest-final-types #      # does what it sounds like
override CXXFLAGS += -Wsuggest-override #         # overridden virtual func w/out override keyword
override CXXFLAGS += -Wuseless-cast #             # pretty much what ya think here
override CXXFLAGS += -Wzero-as-null-pointer-constant # use of 0 as NULL

# -Wc++-compat #                         # C/C++ compat issues
# -Wc++11-compat #                       # C11 compat issues
# -Wc++14-compat #                       # C14 compat issues
# -Wconditionally-supported #            # conditionally-supported (C++11 [intro.defs]) constructs (?)
# -Weffc++                               # violations of style guidelines from Meyers' Effective C++ books
# -Wmultiple-inheritance                 # used to enforce coding conventions, does what you'd think
# -Wnamespaces                           # used to enforce coding conventions, warn if namespace opened
# -Wnoexcept #                           # (?) I think warns if missing noexcept
# -Wnon-virtual-dtor #                   # something deeply c++, part of effc++
# -Wsynth                                # legacy flag, g++ != cfront
# -Wtemplates                            # used to enforce coding conventions, warn if new template
# -Wvirtual-inheritance                  # used to enforce coding conventions, does what you'd think

# Fortran-only warnings
# -Waliasing
# -Wampersand
# -Warray-temporaries
# -Wc-binding-type
# -Wcharacter-truncation
# -Wcompare-reals
# -Wconversion-extra
# -Wfunction-elimination
# -Wimplicit-interface
# -Wimplicit-procedure
# -Winteger-division
# -Wintrinsic-shadow
# -Wintrinsics-std
# -Wreal-q-constant
# -Wrealloc-lhs
# -Wrealloc-lhs-all
# -Wsurprising
# -Wtabs
# -Wtarget-lifetime
# -Wunused-dummy-argument
# -Wuse-without-only

# Objective-C(++)-only
# -Wassign-intercept
# -Wselector
# -Wstrict-selector-match
# -Wundeclared-selector

# END WARNINGS
################################################################################


# C/C++ Linter configuration
UNCRUSTIFY := $(TOCK_USERLAND_BASE_DIR)/tools/uncrustify/uncrustify.sh


# Dump configuration for verbose builds
ifneq ($(V),)
  $(info )
  $(info **************************************************)
  $(info TOCK USERLAND BUILD SYSTEM -- VERBOSE BUILD)
  $(info **************************************************)
  $(info Config:)
  $(info GIT: $(shell git describe --always 2>&1))
  $(info $(TOOLCHAIN_cortex-m4)$(CC_cortex-m4) --version: $(shell $(TOOLCHAIN_cortex-m4)$(CC_cortex-m4) --version))
ifneq ($(RISCV),)
  $(info $(TOOLCHAIN_rv32i)$(CC_rv32i) --version: $(shell $(TOOLCHAIN_rv32i)$(CC_rv32i) --version))
endif
  $(info LAYOUT=$(LAYOUT))
  $(info MAKEFLAGS=$(MAKEFLAGS))
  $(info PACKAGE_NAME=$(PACKAGE_NAME))
  $(info TOCK_ARCHS=$(TOCK_ARCHS))
  $(info TOCK_TARGETS=$(TOCK_TARGETS))
  $(info TOCK_USERLAND_BASE_DIR=$(TOCK_USERLAND_BASE_DIR))
  $(info TOOLCHAIN=$(TOOLCHAIN))
  $(info **************************************************)
  $(info )
endif

endif
