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
/** HAP Apple Characteristics
 *
 * This offers helper APIs for all the standard HomeKit Characteristics defined by Apple
 */
#ifndef _HAP_APPLE_CHARS_H_
#define _HAP_APPLE_CHARS_H_

#include <stdint.h>
#include <stdbool.h>

#include <hap.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HAP_CHAR_UUID_ADMINISTRATOR_ONLY_ACCESS                 "1"
#define HAP_CHAR_UUID_BRIGHTNESS                                "8"
#define HAP_CHAR_UUID_COOLING_THRESHOLD_TEMPERATURE             "D"
#define HAP_CHAR_UUID_CURRENT_DOOR_STATE                        "E"
#define HAP_CHAR_UUID_CURRENT_HEATING_COOLING_STATE             "F"
#define HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY                 "10"
#define HAP_CHAR_UUID_CURRENT_TEMPERATURE                       "11"
#define HAP_CHAR_UUID_FIRMWARE_REVISION                         "52"
#define HAP_CHAR_UUID_HARDWARE_REVISION                         "53"
#define HAP_CHAR_UUID_HEATING_THRESHOLD_TEMPERATURE             "12"
#define HAP_CHAR_UUID_HUE                                       "13"
#define HAP_CHAR_UUID_IDENTIFY                                  "14"
#define HAP_CHAR_UUID_LOCK_CONTROL_POINT                        "19"
#define HAP_CHAR_UUID_LOCK_CURRENT_STATE                        "1D"
#define HAP_CHAR_UUID_LOCK_LAST_KNOWN_ACTION                    "1C"
#define HAP_CHAR_UUID_LOCK_MANAGEMENT_AUTO_SECURITY_TIMEOUT     "1A"
#define HAP_CHAR_UUID_LOCK_TARGET_STATE                         "1E"
#define HAP_CHAR_UUID_LOGS                                      "1F"
#define HAP_CHAR_UUID_MANUFACTURER                              "20"
#define HAP_CHAR_UUID_MODEL                                     "21"
#define HAP_CHAR_UUID_MOTION_DETECTED                           "22"
#define HAP_CHAR_UUID_NAME                                      "23"
#define HAP_CHAR_UUID_OBSTRUCTION_DETECTED                      "24"
#define HAP_CHAR_UUID_ON                                        "25"
#define HAP_CHAR_UUID_OUTLET_IN_USE                             "26"
#define HAP_CHAR_UUID_ROTATION_DIRECTION                        "28"
#define HAP_CHAR_UUID_ROTATION_SPEED                            "29"
#define HAP_CHAR_UUID_SATURATION                                "2F"
#define HAP_CHAR_UUID_SERIAL_NUMBER                             "30"
#define HAP_CHAR_UUID_TARGET_DOOR_STATE                         "32"
#define HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE              "33"
#define HAP_CHAR_UUID_TARGET_RELATIVE_HUMIDITY                  "34"
#define HAP_CHAR_UUID_TARGET_TEMPERATURE                        "35"
#define HAP_CHAR_UUID_TEMPERATURE_DISPLAY_UNITS                 "36"
#define HAP_CHAR_UUID_VERSION                                   "37"
#define HAP_CHAR_UUID_AIR_PARTICULATE_DENSITY                   "64"
#define HAP_CHAR_UUID_AIR_PARTICULATE_SIZE                      "65"
#define HAP_CHAR_UUID_SECURITY_SYSTEM_CURRENT_STATE             "66"
#define HAP_CHAR_UUID_SECURITY_SYSTEM_TARGET_STATE              "67"
#define HAP_CHAR_UUID_BATTERY_LEVEL                             "68"
#define HAP_CHAR_UUID_CARBON_MONOXIDE_DETECTED                  "69"
#define HAP_CHAR_UUID_CONTACT_SENSOR_STATE                      "6A"
#define HAP_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL               "6B"
#define HAP_CHAR_UUID_CURRENT_HORIZONTAL_TILT_ANGLE             "6C"
#define HAP_CHAR_UUID_CURRENT_POSITION                          "6D"
#define HAP_CHAR_UUID_CURRENT_VERTICAL_TILT_ANGLE               "6E"
#define HAP_CHAR_UUID_HOLD_POSITION                             "6F"
#define HAP_CHAR_UUID_LEAK_DETECTED                             "70"
#define HAP_CHAR_UUID_OCCUPANCY_DETECTED                        "71"
#define HAP_CHAR_UUID_POSITION_STATE                            "72"
#define HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT                 "73"
#define HAP_CHAR_UUID_STATUS_ACTIVE                             "75"
#define HAP_CHAR_UUID_SMOKE_DETECTED                            "76"
#define HAP_CHAR_UUID_STATUS_FAULT                              "77"
#define HAP_CHAR_UUID_STATUS_JAMMED                             "78"
#define HAP_CHAR_UUID_STATUS_LOW_BATTERY                        "79"
#define HAP_CHAR_UUID_STATUS_TAMPERED                           "7A"
#define HAP_CHAR_UUID_TARGET_HORIZONTAL_TILT_ANGLE              "7B"
#define HAP_CHAR_UUID_TARGET_POSITION                           "7C"
#define HAP_CHAR_UUID_TARGET_VERTICAL_TILT_ANGLE                "7D"
#define HAP_CHAR_UUID_STATUS_SECURITY_SYSTEM_ALARM_TYPE         "8E"
#define HAP_CHAR_UUID_CHARGING_STATE                            "8F"
#define HAP_CHAR_UUID_CARBON_MONOXIDE_LEVEL                     "90"
#define HAP_CHAR_UUID_CARBON_MONOXIDE_PEAK_LEVEL                "91"
#define HAP_CHAR_UUID_CARBON_DIOXIDE_DETECTED                   "92"
#define HAP_CHAR_UUID_CARBON_DIOXIDE_LEVEL                      "93"
#define HAP_CHAR_UUID_CARBON_DIOXIDE_PEAK_LEVEL                 "94"
#define HAP_CHAR_UUID_AIR_QUALITY                               "95"
#define HAP_CHAR_UUID_ACCESSORY_FLAGS                           "A6"
#define HAP_CHAR_UUID_LOCK_PHYSICAL_CONTROLS                    "A7"
#define HAP_CHAR_UUID_CURRENT_AIR_PURIFIER_STATE                "A9"
#define HAP_CHAR_UUID_CURRENT_SLAT_STATE                        "AA"
#define HAP_CHAR_UUID_SLAT_TYPE                                 "C0"
#define HAP_CHAR_UUID_FILTER_LIFE_LEVEL                         "AB"
#define HAP_CHAR_UUID_FILTER_CHANGE_INDICATION                  "AC"
#define HAP_CHAR_UUID_RESET_FILTER_INDICATION                   "AD"
#define HAP_CHAR_UUID_TARGET_AIR_PURIFIER_STATE                 "A8"
#define HAP_CHAR_UUID_TARGET_FAN_STATE                          "BF"
#define HAP_CHAR_UUID_CURRENT_FAN_STATE                         "AF"
#define HAP_CHAR_UUID_ACTIVE                                    "B0"
#define HAP_CHAR_UUID_SWING_MODE                                "B6"
#define HAP_CHAR_UUID_CURRENT_TILT_ANGLE                        "C1"
#define HAP_CHAR_UUID_TARGET_TILT_ANGLE                         "C2"
#define HAP_CHAR_UUID_OZONE_DENSITY                             "C3"
#define HAP_CHAR_UUID_NITROGEN_DIOXIDE_DENSITY                  "C4"
#define HAP_CHAR_UUID_SULPHUR_DIOXIDE_DENSITY                   "C5"
#define HAP_CHAR_UUID_PM_2_5_DENSITY                            "C6"
#define HAP_CHAR_UUID_PM_10_DENSITY                             "C7"
#define HAP_CHAR_UUID_VOC_DENSITY                               "C8"
#define HAP_CHAR_UUID_SERVICE_LABEL_INDEX                       "CB"
#define HAP_CHAR_UUID_SERVICE_LABEL_NAMESPACE                   "CD"
#define HAP_CHAR_UUID_COLOR_TEMPERATURE                         "CE"
#define HAP_CHAR_UUID_CURRENT_HEATER_COOLER_STATE               "B1"
#define HAP_CHAR_UUID_TARGET_HEATER_COOLER_STATE                "B2"
#define HAP_CHAR_UUID_CURRENT_HUMIDIFIER_DEHUMIDIFIER_STATE     "B3"
#define HAP_CHAR_UUID_TARGET_HUMIDIFIER_DEHUMIDIFIER_STATE      "B4"
#define HAP_CHAR_UUID_WATER_LEVEL                               "B5"
#define HAP_CHAR_UUID_RELATIVE_HUMIDITY_DEHUMIDIFIER_THRESHOLD  "C9"
#define HAP_CHAR_UUID_RELATIVE_HUMIDITY_HUMIDIFIER_THRESHOLD    "CA"
#define HAP_CHAR_UUID_PROGRAM_MODE                              "D1"
#define HAP_CHAR_UUID_IN_USE                                    "D2"
#define HAP_CHAR_UUID_SET_DURATION                              "D3"
#define HAP_CHAR_UUID_REMAINING_DURATION                        "D4"
#define HAP_CHAR_UUID_VALVE_TYPE                                "D5"
#define HAP_CHAR_UUID_IS_CONFIGURED                             "D6"
#define HAP_CHAR_UUID_WATTAGE                                   "DC"
#define HAP_CHAR_UUID_PRODUCT_DATA                              "220"

