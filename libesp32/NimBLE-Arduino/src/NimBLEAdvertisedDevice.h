/*
 * NimBLEAdvertisedDevice.h
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLEAdvertisedDevice.h
 *
 *  Created on: Jul 3, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEADVERTISEDDEVICE_H_
#define COMPONENTS_NIMBLEADVERTISEDDEVICE_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEAddress.h"
#include "NimBLEScan.h"
#include "NimBLEUUID.h"

#include "host/ble_hs_adv.h"

#include <map>
#include <vector> 


class NimBLEScan;
/**
 * @brief A representation of a %BLE advertised device found by a scan.
 *
 * When we perform a %BLE scan, the result will be a set of devices that are advertising.  This
 * class provides a model of a detected device.
 */
class NimBLEAdvertisedDevice {
public:
    NimBLEAdvertisedDevice();

    NimBLEAddress   getAddress();
    uint16_t        getAppearance();
    std::string     getManufacturerData();
    std::string     getName();
    int             getRSSI();
    NimBLEScan*     getScan();
    std::string     getServiceData();
    NimBLEUUID      getServiceDataUUID();
    NimBLEUUID      getServiceUUID();
    int8_t          getTXPower();
    uint8_t*        getPayload();
    size_t          getPayloadLength();
    uint8_t         getAddressType();
    void setAddressType(uint8_t type);


    bool        isAdvertisingService(NimBLEUUID uuid);
    bool        haveAppearance();
    bool        haveManufacturerData();
    bool        haveName();
    bool        haveRSSI();
    bool        haveServiceData();
    bool        haveServiceUUID();
    bool        haveTXPower();

    std::string toString();

private:
    friend class NimBLEScan;

    void parseAdvertisement(ble_hs_adv_fields *fields);
    void setAddress(NimBLEAddress address);
    void setAdvType(uint8_t advType);
    void setAdvertisementResult(uint8_t* payload, uint8_t length);
    void setAppearance(uint16_t appearance);
    void setManufacturerData(std::string manufacturerData);
    void setName(std::string name);
    void setRSSI(int rssi);
    void setScan(NimBLEScan* pScan);
    void setServiceData(std::string data);
    void setServiceDataUUID(NimBLEUUID uuid);
    void setServiceUUID(const char* serviceUUID);
    void setServiceUUID(NimBLEUUID serviceUUID);
    void setTXPower(int8_t txPower);

    bool m_haveAppearance;
    bool m_haveManufacturerData;
    bool m_haveName;
    bool m_haveRSSI;
    bool m_haveServiceData;
    bool m_haveServiceUUID;
    bool m_haveTXPower;


    NimBLEAddress  m_address = NimBLEAddress("\0\0\0\0\0\0");
    uint8_t         m_advType;
    uint16_t        m_appearance;
    int             m_deviceType;
    std::string     m_manufacturerData;
    std::string     m_name;
    NimBLEScan*     m_pScan;
    int             m_rssi;
    std::vector<NimBLEUUID> m_serviceUUIDs;
    int8_t          m_txPower;
    std::string     m_serviceData;
    NimBLEUUID      m_serviceDataUUID;
    uint8_t*        m_payload;
    size_t          m_payloadLength = 0;
    uint8_t         m_addressType;
};

/**
 * @brief A callback handler for callbacks associated device scanning.
 *
 * When we are performing a scan as a %BLE client, we may wish to know when a new device that is advertising
 * has been found.  This class can be sub-classed and registered such that when a scan is performed and
 * a new advertised device has been found, we will be called back to be notified.
 */
class NimBLEAdvertisedDeviceCallbacks {
public:
    virtual ~NimBLEAdvertisedDeviceCallbacks() {}
    /**
     * @brief Called when a new scan result is detected.
     *
     * As we are scanning, we will find new devices.  When found, this call back is invoked with a reference to the
     * device that was found.  During any individual scan, a device will only be detected one time.
     */
    //virtual void onResult(NimBLEAdvertisedDevice advertisedDevice) = 0;
    virtual void onResult(NimBLEAdvertisedDevice* advertisedDevice) = 0;
};

#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEADVERTISEDDEVICE_H_ */
