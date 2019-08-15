/*
  xsns_50_paj7620.ino - gesture sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends & Christian Baars

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  --------------------------------------------------------------------------------------------
  Version Date      Action    Description
  --------------------------------------------------------------------------------------------


  ---
  1.0.0.0 20190808  started   - further development by Christian Baars  - https://github.com/Staars/Sonoff-Tasmota
                    forked    - from arendst/tasmota                    - https://github.com/arendst/Sonoff-Tasmota
                    base      - code base from arendst and              - https://github.com/Seeed-Studio/Gesture_PAJ7620

*/

#ifdef USE_I2C
#ifdef USE_PAJ7620

/*********************************************************************************************\
 * PAJ7620 - Gesture sensor
 * 
 * I2C Address: 0x73 - standard address
\*********************************************************************************************/

#define XSNS_50                     50

#define PAJ7620_ADDR                0x73              // standard address

#define PAJ7620_BANK_SEL		        0xEF 	            // 8 bit, write -> 0 or 1

// the registers are organized in 2 banks  
// bank: 0
#define PAJ7620_GET_GESTURE           0x43            // 8 bit, read
#define PAJ7620_PROXIMITY_AVG_Y       0x6c            // 8 bit, read -> 255: near , lower numbers: far

#define PAJ7620_OBJECT_CENTER_X       0xad						// 5 bit, read
#define PAJ7620_OBJECT_CENTER_Y       0xaf						// 5 bit, read

#define PAJ7620_DOWN                  1               // readings from PAJ7620_GET_GESTURE
#define PAJ7620_UP                    2
#define PAJ7620_RIGHT                 4
#define PAJ7620_LEFT                  8
#define PAJ7620_NEAR                  16
#define PAJ7620_FAR                   32
#define PAJ7620_CW                    64
#define PAJ7620_CCW                   128

// bank: 1
// nothing at the moment

