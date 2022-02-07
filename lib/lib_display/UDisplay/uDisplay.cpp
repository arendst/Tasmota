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
#include "uDisplay.h"

#ifdef ESP32
#include "esp8266toEsp32.h"
#endif

// #define UDSP_DEBUG

const uint16_t udisp_colors[]={UDISP_BLACK,UDISP_WHITE,UDISP_RED,UDISP_GREEN,UDISP_BLUE,UDISP_CYAN,UDISP_MAGENTA,\
  UDISP_YELLOW,UDISP_NAVY,UDISP_DARKGREEN,UDISP_DARKCYAN,UDISP_MAROON,UDISP_PURPLE,UDISP_OLIVE,\
UDISP_LIGHTGREY,UDISP_DARKGREY,UDISP_ORANGE,UDISP_GREENYELLOW,UDISP_PINK};

uint16_t uDisplay::GetColorFromIndex(uint8_t index) {
  if (index >= sizeof(udisp_colors) / 2) index = 0;
  return udisp_colors[index];
}

uint16_t uDisplay::fgcol(void) {
  return fg_col;
}
uint16_t uDisplay::bgcol(void) {
  return bg_col;
}

int8_t uDisplay::color_type(void) {
  return col_type;
}


uDisplay::~uDisplay(void) {
  if (framebuffer) {
    free(framebuffer);
  }
}

