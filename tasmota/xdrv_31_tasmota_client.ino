/*
  xdrv_31_tasmota_client.ino - Support for external microcontroller on serial

  Copyright (C) 2020  Andre Thomas and Theo Arends

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

#ifdef USE_TASMOTA_CLIENT
/*********************************************************************************************\
 * Tasmota to microcontroller
\*********************************************************************************************/

#define XDRV_31                    31

#define CONST_STK_CRC_EOP          0x20

#define CMND_STK_GET_SYNC          0x30
#define CMND_STK_SET_DEVICE        0x42
#define CMND_STK_SET_DEVICE_EXT    0x45
#define CMND_STK_ENTER_PROGMODE    0x50
#define CMND_STK_LEAVE_PROGMODE    0x51
#define CMND_STK_LOAD_ADDRESS      0x55
#define CMND_STK_PROG_PAGE         0x64

/*************************************************\
 * Tasmota Client Specific Commands
\*************************************************/

#define CMND_START                     0xFC
#define CMND_END                       0xFD

#define CMND_FEATURES                  0x01
#define CMND_JSON                      0x02
#define CMND_FUNC_EVERY_SECOND         0x03
#define CMND_FUNC_EVERY_100_MSECOND    0x04
#define CMND_CLIENT_SEND               0x05
#define CMND_PUBLISH_TELE              0x06
#define CMND_EXECUTE_CMND              0x07

#define PARAM_DATA_START               0xFE
#define PARAM_DATA_END                 0xFF

#include <TasmotaSerial.h>

/*
 * Embedding class in here since its rather specific to Arduino bootloader
 */

class SimpleHexParse {
  public:
   SimpleHexParse(void);
   uint8_t parseLine(char *hexline);
   uint8_t ptr_l = 0;
   uint8_t ptr_h = 0;
   bool PageIsReady = false;
   bool firstrun = true;
   bool EndOfFile = false;
   uint8_t FlashPage[128];
   uint8_t FlashPageIdx = 0;
   uint8_t layoverBuffer[16];
   uint8_t layoverIdx = 0;
   uint8_t getByte(char *hexline, uint8_t idx);
};

SimpleHexParse::SimpleHexParse(void) {

}

uint8_t SimpleHexParse::parseLine(char *hexline) {
  if (layoverIdx) {
    memcpy(&FlashPage[0], &layoverBuffer[0], layoverIdx);
    FlashPageIdx = layoverIdx;
    layoverIdx = 0;
  }
  uint8_t len = getByte(hexline, 1);
  uint8_t addr_h = getByte(hexline, 2);
  uint8_t addr_l = getByte(hexline, 3);
  uint8_t rectype = getByte(hexline, 4);
  for (uint8_t idx = 0; idx < len; idx++) {
       if (FlashPageIdx < 128) {
         FlashPage[FlashPageIdx] = getByte(hexline, idx+5);
         FlashPageIdx++;
       } else { // We have layover bytes
         layoverBuffer[layoverIdx] = getByte(hexline, idx+5);
         layoverIdx++;
       }
  }
  if (1 == rectype) {
    EndOfFile = true;
    while (FlashPageIdx < 128) {
      FlashPage[FlashPageIdx] = 0xFF;
      FlashPageIdx++;
    }
  }
  if (FlashPageIdx == 128) {
    if (firstrun) {
      firstrun = false;
    } else {
      ptr_l += 0x40;
      if (ptr_l == 0) {
        ptr_l = 0;
        ptr_h++;
      }
    }
    firstrun = false;
    PageIsReady = true;
  }
  return 0;
}

uint8_t SimpleHexParse::getByte(char* hexline, uint8_t idx) {
 char buff[3];
 buff[3] = '\0';
 memcpy(&buff, &hexline[(idx*2)-1], 2);
 return strtol(buff, 0, 16);
}

/*
 * End of embedded class SimpleHexParse
 */

struct TCLIENT {
  uint32_t spi_hex_size = 0;
  uint32_t spi_sector_counter = 0;
  uint8_t spi_sector_cursor = 0;
  uint8_t inverted = LOW;
  bool type = false;
  bool flashing  = false;
  bool SerialEnabled = false;
  uint8_t waitstate = 0;            // We use this so that features detection does not slow down other stuff on startup
  bool unsupported = false;
} TClient;