/** Create Brightness Characteristic
 *
 * This API creates the Brightness characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] brightness Initial value of brightness
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_brightness_create(int brightness);

/** Cooling Threshold Temperature Characteristic
 *
 * This API creates the Cooling Threshold Temperature characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] cooling_threshold_temp Cooling Threshold Value of Cooling Threshold Temperature characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_cooling_threshold_temperature_create(float cooling_threshold_temp);

/** Create Current Door State Characteristic
 *
 * This API creates the Current Door State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_door_state Initial value of Current Door State
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_door_state_create(uint8_t curr_door_state);

/** Create Current Heating Cooling State Characteristic
 *
 * This API creates the Current Heating Cooling State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_heating_cooling_state Initial value of current heating cooling state characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_heating_cooling_state_create(uint8_t curr_heating_cooling_state);

/** Current Relative Humidity Characteristic
 *
 * This API creates the Current Relative Humidity characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_rel_humidity Current Relative Humidity Value
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_relative_humidity_create(float curr_rel_humidity);

/** Current Temperature Characteristic
 *
 * This API creates the Current Temperature characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_temp Initial value of current temperature characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_temperature_create(float curr_temp);

/** Create Firmware Revision Characteristic
 *
 * This API creates the firmware revision characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] fw_rev Firmware Revision string
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_firmware_revision_create(const char *fw_rev);

/** Create Hardware Revision Characteristic
 *
 * This API creates the name characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] hw_rev Hardware Revision string
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_hardware_revision_create(const char *hw_rev);

/** Heating Threshold Temperature Characteristic
 *
 * This API creates the Heating Threshold Temperature characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] heating_threshold_temp Heating Threshold Value of Heating Threshold Temperature characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_heating_threshold_temperature_create(float heating_threshold_temp);

/** Create Hue Characteristic
 *
 * This API creates the hue characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] hue Initial value of hue
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_hue_create(float hue);

/** Create Identify Characteristic
 *
 * This API creates the Identify characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_identify_create(void);

/** Lock Current State Characteristic
 *
 * This API creates the Lock Current State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] lock_curr_state  Current lock state value of characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_lock_current_state_create(uint8_t lock_curr_state);

/** Lock Target State Characteristic
 *
 * This API creates the Lock Target State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] lock_targ_state  Target lock state value of characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_lock_target_state_create(uint8_t lock_targ_state);

/** Create Manufacturer Characteristic
 *
 * This API creates the Manufacturer characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] manufacturer Manufacturer string
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_manufacturer_create(const char *manufacturer);

/** Create Model Characteristic
 *
 * This API creates the Model characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] model Model string
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_model_create(const char *model);

/** Create Motion Detected Characteristic
 *
 * This API creates the Motion Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] motion_detected   Value of Motion Detected Charateristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_motion_detected_create(bool motion_detected);

/** Create Name Characteristic
 *
 * This API creates the Name characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] name Name string
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_name_create(const char *name);

/** Create Obstruction Detected Characteristic
 *
 * This API creates the Obstruction Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] obstr_detect Initial value of obstruction detected characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_obstruction_detect_create(bool obstr_detect);

/** Create On Characteristic
 *
 * This API creates the On characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] on Initial value of on
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_on_create(bool on);

/** Create Outlet in Use Characteristic
 *
 * This API creates the Outlet in Use characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] outlet_in_use Initial value of outlet in use
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_outlet_in_use_create(bool outlet_in_use);

/** Create Rotation Direction Characteristic
 *
 * This API creates the Rotation Direction characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] rotation_direction Initial value of Rotation Direction
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_rotation_direction_create(int rotation_direction);

/** Create Rotation Speed Characteristic
 *
 * This API creates the Rotation Speed characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] rotation_speed Initial value of Rotation Speed
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_rotation_speed_create(float rotation_speed);

/** Create Saturation Characteristic
 *
 * This API creates the saturation characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] saturation Initial value of saturation
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_saturation_create(float saturation);

/** Create Serial Number Characteristic
 *
 * This API creates the serial number characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] serial_num Serial Number string
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_serial_number_create(const char *serial_num);

/** Create Target Door State Characteristic
 *
 * This API creates the Target Door State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_door_state Initial value of Target Door State
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_door_state_create(uint8_t targ_door_state);

/** Create Target Heating Cooling State Characteristic
 *
 * This API creates the Target Heating Cooling State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_heating_cooling_state Value of target heating cooling state characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_heating_cooling_state_create(uint8_t targ_heating_cooling_state);

/** Target Relative Humidity Characteristic
 *
 * This API creates the Target Relative Humidity characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_rel_humidity Target Relative Humidity Value
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_relative_humidity_create(float targ_rel_humidity);

/** Target Temperature Characteristic
 *
 * This API creates the Target Temperature characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_temp Target value of target temperature characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_temperature_create(float targ_temp);

/** Temperature Display Units Characteristic
 *
 * This API creates the Temperature Display Units characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] temp_disp_units Initial value of Temperature Display Units characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_temperature_display_units_create(uint8_t temp_disp_units);

/** Create Version Characteristic
 *
 * This API creates the version characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] version Version String
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_version_create(const char *version);

/** Create Security System Current State Characteristic
 *
 * This API creates the Security System Current State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] security_sys_curr_state  Initial value of Security System Current State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_security_system_current_state_create(uint8_t security_sys_curr_state);

/** Create Security System Target State Characteristic
 *
 * This API creates the Security System Target State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] security_sys_targ_state  Initial value of Security System Target State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_security_system_target_state_create(uint8_t security_sys_targ_state);

/** Create Battery Level Characteristic
 *
 * This API creates the Battery Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] battery_level  Initial value of Battery Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_battery_level_create(uint8_t battery_level);

/** Current Position Characteristic
 *
 * This API creates the Current Position characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_pos  Initial value of Cuurent Position characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_position_create(uint8_t curr_pos);

/** Current Vertical Tilt Angle Characteristic
 *
 * This API creates the Current Vertical Tilt Angle characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  curr_vert_tilt_angle Initial value of Current Vertical Tilt Angle characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_vertical_tilt_angle_create(int curr_vert_tilt_angle);

/** Hold Position Characteristic
 *
 * This API creates the Hold Position characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] hold_pos  Initial value of Hold Position characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_hold_position_create(bool hold_pos);

/** Leak Detected Characteristic
 *
 * This API creates the Leak Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] leak_detected  Initial value of Leak Detected characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_leak_detected_create(uint8_t leak_detected);

/** Occupancy Detected Characteristic
 *
 * This API creates the Occupancy Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] occupancy_detected  Initial value of Occupancy Detected characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_occupancy_detected_create(uint8_t occupancy_detected);

/** Status Active Characteristic
 *
 * This API creates the Status Active characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] status_active  Initial value of Status Active characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_status_active_create(bool status_active);

/** Smoke Detected Characteristic
 *
 * This API creates the Smoke Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] smoke_detected  Initial value of Smoke Detected characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_smoke_detected_create(uint8_t smoke_detected);

/** Status Fault Characteristic
 *
 * This API creates the Status Fault characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] status_fault  Initial value of Status Fault characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_status_fault_create(uint8_t status_fault);

/** Status Low Battery Characteristic
 *
 * This API creates the Status Low Battery characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] status_low_battery  Initial value of Status Low Battery characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_status_low_battery_create(uint8_t status_low_battery);

/** Status Tampered Characteristic
 *
 * This API creates the Status Tampered characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] status_tampered  Initial value of Status Tampered characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_status_tampered_create(uint8_t status_tampered);

/** Target Horizontal Tilt Angle Characteristic
 *
 * This API creates the Target Horizontal Tilt Angle characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  targ_horiz_tilt_angle     Initial value of Target Horizontal Tilt Angle characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_horizontal_tilt_angle_create(int targ_horiz_tilt_angle);

/** Target Position Characteristic
 *
 * This API creates the Target Position characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_pos  Initial value of Target Position characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_position_create(uint8_t targ_pos);

/** Target Vertical Tilt Angle Characteristic
 *
 * This API creates the Target Vertical Tilt Angle characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  targ_vert_tilt_angle  Initial value of Target Vertical Tilt Angle characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_vertical_tilt_angle_create(int targ_vert_tilt_angle);

/** Security System Alarm Type Characteristic
 *
 * This API creates the Security System Alarm Type characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] security_sys_alarm_type  Initial value of Security System Alarm Type characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_security_system_alarm_type_create(uint8_t security_sys_alarm_type);

/** Charging State Characteristic
 *
 * This API creates the Charging State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  charging_state   Charging State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_charging_state_create(uint8_t charging_state);

/** Carbon Monoxide Level Characteristic
 *
 * This API creates the Carbon Monoxide Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] carbon_monoxide_level  Initial value of Carbon Monoxide Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_carbon_monoxide_level_create(float carbon_monoxide_level);

/** Carbon Monoxide Peak Level Characteristic
 *
 * This API creates the Carbon Monoxide Peak Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] carbon_monoxide_peak_level  Initial value of Carbon Monoxide Peak Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_carbon_monoxide_peak_level_create(float carbon_monoxide_peak_level);

/** Carbon Dioxide Detected Characteristic
 *
 * This API creates the Carbon Dioxide Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] carbon_dioxide_detected  Initial value of Carbon Dioxide Detected characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_carbon_dioxide_detected_create(uint8_t carbon_dioxide_detected);

/** Carbon Dioxide Level Characteristic
 *
 * This API creates the Carbon Dioxide Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] carbon_dioxide_level  Initial value of Carbon Dioxide Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_carbon_dioxide_level_create(float carbon_dioxide_level);

/** Carbon Dioxide Peak Level Characteristic
 *
 * This API creates the Carbon Dioxide Peak Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] carbon_dioxide_peak_level  Initial value of Carbon Dioxide Peak Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_carbon_dioxide_peak_level_create(float carbon_dioxide_peak_level);

/** Carbon Monoxide Detected Characteristic
 *
 * This API creates the Carbon Monoxide Detected characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] carbon_monoxide_detected  Initial value of Carbon Monoxide Detected characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_carbon_monoxide_detected_create(uint8_t carbon_monoxide_detected);

/** Contact Sensor State Characteristic
 *
 * This API creates the Contact Sensor State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] contact_sensor_state  Initial value of Contact Sensor State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_contact_sensor_state_create(uint8_t contact_sensor_state);

/** Current Ambient Light Level Characteristic
 *
 * This API creates the Current Ambient Light Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  curr_ambient_light_level  Initial value of Current Ambient Light Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_ambient_light_level_create(float curr_ambient_light_level);

/** Current Horizontal Tilt Angle Characteristic
 *
 * This API creates the Current Horizontal Tilt Angle characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   curr_horiz_tilt_angle   Initial value of Current Horizontal Tilt Angle characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_horizontal_tilt_angle_create(int curr_horiz_tilt_angle);

/** Air Quality Characteristic
 *
 * This API creates the Air Quality characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] air_quality  Initial value of Air Quality characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_air_quality_create(uint8_t air_quality);

/** Create Accessory Flags Characteristic
 *
 * This API creates the Accessory Flags characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] flags Initial value of flags
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_accessory_flags_create(uint32_t flags);


/** Create Product Data Characteristic
 *
 * This API creates the Product Data characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] product_data Value of the product data
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_product_data_create(hap_data_val_t *product_data);

/** Create Lock Physical Controls Characteristic
 *
 * This API creates the Lock Physical Controls characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] lock_physical_controls Initial value of Lock Physical Controls characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_lock_physical_controls_create(uint8_t lock_physical_controls);

/** Current Air Purifier State Characteristic
 *
 * This API creates the Current Air Purifier State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   curr_air_purifier_state Initial value of Current Air Purifier State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_air_purifier_state_create(uint8_t curr_air_purifier_state);

/** Current Slat State Characteristic
 *
 * This API creates the Current Slat State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_slat_state  Initial value of Current Slat State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_slat_state_create(uint8_t curr_slat_state);

/** Slat Type Characteristic
 *
 * This API creates the Slat Type characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] slat_type  Value of Slat Type characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_slat_type_create(uint8_t slat_type);

/** Filter Life Level Characteristic
 *
 * This API creates the Filter Life Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  filter_life_level  Initial value of Filter Life Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_filter_life_level_create(float filter_life_level);

/** Current Filter Change Indication Characteristic
 *
 * This API creates the Filter Change Indication characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  filter_change_indication  Initial value of Filter Change Indication characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_filter_change_indication_create(uint8_t filter_change_indication);

/** Reset Filter Indication Characteristic
 *
 * This API creates the Reset Filter Indication characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  reset_filter_indication  Initial value of Reset Filter Indication characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_reset_filter_indication_create(uint8_t reset_filter_indication);

/** Target Air Purifier State Characteristic
 *
 * This API creates the Target Air Purifier State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   targ_air_purifier_state Initial value of Target Air Purifier State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_air_purifier_state_create(uint8_t targ_air_purifier_state);

/** Target Fan State Characteristic
 *
 * This API creates the Target Fan State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   targ_fan_state  Initial value of Target Fan State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_fan_state_create(uint8_t targ_fan_state);

/** Current Fan State Characteristic
 *
 * This API creates the Current Fan State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   curr_fan_state  Initial value of Current Fan State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_fan_state_create(uint8_t curr_fan_state);

/** Position State Characteristic
 *
 * This API creates the Position State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] pos_state  Initial value of Position State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_position_state_create(uint8_t pos_state);

/** Programmable Switch Event Characteristic
 *
 * This API creates the Programmable Switch Event characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] programmable_switch_event  Initial value of Programmable Switch Event characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_programmable_switch_event_create(uint8_t programmable_switch_event);

/** Active Characteristic
 *
 * This API creates the Active characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   active  Initial value of active characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_active_create(uint8_t active);

/** Swing Mode Characteristic
 *
 * This API creates the Swing Mode characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   swing_mode  Initial value of Swing Mode characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_swing_mode_create(uint8_t swing_mode);

/** Current Tilt Angle Characteristic
 *
 * This API creates the Current Tilt Angle characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   curr_tilt_angle Initial value of Current Tilt Angle characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_tilt_angle_create(int curr_tilt_angle);

/** Target Tilt Angle Characteristic
 *
 * This API creates the Target Tilt Angle characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]   targ_tilt_angle Initial value of Target Tilt Angle characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
 hap_char_t *hap_char_target_tilt_angle_create(int targ_tilt_angle);

/** Ozone Density Characteristic
 *
 * This API creates the Ozone Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  ozone_density     Initial value of Ozone Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_ozone_density_create(float ozone_density);

/** Nitrogen Dioxide Density Characteristic
 *
 * This API creates the Nitrogen Dioxide Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  nitrogen_dioxide_density  Initial value of Nitrogen Dioxide Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_nitrogen_dioxide_density_create(float nitrogen_dioxide_density);

/** Sulphur Dioxide Density Characteristic
 *
 * This API creates the Sulphur Dioxide Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  sulphur_dioxide_density   Initial value of Sulphur Dioxide Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_sulphur_dioxide_density_create(float sulphur_dioxide_density);

/** PM2.5 Density Characteristic
 *
 * This API creates the PM2.5 Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  pm_2_5_density    Initial value of PM2.5 Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_pm_2_5_density_create(float pm_2_5_density);

/** PM10 Density Characteristic
 *
 * This API creates the PM10 Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  pm_10_density   Initial value of PM10 Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_pm_10_density_create(float pm_10_density);

/** VOC Density Characteristic
 *
 * This API creates the VOC Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in]  voc_density   Initial value of VOC Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_voc_density_create(float voc_density);

/** Create Service Label Index Characteristic
 *
 * This API creates the Service Label Index characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] service_label_index Initial value of Service Label Index characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_service_label_index_create(uint8_t service_label_index);

/** Create Service Label Namespace Characteristic
 *
 * This API creates the Service Label Namespace characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] service_label_namespace Initial value of Service Label Namespace characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_service_label_namespace_create(uint8_t service_label_namespace);

/** Create Color Temperature Characteristic
 *
 * This API creates the color temperature characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] color_temp Initial value of Color Temperature
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_color_temperature_create(uint32_t color_temp);

/** Create Curr Heater Cooler State Characteristic
 *
 * This API creates the Curr Heater Cooler State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_heater_cooler_state  Initial value of Curr Heater Cooler State
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_heater_cooler_state_create(uint8_t curr_heater_cooler_state);

/** Create Target Heater Cooler State Characteristic
 *
 * This API creates the Target Heater Cooler State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_heater_cooler_state  Initial value of Target Heater Cooler State
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_heater_cooler_state_create(uint8_t targ_heater_cooler_state);

/** Create Current Humidifier Dehumidifier State Characteristic
 *
 * This API creates the Current Humidifier Dehumidifier State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] curr_humidifier_dehumidifier_state Initial value of Current Humidifier Dehumidifier State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_current_humidifier_dehumidifier_state_create(uint8_t curr_humidifier_dehumidifier_state);

/** Create Target Humidifier Dehumidifier State Characteristic
 *
 * This API creates the Target Humidifier Dehumidifier State characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] targ_humidifier_dehumidifier_state Initial value of Target Humidifier Dehumidifier State characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_target_humidifier_dehumidifier_state_create(uint8_t targ_humidifier_dehumidifier_state);

/** Create Water Level Characteristic
 *
 * This API creates the Water Level characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] water_level Initial value of Water Level characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_water_level_create(float water_level);

/** Create Relative Humidity Dehumidifier Threshold Characteristic
 *
 * This API creates the Relative Humidity Dehumidifier Threshold characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] rel_humidity_dehumidifier_threshold Initial value of Relative Humidity Dehumidifier Threshold characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_relative_humidity_dehumidifier_threshold_create(float rel_humidity_dehumidifier_threshold);

/** Create Relative Humidity Humidifier Threshold Characteristic
 *
 * This API creates the Relative Humidity Humidifier Threshold characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] rel_humidity_humidifier_threshold Initial value of Relative Humidity Humidifier Threshold characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_relative_humidity_humidifier_threshold_create(float rel_humidity_humidifier_threshold);

/** Create Program Mode Characteristic
 *
 * This API creates the Program Mode characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] prog_mode Initial value of Program Mode characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_program_mode_create(uint8_t prog_mode);

/** Create In Use Characteristic
 *
 * This API creates the In Use characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] in_use Initial value of In Use characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_in_use_create(uint8_t in_use);

/** Create Set Duration Characteristic
 *
 * This API creates the Set Duration characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] set_duration Initial value of Set Duration characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_set_duration_create(uint32_t set_duration);

/** Create Remaining Duration Characteristic
 *
 * This API creates the Remaining Duration characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] remaining_duration Initial value of Remaining Duration characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_remaining_duration_create(uint32_t remaining_duration);

/** Create Valve Type Characteristic
 *
 * This API creates the Valve Type characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] valve_type Initial value of Valve Type characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_valve_type_create(uint8_t valve_type);

/** Create Is Configured Characteristic
 *
 * This API creates the Is Configured characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] is_configured Initial value of Is Configured characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_is_configured_create(uint8_t is_configured);

/** Status Jammed Characteristic
 *
 * This API creates the Status Jammed characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] status_jammed  Initial value of Status Jammed characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_status_jammed_create(uint8_t status_jammed);

/** Administrator Only Access Characteristic
 *
 * This API creates the Administrator Only Access characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] administrator_only_access  Initial value of Administrator Only Access characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_administrator_only_access_create(bool administrator_only_access);

/** Lock Control Point Characteristic
 *
 * This API creates the Lock Control Point characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] lock_control_point  Initial value of Lock Control Point characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_lock_control_point_create(hap_tlv8_val_t *lock_control_point);

/** Lock Last Known Action Characteristic
 *
 * This API creates the Lock Last Known Action characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] lock_last_known_action  Initial value of Lock Last Known Action characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_lock_last_known_action_create(uint8_t lock_last_known_action);

/** Lock Management Auto Security Timeout Characteristic
 *
 * This API creates the Lock Management Auto Security Timeout characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] lock_management_auto_security_timeout  Initial value of Lock Management Auto Security Timeout characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_lock_management_auto_security_timeout_create(uint32_t lock_management_auto_security_timeout);

/** Logs Characteristic
 *
 * This API creates the Logs characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] logs  Initial value of Logs characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_logs_create(hap_tlv8_val_t *logs);

/** Air Particulate Density Characteristic
 *
 * This API creates the Air Particulate Density characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] air_particulate_density  Initial value of Air Particulate Density characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_air_particulate_density_create(float air_particulate_density);

/** Air Particulate Size Characteristic
 *
 * This API creates the Air Particulate Size characteristic object with other metadata
 * (format, constraints, permissions, etc.) set as per the HAP Specs
 *
 * @param[in] air_particulate_size  Initial value of Air Particulate Size characteristic
 *
 * @return Pointer to the characteristic object on success
 * @return NULL on failure
 */
hap_char_t *hap_char_air_particulate_size_create(uint8_t air_particulate_size);

hap_char_t *hap_char_wattage_create(float watts);

#ifdef __cplusplus
}
#endif

#endif /* _HAP_APPLE_CHARS_H_ */
