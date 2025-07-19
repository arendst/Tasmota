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

#ifndef NIMBLE_CPP_REMOTE_VALUE_ATTRIBUTE_H_
#define NIMBLE_CPP_REMOTE_VALUE_ATTRIBUTE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include <host/ble_gatt.h>
# else
#  include <nimble/nimble/host/include/host/ble_gatt.h>
# endif

/****  FIX COMPILATION ****/
# undef min
# undef max
/**************************/

# include "NimBLEValueAttribute.h"
# include "NimBLEAttValue.h"

class NimBLEClient;

class NimBLERemoteValueAttribute : public NimBLEValueAttribute, public NimBLEAttribute {
  public:
    /**
     * @brief Read the value of the remote attribute.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @return The value of the remote attribute.
     */
    NimBLEAttValue readValue(time_t* timestamp = nullptr);

    /**
     * Get the client instance that owns this attribute.
     */
    virtual NimBLEClient* getClient() const = 0;

    /**
     * @brief Write a new value to the remote characteristic from a data buffer.
     * @param [in] data A pointer to a data buffer.
     * @param [in] length The length of the data in the data buffer.
     * @param [in] response Whether we require a response from the write.
     * @return false if not connected or otherwise cannot perform write.
     */
    bool writeValue(const uint8_t* data, size_t length, bool response = false) const;

    /**
     * @brief Write a new value to the remote characteristic from a const char*.
     * @param [in] str A character string to write to the remote characteristic.
     * @param [in] length (optional) The length of the character string, uses strlen if omitted.
     * @param [in] response Whether we require a response from the write.
     * @return false if not connected or otherwise cannot perform write.
     */
    bool writeValue(const char* str, size_t length = 0, bool response = false) const {
        return writeValue(reinterpret_cast<const uint8_t*>(str), length ? length : strlen(str), response);
    }

# if __cplusplus < 201703L
    /**
     * @brief Template to set the remote characteristic value to <type\>val.
     * @param [in] v The value to write.
     * @param [in] response True == request write response.
     * @details Only used for types without a `c_str()` and `length()` or `data()` and `size()` method.
     * <type\> size must be evaluatable by `sizeof()` if no length is provided.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<!std::is_pointer<T>::value && !Has_c_str_length<T>::value && !Has_data_size<T>::value, bool>::type
#  endif
    writeValue(const T& v, bool response = false) const {
        return writeValue(reinterpret_cast<const uint8_t*>(&v), sizeof(T), response);
    }

    /**
     * @brief Template to set the remote characteristic value to <type\>val.
     * @param [in] s The value to write.
     * @param [in] response True == request write response.
     * @details Only used if the <type\> has a `c_str()` and `length()` method.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_c_str_length<T>::value && !Has_data_size<T>::value, bool>::type
#  endif
    writeValue(const T& s, bool response = false) const {
        return writeValue(reinterpret_cast<const uint8_t*>(s.c_str()), s.length(), response);
    }

    /**
     * @brief Template to set the remote characteristic value to <type\>val.
     * @param [in] v The value to write.
     * @param [in] response True == request write response.
     * @details Only used if the <type\> has a `data()` and `size()` method.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_data_size<T>::value, bool>::type
#  endif
    writeValue(const T& v, bool response = false) const {
        return writeValue(reinterpret_cast<const uint8_t*>(v.data()), v.size(), response);
    }

# else
    /**
     * @brief Template to set the remote characteristic value to <type\>val.
     * @param [in] s The value to write.
     * @param [in] response True == request write response.
     * @note This function is only available if the type T is not a pointer.
     */
    template <typename T>
    typename std::enable_if<!std::is_pointer<T>::value, bool>::type writeValue(const T& v, bool response = false) const {
        if constexpr (Has_data_size<T>::value) {
            return writeValue(reinterpret_cast<const uint8_t*>(v.data()), v.size(), response);
        } else if constexpr (Has_c_str_length<T>::value) {
            return writeValue(reinterpret_cast<const uint8_t*>(v.c_str()), v.length(), response);
        } else {
            return writeValue(reinterpret_cast<const uint8_t*>(&v), sizeof(v), response);
        }
    }
# endif

    /**
     * @brief Template to convert the remote characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>readValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template <typename T>
    T readValue(time_t* timestamp = nullptr, bool skipSizeCheck = false) {
        readValue();
        return getValue<T>(timestamp, skipSizeCheck);
    }

  protected:
    /**
     * @brief Construct a new NimBLERemoteValueAttribute object.
     */
    NimBLERemoteValueAttribute(const ble_uuid_any_t& uuid, uint16_t handle) : NimBLEAttribute{uuid, handle} {}

    /**
     * @brief Destroy the NimBLERemoteValueAttribute object.
     */
    virtual ~NimBLERemoteValueAttribute() = default;

    static int onReadCB(uint16_t conn_handle, const ble_gatt_error* error, ble_gatt_attr* attr, void* arg);
    static int onWriteCB(uint16_t conn_handle, const ble_gatt_error* error, ble_gatt_attr* attr, void* arg);
};

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL
#endif // NIMBLE_CPP_REMOTE_VALUE_ATTRIBUTE_H_
