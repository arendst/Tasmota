/*
  xsns_36_MGC3130.ino - Support for I2C MGC3130 Electric Field Sensor for Sonoff-Tasmota

  Copyright (C) 2019  Christian Baars & Theo Arends

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
*/

//#define USE_MGC3130

#ifdef USE_I2C
#ifdef USE_MGC3130
/*********************************************************************************************\
 * MGC3130 - Electric Field Sensor
 *
 * Adaption for TASMOTA: Christian Baars
 * based on various implementations from Pimoroni, jspark311, hoverlabs and scjurgen
 *
 * I2C Address: 0x42
 *
 * Wiring: SDA/SCL as usual plus RESET and TRANSFER -> 4 Wires
\*********************************************************************************************/

#define XSNS_36                 36

#warning **** MGC3130: It is recommended to disable all unneeded I2C-drivers ****

#define MGC3130_I2C_ADDR        0x42

#define MGC3130_xfer            pin[GPIO_MGC3130_XFER]
#define MGC3130_reset           pin[GPIO_MGC3130_RESET]


bool MGC3130_type = false;
char MGC3130stype[8];


#define MGC3130_SYSTEM_STATUS 0x15
#define MGC3130_REQUEST_MSG   0x06
#define MGC3130_FW_VERSION    0x83
#define MGC3130_SET_RUNTIME   0xA2
#define MGC3130_SENSOR_DATA   0x91


#define MGC3130_GESTURE_GARBAGE         1
#define MGC3130_FLICK_WEST_EAST         2
#define MGC3130_FLICK_EAST_WEST         3
#define MGC3130_FLICK_SOUTH_NORTH       4
#define MGC3130_FLICK_NORTH_SOUTH       5
#define MGC3130_CIRCLE_CLOCKWISE        6 //not active in airwheel mode
#define MGC3130_CIRCLE_CCLOCKWISE       7 //not active in airwheel mode

#define MGC3130_MIN_ROTVALUE            0
#define MGC3130_MAX_ROTVALUE            1023
#define MGC3130_MIN_ZVALUE              32768 // if we fly under the radar, we do not report anything


#ifdef USE_WEBSERVER
const char HTTP_MGC_3130_SNS[] PROGMEM = "%s"
  "{s}" "%s" "{m}%s{e}"
  "{s}" "HwRev" "{m}%u.%u{e}"
  "{s}" "loaderVer" "{m}%u.%u{e}"
  "{s}" "platVer" "{m}%u{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER


/*********************************************************************************************\
 * MGC3130
 *
 * Programmer : MGC3130 Datasheet
\*********************************************************************************************/
#pragma pack(1)
union MGC3130_Union{
  uint8_t buffer[132];
  struct
  {
    // header
    uint8_t msgSize; // in Bytes
    uint8_t flag;  //not used
    uint8_t counter; // cyclic counter of transmitted messages
    uint8_t id; // 0x91 for data output
    // payload
    struct {
        uint8_t DSPStatus:1;
        uint8_t gestureInfo:1;
        uint8_t touchInfo:1;
        uint8_t airWheelInfo:1;
        uint8_t xyzPosition:1;
        uint8_t noisePower:1;
        uint8_t reserved:2;
        uint8_t electrodeConfiguration:3;
        uint8_t CICData:1;
        uint8_t SDData:1;
        uint16_t reserved2:3;
    } outputConfigMask;
    uint8_t timestamp;
    struct {
        uint8_t positionValid:1;
        uint8_t airWheelValid:1;
        uint8_t rawDataValid:1;
        uint8_t noisePowerValid:1;
        uint8_t environmentalNoise:1;
        uint8_t clipping:1;
        uint8_t reserved:1;
        uint8_t DSPRunning:1;
    } systemInfo;
    uint16_t dspInfo;
    struct {
        uint8_t gestureCode:8;		//	0 -> No Gesture
        uint8_t reserved:4;
        uint8_t gestureType:4; //garbage, flick or circular
        uint8_t edgeFlick:1;
        uint16_t reserved2:14;
        uint8_t gestureInProgress:1; //	If "1" Gesture recognition in progress
    } gestureInfo;
    struct {
        uint8_t touchSouth:1;
        uint8_t touchWest:1;	//:Bit 01
        uint8_t touchNorth:1;	//:Bit 02
        uint8_t touchEast:1;	//:Bit 03
        uint8_t touchCentre:1;	//:Bit 04
        uint8_t tapSouth:1;	//:Bit 05
        uint8_t tapWest:1;	//:Bit 06
        uint8_t tapNorth:1;	//:Bit 07
        uint8_t tapEast	:1;	//:Bit 08
        uint8_t tapCentre:1;	//:Bit 09
        uint8_t doubleTapSouth:1;	//:Bit 10
        uint8_t doubleTapWest:1;	//:Bit 11
        uint8_t doubleTapNorth:1;	//:Bit 12
        uint8_t doubleTapEast:1;	//:Bit 13
        uint8_t doubleTapCentre:1; //:Bit 14
        uint8_t reserved:1; //:Bit 15
        uint8_t touchCounter; //period between the time when the hand starts moving to touch until it is detected
        uint8_t reserved2;
    } touchInfo;
    int8_t	airWheel;
    uint8_t reserved;
    uint16_t  x;
    uint16_t  y;
    uint16_t  z;
    float noisePower;
    float CICData[4]; // uncalibrated sensor data
    float SDData[4]; // signal deviation
  } out;
  struct {
    uint8_t header[3];
    // payload
    uint8_t valid;
    uint8_t hwRev[2];
    uint8_t parameterStartAddr;
    uint8_t loaderVersion[2];
    uint8_t loaderPlatform;
    uint8_t fwStartAddr;
    char fwVersion[120];
  } fw;
  struct{
    uint8_t id;
    uint8_t size;
    uint16_t error;
    uint32_t reserved;
    uint32_t reserved1;
  } status;
} MGC_data;
#pragma pack()

