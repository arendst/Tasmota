/*
 * NimBLERemoteDescriptor.h
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLERemoteDescriptor.h
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_
#define COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLERemoteCharacteristic.h"

class NimBLERemoteCharacteristic;
/**
 * @brief A model of remote %BLE descriptor.
 */
class NimBLERemoteDescriptor {
public:
    uint16_t                    getHandle();
    NimBLERemoteCharacteristic* getRemoteCharacteristic();
    NimBLEUUID                  getUUID();
    std::string                 readValue();

    /**
     * @brief A template to convert the remote descriptor data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>readValue<type>(skipSizeCheck);</tt>
     */
    template<typename T>
    T                           readValue(bool skipSizeCheck = false) {
        std::string value = readValue();
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        const char *pData = value.data();
        return *((T *)pData);
    }

    uint8_t                     readUInt8()  __attribute__ ((deprecated("Use template readValue<uint8_t>()")));
    uint16_t                    readUInt16() __attribute__ ((deprecated("Use template readValue<uint16_t>()")));
    uint32_t                    readUInt32() __attribute__ ((deprecated("Use template readValue<uint32_t>()")));
    std::string                 toString(void);
    bool                        writeValue(const uint8_t* data, size_t length, bool response = false);
    bool                        writeValue(const std::string &newValue, bool response = false);
    
    /**
     * @brief Convenience template to set the remote descriptor value to <type\>val.
     * @param [in] s The value to write.
     * @param [in] response True == request write response.
     */
    template<typename T>
    bool writeValue(const T &s, bool response = false) {
        return writeValue((uint8_t*)&s, sizeof(T), response);
    }

private:
    friend class                NimBLERemoteCharacteristic;

    NimBLERemoteDescriptor      (NimBLERemoteCharacteristic* pRemoteCharacteristic,
                                const struct ble_gatt_dsc *dsc);
    static int                  onWriteCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                          struct ble_gatt_attr *attr, void *arg);
    static int                  onReadCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                         struct ble_gatt_attr *attr, void *arg);

    uint16_t                    m_handle;
    NimBLEUUID                  m_uuid;
    NimBLERemoteCharacteristic* m_pRemoteCharacteristic;
};

#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_ */
