#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int aes_set_callback(subscribe_upcall callback, void* callback_args);

int aes_set_key_buffer(const uint8_t* buffer, uint32_t len);
/*
 * Contains the IV buffer, if applicable.
 */
int aes_set_iv_buffer(const uint8_t* buffer, uint32_t len);

/*
 * Contains the nonce for CCM buffer.
 */
int aes_set_nonce_buffer(const uint8_t* buffer, uint32_t len);

/*
 * Contains the source buffer.
 * If doing a AES CCM operation this sets the mlen value to `len`.
 */
int aes_set_source_buffer(const uint8_t* buffer, uint32_t len);
int aes_set_dest_buffer(uint8_t* buffer, uint32_t len);

/*
 * Check that the AES system exists
 */
int aes_check_status(void);

/*
 * Set the AES algorithm
 * operation:
 *    * 0 -> AES128Ctr
 *    * 1 -> AES128CBC
 *    * 2 -> AES128ECB
 *    * 3 -> AES128CCM
 * encrypting:
 *    * true -> Encrypt the source data
 *    * false -> Decrypt the source data
 */
int aes_set_algorithm(uint8_t operation, bool encrypting);

/*
 * Setup the platform and run the first encryption.
 * This must be called before `aes_crypt()` and `aes_finish()`
 * but must be called after `aes_set_algorithm()` and all relevent
 * buffers are set up.
 *
 * This will load in the specified key, IV and then run the
 * encryption/decryption operation on the data in the source
 * buffer and save the output in the destination buffer.
 *
 * This will trigger a callback.
 *
 */
int aes_setup(void);

/*
 * This must be called after aes_setup(). It will run the
 * encryption/decryption operation on the source data and store
 * it in the destination buffer, based on thte key and IV that was
 * loaded by `aes_setup()`.
 *
 * Between a call of `aes_setup()` and `aes_finish()` this can be called
 * any number of times (from 0 to infinite).
 *
 * Each call to this will trigger a callback
 */
int aes_crypt(void);

/*
 * This must be called when the operation is complete. This indicates that the
 * app has finished the operation. Any future operations need to call
 * `aes_set_algorithm()` and `aes_setup()` as the key and IV will be cleared.
 */
int aes_finish(void);

/*
 * This sets the A offset for AES CCM.
 */
int aes_ccm_set_a_off(uint32_t value);

/*
 * This sets the M offset for AES CCM.
 */
int aes_ccm_set_m_off(uint32_t value);

/*
 * This sets the mic length for AES CCM.
 */
int aes_ccm_set_mic_len(uint32_t value);

/*
 * This sets the confidential bool for AES CCM.
 */
int aes_ccm_set_confidential(bool value);

#ifdef __cplusplus
}
#endif
