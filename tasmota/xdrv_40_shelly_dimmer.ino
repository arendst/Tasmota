/*
    xdrv_32_shelly_dimmer.ino - shelly dimmer support for Tasmota

    Copyright (C) 2020  James Turton

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_LIGHT
#ifdef USE_SHELLY_DIMMER
/*********************************************************************************************\
 * Shelly WiFi Dimmer v1 (ESP8266 w/ separate MCU dimmer)
 * https://shelly.cloud/wifi-smart-home-automation-shelly-dimmer/
\*********************************************************************************************/
#define SHELLY_DIMMER_DEBUG

#define XDRV_32                     40
#define XNRG_17                     17

#define SHD_FIRMWARE_MAJOR_VERSION  0x02
#define SHD_FIRMWARE_MINOR_VERSION  0x16

#define SHD_SWITCH_CMD              0x01
#define SHD_GET_STATE_CMD           0x10
#define SHD_VERSION_CMD             0x11
#define SHD_SET_STATE_CMD           0x20

#define SHD_START_BYTE              0x01
#define SHD_END_BYTE                0x04

#define SHD_BUFFER_SIZE             256
#define SHD_ACK_TIMEOUT             200 // 200 ms ACK timeout

#include <TasmotaSerial.h>

TasmotaSerial *ShdSerial = nullptr;

typedef struct
{
    uint8_t version_major = 0;
    uint8_t version_minor = 0;

    uint32_t brightness = 0;
    uint32_t power = 0;
    uint32_t fade_rate = 0;
} SHD_DIMMER;

struct SHD
{
    uint8_t *buffer = nullptr; // Serial receive buffer
    int byte_counter = 0;      // Index in serial receive buffer
    uint8_t req_brightness = 0;
    SHD_DIMMER dimmer;
    uint32_t start_time = 0;
    uint8_t counter = 1;        // Packet counter
#ifdef USE_ENERGY_SENSOR
    uint32_t last_power_check = 0;       // Time when last power was checked
#endif // USE_ENERGY_SENSOR
} Shd;

static const uint8_t fade_rate_tab[] = {0x05, 0x05, 0x0a, 0x0f, 0x14, 0x19, 0x19, 0x19};

/*********************************************************************************************\
 * Helper Functions
\*********************************************************************************************/

uint16_t checksum(uint8_t *buf, int len)
{
    uint16_t chksm = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        chksm += buf[i];
    }
    return chksm;
}

int check_byte()
{
    uint8_t index = Shd.byte_counter;
    uint8_t byte = Shd.buffer[index];

    if (index == 0)
    {
        return byte == SHD_START_BYTE;
    }

    if (index < 4)
    {
        return 1;
    }

    uint8_t data_length = Shd.buffer[3];
    if ((4 + data_length + 3) > SHD_BUFFER_SIZE)
    {
        return 0;
    }

    if (index < 4 + data_length + 1)
    {
        return 1;
    }

    if (index == 4 + data_length + 1)
    {
        uint16_t chksm = (Shd.buffer[index - 1] << 8 | Shd.buffer[index]);
        if (chksm != checksum(&Shd.buffer[1], 3 + data_length))
        {
            return 0;
        }
        return 1;
    }

    if (index == 4 + data_length + 2 && byte == SHD_END_BYTE)
    {
        return index;
    }
    
    return 0;
}

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void ShdSerialSend(const uint8_t data[] = nullptr, uint16_t len = 0)
{
    int retries = 3;

#ifdef SHELLY_DIMMER_DEBUG
    snprintf_P(log_data, sizeof(log_data), PSTR("SHD: Tx Packet:"));
    for (uint32_t i = 0; i < len; i++)
        snprintf_P(log_data, sizeof(log_data), PSTR("%s %02x"), log_data, data[i]);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    while (retries--)
    {
        ShdSerial->write(data, len);
        ShdSerial->flush();


        // wait for any response
        uint32_t snd_time = millis();
        while (TimePassedSince(snd_time) < SHD_ACK_TIMEOUT)
        {
            // ShdSerialInput();
            if (ShdSerialInput())
                return;
            // return;
            delay(1);
        }

#ifdef SHELLY_DIMMER_DEBUG
        // timeout
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("SHD: serial send timeout"));
#endif
    }
}

void ShdSendCmd(uint8_t cmd, uint8_t *payload, uint8_t len)
{
    uint8_t data[4 + 72 + 3]; // maximum payload for 0x30 packet is 72
    uint16_t chksm;
    uint8_t pos = 0;

    data[0] = SHD_START_BYTE;
    data[1] = Shd.counter++;
    data[2] = cmd;
    data[3] = len;

    pos += 4;

    if (payload)
    {
        memcpy(data + 4, payload, len);
        pos += len;
    }

    // calculate checksum from id and onwards
    chksm = checksum(data + 1, 3 + len); 
    data[pos++] = chksm >> 8;
    data[pos++] = chksm & 0xff;
    data[pos++] = SHD_END_BYTE;

    ShdSerialSend(data, pos);
}

