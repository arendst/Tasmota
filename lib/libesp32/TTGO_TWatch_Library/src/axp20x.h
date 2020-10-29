/////////////////////////////////////////////////////////////////
/*
MIT License

Copyright (c) 2019 lewis he

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

axp20x.h - Arduino library for X-Power AXP202 chip.
Created by Lewis he on April 1, 2019.
github:https://github.com/lewisxhe/AXP202X_Libraries
*/
/////////////////////////////////////////////////////////////////
#pragma once

#include <Arduino.h>
#include <Wire.h>

// #define AXP_DEBUG_PORT  Serial
#ifdef AXP_DEBUG_PORT
#define AXP_DEBUG(fmt, ...) AXP_DEBUG_PORT.printf_P((PGM_P)PSTR(fmt), ##__VA_ARGS__)
#else
#define AXP_DEBUG(...)
#endif

#ifndef RISING
#define RISING 0x01
#endif

#ifndef FALLING
#define FALLING 0x02
#endif

#ifdef _BV
#undef _BV
#endif
#define _BV(b) (1ULL << (b))

//! Error Code
#define AXP_PASS            (0)
#define AXP_FAIL            (-1)
#define AXP_INVALID         (-2)
#define AXP_NOT_INIT        (-3)
#define AXP_NOT_SUPPORT     (-4)
#define AXP_ARG_INVALID     (-5)

//! Chip Address
#define AXP202_SLAVE_ADDRESS (0x35)
#define AXP192_SLAVE_ADDRESS (0x34)
#define AXP173_SLAVE_ADDRESS (0x34)

//! Chip ID
#define AXP202_CHIP_ID 0x41
#define AXP192_CHIP_ID 0x03
#define AXP173_CHIP_ID 0xAD     //!Axp173 does not have a chip ID, given a custom ID

//! Logic states
#define AXP202_ON 1
#define AXP202_OFF 0

//! REG MAP
#define AXP202_STATUS (0x00)
#define AXP202_MODE_CHGSTATUS (0x01)
#define AXP202_OTG_STATUS (0x02)
#define AXP202_IC_TYPE (0x03)
#define AXP202_DATA_BUFFER1 (0x04)
#define AXP202_DATA_BUFFER2 (0x05)
#define AXP202_DATA_BUFFER3 (0x06)
#define AXP202_DATA_BUFFER4 (0x07)
#define AXP202_DATA_BUFFER5 (0x08)
#define AXP202_DATA_BUFFER6 (0x09)
#define AXP202_DATA_BUFFER7 (0x0A)
#define AXP202_DATA_BUFFER8 (0x0B)
#define AXP202_DATA_BUFFER9 (0x0C)
#define AXP202_DATA_BUFFERA (0x0D)
#define AXP202_DATA_BUFFERB (0x0E)
#define AXP202_DATA_BUFFERC (0x0F)
#define AXP202_LDO234_DC23_CTL (0x12)
#define AXP202_DC2OUT_VOL (0x23)
#define AXP202_LDO3_DC2_DVM (0x25)
#define AXP202_DC3OUT_VOL (0x27)
#define AXP202_LDO24OUT_VOL (0x28)
#define AXP202_LDO3OUT_VOL (0x29)
#define AXP202_IPS_SET (0x30)
#define AXP202_VOFF_SET (0x31)
#define AXP202_OFF_CTL (0x32)
#define AXP202_CHARGE1 (0x33)
#define AXP202_CHARGE2 (0x34)
#define AXP202_BACKUP_CHG (0x35)
#define AXP202_POK_SET (0x36)
#define AXP202_DCDC_FREQSET (0x37)
#define AXP202_VLTF_CHGSET (0x38)
#define AXP202_VHTF_CHGSET (0x39)
#define AXP202_APS_WARNING1 (0x3A)
#define AXP202_APS_WARNING2 (0x3B)
#define AXP202_TLTF_DISCHGSET (0x3C)
#define AXP202_THTF_DISCHGSET (0x3D)
#define AXP202_DCDC_MODESET (0x80)
#define AXP202_ADC_EN1 (0x82)
#define AXP202_ADC_EN2 (0x83)
#define AXP202_ADC_SPEED (0x84)
#define AXP202_ADC_INPUTRANGE (0x85)
#define AXP202_ADC_IRQ_RETFSET (0x86)
#define AXP202_ADC_IRQ_FETFSET (0x87)
#define AXP202_TIMER_CTL (0x8A)
#define AXP202_VBUS_DET_SRP (0x8B)
#define AXP202_HOTOVER_CTL (0x8F)
#define AXP202_GPIO0_CTL (0x90)
#define AXP202_GPIO0_VOL (0x91)
#define AXP202_GPIO1_CTL (0x92)
#define AXP202_GPIO2_CTL (0x93)
#define AXP202_GPIO012_SIGNAL (0x94)
#define AXP202_GPIO3_CTL (0x95)
#define AXP202_INTEN1 (0x40)
#define AXP202_INTEN2 (0x41)
#define AXP202_INTEN3 (0x42)
#define AXP202_INTEN4 (0x43)
#define AXP202_INTEN5 (0x44)
#define AXP202_INTSTS1 (0x48)
#define AXP202_INTSTS2 (0x49)
#define AXP202_INTSTS3 (0x4A)
#define AXP202_INTSTS4 (0x4B)
#define AXP202_INTSTS5 (0x4C)

//Irq control register
#define AXP192_INTEN1 (0x40)
#define AXP192_INTEN2 (0x41)
#define AXP192_INTEN3 (0x42)
#define AXP192_INTEN4 (0x43)
#define AXP192_INTEN5 (0x4A)
//Irq status register
#define AXP192_INTSTS1 (0x44)
#define AXP192_INTSTS2 (0x45)
#define AXP192_INTSTS3 (0x46)
#define AXP192_INTSTS4 (0x47)
#define AXP192_INTSTS5 (0x4D)

#define AXP192_DC1_VLOTAGE (0x26)
#define AXP192_LDO23OUT_VOL (0x28)
#define AXP192_GPIO0_CTL (0x90)
#define AXP192_GPIO0_VOL (0x91)
#define AXP192_GPIO1_CTL (0X92)
#define AXP192_GPIO2_CTL (0x93)
#define AXP192_GPIO012_SIGNAL (0x94)
#define AXP192_GPIO34_CTL (0x95)



/* axp 192/202 adc data register */
#define AXP202_BAT_AVERVOL_H8 (0x78)
#define AXP202_BAT_AVERVOL_L4 (0x79)
#define AXP202_BAT_AVERCHGCUR_H8 (0x7A)
#define AXP202_BAT_AVERCHGCUR_L4 (0x7B)
#define AXP202_BAT_AVERCHGCUR_L5 (0x7B)
#define AXP202_ACIN_VOL_H8 (0x56)
#define AXP202_ACIN_VOL_L4 (0x57)
#define AXP202_ACIN_CUR_H8 (0x58)
#define AXP202_ACIN_CUR_L4 (0x59)
#define AXP202_VBUS_VOL_H8 (0x5A)
#define AXP202_VBUS_VOL_L4 (0x5B)
#define AXP202_VBUS_CUR_H8 (0x5C)
#define AXP202_VBUS_CUR_L4 (0x5D)
#define AXP202_INTERNAL_TEMP_H8 (0x5E)
#define AXP202_INTERNAL_TEMP_L4 (0x5F)
#define AXP202_TS_IN_H8 (0x62)
#define AXP202_TS_IN_L4 (0x63)
#define AXP202_GPIO0_VOL_ADC_H8 (0x64)
#define AXP202_GPIO0_VOL_ADC_L4 (0x65)
#define AXP202_GPIO1_VOL_ADC_H8 (0x66)
#define AXP202_GPIO1_VOL_ADC_L4 (0x67)

#define AXP202_BAT_AVERDISCHGCUR_H8 (0x7C)
#define AXP202_BAT_AVERDISCHGCUR_L5 (0x7D)
#define AXP202_APS_AVERVOL_H8 (0x7E)
#define AXP202_APS_AVERVOL_L4 (0x7F)
#define AXP202_INT_BAT_CHGCUR_H8 (0xA0)
#define AXP202_INT_BAT_CHGCUR_L4 (0xA1)
#define AXP202_EXT_BAT_CHGCUR_H8 (0xA2)
#define AXP202_EXT_BAT_CHGCUR_L4 (0xA3)
#define AXP202_INT_BAT_DISCHGCUR_H8 (0xA4)
#define AXP202_INT_BAT_DISCHGCUR_L4 (0xA5)
#define AXP202_EXT_BAT_DISCHGCUR_H8 (0xA6)
#define AXP202_EXT_BAT_DISCHGCUR_L4 (0xA7)
#define AXP202_BAT_CHGCOULOMB3 (0xB0)
#define AXP202_BAT_CHGCOULOMB2 (0xB1)
#define AXP202_BAT_CHGCOULOMB1 (0xB2)
#define AXP202_BAT_CHGCOULOMB0 (0xB3)
#define AXP202_BAT_DISCHGCOULOMB3 (0xB4)
#define AXP202_BAT_DISCHGCOULOMB2 (0xB5)
#define AXP202_BAT_DISCHGCOULOMB1 (0xB6)
#define AXP202_BAT_DISCHGCOULOMB0 (0xB7)
#define AXP202_COULOMB_CTL (0xB8)
#define AXP202_BAT_POWERH8 (0x70)
#define AXP202_BAT_POWERM8 (0x71)
#define AXP202_BAT_POWERL8 (0x72)

#define AXP202_VREF_TEM_CTRL (0xF3)
#define AXP202_BATT_PERCENTAGE (0xB9)

/* bit definitions for AXP events, irq event */
/*  AXP202  */
#define AXP202_IRQ_USBLO (1)
#define AXP202_IRQ_USBRE (2)
#define AXP202_IRQ_USBIN (3)
#define AXP202_IRQ_USBOV (4)
#define AXP202_IRQ_ACRE (5)
#define AXP202_IRQ_ACIN (6)
#define AXP202_IRQ_ACOV (7)

#define AXP202_IRQ_TEMLO (8)
#define AXP202_IRQ_TEMOV (9)
#define AXP202_IRQ_CHAOV (10)
#define AXP202_IRQ_CHAST (11)
#define AXP202_IRQ_BATATOU (12)
#define AXP202_IRQ_BATATIN (13)
#define AXP202_IRQ_BATRE (14)
#define AXP202_IRQ_BATIN (15)

#define AXP202_IRQ_POKLO (16)
#define AXP202_IRQ_POKSH (17)
#define AXP202_IRQ_LDO3LO (18)
#define AXP202_IRQ_DCDC3LO (19)
#define AXP202_IRQ_DCDC2LO (20)
#define AXP202_IRQ_CHACURLO (22)
#define AXP202_IRQ_ICTEMOV (23)

#define AXP202_IRQ_EXTLOWARN2 (24)
#define AXP202_IRQ_EXTLOWARN1 (25)
#define AXP202_IRQ_SESSION_END (26)
#define AXP202_IRQ_SESS_AB_VALID (27)
#define AXP202_IRQ_VBUS_UN_VALID (28)
#define AXP202_IRQ_VBUS_VALID (29)
#define AXP202_IRQ_PDOWN_BY_NOE (30)
#define AXP202_IRQ_PUP_BY_NOE (31)

#define AXP202_IRQ_GPIO0TG (32)
#define AXP202_IRQ_GPIO1TG (33)
#define AXP202_IRQ_GPIO2TG (34)
#define AXP202_IRQ_GPIO3TG (35)
#define AXP202_IRQ_PEKFE (37)
#define AXP202_IRQ_PEKRE (38)
#define AXP202_IRQ_TIMER (39)

//Signal Capture
#define AXP202_BATT_VOLTAGE_STEP (1.1F)
#define AXP202_BATT_DISCHARGE_CUR_STEP (0.5F)
#define AXP202_BATT_CHARGE_CUR_STEP (0.5F)
#define AXP202_ACIN_VOLTAGE_STEP (1.7F)
#define AXP202_ACIN_CUR_STEP (0.625F)
#define AXP202_VBUS_VOLTAGE_STEP (1.7F)
#define AXP202_VBUS_CUR_STEP (0.375F)
#define AXP202_INTERNAL_TEMP_STEP (0.1F)
#define AXP202_APS_VOLTAGE_STEP (1.4F)
#define AXP202_TS_PIN_OUT_STEP (0.8F)
#define AXP202_GPIO0_STEP (0.5F)
#define AXP202_GPIO1_STEP (0.5F)
// AXP192 only
#define AXP202_GPIO2_STEP (0.5F)
#define AXP202_GPIO3_STEP (0.5F)

// AXP173
#define AXP173_EXTEN_DC2_CTL   (0x10)
#define AXP173_CTL_DC2_BIT      (0)
#define AXP173_CTL_EXTEN_BIT    (2)
#define AXP173_DC1_VLOTAGE      (0x26)
#define AXP173_LDO4_VLOTAGE     (0x27)

#define FORCED_OPEN_DCDC3(x) (x |= (AXP202_ON << AXP202_DCDC3))
#define BIT_MASK(x) (1 << x)
#define IS_OPEN(reg, channel) (bool)(reg & BIT_MASK(channel))

enum {
    AXP202_EXTEN = 0,
    AXP202_DCDC3 = 1,
    AXP202_LDO2 = 2,
    AXP202_LDO4 = 3,
    AXP202_DCDC2 = 4,
    AXP202_LDO3 = 6,
    AXP202_OUTPUT_MAX,
};

