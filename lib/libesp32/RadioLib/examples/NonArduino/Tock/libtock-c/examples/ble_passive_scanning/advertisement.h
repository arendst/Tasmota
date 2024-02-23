#pragma once

#include <cstdio>
#include <cstring>

const unsigned char HEADER_START = 0;
const unsigned char HEADER_SIZE = 2;
const unsigned char ADDRESS_START = 2;
const unsigned char ADDRESS_SIZE = 6;
const unsigned char DATA_START = 8;
const unsigned char DATA_MAX_SIZE = 31;

const unsigned char ADV_MIN_SIZE = 8;
const unsigned char ADV_MAX_SIZE = HEADER_SIZE + ADDRESS_SIZE + DATA_MAX_SIZE;

const unsigned char PDU_LEN_HEADER_MASK = 0x3F;
const unsigned char PDU_TYPE_HEADER_MASK = 0xF;
const unsigned char PDU_TXADD_HEADER_MASK = 0x40;
const unsigned char PDU_RXADD_HEADER_MASK = 0x80;

class Advertisement {
  private:
    unsigned char header_[HEADER_SIZE];
    unsigned char address_[ADDRESS_SIZE];
    unsigned char data_[DATA_MAX_SIZE];

  public:
    // Constructors
    Advertisement(const unsigned char* buf, int len);
    Advertisement();

    // Methods
    bool device_detected(const Advertisement& other) const;
    bool operator==(const Advertisement& other) const;
    bool operator!=(const Advertisement& other) const;
    unsigned char pduType() const;
    const char* pduTypeStr() const;
    bool pduTxAddSet() const;
    bool pduRxAddSet() const;
    unsigned char pduLength() const;
    void print() const;
    static bool checkScanResult(const unsigned char* buf, int len);
};
