/*
    si2c.c - Software I2C library for esp8266

    Copyright (c) 2015 Hristo Gochkov. All rights reserved.
    This file is part of the esp8266 core for Arduino environment.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
    Modified January 2017 by Bjorn Hammarberg (bjoham@esp8266.com) - i2c slave support
*/
#include "twi.h"
#include "twi_class.h"
#include "pins_arduino.h"
#include "wiring_private.h"
#include "PolledTimeout.h"

extern "C"
{
#include "twi_util.h"
#include "ets_sys.h"
};

/*** so bad ****/
// https://github.com/esp8266/Arduino/issues/3063#issuecomment-786266957
#define MAX_TWI_INSTANCES 5
typedef void (*fn_timer)(void *);
typedef void (*fn_event)(ETSEvent *e);
typedef void (*fn_sclChange)();
typedef void (*fn_sdaChange)();

int timers = 0;
Twi *twi_instances[5];
void ICACHE_RAM_ATTR timer0(void *n)
{
    twi_instances[0]->onTimer(n);
}
void ICACHE_RAM_ATTR timer1(void *n)
{
    twi_instances[1]->onTimer(n);
}
void ICACHE_RAM_ATTR timer2(void *n)
{
    twi_instances[2]->onTimer(n);
}
void ICACHE_RAM_ATTR timer3(void *n)
{
    twi_instances[3]->onTimer(n);
}
void ICACHE_RAM_ATTR timer4(void *n)
{
    twi_instances[4]->onTimer(n);
}
void eventTask0(ETSEvent *e)
{
    twi_instances[0]->eventTask(e);
}
void eventTask1(ETSEvent *e)
{
    twi_instances[1]->eventTask(e);
}
void eventTask2(ETSEvent *e)
{
    twi_instances[2]->eventTask(e);
}
void eventTask3(ETSEvent *e)
{
    twi_instances[3]->eventTask(e);
}
void eventTask4(ETSEvent *e)
{
    twi_instances[4]->eventTask(e);
}

void ICACHE_RAM_ATTR sclChange0()
{
    twi_instances[0]->onSclChange();
}

void ICACHE_RAM_ATTR sdaChange0()
{
    twi_instances[0]->onSdaChange();
}

void ICACHE_RAM_ATTR sclChange1()
{
    twi_instances[1]->onSclChange();
}

void ICACHE_RAM_ATTR sdaChange1()
{
    twi_instances[1]->onSdaChange();
}

void ICACHE_RAM_ATTR sclChange2()
{
    twi_instances[2]->onSclChange();
}

void ICACHE_RAM_ATTR sdaChange2()
{
    twi_instances[2]->onSdaChange();
}

void ICACHE_RAM_ATTR sclChange3()
{
    twi_instances[3]->onSclChange();
}

void ICACHE_RAM_ATTR sdaChange3()
{
    twi_instances[3]->onSdaChange();
}

void ICACHE_RAM_ATTR sclChange4()
{
    twi_instances[4]->onSclChange();
}

void ICACHE_RAM_ATTR sdaChange4()
{
    twi_instances[4]->onSdaChange();
}

fn_event twi_eventTasks[] = {&eventTask0, &eventTask1, &eventTask2, &eventTask3, &eventTask4};
fn_sclChange twi_sdaChanges[] = {&sdaChange0, &sdaChange1, &sdaChange2, &sdaChange3, &sdaChange4};
fn_sdaChange twi_sclChanges[] = {&sclChange0, &sclChange1, &sclChange2, &sclChange3, &sclChange4};
fn_timer twi_timers[] = {&timer0, &timer1, &timer2, &timer3, &timer4};

fn_timer getTimer(Twi *obj)
{
    for (int i = 0; i < MAX_TWI_INSTANCES; i++)
        if (obj == twi_instances[i])
            return twi_timers[i];

    if (timers < MAX_TWI_INSTANCES)
        return twi_timers[timers++];

    return NULL;
}

fn_event getEventTask(Twi *obj)
{
    for (int i = 0; i < MAX_TWI_INSTANCES; i++)
        if (obj == twi_instances[i])
            return twi_eventTasks[i];

    if (timers < MAX_TWI_INSTANCES)
        return twi_eventTasks[timers++];

    return NULL;
}

