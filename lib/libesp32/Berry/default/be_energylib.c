/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_object.h"

extern int b_nrg_read(bvm *vm);

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later
be_native_module_attr_table(energy) {
    be_native_module_function("read", b_nrg_read),
};

be_define_native_module(energy, NULL);
#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    get_free_heap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
