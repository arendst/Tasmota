/*
 * NimBLESecurity.h
 *
 *  Created: on Feb 22 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLESecurity.h
 *
 *  Created on: Dec 17, 2017
 *      Author: chegewara
 */

#ifndef COMPONENTS_NIMBLESECURITY_H_
#define COMPONENTS_NIMBLESECURITY_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "host/ble_gap.h"
/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

#include <stdint.h>

#define ESP_LE_AUTH_NO_BOND                 0x00                                     /*!< 0*/                     /* relate to BTM_LE_AUTH_NO_BOND in stack/btm_api.h */
#define ESP_LE_AUTH_BOND                    0x01                                     /*!< 1 << 0 */               /* relate to BTM_LE_AUTH_BOND in stack/btm_api.h */
#define ESP_LE_AUTH_REQ_MITM                (1 << 2)                                 /*!< 1 << 2 */               /* relate to BTM_LE_AUTH_REQ_MITM in stack/btm_api.h */
#define ESP_LE_AUTH_REQ_BOND_MITM           (ESP_LE_AUTH_BOND | ESP_LE_AUTH_REQ_MITM)/*!< 0101*/
#define ESP_LE_AUTH_REQ_SC_ONLY             (1 << 3)                                 /*!< 1 << 3 */               /* relate to BTM_LE_AUTH_REQ_SC_ONLY in stack/btm_api.h */
#define ESP_LE_AUTH_REQ_SC_BOND             (ESP_LE_AUTH_BOND | ESP_LE_AUTH_REQ_SC_ONLY)            /*!< 1001 */  /* relate to BTM_LE_AUTH_REQ_SC_BOND in stack/btm_api.h */
#define ESP_LE_AUTH_REQ_SC_MITM             (ESP_LE_AUTH_REQ_MITM | ESP_LE_AUTH_REQ_SC_ONLY)        /*!< 1100 */  /* relate to BTM_LE_AUTH_REQ_SC_MITM in stack/btm_api.h */
#define ESP_LE_AUTH_REQ_SC_MITM_BOND        (ESP_LE_AUTH_REQ_MITM | ESP_LE_AUTH_REQ_SC_ONLY | ESP_LE_AUTH_BOND)   /*!< 1101 */  /* relate to BTM_LE_AUTH_REQ_SC_MITM_BOND in stack/btm_api.h */

#define ESP_IO_CAP_OUT                      0   /*!< DisplayOnly */         /* relate to BTM_IO_CAP_OUT in stack/btm_api.h */
#define ESP_IO_CAP_IO                       1   /*!< DisplayYesNo */        /* relate to BTM_IO_CAP_IO in stack/btm_api.h */
#define ESP_IO_CAP_IN                       2   /*!< KeyboardOnly */        /* relate to BTM_IO_CAP_IN in stack/btm_api.h */
#define ESP_IO_CAP_NONE                     3   /*!< NoInputNoOutput */     /* relate to BTM_IO_CAP_NONE in stack/btm_api.h */
#define ESP_IO_CAP_KBDISP                   4   /*!< Keyboard display */    /* relate to BTM_IO_CAP_KBDISP in stack/btm_api.h */

/// Used to exchange the encryption key in the init key & response key
#define ESP_BLE_ENC_KEY_MASK    (1 << 0)            /* relate to BTM_BLE_ENC_KEY_MASK in stack/btm_api.h */
/// Used to exchange the IRK key in the init key & response key
#define ESP_BLE_ID_KEY_MASK     (1 << 1)            /* relate to BTM_BLE_ID_KEY_MASK in stack/btm_api.h */
/// Used to exchange the CSRK key in the init key & response key
#define ESP_BLE_CSR_KEY_MASK    (1 << 2)            /* relate to BTM_BLE_CSR_KEY_MASK in stack/btm_api.h */
/// Used to exchange the link key(this key just used in the BLE & BR/EDR coexist mode) in the init key & response key
#define ESP_BLE_LINK_KEY_MASK   (1 << 3)            /* relate to BTM_BLE_LINK_KEY_MASK in stack/btm_api.h */

typedef uint8_t   esp_ble_auth_req_t;         /*!< combination of the above bit pattern */
typedef uint8_t   esp_ble_io_cap_t;               /*!< combination of the io capability */


/**
 * @brief A class to handle BLE security operations.
 * <b>Deprecated - provided for backward compatibility only.</b>
 * @deprecated Use the security methods provided in NimBLEDevice instead.
 */
class NimBLESecurity {
public:
    NimBLESecurity();
    virtual ~NimBLESecurity();
    void setAuthenticationMode(esp_ble_auth_req_t auth_req);
    void setCapability(esp_ble_io_cap_t iocap);
    void setInitEncryptionKey(uint8_t init_key);
    void setRespEncryptionKey(uint8_t resp_key);
    void setKeySize(uint8_t key_size = 16);
    void setStaticPIN(uint32_t pin);
    //static char* esp_key_type_to_str(esp_ble_key_type_t key_type);
/*
private:
    esp_ble_auth_req_t m_authReq;
    esp_ble_io_cap_t m_iocap;
    uint8_t m_initKey;
    uint8_t m_respKey;
    uint8_t m_keySize;
*/
}; // BLESecurity


/**
 * @brief Callbacks to handle GAP events related to authorization.
 * <b>Deprecated - provided for backward compatibility only.</b>
 * @deprecated Use the callbacks provided in NimBLEClientCallbacks and NimBLEServerCallbacks instead.
 */
class NimBLESecurityCallbacks {
public:
    virtual ~NimBLESecurityCallbacks() {};

    /**
     * @brief Its request from peer device to input authentication pin code displayed on peer device.
     * It requires that our device is capable to input 6-digits code by end user
     * @return Return 6-digits integer value from input device
     */
    virtual uint32_t onPassKeyRequest() = 0;

    /**
     * @brief Provide us 6-digits code to perform authentication.
     * It requires that our device is capable to display this code to end user
     * @param [in] pass_key The PIN provided by the peer.
     */
    virtual void onPassKeyNotify(uint32_t pass_key) = 0;

    /**
     * @brief Here we can make decision if we want to let negotiate authorization with peer device or not
     * @return Return true if we accept this peer device request
     */
    virtual bool onSecurityRequest() = 0 ;
    /**
     * @brief Provides us information when authentication process is completed
     */
    virtual void onAuthenticationComplete(ble_gap_conn_desc*) = 0;

    /**
     * @brief Called when using numeric comparison for authentication.
     * @param [in] pin The PIN to compare.
     * @return True to accept and pair.
     */
    virtual bool onConfirmPIN(uint32_t pin) = 0;
}; // BLESecurityCallbacks

#endif // CONFIG_BT_ENABLED
#endif // COMPONENTS_NIMBLESECURITY_H_