typedef union {
  uint32_t data;
  struct {
    uint32_t func_json_append : 1;              // Client supports providing a JSON for TELEPERIOD
    uint32_t func_every_second : 1;             // Client supports receiving a FUNC_EVERY_SECOND callback with no response
    uint32_t func_every_100_msecond : 1;        // Client supports receiving a FUNC_EVERY_100_MSECOND callback with no response
    uint32_t func_client_send : 1;              // Client supports receiving commands with "client send xxx"
    uint32_t spare4 : 1;
    uint32_t spare5 : 1;
    uint32_t spare6 : 1;
    uint32_t spare7 : 1;
    uint32_t spare8 : 1;
    uint32_t spare9 : 1;
    uint32_t spare10 : 1;
    uint32_t spare11 : 1;
    uint32_t spare12 : 1;
    uint32_t spare13 : 1;
    uint32_t spare14 : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t spare17 : 1;
    uint32_t spare18 : 1;
    uint32_t spare19 : 1;
    uint32_t spare20 : 1;
    uint32_t spare21 : 1;
    uint32_t spare22 : 1;
    uint32_t spare23 : 1;
    uint32_t spare24 : 1;
    uint32_t spare25 : 1;
    uint32_t spare26 : 1;
    uint32_t spare27 : 1;
    uint32_t spare28 : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t spare31 : 1;
  };
} TClientFeatureCfg;

/*
 * The structure below must remain 4 byte aligned to be compatible with
 * Tasmota as master
 */

struct TCLIENT_FEATURES {
  uint32_t features_version;
  TClientFeatureCfg features;
} TClientSettings;

struct TCLIENT_COMMAND {
  uint8_t command;
  uint8_t parameter;
  uint8_t unused2;
  uint8_t unused3;
} TClientCommand;

TasmotaSerial *TasmotaClient_Serial;

uint32_t TasmotaClient_FlashStart(void) {
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;  // Stay on the safe side
}

uint8_t TasmotaClient_UpdateInit(void) {
  TClient.spi_hex_size = 0;
  TClient.spi_sector_counter = TasmotaClient_FlashStart();  // Reset the pre-defined write address where firmware will temporarily be stored
  TClient.spi_sector_cursor = 0;
  return 0;
}

void TasmotaClient_Reset(void) {
  if (TClient.SerialEnabled) {
    digitalWrite(Pin(GPIO_TASMOTACLIENT_RST), !TClient.inverted);
    delay(1);
    digitalWrite(Pin(GPIO_TASMOTACLIENT_RST), TClient.inverted);
    delay(1);
    digitalWrite(Pin(GPIO_TASMOTACLIENT_RST), !TClient.inverted);
    delay(5);
  }
}

uint8_t TasmotaClient_waitForSerialData(int dataCount, int timeout) {
  int timer = 0;
  while (timer < timeout) {
    if (TasmotaClient_Serial->available() >= dataCount) {
      return 1;
    }
    delay(1);
    timer++;
  }
  return 0;
}

uint8_t TasmotaClient_sendBytes(uint8_t* bytes, int count) {
  TasmotaClient_Serial->write(bytes, count);
  TasmotaClient_waitForSerialData(2, 250);
  uint8_t sync = TasmotaClient_Serial->read();
  uint8_t ok = TasmotaClient_Serial->read();
  if ((sync == 0x14) && (ok == 0x10)) {
    return 1;
  }
  return 0;
}

uint8_t TasmotaClient_execCmd(uint8_t cmd) {
  uint8_t bytes[] = { cmd, CONST_STK_CRC_EOP };
  return TasmotaClient_sendBytes(bytes, 2);
}

uint8_t TasmotaClient_execParam(uint8_t cmd, uint8_t* params, int count) {
  uint8_t bytes[32];
  bytes[0] = cmd;
  int i = 0;
  while (i < count) {
    bytes[i + 1] = params[i];
    i++;
  }
  bytes[i + 1] = CONST_STK_CRC_EOP;
  return TasmotaClient_sendBytes(bytes, i + 2);
}

uint8_t TasmotaClient_exitProgMode(void) {
  return TasmotaClient_execCmd(CMND_STK_LEAVE_PROGMODE); // Exit programming mode
}

