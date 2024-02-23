nRF Serialization Library for Tock
==================================

`libnrfserialization` is a pre-compiled library for the Nordic nRF51822
application-side serialization tool. Briefy, serialization allows Nordic's BLE
SDK library to run on one microcontroller (in this case the main Tock MCU) and
then rather than call down to a Softdevice, the commands are packetized and sent
to an nRF co-processor, which executes the actual BLE operations.

`libnrfserialization` allows any Tock application that wishes to use
serialization to compile against the library, simplifying development.


Using `libnrfserialization`
--------------

To use `libnrfserialization`, add the following include to the application's
Makefile:

    EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/libnrfserialization



Compiling `libnrfserialization`
------------------

    ./create_libnrfserialization.sh
