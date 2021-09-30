/********************************************************************
 * Tasmota LVGL ctypes mapping
 *******************************************************************/
#include "be_ctypes.h"

#ifdef USE_ENERGY_SENSOR

/********************************************************************
 * Generated code, don't edit
 *******************************************************************/

static const char * be_ctypes_instance_mappings[];    /* forward definition */

const be_ctypes_structure_t be_energy_struct = {
  170,  /* size in bytes */
  66,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[66]) {
    { "active_power", 24, 0, 0, 5, 0 },
    { "active_power_2", 28, 0, 0, 5, 0 },
    { "active_power_3", 32, 0, 0, 5, 0 },
    { "apparent_power", 36, 0, 0, 5, 0 },
    { "apparent_power_2", 40, 0, 0, 5, 0 },
    { "apparent_power_3", 44, 0, 0, 5, 0 },
    { "command_code", 125, 0, 0, 1, 0 },
    { "current", 12, 0, 0, 5, 0 },
    { "current_2", 16, 0, 0, 5, 0 },
    { "current_3", 20, 0, 0, 5, 0 },
    { "current_available", 135, 0, 0, 1, 0 },
    { "daily", 100, 0, 0, 5, 0 },
    { "data_valid", 126, 0, 0, 1, 0 },
    { "data_valid_2", 127, 0, 0, 1, 0 },
    { "data_valid_3", 128, 0, 0, 1, 0 },
    { "export_active", 84, 0, 0, 5, 0 },
    { "export_active_2", 88, 0, 0, 5, 0 },
    { "export_active_3", 92, 0, 0, 5, 0 },
    { "fifth_second", 124, 0, 0, 1, 0 },
    { "frequency", 72, 0, 0, 5, 0 },
    { "frequency_2", 76, 0, 0, 5, 0 },
    { "frequency_3", 80, 0, 0, 5, 0 },
    { "frequency_common", 131, 0, 0, 1, 0 },
    { "max_current_flag", 162, 0, 0, 1, 0 },
    { "max_energy_state", 169, 0, 0, 1, 0 },
    { "max_power_flag", 158, 0, 0, 1, 0 },
    { "max_voltage_flag", 160, 0, 0, 1, 0 },
    { "min_current_flag", 161, 0, 0, 1, 0 },
    { "min_power_flag", 157, 0, 0, 1, 0 },
    { "min_voltage_flag", 159, 0, 0, 1, 0 },
    { "mplh_counter", 164, 0, 0, 2, 0 },
    { "mplr_counter", 168, 0, 0, 1, 0 },
    { "mplw_counter", 166, 0, 0, 2, 0 },
    { "period", 120, 0, 0, 4, 0 },
    { "phase_count", 129, 0, 0, 1, 0 },
    { "power_factor", 60, 0, 0, 5, 0 },
    { "power_factor_2", 64, 0, 0, 5, 0 },
    { "power_factor_3", 68, 0, 0, 5, 0 },
    { "power_history_0", 138, 0, 0, 2, 0 },
    { "power_history_0_2", 140, 0, 0, 2, 0 },
    { "power_history_0_3", 142, 0, 0, 2, 0 },
    { "power_history_1", 144, 0, 0, 2, 0 },
    { "power_history_1_2", 146, 0, 0, 2, 0 },
    { "power_history_1_3", 148, 0, 0, 2, 0 },
    { "power_history_2", 150, 0, 0, 2, 0 },
    { "power_history_2_2", 152, 0, 0, 2, 0 },
    { "power_history_2_3", 154, 0, 0, 2, 0 },
    { "power_on", 137, 0, 0, 1, 0 },
    { "power_steady_counter", 156, 0, 0, 1, 0 },
    { "reactive_power", 48, 0, 0, 5, 0 },
    { "reactive_power_2", 52, 0, 0, 5, 0 },
    { "reactive_power_3", 56, 0, 0, 5, 0 },
    { "start_energy", 96, 0, 0, 5, 0 },
    { "stuff", 163, 0, 0, 1, 0 },
    { "today_delta_kwh", 108, 0, 0, 4, 0 },
    { "today_kwh", 116, 0, 0, 4, 0 },
    { "today_offset_init_kwh", 133, 0, 0, 1, 0 },
    { "today_offset_kwh", 112, 0, 0, 4, 0 },
    { "total", 104, 0, 0, 5, 0 },
    { "type_dc", 136, 0, 0, 1, 0 },
    { "use_overtemp", 132, 0, 0, 1, 0 },
    { "voltage", 0, 0, 0, 5, 0 },
    { "voltage_2", 4, 0, 0, 5, 0 },
    { "voltage_3", 8, 0, 0, 5, 0 },
    { "voltage_available", 134, 0, 0, 1, 0 },
    { "voltage_common", 130, 0, 0, 1, 0 },
}};

static const char * be_ctypes_instance_mappings[] = {
  NULL
};

static be_define_ctypes_class(energy_struct, &be_energy_struct, &be_class_ctypes, "energy_struct");

void be_load_ctypes_energy_definitions_lib(bvm *vm) {
  ctypes_register_class(vm, &be_class_energy_struct, &be_energy_struct);
}
/********************************************************************/

#endif // USE_ENERGY_SENSOR