# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := simple-ble
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/*.c)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
