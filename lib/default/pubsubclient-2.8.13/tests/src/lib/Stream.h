#ifndef Stream_h
#define Stream_h

#include "Arduino.h"
#include "Buffer.h"

class Stream {
private:
    Buffer* expectBuffer;
    bool _error;
    uint16_t _written;

public:
    Stream();
    virtual size_t write(uint8_t);
    
    virtual bool error();
    virtual void expect(uint8_t *buf, size_t size);
    virtual uint16_t length();
};

#endif
