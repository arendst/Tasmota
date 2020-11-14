#ifndef C2_H
#define C2_H

#include <stdint.h>
#include <Arduino.h>

////////////////////////////////
// Hardware Abstraction Layer //
////////////////////////////////

// Rewrite these for your hardware

#define PIN_C2CK 4
#define PIN_C2D  5

// Set C2CK state
inline void C2CK(bool ck) {
	digitalWrite(PIN_C2CK, ck);
}

// get C2CK state
inline bool C2CK() {
	return digitalRead(PIN_C2CK);
}

// Set C2D state
inline void C2D(bool d) {
	digitalWrite(PIN_C2D, d);
}

// Get C2D state
inline bool C2D() {
	return digitalRead(PIN_C2D);
}

// Enable/disable C2D output butter
inline void C2D_enable(bool oe) {
	if (oe) pinMode(PIN_C2D, OUTPUT);
	else    pinMode(PIN_C2D, INPUT);
}

// Delay functions
#define C2_DELAY_US(n) delayMicroseconds(n)
#define C2_DELAY_MS(n) delay(n)

////////////////////////////////////////////
// Nothing should need change from now on //
////////////////////////////////////////////

// Exceptions:
#define C2_SUCCESS       0x00 // Compare for success
#define C2_ERROR         0xFF // Mask for all errors
#define C2_TIMEOUT       0x03 // Mask for timeouts
#define C2_SHIFT_TIMEOUT 0x01 // Shift wait
#define C2_POLL_TIMEOUT  0x02 // Register poll
#define C2_CMD_ERROR     0x04 // In-command Error
#define C2_BROKEN_LINK   0x08 // Address read returned 0xFF, comms disabled

// Register Addresses
#define C2DEVID 0x00
#define C2REVID 0x01
#define C2FPCTL 0x02
#define C2FPDAT 0xB4

#define C2EPCTL 0x00
#define C2EPDAT 0x00
#define C2EPDAT 0x00
#define C2EPADDRH 0x00
#define C2EPADDRL 0x00
#define C2EPSTAT 0x00

// Commands for FPCTL register
#define C2FPCTL_ENABLE0          0x02
#define C2FPCTL_CORE_HALT        0x04 
#define C2FPCTL_ENABLE1          0x01

// Commands for FPDAT register
#define C2FPDAT_DEVICE_ERASE     0x03
#define C2FPDAT_FLASH_BLOCK_READ 0x06
#define C2FPDAT_BLOCK_WRITE      0x07
#define C2FPDAT_FLASH_PAGE_ERASE 0x08
#define C2FPDAT_BLOCK_READ       0x06
#define C2FPDAT_GET_VERSION      0x01
#define C2FPDAT_GET_DERIVATIVE   0x02
#define C2FPDAT_DIRECT_READ      0x09
#define C2FPDAT_DIRECT_WRITE     0x0A
#define C2FPDAT_INDIRECT_READ    0x0B
#define C2FPDAT_INDIRECT_WRITE   0x0C

// Commands for EPCTL register
#define C2EPCTL_ENABLE0          0x40
#define C2EPCTL_ENABLE1          0x58

// EPSTAT status bits
#define C2EPSTAT_WLOCK 0x80
#define C2EPSTAT_RLOCK 0x40
#define C2EPSTAT_ERROR 0x01

// 'Address read' status bits
#define C2_FLBUSY   0x08
#define C2_EEBUSY   C2_FLBUSY
#define C2_EEERROR  0x04
#define C2_INBUSY   0x02
#define C2_OUTREADY 0x01

// Device families (https://www.silabs.com/documents/public/application-notes/AN127.pdf)
#define	C2_DEVID_UNKNOWN	0x00
#define	C2_DEVID_EFM8BB1	0x30
#define	C2_DEVID_EFM8BB2	0x32
#define	C2_DEVID_EFM8BB3	0x34
#define	C2_DEVID_EFM8LB1	0x34

// Layer 1: C2 Programmig Interface (PI) Register access
void c2_address_write(uint8_t address);
uint8_t c2_address_read();
uint8_t c2_data_write(uint32_t d, uint8_t bytes);
uint8_t c2_data_read(uint32_t &d, uint8_t bytes=4);

// Shorcuts for smaller words
inline uint8_t c2_data_read(uint16_t &d, uint8_t bytes=2) {
	uint32_t dd;
	uint8_t r = c2_data_read(dd, 2);
	d = dd;
	return r;
}
inline uint8_t c2_data_read(uint8_t &d, uint8_t bytes=1) {
	uint32_t dd;
	uint8_t r = c2_data_read(dd, 1);
	d = dd;
	return r;
}

// Layer 2: Operations
uint8_t c2_reset();
uint8_t c2_programming_init(uint8_t devid);
uint8_t c2_block_write(uint32_t address, uint8_t *data, uint8_t len);
uint8_t c2_block_read(uint32_t address, uint8_t *data, uint8_t len);
uint8_t c2_eeprom_read(uint32_t address, uint8_t *data, uint8_t len);
uint8_t c2_page_erase(uint8_t page);
uint8_t c2_device_erase();
uint8_t c2_sfr_write_non_paged(uint8_t address, uint8_t data);
uint8_t c2_sfr_write_paged(uint8_t address, uint8_t data);
uint8_t c2_sfr_read_non_paged(uint8_t address, uint8_t &data);
uint8_t c2_sfr_read_paged(uint8_t address, uint8_t &data);

const char *c2_print_status_by_name(uint8_t ch);

#endif // C2_H