void ShdSetBri(uint16 brightness)
{
    uint8_t payload[2];

    payload[0] = brightness & 0xff;
    payload[1] = brightness >> 8;

    ShdSendCmd(SHD_SWITCH_CMD, payload, sizeof(payload));
}

// TODO(james): Check what's going on here.
void ShdSendFadeRate(uint8_t fade_rate)
{
  ShdSendSetState(0, 2, fade_rate_tab[fade_rate & 0x07]);
}

void ShdSendSetState(uint16_t brightness, uint16_t func, uint16_t fade_rate)
{
    uint8_t payload[6];

    payload[0] = brightness & 0xff;
    payload[1] = brightness >> 8;

    // func = 0x00 == set current?
    // 1, 2 (used in combination with fade rate) or ...
    payload[2] = func & 0xff;
    payload[3] = func >> 8;

    // as specified in STM32 assembly
    if (fade_rate > 100)
        fade_rate = 100;

    payload[4] = fade_rate & 0xff;
    payload[5] = fade_rate >> 8;

    ShdSendCmd(SHD_SET_STATE_CMD, payload, sizeof(payload));
}

bool ShdSyncState()
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Serial %p"), ShdSerial);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Set Brightness Want %d, Is %d"), Shd.req_brightness, Shd.dimmer.brightness);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Set Fade Want %d, Is %d"), Settings.light_speed, Shd.dimmer.fade_rate);
#endif

    if (!ShdSerial)
        return false;

    if (Settings.light_speed != Shd.dimmer.fade_rate)
        ShdSendSetState(Shd.req_brightness, 2, fade_rate_tab[Settings.light_speed & 0x07]);
    else if (Shd.req_brightness != Shd.dimmer.brightness)
        ShdSetBri(Shd.req_brightness);
}

void ShdDebugState()
{
#ifdef SHELLY_DIMMER_DEBUG
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: MCU v%d.%d, Brightness:%d(%d%%), Power:%d, Fade:%d"),
                            Shd.dimmer.version_major, Shd.dimmer.version_minor,
                            Shd.dimmer.brightness,
                            changeUIntScale(Shd.dimmer.brightness, 0, 255, 0, 100),
                            Shd.dimmer.power,
                            Shd.dimmer.fade_rate);
#endif
}

bool ShdPacketProcess(void)
{
    uint8_t pos = 0;
    uint8_t id, cmd, len;
    bool ret = false;
    
    if (Shd.buffer[pos++] != SHD_START_BYTE)
        return false;
        
    id = Shd.buffer[pos++];
    cmd = Shd.buffer[pos++];
    len = Shd.buffer[pos++];
    
    switch (cmd)
    {
        case 0x03:
            {
                uint16_t brightness = Shd.buffer[pos + 1] << 8 | Shd.buffer[pos + 0];

            }
            break;
        case SHD_GET_STATE_CMD:
            {
                // 1 when returning fade_rate, 0 when returning wattage, brightness?
                uint16_t unknown_0 = Shd.buffer[pos + 1] << 8 | Shd.buffer[pos + 0];
                
                uint16_t brightness = Shd.buffer[pos + 3] << 8 | Shd.buffer[pos + 2];
                uint32_t wattage_raw = Shd.buffer[pos + 7] << 24 | 
                        Shd.buffer[pos + 6] << 16 | 
                        Shd.buffer[pos + 5] << 8 | 
                        Shd.buffer[pos + 4];
                float wattage = 0;
                if (wattage_raw > 0)
                    wattage = 1000000 / wattage_raw;
                uint32_t fade_rate = Shd.buffer[pos + 8] << 24 | 
                        Shd.buffer[pos + 9] << 16 | 
                        Shd.buffer[pos + 10] << 8 | 
                        Shd.buffer[pos + 11];

#ifdef USE_ENERGY_SENSOR
                Energy.active_power[0] = wattage;
                
                if (Shd.last_power_check != 0 && Energy.active_power[0] > 0)
                {
                    Energy.kWhtoday += (float)Energy.active_power[0] * (Rtc.utc_time - Shd.last_power_check) / 36;
                    EnergyUpdateToday();
                }
                Shd.last_power_check = Rtc.utc_time;
#endif // USE_ENERGY_SENSOR

                Shd.dimmer.brightness = brightness;
                Shd.dimmer.power = wattage_raw;
                Shd.dimmer.fade_rate = fade_rate;
            }
            break;  
        case SHD_VERSION_CMD:
            {
                // returns a static, is this a version number?
                ret = Shd.buffer[pos] == SHD_FIRMWARE_MINOR_VERSION && 
                    Shd.buffer[pos + 1] == SHD_FIRMWARE_MAJOR_VERSION;

                Shd.dimmer.version_minor = Shd.buffer[pos];
                Shd.dimmer.version_major = Shd.buffer[pos + 1];
            }
            break;
        case SHD_SWITCH_CMD:
        case 0x02:
        case 0x04:
        case SHD_SET_STATE_CMD:
        case 0x30:
        case 0x31:
            ret = (Shd.buffer[pos] == 0x01);
            break;
    }

    return ret;
}