uint8_t TasmotaClient_SetupFlash(void) {
  uint8_t ProgParams[] = {0x86, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x80, 0x00};
  uint8_t ExtProgParams[] = {0x05, 0x04, 0xd7, 0xc2, 0x00};
  TasmotaClient_Serial->begin(USE_TASMOTA_CLIENT_FLASH_SPEED);
  if (TasmotaClient_Serial->hardwareSerial()) {
    ClaimSerial();
  }

  TasmotaClient_Reset();

  uint8_t timeout = 0;
  uint8_t no_error = 0;
  while (50 > timeout) {
    if (TasmotaClient_execCmd(CMND_STK_GET_SYNC)) {
      timeout = 200;
      no_error = 1;
    }
    timeout++;
    delay(1);
  }
  if (no_error) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Found bootloader"));
  } else {
    no_error = 0;
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Bootloader could not be found"));
  }
  if (no_error) {
    if (TasmotaClient_execParam(CMND_STK_SET_DEVICE, ProgParams, sizeof(ProgParams))) {
    } else {
      no_error = 0;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Could not configure device for programming (1)"));
    }
  }
  if (no_error) {
    if (TasmotaClient_execParam(CMND_STK_SET_DEVICE_EXT, ExtProgParams, sizeof(ExtProgParams))) {
    } else {
      no_error = 0;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Could not configure device for programming (2)"));
    }
  }
  if (no_error) {
    if (TasmotaClient_execCmd(CMND_STK_ENTER_PROGMODE)) {
    } else {
      no_error = 0;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Failed to put bootloader into programming mode"));
    }
  }
  return no_error;
}

uint8_t TasmotaClient_loadAddress(uint8_t adrHi, uint8_t adrLo) {
  uint8_t params[] = { adrLo, adrHi };
  return TasmotaClient_execParam(CMND_STK_LOAD_ADDRESS, params, sizeof(params));
}

void TasmotaClient_FlashPage(uint8_t addr_h, uint8_t addr_l, uint8_t* data) {
  uint8_t Header[] = {CMND_STK_PROG_PAGE, 0x00, 0x80, 0x46};
  TasmotaClient_loadAddress(addr_h, addr_l);
  TasmotaClient_Serial->write(Header, 4);
  for (int i = 0; i < 128; i++) {
    TasmotaClient_Serial->write(data[i]);
  }
  TasmotaClient_Serial->write(CONST_STK_CRC_EOP);
  TasmotaClient_waitForSerialData(2, 250);
  TasmotaClient_Serial->read();
  TasmotaClient_Serial->read();
}

void TasmotaClient_Flash(void) {
  bool reading = true;
  uint32_t read = 0;
  uint32_t processed = 0;
  char thishexline[50];
  uint8_t position = 0;
  char* flash_buffer;

  SimpleHexParse hexParse = SimpleHexParse();

  if (!TasmotaClient_SetupFlash()) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Flashing aborted!"));
    TClient.flashing  = false;
    restart_flag = 2;
    return;
  }

  flash_buffer = new char[SPI_FLASH_SEC_SIZE];
  uint32_t flash_start = TasmotaClient_FlashStart() * SPI_FLASH_SEC_SIZE;
  while (reading) {
    ESP.flashRead(flash_start + read, (uint32_t*)flash_buffer, SPI_FLASH_SEC_SIZE);
    read = read + SPI_FLASH_SEC_SIZE;
    if (read >= TClient.spi_hex_size) {
      reading = false;
    }
    for (uint32_t ca = 0; ca < SPI_FLASH_SEC_SIZE; ca++) {
      processed++;
      if ((processed <= TClient.spi_hex_size) && (!hexParse.EndOfFile)) {
        if (':' == flash_buffer[ca]) {
          position = 0;
        }
        if (0x0D == flash_buffer[ca]) {
          thishexline[position] = 0;
          hexParse.parseLine(thishexline);
          if (hexParse.PageIsReady) {
            TasmotaClient_FlashPage(hexParse.ptr_h, hexParse.ptr_l, hexParse.FlashPage);
            hexParse.PageIsReady = false;
            hexParse.FlashPageIdx = 0;
          }
        } else {
          if (0x0A != flash_buffer[ca]) {
            thishexline[position] = flash_buffer[ca];
            position++;
          }
        }
      }
    }
  }
  TasmotaClient_exitProgMode();
  AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Flash done!"));
  TClient.flashing  = false;
  restart_flag = 2;
}

void TasmotaClient_SetFlagFlashing(bool value) {
  TClient.flashing  = value;
}

