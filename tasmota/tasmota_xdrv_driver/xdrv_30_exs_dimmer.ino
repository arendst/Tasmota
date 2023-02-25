/*
  xdrv_30_exs_dimmer.ino - ex-store dimmer support for Tasmota

  Copyright (C) 2021  Andreas Schultz

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

#ifdef USE_LIGHT
#ifdef USE_EXS_DIMMER
/*********************************************************************************************\
 * EX-Store WiFi Dimmer V4
 * https://ex-store.de/2-Kanal-RS232-WiFi-WLan-Dimmer-Modul-V4-fuer-Unterputzmontage-230V-3A
 * https://ex-store.de/2-Kanal-RS232-WiFi-WLan-Dimmer-Modul-V4-fuer-Unterputzmontage-230V-3A-ESP8266-V12-Stift-und-Buchsenleisten
\*********************************************************************************************/
//#define EXS_DEBUG

#define XDRV_30 30

#define EXS_GATE_1_ON 0x20
#define EXS_GATE_1_OFF 0x21
#define EXS_DIMM_1_ON 0x22
#define EXS_DIMM_1_OFF 0x23
#define EXS_DIMM_1_TBL 0x24
#define EXS_DIMM_1_VAL 0x25
#define EXS_GATE_2_ON 0x30
#define EXS_GATE_2_OFF 0x31
#define EXS_DIMM_2_ON 0x32
#define EXS_DIMM_2_OFF 0x33
#define EXS_DIMM_2_TBL 0x34
#define EXS_DIMM_2_VAL 0x35
#define EXS_GATES_ON 0x40
#define EXS_GATES_OFF 0x41
#define EXS_DIMMS_ON 0x50
#define EXS_DIMMS_OFF 0x51
#define EXS_CH_LOCK 0x60
#define EXS_GET_VALUES 0xFA
#define EXS_WRITE_EE 0xFC
#define EXS_READ_EE 0xFD
#define EXS_GET_VERSION 0xFE
#define EXS_RESET 0xFF

#define EXS_BUFFER_SIZE 256
#define EXS_ACK_TIMEOUT 200 // 200 ms ACK timeout

#include <TasmotaSerial.h>

TasmotaSerial *ExsSerial = nullptr;

typedef struct
{
  uint8_t on = 0;
  uint8_t bright_tbl = 0;
  uint8_t dimm = 0;
  uint8_t impuls_start = 0;
  uint32_t impuls_len = 0;
} CHANNEL;

typedef struct
{
  uint8_t version_major = 0;
  uint8_t version_minor = 0;
  CHANNEL channel[2];
  uint8_t gate_lock = 0;
} DIMMER;

struct EXS
{
  uint8_t *buffer = nullptr; // Serial receive buffer
  int byte_counter = 0;      // Index in serial receive buffer
  int cmd_status = 0;
  uint8_t power = 0;
  uint8_t dimm[2] = {0, 0};
  DIMMER dimmer;
} Exs;

/*
 * Internal Functions
 */

uint8_t crc8(const uint8_t *p, uint8_t len)
{
  const uint8_t table[] = {
      0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
      0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D};

  const uint8_t table_rev[] = {
      0x00, 0x70, 0xE0, 0x90, 0xC1, 0xB1, 0x21, 0x51,
      0x83, 0xF3, 0x63, 0x13, 0x42, 0x32, 0xA2, 0xD2};

  uint8_t offset;
  uint8_t temp, crc8_temp;
  uint8_t crc8 = 0;

  for (int i = 0; i < len; i++)
  {
    temp = *(p + i);
    offset = temp ^ crc8;
    offset >>= 4;
    crc8_temp = crc8 & 0x0f;
    crc8 = crc8_temp ^ table_rev[offset];
    offset = crc8 ^ temp;
    offset &= 0x0f;
    crc8_temp = crc8 & 0xf0;
    crc8 = crc8_temp ^ table[offset];
  }
  return crc8 ^ 0x55;
}

