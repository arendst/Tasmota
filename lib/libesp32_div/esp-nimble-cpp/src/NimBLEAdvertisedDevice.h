/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NIMBLE_CPP_ADVERTISED_DEVICE_H_
#define NIMBLE_CPP_ADVERTISED_DEVICE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_OBSERVER

# include "NimBLEAddress.h"
# include "NimBLEScan.h"
# include "NimBLEUUID.h"

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "host/ble_hs_adv.h"
#  include "host/ble_gap.h"
# else
#  include "nimble/nimble/host/include/host/ble_hs_adv.h"
#  include "nimble/nimble/host/include/host/ble_gap.h"
# endif

# include <vector>

class NimBLEScan;
/**
 * @brief A representation of a %BLE advertised device found by a scan.
 *
 * When we perform a %BLE scan, the result will be a set of devices that are advertising.  This
 * class provides a model of a detected device.
 */
class NimBLEAdvertisedDevice {
  public:
    NimBLEAdvertisedDevice() = default;

    uint8_t              getAdvType() const;
    uint8_t              getAdvFlags() const;
    uint16_t             getAppearance() const;
    uint16_t             getAdvInterval() const;
    uint16_t             getMinInterval() const;
    uint16_t             getMaxInterval() const;
    uint8_t              getManufacturerDataCount() const;
    const NimBLEAddress& getAddress() const;
    std::string          getManufacturerData(uint8_t index = 0) const;
    std::string          getURI() const;
    std::string          getPayloadByType(uint16_t type, uint8_t index = 0) const;
    std::string          getName() const;
    int8_t               getRSSI() const;
    NimBLEScan*          getScan() const;
    uint8_t              getServiceDataCount() const;
    std::string          getServiceData(uint8_t index = 0) const;
    std::string          getServiceData(const NimBLEUUID& uuid) const;
    NimBLEUUID           getServiceDataUUID(uint8_t index = 0) const;
    NimBLEUUID           getServiceUUID(uint8_t index = 0) const;
    uint8_t              getServiceUUIDCount() const;
    NimBLEAddress        getTargetAddress(uint8_t index = 0) const;
    uint8_t              getTargetAddressCount() const;
    int8_t               getTXPower() const;
    uint8_t              getAdvLength() const;
    uint8_t              getAddressType() const;
    bool                 isAdvertisingService(const NimBLEUUID& uuid) const;
    bool                 haveAppearance() const;
    bool                 haveManufacturerData() const;
    bool                 haveName() const;
    bool                 haveServiceData() const;
    bool                 haveServiceUUID() const;
    bool                 haveTXPower() const;
    bool                 haveConnParams() const;
    bool                 haveAdvInterval() const;
    bool                 haveTargetAddress() const;
    bool                 haveURI() const;
    bool                 haveType(uint16_t type) const;
    std::string          toString() const;
    bool                 isConnectable() const;
    bool                 isScannable() const;
    bool                 isLegacyAdvertisement() const;
# if CONFIG_BT_NIMBLE_EXT_ADV
    uint8_t  getSetId() const;
    uint8_t  getPrimaryPhy() const;
    uint8_t  getSecondaryPhy() const;
    uint16_t getPeriodicInterval() const;
# endif
    operator NimBLEAddress() const;

    const std::vector<uint8_t>&                getPayload() const;
    const std::vector<uint8_t>::const_iterator begin() const;
    const std::vector<uint8_t>::const_iterator end() const;

    /**
     * @brief A template to convert the service data to <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getManufacturerData<type>(skipSizeCheck);</tt>
     */
    template <typename T>
    T getManufacturerData(bool skipSizeCheck = false) const {
        std::string data = getManufacturerData();
        if (!skipSizeCheck && data.size() < sizeof(T)) return T();
        const char* pData = data.data();
        return *((T*)pData);
    }

    /**
     * @brief A template to convert the service data to <tt><type\></tt>.
     * @tparam T The type to convert the data to.
     * @param [in] index The vector index of the service data requested.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getServiceData<type>(skipSizeCheck);</tt>
     */
    template <typename T>
    T getServiceData(uint8_t index = 0, bool skipSizeCheck = false) const {
        std::string data = getServiceData(index);
        if (!skipSizeCheck && data.size() < sizeof(T)) return T();
        const char* pData = data.data();
        return *((T*)pData);
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
    template <typename T>
    T getServiceData(const NimBLEUUID& uuid, bool skipSizeCheck = false) const {
        std::string data = getServiceData(uuid);
        if (!skipSizeCheck && data.size() < sizeof(T)) return T();
        const char* pData = data.data();
        return *((T*)pData);
    }

  private:
    friend class NimBLEScan;

    NimBLEAdvertisedDevice(const ble_gap_event* event, uint8_t eventType);
    void    update(const ble_gap_event* event, uint8_t eventType);
    uint8_t findAdvField(uint8_t type, uint8_t index = 0, size_t* data_loc = nullptr) const;
    size_t  findServiceData(uint8_t index, uint8_t* bytes) const;

    NimBLEAddress m_address{};
    uint8_t       m_advType{};
    int8_t        m_rssi{};
    uint8_t       m_callbackSent{};
    uint8_t       m_advLength{};

# if CONFIG_BT_NIMBLE_EXT_ADV
    bool     m_isLegacyAdv{};
    uint8_t  m_sid{};
    uint8_t  m_primPhy{};
    uint8_t  m_secPhy{};
    uint16_t m_periodicItvl{};
# endif

    std::vector<uint8_t> m_payload;
};

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_OBSERVER */
#endif /* NIMBLE_CPP_ADVERTISED_DEVICE_H_ */