fn_sclChange getSclChange(Twi *obj)
{
    for (int i = 0; i < MAX_TWI_INSTANCES; i++)
        if (obj == twi_instances[i])
            return twi_sclChanges[i];

    if (timers < MAX_TWI_INSTANCES)
        return twi_sclChanges[timers++];

    return NULL;
}

fn_sdaChange getSdaChange(Twi *obj)
{
    for (int i = 0; i < MAX_TWI_INSTANCES; i++)
        if (obj == twi_instances[i])
            return twi_sdaChanges[i];

    if (timers < MAX_TWI_INSTANCES)
        return twi_sdaChanges[timers++];

    return NULL;
}

/* end soooooo baaaaaad */

class txEventClass
{
    void (*fn)(void);

public:
    txEventClass(void (*thefn)(void)) { fn = thefn; }
    void callback() { fn(); }
};

class rxEventClass
{
    void (*fn)(uint8_t *, size_t);

public:
    rxEventClass(void (*thefn)(uint8_t *, size_t)) { fn = thefn; }
    void callback(uint8_t *buff, size_t size) { fn(buff, size); }
};

// Inline helpers
static inline __attribute__((always_inline)) void SDA_LOW(const int twi_sda)
{
    GPES = (1 << twi_sda);
}
static inline __attribute__((always_inline)) void SDA_HIGH(const int twi_sda)
{
    GPEC = (1 << twi_sda);
}
static inline __attribute__((always_inline)) bool SDA_READ(const int twi_sda)
{
    return (GPI & (1 << twi_sda)) != 0;
}
static inline __attribute__((always_inline)) void SCL_LOW(const int twi_scl)
{
//    GPES = (1 << twi_scl);
    (twi_scl != 16) ? GPES = (1 << twi_scl) : GP16O &= ~1;
}
static inline __attribute__((always_inline)) void SCL_HIGH(const int twi_scl)
{
//    GPEC = (1 << twi_scl);
    (twi_scl != 16) ? GPEC = (1 << twi_scl) : GP16O |= 1;
}
static inline __attribute__((always_inline)) bool SCL_READ(const int twi_scl)
{
//    return (GPI & (1 << twi_scl)) != 0;
    return (twi_scl != 16) ? (GPI & (1 << twi_scl)) != 0 : (GP16I & 0x01);
}

// Implement as a class to reduce code size by allowing access to many global variables with a single base pointer

#ifndef FCPU80
#define FCPU80 80000000L
#endif

// Handle the case where a slave needs to stretch the clock with a time-limited busy wait
void Twi::WAIT_CLOCK_STRETCH()
{
    esp8266::polledTimeout::oneShotFastUs timeout(twi_clockStretchLimit);
    esp8266::polledTimeout::periodicFastUs yieldTimeout(5000);
    while (!timeout && !SCL_READ(twi_scl)) // outer loop is stretch duration up to stretch limit
    {
        if (yieldTimeout) // inner loop yields every 5ms
        {
            yield();
        }
    }
}

void Twi::setClock(unsigned int freq)
{
    if (freq < 1000) // minimum freq 1000Hz to minimize slave timeouts and WDT resets
    {
        freq = 1000;
    }

    preferred_si2c_clock = freq;

#if F_CPU == FCPU80

    if (freq > 400000)
    {
        freq = 400000;
    }
    twi_dcount = (500000000 / freq);                   // half-cycle period in ns
    twi_dcount = (1000 * (twi_dcount - 1120)) / 62500; // (half cycle - overhead) / busywait loop time

#else

    if (freq > 800000)
    {
        freq = 800000;
    }
    twi_dcount = (500000000 / freq);                  // half-cycle period in ns
    twi_dcount = (1000 * (twi_dcount - 560)) / 31250; // (half cycle - overhead) / busywait loop time

#endif
}

void Twi::setClockStretchLimit(uint32_t limit)
{
    twi_clockStretchLimit = limit;
}

void Twi::init(unsigned char sda, unsigned char scl)
{
    // set timer function
    fn_timer fnt = getTimer(this);
    //ets_timer_setfn(&timer, onTimer, NULL);
    ets_timer_setfn(&timer, fnt, NULL);

    // create event task
    fn_event fne = getEventTask(this);
    //ets_task(eventTask, EVENTTASK_QUEUE_PRIO, eventTaskQueue, EVENTTASK_QUEUE_SIZE);
    ets_task(fne, EVENTTASK_QUEUE_PRIO, eventTaskQueue, EVENTTASK_QUEUE_SIZE);

    twi_sda = sda;
    twi_scl = scl;
    pinMode(twi_sda, INPUT_PULLUP);
//    pinMode(twi_scl, INPUT_PULLUP);
    if (16 == twi_scl) {
      pinMode(twi_scl, OUTPUT);
    } else {
      pinMode(twi_scl, INPUT_PULLUP);
    }
    twi_setClock(preferred_si2c_clock);
    twi_setClockStretchLimit(150000L); // default value is 150 mS
}