void ExsSerialSend(const uint8_t data[] = nullptr, uint16_t len = 0)
{
  int retries = 3;
  char rc;

#ifdef EXS_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("EXS: Tx Packet %*_H"), len, (uint8_t *)data);
#endif

  while (retries)
  {
    retries--;

    ExsSerial->write(data, len);
    ExsSerial->flush();

    // wait for any response
    uint32_t snd_time = millis();
    while ((TimePassedSince(snd_time) < EXS_ACK_TIMEOUT) &&
           (!ExsSerial->available()))
      ;

    if (!ExsSerial->available())
    {
      // timeout
#ifdef EXS_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("ESX: serial send timeout"));
#endif
      continue;
    }

    rc = ExsSerial->read();
    if (rc == 0xFF)
      break;
  }
}

void ExsSendCmd(uint8_t cmd, uint8_t value)
{
  uint8_t buffer[8];
  uint16_t len;

  buffer[0] = 0x7b;
  buffer[3] = cmd;

  switch (cmd)
  {
  case EXS_GATE_1_ON:
  case EXS_GATE_1_OFF:
  case EXS_DIMM_1_ON:
  case EXS_DIMM_1_OFF:
  case EXS_GATE_2_ON:
  case EXS_GATE_2_OFF:
  case EXS_DIMM_2_ON:
  case EXS_DIMM_2_OFF:
  case EXS_GATES_ON:
  case EXS_GATES_OFF:
  case EXS_DIMMS_ON:
  case EXS_DIMMS_OFF:
  case EXS_GET_VALUES:
  case EXS_GET_VERSION:
  case EXS_RESET:
    buffer[2] = 1;
    len = 4;
    break;

  case EXS_CH_LOCK:
  case EXS_DIMM_1_TBL:
  case EXS_DIMM_1_VAL:
  case EXS_DIMM_2_TBL:
  case EXS_DIMM_2_VAL:
    buffer[2] = 2;
    buffer[4] = value;
    len = 5;
    break;
  }
  buffer[1] = crc8(&buffer[3], buffer[2]);

  ExsSerialSend(buffer, len);
}

void ExsSetPower(uint8_t device, uint8_t power)
{
  Exs.dimmer.channel[device].dimm = power;
  ExsSendCmd(EXS_DIMM_1_ON + 0x10 * device + power ^ 1, 0);
}

void ExsSetBri(uint8_t device, uint8_t bri)
{
  Exs.dimmer.channel[device].bright_tbl = bri;
  ExsSendCmd(EXS_DIMM_1_TBL + 0x10 * device, bri);
}

void ExsSyncState(uint8_t device)
{
#ifdef EXS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("EXS: Channel %d Power Want %d, Is %d"),
            device, bitRead(Exs.power, device), Exs.dimmer.channel[device].dimm);
  AddLog(LOG_LEVEL_DEBUG, PSTR("EXS: Set Channel %d Brightness Want %d, Is %d"),
            device, Exs.dimm[device], Exs.dimmer.channel[device].bright_tbl);
#endif

  if (bitRead(Exs.power, device) &&
      Exs.dimm[device] != Exs.dimmer.channel[device].bright_tbl) {
    ExsSetBri(device, Exs.dimm[device]);
  }

  if (!Exs.dimm[device]) {
    Exs.dimmer.channel[device].dimm = 0;
  } else if (Exs.dimmer.channel[device].dimm != bitRead(Exs.power, device)) {
    ExsSetPower(device, bitRead(Exs.power, device));
  }
}

bool ExsSyncState()
{
#ifdef EXS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("EXS: Serial %p, Cmd %d"), ExsSerial, Exs.cmd_status);
#endif

  if (!ExsSerial || Exs.cmd_status != 0)
    return false;

  ExsSyncState(0);
  ExsSyncState(1);
  return true;
}

void ExsDebugState()
{
#ifdef EXS_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("EXS: MCU v%d.%d, c0: On:%d,Dim:%d,Tbl:%d(%d%%), c1: On:%d,Dim:%d,Tbl:%d(%d%%), ChLock: %d"),
              Exs.dimmer.version_major, Exs.dimmer.version_minor,
              Exs.dimmer.channel[0].on, Exs.dimmer.channel[0].dimm,
              Exs.dimmer.channel[0].bright_tbl,
              changeUIntScale(Exs.dimmer.channel[0].bright_tbl, 0, 255, 0, 100),
              Exs.dimmer.channel[1].on, Exs.dimmer.channel[1].dimm,
              Exs.dimmer.channel[1].bright_tbl,
              changeUIntScale(Exs.dimmer.channel[1].bright_tbl, 0, 255, 0, 100),
              Exs.dimmer.gate_lock);
