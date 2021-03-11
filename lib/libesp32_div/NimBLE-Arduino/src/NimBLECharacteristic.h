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

#include <string>
#include <vector>

class NimBLEService;
class NimBLEDescriptor;
class NimBLECharacteristicCallbacks;


/**
 * @brief The model of a %BLE Characteristic.
 *
 * A BLE Characteristic is an identified value container that manages a value. It is exposed by a BLE server and
 * can be read and written to by a %BLE client.
 */
class NimBLECharacteristic {
public:

    uint16_t          getHandle();
    NimBLEUUID        getUUID();
    std::string       toString();

    void              setCallbacks(NimBLECharacteristicCallbacks* pCallbacks);

    void              indicate();
    void              notify(bool is_notification = true);
    size_t            getSubscribedCount();

    NimBLEDescriptor* createDescriptor(const char* uuid,
                                       uint32_t properties =
                                       NIMBLE_PROPERTY::READ |
                                       NIMBLE_PROPERTY::WRITE,
                                       uint16_t max_len = 100);
    NimBLEDescriptor* createDescriptor(const NimBLEUUID &uuid,
                                       uint32_t properties =
                                       NIMBLE_PROPERTY::READ |
                                       NIMBLE_PROPERTY::WRITE,
                                       uint16_t max_len = 100);

    NimBLEDescriptor* getDescriptorByUUID(const char* uuid);
    NimBLEDescriptor* getDescriptorByUUID(const NimBLEUUID &uuid);
    NimBLEDescriptor* getDescriptorByHandle(uint16_t handle);

    std::string       getValue(time_t *timestamp = nullptr);
    size_t            getDataLength();
    /**
     * @brief A template to convert the characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T                 getValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        std::string value = getValue();
        if(!skipSizeCheck && value.size() < sizeof(T)) return T();
        const char *pData = value.data();
        return *((T *)pData);
    }

    void              setValue(const uint8_t* data, size_t size);
    void              setValue(const std::string &value);
    /**
     * @brief Convenience template to set the characteristic value to <type\>val.
     * @param [in] s The value to set.
     */
    template<typename T>
    void              setValue(const T &s) {
        setValue((uint8_t*)&s, sizeof(T));
    }





private:

    friend class      NimBLEServer;
    friend class      NimBLEService;

    NimBLECharacteristic(const char* uuid,
                         uint16_t properties =
                         NIMBLE_PROPERTY::READ |
                         NIMBLE_PROPERTY::WRITE,
                         NimBLEService* pService = nullptr);
    NimBLECharacteristic(const NimBLEUUID &uuid,
                         uint16_t properties =
                         NIMBLE_PROPERTY::READ |
                         NIMBLE_PROPERTY::WRITE,
                         NimBLEService* pService = nullptr);

    ~NimBLECharacteristic();

    NimBLEService*  getService();
    uint16_t        getProperties();
    void            setSubscribe(struct ble_gap_event *event);
    static int      handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg);

    NimBLEUUID                     m_uuid;
    uint16_t                       m_handle;
    uint16_t                       m_properties;
    NimBLECharacteristicCallbacks* m_pCallbacks;
    NimBLEService*                 m_pService;
    std::string                    m_value;
    std::vector<NimBLEDescriptor*> m_dscVec;
    ble_task_data_t                *m_pTaskData;
    portMUX_TYPE                   m_valMux;
    time_t                         m_timestamp;

    std::vector<std::pair<uint16_t, uint16_t>>  m_subscribedVec;
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

/**
 * @brief An enum to provide the callback the status of the
 * notification/indication, implemented for backward compatibility.
 * @deprecated To be removed in the future as the NimBLE stack return code is also provided.
 */
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
    virtual void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc);
    virtual void onWrite(NimBLECharacteristic* pCharacteristic);
    virtual void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc);
    virtual void onNotify(NimBLECharacteristic* pCharacteristic);
    virtual void onStatus(NimBLECharacteristic* pCharacteristic, Status s, int code);
    virtual void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue);
};

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /*MAIN_NIMBLECHARACTERISTIC_H_*/
