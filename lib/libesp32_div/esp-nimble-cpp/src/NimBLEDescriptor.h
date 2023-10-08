/*
 * NimBLEDescriptor.h
 *
 *  Created: on March 10, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEDescriptor.h
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLEDESCRIPTOR_H_
#define MAIN_NIMBLEDESCRIPTOR_H_

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLECharacteristic.h"
#include "NimBLEUUID.h"
#include "NimBLEAttValue.h"
#include "NimBLEConnInfo.h"

#include <string>

class NimBLEService;
class NimBLECharacteristic;
class NimBLEDescriptorCallbacks;


/**
 * @brief A model of a %BLE descriptor.
 */
class NimBLEDescriptor {
public:
    NimBLEDescriptor(const char* uuid, uint16_t properties,
                     uint16_t max_len,
                     NimBLECharacteristic* pCharacteristic = nullptr);

    NimBLEDescriptor(NimBLEUUID uuid, uint16_t properties,
                     uint16_t max_len,
                     NimBLECharacteristic* pCharacteristic = nullptr);

    ~NimBLEDescriptor();

    uint16_t              getHandle();
    NimBLEUUID            getUUID();
    std::string           toString();
    void                  setCallbacks(NimBLEDescriptorCallbacks* pCallbacks);
    NimBLECharacteristic* getCharacteristic();

    size_t                getLength();
    NimBLEAttValue        getValue(time_t *timestamp = nullptr);
    std::string           getStringValue();

    void                  setValue(const uint8_t* data, size_t size);
    void                  setValue(const std::vector<uint8_t>& vec);

    /*********************** Template Functions ************************/

    /**
     * @brief Template to set the characteristic value to <type\>val.
     * @param [in] s The value to set.
     */
    template<typename T>
    void setValue(const T &s) { m_value.setValue<T>(s); }

    /**
     * @brief Template to convert the descriptor data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp (Optional) A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck (Optional) If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T getValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        return m_value.getValue<T>(timestamp, skipSizeCheck);
    }

private:
    friend class NimBLECharacteristic;
    friend class NimBLEService;
    friend class NimBLE2904;

    static int handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                              struct ble_gatt_access_ctxt *ctxt, void *arg);
    void       setHandle(uint16_t handle);
    void       setCharacteristic(NimBLECharacteristic* pChar);

    NimBLEUUID                 m_uuid;
    uint16_t                   m_handle;
    NimBLEDescriptorCallbacks* m_pCallbacks;
    NimBLECharacteristic*      m_pCharacteristic;
    uint8_t                    m_properties;
    NimBLEAttValue             m_value;
    uint8_t                    m_removed;
}; // NimBLEDescriptor


/**
 * @brief Callbacks that can be associated with a %BLE descriptors to inform of events.
 *
 * When a server application creates a %BLE descriptor, we may wish to be informed when there is either
 * a read or write request to the descriptors value.  An application can register a
 * sub-classed instance of this class and will be notified when such an event happens.
 */
class NimBLEDescriptorCallbacks {
public:
    virtual ~NimBLEDescriptorCallbacks(){}
    virtual void onRead(NimBLEDescriptor* pDescriptor, NimBLEConnInfo& connInfo);
    virtual void onWrite(NimBLEDescriptor* pDescriptor, NimBLEConnInfo& connInfo);
};

#include "NimBLE2904.h"

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
#endif /* MAIN_NIMBLEDESCRIPTOR_H_ */