bool TasmotaClient_GetFlagFlashing(void) {
  return TClient.flashing;
}

void TasmotaClient_WriteBuffer(uint8_t *buf, size_t size) {
  if (0 == TClient.spi_sector_cursor) { // Starting a new sector write so we need to erase it first
    ESP.flashEraseSector(TClient.spi_sector_counter);
  }
  TClient.spi_sector_cursor++;
  ESP.flashWrite((TClient.spi_sector_counter * SPI_FLASH_SEC_SIZE) + ((TClient.spi_sector_cursor-1)*2048), (uint32_t*)buf, size);
  TClient.spi_hex_size = TClient.spi_hex_size + size;
  if (2 == TClient.spi_sector_cursor) {  // The web upload sends 2048 bytes at a time so keep track of the cursor position to reset it for the next flash sector erase
    TClient.spi_sector_cursor = 0;
    TClient.spi_sector_counter++;
  }
}

void TasmotaClient_Init(void) {
  if (TClient.type) {
    return;
  }
  if (10 > TClient.waitstate) {
    TClient.waitstate++;
    return;
  }
  if (!TClient.SerialEnabled) {
    if (PinUsed(GPIO_TASMOTACLIENT_RXD) && PinUsed(GPIO_TASMOTACLIENT_TXD) &&
        (PinUsed(GPIO_TASMOTACLIENT_RST) || PinUsed(GPIO_TASMOTACLIENT_RST_INV))) {
      TasmotaClient_Serial = new TasmotaSerial(Pin(GPIO_TASMOTACLIENT_RXD), Pin(GPIO_TASMOTACLIENT_TXD), 1, 0, 200);
      if (TasmotaClient_Serial->begin(USE_TASMOTA_CLIENT_SERIAL_SPEED)) {
        if (TasmotaClient_Serial->hardwareSerial()) {
          ClaimSerial();
        }
        TasmotaClient_Serial->setTimeout(100);  // Theo 20200502 - increase from 50
        if (PinUsed(GPIO_TASMOTACLIENT_RST_INV)) {
          SetPin(Pin(GPIO_TASMOTACLIENT_RST_INV), AGPIO(GPIO_TASMOTACLIENT_RST));
          TClient.inverted = HIGH;
        }
        pinMode(Pin(GPIO_TASMOTACLIENT_RST), OUTPUT);
        TClient.SerialEnabled = true;
        TasmotaClient_Reset();
        AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Enabled"));
      }
    }
  }
  if (TClient.SerialEnabled) {  // All go for hardware now we need to detect features if there are any
    TasmotaClient_sendCmnd(CMND_FEATURES, 0);
    char buffer[32] = { 0 };
    TasmotaClient_Serial->readBytesUntil(char(PARAM_DATA_START), buffer, sizeof(buffer));
    uint8_t len = TasmotaClient_Serial->readBytesUntil(char(PARAM_DATA_END), buffer, sizeof(buffer));

    if (len) { AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)buffer, len); }  // Theo 20200502 - DMP: 99 17 34 01 02 00 00 00

    memcpy(&TClientSettings, &buffer, sizeof(TClientSettings));
    if (20191129 == TClientSettings.features_version) {
      TClient.type = true;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Version %u"), TClientSettings.features_version);
    } else {
      if ((!TClient.unsupported) && (TClientSettings.features_version > 0)) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR("TCL: Version %u not supported!"), TClientSettings.features_version);
        TClient.unsupported = true;
      }
    }
  }
}

bool TasmotaClient_Available(void) {
  return TClient.SerialEnabled;
}

void TasmotaClient_Show(void) {
  if ((TClient.type) && (TClientSettings.features.func_json_append)) {
    char buffer[100];
    TasmotaClient_sendCmnd(CMND_JSON, 0);
    TasmotaClient_Serial->readBytesUntil(char(PARAM_DATA_START), buffer, sizeof(buffer)-1);
    uint8_t len = TasmotaClient_Serial->readBytesUntil(char(PARAM_DATA_END), buffer, sizeof(buffer)-1);
    buffer[len] = '\0';
    ResponseAppend_P(PSTR(",\"TasmotaClient\":%s"), buffer);
  }
}

