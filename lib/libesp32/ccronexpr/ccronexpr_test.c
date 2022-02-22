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
 * File:   CronExprParser_test.cpp
 * Author: alex
 *
 * Created on February 24, 2015, 9:36 AM
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "ccronexpr.h"

#define MAX_SECONDS 60
#define CRON_MAX_MINUTES 60
#define CRON_MAX_HOURS 24
#define CRON_MAX_DAYS_OF_WEEK 8
#define CRON_MAX_DAYS_OF_MONTH 32
#define CRON_MAX_MONTHS 12

#define INVALID_INSTANT ((time_t) -1)

#define DATE_FORMAT "%Y-%m-%d_%H:%M:%S"

#ifndef ARRAY_LEN
#define ARRAY_LEN(x) sizeof(x)/sizeof(x[0])
#endif

#ifdef CRON_TEST_MALLOC
static int cronAllocations = 0;
static int cronTotalAllocations = 0;
static int maxAlloc = 0;
void* cron_malloc(size_t n) {
    cronAllocations++;
    cronTotalAllocations++;
    if (cronAllocations > maxAlloc) {
        maxAlloc = cronAllocations;
    }
    return malloc(n);
}

void cron_free(void* p) {
    cronAllocations--;
    free(p);
}
#endif

#ifndef ANDROID
#ifndef _WIN32
time_t timegm(struct tm* __tp);
#else /* _WIN32 */
static time_t timegm(struct tm* tm) {
    return _mkgmtime(tm);
}
#endif /* _WIN32 */
#else /* ANDROID */
static time_t timegm(struct tm * const t) {
    /* time_t is signed on Android. */
    static const time_t kTimeMax = ~(1L << (sizeof (time_t) * CHAR_BIT - 1));
    static const time_t kTimeMin = (1L << (sizeof (time_t) * CHAR_BIT - 1));
    time64_t result = timegm64(t);
    if (result < kTimeMin || result > kTimeMax)
    return -1;
    return result;
}
#endif

/**
 * uint8_t* replace char* for storing hit dates, set_bit and get_bit are used as handlers
 */
uint8_t cron_get_bit(uint8_t* rbyte, int idx);
void cron_set_bit(uint8_t* rbyte, int idx);
void cron_del_bit(uint8_t* rbyte, int idx);

static int crons_equal(cron_expr* cr1, cron_expr* cr2) {
    unsigned int i;
    for (i = 0; i < ARRAY_LEN(cr1->seconds); i++) {
        if (cr1->seconds[i] != cr2->seconds[i]) {
            printf("seconds not equal @%d %02x != %02x", i, cr1->seconds[i], cr2->seconds[i]);
            return 0;
        }
    }
    for (i = 0; i < ARRAY_LEN(cr1->minutes); i++) {
        if (cr1->minutes[i] != cr2->minutes[i]) {
            printf("minutes not equal @%d %02x != %02x", i, cr1->minutes[i], cr2->minutes[i]);
            return 0;
        }
    }
    for (i = 0; i < ARRAY_LEN(cr1->hours); i++) {
        if (cr1->hours[i] != cr2->hours[i]) {
            printf("hours not equal @%d %02x != %02x", i, cr1->hours[i], cr2->hours[i]);
            return 0;
        }
    }
    for (i = 0; i < ARRAY_LEN(cr1->days_of_week); i++) {
        if (cr1->days_of_week[i] != cr2->days_of_week[i]) {
            printf("days_of_week not equal @%d %02x != %02x", i, cr1->days_of_week[i], cr2->days_of_week[i]);
            return 0;
        }
    }
    for (i = 0; i < ARRAY_LEN(cr1->days_of_month); i++) {
        if (cr1->days_of_month[i] != cr2->days_of_month[i]) {
            printf("days_of_month not equal @%d %02x != %02x", i, cr1->days_of_month[i], cr2->days_of_month[i]);
            return 0;
        }
    }
    for (i = 0; i < ARRAY_LEN(cr1->months); i++) {
        if (cr1->months[i] != cr2->months[i]) {
            printf("months not equal @%d %02x != %02x", i, cr1->months[i], cr2->months[i]);
            return 0;
        }
    }
    return 1;
}

int one_dec_num(const char ch) {
    switch (ch) {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    default:
        return -1;
    }
}

int two_dec_num(const char* first) {
    return one_dec_num(first[0]) * 10 + one_dec_num(first[1]);
}

int four_dec_num(const char *first) {
    return ((one_dec_num(first[0]) * 1000)
          + (one_dec_num(first[1]) * 100)
          + (one_dec_num(first[2]) * 10)
          + (one_dec_num(first[3]) * 1));
}

/* strptime is not available in msvc */
/* 2012-07-01_09:53:50 */
/* 0123456789012345678 */
struct tm* poors_mans_strptime(const char* str) {
    struct tm* cal = (struct tm*) malloc(sizeof(struct tm));
    assert(cal != NULL);
    memset(cal, 0, sizeof(struct tm));
    cal->tm_year = four_dec_num(str) - 1900;
    cal->tm_mon = two_dec_num(str + 5) - 1;
    cal->tm_mday = two_dec_num(str + 8);
    cal->tm_wday = 0;
    cal->tm_yday = 0;
    cal->tm_hour = two_dec_num(str + 11);
    cal->tm_min = two_dec_num(str + 14);
    cal->tm_sec = two_dec_num(str + 17);
    return cal;
}

void check_next(const char* pattern, const char* initial, const char* expected) {
    const char* err = NULL;
    cron_expr parsed;
    cron_parse_expr(pattern, &parsed, &err);

    struct tm* calinit = poors_mans_strptime(initial);
#ifdef CRON_USE_LOCAL_TIME
    time_t dateinit = mktime(calinit);
#else
    time_t dateinit = timegm(calinit);
#endif
    assert(-1 != dateinit);
    time_t datenext = cron_next(&parsed, dateinit);
#ifdef CRON_USE_LOCAL_TIME
    struct tm* calnext = localtime(&datenext);
#else
    struct tm* calnext = gmtime(&datenext);
#endif
    assert(calnext);
    char* buffer = (char*) malloc(21);
    memset(buffer, 0, 21);
    strftime(buffer, 20, DATE_FORMAT, calnext);
    if (0 != strcmp(expected, buffer)) {
        printf("Pattern: %s\n", pattern);
        printf("Initial: %s\n", initial);
        printf("Expected: %s\n", expected);
        printf("Actual: %s\n", buffer);
        assert(0);
    }
    free(buffer);
    free(calinit);
}

void check_same(const char* expr1, const char* expr2) {
    cron_expr parsed1;
    cron_parse_expr(expr1, &parsed1, NULL);
    cron_expr parsed2;
    cron_parse_expr(expr2, &parsed2, NULL);
    assert(crons_equal(&parsed1, &parsed2));
}

void check_calc_invalid() {
    cron_expr parsed;
    cron_parse_expr("0 0 0 31 6 *", &parsed, NULL);
    struct tm * calinit = poors_mans_strptime("2012-07-01_09:53:50");
    time_t dateinit = timegm(calinit);
    time_t res = cron_next(&parsed, dateinit);
    assert(INVALID_INSTANT == res);
    free(calinit);
}

void check_expr_invalid(const char* expr) {
    const char* err = NULL;
    cron_expr test;
    cron_parse_expr(expr, &test, &err);
    assert(err);
}

