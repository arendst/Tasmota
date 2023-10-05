/********************************************************************
 * Tasmota lib
 * 
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#ifdef USE_LIGHT
extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);

extern int l_gamma8(bvm *vm);
extern int l_gamma10(bvm *vm);
extern int l_rev_gamma10(bvm *vm);

// light.set_bri(bri:int) -> nil
extern void l_set_bri(int bri);
BE_FUNC_CTYPE_DECLARE(l_set_bri, "", "i");

/* @const_object_info_begin
module light (scope: global) {
    get, func(l_getlight)
    set, func(l_setlight)

    gamma8, func(l_gamma8)
    gamma10, func(l_gamma10)
    reverse_gamma10, func(l_rev_gamma10)

    set_bri, ctype_func(l_set_bri)
}
@const_object_info_end */
#include "be_fixed_light.h"

#endif // USE_LIGHT