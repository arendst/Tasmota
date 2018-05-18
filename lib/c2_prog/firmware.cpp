#include "firmware.h"
#include "ihx.h"
#include "c2.h"

ssize_t rf_search_and_write(uint8_t *buf, size_t size) {
  // Binary contains a set of commands, decode and program each one
  uint16_t cmd_start = 0;
  uint8_t err;

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

uint8_t rf_erase_flash() {
  uint8_t err;
  // FIXME: Try multiple times as it sometimes fails (unclear why)
  for (int i = 0; i < 4; i++) {
    err = c2_programming_init();
    if (err != C2_SUCCESS) {
      return 10;
    }

    err = c2_device_erase();
    if (err != C2_SUCCESS) {
      Serial.printf("RF erase result: %s\n", c2_print_status_by_name(err));
      if (i < 3) {
        // Reset RF chip and try again
        c2_reset();
      } else {
        return 11;
      }
    } else {
      break;
    }
  }
  return 0;
}