uDisplay::uDisplay(char *lp) : Renderer(800, 600) {
  // analyse decriptor
  pwr_cbp = 0;
  dim_cbp = 0;
  framebuffer = 0;
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
  fg_col = 1;
  bg_col = 0;
  splash_font = -1;
  rotmap_xmin = -1;
  bpanel = -1;
  allcmd_mode = 0;
  startline = 0xA1;
  uint8_t section = 0;
  dsp_ncmds = 0;
  lut_num = 0;
  lvgl_param.data = 0;
  lvgl_param.fluslines = 40;

  for (uint32_t cnt = 0; cnt < 5; cnt++) {
    lut_cnt[cnt] = 0;
    lut_cmd[cnt] = 0xff;
  }
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
        if (section == 'I') {
          if (*lp1 == 'C') {
            allcmd_mode = 1;
            lp1++;
          }
        } else if (section == 'L') {
          if (*lp1 >= '1' && *lp1 <= '5') {
            lut_num = (*lp1 & 0x07);
            lp1+=2;
            lut_cmd[lut_num - 1] = next_hex(&lp1);
          }
        }
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
            setwidth(gxs);
            gys = next_val(&lp1);
            setheight(gys);
            disp_bpp = next_val(&lp1);
            bpp = abs(disp_bpp);
            if (bpp == 1) {
              col_type = uCOLOR_BW;
            } else {
              col_type = uCOLOR_COLOR;
            }
            str2c(&lp1, ibuff, sizeof(ibuff));
            if (!strncmp(ibuff, "I2C", 3)) {
              interface = _UDSP_I2C;
              wire_n = 0;
              if (!strncmp(ibuff, "I2C2", 4)) {
               wire_n = 1;
              }
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
            if (interface == _UDSP_I2C || bpp == 1) {
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
            if (!lut_num) {
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  lut_full[lutfsize++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
                if (lutfsize >= LUTMAXSIZE) break;
              }
            } else {
              uint8_t index = lut_num - 1;
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  lut_array[lut_cnt[index]++][index] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
                if (lut_cnt[index] >= LUTMAXSIZE) break;
              }
            }
            break;
          case 'l':
            while (1) {
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                lut_partial[lutpsize++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
              if (lutpsize >= LUTMAXSIZE) break;
            }
            break;
          case 'T':
            lutftime = next_val(&lp1);
            lutptime = next_val(&lp1);
            lut3time = next_val(&lp1);
            break;
          case 'B':
            lvgl_param.fluslines = next_val(&lp1);
            lvgl_param.data = next_val(&lp1);
            break;
          case 'M':
            rotmap_xmin = next_val(&lp1);
            rotmap_xmax = next_val(&lp1);
            rotmap_ymin = next_val(&lp1);
            rotmap_ymax = next_val(&lp1);
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
    // 2 table mode
    ep_mode = 1;
  }

  if (lut_cnt[0]>0 && lut_cnt[1]==lut_cnt[2] && lut_cnt[1]==lut_cnt[3] && lut_cnt[1]==lut_cnt[4]) {
    // 5 table mode
    ep_mode = 2;
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
    Serial.printf("DMA-Mode: %d\n", lvgl_param.use_dma);

    Serial.printf("opts: %02x,%02x,%02x\n", saw_3, dim_op, startline);

    Serial.printf("SetAddr : %x,%x,%x\n", saw_1, saw_2, saw_3);

    Serial.printf("Rot 0: %x,%x - %d - %d\n", madctrl, rot[0], x_addr_offs[0], y_addr_offs[0]);

    if (ep_mode == 1) {
      Serial.printf("LUT_Partial : %d\n", lutpsize);
      Serial.printf("LUT_Full : %d\n", lutfsize);
    }
    if (ep_mode == 2) {
      Serial.printf("LUT_SIZE 1: %d\n", lut_cnt[0]);
      Serial.printf("LUT_SIZE 2: %d\n", lut_cnt[1]);
      Serial.printf("LUT_SIZE 3: %d\n", lut_cnt[2]);
      Serial.printf("LUT_SIZE 4: %d\n", lut_cnt[3]);
      Serial.printf("LUT_SIZE 5: %d\n", lut_cnt[4]);
      Serial.printf("LUT_CMDS %02x-%02x-%02x-%02x-%02x\n", lut_cmd[0], lut_cmd[1], lut_cmd[2], lut_cmd[3], lut_cmd[4]);
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
}


Renderer *uDisplay::Init(void) {
  extern bool UsePSRAM(void);

  // for any bpp below native 16 bits, we allocate a local framebuffer to copy into
  if (ep_mode || bpp < 16) {
    if (framebuffer) free(framebuffer);
#ifdef ESP8266
    framebuffer = (uint8_t*)calloc((gxs * gys * bpp) / 8, 1);
#else
    if (UsePSRAM()) {
      framebuffer = (uint8_t*)heap_caps_malloc((gxs * gys * bpp) / 8, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    } else {
      framebuffer = (uint8_t*)calloc((gxs * gys * bpp) / 8, 1);
    }
    #endif
  }


  if (interface == _UDSP_I2C) {
    if (wire_n == 0) {
      wire = &Wire;
    }
#ifdef ESP32
    if (wire_n == 1) {
      wire = &Wire1;
    }
#endif
    wire->begin(i2c_sda, i2c_scl);    // TODO: aren't I2C buses already initialized? Shouldn't this be moved to display driver?

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

    if (bpanel >= 0) {
#ifdef ESP32
        analogWrite(bpanel, 32);
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
      if (lvgl_param.use_dma) {
        spi_host = VSPI_HOST;
        initDMA(spi_cs);
      }

    } else if (spi_nr == 2) {
      uspi = new SPIClass(HSPI);
      uspi->begin(spi_clk, spi_miso, spi_mosi, -1);
      if (lvgl_param.use_dma) {
        spi_host = HSPI_HOST;
        initDMA(spi_cs);
      }
    } else {
      pinMode(spi_clk, OUTPUT);
      digitalWrite(spi_clk, LOW);
      pinMode(spi_mosi, OUTPUT);
      digitalWrite(spi_mosi, LOW);
    }
#endif // ESP32


    spiSettings = SPISettings((uint32_t)spi_speed*1000000, MSBFIRST, SPI_MODE3);
    SPI_BEGIN_TRANSACTION


    if (reset >= 0) {
      pinMode(reset, OUTPUT);
      digitalWrite(reset, HIGH);
      delay(50);
      digitalWrite(reset, LOW);
      delay(50);
      digitalWrite(reset, HIGH);
      delay(200);
    }

    uint16_t index = 0;
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
        if (!allcmd_mode) {
          spi_data8(iob);
        } else {
          spi_command(iob);
        }
      }
      SPI_CS_HIGH
#ifdef UDSP_DEBUG
      Serial.printf("\n");
#endif
      if (args & 0x80) {  // delay after the command
        uint32_t delay_ms = 0;
        switch (args & 0xE0) {
          case 0x80:  delay_ms = 150; break;
          case 0xA0:  delay_ms =  10; break;
          case 0xE0:  delay_ms = 500; break;
        }
        if (delay_ms > 0) {
          delay(delay_ms);
#ifdef UDSP_DEBUG
          Serial.printf("delay %d ms\n", delay_ms);
#endif
        }

      }
      if (index >= dsp_ncmds) break;
    }
    SPI_END_TRANSACTION

  }

  // must init luts on epaper
  if (ep_mode) {
    Init_EPD(DISPLAY_INIT_FULL);
    if (ep_mode == 1) Init_EPD(DISPLAY_INIT_PARTIAL);
  }

  return this;
}


void uDisplay::DisplayInit(int8_t p, int8_t size, int8_t rot, int8_t font) {
  if (p != DISPLAY_INIT_MODE && ep_mode) {
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
      }
      if (ep_mode == 2) {
        ClearFrame_42();
        DisplayFrame_42();
      }
      delay(lutftime * 10);
      return;
    }
  } else {
    setRotation(rot);
    invertDisplay(false);
    setTextWrap(false);
    cp437(true);
    setTextFont(font);
    setTextSize(size);
    setTextColor(fg_col, bg_col);
    setCursor(0,0);
    if (splash_font >= 0) {
      fillScreen(bg_col);
      Updateframe();
    }

#ifdef UDSP_DEBUG
    Serial.printf("Dsp Init complete \n");
#endif
  }
}

