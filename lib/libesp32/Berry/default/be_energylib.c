/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ENERGY_SENSOR

extern int b_nrg_read(bvm *vm);

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(energy) {
    be_native_module_function("read", b_nrg_read),
};

be_define_native_module(energy, NULL);
#else
/* @const_object_info_begin
module energy (scope: global) {
    read, func(b_nrg_read)
}
@const_object_info_end */
#include "../generate/be_fixed_energy.h"
#endif

#endif // USE_ENERGY_SENSOR