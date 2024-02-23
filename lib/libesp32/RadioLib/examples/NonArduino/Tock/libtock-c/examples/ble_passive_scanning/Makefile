# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Which files to compile.
CXX_SRCS := $(wildcard *.cpp)

APP_HEAP_SIZE = 4096

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
