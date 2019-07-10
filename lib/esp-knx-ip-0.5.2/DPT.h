/**
 * esp-knx-ip library for KNX/IP communication on an ESP8266
 * Author: Nico Weichbrodt <envy>
 * License: MIT
 */

typedef enum __dpt_1_001
{
	DPT_1_001_OFF = 0x00,
	DPT_1_001_ON  = 0x01,
} dpt_1_001_t;

typedef enum __dpt_2_001
{
	DPT_2_001_NO_OFF  = 0b00,
	DPT_2_001_NO_ON   = 0b01,
	DPT_2_001_YES_OFF = 0b10,
	DPT_2_001_YES_ON  = 0b11,
} dpt_2_001_t;

typedef enum __dpt_3_007
{
	DPT_3_007_DECREASE_STOP = 0x00,
	DPT_3_007_DECREASE_100  = 0x01,
	DPT_3_007_DECREASE_50   = 0x02,
	DPT_3_007_DECREASE_25   = 0x03,
	DPT_3_007_DECREASE_12   = 0x04,
	DPT_3_007_DECREASE_6    = 0x05,
	DPT_3_007_DECREASE_3    = 0x06,
	DPT_3_007_DECREASE_1    = 0x07,
	DPT_3_007_INCREASE_STOP = 0x08,
	DPT_3_007_INCREASE_100  = 0x09,
	DPT_3_007_INCREASE_50   = 0x0A,
	DPT_3_007_INCREASE_25   = 0x0B,
	DPT_3_007_INCREASE_12   = 0x0C,
	DPT_3_007_INCREASE_6    = 0x0D,
	DPT_3_007_INCREASE_3    = 0x0E,
	DPT_3_007_INCREASE_1    = 0x0F,
} dpt_3_007_t;

typedef enum __weekday
{
	DPT_10_001_WEEKDAY_NODAY = 0,
	DPT_10_001_WEEKDAY_MONDAY = 1,
	DPT_10_001_WEEKDAY_TUESDAY = 2,
	DPT_10_001_WEEKDAY_WEDNESDAY = 3,
	DPT_10_001_WEEKDAY_THURSDAY = 4,
	DPT_10_001_WEEKDAY_FRIDAY = 5,
	DPT_10_001_WEEKDAY_SATURDAY = 6,
	DPT_10_001_WEEKDAY_SUNDAY = 7,
} weekday_t;

typedef struct __time_of_day
{
	weekday_t weekday;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} time_of_day_t;

typedef struct __date
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
} date_t;

typedef struct __color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} color_t;
