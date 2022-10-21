/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <hap.h>
#include <hap_apple_chars.h>

/* Char: Brightness */
hap_char_t *hap_char_brightness_create(int brightness)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_BRIGHTNESS,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, brightness);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 100, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Brightness */
hap_char_t *hap_char_wattage_create(float watts)
{
  hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, watts);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, -10000.0, 10000.0, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_LUX);

    return hc;
}

/* Char: Cooling Threshold Temperature */
hap_char_t *hap_char_cooling_threshold_temperature_create(float cooling_threshold_temp)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_COOLING_THRESHOLD_TEMPERATURE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, cooling_threshold_temp);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 10.0, 35.0, 0.1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_CELSIUS);

    return hc;
}

/* Char: Current Door State */
hap_char_t *hap_char_current_door_state_create(uint8_t curr_door_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_DOOR_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_door_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 4, 1);

    return hc;
}

/* Char: Current Heating Cooling State */
hap_char_t *hap_char_current_heating_cooling_state_create(uint8_t curr_heating_cooling_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_HEATING_COOLING_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_heating_cooling_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Current Relative Humidity */
hap_char_t *hap_char_current_relative_humidity_create(float curr_rel_humidity)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_rel_humidity);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Current Temperature */
hap_char_t *hap_char_current_temperature_create(float curr_temp)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CURRENT_TEMPERATURE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_temp);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 0.1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_CELSIUS);

    return hc;
}

/* Char: Firmware Revision */
hap_char_t *hap_char_firmware_revision_create(const char *fw_rev)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_FIRMWARE_REVISION,
                                            HAP_CHAR_PERM_PR, fw_rev);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Hardware Revision */
hap_char_t *hap_char_hardware_revision_create(const char *hw_rev)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_HARDWARE_REVISION,
                                            HAP_CHAR_PERM_PR, hw_rev);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Heating Threshold Temperature */
hap_char_t *hap_char_heating_threshold_temperature_create(float heating_threshold_temp)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_HEATING_THRESHOLD_TEMPERATURE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, heating_threshold_temp);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 25.0, 0.1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_CELSIUS);

    return hc;
}

/* Char: Hue */
hap_char_t *hap_char_hue_create(float hue)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_HUE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, hue);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 360.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Identify */
hap_char_t *hap_char_identify_create(void)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_IDENTIFY, HAP_CHAR_PERM_PW, false);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Lock Current State */
hap_char_t *hap_char_lock_current_state_create(uint8_t lock_curr_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_LOCK_CURRENT_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, lock_curr_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3, 1);

    return hc;
}

/* Char: Lock Target State */
hap_char_t *hap_char_lock_target_state_create(uint8_t lock_targ_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_LOCK_TARGET_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, lock_targ_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Manufacturer */
hap_char_t *hap_char_manufacturer_create(const char *manufacturer)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_MANUFACTURER,
                                            HAP_CHAR_PERM_PR, manufacturer);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Model */
hap_char_t *hap_char_model_create(const char *model)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_MODEL,
                                            HAP_CHAR_PERM_PR, model);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Motion Detected */
hap_char_t *hap_char_motion_detected_create(bool motion_detected)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_MOTION_DETECTED,
                                          HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, motion_detected);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Name */
hap_char_t *hap_char_name_create(const char *name)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_NAME,
                                            HAP_CHAR_PERM_PR, name);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Obstruction Detected */
hap_char_t *hap_char_obstruction_detect_create(bool obstr_detect)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_OBSTRUCTION_DETECTED,
                                          HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, obstr_detect);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: On */
hap_char_t *hap_char_on_create(bool on)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_ON,
                                          HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, on);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Outlet in Use */
hap_char_t *hap_char_outlet_in_use_create(bool outlet_in_use)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_OUTLET_IN_USE,
                                          HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, outlet_in_use);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Rotation Direction */
hap_char_t *hap_char_rotation_direction_create(int rotation_direction)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_ROTATION_DIRECTION,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, rotation_direction);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Rotation Speed */
hap_char_t *hap_char_rotation_speed_create(float rotation_speed)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_ROTATION_SPEED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, rotation_speed);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Saturation */
hap_char_t *hap_char_saturation_create(float saturation)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_SATURATION,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, saturation);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Serial Number */
hap_char_t *hap_char_serial_number_create(const char *serial_num)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_SERIAL_NUMBER,
                                            HAP_CHAR_PERM_PR, serial_num);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Target Door State */
hap_char_t *hap_char_target_door_state_create(uint8_t targ_door_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_DOOR_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_door_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Target Heating Cooling State */
hap_char_t *hap_char_target_heating_cooling_state_create(uint8_t targ_heating_cooling_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_heating_cooling_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3, 1);

    return hc;
}

