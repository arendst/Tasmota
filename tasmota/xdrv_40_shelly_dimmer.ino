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
#define SHD_CMDS

#define XDRV_32                     40
#define XNRG_17                     17

#define SHD_DRIVER_MAJOR_VERSION    0
#define SHD_DRIVER_MINOR_VERSION    7

#define SHD_SWITCH_CMD              0x01
#define SHD_SWITCH_FADE_CMD         0x02
#define SHD_POLL_CMD                0x10
#define SHD_VERSION_CMD             0x11
#define SHD_SETTINGS_CMD            0x20
#define SHD_WARMUP_CMD              0x21
#define SHD_CALIBRATION1_CMD        0x30
#define SHD_CALIBRATION2_CMD        0x31

#define SHD_SWITCH_SIZE             2
#define SHD_SWITCH_FADE_SIZE        6
#define SHD_SETTINGS_SIZE           10
#define SHD_WARMUP_SIZE             4
#define SHD_CALIBRATION_SIZE        200

#define SHD_START_BYTE              0x01
#define SHD_END_BYTE                0x04

#define SHD_BUFFER_SIZE             256
#define SHD_ACK_TIMEOUT             200 // 200 ms ACK timeout

#include <stm32flash.h>
#include <fw/shelly/dimmer/stm_v1.7.0.h>

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
    uint8_t *buffer = nullptr;          // Serial receive buffer
    int byte_counter = 0;               // Index in serial receive buffer
    uint16_t req_brightness = 0;
    bool req_on = false;
    SHD_DIMMER dimmer;
    uint32_t start_time = 0;
    uint8_t counter = 1;                // Packet counter
    uint16_t req_fade_rate = 0;
    uint16_t leading_edge = 2;          // Leading edge = 2 Trailing edge = 1
    uint16_t warmup_brightness = 100;   // 10%
    uint16_t warmup_time = 20;          // 20ms
#ifdef USE_ENERGY_SENSOR
    uint32_t last_power_check = 0;      // Time when last power was checked
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

void ShdSetBrightness()
{
    // Payload format:
    // [0-1] Brightness (%) * 10

    uint8_t payload[SHD_SWITCH_SIZE];

    payload[0] = Shd.req_brightness & 0xff;
    payload[1] = Shd.req_brightness >> 8;

    ShdSendCmd(SHD_SWITCH_CMD, payload, SHD_SWITCH_SIZE);
}

void ShdSetBrightnessFade()
{
    // Payload format:
    // [0-1] Brightness (%) * 10
    // [2-3] Delta brightness (%) * 8
    // [4-5] 0?? ToDo(jamesturton): Find out what this word is!

    uint16_t delta = 0;
    if (Shd.req_brightness > Shd.dimmer.brightness)
        delta = (Shd.req_brightness - Shd.dimmer.brightness) * 0.8;
    else
        delta = (Shd.dimmer.brightness - Shd.req_brightness) * 0.8;

    uint8_t payload[SHD_SWITCH_FADE_SIZE];

    payload[0] = Shd.req_brightness & 0xff;
    payload[1] = Shd.req_brightness >> 8;

    payload[2] = delta & 0xff;
    payload[3] = delta >> 8;

    payload[4] = 0;
    payload[5] = 0;

    ShdSendCmd(SHD_SWITCH_FADE_CMD, payload, SHD_SWITCH_FADE_SIZE);
}

void ShdSendSettings()
{
    // as specified in STM32 assembly
    uint16_t fade_rate = Shd.req_fade_rate;
    if (fade_rate > 100)
        fade_rate = 100;

    // Payload format:
    // [0-1] Brightness (%) * 10
    // [2-3] Leading / trailing edge (1=leading, 2=trailing) ToDo(jamesturton): Are there any other values this can take?
    // [4-5] Fade rate (units unknown) ToDo(jamesturton): Find fade rate units
    // [6-7] Warm up brightness (%) * 10
    // [8-9] Warm up duration (ms)

    uint8_t payload[SHD_SETTINGS_SIZE];

    payload[0] = Shd.req_brightness & 0xff;
    payload[1] = Shd.req_brightness >> 8;

    payload[2] = Shd.leading_edge & 0xff;
    payload[3] = Shd.leading_edge >> 8;

    payload[4] = fade_rate & 0xff;
    payload[5] = fade_rate >> 8;

    payload[6] = Shd.warmup_brightness & 0xff;
    payload[7] = Shd.warmup_brightness >> 8;

    payload[8] = Shd.warmup_time & 0xff;
    payload[9] = Shd.warmup_time >> 8;

    ShdSendCmd(SHD_SETTINGS_CMD, payload, SHD_SETTINGS_SIZE);
}

void ShdSendWarmup()
{
    // Payload format:
    // [0-1] Warm up brightness (%) * 10
    // [2-3] Warm up duration (ms)

    uint8_t payload[SHD_WARMUP_SIZE];

    payload[0] = Shd.warmup_brightness & 0xff;
    payload[1] = Shd.warmup_brightness >> 8;

    payload[2] = Shd.warmup_time & 0xff;
    payload[3] = Shd.warmup_time >> 8;

    ShdSendCmd(SHD_WARMUP_CMD, payload, SHD_WARMUP_SIZE);
}

