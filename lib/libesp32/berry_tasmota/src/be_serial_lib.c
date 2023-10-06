/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_constobj.h"
#include "esp_idf_version.h"

extern int b_serial_init(bvm *vm);
extern int b_serial_deinit(bvm *vm);

extern int b_serial_write(bvm *vm);
extern int b_serial_read(bvm *vm);
extern int b_serial_available(bvm *vm);
extern int b_serial_flush(bvm *vm);

#if ESP_IDF_VERSION_MAJOR < 5
    #include "esp32-hal.h"
#else
    // it should be #include "HardwareSerial.h"
    // but the C++ header cannot be included in a C file
    // so there's a copy below
    enum SerialConfig {
        SERIAL_5N1 = 0x8000010,
        SERIAL_6N1 = 0x8000014,
        SERIAL_7N1 = 0x8000018,
        SERIAL_8N1 = 0x800001c,
        SERIAL_5N2 = 0x8000030,
        SERIAL_6N2 = 0x8000034,
        SERIAL_7N2 = 0x8000038,
        SERIAL_8N2 = 0x800003c,
        SERIAL_5E1 = 0x8000012,
        SERIAL_6E1 = 0x8000016,
        SERIAL_7E1 = 0x800001a,
        SERIAL_8E1 = 0x800001e,
        SERIAL_5E2 = 0x8000032,
        SERIAL_6E2 = 0x8000036,
        SERIAL_7E2 = 0x800003a,
        SERIAL_8E2 = 0x800003e,
        SERIAL_5O1 = 0x8000013,
        SERIAL_6O1 = 0x8000017,
        SERIAL_7O1 = 0x800001b,
        SERIAL_8O1 = 0x800001f,
        SERIAL_5O2 = 0x8000033,
        SERIAL_6O2 = 0x8000037,
        SERIAL_7O2 = 0x800003b,
        SERIAL_8O2 = 0x800003f
    };
#endif

#include "be_fixed_be_class_serial.h"

/* @const_object_info_begin

class be_class_serial (scope: global, name: serial) {
    .p, var

    SERIAL_5N1, int(SERIAL_5N1)
    SERIAL_6N1, int(SERIAL_6N1)
    SERIAL_7N1, int(SERIAL_7N1)
    SERIAL_8N1, int(SERIAL_8N1)
    SERIAL_5N2, int(SERIAL_5N2)
    SERIAL_6N2, int(SERIAL_6N2)
    SERIAL_7N2, int(SERIAL_7N2)
    SERIAL_8N2, int(SERIAL_8N2)
    SERIAL_5E1, int(SERIAL_5E1)
    SERIAL_6E1, int(SERIAL_6E1)
    SERIAL_7E1, int(SERIAL_7E1)
    SERIAL_8E1, int(SERIAL_8E1)
    SERIAL_5E2, int(SERIAL_5E2)
    SERIAL_6E2, int(SERIAL_6E2)
    SERIAL_7E2, int(SERIAL_7E2)
    SERIAL_8E2, int(SERIAL_8E2)
    SERIAL_5O1, int(SERIAL_5O1)
    SERIAL_6O1, int(SERIAL_6O1)
    SERIAL_7O1, int(SERIAL_7O1)
    SERIAL_8O1, int(SERIAL_8O1)
    SERIAL_5O2, int(SERIAL_5O2)
    SERIAL_6O2, int(SERIAL_6O2)
    SERIAL_7O2, int(SERIAL_7O2)
    SERIAL_8O2, int(SERIAL_8O2)

    init, func(b_serial_init)
    deinit, func(b_serial_deinit)

    write, func(b_serial_write)
    read, func(b_serial_read)
    available, func(b_serial_available)
    flush, func(b_serial_flush)
}
@const_object_info_end */
