/*
  IPAddress.h - Host-side Arduino `IPAddress` shim for TasmotaPubSub host tests.

  Minimal reproduction of the Arduino `IPAddress` type. The library holds an
  `IPAddress ip;` member, constructs one from 4 octets in setServer(uint8_t*),
  copies/assigns it, and passes it to `Client::connect(IPAddress, uint16_t)`.
  Equality is provided for completeness (used when comparing endpoints).
*/

#ifndef TASMOTA_PUBSUB_TEST_IPADDRESS_H
#define TASMOTA_PUBSUB_TEST_IPADDRESS_H

#include <cstdint>

class IPAddress {
public:
    IPAddress();
    IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);

    // Access an individual octet (0..3). Mirrors the Arduino operator[].
    uint8_t  operator[](int index) const;
    uint8_t& operator[](int index);

    bool operator==(const IPAddress& other) const;
    bool operator!=(const IPAddress& other) const;

private:
    uint8_t _octets[4];
};

#endif  // TASMOTA_PUBSUB_TEST_IPADDRESS_H