void uDisplay::spi_command(uint8_t val) {

  if (spi_dc < 0) {
    if (spi_nr > 2) {
      if (spi_nr == 3) {
        write9(val, 0);
      } else {
        write9_slow(val, 0);
      }
    } else {
      hw_write9(val, 0);
    }
  } else {
    SPI_DC_LOW
    if (spi_nr > 2) {
      if (spi_nr == 3) {
        write8(val);
      } else {
        write8_slow(val);
      }
    } else {
      uspi->write(val);
    }
    SPI_DC_HIGH
  }
}

void uDisplay::spi_data8(uint8_t val) {
  if (spi_dc < 0) {
    if (spi_nr > 2) {
      if (spi_nr == 3) {
        write9(val, 1);
      } else {
        write9_slow(val, 1);
      }
    } else {
      hw_write9(val, 1);
    }
  } else {
    if (spi_nr > 2) {
      if (spi_nr == 3) {
        write8(val);
      } else {
        write8_slow(val);
      }
    } else {
      uspi->write(val);
    }
  }
}

void uDisplay::spi_data16(uint16_t val) {
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

void uDisplay::spi_data32(uint32_t val) {
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

void uDisplay::spi_command_one(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  spi_command(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay::i2c_command(uint8_t val) {
  //Serial.printf("%02x\n",val );
  wire->beginTransmission(i2caddr);
  wire->write(0);
  wire->write(val);
  wire->endTransmission();
}


#define WIRE_MAX 32

void uDisplay::Updateframe(void) {

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
    uint8_t *ptr   = framebuffer;
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
		            wire->write(framebuffer[p]);
            }
            wire->endTransmission();
	      }
    }
#endif

 }


  if (interface == _UDSP_SPI) {
    if (framebuffer == nullptr) { return; }

    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW

    // below commands are not needed for SH1107
    // spi_command(saw_1 | 0x0);  // set low col = 0, 0x00
    // spi_command(i2c_page_start | 0x0);  // set hi col = 0, 0x10
    // spi_command(i2c_page_end | 0x0); // set startline line #0, 0x40

	  uint8_t ys = gys >> 3;
	  uint8_t xs = gxs >> 3;
    //uint8_t xs = 132 >> 3;
	  uint8_t m_row = saw_2;
	  uint8_t m_col = i2c_col_start;
    // Serial.printf("m_row=%d m_col=%d xs=%d ys=%d\n", m_row, m_col, xs, ys);

	  uint16_t p = 0;

	  uint8_t i, j, k = 0;
	  for ( i = 0; i < ys; i++) {   // i = line from 0 to ys
		    // send a bunch of data in one xmission
        spi_command(0xB0 + i + m_row); //set page address
        spi_command(m_col & 0xf); //set lower column address
        spi_command(0x10 | (m_col >> 4)); //set higher column address

        for ( j = 0; j < 8; j++) {
            for ( k = 0; k < xs; k++, p++) {
		            spi_data8(framebuffer[p]);
            }
	      }
    }

    SPI_CS_HIGH
    SPI_END_TRANSACTION

  }

}

void uDisplay::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {

  if (ep_mode) {
    drawFastVLine_EPD(x, y, h, color);
    return;
  }

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

  if (col_mode == 18) {
    uint8_t r = (color & 0xF800) >> 11;
    uint8_t g = (color & 0x07E0) >> 5;
    uint8_t b = color & 0x001F;
    r = (r * 255) / 31;
    g = (g * 255) / 63;
    b = (b * 255) / 31;

    while (h--) {
      spi_data8(r);
      spi_data8(g);
      spi_data8(b);
    }
  } else {
    while (h--) {
      WriteColor(color);
    }
  }

  SPI_CS_HIGH

  SPI_END_TRANSACTION
}

void uDisplay::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {


  if (ep_mode) {
    drawFastHLine_EPD(x, y, w, color);
    return;
  }

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

  if (col_mode == 18) {
    uint8_t r = (color & 0xF800) >> 11;
    uint8_t g = (color & 0x07E0) >> 5;
    uint8_t b = color & 0x001F;
    r = (r * 255) / 31;
    g = (g * 255) / 63;
    b = (b * 255) / 31;

    while (w--) {
      spi_data8(r);
      spi_data8(g);
      spi_data8(b);
    }
  } else {
    while (w--) {
      WriteColor(color);
    }
  }

  SPI_CS_HIGH

  SPI_END_TRANSACTION
}

//#define CD_XS gxs
//#define CD_YS gys
#define CD_XS width()
#define CD_YS height()

