#include "lora_phy.h"

int lora_phy_set_rate(int rate) {
  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 5, rate, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_get_rate(void) {
  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 6, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_set_phase(bool phase) {
  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 7, (unsigned char)phase, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_get_phase(void) {
  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 8, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_set_polarity(bool pol) {
  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 9, (unsigned char)pol, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_get_polarity(void) {
  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 10, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LORAPHY_SPI, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int lora_phy_set_master_write_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_LORAPHY_SPI, 0, (const void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int lora_phy_set_master_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_LORAPHY_SPI, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

static void lora_phy_upcall(__attribute__ ((unused)) int   unused0,
                            __attribute__ ((unused)) int   unused1,
                            __attribute__ ((unused)) int   unused2,
                            __attribute__ ((unused)) void* ud) {
  *((bool*)ud) = true;
}

int lora_phy_write(const char* buf,
                   size_t len,
                   subscribe_upcall cb, bool* cond) {
  int ret = 0;

  ret = lora_phy_set_master_write_buffer((const uint8_t*) buf, len);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  ret = lora_phy_set_callback(cb, cond);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  syscall_return_t cval = command(DRIVER_NUM_LORAPHY_SPI, 2, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int lora_phy_read_write(const char* write,
                        char* read,
                        size_t len,
                        subscribe_upcall cb, bool* cond) {
  int ret = 0;

  ret = lora_phy_set_master_read_buffer((uint8_t*) read, len);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  return lora_phy_write(write, len, cb, cond);
}

int lora_phy_write_sync(const char* write,
                        size_t      len) {
  bool cond = false;
  int ret   = 0;

  ret = lora_phy_set_master_read_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  int err = lora_phy_write(write, len, lora_phy_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}

int lora_phy_read_write_sync(const char* write,
                             char*       read,
                             size_t      len) {
  bool cond = false;
  int ret   = 0;

  ret = lora_phy_set_master_read_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  int err = lora_phy_read_write(write, read, len, lora_phy_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}

int lora_phy_gpio_enable_output(GPIO_Pin_t pin) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 1, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_set(GPIO_Pin_t pin) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 2, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_clear(GPIO_Pin_t pin) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 3, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_toggle(GPIO_Pin_t pin) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 4, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_enable_input(GPIO_Pin_t pin, GPIO_InputMode_t pin_config) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 5, pin, pin_config);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_read(GPIO_Pin_t pin, int* pin_value) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 6, pin, 0);
  return tock_command_return_u32_to_returncode(rval, (uint32_t*) pin_value);
}

int lora_phy_gpio_enable_interrupt(GPIO_Pin_t pin, GPIO_InterruptMode_t irq_config) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 7, pin, irq_config);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_disable_interrupt(GPIO_Pin_t pin) {
  syscall_return_t rval = command(DRIVER_NUM_LORAPHY_GPIO, 8, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int lora_phy_gpio_interrupt_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LORAPHY_GPIO, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}
