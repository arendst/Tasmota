################################################################################
##
## libtock-c main build system Makefile.
##
## Individual applications use this Makefile to invoke the compiler and create
## Tock apps.
##
## This Makefile works by generating at runtime the rules necessary to build the
## libtock-c app for all desired architectures, then running each of those rules
## to create the compiled output. Each architecture-specific compiled binary is
## then combined into a single Tock TAB file.
##
################################################################################

# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule
all:

# Directory for built output.
BUILDDIR ?= build

# Build settings.
include $(TOCK_USERLAND_BASE_DIR)/Configuration.mk

# Helper functions.
include $(TOCK_USERLAND_BASE_DIR)/Helpers.mk

# Include the libtock makefile. Adds rules that will rebuild the core libtock
# library when needed.
include $(TOCK_USERLAND_BASE_DIR)/libtock/Makefile

# Connection to the Tock kernel. Apps need the ability to be loaded onto a
# board, and that method is board-specific. So for now, we have the TOCK_BOARD
# variable which selects one and we include the appropriate Makefile-app from
# within the Tock base directory.
TOCK_BOARD ?= hail

# Include the makefile that has the programming functions for each board.
include $(TOCK_USERLAND_BASE_DIR)/Program.mk



# Rules to incorporate external libraries
define EXTERN_LIB_RULES
EXTERN_LIB_NAME_$(notdir $(1)) := $(notdir $(1))

# If this library has any additional rules, add them
-include $(1)/Makefile.app

# If this library has an include directory, add it to search path
ifneq "$$(wildcard $(1)/include)" ""
  override CPPFLAGS += -I$(1)/include
endif

# Add arch-specific rules for each library
# Use the $(LIBNAME)_BUILDDIR as build directory, if set.
$$(notdir $(1))_BUILDDIR ?= $(1)/build
$$(foreach arch, $$(TOCK_ARCHS), $$(eval LIBS_$$(arch) += $$($(notdir $(1))_BUILDDIR)/$$(arch)/$(notdir $(1)).a))

endef

# To see the generated rules, run:
# $(info $(foreach lib, $(EXTERN_LIBS), $(call EXTERN_LIB_RULES,$(lib))))
$(foreach lib, $(EXTERN_LIBS), $(eval $(call EXTERN_LIB_RULES,$(lib))))


# Some sanity checks for variables before they are used

# Warn users about LDFLAGS currently being ignored. We currently use the WLFLAGS
# and WLFLAGS_$(arch) variables to pass linker options through the compiler, by
# encoding them as `-Wl,...` options.
ifdef LDFLAGS
  $(warning *******************************************************)
  $(warning LDFLAGS are currently ignored!!)
  $(warning )
  $(warning This is because we need to invoke the gcc frontend not the)
  $(warning ld frontend for the final link step, which means that we would)
  $(warning need to parse the LDFLAGS into things like -Wl,-<flag> for each)
  $(warning entry, but that proved a little fragile on first attempt so)
  $(warning it is not currently done. Sorry.)
  $(warning Please use WLFLAGS if you need to pass linker flags.)
  $(warning *******************************************************)
endif

# Warn users about improperly defined `HEAP_SIZE`.
ifdef HEAP_SIZE
    $(warning The variable HEAP_SIZE is set but will not be used.)
    $(warning Tock has two heaps, the application heap which is memory your)
    $(warning program uses and the kernel heap or grant regions, which is memory)
    $(warning dynamically allocated by drivers on behalf of your program.)
    $(warning )
    $(warning These regions are controlled by the APP_HEAP_SIZE and)
    $(warning KERNEL_HEAP_SIZE variables respectively.)
endif


# Rules to create object files for a specific architecture.
#
# - Argument $(1) is the architecture (e.g. cortex-m0) to compile for.
define BUILD_RULES_PER_ARCH

# BUILDDIR holds architecture dependent, but board-independent outputs
$$(BUILDDIR)/$(1):
	$$(TRACE_DIR)
	$$(Q)mkdir -p $$@

# First step doesn't actually compile, just generate header dependency information
# More info on our approach here: http://stackoverflow.com/questions/97338
$$(BUILDDIR)/$(1)/%.o: %.c | $$(BUILDDIR)/$(1)
	$$(TRACE_CC)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CC_$(1)) $$(CFLAGS) $$(CFLAGS_$(1)) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -MF"$$(@:.o=.d)" -MG -MM -MP -MT"$$(@:.o=.d)@" -MT"$$@" "$$<"
	$$(Q)$$(TOOLCHAIN_$(1))$$(CC_$(1)) $$(CFLAGS) $$(CFLAGS_$(1)) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -c -o $$@ $$<