/* Char: Target Relative Humidity */
hap_char_t *hap_char_target_relative_humidity_create(float targ_rel_humidity)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_TARGET_RELATIVE_HUMIDITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_rel_humidity);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Target Temperature */
hap_char_t *hap_char_target_temperature_create(float targ_temp)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_TARGET_TEMPERATURE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_temp);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 10.0, 38.0, 0.1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_CELSIUS);

    return hc;
}

/* Char: Temperature Display Units */
hap_char_t *hap_char_temperature_display_units_create(uint8_t temp_disp_units)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TEMPERATURE_DISPLAY_UNITS,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, temp_disp_units);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Version */
hap_char_t *hap_char_version_create(const char *version)
{
    hap_char_t *hc = hap_char_string_create(HAP_CHAR_UUID_VERSION,
                                            HAP_CHAR_PERM_PR, version);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Security System Current State */
hap_char_t *hap_char_security_system_current_state_create(uint8_t security_sys_curr_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SECURITY_SYSTEM_CURRENT_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, security_sys_curr_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 4, 1);

    return hc;
}

/* Char: Security System Target State */
hap_char_t *hap_char_security_system_target_state_create(uint8_t security_sys_targ_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SECURITY_SYSTEM_TARGET_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, security_sys_targ_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3, 1);

    return hc;
}

/* Char: Battery Level */
hap_char_t *hap_char_battery_level_create(uint8_t battery_level)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_BATTERY_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, battery_level);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 100, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Carbon Monoxide Detected */
hap_char_t *hap_char_carbon_monoxide_detected_create(uint8_t carbon_monoxide_detected)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CARBON_MONOXIDE_DETECTED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, carbon_monoxide_detected);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Contact Sensor State */
hap_char_t *hap_char_contact_sensor_state_create(uint8_t contact_sensor_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CONTACT_SENSOR_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, contact_sensor_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Current Ambient Light Level */
hap_char_t *hap_char_current_ambient_light_level_create(float curr_ambient_light_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_ambient_light_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0001, 100000.0, 0.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_LUX);

    return hc;
}

/* Char: Current Horizontal Tilt Angle */
hap_char_t *hap_char_current_horizontal_tilt_angle_create(int curr_horiz_tilt_angle)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_CURRENT_HORIZONTAL_TILT_ANGLE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_horiz_tilt_angle);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, -90, 90, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Current Position */
hap_char_t *hap_char_current_position_create(uint8_t curr_pos)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_POSITION,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_pos);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 100, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Current Vertical Tilt Angle */
hap_char_t *hap_char_current_vertical_tilt_angle_create(int curr_vert_tilt_angle)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_CURRENT_VERTICAL_TILT_ANGLE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_vert_tilt_angle);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, -90, 90, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Hold Position */
hap_char_t *hap_char_hold_position_create(bool hold_pos)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_HOLD_POSITION,
                                          HAP_CHAR_PERM_PW, hold_pos);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Leak Detected */
hap_char_t *hap_char_leak_detected_create(uint8_t leak_detected)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_LEAK_DETECTED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, leak_detected);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Occupancy Detected */
hap_char_t *hap_char_occupancy_detected_create(uint8_t occupancy_detected)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_OCCUPANCY_DETECTED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, occupancy_detected);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Position State */
hap_char_t *hap_char_position_state_create(uint8_t pos_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_POSITION_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, pos_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Programmable Switch Event */
hap_char_t *hap_char_programmable_switch_event_create(uint8_t programmable_switch_event)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV | HAP_CHAR_PERM_SPECIAL_READ,
                                           programmable_switch_event);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Status Active */
hap_char_t *hap_char_status_active_create(bool status_active)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_STATUS_ACTIVE,
                                          HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, status_active);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Smoke Detected */
