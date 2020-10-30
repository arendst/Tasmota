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

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)

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
    uint8_t         getAdvType();
    uint16_t        getAppearance();
    std::string     getManufacturerData();

    /**
     * @brief A template to convert the service data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getManufacturerData<type>(skipSizeCheck);</tt>
     */
    template<typename T>
    T       getManufacturerData(bool skipSizeCheck = false) {
        std::string data = getManufacturerData();
        if(!skipSizeCheck && data.size() < sizeof(T)) return T();
        const char *pData = data.data();
        return *((T *)pData);
    }

    std::string     getName();
    int             getRSSI();
    NimBLEScan*     getScan();
    size_t          getServiceDataCount();
    std::string     getServiceData(uint8_t index = 0);
    std::string     getServiceData(const NimBLEUUID &uuid) const;

    /**
     * @brief A template to convert the service data to <tt><type\></tt>.
     * @tparam T The type to convert the data to.
     * @param [in] index The vector index of the service data requested.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getServiceData<type>(skipSizeCheck);</tt>
     */
    template<typename T>
    T       getServiceData(uint8_t index = 0, bool skipSizeCheck = false) {
        std::string data = getServiceData(index);
        if(!skipSizeCheck && data.size() < sizeof(T)) return T();
        const char *pData = data.data();
        return *((T *)pData);
    }

    /**
     * @brief A template to convert the service data to <tt><type\></tt>.
     * @tparam T The type to convert the data to.
     * @param [in] uuid The uuid of the service data requested.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getServiceData<type>(skipSizeCheck);</tt>
     */
    template<typename T>
    T       getServiceData(const NimBLEUUID &uuid, bool skipSizeCheck = false) {
        std::string data = getServiceData(uuid);
        if(!skipSizeCheck && data.size() < sizeof(T)) return T();
        const char *pData = data.data();
        return *((T *)pData);
    }

    NimBLEUUID      getServiceDataUUID(uint8_t index = 0);
    NimBLEUUID      getServiceUUID(uint8_t index = 0);
    size_t          getServiceUUIDCount();
    int8_t          getTXPower();
    uint8_t*        getPayload();
    size_t          getPayloadLength();
    uint8_t         getAddressType();
    time_t          getTimestamp();
    bool            isAdvertisingService(const NimBLEUUID &uuid) const;
    bool            haveAppearance();
    bool            haveManufacturerData();
    bool            haveName();
    bool            haveRSSI();
    bool            haveServiceData();
    bool            haveServiceUUID();
    bool            haveTXPower();
    std::string     toString();

private:
    friend class NimBLEScan;

    void parseAdvertisement(uint8_t* payload, uint8_t length);
    void setAddress(NimBLEAddress address);
    void setAdvType(uint8_t advType);
    void setAppearance(uint16_t appearance);
    void setManufacturerData(std::string manufacturerData);
    void setName(std::string name);
    void setRSSI(int rssi);
    void setServiceData(NimBLEUUID serviceUUID, std::string data);
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


    NimBLEAddress   m_address = NimBLEAddress("");
    uint8_t         m_advType;
    uint16_t        m_appearance;
    std::string     m_manufacturerData;
    std::string     m_name;
    int             m_rssi;
    int8_t          m_txPower;
    uint8_t*        m_payload;
    size_t          m_payloadLength;
    time_t          m_timestamp;
    bool            m_callbackSent;

    std::vector<NimBLEUUID> m_serviceUUIDs;
    std::vector<std::pair<NimBLEUUID, std::string>>m_serviceDataVec;
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
    virtual void onResult(NimBLEAdvertisedDevice* advertisedDevice) = 0;
};

#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEADVERTISEDDEVICE_H_ */
