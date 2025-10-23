/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NIMBLE_CPP_LOG_H_
#define NIMBLE_CPP_LOG_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "esp_log.h"
#  include "console/console.h"
#  ifndef CONFIG_NIMBLE_CPP_LOG_LEVEL
#   define CONFIG_NIMBLE_CPP_LOG_LEVEL 0
#  endif

#  if defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR)
#   if CONFIG_LOG_COLORS
#    if defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_BLACK)
#     define NIMBLE_CPP_LOG_COLOR_D		LOG_COLOR(LOG_COLOR_BLACK)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_RED)
#     define NIMBLE_CPP_LOG_COLOR_D     LOG_COLOR(LOG_COLOR_RED)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_GREEN)
#     define NIMBLE_CPP_LOG_COLOR_D     LOG_COLOR(LOG_COLOR_GREEN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_YELLOW)
#     define NIMBLE_CPP_LOG_COLOR_D     LOG_COLOR(LOG_COLOR_BROWN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_BLUE)
#     define NIMBLE_CPP_LOG_COLOR_D     LOG_COLOR(LOG_COLOR_BLUE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_PURPLE)
#     define NIMBLE_CPP_LOG_COLOR_D     LOG_COLOR(LOG_COLOR_PURPLE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_DEBUG_CYAN)
#     define NIMBLE_CPP_LOG_COLOR_D     LOG_COLOR(LOG_COLOR_CYAN)
#    else
#     define NIMBLE_CPP_LOG_COLOR_D
#    endif

#    if defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_BLACK)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_BLACK)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_RED)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_RED)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_GREEN)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_GREEN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_YELLOW)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_BROWN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_BLUE)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_BLUE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_PURPLE)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_PURPLE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_INFO_CYAN)
#     define NIMBLE_CPP_LOG_COLOR_I     LOG_COLOR(LOG_COLOR_CYAN)
#    else
#     define NIMBLE_CPP_LOG_COLOR_I
#    endif

#    if defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_BLACK)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_BLACK)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_RED)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_RED)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_GREEN)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_GREEN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_YELLOW)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_BROWN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_BLUE)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_BLUE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_PURPLE)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_PURPLE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_WARN_CYAN)
#     define NIMBLE_CPP_LOG_COLOR_W     LOG_COLOR(LOG_COLOR_CYAN)
#    else
#     define NIMBLE_CPP_LOG_COLOR_W
#    endif

#    if defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_BLACK)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_BLACK)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_RED)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_RED)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_GREEN)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_GREEN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_YELLOW)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_BROWN)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_BLUE)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_BLUE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_PURPLE)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_PURPLE)
#    elif defined(CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR_ERR_CYAN)
#     define NIMBLE_CPP_LOG_COLOR_E     LOG_COLOR(LOG_COLOR_CYAN)
#    else
#     define NIMBLE_CPP_LOG_COLOR_E
#    endif
#   else //CONFIG_LOG_COLORS
#    define NIMBLE_CPP_LOG_COLOR_D
#    define NIMBLE_CPP_LOG_COLOR_I
#    define NIMBLE_CPP_LOG_COLOR_W
#    define NIMBLE_CPP_LOG_COLOR_E
#   endif //CONFIG_LOG_COLORS

#   define NIMBLE_CPP_LOG_FORMAT(letter, format) NIMBLE_CPP_LOG_COLOR_##letter #letter " (%lu) %s: " format LOG_RESET_COLOR "\n"

#   define NIMBLE_CPP_LOG_LEVEL_LOCAL(level, tag, format, ...)                                                                                                          \
      do {                                                                                                                                                              \
        if (level==ESP_LOG_ERROR)       { esp_log_write(ESP_LOG_ERROR, tag, NIMBLE_CPP_LOG_FORMAT(E, format), esp_log_timestamp(), tag __VA_OPT__(,) __VA_ARGS__); }    \
        else if (level==ESP_LOG_WARN)   { esp_log_write(ESP_LOG_WARN, tag, NIMBLE_CPP_LOG_FORMAT(W, format), esp_log_timestamp(), tag __VA_OPT__(,) __VA_ARGS__); }     \
        else if (level==ESP_LOG_INFO)   { esp_log_write(ESP_LOG_INFO, tag, NIMBLE_CPP_LOG_FORMAT(I, format), esp_log_timestamp(), tag __VA_OPT__(,) __VA_ARGS__); }     \
        else                            { esp_log_write(ESP_LOG_DEBUG, tag, NIMBLE_CPP_LOG_FORMAT(D, format), esp_log_timestamp(), tag __VA_OPT__(,) __VA_ARGS__); }    \
      } while(0)

