/*
    xdrv_45_shelly_dimmer.ino - shelly dimmer support for Tasmota

    Copyright (C) 2021  James Turton

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
 * Shelly WiFi Dimmer v1 and v2 (ESP8266 w/ separate co-processor dimmer)
 *
 * {"NAME":"Shelly Dimmer 1","GPIO":[0,3200,0,3232,5568,5600,0,0,192,0,193,288,0,4736],"FLAG":0,"BASE":18}
 * {"NAME":"Shelly Dimmer 2","GPIO":[0,3200,0,3232,5568,5600,0,0,193,0,192,0,320,4736],"FLAG":0,"BASE":18}
 *
 * https://shelly.cloud/wifi-smart-home-automation-shelly-dimmer/
 * https://shelly.cloud/products/shelly-dimmer-2-smart-home-light-controller/
\*********************************************************************************************/

#define XDRV_45                     45
#define XNRG_31                     31

// #define SHELLY_DIMMER_DEBUG
// #define SHELLY_HW_DIMMING

#define SHD_LOGNAME                 "SHD: "

#ifdef SHELLY_CMDS
#define D_PRFX_SHD                  "Shd"
#define D_CMND_LEADINGEDGE          "LeadingEdge"
#define D_CMND_WARMUPBRIGHTNESS     "WarmupBrightness"
#define D_CMND_WARMUPTIME           "WarmupTime"
#endif // SHELLY_CMDS

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

#ifdef SHELLY_FW_UPGRADE
#include <stm32flash.h>
#endif // SHELLY_FW_UPGRADE

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
    bool present = false;
    uint8_t hw_version = 0;             // Dimmer 1 = 1 Dimmer 2 = 2
} Shd;

/*********************************************************************************************\
 * SHD firmware update Functions
\*********************************************************************************************/

#ifdef SHELLY_FW_UPGRADE

void ShdResetToDFUMode()
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Request co-processor reset in dfu mode"));
#endif  // SHELLY_DIMMER_DEBUG

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

bool ShdUpdateFirmware(uint8_t* data, uint32_t size)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Update firmware"));
#endif  // SHELLY_DIMMER_DEBUG

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

#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "STM32 erase memory"));
#endif  // SHELLY_DIMMER_DEBUG

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

            memcpy(buffer, p_st, sizeof(buffer));  // We need 4-byte bounadry flash access
            p_st += sizeof(buffer);

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

bool ShdPresent(void) {
  return Shd.present;
}

uint32_t ShdFlash(uint8_t* data, size_t size) {
#ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Updating firmware v%u.%u with %u bytes"), Shd.dimmer.version_major, Shd.dimmer.version_minor, size);
#endif  // SHELLY_DIMMER_DEBUG

  Serial.end();
  Serial.begin(115200, SERIAL_8E1);
  ShdResetToDFUMode();
  bool error = !ShdUpdateFirmware(data, size);  // Allow flash access without ESP.flashRead
  Serial.end();
  ShdResetToAppMode();
  Serial.begin(115200, SERIAL_8N1);
  ShdSendVersion();

  return error;
}

#endif // SHELLY_FW_UPGRADE

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
        uint16_t chksm_calc = checksum(&Shd.buffer[1], 3 + data_length);
        if (chksm != chksm_calc)
        {
#ifdef SHELLY_DIMMER_DEBUG
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Checksum: %x calculated: %x"), chksm, chksm_calc);
#endif  // SHELLY_DIMMER_DEBUG
            return 0;
        }

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
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "Tx Packet:"));
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)data, len);
#endif  // SHELLY_DIMMER_DEBUG

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

        // timeout
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "Serial send timeout"));
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
    uint16_t delta = 0;
    if (Shd.req_brightness > Shd.dimmer.brightness)
        delta = (Shd.req_brightness - Shd.dimmer.brightness) * 0.8;
    else
        delta = (Shd.dimmer.brightness - Shd.req_brightness) * 0.8;

    // Payload format:
    // [0-1] Brightness (%) * 10
    // [2-3] Delta brightness (%) * 8
    // [4-5] 0?? ToDo(jamesturton): Find out what this word is!

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
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Serial %p"), ShdSerial);
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set Brightness Want %d, Is %d"), Shd.req_brightness, Shd.dimmer.brightness);
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set Fade Want %d, Is %d"), Settings->light_speed, Shd.dimmer.fade_rate);
#endif  // SHELLY_DIMMER_DEBUG

    if (!ShdSerial)
        return false;