void Twi::setAddress(uint8_t address)
{
    // set twi slave address (skip over R/W bit)
    twi_addr = address << 1;
}

void Twi::enableSlave()
{
    if (!_slaveEnabled)
    {
        //attachInterrupt(twi_scl, onSclChange, CHANGE);
        //attachInterrupt(twi_sda, onSdaChange, CHANGE);

        attachInterrupt(twi_scl, getSclChange(this), CHANGE);
        attachInterrupt(twi_sda, getSdaChange(this), CHANGE);
        _slaveEnabled = true;
    }
}

void ICACHE_RAM_ATTR Twi::busywait(unsigned int v)
{
    unsigned int i;
    for (i = 0; i < v; i++) // loop time is 5 machine cycles: 31.25ns @ 160MHz, 62.5ns @ 80MHz
    {
        asm("nop"); // minimum element to keep GCC from optimizing this function out.
    }
}

bool Twi::write_start(void)
{
    SCL_HIGH(twi_scl);
    SDA_HIGH(twi_sda);
    if (!SDA_READ(twi_sda))
    {
        return false;
    }
    busywait(twi_dcount);
    SDA_LOW(twi_sda);
    busywait(twi_dcount);
    return true;
}

bool Twi::write_stop(void)
{
    SCL_LOW(twi_scl);
    SDA_LOW(twi_sda);
    busywait(twi_dcount);
    SCL_HIGH(twi_scl);
    WAIT_CLOCK_STRETCH();
    busywait(twi_dcount);
    SDA_HIGH(twi_sda);
    busywait(twi_dcount);
    return true;
}

bool Twi::write_bit(bool bit)
{
    SCL_LOW(twi_scl);
    if (bit)
    {
        SDA_HIGH(twi_sda);
    }
    else
    {
        SDA_LOW(twi_sda);
    }
    busywait(twi_dcount + 1);
    SCL_HIGH(twi_scl);
    WAIT_CLOCK_STRETCH();
    busywait(twi_dcount);
    return true;
}

bool Twi::read_bit(void)
{
    SCL_LOW(twi_scl);
    SDA_HIGH(twi_sda);
    busywait(twi_dcount + 2);
    SCL_HIGH(twi_scl);
    WAIT_CLOCK_STRETCH();
    bool bit = SDA_READ(twi_sda);
    busywait(twi_dcount);
    return bit;
}

bool Twi::write_byte(unsigned char byte)
{
    unsigned char bit;
    for (bit = 0; bit < 8; bit++)
    {
        write_bit(byte & 0x80);
        byte <<= 1;
    }
    return !read_bit(); //NACK/ACK
}

unsigned char Twi::read_byte(bool nack)
{
    unsigned char byte = 0;
    unsigned char bit;
    for (bit = 0; bit < 8; bit++)
    {
        byte = (byte << 1) | read_bit();
    }
    write_bit(nack);
    return byte;
}

unsigned char Twi::writeTo(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop)
{
    unsigned int i;
    if (!write_start())
    {
        return 4; //line busy
    }
    if (!write_byte(((address << 1) | 0) & 0xFF))
    {
        if (sendStop)
        {
            write_stop();
        }
        return 2; //received NACK on transmit of address
    }
    for (i = 0; i < len; i++)
    {
        if (!write_byte(buf[i]))
        {
            if (sendStop)
            {
                write_stop();
            }
            return 3; //received NACK on transmit of data
        }
    }
    if (sendStop)
    {
        write_stop();
    }
    else
    {
        twi_scl_valley();
        // TD-er: Also busywait(twi_dcount) here?
        // busywait(twi_dcount);
    }
    i = 0;
    while (!SDA_READ(twi_sda) && (i++) < 10)
    {
        twi_scl_valley();
        busywait(twi_dcount);
    }
    return 0;
}

