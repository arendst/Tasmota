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
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>

hap_serv_t *hap_serv_accessory_information_create(hap_acc_cfg_t *cfg)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_ACCESSORY_INFORMATION);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_name_create(cfg->name)) != HAP_SUCCESS) {
        goto err;
    }

    if (hap_serv_add_char(hs, hap_char_model_create(cfg->model)) != HAP_SUCCESS) {
        goto err;
    }

    if (hap_serv_add_char(hs, hap_char_manufacturer_create(cfg->manufacturer)) != HAP_SUCCESS) {
        goto err;
    }

    if (hap_serv_add_char(hs, hap_char_serial_number_create(cfg->serial_num)) != HAP_SUCCESS) {
        goto err;
    }

    if (hap_serv_add_char(hs, hap_char_firmware_revision_create(cfg->fw_rev)) != HAP_SUCCESS) {
        goto err;
    }

    if (hap_serv_add_char(hs, hap_char_identify_create()) != HAP_SUCCESS) {
        goto err;
    }

    if (cfg->hw_rev) {
        if (hap_serv_add_char(hs, hap_char_hardware_revision_create(cfg->hw_rev)) != HAP_SUCCESS) {
            goto err;
        }
    }

    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_protocol_information_create(char *version)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_PROTOCOL_INFORMATION);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_version_create(version)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_fan_create(bool on)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_FAN);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_on_create(on)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_garage_door_opener_create(uint8_t curr_door_state, uint8_t targ_door_state, bool obstr_detect)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_GARAGE_DOOR_OPENER);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_door_state_create(curr_door_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_door_state_create(targ_door_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_obstruction_detect_create(obstr_detect)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_lightbulb_create(bool on)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_LIGHTBULB);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_on_create(on)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_lock_management_create(hap_tlv8_val_t *lock_control_point, char * version)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_LOCK_MANAGEMENT);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_lock_control_point_create(lock_control_point)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_version_create(version)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_lock_mechanism_create(uint8_t lock_curr_state, uint8_t lock_targ_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_LOCK_MECHANISM);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_lock_current_state_create(lock_curr_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_lock_target_state_create(lock_targ_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_outlet_create(bool on, bool outlet_in_use)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_OUTLET);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_on_create(on)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_outlet_in_use_create(outlet_in_use)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_switch_create(bool on)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_SWITCH);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_on_create(on)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_thermostat_create(uint8_t curr_heating_cooling_state, uint8_t targ_heating_cooling_state, float curr_temp,
                                       float targ_temp, uint8_t temp_disp_units)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_THERMOSTAT);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_heating_cooling_state_create(curr_heating_cooling_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_heating_cooling_state_create(targ_heating_cooling_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_temperature_create(curr_temp)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_temperature_create(targ_temp)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_temperature_display_units_create(temp_disp_units)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_air_quality_sensor_create(uint8_t air_quality)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_AIR_QUALITY_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_air_quality_create(air_quality)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_security_system_create(uint8_t security_sys_curr_state, uint8_t security_sys_targ_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_SECURITY_SYSTEM);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_security_system_current_state_create(security_sys_curr_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_security_system_target_state_create(security_sys_targ_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_carbon_monoxide_sensor_create(uint8_t carbon_monoxide_detected)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_CARBON_MONOXIDE_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_carbon_monoxide_detected_create(carbon_monoxide_detected)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_contact_sensor_create(uint8_t contact_sensor_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_CONTACT_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_contact_sensor_state_create(contact_sensor_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_door_create(uint8_t curr_pos, uint8_t targ_pos, uint8_t pos_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_DOOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_position_create(curr_pos)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_position_create(targ_pos)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_position_state_create(pos_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_humidity_sensor_create(float curr_rel_humidity)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_HUMIDITY_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_relative_humidity_create(curr_rel_humidity)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_leak_sensor_create(uint8_t leak_detected)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_LEAK_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_leak_detected_create(leak_detected)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_light_sensor_create(float curr_ambient_light_level)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_LIGHT_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_ambient_light_level_create(curr_ambient_light_level)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_motion_sensor_create(bool motion_detected)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_MOTION_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_motion_detected_create(motion_detected)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_occupancy_sensor_create(uint8_t occupancy_detected)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_OCCUPANCY_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_occupancy_detected_create(occupancy_detected)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_smoke_sensor_create(uint8_t smoke_detected)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_SMOKE_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_smoke_detected_create(smoke_detected)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_stateless_programmable_switch_create(uint8_t programmable_switch_event)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_STATLESS_PROGRAMMABLE_SWITCH);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_programmable_switch_event_create(programmable_switch_event)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_wattage_create(float curr_watts)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_LIGHT_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_wattage_create(curr_watts)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}


hap_serv_t *hap_serv_temperature_sensor_create(float curr_temp)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_TEMPERATURE_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_temperature_create(curr_temp)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_window_create(uint8_t curr_pos, uint8_t targ_pos, uint8_t pos_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_WINDOW);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_position_create(curr_pos)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_position_create(targ_pos)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_position_state_create(pos_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_window_covering_create(uint8_t targ_pos, uint8_t curr_pos, uint8_t pos_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_WINDOW_COVERING);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_target_position_create(targ_pos)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_position_create(curr_pos)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_position_state_create(pos_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_battery_service_create(uint8_t battery_level, uint8_t charging_state, uint8_t status_low_battery)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_BATTERY_SERVICE);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_battery_level_create(battery_level)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_charging_state_create(charging_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_status_low_battery_create(status_low_battery)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_carbon_dioxide_sensor_create(uint8_t carbon_dioxide_detected)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_CARBON_DIOXIDE_SENSOR);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_carbon_dioxide_detected_create(carbon_dioxide_detected)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_fan_v2_create(uint8_t active)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_FAN_V2);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_slat_create(uint8_t curr_slat_state, uint8_t slat_type)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_SLAT);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_current_slat_state_create(curr_slat_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_slat_type_create(slat_type)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_filter_maintenance_create(uint8_t filter_change_indication)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_FILTER_MAINTENANCE);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_filter_change_indication_create(filter_change_indication)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_air_purifier_create(uint8_t active, uint8_t curr_air_purifier_state, uint8_t
        targ_air_purifier_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_AIR_PURIFIER);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_air_purifier_state_create(curr_air_purifier_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_air_purifier_state_create(targ_air_purifier_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_heater_cooler_create(uint8_t active, float curr_temp, uint8_t curr_heater_cooler_state, uint8_t targ_heater_cooler_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_HEATER_COOLER);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_temperature_create(curr_temp)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_heater_cooler_state_create(curr_heater_cooler_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_heater_cooler_state_create(targ_heater_cooler_state)) != HAP_SUCCESS) {
        goto err;
    }

    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_humidifier_dehumidifier_create(uint8_t active, float curr_rel_humidity, uint8_t
        curr_humid_dehumid_state, uint8_t targ_humid_dehumid_state)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_HUMIDIFIER_DEHUMIDIFIER);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_relative_humidity_create(curr_rel_humidity)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_current_humidifier_dehumidifier_state_create(curr_humid_dehumid_state)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_target_humidifier_dehumidifier_state_create(targ_humid_dehumid_state)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_service_label_create(uint8_t service_label_namespace)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_SERVICE_LABEL);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_service_label_namespace_create(service_label_namespace)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_irrigation_system_create(uint8_t active, uint8_t prog_mode, uint8_t in_use)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_IRRIGATION_SYSTEM);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_program_mode_create(prog_mode)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_in_use_create(in_use)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_valve_create(uint8_t active, uint8_t in_use, uint8_t valve_type)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_VALVE);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_in_use_create(in_use)) != HAP_SUCCESS) {
        goto err;
    }
    if (hap_serv_add_char(hs, hap_char_valve_type_create(valve_type)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}

hap_serv_t *hap_serv_faucet_create(uint8_t active)
{
    hap_serv_t *hs = hap_serv_create(HAP_SERV_UUID_FAUCET);
    if (!hs) {
        return NULL;
    }
    if (hap_serv_add_char(hs, hap_char_active_create(active)) != HAP_SUCCESS) {
        goto err;
    }
    return hs;
err:
    hap_serv_delete(hs);
    return NULL;
}
