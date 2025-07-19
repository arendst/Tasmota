#include "CRC.h"

RadioLibCRC::RadioLibCRC() {

}

uint32_t RadioLibCRC::checksum(uint8_t* buff, size_t len) {
  uint32_t crc = this->init;
  size_t pos = 0;
  for(size_t i = 0; i < 8*len; i++) {
    if(i % 8 == 0) {
      uint32_t in = buff[pos++];
      if(this->refIn) {
        in = Module::reflect(in, 8);
      }
      crc ^= (in << (this->size - 8));
    }

    if(crc & ((uint32_t)1 << (this->size - 1))) {
      crc <<= (uint32_t)1;
      crc ^= this->poly;
    } else {
      crc <<= (uint32_t)1;
    }
  }

  crc ^= this->out;
  if(this->refOut) {
    crc = Module::reflect(crc, this->size);
  }
  crc &= (uint32_t)0xFFFFFFFF >> (32 - this->size);
  return(crc);
}

RadioLibCRC RadioLibCRCInstance;
