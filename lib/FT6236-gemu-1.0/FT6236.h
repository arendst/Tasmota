#ifndef FT6236
#define FT6236

#define FT6236_MODE_NORMAL	0x00
#define FT6236_MODE_TEST	0x04
#define FT6236_MODE_SYSTEM	0x01

#define FT6236_BUFFER_SIZE	0x1E	// 30 bytes buffer
#define FT6236_MAX_BUTTONS	1	// 50 buttons should be enough for just about any page

struct TouchLocation {
  uint16_t y;				// we swop x and y in position because we're using the screen in portrait mode
  uint16_t x;
};

void FT6236flushbuttonregister(void);
void FT6236registerbutton(uint16_t buttonid,uint16_t xmin,uint16_t ymin,uint16_t xmax, uint16_t ymax);
uint16_t FT6236GetButtonMask(void);
void FT6236begin(uint8_t i2c_addr);
uint8_t FT6236readTouchRegister( uint8_t reg );
uint8_t FT6236readTouchLocation( TouchLocation * pLoc, uint8_t num );
uint8_t FT6236readTouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len );
void FT6236writeTouchRegister( uint8_t reg, uint8_t val);
uint32_t FT6236dist(const TouchLocation & loc);
uint32_t FT6236dist(const TouchLocation & loc1, const TouchLocation & loc2);
bool FT6236sameLoc( const TouchLocation & loc, const TouchLocation & loc2 );

#endif
