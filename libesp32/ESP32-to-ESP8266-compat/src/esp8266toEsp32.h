/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#pragma once
#ifdef ESP32
// 
// basics
//

#define SPIFFS_END        (SPI_FLASH_SEC_SIZE*200)
#define SETTINGS_LOCATION SPIFFS_END

#define SPI_FLASH_RESULT_OK 1

#include <Esp.h>

//
// ESP32
//
String ESP_getResetReason(void);
String ESP_getBootVersion(void);
bool ESP_rtcUserMemoryWrite(uint32_t offset, uint32_t *data, size_t size);
bool ESP_rtcUserMemoryRead(uint32_t offset, uint32_t *data, size_t size);
void ESP_reset();
String ESP_getResetInfo(void);
uint32_t ESP_getFlashChipId();
String String_ESP_getChipId();

//void Serial_set_tx(uint8_t tx_pin);

// Analog
inline void analogWrite(uint8_t pin, int val)
{
}

inline void analogWriteFreq(uint32_t freq)
{
}
inline void analogWriteRange(uint32_t range)
{
}

#define INPUT_PULLDOWN_16	INPUT_PULLUP

/*
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
*/
typedef double real64_t;

//
// Time and Timer
//
String GetTime(int type);
uint32_t sntp_get_current_timestamp(void);
#define ETS_UART_INTR_DISABLE()
#define ETS_UART_INTR_ENABLE()

void GpioInitESP32();
#define getChipId() getEfuseMac()
#define ESPhttpUpdate httpUpdate
#define getFlashChipRealSize() getFlashChipSize()

#define os_delay_us ets_delay_us
typedef int SerialConfig;

// SNTP stuff
void SntpInit(const char *SntpServer1, const char *SntpServer2, const char *SntpServer3);

char* sntp_get_real_time(long t);
#define analogWrite(a,b)
#define PortUdp_writestr(log_data) PortUdp.write((const uint8_t *)(log_data), strlen(log_data))
#define PortUdp_write(log_data, n) PortUdp.write((const uint8_t *)(log_data), n)
//
// WS2812
//
#define NeoEsp8266BitBang800KbpsMethod  NeoEsp32BitBang800KbpsMethod
//
// UDP 
//
#define wifi_forceSleepBegin()

#undef LWIP_IPV6

struct rst_info 
{
    int reason;
};

#define REASON_DEFAULT_RST 1
#define REASON_EXT_SYS_RST 2

// memmove ...
#define memcpy_P memcpy
#define memmove_P memmove
#define strncpy_P strncpy
#define strcmp_P strcmp
#define memccpy_P memccpy
#define snprintf_P snprintf
#define sprintf_P sprintf
#define strncmp_P strncmp

#endif
