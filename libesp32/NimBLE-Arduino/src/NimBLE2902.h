/*
 * NimBLE2902.h
 *
 *  Created: on March 10, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLE2902.h
 *
 *  Created on: Jun 25, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLE2902_H_
#define MAIN_NIMBLE2902_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEDescriptor.h"

#include <vector>

#define NIMBLE_DESC_FLAG_NOTIFY   0x0001
#define NIMBLE_DESC_FLAG_INDICATE 0x0002

typedef struct {
    uint16_t conn_id;
    uint16_t sub_val;
} chr_sub_status_t;


/**
 * @brief Descriptor for Client Characteristic Configuration.
 *
 * This is a convenience descriptor for the Client Characteristic Configuration which has a UUID of 0x2902.
 *
 * See also:
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
 */
class NimBLE2902: public NimBLEDescriptor {
public:
    bool getNotifications();
    bool getIndications();
    void setNotifications(bool flag);
    void setIndications(bool flag);
private:
    NimBLE2902(NimBLECharacteristic* pCharacterisitic);
    friend class NimBLECharacteristic;
    std::vector<chr_sub_status_t> m_subscribedVec;

}; // NimBLE2902

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLE2902_H_ */
