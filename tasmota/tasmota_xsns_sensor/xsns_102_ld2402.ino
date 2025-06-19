/*
  xsns_102_ld2402.ino - HLK-LD2402 24GHz smart wave motion sensor support for Tasmota

  Copyright (C) 2025  Anthony Sepa

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

  Based in part on xsns_102_ld2410s.ino by:
  SPDX-FileCopyrightText: 2022 Theo Arends, 2024 md5sum-as (https://github.com/md5sum-as)

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_LD2402
/*********************************************************************************************\
 * HLK-LD2402 24GHz smart wave motion sensor
 * 
 * Attention!
 * This module works with HLK-LD2402 devices. 
 * 
 * Available commands:
 * LD2402_Status 0|1|2 - showing parameters|sensor readings|firmware info
 * LD2402_ReRead - reread common, motion and micro threshold parameters from device
 * LD2402_SetCommon 7-100,0-65535 - set common: maximum distance, disappearance delay
 * LD2402_SetMotion n,n1..n16 - set motion threshold values (16)
 * LD2402_SetMicro n,n1..n16 - set micro threshold values (16)
 * LD2402_Mode 0/1 - set device output mode 0-normal, 1-engineering
 * LD2402_Save - save the internal device motion/micro thresholds (fw 3.3.2+)
 * LD2402_AutoUpdate 3.0,2.0,3.0 - start autoupdate trigger,keep,micro magnification factor
 * LD2402_AutoGain - start autogain process (fw 3.3.5+)
 * LD2402_Follow 0/n - reports every n seconds
 *
\*********************************************************************************************/

#define XSNS_102                        102

#define LD2402_NUM_GATES                0x10
#define LD2402_ENG_LINE_LENGTH          0x83
// Max line length is in engineering mode at 141 characters
#define LD2402_BUFFER_SIZE              LD2402_ENG_LINE_LENGTH + 10

// step values which are a combination of the number of bytes for the command arg
// the command and the delay amount +1 before sending the command
// for example: 0x6121, 0x6 byte argument, 0x12 command change mode, 0 ticks delay)
enum LD2402_Steps {
  LD2402_CMND_READ_VERSION=0x0001,          // get version, no arg, no delay
  LD2402_CMND_AUTO_PROGRESS=0x00A9,         // get auto progress, no arg, 8 ticks delay (2 seconds)
  LD2402_CMND_READ_SERIAL=0x0111,           // get serial, no arg, no delay
  LD2402_CMND_AUTO_INTERFERENCE=0x0141,     // get auto interference, no arg, no delay
  LD2402_CMND_AUTO_GAIN=0x0EE1,             // run auto gain, no arg, no delay
  LD2402_CMND_AUTO_GAIN_COMPLETE=0x0F01,    // notify auto gain complete, no arg, no delay (special processing required)
  LD2402_CMND_SAVE_PARAM=0x0FD1,            // save parameters, no arg, no delay (fw 3.3.2+)
  LD2402_CMND_END_CONFIGURATION=0x0FE1,     // end configuration mode, no arg, no delay
  LD2402_CMND_READ_PARAM=0x2081,            // get a parameter, 2-byte arg, no delay
  LD2402_CMND_START_CONFIGURATION=0x2FF1,   // start configuration mode, 2-byte arg, no delay
  LD2402_CMND_WRITE_PARAM=0x6071,           // set a parameter, 6-byte arg, no delay
  LD2402_CMND_AUTO_THRESHOLD=0x6091,        // run auto threshold, 6-byte arg, no delay
  LD2402_CMND_MODE=0x6121,                  // set normal/engineering mode, 6-byte arg, no delay
  LD2402_CMND_Wait_Timeout=0xFFF9           // special timeout counter, never sent, 8 ticks delay (2 seconds)
};

// parameter types
enum LD2402_Parameters {
  LD2402_MAX_DISTANCE=0x01,
  LD2402_DISP_DELAY=0x04,
  LD2402_PWR_INTERFER=0x05,
  LD2402_MOTION_START=0x10,
  LD2402_MOTION_END=LD2402_MOTION_START+LD2402_NUM_GATES-1,
  LD2402_MICRO_START=0x30,
  LD2402_MICRO_END=LD2402_MICRO_START+LD2402_NUM_GATES-1,
  LD2402_DONE
};

// engineering types
enum LD2402_EngTypes {
  LD2402_NO_PERSON,
  LD2402_PERSON_MOVING,
  LD2402_PERSON_STILL,
  LD2402_PERSON_UNKNOWN
};

// quick lookup stubs
enum LD2402_Stubs {
  LD2402_target_Error=0x4572726f,         // Erro
  LD2402_target_OFF=0x4F46460D,           // OFF
  LD2402_target_distance=0x64697374,      // dist
  LD2402_engineering_footer=0xF8F7F6F5,
  LD2402_engineering_header=0xF4F3F2F1,
  LD2402_config_footer=0x04030201,
  LD2402_config_header=0xFDFCFBFA
};

// read buffer state
enum LD2402_State {
  LD2402_DISCONNECTED,          // no response in timeout period, disconnected
  LD2402_ERROR,                 // buffer not used, just received an error message
  LD2402_NORMAL,                // buffer not used, just received a distance/off message
  LD2402_UNKNOWN,               // for when we are unsure what is in the buffer
  LD2402_CONFIGURATION,         // buffer holds configuration data
  LD2402_AUTOUPDATE,            // buffer holds configuration data, running autoupdate
  LD2402_ENGINEERING            // buffer holds engineering data
};

#define LD2402_config_send_stub         {(LD2402_config_header>>24), (LD2402_config_header>>16)&0xFF, (LD2402_config_header>>8)&0xFF, LD2402_config_header&0xFF, 0x00, 0x00, 0x00, 0x00}
#define LD2402_config_footer_a          {(LD2402_config_footer>>24), (LD2402_config_footer>>16)&0xFF, (LD2402_config_footer>>8)&0xFF, LD2402_config_footer&0xFF}

#include <TasmotaSerial.h>
TasmotaSerial *LD2402Serial = nullptr;

#define LD2402_MOTION_ENERGY      LD2402.buffer+3
#define LD2402_MICRO_ENERGY       LD2402_MOTION_ENERGY+64

struct {
  uint8_t *buffer;
  uint8_t cmnd_param[6] = {};   // global cmnd args
  uint16_t sent_cmnd;           // this is the step with the delay part shifted off (2 bytes)
  uint32_t step;                // this is a combination of arg bytes, command and delay (4 bytes)
  uint32_t saved_step;          // saved step to enable configuration mode (4 bytes)
// Static params
  char *version;                // version and serial number are pulled at start to ensure connection
  char *serial_number;
// Detection Parameters
  uint8_t pwr_interf;           // detail power interference information
  uint8_t max_distance;         // maximum detection distanmce
  uint16_t disp_delay;          // delay before an empty area is empty
  uint8_t *motion_threshold;    // allocated on first use, to save memory if not needed
  char *gates;                  // power interference gate information
// Report values
  enum LD2402_EngTypes person;  // engineering line type (0 - no one, 1 - someone, 2 - someone still)
  uint16_t detect_distance;
// autoupdate
  uint8_t auto_upd_progress;
// endianess
  uint8_t endian_offset;
// program flow
#ifdef LD2402_INCLUDE_FOLLOW
  uint32_t follow;
  uint32_t timer;
#endif // LD2402_INCLUDE_FOLLOW
  enum LD2402_State state;
} LD2402;

#define D_LD2402                  "LD2402"
#define D_LD2402_LOG_PREFIX       "LD2: "
#define D_BUSY_MSG                "{\"" D_LD2402 "\":\"Busy, wait till finished current command.\"}"
#define D_DISCONNECTED_MSG        "{\"" D_LD2402 "\":\"Disconnected, reconnect device.\"}"
#define D_COMMAND_PREFIX_JSON     "{\"" D_LD2402 "_%s\":"
#define D_COMMAND_HELP_MSG        D_COMMAND_PREFIX_JSON "\"%s\"}"
/********************************************************************************************/

void Ld2402ToBinary(char *buffer, uint32_t byte_h, uint32_t byte_l) {
  uint32_t mask_01 = 0x01, mask_10 = 0x10;
  for (uint32_t i = 4; i; i--) {
    buffer[i-1] = '0' + (byte_h & mask_10);
    buffer[i+4] = '0' + (byte_h & mask_01);
    buffer[i+9] = '0' + (byte_l & mask_10);
    buffer[i+14] = '0' + (byte_l & mask_01);
    mask_10<<=1;
    mask_01<<=1;
  }
  buffer[4] = buffer[9] = buffer[14] = ' ';
  buffer[19] = 0x0;
}

// arguments: results buffer, current LD2402_Parameters
// updates internal detection parameter
// sets LD2402.step to:
//  READ_PARAM, AUTO_INTERFERENCE, END_CONFIGURATION
// returns next LD2402_Parameters
uint8_t Ld2402ProcessParameter(uint8_t *buffer, uint8_t parameter) {
  LD2402.step = LD2402_CMND_READ_PARAM;       // default is read next parameter
  if (LD2402_MAX_DISTANCE == parameter) {
    LD2402.max_distance = buffer[0];
    return LD2402_DISP_DELAY;
  } else if (LD2402_DISP_DELAY == parameter) {
    LD2402.disp_delay = buffer[1] << 8 | buffer[0];
    return LD2402_PWR_INTERFER;
  } else if (LD2402_PWR_INTERFER == parameter) {
    LD2402.pwr_interf = buffer[0];
    if (2 == LD2402.pwr_interf) {
      LD2402.step = LD2402_CMND_AUTO_INTERFERENCE;  // sidetrack through interference details
    }
    return LD2402_MOTION_START;
  } else {
    if (LD2402_MOTION_END >= parameter) {
      memcpy(LD2402.motion_threshold + (parameter & 0x0F) * 4, buffer, 4);
      if (LD2402_MOTION_END == parameter) {
        return LD2402_MICRO_START;
      }
    } else if (LD2402_MICRO_END >= parameter) {
      memcpy(LD2402.motion_threshold + (parameter & 0x1F) * 4, buffer, 4);
      if (LD2402_MICRO_END == parameter) {
        LD2402.step = LD2402_CMND_END_CONFIGURATION;    // end configuration
      }
    }
  }
  return parameter + 1;
}

void Ld2402HandleConfigData(void) {
  uint32_t rcvd_cmnd = LD2402.buffer[0];
  if (rcvd_cmnd != (LD2402.sent_cmnd&0x0FF)) {
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Command mismatch sent 0x%03X"), LD2402.sent_cmnd);
    return;
  }

  if ((1 == LD2402.buffer[1]) && // is this a response
      (LD2402.buffer[2] || LD2402.buffer[3])) { // did the command fail?
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Command 0x%03X received ACK: 0x%02X%02X"), LD2402.sent_cmnd, LD2402.buffer[3], LD2402.buffer[2]);
    if (LD2402_CMND_END_CONFIGURATION>>4 != LD2402.sent_cmnd) {  // for non end configuration errors try to end configuration
      LD2402.step = LD2402_CMND_END_CONFIGURATION;
    } else {  // we are still receiving messages, might be in bootloader mode
      LD2402.state = LD2402_UNKNOWN;
    }
    return;
  }

  LD2402.step = LD2402_CMND_END_CONFIGURATION;  // default is end configuration
  switch (LD2402.sent_cmnd) {
  case LD2402_CMND_START_CONFIGURATION>>4:      // run before configuration commands
    LD2402.state = LD2402_CONFIGURATION;
    LD2402.step = LD2402.saved_step+1;          // restore saved step to run
    break;

  case LD2402_CMND_END_CONFIGURATION>>4:        // run after configuration commands
    LD2402.state = LD2402_UNKNOWN;
    LD2402.step = 0;
    return;
  
  case LD2402_CMND_READ_VERSION>>4: {           // run on initialization and reconnect
    uint32_t length = LD2402.buffer[4];
    LD2402.version = LD2402.version ? LD2402.version : (char*)malloc(length+1);
    memcpy(LD2402.version, LD2402.buffer + 6, length);
    LD2402.version[length] = 0;
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Rcvd %s"), LD2402.version);
    LD2402.step = LD2402_CMND_READ_SERIAL;      // process read serial
    }break;
  
  case LD2402_CMND_READ_SERIAL>>4: {            // run after read version
    uint32_t length = LD2402.buffer[4];
    LD2402.serial_number = LD2402.serial_number ? LD2402.serial_number : (char*)malloc(length+1);
    memcpy(LD2402.serial_number, LD2402.buffer + 6, length);
    LD2402.serial_number[length] = 0;
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Rcvd %s"), LD2402.serial_number);
    }break;

  // here I am processing the last thing
  case LD2402_CMND_READ_PARAM>>4:               // run from terminal command
    LD2402.cmnd_param[0] = Ld2402ProcessParameter(LD2402.buffer + 4, LD2402.cmnd_param[0]);
    break;

  case LD2402_CMND_AUTO_INTERFERENCE>>4:         // sidetracked here through read param go back to it
    free(LD2402.gates);
    LD2402.gates = (char*)malloc(LD2402_NUM_GATES+3+1);  // number of gates, three spaces, ending /0
    Ld2402ToBinary(LD2402.gates, LD2402.buffer[5], LD2402.buffer[4]);
    LD2402.step = LD2402_CMND_READ_PARAM;       // process next read parameter
    break;
  
  case LD2402_CMND_AUTO_THRESHOLD>>4:           // run from terminal command
    LD2402.state = LD2402_AUTOUPDATE;
    LD2402.step = LD2402_CMND_AUTO_PROGRESS;    // start auto progress reading
    return;                                     // no point running command right away

  case LD2402_CMND_AUTO_PROGRESS>>4:            // run after auto threshold
    LD2402.state = LD2402_AUTOUPDATE;
    LD2402.auto_upd_progress = LD2402.buffer[4];
    if (100 > LD2402.auto_upd_progress) {
      LD2402.step = LD2402_CMND_AUTO_PROGRESS;
      return;                                   // no point running command right away
    }
    LD2402.state = LD2402_CONFIGURATION;
    break;

  case LD2402_CMND_AUTO_GAIN>>4:                // run from terminal command
    LD2402.sent_cmnd = LD2402_CMND_AUTO_GAIN_COMPLETE>>4;  // auto gain complete is just received no command is sent
    LD2402.step = LD2402_CMND_Wait_Timeout;     // pretend as if we sent a command and wait for it
    break;

  // here I am setting up to do the next thing
  case LD2402_CMND_WRITE_PARAM>>4:              // run from terminal command
    if (((LD2402.cmnd_param[0]&0xF0 == LD2402_MICRO_START) || (LD2402.cmnd_param[0]&0xF0 == LD2402_MOTION_START)) &&  // motion/micro threshold
        (LD2402.cmnd_param[0]&0x0F != (LD2402_NUM_GATES-1))) {                                                        // but not the last one
      Ld2402LoadLastParam(LD2402.cmnd_param[0]+1);
      LD2402.step = LD2402_CMND_WRITE_PARAM;  // process next write parameter
    } else if (LD2402_MAX_DISTANCE == LD2402.cmnd_param[0]) {
      Ld2402LoadLastParam(LD2402_DISP_DELAY);
      LD2402.step = LD2402_CMND_WRITE_PARAM;    // process next write parameter
    }
    break;
  }
  Ld2402Every250MSecond();  // immediately send the next command queued
}

// prcess uart messages from the module
// first section finds the start of a line, for simple text lines they are processed immediately
// second section processes command responses and engineering data
void Ld2402Input(void) {
  typedef union { // put in function so it doesn't interfere with anything else
    uint8_t buffer[4];
    uint32_t buffer_32;
  } uint32_u;
  static uint32_t need_counter = 5;
  static uint32_u stub;
  uint32_t avail = LD2402Serial->available();
  // quickly exit if we don't have what we need
  if (avail < need_counter) { return; }

  // load stub only when we are start finding a new line
  if (5 == need_counter) {
    for (uint32_t i = 4; i; i--) {
      stub.buffer_32<<=8;
      stub.buffer[LD2402.endian_offset] = LD2402Serial->read();
      avail--;
    }
    need_counter = 1;
  }

  // first section
  uint32_t test_m = millis();
  while (1 == need_counter) {  // header processing is done when we no longer need just one byte
    yield();  // WDT needs to be yielded to
    switch (stub.buffer_32) {
    default:  // if there are no matches power through the available bytes
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Available: %d, Header: %*_H, Yield: %d"), avail, 4, stub.buffer, millis() - test_m);
      if (!avail--) {
        return;
      }
      stub.buffer_32<<=8;
      stub.buffer[LD2402.endian_offset] = LD2402Serial->read();
      continue;

    case LD2402_engineering_footer:
    case LD2402_config_footer:
      goto finished_line;
    
    case LD2402_target_OFF:
      LD2402.detect_distance = 0;
      LD2402.person = LD2402_NO_PERSON;
      LD2402Serial->read();
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "OFF"));
      LD2402.state = LD2402_NORMAL;
      goto finished_line;
    
    case LD2402_target_Error:
      need_counter = LD2402Serial->readBytesUntil(0x0A, LD2402.buffer, LD2402_BUFFER_SIZE);
      LD2402.buffer[need_counter] = 0;
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Erro%s"), LD2402.buffer);
      LD2402.state = LD2402_ERROR;
      goto finished_line;

    case LD2402_target_distance:
      need_counter = LD2402Serial->readBytesUntil(0x0A, LD2402.buffer, LD2402_BUFFER_SIZE);
      LD2402.buffer[need_counter] = 0;
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Dist%s"), LD2402.buffer);
      // ance:105\r\n
      LD2402.detect_distance = atoi((char*) LD2402.buffer + 5);
      LD2402.person = LD2402_PERSON_STILL;
      LD2402.state = LD2402_NORMAL;
      goto finished_line;

    case LD2402_engineering_header:
    case LD2402_config_header:
      need_counter = 5 + LD2402Serial->read(); // unused length byte + length + footer
      if (need_counter > LD2402_ENG_LINE_LENGTH+5) {
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Line length %d too long for buffer"), need_counter - 5);
        goto finished_line;
      }
      break;
    }
  }

  // make sure we have everything we need waiting for us
  // becuase the need counter will always be greater than
  // 5 bytes we can return from here and will come back
  // when we have all that we need
  if (LD2402Serial->available() < need_counter) { return; }
  need_counter -= 5;

  // second section
  LD2402Serial->read();                             // second byte of length, unused
  LD2402Serial->readBytes(LD2402.buffer, need_counter);  // main information
  LD2402Serial->readBytes(stub.buffer, 4);          // footer validation
  LD2402.state = LD2402_UNKNOWN;                    // buffer used for motion energies has been overwritten
  if (LD2402_ENG_LINE_LENGTH == need_counter) {     // only engineering lines are this long
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Eng %*_H"), need_counter, LD2402.buffer);
    //  reverse order, last digit is most likely to be "bad"
    if (((uint8_t)(LD2402_engineering_footer&0xFF) == stub.buffer[3]) && ((uint8_t)(LD2402_engineering_footer>>8&0xFF) == stub.buffer[2]) &&
        ((uint8_t)(LD2402_engineering_footer>>16&0xFF) == stub.buffer[1]) && ((uint8_t)(LD2402_engineering_footer>>24&0xFF) == stub.buffer[0])) {
      LD2402.state = LD2402_ENGINEERING;
      LD2402.detect_distance = LD2402.buffer[2] << 8 | LD2402.buffer[1];
      LD2402.person = (enum LD2402_EngTypes)LD2402.buffer[0];
    } else {
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "avail: %d, length %d"), avail, need_counter);
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Foot %*_H, 0x%08X"), 4, stub.buffer, stub.buffer_32);
    }
  } else {
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Ret %*_H"), need_counter, LD2402.buffer);
    //  reverse order, last digit is most likely to be "bad"
    if (((uint8_t)(LD2402_config_footer&0xFF) == stub.buffer[3]) && ((uint8_t)(LD2402_config_footer>>8&0xFF) == stub.buffer[2]) &&
        ((uint8_t)(LD2402_config_footer>>16&0xFF) == stub.buffer[1]) && ((uint8_t)(LD2402_config_footer>>24&0xFF) == stub.buffer[0])) {
      LD2402.state = LD2402_CONFIGURATION;
      Ld2402HandleConfigData();
    } else {
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "avail: %d, length %d"), avail, need_counter);
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Foot %*_H, 0x%08X"), 4, stub.buffer, stub.buffer_32);
    }
  }
