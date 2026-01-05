/********************************************************************
 * Tasmota lib for the ISP of the ESP32-P4 (and maybe successors)
 * 
 * To use: import ISP`
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#if !defined(USE_BERRY_ISP)

#include "be_constobj.h"
#include "be_mapping.h"

extern int be_ISP_init(bvm *vm);
extern int be_ISP_set_ccm(bvm *vm);
extern int be_ISP_set_gamma(bvm *vm);
extern int be_ISP_set_sharpen(bvm *vm);
extern int be_ISP_set_color(bvm *vm);
extern int be_ISP_set_bf(bvm *vm);

/* @const_object_info_begin
module ISP (scope: global) {
  init,        func(be_ISP_init)
  set_ccm,     func(be_ISP_set_ccm)
  set_gamma,   func(be_ISP_set_gamma)
  set_sharpen, func(be_ISP_set_sharpen)
  set_color,   func(be_ISP_set_color)
  set_bf,      func(be_ISP_set_bf)
}
@const_object_info_end */

#include "be_fixed_ISP.h"

#endif // USE_BERRY_ULP
