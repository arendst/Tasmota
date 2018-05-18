#include "firmware.h"
#include "ihx.h"
#include "c2.h"

ssize_t search_and_flash(uint8_t *buf, size_t size) {
  // Binary contains a set of commands, decode and program each one
  uint16_t cmd_start = 0;
  uint8_t err;

  // Skip first byte, we already know if is ':' of the first command
  for (int i = 0; i < size; i++) {
    // Find start of command
    if (buf[i] == ':') {
      cmd_start = i;
    } else if (buf[i] == '\n') {
      err = ihx_decode(buf + cmd_start, i - cmd_start);
      if (err != IHX_SUCCESS) {
        Serial.printf("Hex decoding failed with error %d\n", err);
        return -13;
      } else {
        ihx_t *h = (ihx_t *) (buf + cmd_start);

        if (h->record_type == IHX_RT_DATA) {
          int retries = 5;
          uint16_t address = h->address_high * 0x100 + h->address_low;

          do {
            err = c2_programming_init();
            err = c2_block_write(address, h->data, h->len);
          } while (err != C2_SUCCESS && retries--);
        } else if (h->record_type == IHX_RT_END_OF_FILE) {
          Serial.println("RF firmware upgrade done, restarting RF chip");
          err = c2_reset();
        }

        if (err != C2_SUCCESS) {
          return -12;
        }
        cmd_start = i;
      }
    }
  }
  return cmd_start;
}


