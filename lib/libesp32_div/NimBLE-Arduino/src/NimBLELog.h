/*
 * NimBLELog.h
 *
 *  Created: on Feb 24 2020
 *      Author H2zero
 *
 */
#ifndef MAIN_NIMBLELOG_H_
#define MAIN_NIMBLELOG_H_

#include "sdkconfig.h"

#if defined(CONFIG_BT_ENABLED)

#ifdef ARDUINO_ARCH_ESP32
#include "syscfg/syscfg.h"
#include "modlog/modlog.h"

// If Arduino is being used, strip out the colors and ignore log printing below ui setting.
// Note: because CONFIG_LOG_DEFAULT_LEVEL is set at ERROR in Arduino we must use MODLOG_DFLT(ERROR
// otherwise no messages will be printed above that level.

#ifndef CORE_DEBUG_LEVEL
#define CORE_DEBUG_LEVEL CONFIG_ARDUHAL_LOG_DEFAULT_LEVEL
#endif

#if CORE_DEBUG_LEVEL >= 4
#define NIMBLE_LOGD( tag, format, ... ) MODLOG_DFLT(ERROR,      "D %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGD( tag, format, ... ) (void)tag
#endif

#if CORE_DEBUG_LEVEL >= 3
#define NIMBLE_LOGI( tag, format, ... ) MODLOG_DFLT(ERROR,      "I %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGI( tag, format, ... ) (void)tag
#endif

#if CORE_DEBUG_LEVEL >= 2
#define NIMBLE_LOGW( tag, format, ... ) MODLOG_DFLT(ERROR,      "W %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGW( tag, format, ... ) (void)tag
#endif

#if CORE_DEBUG_LEVEL >= 1
#define NIMBLE_LOGE( tag, format, ... ) MODLOG_DFLT(ERROR,      "E %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGE( tag, format, ... ) (void)tag
#endif

#define NIMBLE_LOGC( tag, format, ... ) MODLOG_DFLT(CRITICAL,   "CRIT %s: "#format"\n",tag,##__VA_ARGS__)

#else

#include "esp_log.h"

#define NIMBLE_LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define NIMBLE_LOGC(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)

#endif /*ARDUINO_ARCH_ESP32*/

#endif /*CONFIG_BT_ENABLED*/
#endif /*MAIN_NIMBLELOG_H_*/