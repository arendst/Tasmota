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

#ifndef NIMBLE_CPP_CHARACTERISTIC_H_
#define NIMBLE_CPP_CHARACTERISTIC_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

class NimBLECharacteristicCallbacks;
class NimBLEService;
class NimBLECharacteristic;
class NimBLEDescriptor;
class NimBLE2904;

# include "NimBLELocalValueAttribute.h"

# include <string>
# include <vector>

/**
 * @brief The model of a BLE Characteristic.
 *
 * A BLE Characteristic is an identified value container that manages a value. It is exposed by a BLE service and
 * can be read and written to by a BLE client.
 */
class NimBLECharacteristic : public NimBLELocalValueAttribute {
  public:
    NimBLECharacteristic(const char*    uuid,
                         uint16_t       properties = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE,
                         uint16_t       maxLen     = BLE_ATT_ATTR_MAX_LEN,
                         NimBLEService* pService   = nullptr);
    NimBLECharacteristic(const NimBLEUUID& uuid,
                         uint16_t          properties = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE,
                         uint16_t          maxLen     = BLE_ATT_ATTR_MAX_LEN,
                         NimBLEService*    pService   = nullptr);

    ~NimBLECharacteristic();

    std::string toString() const;
    void        addDescriptor(NimBLEDescriptor* pDescriptor);
    void        removeDescriptor(NimBLEDescriptor* pDescriptor, bool deleteDsc = false);
    uint16_t    getProperties() const;
    void        setCallbacks(NimBLECharacteristicCallbacks* pCallbacks);
    bool        indicate(uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const;
    bool        indicate(const uint8_t* value, size_t length, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const;
    bool        notify(uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const;
    bool        notify(const uint8_t* value, size_t length, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const;

    NimBLEDescriptor* createDescriptor(const char* uuid,
                                       uint32_t    properties = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE,
                                       uint16_t    maxLen     = BLE_ATT_ATTR_MAX_LEN);
    NimBLEDescriptor* createDescriptor(const NimBLEUUID& uuid,
                                       uint32_t          properties = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE,
                                       uint16_t          maxLen     = BLE_ATT_ATTR_MAX_LEN);
    NimBLE2904*       create2904();
    NimBLEDescriptor* getDescriptorByUUID(const char* uuid) const;
    NimBLEDescriptor* getDescriptorByUUID(const NimBLEUUID& uuid) const;
    NimBLEDescriptor* getDescriptorByHandle(uint16_t handle) const;
    NimBLEService*    getService() const;

    NimBLECharacteristicCallbacks* getCallbacks() const;

    /*********************** Template Functions ************************/

# if __cplusplus < 201703L
    /**
     * @brief Template to send a notification with a value from a struct or array.
     * @param [in] v The value to send.
     * @param [in] connHandle Optional, a connection handle to send the notification to.
     * @details <type\> size must be evaluatable by `sizeof()`.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<!std::is_pointer<T>::value && !std::is_array<T>::value && !Has_c_str_length<T>::value &&
                                !Has_data_size<T>::value,
                            bool>::type
#  endif
    notify(const T& v, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        return notify(reinterpret_cast<const uint8_t*>(&v), sizeof(T), connHandle);
    }

    /**
     * @brief Template to send a notification with a value from a class that has a c_str() and length() method.
     * @param [in] s The value to send.
     * @param [in] connHandle Optional, a connection handle to send the notification to.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_c_str_length<T>::value && !Has_data_size<T>::value, bool>::type
#  endif
    notify(const T& s, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        return notify(reinterpret_cast<const uint8_t*>(s.c_str()), s.length(), connHandle);
    }

    /**
     * @brief Template to send a notification with a value from a class that has a data() and size() method.
     * @param [in] v The value to send.
     * @param [in] connHandle Optional, a connection handle to send the notification to.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_data_size<T>::value, bool>::type
#  endif
    notify(const T& v, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        return notify(reinterpret_cast<const uint8_t*>(v.data()), v.size(), connHandle);
    }

    /**
     * @brief Template to send an indication with a value from a struct or array.
     * @param [in] v The value to send.
     * @param [in] connHandle Optional, a connection handle to send the notification to.
     * @details <type\> size must be evaluatable by `sizeof()`.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<!std::is_pointer<T>::value && !std::is_array<T>::value && !Has_c_str_length<T>::value &&
                                !Has_data_size<T>::value,
                            bool>::type
#  endif
    indicate(const T& v, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        return indicate(reinterpret_cast<const uint8_t*>(&v), sizeof(T), connHandle);
    }

    /**
     * @brief Template to send a indication with a value from a class that has a c_str() and length() method.
     * @param [in] s The value to send.
     * @param [in] connHandle Optional, a connection handle to send the notification to.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_c_str_length<T>::value && !Has_data_size<T>::value, bool>::type
#  endif
    indicate(const T& s, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        return indicate(reinterpret_cast<const uint8_t*>(s.c_str()), s.length(), connHandle);
    }

    /**
     * @brief Template to send a indication with a value from a class that has a data() and size() method.
     * @param [in] v The value to send.
     * @param [in] connHandle Optional, a connection handle to send the notification to.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_data_size<T>::value, bool>::type
#  endif
    indicate(const T& v, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        return indicate(reinterpret_cast<const uint8_t*>(v.data()), v.size(), connHandle);
    }

# else

    /**
     * @brief Template to send a notification for classes which may have
     *        data()/size() or c_str()/length() methods. Falls back to sending
     *        the data by casting the first element of the array to a uint8_t
     *        pointer and getting the length of the array using sizeof.
     * @tparam T The a reference to a class containing the data to send.
     * @param[in] value The <type\>value to set.
     * @param[in] connHandle The connection handle to send the notification to.
     * @note This function is only available if the type T is not a pointer.
     */
    template <typename T>
    typename std::enable_if<!std::is_pointer<T>::value && !std::is_array<T>::value, bool>::type notify(
        const T& value, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        if constexpr (Has_data_size<T>::value) {
            return notify(reinterpret_cast<const uint8_t*>(value.data()), value.size(), connHandle);
        } else if constexpr (Has_c_str_length<T>::value) {
            return notify(reinterpret_cast<const uint8_t*>(value.c_str()), value.length(), connHandle);
        } else {
            return notify(reinterpret_cast<const uint8_t*>(&value), sizeof(value), connHandle);
        }
    }

    /**
     * @brief Template to send an indication for classes which may have
     *       data()/size() or c_str()/length() methods. Falls back to sending
     *       the data by casting the first element of the array to a uint8_t
     *       pointer and getting the length of the array using sizeof.
     * @tparam T The a reference to a class containing the data to send.
     * @param[in] value The <type\>value to set.
     * @param[in] connHandle The connection handle to send the indication to.
     * @note This function is only available if the type T is not a pointer.
     */
    template <typename T>
    typename std::enable_if<!std::is_pointer<T>::value && !std::is_array<T>::value, bool>::type indicate(
        const T& value, uint16_t connHandle = BLE_HS_CONN_HANDLE_NONE) const {
        if constexpr (Has_data_size<T>::value) {
            return indicate(reinterpret_cast<const uint8_t*>(value.data()), value.size(), connHandle);
        } else if constexpr (Has_c_str_length<T>::value) {
            return indicate(reinterpret_cast<const uint8_t*>(value.c_str()), value.length(), connHandle);
        } else {
            return indicate(reinterpret_cast<const uint8_t*>(&value), sizeof(value), connHandle);
        }
    }
# endif

  private:
    friend class NimBLEServer;
    friend class NimBLEService;

    void setService(NimBLEService* pService);
    void readEvent(NimBLEConnInfo& connInfo) override;
    void writeEvent(const uint8_t* val, uint16_t len, NimBLEConnInfo& connInfo) override;
    bool sendValue(const uint8_t* value,
                   size_t         length,
                   bool           is_notification = true,
                   uint16_t       connHandle      = BLE_HS_CONN_HANDLE_NONE) const;

    NimBLECharacteristicCallbacks* m_pCallbacks{nullptr};
    NimBLEService*                 m_pService{nullptr};
    std::vector<NimBLEDescriptor*> m_vDescriptors{};
}; // NimBLECharacteristic

/**
 * @brief Callbacks that can be associated with a %BLE characteristic to inform of events.
 *
 * When a server application creates a %BLE characteristic, we may wish to be informed when there is either
 * a read or write request to the characteristic's value. An application can register a
 * sub-classed instance of this class and will be notified when such an event happens.
 */
class NimBLECharacteristicCallbacks {
  public:
    virtual ~NimBLECharacteristicCallbacks() {}
    virtual void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
    virtual void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
    virtual void onStatus(NimBLECharacteristic* pCharacteristic, int code);
    virtual void onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue);
};

#endif // CONFIG_BT_ENABLED  && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // NIMBLE_CPP_CHARACTERISTIC_H_