enum {
    AXP192_DCDC1 = 0,
    AXP192_DCDC3 = 1,
    AXP192_LDO2 = 2,
    AXP192_LDO3 = 3,
    AXP192_DCDC2 = 4,
    AXP192_EXTEN = 6,
    AXP192_OUTPUT_MAX,
};

enum {
    AXP173_DCDC1 = 0,
    AXP173_LDO4 = 1,
    AXP173_LDO2 = 2,
    AXP173_LDO3 = 3,
    AXP173_DCDC2 = 4,
    AXP173_EXTEN = 6,
    AXP173_OUTPUT_MAX,
};

typedef enum {
    AXP202_STARTUP_TIME_128MS,
    AXP202_STARTUP_TIME_3S,
    AXP202_STARTUP_TIME_1S,
    AXP202_STARTUP_TIME_2S,
} axp202_startup_time_t;

typedef enum {
    AXP192_STARTUP_TIME_128MS,
    AXP192_STARTUP_TIME_512MS,
    AXP192_STARTUP_TIME_1S,
    AXP192_STARTUP_TIME_2S,
} axp192_startup_time_t;

typedef enum {
    AXP_LONGPRESS_TIME_1S,
    AXP_LONGPRESS_TIME_1S5,
    AXP_LONGPRESS_TIME_2S,
    AXP_LONGPRESS_TIME_2S5,
} axp_loonPress_time_t;

typedef enum {
    AXP_POWER_OFF_TIME_4S,
    AXP_POWER_OFF_TIME_65,
    AXP_POWER_OFF_TIME_8S,
    AXP_POWER_OFF_TIME_16S,
} axp_poweroff_time_t;

//REG 33H: Charging control 1 Charging target-voltage setting
typedef enum {
    AXP202_TARGET_VOL_4_1V,
    AXP202_TARGET_VOL_4_15V,
    AXP202_TARGET_VOL_4_2V,
    AXP202_TARGET_VOL_4_36V
} axp_chargeing_vol_t;

//REG 82H: ADC Enable 1 register Parameter
typedef enum {
    AXP202_BATT_VOL_ADC1 = 1 << 7,
    AXP202_BATT_CUR_ADC1 = 1 << 6,
    AXP202_ACIN_VOL_ADC1 = 1 << 5,
    AXP202_ACIN_CUR_ADC1 = 1 << 4,
    AXP202_VBUS_VOL_ADC1 = 1 << 3,
    AXP202_VBUS_CUR_ADC1 = 1 << 2,
    AXP202_APS_VOL_ADC1 = 1 << 1,
    AXP202_TS_PIN_ADC1 = 1 << 0
} axp_adc1_func_t;

// REG 83H: ADC Enable 2 register Parameter
typedef enum {
    AXP202_TEMP_MONITORING_ADC2 = 1 << 7,
    AXP202_GPIO1_FUNC_ADC2 = 1 << 3,
    AXP202_GPIO0_FUNC_ADC2 = 1 << 2
} axp_adc2_func_t;

typedef enum {
    AXP202_LDO3_MODE_LDO,
    AXP202_LDO3_MODE_DCIN
} axp202_ldo3_mode_t;



