/********************************************************************
 * Tasmota lib
 * 
 * To use: `import display`
 * 
 * Initialize Universal Display driver
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_DISPLAY

extern int be_ntv_display_start(bvm *vm);
extern int be_ntv_display_dimmer(bvm *vm);

/* @const_object_info_begin
module display (scope: global) {
    start, func(be_ntv_display_start)
    dimmer, func(be_ntv_display_dimmer)
}
@const_object_info_end */
#include "be_fixed_display.h"

#endif // USE_DISPLAY
