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
/** HAP Apple Services
 *
 * This offers helper APIs for all the standard HomeKit Services defined by Apple
 */
#ifndef _HAP_APPLE_SERVS_H_
#define _HAP_APPLE_SERVS_H_

#include <stdint.h>
#include <stdbool.h>

#include <hap.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HAP_SERV_UUID_ACCESSORY_INFORMATION         "3E"
#define HAP_SERV_UUID_PROTOCOL_INFORMATION          "A2"
#define HAP_SERV_UUID_FAN                           "40"
#define HAP_SERV_UUID_GARAGE_DOOR_OPENER            "41"
#define HAP_SERV_UUID_LIGHTBULB                     "43"
#define HAP_SERV_UUID_LOCK_MANAGEMENT               "44"
#define HAP_SERV_UUID_LOCK_MECHANISM                "45"
#define HAP_SERV_UUID_SWITCH                        "49"
#define HAP_SERV_UUID_OUTLET                        "47"
#define HAP_SERV_UUID_THERMOSTAT                    "4A"
#define HAP_SERV_UUID_AIR_QUALITY_SENSOR            "8D"
#define HAP_SERV_UUID_SECURITY_SYSTEM               "7E"
#define HAP_SERV_UUID_CARBON_MONOXIDE_SENSOR        "7F"
#define HAP_SERV_UUID_CONTACT_SENSOR                "80"
#define HAP_SERV_UUID_DOOR                          "81"
#define HAP_SERV_UUID_HUMIDITY_SENSOR               "82"
#define HAP_SERV_UUID_LEAK_SENSOR                   "83"
#define HAP_SERV_UUID_LIGHT_SENSOR                  "84"
#define HAP_SERV_UUID_MOTION_SENSOR                 "85"
#define HAP_SERV_UUID_OCCUPANCY_SENSOR              "86"
#define HAP_SERV_UUID_SMOKE_SENSOR                  "87"
#define HAP_SERV_UUID_STATLESS_PROGRAMMABLE_SWITCH  "89"
#define HAP_SERV_UUID_TEMPERATURE_SENSOR            "8A"
#define HAP_SERV_UUID_WINDOW                        "8B"
#define HAP_SERV_UUID_WINDOW_COVERING               "8C"
#define HAP_SERV_UUID_BATTERY_SERVICE               "96"
#define HAP_SERV_UUID_CARBON_DIOXIDE_SENSOR         "97"
#define HAP_SERV_UUID_FAN_V2                        "B7"
#define HAP_SERV_UUID_SLAT                          "B9"
#define HAP_SERV_UUID_FILTER_MAINTENANCE            "BA"
#define HAP_SERV_UUID_AIR_PURIFIER                  "BB"
#define HAP_SERV_UUID_HEATER_COOLER                 "BC"
#define HAP_SERV_UUID_HUMIDIFIER_DEHUMIDIFIER       "BD"
#define HAP_SERV_UUID_SERVICE_LABEL                 "CC"
#define HAP_SERV_UUID_IRRIGATION_SYSTEM             "CF"
#define HAP_SERV_UUID_VALVE                         "D0"
#define HAP_SERV_UUID_FAUCET                        "D7"