typedef enum {
    //! IRQ1 REG 40H
    AXP202_VBUS_VHOLD_LOW_IRQ       = _BV(1),   //VBUS is available, but lower than V HOLD, IRQ enable
    AXP202_VBUS_REMOVED_IRQ         = _BV(2),   //VBUS removed, IRQ enable
    AXP202_VBUS_CONNECT_IRQ         = _BV(3),   //VBUS connected, IRQ enable
    AXP202_VBUS_OVER_VOL_IRQ        = _BV(4),   //VBUS over-voltage, IRQ enable
    AXP202_ACIN_REMOVED_IRQ         = _BV(5),   //ACIN removed, IRQ enable
    AXP202_ACIN_CONNECT_IRQ         = _BV(6),   //ACIN connected, IRQ enable
    AXP202_ACIN_OVER_VOL_IRQ        = _BV(7),   //ACIN over-voltage, IRQ enable

    //! IRQ2 REG 41H
    AXP202_BATT_LOW_TEMP_IRQ        = _BV(8),   //Battery low-temperature, IRQ enable
    AXP202_BATT_OVER_TEMP_IRQ       = _BV(9),   //Battery over-temperature, IRQ enable
    AXP202_CHARGING_FINISHED_IRQ    = _BV(10),  //Charge finished, IRQ enable
    AXP202_CHARGING_IRQ             = _BV(11),  //Be charging, IRQ enable
    AXP202_BATT_EXIT_ACTIVATE_IRQ   = _BV(12),  //Exit battery activate mode, IRQ enable
    AXP202_BATT_ACTIVATE_IRQ        = _BV(13),  //Battery activate mode, IRQ enable
    AXP202_BATT_REMOVED_IRQ         = _BV(14),  //Battery removed, IRQ enable
    AXP202_BATT_CONNECT_IRQ         = _BV(15),  //Battery connected, IRQ enable

    //! IRQ3 REG 42H
    AXP202_PEK_LONGPRESS_IRQ        = _BV(16),  //PEK long press, IRQ enable
    AXP202_PEK_SHORTPRESS_IRQ       = _BV(17),  //PEK short press, IRQ enable
    AXP202_LDO3_LOW_VOL_IRQ         = _BV(18),  //LDO3output voltage is lower than the set value, IRQ enable
    AXP202_DC3_LOW_VOL_IRQ          = _BV(19),  //DC-DC3output voltage is lower than the set value, IRQ enable
    AXP202_DC2_LOW_VOL_IRQ          = _BV(20),  //DC-DC2 output voltage is lower than the set value, IRQ enable
    //**Reserved and unchangeable BIT 5
    AXP202_CHARGE_LOW_CUR_IRQ       = _BV(22),  //Charge current is lower than the set current, IRQ enable
    AXP202_CHIP_TEMP_HIGH_IRQ       = _BV(23),  //AXP202 internal over-temperature, IRQ enable

    //! IRQ4 REG 43H
    AXP202_APS_LOW_VOL_LEVEL2_IRQ   = _BV(24),  //APS low-voltage, IRQ enable（LEVEL2）
    APX202_APS_LOW_VOL_LEVEL1_IRQ   = _BV(25),  //APS low-voltage, IRQ enable（LEVEL1）
    AXP202_VBUS_SESSION_END_IRQ     = _BV(26),  //VBUS Session End IRQ enable
    AXP202_VBUS_SESSION_AB_IRQ      = _BV(27),  //VBUS Session A/B IRQ enable
    AXP202_VBUS_INVALID_IRQ         = _BV(28),  //VBUS invalid, IRQ enable
    AXP202_VBUS_VAILD_IRQ           = _BV(29),  //VBUS valid, IRQ enable
    AXP202_NOE_OFF_IRQ              = _BV(30),  //N_OE shutdown, IRQ enable
    AXP202_NOE_ON_IRQ               = _BV(31),  //N_OE startup, IRQ enable

    //! IRQ5 REG 44H
    AXP202_GPIO0_EDGE_TRIGGER_IRQ   = _BV(32),  //GPIO0 input edge trigger, IRQ enable
    AXP202_GPIO1_EDGE_TRIGGER_IRQ   = _BV(33),  //GPIO1input edge trigger or ADC input, IRQ enable
    AXP202_GPIO2_EDGE_TRIGGER_IRQ   = _BV(34),  //GPIO2input edge trigger, IRQ enable
    AXP202_GPIO3_EDGE_TRIGGER_IRQ   = _BV(35),  //GPIO3 input edge trigger, IRQ enable
    //**Reserved and unchangeable BIT 4
    AXP202_PEK_FALLING_EDGE_IRQ     = _BV(37),  //PEK press falling edge, IRQ enable
    AXP202_PEK_RISING_EDGE_IRQ      = _BV(38),  //PEK press rising edge, IRQ enable
    AXP202_TIMER_TIMEOUT_IRQ        = _BV(39),  //Timer timeout, IRQ enable

    AXP202_ALL_IRQ                  = (0xFFFFFFFFFFULL)
} axp_irq_t;

typedef enum {
    AXP202_LDO4_1250MV,
    AXP202_LDO4_1300MV,
    AXP202_LDO4_1400MV,
    AXP202_LDO4_1500MV,
    AXP202_LDO4_1600MV,
    AXP202_LDO4_1700MV,
    AXP202_LDO4_1800MV,
    AXP202_LDO4_1900MV,
    AXP202_LDO4_2000MV,
    AXP202_LDO4_2500MV,
    AXP202_LDO4_2700MV,
    AXP202_LDO4_2800MV,
    AXP202_LDO4_3000MV,
    AXP202_LDO4_3100MV,
    AXP202_LDO4_3200MV,
    AXP202_LDO4_3300MV,
    AXP202_LDO4_MAX,
} axp_ldo4_table_t;

typedef enum {
    AXP202_LDO5_1800MV,
    AXP202_LDO5_2500MV,
    AXP202_LDO5_2800MV,
    AXP202_LDO5_3000MV,
    AXP202_LDO5_3100MV,
    AXP202_LDO5_3300MV,
    AXP202_LDO5_3400MV,
    AXP202_LDO5_3500MV,
} axp_ldo5_table_t;