$$(BUILDDIR)/$(1)/%.o: %.cc | $$(BUILDDIR)/$(1)
	$$(TRACE_CXX)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CXX) $$(CXXFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -MF"$$(@:.o=.d)" -MG -MM -MP -MT"$$(@:.o=.d)@" -MT"$$@" "$$<"
	$$(Q)$$(TOOLCHAIN_$(1))$$(CXX) $$(CXXFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -c -o $$@ $$<

$$(BUILDDIR)/$(1)/%.o: %.cpp | $$(BUILDDIR)/$(1)
	$$(TRACE_CXX)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CXX) $$(CXXFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -MF"$$(@:.o=.d)" -MG -MM -MP -MT"$$(@:.o=.d)@" -MT"$$@" "$$<"
	$$(Q)$$(TOOLCHAIN_$(1))$$(CXX) $$(CXXFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -c -o $$@ $$<

$$(BUILDDIR)/$(1)/%.o: %.cxx | $$(BUILDDIR)/$(1)
	$$(TRACE_CXX)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CXX) $$(CXXFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -MF"$$(@:.o=.d)" -MG -MM -MP -MT"$$(@:.o=.d)@" -MT"$$@" "$$<"
	$$(Q)$$(TOOLCHAIN_$(1))$$(CXX) $$(CXXFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -c -o $$@ $$<

OBJS_$(1) += $$(patsubst %.c,$$(BUILDDIR)/$(1)/%.o,$$(C_SRCS))
OBJS_$(1) += $$(patsubst %.cc,$$(BUILDDIR)/$(1)/%.o,$$(filter %.cc, $$(CXX_SRCS)))
OBJS_$(1) += $$(patsubst %.cpp,$$(BUILDDIR)/$(1)/%.o,$$(filter %.cpp, $$(CXX_SRCS)))
OBJS_$(1) += $$(patsubst %.cxx,$$(BUILDDIR)/$(1)/%.o,$$(filter %.cxx, $$(CXX_SRCS)))

endef


# Rules to generate an app for a given architecture and target. These actually
# create the ELF which can be linked for a specific address as needed.
#
# - Argument $(1) is the architecture (e.g. cortex-m0) to build for.
# - Argument $(2) is the output name to use for the .elf and other files.
# - Argument $(3) is the flash address to use for linking.
# - Argument $(4) is the RAM address to use for linking.
#
# Note: all variables, other than $(1), used within this block must be double
# dollar-signed so that their values will be evaluated when run, not when
# generated
define BUILD_RULES

$$(BUILDDIR)/$(1)/$(2).custom.ld: $$(LAYOUT) | $$(BUILDDIR)/$(1)
	@# Start with a copy of the template / generic ld script
	$$(Q)cp $$< $$@
	@# #616 #635: sed is not cross-platform
	@# https://stackoverflow.com/a/22247781/358675 <-- Use perl in place of sed
	$$(Q)\
	  perl -pi -e "s/(FLASH.*ORIGIN[ =]*)([x0-9]*)(,.*LENGTH)/\$$$${1}$(3)\$$$$3/" $$@ &&\
	  perl -pi -e "s/(SRAM.*ORIGIN[ =]*)([x0-9]*)(,.*LENGTH)/\$$$${1}$(4)\$$$$3/" $$@

# Collect all desired built output.
$$(BUILDDIR)/$(1)/$(2).elf: $$(OBJS_$(1)) $$(LIBS_$(1)) $$(LEGACY_LIBS_$(1)) $$(BUILDDIR)/$(1)/$(2).custom.ld | $$(BUILDDIR)/$(1)
	$$(TRACE_LD)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CC_$(1)) $$(CFLAGS) $$(CFLAGS_$(1)) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) $$(WLFLAGS) $$(WLFLAGS_$(1))\
	    -Xlinker --defsym=STACK_SIZE=$$(STACK_SIZE)\
	    -Xlinker --defsym=APP_HEAP_SIZE=$$(APP_HEAP_SIZE)\
	    -Xlinker --defsym=KERNEL_HEAP_SIZE=$$(KERNEL_HEAP_SIZE)\
	    -T $$(BUILDDIR)/$(1)/$(2).custom.ld\
	    -nostdlib\
	    -Wl,--start-group $$(OBJS_$(1)) $$(LIBS_$(1)) $$(LEGACY_LIBS_$(1)) $$(LINK_LIBS_$(1)) -Wl,--end-group\
	    -Wl,-Map=$$(BUILDDIR)/$(1)/$(2).Map\
	    -o $$@

# NOTE: This rule creates an lst file for the elf as flashed on the board
#       (i.e. at address 0x80000000). This is not likely what you want.
$$(BUILDDIR)/$(1)/$(2).lst: $$(BUILDDIR)/$(1)/$(2).elf
	$$(TRACE_LST)
	$$(Q)$$(TOOLCHAIN_$(1))$$(OBJDUMP) $$(OBJDUMP_FLAGS) $$(OBJDUMP_FLAGS_$(1)) $$< > $$@
	@echo $$$$(tput bold)Listings generated at $$@$$$$(tput sgr0)

# checks compiled ELF files to ensure that all libraries and applications were
# built with the correct flags in order to work on a Tock board
.PHONY: validate_gcc_flags
validate_gcc_flags:: $$(BUILDDIR)/$(1)/$(2).elf
ifndef TOCK_NO_CHECK_SWITCHES
	$$(Q)$$(TOOLCHAIN_$(1))$$(READELF) -p .GCC.command.line $$< 2>&1 | grep -q "does not exist" && { echo "Error: Missing section .GCC.command.line"; echo ""; echo "Tock requires that applications are built with"; echo "  -frecord-gcc-switches"; echo "to validate that all required flags were used"; echo ""; echo "You can skip this check by defining the make variable TOCK_NO_CHECK_SWITCHES"; exit 1; } || exit 0
	$$(Q)$$(TOOLCHAIN_$(1))$$(READELF) -p .GCC.command.line $$< | grep -q -- -msingle-pic-base && $$(READELF) -p .GCC.command.line $$< | grep -q -- -mpic-register=r9 && $$(READELF) -p .GCC.command.line $$< | grep -q -- -mno-pic-data-is-text-relative || { echo "Error: Missing required build flags."; echo ""; echo "Tock requires applications are built with"; echo "  -msingle-pic-base"; echo "  -mpic-register=r9"; echo "  -mno-pic-data-is-text-relative"; echo "But one or more of these flags are missing"; echo ""; echo "To see the flags your application was built with, run"; echo "$$(READELF) -p .GCC.command.line $$<"; echo ""; exit 1; }
endif

# rules to print the size of the built binaries
.PHONY: size-$(1)-$(2)
size-$(1)-$(2):	$$(BUILDDIR)/$(1)/$(2).elf
	@echo Application size report for target $(2):
	$$(Q)$$(TOOLCHAIN_$(1))$$(SIZE) $$^

size::	size-$(1)-$(2)


############################################################################################
# DEBUGGING STUFF
#
# The approach here is that we're going create a new elf file that is compiled
# at the actual flash and ram offset of the loaded program
#
# We want to build a rule that fails if these needed env variables aren't set
# only when actually trying to use them to build the lst file. We also want to
# force this to rerun every time it's invoked so that it picks up new env
# variable settings


# Step 0: Force this to be built every time
.PHONY: _FORCE_USERLAND_DEBUG_LD

# Step 1: Create a new linker script. Note this depends on original (non-shifted) elf
# (supposedly this could be one-lined, but I couldn't make that work, so here goes)
ifdef RAM_START
  ifdef FLASH_INIT
    _USERLAND_DEBUG_ALL_NEEDED_VARS := 1
  endif
endif

$$(BUILDDIR)/$(1)/$(2).userland_debug.ld: $$(TOCK_USERLAND_BASE_DIR)/userland_generic.ld $$(BUILDDIR)/$(1)/$(2).elf _FORCE_USERLAND_DEBUG_LD | $$(BUILDDIR)/$(1)
ifndef _USERLAND_DEBUG_ALL_NEEDED_VARS
	@echo "ERROR: Required variables RAM_START and FLASH_INIT are not set."
	@echo "       These are needed to compute the offset your program was loaded at."
	@echo "       See the kernel panic message for these values."
	@exit 1
else
	@# Start with a copy of the template / generic ld script
	$$(Q)cp $$< $$@
	@# And with apologies to future readers, this is easier as one shell command/script so
	@# we can set intervening variables, away we go
	@#
	@# Get the offset between the init function and the start of text (0x80000000).
	@# We then use that offset to calculate where the start of text was on the actual MCU.
	@# Create a new LD file at the correct flash and ram locations.
	@#
	@# #616 #635: sed is not cross-platform
	@# https://stackoverflow.com/a/22247781/358675 <-- Use perl in place of sed
	$$(Q)set -e ;\
	  ORIGINAL_ENTRY=`$$(TOOLCHAIN_$(1))$$(READELF) -h $$(BUILDDIR)/$(1)/$(2).elf | grep Entry | awk '{print $$$$4}'` ;\
	  INIT_OFFSET=$$$$(($$$$ORIGINAL_ENTRY - 0x80000000)) ;\
	  FLASH_START=$$$$(($$$$FLASH_INIT-$$$$INIT_OFFSET)) ;\
	  perl -pi -e "s/(FLASH.*ORIGIN[ =]*)([x0-9]*)(,.*LENGTH)/\$$$${1}$$$$FLASH_START\$$$$3/" $$@ ;\
	  perl -pi -e "s/(SRAM.*ORIGIN[ =]*)([x0-9]*)(,.*LENGTH)/\$$$${1}$$$$RAM_START\$$$$3/" $$@
endif

# Step 2: Create a new ELF with the layout that matches what's loaded
$$(BUILDDIR)/$(1)/$(2).userland_debug.elf: $$(OBJS_$(1)) $$(LIBS_$(1)) $$(LEGACY_LIBS_$(1)) $$(BUILDDIR)/$(1)/$(2).userland_debug.ld | $$(BUILDDIR)/$(1)
	$$(TRACE_LD)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CC_$(1)) $$(CFLAGS) $$(CFLAGS_$(1)) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) $$(WLFLAGS) $$(WLFLAGS_$(1))\
	    -Xlinker --defsym=STACK_SIZE=$$(STACK_SIZE)\
	    -Xlinker --defsym=APP_HEAP_SIZE=$$(APP_HEAP_SIZE)\
	    -Xlinker --defsym=KERNEL_HEAP_SIZE=$$(KERNEL_HEAP_SIZE)\
	    -T $$(BUILDDIR)/$(1)/$(2).userland_debug.ld\
	    -nostdlib\
	    -Wl,--start-group $$(OBJS_$(1)) $$(LIBS_$(1)) $$(LEGACY_LIBS_$(1)) $$(LINK_LIBS_$(1)) -Wl,--end-group\
	    -Wl,-Map=$$(BUILDDIR)/$(1)/$(2).Map\
	    -o $$@

