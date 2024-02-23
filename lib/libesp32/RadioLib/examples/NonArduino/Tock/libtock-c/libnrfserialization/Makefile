TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME=libnrfserialization
$(LIBNAME)_DIR=.

include NRFMakefile.mk

$(LIBNAME)_SRCS += $(SYSTEM_FILE) $(notdir $(APPLICATION_SRCS))


##############################################################################################
## Rules to create libnrfserialization

# Need libtock headers
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk


##############################################################################################
## Rules to create libnrfserialization headers

# Require all to build this too
all: headers.tar.gz

$($(LIBNAME)_BUILDDIR)/headers:
	$(TRACE_DIR)
	$(Q)mkdir -p $@

HDRS += $(patsubst %.c,$($(LIBNAME)_BUILDDIR)/headers/%.headers, $($(LIBNAME)_SRCS))
HDRS += $(addprefix $($(LIBNAME)_BUILDDIR)/headers/,$(APPLICATION_LIBS))

$($(LIBNAME)_BUILDDIR)/headers/%.headers: %.c | $($(LIBNAME)_BUILDDIR)/headers
	$(TRACE_CCM)
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) -MM $< > $@

headers.tar.gz: $(HDRS)
	mkdir -p headers
	cat build/headers/*.headers | awk '{$$1=$$1};1' | awk '{print $$1}' | sort | grep '\.h' | grep -v libtock | uniq | xargs -IFOO cp FOO headers/
	# Make tar file reproducible, as described at https://reproducible-builds.org/docs/archives/
	tar \
	  --sort=name \
	  --mtime="1970-01-01 00:01:00Z" \
	  --owner=0 --group=0 --numeric-owner \
	  --pax-option=exthdr.name=%d/PaxHeaders/%f,delete=atime,delete=ctime \
	  -czf headers.tar.gz headers

.PHONY: clean
clean::
	rm -Rf headers/
	rm -Rf headers.tar.gz
