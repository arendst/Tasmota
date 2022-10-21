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

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)

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
    NimBLEAttValue              readValue();

    uint8_t                     readUInt8()  __attribute__ ((deprecated("Use template readValue<uint8_t>()")));
    uint16_t                    readUInt16() __attribute__ ((deprecated("Use template readValue<uint16_t>()")));
    uint32_t                    readUInt32() __attribute__ ((deprecated("Use template readValue<uint32_t>()")));
    std::string                 toString(void);
    bool                        writeValue(const uint8_t* data, size_t length, bool response = false);
    bool                        writeValue(const std::vector<uint8_t>& v, bool response = false);
    bool                        writeValue(const char* s, bool response = false);


    /*********************** Template Functions ************************/

    /**
     * @brief Template to set the remote descriptor value to <type\>val.
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
     * @brief Template to set the remote descriptor value to <type\>val.
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
     * @brief Template to convert the remote descriptor data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>readValue<type>(skipSizeCheck);</tt>
     */
    template<typename T>
    T readValue(bool skipSizeCheck = false) {
        NimBLEAttValue value = readValue();
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        return *((T *)value.data());
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

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL */
#endif /* COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_ */
