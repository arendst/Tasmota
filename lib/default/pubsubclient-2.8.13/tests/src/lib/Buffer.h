#ifndef buffer_h
#define buffer_h

#include "Arduino.h"

class Buffer {
private:
    uint8_t buffer[2048];
    uint16_t pos;
    uint16_t length;

public:
    Buffer();
    Buffer(uint8_t* buf, size_t size);

    virtual bool available();
    virtual uint8_t next();
    virtual void reset();

    virtual void add(uint8_t* buf, size_t size);
};

#endif