unsigned char Twi::readFrom(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop)
{
    unsigned int i;
    if (!write_start())
    {
        return 4; //line busy
    }
    if (!write_byte(((address << 1) | 1) & 0xFF))
    {
        if (sendStop)
        {
            write_stop();
        }
        return 2; //received NACK on transmit of address
    }
    for (i = 0; i < (len - 1); i++)
    {
        buf[i] = read_byte(false);
    }
    buf[len - 1] = read_byte(true);
    if (sendStop)
    {
        write_stop();
    }
    else
    {
        twi_scl_valley();
        // TD-er: Also busywait(twi_dcount) here?
        // busywait(twi_dcount);
    }
    i = 0;
    while (!SDA_READ(twi_sda) && (i++) < 10)
    {
        twi_scl_valley();
        busywait(twi_dcount);
    }
    return 0;
}

void Twi::twi_scl_valley(void)
{
    SCL_LOW(twi_scl);
    busywait(twi_dcount);
    SCL_HIGH(twi_scl);
    WAIT_CLOCK_STRETCH();
}

uint8_t Twi::status()
{
    WAIT_CLOCK_STRETCH(); // wait for a slow slave to finish
    if (!SCL_READ(twi_scl))
    {
        return I2C_SCL_HELD_LOW; // SCL held low by another device, no procedure available to recover
    }

    int clockCount = 20;
    while (!SDA_READ(twi_sda) && clockCount-- > 0) // if SDA low, read the bits slaves have to sent to a max
    {
        read_bit();
        if (!SCL_READ(twi_scl))
        {
            return I2C_SCL_HELD_LOW_AFTER_READ; // I2C bus error. SCL held low beyond slave clock stretch time
        }
    }
    if (!SDA_READ(twi_sda))
    {
        return I2C_SDA_HELD_LOW; // I2C bus error. SDA line held low by slave/another_master after n bits.
    }

    return I2C_OK;
}

uint8_t Twi::transmit(const uint8_t *data, uint8_t length)
{
    uint8_t i;

    // ensure data will fit into buffer
    if (length > TWI_BUFFER_LENGTH)
    {
        return 1;
    }

    // ensure we are currently a slave transmitter
    if (twi_state != TWI_STX)
    {
        return 2;
    }

    // set length and copy data into tx buffer
    twi_txBufferLength = length;
    for (i = 0; i < length; ++i)
    {
        twi_txBuffer[i] = data[i];
    }

    return 0;
}

void Twi::attachSlaveRxEvent(rxEvent fn)
{
    twi_onSlaveReceive = fn;
}

void Twi::attachSlaveRxEvent(void (*fn)(uint8_t *, size_t))
{
    //leak must destroy class after
    rxEventClass rx(fn);
    twi_onSlaveReceive = std::bind(&rxEventClass::callback, &rx, std::placeholders::_1, std::placeholders::_2);
}

void Twi::attachSlaveTxEvent(txEvent fn)
{
    twi_onSlaveTransmit = fn;
}

void Twi::attachSlaveTxEvent(void (*fn)(void))
{
    //leak must destroy class after
    txEventClass tx(fn);
    twi_onSlaveTransmit = std::bind(&txEventClass::callback, &tx);
}

// DO NOT INLINE, inlining reply() in combination with compiler optimizations causes function breakup into
// parts and the ICACHE_RAM_ATTR isn't propagated correctly to all parts, which of course causes crashes.
// TODO: test with gcc 9.x and if it still fails, disable optimization with -fdisable-ipa-fnsplit
void ICACHE_RAM_ATTR Twi::reply(uint8_t ack)
{
    // transmit master read ready signal, with or without ack
    if (ack)
    {
        //TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
        SCL_HIGH(twi_scl); // _BV(TWINT)
        twi_ack = 1;       // _BV(TWEA)
    }
    else
    {
        //TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
        SCL_HIGH(twi_scl); // _BV(TWINT)
        twi_ack = 0;       // ~_BV(TWEA)
    }
}

void ICACHE_RAM_ATTR Twi::releaseBus(void)
{
    // release bus
    //TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
    SCL_HIGH(twi_scl); // _BV(TWINT)
    twi_ack = 1;       // _BV(TWEA)
    SDA_HIGH(twi_sda);

    // update twi state
    twi_state = TWI_READY;
}