# Step 3: Now we can finally generate an LST
$$(BUILDDIR)/$(1)/$(2).userland_debug.lst: $$(BUILDDIR)/$(1)/$(2).userland_debug.elf
	$$(TRACE_LST)
	$$(Q)$$(TOOLCHAIN_$(1))$$(OBJDUMP) $$(OBJDUMP_FLAGS) $$(OBJDUMP_FLAGS_$(1)) $$< > $$@
	@echo $$$$(tput bold)Listings generated at $$@$$$$(tput sgr0)

# END DEBUGGING STUFF
############################################################################################
endef


# Rules that apply to an entire architecture family (e.g. cortex-m).
#
# - Argument $(1) is the architecture family (e.g. cortex-m).
# - Argument $(2) is the list of architectures in that family.
# - Argument $(3) is the list of output names for the .elf of each arch.
#
# Note: all variables, other than $(1), used within this block must be double
# dollar-signed so that their values will be evaluated when run, not when
# generated.
define ARCH_FAMILY_RULES

$(1)_DIRECTORY_NAMES := $$(addsuffix /,$(2))
$(1)_DIRECTORY_FILES := $$(join $$($(1)_DIRECTORY_NAMES),$(3))
$(1)_DIRECTORY_FILES_EXT := $$(addsuffix .elf,$$($(1)_DIRECTORY_FILES))
$(1)_ELF_FILES := $$(addprefix $$(BUILDDIR)/,$$($(1)_DIRECTORY_FILES_EXT))