typedef enum {
    AXP20X_LED_OFF,
    AXP20X_LED_BLINK_1HZ,
    AXP20X_LED_BLINK_4HZ,
    AXP20X_LED_LOW_LEVEL,
} axp_chgled_mode_t;

typedef enum {
    AXP_ADC_SAMPLING_RATE_25HZ = 0,
    AXP_ADC_SAMPLING_RATE_50HZ = 1,
    AXP_ADC_SAMPLING_RATE_100HZ = 2,
    AXP_ADC_SAMPLING_RATE_200HZ = 3,
} axp_adc_sampling_rate_t;

typedef enum {
    AXP_TS_PIN_CURRENT_20UA = 0,
    AXP_TS_PIN_CURRENT_40UA = 1,
    AXP_TS_PIN_CURRENT_60UA = 2,
    AXP_TS_PIN_CURRENT_80UA = 3,
} axp_ts_pin_current_t;

typedef enum {
    AXP_TS_PIN_FUNCTION_BATT = 0,
    AXP_TS_PIN_FUNCTION_ADC = 1,
} axp_ts_pin_function_t;

typedef enum {
    AXP_TS_PIN_MODE_DISABLE = 0,
    AXP_TS_PIN_MODE_CHARGING = 1,
    AXP_TS_PIN_MODE_SAMPLING = 2,
    AXP_TS_PIN_MODE_ENABLE = 3,
} axp_ts_pin_mode_t;

//! Only AXP192 and AXP202 have gpio function
typedef enum {
    AXP_GPIO_0,
    AXP_GPIO_1,
    AXP_GPIO_2,
    AXP_GPIO_3,
    AXP_GPIO_4,
} axp_gpio_t;

typedef enum {
    AXP_IO_OUTPUT_LOW_MODE,
    AXP_IO_OUTPUT_HIGH_MODE,
    AXP_IO_INPUT_MODE,
    AXP_IO_LDO_MODE,
    AXP_IO_ADC_MODE,
    AXP_IO_FLOATING_MODE,
    AXP_IO_OPEN_DRAIN_OUTPUT_MODE,
    AXP_IO_PWM_OUTPUT_MODE,
    AXP_IO_EXTERN_CHARGING_CTRL_MODE,
} axp_gpio_mode_t;

typedef enum {
    AXP_IRQ_NONE,
    AXP_IRQ_RISING,
    AXP_IRQ_FALLING,
    AXP_IRQ_DOUBLE_EDGE,
} axp_gpio_irq_t;


typedef enum {
    AXP192_GPIO_1V8,
    AXP192_GPIO_1V9,
    AXP192_GPIO_2V0,
    AXP192_GPIO_2V1,
    AXP192_GPIO_2V2,
    AXP192_GPIO_2V3,
    AXP192_GPIO_2V4,
    AXP192_GPIO_2V5,
    AXP192_GPIO_2V6,
    AXP192_GPIO_2V7,
    AXP192_GPIO_2V8,
    AXP192_GPIO_2V9,
    AXP192_GPIO_3V0,
    AXP192_GPIO_3V1,
    AXP192_GPIO_3V2,
    AXP192_GPIO_3V3,
} axp192_gpio_voltage_t;

typedef enum {
    AXP1XX_CHARGE_CUR_100MA,
    AXP1XX_CHARGE_CUR_190MA,
    AXP1XX_CHARGE_CUR_280MA,
    AXP1XX_CHARGE_CUR_360MA,
    AXP1XX_CHARGE_CUR_450MA,
    AXP1XX_CHARGE_CUR_550MA,
    AXP1XX_CHARGE_CUR_630MA,
    AXP1XX_CHARGE_CUR_700MA,
    AXP1XX_CHARGE_CUR_780MA,
    AXP1XX_CHARGE_CUR_880MA,
    AXP1XX_CHARGE_CUR_960MA,
    AXP1XX_CHARGE_CUR_1000MA,
    AXP1XX_CHARGE_CUR_1080MA,
    AXP1XX_CHARGE_CUR_1160MA,
    AXP1XX_CHARGE_CUR_1240MA,
    AXP1XX_CHARGE_CUR_1320MA,
} axp1xx_charge_current_t;

typedef uint8_t (*axp_com_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);


class AXP20X_Class
{
public:
    int begin(TwoWire &port = Wire, uint8_t addr = AXP202_SLAVE_ADDRESS, bool isAxp173 = false);
    int begin(axp_com_fptr_t read_cb, axp_com_fptr_t write_cb, uint8_t addr = AXP202_SLAVE_ADDRESS, bool isAxp173 = false);

    // Power Output Control
    int setPowerOutPut(uint8_t ch, bool en);

