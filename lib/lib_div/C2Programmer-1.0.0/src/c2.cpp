#include "c2.h"

/////////////////////////////////////////////
// Nothing should need change on this file //
/////////////////////////////////////////////

// Times in microseconds
#define T_RD (20+5)
#define T_SD ( 2+5)

// Layer 0: Bit shifter
static bool c2_bit(bool b) {
  C2D(b);
  //  C2_DELAY_US(1);
  C2CK(0);
  //  C2_DELAY_US(1);
  b = C2D();
  C2CK(1);
  return b;
}

// Layer 1: C2D Register read/write
void c2_address_write(uint8_t address) {
#ifdef C2_DEBUG
  Serial.print("AW");
  Serial.println(address, HEX);
#endif
  // start
  c2_bit(true);
  C2D_enable(true);

  // instruction
  c2_bit(1);
  c2_bit(1);

  // Address
  for (int i = 0; i < 8; ++i) {
    c2_bit(address & 1);
    address >>= 1;
  }

  // Stop
  C2D_enable(false);
  c2_bit(1);
}

uint8_t c2_address_read() {
  // start
  c2_bit(true);
  C2D_enable(true);

  // instruction
  c2_bit(0);
  c2_bit(1);

  // Change C2D direction
  C2D_enable(false);
  c2_bit(0);

  // Address
  uint8_t a = 0, m = 1;
  for (int i = 0; i < 8; ++i) {
    if (c2_bit(a & 1)) {
      a |= m;
    }
    m <<= 1;
  }

  // Stop is implied
#ifdef C2_DEBUG
  Serial.print("AR");
  Serial.println(a, HEX);
#endif
  return a;
}

uint8_t c2_data_write(uint32_t d, uint8_t bytes) {
#ifdef C2_DEBUG
  Serial.print("DW");
  Serial.println(d, HEX);
#endif

  // start
  c2_bit(true);
  C2D_enable(true);

  // instruction
  c2_bit(1);
  c2_bit(0);

  // Length
  bytes--;
  c2_bit(bytes & 1);
  c2_bit(bytes & 2);
  bytes++;

  // Data
  for (int i = 0; i < 8 * bytes; ++i) {
    c2_bit(d & 1);
    d >>= 1;
  }

  // Reverse C2D direction
  C2D_enable(false);
  c2_bit(0);

  // Wait
  uint8_t to = 128;
  while (!c2_bit(0)) if (!--to) return C2_SHIFT_TIMEOUT;

  // Stop
  //c2_bit(0); implied

  return C2_SUCCESS;
}

uint8_t c2_data_read(uint32_t &d, uint8_t bytes) {
  // start
  c2_bit(true);
  C2D_enable(true);

  // instruction
  c2_bit(0);
  c2_bit(0);

  // Length
  bytes--;
  c2_bit(bytes & 1);
  c2_bit(bytes & 2);
  bytes++;

  // Reverse C2D direction
  C2D_enable(false);
  c2_bit(0);

  // Wait
  uint8_t to = 128;
  while (!c2_bit(0)) if (!--to) return C2_SHIFT_TIMEOUT;

  // Data
  d = 0;
  uint32_t m = 1;
  for (int i = 0; i < 8 * bytes; ++i) {
    if (c2_bit(d & 1)) {
      d |= m;
    }
    m <<= 1;
  }

  // Stop is implied
#ifdef C2D_DEBUG
  Serial.print("DR");
  Serial.println(d, HEX);
#endif
  return C2_SUCCESS;
}

// Layer 2: Operations
#define C2_POLL_INBUSY() {                   \
    uint16_t to = 1000;                      \
    uint8_t a;                               \
    while (1) {                              \
      a = c2_address_read();                 \
      if (a == 0xFF) return C2_BROKEN_LINK;  \
      if (~a & C2_INBUSY) break;             \
      if (--to == 0) return C2_POLL_TIMEOUT; \
      C2_DELAY_MS(1);                        \
    };                                       \
  }