hap_char_t *hap_char_smoke_detected_create(uint8_t smoke_detected)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SMOKE_DETECTED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, smoke_detected);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Status Fault */
hap_char_t *hap_char_status_fault_create(uint8_t status_fault)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_STATUS_FAULT,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, status_fault);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Status Low Battery */
hap_char_t *hap_char_status_low_battery_create(uint8_t status_low_battery)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_STATUS_LOW_BATTERY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, status_low_battery);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Status Tampered */
hap_char_t *hap_char_status_tampered_create(uint8_t status_tampered)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_STATUS_TAMPERED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, status_tampered);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Target Horizontal Tilt Angle */
hap_char_t *hap_char_target_horizontal_tilt_angle_create(int targ_horiz_tilt_angle)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_TARGET_HORIZONTAL_TILT_ANGLE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_horiz_tilt_angle);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, -90, 90, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Target Position */
hap_char_t *hap_char_target_position_create(uint8_t targ_pos)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_POSITION,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_pos);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 100, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Target Vertical Tilt Angle */
hap_char_t *hap_char_target_vertical_tilt_angle_create(int targ_vert_tilt_angle)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_TARGET_VERTICAL_TILT_ANGLE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_vert_tilt_angle);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, -90, 90, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Security System Alarm Type */
hap_char_t *hap_char_security_system_alarm_type_create(uint8_t security_sys_alarm_type)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_STATUS_SECURITY_SYSTEM_ALARM_TYPE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, security_sys_alarm_type);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Charging State */
hap_char_t *hap_char_charging_state_create(uint8_t charging_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CHARGING_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, charging_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Carbon Monoxide Level */
hap_char_t *hap_char_carbon_monoxide_level_create(float carbon_monoxide_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CARBON_MONOXIDE_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, carbon_monoxide_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 0.0);

    return hc;
}

/* Char: Carbon Monoxide Peak Level */
hap_char_t *hap_char_carbon_monoxide_peak_level_create(float carbon_monoxide_peak_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CARBON_MONOXIDE_PEAK_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, carbon_monoxide_peak_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 0.0);

    return hc;
}

/* Char: Carbon Dioxide Detected */
hap_char_t *hap_char_carbon_dioxide_detected_create(uint8_t carbon_dioxide_detected)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CARBON_DIOXIDE_DETECTED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, carbon_dioxide_detected);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Carbon Dioxide Level */
hap_char_t *hap_char_carbon_dioxide_level_create(float carbon_dioxide_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CARBON_DIOXIDE_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, carbon_dioxide_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100000.0, 0.0);

    return hc;
}

/* Char: Carbon Dioxide Peak Level */
hap_char_t *hap_char_carbon_dioxide_peak_level_create(float carbon_dioxide_peak_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_CARBON_DIOXIDE_PEAK_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, carbon_dioxide_peak_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100000.0, 0.0);

    return hc;
}


/* Char: Air Quality */
hap_char_t *hap_char_air_quality_create(uint8_t air_quality)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_AIR_QUALITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, air_quality);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 5, 1);

    return hc;
}

/* Char: Accessory Flags */
hap_char_t *hap_char_accessory_flags_create(uint32_t flags)
{
    hap_char_t *hc = hap_char_uint32_create(HAP_CHAR_UUID_ACCESSORY_FLAGS,
                                            HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, flags);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Product Data */
hap_char_t *hap_char_product_data_create(hap_data_val_t *product_data)
{
    hap_char_t *hc = hap_char_data_create(HAP_CHAR_UUID_PRODUCT_DATA,
                                            HAP_CHAR_PERM_PR , product_data);
    if (!hc) {
        return NULL;
    }
    return hc;
}

/* Char: Lock Physical Controls */
hap_char_t *hap_char_lock_physical_controls_create(uint8_t lock_physical_controls)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_LOCK_PHYSICAL_CONTROLS,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, lock_physical_controls);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Current Air Purifier State */
hap_char_t *hap_char_current_air_purifier_state_create(uint8_t curr_air_purifier_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_AIR_PURIFIER_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_air_purifier_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Current Slat State */
hap_char_t *hap_char_current_slat_state_create(uint8_t curr_slat_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_SLAT_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_slat_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Slat Type */
hap_char_t *hap_char_slat_type_create(uint8_t slat_type)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SLAT_TYPE,
                                           HAP_CHAR_PERM_PR, slat_type);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Filter Life Level */
hap_char_t *hap_char_filter_life_level_create(float filter_life_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_FILTER_LIFE_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, filter_life_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);

    return hc;
}

/* Char: Filter Change Indication */
hap_char_t *hap_char_filter_change_indication_create(uint8_t filter_change_indication)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_FILTER_CHANGE_INDICATION,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, filter_change_indication);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Reset Filter Indication */
hap_char_t *hap_char_reset_filter_indication_create(uint8_t reset_filter_indication)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_RESET_FILTER_INDICATION,
                                           HAP_CHAR_PERM_PW, reset_filter_indication);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 1, 1, 0);

    return hc;
}