void ICACHE_RAM_ATTR Twi::onTwipEvent(uint8_t status)
{
    twip_status = status;
    switch (status)
    {
    // Slave Receiver
    case TW_SR_SLA_ACK:            // addressed, returned ack
    case TW_SR_GCALL_ACK:          // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
        // enter slave receiver mode
        twi_state = TWI_SRX;
        // indicate that rx buffer can be overwritten and ack
        twi_rxBufferIndex = 0;
        reply(1);
        break;
    case TW_SR_DATA_ACK:       // data received, returned ack
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
        // if there is still room in the rx buffer
        if (twi_rxBufferIndex < TWI_BUFFER_LENGTH)
        {
            // put byte in buffer and ack
            twi_rxBuffer[twi_rxBufferIndex++] = twi_data;
            reply(1);
        }
        else
        {
            // otherwise nack
            reply(0);
        }
        break;
    case TW_SR_STOP: // stop or repeated start condition received
        // put a null char after data if there's room
        if (twi_rxBufferIndex < TWI_BUFFER_LENGTH)
        {
            twi_rxBuffer[twi_rxBufferIndex] = '\0';
        }
        // callback to user-defined callback over event task to allow for non-RAM-residing code
        //twi_rxBufferLock = true; // This may be necessary
        ets_post(EVENTTASK_QUEUE_PRIO, TWI_SIG_RX, twi_rxBufferIndex);

        // since we submit rx buffer to "wire" library, we can reset it
        twi_rxBufferIndex = 0;
        break;

    case TW_SR_DATA_NACK:       // data received, returned nack
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
        // nack back at master
        reply(0);
        break;

    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
        // enter slave transmitter mode
        twi_state = TWI_STX;
        // ready the tx buffer index for iteration
        twi_txBufferIndex = 0;
        // set tx buffer length to be zero, to verify if user changes it
        twi_txBufferLength = 0;
        // callback to user-defined callback over event task to allow for non-RAM-residing code
        // request for txBuffer to be filled and length to be set
        // note: user must call twi_transmit(bytes, length) to do this
        ets_post(EVENTTASK_QUEUE_PRIO, TWI_SIG_TX, 0);
        break;

    case TW_ST_DATA_ACK: // byte sent, ack returned
        // copy data to output register
        twi_data = twi_txBuffer[twi_txBufferIndex++];

        bitCount = 8;
        bitCount--;
        if (twi_data & 0x80)
        {
            SDA_HIGH(twi_sda);
        }
        else
        {
            SDA_LOW(twi_sda);
        }
        twi_data <<= 1;

        // if there is more to send, ack, otherwise nack
        if (twi_txBufferIndex < twi_txBufferLength)
        {
            reply(1);
        }
        else
        {
            reply(0);
        }
        break;
    case TW_ST_DATA_NACK: // received nack, we are done
    case TW_ST_LAST_DATA: // received ack, but we are done already!
        // leave slave receiver state
        releaseBus();
        break;

    // All
    case TW_NO_INFO: // no state information
        break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
        twi_error = TW_BUS_ERROR;
        break;
    }
}

void ICACHE_RAM_ATTR Twi::onTimer(void *unused)
{
    (void)unused;
    releaseBus();
    onTwipEvent(TW_BUS_ERROR);
    twip_mode = TWIPM_WAIT;
    twip_state = TWIP_BUS_ERR;
}

void Twi::eventTask(ETSEvent *e)
{

    if (e == NULL)
    {
        return;
    }

    switch (e->sig)
    {
    case TWI_SIG_TX:
        twi_onSlaveTransmit();

        // if they didn't change buffer & length, initialize it
        if (twi_txBufferLength == 0)
        {
            twi_txBufferLength = 1;
            twi_txBuffer[0] = 0x00;
        }

        // Initiate transmission
        onTwipEvent(TW_ST_DATA_ACK);

        break;

    case TWI_SIG_RX:
        // ack future responses and leave slave receiver state
        releaseBus();
        twi_onSlaveReceive(twi_rxBuffer, e->par);
        break;  
    }
}

// The state machine is converted from a 0...15 state to a 1-hot encoded state, and then
// compared to the logical-or of all states with the same branch.  This removes the need
// for a large series of straight-line compares.  The biggest win is when multiple states
// all have the same branch (onSdaChange), but for others there is some benefit, still.
#define S2M(x) (1 << (x))
// Shorthand for if the state is any of the or'd bitmask x
#define IFSTATE(x) if (twip_state_mask & (x))

