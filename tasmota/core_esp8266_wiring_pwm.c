/*
  pwm.c - analogWrite implementation for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef ESP8266

// Use PWM from core 2.4.0 as all versions below 2.5.0-beta3 produce LED flickering when settings are saved to flash
#include <core_version.h>
#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
#warning **** Tasmota is using v2.4.0 wiring_pwm.c as planned ****

#include "wiring_private.h"
#include "pins_arduino.h"
#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"

#ifndef F_CPU
#define F_CPU 800000000L
#endif

struct pwm_isr_table {
    uint8_t len;
    uint16_t steps[17];
    uint32_t masks[17];
};

struct pwm_isr_data {
    struct pwm_isr_table tables[2];
    uint8_t active;//0 or 1, which table is active in ISR
};

static struct pwm_isr_data _pwm_isr_data;

uint32_t pwm_mask = 0;
uint16_t pwm_values[17] = {0,};
uint32_t pwm_freq = 1000;
uint32_t pwm_range = PWMRANGE;

uint8_t pwm_steps_changed = 0;
uint32_t pwm_multiplier = 0;

int pwm_sort_array(uint16_t a[], uint16_t al)
{
    uint16_t i, j;
    for (i = 1; i < al; i++) {
        uint16_t tmp = a[i];
        for (j = i; j >= 1 && tmp < a[j-1]; j--) {
            a[j] = a[j-1];
        }
        a[j] = tmp;
    }
    int bl = 1;
    for(i = 1; i < al; i++) {
        if(a[i] != a[i-1]) {
            a[bl++] = a[i];
        }
    }
    return bl;
}

uint32_t pwm_get_mask(uint16_t value)
{
    uint32_t mask = 0;
    int i;
    for(i=0; i<17; i++) {
        if((pwm_mask & (1 << i)) != 0 && pwm_values[i] == value) {
            mask |= (1 << i);
        }
    }
    return mask;
}

void prep_pwm_steps(void)
{
    if(pwm_mask == 0) {
        return;
    }

    int pwm_temp_steps_len = 0;
    uint16_t pwm_temp_steps[17];
    uint32_t pwm_temp_masks[17];
    uint32_t range = pwm_range;

    if((F_CPU / ESP8266_CLOCK) == 1) {
        range /= 2;
    }

    int i;
    for(i=0; i<17; i++) {
        if((pwm_mask & (1 << i)) != 0 && pwm_values[i] != 0) {
            pwm_temp_steps[pwm_temp_steps_len++] = pwm_values[i];
        }
    }
    pwm_temp_steps[pwm_temp_steps_len++] = range;
    pwm_temp_steps_len = pwm_sort_array(pwm_temp_steps, pwm_temp_steps_len) - 1;
    for(i=0; i<pwm_temp_steps_len; i++) {
        pwm_temp_masks[i] = pwm_get_mask(pwm_temp_steps[i]);
    }
    for(i=pwm_temp_steps_len; i>0; i--) {
        pwm_temp_steps[i] = pwm_temp_steps[i] - pwm_temp_steps[i-1];
    }

    pwm_steps_changed = 0;
    struct pwm_isr_table *table = &(_pwm_isr_data.tables[!_pwm_isr_data.active]);
    table->len = pwm_temp_steps_len;
    ets_memcpy(table->steps, pwm_temp_steps, (pwm_temp_steps_len + 1) * 2);
    ets_memcpy(table->masks, pwm_temp_masks, pwm_temp_steps_len * 4);
    pwm_multiplier = ESP8266_CLOCK/(range * pwm_freq);
    pwm_steps_changed = 1;
}

void ICACHE_RAM_ATTR pwm_timer_isr(void) //103-138
{
    struct pwm_isr_table *table = &(_pwm_isr_data.tables[_pwm_isr_data.active]);
    static uint8_t current_step = 0;
    TEIE &= ~TEIE1;//14
    T1I = 0;//9
    if(current_step < table->len) { //20/21
        uint32_t mask = table->masks[current_step] & pwm_mask;
        if(mask & 0xFFFF) {
            GPOC = mask & 0xFFFF;    //15/21
        }
        if(mask & 0x10000) {
            GP16O = 0;    //6/13
        }
        current_step++;//1
    } else {
        current_step = 0;//1
        if(pwm_mask == 0) { //12
            table->len = 0;
            return;
        }
        if(pwm_mask & 0xFFFF) {
            GPOS = pwm_mask & 0xFFFF;    //11
        }
        if(pwm_mask & 0x10000) {
            GP16O = 1;    //5/13
        }
        if(pwm_steps_changed) { //12/21
            _pwm_isr_data.active = !_pwm_isr_data.active;
            table = &(_pwm_isr_data.tables[_pwm_isr_data.active]);
            pwm_steps_changed = 0;
        }
    }
    T1L = (table->steps[current_step] * pwm_multiplier);//23
    TEIE |= TEIE1;//13
}

void pwm_start_timer(void)
{
    timer1_disable();
    ETS_FRC_TIMER1_INTR_ATTACH(NULL, NULL);
    ETS_FRC_TIMER1_NMI_INTR_ATTACH(pwm_timer_isr);
    timer1_enable(TIM_DIV1, TIM_EDGE, TIM_SINGLE);
    timer1_write(1);
}

void ICACHE_RAM_ATTR pwm_stop_pin(uint8_t pin)
{
    if(pwm_mask){
        pwm_mask &= ~(1 << pin);
        if(pwm_mask == 0) {
            ETS_FRC_TIMER1_NMI_INTR_ATTACH(NULL);
            timer1_disable();
            timer1_isr_init();
        }
    }
}

extern void __analogWrite(uint8_t pin, int value)
{
    bool start_timer = false;
    if(value == 0) {
        digitalWrite(pin, LOW);
        prep_pwm_steps();
        return;
    }
    if(value == pwm_range) {
        digitalWrite(pin, HIGH);
        prep_pwm_steps();
        return;
    }
    if((pwm_mask & (1 << pin)) == 0) {
        if(pwm_mask == 0) {
            memset(&_pwm_isr_data, 0, sizeof(_pwm_isr_data));
            start_timer = true;
        }
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        pwm_mask |= (1 << pin);
    }
    if((F_CPU / ESP8266_CLOCK) == 1) {
        value = (value+1) / 2;
    }
    pwm_values[pin] = value % (pwm_range + 1);
    prep_pwm_steps();
    if(start_timer) {
        pwm_start_timer();
    }
}

extern void __analogWriteFreq(uint32_t freq)
{
    pwm_freq = freq;
    prep_pwm_steps();
}

extern void __analogWriteRange(uint32_t range)
{
    pwm_range = range;
    prep_pwm_steps();
}

extern void analogWrite(uint8_t pin, int val) __attribute__ ((weak, alias("__analogWrite")));
extern void analogWriteFreq(uint32_t freq) __attribute__ ((weak, alias("__analogWriteFreq")));
extern void analogWriteRange(uint32_t range) __attribute__ ((weak, alias("__analogWriteRange")));

#endif  // ARDUINO_ESP8266_RELEASE

#endif  // ESP8266
