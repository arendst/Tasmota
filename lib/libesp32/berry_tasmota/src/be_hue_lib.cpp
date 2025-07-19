/********************************************************************
 * Berry module `hue`
 * 
 * Management of virtual lights for use with Alexa-Hue emulation
 * 
 * To use: `import hue`
 * 
 * 
 *******************************************************************/

#if defined(USE_EMULATION) && defined(USE_EMULATION_HUE)

#include "be_constobj.h"
#include "be_mapping.h"

extern char* be_hue_light_state(class LightStateClass* l);      BE_FUNC_CTYPE_DECLARE(be_hue_light_state, "$", "(light_state)")
extern char* be_hue_light_full_state(int32_t id, class LightStateClass* l, const char* name, const char* model, const char* manuf);
  BE_FUNC_CTYPE_DECLARE(be_hue_light_full_state, "$", "i(light_state)s[ss]")

/* @const_object_info_begin
module hue_ntv (scope: global) {
  light_state, ctype_func(be_hue_light_state)
  full_state, ctype_func(be_hue_light_full_state)
}
@const_object_info_end */
#include "be_fixed_hue_ntv.h"

#endif // defined(USE_EMULATION) && defined(USE_EMULATION_HUE)
