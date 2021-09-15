/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ENERGY_SENSOR

extern int b_nrg_read(bvm *vm);

/* @const_object_info_begin
module energy (scope: global) {
    read, func(b_nrg_read)
}
@const_object_info_end */
#include "../generate/be_fixed_energy.h"

#endif // USE_ENERGY_SENSOR