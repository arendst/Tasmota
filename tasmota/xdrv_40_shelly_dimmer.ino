/*
    xdrv_40_shelly_dimmer.ino - shelly dimmer support for Tasmota

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
 * Shelly WiFi Dimmer v1 (ESP8266 w/ separate co-processor dimmer)
 * https://shelly.cloud/wifi-smart-home-automation-shelly-dimmer/
\*********************************************************************************************/
#define SHELLY_DIMMER_DEBUG
// #define SHELLY_HW_DIMMING

#define XDRV_32                     40
#define XNRG_17                     17

#define SHD_DRIVER_MAJOR_VERSION    0
#define SHD_DRIVER_MINOR_VERSION    5

#define SHD_SWITCH_CMD              0x01
#define SHD_GET_STATE_CMD           0x10
#define SHD_VERSION_CMD             0x11
#define SHD_SET_STATE_CMD           0x20

#define SHD_START_BYTE              0x01
#define SHD_END_BYTE                0x04

#define SHD_BUFFER_SIZE             256
#define SHD_ACK_TIMEOUT             200 // 200 ms ACK timeout

#include <stm32flash.h>
#include <fw/shelly/dimmer/stm_v1.6.0.h>

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

/*********************************************************************************************\
 * Helper Functions
\*********************************************************************************************/

uint16_t checksum(uint8_t *buf, int len)
{
    uint16_t chksm = 0;
    for (uint8_t i = 0; i < len; i++)
        chksm += buf[i];
    return chksm;
}

int check_byte()
{
    uint8_t index = Shd.byte_counter;
    uint8_t byte = Shd.buffer[index];

    if (index == 0)
        return byte == SHD_START_BYTE;

    if (index < 4)
        return 1;

    uint8_t data_length = Shd.buffer[3];
    if ((4 + data_length + 3) > SHD_BUFFER_SIZE)
        return 0;

    if (index < 4 + data_length + 1)
        return 1;

    if (index == 4 + data_length + 1)
    {
        uint16_t chksm = (Shd.buffer[index - 1] << 8 | Shd.buffer[index]);
        if (chksm != checksum(&Shd.buffer[1], 3 + data_length))
            return 0;

        return 1;
    }

    if (index == 4 + data_length + 2 && byte == SHD_END_BYTE)
        return index;
    
    return 0;
}

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

bool ShdSerialSend(const uint8_t data[] = nullptr, uint16_t len = 0)
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
            if (ShdSerialInput())
                return true;

            delay(1);
        }

#ifdef SHELLY_DIMMER_DEBUG
        // timeout
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("SHD: serial send timeout"));
#endif
    }
    return false;
}

bool ShdSendCmd(uint8_t cmd, uint8_t *payload, uint8_t len)
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

    return ShdSerialSend(data, pos);
}

void ShdSetBrightness(uint16 brightness)
{
    brightness = changeUIntScale(brightness, 0, 255, 0, 1000);

    uint8_t payload[2];

    payload[0] = brightness & 0xff;
    payload[1] = brightness >> 8;

    ShdSendCmd(SHD_SWITCH_CMD, payload, sizeof(payload));
}

void ShdSendFadeRate(uint16_t brightness, uint8_t fade_rate)
{
    ShdSendSetState(brightness, 2, fade_rate);
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

#ifdef SHELLY_HW_DIMMING
    // TODO(jamesturton): HW dimming seems to conflict with SW dimming. See how
    // we can disbale SW dimming when using HW dimming.
    if (Settings.light_speed != Shd.dimmer.fade_rate)
    {
        ShdSendFadeRate(Shd.req_brightness, Settings.light_speed);
        ShdDebugState();
    }
    else
#endif
    if (Shd.req_brightness != Shd.dimmer.brightness)
    {
        ShdSetBrightness(Shd.req_brightness);
        ShdDebugState();
    }
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
                brightness = changeUIntScale(brightness, 0, 1000, 0, 255);
            }
            break;
        case SHD_GET_STATE_CMD:
            {
                // 1 when returning fade_rate, 0 when returning wattage, brightness?
                uint16_t unknown_0 = Shd.buffer[pos + 1] << 8 | Shd.buffer[pos + 0];
                
                uint16_t brightness = Shd.buffer[pos + 3] << 8 | Shd.buffer[pos + 2];
                brightness = changeUIntScale(brightness, 0, 1000, 0, 255);

                uint32_t wattage_raw = Shd.buffer[pos + 7] << 24 | 
                        Shd.buffer[pos + 6] << 16 | 
                        Shd.buffer[pos + 5] << 8 | 
                        Shd.buffer[pos + 4];
                float wattage = 0;
                if (wattage_raw > 0)
                    wattage = 1000000 / wattage_raw;
                uint32_t fade_rate = Shd.buffer[pos + 8];

#ifdef USE_ENERGY_SENSOR
                Energy.active_power[0] = wattage;
                
                if (Shd.last_power_check > 10 && Energy.active_power[0] > 0)
                {
                    float kWhused = (float)Energy.active_power[0] * (Rtc.utc_time - Shd.last_power_check) / 36;
                    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Adding %i mWh to todays usage from %lu to %lu"), (int)(kWhused * 10), Shd.last_power_check, Rtc.utc_time);
                    Energy.kWhtoday += kWhused;
                    EnergyUpdateToday();
                }
                Shd.last_power_check = Rtc.utc_time;
#endif // USE_ENERGY_SENSOR

                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: ShdPacketProcess: Brightness:%d Power:%d Fade:%d"), brightness, wattage_raw, fade_rate);
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

                AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: ShdPacketProcess: Version: %u.%u"), Shd.buffer[pos + 1], Shd.buffer[pos]);
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
            {
                ret = (Shd.buffer[pos] == 0x01);
            }
            break;
    }

    return ret;
}

