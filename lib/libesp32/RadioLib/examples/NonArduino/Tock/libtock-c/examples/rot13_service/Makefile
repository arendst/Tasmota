# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

PACKAGE_NAME = org.tockos.examples.rot13

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
