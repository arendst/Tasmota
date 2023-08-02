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
#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#if defined(CONFIG_NIMBLE_CPP_IDF)
#include "host/ble_hs.h"
#else
#include "nimble/nimble/host/include/host/ble_hs.h"
#endif

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
#include "NimBLEAttValue.h"

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
    NimBLECharacteristic(const char* uuid,
                         uint16_t properties =
                         NIMBLE_PROPERTY::READ |
                         NIMBLE_PROPERTY::WRITE,
                         uint16_t max_len = BLE_ATT_ATTR_MAX_LEN,
                         NimBLEService* pService = nullptr);
    NimBLECharacteristic(const NimBLEUUID &uuid,
                         uint16_t properties =
                         NIMBLE_PROPERTY::READ |
                         NIMBLE_PROPERTY::WRITE,
                         uint16_t max_len = BLE_ATT_ATTR_MAX_LEN,
                         NimBLEService* pService = nullptr);

    ~NimBLECharacteristic();

    uint16_t          getHandle();
    NimBLEUUID        getUUID();
    std::string       toString();
    void              indicate();
    void              indicate(const uint8_t* value, size_t length);
    void              indicate(const std::vector<uint8_t>& value);
    void              notify(bool is_notification = true);
    void              notify(const uint8_t* value, size_t length, bool is_notification = true);
    void              notify(const std::vector<uint8_t>& value, bool is_notification = true);
    size_t            getSubscribedCount();
    void              addDescriptor(NimBLEDescriptor *pDescriptor);
    NimBLEDescriptor* getDescriptorByUUID(const char* uuid);
    NimBLEDescriptor* getDescriptorByUUID(const NimBLEUUID &uuid);
    NimBLEDescriptor* getDescriptorByHandle(uint16_t handle);
    void              removeDescriptor(NimBLEDescriptor *pDescriptor, bool deleteDsc = false);
    NimBLEService*    getService();
    uint16_t          getProperties();
    NimBLEAttValue    getValue(time_t *timestamp = nullptr);
    size_t            getDataLength();
    void              setValue(const uint8_t* data, size_t size);
    void              setValue(const std::vector<uint8_t>& vec);
    void              setCallbacks(NimBLECharacteristicCallbacks* pCallbacks);
    NimBLEDescriptor* createDescriptor(const char* uuid,
                                       uint32_t properties =
                                       NIMBLE_PROPERTY::READ |
                                       NIMBLE_PROPERTY::WRITE,
                                       uint16_t max_len = BLE_ATT_ATTR_MAX_LEN);;
    NimBLEDescriptor* createDescriptor(const NimBLEUUID &uuid,
                                       uint32_t properties =
                                       NIMBLE_PROPERTY::READ |
                                       NIMBLE_PROPERTY::WRITE,
                                       uint16_t max_len = BLE_ATT_ATTR_MAX_LEN);

    NimBLECharacteristicCallbacks* getCallbacks();


    /*********************** Template Functions ************************/

    /**
     * @brief Template to set the characteristic value to <type\>val.
     * @param [in] s The value to set.
     */
    template<typename T>
    void              setValue(const T &s) { m_value.setValue<T>(s); }

    /**
     * @brief Template to convert the characteristic data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp (Optional) A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck (Optional) If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template<typename T>
    T   getValue(time_t *timestamp = nullptr, bool skipSizeCheck = false) {
        return m_value.getValue<T>(timestamp, skipSizeCheck);
    }

    /**
     * @brief Template to send a notification from a class type that has a c_str() and length() method.
     * @tparam T The a reference to a class containing the data to send.
     * @param[in] value The <type\>value to set.
     * @param[in] is_notification if true sends a notification, false sends an indication.
     * @details Only used if the <type\> has a `c_str()` method.
     */
    template<typename T>
#ifdef _DOXYGEN_
    void
#else
    typename std::enable_if<Has_c_str_len<T>::value, void>::type
#endif
    notify(const T& value, bool is_notification = true) {
        notify((uint8_t*)value.c_str(), value.length(), is_notification);
    }

    /**
     * @brief Template to send an indication from a class type that has a c_str() and length() method.
     * @tparam T The a reference to a class containing the data to send.
     * @param[in] value The <type\>value to set.
     * @details Only used if the <type\> has a `c_str()` method.
     */
    template<typename T>
#ifdef _DOXYGEN_
    void
#else
    typename std::enable_if<Has_c_str_len<T>::value, void>::type
#endif
    indicate(const T& value) {
        indicate((uint8_t*)value.c_str(), value.length());
    }

private:

    friend class    NimBLEServer;
    friend class    NimBLEService;

    void            setService(NimBLEService *pService);
    void            setSubscribe(struct ble_gap_event *event);
    static int      handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg);

    NimBLEUUID                     m_uuid;
    uint16_t                       m_handle;
    uint16_t                       m_properties;
    NimBLECharacteristicCallbacks* m_pCallbacks;
    NimBLEService*                 m_pService;
    NimBLEAttValue                 m_value;
    std::vector<NimBLEDescriptor*> m_dscVec;
    uint8_t                        m_removed;

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

    virtual      ~NimBLECharacteristicCallbacks();
    virtual void onRead(NimBLECharacteristic* pCharacteristic);
    virtual void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc);
    virtual void onWrite(NimBLECharacteristic* pCharacteristic);
    virtual void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc);
    virtual void onNotify(NimBLECharacteristic* pCharacteristic);
    virtual void onStatus(NimBLECharacteristic* pCharacteristic, Status s, int code);
    virtual void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue);
};

#endif /* CONFIG_BT_ENABLED  && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
#endif /*MAIN_NIMBLECHARACTERISTIC_H_*/
