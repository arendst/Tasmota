#include "Buffer.h"
#include "Arduino.h"

Buffer::Buffer() {
    this->pos = 0;
    this->length = 0;
}

Buffer::Buffer(uint8_t* buf, size_t size) {
    this->pos = 0;
    this->length = 0;
    this->add(buf,size);
}
bool Buffer::available() {
    return this->pos < this->length;
}

uint8_t Buffer::next() {
    if (this->available()) {
        return this->buffer[this->pos++];
    }
    return 0;
}

void Buffer::reset() {
    this->pos = 0;
}

void Buffer::add(uint8_t* buf, size_t size) {
    uint16_t i = 0;
    for (;i<size;i++) {
        this->buffer[this->length++] = buf[i];
    }
}