/* Char: Target Air Purifier State */
hap_char_t *hap_char_target_air_purifier_state_create(uint8_t targ_air_purifier_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_AIR_PURIFIER_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_air_purifier_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Target Fan State */
hap_char_t *hap_char_target_fan_state_create(uint8_t targ_fan_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_FAN_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_fan_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Current Fan State */
hap_char_t *hap_char_current_fan_state_create(uint8_t curr_fan_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_FAN_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_fan_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Active State */
hap_char_t *hap_char_active_create(uint8_t active)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_ACTIVE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, active);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Swing Mode */
hap_char_t *hap_char_swing_mode_create(uint8_t swing_mode)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SWING_MODE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, swing_mode);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Current Tilt Angle */
hap_char_t *hap_char_current_tilt_angle_create(int curr_tilt_angle)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_CURRENT_TILT_ANGLE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_tilt_angle);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, -90, 90, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Target Tilt Angle */
hap_char_t *hap_char_target_tilt_angle_create(int targ_tilt_angle)
{
    hap_char_t *hc = hap_char_int_create(HAP_CHAR_UUID_TARGET_TILT_ANGLE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_tilt_angle);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, -90, 90, 1);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_ARCDEGREES);

    return hc;
}

/* Char: Ozone Density */
hap_char_t *hap_char_ozone_density_create(float ozone_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_OZONE_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, ozone_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 1000.0, 0.0);

    return hc;
}

/* Char: Nitrogen Dioxide Density */
hap_char_t *hap_char_nitrogen_dioxide_density_create(float nitrogen_dioxide_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_NITROGEN_DIOXIDE_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, nitrogen_dioxide_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 1000.0, 0.0);

    return hc;
}

/* Char: Sulphur Dioxide Density */
hap_char_t *hap_char_sulphur_dioxide_density_create(float sulphur_dioxide_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_SULPHUR_DIOXIDE_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, sulphur_dioxide_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 1000.0, 0.0);

    return hc;
}

/* Char: PM2.5 Density */
hap_char_t *hap_char_pm_2_5_density_create(float pm_2_5_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_PM_2_5_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, pm_2_5_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 1000.0, 0.0);

    return hc;
}

/* Char: PM10 Density */
hap_char_t *hap_char_pm_10_density_create(float pm_10_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_PM_10_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, pm_10_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 1000.0, 0.0);

    return hc;
}

/* Char: VOC Density */
hap_char_t *hap_char_voc_density_create(float voc_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_VOC_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, voc_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 1000.0, 0.0);

    return hc;
}

/* Char: Service Label Index */
hap_char_t *hap_char_service_label_index_create(uint8_t service_label_index)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SERVICE_LABEL_INDEX,
                                         HAP_CHAR_PERM_PR, service_label_index);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Service Label Namespace */
hap_char_t *hap_char_service_label_namespace_create(uint8_t service_label_namespace)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_SERVICE_LABEL_NAMESPACE,
                                         HAP_CHAR_PERM_PR, service_label_namespace);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Color Temperature */
hap_char_t *hap_char_color_temperature_create(uint32_t color_temp)
{
    hap_char_t *hc = hap_char_uint32_create(HAP_CHAR_UUID_COLOR_TEMPERATURE,
                                            HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, color_temp);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 50, 400, 1);

    return hc;
}

/* Char: Current Heater Cooler State */
hap_char_t *hap_char_current_heater_cooler_state_create(uint8_t curr_heater_cooler_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_HEATER_COOLER_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_heater_cooler_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3, 1);

    return hc;
}

/* Char: Target Heater Cooler State */
hap_char_t *hap_char_target_heater_cooler_state_create(uint8_t targ_heater_cooler_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_HEATER_COOLER_STATE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_heater_cooler_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Current Humidifier Dehumidifier State */
hap_char_t *hap_char_current_humidifier_dehumidifier_state_create(uint8_t curr_humidifier_dehumidifier_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_CURRENT_HUMIDIFIER_DEHUMIDIFIER_STATE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, curr_humidifier_dehumidifier_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3, 1);

    return hc;
}

/* Char: Target Humidifier Dehumidifier State */
hap_char_t *hap_char_target_humidifier_dehumidifier_state_create(uint8_t targ_humidifier_dehumidifier_state)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_TARGET_HUMIDIFIER_DEHUMIDIFIER_STATE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, targ_humidifier_dehumidifier_state);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: Water Level */
hap_char_t *hap_char_water_level_create(float water_level)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_WATER_LEVEL,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, water_level);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);

    return hc;
}

/* Char: Relative Humidity Dehumidifier Threshold  */
hap_char_t *hap_char_relative_humidity_dehumidifier_threshold_create(float rel_humidity_dehumidifier_threshold)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_RELATIVE_HUMIDITY_DEHUMIDIFIER_THRESHOLD,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, rel_humidity_dehumidifier_threshold);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_PERCENTAGE);

    return hc;
}

