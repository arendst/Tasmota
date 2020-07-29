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

#include "NimBLEDescriptor.h"

#include <map>

#define NIMBLE_DESC_FLAG_NOTIFY   0x0001
#define NIMBLE_DESC_FLAG_INDICATE 0x0002


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
    std::map<uint16_t, uint16_t> m_subscribedMap;

}; // NimBLE2902

#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLE2902_H_ */