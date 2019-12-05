/*
  xdrv_23_crash_recorder.ino - record a complete stacktrace in Flash in case of crash

  Copyright (C) 2019  Stephan Hadinger, Theo Arends, 

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

#ifdef USE_CRASH_RECORDER

#define XDRV_32             32

const char kCrashRecorderCommands[] PROGMEM = "|" D_CMND_CRASHRECORD "|" D_CMND_CRASHDUMP "|" D_CMND_CRASH; // No prefix

void (* const CrashRecorderCommand[])(void) PROGMEM = { &CmndCrashRecord, &CmndCrashDump, &CmndCrash };

const uint32_t crash_bank = SETTINGS_LOCATION - CFG_ROTATES - 1;
const uint32_t crash_addr = crash_bank * SPI_FLASH_SEC_SIZE;
const uint64_t crash_sig   = 0xDEADBEEFCCAA5588L;     // arbitrary signature to check if crash was recorded
const uint64_t crash_empty = 0xFFFFFFFFFFFFFFFFL;     // all ones means the flash was correctly erased
const uint32_t dump_max_len = 1024;                   // dump only 1024 bytes of stack, i.e. 256 addresses

static bool stacktrace_armed = false;        // should we record the stacktrace

typedef struct CrashRecorder_t {
  uint64_t crash_signature = crash_sig;
  uint32_t epc1;
  uint32_t stack_start;
  uint32_t stack_end;
  uint32_t stack_dump_len;
  uint32_t stack[];
} CrashRecorder_t;

// See: https://github.com/esp8266/Arduino/blob/master/cores/esp8266/core_esp8266_postmortem.cpp

/**
 * Save crash information in Flash
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 */
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end ) {
  if (!stacktrace_armed) { return; }    // exit if nothing to do

  CrashRecorder_t crash_recorder;

  crash_recorder.epc1             = rst_info->epc1;
  crash_recorder.stack_start      = stack;
  crash_recorder.stack_end        = stack_end;

  uint64_t sig = 0;
  ESP.flashRead(crash_addr, (uint32_t*) &sig, sizeof(sig));

  if (crash_sig == sig) {
    return;                 // we already have a crash recorded, we leave it untouched and quit
  }
  if (crash_empty != sig) {
    // crash recorder zone is unknown content, we leave it untouched
    return;
  }
  // Flash has been erased, so we're good

  // now store the stack trace, limited to 3KB (which should be far enough)
  crash_recorder.stack_dump_len = stack - stack_end;
  if (crash_recorder.stack_dump_len > dump_max_len) { crash_recorder.stack_dump_len = dump_max_len; }

  ESP.flashWrite(crash_addr, (uint32_t*) &crash_recorder, sizeof(crash_recorder));
  ESP.flashWrite(crash_addr + sizeof(crash_recorder), (uint32_t*) stack, crash_recorder.stack_dump_len);
}

/*********************************************************************************************\
 * CmndCrashRecord - arm the crash recorder until next reboot
\*********************************************************************************************/

// Input:
//  0 : disable crash recorcer
//  1 : soft enable, enable if there is not already a crash record present
//  2 : hard enable, erase any previous crash record and enable new record
//
// Output:
//  0 : Ok, disabled
//  1 : Ok, enabled
//  2 : Ok, enabled and previous record erased
// -1 : Abort, crash record already present
// -2 : Flash erase failed
int32_t SetCrashRecorder(int32_t mode) {
  int32_t ret = 0;

  if (0 != mode) {
    uint64_t sig = 0;
    ESP.flashRead(crash_addr, (uint32_t*) &sig, sizeof(sig));

    if ((crash_sig == sig) && (1 == mode)) {
      ret = -1;
    } else {
      if (crash_empty != sig) {
        // crash recorder zone is not clean
        if (ESP.flashEraseSector(crash_bank)) {
          ret = 2;
        } else {
          ret = -2;
        }
      } else {
        ret = 1;
      }
    }
  }

  stacktrace_armed = (ret > 0) ? true : false;
  return ret;
}

