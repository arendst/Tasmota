/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ENERGY_SENSOR

// unless we get it from an include
#define ENERGY_MAX_PHASES         8        // Support max eight phases/channels

extern struct ENERGY Energy;
extern int module_energy_update_total(bvm *vm);

#include "solidify/solidified_energy.h"
#include "be_fixed_energy.h"

/* @const_object_info_begin

module energy (scope: global) {
  init, closure(module_energy_init_closure)
  _ptr, comptr(&Energy)
  _deref, closure(module_energy__deref_closure)
  _phases, int(ENERGY_MAX_PHASES)

  _phases_float, class(be_class_energy_phases_float)
  _phases_int32, class(be_class_energy_phases_int32)
  _phases_uint8, class(be_class_energy_phases_uint8)
  _phases_uint16, class(be_class_energy_phases_uint16)

  read, closure(module_energy_read_closure)
  member, closure(module_energy_member_closure)
  setmember, closure(module_energy_setmember_closure)
  tomap, closure(module_energy_tomap_closure)

  update_total, func(module_energy_update_total)
}

@const_object_info_end */

#endif // USE_ENERGY_SENSOR