/*
  uDisplay_lvgl.cpp -  universal display driver support for Tasmota

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
#include "uDisplay_lvgl.h"

#define GPIO_OLED_RESET     32
#define GPIO_I2C_SCL        19
#define GPIO_I2C_SDA        20
#define GPIO_BACKLIGHT      31
#define GPIO_SPI_CS         24
#define GPIO_SPI_CLK        23
#define GPIO_SPI_MOSI       22
#define GPIO_SPI_DC         25
#define GPIO_SPI_MISO       21
#define GPIO_SSPI_CS        29
#define GPIO_SSPI_SCLK      28
#define GPIO_SSPI_MOSI      27
#define GPIO_SSPI_DC        30
#define GPIO_SSPI_MISO      26

extern int Pin(uint32_t gpio, uint32_t index = 0);
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
#define LOG_LEVEL_DEBUG  3

#define UDSP_DEBUG

const uint16_t udisp_colors[]={UDISP_BLACK,UDISP_WHITE,UDISP_RED,UDISP_GREEN,UDISP_BLUE,UDISP_CYAN,UDISP_MAGENTA,\
  UDISP_YELLOW,UDISP_NAVY,UDISP_DARKGREEN,UDISP_DARKCYAN,UDISP_MAROON,UDISP_PURPLE,UDISP_OLIVE,\
UDISP_LIGHTGREY,UDISP_DARKGREY,UDISP_ORANGE,UDISP_GREENYELLOW,UDISP_PINK};

uint16_t uDisplay_lvgl::GetColorFromIndex(uint8_t index) {
  if (index >= sizeof(udisp_colors) / 2) index = 0;
  return udisp_colors[index];
}

extern uint8_t *buffer;
extern uint8_t color_type;

uDisplay_lvgl::uDisplay_lvgl(char *lp) {
  // analyse decriptor
  col_mode = 16;
  sa_mode = 16;
  saw_3 = 0xff;
  dim_op = 0xff;
  dsp_off = 0xff;
  dsp_on = 0xff;
  lutpsize = 0;
  lutfsize = 0;
  lutptime = 35;
  lutftime = 350;
  lut3time = 10;
  ep_mode = 0;
  startline = 0xA1;
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
    if (*lp1 == ' ') lp1++;   // Add space char
    while (*lp1 == ' ') lp1++;
    //Serial.printf(">> %s\n",lp1);
    if (*lp1 != ';') {
      // check ids:
      if (*lp1 == ':') {
        // id line
        lp1++;
        section = *lp1++;
        if (*lp1 == ',') lp1++;
      }
      if (*lp1 != ':' && *lp1 != '\n' && *lp1 != ' ') {   // Add space char
        switch (section) {
          case 'H':
            // header line
            // SD1306,128,64,1,I2C,5a,*,*,*
            str2c(&lp1, dname, sizeof(dname));
            char ibuff[16];
            gxs = next_val(&lp1);
            gys = next_val(&lp1);
            bpp = next_val(&lp1);
            if (bpp == 1) {
              color_type = uCOLOR_BW;
            } else {
              color_type = uCOLOR_COLOR;
            }
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
            }
            break;
          // case 'S':
          //   splash_font = next_val(&lp1);
          //   splash_size = next_val(&lp1);
          //   fg_col = next_val(&lp1);
          //   if (bpp == 16) {
          //     fg_col = GetColorFromIndex(fg_col);
          //   }
          //   bg_col = next_val(&lp1);
          //   if (bpp == 16) {
          //     bg_col = GetColorFromIndex(bg_col);
          //   }
          //   splash_xp = next_val(&lp1);
          //   splash_yp = next_val(&lp1);
          //   break;
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
            dsp_off = next_hex(&lp1);
            break;
          case 'O':
            dsp_on = next_hex(&lp1);
            break;
          case 'R':
            madctrl = next_hex(&lp1);
            startline = next_hex(&lp1);
            break;
          case '0':
            rot[0] = next_hex(&lp1);
            x_addr_offs[0] = next_hex(&lp1);
            y_addr_offs[0] = next_hex(&lp1);
            rot_t[0] = next_hex(&lp1);
            break;
          case '1':
            rot[1] = next_hex(&lp1);
            x_addr_offs[1] = next_hex(&lp1);
            y_addr_offs[1] = next_hex(&lp1);
            rot_t[1] = next_hex(&lp1);
            break;
          case '2':
            rot[2] = next_hex(&lp1);
            x_addr_offs[2] = next_hex(&lp1);
            y_addr_offs[2] = next_hex(&lp1);
            rot_t[2] = next_hex(&lp1);
            break;
          case '3':
            rot[3] = next_hex(&lp1);
            x_addr_offs[3] = next_hex(&lp1);
            y_addr_offs[3] = next_hex(&lp1);
            rot_t[3] = next_hex(&lp1);
            break;
          case 'A':
            if (interface == _UDSP_I2C) {
              saw_1 = next_hex(&lp1);
              i2c_page_start = next_hex(&lp1);
              i2c_page_end = next_hex(&lp1);
              saw_2 = next_hex(&lp1);
              i2c_col_start = next_hex(&lp1);
              i2c_col_end = next_hex(&lp1);
              saw_3 = next_hex(&lp1);
            } else {
              saw_1 = next_hex(&lp1);
              saw_2 = next_hex(&lp1);
              saw_3 = next_hex(&lp1);
              sa_mode = next_val(&lp1);
            }
            break;
          case 'P':
            col_mode = next_val(&lp1);
            break;
          case 'i':
            inv_off = next_hex(&lp1);
            inv_on = next_hex(&lp1);
            break;
          case 'D':
            dim_op = next_hex(&lp1);
            break;
          case 'L':
            while (1) {
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                lut_full[lutfsize++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
              if (lutfsize >= sizeof(lut_full)) break;
            }
            break;
          case 'l':
            while (1) {
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                lut_partial[lutpsize++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
              if (lutpsize >= sizeof(lut_partial)) break;
            }
            break;
          case 'T':
            lutftime = next_val(&lp1);
            lutptime = next_val(&lp1);
            lut3time = next_val(&lp1);
            break;
        }
      }
    }
    if (*lp == '\n' || *lp == ' ') {   // Add space char
      lp++;
    } else {
      lp = strchr(lp, '\n');
      if (!lp) {
        lp = strchr(lp, ' ');
        if (!lp) {
          break;
        }
      }
      lp++;
    }
  }

  if (lutfsize && lutpsize) {
    ep_mode = 1;
  }
}


void uDisplay_lvgl::Init(void) {

  // Check pins against configuration
  if (reset < 0) { reset = Pin(GPIO_OLED_RESET); }
  if (i2c_scl < 0) { i2c_scl = Pin(GPIO_I2C_SCL); }
  if (i2c_sda < 0) { i2c_scl = Pin(GPIO_I2C_SDA); }
  if (bpanel < 0) { spi_cs = Pin(GPIO_BACKLIGHT); }
  if ((spi_nr == 1) || (spi_nr == 2)) { //SPI0
    if (spi_cs < 0) { spi_cs = Pin(GPIO_SPI_CS, spi_nr - 1); }
    if (spi_clk < 0) { spi_clk = Pin(GPIO_SPI_CLK, spi_nr - 1); }
    if (spi_mosi < 0) { spi_mosi = Pin(GPIO_SPI_MOSI, spi_nr - 1); }
    if (spi_dc < 0) { spi_dc = Pin(GPIO_SPI_DC, spi_nr - 1); }
    if (spi_miso < 0) { spi_miso = Pin(GPIO_SPI_MISO, spi_nr - 1); }
  } else {
    if (spi_cs < 0) { spi_cs = Pin(GPIO_SSPI_CS); }
    if (spi_clk < 0) { spi_clk = Pin(GPIO_SSPI_SCLK); }
    if (spi_mosi < 0) { spi_mosi = Pin(GPIO_SSPI_MOSI); }
    if (spi_dc < 0) { spi_dc = Pin(GPIO_SSPI_DC); }
    if (spi_miso < 0) { spi_miso = Pin(GPIO_SSPI_MISO); }
  }
#ifdef UDSP_DEBUG

  Serial.printf("xs : %d\n", gxs);
  Serial.printf("ys : %d\n", gys);
  Serial.printf("bpp: %d\n", bpp);

  if (interface == _UDSP_SPI) {
    Serial.printf("Nr. : %d\n", spi_nr);
    Serial.printf("CS  : %d\n", spi_cs);
    Serial.printf("CLK : %d\n", spi_clk);
    Serial.printf("MOSI: %d\n", spi_mosi);
    Serial.printf("DC  : %d\n", spi_dc);
    Serial.printf("BPAN: %d\n", bpanel);
    Serial.printf("RES : %d\n", reset);
    Serial.printf("MISO: %d\n", spi_miso);
    Serial.printf("SPED: %d\n", spi_speed*1000000);
    Serial.printf("Pixels: %d\n", col_mode);
    Serial.printf("SaMode: %d\n", sa_mode);

    Serial.printf("opts: %02x,%02x,%02x\n", saw_3, dim_op, startline);

    Serial.printf("SetAddr : %x,%x,%x\n", saw_1, saw_2, saw_3);

    Serial.printf("Rot 0: %x,%x - %d - %d\n", madctrl, rot[0], x_addr_offs[0], y_addr_offs[0]);

    if (ep_mode) {
      Serial.printf("LUT_Partial : %d\n", lutpsize);
      Serial.printf("LUT_Full : %d\n", lutfsize);
    }
  }
  if (interface == _UDSP_I2C) {
    Serial.printf("Addr : %02x\n", i2caddr);
    Serial.printf("SCL  : %d\n", i2c_scl);
    Serial.printf("SDA  : %d\n", i2c_sda);

    Serial.printf("SPA   : %x\n", saw_1);
    Serial.printf("pa_sta: %x\n", i2c_page_start);
    Serial.printf("pa_end: %x\n", i2c_page_end);
    Serial.printf("SCA   : %x\n", saw_2);
    Serial.printf("ca_sta: %x\n", i2c_col_start);
    Serial.printf("pa_end: %x\n", i2c_col_end);
    Serial.printf("WRA   : %x\n", saw_3);
  }
#endif

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
    wire = &Wire;
    wire->begin(i2c_sda, i2c_scl);
    if (bpp < 16) {
      if (buffer) free(buffer);
#ifdef ESP8266
      buffer = (uint8_t*)calloc((gxs * gys * bpp) / 8, 1);
#else
      if (psramFound()) {
        buffer = (uint8_t*)heap_caps_malloc((gxs * gys * bpp) / 8, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      } else {
        buffer = (uint8_t*)calloc((gxs * gys * bpp) / 8, 1);
      }
#endif
    }

#ifdef UDSP_DEBUG
    Serial.printf("I2C cmds: %d\n", dsp_ncmds);
#endif
    for (uint32_t cnt = 0; cnt < dsp_ncmds; cnt++) {
      i2c_command(dsp_cmds[cnt]);
#ifdef UDSP_DEBUG
      Serial.printf("cmd = %x\n", dsp_cmds[cnt]);
#endif
    }

  }
  if (interface == _UDSP_SPI) {

    if (ep_mode) {
  #ifdef ESP8266
      buffer = (uint8_t*)calloc((gxs * gys * bpp) / 8, 1);
  #else
      if (psramFound()) {
        buffer = (uint8_t*)heap_caps_malloc((gxs * gys * bpp) / 8, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      } else {
        buffer = (uint8_t*)calloc((gxs * gys * bpp) / 8, 1);
      }
      #endif
    }

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

#ifdef ESP8266
    if (spi_nr <= 1) {
      SPI.begin();
      uspi = &SPI;
    } else {
      pinMode(spi_clk, OUTPUT);
      digitalWrite(spi_clk, LOW);
      pinMode(spi_mosi, OUTPUT);
      digitalWrite(spi_mosi, LOW);
    }
#endif // ESP8266

#ifdef ESP32
    if (spi_nr == 1) {
      uspi = &SPI;
      uspi->begin(spi_clk, spi_miso, spi_mosi, -1);
    } else if (spi_nr == 2) {
      uspi = new SPIClass(HSPI);
      uspi->begin(spi_clk, spi_miso, spi_mosi, -1);
    } else {
      pinMode(spi_clk, OUTPUT);
      digitalWrite(spi_clk, LOW);
      pinMode(spi_mosi, OUTPUT);
      digitalWrite(spi_mosi, LOW);
    }
#endif // ESP32

    spiSettings = SPISettings((uint32_t)spi_speed*1000000, MSBFIRST, SPI_MODE3);

    uint16_t index = 0;

    SPI_BEGIN_TRANSACTION
    while (1) {
      uint8_t iob;
      SPI_CS_LOW

      iob = dsp_cmds[index++];
      spi_command(iob);

      uint8_t args = dsp_cmds[index++];
#ifdef UDSP_DEBUG
      Serial.printf("cmd, args %02x, %d ", iob, args&0x1f);
#endif
      for (uint32_t cnt = 0; cnt < (args & 0x1f); cnt++) {
        iob = dsp_cmds[index++];
#ifdef UDSP_DEBUG
        Serial.printf("%02x ", iob );
#endif
        spi_data8(iob);
      }
      SPI_CS_HIGH
#ifdef UDSP_DEBUG
      Serial.printf("\n");
#endif
      if (args & 0x80) {
        if (args&0x60) delay(500);
        else delay(150);
      }
      if (index >= dsp_ncmds) break;
    }
    SPI_END_TRANSACTION

  }

  // must init luts on epaper
  if (ep_mode) {
    Init_EPD(DISPLAY_INIT_FULL);
    Init_EPD(DISPLAY_INIT_PARTIAL);
  }
}


void uDisplay_lvgl::DisplayInit(int8_t p, int8_t size, int8_t rot, int8_t font) {
  if (p !=DISPLAY_INIT_MODE && ep_mode) {
    if (p == DISPLAY_INIT_PARTIAL) {
      if (lutpsize) {
        SetLut(lut_partial);
        Updateframe_EPD();
        delay(lutptime * 10);
      }
      return;
    } else if (p == DISPLAY_INIT_FULL) {
      if (lutfsize) {
        SetLut(lut_full);
        Updateframe_EPD();
        delay(lutftime * 10);
      }
      return;
    }
  } else {
    setRotation(rot);
    invertDisplay(false);
    // setTextWrap(false);
    // cp437(true);
    // setTextFont(font);
    // setTextSize(size);
    // setTextColor(fg_col, bg_col);
    // setCursor(0,0);
    // fillScreen(bg_col);
    Updateframe();

#ifdef UDSP_DEBUG
    Serial.printf("Dsp Init complete \n");
#endif
  }
}

void uDisplay_lvgl::spi_command(uint8_t val) {

  if (spi_dc < 0) {
    if (spi_nr > 2) {
      write9(val, 0);
    } else {
      hw_write9(val, 0);
    }
  } else {
    SPI_DC_LOW
    if (spi_nr > 2) {
      write8(val);
    } else {
      uspi->write(val);
    }
    SPI_DC_HIGH
  }
}

void uDisplay_lvgl::spi_data8(uint8_t val) {
  if (spi_dc < 0) {
    if (spi_nr > 2) {
      write9(val, 1);
    } else {
      hw_write9(val, 1);
    }
  } else {
    if (spi_nr > 2) {
      write8(val);
    } else {
      uspi->write(val);
    }
  }
}

void uDisplay_lvgl::spi_data16(uint16_t val) {
  if (spi_dc < 0) {
    if (spi_nr > 2) {
      write9(val >> 8, 1);
      write9(val, 1);
    } else {
      hw_write9(val >> 8, 1);
      hw_write9(val, 1);
    }
  } else {
    if (spi_nr > 2) {
      write16(val);
    } else {
      uspi->write16(val);
    }
  }
}

void uDisplay_lvgl::spi_data32(uint32_t val) {
  if (spi_dc < 0) {
    if (spi_nr > 2) {
      write9(val >> 24, 1);
      write9(val >> 16, 1);
      write9(val >> 8, 1);
      write9(val, 1);
    } else {
      hw_write9(val >> 24, 1);
      hw_write9(val >> 16, 1);
      hw_write9(val >> 8, 1);
      hw_write9(val, 1);
    }
  } else {
    if (spi_nr > 2) {
      write32(val);
    } else {
      uspi->write32(val);
    }
  }
}

void uDisplay_lvgl::spi_command_one(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  spi_command(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay_lvgl::i2c_command(uint8_t val) {
  //Serial.printf("%02x\n",val );
  wire->beginTransmission(i2caddr);
  wire->write(0);
  wire->write(val);
  wire->endTransmission();
}


#define WIRE_MAX 32

void uDisplay_lvgl::Updateframe(void) {

  if (ep_mode) {
    Updateframe_EPD();
    return;
  }

  if (interface == _UDSP_I2C) {

  #if 0
    i2c_command(saw_1);
    i2c_command(i2c_page_start);
    i2c_command(i2c_page_end);
    i2c_command(saw_2);
    i2c_command(i2c_col_start);
    i2c_command(i2c_col_end);

    uint16_t count = gxs * ((gys + 7) / 8);
    uint8_t *ptr   = buffer;
    wire->beginTransmission(i2caddr);
    i2c_command(saw_3);
    uint8_t bytesOut = 1;
    while (count--) {
      if (bytesOut >= WIRE_MAX) {
        wire->endTransmission();
        wire->beginTransmission(i2caddr);
        i2c_command(saw_3);
        bytesOut = 1;
      }
      i2c_command(*ptr++);
      bytesOut++;
    }
    wire->endTransmission();
#else

    i2c_command(saw_1 | 0x0);  // set low col = 0, 0x00
    i2c_command(i2c_page_start | 0x0);  // set hi col = 0, 0x10
    i2c_command(i2c_page_end | 0x0); // set startline line #0, 0x40

	  uint8_t ys = gys >> 3;
	  uint8_t xs = gxs >> 3;
    //uint8_t xs = 132 >> 3;
	  uint8_t m_row = saw_2;
	  uint8_t m_col = i2c_col_start;

	  uint16_t p = 0;

	  uint8_t i, j, k = 0;

	  for ( i = 0; i < ys; i++) {
		    // send a bunch of data in one xmission
        i2c_command(0xB0 + i + m_row); //set page address
        i2c_command(m_col & 0xf); //set lower column address
        i2c_command(0x10 | (m_col >> 4)); //set higher column address

        for ( j = 0; j < 8; j++) {
			      wire->beginTransmission(i2caddr);
            wire->write(0x40);
            for ( k = 0; k < xs; k++, p++) {
		            wire->write(buffer[p]);
            }
            wire->endTransmission();
	      }
    }
#endif
 }


}


void uDisplay_lvgl::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  if (!x0 && !y0 && !x1 && !y1) {
    SPI_CS_HIGH
    SPI_END_TRANSACTION
  } else {
    SPI_CS_LOW
    SPI_BEGIN_TRANSACTION
    setAddrWindow_int(x0, y0, x1 - x0, y1 - y0 );
  }
}

#define udisp_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

void uDisplay_lvgl::setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    x += x_addr_offs[cur_rot];
    y += y_addr_offs[cur_rot];

    if (sa_mode != 8) {
      uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
      uint32_t ya = ((uint32_t)y << 16) | (y+h-1);

      spi_command(saw_1);
      spi_data32(xa);

      spi_command(saw_2);
      spi_data32(ya);

      if (saw_3 != 0xff) {
        spi_command(saw_3); // write to RAM
      }
    } else {
      uint16_t x2 = x + w - 1,
               y2 = y + h - 1;

      if (cur_rot & 1) { // Vertical address increment mode
        udisp_swap(x,y);
        udisp_swap(x2,y2);
      }
      spi_command(saw_1);
      spi_data8(x);
      spi_data8(x2);
      spi_command(saw_2);
      spi_data8(y);
      spi_data8(y2);
      if (saw_3 != 0xff) {
        spi_command(saw_3); // write to RAM
      }
    }
}


void uDisplay_lvgl::pushColors(uint16_t *data, uint16_t len, bool first) {
  uint16_t color;

  while (len--) {
    color = *data++;
    WriteColor(color);
  }

}

void uDisplay_lvgl::WriteColor(uint16_t color) {

  if (col_mode == 18) {
    uint8_t r = (color & 0xF800) >> 11;
    uint8_t g = (color & 0x07E0) >> 5;
    uint8_t b = color & 0x001F;

    r = (r * 255) / 31;
    g = (g * 255) / 63;
    b = (b * 255) / 31;

    spi_data8(r);
    spi_data8(g);
    spi_data8(b);
  } else {
    spi_data16(color);
  }
}

void uDisplay_lvgl::drawPixel(int16_t x, int16_t y, uint16_t color) {


  // if (ep_mode) {
  //   drawPixel_EPD(x, y, color);
  //   return;
  // }

  // if (interface != _UDSP_SPI) {
  //   Renderer::drawPixel(x, y, color);
  //   return;
  // }



  // if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;


  // SPI_BEGIN_TRANSACTION

  // SPI_CS_LOW

  // setAddrWindow_int(x, y, 1, 1);

  // WriteColor(color);

  // SPI_CS_HIGH

  // SPI_END_TRANSACTION
}

void uDisplay_lvgl::writePixels(int16_t x, int16_t y, int16_t w, int16_t h, 
                                uint16_t *colors, uint32_t len) {


  // if (ep_mode) {
  //   fillRect_EPD(x, y, w, h, color);
  //   return;
  // }

  // if (interface != _UDSP_SPI) {
  //   Renderer::fillRect(x, y, w, h, color);
  //   return;
  // }

  // if((x >= gxs) || (y >= gys)) return;
  // if((x + w - 1) >= gxs)  w = gxs  - x;
  // if((y + h - 1) >= gys) h = gys - y;


  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW

  setAddrWindow_int(x, y, w, h);

  uint32_t chrono_start = millis();
  uspi->writePixels(colors, len * 2);
  uint32_t chrono_time = millis() - chrono_start;

  if (len >= 10000) {
    AddLog(LOG_LEVEL_DEBUG, "LVG: refreshed %d pixels in %d ms (%i pix/ms)", len, chrono_time,
            chrono_time > 0 ? len / chrono_time : -1);
  }
  // while(len--) {
  //   spi_data16(*colors++);
  // }

  SPI_CS_HIGH
  SPI_END_TRANSACTION
}
  // if (interface == _UDSP_SPI) {
  //   // if(connection == TFT_HARD_SPI) {
  //     uspi->writePixels(colors, len * 2);
  //       // hwspi._spi->writePixels(colors, len * 2);
  //       return;
  //   }
    

void uDisplay_lvgl::setRotation(uint8_t rotation) {
  cur_rot = rotation;

  if (interface != _UDSP_SPI) {
    // Renderer::setRotation(cur_rot);
    return;
  }

  if (interface == _UDSP_SPI) {

    if (ep_mode) {
      // Renderer::setRotation(cur_rot);
      return;
    }
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    spi_command(madctrl);
    spi_data8(rot[cur_rot]);
    if (sa_mode == 8) {
      spi_command(startline);
      spi_data8((cur_rot < 2) ? height() : 0);
    }

    SPI_CS_HIGH
    SPI_END_TRANSACTION
  }
  switch (rotation) {
    case 0:
      _width  = gxs;
      _height = gys;
      break;
    case 1:
      _width  = gys;
      _height = gxs;
      break;
    case 2:
      _width  = gxs;
      _height = gys;
      break;
    case 3:
      _width  = gys;
      _height = gxs;
      break;
  }

}

void udisp_bpwr(uint8_t on);

void uDisplay_lvgl::DisplayOnff(int8_t on) {

  if (ep_mode) {
    return;
  }

  udisp_bpwr(on);

  if (interface == _UDSP_I2C) {
    if (on) {
      i2c_command(dsp_on);
    } else {
      i2c_command(dsp_off);
    }
  } else {
    if (on) {
      if (dsp_on != 0xff) spi_command_one(dsp_on);
      if (bpanel >= 0) {
#ifdef ESP32
        ledcWrite(ESP32_PWM_CHANNEL, dimmer);
#else
        digitalWrite(bpanel, HIGH);
#endif
      }

    } else {
      if (dsp_off != 0xff) spi_command_one(dsp_off);
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

void uDisplay_lvgl::invertDisplay(bool i) {

  if (ep_mode) {
    return;
  }

  if (interface == _UDSP_SPI) {
    if (i) {
      spi_command_one(inv_on);
    } else {
      spi_command_one(inv_off);
    }
  }
  if (interface == _UDSP_I2C) {
    if (i) {
      i2c_command(inv_on);
    } else {
      i2c_command(inv_off);
    }
  }
}

void udisp_dimm(uint8_t dim);

void uDisplay_lvgl::dim(uint8_t dim) {
  dimmer = dim;

  if (ep_mode) {
    return;
  }

  if (interface == _UDSP_SPI) {
    if (dimmer > 15) dimmer = 15;
    dimmer = ((float)dimmer / 15.0) * 255.0;
#ifdef ESP32
    if (bpanel >= 0) {
      ledcWrite(ESP32_PWM_CHANNEL, dimmer);
    } else {
      udisp_dimm(dim);
    }
#endif

    if (dim_op != 0xff) {
      SPI_BEGIN_TRANSACTION
      SPI_CS_LOW
      spi_command(dim_op);
      spi_data8(dim);
      SPI_CS_HIGH
      SPI_END_TRANSACTION
    }
  }
}


void uDisplay_lvgl::TS_RotConvert(int16_t *x, int16_t *y) {
  int16_t temp;

  switch (rot_t[cur_rot]) {
    case 0:
      break;
    case 1:
      temp = *y;
      *y = height() - *x;
      *x = temp;
      break;
    case 2:
      *x = width() - *x;
      *y = height() - *y;
      break;
    case 3:
      temp = *y;
      *y = *x;
      *x = width() - temp;
      break;
  }
}

uint8_t uDisplay_lvgl::strlen_ln(char *str) {
  for (uint32_t cnt = 0; cnt < 256; cnt++) {
    if (!str[cnt] || str[cnt] == '\n' || str[cnt] == ' ') return cnt;
  }
  return 0;
}

char *uDisplay_lvgl::devname(void) {
  return dname;
}

uint32_t uDisplay_lvgl::str2c(char **sp, char *vp, uint32_t len) {
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

int32_t uDisplay_lvgl::next_val(char **sp) {
  char ibuff[16];
  if (!str2c(sp, ibuff, sizeof(ibuff))) {
    return atoi(ibuff);
  }
  return 0xff;
}

uint32_t uDisplay_lvgl::next_hex(char **sp) {
  char ibuff[16];
  if (!str2c(sp, ibuff, sizeof(ibuff))) {
    return strtol(ibuff, 0, 16);
  }
  return 0xff;
}

#ifdef ESP32
#include "soc/spi_reg.h"
#include "soc/spi_struct.h"
#include "esp32-hal-spi.h"
#include "esp32-hal.h"
#include "soc/spi_struct.h"

// since ardunio transferBits ia completely disfunctional
// we use our own hardware driver for 9 bit spi
void uDisplay_lvgl::hw_write9(uint8_t val, uint8_t dc) {

    uint32_t regvalue = val >> 1;
    if (dc) regvalue |= 0x80;
    else regvalue &= 0x7f;
    if (val & 1) regvalue |= 0x8000;

    REG_SET_BIT(SPI_USER_REG(3), SPI_USR_MOSI);
    REG_WRITE(SPI_MOSI_DLEN_REG(3), 9 - 1);
    uint32_t *dp = (uint32_t*)SPI_W0_REG(3);
    *dp = regvalue;
    REG_SET_BIT(SPI_CMD_REG(3), SPI_USR);
    while (REG_GET_FIELD(SPI_CMD_REG(3), SPI_USR));
}

#else
#include "spi_register.h"
void uDisplay_lvgl::hw_write9(uint8_t val, uint8_t dc) {

    uint32_t regvalue;
    uint8_t bytetemp;
    if (!dc) {
      bytetemp = (val>> 1) & 0x7f;
    } else {
      bytetemp = (val >> 1) | 0x80;
    }

    regvalue = ((8 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) | ((uint32)bytetemp);		//configure transmission variable,9bit transmission length and first 8 command bit
    if (val & 0x01) 	regvalue |= BIT15;        //write the 9th bit
    while (READ_PERI_REG(SPI_CMD(1)) & SPI_USR);		//waiting for spi module available
    WRITE_PERI_REG(SPI_USER2(1), regvalue);				//write  command and command length into spi reg
    SET_PERI_REG_MASK(SPI_CMD(1), SPI_USR);		//transmission start

}
#endif

#define USECACHE ICACHE_RAM_ATTR


void USECACHE uDisplay_lvgl::write8(uint8_t val) {
  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    GPIO_CLR(spi_clk);
    if (val & bit) GPIO_SET(spi_mosi);
    else   GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);
  }
}

void USECACHE uDisplay_lvgl::write9(uint8_t val, uint8_t dc) {

  GPIO_CLR(spi_clk);
  if (dc) GPIO_SET(spi_mosi);
  else  GPIO_CLR(spi_mosi);
  GPIO_SET(spi_clk);

  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    GPIO_CLR(spi_clk);
    if (val & bit) GPIO_SET(spi_mosi);
    else   GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);
  }
}

void USECACHE uDisplay_lvgl::write16(uint16_t val) {
  for (uint16_t bit = 0x8000; bit; bit >>= 1) {
    GPIO_CLR(spi_clk);
    if (val & bit) GPIO_SET(spi_mosi);
    else   GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);
  }
}

void USECACHE uDisplay_lvgl::write32(uint32_t val) {
  for (uint32_t bit = 0x80000000; bit; bit >>= 1) {
    GPIO_CLR(spi_clk);
    if (val & bit) GPIO_SET(spi_mosi);
    else   GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);
  }
}


// epaper section

// EPD2IN9 commands
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF


void uDisplay_lvgl::spi_data8_EPD(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  spi_data8(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay_lvgl::spi_command_EPD(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  spi_command(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay_lvgl::Init_EPD(int8_t p) {
  if (p == DISPLAY_INIT_PARTIAL) {
    SetLut(lut_partial);
  } else {
    SetLut(lut_full);
  }
  ClearFrameMemory(0xFF);
  Updateframe_EPD();
  if (p == DISPLAY_INIT_PARTIAL) {
    delay(lutptime * 10);
  } else {
    delay(lutftime * 10);
  }
}

void uDisplay_lvgl::ClearFrameMemory(unsigned char color) {
    SetMemoryArea(0, 0, gxs - 1, gys - 1);
    SetMemoryPointer(0, 0);
    spi_command_EPD(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(color);
    }
}

void uDisplay_lvgl::SetLut(const unsigned char* lut) {
    spi_command_EPD(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < lutfsize; i++) {
        spi_data8_EPD(lut[i]);
    }
}

void uDisplay_lvgl::Updateframe_EPD(void) {
  SetFrameMemory(buffer, 0, 0, gxs, gys);
  DisplayFrame();
}

void uDisplay_lvgl::DisplayFrame(void) {
    spi_command_EPD(DISPLAY_UPDATE_CONTROL_2);
    spi_data8_EPD(0xC4);
    spi_command_EPD(MASTER_ACTIVATION);
    spi_data8_EPD(TERMINATE_FRAME_READ_WRITE);
}

void uDisplay_lvgl::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    spi_command_EPD(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    spi_data8_EPD((x_start >> 3) & 0xFF);
    spi_data8_EPD((x_end >> 3) & 0xFF);
    spi_command_EPD(SET_RAM_Y_ADDRESS_START_END_POSITION);
    spi_data8_EPD(y_start & 0xFF);
    spi_data8_EPD((y_start >> 8) & 0xFF);
    spi_data8_EPD(y_end & 0xFF);
    spi_data8_EPD((y_end >> 8) & 0xFF);
}

void uDisplay_lvgl::SetFrameMemory(const unsigned char* image_buffer) {
    SetMemoryArea(0, 0, gxs - 1, gys - 1);
    SetMemoryPointer(0, 0);
    spi_command_EPD(WRITE_RAM);
    /* send the image data */
    for (int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(image_buffer[i] ^ 0xff);
    }
}

