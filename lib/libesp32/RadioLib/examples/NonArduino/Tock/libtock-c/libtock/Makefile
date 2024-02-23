# Libtock makefile. Can build Libtock standalone. Also included by application
# makefiles to ensure their libtock dependency is built

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libtock
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/internal/*.c) $(wildcard $($(LIBNAME)_DIR)/*.c) $(wildcard $($(LIBNAME)_DIR)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
