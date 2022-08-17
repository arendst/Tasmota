/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_LOG_COMMON_
#define H_LOG_COMMON_

#include <stdint.h>
#include "ignore.h"

#ifdef __cplusplus
extern "C" {
#endif

struct log;

#define LOG_VERSION_V3  3

#define LOG_TYPE_STREAM  (0)
#define LOG_TYPE_MEMORY  (1)
#define LOG_TYPE_STORAGE (2)

/*
#define LOG_LEVEL_DEBUG    (0)
#define LOG_LEVEL_INFO     (1)
#define LOG_LEVEL_WARN     (2)
#define LOG_LEVEL_ERROR    (3)
#define LOG_LEVEL_CRITICAL (4)
#define LOG_LEVEL_NONE     (5)
*/
#define LOG_LEVEL_DEBUG    (3)
#define LOG_LEVEL_INFO     (2)
#define LOG_LEVEL_WARN     (4)
#define LOG_LEVEL_ERROR    (1)
#define LOG_LEVEL_CRITICAL (5)
#define LOG_LEVEL_NONE     (0)
/* Up to 7 custom log levels. */
#define LOG_LEVEL_MAX      (15)

#define LOG_LEVEL_STR(level) \
    (LOG_LEVEL_DEBUG    == level ? "DEBUG"    :\
    (LOG_LEVEL_INFO     == level ? "INFO"     :\
    (LOG_LEVEL_WARN     == level ? "WARN"     :\
    (LOG_LEVEL_ERROR    == level ? "ERROR"    :\
    (LOG_LEVEL_CRITICAL == level ? "CRITICAL" :\
     "UNKNOWN")))))

/* XXX: These module IDs are defined for backwards compatibility.  Application
 * code should use the syscfg settings directly.  These defines will be removed
 * in a future release.
 */
#define LOG_MODULE_DEFAULT          0
#define LOG_MODULE_OS               1
#define LOG_MODULE_NEWTMGR          2
#define LOG_MODULE_NIMBLE_CTLR      3
#define LOG_MODULE_NIMBLE_HOST      4
#define LOG_MODULE_NFFS             5
#define LOG_MODULE_REBOOT           6
#define LOG_MODULE_IOTIVITY         7
#define LOG_MODULE_TEST             8

#define LOG_MODULE_PERUSER          64
#define LOG_MODULE_MAX              (255)

#define LOG_ETYPE_STRING         (0)
#define LOG_ETYPE_CBOR           (1)
#define LOG_ETYPE_BINARY         (2)

/* UTC Timestamp for Jan 2016 00:00:00 */
#define UTC01_01_2016    1451606400

#define LOG_NAME_MAX_LEN    (64)

#ifndef MYNEWT_VAL_LOG_LEVEL
#define LOG_SYSLEVEL    ((uint8_t)LOG_LEVEL_MAX)
#else
#define LOG_SYSLEVEL    ((uint8_t)MYNEWT_VAL_LOG_LEVEL)
#endif

/**
 * @brief Determines if a log module will accept an entry with a given level.
 *
 * A log entry is only accepted if its level is less than or equal to both:
 *    o Global log level setting (LOG_LEVEL), and
 *    o The specified module log level
 *
 * @param mod_level             The module's minimum log level.
 * @param entry_level           The level of the entry to be logged.
 *
 * @return                      true if the entry would be logged;
 *                              false otherwise.
 */
#define LOG_MOD_LEVEL_IS_ACTIVE(mod_level, entry_level) \
    (LOG_LEVEL <= (entry_level) && (mod_level) <= (entry_level))

/* Newtmgr Log opcodes */
#define LOGS_NMGR_OP_READ               (0)
#define LOGS_NMGR_OP_CLEAR              (1)
#define LOGS_NMGR_OP_APPEND             (2)
#define LOGS_NMGR_OP_MODULE_LIST        (3)
#define LOGS_NMGR_OP_LEVEL_LIST         (4)
#define LOGS_NMGR_OP_LOGS_LIST          (5)
#define LOGS_NMGR_OP_SET_WATERMARK      (6)
#define LOGS_NMGR_OP_MODLEVEL           (8)

#define LOG_PRINTF_MAX_ENTRY_LEN (128)

/* Global log info */
struct log_info {
#if MYNEWT_VAL(LOG_GLOBAL_IDX)
    uint32_t li_next_index;
#endif
    uint8_t li_version;
};

extern struct log_info g_log_info;

/** @typedef log_append_cb
 * @brief Callback that is executed each time the corresponding log is appended
 * to.
 *
 * @param log                   The log that was just appended to.
 * @param idx                   The index of newly appended log entry.
 */
typedef void log_append_cb(struct log *log, uint32_t idx);

/** @typdef log_notify_rotate_cb
 * @brief Callback that is executed each time we are about to rotate a log.
 *
 * @param log                   The log that is about to rotate
 */
typedef void log_notify_rotate_cb(const struct log *log);

#ifdef __cplusplus
}
#endif

#endif
