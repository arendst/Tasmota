/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ENERGY_SENSOR

extern struct ENERGY Energy;
extern int module_energy_update_total(bvm *vm);

#include "solidify/solidified_energy.h"
#include "be_fixed_energy.h"

/* @const_object_info_begin

module energy (scope: global) {
  init, closure(module_energy_init_closure)
  _ptr, comptr(&Energy)
  _deref, closure(module_energy__deref_closure)

  read, closure(module_energy_read_closure)
  member, closure(module_energy_member_closure)
  setmember, closure(module_energy_setmember_closure)

  update_total, func(module_energy_update_total)
}

@const_object_info_end */

#endif // USE_ENERGY_SENSOR