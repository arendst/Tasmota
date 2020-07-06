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

    template<typename T>
    T                           readValue(bool skipSizeCheck = false) {
        std::string value = readValue();
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        const char *pData = value.data();
        return *((T *)pData);
    }

    uint8_t                     readUInt8()  __attribute__ ((deprecated));
    uint16_t                    readUInt16() __attribute__ ((deprecated));
    uint32_t                    readUInt32() __attribute__ ((deprecated));
    std::string                 toString(void);
    bool                        writeValue(const uint8_t* data, size_t length, bool response = false);
    bool                        writeValue(const std::string &newValue, bool response = false);
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
