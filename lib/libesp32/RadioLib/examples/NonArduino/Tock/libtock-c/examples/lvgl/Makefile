# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lvgl

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/lvgl

# Which files to compile.
C_SRCS += $(wildcard *.c)

APP_HEAP_SIZE := 40000
STACK_SIZE := 4096

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Ensure the required submodule is present
lvgl_driver.c:	| ../../lvgl/lvgl/lvgl.h

../../lvgl/lvgl/lvgl.h:
	git submodule init ../../lvgl/lvgl
	git submodule update ../../lvgl/lvgl
