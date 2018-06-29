/*
  xsns_29_mcp23008.ino - Support for I2C GPIO Expander

  Copyright (C) 2018  Andre Thomas and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_MCP23008

/*********************************************************************************************\
 * MCP23008 - I2C GPIO EXPANDER
 *
 * Docs at https://www.microchip.com/wwwproducts/en/MCP23008
 *
 * I2C Address: 0x20 - 0x27
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_MCP23008_GPIO[] PROGMEM = "%s{s}%s MCP23008 D%d{m}%d{e}";                               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif

#define MCP23008_ADDRESS1					0x20
#define MCP23008_ADDRESS2					0x21
#define MCP23008_ADDRESS3					0x22
#define MCP23008_ADDRESS4					0x23
#define MCP23008_ADDRESS5					0x24
#define MCP23008_ADDRESS6					0x25
#define MCP23008_ADDRESS7					0x26
#define MCP23008_ADDRESS8					0x27

#define MCP23008_IODIR            0x00
#define MCP23008_IPOL             0x01
#define MCP23008_GPINTEN          0x02
#define MCP23008_DEFVAL           0x03
#define MCP23008_INTCON           0x04
#define MCP23008_IOCON            0x05
#define MCP23008_GPPU             0x06
#define MCP23008_INTF             0x07
#define MCP23008_INTCAP           0x08
#define MCP23008_GPIO             0x09
#define MCP23008_OLAT             0x0A

uint8_t mcp23008_type = 0;
uint8_t mcp23008_address;
uint8_t mcp23008_addresses[] = { MCP23008_ADDRESS1, MCP23008_ADDRESS2, MCP23008_ADDRESS3, MCP23008_ADDRESS4, MCP23008_ADDRESS5, MCP23008_ADDRESS6, MCP23008_ADDRESS7, MCP23008_ADDRESS8 };

void MCP23008_pinMode(uint8_t pin,uint8_t dir) {
  uint8_t io_direction = I2cRead8(mcp23008_address, MCP23008_IODIR);
  if (dir == INPUT) {
    io_direction |= 1 << pin;
  } else {
    io_direction &= ~(1 << pin);
  }
  I2cWrite8(mcp23008_address,MCP23008_IODIR,io_direction);
}

uint8_t MCP23008_readGPIO(void) {
  return I2cRead8(mcp23008_address,MCP23008_GPIO);
}

void MCP23008_writeGPIO(uint8_t gpio) {
  I2cWrite8(mcp23008_address,MCP23008_GPIO,gpio);
}

void MCP23008_digitalWrite(uint8_t pin, uint8_t state) {
  uint8_t gpio = MCP23008_readGPIO();
  if (state == HIGH) {
    gpio |= 1 << pin; 
  } else {
    gpio &= ~(1 << pin);
  }
  MCP23008_writeGPIO(gpio);
}

void MCP23008_pullUp(uint8_t pin, uint8_t state) {
  uint8_t pullupstate = I2cRead8(mcp23008_address,MCP23008_GPPU);
  if (state == HIGH) {
    pullupstate |= 1 << pin;
  } else {
    pullupstate &= ~(1 << pin);
  }
  I2cWrite8(mcp23008_address,MCP23008_GPPU,pullupstate);
}

uint8_t MCP23008_digitalRead(uint8_t pin) {
  return(MCP23008_readGPIO() >> pin) & 0x01;
}

void MCP23008_Detect()
{
  uint8_t buffer;

  if (mcp23008_type) { return; }

  for (byte i = 0; i < sizeof(mcp23008_addresses); i++) {
    mcp23008_address = mcp23008_addresses[i];
    if (I2cValidRead8(&buffer, mcp23008_address, MCP23008_IODIR)) { // Can we get a valid response? If so, then chip must be valid for this address
      mcp23008_type = 1;
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "MCP23008", mcp23008_address);
      AddLog(LOG_LEVEL_DEBUG);
      break;
    }
  }
}

void MCP23008Show(boolean json)
{
  if (mcp23008_type) {
    char temperature[10];

    float t = 0;
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP23008\":{\"D0\":%i, \"D1\":%i, \"D2\":%i, \"D3\":%i, \"D4\":%i, \"D5\":%i, \"D6\":%i, \"D7\":%i }"),
                          mqtt_data, MCP23008_digitalRead(0), MCP23008_digitalRead(1), MCP23008_digitalRead(2), MCP23008_digitalRead(3), MCP23008_digitalRead(4), MCP23008_digitalRead(5), MCP23008_digitalRead(6), MCP23008_digitalRead(7));
#ifdef USE_WEBSERVER
    } else {
      for (uint8_t pin=0;pin<8;pin++) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_MCP23008_GPIO, mqtt_data, "", pin, MCP23008_digitalRead(pin));
      }
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_29

boolean Xsns29(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        MCP23008_Detect();
        break;
      case FUNC_JSON_APPEND:
        MCP23008Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        MCP23008Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MCP23008
#endif  // USE_I2C