    bool isBatteryConnect();
    bool isChargeing();
    bool isLDO2Enable();
    bool isLDO3Enable();
    bool isLDO4Enable();
    bool isDCDC3Enable();
    bool isDCDC2Enable();
    bool isChargeingEnable();
    bool isVBUSPlug();
    bool isExtenEnable();

    //Only axp192 chip
    bool isDCDC1Enable();


    //IRQ Status
    bool isAcinOverVoltageIRQ();
    bool isAcinPlugInIRQ();
    bool isAcinRemoveIRQ();
    bool isVbusOverVoltageIRQ();
    bool isVbusPlugInIRQ();
    bool isVbusRemoveIRQ();
    bool isVbusLowVHOLDIRQ();

    bool isBattPlugInIRQ();
    bool isBattRemoveIRQ();
    bool isBattEnterActivateIRQ();
    bool isBattExitActivateIRQ();
    bool isChargingIRQ();
    bool isChargingDoneIRQ();
    bool isBattTempLowIRQ();
    bool isBattTempHighIRQ();

    bool isPEKShortPressIRQ();
    bool isPEKLongtPressIRQ();
    bool isTimerTimeoutIRQ();

    //! Group4 ADC data
    float getAcinVoltage();
    float getAcinCurrent();
    float getVbusVoltage();
    float getVbusCurrent();
    float getTemp();
    float getTSTemp();
    float getGPIO0Voltage();
    float getGPIO1Voltage();
    float getBattInpower();
    float getBattVoltage();
    float getBattChargeCurrent();
    float getBattDischargeCurrent();
    float getSysIPSOUTVoltage();
    uint32_t getBattChargeCoulomb();
    uint32_t getBattDischargeCoulomb();
    float getSettingChargeCurrent();

    int setChargingTargetVoltage(axp_chargeing_vol_t param);
    int enableChargeing(bool en);

    int adc1Enable(uint16_t params, bool en);
    int adc2Enable(uint16_t params, bool en);

    int setTScurrent(axp_ts_pin_current_t current);
    int setTSfunction(axp_ts_pin_function_t func);
    int setTSmode(axp_ts_pin_mode_t mode);


    int setTimer(uint8_t minutes);
    int offTimer();
    int clearTimerStatus();
    /**
     * param:   axp202_startup_time_t or axp192_startup_time_t
     */
    int setStartupTime(uint8_t param);

    /**
     * param: axp_loonPress_time_t
     */
    int setlongPressTime(uint8_t param);

    /**
     * @param  param: axp_poweroff_time_t
     */
    int setShutdownTime(uint8_t param);

    int setTimeOutShutdown(bool en);

    int shutdown();

    /**
     * params: axp_irq_t
     */
    int enableIRQ(uint64_t params, bool en);
    int readIRQ();
    void clearIRQ();

    int setDCDC1Voltage(uint16_t mv); //! Only AXP192 support and AXP173
    // return mv
    uint16_t getDCDC1Voltage(); //! Only AXP192 support and AXP173

    // -----------------

    /*
    !! Chip resource table
    | CHIP     | AXP173           | AXP192           | AXP202           |
    | -------- | ---------------- | ---------------- | ---------------- |
    | DC1      | 0v7~3v5  /1200mA | 0v7~3v5  /1200mA | X                |
    | DC2      | 0v7~2v275/1600mA | 0v7~2v275/1600mA | 0v7~2v275/1600mA |
    | DC3      | X                | 0v7~3v5  /700mA  | 0v7~3v5  /1200mA |
    | LDO1     | 3v3      /30mA   | 3v3      /30mA   | 3v3      /30mA   |
    | LDO2     | 1v8~3v3  /200mA  | 1v8~3v3  /200mA  | 1v8~3v3  /200mA  |
    | LDO3     | 1v8~3v3  /200mA  | 1v8~3v3  /200mA  | 0v7~3v3  /200mA  |
    | LDO4     | 0v7~3v5  /500mA  | X                | 1v8~3v3  /200mA  |
    | LDO5/IO0 | X                | 1v8~3v3  /50mA   | 1v8~3v3  /50mA   |
    */
    int setDCDC2Voltage(uint16_t mv);
    uint16_t getDCDC2Voltage();

    int setDCDC3Voltage(uint16_t mv);
    uint16_t getDCDC3Voltage();

    int setLDO2Voltage(uint16_t mv);
    uint16_t getLDO2Voltage();

    int setLDO3Voltage(uint16_t mv);
    uint16_t getLDO3Voltage();


    int setLDO4Voltage(axp_ldo4_table_t param); //! Only axp202 support
    int setLDO4Voltage(uint16_t mv);            //! Only axp173 support