#endif
}

void ExsPacketProcess(void)
{
  uint8_t len = Exs.buffer[1];
  uint8_t cmd = Exs.buffer[2];

  switch (cmd)
  {
  case EXS_GET_VALUES:
      /*
         format firmware 2.1
         0. byte  = startMarker
         1. byte  =  0. crc of bytes 2(CMD) - 11(GATE_LOCK)
         2. byte  =  1. len_Of_Payload
         3. byte  =  2. CMD
         4. byte  =  3. MAJOR
         5. byte  =  4. MINOR
         6. byte  =  5. GATE1_ON
         7. byte  =  6. GATE1_DIMM
         8. byte  =  7. GATE1.BRIGHT
         9. byte  =  8. GATE2_ON
        10. byte  =  9. GATE2_DIMM
        11. byte  = 10. GATE2.BRIGHT
        12. byte  = 11. GATE_LOCK
        13. byte  = '\0'
      */
    if (len > 9)
    {
      Exs.dimmer.version_major = Exs.buffer[3];
      Exs.dimmer.version_minor = Exs.buffer[4];

      //Exs.dimmer.channel[0].on = Exs.buffer[5];
      Exs.dimmer.channel[0].on = Exs.buffer[6];
      Exs.dimmer.channel[0].dimm = Exs.buffer[6];
      Exs.dimmer.channel[0].bright_tbl = Exs.buffer[7];

      //Exs.dimmer.channel[1].on = Exs.buffer[8];
      Exs.dimmer.channel[1].on = Exs.buffer[9];
      Exs.dimmer.channel[1].dimm = Exs.buffer[9];
      Exs.dimmer.channel[1].bright_tbl = Exs.buffer[10];

      Exs.dimmer.gate_lock = Exs.buffer[11];
    }
    else
      /*
         format firmware 1.0
         0. byte  = startMarker
         1. byte  =  0. crc of bytes 2(CMD) - 9(GATE_LOCK)
         2. byte  =  1. len_Of_Payload
         3. byte  =  2. CMD
         4. byte  =  3. GATE1_ON
         5. byte  =  4. GATE1_DIMM
         6. byte  =  5. GATE1.BRIGHT
         7. byte  =  6. GATE2_ON
         8. byte  =  7. GATE2_DIMM
         9. byte  =  8. GATE2.BRIGHT
        10. byte  =  9. GATE_LOCK
        11. byte  = '\0'
    */
    {
      Exs.dimmer.version_major = 1;
      Exs.dimmer.version_minor = 0;

      //Exs.dimmer.channel[0].on = Exs.buffer[3] - 48;
      Exs.dimmer.channel[0].on = Exs.buffer[4] - 48;
      Exs.dimmer.channel[0].dimm = Exs.buffer[4] - 48;
      Exs.dimmer.channel[0].bright_tbl = Exs.buffer[5] - 48;

      //Exs.dimmer.channel[1].on = Exs.buffer[6] - 48;
      Exs.dimmer.channel[1].on = Exs.buffer[7] - 48;
      Exs.dimmer.channel[1].dimm = Exs.buffer[7] - 48;
      Exs.dimmer.channel[1].bright_tbl = Exs.buffer[8] - 48;

      Exs.dimmer.gate_lock = Exs.buffer[9] - 48;
    }

    ExsDebugState();
    ExsSyncState();
    ExsDebugState();
    break;
  default:
    break;
  }
}
/*
 * API Functions
 */
bool ExsModuleSelected(void)
{
  Settings->light_correction = 0;
  Settings->flag.mqtt_serial = 0;          // CMND_SERIALSEND and CMND_SERIALLOG
  Settings->flag3.pwm_multi_channels = 1;  // SetOption68 - Enable multi-channels PWM instead of Color PWM
  SetSeriallog(LOG_LEVEL_NONE);

  UpdateDevicesPresent(2);
  TasmotaGlobal.light_type = LT_SERIAL2;
  return true;
}

