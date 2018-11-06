#ifndef LWMQTT_HELPERS_H
#define LWMQTT_HELPERS_H

#include "lwmqtt.h"

/**
 * Reads bits from a byte.
 *
 * @param byte - The byte to read from.
 * @param pos - The position of the first bit.
 * @param num - The number of bits to read.
 * @return The read bits as a byte.
 */
uint8_t lwmqtt_read_bits(uint8_t byte, int pos, int num);

/**
 * Write bits to a byte.
 *
 * @param byte - The byte to write bits to.
 * @param value - The bits to write as a byte.
 * @param pos - The position of the first bit.
 * @param num - The number of bits to write.
 */
void lwmqtt_write_bits(uint8_t *byte, uint8_t value, int pos, int num);

/**
 * Reads arbitrary data from the specified buffer. The pointer is incremented by bytes read.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param data - Pointer to beginning of data.
 * @param len - The amount of data to read.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_read_data(uint8_t **buf, const uint8_t *buf_end, uint8_t **data, size_t len);

/**
 * Writes arbitrary data to the specified buffer. The pointer is incremented by the bytes written.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param data - Pointer to the to be written data.
 * @param len - The amount of data to write.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_write_data(uint8_t **buf, const uint8_t *buf_end, uint8_t *data, size_t len);

/**
 * Reads two byte number from the specified buffer. The pointer is incremented by two.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param num - The read number.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_read_num(uint8_t **buf, const uint8_t *buf_end, uint16_t *num);

/**
 * Writes a two byte number to the specified buffer. The pointer is incremented by two.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param num - The number to write.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_write_num(uint8_t **buf, const uint8_t *buf_end, uint16_t num);

/**
 * Reads a string from the specified buffer into the passed object. The pointer is incremented by the bytes read.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param str - The object into which the data is to be read.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_read_string(uint8_t **buf, const uint8_t *buf_end, lwmqtt_string_t *str);

/**
 * Writes a string to the specified buffer. The pointer is incremented by the bytes written.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param str - The string to write.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_write_string(uint8_t **buf, const uint8_t *buf_end, lwmqtt_string_t str);

/**
 * Reads one byte from the buffer. The pointer is incremented by one.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param byte - The read byte.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_read_byte(uint8_t **buf, const uint8_t *buf_end, uint8_t *byte);

/**
 * Writes one byte to the specified buffer. The pointer is incremented by one.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param byte - The byte to write.
 * @return LWMQTT_SUCCESS or LWMQTT_BUFFER_TOO_SHORT.
 */
lwmqtt_err_t lwmqtt_write_byte(uint8_t **buf, const uint8_t *buf_end, uint8_t byte);

/**
 * Returns the amount of bytes required by the variable number.
 *
 * @param varnum - The number to check.
 * @param len - The required length;
 * @return LWMQTT_SUCCESS or LWMQTT_VARNUM_OVERFLOW.
 */
lwmqtt_err_t lwmqtt_varnum_length(uint32_t varnum, int *len);

/**
 * Reads a variable number from the specified buffer. The pointer is incremented by the bytes read.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param varnum - The read varnum.
 * @return LWMQTT_SUCCESS, LWMQTT_BUFFER_TOO_SHORT or LWMQTT_VARNUM_OVERFLOW.
 */
lwmqtt_err_t lwmqtt_read_varnum(uint8_t **buf, const uint8_t *buf_end, uint32_t *varnum);

/**
 * Writes a variable number to the specified buffer. The pointer is incremented by the bytes written.
 *
 * @param buf - Pointer to the buffer.
 * @param buf_end - Pointer to the end of the buffer.
 * @param varnum - The number to write.
 * @return LWMQTT_SUCCESS, LWMQTT_BUFFER_TOO_SHORT or LWMQTT_VARNUM_OVERFLOW.
 */
lwmqtt_err_t lwmqtt_write_varnum(uint8_t **buf, const uint8_t *buf_end, uint32_t varnum);

#endif