void uDisplay::fillScreen(uint16_t color) {
  fillRect(0, 0,  CD_XS, CD_YS, color);
}

// fill a rectangle
void uDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {


  if (ep_mode) {
    fillRect_EPD(x, y, w, h, color);
    return;
  }

  if (interface != _UDSP_SPI) {
    Renderer::fillRect(x, y, w, h, color);
    return;
  }

  if((x >= CD_XS) || (y >= CD_YS)) return;
  if((x + w - 1) >= CD_XS)  w = CD_XS  - x;
  if((y + h - 1) >= CD_YS) h = CD_YS - y;


  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW

  setAddrWindow_int(x, y, w, h);

  if (col_mode == 18) {
    uint8_t r = (color & 0xF800) >> 11;
    uint8_t g = (color & 0x07E0) >> 5;
    uint8_t b = color & 0x001F;
    r = (r * 255) / 31;
    g = (g * 255) / 63;
    b = (b * 255) / 31;

    for (y = h; y > 0; y--) {
      for (x = w; x > 0; x--) {
        spi_data8(r);
        spi_data8(g);
        spi_data8(b);
      }
    }

  } else {
    for (y = h; y > 0; y--) {
      for (x = w; x > 0; x--) {
        WriteColor(color);
      }
    }
  }
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

/*

// pack RGB into uint32
uint32_t pack_rgb(uint32_t r, uint32_t g, uint32_t b) {
  uint32_t data;
  data=r<<23;
  data|=g<<14;
  data|=b<<5;
  data|=0b10000000010000000010000000000000;
  return ulswap(data);
}

// init 27 bit mode
uint32_t data=pack_rgb(r,g,b);
REG_SET_BIT(SPI_USER_REG(3), SPI_USR_MOSI);
REG_WRITE(SPI_MOSI_DLEN_REG(3), 27 - 1);
uint32_t *dp=(uint32_t*)SPI_W0_REG(3);
digitalWrite( _cs, LOW);
for(y=h; y>0; y--) {
  for(x=w; x>0; x--) {
    while (REG_GET_FIELD(SPI_CMD_REG(3), SPI_USR));
    *dp=data;
    REG_SET_BIT(SPI_CMD_REG(3), SPI_USR);
  }
}
*/


void uDisplay::Splash(void) {

  if (splash_font < 0) return;

  if (ep_mode) {
    Updateframe();
    delay(lut3time * 10);
  }
  setTextFont(splash_font);
  setTextSize(splash_size);
  DrawStringAt(splash_xp, splash_yp, dname, fg_col, 0);
  Updateframe();
}

void uDisplay::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  if (bpp != 16) {
    // just save params or update frame
    if (!x0 && !y0 && !x1 && !y1) {
      if (!ep_mode) {
        Updateframe();
      }
    } else {
      seta_xp1 = x0;
      seta_xp2 = x1;
      seta_yp1 = y0;
      seta_yp2 = y1;
      // Serial.printf("xp1=%d xp2=%d yp1=%d yp2=%d\n", seta_xp1, seta_xp2, seta_yp1, seta_yp2);
    }
    return;
  }

  if (!x0 && !y0 && !x1 && !y1) {
    SPI_CS_HIGH
    SPI_END_TRANSACTION
  } else {
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    setAddrWindow_int(x0, y0, x1 - x0, y1 - y0 );
  }
}

#define udisp_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

void uDisplay::setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
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
      if (allcmd_mode) {
        spi_data8(x);
        spi_data8(x2);
      } else {
        spi_command(x);
        spi_command(x2);
      }
      spi_command(saw_2);
      if (allcmd_mode) {
        spi_data8(y);
        spi_data8(y2);
      } else {
        spi_command(y);
        spi_command(y2);
      }
      if (saw_3 != 0xff) {
        spi_command(saw_3); // write to RAM
      }
    }
}

#define RGB16_TO_MONO       0x8410
#define RGB16_SWAP_TO_MONO  0x1084
// #define CNV_B1_OR  ((0x10<<11) | (0x20<<5) | 0x10)
// static inline uint8_t ulv_color_to1(uint16_t color) {
//   if (color & CNV_B1_OR) {
//       return 1;
//   }
//   else {
//       return 0;
//   }
/*
// this needs optimization
  if (((color>>11) & 0x10) || ((color>>5) & 0x20) || (color & 0x10)) {
      return 1;
  }
  else {
      return 0;
  }*/
// }

// convert to mono, these are framebuffer based
void uDisplay::pushColorsMono(uint16_t *data, uint16_t len, bool rgb16_swap) {
  // pixel is white if at least one of the 3 components is above 50%
  // this is tested with a simple mask, swapped if needed
  uint16_t rgb16_to_mono_mask = rgb16_swap ? RGB16_SWAP_TO_MONO : RGB16_TO_MONO;

  for (uint32_t y = seta_yp1; y < seta_yp2; y++) {
    for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
      uint16_t color = *data++;
      if (bpp == 1) color = (color & rgb16_to_mono_mask) ? 1 : 0;
      drawPixel(x, y, color);   // todo - inline the method to save speed
      len--;
      if (!len) return;         // failsafe - exist if len (pixel number) is exhausted
    }
  }
}

