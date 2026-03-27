#include <functional>
#include "ets_sys.h"

class TwoWire;

class Twi {
private:
    unsigned int preferred_si2c_clock = 100000;
    uint32_t twi_dcount = 18;
    unsigned char twi_sda = 0;
    unsigned char twi_scl = 0;
    uint32_t twi_clockStretchLimit = 150000;

    // Internal use functions
    void ICACHE_RAM_ATTR busywait(unsigned int v);
    bool write_start(void);
    bool write_stop(void);
    bool write_bit(bool bit);
    bool read_bit(void);
    bool write_byte(unsigned char byte);
    unsigned char read_byte(bool nack);
    void WAIT_CLOCK_STRETCH();

    // Generate a clock "valley" (at the end of a segment, just before a repeated start)
    void twi_scl_valley(void);

public:
    void setClock(unsigned int freq);
    void setClockStretchLimit(uint32_t limit);
    void init(unsigned char sda, unsigned char scl);
    unsigned char writeTo(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop);
    unsigned char readFrom(unsigned char address, unsigned char* buf, unsigned int len, unsigned char sendStop);
    uint8_t status();
};