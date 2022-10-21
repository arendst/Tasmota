/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_constobj.h"

#if defined(USE_ONEWIRE) || defined(USE_DS18x20)

extern int b_onewire_init(bvm *vm);
extern int b_onewire_deinit(bvm *vm);

extern int b_onewire_reset(bvm *vm);
extern int b_onewire_select(bvm *vm);
extern int b_onewire_skip(bvm *vm);
extern int b_onewire_depower(bvm *vm);

extern int b_onewire_write(bvm *vm);
extern int b_onewire_read(bvm *vm);

extern int b_onewire_reset_search(bvm *vm);
extern int b_onewire_target_search(bvm *vm);
extern int b_onewire_search(bvm *vm);

#include "be_fixed_be_class_OneWire.h"

/* @const_object_info_begin

class be_class_OneWire (scope: global, name: OneWire, strings: weak) {
    .p, var

    init, func(b_onewire_init)
    deinit, func(b_onewire_deinit)

    reset, func(b_onewire_reset)
    select, func(b_onewire_select)
    skip, func(b_onewire_skip)
    depower, func(b_onewire_depower)

    write, func(b_onewire_write)
    read, func(b_onewire_read)

    reset_search, func(b_onewire_reset_search)
    target_search, func(b_onewire_target_search)
    search, func(b_onewire_search)
}
@const_object_info_end */

#endif // defined(USE_ONEWIRE) || defined(USE_DS18x20)