void test_expr() {
#ifdef CRON_USE_LOCAL_TIME
    check_next("* 15 11 * * *",     "2019-03-09_11:43:00", "2019-03-10_11:15:00");
#else
    check_next("*/15 * 1-4 * * *",  "2012-07-01_09:53:50", "2012-07-02_01:00:00");
    check_next("*/15 * 1-4 * * *",  "2012-07-01_09:53:00", "2012-07-02_01:00:00");
    check_next("0 */2 1-4 * * *",   "2012-07-01_09:00:00", "2012-07-02_01:00:00");
    check_next("0 */2 * * * *",     "2012-07-01_09:00:00", "2012-07-01_09:02:00");
    check_next("0 */2 * * * *",     "2013-07-01_09:00:00", "2013-07-01_09:02:00");
    check_next("0 */2 * * * *",     "2018-09-14_14:24:00", "2018-09-14_14:26:00");
    check_next("0 */2 * * * *",     "2018-09-14_14:25:00", "2018-09-14_14:26:00");
    check_next("0 */20 * * * *",    "2018-09-14_14:24:00", "2018-09-14_14:40:00");
    check_next("* * * * * *",       "2012-07-01_09:00:00", "2012-07-01_09:00:01");
    check_next("* * * * * *",       "2012-12-01_09:00:58", "2012-12-01_09:00:59");
    check_next("10 * * * * *",      "2012-12-01_09:42:09", "2012-12-01_09:42:10");
    check_next("11 * * * * *",      "2012-12-01_09:42:10", "2012-12-01_09:42:11");
    check_next("10 * * * * *",      "2012-12-01_09:42:10", "2012-12-01_09:43:10");
    check_next("10-15 * * * * *",   "2012-12-01_09:42:09", "2012-12-01_09:42:10");
    check_next("10-15 * * * * *",   "2012-12-01_21:42:14", "2012-12-01_21:42:15");
    check_next("0 * * * * *",       "2012-12-01_21:10:42", "2012-12-01_21:11:00");
    check_next("0 * * * * *",       "2012-12-01_21:11:00", "2012-12-01_21:12:00");
    check_next("0 11 * * * *",      "2012-12-01_21:10:42", "2012-12-01_21:11:00");
    check_next("0 10 * * * *",      "2012-12-01_21:11:00", "2012-12-01_22:10:00");
    check_next("0 0 * * * *",       "2012-09-30_11:01:00", "2012-09-30_12:00:00");
    check_next("0 0 * * * *",       "2012-09-30_12:00:00", "2012-09-30_13:00:00");
    check_next("0 0 * * * *",       "2012-09-10_23:01:00", "2012-09-11_00:00:00");
    check_next("0 0 * * * *",       "2012-09-11_00:00:00", "2012-09-11_01:00:00");
    check_next("0 0 0 * * *",       "2012-09-01_14:42:43", "2012-09-02_00:00:00");
    check_next("0 0 0 * * *",       "2012-09-02_00:00:00", "2012-09-03_00:00:00");
    check_next("* * * 10 * *",      "2012-10-09_15:12:42", "2012-10-10_00:00:00");
    check_next("* * * 10 * *",      "2012-10-11_15:12:42", "2012-11-10_00:00:00");
    check_next("0 0 0 * * *",       "2012-09-30_15:12:42", "2012-10-01_00:00:00");
    check_next("0 0 0 * * *",       "2012-10-01_00:00:00", "2012-10-02_00:00:00");
    check_next("0 0 0 * * *",       "2012-08-30_15:12:42", "2012-08-31_00:00:00");
    check_next("0 0 0 * * *",       "2012-08-31_00:00:00", "2012-09-01_00:00:00");
    check_next("0 0 0 * * *",       "2012-10-30_15:12:42", "2012-10-31_00:00:00");
    check_next("0 0 0 * * *",       "2012-10-31_00:00:00", "2012-11-01_00:00:00");
    check_next("0 0 0 1 * *",       "2012-10-30_15:12:42", "2012-11-01_00:00:00");
    check_next("0 0 0 1 * *",       "2012-11-01_00:00:00", "2012-12-01_00:00:00");
    check_next("0 0 0 1 * *",       "2010-12-31_15:12:42", "2011-01-01_00:00:00");
    check_next("0 0 0 1 * *",       "2011-01-01_00:00:00", "2011-02-01_00:00:00");
    check_next("0 0 0 31 * *",      "2011-10-30_15:12:42", "2011-10-31_00:00:00");
    check_next("0 0 0 1 * *",       "2011-10-30_15:12:42", "2011-11-01_00:00:00");
    check_next("* * * * * 2",       "2010-10-25_15:12:42", "2010-10-26_00:00:00");
    check_next("* * * * * 2",       "2010-10-20_15:12:42", "2010-10-26_00:00:00");
    check_next("* * * * * 2",       "2010-10-27_15:12:42", "2010-11-02_00:00:00");
    check_next("55 5 * * * *",      "2010-10-27_15:04:54", "2010-10-27_15:05:55");
    check_next("55 5 * * * *",      "2010-10-27_15:05:55", "2010-10-27_16:05:55");
    check_next("55 * 10 * * *",     "2010-10-27_09:04:54", "2010-10-27_10:00:55");
    check_next("55 * 10 * * *",     "2010-10-27_10:00:55", "2010-10-27_10:01:55");
    check_next("* 5 10 * * *",      "2010-10-27_09:04:55", "2010-10-27_10:05:00");
    check_next("* 5 10 * * *",      "2010-10-27_10:05:00", "2010-10-27_10:05:01");
    check_next("55 * * 3 * *",      "2010-10-02_10:05:54", "2010-10-03_00:00:55");
    check_next("55 * * 3 * *",      "2010-10-03_00:00:55", "2010-10-03_00:01:55");
    check_next("* * * 3 11 *",      "2010-10-02_14:42:55", "2010-11-03_00:00:00");
    check_next("* * * 3 11 *",      "2010-11-03_00:00:00", "2010-11-03_00:00:01");
    check_next("0 0 0 29 2 *",      "2007-02-10_14:42:55", "2008-02-29_00:00:00");
    check_next("0 0 0 29 2 *",      "2008-02-29_00:00:00", "2012-02-29_00:00:00");
    check_next("0 0 7 ? * MON-FRI", "2009-09-26_00:42:55", "2009-09-28_07:00:00");
    check_next("0 0 7 ? * MON-FRI", "2009-09-28_07:00:00", "2009-09-29_07:00:00");
    check_next("0 30 23 30 1/3 ?",  "2010-12-30_00:00:00", "2011-01-30_23:30:00");
    check_next("0 30 23 30 1/3 ?",  "2011-01-30_23:30:00", "2011-04-30_23:30:00");
    check_next("0 30 23 30 1/3 ?",  "2011-04-30_23:30:00", "2011-07-30_23:30:00");    
#endif
}

