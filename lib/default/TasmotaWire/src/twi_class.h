#include <functional>

#include "twi_util.h"
#include "ets_sys.h"

class TwoWire;
typedef std::function<void(void)> txEvent;
typedef std::function<void(uint8_t*,size_t)> rxEvent;
class Twi
{
private:
    unsigned int preferred_si2c_clock = 100000;
    uint32_t twi_dcount = 18;
    unsigned char twi_sda = 0;
    unsigned char twi_scl = 0;
    unsigned char twi_addr = 0;
    uint32_t twi_clockStretchLimit = 0;

    // These are int-wide, even though they could all fit in a byte, to reduce code size and avoid any potential
    // issues about RmW on packed bytes.  The int-wide variations of asm instructions are smaller than the equivalent
    // byte-wide ones, and since these emums are used everywhere, the difference adds up fast.  There is only a single
    // instance of the class, though, so the extra 12 bytes of RAM used here saves a lot more IRAM.
    volatile enum { TWIPM_UNKNOWN = 0, TWIPM_IDLE, TWIPM_ADDRESSED, TWIPM_WAIT} twip_mode = TWIPM_IDLE;
    volatile enum { TWIP_UNKNOWN = 0, TWIP_IDLE, TWIP_START, TWIP_SEND_ACK, TWIP_WAIT_ACK, TWIP_WAIT_STOP, TWIP_SLA_W, TWIP_SLA_R, TWIP_REP_START, TWIP_READ, TWIP_STOP, TWIP_REC_ACK, TWIP_READ_ACK, TWIP_RWAIT_ACK, TWIP_WRITE, TWIP_BUS_ERR } twip_state = TWIP_IDLE;
    volatile int twip_status = TW_NO_INFO;
    volatile int bitCount = 0;

    volatile uint8_t twi_data = 0x00;
    volatile int twi_ack = 0;
    volatile int twi_ack_rec = 0;
    volatile int twi_timeout_ms = 10;

    volatile enum { TWI_READY = 0, TWI_MRX, TWI_MTX, TWI_SRX, TWI_STX } twi_state = TWI_READY;
    volatile uint8_t twi_error = 0xFF;

    uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
    volatile int twi_txBufferIndex = 0;
    volatile int twi_txBufferLength = 0;

    uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
    volatile int twi_rxBufferIndex = 0;

    txEvent twi_onSlaveTransmit;
    rxEvent twi_onSlaveReceive;
    //void (*twi_onSlaveTransmit)(void);
    //void (*twi_onSlaveReceive)(uint8_t*, size_t);

    // ETS queue/timer interfaces
    enum { EVENTTASK_QUEUE_SIZE = 1, EVENTTASK_QUEUE_PRIO = 2 };
    enum { TWI_SIG_RANGE = 0x00000100, TWI_SIG_RX = 0x00000101, TWI_SIG_TX = 0x00000102 };
    ETSEvent eventTaskQueue[EVENTTASK_QUEUE_SIZE];
    ETSTimer timer;

    // Event/IRQ callbacks, so they can't use "this" and need to be static
    //void ICACHE_RAM_ATTR onSclChange(void);
    //void ICACHE_RAM_ATTR onSdaChange(void);

    // Allow not linking in the slave code if there is no call to setAddress
    bool _slaveEnabled = false;

    // Internal use functions
    void ICACHE_RAM_ATTR busywait(unsigned int v);
    bool write_start(void);
    bool write_stop(void);
    bool write_bit(bool bit);
    bool read_bit(void);
    bool write_byte(unsigned char byte);
    unsigned char read_byte(bool nack);
    void ICACHE_RAM_ATTR onTwipEvent(uint8_t status);
    void WAIT_CLOCK_STRETCH();

    // Generate a clock "valley" (at the end of a segment, just before a repeated start)
    void twi_scl_valley(void);

public:
    //was private
    void eventTask(ETSEvent *e);
    void ICACHE_RAM_ATTR onTimer(void *unused);
    void ICACHE_RAM_ATTR onSclChange(void);
    void ICACHE_RAM_ATTR onSdaChange(void);

    //
    void setClock(unsigned int freq);
    void setClockStretchLimit(uint32_t limit);
    void init(unsigned char sda, unsigned char scl);
    void setAddress(uint8_t address);
    unsigned char writeTo(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop);
    unsigned char readFrom(unsigned char address, unsigned char* buf, unsigned int len, unsigned char sendStop);
    uint8_t status();
    uint8_t transmit(const uint8_t* data, uint8_t length);
    void attachSlaveRxEvent(rxEvent fn);
    void attachSlaveRxEvent(void (*fn)(uint8_t*, size_t));
    void attachSlaveTxEvent(txEvent fn);
    void attachSlaveTxEvent(void (*fn)(void));
    void ICACHE_RAM_ATTR reply(uint8_t ack);
    void ICACHE_RAM_ATTR releaseBus(void);
    void enableSlave();
};