const uint8_t PAJ7620initRegisterArray[][2] PROGMEM = {	// set all needed registers 
    {0xEF,0x00},  // bank 0
	{0x32,0x29},	{0x33,0x01},	{0x34,0x00},  {0x35,0x01},	{0x36,0x00},	{0x37,0x07},	{0x38,0x17},	{0x39,0x06},
	{0x3A,0x12},	{0x3F,0x00},	{0x40,0x02},	{0x41,0xFF},	{0x42,0x01},	{0x46,0x2D},	{0x47,0x0F},	{0x48,0x3C},
	{0x49,0x00},	{0x4A,0x1E},	{0x4B,0x00},	{0x4C,0x20},	{0x4D,0x00},	{0x4E,0x1A},	{0x4F,0x14},	{0x50,0x00},
	{0x51,0x10},	{0x52,0x00},	{0x5C,0x02},	{0x5D,0x00},	{0x5E,0x10},	{0x5F,0x3F},	{0x60,0x27},	{0x61,0x28},
	{0x62,0x00},	{0x63,0x03},	{0x64,0xF7},	{0x65,0x03},	{0x66,0xD9},	{0x67,0x03},	{0x68,0x01},	{0x69,0xC8},
	{0x6A,0x40},	{0x6D,0x04},	{0x6E,0x00},	{0x6F,0x00},	{0x70,0x80},	{0x71,0x00},	{0x72,0x00},	{0x73,0x00},
	{0x74,0xF0},	{0x75,0x00},	{0x80,0x42},	{0x81,0x44},	{0x82,0x04},	{0x83,0x20},	{0x84,0x20},	{0x85,0x00},
	{0x86,0x10},	{0x87,0x00},	{0x88,0x05},	{0x89,0x18},	{0x8A,0x10},	{0x8B,0x01},	{0x8C,0x37},	{0x8D,0x00},
	{0x8E,0xF0},	{0x8F,0x81},	{0x90,0x06},	{0x91,0x06},	{0x92,0x1E},	{0x93,0x0D},	{0x94,0x0A},	{0x95,0x0A},
	{0x96,0x0C},	{0x97,0x05},	{0x98,0x0A},	{0x99,0x41},	{0x9A,0x14},	{0x9B,0x0A},	{0x9C,0x3F},	{0x9D,0x33},
	{0x9E,0xAE},	{0x9F,0xF9},	{0xA0,0x48},	{0xA1,0x13},	{0xA2,0x10},	{0xA3,0x08},	{0xA4,0x30},	{0xA5,0x19},
	{0xA6,0x10},	{0xA7,0x08},	{0xA8,0x24},	{0xA9,0x04},	{0xAA,0x1E},	{0xAB,0x1E},	{0xCC,0x19},	{0xCD,0x0B},
	{0xCE,0x13},	{0xCF,0x64},	{0xD0,0x21},	{0xD1,0x0F},	{0xD2,0x88},	{0xE0,0x01},	{0xE1,0x04},	{0xE2,0x41},
	{0xE3,0xD6},	{0xE4,0x00},	{0xE5,0x0C},	{0xE6,0x0A},	{0xE7,0x00},	{0xE8,0x00},	{0xE9,0x00},	{0xEE,0x07},
	{0xEF,0x01}, // bank 1
	{0x00,0x1E},	{0x01,0x1E},	{0x02,0x0F},	{0x03,0x10},	{0x04,0x02},	{0x05,0x00},	{0x06,0xB0},	{0x07,0x04},
	{0x08,0x0D},	{0x09,0x0E},	{0x0A,0x9C},	{0x0B,0x04},	{0x0C,0x05},	{0x0D,0x0F},	{0x0E,0x02},	{0x0F,0x12},
	{0x10,0x02},	{0x11,0x02},	{0x12,0x00},	{0x13,0x01},	{0x14,0x05},	{0x15,0x07},	{0x16,0x05},	{0x17,0x07},
	{0x18,0x01},	{0x19,0x04},	{0x1A,0x05},	{0x1B,0x0C},	{0x1C,0x2A},	{0x1D,0x01},	{0x1E,0x00},	{0x21,0x00},
	{0x22,0x00},	{0x23,0x00},	{0x25,0x01},	{0x26,0x00},	{0x27,0x39},	{0x28,0x7F},	{0x29,0x08},	{0x30,0x03},
	{0x31,0x00},	{0x32,0x1A},	{0x33,0x1A},	{0x34,0x07},	{0x35,0x07},	{0x36,0x01},	{0x37,0xFF},	{0x38,0x36},
	{0x39,0x07},	{0x3A,0x00},	{0x3E,0xFF},	{0x3F,0x00},	{0x40,0x77},	{0x41,0x40},	{0x42,0x00},	{0x43,0x30},
	{0x44,0xA0},	{0x45,0x5C},	{0x46,0x00},	{0x47,0x00},	{0x48,0x58},	{0x4A,0x1E},	{0x4B,0x1E},	{0x4C,0x00},
	{0x4D,0x00},	{0x4E,0xA0},	{0x4F,0x80},	{0x50,0x00},	{0x51,0x00},	{0x52,0x00},	{0x53,0x00},	{0x54,0x00},
	{0x57,0x80},	{0x59,0x10},	{0x5A,0x08},	{0x5B,0x94},	{0x5C,0xE8},	{0x5D,0x08},	{0x5E,0x3D},	{0x5F,0x99},
	{0x60,0x45},	{0x61,0x40},	{0x63,0x2D},	{0x64,0x02},	{0x65,0x96},	{0x66,0x00},	{0x67,0x97},	{0x68,0x01},
	{0x69,0xCD},	{0x6A,0x01},	{0x6B,0xB0},	{0x6C,0x04},	{0x6D,0x2C},	{0x6E,0x01},	{0x6F,0x32},	{0x71,0x00},
	{0x72,0x01},	{0x73,0x35},	{0x74,0x00},	{0x75,0x33},	{0x76,0x31},	{0x77,0x01},	{0x7C,0x84},	{0x7D,0x03},
	{0x7E,0x01},
  {0xEF,0x00} // back to bank 0
};

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_PAJ7620 "PAJ7620"

const char S_JSON_PAJ7620_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_PAJ7620 "%s\":%d}";

const char kPAJ7620Types[] PROGMEM = "PAJ7620";

const uint8_t PAJ7620_PIN[]= {1,2,3,4}; // TOP-SECRET!! ;)


/*********************************************************************************************\
 * helper function
\*********************************************************************************************/

