// /********************************************************************
//  * Tasmota LVGL ctypes mapping
//  *******************************************************************/
#include "be_ctypes.h"

#ifdef USE_ENERGY_SENSOR

extern const be_ctypes_structure_t be_energy_struct[];
be_define_ctypes_class(energy_struct, &be_energy_struct, &be_class_ctypes_bytes, "energy_struct");

#endif // USE_ENERGY_SENSOR