void ShdSendCalibration(uint16_t brightness, uint16_t func, uint16_t fade_rate)
{
    // Payload format:
    // ??? ToDo(jamesturton): Find calibration payload format!
    uint8_t payload[SHD_CALIBRATION_SIZE];

    memset(payload, 0, sizeof(payload));
    
    ShdSendCmd(SHD_CALIBRATION1_CMD, payload, SHD_CALIBRATION_SIZE);
    ShdSendCmd(SHD_CALIBRATION2_CMD, payload, SHD_CALIBRATION_SIZE);
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
        ShdSetBrightnessFade();
        ShdDebugState();
    }
    else
#endif
    if (Shd.req_brightness != Shd.dimmer.brightness)
    {
        ShdSetBrightness();
        ShdDebugState();
    }
}

void ShdDebugState()
{
#ifdef SHELLY_DIMMER_DEBUG
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: MCU v%d.%d, Brightness:%d(%d%%), Power:%d, Fade:%d"),
                            Shd.dimmer.version_major, Shd.dimmer.version_minor,
                            Shd.dimmer.brightness,
                            changeUIntScale(Shd.dimmer.brightness, 0, 1000, 0, 100),
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
        case SHD_POLL_CMD:
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
        case SHD_SWITCH_FADE_CMD:
        case SHD_SETTINGS_CMD:
        case SHD_WARMUP_CMD:
        case SHD_CALIBRATION1_CMD:
        case SHD_CALIBRATION2_CMD:
            {
                ret = (Shd.buffer[pos] == 0x01);
            }
            break;
    }

    return ret;
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

    ShdSendCmd(SHD_POLL_CMD, 0, 0);
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
                ShdSendSettings();
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

bool ShdSetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    snprintf_P(log_data, sizeof(log_data), PSTR("SHD: SetChannels: \""));
    for (int i = 0; i < XdrvMailbox.data_len; i++)
        snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, ((uint8_t *)XdrvMailbox.data)[i]);
    snprintf_P(log_data, sizeof(log_data), PSTR("%s\""), log_data);
    AddLog(LOG_LEVEL_DEBUG_MORE);
#endif

    uint16_t brightness = ((uint32_t *)XdrvMailbox.data)[0];
    brightness = changeUIntScale(brightness, 0, 255, 0, 1000);
    Shd.req_brightness = brightness;

    ShdDebugState();

    return ShdSyncState();
}

bool ShdSetPower(void)
{
    AddLog_P2(LOG_LEVEL_INFO, PSTR("EXS: Set Power, Power 0x%02x"), XdrvMailbox.index);

    Shd.req_on = (bool)XdrvMailbox.index;
    return ShdSyncState();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef SHD_CMDS

#define D_PRFX_SHD              "Shd"
#define D_CMND_LEADINGEDGE      "LeadingEdge"
#define D_CMND_WARMUPBRIGHTNESS "WarmupBrightness"
#define D_CMND_WARMUPTIME       "WarmupTime"

const char kShdCommands[] PROGMEM = D_PRFX_SHD "|"  // No prefix
  D_CMND_LEADINGEDGE "|"  D_CMND_WARMUPBRIGHTNESS "|" D_CMND_WARMUPTIME;

void (* const ShdCommand[])(void) PROGMEM = {
  &CmndShdLeadingEdge, &CmndShdWarmupBrightness, &CmndShdWarmupTime };

void CmndShdLeadingEdge(void)
{
    if (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1)
    {
        Shd.leading_edge = 2 - XdrvMailbox.payload;
        Settings.shd_leading_edge = XdrvMailbox.payload;
        if (Shd.leading_edge == 1)
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Set to trailing edge"));
        else
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Set to leading edge"));
        ShdSendSettings();
    }
    ResponseCmndNumber(Settings.shd_leading_edge);
}

void CmndShdWarmupBrightness(void)
{
    if ((10 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 100))
    {
        Shd.warmup_brightness = XdrvMailbox.payload * 10;
        Settings.shd_warmup_brightness = XdrvMailbox.payload;
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Set warmup brightness to %d%%"), XdrvMailbox.payload);
        ShdSendSettings();
    }
    ResponseCmndNumber(Settings.shd_warmup_brightness);
}

void CmndShdWarmupTime(void)
{
    if ((20 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 200))
    {
        Shd.warmup_time = XdrvMailbox.payload;
        Settings.shd_warmup_time = XdrvMailbox.payload;
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHD: Set warmup time to %dms"), XdrvMailbox.payload);
        ShdSendSettings();
    }
    ResponseCmndNumber(Settings.shd_warmup_time);
}

#endif

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
        case FUNC_SET_DEVICE_POWER:
            result = ShdSetPower();
            break;
        case FUNC_SET_CHANNELS:
            result = ShdSetChannels();
            break;
#ifdef SHD_CMDS
        case FUNC_COMMAND:
            result = DecodeCommand(kShdCommands, ShdCommand);
            break;
#endif
        }
    }
    return result;
}

#endif  // USE_SHELLY_DIMMER
#endif  // USE_LIGHT