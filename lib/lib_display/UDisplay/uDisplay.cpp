/*
  uDisplay.cpp -  universal display driver support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and  Theo Arends

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

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "uDisplay.h"


const uint16_t udisp_colors[]={UDISP_BLACK,UDISP_WHITE,UDISP_RED,UDISP_GREEN,UDISP_BLUE,UDISP_CYAN,UDISP_MAGENTA,\
  UDISP_YELLOW,UDISP_NAVY,UDISP_DARKGREEN,UDISP_DARKCYAN,UDISP_MAROON,UDISP_PURPLE,UDISP_OLIVE,\
UDISP_LIGHTGREY,UDISP_DARKGREY,UDISP_ORANGE,UDISP_GREENYELLOW,UDISP_PINK};

uint16_t uDisplay::GetColorFromIndex(uint8_t index) {
  if (index >= sizeof(udisp_colors) / 2) index = 0;
  return udisp_colors[index];
}

extern uint8_t *buffer;

uDisplay::uDisplay(char *lp) : Renderer(800, 600) {
  // analyse decriptor
  uint8_t section = 0;
  dsp_ncmds = 0;
  char linebuff[128];
  while (*lp) {

    uint16_t llen = strlen_ln(lp);
    strncpy(linebuff, lp, llen);
    linebuff[llen] = 0;
    lp += llen;
    char *lp1 = linebuff;

    if (*lp1 == '#') break;
    if (*lp1 == '\n') lp1++;
    while (*lp1 == ' ') lp1++;
    //Serial.printf(">> %s\n",lp1);
    if (*lp1 != ';') {
      // check ids:
      if (*lp1 == ':') {
        // id line
        lp1++;
        section = *lp1++;
      } else {
        switch (section) {
          case 'H':
            // header line
            // SD1306,128,64,1,I2C,5a,*,*,*
            str2c(&lp1, dname, sizeof(dname));
            char ibuff[16];
            gxs = next_val(&lp1);
            setwidth(gxs);
            gys = next_val(&lp1);
            setheight(gys);
            bpp = next_val(&lp1);
            str2c(&lp1, ibuff, sizeof(ibuff));
            if (!strncmp(ibuff, "I2C", 3)) {
              interface = _UDSP_I2C;
              i2caddr = next_hex(&lp1);
              i2c_scl = next_val(&lp1);
              i2c_sda = next_val(&lp1);
              reset = next_val(&lp1);
              section = 0;
            } else if (!strncmp(ibuff, "SPI", 3)) {
              interface = _UDSP_SPI;
              spi_nr = next_val(&lp1);
              spi_cs = next_val(&lp1);
              spi_clk = next_val(&lp1);
              spi_mosi = next_val(&lp1);
              spi_dc = next_val(&lp1);
              bpanel = next_val(&lp1);
              reset = next_val(&lp1);
              spi_miso = next_val(&lp1);
              spi_speed = next_val(&lp1);

              section = 0;
              Serial.printf("%d %d %d %d %d %d %d %d\n", spi_cs, spi_clk, spi_mosi, spi_dc, bpanel, reset, spi_miso, spi_speed);
            }
            break;
          case 'S':
            splash_font = next_val(&lp1);
            splash_size = next_val(&lp1);
            fg_col = next_val(&lp1);
            if (bpp == 16) {
              fg_col = GetColorFromIndex(fg_col);
            }
            bg_col = next_val(&lp1);
            if (bpp == 16) {
              bg_col = GetColorFromIndex(bg_col);
            }
            splash_xp = next_val(&lp1);
            splash_yp = next_val(&lp1);
            break;
          case 'I':
            // init data
            if (interface == _UDSP_I2C) {
              dsp_cmds[dsp_ncmds++] = next_hex(&lp1);
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                dsp_cmds[dsp_ncmds++] = strtol(ibuff, 0, 16);
              }
            } else {
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  dsp_cmds[dsp_ncmds++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
                if (dsp_ncmds >= sizeof(dsp_cmds)) break;

              }
            }
            break;
          case 'o':
            str2c(&lp1, ibuff, sizeof(ibuff));
            dsp_off = strtol(ibuff, 0, 16);
            break;
          case 'O':
            str2c(&lp1, ibuff, sizeof(ibuff));
            dsp_on = strtol(ibuff, 0, 16);
            break;
          case '0':
            rot_0 = next_hex(&lp1);
            break;
          case '1':
            rot_1 = next_hex(&lp1);
            break;
          case '2':
            rot_2 = next_hex(&lp1);
            break;
          case '3':
            rot_3 = next_hex(&lp1);
            break;
          case 'A':
            saw_1 = next_hex(&lp1);
            saw_2 = next_hex(&lp1);
            saw_3 = next_hex(&lp1);
            break;
        }
      }
    }
    if (*lp == '\n') {
      lp++;
    } else {
      lp = strchr(lp, '\n');
      if (!lp) break;
      lp++;
    }
  }
}


Renderer *uDisplay::Init(void) {


  if (reset >= 0) {
    pinMode(reset, OUTPUT);
    digitalWrite(reset, HIGH);
    delay(50);
    digitalWrite(reset, LOW);
    delay(50);
    digitalWrite(reset, HIGH);
    delay(200);
  }

  if (interface == _UDSP_I2C) {
    Wire.begin(i2c_sda, i2c_scl);
    if (bpp < 16) {
      if (buffer) free(buffer);
      buffer = (uint8_t*)calloc((width()*height()*bpp)/8, 1);

      for (uint32_t cnt = 0; cnt < dsp_ncmds; cnt++) {
        i2c_command(dsp_cmds[cnt]);
      }
    }
  }
  if (interface == _UDSP_SPI) {
    if (bpanel >= 0) {
#ifdef ESP32
        ledcSetup(ESP32_PWM_CHANNEL, 4000, 8);
        ledcAttachPin(bpanel, ESP32_PWM_CHANNEL);
        ledcWrite(ESP32_PWM_CHANNEL, 128);
#else
        pinMode(bpanel, OUTPUT);
        digitalWrite(bpanel, HIGH);
#endif // ESP32
    }
    if (spi_dc >= 0) {
      pinMode(spi_dc, OUTPUT);
      digitalWrite(spi_dc, HIGH);
    }
    if (spi_cs >= 0) {
      pinMode(spi_cs, OUTPUT);
      digitalWrite(spi_cs, HIGH);
    }

    spiSettings = SPISettings(spi_speed, MSBFIRST, SPI_MODE3);

#ifdef ESP8266
    SPI.begin();
    uspi = &SPI;
#else
    if (spi_nr != 1) {
      uspi = new SPIClass(HSPI);
    } else {
      uspi = &SPI;
    }
    uspi->begin(spi_clk, spi_miso, spi_mosi, -1);
#endif

    uint16_t index = 0;

    SPI_BEGIN_TRANSACTION
    while (1) {
      uint8_t iob;
      SPI_CS_LOW
      SPI_DC_LOW
      iob = dsp_cmds[index++];
      uspi->write(iob);
      SPI_DC_HIGH
      uint8_t args = dsp_cmds[index++];
      //Serial.printf("cmd, args %x, %d ", iob, args&0x7f);
      for (uint32_t cnt = 0; cnt < (args & 0x7f); cnt++) {
        iob = dsp_cmds[index++];
        //Serial.printf("%02x ", iob );
        uspi->write(iob);
      }
      SPI_CS_HIGH
      //Serial.printf("\n");
      if (args & 0x80) delay(120);
      if (index >= dsp_ncmds) break;
    }
    SPI_END_TRANSACTION

  }
  return this;
}

void uDisplay::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
    setRotation(rot);
    invertDisplay(false);
    setTextWrap(false);
    cp437(true);
    setTextFont(font);
    setTextSize(size);
    setTextColor(fg_col, bg_col);
    setCursor(0,0);
    fillScreen(bg_col);
    Updateframe();
}

void uDisplay::spi_command(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_DC_LOW
  SPI_CS_LOW
  uspi->write(val);
  SPI_CS_HIGH
  SPI_DC_HIGH
  SPI_END_TRANSACTION
}

void uDisplay::i2c_command(uint8_t val) {
  //Serial.printf("%02x\n",val );
  Wire.beginTransmission(i2caddr);
  Wire.write(0);
  Wire.write(val);
  Wire.endTransmission();
}

#define SH1106_SETLOWCOLUMN 0
#define SH1106_SETHIGHCOLUMN 0x10
#define SH1106_SETSTARTLINE 0x40


void uDisplay::Updateframe(void) {

  if (interface == _UDSP_I2C) {
    i2c_command(SH1106_SETLOWCOLUMN | 0x0);  // low col = 0
    i2c_command(SH1106_SETHIGHCOLUMN | 0x0);  // hi col = 0
    i2c_command(SH1106_SETSTARTLINE | 0x0); // line #0

	  uint8_t ys = gys >> 3;
	  uint8_t xs = gxs >> 3;
    //uint8_t xs = 132 >> 3;
	  uint8_t m_row = 0;
	  uint8_t m_col = 2;

	  uint16_t p = 0;

	  uint8_t i, j, k = 0;

	  for ( i = 0; i < ys; i++) {
		    // send a bunch of data in one xmission
        i2c_command(0xB0 + i + m_row);//set page address
        i2c_command(m_col & 0xf);//set lower column address
        i2c_command(0x10 | (m_col >> 4));//set higher column address

        for( j = 0; j < 8; j++){
			      Wire.beginTransmission(i2caddr);
            Wire.write(0x40);
            for ( k = 0; k < xs; k++, p++) {
		            Wire.write(buffer[p]);
            }
            Wire.endTransmission();
        	}
	      }
    }
}

void uDisplay::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {

  if (interface != _UDSP_SPI) {
    Renderer::drawFastVLine(x, y, h, color);
    return;
  }
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((y + h - 1) >= _height) h = _height - y;

  SPI_BEGIN_TRANSACTION

  SPI_CS_LOW

  setAddrWindow_int(x, y, 1, h);

  while (h--) {
    uspi->write16(color);
  }

  SPI_CS_HIGH

  SPI_END_TRANSACTION
}

void uDisplay::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {

  if (interface != _UDSP_SPI) {
    Renderer::drawFastHLine(x, y, w, color);
    return;
  }

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;


  SPI_BEGIN_TRANSACTION

  SPI_CS_LOW

  setAddrWindow_int(x, y, w, 1);


  while (w--) {
    uspi->write16(color);
  }

  SPI_CS_HIGH

  SPI_END_TRANSACTION
}

void uDisplay::fillScreen(uint16_t color) {
  fillRect(0, 0,  gxs, gys, color);
}

// fill a rectangle
void uDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

  if (interface != _UDSP_SPI) {
    Renderer::fillRect(x, y, w, h, color);
    return;
  }

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= gxs) || (y >= gys)) return;
  if((x + w - 1) >= gxs)  w = gxs  - x;
  if((y + h - 1) >= gys) h = gys - y;


  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW

  setAddrWindow_int(x, y, w, h);

  for (y = h; y > 0; y--) {
    for (x = w; x > 0; x--) {
      uspi->write16(color);
    }
  }
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}


void uDisplay::Splash(void) {
  setTextFont(splash_font);
  setTextSize(splash_size);
  DrawStringAt(splash_xp, splash_yp, dname, fg_col, 0);
  Updateframe();
}

void uDisplay::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  if (!x0 && !y0 && !x1 && !y1) {
    SPI_CS_HIGH
    SPI_END_TRANSACTION
  } else {
    SPI_CS_LOW
    SPI_BEGIN_TRANSACTION
    setAddrWindow_int(x0, y0, x1 - x0, y1 - y0 );
  }
}

void uDisplay::setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
    uint32_t ya = ((uint32_t)y << 16) | (y+h-1);

    SPI_DC_LOW
    uspi->write(saw_1);
    SPI_DC_HIGH

    uspi->write32(xa);

    SPI_DC_LOW
    uspi->write(saw_2);
    SPI_DC_HIGH

    uspi->write32(ya);

    SPI_DC_LOW
    uspi->write(saw_3); // write to RAM
    SPI_DC_HIGH

}

void uDisplay::pushColors(uint16_t *data, uint16_t len, boolean first) {
  uint16_t color;

  while (len--) {
    color = *data++;
    uspi->write16(color);
  }

}

void uDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if (interface != _UDSP_SPI) {
    Renderer::drawPixel(x, y, color);
    return;
  }

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;


  SPI_BEGIN_TRANSACTION

  SPI_CS_LOW

  setAddrWindow_int(x, y, 1, 1);

  uspi->write16(color);

  SPI_CS_HIGH

  SPI_END_TRANSACTION
}

void uDisplay::setRotation(uint8_t m) {
  if (interface != _UDSP_SPI) {
    Renderer::setRotation(m);
    return;
  }
  switch (rotation) {
    case 0:
      if (interface == _UDSP_SPI) spi_command(rot_0);
      _width  = gxs;
      _height = gys;
      break;
    case 1:
      if (interface == _UDSP_SPI) spi_command(rot_1);
      _width  = gys;
      _height = gxs;
      break;
    case 2:
      if (interface == _UDSP_SPI) spi_command(rot_2);
      _width  = gxs;
      _height = gys;
      break;
    case 3:
      if (interface == _UDSP_SPI) spi_command(rot_3);
      _width  = gys;
      _height = gxs;
      break;
  }
}

void uDisplay::DisplayOnff(int8_t on) {

  if (interface == _UDSP_I2C) {
    if (on) {
      i2c_command(dsp_on);
    } else {
      i2c_command(dsp_off);
    }
  } else {
    if (on) {
      spi_command(dsp_on);
      if (bpanel >= 0) {
#ifdef ESP32
        ledcWrite(ESP32_PWM_CHANNEL, dimmer);
#else
        digitalWrite(bpanel, HIGH);
#endif
      }

    } else {
      spi_command(dsp_off);
      if (bpanel >= 0) {
#ifdef ESP32
        ledcWrite(ESP32_PWM_CHANNEL, 0);
#else
        digitalWrite(bpanel, LOW);
#endif
      }
    }
  }
}

void uDisplay::dim(uint8_t dim) {
  dimmer = dim;
  if (dimmer > 15) dimmer = 15;
  dimmer = ((float)dimmer / 15.0) * 255.0;
#ifdef ESP32
  ledcWrite(ESP32_PWM_CHANNEL, dimmer);
#endif
}



uint8_t uDisplay::strlen_ln(char *str) {
  for (uint32_t cnt = 0; cnt < 256; cnt++) {
    if (!str[cnt] || str[cnt] == '\n') return cnt;
  }
  return 0;
}

char *uDisplay::devname(void) {
  return dname;
}

uint32_t uDisplay::str2c(char **sp, char *vp, uint32_t len) {
    char *lp = *sp;
    if (len) len--;
    char *cp = strchr(lp, ',');
    if (cp) {
        while (1) {
            if (*lp == ',') {
                *vp = 0;
                *sp = lp + 1;
                return 0;
            }
            if (len) {
                *vp++ = *lp++;
                len--;
            } else {
                lp++;
            }
        }
    } else {
      uint8_t slen = strlen(lp);
      if (slen) {
        strlcpy(vp, *sp, len);
        *sp = lp + slen;
        return 0;
      }
    }
    return 1;
}

int32_t uDisplay::next_val(char **sp) {
  char ibuff[16];
  str2c(sp, ibuff, sizeof(ibuff));
  return atoi(ibuff);
}

uint32_t uDisplay::next_hex(char **sp) {
  char ibuff[16];
  str2c(sp, ibuff, sizeof(ibuff));
  return strtol(ibuff, 0, 16);
}
