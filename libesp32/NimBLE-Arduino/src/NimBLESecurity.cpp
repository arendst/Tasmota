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

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLESecurity.h"
#include "NimBLEDevice.h"

/**
 * @brief This class is for backward compatibility with the bluedroid based library.
 * Use the new security functions in NimBLEDevice instead.
 * New callback functions in NimBLEServer and NimBLEClient.
 */

NimBLESecurity::NimBLESecurity() {
}

NimBLESecurity::~NimBLESecurity() {
}


/**
 * @brief Set requested authentication mode
 */
void NimBLESecurity::setAuthenticationMode(esp_ble_auth_req_t auth_req) {
    NimBLEDevice::setSecurityAuth((auth_req & BLE_SM_PAIR_AUTHREQ_BOND)>0,
                                (auth_req & BLE_SM_PAIR_AUTHREQ_MITM)>0,
                                (auth_req & BLE_SM_PAIR_AUTHREQ_SC)>0);
}


/**
 * @brief   Set our device IO capability to let end user perform authorization
 *          either by displaying or entering generated 6-digits pin code
 */
void NimBLESecurity::setCapability(esp_ble_io_cap_t iocap) {
    NimBLEDevice::setSecurityIOCap(iocap);
} // setCapability


/**
 * @brief Init encryption key by server
 * @param key_size is value between 7 and 16
 */
void NimBLESecurity::setInitEncryptionKey(uint8_t init_key) {
    NimBLEDevice::setSecurityInitKey(init_key);
} // setInitEncryptionKey


/**
 * @brief Init encryption key by client
 * @param key_size is value between 7 and 16
 */
void NimBLESecurity::setRespEncryptionKey(uint8_t resp_key) {
    NimBLEDevice::setSecurityRespKey(resp_key);
} // setRespEncryptionKey


/**
 *@todo  Requires implementation
 *
 */
void NimBLESecurity::setKeySize(uint8_t key_size) {

    //m_keySize = key_size;
    //esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &m_keySize, sizeof(uint8_t));
} //setKeySize


/**
 * Setup for static PIN connection.
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
