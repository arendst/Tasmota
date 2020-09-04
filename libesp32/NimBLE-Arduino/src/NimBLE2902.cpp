/*
 * NimBLE2902.cpp
 *
 *  Created: on March 10, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLE2902.cpp
 *
 *  Created on: Jun 25, 2017
 *      Author: kolban
 */


/*
 * See also:
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLE2902.h"

NimBLE2902::NimBLE2902(NimBLECharacteristic* pCharacterisitic)
: NimBLEDescriptor(NimBLEUUID((uint16_t) 0x2902),
                        BLE_GATT_CHR_F_READ |
                        BLE_GATT_CHR_F_WRITE,
                        2, pCharacterisitic)
{
    uint8_t data[2] = { 0, 0 };
    setValue(data, 2);
} // NimBLE2902


/**
 * @brief Get the notifications value.
 * @return The notifications value.  True if notifications are enabled and false if not.
 */
bool NimBLE2902::getNotifications() {
    return (getValue()[0] & (1 << 0)) != 0;
} // getNotifications


/**
 * @brief Get the indications value.
 * @return The indications value.  True if indications are enabled and false if not.
 */
bool NimBLE2902::getIndications() {
    return (getValue()[0] & (1 << 1)) != 0;
} // getIndications


/**
 * @brief Set the indications flag.
 * @param [in] flag The indications flag.
 */
void NimBLE2902::setIndications(bool flag) {
    uint8_t *pValue = getValue();
    if (flag) pValue[0] |= 1 << 1;
    else pValue[0] &= ~(1 << 1);
} // setIndications


/**
 * @brief Set the notifications flag.
 * @param [in] flag The notifications flag.
 */
void NimBLE2902::setNotifications(bool flag) {
    uint8_t *pValue = getValue();
    if (flag) pValue[0] |= 1 << 0;
    else pValue[0] &= ~(1 << 0);
} // setNotifications

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif
