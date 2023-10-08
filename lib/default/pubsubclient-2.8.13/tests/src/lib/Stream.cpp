#include "Stream.h"
#include "trace.h"
#include <iostream>
#include <Arduino.h>

Stream::Stream() {
    this->expectBuffer = new Buffer();
    this->_error = false;
    this->_written = 0;
}

size_t Stream::write(uint8_t b)  {
    this->_written++;
    TRACE(std::hex << (unsigned int)b);
    if (this->expectBuffer->available()) {
        uint8_t expected = this->expectBuffer->next();
        if (expected != b) {
            this->_error = true;
            TRACE("!=" << (unsigned int)expected);
        }
    } else {
        this->_error = true;
    }
    TRACE("\n"<< std::dec);
    return 1;
}


bool Stream::error() {
    return this->_error;
}

void Stream::expect(uint8_t *buf, size_t size) {
    this->expectBuffer->add(buf,size);
}

uint16_t Stream::length() {
    return this->_written;
}
