#include "ltc294x.h"
#include "tock.h"


struct ltc294x_data {
  int charge;
  bool fired;
};

static struct ltc294x_data result = { .fired = false, .charge = 0 };

// Internal callback for faking synchronous reads
static void ltc294x_upcall(__attribute__ ((unused)) int callback_type,
                           __attribute__ ((unused)) int value,
                           __attribute__ ((unused)) int chip,
                           void*                        ud) {
  struct ltc294x_data* data = (struct ltc294x_data*) ud;
  data->charge = value;
  data->fired  = true;
}

int ltc294x_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LTC294X, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int ltc294x_read_status(void) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_configure(ltc294x_model_e       model,
                      interrupt_pin_conf_e  int_pin,
                      uint16_t              prescaler,
                      vbat_alert_adc_mode_e vbat) {
  uint8_t M = 0;
  if (model == LTC2941 || model == LTC2942) {
    // ltc2941/2 expects log_2 of prescaler value
    for (int i = 0; i < 8; i++) {
      if ((1 << i) & prescaler) {
        M = i;
      }
    }
  } else if (model == LTC2943) {
    // See Table 3 in the datasheet.
    switch (prescaler) {
      case 1:    M = 0;
        break;
      case 4:    M = 1;
        break;
      case 16:   M = 2;
        break;
      case 64:   M = 3;
        break;
      case 256:  M = 4;
        break;
      case 1024: M = 5;
        break;
      case 4096: M = 7;
        break;
      default:   M = 4;
        break;
    }
  }

  syscall_return_t com = command(DRIVER_NUM_LTC294X, 10, model, 0);
  int ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return ret;

  uint8_t cmd = (int_pin & 0x03) | ((M & 0x07) << 2) | ((vbat & 0x03) << 5);
  com = command(DRIVER_NUM_LTC294X, 2, cmd, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_reset_charge(void) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_set_high_threshold(uint16_t threshold) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 4, threshold, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_set_low_threshold(uint16_t threshold) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 5, threshold, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_get_charge(void) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 6, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_get_voltage(void) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 8, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_get_current(void) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 9, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_shutdown(void) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 7, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_set_model(ltc294x_model_e model) {
  syscall_return_t com = command(DRIVER_NUM_LTC294X, 10, model, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ltc294x_read_status_sync(void) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_read_status();
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int ltc294x_configure_sync(ltc294x_model_e       model,
                           interrupt_pin_conf_e  int_pin,
                           uint16_t              prescaler,
                           vbat_alert_adc_mode_e vbat) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_configure(model, int_pin, prescaler, vbat);
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int ltc294x_reset_charge_sync(void) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_reset_charge();
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int ltc294x_set_high_threshold_sync(uint16_t threshold) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_set_high_threshold(threshold);
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int ltc294x_set_low_threshold_sync(uint16_t threshold) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_set_low_threshold(threshold);
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int ltc294x_get_charge_sync(int* charge) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_get_charge();
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  *charge = result.charge;

  return RETURNCODE_SUCCESS;
}

int ltc294x_get_voltage_sync(int* voltage) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_get_voltage();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *voltage = result.charge;

  return RETURNCODE_SUCCESS;
}

int ltc294x_get_current_sync(int* current) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_get_current();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *current = result.charge;

  return RETURNCODE_SUCCESS;
}

int ltc294x_shutdown_sync(void) {
  int err;
  result.fired = false;

  err = ltc294x_set_callback(ltc294x_upcall, (void*) &result);
  if (err < 0) return err;

  err = ltc294x_shutdown();
  if (err < 0) return err;

  // Wait for the ADC callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int ltc294x_convert_to_coulomb_uah(int c, int Rsense, uint16_t prescaler, ltc294x_model_e model) {
  if (model == LTC2941 || model == LTC2942) {
    return (int)(c * 85 * (50.0 / Rsense) * (prescaler / 128.0));
  } else {
    return (int)(c * 340 * (50.0 / Rsense) * (prescaler / 4096.0));
  }
}

int ltc294x_convert_to_voltage_mv (int v) {
  return 23.6 * (v / (float)0xFFFF) * 1000;
}

int ltc294x_convert_to_current_ua (int c, int Rsense) {
  return (int)((60.0 / Rsense) * ((c - 0x7FFF) / (float)0x7FFF) * 1000000.0);
}
