/*
 * NimBLELog.h
 *
 *  Created: on Feb 24 2020
 *      Author H2zero
 *
 */
#ifndef MAIN_NIMBLELOG_H_
#define MAIN_NIMBLELOG_H_

#include "nimconfig.h"

#if defined(CONFIG_BT_ENABLED)

#if defined(CONFIG_NIMBLE_CPP_IDF) // using esp-idf

#include "esp_log.h"

#define NIMBLE_LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGC(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)

#else // using Arduino

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/console/console.h"

// If Arduino is being used, strip out the colors and ignore log printing below ui setting.
// Note: because CONFIG_LOG_DEFAULT_LEVEL is set at ERROR in Arduino we must use MODLOG_DFLT(ERROR
// otherwise no messages will be printed above that level.

#ifndef NIMBLE_CPP_DEBUG_LEVEL
  #if defined(ARDUINO_ARCH_ESP32) && defined(CORE_DEBUG_LEVEL)
    #define NIMBLE_CPP_DEBUG_LEVEL CORE_DEBUG_LEVEL
  #else
    #define NIMBLE_CPP_DEBUG_LEVEL 0
  #endif
#endif

#if NIMBLE_CPP_DEBUG_LEVEL >= 4
#define NIMBLE_LOGD( tag, format, ... ) console_printf("D %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGD( tag, format, ... ) (void)tag
#endif

#if NIMBLE_CPP_DEBUG_LEVEL >= 3
#define NIMBLE_LOGI( tag, format, ... ) console_printf("I %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGI( tag, format, ... ) (void)tag
#endif

#if NIMBLE_CPP_DEBUG_LEVEL >= 2
#define NIMBLE_LOGW( tag, format, ... ) console_printf("W %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGW( tag, format, ... ) (void)tag
#endif

#if NIMBLE_CPP_DEBUG_LEVEL >= 1
#define NIMBLE_LOGE( tag, format, ... ) console_printf("E %s: "#format"\n",tag,##__VA_ARGS__)
#define NIMBLE_LOGC( tag, format, ... ) console_printf("CRIT %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGE( tag, format, ... ) (void)tag
#define NIMBLE_LOGC( tag, format, ... ) (void)tag
#endif



#endif /* CONFIG_NIMBLE_CPP_IDF */
#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLELOG_H_ */
