# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

STACK_SIZE=512
KERNEL_HEAP_SIZE=512
APP_HEAP_SIZE=0

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
