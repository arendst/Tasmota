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
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLERemoteService.h"
#include "NimBLERemoteDescriptor.h"

#include <vector>
#include <functional>

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
    std::string                                    readValue(time_t *timestamp = nullptr);

    /**
     * @brief A template to convert the remote characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>readValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T                                              readValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        std::string value = readValue(timestamp);
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        const char *pData = value.data();
        return *((T *)pData);
    }

    uint8_t                                        readUInt8()  __attribute__ ((deprecated("Use template readValue<uint8_t>()")));
    uint16_t                                       readUInt16() __attribute__ ((deprecated("Use template readValue<uint16_t>()")));
    uint32_t                                       readUInt32() __attribute__ ((deprecated("Use template readValue<uint32_t>()")));
    float                                          readFloat()  __attribute__ ((deprecated("Use template readValue<float>()")));
    std::string                                    getValue(time_t *timestamp = nullptr);

    /**
     * @brief A template to convert the remote characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T                                              getValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        std::string value = getValue(timestamp);
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        const char *pData = value.data();
        return *((T *)pData);
    }

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
    bool                                           writeValue(const std::string &newValue,
                                                              bool response = false);
    /**
     * @brief Convenience template to set the remote characteristic value to <type\>val.
     * @param [in] s The value to write.
     * @param [in] response True == request write response.
     */
    template<typename T>
    bool writeValue(const T &s, bool response = false) {
        return writeValue((uint8_t*)&s, sizeof(T), response);
    }

    std::string                                    toString();
    NimBLERemoteService*                           getRemoteService();

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

    // Private properties
    NimBLEUUID              m_uuid;
    uint8_t                 m_charProp;
    uint16_t                m_handle;
    uint16_t                m_defHandle;
    NimBLERemoteService*    m_pRemoteService;
    std::string             m_value;
    notify_callback         m_notifyCallback;
    time_t                  m_timestamp;
    portMUX_TYPE            m_valMux;

    // We maintain a vector of descriptors owned by this characteristic.
    std::vector<NimBLERemoteDescriptor*> m_descriptorVector;
}; // NimBLERemoteCharacteristic

#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_ */
