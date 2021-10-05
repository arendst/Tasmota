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
  250,  /* size in bytes */
  85,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[85]) {
    { "active_power", 24, 0, 0, ctypes_float, 0 },
    { "active_power_2", 28, 0, 0, ctypes_float, 0 },
    { "active_power_3", 32, 0, 0, ctypes_float, 0 },
    { "apparent_power", 36, 0, 0, ctypes_float, 0 },
    { "apparent_power_2", 40, 0, 0, ctypes_float, 0 },
    { "apparent_power_3", 44, 0, 0, ctypes_float, 0 },
    { "command_code", 205, 0, 0, ctypes_u8, 0 },
    { "current", 12, 0, 0, ctypes_float, 0 },
    { "current_2", 16, 0, 0, ctypes_float, 0 },
    { "current_3", 20, 0, 0, ctypes_float, 0 },
    { "current_available", 215, 0, 0, ctypes_u8, 0 },
    { "daily", 120, 0, 0, ctypes_float, 0 },
    { "daily_2", 124, 0, 0, ctypes_float, 0 },
    { "daily_3", 128, 0, 0, ctypes_float, 0 },
    { "daily_sum", 144, 0, 0, ctypes_float, 0 },
    { "data_valid", 206, 0, 0, ctypes_u8, 0 },
    { "data_valid_2", 207, 0, 0, ctypes_u8, 0 },
    { "data_valid_3", 208, 0, 0, ctypes_u8, 0 },
    { "export_active", 96, 0, 0, ctypes_float, 0 },
    { "export_active_2", 100, 0, 0, ctypes_float, 0 },
    { "export_active_3", 104, 0, 0, ctypes_float, 0 },
    { "fifth_second", 204, 0, 0, ctypes_u8, 0 },
    { "frequency", 72, 0, 0, ctypes_float, 0 },
    { "frequency_2", 76, 0, 0, ctypes_float, 0 },
    { "frequency_3", 80, 0, 0, ctypes_float, 0 },
    { "frequency_common", 211, 0, 0, ctypes_u8, 0 },
    { "import_active", 84, 0, 0, ctypes_float, 0 },
    { "import_active_2", 88, 0, 0, ctypes_float, 0 },
    { "import_active_3", 92, 0, 0, ctypes_float, 0 },
    { "max_current_flag", 242, 0, 0, ctypes_u8, 0 },
    { "max_energy_state", 249, 0, 0, ctypes_u8, 0 },
    { "max_power_flag", 238, 0, 0, ctypes_u8, 0 },
    { "max_voltage_flag", 240, 0, 0, ctypes_u8, 0 },
    { "min_current_flag", 241, 0, 0, ctypes_u8, 0 },
    { "min_power_flag", 237, 0, 0, ctypes_u8, 0 },
    { "min_voltage_flag", 239, 0, 0, ctypes_u8, 0 },
    { "mplh_counter", 244, 0, 0, ctypes_u16, 0 },
    { "mplr_counter", 248, 0, 0, ctypes_u8, 0 },
    { "mplw_counter", 246, 0, 0, ctypes_u16, 0 },
    { "period", 192, 0, 0, ctypes_u32, 0 },
    { "period_2", 196, 0, 0, ctypes_u32, 0 },
    { "period_3", 200, 0, 0, ctypes_u32, 0 },
    { "phase_count", 209, 0, 0, ctypes_u8, 0 },
    { "power_factor", 60, 0, 0, ctypes_float, 0 },
    { "power_factor_2", 64, 0, 0, ctypes_float, 0 },
    { "power_factor_3", 68, 0, 0, ctypes_float, 0 },
    { "power_history_0", 218, 0, 0, ctypes_u16, 0 },
    { "power_history_0_2", 220, 0, 0, ctypes_u16, 0 },
    { "power_history_0_3", 222, 0, 0, ctypes_u16, 0 },
    { "power_history_1", 224, 0, 0, ctypes_u16, 0 },
    { "power_history_1_2", 226, 0, 0, ctypes_u16, 0 },
    { "power_history_1_3", 228, 0, 0, ctypes_u16, 0 },
    { "power_history_2", 230, 0, 0, ctypes_u16, 0 },
    { "power_history_2_2", 232, 0, 0, ctypes_u16, 0 },
    { "power_history_2_3", 234, 0, 0, ctypes_u16, 0 },
    { "power_on", 217, 0, 0, ctypes_u8, 0 },
    { "power_steady_counter", 236, 0, 0, ctypes_u8, 0 },
    { "reactive_power", 48, 0, 0, ctypes_float, 0 },
    { "reactive_power_2", 52, 0, 0, ctypes_float, 0 },
    { "reactive_power_3", 56, 0, 0, ctypes_float, 0 },
    { "start_energy", 108, 0, 0, ctypes_float, 0 },
    { "start_energy_2", 112, 0, 0, ctypes_float, 0 },
    { "start_energy_3", 116, 0, 0, ctypes_float, 0 },
    { "today_delta_kwh", 156, 0, 0, ctypes_u32, 0 },
    { "today_delta_kwh_2", 160, 0, 0, ctypes_u32, 0 },
    { "today_delta_kwh_3", 164, 0, 0, ctypes_u32, 0 },
    { "today_kwh", 180, 0, 0, ctypes_u32, 0 },
    { "today_kwh_2", 184, 0, 0, ctypes_u32, 0 },
    { "today_kwh_3", 188, 0, 0, ctypes_u32, 0 },
    { "today_offset_init_kwh", 213, 0, 0, ctypes_u8, 0 },
    { "today_offset_kwh", 168, 0, 0, ctypes_u32, 0 },
    { "today_offset_kwh_2", 172, 0, 0, ctypes_u32, 0 },
    { "today_offset_kwh_3", 176, 0, 0, ctypes_u32, 0 },
    { "total", 132, 0, 0, ctypes_float, 0 },
    { "total_2", 136, 0, 0, ctypes_float, 0 },
    { "total_3", 140, 0, 0, ctypes_float, 0 },
    { "total_sum", 148, 0, 0, ctypes_float, 0 },
    { "type_dc", 216, 0, 0, ctypes_u8, 0 },
    { "use_overtemp", 212, 0, 0, ctypes_u8, 0 },
    { "voltage", 0, 0, 0, ctypes_float, 0 },
    { "voltage_2", 4, 0, 0, ctypes_float, 0 },
    { "voltage_3", 8, 0, 0, ctypes_float, 0 },
    { "voltage_available", 214, 0, 0, ctypes_u8, 0 },
    { "voltage_common", 210, 0, 0, ctypes_u8, 0 },
    { "yesterday_sum", 152, 0, 0, ctypes_float, 0 },
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