void PAJ7620SelectBank(uint8_t bank)
{
  switch(bank){
		case 0:
      I2cWrite(PAJ7620_ADDR, PAJ7620_BANK_SEL, 0, 1);
			break;
		case 1:
      I2cWrite(PAJ7620_ADDR, PAJ7620_BANK_SEL, 1, 1);
			break;
		default:
			break;
	}
}

/********************************************************************************************/

void PAJ7620TriggerTele(){
  mqtt_data[0] = '\0';
  if (MqttShowSensor()) {
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
#ifdef USE_RULES
    RulesTeleperiod();  // Allow rule based HA messages
#endif  // USE_RULES
  }
}

/********************************************************************************************\
| *globals
\*********************************************************************************************/

char       PAJ7620_name[9];

uint32_t PAJ7620_timeout_counter = 10;    // the time interval is 100 ms -> i.e. 10 is 1 second (= start up interval)
uint32_t PAJ7620_next_job = 0;            // 0 = detect, 1 = init, 2 = wait for gesture, 255 = sensor not found and do nothing
uint32_t PAJ7620_mode = 1;								// 0 = mute, 1 = gestures only, 2 = gestures, 3 = corner, 4 = PIN, 5 = xy 

struct {
  uint8_t current;
  uint8_t last;
  uint8_t same;        // number of identical gestures in a row - 255 should be enough
  uint8_t unfinished;  // used for up,down,left,right to avoid false interpretation for near and far
} PAJ7620_gesture;

bool PAJ7620_finished_gesture = false;
char PAJ7620_currentGestureName[6];

struct{ 
  uint8_t x;
  uint8_t y;
  uint8_t last_x;
  uint8_t last_y;
  uint8_t proximity;
  uint8_t last_proximity;
  uint8_t corner;
  struct { //TODO: multiple PIN's
    uint8_t step:3; // max. 4 steps ATM, but leave a little more space
    uint8_t countdown:3; // max. 0.7 seconds for each corner
    uint8_t valid:1;
    } PIN;
} PAJ7620_state;

/********************************************************************************************/

