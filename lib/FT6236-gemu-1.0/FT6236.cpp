#include <Wire.h>
#include <FT6236.h>

/*
 * This is a static library so we need to make sure we process stuff as quick as possible
 * as we do not want it to interfere with the RTOS by delaying routines unnecessarily.
 * So, no delay()'s etc and opto the code as much as possible.
 * ^^^ Need to be on TODO list to go through and make sure everything is as opto as
 * possible
 */

uint8_t FT6236buf[FT6236_BUFFER_SIZE];
uint8_t FT6236_i2c_addr = 0x38;
uint8_t lenLibVersion = 0;
uint8_t firmwareId = 0;

struct tbuttonregister {
  uint16_t BUTTONID;
  uint16_t xmin;
  uint16_t xmax;
  uint16_t ymin;
  uint16_t ymax;
} buttonregister[FT6236_MAX_BUTTONS];  // we're limiting to 16 buttons for now - can reduce or increase later as needed.

uint8_t buttoncount = 0;

void FT6236flushbuttonregister(void) {
  uint16_t bid;
  for (bid=0;bid<FT6236_MAX_BUTTONS;bid++) {
    buttonregister[bid].BUTTONID=0;
    buttonregister[bid].xmin=0;
    buttonregister[bid].xmax=0;
    buttonregister[bid].ymin=0;
    buttonregister[bid].ymax=0;
  }
  buttoncount=0;
}

void FT6236registerbutton(uint16_t buttonid,uint16_t xmin,uint16_t ymin,uint16_t xmax, uint16_t ymax) {
  buttonregister[buttoncount].BUTTONID=buttonid;
  buttonregister[buttoncount].xmin=xmin;
  buttonregister[buttoncount].xmax=xmax;
  buttonregister[buttoncount].ymin=ymin;
  buttonregister[buttoncount].ymax=ymax;
  buttoncount++;
}

uint16_t FT6236GetButtonMask(void) {
  uint16 bid;
  TouchLocation tl[2];
  uint8_t count = FT6236readTouchLocation(tl,2);
  if (count > 0) {
      uint16_t x = tl[0].x;
	  uint16_t y = tl[0].y;
	  for (bid=0;bid<buttoncount;bid++) {
	       if (x >= buttonregister[bid].xmin) {
		       if (x <= buttonregister[bid].xmax) {
			       if (y >= buttonregister[bid].ymin) {
				       if (y <= buttonregister[bid].ymax) {
					       return buttonregister[bid].BUTTONID;
					   }
				   }
			   }
		   }
	  }
  }
  return 0;
}

void FT6236begin(uint8_t i2c_addr) {
 FT6236_i2c_addr=i2c_addr;
 Wire.begin();
 FT6236writeTouchRegister(0,FT6236_MODE_NORMAL);
 lenLibVersion = FT6236readTouchAddr(0x0a1, FT6236buf, 2 );
 firmwareId = FT6236readTouchRegister( 0xa6 );
}

void FT6236writeTouchRegister(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(FT6236_i2c_addr);
  Wire.write(reg);  					// register 0
  Wire.write(val);  					// value
  Wire.endTransmission();
}

uint8_t FT6236readTouchRegister(uint8_t reg)
{
  Wire.beginTransmission(FT6236_i2c_addr);
  Wire.write(reg);  // register 0
  uint8_t retVal = Wire.endTransmission();
  uint8_t returned = Wire.requestFrom(FT6236_i2c_addr,uint8_t(1));    // request 6 uint8_ts from slave device #2
  if (Wire.available())
  {
    retVal = Wire.read();
  }
  return retVal;
}

uint8_t FT6236readTouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len )
{
  Wire.beginTransmission(FT6236_i2c_addr);
  Wire.write( regAddr );  // register 0
  uint8_t retVal = Wire.endTransmission();
  uint8_t returned = Wire.requestFrom(FT6236_i2c_addr, len);    // request 1 bytes from slave device #2
  uint8_t i;
  for (i = 0; (i < len) && Wire.available(); i++) {
    pBuf[i] = Wire.read();
  }
  return i;
}

uint8_t FT6236readTouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal = 0;
  uint8_t i;
  uint8_t k;
  do
  {
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one
    uint8_t status = FT6236readTouchRegister(2);
    static uint8_t tbuf[40];
    if ((status & 0x0f) == 0) break; // no points detected
    uint8_t hitPoints = status & 0x0f;
    FT6236readTouchAddr( 0x03, tbuf, hitPoints*6);
    for (k=0,i = 0; (i < hitPoints*6)&&(k < num); k++, i += 6) {
      pLoc[k].x = (tbuf[i+0] & 0x0f) << 8 | tbuf[i+1];
      pLoc[k].y = (tbuf[i+2] & 0x0f) << 8 | tbuf[i+3];
    }
    retVal = k;
  } while (0);
  return retVal;
}

uint32_t FT6236dist(const TouchLocation & loc)
{
  uint32_t retVal = 0;
  uint32_t x = loc.x;
  uint32_t y = loc.y;
  retVal = x*x + y*y;
  return retVal;
}


/*
uint32_t FT6236dist(const TouchLocation & loc1, const TouchLocation & loc2)
{
  uint32_t retVal = 0;
  uint32_t x = loc1.x - loc2.x;
  uint32_t y = loc1.y - loc2.y;
  retVal = sqrt(x*x + y*y);
  return retVal;
}
*/

bool FT6236sameLoc( const TouchLocation & loc, const TouchLocation & loc2 )
{
  return FT6236dist(loc,loc2) < 50;
}
