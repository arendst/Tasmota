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

#ifndef NIMBLE_CPP_REMOTE_SERVICE_H_
#define NIMBLE_CPP_REMOTE_SERVICE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL

# include "NimBLEAttribute.h"
# include <vector>

class NimBLERemoteCharacteristic;
class NimBLEClient;
class NimBLEAttValue;

/**
 * @brief A model of a remote BLE service.
 */
class NimBLERemoteService : public NimBLEAttribute {
  public:
    NimBLERemoteCharacteristic* getCharacteristic(const char* uuid) const;
    NimBLERemoteCharacteristic* getCharacteristic(const NimBLEUUID& uuid) const;
    void                        deleteCharacteristics() const;
    size_t                      deleteCharacteristic(const NimBLEUUID& uuid) const;
    NimBLEClient*               getClient(void) const;
    NimBLEAttValue              getValue(const NimBLEUUID& characteristicUuid) const;
    bool                        setValue(const NimBLEUUID& characteristicUuid, const NimBLEAttValue& value) const;
    std::string                 toString(void) const;
    uint16_t                    getStartHandle() const { return getHandle(); }
    uint16_t                    getEndHandle() const { return m_endHandle; }

    const std::vector<NimBLERemoteCharacteristic*>&    getCharacteristics(bool refresh = false) const;
    std::vector<NimBLERemoteCharacteristic*>::iterator begin() const;
    std::vector<NimBLERemoteCharacteristic*>::iterator end() const;

  private:
    friend class NimBLEClient;

    NimBLERemoteService(NimBLEClient* pClient, const struct ble_gatt_svc* service);
    ~NimBLERemoteService();
    bool       retrieveCharacteristics(const NimBLEUUID* uuidFilter = nullptr) const;
    static int characteristicDiscCB(uint16_t                     conn_handle,
                                    const struct ble_gatt_error* error,
                                    const struct ble_gatt_chr*   chr,
                                    void*                        arg);

    mutable std::vector<NimBLERemoteCharacteristic*> m_vChars{};
    NimBLEClient*                                    m_pClient{nullptr};
    uint16_t                                         m_endHandle{0};
}; // NimBLERemoteService

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL
#endif // NIMBLE_CPP_REMOTE_SERVICE_H_