/********************************************************************************************/
void PAJ7620DecodeGesture(void)
{
  switch (PAJ7620_gesture.current) { // we will accept only "clean" recognized gestures, the sensor can report multiple gestures at once via bitfield, but these are discarded
    case PAJ7620_DOWN:
      DEBUG_SENSOR_LOG(PSTR("DOWN"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("Down"));
      if(PAJ7620_gesture.unfinished){            // for better recognition of NEAR and FAR
        PAJ7620_finished_gesture = true;      // consider the gesture finished only in the second try, this adds some delay for up,down,left,right
        break;
      }
      PAJ7620_gesture.unfinished = PAJ7620_gesture.current;  // save the gesture, maybe it will be the final one
      PAJ7620_timeout_counter = 5; // 0.5 (plus 0.3) seconds time interval to go into the sensing area and change movement to NEAR or FAR
      break;
    case PAJ7620_UP:
      DEBUG_SENSOR_LOG(PSTR("UP"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("Up"));
      if(PAJ7620_gesture.unfinished){
        PAJ7620_finished_gesture = true;
        break;
      }
      PAJ7620_gesture.unfinished = PAJ7620_gesture.current;
      PAJ7620_timeout_counter = 5;
      break;
    case PAJ7620_RIGHT:
      DEBUG_SENSOR_LOG(PSTR("RIGHT"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("Right"));
      if(PAJ7620_gesture.unfinished){
        PAJ7620_finished_gesture = true;
        break;
      }
      PAJ7620_gesture.unfinished = PAJ7620_gesture.current;
      PAJ7620_timeout_counter = 5;
      break;
    case PAJ7620_LEFT:
      DEBUG_SENSOR_LOG(PSTR("LEFT"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("Left"));
      if(PAJ7620_gesture.unfinished){
        PAJ7620_finished_gesture = true;
        break;
      }
      PAJ7620_gesture.unfinished = PAJ7620_gesture.current;
      PAJ7620_timeout_counter = 5;
      break;
    case PAJ7620_NEAR:
      DEBUG_SENSOR_LOG(PSTR("NEAR"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("Near"));
      PAJ7620_finished_gesture = true;
      PAJ7620_timeout_counter = 25; // more time to "escape" from gesture (will be 2.8 second)
      break;
    case PAJ7620_FAR:
      DEBUG_SENSOR_LOG(PSTR("FAR"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("Far"));
      PAJ7620_finished_gesture = true;
      PAJ7620_timeout_counter = 25;
      break;
    case PAJ7620_CW:
      DEBUG_SENSOR_LOG(PSTR("ClockWise"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("CW"));
      PAJ7620_finished_gesture = true;
      break;
    case PAJ7620_CCW:
      DEBUG_SENSOR_LOG(PSTR("CounterClockWise"));
      snprintf_P(PAJ7620_currentGestureName, sizeof(PAJ7620_currentGestureName), PSTR("CCW"));
      PAJ7620_finished_gesture = true;
      break;
    default:
      if(PAJ7620_gesture.unfinished){
        PAJ7620_finished_gesture = true; // this will finish up, down, left, right
        break;
      }
      break;
  }
if(PAJ7620_finished_gesture){
  if (PAJ7620_gesture.unfinished){
    if(PAJ7620_gesture.current!=PAJ7620_NEAR && PAJ7620_gesture.current!=PAJ7620_FAR){
      PAJ7620_gesture.current = PAJ7620_gesture.unfinished; // to count correctly for up, down, right, left
      }
  }
  if (PAJ7620_gesture.current == PAJ7620_gesture.last){
    PAJ7620_gesture.same++;
    }
  else{
    PAJ7620_gesture.same = 1;
    }
  PAJ7620_gesture.last = PAJ7620_gesture.current;
  PAJ7620_finished_gesture = false;
  PAJ7620_gesture.unfinished = 0;
  PAJ7620_timeout_counter += 3; // add delay 0.3 seconds for every kind of gesture
  PAJ7620TriggerTele();
  }
}

/********************************************************************************************/
void PAJ7620ReadGesture(void){
  switch(PAJ7620_mode){
    case 1:
    PAJ7620_gesture.current = I2cRead8(PAJ7620_ADDR,PAJ7620_GET_GESTURE);
    if(PAJ7620_gesture.current > 0 || PAJ7620_gesture.unfinished){
      DEBUG_SENSOR_LOG(PSTR("PAJ7620: gesture: %u"),PAJ7620_gesture.current );
      PAJ7620DecodeGesture();
      }
    
    break;
    case 2:
    PAJ7620_state.proximity = I2cRead8(PAJ7620_ADDR, PAJ7620_PROXIMITY_AVG_Y);
    if((PAJ7620_state.proximity>0)||(PAJ7620_state.last_proximity>0))
    {
      if(PAJ7620_state.proximity!=PAJ7620_state.last_proximity){
        PAJ7620_state.last_proximity = PAJ7620_state.proximity;
        DEBUG_SENSOR_LOG(PSTR("PAJ7620: Proximity: %u"),PAJ7620_state.proximity );
        PAJ7620TriggerTele();
      }
    }
    break;
    case 3: case 4: case 5:
    PAJ7620_state.x = I2cRead8(PAJ7620_ADDR, PAJ7620_OBJECT_CENTER_X);
    PAJ7620_state.y = I2cRead8(PAJ7620_ADDR, PAJ7620_OBJECT_CENTER_Y);
    if(PAJ7620_state.y>0 && PAJ7620_state.x>0){
      if(PAJ7620_state.y!=PAJ7620_state.last_y || PAJ7620_state.x!=PAJ7620_state.last_x){
        PAJ7620_state.last_y = PAJ7620_state.y;
        PAJ7620_state.last_x = PAJ7620_state.x;
        DEBUG_SENSOR_LOG(PSTR("PAJ7620: x: %u   y: %u"), PAJ7620_state.x, PAJ7620_state.y);

        PAJ7620_state.corner = 0;
        // 1|2
        // ---
        // 3|4
        switch(PAJ7620_state.y){
          case 0: case 1: case 2: case 3: case 4: case 5:// case 0..5: would be nicer 
            PAJ7620_state.corner = 3;
            break;
          case 9: case 10: case 11: case 12: case 13: case 14:
            PAJ7620_state.corner = 1;
            break;
          default:
          break;
        }
        if(PAJ7620_state.corner!=0){      
          switch(PAJ7620_state.x){
          case 0: case 1: case 2: case 3: case 4: case 5:
            break;
          case 9: case 10: case 11: case 12: case 13: case 14:
            PAJ7620_state.corner++;
            break;
          default:
          PAJ7620_state.corner = 0;
          break;
          }
        }
        DEBUG_SENSOR_LOG(PSTR("PAJ7620: corner: %u"), PAJ7620_state.corner);
        // PIN-part:
        if(PAJ7620_state.PIN.countdown == 0){
            PAJ7620_state.PIN.step=0;
            PAJ7620_state.PIN.valid=0;
        }
        if(!PAJ7620_state.PIN.step){
          if(PAJ7620_state.corner == PAJ7620_PIN[PAJ7620_state.PIN.step]){
            PAJ7620_state.PIN.step=1;
            PAJ7620_state.PIN.countdown=7;
          }
        }
        else{
          if(PAJ7620_state.corner == PAJ7620_PIN[PAJ7620_state.PIN.step]){
            PAJ7620_state.PIN.step+=1;
            PAJ7620_state.PIN.countdown=7;
          }
          else{
            PAJ7620_state.PIN.countdown-=1;
          }
        }
        if(PAJ7620_state.PIN.step == 4){
          PAJ7620_state.PIN.valid = 1;
          DEBUG_SENSOR_LOG(PSTR("PAJ7620: PIN valid!!"));
          PAJ7620_state.PIN.countdown = 0; // will restart in the next loop
        }
        PAJ7620TriggerTele();
        }
    }
    break;
    default:
    break;
  }
}

/********************************************************************************************/

void PAJ7620Detect(void)
{
  DEBUG_SENSOR_LOG(PSTR("PAJ7620: scan will start ..."));
  PAJ7620SelectBank(0);
  PAJ7620SelectBank(0); // do it twice
  uint16_t PAJ7620_id = I2cRead16LE(PAJ7620_ADDR,0); // read ID from reg 1 and 0
	uint8_t PAJ7620_ver = I2cRead8(PAJ7620_ADDR,2);
  if (PAJ7620_id == 0x7620) { // this device ID makes sense ;)
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PAJ7620: sensor found with ID: 0x%x and VER: %u"), PAJ7620_id, PAJ7620_ver);
    uint8_t PAJ7620_model = 0; 
    GetTextIndexed(PAJ7620_name, sizeof(PAJ7620_name), PAJ7620_model, kPAJ7620Types);
    PAJ7620_next_job = 1; // now init
  }
  else {
    DEBUG_SENSOR_LOG(PSTR("PAJ7620: sensor not found, false ID 0x%x"), PAJ7620_id);
    PAJ7620_next_job = 255; // do not loop
  }
}

/********************************************************************************************/
void PAJ7620Init(void)
{
  DEBUG_SENSOR_LOG(PSTR("PAJ7620: init sensor start %u"),millis());
  union{
    uint32_t raw;
    uint8_t reg_val[4];
  } buf;
  for(uint32_t i = 0; i < (sizeof(PAJ7620initRegisterArray)/2); i+=2) 
	{
    buf.raw = pgm_read_dword(PAJ7620initRegisterArray+i);
    DEBUG_SENSOR_LOG("%x %x %x %x",buf.reg_val[0],buf.reg_val[1],buf.reg_val[2],buf.reg_val[3]);
    I2cWrite(PAJ7620_ADDR, buf.reg_val[0], buf.reg_val[1], 1);
    I2cWrite(PAJ7620_ADDR, buf.reg_val[2], buf.reg_val[3], 1);
	}
  DEBUG_SENSOR_LOG(PSTR("PAJ7620: init sensor done %u"),millis());
  PAJ7620_next_job = 2; // now loop and wait for gestures
}

/********************************************************************************************/

void PAJ7620SelectMode(uint16_t mode){
	DEBUG_SENSOR_LOG(PSTR("PAJ7620: set mode to %u"),mode);
	switch(mode){
		case 0:
    PAJ7620_mode = 0;
		break;
		case 1:
    PAJ7620_mode = 1;
    break;
    case 2:
    PAJ7620_mode = 2;
    break;
    case 3:
    PAJ7620_mode = 3;
    break;
    case 4:
    PAJ7620_mode = 4;
    break;
    case 5:
    PAJ7620_mode = 5;
    break;
		default:
		break;
	}
}
/********************************************************************************************/

void PAJ7620Loop(void)
{
  if(PAJ7620_timeout_counter == 0){
    switch(PAJ7620_next_job){
      case 0:
      PAJ7620Detect();
      break;
      case 1:
      PAJ7620Init();
      break;
      case 2:
			if(PAJ7620_mode != 0){
				PAJ7620ReadGesture();
				}
      break;
      default:
      break;
    }
  }
  else {
    PAJ7620_timeout_counter--;
  }
}

/********************************************************************************************/
// normaly in i18n.h

#define D_JSON_PAJ7620 "PAJ7620"

#ifdef USE_WEBSERVER
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

 const char HTTP_SNS_PAJ7620[] PROGMEM = "{s} " D_JSON_PAJ7620 ": {m}%s {e}";
 const char HTTP_SNS_PAJ7620VER[] PROGMEM = "{s} PAJ7620 at address: {m}0x73{e}"
                                          "{s} version: {m}1 {e}";  // only hard-coded ATM                                                                                          ;

#endif  // USE_WEBSERVER


/********************************************************************************************/

void PAJ7620Show(bool json)
{
  if (json) {
    if((PAJ7620_currentGestureName[0] != '\0' )){
      ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%u}"), PAJ7620_name, PAJ7620_currentGestureName, PAJ7620_gesture.same);
      PAJ7620_currentGestureName[0] = '\0';
      return;
    }
    switch(PAJ7620_mode){
      case 2:
      if(PAJ7620_mode>1){
        ResponseAppend_P(PSTR(",\"%s\":{\"Proximity\":%u}"), PAJ7620_name, PAJ7620_state.proximity);
      }
      break;
      case 3:
      if(PAJ7620_mode>1 && PAJ7620_state.corner>0){
        ResponseAppend_P(PSTR(",\"%s\":{\"Corner\":%u}"), PAJ7620_name, PAJ7620_state.corner);
      }
      break;
      case 4:
      if(PAJ7620_mode>1 && PAJ7620_state.PIN.valid){
        ResponseAppend_P(PSTR(",\"%s\":{\"PIN\":%u}"), PAJ7620_name, 1); //TODO:  more than one PIN
        PAJ7620_state.PIN.valid = 0;
      }
      break;
      case 5:
      if(PAJ7620_mode>1){
        ResponseAppend_P(PSTR(",\"%s\":{\"x\":%u,\"y\":%u}"), PAJ7620_name, PAJ7620_state.x, PAJ7620_state.y);
      }
      break;
      default:
      break;
    }
  #ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_PAJ7620VER);
  #endif  // USE_WEBSERVER
      }
}

/*********************************************************************************************\
 * check the PAJ7620 commands
\*********************************************************************************************/

bool PAJ7620Cmd(void) {
  bool serviced = true;
			if (XdrvMailbox.data_len > 0) {
				DEBUG_SENSOR_LOG(PSTR("PAJ7620: got argument for mode"));  
        PAJ7620SelectMode(XdrvMailbox.payload);     //select mode
        Response_P(S_JSON_PAJ7620_COMMAND_NVALUE, XdrvMailbox.command, XdrvMailbox.payload);
        }
      else {
        DEBUG_SENSOR_LOG(PSTR("PAJ7620: show mode")); 
        Response_P(S_JSON_PAJ7620_COMMAND_NVALUE, XdrvMailbox.command, PAJ7620_mode);
      }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns50(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {  
    switch (function) {
      case FUNC_INIT:
        DEBUG_SENSOR_LOG(PSTR("PAJ7620: 1 second until init"));    
        break;
			case FUNC_COMMAND_SENSOR:
				if (XSNS_50 == XdrvMailbox.index){
          result = PAJ7620Cmd();
        }  
        break;
      case FUNC_EVERY_100_MSECOND:
        if(PAJ7620_next_job <255) {
          PAJ7620Loop();
        }
        break;
      case FUNC_JSON_APPEND:
        PAJ7620Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        PAJ7620Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PAJ7620
#endif  // USE_I2C