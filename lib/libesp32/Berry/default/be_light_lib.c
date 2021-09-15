/********************************************************************
 * Tasmota lib
 * 
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LIGHT
extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);

extern int l_gamma8(bvm *vm);
extern int l_gamma10(bvm *vm);
extern int l_rev_gamma10(bvm *vm);

/* @const_object_info_begin
module light (scope: global) {
    get, func(l_getlight)
    set, func(l_setlight)

    gamma8, func(l_gamma8)
    gamma10, func(l_gamma10)
    reverse_gamma10, func(l_rev_gamma10)
}
@const_object_info_end */
#include "../generate/be_fixed_light.h"

#endif // USE_LIGHT