char MGC3130_currentGesture[12];

int8_t MGC3130_delta, MGC3130_lastrotation = 0;
int16_t MGC3130_rotValue, MGC3130_lastSentRotValue = 0;

uint16_t MGC3130_lastSentX, MGC3130_lastSentY, MGC3130_lastSentZ = 0;

uint8_t hwRev[2], loaderVersion[2], loaderPlatform = 0;
char MGC3130_firmwareInfo[20];

uint8_t MGC3130_touchTimeout = 0;
uint16_t MGC3130_touchCounter = 1; // measure how long you touch the surface in loop cycles
uint32_t MGC3130_touchTimeStamp = millis();
bool MGC3130_triggeredByTouch = false;

uint8_t MGC3130_mode = 1; // 1-gesture; 2-airwheel; 3-position


// predefined messages
uint8_t MGC3130autoCal[] = {0x10, 0x00, 0x00, 0xA2, 0x80, 0x00 , 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t MGC3130disableAirwheel[] = {0x10, 0x00, 0x00, 0xA2, 0x90, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
uint8_t MGC3130enableAirwheel[] = {0x10, 0x00, 0x00, 0xA2, 0x90, 0x00 , 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};

void MGC3130_triggerTele(){
    mqtt_data[0] = '\0';
    if (MqttShowSensor()) {
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
    #ifdef USE_RULES
      RulesTeleperiod();  // Allow rule based HA messages
    #endif  // USE_RULES
    }
}

void MGC3130_handleSensorData(){
      if ( MGC_data.out.outputConfigMask.touchInfo && MGC3130_touchTimeout == 0){
        if (MGC3130_handleTouch()){
            MGC3130_triggeredByTouch = true;
            MGC3130_triggerTele();
        }
      }

      if(MGC3130_mode == 1){
        if( MGC_data.out.outputConfigMask.gestureInfo && MGC_data.out.gestureInfo.gestureCode > 0){
          MGC3130_handleGesture();
          MGC3130_triggerTele();
        }
      }
      if(MGC3130_mode == 2){
        if(MGC_data.out.outputConfigMask.airWheelInfo && MGC_data.out.systemInfo.airWheelValid){
          MGC3130_handleAirWheel();
          MGC3130_triggerTele();
        }
      }
      if(MGC3130_mode == 3){
        if(MGC_data.out.systemInfo.positionValid && (MGC_data.out.z > MGC3130_MIN_ZVALUE)){
          MGC3130_triggerTele();
          }
      }
}

void MGC3130_sendMessage(uint8_t data[], uint8_t length){
  	Wire.beginTransmission(MGC3130_I2C_ADDR);
    Wire.write(data,length);
  	Wire.endTransmission();
    delay(2);
    MGC3130_receiveMessage();
}


void MGC3130_handleGesture(){
  //char log[LOGSZ];
  char edge[5];
  if (MGC_data.out.gestureInfo.edgeFlick){
    snprintf_P(edge, sizeof(edge), PSTR("ED_"));
  }
  else{
    snprintf_P(edge, sizeof(edge), PSTR(""));
  }
  switch(MGC_data.out.gestureInfo.gestureCode){
    case MGC3130_GESTURE_GARBAGE:
    //snprintf_P(log, sizeof(log), PSTR("NONE"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("NONE"));
    break;
    case MGC3130_FLICK_WEST_EAST:
    //snprintf_P(log, sizeof(log), PSTR("%sFL_WE"), edge);
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("%sFL_WE"), edge);
    break;
    case MGC3130_FLICK_EAST_WEST:
    //snprintf_P(log, sizeof(log), PSTR("%sFL_EW"), edge);
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("%sFL_EW"), edge);
    break;
    case MGC3130_FLICK_SOUTH_NORTH:
    //snprintf_P(log, sizeof(log), PSTR("%sFL_SN"), edge);
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("%sFL_SN"), edge);
    break;
    case MGC3130_FLICK_NORTH_SOUTH:
    //snprintf_P(log, sizeof(log), PSTR("%sFL_NS"), edge);
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("%sFL_NS"), edge);
    break;
    case MGC3130_CIRCLE_CLOCKWISE:
    //snprintf_P(log, sizeof(log), PSTR("CW"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("CW"));
    break;
    case MGC3130_CIRCLE_CCLOCKWISE:
    //snprintf_P(log, sizeof(log), PSTR("CCW"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("CCW"));
    break;
  }
  //AddLog_P(LOG_LEVEL_DEBUG, log);
}