# Rule to print the size of the built binaries from an architecture family.
.PHONY: size-$(1)
size-$(1): $$($(1)_ELF_FILES)
	@echo Application size report for arch family $(1):
	$$(Q)$$(TOOLCHAIN_$(1))$$(SIZE) -t $$^

endef

# Functions to parse the `TOCK_TARGETS` array. Entries 3 and 4 default to the
# PIC addresses if they are not specified.
ARCH_FN = $(firstword $(subst |, ,$1))
OUTPUT_NAME_FN = $(if $(word 2,$(subst |, ,$1)),$(word 2,$(subst |, ,$1)),$(firstword $(subst |, ,$1)))
FLASH_ADDRESS_FN = $(if $(word 3,$(subst |, ,$1)),$(word 3,$(subst |, ,$1)),0x80000000)
RAM_ADDRESS_FN = $(if $(word 4,$(subst |, ,$1)),$(word 4,$(subst |, ,$1)),0x00000000)

# To see the generated rules, run:
#$(info $(foreach platform, $(TOCK_ARCHS), $(eval $(call BUILD_RULES_PER_ARCH,$(platform)))))
#$(info $(foreach platform, $(TOCK_TARGETS), $(call BUILD_RULES,$(call ARCH_FN,$(platform)),$(call OUTPUT_NAME_FN,$(platform)),$(call FLASH_ADDRESS_FN,$(platform)),$(call RAM_ADDRESS_FN,$(platform)))))
# Actually generate the rules for each architecture
$(foreach platform, $(TOCK_ARCHS), $(eval $(call BUILD_RULES_PER_ARCH,$(platform))))
$(foreach platform, $(TOCK_TARGETS), $(eval $(call BUILD_RULES,$(call ARCH_FN,$(platform)),$(call OUTPUT_NAME_FN,$(platform)),$(call FLASH_ADDRESS_FN,$(platform)),$(call RAM_ADDRESS_FN,$(platform)))))
$(foreach family, $(TOCK_ARCH_FAMILIES), $(eval $(call ARCH_FAMILY_RULES,$(family),$(foreach target, $(filter $(family)%,$(TOCK_TARGETS)), $(call ARCH_FN, $(target))),$(foreach target, $(filter $(family)%,$(TOCK_TARGETS)), $(call OUTPUT_NAME_FN, $(target))))))




