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

#ifndef NIMBLE_CPP_LOCAL_ATTRIBUTE_H_
#define NIMBLE_CPP_LOCAL_ATTRIBUTE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

# include "NimBLEAttribute.h"

/**
 * @brief A base class for local BLE attributes.
 */
class NimBLELocalAttribute : public NimBLEAttribute {
  public:
    /**
     * @brief Get the removed flag.
     * @return The removed flag.
     */
    uint8_t getRemoved() const { return m_removed; }

  protected:
    /**
     * @brief Construct a local attribute.
     */
    NimBLELocalAttribute(const NimBLEUUID& uuid, uint16_t handle) : NimBLEAttribute{uuid, handle}, m_removed{0} {}

    /**
     * @brief Destroy the local attribute.
     */
    ~NimBLELocalAttribute() = default;

    /**
     * @brief Set the removed flag.
     * @param [in] removed The removed flag.
     */
    void setRemoved(uint8_t removed) { m_removed = removed; }

    uint8_t m_removed{0};
};

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // NIMBLE_CPP_LOCAL_ATTRIBUTE_H_
