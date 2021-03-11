/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef ESP_MFI_I2C_H_
#define ESP_MFI_I2C_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize I2C
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_i2c_init(void);

/**
 * @brief I2C operation end
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_i2c_end(void);

/**
 * @brief Write data to I2C slave
 *
 * @param slvaddr  address of slave
 * @param regaddr  address of register
 * @param buff     pointer of data to write
 * @param len      the data length
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_i2c_write(uint8_t slvaddr, uint8_t regaddr, uint8_t *buff, uint32_t len);

/**
 * @brief Read data from I2C slave
 *
 * @param slvaddr  address of slave
 * @param regaddr  address of register
 * @param buff     pointer of data read
 * @param len      the data length
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_i2c_read(uint8_t slvaddr, uint8_t regaddr, uint8_t *buff, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* ESP_MFI_I2C_H_ */