// finished processing current line
finished_line:
  need_counter = 5;
}

void Ld2402LoadLastParam(uint32_t curr_param) {
  memset(LD2402.cmnd_param + 2, 0x00, 4);
  LD2402.cmnd_param[0] = curr_param;
  if (LD2402_MAX_DISTANCE == curr_param) {
    LD2402.cmnd_param[2] = LD2402.max_distance;
  } else if (LD2402_DISP_DELAY == curr_param) {
    LD2402.cmnd_param[2] = LD2402.disp_delay & 0x00FF;
    LD2402.cmnd_param[3] = LD2402.disp_delay >> 8;
  } else if (LD2402_MOTION_END >= curr_param) {
    memcpy(LD2402.cmnd_param + 2, LD2402.motion_threshold + (curr_param & 0x0F) * 4, 4);
  } else if (LD2402_MICRO_END >= curr_param) {
    memcpy(LD2402.cmnd_param + 2, LD2402.motion_threshold + (curr_param & 0x1F) * 4, 4);
  }
}

void Ld2402ResponseAppendGates(uint8_t *energies) {
  if (!energies) { return; }
  uint32_t i = 0;
  float val;
  while (i < LD2402_NUM_GATES * 4) {
    val = energies[i++] | energies[i++] << 8 | energies[i++] << 16 | energies[i++] << 24;
    val = (val ? 10.0f / 2.302585f * logf(val) : 0);  // 10.0f / logf(10) * logf(val)
    if (4 != i) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("%2_f"), &val);
  }
}

void Ld2402ResponseAppendReport() {
  if (LD2402_AUTOUPDATE == LD2402.state) {
    ResponseAppend_P(PSTR("\"AutoUpdate\":\"%1d%%\""), LD2402.auto_upd_progress);
  } else if (LD2402_DISCONNECTED == LD2402.state) {
    ResponseAppend_P(PSTR("\"Error\":\"Disconnected\""));
  } else {
    ResponseAppend_P(PSTR("\"" D_JSON_DISTANCE "\":%d"), LD2402.detect_distance);
    if (LD2402_ENGINEERING == LD2402.state) {
      if (LD2402.gates) { ResponseAppend_P(PSTR(",\"InterferenceGates\":\"%s\""), LD2402.gates); }
      ResponseAppend_P(PSTR(",\"Motion\":%s,\"Occupancy\":%s,\"PowerInterference\":%d,\"MotionEnergies\":["), (LD2402_PERSON_MOVING == LD2402.person ? "true" : "false"), (LD2402.person ? "true" : "false"), LD2402.pwr_interf);
      Ld2402ResponseAppendGates(LD2402_MOTION_ENERGY);
      ResponseAppend_P(PSTR("],\"MicroEnergies\":["));
      Ld2402ResponseAppendGates(LD2402_MICRO_ENERGY);
      ResponseAppend_P(PSTR("]"));
    }
  }
}

/********************************************************************************************/

// this gets run from the scheduler and from the command processing after the uart
// processing main loop, in either case returning from here will release back to the main
// scheduler
void Ld2402Every250MSecond(void) {
  if (LD2402.step) {
    LD2402.step--;
    if ((LD2402_CMND_Wait_Timeout&0xFFF0) == LD2402.step) {  // Timeout countdown reaches zero
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Device disconnected"));
      LD2402.state = LD2402_DISCONNECTED;
    } else if (!(LD2402.step&0x00F)) {  // Command reaches zero
      // preprocess - ensure module is in configuration mode except for end configuration
      if ((LD2402_CMND_END_CONFIGURATION&0x00F != LD2402.step) && (LD2402_CONFIGURATION != LD2402.state)) {
        LD2402.saved_step = LD2402.step;
        LD2402.sent_cmnd = LD2402_CMND_START_CONFIGURATION>>4;
      } else {
        LD2402.sent_cmnd = LD2402.step>>4;
      }

      // send configuration command
      uint32_t val_len = LD2402.sent_cmnd>>8;
      uint8_t stub[8] = LD2402_config_send_stub;
      stub[4] = val_len + 2;
      stub[6] = LD2402.sent_cmnd&0x0FF;
      LD2402Serial->flush();
      LD2402Serial->write(stub, 8);
      if ((LD2402_CMND_START_CONFIGURATION>>4) == LD2402.sent_cmnd) {  // special override if we are setting configuration mode
        stub[4] = 0x01;  // reuse already sent stub buffer
        LD2402Serial->write(stub+4, 2);
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Send 0x%03X, 0100"), LD2402.sent_cmnd);
      } else if (val_len) {  // send configuration command argument
        LD2402Serial->write(LD2402.cmnd_param, val_len);
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Send 0x%03X, %*_H"), LD2402.sent_cmnd, val_len, LD2402.cmnd_param);
      } else {
        DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Send 0x%03X"), LD2402.sent_cmnd);
      }
      const uint8_t footer[4] = LD2402_config_footer_a;
      LD2402Serial->write(footer, sizeof(footer));
    
      LD2402.step = LD2402_CMND_Wait_Timeout;
    }
  }
}