// swap high low byte
static inline void lvgl_color_swap(uint16_t *data, uint16_t len) { for (uint32_t i = 0; i < len; i++) (data[i] = data[i] << 8 | data[i] >> 8); }

void uDisplay::pushColors(uint16_t *data, uint16_t len, boolean not_swapped) {
  uint16_t color;

  if (lvgl_param.swap_color) {
    not_swapped = !not_swapped;
  }

  //Serial.printf("push %x - %d - %d - %d\n", (uint32_t)data, len, not_swapped,lvgl_param.data);
  if (not_swapped == false) {
    // called from LVGL bytes are swapped
    if (bpp != 16) {
      // lvgl_color_swap(data, len); -- no need to swap anymore, we have inverted the mask
      pushColorsMono(data, len, true);
      return;
    }

    if ( (col_mode != 18) && (spi_dc >= 0) && (spi_nr <= 2) ) {
      // special version 8 bit spi I or II
#ifdef ESP8266
      lvgl_color_swap(data, len);
      while (len--) {
        uspi->write(*data++);
      }
#else
      if (lvgl_param.use_dma) {
        pushPixelsDMA(data, len );
      } else {
        uspi->writeBytes((uint8_t*)data, len * 2);
      }
#endif
    } else {

#ifdef ESP32
      if ( (col_mode == 18) && (spi_dc >= 0) && (spi_nr <= 2) ) {
        uint8_t *line = (uint8_t*)malloc(len * 3);
        uint8_t *lp = line;
        if (line) {
          for (uint32_t cnt = 0; cnt < len; cnt++) {
            color = *data++;
            color = (color << 8) | (color >> 8);
            uint8_t r = (color & 0xF800) >> 11;
            uint8_t g = (color & 0x07E0) >> 5;
            uint8_t b = color & 0x001F;
            r = (r * 255) / 31;
            g = (g * 255) / 63;
            b = (b * 255) / 31;
            *lp++ = r;
            *lp++ = g;
            *lp++ = b;
          }

          if (lvgl_param.use_dma) {
            pushPixels3DMA(line, len );
          } else {
            uspi->writeBytes(line, len * 3);
          }
          free(line);
        }

      } else {
        // 9 bit and others
        lvgl_color_swap(data, len);
        while (len--) {
          WriteColor(*data++);
        }
      }
#endif // ESP32

#ifdef ESP8266
      lvgl_color_swap(data, len);
      while (len--) {
        WriteColor(*data++);
      }
#endif
    }
  } else {
    // called from displaytext, no byte swap, currently no dma here
    if (bpp != 16) {
      pushColorsMono(data, len);
      return;
    }
    if ( (col_mode != 18) && (spi_dc >= 0) && (spi_nr <= 2) ) {
      // special version 8 bit spi I or II
  #ifdef ESP8266
      while (len--) {
        //uspi->write(*data++);
        WriteColor(*data++);
      }
  #else
      uspi->writePixels(data, len * 2);
  #endif
    } else {
      // 9 bit and others
      while (len--) {
        WriteColor(*data++);
      }
    }
  }
}

