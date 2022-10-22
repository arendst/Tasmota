/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   ccronexpr.h
 * Author: alex
 *
 * Created on February 24, 2015, 9:35 AM
 */

#ifndef CCRONEXPR_H
#define CCRONEXPR_H

#if defined(__cplusplus) && !defined(CRON_COMPILE_AS_CXX)
extern "C" {
#endif

#ifndef ANDROID
#include <time.h>
#else /* ANDROID */
#include <time64.h>
#endif /* ANDROID */

#include <stdint.h> /*added for use if uint*_t data types*/


#define CRON_INVALID_INSTANT ((time_t) -1)

// Define to use local time
#define CRON_USE_LOCAL_TIME

/**
 * Parsed cron expression
 */
typedef struct {
    uint8_t seconds[8];
    uint8_t minutes[8];
    uint8_t hours[3];
    uint8_t days_of_week[1];
    uint8_t days_of_month[4];
    uint8_t months[2];
} cron_expr;

/**
 * Parses specified cron expression.
 * 
 * @param expression cron expression as nul-terminated string,
 *        should be no longer that 256 bytes
 * @param pointer to cron expression structure, it's client code responsibility
 *        to free/destroy it afterwards
 * @param error output error message, will be set to string literal
 *        error message in case of error. Will be set to NULL on success.
 *        The error message should NOT be freed by client.
 */
void cron_parse_expr(const char* expression, cron_expr* target, const char** error);

/**
 * Uses the specified expression to calculate the next 'fire' date after
 * the specified date. All dates are processed as UTC (GMT) dates 
 * without timezones information. To use local dates (current system timezone) 
 * instead of GMT compile with '-DCRON_USE_LOCAL_TIME'
 * 
 * @param expr parsed cron expression to use in next date calculation
 * @param date start date to start calculation from
 * @return next 'fire' date in case of success, '((time_t) -1)' in case of error.
 */
time_t cron_next(cron_expr* expr, time_t date);

/**
 * Uses the specified expression to calculate the previous 'fire' date after
 * the specified date. All dates are processed as UTC (GMT) dates 
 * without timezones information. To use local dates (current system timezone) 
 * instead of GMT compile with '-DCRON_USE_LOCAL_TIME'
 * 
 * @param expr parsed cron expression to use in previous date calculation
 * @param date start date to start calculation from
 * @return previous 'fire' date in case of success, '((time_t) -1)' in case of error.
 */
time_t cron_prev(cron_expr* expr, time_t date);


#if defined(__cplusplus) && !defined(CRON_COMPILE_AS_CXX)
} /* extern "C"*/
#endif

#endif /* CCRONEXPR_H */

