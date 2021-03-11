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
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLECharacteristic.h"
#include "NimBLEUUID.h"

#include <string>


typedef struct
{
    uint16_t attr_max_len;  /*!<  attribute max value length */
    uint16_t attr_len;      /*!<  attribute current value length */
    uint8_t  *attr_value;    /*!<  the pointer to attribute value */
} attr_value_t;

class NimBLEService;
class NimBLECharacteristic;
class NimBLEDescriptorCallbacks;


/**
 * @brief A model of a %BLE descriptor.
 */
class NimBLEDescriptor {
public:
    uint16_t     getHandle();
    NimBLEUUID   getUUID();
    std::string  toString();

    void         setCallbacks(NimBLEDescriptorCallbacks* pCallbacks);

    size_t       getLength();
    uint8_t*     getValue();
    std::string  getStringValue();

    void         setValue(const uint8_t* data, size_t size);
    void         setValue(const std::string &value);
    /**
     * @brief Convenience template to set the descriptor value to <type\>val.
     * @param [in] s The value to set.
     */
    template<typename T>
    void         setValue(const T &s) {
        setValue((uint8_t*)&s, sizeof(T));
    }

private:
    friend class NimBLECharacteristic;
    friend class NimBLEService;
    friend class NimBLE2902;
    friend class NimBLE2904;

    NimBLEDescriptor(const char* uuid, uint16_t properties,
                     uint16_t max_len,
                     NimBLECharacteristic* pCharacteristic);

    NimBLEDescriptor(NimBLEUUID uuid, uint16_t properties,
                     uint16_t max_len,
                     NimBLECharacteristic* pCharacteristic);

    ~NimBLEDescriptor();

    static int handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                              struct ble_gatt_access_ctxt *ctxt, void *arg);
    void       setHandle(uint16_t handle);

    NimBLEUUID                 m_uuid;
    uint16_t                   m_handle;
    NimBLEDescriptorCallbacks* m_pCallbacks;
    NimBLECharacteristic*      m_pCharacteristic;
    uint8_t                    m_properties;
    attr_value_t               m_value;
    portMUX_TYPE               m_valMux;
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
    virtual ~NimBLEDescriptorCallbacks();
    virtual void onRead(NimBLEDescriptor* pDescriptor);
    virtual void onWrite(NimBLEDescriptor* pDescriptor);
};

#include "NimBLE2904.h"

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLEDESCRIPTOR_H_ */
