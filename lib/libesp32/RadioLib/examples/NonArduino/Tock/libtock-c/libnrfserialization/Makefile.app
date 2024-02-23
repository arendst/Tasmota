LIBNRFSER_DIR := $(TOCK_USERLAND_BASE_DIR)/libnrfserialization

# So it doesn't think it's on the nRF and try to include nRF code.
override CFLAGS += -D__TOCK__
override CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION
override CFLAGS += -DSOFTDEVICE_s130

override CFLAGS += -I$(LIBNRFSER_DIR)/headers

# If environment variable V is non-empty, be verbose
ifneq ($(V),)
	Q=
	TRACE_TAR =
else
	Q=@
	TRACE_TAR = @echo " TAR       " $<
endif

$(C_SRCS):	$(LIBNRFSER_DIR)/headers
$(CXX_SRCS):	$(LIBNRFSER_DIR)/headers

$(LIBNRFSER_DIR)/headers: $(LIBNRFSER_DIR)/headers.tar.gz
	$(TRACE_TAR)
	$(Q)tar xf $< --directory $(LIBNRFSER_DIR)
	@touch $@

.PHONY: clean
clean::
	rm -rf $(LIBNRFSER_DIR)/headers
