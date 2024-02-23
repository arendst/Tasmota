#pragma once

#include <stdint.h>

#include "tock.h"

#define DRIVER_NUM_LTC294X 0x80000

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  InterruptPinDisabled = 0,
  InterruptPinChargeCompleteMode = 1,
  InterruptPinAlertMode = 2,
} interrupt_pin_conf_e;

typedef enum {
  // LTC2941
  VbatAlertOff = 0,
  VbatAlert2V8 = 1,
  VbatAlert2V9 = 2,
  VbatAlert3V0 = 3,
  // LTC29412/3
  ADCSleep = 0,
  ADCManual = 1,
  ADCScan = 2,
  ADCAuto = 3,
} vbat_alert_adc_mode_e;

typedef enum {
  LTC2941 = 1,
  LTC2942 = 2,
  LTC2943 = 3,
} ltc294x_model_e;


// Set a callback for the LTC294X driver.
//
// The callback function should look like:
//
//     void callback (int callback_type, int data, int data2, void* callback_args)
//
// callback_type is one of:
//    0: If the interrupt pin is setup in the kernel, the interrupt occurred.
//    1: Got the contents of the status register. `data` is:
//           bit 0: Undervoltage lockout (bool)
//           bit 1: VBat Alert (bool)
//           bit 2: Charge Alert Low (bool)
//           bit 3: Charge Alert High (bool)
//           bit 4: Accumulated Charge Overflow/Underflow (bool)
//        and `data2` is the Chip type:
//           1 = LTC2941
//           2 = LTC2942
//           3 = LTC2943
//     2: Got the charge value.
//     3: A write operation finished.
int ltc294x_set_callback (subscribe_upcall callback, void* callback_args);

// Get the current value of the status register. The result will be returned
// in the callback.
int ltc294x_read_status(void);

// Setup the LTC294X by configuring its !AL/CC pin, charge counting prescaler,
// and VBat alert threshold or ADC mode.
// Will trigger a `done` callback.
int ltc294x_configure(ltc294x_model_e model,
                      interrupt_pin_conf_e int_pin,
                      uint16_t prescaler,
                      vbat_alert_adc_mode_e vbat);

// Set the current accumulated charge register to 0.
// Will trigger a `done` callback.
int ltc294x_reset_charge(void);

// Configure the high charge threshold. This will be triggered when the
// accumulated charge is greater than this value.
// Will trigger a `done` callback when the register has been set.
int ltc294x_set_high_threshold(uint16_t threshold);

// Configure the low charge threshold. This will be triggered when the
// accumulated charge is lower than this value.
// Will trigger a `done` callback when the register has been set.
int ltc294x_set_low_threshold(uint16_t threshold);

// Read the current charge.
// Will be returned in the callback.
int ltc294x_get_charge(void);

// Get the current voltage. Not supported on all models.
// Will be returned in the callback.
int ltc294x_get_voltage(void);

// Get the current current reading. Not supported on all models.
// Will be returned in the callback.
int ltc294x_get_current(void);

// Put the LTC294X in a low power state.
// Will trigger a `done` callback.
int ltc294x_shutdown(void);

// Configure which LTC294X chip we are actually using.
int ltc294x_set_model(ltc294x_model_e model);


//
// Synchronous Versions
//
int ltc294x_read_status_sync(void);
int ltc294x_configure_sync(ltc294x_model_e model,
                           interrupt_pin_conf_e int_pin,
                           uint16_t prescaler,
                           vbat_alert_adc_mode_e vbat);
int ltc294x_reset_charge_sync(void);
int ltc294x_set_high_threshold_sync(uint16_t threshold);
int ltc294x_set_low_threshold_sync(uint16_t threshold);
int ltc294x_get_charge_sync(int* charge);
int ltc294x_get_voltage_sync(int* voltage);
int ltc294x_get_current_sync(int* current);
int ltc294x_shutdown_sync(void);

//
// Helpers
//
int ltc294x_convert_to_coulomb_uah(int c, int Rsense, uint16_t prescaler, ltc294x_model_e model) __attribute__((const));
int ltc294x_convert_to_voltage_mv(int v) __attribute__((const));
int ltc294x_convert_to_current_ua(int c, int Rsense) __attribute__((const));

#ifdef __cplusplus
}
#endif
