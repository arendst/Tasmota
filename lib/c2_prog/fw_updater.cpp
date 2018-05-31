#include "fw_updater.h"
#include "ihx.h"
#include "c2.h"

ssize_t rf_find_hex_record_start(uint8_t *buf, size_t size) {
  for (int i = 0; i < size; i++) {
    if (buf[i] == ':') {
      return i;
    }
  }
  return RF_RECORD_NO_START_FOUND;
}

ssize_t rf_find_hex_record_end(uint8_t *buf, size_t size) {
  for (ssize_t i = 0; i < size; i++) {
    if (buf[i] == '\n') {
      return i;
    }
  }
  return RF_RECORD_NO_END_FOUND;
}

ssize_t rf_glue_remnant_with_new_data_and_write(const uint8_t *remnant_data, uint8_t *new_data, size_t new_data_len) {
  ssize_t record_start, record_end;
  ssize_t glue_record_sz;
  uint8_t *glue_buf;
  ssize_t result;
  
  if (remnant_data[0] != ':') {
    Serial.printf("RF Remnant data did not start with a start token\n");
    return -16;
  }
  
  // Find end token in new data
  record_end = rf_find_hex_record_end(new_data, new_data_len);
  record_start = rf_find_hex_record_start(new_data, new_data_len);
  
  // Be paranoid and check that there is no start marker before the end record
  // If so this implies that there was something wrong with the last start marker saved
  // in the last upload part
  if ((record_start != RF_RECORD_NO_START_FOUND) && (record_start < record_end)) {
    Serial.println("Unexpected RF start marker found before RF end marker");
    return -16;
  }
  
  glue_record_sz = strlen((const char *) remnant_data) + record_end;
  
  glue_buf = (uint8_t *) malloc(glue_record_sz);
  if (glue_buf == NULL) {
    return -15;
  }
  
  // Assemble new glue buffer
  memcpy(glue_buf, remnant_data, strlen((const char *) remnant_data));
  memcpy(glue_buf + strlen((const char *) remnant_data), new_data, record_end);
      
  result = rf_decode_and_write(glue_buf, glue_record_sz);
  free(glue_buf);
  return result;
}

ssize_t rf_decode_and_write(uint8_t *record, size_t size) {
  uint8_t err;
  
  err = ihx_decode(record, size);
  if (err != IHX_SUCCESS) {
    Serial.printf("Hex decoding failed with error %d\n", err);
    return -13;
  } else {
    ihx_t *h = (ihx_t *) record;

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
  }
  return 0;
}

ssize_t rf_search_and_write(uint8_t *buf, size_t size) {
  // Binary contains a set of commands, decode and program each one
  ssize_t rec_end, rec_start, err;

  for (size_t i = 0; i < size; i++) {
    // Find starts and ends of commands
    rec_start = rf_find_hex_record_start(buf + i, size - i);
    if (rec_start == RF_RECORD_NO_START_FOUND) { 
      // There is nothing left to save in this buffer
      return RF_RECORD_NO_START_FOUND;
    }

    // Translate rec_start from local buffer position to chunk position
    rec_start += i;
    
    rec_end = rf_find_hex_record_end(buf + rec_start, size - rec_start);
    if (rec_end == RF_RECORD_NO_END_FOUND) {
      // We have found a start but not an end, save remnant
      return rec_start;
    }
    
    // Translate rec_end from local buffer position to chunk position
    rec_end += rec_start;
    
    err = rf_decode_and_write(buf + rec_start, rec_end - rec_start);
    if (err < 0) {
      return err;
    }
    i = rec_end;
  }
  // Buffer was perfectly aligned, start and end found without any remaining trailing characters
  return RF_RECORD_START_AND_END_FOUND;
}

uint8_t rf_erase_flash() {
  uint8_t err;
  // HACK: Try multiple times as the command sometimes fails (unclear why)
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
