/*
 * NimBLEAdvertising.h
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEAdvertising.h
 *
 *  Created on: Jun 21, 2017
 *      Author: kolban
 */

#ifndef MAIN_BLEADVERTISING_H_
#define MAIN_BLEADVERTISING_H_
#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)

#if defined(CONFIG_NIMBLE_CPP_IDF)
#include "host/ble_gap.h"
#else
#include "nimble/nimble/host/include/host/ble_gap.h"
#endif

/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

#include "NimBLEUUID.h"

#include <vector>

/* COMPATIBILITY - DO NOT USE */
#define ESP_BLE_ADV_FLAG_LIMIT_DISC         (0x01 << 0)
#define ESP_BLE_ADV_FLAG_GEN_DISC           (0x01 << 1)
#define ESP_BLE_ADV_FLAG_BREDR_NOT_SPT      (0x01 << 2)
#define ESP_BLE_ADV_FLAG_DMT_CONTROLLER_SPT (0x01 << 3)
#define ESP_BLE_ADV_FLAG_DMT_HOST_SPT       (0x01 << 4)
#define ESP_BLE_ADV_FLAG_NON_LIMIT_DISC     (0x00 )
 /* ************************* */


/**
 * @brief Advertisement data set by the programmer to be published by the %BLE server.
 */
class NimBLEAdvertisementData {
    // Only a subset of the possible BLE architected advertisement fields are currently exposed.  Others will
    // be exposed on demand/request or as time permits.
    //
public:
    void setAppearance(uint16_t appearance);
    void setCompleteServices(const NimBLEUUID &uuid);
    void setCompleteServices16(const std::vector<NimBLEUUID> &v_uuid);
    void setCompleteServices32(const std::vector<NimBLEUUID> &v_uuid);
    void setFlags(uint8_t);
    void setManufacturerData(const std::string &data);
    void setURI(const std::string &uri);
    void setName(const std::string &name);
    void setPartialServices(const NimBLEUUID &uuid);
    void setPartialServices16(const std::vector<NimBLEUUID> &v_uuid);
    void setPartialServices32(const std::vector<NimBLEUUID> &v_uuid);
    void setServiceData(const NimBLEUUID &uuid, const std::string &data);
    void setShortName(const std::string &name);
    void addData(const std::string &data);  // Add data to the payload.
    void addData(char * data, size_t length);
    void addTxPower();
    void setPreferredParams(uint16_t min, uint16_t max);
    std::string getPayload();               // Retrieve the current advert payload.

private:
    friend class NimBLEAdvertising;
    void setServices(const bool complete, const uint8_t size,
                     const std::vector<NimBLEUUID> &v_uuid);
    std::string m_payload;   // The payload of the advertisement.
};   // NimBLEAdvertisementData


/**
 * @brief Perform and manage %BLE advertising.
 *
 * A %BLE server will want to perform advertising in order to make itself known to %BLE clients.
 */
class NimBLEAdvertising {
public:
    NimBLEAdvertising();
    void addServiceUUID(const NimBLEUUID &serviceUUID);
    void addServiceUUID(const char* serviceUUID);
    void removeServiceUUID(const NimBLEUUID &serviceUUID);
    bool start(uint32_t duration = 0, void (*advCompleteCB)(NimBLEAdvertising *pAdv) = nullptr);
    void stop();
    void setAppearance(uint16_t appearance);
    void setName(const std::string &name);
    void setManufacturerData(const std::string &data);
    void setURI(const std::string &uri);
    void setServiceData(const NimBLEUUID &uuid, const std::string &data);
    void setAdvertisementType(uint8_t adv_type);
    void setMaxInterval(uint16_t maxinterval);
    void setMinInterval(uint16_t mininterval);
    void setAdvertisementData(NimBLEAdvertisementData& advertisementData);
    void setScanFilter(bool scanRequestWhitelistOnly, bool connectWhitelistOnly);
    void setScanResponseData(NimBLEAdvertisementData& advertisementData);
    void setScanResponse(bool);
    void setMinPreferred(uint16_t);
    void setMaxPreferred(uint16_t);
    void addTxPower();
    void reset();
    void advCompleteCB();
    bool isAdvertising();

private:
    friend class NimBLEDevice;

    void                    onHostSync();
    static int              handleGapEvent(struct ble_gap_event *event, void *arg);

    ble_hs_adv_fields       m_advData;
    ble_hs_adv_fields       m_scanData;
    ble_gap_adv_params      m_advParams;
    std::vector<NimBLEUUID> m_serviceUUIDs;
    bool                    m_customAdvData;
    bool                    m_customScanResponseData;
    bool                    m_scanResp;
    bool                    m_advDataSet;
    void                    (*m_advCompCB)(NimBLEAdvertising *pAdv);
    uint8_t                 m_slaveItvl[4];
    uint32_t                m_duration;
    std::vector<uint8_t>    m_svcData16;
    std::vector<uint8_t>    m_svcData32;
    std::vector<uint8_t>    m_svcData128;
    std::vector<uint8_t>    m_name;
    std::vector<uint8_t>    m_mfgData;
    std::vector<uint8_t>    m_uri;
};

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER */
#endif /* MAIN_BLEADVERTISING_H_ */