bool MGC3130_handleTouch(){
  //char log[LOGSZ];
  bool success = false; // if we find a touch of higher order, we are done
    if  (MGC_data.out.touchInfo.doubleTapCentre && !success){
    //snprintf_P(log, sizeof(log), PSTR("DTAP_CENTRE"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("DT_C"));
    MGC3130_touchTimeout = 5;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.doubleTapEast && !success){
    //snprintf_P(log, sizeof(log), PSTR("DTAP_EAST"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("DT_E"));
    MGC3130_touchTimeout = 5;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.doubleTapNorth && !success){
    //snprintf_P(log, sizeof(log), PSTR("DTAP_NORTH"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("DT_N"));
    MGC3130_touchTimeout = 5;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.doubleTapWest && !success){
    //snprintf_P(log, sizeof(log), PSTR("DTAP_WEST"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("DT_W"));
    MGC3130_touchTimeout = 5;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.doubleTapSouth && !success){
    //snprintf_P(log, sizeof(log), PSTR("DTAP_SOUTH"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("DT_S"));
    MGC3130_touchTimeout = 5;
    success = true;
    MGC3130_touchCounter = 1;
    }
    if (MGC_data.out.touchInfo.tapCentre && !success){
    //snprintf_P(log, sizeof(log), PSTR("TAP_CENTRE"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TP_C"));
    MGC3130_touchTimeout = 2;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.tapEast && !success){
    //snprintf_P(log, sizeof(log), PSTR("TAP_EAST"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TP_E"));
    MGC3130_touchTimeout = 2;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.tapNorth && !success){
    //snprintf_P(log, sizeof(log), PSTR("TAP_NORTH"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TP_N"));
    MGC3130_touchTimeout = 2;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.tapWest && !success){
    //snprintf_P(log, sizeof(log), PSTR("TAP_WEST"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TP_W"));
    MGC3130_touchTimeout = 2;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.tapSouth && !success){
    //snprintf_P(log, sizeof(log), PSTR("TAP_SOUTH"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TP_S"));
    MGC3130_touchTimeout = 2;
    success = true;
    MGC3130_touchCounter = 1;
    }
    else if (MGC_data.out.touchInfo.touchCentre && !success){
    //snprintf_P(log, sizeof(log), PSTR("TOUCH_CENTRE"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TH_C"));
    success = true;
    MGC3130_touchCounter++; // This will reset to 0 after touching for approx. 1h and 50 minutes ;)
    }
    else if (MGC_data.out.touchInfo.touchEast && !success){
    //snprintf_P(log, sizeof(log), PSTR("TOUCH_EAST"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TH_E"));
    success = true;
    MGC3130_touchCounter++;
    }
    else if (MGC_data.out.touchInfo.touchNorth && !success){
    //snprintf_P(log, sizeof(log), PSTR("TOUCH_NORTH"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TH_N"));
    success = true;
    MGC3130_touchCounter++;
    }
    else if (MGC_data.out.touchInfo.touchWest && !success){
    //snprintf_P(log, sizeof(log), PSTR("TOUCH_WEST"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TH_W"));
    success = true;
    MGC3130_touchCounter++;
    }
    else if (MGC_data.out.touchInfo.touchSouth && !success){
    //snprintf_P(log, sizeof(log), PSTR("TOUCH_SOUTH"));
    snprintf_P(MGC3130_currentGesture, sizeof(MGC3130_currentGesture), PSTR("TH_S"));
    success = true;
    MGC3130_touchCounter++;
    }
  //AddLog_P(LOG_LEVEL_DEBUG, log);
  return success;
}