#ifdef SHELLY_HW_DIMMING
    // TODO(jamesturton): HW dimming seems to conflict with SW dimming. See how
    // we can disbale SW dimming when using HW dimming.
    if (Settings->light_speed != Shd.dimmer.fade_rate)
    {
        ShdSetBrightnessFade();
        ShdDebugState();
    }
    else
#endif // SHELLY_HW_DIMMING
    if (Shd.req_brightness != Shd.dimmer.brightness)
    {
        ShdSetBrightness();
        ShdDebugState();
    }

    return true;
}

void ShdDebugState()
{
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "MCU v%d.%d, Brightness:%d(%d%%), Power:%d, Fade:%d"),
                            Shd.dimmer.version_major, Shd.dimmer.version_minor,
                            Shd.dimmer.brightness,
                            changeUIntScale(Shd.dimmer.brightness, 0, 1000, 0, 100),
                            Shd.dimmer.power,
                            Shd.dimmer.fade_rate);
#endif  // SHELLY_DIMMER_DEBUG
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
                uint8_t hw_version_raw = Shd.buffer[pos + 0];

                uint16_t unknown_0 = Shd.buffer[pos + 1];

                uint16_t brightness = Shd.buffer[pos + 3] << 8 |
                        Shd.buffer[pos + 2];

                uint32_t wattage_raw = Shd.buffer[pos + 7] << 24 |
                        Shd.buffer[pos + 6] << 16 |
                        Shd.buffer[pos + 5] << 8 |
                        Shd.buffer[pos + 4];

                uint32_t voltage_raw = Shd.buffer[pos + 11] << 24 |
                        Shd.buffer[pos + 10] << 16 |
                        Shd.buffer[pos + 9] << 8 |
                        Shd.buffer[pos + 8];

                uint32_t current_raw = Shd.buffer[pos + 15] << 24 |
                        Shd.buffer[pos + 14] << 16 |
                        Shd.buffer[pos + 13] << 8 |
                        Shd.buffer[pos + 12];

                uint32_t fade_rate = Shd.buffer[pos + 16];

                float wattage = 0;
                if (wattage_raw > 0)
                    wattage = 880373 / (float)wattage_raw;

                float voltage = 0;
                if (voltage_raw > 0)
                    voltage = 347800 / (float)voltage_raw;

                float current = 0;
                if (current_raw > 0)
                    current = 1448 / (float)current_raw;

                if (hw_version_raw == 0)
                    Shd.hw_version = 1;
                else if (hw_version_raw == 1)
                    Shd.hw_version = 2;

#ifdef USE_ENERGY_SENSOR
                if (Shd.hw_version == 2)
                {
                    Energy.current_available = true;
                    Energy.voltage_available = true;
                }
                Energy.active_power[0] = wattage;
                Energy.voltage[0] = voltage;
                Energy.current[0] = current;
                Energy.apparent_power[0] = voltage * current;
                if ((voltage * current) > wattage)
                    Energy.reactive_power[0] = sqrt((voltage * current) * (voltage * current) - wattage * wattage);
                else
                    Energy.reactive_power[0] = 0;
                if (wattage > (voltage * current))
                    Energy.power_factor[0] = 1;
                else if ((voltage * current) == 0)
                    Energy.power_factor[0] = 0;
                else
                    Energy.power_factor[0] = wattage / (voltage * current);

                if (Shd.last_power_check > 10 && Energy.active_power[0] > 0)
                {
                    float kWhused = (float)Energy.active_power[0] * (Rtc.utc_time - Shd.last_power_check) / 36;
#ifdef SHELLY_DIMMER_DEBUG
                    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Adding %i mWh to todays usage from %lu to %lu"), (int)(kWhused * 10), Shd.last_power_check, Rtc.utc_time);
#endif  // SHELLY_DIMMER_DEBUG
                    Energy.kWhtoday += kWhused;
                    EnergyUpdateToday();
                }
                Shd.last_power_check = Rtc.utc_time;
