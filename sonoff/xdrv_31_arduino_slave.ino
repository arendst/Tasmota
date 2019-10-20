/*
  xdrv_31_arduino_slave.ino - Support for Arduino Slave on Serial

  Copyright (C) 2019  Andre Thomas and Theo Arends

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

#ifdef USE_ARDUINO_SLAVE

#include <TasmotaSerial.h>
#include <ArduinoHexParse.h>

#define CONST_STK_CRC_EOP          0x20

#define CMND_STK_GET_SYNC          0x30
#define CMND_STK_SET_DEVICE        0x42
#define CMND_STK_SET_DEVICE_EXT    0x45
#define CMND_STK_ENTER_PROGMODE    0x50
#define CMND_STK_LEAVE_PROGMODE    0x51
#define CMND_STK_LOAD_ADDRESS      0x55
#define CMND_STK_PROG_PAGE         0x64

uint32_t as_spi_hex_size = 0;
uint8_t as_spi_sector_start = 0x96;
uint8_t as_spi_sector_counter = 0x96;
uint8_t as_spi_sector_cursor = 0;
bool as_flashing = false;

uint8_t as_type = 0;

TasmotaSerial *ArduinoSlave_Serial;

#define XDRV_31                    31

uint8_t ArduinoSlave_UpdateInit(void)
{
  as_spi_hex_size = 0;
  as_spi_sector_counter = as_spi_sector_start; // Reset the pre-defined write address where firmware will temporarily be stored
  as_spi_sector_cursor = 0;
  return 0;
}

void ArduinoSlave_Reset(void)
{
  if (as_type) {
#ifdef USE_ARDUINO_INVERT_RESET    
    digitalWrite(pin[GPIO_ARDUINO_RESET], LOW);
    delay(1);
    digitalWrite(pin[GPIO_ARDUINO_RESET], HIGH);
    delay(1);
    digitalWrite(pin[GPIO_ARDUINO_RESET], LOW);
    delay(5);
#else
    digitalWrite(pin[GPIO_ARDUINO_RESET], HIGH);
    delay(1);
    digitalWrite(pin[GPIO_ARDUINO_RESET], LOW);
    delay(1);
    digitalWrite(pin[GPIO_ARDUINO_RESET], HIGH);
    delay(5);
#endif
  }
}

uint8_t ArduinoSlave_waitForSerialData(int dataCount, int timeout) {
  int timer = 0;
  while (timer < timeout) {
    if (ArduinoSlave_Serial->available() >= dataCount) {
      return 1;
    }
    delay(1);
    timer++;
  }
  return 0;
}

byte ArduinoSlave_sendBytes(byte* bytes, int count) {
  ArduinoSlave_Serial->write(bytes, count);
  ArduinoSlave_waitForSerialData(2, 1000);
  byte sync = ArduinoSlave_Serial->read();
  byte ok = ArduinoSlave_Serial->read();
  if (sync == 0x14 && ok == 0x10) {
    return 1;
  }
  return 0;
}

byte ArduinoSlave_execCmd(byte cmd) {
  byte bytes[] = { cmd, CONST_STK_CRC_EOP };
  return ArduinoSlave_sendBytes(bytes, 2);
}

byte ArduinoSlave_execParam(byte cmd, byte* params, int count) {
  byte bytes[32];
  bytes[0] = cmd;
  int i = 0;
  while (i < count) {
    bytes[i + 1] = params[i];
    i++;
  }
  bytes[i + 1] = CONST_STK_CRC_EOP;
  return ArduinoSlave_sendBytes(bytes, i + 2);
}

uint8_t ArduinoSlave_exitProgMode(void)
{
  return ArduinoSlave_execCmd(CMND_STK_LEAVE_PROGMODE); // Exit programming mode
}

void ArduinoSlave_SetupFlash(void)
{
  byte ProgParams[] = {0x86,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0xff,0xff,0xff,0xff,0x00,0x80,0x04,0x00,0x00,0x00,0x80,0x00};
  byte ExtProgParams[] = {0x05,0x04,0xd7,0xc2,0x00};
  ArduinoSlave_Serial->begin(USE_ARDUINO_FLASH_SPEED);
  if (ArduinoSlave_Serial->hardwareSerial()) { 
    ClaimSerial();
  }
  ArduinoSlave_Reset();
  ArduinoSlave_execCmd(CMND_STK_GET_SYNC);
  ArduinoSlave_execParam(CMND_STK_SET_DEVICE, ProgParams, sizeof(ProgParams));            // Set programming parameters
  ArduinoSlave_execParam(CMND_STK_SET_DEVICE_EXT, ExtProgParams, sizeof(ExtProgParams));  // Set extended programming parameters
  ArduinoSlave_execCmd(CMND_STK_ENTER_PROGMODE);                                          // Enter programming mode
}

uint8_t ArduinoSlave_loadAddress(byte adrHi, byte adrLo) {
  byte params[] = { adrHi, adrLo };
  return ArduinoSlave_execParam(CMND_STK_LOAD_ADDRESS, params, sizeof(params));
}

void ArduinoSlave_FlashPage(byte* address, byte* data)
{
  byte Header[] = {CMND_STK_PROG_PAGE, 0x00, 0x80, 0x46};
  ArduinoSlave_loadAddress(address[1], address[0]);
  ArduinoSlave_Serial->write(Header, 4);
  for (int i = 0; i < 128; i++) {
    ArduinoSlave_Serial->write(data[i]);
  }
  ArduinoSlave_Serial->write(CONST_STK_CRC_EOP);
  ArduinoSlave_waitForSerialData(2, 1000);
  ArduinoSlave_Serial->read();
  ArduinoSlave_Serial->read();
}

void ArduinoSlave_Flash(void)
{
  bool reading = true;
  uint32_t read = 0;
  uint32_t processed = 0;
  char thishexline[50];
  uint8_t position = 0;
  char* flash_buffer;
  ArduinoHexParse hexParse = ArduinoHexParse();

  ArduinoSlave_SetupFlash();

  flash_buffer = new char[SPI_FLASH_SEC_SIZE];
  while (reading) {
    ESP.flashRead(0x96000 + read, (uint32_t*)flash_buffer, FLASH_SECTOR_SIZE);
    read = read + FLASH_SECTOR_SIZE;
    if (read >= as_spi_hex_size) { 
      reading = false;
    }
    for (uint16_t ca=0; ca<FLASH_SECTOR_SIZE; ca++) {
      processed++;
      if (processed <= as_spi_hex_size) {
        if (':' == flash_buffer[ca]) {
          position = 0;
        }
        if (0x0D == flash_buffer[ca]) {
          thishexline[position] = 0;
          hexParse.ParseLine((byte*)thishexline);
          if (hexParse.IsFlashPageReady()) {
            byte* page = hexParse.GetFlashPage();
            byte* address = hexParse.GetLoadAddress();
            ArduinoSlave_FlashPage(address, page);
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
  as_flashing = false;
  ArduinoSlave_exitProgMode();
  restart_flag = 2;
}

void ArduinoSlave_SetFlagFlashing(bool value)
{
  as_flashing = value;
}

bool ArduinoSlave_GetFlagFlashing(void)
{
  return as_flashing;
}

void ArduinoSlave_WriteBuffer(uint8_t *buf, size_t size)
{
  if (0 == as_spi_sector_cursor) { // Starting a new sector write so we need to erase it first
    ESP.flashEraseSector(as_spi_sector_counter);
  }
  as_spi_sector_cursor++;
  ESP.flashWrite((as_spi_sector_counter * 0x1000)+((as_spi_sector_cursor-1)*2048), (uint32_t*)buf, size);
  as_spi_hex_size = as_spi_hex_size + size;
  if (2 == as_spi_sector_cursor) {  // The web upload sends 2048 bytes at a time so keep track of the cursor position to reset it for the next flash sector erase
    as_spi_sector_cursor = 0;
    as_spi_sector_counter++;
  }
}

void ArduinoSlave_Init(void)
{
  if (as_type) {
    return;
  }
  if ((pin[GPIO_ARDUINO_RXD] < 99) && (pin[GPIO_ARDUINO_TXD] < 99) && (pin[GPIO_ARDUINO_RESET] < 99)) {
    ArduinoSlave_Serial = new TasmotaSerial(pin[GPIO_ARDUINO_RXD], pin[GPIO_ARDUINO_TXD], 1, 0, 200);
    if (ArduinoSlave_Serial->begin(USE_ARDUINO_SERIAL_SPEED)) {
      if (ArduinoSlave_Serial->hardwareSerial()) { 
        ClaimSerial();
      }
      pinMode(pin[GPIO_ARDUINO_RESET], OUTPUT);
      as_type = 1;
      ArduinoSlave_Reset();
      AddLog_P2(LOG_LEVEL_INFO, PSTR("Arduino Slave Enabled"));
    }
  }
}

void ArduinoSlave_Show(bool json)
{
  if (as_type) {
    char buffer[100];
    ArduinoSlave_Serial->flush();
    ArduinoSlave_Serial->print("JSON");
    ArduinoSlave_Serial->find(char(0xFE));
    uint16_t haveread = ArduinoSlave_Serial->readBytesUntil(char(0xFF), buffer, sizeof(buffer)-1);
    buffer[haveread] = '\0';
    if (json) {
      ResponseAppend_P(PSTR(",\"ArduinoSlave\":%s"), buffer);
    }
  }
}

bool Xdrv31(uint8_t function)
{
  bool result = false;
  switch (function) {
    case FUNC_EVERY_SECOND:
      ArduinoSlave_Init();
      break;
    case FUNC_JSON_APPEND:
      ArduinoSlave_Show(1);
      break;
    case FUNC_COMMAND_DRIVER:
      break;
    default:
      break;
  }
}

#endif // USE_ARDUINO_SLAVE