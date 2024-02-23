#include "aes.h"

#define DRIVER_NUM_AES 0x40006

#define TOCK_AES_CB           0

#define TOCK_AES_KEY_BUF      0
#define TOCK_AES_IV_BUF       1
#define TOCK_AES_SOURCE_BUF   2
#define TOCK_AES_DEST_BUF     0

#define TOCK_AES_CHECK_PRESENT   0
#define TOCK_AES_SET_ALG         1
#define TOCK_AES_SETUP           2
#define TOCK_AES_CRYPT           3
#define TOCK_AES_FINISH          4

#define TOCK_AES_CCM_AOFF        5
#define TOCK_AES_CCM_MOFF        6
#define TOCK_AES_CCM_MIC_LEN     7
#define TOCK_AES_CCM_CONF        8

int aes_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_AES, TOCK_AES_CB, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int aes_set_key_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_AES, TOCK_AES_KEY_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

/*
 * Contains the IV buffer, if applicable.
 * If doing a AES CCM operation this contains the nonce instead.
 */
int aes_set_iv_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_AES, TOCK_AES_IV_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int aes_set_nonce_buffer(const uint8_t* buffer, uint32_t len) {
  return aes_set_iv_buffer(buffer, len);
}

int aes_set_source_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_AES, TOCK_AES_SOURCE_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int aes_set_dest_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_AES, TOCK_AES_DEST_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int aes_check_status(void) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_CHECK_PRESENT, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_set_algorithm(uint8_t operation, bool encrypting) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_SET_ALG, operation, encrypting);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_setup(void) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_SETUP, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_crypt(void) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_CRYPT, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_finish(void) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_FINISH, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_ccm_set_a_off(uint32_t value) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_CCM_AOFF, value, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_ccm_set_m_off(uint32_t value) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_CCM_MOFF, value, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_ccm_set_mic_len(uint32_t value) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_CCM_MIC_LEN, value, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int aes_ccm_set_confidential(bool value) {
  syscall_return_t cval = command(DRIVER_NUM_AES, TOCK_AES_CCM_CONF, value, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
