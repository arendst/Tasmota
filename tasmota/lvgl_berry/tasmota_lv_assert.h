/**
 * @file tasmota_lv_assert.h
 *
 * Custom LVGL assert handler for Tasmota, included by lv_conf_internal.h
 * when LV_ASSERT_USE_CUSTOM_INCLUDE is 1 (see tasmota_lv_conf.h).
 *
 * Replaces LVGL's default `while(1);` with a call that prints heap state
 * and aborts, so a failed assert gives a decodable backtrace and a reboot
 * instead of a silent hang. Implemented in xdrv_54_lvgl.ino.
 */
#ifndef TASMOTA_LV_ASSERT_H
#define TASMOTA_LV_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

void lv_tasmota_assert_failed(void) __attribute__((noreturn));

#ifdef __cplusplus
} /*extern "C"*/
#endif

#define LV_ASSERT_HANDLER lv_tasmota_assert_failed();

#endif /*TASMOTA_LV_ASSERT_H*/
