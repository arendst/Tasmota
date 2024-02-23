################################################################################
##
## Makefile for loading applications onto a Tockloader compatible board.
##
################################################################################

$(call check_defined, BUILDDIR)
$(call check_defined, PACKAGE_NAME)

TOCKLOADER ?= tockloader
OPENOCD ?= openocd

# Upload programs over UART with tockloader.
ifdef PORT
  TOCKLOADER_GENERAL_FLAGS += --port $(PORT)
endif

# Setup specific commands for each board.
ifeq ("$(TOCK_BOARD)","hail")
PROGRAM = $(TOCKLOADER) $(TOCKLOADER_GENERAL_FLAGS) install $<
FLASH = $(TOCKLOADER) $(TOCKLOADER_GENERAL_FLAGS) install --jlink $<

else ifeq ("$(TOCK_BOARD)","imix")
# Change program region offset
TOCKLOADER_INSTALL_FLAGS += --app-address 0x40000
PROGRAM = $(TOCKLOADER) $(TOCKLOADER_GENERAL_FLAGS) install $(TOCKLOADER_INSTALL_FLAGS) $<
FLASH = $(TOCKLOADER) $(TOCKLOADER_GENERAL_FLAGS) install $(TOCKLOADER_INSTALL_FLAGS) --jlink $<

else ifeq ("$(TOCK_BOARD)","ek-tm4c1294xl")
FLASH = $(OPENOCD) -c "source [find board/ek-tm4c1294xl.cfg]; init; reset halt; flash write_image erase $< 0x00020000 bin; reset; shutdown"

else ifeq ("$(TOCK_BOARD)","nrf51dk")
FLASH = $(TOCKLOADER) install --jlink --board nrf51dk $<

else ifeq ("$(TOCK_BOARD)","nrf52dk")
FLASH = $(TOCKLOADER) install --jlink --board nrf52dk $<

endif

PROGRAM ?= @(echo "Cannot program over serial $<"; exit 1)
FLASH ?= @(echo "Cannot flash $<"; exit 1)

.PHONY: program
program: $(BUILDDIR)/$(PACKAGE_NAME).tab
	$(PROGRAM)

# Upload programs over JTAG
.PHONY: flash
flash: $(BUILDDIR)/$(PACKAGE_NAME).tab
	$(FLASH)