void MGC3130_handleAirWheel(){
  MGC3130_delta = MGC_data.out.airWheel - MGC3130_lastrotation;
  MGC3130_lastrotation = MGC_data.out.airWheel;

      MGC3130_rotValue = MGC3130_rotValue + MGC3130_delta;
      if(MGC3130_rotValue < MGC3130_MIN_ROTVALUE){
        MGC3130_rotValue = MGC3130_MIN_ROTVALUE;
      }
      if(MGC3130_rotValue > MGC3130_MAX_ROTVALUE){
        MGC3130_rotValue = MGC3130_MAX_ROTVALUE;
      }
}

void MGC3130_handleSystemStatus(){
  //Serial.println("Got System status");
}

bool MGC3130_receiveMessage(){
  if(MGC3130_readData()){
      switch(MGC_data.out.id){
        case MGC3130_SENSOR_DATA:
          MGC3130_handleSensorData();
          break;
        case MGC3130_SYSTEM_STATUS:
          MGC3130_handleSystemStatus();
          break;
        case MGC3130_FW_VERSION:
          hwRev[0] = MGC_data.fw.hwRev[1];
          hwRev[1] = MGC_data.fw.hwRev[0];
          loaderVersion[0] = MGC_data.fw.loaderVersion[0];
          loaderVersion[1] = MGC_data.fw.loaderVersion[1];
          loaderPlatform = MGC_data.fw.loaderPlatform;
          snprintf_P(MGC3130_firmwareInfo, sizeof(MGC3130_firmwareInfo), PSTR("FW: %s"), MGC_data.fw.fwVersion);
          MGC3130_firmwareInfo[20] = '\0';
          // Serial.print(MGC3130_firmwareInfo);
          break;
      }
    return true;
  }
  return false;
}

bool MGC3130_readData()
{
  bool success = false;
  if (!digitalRead(MGC3130_xfer)){
    pinMode(MGC3130_xfer, OUTPUT);
    digitalWrite(MGC3130_xfer, LOW);
    Wire.requestFrom(MGC3130_I2C_ADDR, (uint16_t)32); // request usual data output

    MGC_data.buffer[0] = 4; // read at least header, but update after first read anyway
    unsigned char i = 0;
    while(Wire.available() && (i < MGC_data.buffer[0])){
      MGC_data.buffer[i] = Wire.read();
      i++;
      }
    digitalWrite(MGC3130_xfer, HIGH);
    pinMode(MGC3130_xfer, INPUT);
    success = true;
  }
  return success;
}

void MGC3130_nextMode(){
  if (MGC3130_mode < 3){
    MGC3130_mode++;
  }
  else{
    MGC3130_mode = 1;
  }
  switch(MGC3130_mode){ // there is more to be done in the future
    case 1:
    MGC3130_sendMessage(MGC3130disableAirwheel,16);
    break;
    case 2:
    MGC3130_sendMessage(MGC3130enableAirwheel,16);
    break;
    case 3:
    MGC3130_sendMessage(MGC3130disableAirwheel,16);
    break;
  }
}

void MGC3130_loop()
{
  if(MGC3130_touchTimeout > 0){
    MGC3130_touchTimeout--;
  }
  MGC3130_receiveMessage();
}