#endif  // USE_ENERGY_SENSOR

#ifdef SHELLY_DIMMER_DEBUG
                AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "ShdPacketProcess: Dimmer %d Brightness:%d Power:%lu Voltage:%lu Current:%lu Fade:%d"), Shd.hw_version, brightness, wattage_raw, voltage_raw, current_raw, fade_rate);
#endif  // SHELLY_DIMMER_DEBUG
                Shd.dimmer.brightness = brightness;
                Shd.dimmer.power = wattage_raw;
                Shd.dimmer.fade_rate = fade_rate;
            }
            break;
        case SHD_VERSION_CMD:
            {
#ifdef SHELLY_DIMMER_DEBUG
                AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "ShdPacketProcess: Version: %u.%u"), Shd.buffer[pos + 1], Shd.buffer[pos]);
#endif  // SHELLY_DIMMER_DEBUG
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
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Request co-processor reset in app mode"));
#endif  // SHELLY_DIMMER_DEBUG

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

void ShdPoll(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Poll"));
#endif  // SHELLY_DIMMER_DEBUG

    if (!ShdSerial)
        return;

    ShdSendCmd(SHD_POLL_CMD, 0, 0);
    ShdSyncState();
}

bool ShdSendVersion(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Sending version command"));
#endif  // SHELLY_DIMMER_DEBUG
    return ShdSendCmd(SHD_VERSION_CMD, 0, 0);
}

void ShdGetSettings(void)
{
    char parameters[32];
    // Shd.req_brightness      = 0;
    Shd.leading_edge        = 0;
    // Shd.req_fade_rate       = 0;
    Shd.warmup_brightness   = 0;
    Shd.warmup_time         = 0;
    if (strstr(SettingsText(SET_SHD_PARAM), ",") != nullptr)
    {
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Loading params: %s"), SettingsText(SET_SHD_PARAM));
#endif  // SHELLY_DIMMER_DEBUG
        // Shd.req_brightness      = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 1));
        Shd.leading_edge        = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 2));
        // Shd.req_fade_rate       = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 3));
        Shd.warmup_brightness   = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 4));
        Shd.warmup_time         = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 5));
    }
}

void ShdSaveSettings(void)
{
    char parameters[32];
    snprintf_P(parameters, sizeof(parameters), PSTR("%d,%d,%d,%d,%d"),
               Shd.req_brightness, Shd.leading_edge, Shd.req_fade_rate, Shd.warmup_brightness, Shd.warmup_time);
    SettingsUpdateText(SET_SHD_PARAM, parameters);
}

void ShdInit(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Shelly Dimmer Driver Starting Tx %d Rx %d"), Pin(GPIO_TXD), Pin(GPIO_RXD));
#endif  // SHELLY_DIMMER_DEBUG

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
            AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Shelly Dimmer Co-processor Version v%u.%u"), Shd.dimmer.version_major, Shd.dimmer.version_minor);
            ShdGetSettings();
            ShdSaveSettings();

            ShdSendSettings();
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
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "Rx Packet:"));
            AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Shd.buffer, Shd.byte_counter);
#endif  // SHELLY_DIMMER_DEBUG
            Shd.byte_counter = 0;

            ShdPacketProcess();

            return true;
        }
        else if (check == 0)
        {
            // wrong data
#ifdef SHELLY_DIMMER_DEBUG
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Byte %i of received data frame is invalid. Rx Packet:"), Shd.byte_counter);
            Shd.byte_counter++;
            AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Shd.buffer, Shd.byte_counter);
