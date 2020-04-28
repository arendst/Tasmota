/*
  xdrv_31_tasmota_slave.ino - Support for external microcontroller slave on serial

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

#ifdef USE_TASMOTA_SLAVE
/*********************************************************************************************\
 * Tasmota slave
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
 * Tasmota Slave Specific Commands
\*************************************************/

#define CMND_START                     0xFC
#define CMND_END                       0xFD

#define CMND_FEATURES                  0x01
#define CMND_JSON                      0x02
#define CMND_FUNC_EVERY_SECOND         0x03
#define CMND_FUNC_EVERY_100_MSECOND    0x04
#define CMND_SLAVE_SEND                0x05
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

SimpleHexParse::SimpleHexParse(void)
{

}

uint8_t SimpleHexParse::parseLine(char *hexline)
{
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

uint8_t SimpleHexParse::getByte(char* hexline, uint8_t idx)
{
 char buff[3];
 buff[3] = '\0';
 memcpy(&buff, &hexline[(idx*2)-1], 2);
 return strtol(buff, 0, 16);
}

/*
 * End of embedded class SimpleHexParse
 */

struct TSLAVE {
  uint32_t spi_hex_size = 0;
  uint32_t spi_sector_counter = 0;
  uint8_t spi_sector_cursor = 0;
  uint8_t inverted = LOW;
  bool type = false;
  bool flashing  = false;
  bool SerialEnabled = false;
  uint8_t waitstate = 0;            // We use this so that features detection does not slow down other stuff on startup
  bool unsupported = false;
} TSlave;

typedef union {
  uint32_t data;
  struct {
    uint32_t func_json_append : 1;              // Slave supports providing a JSON for TELEPERIOD
    uint32_t func_every_second : 1;             // Slave supports receiving a FUNC_EVERY_SECOND callback with no response
    uint32_t func_every_100_msecond : 1;        // Slave supports receiving a FUNC_EVERY_100_MSECOND callback with no response
    uint32_t func_slave_send : 1;               // Slave supports receiving commands with "slave send xxx"
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
} TSlaveFeatureCfg;

/*
 * The structure below must remain 4 byte aligned to be compatible with
 * Tasmota as master
 */

struct TSLAVE_FEATURES {
  uint32_t features_version;
  TSlaveFeatureCfg features;
} TSlaveSettings;

struct TSLAVE_COMMAND {
  uint8_t command;
  uint8_t parameter;
  uint8_t unused2;
  uint8_t unused3;
} TSlaveCommand;

TasmotaSerial *TasmotaSlave_Serial;

uint32_t TasmotaSlave_FlashStart(void)
{
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;  // Stay on the safe side
}

uint8_t TasmotaSlave_UpdateInit(void)
{
  TSlave.spi_hex_size = 0;
  TSlave.spi_sector_counter = TasmotaSlave_FlashStart();  // Reset the pre-defined write address where firmware will temporarily be stored
  TSlave.spi_sector_cursor = 0;
  return 0;
}

void TasmotaSlave_Reset(void)
{
  if (TSlave.SerialEnabled) {
    digitalWrite(Pin(GPIO_TASMOTASLAVE_RST), !TSlave.inverted);
    delay(1);
    digitalWrite(Pin(GPIO_TASMOTASLAVE_RST), TSlave.inverted);
    delay(1);
    digitalWrite(Pin(GPIO_TASMOTASLAVE_RST), !TSlave.inverted);
    delay(5);
  }
}

uint8_t TasmotaSlave_waitForSerialData(int dataCount, int timeout)
{
  int timer = 0;
  while (timer < timeout) {
    if (TasmotaSlave_Serial->available() >= dataCount) {
      return 1;
    }
    delay(1);
    timer++;
  }
  return 0;
}

uint8_t TasmotaSlave_sendBytes(uint8_t* bytes, int count)
{
  TasmotaSlave_Serial->write(bytes, count);
  TasmotaSlave_waitForSerialData(2, 250);
  uint8_t sync = TasmotaSlave_Serial->read();
  uint8_t ok = TasmotaSlave_Serial->read();
  if ((sync == 0x14) && (ok == 0x10)) {
    return 1;
  }
  return 0;
}

uint8_t TasmotaSlave_execCmd(uint8_t cmd)
{
  uint8_t bytes[] = { cmd, CONST_STK_CRC_EOP };
  return TasmotaSlave_sendBytes(bytes, 2);
}

uint8_t TasmotaSlave_execParam(uint8_t cmd, uint8_t* params, int count)
{
  uint8_t bytes[32];
  bytes[0] = cmd;
  int i = 0;
  while (i < count) {
    bytes[i + 1] = params[i];
    i++;
  }
  bytes[i + 1] = CONST_STK_CRC_EOP;
  return TasmotaSlave_sendBytes(bytes, i + 2);
}

uint8_t TasmotaSlave_exitProgMode(void)
{
  return TasmotaSlave_execCmd(CMND_STK_LEAVE_PROGMODE); // Exit programming mode
}

uint8_t TasmotaSlave_SetupFlash(void)
{
  uint8_t ProgParams[] = {0x86, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x80, 0x00};
  uint8_t ExtProgParams[] = {0x05, 0x04, 0xd7, 0xc2, 0x00};
  TasmotaSlave_Serial->begin(USE_TASMOTA_SLAVE_FLASH_SPEED);
  if (TasmotaSlave_Serial->hardwareSerial()) {
    ClaimSerial();
  }

  TasmotaSlave_Reset();

  uint8_t timeout = 0;
  uint8_t no_error = 0;
  while (50 > timeout) {
    if (TasmotaSlave_execCmd(CMND_STK_GET_SYNC)) {
      timeout = 200;
      no_error = 1;
    }
    timeout++;
    delay(1);
  }
  if (no_error) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Found bootloader"));
  } else {
    no_error = 0;
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Bootloader could not be found"));
  }
  if (no_error) {
    if (TasmotaSlave_execParam(CMND_STK_SET_DEVICE, ProgParams, sizeof(ProgParams))) {
    } else {
      no_error = 0;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Could not configure device for programming (1)"));
    }
  }
  if (no_error) {
    if (TasmotaSlave_execParam(CMND_STK_SET_DEVICE_EXT, ExtProgParams, sizeof(ExtProgParams))) {
    } else {
      no_error = 0;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Could not configure device for programming (2)"));
    }
  }
  if (no_error) {
    if (TasmotaSlave_execCmd(CMND_STK_ENTER_PROGMODE)) {
    } else {
      no_error = 0;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Failed to put bootloader into programming mode"));
    }
  }
  return no_error;
}

uint8_t TasmotaSlave_loadAddress(uint8_t adrHi, uint8_t adrLo)
{
  uint8_t params[] = { adrLo, adrHi };
  return TasmotaSlave_execParam(CMND_STK_LOAD_ADDRESS, params, sizeof(params));
}

void TasmotaSlave_FlashPage(uint8_t addr_h, uint8_t addr_l, uint8_t* data)
{
  uint8_t Header[] = {CMND_STK_PROG_PAGE, 0x00, 0x80, 0x46};
  TasmotaSlave_loadAddress(addr_h, addr_l);
  TasmotaSlave_Serial->write(Header, 4);
  for (int i = 0; i < 128; i++) {
    TasmotaSlave_Serial->write(data[i]);
  }
  TasmotaSlave_Serial->write(CONST_STK_CRC_EOP);
  TasmotaSlave_waitForSerialData(2, 250);
  TasmotaSlave_Serial->read();
  TasmotaSlave_Serial->read();
}

void TasmotaSlave_Flash(void)
{
  bool reading = true;
  uint32_t read = 0;
  uint32_t processed = 0;
  char thishexline[50];
  uint8_t position = 0;
  char* flash_buffer;

  SimpleHexParse hexParse = SimpleHexParse();

  if (!TasmotaSlave_SetupFlash()) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Flashing aborted!"));
    TSlave.flashing  = false;
    restart_flag = 2;
    return;
  }

  flash_buffer = new char[SPI_FLASH_SEC_SIZE];
  uint32_t flash_start = TasmotaSlave_FlashStart() * SPI_FLASH_SEC_SIZE;
  while (reading) {
    ESP.flashRead(flash_start + read, (uint32_t*)flash_buffer, SPI_FLASH_SEC_SIZE);
    read = read + SPI_FLASH_SEC_SIZE;
    if (read >= TSlave.spi_hex_size) {
      reading = false;
    }
    for (uint32_t ca = 0; ca < SPI_FLASH_SEC_SIZE; ca++) {
      processed++;
      if ((processed <= TSlave.spi_hex_size) && (!hexParse.EndOfFile)) {
        if (':' == flash_buffer[ca]) {
          position = 0;
        }
        if (0x0D == flash_buffer[ca]) {
          thishexline[position] = 0;
          hexParse.parseLine(thishexline);
          if (hexParse.PageIsReady) {
            TasmotaSlave_FlashPage(hexParse.ptr_h, hexParse.ptr_l, hexParse.FlashPage);
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
  TasmotaSlave_exitProgMode();
  AddLog_P2(LOG_LEVEL_INFO, PSTR("TasmotaSlave: Flash done!"));
  TSlave.flashing  = false;
  restart_flag = 2;
}

void TasmotaSlave_SetFlagFlashing(bool value)
{
  TSlave.flashing  = value;
}

bool TasmotaSlave_GetFlagFlashing(void)
{
  return TSlave.flashing;
}

void TasmotaSlave_WriteBuffer(uint8_t *buf, size_t size)
{
  if (0 == TSlave.spi_sector_cursor) { // Starting a new sector write so we need to erase it first
    ESP.flashEraseSector(TSlave.spi_sector_counter);
  }
  TSlave.spi_sector_cursor++;
  ESP.flashWrite((TSlave.spi_sector_counter * SPI_FLASH_SEC_SIZE) + ((TSlave.spi_sector_cursor-1)*2048), (uint32_t*)buf, size);
  TSlave.spi_hex_size = TSlave.spi_hex_size + size;
  if (2 == TSlave.spi_sector_cursor) {  // The web upload sends 2048 bytes at a time so keep track of the cursor position to reset it for the next flash sector erase
    TSlave.spi_sector_cursor = 0;
    TSlave.spi_sector_counter++;
  }
}

void TasmotaSlave_Init(void)
{
  if (TSlave.type) {
    return;
  }
  if (10 > TSlave.waitstate) {
    TSlave.waitstate++;
    return;
  }
  if (!TSlave.SerialEnabled) {
    if (PinUsed(GPIO_TASMOTASLAVE_RXD) && PinUsed(GPIO_TASMOTASLAVE_TXD) &&
        (PinUsed(GPIO_TASMOTASLAVE_RST) || PinUsed(GPIO_TASMOTASLAVE_RST_INV))) {
      TasmotaSlave_Serial = new TasmotaSerial(Pin(GPIO_TASMOTASLAVE_RXD), Pin(GPIO_TASMOTASLAVE_TXD), 1, 0, 200);
      if (TasmotaSlave_Serial->begin(USE_TASMOTA_SLAVE_SERIAL_SPEED)) {
        if (TasmotaSlave_Serial->hardwareSerial()) {
          ClaimSerial();
        }
        TasmotaSlave_Serial->setTimeout(50);
        if (PinUsed(GPIO_TASMOTASLAVE_RST_INV)) {
          SetPin(Pin(GPIO_TASMOTASLAVE_RST_INV), GPIO_TASMOTASLAVE_RST);
#ifdef LEGACY_GPIO_ARRAY
          SetPin(99, GPIO_TASMOTASLAVE_RST_INV);
#endif
          TSlave.inverted = HIGH;
        }
        pinMode(Pin(GPIO_TASMOTASLAVE_RST), OUTPUT);
        TSlave.SerialEnabled = true;
        TasmotaSlave_Reset();
        AddLog_P2(LOG_LEVEL_INFO, PSTR("Tasmota Slave Enabled"));
      }
    }
  }
  if (TSlave.SerialEnabled) { // All go for hardware now we need to detect features if there are any
    TasmotaSlave_sendCmnd(CMND_FEATURES, 0);
    char buffer[32];
    TasmotaSlave_Serial->readBytesUntil(char(PARAM_DATA_START), buffer, sizeof(buffer));
    uint8_t len = TasmotaSlave_Serial->readBytesUntil(char(PARAM_DATA_END), buffer, sizeof(buffer));
    memcpy(&TSlaveSettings, &buffer, sizeof(TSlaveSettings));
    if (20191129 == TSlaveSettings.features_version) {
      TSlave.type = true;
      AddLog_P2(LOG_LEVEL_INFO, PSTR("Tasmota Slave Version %u"), TSlaveSettings.features_version);
    } else {
      if ((!TSlave.unsupported) && (TSlaveSettings.features_version > 0)) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR("Tasmota Slave Version %u not supported!"), TSlaveSettings.features_version);
        TSlave.unsupported = true;
      }
    }
  }
}

void TasmotaSlave_Show(void)
{
  if ((TSlave.type) && (TSlaveSettings.features.func_json_append)) {
    char buffer[100];
    TasmotaSlave_sendCmnd(CMND_JSON, 0);
    TasmotaSlave_Serial->readBytesUntil(char(PARAM_DATA_START), buffer, sizeof(buffer)-1);
    uint8_t len = TasmotaSlave_Serial->readBytesUntil(char(PARAM_DATA_END), buffer, sizeof(buffer)-1);
    buffer[len] = '\0';
    ResponseAppend_P(PSTR(",\"TasmotaSlave\":%s"), buffer);
  }
}

void TasmotaSlave_sendCmnd(uint8_t cmnd, uint8_t param)
{
  TSlaveCommand.command = cmnd;
  TSlaveCommand.parameter = param;
  char buffer[sizeof(TSlaveCommand)+2];
  buffer[0] = CMND_START;
  memcpy(&buffer[1], &TSlaveCommand, sizeof(TSlaveCommand));
  buffer[sizeof(TSlaveCommand)+1] = CMND_END;
  for (uint8_t ca = 0; ca < sizeof(buffer); ca++) {
    TasmotaSlave_Serial->write(buffer[ca]);
  }
}

#define D_PRFX_SLAVE "Slave"
#define D_CMND_SLAVE_RESET "Reset"
#define D_CMND_SLAVE_SEND "Send"

const char kTasmotaSlaveCommands[] PROGMEM = D_PRFX_SLAVE "|"
  D_CMND_SLAVE_RESET "|" D_CMND_SLAVE_SEND;

void (* const TasmotaSlaveCommand[])(void) PROGMEM = {
  &CmndTasmotaSlaveReset, &CmndTasmotaSlaveSend };

void CmndTasmotaSlaveReset(void)
{
  TasmotaSlave_Reset();
  TSlave.type = false;        // Force redetection
  TSlave.waitstate = 7;       // give it at least 3 seconds to restart from bootloader
  TSlave.unsupported = false; // Reset unsupported flag
  ResponseCmndDone();
}

void CmndTasmotaSlaveSend(void)
{
  if (0 < XdrvMailbox.data_len) {
    TasmotaSlave_sendCmnd(CMND_SLAVE_SEND, XdrvMailbox.data_len);
    TasmotaSlave_Serial->write(char(PARAM_DATA_START));
    for (uint8_t idx = 0; idx < XdrvMailbox.data_len; idx++) {
      TasmotaSlave_Serial->write(XdrvMailbox.data[idx]);
    }
    TasmotaSlave_Serial->write(char(PARAM_DATA_END));
  }
  ResponseCmndDone();
}

void TasmotaSlave_ProcessIn(void)
{
  uint8_t cmnd = TasmotaSlave_Serial->read();
  switch (cmnd) {
    case CMND_START:
      TasmotaSlave_waitForSerialData(sizeof(TSlaveCommand),50);
      uint8_t buffer[sizeof(TSlaveCommand)];
      for (uint8_t idx = 0; idx < sizeof(TSlaveCommand); idx++) {
        buffer[idx] = TasmotaSlave_Serial->read();
      }
      TasmotaSlave_Serial->read(); // read trailing byte of command
      memcpy(&TSlaveCommand, &buffer, sizeof(TSlaveCommand));
      char inbuf[TSlaveCommand.parameter+1];
      TasmotaSlave_waitForSerialData(TSlaveCommand.parameter, 50);
      TasmotaSlave_Serial->read(); // Read leading byte
      for (uint8_t idx = 0; idx < TSlaveCommand.parameter; idx++) {
        inbuf[idx] = TasmotaSlave_Serial->read();
      }
      TasmotaSlave_Serial->read(); // Read trailing byte
      inbuf[TSlaveCommand.parameter] = '\0';

      if (CMND_PUBLISH_TELE == TSlaveCommand.command) { // We need to publish stat/ with incoming stream as content
        Response_P(PSTR("{\"TasmotaSlave\":"));
        ResponseAppend_P("%s", inbuf);
        ResponseJsonEnd();
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);
        XdrvRulesProcess();
      }
      if (CMND_EXECUTE_CMND == TSlaveCommand.command) { // We need to execute the incoming command
        ExecuteCommand(inbuf, SRC_IGNORE);
      }
      break;
    default:
      break;
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv31(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_100_MSECOND:
      if (TSlave.type) {
        if (TasmotaSlave_Serial->available()) {
          TasmotaSlave_ProcessIn();
        }
        if (TSlaveSettings.features.func_every_100_msecond) {
          TasmotaSlave_sendCmnd(CMND_FUNC_EVERY_100_MSECOND, 0);
        }
      }
      break;
    case FUNC_EVERY_SECOND:
      if ((TSlave.type) && (TSlaveSettings.features.func_every_second)) {
        TasmotaSlave_sendCmnd(CMND_FUNC_EVERY_SECOND, 0);
      }
      TasmotaSlave_Init();
      break;
    case FUNC_JSON_APPEND:
      if ((TSlave.type) && (TSlaveSettings.features.func_json_append)) {
        TasmotaSlave_Show();
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTasmotaSlaveCommands, TasmotaSlaveCommand);
      break;
  }
  return result;
}

#endif  // USE_TASMOTA_SLAVE
