#ifdef ESP32
#include <esp8266toEsp32.h>
#undef USE_ADC_VCC      // Needs to be ported
#undef USE_IR_REMOTE    // IRremoteESP8266 needs to be ported
#undef SNFBRIDGE        // No commercial device, no sense porting
// Modul
#undef MODULE
#define MODULE   WEMOS // [Module] Select default model
// mqtt
#undef MQTT_LIBRARY_TYPE
#define MQTT_LIBRARY_TYPE      MQTT_PUBSUBCLIENT
#undef USE_DISCOVERY
#undef USE_DOMOTICZ
#undef USE_HOME_ASSISTANT
#undef USE_I2C
#undef USE_MHZ19
#undef USE_SENSEAIR
#undef USE_IR_REMOTE
#undef USE_ASYNC_UDP
#undef USE_RF_FLASH
#undef USE_TX20_WIND_SENSOR
#undef USE_MCP39F501
#undef USE_ARILUX_RF
#undef USE_ARMTRONIX_DIMMERS
#undef USE_TUYA_DIMMER
#undef USE_SONOFF_RF
#undef USE_SONOFF_SC
#undef USE_SONOFF_IFAN
#undef USE_PS_16_DZ
#undef USE_SONOFF_L1
#undef USE_DEEPSLEEP
#undef USE_EMULATION_WEMO
#undef USE_MY92X1
#undef USE_TUYA_MCU
#undef USE_ESPTOOL

#endif

#ifdef ESP8266
// ESP8266
#define ESP_rtcUserMemoryWrite(offset, data, size)	ESP.rtcUserMemoryWrite(offset, data, size)
#define ESP_rtcUserMemoryRead(offset, data, size)	ESP.rtcUserMemoryRead(offset, data, size)
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

extern "C" uint32 sntp_get_current_timestamp();
#endif