#   define NIMBLE_CPP_LOG_PRINT(level, tag, format, ...)                                                            \
      do {                                                                                                          \
        if (CONFIG_NIMBLE_CPP_LOG_LEVEL >= level) NIMBLE_CPP_LOG_LEVEL_LOCAL(level, tag, format, ##__VA_ARGS__);    \
      } while (0)

#  else
#   define NIMBLE_CPP_LOG_PRINT(level, tag, format, ...)                                                    \
      do {                                                                                                  \
        if (CONFIG_NIMBLE_CPP_LOG_LEVEL >= level) ESP_LOG_LEVEL_LOCAL(level, tag, format, ##__VA_ARGS__);   \
      } while (0)

#  endif /* CONFIG_NIMBLE_CPP_LOG_OVERRIDE_COLOR */

#  define NIMBLE_LOGD(tag, format, ...) NIMBLE_CPP_LOG_PRINT(ESP_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#  define NIMBLE_LOGI(tag, format, ...) NIMBLE_CPP_LOG_PRINT(ESP_LOG_INFO, tag, format, ##__VA_ARGS__)
#  define NIMBLE_LOGW(tag, format, ...) NIMBLE_CPP_LOG_PRINT(ESP_LOG_WARN, tag, format, ##__VA_ARGS__)
#  define NIMBLE_LOGE(tag, format, ...) NIMBLE_CPP_LOG_PRINT(ESP_LOG_ERROR, tag, format, ##__VA_ARGS__)

# else
#  include "nimble/porting/nimble/include/syscfg/syscfg.h"
#  include "nimble/console/console.h"
#  ifndef CONFIG_NIMBLE_CPP_LOG_LEVEL
#   if defined(ARDUINO_ARCH_ESP32) && defined(CORE_DEBUG_LEVEL)
#    define CONFIG_NIMBLE_CPP_LOG_LEVEL CORE_DEBUG_LEVEL
#   else
#    define CONFIG_NIMBLE_CPP_LOG_LEVEL 0
#   endif
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 4
#   define NIMBLE_LOGD(tag, format, ...) console_printf("D %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#   define NIMBLE_LOGD(tag, format, ...) (void)tag
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 3
#   define NIMBLE_LOGI(tag, format, ...) console_printf("I %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#   define NIMBLE_LOGI(tag, format, ...) (void)tag
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 2
#   define NIMBLE_LOGW(tag, format, ...) console_printf("W %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#   define NIMBLE_LOGW(tag, format, ...) (void)tag
#  endif

#  if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 1
#   define NIMBLE_LOGE(tag, format, ...) console_printf("E %s: " format "\n", tag, ##__VA_ARGS__)
#  else
#   define NIMBLE_LOGE(tag, format, ...) (void)tag
#  endif

# endif  /* CONFIG_NIMBLE_CPP_IDF */

#  define NIMBLE_LOGD_IF(cond, tag, format, ...) { if (cond) { NIMBLE_LOGD(tag, format, ##__VA_ARGS__); }}
#  define NIMBLE_LOGI_IF(cond, tag, format, ...) { if (cond) { NIMBLE_LOGI(tag, format, ##__VA_ARGS__); }}
#  define NIMBLE_LOGW_IF(cond, tag, format, ...) { if (cond) { NIMBLE_LOGW(tag, format, ##__VA_ARGS__); }}
#  define NIMBLE_LOGE_IF(cond, tag, format, ...) { if (cond) { NIMBLE_LOGE(tag, format, ##__VA_ARGS__); }}
#  define NIMBLE_LOGE_RC(rc, tag, format, ...) { if (rc) { NIMBLE_LOGE(tag, format "; rc=%d %s", ##__VA_ARGS__, rc, NimBLEUtils::returnCodeToString(rc)); }}

// The LOG_LEVEL macros are used to set the log level for the NimBLE stack, but they pollute the global namespace and would override the loglevel enum of Tasmota.
// So we undefine them here to avoid conflicts.

#ifdef LOG_LEVEL_DEBUG
#undef LOG_LEVEL_DEBUG
#endif

#ifdef LOG_LEVEL_DEBUG_MORE
#undef LOG_LEVEL_DEBUG_MORE
#endif

#ifdef LOG_LEVEL_INFO
#undef LOG_LEVEL_INFO
#endif

#ifdef LOG_LEVEL_NONE
#undef LOG_LEVEL_NONE
#endif

#ifdef LOG_LEVEL_ERROR
#undef LOG_LEVEL_ERROR
#endif

#endif   /* CONFIG_BT_ENABLED */
#endif   /* NIMBLE_CPP_LOG_H_ */