// Generate a crash to test the crash record
void CmndCrash(void)
{
  if (1 == XdrvMailbox.payload) {
    volatile uint32_t dummy;
    dummy = *((uint32_t*) 0x00000000);                // invalid address
  } else {
    ResponseCmndChar_P(PSTR(D_JSON_ONE_TO_CRASH));
  }
}


void CmndCrashRecord(void)
{
  int32_t mode, ret;

  switch (XdrvMailbox.payload) {
    case -99:
      mode = 1;
      break;
    case 1:
      mode = 2;
      break;
    default:
      mode = 0;
  }

  ret = SetCrashRecorder(mode);
  const char *msg;

  switch (ret) {
    case 0:
      msg = PSTR(D_JSON_CRASH_RECORD " " D_DISABLED);
      break;
    case 1:
      msg = PSTR(D_JSON_CRASH_RECORD " " D_ENABLED);
      break;
    case 2:
      msg = PSTR(D_JSON_CRASH_RECORD_ERASED ", " D_ENABLED);
      break;
    case -1:
      msg = PSTR(D_JSON_ABORTED ": " D_JSON_CRASH_RECORD_PRESENT);
      break;
    case -2:
      msg = PSTR(D_JSON_ERROR ": " D_JSON_ERR_ERASE_SECTOR);
      break;
    default:
      msg = PSTR(D_JSON_UNKNOWN);
  }
  ResponseCmndChar_P(msg);
}

/*********************************************************************************************\
 * CmndCrashDump - dump the crash history
\*********************************************************************************************/

void CmndCrashDump(void)
{
  CrashRecorder_t dump;
  const char *msg;

  ESP.flashRead(crash_addr, (uint32_t*) &dump, sizeof(dump));
  if (crash_sig == dump.crash_signature) {
    // Response_P(PSTR("{\"reason\":%d,\"exccause\":%d,"
    //                 "\"epc1\":\"0x%08x\",\"epc2\":\"0x%08x\",\"epc3\":\"0x%08x\","
    //                 "\"excvaddr\":\"0x%08x\",\"depc\":\"0x%08x\","
    //                 "\"stack_start\":\"0x%08x\",\"stack_end\":\"0x%08x\","
    //                 "\"uptime\":%d"
    //                 "}"),
    //                 dump.info.reason, dump.info.exccause,
    //                 dump.info.epc1, dump.info.epc2, dump.info.epc3,
    //                 dump.info.excvaddr, dump.info.depc,
    //                 dump.stack_start, dump.stack_end,
    //                 dump.crash_date / 1000
    //                 );
    // MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);

    uint32_t stack_len = dump.stack_dump_len <= dump_max_len ? dump.stack_dump_len : dump_max_len; // we will limit to 1k
    uint32_t dump_stack[stack_len / 4];

    ESP.flashRead(crash_addr + sizeof(CrashRecorder_t), dump_stack, stack_len);

    uint32_t dumped = 0;
    Response_P(PSTR("{\"epc1\":\"0x%08x\",\"call_chain\":\""), dump.epc1);
    for (uint32_t i = 0; i < stack_len / 4; i++) {
      uint32_t value = dump_stack[i];
      if ((value >= 0x40000000) && (value < 0x40300000)) {
        ResponseAppend_P(PSTR("%08x "), value);
        dumped++;
        if (dumped >= 64) { break; }
      }
    }
    ResponseAppend_P(PSTR("\"}"));
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);
    XdrvRulesProcess();
    msg = PSTR(D_OK);
  } else {
    msg = PSTR(D_JSON_NO_DUMP);
  }
  ResponseCmndChar_P(msg);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv32(uint8_t function)
{
  if (FUNC_COMMAND == function) {
    return DecodeCommand(kCrashRecorderCommands, CrashRecorderCommand);
  } else {
    return false;
  }
}

#endif // USE_CRASH_RECORDER