void TasmotaClient_sendCmnd(uint8_t cmnd, uint8_t param) {
  TClientCommand.command = cmnd;
  TClientCommand.parameter = param;
  char buffer[sizeof(TClientCommand)+2];
  buffer[0] = CMND_START;
  memcpy(&buffer[1], &TClientCommand, sizeof(TClientCommand));
  buffer[sizeof(TClientCommand)+1] = CMND_END;

  TasmotaClient_Serial->flush();  // Theo 20200502

  for (uint8_t ca = 0; ca < sizeof(buffer); ca++) {
    TasmotaClient_Serial->write(buffer[ca]);
  }
}

#define D_PRFX_CLIENT "Client"
#define D_CMND_CLIENT_RESET "Reset"
#define D_CMND_CLIENT_SEND "Send"

const char kTasmotaClientCommands[] PROGMEM = D_PRFX_CLIENT "|"
  D_CMND_CLIENT_RESET "|" D_CMND_CLIENT_SEND;

void (* const TasmotaClientCommand[])(void) PROGMEM = {
  &CmndClientReset, &CmndClientSend };

void CmndClientReset(void) {
  TasmotaClient_Reset();
  TClient.type = false;        // Force redetection
  TClient.waitstate = 7;       // give it at least 3 seconds to restart from bootloader
  TClient.unsupported = false; // Reset unsupported flag
  ResponseCmndDone();
}

void CmndClientSend(void) {
  if (TClient.SerialEnabled) {
    if (0 < XdrvMailbox.data_len) {
      TasmotaClient_sendCmnd(CMND_CLIENT_SEND, XdrvMailbox.data_len);
      TasmotaClient_Serial->write(char(PARAM_DATA_START));
      for (uint8_t idx = 0; idx < XdrvMailbox.data_len; idx++) {
        TasmotaClient_Serial->write(XdrvMailbox.data[idx]);
      }
      TasmotaClient_Serial->write(char(PARAM_DATA_END));
    }
    ResponseCmndDone();
  }
}

void TasmotaClient_ProcessIn(void) {
  uint8_t cmnd = TasmotaClient_Serial->read();
  if (CMND_START == cmnd) {
    TasmotaClient_waitForSerialData(sizeof(TClientCommand),50);
    uint8_t buffer[sizeof(TClientCommand)];
    for (uint8_t idx = 0; idx < sizeof(TClientCommand); idx++) {
      buffer[idx] = TasmotaClient_Serial->read();
    }
    TasmotaClient_Serial->read(); // read trailing byte of command
    memcpy(&TClientCommand, &buffer, sizeof(TClientCommand));
    char inbuf[TClientCommand.parameter+1];
    TasmotaClient_waitForSerialData(TClientCommand.parameter, 50);
    TasmotaClient_Serial->read(); // Read leading byte
    for (uint8_t idx = 0; idx < TClientCommand.parameter; idx++) {
      inbuf[idx] = TasmotaClient_Serial->read();
    }
    TasmotaClient_Serial->read(); // Read trailing byte
    inbuf[TClientCommand.parameter] = '\0';

    if (CMND_PUBLISH_TELE == TClientCommand.command) { // We need to publish stat/ with incoming stream as content
      Response_P(PSTR("{\"TasmotaClient\":"));
      ResponseAppend_P("%s", inbuf);
      ResponseJsonEnd();
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, mqtt_data);
    }
    if (CMND_EXECUTE_CMND == TClientCommand.command) { // We need to execute the incoming command
      ExecuteCommand(inbuf, SRC_IGNORE);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv31(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_100_MSECOND:
      if (TClient.type) {
        if (TasmotaClient_Serial->available()) {
          TasmotaClient_ProcessIn();
        }
        if (TClientSettings.features.func_every_100_msecond) {
          TasmotaClient_sendCmnd(CMND_FUNC_EVERY_100_MSECOND, 0);
        }
      }
      break;
    case FUNC_EVERY_SECOND:
      if ((TClient.type) && (TClientSettings.features.func_every_second)) {
        TasmotaClient_sendCmnd(CMND_FUNC_EVERY_SECOND, 0);
      }
      TasmotaClient_Init();
      break;
    case FUNC_JSON_APPEND:
      if ((TClient.type) && (TClientSettings.features.func_json_append)) {
        TasmotaClient_Show();
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTasmotaClientCommands, TasmotaClientCommand);
      break;
  }
  return result;
}

#endif  // USE_TASMOTA_CLIENT