bool ExsSetChannels(void)
{
#ifdef EXS_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("EXS: SetChannels %*_H"), XdrvMailbox.data_len, (uint8_t *)XdrvMailbox.data);
#endif

  Exs.dimm[0] = ((uint8_t *)XdrvMailbox.data)[0];
  Exs.dimm[1] = ((uint8_t *)XdrvMailbox.data)[1];
  return ExsSyncState();
}

bool ExsSetPower(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("EXS: Set Power, Device %d, Power 0x%02x"),
            TasmotaGlobal.active_device, XdrvMailbox.index);

  Exs.power = XdrvMailbox.index;
  return ExsSyncState();
}

void EsxMcuStart(void)
{
  int retries = 3;

#ifdef EXS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("EXS: Request MCU configuration, PIN %d to Low"), Pin(GPIO_EXS_ENABLE));
#endif

  pinMode(Pin(GPIO_EXS_ENABLE), OUTPUT);
  digitalWrite(Pin(GPIO_EXS_ENABLE), LOW);

  delay(1); // wait 1ms fot the MCU to come online

  while (ExsSerial->available())
  {
    // clear in the receive buffer
    ExsSerial->read();
  }
}

void ExsInit(void)
{
#ifdef EXS_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("EXS: Starting Tx %d Rx %d"), Pin(GPIO_TXD), Pin(GPIO_RXD));
#endif

  Exs.buffer = (uint8_t *)malloc(EXS_BUFFER_SIZE);
  if (Exs.buffer != nullptr)
  {
    ExsSerial = new TasmotaSerial(Pin(GPIO_RXD), Pin(GPIO_TXD), 2);
    if (ExsSerial->begin(9600))
    {
      if (ExsSerial->hardwareSerial())
      {
        ClaimSerial();
      }
      ExsSerial->flush();
      EsxMcuStart();
      ExsSendCmd(EXS_CH_LOCK, 0);
      ExsSendCmd(EXS_GET_VALUES, 0);
   }
  }
}

void ExsSerialInput(void)
{
  while (ExsSerial->available())
  {
    yield();
    uint8_t serial_in_byte = ExsSerial->read();

    AddLog(LOG_LEVEL_INFO, PSTR("EXS: Serial In Byte 0x%02x"), serial_in_byte);

    if (Exs.cmd_status == 0 &&
        serial_in_byte == 0x7B)
    {
      Exs.cmd_status = 1;
      Exs.byte_counter = 0;
    }
    else if (Exs.byte_counter >= EXS_BUFFER_SIZE)
    {
      Exs.cmd_status = 0;
    }
    else if (Exs.cmd_status == 1)
    {
      Exs.buffer[Exs.byte_counter++] = serial_in_byte;

      if (Exs.byte_counter > 2 && Exs.byte_counter == Exs.buffer[1] + 2)
      {
        uint8_t crc = crc8(&Exs.buffer[2], Exs.buffer[1]);

        // all read
        Exs.cmd_status = 0;

#ifdef EXS_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("EXS: CRC 0x%02x, RX Packet %*_H"), crc, Exs.byte_counter, (uint8_t *)Exs.buffer);
#endif

        if (Exs.buffer[0] == crc)
        {
          ExsSerial->write(0xFF); //send ACK
          ExsPacketProcess();
        }
        else
        {
          ExsSerial->write(0x00); //send NO-ACK
        }

      }
    }
  }
}

/*
 * Commands
 */

#ifdef EXS_MCU_CMNDS

#define D_PRFX_EXS "Exs"
#define D_CMND_EXS_DIMM "Dimm"
#define D_CMND_EXS_DIMM_TBL "DimmTbl"
#define D_CMND_EXS_DIMM_VAL "DimmVal"
#define D_CMND_EXS_DIMMS "Dimms"
#define D_CMND_EXS_CH_LOCK "ChLock"
#define D_CMND_EXS_STATE "State"

const char kExsCommands[] PROGMEM = D_PRFX_EXS "|"
  D_CMND_EXS_DIMM "|" D_CMND_EXS_DIMM_TBL "|" D_CMND_EXS_DIMM_VAL "|"
  D_CMND_EXS_DIMMS "|" D_CMND_EXS_CH_LOCK "|"
  D_CMND_EXS_STATE;

