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

#ifndef NIMBLE_CPP_EDDYSTONETLM_H_
#define NIMBLE_CPP_EDDYSTONETLM_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER

class NimBLEUUID;

# include <string>

# define EDDYSTONE_TLM_FRAME_TYPE 0x20

/**
 * @brief Representation of a beacon.
 * See:
 * * https://github.com/google/eddystone
 */
class NimBLEEddystoneTLM {
  public:
    struct BeaconData {
        uint8_t  frameType{EDDYSTONE_TLM_FRAME_TYPE};
        uint8_t  version{0};
        uint16_t volt{3300};
        uint16_t temp{23 * 256};
        uint32_t advCount{0};
        uint32_t tmil{0};
    } __attribute__((packed));

    const BeaconData getData();
    NimBLEUUID       getUUID();
    uint8_t          getVersion();
    uint16_t         getVolt();
    int16_t          getTemp();
    uint32_t         getCount();
    uint32_t         getTime();
    std::string      toString();
    void             setData(const uint8_t* data, uint8_t length);
    void             setData(const BeaconData& data);
    void             setUUID(const NimBLEUUID& l_uuid);
    void             setVersion(uint8_t version);
    void             setVolt(uint16_t volt);
    void             setTemp(int16_t temp);
    void             setCount(uint32_t advCount);
    void             setTime(uint32_t tmil);

  private:
    uint16_t   beaconUUID{0xFEAA};
    BeaconData m_eddystoneData;

}; // NimBLEEddystoneTLM

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#endif // NIMBLE_CPP_EDDYSTONETLM_H_
