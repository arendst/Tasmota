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
#  include "esp_log.h"
#  include "console/console.h"
#  ifndef CONFIG_NIMBLE_CPP_LOG_LEVEL
#    define CONFIG_NIMBLE_CPP_LOG_LEVEL 0
#  endif

#  define NIMBLE_CPP_LOG_PRINT(level, tag, format, ...) do { \
    if (CONFIG_NIMBLE_CPP_LOG_LEVEL >= level) \
      ESP_LOG_LEVEL_LOCAL(level, tag, format, ##__VA_ARGS__); \
    } while(0)

#  define NIMBLE_LOGD(tag, format, ...) \
     NIMBLE_CPP_LOG_PRINT(ESP_LOG_DEBUG, tag, format, ##__VA_ARGS__)

#  define NIMBLE_LOGI(tag, format, ...) \
     NIMBLE_CPP_LOG_PRINT(ESP_LOG_INFO, tag, format, ##__VA_ARGS__)

#  define NIMBLE_LOGW(tag, format, ...) \
     NIMBLE_CPP_LOG_PRINT(ESP_LOG_WARN, tag, format, ##__VA_ARGS__)

#  define NIMBLE_LOGE(tag, format, ...) \
     NIMBLE_CPP_LOG_PRINT(ESP_LOG_ERROR, tag, format, ##__VA_ARGS__)

#else // using Arduino
#  include "nimble/porting/nimble/include/syscfg/syscfg.h"
#  include "nimble/console/console.h"
#  ifndef CONFIG_NIMBLE_CPP_LOG_LEVEL
#    if defined(ARDUINO_ARCH_ESP32) && defined(CORE_DEBUG_LEVEL)
#      define CONFIG_NIMBLE_CPP_LOG_LEVEL CORE_DEBUG_LEVEL
#    else
#      define CONFIG_NIMBLE_CPP_LOG_LEVEL 0
#    endif
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 4
#    define NIMBLE_LOGD( tag, format, ... ) console_printf("D %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#    define NIMBLE_LOGD( tag, format, ... ) (void)tag
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 3
#    define NIMBLE_LOGI( tag, format, ... ) console_printf("I %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#    define NIMBLE_LOGI( tag, format, ... ) (void)tag
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 2
#    define NIMBLE_LOGW( tag, format, ... ) console_printf("W %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#    define NIMBLE_LOGW( tag, format, ... ) (void)tag
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 1
#    define NIMBLE_LOGE( tag, format, ... ) console_printf("E %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#    define NIMBLE_LOGE( tag, format, ... ) (void)tag
#  endif

#endif /* CONFIG_NIMBLE_CPP_IDF */

#define NIMBLE_LOGC( tag, format, ... ) console_printf("CRIT %s: " format "\n", tag, ##__VA_ARGS__)

#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLELOG_H_ */
