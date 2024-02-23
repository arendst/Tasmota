# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# We only compile this for Cortex-M platforms because of compiler
# errors when building lua on risc-v.
TOCK_TARGETS := cortex-m0 cortex-m3 cortex-m4 cortex-m7

# Which files to compile.
C_SRCS := $(wildcard *.c)

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lua53

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/lua53/include

STACK_SIZE    = 3072

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