void ICACHE_RAM_ATTR Twi::onSclChange(void)
{
    unsigned int sda;
    unsigned int scl;

    // Store bool return in int to reduce final code size.

    sda = SDA_READ(twi_sda);
    scl = SCL_READ(twi_scl);

    twip_status = 0xF8; // reset TWI status

    int twip_state_mask = S2M(twip_state);
    IFSTATE(S2M(TWIP_START) | S2M(TWIP_REP_START) | S2M(TWIP_SLA_W) | S2M(TWIP_READ))
    {
        if (!scl)
        {
            // ignore
        }
        else
        {
            bitCount--;
            twi_data <<= 1;
            twi_data |= sda;

            if (bitCount != 0)
            {
                // continue
            }
            else
            {
                twip_state = TWIP_SEND_ACK;
            }
        }
    }
    else IFSTATE(S2M(TWIP_SEND_ACK))
    {
        if (scl)
        {
            // ignore
        }
        else
        {
            if (twip_mode == TWIPM_IDLE)
            {
                if ((twi_data & 0xFE) != twi_addr)
                {
                    // ignore
                }
                else
                {
                    SDA_LOW(twi_sda);
                }
            }
            else
            {
                if (!twi_ack)
                {
                    // ignore
                }
                else
                {
                    SDA_LOW(twi_sda);
                }
            }
            twip_state = TWIP_WAIT_ACK;
        }
    }
    else IFSTATE(S2M(TWIP_WAIT_ACK))
    {
        if (scl)
        {
            // ignore
        }
        else
        {
            if (twip_mode == TWIPM_IDLE)
            {
                if ((twi_data & 0xFE) != twi_addr)
                {
                    SDA_HIGH(twi_sda);
                    twip_state = TWIP_WAIT_STOP;
                }
                else
                {
                    SCL_LOW(twi_scl); // clock stretching
                    SDA_HIGH(twi_sda);
                    twip_mode = TWIPM_ADDRESSED;
                    if (!(twi_data & 0x01))
                    {
                        onTwipEvent(TW_SR_SLA_ACK);
                        bitCount = 8;
                        twip_state = TWIP_SLA_W;
                    }
                    else
                    {
                        onTwipEvent(TW_ST_SLA_ACK);
                        twip_state = TWIP_SLA_R;
                    }
                }
            }
            else
            {
                SCL_LOW(twi_scl); // clock stretching
                SDA_HIGH(twi_sda);
                if (!twi_ack)
                {
                    onTwipEvent(TW_SR_DATA_NACK);
                    twip_mode = TWIPM_WAIT;
                    twip_state = TWIP_WAIT_STOP;
                }
                else
                {
                    onTwipEvent(TW_SR_DATA_ACK);
                    bitCount = 8;
                    twip_state = TWIP_READ;
                }
            }
        }
    }
    else IFSTATE(S2M(TWIP_SLA_R) | S2M(TWIP_WRITE))
    {
        if (scl)
        {
            // ignore
        }
        else
        {
            bitCount--;
            if (twi_data & 0x80)
            {
                SDA_HIGH(twi_sda);
            }
            else
            {
                SDA_LOW(twi_sda);
            }
            twi_data <<= 1;

            if (bitCount != 0)
            {
                // continue
            }
            else
            {
                twip_state = TWIP_REC_ACK;
            }
        }
    }
    else IFSTATE(S2M(TWIP_REC_ACK))
    {
        if (scl)
        {
            // ignore
        }
        else
        {
            SDA_HIGH(twi_sda);
            twip_state = TWIP_READ_ACK;
        }
    }
    else IFSTATE(S2M(TWIP_READ_ACK))
    {
        if (!scl)
        {
            // ignore
        }
        else
        {
            twi_ack_rec = !sda;
            twip_state = TWIP_RWAIT_ACK;
        }
    }
    else IFSTATE(S2M(TWIP_RWAIT_ACK))
    {
        if (scl)
        {
            // ignore
        }
        else
        {
            SCL_LOW(twi_scl); // clock stretching
            if (twi_ack && twi_ack_rec)
            {
                onTwipEvent(TW_ST_DATA_ACK);
                twip_state = TWIP_WRITE;
            }
            else
            {
                // we have no more data to send and/or the master doesn't want anymore
                onTwipEvent(twi_ack_rec ? TW_ST_LAST_DATA : TW_ST_DATA_NACK);
                twip_mode = TWIPM_WAIT;
                twip_state = TWIP_WAIT_STOP;
            }
        }
    }
}

