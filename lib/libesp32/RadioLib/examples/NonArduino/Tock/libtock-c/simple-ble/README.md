Simple BLE Library
==================

A library for simplifying interactions with a BLE radio. So far, this library
helps serialize GAP advertizing data as defined in the Bluetooth Core
Specification Supplement (CSS).

Using `simple-ble` in Tock
--------------------

To use `simple-ble`, add the following include to the application's
Makefile:

    EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/simple-ble

To make sure the library is compiled when compiling your process, you can also add:

    include $(TOCK_USERLAND_BASE_DIR)/simple-ble/Makefile