void uDisplay_lvgl::SetMemoryPointer(int x, int y) {
    spi_command_EPD(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    spi_data8_EPD((x >> 3) & 0xFF);
    spi_command_EPD(SET_RAM_Y_ADDRESS_COUNTER);
    spi_data8_EPD(y & 0xFF);
    spi_data8_EPD((y >> 8) & 0xFF);
}

void uDisplay_lvgl::SetFrameMemory(
    const unsigned char* image_buffer,
    uint16_t x,
    uint16_t y,
    uint16_t image_width,
    uint16_t image_height
) {
    uint16_t x_end;
    uint16_t y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }

    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xFFF8;
    image_width &= 0xFFF8;
    if (x + image_width >= gxs) {
        x_end = gxs - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= gys) {
        y_end = gys - 1;
    } else {
        y_end = y + image_height - 1;
    }

    if (!x && !y && image_width == gxs && image_height == gys) {
      SetFrameMemory(image_buffer);
      return;
    }

    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    spi_command_EPD(WRITE_RAM);
    /* send the image data */
    for (uint16_t j = 0; j < y_end - y + 1; j++) {
        for (uint16_t i = 0; i < (x_end - x + 1) / 8; i++) {
            spi_data8_EPD(image_buffer[i + j * (image_width / 8)]^0xff);
        }
    }
}

