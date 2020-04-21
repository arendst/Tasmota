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

#include "syscfg/syscfg.h"
#include "modlog/modlog.h"


// If Arduino is being used, strip out the colors and ignore log printing below ui setting. 
// Note: because CONFIG_LOG_DEFAULT_LEVEL is set at ERROR in Arduino we must use MODLOG_DFLT(ERROR 
// otherwise no messages will be printed above that level.
#ifdef ARDUINO_ARCH_ESP32
#ifndef CORE_DEBUG_LEVEL 
#define CORE_DEBUG_LEVEL CONFIG_ARDUHAL_LOG_DEFAULT_LEVEL
#endif

#if CORE_DEBUG_LEVEL >= 4
#define NIMBLE_LOGD( tag, format, ... ) MODLOG_DFLT(ERROR,      "D %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGD( tag, format, ... )
#endif

#if CORE_DEBUG_LEVEL >= 3
#define NIMBLE_LOGI( tag, format, ... ) MODLOG_DFLT(ERROR,      "I %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGI( tag, format, ... )
#endif

#if CORE_DEBUG_LEVEL >= 2
#define NIMBLE_LOGW( tag, format, ... ) MODLOG_DFLT(ERROR,      "W %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGW( tag, format, ... )
#endif

#if CORE_DEBUG_LEVEL >= 1
#define NIMBLE_LOGE( tag, format, ... ) MODLOG_DFLT(ERROR,      "E %s: "#format"\n",tag,##__VA_ARGS__)
#else
#define NIMBLE_LOGE( tag, format, ... )
#endif

#define NIMBLE_LOGC( tag, format, ... ) MODLOG_DFLT(CRITICAL,   "CRIT %s: "#format"\n",tag,##__VA_ARGS__)

#else 
#define NIMBLE_LOGE( tag, format, ... ) MODLOG_DFLT(ERROR,      "\033[0;31mE %s: "#format"\033[0m\n",tag,##__VA_ARGS__)
#define NIMBLE_LOGW( tag, format, ... ) MODLOG_DFLT(WARN,       "\033[0;33mW %s: "#format"\033[0m\n",tag,##__VA_ARGS__)
#define NIMBLE_LOGI( tag, format, ... ) MODLOG_DFLT(INFO,       "\033[0;32mI %s: "#format"\033[0m\n",tag,##__VA_ARGS__)
#define NIMBLE_LOGD( tag, format, ... ) MODLOG_DFLT(DEBUG,      "D %s: "#format"\n",tag,##__VA_ARGS__)
#define NIMBLE_LOGC( tag, format, ... ) MODLOG_DFLT(CRITICAL,   "\033[1;31mCRIT %s: "#format"\033[0m\n",tag,##__VA_ARGS__)
#endif /*ARDUINO_ARCH_ESP32*/

#endif /*CONFIG_BT_ENABLED*/
#endif /*MAIN_NIMBLELOG_H_*/