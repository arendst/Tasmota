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
/*********************************************************************************************\
 * Arduino slave
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

#include <TasmotaSerial.h>
#include <ArduinoHexParse.h>

struct ASLAVE {
  uint32_t spi_hex_size = 0;
  uint32_t spi_sector_counter = 0;
  uint8_t spi_sector_cursor = 0;
  uint8_t inverted = LOW;
  bool type = false;
  bool flashing  = false;
} ASlave;

TasmotaSerial *ArduinoSlave_Serial;

uint32_t ArduinoSlaveFlashStart(void)
{
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;  // Stay on the safe side
}

uint8_t ArduinoSlave_UpdateInit(void)
{
  ASlave.spi_hex_size = 0;
  ASlave.spi_sector_counter = ArduinoSlaveFlashStart();  // Reset the pre-defined write address where firmware will temporarily be stored
  ASlave.spi_sector_cursor = 0;
  return 0;
}

void ArduinoSlave_Reset(void)
{
  if (ASlave.type) {
    digitalWrite(pin[GPIO_ARDUINO_RST], !ASlave.inverted);
    delay(1);
    digitalWrite(pin[GPIO_ARDUINO_RST], ASlave.inverted);
    delay(1);
    digitalWrite(pin[GPIO_ARDUINO_RST], !ASlave.inverted);
    delay(5);
  }
}

uint8_t ArduinoSlave_waitForSerialData(int dataCount, int timeout)
{
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

uint8_t ArduinoSlave_sendBytes(uint8_t* bytes, int count)
{
  ArduinoSlave_Serial->write(bytes, count);
  ArduinoSlave_waitForSerialData(2, 1000);
  uint8_t sync = ArduinoSlave_Serial->read();
  uint8_t ok = ArduinoSlave_Serial->read();
  if (sync == 0x14 && ok == 0x10) {
    return 1;
  }
  return 0;
}

uint8_t ArduinoSlave_execCmd(uint8_t cmd)
{
  uint8_t bytes[] = { cmd, CONST_STK_CRC_EOP };
  return ArduinoSlave_sendBytes(bytes, 2);
}

uint8_t ArduinoSlave_execParam(uint8_t cmd, uint8_t* params, int count)
{
  uint8_t bytes[32];
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
  uint8_t ProgParams[] = {0x86,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0xff,0xff,0xff,0xff,0x00,0x80,0x04,0x00,0x00,0x00,0x80,0x00};
  uint8_t ExtProgParams[] = {0x05,0x04,0xd7,0xc2,0x00};
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

uint8_t ArduinoSlave_loadAddress(uint8_t adrHi, uint8_t adrLo)
{
  uint8_t params[] = { adrHi, adrLo };
  return ArduinoSlave_execParam(CMND_STK_LOAD_ADDRESS, params, sizeof(params));
}

void ArduinoSlave_FlashPage(uint8_t* address, uint8_t* data)
{
  uint8_t Header[] = {CMND_STK_PROG_PAGE, 0x00, 0x80, 0x46};
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
  uint32_t flash_start = ArduinoSlaveFlashStart() * SPI_FLASH_SEC_SIZE;
  while (reading) {
    ESP.flashRead(flash_start + read, (uint32_t*)flash_buffer, SPI_FLASH_SEC_SIZE);
    read = read + SPI_FLASH_SEC_SIZE;
    if (read >= ASlave.spi_hex_size) {
      reading = false;
    }
    for (uint32_t ca = 0; ca < SPI_FLASH_SEC_SIZE; ca++) {
      processed++;
      if (processed <= ASlave.spi_hex_size) {
        if (':' == flash_buffer[ca]) {
          position = 0;
        }
        if (0x0D == flash_buffer[ca]) {
          thishexline[position] = 0;
          hexParse.ParseLine((uint8_t*)thishexline);
          if (hexParse.IsFlashPageReady()) {
            uint8_t* page = hexParse.GetFlashPage();
            uint8_t* address = hexParse.GetLoadAddress();
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
  ASlave.flashing  = false;
  ArduinoSlave_exitProgMode();
  restart_flag = 2;
}

void ArduinoSlave_SetFlagFlashing(bool value)
{
  ASlave.flashing  = value;
}

bool ArduinoSlave_GetFlagFlashing(void)
{
  return ASlave.flashing ;
}

void ArduinoSlave_WriteBuffer(uint8_t *buf, size_t size)
{
  if (0 == ASlave.spi_sector_cursor) { // Starting a new sector write so we need to erase it first
    ESP.flashEraseSector(ASlave.spi_sector_counter);
  }
  ASlave.spi_sector_cursor++;
  ESP.flashWrite((ASlave.spi_sector_counter * SPI_FLASH_SEC_SIZE) + ((ASlave.spi_sector_cursor-1)*2048), (uint32_t*)buf, size);
  ASlave.spi_hex_size = ASlave.spi_hex_size + size;
  if (2 == ASlave.spi_sector_cursor) {  // The web upload sends 2048 bytes at a time so keep track of the cursor position to reset it for the next flash sector erase
    ASlave.spi_sector_cursor = 0;
    ASlave.spi_sector_counter++;
  }
}

void ArduinoSlave_Init(void)
{
  if (ASlave.type) {
    return;
  }
  if ((pin[GPIO_ARDUINO_RXD] < 99) && (pin[GPIO_ARDUINO_TXD] < 99) &&
      ((pin[GPIO_ARDUINO_RST] < 99) || (pin[GPIO_ARDUINO_RST_INV] < 99))) {
    ArduinoSlave_Serial = new TasmotaSerial(pin[GPIO_ARDUINO_RXD], pin[GPIO_ARDUINO_TXD], 1, 0, 200);
    if (ArduinoSlave_Serial->begin(USE_ARDUINO_SERIAL_SPEED)) {
      if (ArduinoSlave_Serial->hardwareSerial()) {
        ClaimSerial();
      }
      if (pin[GPIO_ARDUINO_RST_INV] < 99) {
        pin[GPIO_ARDUINO_RST] = pin[GPIO_ARDUINO_RST_INV];
        pin[GPIO_ARDUINO_RST_INV] = 99;
        ASlave.inverted = HIGH;
      }
      pinMode(pin[GPIO_ARDUINO_RST], OUTPUT);
      ASlave.type = true;
      ArduinoSlave_Reset();
      AddLog_P2(LOG_LEVEL_INFO, PSTR("Arduino Slave Enabled"));
    }
  }
}

void ArduinoSlave_Show(bool json)
{
  if (ASlave.type) {
    ArduinoSlave_Serial->flush();
    ArduinoSlave_Serial->print("JSON");
    ArduinoSlave_Serial->find(char(0xFE));
    char buffer[100];
    uint16_t haveread = ArduinoSlave_Serial->readBytesUntil(char(0xFF), buffer, sizeof(buffer)-1);
    buffer[haveread] = '\0';
    if (json) {
      ResponseAppend_P(PSTR(",\"ArduinoSlave\":%s"), buffer);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

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
  }
  return result;
}

#endif  // USE_ARDUINO_SLAVE