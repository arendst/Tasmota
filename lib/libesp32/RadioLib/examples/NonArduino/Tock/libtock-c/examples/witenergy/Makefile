# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

override CFLAGS += -DBLE_STACK_SUPPORT_REQD=1

# Which files to compile.
C_SRCS := $(wildcard *.c)

APP_HEAP_SIZE := 2048
STACK_SIZE := 4096

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/libnrfserialization

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
