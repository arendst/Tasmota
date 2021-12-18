#
#Component Makefile
#

COMPONENT_SRCDIRS += ./
COMPONENT_PRIV_INCLUDEDIRS += ./

COMPONENT_ADD_LDFLAGS = -Wl,--whole-archive -l$(COMPONENT_NAME) -Wl,--no-whole-archive