#define IF_INVERT_COLOR     1
#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }
/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 * we must use this for epaper because these displays have a strange and different bit pattern
 */
void uDisplay_lvgl::DrawAbsolutePixel(int x, int y, int16_t color) {

    int16_t w = width(), h = height();
    if (cur_rot == 1 || cur_rot == 3) {
      renderer_swap(w, h);
    }

    if (x < 0 || x >= w || y < 0 || y >= h) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (color) {
            buffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        } else {
            buffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (color) {
            buffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        } else {
            buffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        }
    }
}

void uDisplay_lvgl::drawPixel_EPD(int16_t x, int16_t y, uint16_t color) {
  if (!buffer) return;
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (cur_rot) {
  case 1:
    renderer_swap(x, y);
    x = gxs - x - 1;
    break;
  case 2:
    x = gxs - x - 1;
    y = gys - y - 1;
    break;
  case 3:
    renderer_swap(x, y);
    y = gys - y - 1;
    break;
  }

  // x is which column
  DrawAbsolutePixel(x, y, color);

}


void uDisplay_lvgl::fillRect_EPD(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  for (uint32_t yp = y; yp < y + h; yp++) {
    for (uint32_t xp = x; xp < x + w; xp++) {
      drawPixel_EPD(xp , yp , color);
    }
  }
}
void uDisplay_lvgl::drawFastVLine_EPD(int16_t x, int16_t y, int16_t h, uint16_t color) {
  while (h--) {
    drawPixel_EPD(x , y , color);
    y++;
  }
}
void uDisplay_lvgl::drawFastHLine_EPD(int16_t x, int16_t y, int16_t w, uint16_t color) {
  while (w--) {
    drawPixel_EPD(x , y , color);
    x++;
  }
}