void test_parse() {

    check_same("* * * 2 * *", "* * * 2 * ?");
    check_same("57,59 * * * * *", "57/2 * * * * *");
    check_same("1,3,5 * * * * *", "1-6/2 * * * * *");
    check_same("* * 4,8,12,16,20 * * *", "* * 4/4 * * *");
    check_same("* * * * * 0-6", "* * * * * TUE,WED,THU,FRI,SAT,SUN,MON");
    check_same("* * * * * 0", "* * * * * SUN");
    check_same("* * * * * 0", "* * * * * 7");
    check_same("* * * * 1-12 *", "* * * * FEB,JAN,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC *");
    check_same("* * * * 2 *", "* * * * Feb *");
    check_same("*  *  * *  1 *", "* * * * 1 *");

    check_expr_invalid("77 * * * * *");
    check_expr_invalid("44-77 * * * * *");
    check_expr_invalid("* 77 * * * *");
    check_expr_invalid("* 44-77 * * * *");
    check_expr_invalid("* * 27 * * *");
    check_expr_invalid("* * 23-28 * * *");
    check_expr_invalid("* * * 45 * *");
    check_expr_invalid("* * * 28-45 * *");
    check_expr_invalid("0 0 0 25 13 ?");
    check_expr_invalid("0 0 0 25 0 ?");
    check_expr_invalid("0 0 0 32 12 ?");
    check_expr_invalid("* * * * 11-13 *");
    check_expr_invalid("-5 * * * * *");
    check_expr_invalid("3-2 */5 * * * *");
    check_expr_invalid("/5 * * * * *");
    check_expr_invalid("*/0 * * * * *");
    check_expr_invalid("*/-0 * * * * *");
    check_expr_invalid("* 1 1 0 * *");
}

void test_bits() {

    uint8_t testbyte[8];
    memset(testbyte, 0, 8);
    int err = 0;
    int i;

    for (i = 0; i <= 63; i++) {
        cron_set_bit(testbyte, i);
        if (!cron_get_bit(testbyte, i)) {
            printf("Bit set error! Bit: %d!\n", i);
            err = 1;
        }
        cron_del_bit(testbyte, i);
        if (cron_get_bit(testbyte, i)) {
            printf("Bit clear error! Bit: %d!\n", i);
            err = 1;
        }
        assert(!err);
    }

    for (i = 0; i < 12; i++) {
        cron_set_bit(testbyte, i);
    }
    if (testbyte[0] != 0xff) {
        err = 1;
    }
    if (testbyte[1] != 0x0f) {
        err = 1;
    }

    assert(!err);
}

/* For this test to work you need to set "-DCRON_TEST_MALLOC=1"*/
#ifdef CRON_TEST_MALLOC
void test_memory() {
    cron_expr cron;
    const char* err;

    cron_parse_expr("* * * * * *", &cron, &err);
    if (cronAllocations != 0) {
        printf("Allocations != 0 but %d", cronAllocations);
        assert(0);
    }
    printf("Allocations: total: %d, max: %d", cronTotalAllocations, maxAlloc);
}
#endif

int main() {

    test_bits();

    test_expr();
    test_parse();
    check_calc_invalid();
    #ifdef CRON_TEST_MALLOC
    test_memory(); /* For this test to work you need to set "-DCRON_TEST_MALLOC=1"*/
    #endif
    printf("\nAll OK!");
    return 0;
}

