# Makefile for user application

# Specify this directory relative to the current application
TOCK_USERLAND_BASE_DIR = ../../..

include $(TOCK_USERLAND_BASE_DIR)/simple-ble/Makefile
# Which files to compile
C_SRCS := $(wildcard *.c)
# Include userland master makefile. Contains rules and flags for actually
# building the application
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/simple-ble