void (* const ExsCommand[])(void) PROGMEM = {
  &CmndExsDimm,  &CmndExsDimmTbl,  &CmndExsDimmVal,
  &CmndExsDimms,  &CmndExsChLock,
  &CmndExsState };

void CmndExsDimm(void)
{
  if ((XdrvMailbox.index == 1 || XdrvMailbox.index == 2) &&
      (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1)) {
    ExsSendCmd(EXS_DIMM_1_ON + 0x10 * (XdrvMailbox.index - 1) +
               XdrvMailbox.payload ^ 1, 0);
  }
  CmndExsState();
}

void CmndExsDimmTbl(void)
{
  if ((XdrvMailbox.index == 1 || XdrvMailbox.index == 2) &&
      (XdrvMailbox.payload > 0 || XdrvMailbox.payload <= 255)) {
    ExsSendCmd(EXS_DIMM_1_TBL + 0x10 * (XdrvMailbox.index - 1),
               XdrvMailbox.payload);
  }
  CmndExsState();
}

void CmndExsDimmVal(void)
{
  if ((XdrvMailbox.index == 1 || XdrvMailbox.index == 2) &&
      (XdrvMailbox.payload > 0 || XdrvMailbox.payload <= 255)) {
    ExsSendCmd(EXS_DIMM_1_VAL + 0x10 * (XdrvMailbox.index - 1),
               XdrvMailbox.payload);
  }
  CmndExsState();
}

void CmndExsDimms(void)
{
  if (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1) {
    ExsSendCmd(EXS_DIMMS_ON + XdrvMailbox.payload ^ 1, 0);
  }
  CmndExsState();
}

void CmndExsChLock(void)
{
  if (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1) {
    ExsSendCmd(EXS_CH_LOCK, XdrvMailbox.payload);
  }
  CmndExsState();
}

void CmndExsState(void)
{
  ExsSendCmd(EXS_GET_VALUES, 0);

  // wait for data
  uint32_t snd_time = millis();
  while ((TimePassedSince(snd_time) < EXS_ACK_TIMEOUT) &&
         (!ExsSerial->available()))
    ;
  ExsSerialInput();

  Response_P(PSTR("{\"" D_CMND_EXS_STATE "\":{"));
  ResponseAppend_P(PSTR("\"McuVersion\":\"%d.%d\","
                        "\"Channels\":["),
                   Exs.dimmer.version_major, Exs.dimmer.version_minor);

  for (uint32_t i = 0; i < 2; i++) {
    if (i != 0) {
      ResponseAppend_P(PSTR(","));
    }
    ResponseAppend_P(PSTR("{\"On\":\"%d\","
                          "\"BrightProz\":\"%d\","
                          "\"BrightTab\":\"%d\","
                          "\"Dimm\":\"%d\"}"),
                     Exs.dimmer.channel[i].on,
                     changeUIntScale(Exs.dimmer.channel[i].bright_tbl, 0, 255, 0, 100),
                     Exs.dimmer.channel[i].bright_tbl,
                     Exs.dimmer.channel[i].dimm);
  }
  ResponseAppend_P(PSTR("],"));
  ResponseAppend_P(PSTR("\"GateLock\":\"%d\""), Exs.dimmer.gate_lock);
  ResponseJsonEndEnd();
}

#endif

/*
 * Interface
 */

bool Xdrv30(uint32_t function)
{
  bool result = false;

  if (EXS_DIMMER == TasmotaGlobal.module_type)
  {
    switch (function)
    {
    case FUNC_LOOP:
      if (ExsSerial)
        ExsSerialInput();
      break;
    case FUNC_MODULE_INIT:
      result = ExsModuleSelected();
      break;
    case FUNC_INIT:
      ExsInit();
      break;
    case FUNC_SET_DEVICE_POWER:
      result = ExsSetPower();
      break;
    case FUNC_SET_CHANNELS:
      result = ExsSetChannels();
      break;
#ifdef EXS_MCU_CMNDS
    case FUNC_COMMAND:
      result = DecodeCommand(kExsCommands, ExsCommand);
      break;
#endif
    }
  }
  return result;
}

#endif  // USE_EXS_DIMMER
#endif  // USE_LIGHT