    // return mv
    uint16_t getLDO4Voltage();                  //! Only axp173/axp202 support


    /**
     * @param  mode: axp_chgled_mode_t
     */
    int setChgLEDMode(axp_chgled_mode_t mode);

    /**
     * @param  mode: axp202_ldo3_mode_t
     */
    int setLDO3Mode(uint8_t mode); //! Only AXP202 support

    int getBattPercentage();

    int debugCharging();
    int debugStatus();
    int limitingOff();

    int setAdcSamplingRate(axp_adc_sampling_rate_t rate);
    uint8_t getAdcSamplingRate();
    float getCoulombData();
    uint8_t getCoulombRegister();
    int setCoulombRegister(uint8_t val);
    int EnableCoulombcounter(void);
    int DisableCoulombcounter(void);
    int StopCoulombcounter(void);
    int ClearCoulombcounter(void);


    int setGPIOMode(axp_gpio_t gpio, axp_gpio_mode_t mode);
    int setGPIOIrq(axp_gpio_t gpio, axp_gpio_irq_t irq);
    int setLDO5Voltage(axp_ldo5_table_t vol);

    int gpioWrite(axp_gpio_t gpio, uint8_t vol);
    int gpioRead(axp_gpio_t gpio);

    // When the chip is axp192 / 173, the allowed values are 0 ~ 15, corresponding to the axp1xx_charge_current_t enumeration
    // When the chip is axp202 allows maximum charging current of 1800mA, minimum 300mA
    int getChargeControlCur();
    int setChargeControlCur(uint16_t mA);

private:
    uint16_t _getRegistH8L5(uint8_t regh8, uint8_t regl5)
    {
        uint8_t hv, lv;
        _readByte(regh8, 1, &hv);
        _readByte(regl5, 1, &lv);
        return (hv << 5) | (lv & 0x1F);
    }

    uint16_t _getRegistResult(uint8_t regh8, uint8_t regl4)
    {
        uint8_t hv, lv;
        _readByte(regh8, 1, &hv);
        _readByte(regl4, 1, &lv);
        return (hv << 4) | (lv & 0x0F);
    }

    int _readByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
    {
        if (_read_cb != nullptr) {
            return _read_cb(_address, reg, data, nbytes);
        }
        if (nbytes == 0 || !data)
            return -1;
        _i2cPort->beginTransmission(_address);
        _i2cPort->write(reg);
        _i2cPort->endTransmission();
        _i2cPort->requestFrom(_address, nbytes);
        uint8_t index = 0;
        while (_i2cPort->available())
            data[index++] = _i2cPort->read();
        return 0;
    }

    int _writeByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
    {
        if (_write_cb != nullptr) {
            return _write_cb(_address, reg, data, nbytes);
        }
        if (nbytes == 0 || !data)
            return -1;
        _i2cPort->beginTransmission(_address);
        _i2cPort->write(reg);
        for (uint8_t i = 0; i < nbytes; i++) {
            _i2cPort->write(data[i]);
        }
        _i2cPort->endTransmission();
        return 0;
    }

    int _setGpioInterrupt(uint8_t *val, int mode, bool en);
    int _axp_probe();
    int _axp_irq_mask(axp_gpio_irq_t irq);

    int _axp192_gpio_set(axp_gpio_t gpio, axp_gpio_mode_t mode);
    int _axp192_gpio_0_select( axp_gpio_mode_t mode);
    int _axp192_gpio_1_select( axp_gpio_mode_t mode);
    int _axp192_gpio_3_select( axp_gpio_mode_t mode);
    int _axp192_gpio_4_select( axp_gpio_mode_t mode);

    int _axp202_gpio_set(axp_gpio_t gpio, axp_gpio_mode_t mode);
    int _axp202_gpio_0_select( axp_gpio_mode_t mode);
    int _axp202_gpio_1_select( axp_gpio_mode_t mode);
    int _axp202_gpio_2_select( axp_gpio_mode_t mode);
    int _axp202_gpio_3_select( axp_gpio_mode_t mode);
    int _axp202_gpio_irq_set(axp_gpio_t gpio, axp_gpio_irq_t irq);
    int _axp202_gpio_write(axp_gpio_t gpio, uint8_t val);
    int _axp202_gpio_read(axp_gpio_t gpio);


    static const uint8_t startupParams[], longPressParams[], shutdownParams[], targetVolParams[];
    static uint8_t _outputReg;
    uint8_t _address, _irq[5], _chip_id, _gpio[4];
    bool _init = false;
    axp_com_fptr_t _read_cb = nullptr;
    axp_com_fptr_t _write_cb = nullptr;
    TwoWire *_i2cPort;
    bool _isAxp173;
};
