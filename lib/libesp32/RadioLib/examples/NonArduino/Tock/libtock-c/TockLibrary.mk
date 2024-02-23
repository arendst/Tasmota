################################################################################
##
## libtock-c library shared makefile. Included by library makefiles to build
##  libraries for use with libtock-c apps.
##
################################################################################

# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule.
all:

# Build settings.
include $(TOCK_USERLAND_BASE_DIR)/Configuration.mk

# Helper functions.
include $(TOCK_USERLAND_BASE_DIR)/Helpers.mk

$(call check_defined, LIBNAME)
$(call check_defined, $(LIBNAME)_DIR)
$(call check_defined, $(LIBNAME)_SRCS)

ifeq ($(strip $($(LIBNAME)_SRCS)),)
  $(error Library "$(LIBNAME)" has no SRCS?)
endif

# directory for built output
$(LIBNAME)_BUILDDIR ?= $($(LIBNAME)_DIR)/build

# Handle complex paths.
#
# Okay, so this merits some explanation:
#
# Our build system aspires to put everything in build/ directories, this means
# that we have to match the path of source files (foo.c) to output directories
# (build/<arch>/foo.o). That's easy enough if all the source files are in the
# same directory, but restricts applications and libraries to a flat file
# structure.
#
# The current solution we employ is built on make's VPATH variable, which is a
# list of directories to search for dependencies, e.g.
#
#    VPATH = foo/ ../bar/
#    somerule: dependency.c
#
# Will find any of ./dependency.c, foo/dependency.c, or ../bar/dependency.c
# We leverage this by flattening the list of SRCS to remove all path
# information and adding all the paths from the SRCS to the VPATH, this means
# we can write rules as-if all the SRCS were in a flat directory.
#
# The obvious pitfall here is what happens when multiple directories hold a
# source file of the same name. However, both libnrf and mbed are set up to
# use VPATH without running into that problem, which gives some pretty serious
# hope that it won't be an issue in practice. The day is actually is a problem,
# we can revisit this, but the only solution I can think of presently is
# another layer of macros that generates the build rules for each path in SRCS,
# which is a pretty hairy sounding proposition

$(LIBNAME)_SRCS_FLAT := $(notdir $($(LIBNAME)_SRCS))
$(LIBNAME)_SRCS_DIRS := $(sort $(dir $($(LIBNAME)_SRCS))) # sort removes duplicates

# Only use vpath for certain types of files
# But must be a global list
VPATH_DIRS += $($(LIBNAME)_SRCS_DIRS)
vpath %.s $(VPATH_DIRS)
vpath %.c $(VPATH_DIRS)
vpath %.cc $(VPATH_DIRS)
vpath %.cpp $(VPATH_DIRS)
vpath %.cxx $(VPATH_DIRS)