#define C2_POLL_OUTREADY() {                 \
    uint16_t to = 10000;                     \
    uint8_t a;                               \
    while (1) {                              \
      a = c2_address_read();                 \
      if (a == 0xFF) return C2_BROKEN_LINK;  \
      if (a & C2_OUTREADY) break;            \
      if (--to == 0) return C2_POLL_TIMEOUT; \
      C2_DELAY_MS(1);                        \
    };                                       \
  }

#define C2_DATA_WRITE_AND_CHECK(v, s) { \
    uint8_t r = c2_data_write(v, s);    \
    if (r != C2_SUCCESS) return r;      \
  }

#define C2_DATA_READ_AND_CHECK(v, s) { \
    uint8_t r = c2_data_read(v, s);    \
    if (r != C2_SUCCESS) return r;     \
  }

#define C2_EXPECT_DATA(value) {            \
    uint8_t d;                             \
    C2_DATA_READ_AND_CHECK(d, 1);          \
    if (d != (value)) return C2_CMD_ERROR; \
  }

uint8_t c2_reset() {
  C2CK(false);
  C2_DELAY_US(T_RD);
  C2CK(true);
  C2_DELAY_US(T_SD);
  return C2_SUCCESS;
}

uint8_t c2_programming_init(uint8_t devid) {
  c2_reset();
  c2_address_write(C2FPCTL);
  C2_DATA_WRITE_AND_CHECK(C2FPCTL_ENABLE0,   1);
  C2_DATA_WRITE_AND_CHECK(C2FPCTL_CORE_HALT, 1);
  C2_DATA_WRITE_AND_CHECK(C2FPCTL_ENABLE1,   1)
  C2_DELAY_MS(21);

  // device specific initialization, see https://www.silabs.com/documents/public/application-notes/AN127.pdf
  switch (devid) {
  case C2_DEVID_UNKNOWN:
    break;
  case C2_DEVID_EFM8BB1:
  case C2_DEVID_EFM8BB2:
  case C2_DEVID_EFM8BB3:	// C2_DEVID_EFM8LB1 is the same
    c2_address_write(0xFF);
    C2_DATA_WRITE_AND_CHECK(0x80, 1);
    C2_DELAY_US(5);
    c2_address_write(0xEF);
    C2_DATA_WRITE_AND_CHECK(0x02, 1);
    c2_address_write(0xA9);
    C2_DATA_WRITE_AND_CHECK(0x00, 1);
    break;
  default:
    return C2_BROKEN_LINK;
  }

  return C2_SUCCESS;
}

uint8_t c2_block_write(uint32_t address, uint8_t *data, uint8_t len) {
  // 1. Perform an Address Write with a value of FPDAT
  c2_address_write(C2FPDAT);

  // 2. Perform a Data Write with the Block Write command.
  C2_DATA_WRITE_AND_CHECK(C2FPDAT_BLOCK_WRITE, 1);

  // 3. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 4. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 5. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  // 6. Perform a Data Write with the high byte of the address.
  C2_DATA_WRITE_AND_CHECK(address >> 8,  1);

  // 7. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 8. Perform a Data Write with the low byte of the address.
  C2_DATA_WRITE_AND_CHECK(address & 255, 1);

  // 9. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 10. Perform a Data Write with the length.
  C2_DATA_WRITE_AND_CHECK(len, 1);

  // 12a. Repeat steps 11 and 12 for each byte specified by the length field.
  uint8_t i = 0;
  do {
    // 11. Poll on InBusy using Address Read until the bit clears.
    C2_POLL_INBUSY();

    // 12. Perform a Data Write with the data. This will write the data to the flash.
    C2_DATA_WRITE_AND_CHECK(data[i], 1);
  } while (++i != len);

  // 13. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 14. Perform a Data Read instruction. A value of 0x0D is okay. write to an EPROM block:
  C2_EXPECT_DATA(0x0D);

  return C2_SUCCESS;
}

