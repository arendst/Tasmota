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

#ifndef NIMBLE_CPP_REMOTE_CHARACTERISTIC_H_
#define NIMBLE_CPP_REMOTE_CHARACTERISTIC_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL

# include "NimBLERemoteValueAttribute.h"
# include <vector>
# include <functional>

class NimBLEUUID;
class NimBLERemoteService;
class NimBLERemoteDescriptor;
struct NimBLEDescriptorFilter;

/**
 * @brief A model of a remote BLE characteristic.
 */
class NimBLERemoteCharacteristic : public NimBLERemoteValueAttribute {
  public:
    std::string                toString() const;
    const NimBLERemoteService* getRemoteService() const;
    void                       deleteDescriptors() const;
    size_t                     deleteDescriptor(const NimBLEUUID& uuid) const;
    bool                       canBroadcast() const;
    bool                       canRead() const;
    bool                       canWriteNoResponse() const;
    bool                       canWrite() const;
    bool                       canNotify() const;
    bool                       canIndicate() const;
    bool                       canWriteSigned() const;
    bool                       hasExtendedProps() const;
    NimBLEClient*              getClient() const override;
    uint8_t                    getProperties() const {return m_properties;};

    typedef std::function<void(NimBLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)> notify_callback;

    bool subscribe(bool notifications = true, const notify_callback notifyCallback = nullptr, bool response = true) const;
    bool unsubscribe(bool response = true) const;

    std::vector<NimBLERemoteDescriptor*>::iterator begin() const;
    std::vector<NimBLERemoteDescriptor*>::iterator end() const;
    NimBLERemoteDescriptor*                        getDescriptor(const NimBLEUUID& uuid) const;
    const std::vector<NimBLERemoteDescriptor*>&    getDescriptors(bool refresh = false) const;

  private:
    friend class NimBLEClient;
    friend class NimBLERemoteService;

    NimBLERemoteCharacteristic(const NimBLERemoteService* pRemoteService, const ble_gatt_chr* chr);
    ~NimBLERemoteCharacteristic();

    bool setNotify(uint16_t val, notify_callback notifyCallback = nullptr, bool response = true) const;
    bool retrieveDescriptors(NimBLEDescriptorFilter* pFilter = nullptr) const;

    static int descriptorDiscCB(
        uint16_t connHandle, const ble_gatt_error* error, uint16_t chrHandle, const ble_gatt_dsc* dsc, void* arg);

    const NimBLERemoteService*                   m_pRemoteService{nullptr};
    uint8_t                                      m_properties{0};
    mutable notify_callback                      m_notifyCallback{nullptr};
    mutable std::vector<NimBLERemoteDescriptor*> m_vDescriptors{};

}; // NimBLERemoteCharacteristic

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL */
#endif /* NIMBLE_CPP_REMOTE_CHARACTERISTIC_H_ */