bool ShdSetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    snprintf_P(log_data, sizeof(log_data), PSTR("SHD: SetChannels: \""));
    for (int i = 0; i < XdrvMailbox.data_len; i++)
    {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, ((uint8_t *)XdrvMailbox.data)[i]);
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s\""), log_data);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    Shd.req_brightness = ((uint32_t *)XdrvMailbox.data)[0];

    ShdDebugState();

    return ShdSyncState();
}

void ShdMcuStart(void)
{
    int retries = 3;

#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Request co-processor reset configuration, PIN %d to High"), Pin(GPIO_SHELLY_DIMMER_RST_INV));
#endif

    pinMode(Pin(GPIO_SHELLY_DIMMER_BOOT0), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_BOOT0), LOW);

    pinMode(Pin(GPIO_SHELLY_DIMMER_RST_INV), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), LOW);
    delay(50);
    while (ShdSerial->available())
    {
        // clear in the receive buffer
        ShdSerial->read();
    }
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), HIGH); // pull out of reset
    delay(50); // wait 50ms fot the co-processor to come online
}

void ShdPoll(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Poll"));
#endif
    ShdSendCmd(SHD_GET_STATE_CMD, 0, 0);
}

void ShdSendVersion(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_INFO, PSTR("SHD: Sending version command"));
#endif
    ShdSendCmd(SHD_VERSION_CMD, 0, 0);
}

void ShdInit(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_INFO, PSTR("SHD: Starting Tx %d Rx %d"), Pin(GPIO_TXD), Pin(GPIO_RXD));
#endif

    Shd.buffer = (uint8_t *)malloc(SHD_BUFFER_SIZE);
    if (Shd.buffer != nullptr)
    {
        ShdSerial = new TasmotaSerial(Pin(GPIO_RXD), Pin(GPIO_TXD), 2, 0, SHD_BUFFER_SIZE);
        if (ShdSerial->begin(115200))
        {
            if (ShdSerial->hardwareSerial())
            {
                ClaimSerial();
            }
            ShdSerial->flush();
            ShdMcuStart();

            ShdSendVersion();
        }
    }
}

void ShdLoop(void)
{
    // poll every two seconds for new data
    if (Shd.start_time == 0 || millis() - Shd.start_time > 1000  * 2)
    {
        Shd.start_time = millis();
        ShdPoll();
    }
}

bool ShdSerialInput(void)
{
    while (ShdSerial->available())
    {
        yield();
        uint8_t serial_in_byte = ShdSerial->read();
        Shd.buffer[Shd.byte_counter] = serial_in_byte;
        
        int check = check_byte();

        if (check > 1)
        {
            // finished
#ifdef SHELLY_DIMMER_DEBUG
            Shd.byte_counter++;
            snprintf_P(log_data, sizeof(log_data), PSTR("SHD: RX Packet:"));
            for (uint32_t i = 0; i < Shd.byte_counter; i++)
                snprintf_P(log_data, sizeof(log_data), PSTR("%s %02x"), log_data, Shd.buffer[i]);
            AddLog(LOG_LEVEL_DEBUG_MORE);
#endif
            Shd.byte_counter = 0;

            ShdPacketProcess();

            return true;
        }
        else if (check == 0)
        {
            // wrong data
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Byte %i of received data frame is invalid"), Shd.byte_counter);
            Shd.byte_counter = 0;
        }
        else
        {
            Shd.byte_counter++;
        }
        
    }
    return false;
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

bool ShdModuleSelected(void)
{
    devices_present++;
    light_type = LT_SERIAL1;

    return true;
}

/*********************************************************************************************\
 * Energy Interface
\*********************************************************************************************/

#ifdef USE_ENERGY_SENSOR
bool Xnrg17(uint8_t function)
{
  bool result = false;

  if (SHELLY_DIMMER == my_module_type && FUNC_PRE_INIT == function)
  {
        Energy.current_available = false;
        Energy.voltage_available = false;
        energy_flg = XNRG_17;
  }
  return result;
}
#endif  // USE_ENERGY_SENSOR

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv32(uint8_t function)
{
    bool result = false;

    if (SHELLY_DIMMER == my_module_type)
    {
        switch (function)
        {
        case FUNC_LOOP:
            if (ShdSerial)
                ShdLoop();
            break;
        case FUNC_MODULE_INIT:
            result = ShdModuleSelected();
            break;
        case FUNC_INIT:
            ShdInit();
            break;
        case FUNC_SET_CHANNELS:
            result = ShdSetChannels();
            break;
        }
    }
    return result;
}

#endif  // USE_SHELLY_DIMMER
#endif  // USE_LIGHT