# TAB file generation. Used for Tockloader
$(BUILDDIR)/$(PACKAGE_NAME).tab: $(foreach platform, $(TOCK_TARGETS), $(BUILDDIR)/$(call ARCH_FN,$(platform))/$(call OUTPUT_NAME_FN,$(platform)).elf)
	$(TRACE_E2T)
	$(Q)$(ELF2TAB) $(ELF2TAB_ARGS) -o $@ $^



# Rules for building apps
SIZE_RULES = $(addprefix size-,$(TOCK_ARCH_FAMILIES))
.PHONY:	all
all:	$(BUILDDIR)/$(PACKAGE_NAME).tab $(SIZE_RULES)

# The size target accumulates dependencies in the platform build rule creation
.PHONY: size

# Generate helpful output for debugging userland applications.
.PHONY: debug
debug:	$(foreach platform, $(TOCK_TARGETS), $(BUILDDIR)/$(call ARCH_FN,$(platform))/$(call OUTPUT_NAME_FN,$(platform)).userland_debug.lst)

# Generate a .lst file for each architecture using the RAM and flash addresses
# specified in the linker file. This will create a valid assembly file, but the
# addresses of the instructions will be wrong unless the application was
# compiled for specific addresses. Notably, on cortex-m platforms, which use
# position-independent code, the addresses will be wrong, and you should use
# `make debug` instead. For architectures without PIC support (like RISC-V),
# `make lst` will work since the linker files uses the correct addresses.
.PHONY: lst
lst:	$(foreach platform, $(TOCK_TARGETS), $(BUILDDIR)/$(call ARCH_FN,$(platform))/$(call OUTPUT_NAME_FN,$(platform)).lst)

.PHONY:
clean::
	rm -Rf $(BUILDDIR)


# Rules for running the C linter
FORMATTED_FILES := $(patsubst %.c,$(BUILDDIR)/format/%.uncrustify,$(C_SRCS))
FORMATTED_FILES += $(patsubst %.cc,$(BUILDDIR)/format/%.uncrustify,$(filter %.cc, $(CXX_SRCS)))
FORMATTED_FILES += $(patsubst %.cpp,$(BUILDDIR)/format/%.uncrustify,$(filter %.cpp, $(CXX_SRCS)))
FORMATTED_FILES += $(patsubst %.cxx,$(BUILDDIR)/format/%.uncrustify,$(filter %.cxx, $(CXX_SRCS)))

$(BUILDDIR)/format:
	@mkdir -p $@

.PHONY: fmt format
fmt format:: $(FORMATTED_FILES)

$(BUILDDIR)/format/%.uncrustify: %.c | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
$(BUILDDIR)/format/%.uncrustify: %.cc | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
$(BUILDDIR)/format/%.uncrustify: %.cpp | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
$(BUILDDIR)/format/%.uncrustify: %.cxx | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)


# Rules to help validate build configuration
fmt format::
	$(Q)$(TOCK_USERLAND_BASE_DIR)/tools/check_override.sh


#########################################################################################
# Include dependency rules for picking up header changes (by convention at bottom of makefile)
OBJS_NO_ARCHIVES += $(filter %.o,$(foreach platform, $(TOCK_ARCHS), $(OBJS_$(platform))))
-include $(OBJS_NO_ARCHIVES:.o=.d)