void Ld2402EverySecond(void) {
  static enum LD2402_EngTypes person = LD2402_PERSON_UNKNOWN;
  if (!LD2402.state || !LD2402.version || !LD2402.serial_number) {
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Trying to connect."));
    LD2402.step = LD2402_CMND_READ_VERSION;
    return;
  }

#ifdef LD2402_INCLUDE_FOLLOW
  // if person has not changed and timer is not set, return
  if (!LD2402.timer && (LD2402.person == person)) { return; }
  // if timer is set, reduce timer
  if (LD2402.timer) { --LD2402.timer; }
  // if person has not changed and timer is still set, return
  if (LD2402.timer && (LD2402.person == person)) { return; }
  // if person has changed, save new person
  if (LD2402.person != person) { person = LD2402.person; }
  // if timer is not set, set to follow
  if (!LD2402.timer) { LD2402.timer = LD2402.follow; }
#else
  // if person has not changed, return
  if (LD2402.person == person) { return; }
  person = LD2402.person;
#endif // LD2402_INCLUDE_FOLLOW

  MqttPublishSensor();
}

void Ld2402Detect(void) {
  if (PinUsed(GPIO_LD2402_RX) && PinUsed(GPIO_LD2402_TX)) {
    LD2402.buffer = (uint8_t*)malloc(LD2402_ENG_LINE_LENGTH);
    DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Buff size %d"), LD2402_BUFFER_SIZE);
    if (!LD2402.buffer) { DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "No buff")); return; }
    LD2402Serial = new TasmotaSerial(Pin(GPIO_LD2402_RX), Pin(GPIO_LD2402_TX), 2, 0, LD2402_BUFFER_SIZE);
    if (LD2402Serial->begin(115200)) {
      if (LD2402Serial->hardwareSerial()) { ClaimSerial(); }
      LD2402Serial->setTimeout(200);
#ifdef ESP32
      DEBUG_SENSOR_LOG(PSTR(D_LD2402_LOG_PREFIX "Serial UART%d"), LD2402Serial->getUart());
#endif
      volatile uint32_t i=0x01234567;
      LD2402.endian_offset = ((*((uint8_t*)(&i))) != 0x67)*3;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMD_HELP                  "Help"
#define D_CMD_FOLLOW                "Follow"
#define D_CMD_MODE                  "Mode"
#define D_CMD_AUTOUPDATE            "AutoUpdate"
#define D_CMD_AUTOGAIN              "AutoGain"
#define D_CMD_STATUS                "Status"
#define D_CMD_SETCOMMON             "SetCommon"
#define D_CMD_SETMOTION             "SetMotion"
#define D_CMD_SETMICRO              "SetMicro"
#define D_CMD_SAVE                  "Save"
#define D_CMD_REREAD                "ReRead"

const char kLd2402Commands[] PROGMEM = D_LD2402 "_|"  // Prefix
#ifdef LD2402_INCLUDE_HELP
  D_CMD_HELP "|"
#endif // LD2402_INCLUDE_HELP
#ifdef LD2402_INCLUDE_FOLLOW
  D_CMD_FOLLOW "|"
#endif // LD2402_INCLUDE_FOLLOW
  D_CMD_MODE "|" D_CMD_AUTOUPDATE "|" D_CMD_AUTOGAIN "|" D_CMD_STATUS "|" D_CMD_SETCOMMON "|"
  D_CMD_SETMOTION "|" D_CMD_SETMICRO "|" D_CMD_SAVE "|" D_CMD_REREAD;

void (* const Ld2402Command[])(void) PROGMEM = {
#ifdef LD2402_INCLUDE_HELP
  &CmndLd2402Help, 
#endif // LD2402_INCLUDE_HELP
#ifdef LD2402_INCLUDE_FOLLOW
  &CmndLd2402Follow,
#endif // LD2402_INCLUDE_FOLLOW
  &CmndLd2402Mode, &CmndLd2402AutoUpdate, &CmndLd2402AutoGain, &CmndLd2402Status, &CmndLd2402Common,
  &CmndLd2402Motion, &CmndLd2402Micro, &CmndLd2402Save, &CmndLd2402ReRead };

#ifdef LD2402_INCLUDE_HELP
void CmndLd2402Help(void) {
  Response_P(PSTR(D_COMMAND_PREFIX_JSON"\"%s"), D_CMD_HELP, "Available commands: ");
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_MODE", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_AUTOUPDATE", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_AUTOGAIN" (fw 3.3.5+), "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_STATUS", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SETCOMMON", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SETMOTION", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SETMICRO", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_SAVE" (fw 3.3.2+), "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_HELP", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_REREAD", "));
  ResponseAppend_P(PSTR(D_LD2402 "_" D_CMD_FOLLOW"\"}"));
}
#endif // LD2402_INCLUDE_HELP

#ifdef LD2402_INCLUDE_FOLLOW
void CmndLd2402Follow(void) {
  if (ArgC() != 1) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_FOLLOW, "Args: 0|n (MQTT update normal|MQTT update every n second(s))");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  char Argument[XdrvMailbox.data_len];
  ArgV(Argument,1);
  LD2402.timer = atoi(Argument);
  LD2402.follow = LD2402.timer;
  Response_P(PSTR(D_COMMAND_PREFIX_JSON"%d}"), D_CMD_FOLLOW, LD2402.follow);
}
#endif // LD2402_INCLUDE_FOLLOW

