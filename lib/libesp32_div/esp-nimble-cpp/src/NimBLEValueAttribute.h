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

#ifndef NIMBLE_CPP_VALUE_ATTRIBUTE_H_
#define NIMBLE_CPP_VALUE_ATTRIBUTE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && (CONFIG_BT_NIMBLE_ROLE_PERIPHERAL || CONFIG_BT_NIMBLE_ROLE_CENTRAL)

# include "NimBLEAttribute.h"
# include "NimBLEAttValue.h"

class NimBLEValueAttribute {
  public:
    NimBLEValueAttribute(uint16_t maxLen = BLE_ATT_ATTR_MAX_LEN, uint16_t initLen = CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH)
        : m_value(initLen, maxLen) {}

    /**
     * @brief Get a copy of the value of the attribute value.
     * @param [in] timestamp (Optional) A pointer to a time_t struct to get the time the value set.
     * @return A copy of the attribute value.
     */
    NimBLEAttValue getValue(time_t* timestamp) const { return m_value.getValue(timestamp); }

    /**
     * @brief Get a copy of the value of the attribute value.
     * @return A copy of the attribute value.
     */
    NimBLEAttValue getValue() const { return m_value; }

    /**
     * @brief Get the length of the attribute value.
     * @return The length of the attribute value.
     */
    size_t getLength() const { return m_value.size(); }

    /**
     * @brief Template to convert the data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp (Optional) A pointer to a time_t struct to get the time the value set.
     * @param [in] skipSizeCheck (Optional) If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     * Used for types that are trivially copyable and convertible to NimBLEAttValue.
     */
    template <typename T>
    typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type
    getValue(time_t* timestamp = nullptr, bool skipSizeCheck = false) const {
        return m_value.getValue<T>(timestamp, skipSizeCheck);
    }

    /**
     * @brief Template to convert the data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp (Optional) A pointer to a time_t struct to get the time the value set.
     * @param [in] skipSizeCheck (Optional) If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     * Used for types that are not trivially copyable and convertible to NimBLEAttValue via it's operators.
     */
    template <typename T>
    typename std::enable_if<!std::is_trivially_copyable<T>::value && std::is_convertible<T, NimBLEAttValue>::value, T>::type
    getValue(time_t* timestamp = nullptr, bool skipSizeCheck = false) const {
        return m_value;
    }

  protected:
    NimBLEAttValue m_value{};
};

#endif // CONFIG_BT_ENABLED && (CONFIG_BT_NIMBLE_ROLE_PERIPHERAL || CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif // NIMBLE_CPP_ATTRIBUTE_H_
