
#define Serial_Debug1(p) Serial.printf p
#define Serial_DebugX(p)

#ifdef ESP32
#include <esp8266toEsp32.h>
#define PACKED __attribute((__packed__))
// Modul
#undef MODULE
#define MODULE WEMOS // [Module] Select default model
#endif

#ifdef ESP8266
// ESP8266
#define PACKED
#define ESP_rtcUserMemoryWrite(offset, data, size) ESP.rtcUserMemoryWrite(offset, data, size)
#define ESP_rtcUserMemoryRead(offset, data, size) ESP.rtcUserMemoryRead(offset, data, size)
#define ESP_getResetReason() ESP.getResetReason()
#define ESP_reset() ESP.reset()
#define ESP_getBootVersion() ESP.getBootVersion()
#define ESP_getFlashChipId() ESP.getFlashChipId()

//
// we need different ESP_flashRead for ESP32
//
inline bool ESP_flashRead(uint32_t offset, uint32_t *data, size_t size)
{
    return ESP.flashRead(offset, data, size);
}

inline bool ESP_flashReadHeader(uint32_t offset, uint32_t *data, size_t size)
{
    return ESP.flashRead(offset, data, size);
}
#endif