bool ShdSetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    snprintf_P(log_data, sizeof(log_data), PSTR("SHD: SetChannels: \""));
    for (int i = 0; i < XdrvMailbox.data_len; i++)
        snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, ((uint8_t *)XdrvMailbox.data)[i]);
    snprintf_P(log_data, sizeof(log_data), PSTR("%s\""), log_data);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    // upscale and then downscale to account for rounding errors
    uint16_t brightness = ((uint32_t *)XdrvMailbox.data)[0];
    brightness = changeUIntScale(brightness, 0, 255, 0, 1000);
    brightness = changeUIntScale(brightness, 0, 1000, 0, 255);
    Shd.req_brightness = brightness;

    ShdDebugState();

    return ShdSyncState();
}

void ShdResetToAppMode()
{
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Request co-processor reset in app mode"));

    pinMode(Pin(GPIO_SHELLY_DIMMER_RST_INV), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), LOW);

    pinMode(Pin(GPIO_SHELLY_DIMMER_BOOT0), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_BOOT0), LOW);

    delay(50);
    
    // clear in the receive buffer
    while (Serial.available())
        Serial.read();
    
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), HIGH); // pull out of reset
    delay(50); // wait 50ms fot the co-processor to come online
}

void ShdResetToDFUMode()
{
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Request co-processor reset in dfu mode"));

    pinMode(Pin(GPIO_SHELLY_DIMMER_RST_INV), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), LOW);

    pinMode(Pin(GPIO_SHELLY_DIMMER_BOOT0), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_BOOT0), HIGH);

    delay(50);
    
    // clear in the receive buffer
    while (Serial.available())
        Serial.read();
    
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), HIGH); // pull out of reset
    delay(50); // wait 50ms fot the co-processor to come online
}

bool ShdUpdateFirmware(const uint8_t data[], unsigned int size)
{
    bool ret = true;
    stm32_t *stm = stm32_init(&Serial, STREAM_SERIAL, 1); 
    if (stm)
    {
      off_t   offset = 0;
      uint8_t   buffer[256];
      unsigned int  len;
      const uint8_t *p_st = data;
      uint32_t  addr, start, end;
      stm32_err_t s_err;

      stm32_erase_memory(stm, 0, STM32_MASS_ERASE);

      addr = stm->dev->fl_start;
      end = addr + size;
      while(addr < end && offset < size) 
      {
          uint32_t left = end - addr;
          len   = sizeof(buffer) > left ? left : sizeof(buffer);
          len   = len > size - offset ? size - offset : len;

          if (len == 0) 
          {
              break;
          }
        
          memcpy(buffer, p_st, len);
          p_st += len;
      
          s_err = stm32_write_memory(stm, addr, buffer, len);
          if (s_err != STM32_ERR_OK) 
          {
              ret = false;
              break;
          }

          addr  += len;
          offset  += len;
      }
      stm32_close(stm);
    }
    return ret;
}

void ShdPoll(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Poll"));
#endif

    if (!ShdSerial)
        return;

    ShdSendCmd(SHD_GET_STATE_CMD, 0, 0);
    ShdSyncState();
}

bool ShdSendVersion(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog_P2(LOG_LEVEL_INFO, PSTR("SHD: Sending version command"));
#endif
    return ShdSendCmd(SHD_VERSION_CMD, 0, 0);
}

void ShdInit(void)
{
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Shelly Dimmer Driver v%u.%u"), SHD_DRIVER_MAJOR_VERSION, SHD_DRIVER_MINOR_VERSION);
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
                ClaimSerial();

            ShdSerial->flush();

            ShdResetToAppMode();
            bool got_version = ShdSendVersion();
            if (!got_version || (got_version && 
                    (Shd.dimmer.version_minor != SHD_FIRMWARE_MINOR_VERSION || 
                     Shd.dimmer.version_major != SHD_FIRMWARE_MAJOR_VERSION))) 
            {
                AddLog_P2(LOG_LEVEL_INFO, PSTR("SHD: Updating firmware from v%u.%u to v%u.%u"), Shd.dimmer.version_major, Shd.dimmer.version_minor, SHD_FIRMWARE_MAJOR_VERSION, SHD_FIRMWARE_MINOR_VERSION);
                
                Serial.end();
                Serial.begin(115200, SERIAL_8E1);
                ShdResetToDFUMode();
                ShdUpdateFirmware(stm_firmware, sizeof(stm_firmware));
                Serial.end();

                ShdResetToAppMode();
                Serial.begin(115200, SERIAL_8N1);
                
                ShdSendVersion();
            }
            delay(100);
            ShdSyncState();
        }
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
 * Driver Interface
\*********************************************************************************************/

bool Xdrv32(uint8_t function)
{
    bool result = false;

    if (SHELLY_DIMMER == my_module_type)
    {
        switch (function)
        {
        case FUNC_EVERY_SECOND:
            ShdPoll();
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