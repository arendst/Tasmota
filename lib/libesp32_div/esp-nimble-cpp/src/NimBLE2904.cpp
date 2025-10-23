/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NimBLE2904.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

NimBLE2904::NimBLE2904(NimBLECharacteristic* pChr)
    : NimBLEDescriptor(NimBLEUUID((uint16_t)0x2904), BLE_GATT_CHR_F_READ, sizeof(NimBLE2904Data), pChr) {
    setValue(m_data);
} // NimBLE2904

/**
 * @brief Set the description.
 * @param [in] description The description value to set.
 */
void NimBLE2904::setDescription(uint16_t description) {
    m_data.m_description = description;
    setValue(m_data);
} // setDescription

/**
 * @brief Set the exponent.
 * @param [in] exponent The exponent value to set.
 */
void NimBLE2904::setExponent(int8_t exponent) {
    m_data.m_exponent = exponent;
    setValue(m_data);
} // setExponent

/**
 * @brief Set the format.
 * @param [in] format The format value to set.
 */
void NimBLE2904::setFormat(uint8_t format) {
    m_data.m_format = format;
    setValue(m_data);
} // setFormat

/**
 * @brief Set the namespace.
 * @param [in] namespace_value The namespace value toset.
 */
void NimBLE2904::setNamespace(uint8_t namespace_value) {
    m_data.m_namespace = namespace_value;
    setValue(m_data);
} // setNamespace

/**
 * @brief Set the units for this value.
 * @param [in] unit The type of units of this characteristic as defined by assigned numbers.
 * @details See https://www.bluetooth.com/specifications/assigned-numbers/units
 */
void NimBLE2904::setUnit(uint16_t unit) {
    m_data.m_unit = unit;
    setValue(m_data);
} // setUnit

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
