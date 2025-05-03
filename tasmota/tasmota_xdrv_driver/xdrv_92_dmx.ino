/*
    xdrv_92_dmx.ino - DMX512 support for Tasmota
*/

#include "c_types.h"
#include "eagle_soc.h"
#include "uart_register.h"

#ifdef USE_DMX512
#ifdef ESP32
#error "ESP32 support not implemented yet"
#endif

/*********************************************************************************************\
 * DMX512 transmission support over UART
\*********************************************************************************************/

#define XDRV_92 92

#define DMX_SPEED      250000
#define DMX_FORMAT     SERIAL_8N2
#define DMX_BREAK      92
#define DMX_MAB        12

struct DMX512
{
    bool    active = true;
    bool    serialSwap = false;
    char    uart = 0; //TODO: implement Serial1 on ESP8266, 2/3 on ESP32
    uint8_t retransmit_count = 0;
} Dmx512;

bool Dmx512Init(void) {
    if(USE_DMX512_MAX_CHANNELS>512 || USE_DMX512_MAX_CHANNELS<1) { 
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DMX512 "Compile time setting USE_DMX512_MAX_CHANNELS has invalid value"));
        return false;
    }
    if (PinUsed(GPIO_DMX512)) {
        if(Pin(GPIO_DMX512) != 1 && Pin(GPIO_DMX512) != 15) {
            AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DMX512 "DMX512 TX may only be assigned to pin 1 or 15"));
            return false;
        }
        if(PinUsed(GPIO_TXD) || PinUsed(GPIO_RXD)) { //TODO: fine-tune check for pin clashes
            AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DMX512 "Both DMX512 TX and Serial Tx/Rx is assigned to pins; not enabling DMX512"));
            return false;
        }
        if(Pin(GPIO_DMX512) == 15) {
            Dmx512.serialSwap = true;
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DMX512 "Serial pins will be swapped to use pin 15 for DMX512"));
        }
        pinMode(Pin(GPIO_DMX512), OUTPUT);

        Serial.begin(DMX_SPEED, DMX_FORMAT);
        if(Dmx512.serialSwap) Serial.swap();
        while(Serial.available()) Serial.read();

        return true;
    } else {
        return false;
    }
}

// Transmit a DMX512 frame
void DmxSendFrame(char* buf, uint32_t count) {
    int pin = Pin(GPIO_DMX512);
    
    SET_PERI_REG_MASK(UART_CONF0((uint32_t)Dmx512.uart), UART_TXD_BRK);
    delayMicroseconds(DMX_BREAK);
    CLEAR_PERI_REG_MASK(UART_CONF0((uint32_t)Dmx512.uart), UART_TXD_BRK);
    delayMicroseconds(DMX_MAB);

    Serial.write(0); 
    Serial.write(buf, count);
    Serial.flush();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kDmxCommands[] PROGMEM = "|" // No prefix
    "DmxSend|DmxRetransmit|";

void (* const DmxCommand[])(void) PROGMEM = {
    & CmndDmxSend, & CmndDmxRetransmit };

// DmxSend transmits a DMX512 frame of at most USE_DMX512_MAX_CHANNELS channels
void CmndDmxSend(void) {
    if (XdrvMailbox.data_len > 0) {
        uint32_t parm[USE_DMX512_MAX_CHANNELS];
        char payload[USE_DMX512_MAX_CHANNELS];
        uint32_t count = ParseParameters(USE_DMX512_MAX_CHANNELS, parm);
        if(count > USE_DMX512_MAX_CHANNELS || count < 1) {
            AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DMX512 "Invalid number of channels"));
            ResponseCmndError();
            return;
        }
        for(int i = 0; i<count; i++) {
            if(parm[i] > 255) {
                AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DMX512 "Channel value %u out of range"), count);
                ResponseCmndError();
                return;
            }
            payload[i] = parm[i];
        }
        for(int i = 0; i<Dmx512.retransmit_count+1;i++) {
            DmxSendFrame(payload, count);
        }
    }
    ResponseCmndDone();
}

// Shitty equipment or cabling might require transmitting each frame multiple times.
// example: after "DmxRetransmit 3", each call to "DmxSend" will transmit the same frame 4 times.
void CmndDmxRetransmit(void) {
    if (XdrvMailbox.data_len > 0) {
        Dmx512.retransmit_count = XdrvMailbox.payload > 8 ? 8 : XdrvMailbox.payload;
    }
    ResponseCmndNumber(Dmx512.retransmit_count);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv92(uint32_t function) {
    bool rv = false;
    if(Dmx512.active) {
        switch (function) {
            case FUNC_PRE_INIT:
                Dmx512.active = Dmx512Init();
                break;
            case FUNC_COMMAND:
                rv = DecodeCommand(kDmxCommands, DmxCommand);
                break;
        }
    }
    return rv;
}

#endif // USE_DMX512