/** Create Accessory Information Service
 *
 * This API will create the Accessory Information Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] cfg The accessory configuration to be used to create the service
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_accessory_information_create(hap_acc_cfg_t *cfg);

/** Create Protocol Information Service
 *
 * This API will create the Protocol Information Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] version The Protocol Version string
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_protocol_information_create(char *version);

/** Create Fan Service
 *
 * This API will create the Fan Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] on Initial "On" state of the service
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_fan_create(bool on);

/** Create Garage Door Opener Service
 *
 * This API will create the Garage Door Opener Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] curr_door_state Initial value of the current door state characteristic
 * @param[in] targ_door_state Value of the target door state characteristic
 * @param[in] obstr_detect Value of the obstruction detected characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_garage_door_opener_create(uint8_t curr_door_state, uint8_t targ_door_state, bool obstr_detect)
;

/** Create Light Bulb Service
 *
 * This API will create the Light Bulb Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] on Initial "On" state of the service
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_lightbulb_create(bool on);

/** Create Lock Management Service
 *
 * This API will create the Lock Management Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] lock_control_point Accepts data from TLV8 commands
 * @param[in] version The Protocol Version string
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_lock_management_create(hap_tlv8_val_t *lock_control_point, char * version);

/** Create Lock Mechanism Service
 *
 * This API will create the Lock Mechanism Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] lock_curr_state Current lock state of the service
 * @param[in] lock_targ_state Target lock state of the service
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_lock_mechanism_create(uint8_t lock_curr_state, uint8_t lock_targ_state);

/** Create Outlet Service
 *
 * This API will create the Outlet Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] on Initial "On" state of the service
 * @param[in] outlet_in_use Initial value of the outlet in use characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_outlet_create(bool on, bool outlet_in_use);

/** Create Switch Service
 *
 * This API will create the Switch Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] on Initial "On" state of the service
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_switch_create(bool on);

/** Create Thermostat Service
 *
 * This API will create the Thermostat Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] curr_heating_cooling_state Initial value of Current Heating Cooling State characteristic
 * @param[in] targ_heating_cooling_state Initial value of Target Heating Cooling State characteristic
 * @param[in] curr_temp Initial value of Current Temperature characteristic
 * @param[in] targ_temp Initial value of Target Temperature  characteristic
 * @param[in] temp_disp_units Initial value of Temperature Display Units characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_thermostat_create(uint8_t curr_heating_cooling_state, uint8_t targ_heating_cooling_state, float curr_temp, float targ_temp, uint8_t temp_disp_units);

/** Create Air Quality Sensor Service
 *
 * This API will create the Air Quality Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] air_quality Initial value of Air Quality characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_air_quality_sensor_create(uint8_t air_quality);

/** Create Security System Current State Service
 *
 * This API will create the Security System Current State Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] security_sys_curr_state Initial value of Security System Current State characteristic
 * @param[in] security_sys_targ_state Initial value of Security System Target State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_security_system_create(uint8_t security_sys_curr_state, uint8_t security_sys_targ_state);

/** Create Carbon Monoxide Sensor Service
 *
 * This API will create the Carbon Monoxide Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] carbon_monoxide_detected Initial value of Carbon Monoxide Sensor characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_carbon_monoxide_sensor_create(uint8_t carbon_monoxide_detected);

/** Create Contact Sensor Service
 *
 * This API will create the Contact Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] contact_sensor_state Initial value of Contact Sensor State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_contact_sensor_create(uint8_t contact_sensor_state);

/** Create Door Service
 *
 * This API will create the Door Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  curr_pos   Initial value of Current Position characteristic
 * @param[in]  targ_pos   Initial value of Target Position characteristic
 * @param[in]  pos_state  Initial value of Position State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_door_create(uint8_t curr_pos, uint8_t targ_pos, uint8_t pos_state);

/** Create Humidity Sensor Service
 *
 * This API will create the Humidity Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] curr_relative_humidity   Initial value of Humidity Sensor State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_humidity_sensor_create(float curr_relative_humidity);

/** Leak Sensor Service
 *
 * This API will create the Leak Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] leak_detected   Initial value of Leak Detected State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_leak_sensor_create(uint8_t leak_detected);

/** Light Sensor Service
 *
 * This API will create the Light Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] curr_ambient_light_level   Initial value of Current Ambient Light Level characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_light_sensor_create(float curr_ambient_light_level);

/** Motion Sensor Service
 *
 * This API will create the Motion Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  motion_detected   Initial value of Motion Detected characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_motion_sensor_create(bool motion_detected);

/** Occupancy Sensor Service
 *
 * This API will create the Occupancy Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  occupancy_detected   Initial value of Occupancy Detected characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_occupancy_sensor_create(uint8_t occupancy_detected);

/** Smoke Sensor Service
 *
 * This API will create the Smoke Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  smoke_detected  Initial value of Smoke Detected characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_smoke_sensor_create(uint8_t smoke_detected);

/** Stateless Programmable Switch Service
 *
 * This API will create the Stateless Programmable Switch Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  programmable_switch_event  Initial value of Programmable Switch Event characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_stateless_programmable_switch_create(uint8_t programmable_switch_event);

/** Temperature Sensor Service
 *
 * This API will create the Temperature Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  curr_temp  Initial value of Current Temprature characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_temperature_sensor_create(float curr_temp);

/** Window Service
 *
 * This API will create the Window Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  curr_pos   Initial value of Current Position characteristic
 * @param[in]  targ_pos   Initial value of Target Position characteristic
 * @param[in]  pos_state  Initial value of Position State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_window_create(uint8_t curr_pos, uint8_t targ_pos, uint8_t pos_state);

/** Window Covering Service
 *
 * This API will create the Window Covering  Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  targ_pos   Initial value of Target Position characteristic
 * @param[in]  curr_pos   Initial value of Current Position characteristic
 * @param[in]  pos_state  Initial value of Position State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_window_covering_create(uint8_t targ_pos, uint8_t curr_pos, uint8_t pos_state);

/** Battery Service
 *
 * This API will create the Battery Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in]  battery_level       Initial value of Battery Level characteristic
 * @param[in]  charging_state      Initial value of Charging State characteristic
 * @param[in]  status_low_battery  Initial value of Status Low Battery characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_battery_service_create(uint8_t battery_level, uint8_t charging_state, uint8_t status_low_battery);

/** Create Carbon Dioxide Sensor Service
 *
 * This API will create the Carbon Dioxide Sensor Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] carbon_dioxide_detected Initial value of Carbon Dioxide Sensor characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_carbon_dioxide_sensor_create(uint8_t carbon_dioxide_detected);

/** Fan v2 Service
 *
 * This API will create the Fan v2 Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active   Value of Active characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_fan_v2_create(uint8_t active);

/** Slat Service
 *
 * This API will create the Slat Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] curr_slat_state   Value of Current Slat State characteristic
 * @param[in] slat_type   Value of Slat Type characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_slat_create(uint8_t curr_slat_state, uint8_t slat_type);

/** Filter Maintenance Service
 *
 * This API will create the Filter Maintenance Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] filter_change_indication  Value of Filter Change Indication characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_filter_maintenance_create(uint8_t filter_change_indication);

/** Air Purifier Service
 *
 * This API will create the Air Purifier Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active Value of Active characteristic
 * @param[in] curr_air_purifier_state Value of Current Air Purifier State characteristic
 * @param[in] targ_air_purifier_state Value of Target Air Purifier State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_air_purifier_create(uint8_t active, uint8_t curr_air_purifier_state, uint8_t
        targ_air_purifier_state);

/** Heater Cooler Service
 *
 * This API will create the Heater Cooler Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active Value of Active characteristic
 * @param[in] curr_temp Value of Current Temperature characteristic
 * @param[in] curr_heater_cooler_state Value of Current Heater Cooler State characteristic
 * @param[in] targ_heater_cooler_state Value of Target Heater Cooler State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_heater_cooler_create(uint8_t active, float curr_temp, uint8_t curr_heater_cooler_state, uint8_t targ_heater_cooler_state);

/** Create Humidifer Dehumidfier Service
 *
 * This API will create the Humidifer Dehumidfier Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active  Initial value of Active characteristic
 * @param[in] curr_rel_humid  Initial value of Current Relative Humidity characteristic
 * @param[in] curr_humid_dehumid_state  Initial value of Current Humidifier Dehumidifier State characteristic
 * @param[in] targ_humid_dehumid_state  Initial value of Target Humidifier Dehumidifier State characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_humidifier_dehumidifier_create(uint8_t active, float curr_rel_humid, uint8_t
        curr_humid_dehumid_state, uint8_t targ_humid_dehumid_state);

/** Create Service Label Service
 *
 * This API will create the Service Label Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] service_label_namespace Initial value of Service Label Service characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_service_label_create(uint8_t service_label_namespace);

/** Create Irrigation System Service
 *
 * This API will create the Irrigation System Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active Initial value of active characteristic
 * @param[in] prog_mode Initial value of Program Mode characteristic
 * @param[in] in_use Initial value of In Use characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_irrigation_system_create(uint8_t active, uint8_t prog_mode, uint8_t in_use);

/** Create Valve Service
 *
 * This API will create the Valve Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active      Initial value of active characteristic
 * @param[in] in_use      Initial value of In Use characteristic
 * @param[in] valve_type  Initial value of Valve Type characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_valve_create(uint8_t active, uint8_t in_use, uint8_t valve_type);

/** Create Faucet Service
 *
 * This API will create the Faucet Service with the mandatory
 * characteristics as per the HAP Specs.
 *
 * @param[in] active Initial value of active characteristic
 *
 * @return Pointer to the service object on success
 * @return NULL on failure
 */
hap_serv_t *hap_serv_faucet_create(uint8_t active);


hap_serv_t *hap_serv_wattage_create(float curr_watts);

#ifdef __cplusplus
}
#endif

#endif /* _HAP_APPLE_SERVS_H_ */
