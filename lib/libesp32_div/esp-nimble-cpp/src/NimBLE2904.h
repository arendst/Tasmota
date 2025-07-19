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

#ifndef NIMBLE_CPP_2904_H_
#define NIMBLE_CPP_2904_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

# include "NimBLEDescriptor.h"

struct NimBLE2904Data {
    uint8_t  m_format{0};
    int8_t   m_exponent{0};
    uint16_t m_unit{0x2700};   // Unitless; See https://www.bluetooth.com/specifications/assigned-numbers/units
    uint8_t  m_namespace{1};   // 1 = Bluetooth SIG Assigned Numbers
    uint16_t m_description{0}; // unknown description
} __attribute__((packed));

/**
 * @brief Descriptor for Characteristic Presentation Format.
 *
 * This is a convenience descriptor for the Characteristic Presentation Format which has a UUID of 0x2904.
 */
class NimBLE2904 : public NimBLEDescriptor {
  public:
    NimBLE2904(NimBLECharacteristic* pChr = nullptr);
    static const uint8_t FORMAT_BOOLEAN   = 1;
    static const uint8_t FORMAT_UINT2     = 2;
    static const uint8_t FORMAT_UINT4     = 3;
    static const uint8_t FORMAT_UINT8     = 4;
    static const uint8_t FORMAT_UINT12    = 5;
    static const uint8_t FORMAT_UINT16    = 6;
    static const uint8_t FORMAT_UINT24    = 7;
    static const uint8_t FORMAT_UINT32    = 8;
    static const uint8_t FORMAT_UINT48    = 9;
    static const uint8_t FORMAT_UINT64    = 10;
    static const uint8_t FORMAT_UINT128   = 11;
    static const uint8_t FORMAT_SINT8     = 12;
    static const uint8_t FORMAT_SINT12    = 13;
    static const uint8_t FORMAT_SINT16    = 14;
    static const uint8_t FORMAT_SINT24    = 15;
    static const uint8_t FORMAT_SINT32    = 16;
    static const uint8_t FORMAT_SINT48    = 17;
    static const uint8_t FORMAT_SINT64    = 18;
    static const uint8_t FORMAT_SINT128   = 19;
    static const uint8_t FORMAT_FLOAT32   = 20;
    static const uint8_t FORMAT_FLOAT64   = 21;
    static const uint8_t FORMAT_SFLOAT16  = 22;
    static const uint8_t FORMAT_SFLOAT32  = 23;
    static const uint8_t FORMAT_IEEE20601 = 24;
    static const uint8_t FORMAT_UTF8      = 25;
    static const uint8_t FORMAT_UTF16     = 26;
    static const uint8_t FORMAT_OPAQUE    = 27;
    static const uint8_t FORMAT_MEDASN1   = 28;

    void setDescription(uint16_t description);
    void setExponent(int8_t exponent);
    void setFormat(uint8_t format);
    void setNamespace(uint8_t namespace_value);
    void setUnit(uint16_t unit);

  private:
    friend class NimBLECharacteristic;
    NimBLE2904Data m_data{};
}; // NimBLE2904

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // NIMBLE_CPP_2904_H_
