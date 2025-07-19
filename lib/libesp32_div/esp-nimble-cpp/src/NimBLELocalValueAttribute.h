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

#ifndef NIMBLE_LOCAL_VALUE_ATTRIBUTE_H_
#define NIMBLE_LOCAL_VALUE_ATTRIBUTE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "host/ble_hs.h"
# else
#  include "nimble/nimble/host/include/host/ble_hs.h"
# endif

/****  FIX COMPILATION ****/
# undef min
# undef max
/**************************/

typedef enum {
    READ         = BLE_GATT_CHR_F_READ,
    READ_ENC     = BLE_GATT_CHR_F_READ_ENC,
    READ_AUTHEN  = BLE_GATT_CHR_F_READ_AUTHEN,
    READ_AUTHOR  = BLE_GATT_CHR_F_READ_AUTHOR,
    WRITE        = BLE_GATT_CHR_F_WRITE,
    WRITE_NR     = BLE_GATT_CHR_F_WRITE_NO_RSP,
    WRITE_ENC    = BLE_GATT_CHR_F_WRITE_ENC,
    WRITE_AUTHEN = BLE_GATT_CHR_F_WRITE_AUTHEN,
    WRITE_AUTHOR = BLE_GATT_CHR_F_WRITE_AUTHOR,
    BROADCAST    = BLE_GATT_CHR_F_BROADCAST,
    NOTIFY       = BLE_GATT_CHR_F_NOTIFY,
    INDICATE     = BLE_GATT_CHR_F_INDICATE
} NIMBLE_PROPERTY;

# include "NimBLELocalAttribute.h"
# include "NimBLEValueAttribute.h"
# include "NimBLEAttValue.h"
# include <vector>
class NimBLEConnInfo;

class NimBLELocalValueAttribute : public NimBLELocalAttribute, public NimBLEValueAttribute {
  public:
    /**
     * @brief Get the properties of the attribute.
     */
    uint16_t getProperties() const { return m_properties; }

    /**
     * @brief Set the value of the attribute value.
     * @param [in] data The data to set the value to.
     * @param [in] size The size of the data.
     */
    void setValue(const uint8_t* data, size_t size) { m_value.setValue(data, size); }

    /**
     * @brief Set the value of the attribute value.
     * @param [in] str The string to set the value to.
     */
    void setValue(const char* str) { m_value.setValue(str); }

    /**
     * @brief Set the value of the attribute value.
     * @param [in] vec The vector to set the value to.
     */
    void setValue(const std::vector<uint8_t>& vec) { m_value.setValue(vec); }

    /**
     * @brief Template to set the value to <type\>val.
     * @param [in] val The value to set.
     */
    template <typename T>
    void setValue(const T& val) {
        m_value.setValue<T>(val);
    }

  protected:
    friend class NimBLEServer;

    /**
     * @brief Construct a new NimBLELocalValueAttribute object.
     * @param [in] uuid The UUID of the attribute.
     * @param [in] handle The handle of the attribute.
     * @param [in] maxLen The maximum length of the attribute value.
     * @param [in] initLen The initial length of the attribute value.
     */
    NimBLELocalValueAttribute(const NimBLEUUID& uuid,
                              uint16_t          handle,
                              uint16_t          maxLen,
                              uint16_t          initLen = CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH)
        : NimBLELocalAttribute(uuid, handle), NimBLEValueAttribute(maxLen, initLen) {}
    /**
     * @brief Destroy the NimBLELocalValueAttribute object.
     */
    virtual ~NimBLELocalValueAttribute() = default;

    /**
     * @brief Callback function to support a read request.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
     * @details This function is called by NimBLEServer when a read request is received.
     */
    virtual void readEvent(NimBLEConnInfo& connInfo) = 0;

    /**
     * @brief Callback function to support a write request.
     * @param [in] val The value to write.
     * @param [in] len The length of the value.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
     * @details This function is called by NimBLEServer when a write request is received.
     */
    virtual void writeEvent(const uint8_t* val, uint16_t len, NimBLEConnInfo& connInfo) = 0;

    /**
     * @brief Get a pointer to value of the attribute.
     * @return A pointer to the value of the attribute.
     * @details This function is used by NimBLEServer when handling read/write requests.
     */
    const NimBLEAttValue& getAttVal() const { return m_value; }

    /**
     * @brief Set the properties of the attribute.
     * @param [in] properties The properties of the attribute.
     */
    void setProperties(uint16_t properties) { m_properties = properties; }

    uint16_t m_properties{0};
};

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // NIMBLE_LOCAL_VALUE_ATTRIBUTE_H_