uint8_t c2_eeprom_write(uint32_t address, uint8_t *data, uint8_t len) {
  // 1. Write 0x04 to the FPCTL register.
  c2_address_write(C2FPCTL);
  C2_DATA_WRITE_AND_CHECK(0x04, 1);

  // 2. Write 0x40 to EPCTL.
  c2_address_write(C2EPCTL);
  C2_DATA_WRITE_AND_CHECK(0x40, 1);

  // 3. Write 0x58 to EPCTL.
  C2_DATA_WRITE_AND_CHECK(0x58, 1);

  // 4. Write the high byte of the address to EPADDRH.
  c2_address_write(C2EPADDRH);
  C2_DATA_WRITE_AND_CHECK(address >> 8, 1);

  // 5. Write the low byte of the address to address EPADDRL.
  c2_address_write(C2EPADDRL);
  C2_DATA_WRITE_AND_CHECK(address, 1);

  // 6. Perform an Address Write with a value of EPDAT.
  c2_address_write(C2EPDAT);

  // 7. Turn on VPP.

  // 8. Wait for the VPP settling time.

  // 10a. Repeat steps 9 and 10 until all bytes are written.
  uint8_t i = 0;
  do {
    // 9. Write the data to the device using a Data Write.
    C2_DATA_WRITE_AND_CHECK(data[i], 1);

    // 10. Perform Address Read instructions until the value returned is not 0x80 and the EPROM is no longer busy.
    C2_POLL_INBUSY();
  } while (++i != len);

  // 12. Turn off VPP. Note that VPP can only be applied for a maximum lifetime amount, and this value is specified in the device data sheet.

  // 13. Write 0x40 to EPCTL.
  c2_address_write(C2EPCTL);
  C2_DATA_WRITE_AND_CHECK(0x40, 1);

  // 14. Write 0x00 to EPCTL.
  C2_DATA_WRITE_AND_CHECK(0x00, 1);

  // 15. Write 0x02 to FPCTL.
  c2_address_write(C2FPCTL);
  C2_DATA_WRITE_AND_CHECK(0x02, 1);

  // 16. Write 0x04 to FPCTL.
  C2_DATA_WRITE_AND_CHECK(0x04, 1);

  // 17. Write 0x01 to FPCTL.
  C2_DATA_WRITE_AND_CHECK(0x01, 1);

  return C2_SUCCESS;
}

uint8_t c2_block_read(uint32_t address, uint8_t *data, uint8_t len) {
  // 1. Perform an Address Write with a value of FPDAT.
  c2_address_write(C2FPDAT);

  // 2. Perform a Data Write with the Block Read command.
  C2_DATA_WRITE_AND_CHECK(C2FPDAT_BLOCK_READ, 1);

  // 3. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 4. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 5. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  // 6. Perform a Data Write with the high byte of the address.
  C2_DATA_WRITE_AND_CHECK(address >> 8, 1);

  // 7. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 8. Perform a Data Write with the low byte of the address.
  C2_DATA_WRITE_AND_CHECK(address, 1);

  // 9. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 10. Perform a Data Write with the length.
  C2_DATA_WRITE_AND_CHECK(len, 1);

  // 11. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 12. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 13. Read FPI Command Status. Abort if Status != 0x0D.
  C2_EXPECT_DATA(0x0D);

  // 15a. Repeat step 14 and 15 for each byte specified by the length field.
  uint8_t i = 0;
  do {
    // 14. Poll on OutReady using Address Read until the bit set.
    C2_POLL_OUTREADY();

    // 15. Perform a Data Read instruction. This will read the data from the flash.
    C2_DATA_READ_AND_CHECK(data[i], 1);
  } while (++i != len);

  return C2_SUCCESS;
}