void CmndLd2402Status(void) {
  if (LD2402_DISCONNECTED == LD2402.state) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_DISCONNECTED_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC() != 1) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_STATUS, "Args: 0|1|2 (parameters|sensor|firmware)");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  uint32_t status_type = 0;
  if (ArgC() == 1) {
    char Argument[XdrvMailbox.data_len];
    ArgV(Argument,1);
    status_type = atoi(Argument);
  }
  
  Response_P(PSTR("{\"" D_LD2402));
  if (1 == status_type) {
    ResponseAppend_P(PSTR("SNS\":{"));
    Ld2402ResponseAppendReport();
    ResponseJsonEnd();
  } else if (2 == status_type) {
    if (LD2402.version && LD2402.serial_number) {
      ResponseAppend_P(PSTR("FWR\":{\"Version\":\"%s\",\"SerialNumber\":\"%s\"}"), LD2402.version, LD2402.serial_number);
    }
  } else {
    ResponseAppend_P(PSTR("\":{\"MaximumDistance\":%d,\"DisappearenceDelay\":%d,\"MotionThresholds\":["), LD2402.max_distance, LD2402.disp_delay);
    Ld2402ResponseAppendGates(LD2402.motion_threshold);
    ResponseAppend_P(PSTR("],\"MicroThresholds\":["));
    Ld2402ResponseAppendGates(LD2402.motion_threshold ? LD2402.motion_threshold + LD2402_NUM_GATES * 4 : LD2402.motion_threshold);
    ResponseAppend_P(PSTR("]}"));
  }
  ResponseJsonEnd();
}

void CmndLd2402AutoGain(void) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_BUSY_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC()) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOGAIN, "No Args: Starts the modules's automatic gain adjustment");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  LD2402.step = LD2402_CMND_AUTO_GAIN;
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOGAIN, "Automatic gain ...");
}

void CmndLd2402Common(void) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_BUSY_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC() != 2) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SETCOMMON, "Args: MaximumDistance,DisappearanceDelay (decimeter,seconds)");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  uint32_t param[2] = { 0 };
  ParseParameters(2, param);
  if (param[0]>100) {param[0] = 100;}
  if (param[0]<7) {param[0] = 7;}
  if (param[1]<0) {param[1] = 0;}
  if (param[1]>65535) {param[1] = 65535;}
  LD2402.max_distance = param[0];
  LD2402.disp_delay = param[1];
  Ld2402LoadLastParam(LD2402_MAX_DISTANCE);
  LD2402.step = LD2402_CMND_WRITE_PARAM;
  CmndLd2402Status();
}

void CmndLd2402Mode(void) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_BUSY_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC() != 1) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_MODE, "Args: 0|1 (normal|engineering)");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  char Argument[XdrvMailbox.data_len];
  ArgV(Argument,1);
  memset(LD2402.cmnd_param, 0x00, 6);
  LD2402.cmnd_param[2] = atoi(Argument) ? 0x04 : 0x64;
  Response_P(PSTR(D_COMMAND_PREFIX_JSON "\"%s\"}"), D_CMD_MODE, (0x04 == LD2402.cmnd_param[2] ? "Engineering" : "Normal"));
  LD2402.step = LD2402_CMND_MODE;
}

void CmndLd2402AutoUpdate(void) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_BUSY_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC() != 3) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOUPDATE, "Args: trigger,keep,micro (magnification factor, 1.0 to 20.0)");
#endif // LD2402_INCLUDE_HELP
    return;
  }

  char strbuf[24];
  for (uint32_t i = 3; i; i--) {
    ArgV(strbuf, i);
    float param = CharToFloat(strbuf);
    LD2402.cmnd_param[(i-1)*2] = (param < 1.0f) ? 10.0f : (param > 20.0f ? 200.0f : param * 10.0f);
  }
  LD2402.cmnd_param[3] = LD2402.cmnd_param[5] = 0x00;

  LD2402.step = LD2402_CMND_AUTO_THRESHOLD;
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_AUTOUPDATE, "Updating ...");
}

void CmndLd2402Save(void) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_BUSY_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC()) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SAVE, "No Args: Saves configuration parameters in case of power failure (v3.3.2 and above)");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  LD2402.step = LD2402_CMND_SAVE_PARAM;
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SAVE, "Saving ...");
}

