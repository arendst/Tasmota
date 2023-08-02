/*
 * NimBLERemoteCharacteristic.h
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLERemoteCharacteristic.h
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_
#define COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLERemoteService.h"
#include "NimBLERemoteDescriptor.h"

#include <vector>
#include <functional>
#include "NimBLELog.h"

class NimBLERemoteService;
class NimBLERemoteDescriptor;


typedef std::function<void (NimBLERemoteCharacteristic* pBLERemoteCharacteristic,
                                uint8_t* pData, size_t length, bool isNotify)> notify_callback;

typedef struct {
    const NimBLEUUID *uuid;
    void *task_data;
} desc_filter_t;


/**
 * @brief A model of a remote %BLE characteristic.
 */
class NimBLERemoteCharacteristic {
public:
    ~NimBLERemoteCharacteristic();

    // Public member functions
    bool                                           canBroadcast();
    bool                                           canIndicate();
    bool                                           canNotify();
    bool                                           canRead();
    bool                                           canWrite();
    bool                                           canWriteNoResponse();
    std::vector<NimBLERemoteDescriptor*>::iterator begin();
    std::vector<NimBLERemoteDescriptor*>::iterator end();
    NimBLERemoteDescriptor*                        getDescriptor(const NimBLEUUID &uuid);
    std::vector<NimBLERemoteDescriptor*>*          getDescriptors(bool refresh = false);
    void                                           deleteDescriptors();
    size_t                                         deleteDescriptor(const NimBLEUUID &uuid);
    uint16_t                                       getHandle();
    uint16_t                                       getDefHandle();
    NimBLEUUID                                     getUUID();
    NimBLEAttValue                                 readValue(time_t *timestamp = nullptr);
    std::string                                    toString();
    NimBLERemoteService*                           getRemoteService();

    uint8_t                                        readUInt8()  __attribute__ ((deprecated("Use template readValue<uint8_t>()")));
    uint16_t                                       readUInt16() __attribute__ ((deprecated("Use template readValue<uint16_t>()")));
    uint32_t                                       readUInt32() __attribute__ ((deprecated("Use template readValue<uint32_t>()")));
    float                                          readFloat()  __attribute__ ((deprecated("Use template readValue<float>()")));
    NimBLEAttValue                                 getValue(time_t *timestamp = nullptr);

    bool                                           subscribe(bool notifications = true,
                                                             notify_callback notifyCallback = nullptr,
                                                             bool response = false);
    bool                                           unsubscribe(bool response = false);
    bool                                           registerForNotify(notify_callback notifyCallback,
                                                                     bool notifications = true,
                                                                     bool response = true)
                                                                     __attribute__ ((deprecated("Use subscribe()/unsubscribe()")));
    bool                                           writeValue(const uint8_t* data,
                                                              size_t length,
                                                              bool response = false);
    bool                                           writeValue(const std::vector<uint8_t>& v, bool response = false);
    bool                                           writeValue(const char* s, bool response = false);


    /*********************** Template Functions ************************/

    /**
     * @brief Template to set the remote characteristic value to <type\>val.
     * @param [in] s The value to write.
     * @param [in] response True == request write response.
     * @details Only used for non-arrays and types without a `c_str()` method.
     */
    template<typename T>
#ifdef _DOXYGEN_
    bool
#else
    typename std::enable_if<!std::is_array<T>::value && !Has_c_str_len<T>::value, bool>::type
#endif
    writeValue(const T& s, bool response = false) {
        return writeValue((uint8_t*)&s, sizeof(T), response);
    }

    /**
     * @brief Template to set the remote characteristic value to <type\>val.
     * @param [in] s The value to write.
     * @param [in] response True == request write response.
     * @details Only used if the <type\> has a `c_str()` method.
     */
    template<typename T>
#ifdef _DOXYGEN_
    bool
#else
    typename std::enable_if<Has_c_str_len<T>::value, bool>::type
#endif
    writeValue(const T& s, bool response = false) {
        return writeValue((uint8_t*)s.c_str(), s.length(), response);
    }

    /**
     * @brief Template to convert the remote characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T getValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        if(!skipSizeCheck && m_value.size() < sizeof(T)) return T();
        return *((T *)m_value.getValue(timestamp));
    }

    /**
     * @brief Template to convert the remote characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>readValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T readValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        NimBLEAttValue value = readValue();
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        return *((T *)value.getValue(timestamp));
    }

private:

    NimBLERemoteCharacteristic(NimBLERemoteService *pRemoteservice, const struct ble_gatt_chr *chr);

    friend class      NimBLEClient;
    friend class      NimBLERemoteService;
    friend class      NimBLERemoteDescriptor;

    // Private member functions
    bool              setNotify(uint16_t val, notify_callback notifyCallback = nullptr, bool response = true);
    bool              retrieveDescriptors(const NimBLEUUID *uuid_filter = nullptr);
    static int        onReadCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                               struct ble_gatt_attr *attr, void *arg);
    static int        onWriteCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                struct ble_gatt_attr *attr, void *arg);
    static int        descriptorDiscCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                       uint16_t chr_val_handle, const struct ble_gatt_dsc *dsc,
                                       void *arg);
    static int        nextCharCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                 const struct ble_gatt_chr *chr, void *arg);

    // Private properties
    NimBLEUUID              m_uuid;
    uint8_t                 m_charProp;
    uint16_t                m_handle;
    uint16_t                m_defHandle;
    uint16_t                m_endHandle;
    NimBLERemoteService*    m_pRemoteService;
    NimBLEAttValue          m_value;
    notify_callback         m_notifyCallback;

    // We maintain a vector of descriptors owned by this characteristic.
    std::vector<NimBLERemoteDescriptor*> m_descriptorVector;
}; // NimBLERemoteCharacteristic

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL */
#endif /* COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_ */
