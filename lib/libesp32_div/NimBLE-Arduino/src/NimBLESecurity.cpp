/*
 * NimBLESecurity.cpp
 *
 *  Created: on Feb 22 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLESecurity.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: chegewara
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLESecurity.h"
#include "NimBLEDevice.h"

NimBLESecurity::NimBLESecurity() {
}

NimBLESecurity::~NimBLESecurity() {
}


/**
 * @brief Set requested authentication mode
 * @param [in] auth_req A bitmask containing one or more of:
 * * ESP_LE_AUTH_NO_BOND              0x00
 * * ESP_LE_AUTH_BOND                 0x01
 * * ESP_LE_AUTH_REQ_MITM             (1 << 2)
 * * ESP_LE_AUTH_REQ_BOND_MITM        (ESP_LE_AUTH_BOND | ESP_LE_AUTH_REQ_MITM)
 * * ESP_LE_AUTH_REQ_SC_ONLY          (1 << 3)
 * * ESP_LE_AUTH_REQ_SC_BOND          (ESP_LE_AUTH_BOND | ESP_LE_AUTH_REQ_SC_ONLY)
 * * ESP_LE_AUTH_REQ_SC_MITM          (ESP_LE_AUTH_REQ_MITM | ESP_LE_AUTH_REQ_SC_ONLY)
 * * ESP_LE_AUTH_REQ_SC_MITM_BOND     (ESP_LE_AUTH_REQ_MITM | ESP_LE_AUTH_REQ_SC_ONLY | ESP_LE_AUTH_BOND)
 */
void NimBLESecurity::setAuthenticationMode(esp_ble_auth_req_t auth_req) {
    NimBLEDevice::setSecurityAuth((auth_req & BLE_SM_PAIR_AUTHREQ_BOND)>0,
                                (auth_req & BLE_SM_PAIR_AUTHREQ_MITM)>0,
                                (auth_req & BLE_SM_PAIR_AUTHREQ_SC)>0);
}


/**
 * @brief Set our device IO capability to let end user perform authorization
 * either by displaying or entering generated 6-digit pin code or use \"just works\".
 * @param [in] iocap  The IO capabilites our device has.\n
 * Can be set to one of:
 * * ESP_IO_CAP_OUT                      0
 * * ESP_IO_CAP_IO                       1
 * * ESP_IO_CAP_IN                       2
 * * ESP_IO_CAP_NONE                     3
 * * ESP_IO_CAP_KBDISP                   4
 */
void NimBLESecurity::setCapability(esp_ble_io_cap_t iocap) {
    NimBLEDevice::setSecurityIOCap(iocap);
} // setCapability


/**
 * @brief Sets the keys we will distribute during encryption.
 * @param [in] init_key A bitmask of the keys we will distribute.\n
 * Can be one or more of:
 * * ESP_BLE_ENC_KEY_MASK    (1 << 0)
 * * ESP_BLE_ID_KEY_MASK     (1 << 1)
 * * ESP_BLE_CSR_KEY_MASK    (1 << 2)
 * * ESP_BLE_LINK_KEY_MASK   (1 << 3)
 */
void NimBLESecurity::setInitEncryptionKey(uint8_t init_key) {
    NimBLEDevice::setSecurityInitKey(init_key);
} // setInitEncryptionKey


/**
 * @brief Sets the keys we will accept during encryption.
 * @param [in] resp_key A bitmask of the keys we will accept.\n
 * Can be one or more of:
 * * ESP_BLE_ENC_KEY_MASK    (1 << 0)
 * * ESP_BLE_ID_KEY_MASK     (1 << 1)
 * * ESP_BLE_CSR_KEY_MASK    (1 << 2)
 * * ESP_BLE_LINK_KEY_MASK   (1 << 3)
 */
void NimBLESecurity::setRespEncryptionKey(uint8_t resp_key) {
    NimBLEDevice::setSecurityRespKey(resp_key);
} // setRespEncryptionKey


/**
 *@todo  Requires implementation
 */
void NimBLESecurity::setKeySize(uint8_t key_size) {

    //m_keySize = key_size;
    //esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &m_keySize, sizeof(uint8_t));
} //setKeySize


/**
 * @brief Sets a static PIN used to authenticate/encrypt the connection.
 * @param [in] pin The 6 digit pin code to accept.
 */
void NimBLESecurity::setStaticPIN(uint32_t pin){
    //uint32_t passkey = pin;
    //esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
    NimBLEDevice::setSecurityPasskey(pin);
    setCapability(ESP_IO_CAP_OUT);
    setKeySize();
    setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
    setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
}


/**
 * @brief Debug function to display what keys are exchanged by peers
 */
 /*
char* BLESecurity::esp_key_type_to_str(esp_ble_key_type_t key_type) {
    char* key_str = nullptr;
    switch (key_type) {
        case ESP_LE_KEY_NONE:
            key_str = (char*) "ESP_LE_KEY_NONE";
            break;
        case ESP_LE_KEY_PENC:
            key_str = (char*) "ESP_LE_KEY_PENC";
            break;
        case ESP_LE_KEY_PID:
            key_str = (char*) "ESP_LE_KEY_PID";
            break;
        case ESP_LE_KEY_PCSRK:
            key_str = (char*) "ESP_LE_KEY_PCSRK";
            break;
        case ESP_LE_KEY_PLK:
            key_str = (char*) "ESP_LE_KEY_PLK";
            break;
        case ESP_LE_KEY_LLK:
            key_str = (char*) "ESP_LE_KEY_LLK";
            break;
        case ESP_LE_KEY_LENC:
            key_str = (char*) "ESP_LE_KEY_LENC";
            break;
        case ESP_LE_KEY_LID:
            key_str = (char*) "ESP_LE_KEY_LID";
            break;
        case ESP_LE_KEY_LCSRK:
            key_str = (char*) "ESP_LE_KEY_LCSRK";
            break;
        default:
            key_str = (char*) "INVALID BLE KEY TYPE";
            break;
    }
    return key_str;

} // esp_key_type_to_str
*/
#endif // CONFIG_BT_ENABLED