void ICACHE_RAM_ATTR Twi::onSdaChange(void)
{
    unsigned int sda;
    unsigned int scl;

    // Store bool return in int to reduce final code size.
    sda = SDA_READ(twi_sda);
    scl = SCL_READ(twi_scl);

    int twip_state_mask = S2M(twip_state);
    if (scl) /* !DATA */
    {
        IFSTATE(S2M(TWIP_IDLE))
        {
            if (sda)
            {
                // STOP - ignore
            }
            else
            {
                // START
                bitCount = 8;
                twip_state = TWIP_START;
                ets_timer_arm_new(&timer, twi_timeout_ms, false, true); // Once, ms
            }
        }
        else IFSTATE(S2M(TWIP_START) | S2M(TWIP_REP_START) | S2M(TWIP_SEND_ACK) | S2M(TWIP_WAIT_ACK) | S2M(TWIP_SLA_R) | S2M(TWIP_REC_ACK) | S2M(TWIP_READ_ACK) | S2M(TWIP_RWAIT_ACK) | S2M(TWIP_WRITE))
        {
            // START or STOP
            SDA_HIGH(twi_sda); // Should not be necessary
            onTwipEvent(TW_BUS_ERROR);
            twip_mode = TWIPM_WAIT;
            twip_state = TWIP_BUS_ERR;
        }
        else IFSTATE(S2M(TWIP_WAIT_STOP) | S2M(TWIP_BUS_ERR))
        {
            if (sda)
            {
                // STOP
                SCL_LOW(twi_scl); // generates a low SCL pulse after STOP
                ets_timer_disarm(&timer);
                twip_state = TWIP_IDLE;
                twip_mode = TWIPM_IDLE;
                SCL_HIGH(twi_scl);
            }
            else
            {
                // START
                if (twip_state == TWIP_BUS_ERR)
                {
                    // ignore
                }
                else
                {
                    bitCount = 8;
                    twip_state = TWIP_REP_START;
                    ets_timer_arm_new(&timer, twi_timeout_ms, false, true); // Once, ms
                }
            }
        }
        else IFSTATE(S2M(TWIP_SLA_W) | S2M(TWIP_READ))
        {
            // START or STOP
            if (bitCount != 7)
            {
                // inside byte transfer - error
                onTwipEvent(TW_BUS_ERROR);
                twip_mode = TWIPM_WAIT;
                twip_state = TWIP_BUS_ERR;
            }
            else
            {
                // during first bit in byte transfer - ok
                SCL_LOW(twi_scl); // clock stretching
                onTwipEvent(TW_SR_STOP);
                if (sda)
                {
                    // STOP
                    ets_timer_disarm(&timer);
                    twip_state = TWIP_IDLE;
                    twip_mode = TWIPM_IDLE;
                }
                else
                {
                    // START
                    bitCount = 8;
                    ets_timer_arm_new(&timer, twi_timeout_ms, false, true); // Once, ms
                    twip_state = TWIP_REP_START;
                    twip_mode = TWIPM_IDLE;
                }
            }
        }
    }
}

static Twi twi;

// C wrappers for the object, since API is exposed only as C
extern "C"
{

    void twi_init(unsigned char sda, unsigned char scl)
    {
        return twi.init(sda, scl);
    }

    void twi_setAddress(uint8_t a)
    {
        return twi.setAddress(a);
    }

    void twi_setClock(unsigned int freq)
    {
        twi.setClock(freq);
    }

    void twi_setClockStretchLimit(uint32_t limit)
    {
        twi.setClockStretchLimit(limit);
    }

    uint8_t twi_writeTo(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop)
    {
        return twi.writeTo(address, buf, len, sendStop);
    }

    uint8_t twi_readFrom(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop)
    {
        return twi.readFrom(address, buf, len, sendStop);
    }

    uint8_t twi_status()
    {
        return twi.status();
    }

    uint8_t twi_transmit(const uint8_t *buf, uint8_t len)
    {
        return twi.transmit(buf, len);
    }

    void twi_attachSlaveRxEvent(void (*cb)(uint8_t *, size_t))
    {
        twi.attachSlaveRxEvent(cb);
    }

    void twi_attachSlaveTxEvent(void (*cb)(void))
    {
        twi.attachSlaveTxEvent(cb);
    }

    void twi_reply(uint8_t r)
    {
        twi.reply(r);
    }

    void twi_releaseBus(void)
    {
        twi.releaseBus();
    }

    void twi_enableSlaveMode(void)
    {
        twi.enableSlave();
    }
};