uint32_t Ld2402AllocThresholds(void) {
  LD2402.motion_threshold = LD2402.motion_threshold ? LD2402.motion_threshold : (uint8_t*)malloc(2*LD2402_NUM_GATES*4);
  if (!LD2402.motion_threshold) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SETMOTION "/" D_CMD_SETMICRO, "No buffer: Can't allocate threshold buffers");
#endif // LD2402_INCLUDE_HELP
    return 0;
  }
  return 1;
}

void CmndLd2402ReRead(void) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_BUSY_MSG));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC()) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_REREAD, "No Args: Retrieves common and threshold data from device");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (!Ld2402AllocThresholds()) { return; }
  LD2402.cmnd_param[0] = LD2402_MAX_DISTANCE;
  LD2402.step = LD2402_CMND_READ_PARAM;
  Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_REREAD, "Retrieving ... wait one second for it to complete");
}

void Ld2402WriteThresholds(uint8_t *thresholds, uint32_t cmnd_param) {
  if (LD2402.step) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR("\" D_BUSY_MSG \"}"));
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (ArgC() != 16) {
#ifdef LD2402_INCLUDE_HELP
    Response_P(PSTR(D_COMMAND_HELP_MSG), D_CMD_SETMOTION "/" D_CMD_SETMICRO, "No Args: Saves configuration parameters in case of power failure (v3.3.2 and above)");
#endif // LD2402_INCLUDE_HELP
    return;
  }
  if (!Ld2402AllocThresholds()) { return; }
  char strbuf[24];
  float param;
  uint32_t i = 0, val;
  for (uint32_t j = 0; j < LD2402_NUM_GATES; j++) {
    ArgV(strbuf, j+1);
    param = CharToFloat(strbuf) / 10.00f;
    val = exp10(param > 9.500f ? 9.500f : param);
    thresholds[i++] = val & 0x000000FF;
    thresholds[i++] = val >> 8 & 0x000000FF;
    thresholds[i++] = val >> 16 & 0x000000FF;
    thresholds[i++] = val >> 24;
  }
  Ld2402LoadLastParam(cmnd_param);
  LD2402.step = LD2402_CMND_WRITE_PARAM;
  Response_P(PSTR(D_COMMAND_HELP_MSG),  D_CMD_SETMOTION "/" D_CMD_SETMICRO, "Saving ...");
}

void CmndLd2402Motion(void) {
  Ld2402WriteThresholds(LD2402.motion_threshold, LD2402_MOTION_START);
}

void CmndLd2402Micro(void) {
  Ld2402WriteThresholds(LD2402.motion_threshold + LD2402_NUM_GATES * 4, LD2402_MICRO_START);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_LD2402_DIS[] PROGMEM =
  "{s}" D_LD2402 " " D_DISCONNECTED "{e}";
const char HTTP_SNS_LD2402_ERR[] PROGMEM =
  "{s}" D_LD2402 " " D_ERROR "{e}";
const char HTTP_SNS_LD2402_INI[] PROGMEM =
  "{s}" D_LD2402 " " D_INITIALIZED "{e}";
const char HTTP_SNS_LD2402_CM[] PROGMEM =
  "{s}" D_LD2402 " " D_DISTANCE "{m}%d " D_UNIT_CENTIMETER "{e}";
const char HTTP_SNS_LD2402_MV[] PROGMEM =
  "{s}" D_LD2402 " " D_DISTANCE " (!){m}%d " D_UNIT_CENTIMETER "{e}";
const char HTTP_SNS_LD2402_UPD[] PROGMEM =
  "{s}" D_LD2402 " " D_AUTO_UPDATE "{m}%d " D_UNIT_PERCENT "{e}";

void Ld2402Web(void) {
  if (LD2402_AUTOUPDATE == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_UPD, LD2402.auto_upd_progress);
  } else if (LD2402_NORMAL == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_CM, LD2402.detect_distance);
  } else if (LD2402_ENGINEERING == LD2402.state) {
    WSContentSend_PD((LD2402_PERSON_MOVING == LD2402.person ? HTTP_SNS_LD2402_MV : HTTP_SNS_LD2402_CM), LD2402.detect_distance);
  } else if (LD2402_ERROR == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_ERR);
  } else if (LD2402_DISCONNECTED == LD2402.state) {
    WSContentSend_PD(HTTP_SNS_LD2402_DIS);
  } else {
    WSContentSend_PD(HTTP_SNS_LD2402_INI);
  }
}
#endif

void Ld2402Show(void) {
  ResponseAppend_P(PSTR(",\"" D_LD2402 "\":{"));
  Ld2402ResponseAppendReport();
  ResponseJsonEnd();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns102(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    Ld2402Detect();
  }
  else if (LD2402Serial) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        Ld2402Input();
        break;
      case FUNC_EVERY_250_MSECOND:
        Ld2402Every250MSecond();
        break;
      case FUNC_EVERY_SECOND:
        Ld2402EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ld2402Show();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ld2402Web();
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kLd2402Commands, Ld2402Command);
        break;
    }
  }
  return result;
}

#endif  // USE_LD2402
