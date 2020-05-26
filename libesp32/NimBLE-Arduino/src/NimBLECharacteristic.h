/*
 * NimBLECharacteristic.h
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 *
 * Originally:
 * BLECharacteristic.h
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLECHARACTERISTIC_H_
#define MAIN_NIMBLECHARACTERISTIC_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "host/ble_hs.h"
/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

typedef enum {
    READ         =  BLE_GATT_CHR_F_READ,
    READ_ENC     =  BLE_GATT_CHR_F_READ_ENC,
    READ_AUTHEN  =  BLE_GATT_CHR_F_READ_AUTHEN,
    READ_AUTHOR  =  BLE_GATT_CHR_F_READ_AUTHOR,
    WRITE        =  BLE_GATT_CHR_F_WRITE,
    WRITE_NR     =  BLE_GATT_CHR_F_WRITE_NO_RSP,
    WRITE_ENC    =  BLE_GATT_CHR_F_WRITE_ENC,
    WRITE_AUTHEN =  BLE_GATT_CHR_F_WRITE_AUTHEN,
    WRITE_AUTHOR =  BLE_GATT_CHR_F_WRITE_AUTHOR,
    BROADCAST    =  BLE_GATT_CHR_F_BROADCAST,
    NOTIFY       =  BLE_GATT_CHR_F_NOTIFY,
    INDICATE     =  BLE_GATT_CHR_F_INDICATE
} NIMBLE_PROPERTY;

#include "NimBLEService.h"
#include "NimBLEDescriptor.h"
#include "NimBLEUUID.h"
#include "NimBLEValue.h"
#include "FreeRTOS.h"

#include <string>
#include <map>


class NimBLEService;
class NimBLEDescriptor;
class NimBLECharacteristicCallbacks;


/**
 * @brief A management structure for %BLE descriptors.
 */
class NimBLEDescriptorMap {
public:
    void setByUUID(const char* uuid, NimBLEDescriptor* pDescriptor);
    void setByUUID(const NimBLEUUID &uuid, NimBLEDescriptor* pDescriptor);
//  void setByHandle(uint16_t handle, NimBLEDescriptor* pDescriptor);
    NimBLEDescriptor* getByUUID(const char* uuid);
    NimBLEDescriptor* getByUUID(const NimBLEUUID &uuid);
//  NimBLEDescriptor* getByHandle(uint16_t handle);
    std::string toString();
    NimBLEDescriptor* getFirst();
    NimBLEDescriptor* getNext();
    uint8_t           getSize();

private:
    std::map<NimBLEDescriptor*, std::string> m_uuidMap;
//  std::map<uint16_t, BLEDescriptor*> m_handleMap;
    std::map<NimBLEDescriptor*, std::string>::iterator m_iterator;
};


/**
 * @brief The model of a %BLE Characteristic.
 *
 * A BLE Characteristic is an identified value container that manages a value. It is exposed by a BLE server and
 * can be read and written to by a %BLE client.
 */
class NimBLECharacteristic {
public:
    NimBLEDescriptor* createDescriptor(const char* uuid,
                        uint32_t properties = NIMBLE_PROPERTY::READ |
                                              NIMBLE_PROPERTY::WRITE,
                                     uint16_t max_len = 100);
    NimBLEDescriptor* createDescriptor(const NimBLEUUID &uuid,
                        uint32_t properties = NIMBLE_PROPERTY::READ |
                                              NIMBLE_PROPERTY::WRITE,
                                     uint16_t max_len = 100);

    NimBLEDescriptor* getDescriptorByUUID(const char* descriptorUUID);
    NimBLEDescriptor* getDescriptorByUUID(const NimBLEUUID &descriptorUUID);
    NimBLEUUID        getUUID();
    std::string       getValue();
    uint8_t*          getData();
    size_t            getDataLength();

    void indicate();
    void notify(bool is_notification = true);
    void setCallbacks(NimBLECharacteristicCallbacks* pCallbacks);
//  Backward Compatibility - to be removed
    void setBroadcastProperty(bool value);
    void setIndicateProperty(bool value);
    void setNotifyProperty(bool value);
    void setReadProperty(bool value);
    void setWriteProperty(bool value);
    void setWriteNoResponseProperty(bool value);
//////////////////////////////////////////////////////
    void setValue(const uint8_t* data, size_t size);
    void setValue(const std::string &value);
    void setValue(uint16_t& data16);
    void setValue(uint32_t& data32);
    void setValue(int& data32);
    void setValue(float& data32);
    void setValue(double& data64);

    std::string toString();
    uint16_t getHandle();
//  void setAccessPermissions(uint16_t perm);

//  Backward Compatibility - to be removed
/*  static const uint32_t PROPERTY_READ      = 1<<0;
    static const uint32_t PROPERTY_WRITE     = 1<<1;
    static const uint32_t PROPERTY_NOTIFY    = 1<<2;
    static const uint32_t PROPERTY_BROADCAST = 1<<3;
    static const uint32_t PROPERTY_INDICATE  = 1<<4;
    static const uint32_t PROPERTY_WRITE_NR  = 1<<5;
*/
//////////////////////////////////////////////////////

private:

    friend class NimBLEServer;
    friend class NimBLEService;
//  friend class NimBLEDescriptor;
//  friend class NimBLECharacteristicMap;

    NimBLECharacteristic(const char* uuid, uint16_t properties = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE,
                                                        NimBLEService* pService = nullptr);
    NimBLECharacteristic(const NimBLEUUID &uuid, uint16_t properties = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE,
                                                        NimBLEService* pService = nullptr);
    virtual ~NimBLECharacteristic();

    NimBLEUUID                     m_uuid;
    NimBLEDescriptorMap            m_descriptorMap;
    uint16_t                       m_handle;
    uint16_t                       m_properties;
    NimBLECharacteristicCallbacks* m_pCallbacks;
    NimBLEService*                 m_pService;
    NimBLEValue                    m_value;
//  uint16_t                       m_permissions;

    void            addDescriptor(NimBLEDescriptor* pDescriptor);
    NimBLEService*  getService();
    uint8_t         getProperties();
    void            setSubscribe(struct ble_gap_event *event);
    static int      handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg);

    FreeRTOS::Semaphore m_semaphoreConfEvt   = FreeRTOS::Semaphore("ConfEvt");
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
    typedef enum {
        SUCCESS_INDICATE,
        SUCCESS_NOTIFY,
        ERROR_INDICATE_DISABLED,
        ERROR_NOTIFY_DISABLED,
        ERROR_GATT,
        ERROR_NO_CLIENT,
        ERROR_INDICATE_TIMEOUT,
        ERROR_INDICATE_FAILURE
    }Status;

    virtual ~NimBLECharacteristicCallbacks();
    virtual void onRead(NimBLECharacteristic* pCharacteristic);
    virtual void onWrite(NimBLECharacteristic* pCharacteristic);
    virtual void onNotify(NimBLECharacteristic* pCharacteristic);
    virtual void onStatus(NimBLECharacteristic* pCharacteristic, Status s, int code);
};

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /*MAIN_NIMBLECHARACTERISTIC_H_*/
