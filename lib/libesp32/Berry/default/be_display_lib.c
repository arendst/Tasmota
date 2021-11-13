/********************************************************************
 * Tasmota lib
 * 
 * To use: `import display`
 * 
 * Initialize Universal Display driver
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_DISPLAY

// Tasmota specific

extern int be_disp_start(bvm *vm);

/********************************************************************
** Solidified module: display
********************************************************************/
be_local_module(display,
    "display",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("start", 1697318111, 5, -1), be_const_func(be_disp_start) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(display);
/********************************************************************/

#endif // USE_DISPLAY