/* Char: Relative Humidity Humidifier Threshold  */
hap_char_t *hap_char_relative_humidity_humidifier_threshold_create(float rel_humidity_humidifier_threshold)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_RELATIVE_HUMIDITY_HUMIDIFIER_THRESHOLD,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, rel_humidity_humidifier_threshold);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0.0, 100.0, 1.0);
    hap_char_add_unit(hc, HAP_CHAR_UNIT_LUX);

    return hc;
}

/* Char: Program Mode */
hap_char_t *hap_char_program_mode_create(uint8_t prog_mode)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_PROGRAM_MODE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, prog_mode);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 2, 1);

    return hc;
}

/* Char: In Use */
hap_char_t *hap_char_in_use_create(uint8_t in_use)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_IN_USE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, in_use);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Set Duration */
hap_char_t *hap_char_set_duration_create(uint32_t set_duration)
{
    hap_char_t *hc = hap_char_uint32_create(HAP_CHAR_UUID_SET_DURATION,
                                            HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, set_duration);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3600, 1);

    return hc;
}

/* Char: Remaining Duration */
hap_char_t *hap_char_remaining_duration_create(uint32_t remaining_duration)
{
    hap_char_t *hc = hap_char_uint32_create(HAP_CHAR_UUID_REMAINING_DURATION,
                                            HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, remaining_duration);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3600, 1);

    return hc;
}

/* Char: Valve Type */
hap_char_t *hap_char_valve_type_create(uint8_t valve_type)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_VALVE_TYPE,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, valve_type);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 3, 1);

    return hc;
}

/* Char: Is Configured */
hap_char_t *hap_char_is_configured_create(uint8_t is_configured)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_IS_CONFIGURED,
                                         HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV, is_configured);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Status Jammed */
hap_char_t *hap_char_status_jammed_create(uint8_t status_jammed)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_STATUS_JAMMED,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV, status_jammed);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}

/* Char: Administrator Only Access */
hap_char_t *hap_char_administrator_only_access_create(bool administrator_only_access)
{
    hap_char_t *hc = hap_char_bool_create(HAP_CHAR_UUID_ADMINISTRATOR_ONLY_ACCESS,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV,
                                           administrator_only_access);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Lock Control Point */
hap_char_t *hap_char_lock_control_point_create(hap_tlv8_val_t *lock_control_point)
{
    hap_char_t *hc = hap_char_tlv8_create(HAP_CHAR_UUID_LOCK_CONTROL_POINT,
                                           HAP_CHAR_PERM_PW , lock_control_point);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Lock Last Known Action */
hap_char_t *hap_char_lock_last_known_action_create(uint8_t lock_last_known_action)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_LOCK_LAST_KNOWN_ACTION,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV , lock_last_known_action);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 8, 1);

    return hc;
}

/* Char: Lock Management Auto Security Timeout */
hap_char_t *hap_char_lock_management_auto_security_timeout_create(uint32_t lock_management_auto_security_timeout)
{
    hap_char_t *hc = hap_char_uint32_create(HAP_CHAR_UUID_LOCK_MANAGEMENT_AUTO_SECURITY_TIMEOUT,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV,
                                           lock_management_auto_security_timeout);
    if (!hc) {
        return NULL;
    }

    hap_char_add_unit(hc, HAP_CHAR_UNIT_SECONDS);

    return hc;
}

/* Char: Logs */
hap_char_t *hap_char_logs_create(hap_tlv8_val_t *logs)
{
    hap_char_t *hc = hap_char_tlv8_create(HAP_CHAR_UUID_LOGS,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV , logs);
    if (!hc) {
        return NULL;
    }

    return hc;
}

/* Char: Air Particulate Density */
hap_char_t *hap_char_air_particulate_density_create(float air_particulate_density)
{
    hap_char_t *hc = hap_char_float_create(HAP_CHAR_UUID_AIR_PARTICULATE_DENSITY,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV , air_particulate_density);
    if (!hc) {
        return NULL;
    }

    hap_char_float_set_constraints(hc, 0, 1000, 0.0);

    return hc;
}

/* Char: Air Particulate Size */
hap_char_t *hap_char_air_particulate_size_create(uint8_t air_particulate_size)
{
    hap_char_t *hc = hap_char_uint8_create(HAP_CHAR_UUID_AIR_PARTICULATE_SIZE,
                                           HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV , air_particulate_size);
    if (!hc) {
        return NULL;
    }

    hap_char_int_set_constraints(hc, 0, 1, 1);

    return hc;
}
