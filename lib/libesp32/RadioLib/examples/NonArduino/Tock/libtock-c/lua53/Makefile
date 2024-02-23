# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lua53
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/lua/*.c)

override CFLAGS += -DLUA_32BITS -D"luai_makeseed()"=0

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
