#############################################################################
#
# Makefile for librf24
#
# License: GPL (General Public License)
# Author:  Charles-Henri Hallard 
# Date:    2013/03/13 
#
# Description:
# ------------
# use make all and make install to install the library 
#

CONFIG_FILE=Makefile.inc
REMOTE_ERROR="[ERROR] Remote machine not configured. Run configure with respective arguments."

include $(CONFIG_FILE)

# Objects to compile
OBJECTS=RF24.o
ifeq ($(DRIVER), MRAA)
OBJECTS+=spi.o gpio.o compatibility.o
else ifeq ($(DRIVER), RPi)
OBJECTS+=spi.o bcm2835.o interrupt.o
else ifeq ($(DRIVER), SPIDEV)
OBJECTS+=spi.o gpio.o compatibility.o interrupt.o
else ifeq ($(DRIVER), wiringPi)
OBJECTS+=spi.o
endif

# make all
# reinstall the library after each recompilation
all: $(LIBNAME)

# Make the library
$(LIBNAME): $(OBJECTS)
	@echo "[Linking]"
	$(CC) $(SHARED_LINKER_FLAGS) $(CFLAGS) -o $(LIBNAME) $^ $(SHARED_LINKER_LIBS)

# Library parts
RF24.o: RF24.cpp	
	$(CXX) -fPIC $(CFLAGS) -c $^

bcm2835.o: $(DRIVER_DIR)/bcm2835.c
	$(CC) -fPIC $(CFLAGS) -c $^

spi.o: $(DRIVER_DIR)/spi.cpp
	$(CXX) -fPIC $(CFLAGS) -c $^

compatibility.o: $(DRIVER_DIR)/compatibility.c
	$(CC) -fPIC  $(CFLAGS) -c $(DRIVER_DIR)/compatibility.c

gpio.o: $(DRIVER_DIR)/gpio.cpp
	$(CXX) -fPIC $(CFLAGS) -c $(DRIVER_DIR)/gpio.cpp

interrupt.o: $(DRIVER_DIR)/interrupt.c
	$(CXX) -fPIC $(CFLAGS) -c $(DRIVER_DIR)/interrupt.c
	
# clear configuration files
cleanconfig:
	@echo "[Cleaning configuration]"
	rm -rf $(CONFIG_FILE) utility/includes.h

# clear build files
clean:
	@echo "[Cleaning]"
	rm -rf *.o $(LIBNAME)

$(CONFIG_FILE):
	@echo "[Running configure]"
	@./configure --no-clean

install: all install-libs install-headers
upload: all upload-libs upload-headers

# Install the library to LIBPATH
install-libs:
	@echo "[Installing Libs to $(LIB_DIR)]"
	@if ( test ! -d $(LIB_DIR) ); then mkdir -p $(LIB_DIR); fi
	@install -m 0755 $(LIBNAME) $(LIB_DIR)
	@orig=$(LIBNAME) && \
	for sl in $(LIBSYMLINKS); do \
		ln -sf $(LIB_DIR)/$${orig} $(LIB_DIR)/$${sl}; \
		orig=$${sl}; \
	done && \
	if [ "$(LIBDEPRECATE)" ]; then ln -sf $(LIB_DIR)/$${orig} $(LIB_DIR)/$(LIBDEPRECATE); fi
ifneq ($(LDCONFIG),)
	@$(LDCONFIG)
endif

upload-libs:
	@echo "[Uploading Libs to $(REMOTE):$(REMOTE_LIB_DIR)]"
ifeq ($(REMOTE),)
	@echo "$(REMOTE_ERROR)"
	@exit 1
endif
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "mkdir -p $(REMOTE_LIB_DIR)"
	@scp -q -P $(REMOTE_PORT) $(LIBNAME) $(REMOTE):/tmp
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo install -m 0755 /tmp/$(LIBNAME) $(REMOTE_LIB_DIR) &&" \
		" orig=$(LIBNAME) && for sl in $(LIBSYMLINKS); do sudo ln -sf $(REMOTE_LIB_DIR)/\$${orig} $(REMOTE_LIB_DIR)/\$${sl}; orig=\$${sl}; done &&" \
		" if [ "$(LIBDEPRECATE)" ]; then sudo ln -sf $(REMOTE_LIB_DIR)/\$${orig} $(REMOTE_LIB_DIR)/$(LIBDEPRECATE); fi &&" \
		" rm /tmp/$(LIBNAME)"
ifneq ($(REMOTE_LDCONFIG),)
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo $(REMOTE_LDCONFIG)"
endif

install-headers:
	@echo "[Installing Headers to $(HEADER_DIR)]"
	@mkdir -p $(HEADER_DIR)/$(DRIVER_DIR)
	@install -m 0644 *.h $(HEADER_DIR)
	@install -m 0644 $(DRIVER_DIR)/*.h $(HEADER_DIR)/$(DRIVER_DIR)
	@install -m 0644 $(ARCH_DIR)/*.h $(HEADER_DIR)/$(ARCH_DIR)

upload-headers:
	@echo "[Uploading Headers to $(REMOTE):$(REMOTE_HEADER_DIR)]"
ifeq ($(REMOTE),)
	@echo "$(REMOTE_ERROR)"
	@exit 1
endif
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo mkdir -p $(REMOTE_HEADER_DIR)/$(DRIVER_DIR)"
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "mkdir -p /tmp/RF24 && rm -rf /tmp/RF24/*"
	@rsync -a --include="*.h" --include="*/" --exclude="*" -e "ssh -p $(REMOTE_PORT)" . $(REMOTE):/tmp/RF24
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo install -m 0644 /tmp/RF24/*.h $(REMOTE_HEADER_DIR)"
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo install -m 0644 /tmp/RF24/$(DRIVER_DIR)/*.h $(REMOTE_HEADER_DIR)/$(DRIVER_DIR)"
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "sudo install -m 0644 /tmp/RF24/$(ARCH_DIR)/*.h $(REMOTE_HEADER_DIR)/$(ARCH_DIR)"
	@ssh -q -t -p $(REMOTE_PORT) $(REMOTE) "rm -rf /tmp/RF24"