bool MGC3130_detect(void)
{
  if (MGC3130_type){
    return true;
  }

  pinMode(MGC3130_xfer, INPUT_PULLUP);
  pinMode(MGC3130_reset,  OUTPUT);
  digitalWrite(MGC3130_reset, LOW);
  delay(10);
  digitalWrite(MGC3130_reset, HIGH);
  delay(50);

  bool success = false;
  success = MGC3130_receiveMessage(); // This should read the firmware info
  if (success) {
    strcpy_P(MGC3130stype, PSTR("MGC3130"));
    AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, MGC3130stype, MGC3130_I2C_ADDR);
    MGC3130_currentGesture[0] = '\0';
    MGC3130_type = true;
  } else {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MGC3130 did not respond at address 0x%x"), MGC3130_I2C_ADDR);
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void MGC3130_show(bool json)
{
  if (!MGC3130_type) { return; }

  char status_chr[2];
  if (MGC_data.out.systemInfo.DSPRunning) {
    sprintf (status_chr, "1");
  }
  else{
    sprintf (status_chr, "0");
  }

  if (json) {
    if (MGC3130_mode == 3 && !MGC3130_triggeredByTouch) {
      if (MGC_data.out.systemInfo.positionValid && !(MGC_data.out.x == MGC3130_lastSentX && MGC_data.out.y == MGC3130_lastSentY && MGC_data.out.z == MGC3130_lastSentZ)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"X\":%u,\"Y\":%u,\"Z\":%u}"),
          mqtt_data, MGC3130stype, MGC_data.out.x/64, MGC_data.out.y/64, (MGC_data.out.z-(uint16_t)MGC3130_MIN_ZVALUE)/64);
        MGC3130_lastSentX = MGC_data.out.x;
        MGC3130_lastSentY = MGC_data.out.y;
        MGC3130_lastSentZ = MGC_data.out.z;
      }
    }
    MGC3130_triggeredByTouch = false;

    if (MGC3130_mode == 2) {
      if (MGC_data.out.systemInfo.airWheelValid && (MGC3130_rotValue != MGC3130_lastSentRotValue)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"AW\":%i}"), mqtt_data, MGC3130stype, MGC3130_rotValue);
        MGC3130_lastSentRotValue = MGC3130_rotValue;
      }
    }

    if (MGC3130_currentGesture[0] != '\0') {
      if (millis() - MGC3130_touchTimeStamp > 220 ) {
        MGC3130_touchCounter = 1;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"%s\":%u}"), mqtt_data, MGC3130stype, MGC3130_currentGesture, MGC3130_touchCounter);
      MGC3130_currentGesture[0] = '\0';
      MGC3130_touchTimeStamp = millis();
    }
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGC_3130_SNS, mqtt_data, MGC3130stype, status_chr, hwRev[0], hwRev[1], loaderVersion[0], loaderVersion[1], loaderPlatform );
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Command Sensor36
 *
 * Command  | Payload | Description
 * ---------|---------|--------------------------
 * Sensor36 |         | ...
 * Sensor36 | 0       | Next Mode - cycle through the modes
 * Sensor36 | 1       | Gesture Mode
 * Sensor36 | 2       | Airwheel Mode
 * Sensor36 | 3       | Position Mode with x,y,z - z must be higher than half of the max. sensing height
\*********************************************************************************************/

bool MGC3130CommandSensor()
{
  bool serviced = true;

  switch (XdrvMailbox.payload) {
    case 0: // cycle through the modes
      MGC3130_nextMode();
      break;
    case 1: // gesture & touch
      MGC3130_mode = 1;
      MGC3130_sendMessage(MGC3130disableAirwheel,16);
      break;
    case 2: // airwheel & touch
      MGC3130_mode = 2;
      MGC3130_sendMessage(MGC3130enableAirwheel,16);
      break;
    case 3: // position & touch
      MGC3130_mode = 3;
      MGC3130_sendMessage(MGC3130disableAirwheel,16);
    break;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns36(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    if ((FUNC_INIT == function) && (pin[GPIO_MGC3130_XFER] < 99) && (pin[GPIO_MGC3130_RESET] < 99)) {
      MGC3130_detect();
    }
    else if (MGC3130_type) {
      switch (function) {
        case FUNC_EVERY_50_MSECOND:
          MGC3130_loop();
          break;
        case FUNC_COMMAND_SENSOR:
          if (XSNS_36 == XdrvMailbox.index) {
            result = MGC3130CommandSensor();
          }
          break;
        case FUNC_JSON_APPEND:
          MGC3130_show(1);
          break;
#ifdef USE_WEBSERVER
        case FUNC_WEB_APPEND:
          MGC3130_show(0);
          break;
#endif  // USE_WEBSERVER
      }
    }
  }
  return result;
}
#endif  // USE_MGC3130
#endif  // USE_I2C