#endif  // SHELLY_DIMMER_DEBUG
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

bool ShdModuleSelected(void) {
  if (PinUsed(GPIO_SHELLY_DIMMER_BOOT0) && PinUsed(GPIO_SHELLY_DIMMER_RST_INV)) {
    TasmotaGlobal.devices_present++;
    TasmotaGlobal.light_type = LT_SERIAL1;

    Shd.present = true;
  }
  return Shd.present;
}

bool ShdSetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "SetChannels:"));
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t *)XdrvMailbox.data, XdrvMailbox.data_len);
#endif  // SHELLY_DIMMER_DEBUG

    uint16_t brightness = ((uint32_t *)XdrvMailbox.data)[0];
    // Use dimmer_hw_min and dimmer_hw_max to constrain our values if the light should be on
    if (brightness > 0)
        brightness = changeUIntScale(brightness, 0, 255, Settings->dimmer_hw_min * 10, Settings->dimmer_hw_max * 10);
    Shd.req_brightness = brightness;

    ShdDebugState();

    return ShdSyncState();
}

bool ShdSetPower(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Set Power, Power 0x%02x"), XdrvMailbox.index);
#endif  // SHELLY_DIMMER_DEBUG

    Shd.req_on = (bool)XdrvMailbox.index;
    return ShdSyncState();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef SHELLY_CMDS

const char kShdCommands[] PROGMEM = D_PRFX_SHD "|"  // Prefix
  D_CMND_LEADINGEDGE "|"  D_CMND_WARMUPBRIGHTNESS "|" D_CMND_WARMUPTIME;

void (* const ShdCommand[])(void) PROGMEM = {
  &CmndShdLeadingEdge, &CmndShdWarmupBrightness, &CmndShdWarmupTime };

void CmndShdLeadingEdge(void)
{
    if (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1)
    {
        Shd.leading_edge = 2 - XdrvMailbox.payload;
        Settings->shd_leading_edge = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        if (Shd.leading_edge == 1)
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to trailing edge"));
        else
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to leading edge"));
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings->shd_leading_edge);
}

void CmndShdWarmupBrightness(void)
{
    if ((10 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 100))
    {
        Shd.warmup_brightness = XdrvMailbox.payload * 10;
        Settings->shd_warmup_brightness = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set warmup brightness to %d%%"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings->shd_warmup_brightness);
}

void CmndShdWarmupTime(void)
{
    if ((20 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 200))
    {
        Shd.warmup_time = XdrvMailbox.payload;
        Settings->shd_warmup_time = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set warmup time to %dms"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings->shd_warmup_time);
}

#endif // SHELLY_CMDS

/*********************************************************************************************\
 * Energy Interface
\*********************************************************************************************/

#ifdef USE_ENERGY_SENSOR
bool Xnrg31(uint8_t function) {
  bool result = false;

  if (Shd.present) {
    if (FUNC_PRE_INIT == function) {
#ifndef SHELLY_VOLTAGE_MON
      Energy.current_available = false;
      Energy.voltage_available = false;
#endif // SHELLY_VOLTAGE_MON
      Energy.use_overtemp = true;   // Use global temperature for overtemp detection
      TasmotaGlobal.energy_driver = XNRG_31;
    }
  }
  return result;
}
#endif  // USE_ENERGY_SENSOR

/*********************************************************************************************\
 * Driver Interface
\*********************************************************************************************/

bool Xdrv45(uint8_t function) {
  bool result = false;

  if (FUNC_MODULE_INIT == function) {
    result = ShdModuleSelected();
  } else if (Shd.present) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        ShdPoll();
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
#ifdef SHELLY_CMDS
      case FUNC_COMMAND:
        result = DecodeCommand(kShdCommands, ShdCommand);
        break;
#endif // SHELLY_CMDS
    }
  }

  return result;
}

#endif  // USE_SHELLY_DIMMER
#endif  // USE_LIGHT