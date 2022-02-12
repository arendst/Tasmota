/********************************************************************
 * Tasmota lib
 * 
 * To use: `import display`
 * 
 * Initialize Universal Display driver
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#ifdef USE_DISPLAY

extern int be_ntv_display_start(bvm *vm);
extern int be_ntv_display_dimmer(bvm *vm);
extern void be_ntv_display_touch_update(int32_t touches, int32_t raw_x, int32_t raw_y, int32_t gesture);
            BE_FUNC_CTYPE_DECLARE(be_ntv_display_touch_update, "", "iiii[ii]")

/* @const_object_info_begin
module display (scope: global) {
    start, func(be_ntv_display_start)
    dimmer, func(be_ntv_display_dimmer)
    touch_update, ctype_func(be_ntv_display_touch_update)
}
@const_object_info_end */
#include "be_fixed_display.h"

#endif // USE_DISPLAY