uint8_t c2_eeprom_read(uint32_t address, uint8_t *data, uint8_t len) {
  // 1. Write 0x04 to the FPCTL register.
  c2_address_write(C2FPCTL);
  C2_DATA_WRITE_AND_CHECK(0x04, 1);

  // 2. Write 0x00 to EPCTL.
  c2_address_write(C2EPCTL);
  C2_DATA_WRITE_AND_CHECK(0x00, 1);

  // 3. Write 0x58 to EPCTL.
  C2_DATA_WRITE_AND_CHECK(0x58, 1);

  // 4. Write the high byte of the address to EPADDRH.
  c2_address_write(C2EPADDRH);
  C2_DATA_WRITE_AND_CHECK(address >> 8, 1);

  // 5. Write the low byte of the address to address EPADDRL.
  c2_address_write(C2EPADDRL);
  C2_DATA_WRITE_AND_CHECK(address, 1);

  // 6. Perform an Address Write with a value of EPDAT.
  c2_address_write(C2EPDAT);

  // 9. Repeat steps 7 and 8 until all bytes are read.
  uint8_t i = 0;
  do {
    // 7.1. Perform an Address Write operation with a value of EPSTAT.
    c2_address_write(C2EPSTAT);

    // 7.2. Perform a Data Read operation and check the bits of the EPSTAT register.
    uint8_t err;
    C2_DATA_READ_AND_CHECK(err, 1);
    if (err & C2EPSTAT_ERROR) return C2_CMD_ERROR;

    // 7.3. Perform an Address Write operation with a value of EPDAT.
    c2_address_write(C2EPDAT);

    // 7. Perform Address Read instructions until the value returned is not 0x80 and the EPROM is no longer busy.
    C2_POLL_INBUSY();

    // 8.1. Perform an Address Write operation with a value of EPSTAT.
    c2_address_write(C2EPSTAT);

    // 8.2. Perform a Data Read operation and check the ERROR bit in the EPSTAT register.
    C2_DATA_READ_AND_CHECK(err, 1);
    if (err & C2EPSTAT_ERROR) return C2_CMD_ERROR;

    // 8.3. Perform an Address Write operation with a value of EPDAT.
    C2_DATA_WRITE_AND_CHECK(C2EPDAT, 1);

    // 8. Read the byte using the Data Read instruction.
    C2_DATA_READ_AND_CHECK(data[i], 1);
  } while (++i != len);

  // 10. Write 0x40 to EPCTL.
  c2_address_write(C2EPCTL);
  C2_DATA_WRITE_AND_CHECK(0x40, 1);

  // 11. Write 0x00 to EPCTL.
  C2_DATA_WRITE_AND_CHECK(0x00, 1);

  // 12. Write 0x02 to FPCTL.
  c2_address_write(C2FPCTL);
  C2_DATA_WRITE_AND_CHECK(0x02, 1);

  // 13. Write 0x04 to FPCTL.
  C2_DATA_WRITE_AND_CHECK(0x04, 1);

  // 14. Write 0x01 to FPCTL.
  C2_DATA_WRITE_AND_CHECK(0x01, 1);

  return C2_SUCCESS;
}

uint8_t c2_page_erase(uint8_t page) {
  // 1. Perform an Address Write with a value of FPDAT.
  c2_address_write(C2FPDAT);

  // 2. Perform a Data Write with the Page Erase command.
  c2_data_write(C2FPDAT_FLASH_PAGE_ERASE, 1);

  // 3. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 4. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 5. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  // 6. Perform a Data Write with the page number.
  c2_data_write(page, 1);

  // 7. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 8. Poll on OutReady using Address Read until the bit clears.
  C2_POLL_OUTREADY();

  // 9. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  // 10. Perform a Data Write with the a value of 0x00.
  c2_data_write(0x00, 1);

  // 11. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 12. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 13. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  return C2_SUCCESS;
}

uint8_t c2_device_erase() {
  // 1. Perform an Address Write with a value of FPDAT.
  c2_address_write(C2FPDAT);

  // 2. Perform a Data Write with the Device Erase command.
  C2_DATA_WRITE_AND_CHECK(C2FPDAT_DEVICE_ERASE, 1);

  // 3. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 4. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 5. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  // 6. Perform a Data Write with a value of 0xDE.
  C2_DATA_WRITE_AND_CHECK(0xDE, 1);

  // 7. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 8. Perform a Data Write with a value of 0xAD.
  C2_DATA_WRITE_AND_CHECK(0xAD, 1);

  // 9. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 10. Perform a Data Write with a value of 0xA5.
  C2_DATA_WRITE_AND_CHECK(0xA5, 1);

  // 11. Poll on InBusy using Address Read until the bit clears.
  C2_POLL_INBUSY();

  // 12. Poll on OutReady using Address Read until the bit set.
  C2_POLL_OUTREADY();

  // 13. Perform a Data Read instruction. A value of 0x0D is okay.
  C2_EXPECT_DATA(0x0D);

  return C2_SUCCESS;
}