# Now, VPATH allows _make_ to find all the sources, but gcc needs to be told
# how to find all of the headers. We do this by `-I`'ing any folder that had a
# LIB_SRC and has any .h files in it. We also check the common convention of
# headers in an include/ folder (both in and adjacent to src/) while we're at it
define LIB_HEADER_INCLUDES
ifneq ($$(wildcard $(1)/*.h),"")
  override CPPFLAGS += -I$(1)
endif
ifneq ($$(wildcard $(1)/include/*.h),"")
  override CPPFLAGS += -I$(1)/include
endif
ifneq ($$(wildcard $(1)/../include/*.h),"")
  override CPPFLAGS += -I$(1)/../include
endif
endef
# uncomment to print generated rules
# $(info $(foreach hdrdir,$($(LIBNAME)_SRCS_DIRS),$(call LIB_HEADER_INCLUDES,$(hdrdir))))
# actually generate the rules
$(foreach hdrdir,$($(LIBNAME)_SRCS_DIRS),$(eval $(call LIB_HEADER_INCLUDES,$(hdrdir))))

# Rules to generate libraries for a given Architecture
# These will be used to create the different architecture versions of LibNRFSerialization
# Argument $(1) is the Architecture (e.g. cortex-m0) to build for
define LIB_RULES

$$($(LIBNAME)_BUILDDIR)/$(1):
	$$(TRACE_DIR)
	$$(Q)mkdir -p $$@

$$($(LIBNAME)_BUILDDIR)/$(1)/%.o: %.c | $$($(LIBNAME)_BUILDDIR)/$(1)
	$$(TRACE_CC)
	$$(Q)$$(TOOLCHAIN_$(1))$$(CC_$(1)) $$(CFLAGS) $$(CFLAGS_$(1)) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -MF"$$(@:.o=.d)" -MG -MM -MP -MT"$$(@:.o=.d)@" -MT"$$@" "$$<"
	$$(Q)$$(TOOLCHAIN_$(1))$$(CC_$(1)) $$(CFLAGS) $$(CFLAGS_$(1)) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -c -o $$@ $$<

$$($(LIBNAME)_BUILDDIR)/$(1)/%.o: %.S | $$($(LIBNAME)_BUILDDIR)/$(1)
	$$(TRACE_AS)
	$$(Q)$$(TOOLCHAIN_$(1))$$(AS) $$(ASFLAGS) $$(CPPFLAGS) $$(CPPFLAGS_$(1)) -c -o $$@ $$<

$(LIBNAME)_OBJS_$(1) += $$(patsubst %.s,$$($(LIBNAME)_BUILDDIR)/$(1)/%.o,$$(filter %.s, $$($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_OBJS_$(1) += $$(patsubst %.c,$$($(LIBNAME)_BUILDDIR)/$(1)/%.o,$$(filter %.c, $$($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_OBJS_$(1) += $$(patsubst %.cc,$$($(LIBNAME)_BUILDDIR)/$(1)/%.o,$$(filter %.cc, $$($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_OBJS_$(1) += $$(patsubst %.cpp,$$($(LIBNAME)_BUILDDIR)/$(1)/%.o,$$(filter %.cpp, $$($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_OBJS_$(1) += $$(patsubst %.cxx,$$($(LIBNAME)_BUILDDIR)/$(1)/%.o,$$(filter %.cxx, $$($(LIBNAME)_SRCS_FLAT)))

# Dependency rules for picking up header changes
-include $$($(LIBNAME)_OBJS_$(1):.o=.d)

# Useful debugging
# $$(info -----------------------------------------------------)
# $$(info $(LIBNAME) $(1))
# $$(info      $(LIBNAME)_SRCS: $$($(LIBNAME)_SRCS))
# $$(info $(LIBNAME)_SRCS_FLAT: $$($(LIBNAME)_SRCS_FLAT))
# $$(info                VPATH: $$(VPATH))
# $$(info $(LIBNAME)_OBJS_$(1): $$($(LIBNAME)_OBJS_$(1)))
# $$(info =====================================================)

$$($(LIBNAME)_BUILDDIR)/$(1)/$(LIBNAME).a: $$($(LIBNAME)_OBJS_$(1)) | $$($(LIBNAME)_BUILDDIR)/$(1)
	$$(TRACE_AR)
	$$(Q)$$(TOOLCHAIN_$(1))$$(AR) rc $$@ $$^
	$$(Q)$$(TOOLCHAIN_$(1))$$(RANLIB) $$@

# If we're building this library as part of a bigger build, add ourselves to
# the list of libraries
#
# Ahh.. make. By default, the RHS of variables aren't expanded at all until the
# variable is _used_ ("lazy set", "="), this means that LIBNAME will have
# changed by the time the variable is evaluated. We want immediate set (":="),
# but we'd also like to append ("+=") to grow the list. Append chooses between
# lazy or immediate set based on how the variable was previously set (yes,
# that's right, the RHS evaluation depends on the LHS type - make was ahead of
# it's time! :D), and defaults to lazy set if the variable is undefined at the
# first append. So, we force it to immediate set. Lovely.
ifndef LIBS_$(1)
  LIBS_$(1) :=
endif
LIBS_$(1) += $$($(LIBNAME)_BUILDDIR)/$(1)/$(LIBNAME).a

endef

# uncomment to print generated rules
# $(info $(foreach platform,$(TOCK_ARCHS), $(call LIB_RULES,$(call ARCH_FN,$(platform)))))
# actually generate the rules for each architecture
$(foreach arch,$(TOCK_ARCHS),$(eval $(call LIB_RULES,$(arch))))

# add each architecture as a target
.PHONY: all
all: $(foreach arch, $(TOCK_ARCHS),$($(LIBNAME)_BUILDDIR)/$(arch)/$(LIBNAME).a)


# Force LIBNAME to be expanded now
define CLEAN_RULE
.PHONY: clean
clean::
	rm -Rf $(1)
endef
$(eval $(call CLEAN_RULE,$($(LIBNAME)_BUILDDIR)))


# Rules for running the C linter
$(LIBNAME)_FORMATTED_FILES := $(patsubst %.c,$($(LIBNAME)_BUILDDIR)/format/%.uncrustify,$(filter %.c, $($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_FORMATTED_FILES += $(patsubst %.cc,$($(LIBNAME)_BUILDDIR)/format/%.uncrustify,$(filter %.cc, $($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_FORMATTED_FILES += $(patsubst %.cpp,$($(LIBNAME)_BUILDDIR)/format/%.uncrustify,$(filter %.cpp, $($(LIBNAME)_SRCS_FLAT)))
$(LIBNAME)_FORMATTED_FILES += $(patsubst %.cxx,$($(LIBNAME)_BUILDDIR)/format/%.uncrustify,$(filter %.cxx, $($(LIBNAME)_SRCS_FLAT)))

$($(LIBNAME)_BUILDDIR)/format:
	@mkdir -p $@

.PHONY: fmt format
fmt format:: $($(LIBNAME)_FORMATTED_FILES)

$($(LIBNAME)_BUILDDIR)/format/%.uncrustify: %.c | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
$($(LIBNAME)_BUILDDIR)/format/%.uncrustify: %.cc | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
$($(LIBNAME)_BUILDDIR)/format/%.uncrustify: %.cpp | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
$($(LIBNAME)_BUILDDIR)/format/%.uncrustify: %.cxx | _format_check_unstaged
	$(Q)$(UNCRUSTIFY) -f $< -o $@
	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
