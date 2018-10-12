#include <string.h>

#include "helpers.h"

uint8_t lwmqtt_read_bits(uint8_t byte, int pos, int num) { return (byte & (uint8_t)((~(0xFF << num)) << pos)) >> pos; }

void lwmqtt_write_bits(uint8_t *byte, uint8_t value, int pos, int num) {
  *byte = (*byte & ~(uint8_t)((~(0xFF << num)) << pos)) | (value << pos);
}

lwmqtt_err_t lwmqtt_read_data(uint8_t **buf, const uint8_t *buf_end, uint8_t **data, size_t len) {
  // check zero length
  if (len == 0) {
    *data = NULL;
    return LWMQTT_SUCCESS;
  }

  // check buffer size
  if ((size_t)(buf_end - (*buf)) < len) {
    return LWMQTT_BUFFER_TOO_SHORT;
  }

  // read data
  *data = *buf;

  // advance pointer
  *buf += len;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_write_data(uint8_t **buf, const uint8_t *buf_end, uint8_t *data, size_t len) {
  // check zero length
  if (len == 0) {
    return LWMQTT_SUCCESS;
  }

  // check buffer size
  if ((size_t)(buf_end - (*buf)) < len) {
    return LWMQTT_BUFFER_TOO_SHORT;
  }

  // write data
  memcpy(*buf, data, len);

  // advance pointer
  *buf += len;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_read_num(uint8_t **buf, const uint8_t *buf_end, uint16_t *num) {
  // check buffer size
  if ((size_t)(buf_end - (*buf)) < 2) {
    *num = 0;
    return LWMQTT_BUFFER_TOO_SHORT;
  }

  // read two byte integer
  *num = (uint16_t)256 * (*buf)[0] + (*buf)[1];

  // adjust pointer
  *buf += 2;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_write_num(uint8_t **buf, const uint8_t *buf_end, uint16_t num) {
  // check buffer size
  if ((size_t)(buf_end - (*buf)) < 2) {
    return LWMQTT_BUFFER_TOO_SHORT;
  }

  // write bytes
  (*buf)[0] = (uint8_t)(num / 256);
  (*buf)[1] = (uint8_t)(num % 256);

  // adjust pointer
  *buf += 2;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_read_string(uint8_t **buf, const uint8_t *buf_end, lwmqtt_string_t *str) {
  // read length
  uint16_t len;
  lwmqtt_err_t err = lwmqtt_read_num(buf, buf_end, &len);
  if (err != LWMQTT_SUCCESS) {
    return err;
  }

  // read data
  err = lwmqtt_read_data(buf, buf_end, (uint8_t **)&str->data, len);
  if (err != LWMQTT_SUCCESS) {
    return err;
  }

  // set length
  str->len = len;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_write_string(uint8_t **buf, const uint8_t *buf_end, lwmqtt_string_t str) {
  // write string length
  lwmqtt_err_t err = lwmqtt_write_num(buf, buf_end, str.len);
  if (err != LWMQTT_SUCCESS) {
    return err;
  }

  // write data
  err = lwmqtt_write_data(buf, buf_end, (uint8_t *)str.data, str.len);
  if (err != LWMQTT_SUCCESS) {
    return err;
  }

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_read_byte(uint8_t **buf, const uint8_t *buf_end, uint8_t *byte) {
  // check buffer size
  if ((size_t)(buf_end - (*buf)) < 1) {
    *byte = 0;
    return LWMQTT_BUFFER_TOO_SHORT;
  }

  // read byte
  *byte = (*buf)[0];

  // adjust pointer
  *buf += 1;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_write_byte(uint8_t **buf, const uint8_t *buf_end, uint8_t byte) {
  // check buffer size
  if ((size_t)(buf_end - (*buf)) < 1) {
    return LWMQTT_BUFFER_TOO_SHORT;
  }

  // write byte
  (*buf)[0] = byte;

  // adjust pointer
  *buf += 1;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_varnum_length(uint32_t varnum, int *len) {
  if (varnum < 128) {
    *len = 1;
    return LWMQTT_SUCCESS;
  } else if (varnum < 16384) {
    *len = 2;
    return LWMQTT_SUCCESS;
  } else if (varnum < 2097151) {
    *len = 3;
    return LWMQTT_SUCCESS;
  } else if (varnum < 268435455) {
    *len = 4;
    return LWMQTT_SUCCESS;
  } else {
    *len = 0;
    return LWMQTT_VARNUM_OVERFLOW;
  }
}

lwmqtt_err_t lwmqtt_read_varnum(uint8_t **buf, const uint8_t *buf_end, uint32_t *varnum) {
  // prepare last byte
  uint8_t byte;

  // prepare multiplier
  uint32_t multiplier = 1;

  // prepare length
  size_t len = 0;

  // initialize number
  *varnum = 0;

  // decode variadic number
  do {
    // increment length
    len++;

    // return error if buffer is to small
    if ((size_t)(buf_end - (*buf)) < len) {
      return LWMQTT_BUFFER_TOO_SHORT;
    }

    // return error if the length has overflowed
    if (len > 4) {
      return LWMQTT_VARNUM_OVERFLOW;
    }

    // read byte
    byte = (*buf)[len - 1];

    // add byte to number
    *varnum += (byte & 127) * multiplier;

    // increase multiplier
    multiplier *= 128;
  } while ((byte & 128) != 0);

  // adjust pointer
  *buf += len;

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_write_varnum(uint8_t **buf, const uint8_t *buf_end, uint32_t varnum) {
  // init len counter
  size_t len = 0;

  // encode variadic number
  do {
    // check overflow
    if (len == 4) {
      return LWMQTT_VARNUM_OVERFLOW;
    }

    // return error if buffer is to small
    if ((size_t)(buf_end - (*buf)) < len + 1) {
      return LWMQTT_BUFFER_TOO_SHORT;
    }

    // calculate current byte
    uint8_t byte = (uint8_t)(varnum % 128);

    // change remaining length
    varnum /= 128;

    // set the top bit of this byte if there are more to encode
    if (varnum > 0) {
      byte |= 0x80;
    }

    // write byte
    (*buf)[len++] = byte;
  } while (varnum > 0);

  // adjust pointer
  *buf += len;

  return LWMQTT_SUCCESS;
}