uint8_t c2_sfr_write_non_paged(uint8_t address, uint8_t data) {
  // 1. Write the SFR address to the device using the Address Write instruction.
  c2_address_write(address);

  // 2. Write the SFR value to the device using the Data Write instruction.
  C2_DATA_WRITE_AND_CHECK(data, 1);

  return C2_SUCCESS;
}

uint8_t c2_sfr_write_paged(uint8_t address, uint8_t data) {
  // 1. Perform an Address Write with a value of FPDAT.
  c2_address_write(C2FPDAT);

  // 2. Write the Direct Write command (0x0A) using a Data Write
  C2_DATA_WRITE_AND_CHECK(C2FPDAT_DIRECT_WRITE, 1);

  // 3. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  // 4. Poll OutReady it sets to 1.
  C2_POLL_OUTREADY();

  // 5. Perform a Data Read to ensure a return value of 0x0D (no errors).
  C2_EXPECT_DATA(0x0D);

  // 6. Perform a Data Write with a value of the SFR address.
  C2_DATA_WRITE_AND_CHECK(address, 1);

  // 7. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  // 8. Perform a Data Write with a value of 0x01.
  C2_DATA_WRITE_AND_CHECK(0x01, 1);

  // 9. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  // 10. Perform a Data Write with the new SFR value.
  C2_DATA_WRITE_AND_CHECK(data, 1);

  // 11. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  return C2_SUCCESS;
}

// 4.6. Reading from an SFR
// To read from an SFR on a device that does not have SFR paging:
uint8_t c2_sfr_read_non_paged(uint8_t address, uint8_t &v) {
  // 1. Write the SFR address to the device using the Address Write instruction.
  c2_address_write(address);

  // 2. Read the SFR value from the device using the Data Read instruction.
  C2_DATA_READ_AND_CHECK(v, 1);

  return C2_SUCCESS;
}

// For devices with SFR paging, direct reads through the PI using the Direct Read command are recommended to ensure the SFR Page is managed properly.
// To read an SFR from a device with SFR paging:
uint8_t c2_sfr_read_paged(uint8_t address, uint8_t &v) {
  // 1. Perform an Address Write with a value of FPDAT.
  c2_address_write(C2FPDAT);

  // 2. Write the Direct Read command (0x09) using a Data Write.
  C2_DATA_WRITE_AND_CHECK(C2FPDAT_DIRECT_READ, 1);

  // 3. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  // 4. Poll OutReady until it sets to 1.
  C2_POLL_OUTREADY();

  // 5. Perform a Data Read to ensure a return value of 0x0D (no errors).
  C2_EXPECT_DATA(0x0D);

  // 6. Perform a Data Write with a value of the SFR address.
  C2_DATA_WRITE_AND_CHECK(address, 1);

  // 7. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  // 8. Perform a Data Write with a value of 0x01.
  C2_DATA_WRITE_AND_CHECK(0x01, 1);

  // 9. Poll InBusy until the data is processed by the PI.
  C2_POLL_INBUSY();

  // 10. Poll OutReady until it sets to 0.
  C2_POLL_OUTREADY();

  // 11. Read the SFR value from the device using the Data Read instruction.
  C2_DATA_READ_AND_CHECK(v, 1);

  return C2_SUCCESS;
}

const char *c2_print_status_by_name(uint8_t ch) {
  switch (ch) {
    case C2_SUCCESS:       return "Success";
    case C2_SHIFT_TIMEOUT: return "Shift wait timeout error";
    case C2_POLL_TIMEOUT:  return "Register poll timeout error";
    case C2_CMD_ERROR:     return "In-command error";
    case C2_BROKEN_LINK:   return "Broken link, address read failed";
    default:               return "unknownl error";
  }
}

// This is to enforce arduino-like formatting in kate
// kate: space-indent on; indent-width 2; mixed-indent off; indent-mode cstyle;
