/*
  IPAddress.cpp - Host-side Arduino `IPAddress` shim implementation.
*/

#include "IPAddress.h"

IPAddress::IPAddress() {
    _octets[0] = 0;
    _octets[1] = 0;
    _octets[2] = 0;
    _octets[3] = 0;
}

IPAddress::IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth) {
    _octets[0] = first;
    _octets[1] = second;
    _octets[2] = third;
    _octets[3] = fourth;
}

uint8_t IPAddress::operator[](int index) const {
    return _octets[index & 3];
}

uint8_t& IPAddress::operator[](int index) {
    return _octets[index & 3];
}

bool IPAddress::operator==(const IPAddress& other) const {
    return _octets[0] == other._octets[0] &&
           _octets[1] == other._octets[1] &&
           _octets[2] == other._octets[2] &&
           _octets[3] == other._octets[3];
}

bool IPAddress::operator!=(const IPAddress& other) const {
    return !(*this == other);
}