void uDisplay::WriteColor(uint16_t color) {

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

void uDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {


  if (ep_mode) {
    drawPixel_EPD(x, y, color);
    return;
  }

  if (interface != _UDSP_SPI || bpp < 16) {
    Renderer::drawPixel(x, y, color);
    return;
  }



  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;


  SPI_BEGIN_TRANSACTION

  SPI_CS_LOW

  setAddrWindow_int(x, y, 1, 1);

  WriteColor(color);

  SPI_CS_HIGH

  SPI_END_TRANSACTION
}

void uDisplay::setRotation(uint8_t rotation) {
  cur_rot = rotation;

  if (interface != _UDSP_SPI) {
    Renderer::setRotation(cur_rot);
    return;
  }

  if (interface == _UDSP_SPI) {

    if (ep_mode) {
      Renderer::setRotation(cur_rot);
      return;
    }
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    spi_command(madctrl);

    if (!allcmd_mode) {
      spi_data8(rot[cur_rot]);
    } else {
      spi_command(rot[cur_rot]);
    }

    if ((sa_mode == 8) && !allcmd_mode) {
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

void uDisplay::DisplayOnff(int8_t on) {

  if (ep_mode) {
    return;
  }

  if (pwr_cbp) {
    pwr_cbp(on);
  }

//  udisp_bpwr(on);

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
        analogWrite(bpanel, dimmer10_gamma);
        // ledcWrite(ESP32_PWM_CHANNEL, dimmer8_gamma);
#else
        digitalWrite(bpanel, HIGH);
#endif
      }

    } else {
      if (dsp_off != 0xff) spi_command_one(dsp_off);
      if (bpanel >= 0) {
#ifdef ESP32
        analogWrite(bpanel, 0);
        // ledcWrite(ESP32_PWM_CHANNEL, 0);
#else
        digitalWrite(bpanel, LOW);
#endif
      }
    }
  }
}

void uDisplay::invertDisplay(boolean i) {

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

// input value is 0..15
// void uDisplay::dim(uint8_t dim) {
//   dim8(((uint32_t)dim * 255) / 15);
// }

// dim is 0..255
void uDisplay::dim10(uint8_t dim, uint16_t dim_gamma) {           // dimmer with 8 bits resolution, 0..255. Gamma correction must be done by caller
  dimmer8 = dim;
  dimmer10_gamma = dim_gamma;
  if (ep_mode) {
    return;
  }

#ifdef ESP32              // TODO should we also add a ESP8266 version for bpanel?
  if (bpanel >= 0) {      // is the BaclPanel GPIO configured
    analogWrite(bpanel, dimmer10_gamma);
    // ledcWrite(ESP32_PWM_CHANNEL, dimmer8_gamma);
  } else if (dim_cbp) {
    dim_cbp(dim);
  }
#endif
  if (interface == _UDSP_SPI) {
    if (dim_op != 0xff) {   // send SPI command if dim configured
      SPI_BEGIN_TRANSACTION
      SPI_CS_LOW
      spi_command(dim_op);
      spi_data8(dimmer8);
      SPI_CS_HIGH
      SPI_END_TRANSACTION
    }
  }
}

// the cases are PSEUDO_OPCODES from MODULE_DESCRIPTOR
// and may be exapnded with more opcodes
void uDisplay::TS_RotConvert(int16_t *x, int16_t *y) {
  int16_t temp;

  if (rot_t[cur_rot] & 0x80) {
    temp = *y;
    *y = *x;
    *x = temp;
  }

  if (rotmap_xmin >= 0) {
    *y = map(*y, rotmap_ymin, rotmap_ymax, 0, gys);
    *x = map(*x, rotmap_xmin, rotmap_xmax, 0, gxs);
    *x = constrain(*x, 0, gxs);
    *y = constrain(*y, 0, gys);
  }
//  *x = constrain(*x, 0, gxs);
//  *y = constrain(*y, 0, gys);

  //Serial.printf("rot 1 %d - %d\n",*x,*y );

  switch (rot_t[cur_rot] & 0xf) {
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
    case 4:
      *x = width() - *x;
      break;
    case 5:
      *y = height() - *y;
      break;
  }

  //Serial.printf("rot 2 %d - %d\n",*x,*y );
}

uint8_t uDisplay::strlen_ln(char *str) {
  for (uint32_t cnt = 0; cnt < 256; cnt++) {
    if (!str[cnt] || str[cnt] == '\n' || str[cnt] == ' ') return cnt;
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
  if (!str2c(sp, ibuff, sizeof(ibuff))) {
    return atoi(ibuff);
  }
  return 0xff;
}

uint32_t uDisplay::next_hex(char **sp) {
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
void uDisplay::hw_write9(uint8_t val, uint8_t dc) {

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
void uDisplay::hw_write9(uint8_t val, uint8_t dc) {

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

// slow software spi needed for displays with max 10 Mhz clck

void USECACHE uDisplay::write8(uint8_t val) {
  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    GPIO_CLR(spi_clk);
    if (val & bit) GPIO_SET(spi_mosi);
    else   GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);
  }
}

void uDisplay::write8_slow(uint8_t val) {
  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    GPIO_CLR_SLOW(spi_clk);
    if (val & bit) GPIO_SET_SLOW(spi_mosi);
    else   GPIO_CLR_SLOW(spi_mosi);
    GPIO_SET_SLOW(spi_clk);
  }
}

void USECACHE uDisplay::write9(uint8_t val, uint8_t dc) {

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

void uDisplay::write9_slow(uint8_t val, uint8_t dc) {

  GPIO_CLR_SLOW(spi_clk);
  if (dc) GPIO_SET_SLOW(spi_mosi);
  else  GPIO_CLR_SLOW(spi_mosi);
  GPIO_SET_SLOW(spi_clk);

  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    GPIO_CLR_SLOW(spi_clk);
    if (val & bit) GPIO_SET_SLOW(spi_mosi);
    else   GPIO_CLR_SLOW(spi_mosi);
    GPIO_SET_SLOW(spi_clk);
  }
}

void USECACHE uDisplay::write16(uint16_t val) {
  for (uint16_t bit = 0x8000; bit; bit >>= 1) {
    GPIO_CLR(spi_clk);
    if (val & bit) GPIO_SET(spi_mosi);
    else   GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);
  }
}

void USECACHE uDisplay::write32(uint32_t val) {
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


void uDisplay::spi_data8_EPD(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  spi_data8(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay::spi_command_EPD(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  spi_command(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay::Init_EPD(int8_t p) {
  if (p == DISPLAY_INIT_PARTIAL) {
    if (lutpsize) {
      SetLut(lut_partial);
    }
  } else {
    if (lutfsize) {
      SetLut(lut_full);
    }
    if (lut_cnt[0]) {
      SetLuts();
    }
  }
  if (ep_mode == 1) {
    ClearFrameMemory(0xFF);
    Updateframe_EPD();
  } else {
    ClearFrame_42();
  }
  if (p == DISPLAY_INIT_PARTIAL) {
    delay(lutptime * 10);
  } else {
    delay(lutftime * 10);
  }
}

void uDisplay::ClearFrameMemory(unsigned char color) {
    SetMemoryArea(0, 0, gxs - 1, gys - 1);
    SetMemoryPointer(0, 0);
    spi_command_EPD(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(color);
    }
}

void uDisplay::SetLuts(void) {
  uint8_t index, count;
  for (index = 0; index < 5; index++) {
    spi_command_EPD(lut_cmd[index]);                            //vcom
    for (count = 0; count < lut_cnt[index]; count++) {
        spi_data8_EPD(lut_array[count][index]);
    }
  }
}

void uDisplay::DisplayFrame_42(void) {
    uint16_t Width, Height;
    Width = (gxs % 8 == 0) ? (gxs / 8 ): (gxs / 8 + 1);
    Height = gys;

    spi_command_EPD(saw_2);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            spi_data8_EPD(framebuffer[i + j * Width] ^ 0xff);
        }
    }
    spi_command_EPD(saw_3);
    delay(100);
    Serial.printf("EPD Diplayframe\n");
}


void uDisplay::ClearFrame_42(void) {
    uint16_t Width, Height;
    Width = (gxs % 8 == 0)? (gxs / 8 ): (gxs / 8 + 1);
    Height = gys;

    spi_command_EPD(saw_1);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            spi_data8_EPD(0xFF);
        }
    }

    spi_command_EPD(saw_2);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            spi_data8_EPD(0xFF);
        }
    }

   spi_command_EPD(saw_3);
   delay(100);
   Serial.printf("EPD Clearframe\n");
}


void uDisplay::SetLut(const unsigned char* lut) {
    spi_command_EPD(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < lutfsize; i++) {
        spi_data8_EPD(lut[i]);
    }
}

void uDisplay::Updateframe_EPD(void) {
  if (ep_mode == 1) {
    SetFrameMemory(framebuffer, 0, 0, gxs, gys);
    DisplayFrame_29();
  } else {
    DisplayFrame_42();
  }
}

void uDisplay::DisplayFrame_29(void) {
    spi_command_EPD(DISPLAY_UPDATE_CONTROL_2);
    spi_data8_EPD(0xC4);
    spi_command_EPD(MASTER_ACTIVATION);
    spi_data8_EPD(TERMINATE_FRAME_READ_WRITE);
}

void uDisplay::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
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

void uDisplay::SetFrameMemory(const unsigned char* image_buffer) {
    SetMemoryArea(0, 0, gxs - 1, gys - 1);
    SetMemoryPointer(0, 0);
    spi_command_EPD(WRITE_RAM);
    /* send the image data */
    for (int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(image_buffer[i] ^ 0xff);
    }
}

void uDisplay::SetMemoryPointer(int x, int y) {
    spi_command_EPD(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    spi_data8_EPD((x >> 3) & 0xFF);
    spi_command_EPD(SET_RAM_Y_ADDRESS_COUNTER);
    spi_data8_EPD(y & 0xFF);
    spi_data8_EPD((y >> 8) & 0xFF);
}

void uDisplay::SetFrameMemory(
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
void uDisplay::DrawAbsolutePixel(int x, int y, int16_t color) {

    int16_t w = width(), h = height();
    if (cur_rot == 1 || cur_rot == 3) {
      renderer_swap(w, h);
    }

    if (x < 0 || x >= w || y < 0 || y >= h) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (color) {
            framebuffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        } else {
            framebuffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (color) {
            framebuffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        } else {
            framebuffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        }
    }
}

void uDisplay::drawPixel_EPD(int16_t x, int16_t y, uint16_t color) {
  if (!framebuffer) return;
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


void uDisplay::fillRect_EPD(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  for (uint32_t yp = y; yp < y + h; yp++) {
    for (uint32_t xp = x; xp < x + w; xp++) {
      drawPixel_EPD(xp , yp , color);
    }
  }
}
void uDisplay::drawFastVLine_EPD(int16_t x, int16_t y, int16_t h, uint16_t color) {
  while (h--) {
    drawPixel_EPD(x , y , color);
    y++;
  }
}
void uDisplay::drawFastHLine_EPD(int16_t x, int16_t y, int16_t w, uint16_t color) {
  while (w--) {
    drawPixel_EPD(x , y , color);
    x++;
  }
}


void uDisplay::beginTransaction(SPISettings s) {
#ifdef ESP32
  if (lvgl_param.use_dma) {
    dmaWait();
  } else {
    uspi->beginTransaction(s);
  }
#else
  uspi->beginTransaction(s);
#endif
}

void uDisplay::endTransaction(void) {
#ifdef ESP32
  if (lvgl_param.use_dma) {
    dmaBusy();
  } else {
    uspi->endTransaction();
  }
#else
  uspi->endTransaction();
#endif
}


// ESP 32 DMA section , derived from TFT_eSPI
#ifdef ESP32

/***************************************************************************************
** Function name:           initDMA
** Description:             Initialise the DMA engine - returns true if init OK
***************************************************************************************/
bool uDisplay::initDMA(bool ctrl_cs)
{
  if (DMA_Enabled) return false;

  esp_err_t ret;
  spi_bus_config_t buscfg = {
    .mosi_io_num = spi_mosi,
    .miso_io_num = -1,
    .sclk_io_num = spi_clk,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = width() * height() * 2 + 8, // TFT screen size
    .flags = 0,
    .intr_flags = 0
  };

  int8_t pin = -1;
  if (ctrl_cs) pin = spi_cs;

  spi_device_interface_config_t devcfg = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = SPI_MODE3,
    .duty_cycle_pos = 0,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = spi_speed*1000000,
    .input_delay_ns = 0,
    .spics_io_num = pin,
    .flags = SPI_DEVICE_NO_DUMMY, //0,
    .queue_size = 1,
    .pre_cb = 0, //dc_callback, //Callback to handle D/C line
    .post_cb = 0
  };
  ret = spi_bus_initialize(spi_host, &buscfg, 1);
  ESP_ERROR_CHECK(ret);
  ret = spi_bus_add_device(spi_host, &devcfg, &dmaHAL);
  ESP_ERROR_CHECK(ret);

  DMA_Enabled = true;
  spiBusyCheck = 0;
  return true;
}

/***************************************************************************************
** Function name:           deInitDMA
** Description:             Disconnect the DMA engine from SPI
***************************************************************************************/
void uDisplay::deInitDMA(void) {
  if (!DMA_Enabled) return;
  spi_bus_remove_device(dmaHAL);
  spi_bus_free(spi_host);
  DMA_Enabled = false;
}

/***************************************************************************************
** Function name:           dmaBusy
** Description:             Check if DMA is busy
***************************************************************************************/
bool uDisplay::dmaBusy(void) {
  if (!DMA_Enabled || !spiBusyCheck) return false;

  spi_transaction_t *rtrans;
  esp_err_t ret;
  uint8_t checks = spiBusyCheck;
  for (int i = 0; i < checks; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, 0);
    if (ret == ESP_OK) spiBusyCheck--;
  }

  //Serial.print("spiBusyCheck=");Serial.println(spiBusyCheck);
  if (spiBusyCheck == 0) return false;
  return true;
}


/***************************************************************************************
** Function name:           dmaWait
** Description:             Wait until DMA is over (blocking!)
***************************************************************************************/
void uDisplay::dmaWait(void) {
  if (!DMA_Enabled || !spiBusyCheck) return;
  spi_transaction_t *rtrans;
  esp_err_t ret;
  for (int i = 0; i < spiBusyCheck; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
  }
  spiBusyCheck = 0;
}


/***************************************************************************************
** Function name:           pushPixelsDMA
** Description:             Push pixels to TFT (len must be less than 32767)
***************************************************************************************/
// This will byte swap the original image if setSwapBytes(true) was called by sketch.
void uDisplay::pushPixelsDMA(uint16_t* image, uint32_t len) {

  if ((len == 0) || (!DMA_Enabled)) return;

  dmaWait();

  esp_err_t ret;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;  //finally send the line data
  trans.length = len * 16;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
}

/***************************************************************************************
** Function name:           pushPixelsDMA
** Description:             Push pixels to TFT (len must be less than 32767)
***************************************************************************************/
// This will byte swap the original image if setSwapBytes(true) was called by sketch.
void uDisplay::pushPixels3DMA(uint8_t* image, uint32_t len) {

  if ((len == 0) || (!DMA_Enabled)) return;

  dmaWait();

  esp_err_t ret;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;  //finally send the line data
  trans.length = len * 24;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
}



#endif // ESP32
