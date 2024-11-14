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

#ifdef USE_ESP32_S3
#include "esp_cache.h"
#endif // USE_ESP32_S3

#include "tasmota_options.h"


extern int Cache_WriteBack_Addr(uint32_t addr, uint32_t size);


//#define UDSP_DEBUG

#ifndef UDSP_LBSIZE
#define UDSP_LBSIZE 256
#endif

#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }

enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);

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
#ifdef UDSP_DEBUG
  Serial.printf("dealloc\n");
#endif
  if (frame_buffer) {
    free(frame_buffer);
  }

  if (lut_full) {
    free(lut_full);
  }

  if (lut_partial) {
    free(lut_partial);
  }

  for (uint16_t cnt = 0; cnt < MAX_LUTS; cnt++ ) {
    if (lut_array[cnt]) {
      free(lut_array[cnt]);
    }
  }

#ifdef USE_UNIVERSAL_TOUCH
  if (ut_init_code) {
    free(ut_init_code);
  }
  if (ut_touch_code) {
    free(ut_touch_code);
  }
  if (ut_getx_code) {
    free(ut_getx_code);
  }
  if (ut_gety_code) {
    free(ut_gety_code);
  }
#endif // USE_UNIVERSAL_TOUCH

#ifdef USE_ESP32_S3
  if (_dmadesc) {
    heap_caps_free(_dmadesc);
    _dmadesc = nullptr;
    _dmadesc_size = 0;
  }
  if (_i80_bus) {
    esp_lcd_del_i80_bus(_i80_bus);
  }
#endif // USE_ESP32_S3
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
  bpmode = 0;
  dsp_off = 0xff;
  dsp_on = 0xff;
  lutpsize = 0;
  lutfsize = 0;
  lutptime = 35;
  lutftime = 350;
  lut3time = 10;
  busy_pin = -1;
  spec_init = -1;
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
  epc_part_cnt = 0;
  epc_full_cnt = 0;
  lut_num = 0;
  lvgl_param.data = 0;
  lvgl_param.flushlines = 40;
  rot_t[0] = 0;
  rot_t[1] = 1;
  rot_t[2] = 2;
  rot_t[3] = 3;
  epcoffs_full = 0;
  epcoffs_part = 0;
  interface = 0;

  for (uint32_t cnt = 0; cnt < MAX_LUTS; cnt++) {
    lut_cnt[cnt] = 0;
    lut_cmd[cnt] = 0xff;
    lut_array[cnt] = 0;
  }

  lut_partial = 0;
  lut_full = 0;
  char linebuff[UDSP_LBSIZE];
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
          
          if (*lp1 == 'S') {
            // pecial case RGB with software SPI init clk,mosi,cs,reset
            lp1++;
            if (interface == _UDSP_RGB) { 
              // collect line and send directly
              lp1++;
              spi_nr = 4;
              spec_init = _UDSP_SPI;
              spi_dc = -1;
              spi_miso = -1;
              spi_clk = next_val(&lp1);
              spi_mosi = next_val(&lp1);
              spi_cs = next_val(&lp1);
              reset = next_val(&lp1);

              pinMode(spi_cs, OUTPUT);
              digitalWrite(spi_cs, HIGH);

              pinMode(spi_clk, OUTPUT);
              digitalWrite(spi_clk, LOW);

              pinMode(spi_mosi, OUTPUT);
              digitalWrite(spi_mosi, LOW);

              if (reset >= 0) {
                pinMode(reset, OUTPUT);
                digitalWrite(reset, HIGH);
                delay(50);
                reset_pin(50, 200);
              }
#ifdef UDSP_DEBUG
              Serial.printf("SSPI_MOSI  : %d\n", spi_mosi);
              Serial.printf("SSPI_SCLK  : %d\n", spi_clk);
              Serial.printf("SSPI_CS  : %d\n", spi_cs);
              Serial.printf("DSP RESET : %d\n", reset);
#endif
            }
          } else if (*lp1 == 'I') {
            // pecial case RGB with i2c init, bus nr, i2c addr
            lp1++;
            if (interface == _UDSP_RGB) { 
              // collect line and send directly
              lp1++;
              wire_n = next_val(&lp1);
              i2caddr = next_hex(&lp1);
#ifdef UDSP_DEBUG
              Serial.printf("I2C_INIT bus : %d\n", wire_n);
              Serial.printf("I2C_INIT addr : %02x\n", i2caddr);
#endif
              if (wire_n == 1) {
                wire = &Wire;
              } else {
#if SOC_HP_I2C_NUM > 1
                wire = &Wire1;
#else
                wire = &Wire;
#endif
              }
              spec_init = _UDSP_I2C;
            }
          }         
        } else if (section == 'L') {
          if (*lp1 >= '1' && *lp1 <= '5') {
            lut_num = (*lp1 & 0x07);
            lp1 += 2;
            lut_siz[lut_num - 1] = next_val(&lp1);
            lut_array[lut_num - 1] = (uint8_t*)malloc(lut_siz[lut_num - 1]);
            lut_cmd[lut_num - 1] = next_hex(&lp1);
          } else {
            lut_num = 0;
            lp1++;
            lut_siz_full = next_val(&lp1);
            lut_full = (uint8_t*)malloc(lut_siz_full);
            lut_cmd[0] = next_hex(&lp1);
          }
        } else if (section == 'l') {
          lp1++;
          lut_siz_partial = next_val(&lp1);
          lut_partial = (uint8_t*)malloc(lut_siz_partial);
          lut_cmd[0] = next_hex(&lp1);
        }
        if (*lp1 == ',') lp1++;
        
      }
      if (*lp1 && *lp1 != ':' && *lp1 != '\n' && *lp1 != ' ') {   // Add space char
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
              if (bpp == 16) {
                fg_col = GetColorFromIndex(fg_col);
                bg_col = GetColorFromIndex(bg_col);
              }
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
            } else if (!strncmp(ibuff, "PAR", 3)) {
#ifdef USE_ESP32_S3
              uint8_t bus = next_val(&lp1);
              if (bus == 8) {
                interface = _UDSP_PAR8;
              } else {
                interface = _UDSP_PAR16;
              }
              reset = next_val(&lp1);
              par_cs = next_val(&lp1);
              par_rs = next_val(&lp1);
              par_wr = next_val(&lp1);
              par_rd = next_val(&lp1);
              bpanel = next_val(&lp1);

              for (uint32_t cnt = 0; cnt < 8; cnt ++) {
                par_dbl[cnt] = next_val(&lp1);
              }

              if (interface == _UDSP_PAR16) {
                for (uint32_t cnt = 0; cnt < 8; cnt ++) {
                  par_dbh[cnt] = next_val(&lp1);
                }
              }
              spi_speed = next_val(&lp1);
#endif // USE_ESP32_S3
              section = 0;
            }  else if (!strncmp(ibuff, "RGB", 3)) {
#ifdef USE_ESP32_S3
              interface = _UDSP_RGB;

              de = next_val(&lp1);
              vsync = next_val(&lp1);
              hsync = next_val(&lp1);
              pclk = next_val(&lp1);
              bpanel = next_val(&lp1);

              for (uint32_t cnt = 0; cnt < 8; cnt ++) {
                par_dbl[cnt] = next_val(&lp1);
              }

              for (uint32_t cnt = 0; cnt < 8; cnt ++) {
                par_dbh[cnt] = next_val(&lp1);
              }
              spi_speed = next_val(&lp1);
#endif // USE_ESP32_S3
              section = 0;
            }
            break;
          case 'S':
            splash_font = next_val(&lp1);
            splash_size = next_val(&lp1);
            fg_col = next_val(&lp1);
            bg_col = next_val(&lp1);
            if (bpp == 16) {
              fg_col = GetColorFromIndex(fg_col);
              bg_col = GetColorFromIndex(bg_col);
            }
            splash_xp = next_val(&lp1);
            splash_yp = next_val(&lp1);
            break;
          case 'I':
            // init data
            if (interface == _UDSP_RGB && spec_init > 0) {
              // special case RGB with SPI or I2C init
              // collect line and send directly
              dsp_ncmds = 0;
              while (1) {
                if (dsp_ncmds >= sizeof(dsp_cmds)) break;
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  dsp_cmds[dsp_ncmds++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
              }
              if (spec_init == _UDSP_SPI) {
                interface = spec_init;
                send_spi_icmds(dsp_ncmds);
              } else {
                if (dsp_ncmds == 2) {
                  wire->beginTransmission(i2caddr);
                  wire->write(dsp_cmds[0]);
                  wire->write(dsp_cmds[1]);
                  wire->endTransmission();
#ifdef UDSP_DEBUG
                  Serial.printf("reg = %02x, val = %02x\n", dsp_cmds[0], dsp_cmds[1]);
#endif
                } else {
                  delay(dsp_cmds[0]);
#ifdef UDSP_DEBUG
                  Serial.printf("delay = %d ms\n", dsp_cmds[0]);
#endif
                }
              }
              interface = _UDSP_RGB;
            } else {
              if (interface == _UDSP_I2C) {
                dsp_cmds[dsp_ncmds++] = next_hex(&lp1);
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  dsp_cmds[dsp_ncmds++] = strtol(ibuff, 0, 16);
                }
              } else {
                while (1) {
                  if (dsp_ncmds >= sizeof(dsp_cmds)) break;
                  if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                    dsp_cmds[dsp_ncmds++] = strtol(ibuff, 0, 16);
                  } else {
                    break;
                  }
                }
              }
            }
            break;
          case 'f':
            // epaper full update cmds
            if (!epcoffs_full) {
              epcoffs_full = dsp_ncmds;
              epc_full_cnt = 0;
            }
            while (1) {
              if (epc_full_cnt >= sizeof(dsp_cmds)) break;
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                dsp_cmds[epcoffs_full + epc_full_cnt++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
            }
            break;
          case 'p':
            // epaper partial update cmds
            if (!epcoffs_part) {
              epcoffs_part = dsp_ncmds + epc_full_cnt;
              epc_part_cnt = 0;
            }
            while (1) {
              if (epc_part_cnt >= sizeof(dsp_cmds)) break;
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                dsp_cmds[epcoffs_part + epc_part_cnt++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
            }
            break;
#ifdef USE_ESP32_S3
          case 'V':
            hsync_polarity = next_val(&lp1);
            hsync_front_porch = next_val(&lp1);
            hsync_pulse_width = next_val(&lp1);
            hsync_back_porch = next_val(&lp1);
            vsync_polarity = next_val(&lp1);
            vsync_front_porch = next_val(&lp1);
            vsync_pulse_width = next_val(&lp1);
            vsync_back_porch = next_val(&lp1);
            pclk_active_neg = next_val(&lp1);
            break;
#endif // USE_ESP32_S3
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
            if (interface != _UDSP_RGB) {
              rot[0] = next_hex(&lp1);
              x_addr_offs[0] = next_hex(&lp1);
              y_addr_offs[0] = next_hex(&lp1);
            }
            rot_t[0] = next_hex(&lp1);
            break;
          case '1':
            if (interface != _UDSP_RGB) {
              rot[1] = next_hex(&lp1);
              x_addr_offs[1] = next_hex(&lp1);
              y_addr_offs[1] = next_hex(&lp1);
            }
            rot_t[1] = next_hex(&lp1);
            break;
          case '2':
            if (interface != _UDSP_RGB) {
              rot[2] = next_hex(&lp1);
              x_addr_offs[2] = next_hex(&lp1);
              y_addr_offs[2] = next_hex(&lp1);
            }
            rot_t[2] = next_hex(&lp1);
            break;
          case '3':
            if (interface != _UDSP_RGB) {
              rot[3] = next_hex(&lp1);
              x_addr_offs[3] = next_hex(&lp1);
              y_addr_offs[3] = next_hex(&lp1);
            }
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
          case 'a':
            saw_1 = next_hex(&lp1);
            saw_2 = next_hex(&lp1);
            saw_3 = next_hex(&lp1);
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
              if (!lut_full) {
                break;
              }
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  lut_full[lutfsize++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
                if (lutfsize >= lut_siz_full) break;
              }
            } else {
              uint8_t index = lut_num - 1;
              if (!lut_array[index]) {
                break;
              }
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  lut_array[index][lut_cnt[index]++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
                if (lut_cnt[index] >= lut_siz[index]) break;
              }
            }
            break;
          case 'l':
            if (!lut_partial) {
              break;
            }
            while (1) {
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                lut_partial[lutpsize++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
              if (lutpsize >= lut_siz_partial) break;
            }
            break;
          case 'T':
            lutftime = next_val(&lp1);
            lutptime = next_val(&lp1);
            lut3time = next_val(&lp1);
            break;
          case 'B':
            lvgl_param.flushlines = next_val(&lp1);
            lvgl_param.data = next_val(&lp1);
            // temporary fix to disable DMA due to a problem in esp-idf 5.3
#ifdef ESP32
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
            lvgl_param.use_dma = false;
#endif
#endif
            break;
          case 'M':
            rotmap_xmin = next_val(&lp1);
            rotmap_xmax = next_val(&lp1);
            rotmap_ymin = next_val(&lp1);
            rotmap_ymax = next_val(&lp1);
            break;
          case 'b':
            bpmode = next_val(&lp1);
            break;
#ifdef USE_UNIVERSAL_TOUCH
          case 'U':
            if (!strncmp(lp1, "TI", 2)) {
              // init
              ut_wire = 0;
              ut_reset = -1;
              ut_irq = -1;
              lp1 += 3;
              str2c(&lp1, ut_name, sizeof(ut_name));
              if (*lp1 == 'I') {
                // i2c mode
                lp1++;
                uint8_t ut_mode = *lp1 & 0xf;
                lp1 += 2;
                ut_i2caddr = next_hex(&lp1);
                ut_reset = next_val(&lp1);
                ut_irq = next_val(&lp1);

                if (ut_mode == 1) {
                  ut_wire = &Wire;
                } else {
#if SOC_HP_I2C_NUM > 1
                  ut_wire = &Wire1;
#else
                  ut_wire = &Wire;
#endif
                }
              } else if (*lp1 == 'S') {
                // spi mode
                lp1++;
                ut_spi_nr = *lp1 & 0xf;
                lp1 += 2;
                ut_spi_cs = next_val(&lp1);
                ut_reset = next_val(&lp1);
                ut_irq = next_val(&lp1);
                pinMode(ut_spi_cs, OUTPUT);
                digitalWrite(ut_spi_cs, HIGH);
                ut_spiSettings = SPISettings(2000000, MSBFIRST, SPI_MODE0);
              } else {
                // simple resistive touch
                lp1++;
              }
              ut_trans(&lp, &ut_init_code);
            } else if (!strncmp(lp1, "TT", 2)) {
              lp1 += 2;
              // touch
              ut_trans(&lp, &ut_touch_code);
            } else if (!strncmp(lp1, "TX", 2)) {
              lp1 += 2;
              // get x
              ut_trans(&lp, &ut_getx_code);
            } else if (!strncmp(lp1, "TY", 2)) {
              lp1 += 2;
              // get y
              ut_trans(&lp, &ut_gety_code);
            }
            break;
#endif // USE_UNIVERSAL_TOUCH
        }
      }
    }
    nextline:
    if (*lp == '\n' || *lp == ' ') {   // Add space char
      lp++;
    } else {
      char *lp1;
      lp1 = strchr(lp, '\n');
      if (!lp1) {
        lp1 = strchr(lp, ' ');
        if (!lp1) {
          break;
        }
      }
      lp = lp1 + 1;
    }
  }

  if (lutfsize && lutpsize) {
    // 2 table mode
    ep_mode = 1;
  }

  if (lut_cnt[0] > 0 && lut_cnt[1] == lut_cnt[2] && lut_cnt[1] == lut_cnt[3] && lut_cnt[1] == lut_cnt[4]) {
    // 5 table mode
    ep_mode = 2;
  }


#ifdef USE_ESP32_S3
void UfsCheckSDCardInit(void);

  if (spec_init == _UDSP_SPI) {
    // special case, assuming sd card and display on same spi bus
    // end spi in case it was running
    SPI.end();
    // reininit SD card
    UfsCheckSDCardInit();
  }
#endif

  if ((epcoffs_full || epcoffs_part) && !(lutfsize || lutpsize)) {
    // no lutfsize or lutpsize, but epcoffs_full or epcoffs_part
    ep_mode = 3;
  }

#ifdef UDSP_DEBUG
  Serial.printf("Device : %s\n", dname);
  Serial.printf("xs : %d\n", gxs);
  Serial.printf("ys : %d\n", gys);
  Serial.printf("bpp: %d\n", bpp);

  if (interface == _UDSP_SPI) {
    Serial.printf("Nr. : %d\n", spi_nr);
    Serial.printf("CS  : %d\n", spi_cs);
    Serial.printf("CLK : %d\n", spi_clk);
    Serial.printf("MOSI: %d\n", spi_mosi);
    Serial.printf("DC  : %d\n", spi_dc);
    Serial.printf("TS_CS: %d\n", ut_spi_cs);
    Serial.printf("TS_RST: %d\n", ut_reset);
    Serial.printf("TS_IRQ: %d\n", ut_irq);
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
      Serial.printf("LUT_Partial : %d - %d - %x - %d - %d\n", lut_siz_partial, lutpsize, lut_cmd[0], epcoffs_part, epc_part_cnt);
      Serial.printf("LUT_Full : %d - %d - %x - %d - %d\n", lut_siz_full, lutfsize, lut_cmd[0], epcoffs_full, epc_full_cnt);
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

  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
#ifdef USE_ESP32_S3
    Serial.printf("par  mode: %d\n", interface);
    Serial.printf("par  res: %d\n", reset);
    Serial.printf("par  cs : %d\n", par_cs);
    Serial.printf("par  rs : %d\n", par_rs);
    Serial.printf("par  wr : %d\n", par_wr);
    Serial.printf("par  rd : %d\n", par_rd);
    Serial.printf("par  bp : %d\n", bpanel);

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      Serial.printf("par  d%d: %d\n", cnt, par_dbl[cnt]);
    }

    if (interface == _UDSP_PAR16) {
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        Serial.printf("par  d%d: %d\n", cnt + 8, par_dbh[cnt]);
      }
    }
    Serial.printf("par  freq : %d\n", spi_speed);
#endif // USE_ESP32_S3

  }
  if (interface == _UDSP_RGB) {
#ifdef USE_ESP32_S3

    Serial.printf("rgb  de: %d\n", de);
    Serial.printf("rgb  vsync: %d\n", vsync);
    Serial.printf("rgb  hsync : %d\n", hsync);
    Serial.printf("rgb  pclk : %d\n", pclk);
    Serial.printf("rgb  bp : %d\n", bpanel);

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      Serial.printf("rgb  d%d: %d\n", cnt, par_dbl[cnt]);
    }
    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      Serial.printf("rgb  d%d: %d\n", cnt + 8, par_dbh[cnt]);
    }

    Serial.printf("rgb  freq : %d\n", spi_speed);

    Serial.printf("rgb  hsync_polarity: %d\n", hsync_polarity);
    Serial.printf("rgb  hsync_front_porch: %d\n", hsync_front_porch);
    Serial.printf("rgb  hsync_pulse_width : %d\n", hsync_pulse_width);
    Serial.printf("rgb  hsync_back_porch : %d\n", hsync_back_porch);
    Serial.printf("rgb  vsync_polarity : %d\n", vsync_polarity);
    Serial.printf("rgb  vsync_front_porch : %d\n", vsync_front_porch);
    Serial.printf("rgb  vsync_pulse_width : %d\n", vsync_pulse_width);
    Serial.printf("rgb  vsync_back_porch : %d\n", vsync_back_porch);
    Serial.printf("rgb  pclk_active_neg : %d\n", pclk_active_neg);

#endif // USE_ESP32_S3
  }
#endif

#ifdef UDSP_DEBUG
  Serial.printf("Dsp class init complete\n");
#endif
}

void uDisplay::delay_arg(uint32_t args) {
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

// epaper pseudo opcodes
#define EP_RESET 0x60
#define EP_LUT_FULL 0x61
#define EP_LUT_PARTIAL 0x62
#define EP_WAITIDLE 0x63
#define EP_SET_MEM_AREA 0x64
#define EP_SET_MEM_PTR 0x65
#define EP_SEND_DATA 0x66
#define EP_CLR_FRAME 0x67
#define EP_SEND_FRAME 0x68
#define EP_BREAK_RR_EQU 0x69
#define EP_BREAK_RR_NEQ 0x6a

extern int32_t ESP_ResetInfoReason();

// special init for GC displays
void uDisplay::send_spi_icmds(uint16_t cmd_size) {
uint16_t index = 0;
uint16_t cmd_offset = 0;


#ifdef UDSP_DEBUG
  Serial.printf("start send icmd table\n");
#endif
  while (1) {
    uint8_t iob;
    SPI_CS_LOW
    iob = dsp_cmds[cmd_offset++];
    index++;
    ulcd_command(iob);
    uint8_t args = dsp_cmds[cmd_offset++];
    index++;
#ifdef UDSP_DEBUG
    Serial.printf("cmd, args %02x, %d ", iob, args & 0x7f);
#endif
    for (uint32_t cnt = 0; cnt < (args & 0x7f); cnt++) {
      iob = dsp_cmds[cmd_offset++];
      index++;
#ifdef UDSP_DEBUG
      Serial.printf("%02x ", iob);
#endif
      ulcd_data8(iob);
    }
    SPI_CS_HIGH
#ifdef UDSP_DEBUG
    Serial.printf("\n");
#endif
    if (args & 0x80) {  // delay after the command
      delay_arg(args);
    }
    if (index >= cmd_size) break;
  }
#ifdef UDSP_DEBUG
  Serial.printf("end send icmd table\n");
#endif
  return;
}


void uDisplay::send_spi_cmds(uint16_t cmd_offset, uint16_t cmd_size) {
uint16_t index = 0;
#ifdef UDSP_DEBUG
  Serial.printf("start send cmd table\n");
#endif
  while (1) {
    uint8_t iob;
    SPI_CS_LOW
    iob = dsp_cmds[cmd_offset++];
    index++;
    if ((ep_mode == 1 || ep_mode == 3) && iob >= EP_RESET) {
      // epaper pseudo opcodes
      uint8_t args = dsp_cmds[cmd_offset++];
      index++;
#ifdef UDSP_DEBUG
      Serial.printf("cmd, args %02x, %d ", iob, args & 0x1f);
#endif
      switch (iob) {
        case EP_RESET:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
          }
          reset_pin(iob, iob);
          break;
        case EP_LUT_FULL:
          SetLut(lut_full);
          ep_update_mode = DISPLAY_INIT_FULL;
          break;
        case EP_LUT_PARTIAL:
          SetLut(lut_partial);
          ep_update_mode = DISPLAY_INIT_PARTIAL;
          break;
        case EP_WAITIDLE:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
          }
          //delay(iob * 10);
          delay_sync(iob * 10);
          break;
        case EP_SET_MEM_AREA:
          SetMemoryArea(0, 0, gxs - 1, gys - 1);
          break;
        case EP_SET_MEM_PTR:
          SetMemoryPointer(0, 0);
          break;
        case EP_SEND_DATA:
          Send_EP_Data();
          break;
        case EP_CLR_FRAME:
          ClearFrameMemory(0xFF);
          break;
        case EP_SEND_FRAME:
          SetFrameMemory(framebuffer);
          break;
        case EP_BREAK_RR_EQU:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
            if (iob == ESP_ResetInfoReason()) {
              ep_update_mode = DISPLAY_INIT_PARTIAL;
              goto exit;
            }
          }
          break;
        case EP_BREAK_RR_NEQ:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
            if (iob != ESP_ResetInfoReason()) {
              ep_update_mode = DISPLAY_INIT_PARTIAL;
              goto exit;
            }
          }
          break;
      }
#ifdef UDSP_DEBUG
      if (args & 1) {
        Serial.printf("%02x ", iob );
      }
      Serial.printf("\n");
#endif
      if (args & 0x80) {  // delay after the command
        delay_arg(args);
      }
    } else {
      if (spi_dc == -2) {
        // pseudo opcodes
        switch (iob) {
          case UDSP_WRITE_16:
            break;
          case UDSP_READ_DATA:
            break;
          case UDSP_READ_STATUS:
            break;
        }
      }
      ulcd_command(iob);
      uint8_t args = dsp_cmds[cmd_offset++];
      index++;
#ifdef UDSP_DEBUG
      Serial.printf("cmd, args %02x, %d ", iob, args & 0x1f);
#endif
      for (uint32_t cnt = 0; cnt < (args & 0x1f); cnt++) {
        iob = dsp_cmds[cmd_offset++];
        index++;
#ifdef UDSP_DEBUG
        Serial.printf("%02x ", iob );
#endif
        if (!allcmd_mode) {
          ulcd_data8(iob);
        } else {
          ulcd_command(iob);
        }
      }
      SPI_CS_HIGH
#ifdef UDSP_DEBUG
      Serial.printf("\n");
#endif
      if (args & 0x80) {  // delay after the command
        delay_arg(args);
      }
    }
    if (index >= cmd_size) break;
  }

exit:
#ifdef UDSP_DEBUG
  Serial.printf("end send cmd table\n");
#endif
  return;
}

Renderer *uDisplay::Init(void) {
  extern bool UsePSRAM(void);

  if (!interface) {   // no valid configuration, abort
    #ifdef UDSP_DEBUG
    Serial.printf("Dsp Init no valid configuration\n");
    #endif
    return NULL;
  }

  #ifdef UDSP_DEBUG
    Serial.printf("Dsp Init 1 start \n");
  #endif

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
#endif // ESP8266
  }
  frame_buffer = framebuffer;

  if (interface == _UDSP_I2C) {
    if (wire_n == 0) {
      wire = &Wire;
    }
#if SOC_HP_I2C_NUM > 1
    if (wire_n == 1) {
      wire = &Wire1;
    }
#endif // ESP32
/*
    if (i2c_sda != i2c_scl) {
      wire->begin(i2c_sda, i2c_scl);    // TODO: aren't I2C buses already initialized? Shouldn't this be moved to display driver?
    }
*/
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
      if (spi_miso >= 0) {
        pinMode(spi_miso, INPUT_PULLUP);
        busy_pin = spi_miso;
      }
    }
#endif // ESP8266

#ifdef ESP32
    if (spi_nr == 1) {
      uspi = &SPI;
      uspi->begin(spi_clk, spi_miso, spi_mosi, -1);
      if (lvgl_param.use_dma) {
        spi_host = VSPI_HOST;
        initDMA(lvgl_param.async_dma ? spi_cs : -1);   // disable DMA CS if sync, we control it directly
      }

    } else if (spi_nr == 2) {
      uspi = new SPIClass(HSPI);
      uspi->begin(spi_clk, spi_miso, spi_mosi, -1);
      if (lvgl_param.use_dma) {
        spi_host = HSPI_HOST;
        initDMA(lvgl_param.async_dma ? spi_cs : -1);   // disable DMA CS if sync, we control it directly
      }
    } else {
      pinMode(spi_clk, OUTPUT);
      digitalWrite(spi_clk, LOW);
      pinMode(spi_mosi, OUTPUT);
      digitalWrite(spi_mosi, LOW);
      if (spi_miso >= 0) {
        busy_pin = spi_miso;
        pinMode(spi_miso, INPUT_PULLUP);
#ifdef UDSP_DEBUG
        Serial.printf("Dsp busy pin: %d\n", busy_pin);
#endif
      }
    }
#endif // ESP32


    spiSettings = SPISettings((uint32_t)spi_speed*1000000, MSBFIRST, SPI_MODE3);
    SPI_BEGIN_TRANSACTION

    if (reset >= 0) {
      pinMode(reset, OUTPUT);
      digitalWrite(reset, HIGH);
      delay(50);
      reset_pin(50, 200);
    }

    send_spi_cmds(0, dsp_ncmds);

    SPI_END_TRANSACTION

  }

  if (interface == _UDSP_RGB) {
#ifdef USE_ESP32_S3
    if (!UsePSRAM())  {        // RGB is not supported on S3 without PSRAM
      #ifdef UDSP_DEBUG
      Serial.printf("Dsp RGB requires PSRAM, abort\n");
      #endif
      return NULL;
    }

    if (bpanel >= 0) {
      analogWrite(bpanel, 32);
    }
    esp_lcd_rgb_panel_config_t *_panel_config = (esp_lcd_rgb_panel_config_t *)heap_caps_calloc(1, sizeof(esp_lcd_rgb_panel_config_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    _panel_config->clk_src = LCD_CLK_SRC_PLL160M;

    //if (spi_speed > 14) {
      //spi_speed = 14;
    //}
    _panel_config->timings.pclk_hz = spi_speed*1000000;
    _panel_config->timings.h_res = gxs;
    _panel_config->timings.v_res = gys;

    _panel_config->timings.hsync_pulse_width = hsync_pulse_width;
    _panel_config->timings.hsync_back_porch = hsync_back_porch;
    _panel_config->timings.hsync_front_porch = hsync_front_porch;
    _panel_config->timings.vsync_pulse_width = vsync_pulse_width;
    _panel_config->timings.vsync_back_porch = vsync_back_porch;
    _panel_config->timings.vsync_front_porch = vsync_front_porch;
    _panel_config->timings.flags.hsync_idle_low = (hsync_polarity == 0) ? 1 : 0;
    _panel_config->timings.flags.vsync_idle_low = (vsync_polarity == 0) ? 1 : 0;
    _panel_config->timings.flags.de_idle_high = 0;
    _panel_config->timings.flags.pclk_active_neg = pclk_active_neg;
    _panel_config->timings.flags.pclk_idle_high = 0;

    _panel_config->data_width = 16; // RGB565 in parallel mode, thus 16bit in width
    _panel_config->sram_trans_align = 8;
    _panel_config->psram_trans_align = 64;
    _panel_config->hsync_gpio_num = hsync;
    _panel_config->vsync_gpio_num = vsync;
    _panel_config->de_gpio_num = de;
    _panel_config->pclk_gpio_num = pclk;

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      _panel_config->data_gpio_nums[cnt] = par_dbh[cnt];
    }
    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      _panel_config->data_gpio_nums[cnt + 8] = par_dbl[cnt];
    }
    _panel_config->disp_gpio_num = GPIO_NUM_NC;

    _panel_config->flags.disp_active_low = 0;
#if ESP_IDF_VERSION_MAJOR >= 5
    _panel_config->flags.refresh_on_demand = 0;
#else
    _panel_config->flags.relax_on_idle = 0;
#endif // ESP_IDF_VERSION_MAJOR >= 5
    _panel_config->flags.fb_in_psram = 1;             // allocate frame buffer in PSRAM

    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(_panel_config, &_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(_panel_handle));

    uint16_t color = random(0xffff);
    ESP_ERROR_CHECK(_panel_handle->draw_bitmap(_panel_handle, 0, 0, 1, 1, &color));

#if ESP_IDF_VERSION_MAJOR < 5
    _rgb_panel = __containerof(_panel_handle, esp_rgb_panel_t, base);
    rgb_fb = (uint16_t *)_rgb_panel->fb;
#else
    void * buf = NULL;
    esp_lcd_rgb_panel_get_frame_buffer(_panel_handle, 1, &buf);
    rgb_fb = (uint16_t *)buf;
#endif



#endif // USE_ESP32_S3
  }

  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {

#ifdef USE_ESP32_S3

    if (bpanel >= 0) {
      analogWrite(bpanel, 32);
    }

    pinMode(par_cs, OUTPUT);
    digitalWrite(par_cs, HIGH);

    pinMode(par_rs, OUTPUT);
    digitalWrite(par_rs, HIGH);

    pinMode(par_wr, OUTPUT);
    digitalWrite(par_wr, HIGH);

    if (par_rd >= 0) {
      pinMode(par_rd, OUTPUT);
      digitalWrite(par_rd, HIGH);
    }

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        pinMode(par_dbl[cnt], OUTPUT);
    }

    uint8_t bus_width = 8;

    if (interface == _UDSP_PAR16) {
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
          pinMode(par_dbh[cnt], OUTPUT);
      }
      bus_width = 16;
    }

    if (reset >= 0) {
      pinMode(reset, OUTPUT);
      digitalWrite(reset, HIGH);
      delay(50);
      reset_pin(50, 200);
    }

    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = par_rs,
        .wr_gpio_num = par_wr,
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .bus_width = bus_width,
        .max_transfer_bytes = 32768
    };

    if (interface == _UDSP_PAR8) {
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        bus_config.data_gpio_nums[cnt] = par_dbl[cnt];
      }
    } else {
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        bus_config.data_gpio_nums[cnt] = par_dbl[cnt];
      }
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        bus_config.data_gpio_nums[cnt + 8] = par_dbh[cnt];
      }
    }

    // to disable SPI TRANSACTION
    spi_nr = 3;
    spi_cs = par_cs;

    _i80_bus = nullptr;

    esp_lcd_new_i80_bus(&bus_config, &_i80_bus);

#if ESP_IDF_VERSION_MAJOR < 5
    _dma_chan = _i80_bus->dma_chan;
#endif

    uint32_t div_a, div_b, div_n, clkcnt;
    calcClockDiv(&div_a, &div_b, &div_n, &clkcnt, 240*1000*1000, spi_speed*1000000);
    lcd_cam_lcd_clock_reg_t lcd_clock;
    lcd_clock.lcd_clkcnt_n = std::max((uint32_t)1u, clkcnt - 1); // ESP_IDF_VERSION_MAJOR >= 5
    lcd_clock.lcd_clk_equ_sysclk = (clkcnt == 1);
    lcd_clock.lcd_ck_idle_edge = true;
    lcd_clock.lcd_ck_out_edge = false;
    lcd_clock.lcd_clkm_div_num = div_n;
    lcd_clock.lcd_clkm_div_b = div_b;
    lcd_clock.lcd_clkm_div_a = div_a;
    lcd_clock.lcd_clk_sel = 2; // clock_select: 1=XTAL CLOCK / 2=240MHz / 3=160MHz
    lcd_clock.clk_en = true;
    _clock_reg_value = lcd_clock.val;

    _alloc_dmadesc(1);

    _dev = &LCD_CAM;

    pb_beginTransaction();
    uint16_t index = 0;
    while (1) {
      uint8_t iob;
      cs_control(0);

      iob = dsp_cmds[index++];
      pb_writeCommand(iob, 8);

      uint8_t args = dsp_cmds[index++];
    #ifdef UDSP_DEBUG
      Serial.printf("cmd, args %02x, %d ", iob, args&0x1f);
    #endif
      for (uint32_t cnt = 0; cnt < (args & 0x1f); cnt++) {
        iob = dsp_cmds[index++];
    #ifdef UDSP_DEBUG
        Serial.printf("%02x ", iob );
    #endif
        pb_writeData(iob, 8);
      }
      cs_control(1);
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

    pb_endTransaction();


#endif // USE_ESP32_S3

  }

  // must init luts on epaper
  if (ep_mode) {
    if (ep_mode == 2) Init_EPD(DISPLAY_INIT_FULL);
    //if (ep_mode == 1) Init_EPD(DISPLAY_INIT_PARTIAL);
  }

#ifdef UDSP_DEBUG
  Serial.printf("Dsp Init 1 complete \n");
#endif
  return this;
}

void uDisplay::DisplayInit(int8_t p, int8_t size, int8_t rot, int8_t font) {
  if (p != DISPLAY_INIT_MODE && ep_mode) {
    ep_update_mode = p;
    if (p == DISPLAY_INIT_PARTIAL) {
      if (lutpsize) {
#ifdef UDSP_DEBUG
        Serial.printf("init partial epaper mode\n");
#endif
        SetLut(lut_partial);
        Updateframe_EPD();
        delay_sync(lutptime * 10);
      }
      return;
    } else if (p == DISPLAY_INIT_FULL) {
#ifdef UDSP_DEBUG
      Serial.printf("init full epaper mode\n");
#endif
      if (lutfsize) {
        SetLut(lut_full);
        Updateframe_EPD();
      }
      if (ep_mode == 2) {
        ClearFrame_42();
        DisplayFrame_42();
      }
      delay_sync(lutftime * 10);
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
    Serial.printf("Dsp Init 2 complete \n");
#endif
  }
}

void uDisplay::reset_pin(int32_t msl, int32_t msh) {
  if (reset > 0) {
    digitalWrite(reset, LOW);
    delay(msl);
    digitalWrite(reset, HIGH);
    delay(msh);
  }
}

#define UDSP_BUSY_TIMEOUT 3000
// epaper sync or delay
void uDisplay::delay_sync(int32_t ms) {
  uint8_t busy_level = HIGH;
  if (lvgl_param.busy_invert) {
    busy_level = LOW;
  }
  uint32_t time = millis();
  if (busy_pin > 0) {

    while (digitalRead(busy_pin) == busy_level) {
      delay(1);
      if  ((millis() - time) > UDSP_BUSY_TIMEOUT) {
        break;
      }
    }
  } else {
    delay(ms);
  }
}


void uDisplay::ulcd_command(uint8_t val) {

  if (interface == _UDSP_SPI) {
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
    return;
  }

#ifdef USE_ESP32_S3
  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
    pb_writeCommand(val, 8);
  }
#endif // USE_ESP32_S3
}

void uDisplay::ulcd_data8(uint8_t val) {

  if (interface == _UDSP_SPI) {
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
    return;
  }

#ifdef USE_ESP32_S3
  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
    pb_writeData(val, 8);
  }
#endif // USE_ESP32_S3
}

void uDisplay::ulcd_data16(uint16_t val) {

  if (interface == _UDSP_SPI) {
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
    return;
  }

#ifdef USE_ESP32_S3
  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
    pb_writeData(val, 16);
  }
#endif // USE_ESP32_S3
}

void uDisplay::ulcd_data32(uint32_t val) {

  if (interface == _UDSP_SPI) {
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
    return;
  }

#ifdef USE_ESP32_S3
  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
    pb_writeData(val, 32);
  }
#endif // USE_ESP32_S3
}

void uDisplay::ulcd_command_one(uint8_t val) {

  if (interface == _UDSP_SPI) {
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    ulcd_command(val);
    SPI_CS_HIGH
    SPI_END_TRANSACTION
  }
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

  if (interface == _UDSP_RGB) {
    return;
  }

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
    // ulcd_command(saw_1 | 0x0);  // set low col = 0, 0x00
    // ulcd_command(i2c_page_start | 0x0);  // set hi col = 0, 0x10
    // ulcd_command(i2c_page_end | 0x0); // set startline line #0, 0x40

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
        ulcd_command(0xB0 + i + m_row); //set page address
        ulcd_command(m_col & 0xf); //set lower column address
        ulcd_command(0x10 | (m_col >> 4)); //set higher column address

        for ( j = 0; j < 8; j++) {
            for ( k = 0; k < xs; k++, p++) {
		            ulcd_data8(framebuffer[p]);
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

  if (framebuffer) {
    Renderer::drawFastVLine(x, y, h, color);
    return;
  }

  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((y + h - 1) >= _height) h = _height - y;


  if (interface == _UDSP_RGB) {
  #ifdef USE_ESP32_S3
    if (lvgl_param.swap_color) {
      color = color << 8 | color >> 8;
    }
    if (cur_rot > 0) {
      while (h--) {
        drawPixel_RGB(x , y , color);
        y++;
      }
    } else {
      uint16_t *fb = rgb_fb;
      fb += (int32_t)y * _width;
      fb += x;
      while (h--) {
        *fb = color;
        Cache_WriteBack_Addr((uint32_t)fb, 2);
        fb+=_width;
        y++;
      }
    }
  #endif
    return;
  }

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
      ulcd_data8(r);
      ulcd_data8(g);
      ulcd_data8(b);
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

  if (framebuffer) {
    Renderer::drawFastHLine(x, y, w, color);
    return;
  }

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width - x;


  if (interface == _UDSP_RGB) {
#ifdef USE_ESP32_S3
    if (lvgl_param.swap_color) {
      color = color << 8 | color >> 8;
    }
    if (cur_rot > 0) {
      while (w--) {
        drawPixel_RGB(x , y , color);
        x++;
      }
    } else {
      uint16_t *fb = rgb_fb;
      fb += (int32_t)y * _width;
      fb += x;
      while (w--) {
        *fb = color;
        Cache_WriteBack_Addr((uint32_t)fb, 2);
        fb++;
        x++;
      }
    }
  #endif
    return;
  }


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
      ulcd_data8(r);
      ulcd_data8(g);
      ulcd_data8(b);
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

  if (interface == _UDSP_RGB) {
    for (uint32_t yp = y; yp < y + h; yp++) {
      drawFastHLine(x, yp, w, color);
    }
    return;
  }


  if (ep_mode) {
    fillRect_EPD(x, y, w, h, color);
    return;
  }

  if (framebuffer) {
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
        ulcd_data8(r);
        ulcd_data8(g);
        ulcd_data8(b);
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

#ifdef USE_UNIVERSAL_TOUCH

uint8_t ut_irq_flg;

void IRAM_ATTR ut_touch_irq(void) {
  ut_irq_flg = 1;
}

// universal touch driver
bool uDisplay::utouch_Init(char **name) {
  *name = ut_name;
  if (ut_init_code) {
    if (ut_reset >= 0) {
      pinMode(ut_reset, OUTPUT);
      digitalWrite(ut_reset, HIGH);
      delay(10);
      digitalWrite(ut_reset, LOW);
      delay(5);
      digitalWrite(ut_reset, HIGH);
      delay(10);
    }
    if (ut_irq >= 0) {
      pinMode(ut_irq, INPUT );
      attachInterrupt(ut_irq, ut_touch_irq, FALLING);
    }

extern SPIClass *SpiBegin(uint32 bus);

    if (ut_spi_nr == spi_nr) {
      // same as display
      ut_spi = uspi;
    } else {
#ifdef ESP32
      ut_spi = SpiBegin(ut_spi_nr);
#endif
    }
    return ut_execute(ut_init_code);
  }
  return false;
}

uint16_t uDisplay::touched(void) {
  if (ut_irq >= 0) {
    if (!ut_irq_flg) {
      return false;
    }
    ut_irq_flg = 0;
  }
  if (ut_touch_code) {
    return ut_execute(ut_touch_code);
  }
  return 0;
}

int16_t uDisplay::getPoint_x(void) {
  if (ut_getx_code) {
    return ut_execute(ut_getx_code);
  }
  return 0;
}

int16_t uDisplay::getPoint_y(void) {
  if (ut_gety_code) {
    return ut_execute(ut_gety_code);
  }
  return 0;
}
#endif // USE_UNIVERSAL_TOUCH


void uDisplay::Splash(void) {

  if (splash_font < 0) return;

  if (ep_mode) {
    Updateframe();
    delay_sync(lut3time * 10);
  }
  setTextFont(splash_font);
  setTextSize(splash_size);
  DrawStringAt(splash_xp, splash_yp, dname, fg_col, 0);
  Updateframe();

#ifdef UDSP_DEBUG
  Serial.printf("draw splash\n");
#endif
}

void uDisplay::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  if (bpp != 16 || interface == _UDSP_RGB) {
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

  if (interface == _UDSP_RGB) {
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

    if (interface == _UDSP_RGB) {
      return;
    }

    x += x_addr_offs[cur_rot];
    y += y_addr_offs[cur_rot];

    if (sa_mode != 8) {
      uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
      uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

      ulcd_command(saw_1);
      ulcd_data32(xa);

      ulcd_command(saw_2);
      ulcd_data32(ya);

      if (saw_3 != 0xff) {
        ulcd_command(saw_3); // write to RAM
      }
    } else {
      uint16_t x2 = x + w - 1,
               y2 = y + h - 1;

      if (cur_rot & 1) { // Vertical address increment mode
        udisp_swap(x,y);
        udisp_swap(x2,y2);
      }
      ulcd_command(saw_1);
      if (allcmd_mode) {
        ulcd_data8(x);
        ulcd_data8(x2);
      } else {
        ulcd_command(x);
        ulcd_command(x2);
      }
      ulcd_command(saw_2);
      if (allcmd_mode) {
        ulcd_data8(y);
        ulcd_data8(y2);
      } else {
        ulcd_command(y);
        ulcd_command(y2);
      }
      if (saw_3 != 0xff) {
        ulcd_command(saw_3); // write to RAM
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
    seta_yp1++;
    if (lvgl_param.invert_bw) {
      for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
        uint16_t color = *data++;
        if (bpp == 1) color = (color & rgb16_to_mono_mask) ? 0 : 1;
        drawPixel(x, y, color);   // todo - inline the method to save speed
        len--;
        if (!len) return;         // failsafe - exist if len (pixel number) is exhausted
      }
    } else {
      for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
        uint16_t color = *data++;
        if (bpp == 1) color = (color & rgb16_to_mono_mask) ? 1 : 0;
        drawPixel(x, y, color);   // todo - inline the method to save speed
        len--;
        if (!len) return;         // failsafe - exist if len (pixel number) is exhausted
      }
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

  //Serial.printf("push %x - %d - %d - %d\n", (uint32_t)data, len, not_swapped, lvgl_param.data);

  // Isolating _UDSP_RGB to increase code sharing
  //
  // LVGL documentation suggest to call the following:
  //    lv_draw_sw_rgb565_swap() to invert bytes
  //    esp_lcd_panel_draw_bitmap() to paste bytes
  // but it appears to be faster to include the color swap in the copy loop
  // because the CPU is much faster than PSRAM (SPI bus), therefore
  // swapping bytes on the fly costs zero performance
  //
  // not_swapped == false : called from LVGL bytes are swapped
  // not_swapped == true : called from displaytext, no byte swap, currently no dma here
  if (interface == _UDSP_RGB) {
#ifdef USE_ESP32_S3
    // check that bytes count matches the size of area, and remove from inner loop
    if ((seta_yp2 - seta_yp1) * (seta_xp2 - seta_xp2) > len) { return; }

    uint16_t lenc = len;

    if (cur_rot > 0) {
      for (uint32_t y = seta_yp1; y < seta_yp2; y++) {
        seta_yp1++;
        for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
          uint16_t color = *data++;
          if (!not_swapped) { color = color << 8 | color >> 8; }
          drawPixel_RGB(x, y, color);
          lenc--;
          if (!lenc) return;         // failsafe - exist if len (pixel number) is exhausted
        }
      }
    } else {
      uint16_t *fb_y = rgb_fb + (int32_t)seta_yp1 * _width;
      for (uint32_t y = seta_yp1; y < seta_yp2; y++) {
        uint16_t * fb_xy = fb_y + seta_xp1;
        // we get the 'not_swapped' test outside of the inner loop
        if (not_swapped) {
          for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
            uint16_t color = *data++;
            *fb_xy = color;
            fb_xy++;
            lenc--;
            if (!lenc) break;         // failsafe - exist if len (pixel number) is exhausted
          }
        } else {
          for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
            uint16_t color = *data++;
            color = color << 8 | color >> 8;
            *fb_xy = color;
            fb_xy++;
            lenc--;
            if (!lenc) break;         // failsafe - exist if len (pixel number) is exhausted
          }
        }
        uint16_t * flush_ptr = rgb_fb + (int32_t)seta_yp1 * _width + seta_xp1;
        Cache_WriteBack_Addr((uint32_t)flush_ptr, (seta_xp2 - seta_xp1) * 2);
        fb_y += _width;
        seta_yp1++;
        if (!lenc) break; 
      }
    }
#endif
    return;
  }


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
        if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
  #ifdef USE_ESP32_S3
          pb_pushPixels(data, len, true, false);
  #endif // USE_ESP32_S3
        } else {
          lvgl_color_swap(data, len);
          while (len--) {
            WriteColor(*data++);
          }
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
      if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
#ifdef USE_ESP32_S3
        pb_pushPixels(data, len, false, false);
#endif // USE_ESP32_S3
      } else {
        while (len--) {
          WriteColor(*data++);
        }
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

    ulcd_data8(r);
    ulcd_data8(g);
    ulcd_data8(b);
  } else {
    ulcd_data16(color);
  }
}

#ifdef USE_ESP32_S3
void uDisplay::drawPixel_RGB(int16_t x, int16_t y, uint16_t color) {
int16_t w = _width, h = _height;

  if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) {
    return;
  }

  // check rotation, move pixel around if necessary
  switch (cur_rot) {
  case 1:
    renderer_swap(w, h);
    renderer_swap(x, y);
    x = w - x - 1;
    break;
  case 2:
    x = w - x - 1;
    y = h - y - 1;
    break;
  case 3:
    renderer_swap(w, h);
    renderer_swap(x, y);
    y = h - y - 1;
    break;
  }

  uint16_t *fb = rgb_fb;
  fb += (int32_t)y * w;
  fb += x;
  *fb = color;
  Cache_WriteBack_Addr((uint32_t)fb, 2);

}
#endif // USE_ESP32_S3

void uDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {

#ifdef USE_ESP32_S3
  if (interface == _UDSP_RGB) {
    if (lvgl_param.swap_color) {
      color = color << 8 | color >> 8;
    }
    drawPixel_RGB(x, y, color);
    return;
  }
#endif

  if (ep_mode) {
    drawPixel_EPD(x, y, color);
    return;
  }

  if (framebuffer) {
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

  if (framebuffer) {
    Renderer::setRotation(cur_rot);
    return;
  }

  if (interface == _UDSP_SPI || interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {

    if (ep_mode) {
      Renderer::setRotation(cur_rot);
      return;
    }
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    ulcd_command(madctrl);

    if (!allcmd_mode) {
      ulcd_data8(rot[cur_rot]);
    } else {
      ulcd_command(rot[cur_rot]);
    }

    if ((sa_mode == 8) && !allcmd_mode) {
      ulcd_command(startline);
      ulcd_data8((cur_rot < 2) ? height() : 0);
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

#define AW_PWMRES 1024

  if (interface == _UDSP_I2C) {
    if (on) {
      i2c_command(dsp_on);
    } else {
      i2c_command(dsp_off);
    }
  } else {
    if (on) {
      if (dsp_on != 0xff) ulcd_command_one(dsp_on);
      if (bpanel >= 0) {
#ifdef ESP32
        if (!bpmode) {
          analogWrite(bpanel, dimmer10_gamma);
        } else {
          analogWrite(bpanel, AW_PWMRES - dimmer10_gamma);
        }
#else
        if (!bpmode) {
          digitalWrite(bpanel, HIGH);
        } else {
          digitalWrite(bpanel, LOW);
        }
#endif
      }

    } else {
      if (dsp_off != 0xff) ulcd_command_one(dsp_off);
      if (bpanel >= 0) {
#ifdef ESP32
        if (!bpmode) {
          analogWrite(bpanel, 0);
        } else {
          analogWrite(bpanel, AW_PWMRES - 1);
        }
#else
        if (!bpmode) {
          digitalWrite(bpanel, LOW);
        } else {
          digitalWrite(bpanel, HIGH);
        }
#endif
      }
    }
  }
}

void uDisplay::invertDisplay(boolean i) {

  if (ep_mode) {
    return;
  }

  if (interface == _UDSP_SPI || interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
    if (i) {
      ulcd_command_one(inv_on);
    } else {
      ulcd_command_one(inv_off);
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
    if (!bpmode) {
      analogWrite(bpanel, dimmer10_gamma);
    } else {
      analogWrite(bpanel, AW_PWMRES - dimmer10_gamma);
    }

    // ledcWrite(ESP32_PWM_CHANNEL, dimmer8_gamma);
  } else if (dim_cbp) {
    dim_cbp(dim);
  }
#endif
  if (interface == _UDSP_SPI) {
    if (dim_op != 0xff) {   // send SPI command if dim configured
      SPI_BEGIN_TRANSACTION
      SPI_CS_LOW
      ulcd_command(dim_op);
      ulcd_data8(dimmer8);
      SPI_CS_HIGH
      SPI_END_TRANSACTION
    }
  }
}

// the cases are PSEUDO_OPCODES from MODULE_DESCRIPTOR
// and may be expanded with more opcodes
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



#ifdef USE_UNIVERSAL_TOUCH

float CharToFloat(const char *str);

uint32_t uDisplay::ut_par(char **lp, uint32_t mode) {
  char *cp = *lp;
  while (*cp != ' ') {
    if (!cp) break;
    cp++;
  }
  cp++;
  uint32_t result;
  if (!mode) {
    // hex
    result = strtol(cp, &cp, 16);
  } else if (mode == 1) {
    // word
    result = strtol(cp, &cp, 10);
  } else {
    // float as 32bit integer
    float fval = CharToFloat(cp);
    result = *(uint32_t*)&fval;
    while (*cp) {
      if (*cp == ' ' || *cp =='\n') {
        break;
      }
      cp++;
    }
  }
  *lp = cp;
  return result;
}

// translate pseudo opcodes to tokens
void uDisplay::ut_trans(char **sp, uint8_t **code) {
  char *cp = *sp;
  uint16_t wval;
  uint8_t tmp_code[64];
  uint8_t *ut_code = tmp_code;
  while (*cp) {
    if (*cp == ':' || *cp == '#') {
      break;
    }
    if (*cp == ';') {
      // skip comment line
      while (*cp) {
        if (*cp == '\n') {
          cp++;
          break;
        }
        cp++;
      }
    }
    if (!strncmp(cp, "RDWM", 4)) {
      // read word many
      *ut_code++ = UT_RDWM;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval>>8;
      *ut_code++ = wval;
      wval = ut_par(&cp, 1);
      if (wval > sizeof(ut_array)) {
        wval = sizeof(ut_array);
      }
      *ut_code++ = wval;
    } else if (!strncmp(cp, "RDW", 3)) {
      // read word one
      *ut_code++ = UT_RDW;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval>>8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "RDM", 3)) {
      // read many
      *ut_code++ = UT_RDM;
      *ut_code++ = ut_par(&cp, 0);
      wval = ut_par(&cp, 1);
      if (wval > sizeof(ut_array)) {
        wval = sizeof(ut_array);
      }
      *ut_code++ = wval;
    } else if (!strncmp(cp, "RD", 2)) {
      // read one
      *ut_code++ = UT_RD;
      *ut_code++ = ut_par(&cp, 0);
    } else if (!strncmp(cp, "CPR", 3)) {
      // cmp and set
      *ut_code++ = UT_CPR;
      *ut_code++ = ut_par(&cp, 0);
     } else if (!strncmp(cp, "CPM", 3)) {
      // cmp multiple and set
      *ut_code++ = UT_CPM;
      uint8_t num = ut_par(&cp, 0);
      *ut_code++ = num;
      for (uint32_t cnt = 0; cnt < num; cnt++) {
        *ut_code++ = ut_par(&cp, 0);
      }
     } else if (!strncmp(cp, "CP", 2)) {
      // cmp and set
      *ut_code++ = UT_CP;
      *ut_code++ = ut_par(&cp, 0);
    } else if (!strncmp(cp, "RTF", 3)) {
      // return when false
      *ut_code++ = UT_RTF;
    } else if (!strncmp(cp, "RTT", 3)) {
      // return when true
      *ut_code++ = UT_RTT;
    } else if (!strncmp(cp, "MVB", 3)) {
      // move
      *ut_code++ = UT_MVB;
      *ut_code++ = ut_par(&cp, 1);
      *ut_code++ = ut_par(&cp, 1);
    } else if (!strncmp(cp, "MV", 2)) {
      // move
      *ut_code++ = UT_MV;
      *ut_code++ = ut_par(&cp, 1);
      *ut_code++ = ut_par(&cp, 1);
    } else if (!strncmp(cp, "RT", 2)) {
      // return status
      *ut_code++ = UT_RT;
    } else if (!strncmp(cp, "WRW", 3)) {
      *ut_code++ = UT_WRW;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval>>8;
      *ut_code++ = wval;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval;
    } else if (!strncmp(cp, "WR", 2)) {
      *ut_code++ = UT_WR;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval;
    } else if (!strncmp(cp, "AND", 3)) {
      *ut_code++ = UT_AND;
      wval = ut_par(&cp, 0);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "SCL", 3)) {
      *ut_code++ = UT_SCALE;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
      uint32_t lval = ut_par(&cp, 2);
      *ut_code++ = lval >> 24;
      *ut_code++ = lval >> 16;
      *ut_code++ = lval >> 8;
      *ut_code++ = lval;
    } else if (!strncmp(cp, "LIM", 3)) {
      *ut_code++ = UT_LIM;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "GSRT", 4)) {
      *ut_code++ = UT_GSRT;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "XPT", 3)) {
      *ut_code++ = UT_XPT;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval >> 8;
      *ut_code++ = wval;
    } else if (!strncmp(cp, "DBG", 3)) {
      *ut_code++ = UT_DBG;
      wval = ut_par(&cp, 1);
      *ut_code++ = wval;
    }
    cp++;
  }
  *ut_code++ = UT_END;
  *sp = cp - 1;
  uint16_t memsize = (uint32_t)ut_code - (uint32_t)tmp_code;
  // allocate memory
  //AddLog(LOG_LEVEL_INFO, PSTR("UT-code: %d bytes"),memsize);
#ifdef UDSP_DEBUG
  Serial.printf("Utouch code size : %d\n", memsize);
#endif
  uint8_t *mp = (uint8_t*)malloc(memsize + 2);
  if (mp) {
    memmove(mp, tmp_code, memsize);
    *code = mp;
  }
}

uint8_t *uDisplay::ut_rd(uint8_t *iop, uint32_t len, uint32_t amode) {
  if (ut_wire) {
    // i2c mode
    ut_wire->beginTransmission(ut_i2caddr);
    ut_wire->write(*iop++);
    if (amode == 2) {
      ut_wire->write(*iop++);
    }
    ut_wire->endTransmission(false);
    if (len > 1) {
      len = *iop++;
    }
    ut_wire->requestFrom(ut_i2caddr, (size_t)len);
    uint8_t index = 0;
    while (ut_wire->available()) {
      ut_array[index++] = ut_wire->read();
    }
  } else {
    // spi mode
    if (amode == 1) {
      uint16_t val = *iop++;
      uint16_t len = *iop++;
      if (ut_spi) {
        digitalWrite(ut_spi_cs, LOW);
        ut_spi->beginTransaction(ut_spiSettings);
        ut_spi->transfer(val);
        val = ut_spi->transfer16(0);
        ut_spi->endTransaction();
        ut_array[len] = val << 8;
        ut_array[len + 1] = val;
        digitalWrite(ut_spi_cs, HIGH);
      }
    }
  }
  return iop;
}

uint8_t *uDisplay::ut_wr(uint8_t *iop, uint32_t amode) {
  if (ut_wire) {
    // i2c mode
    ut_wire->beginTransmission(ut_i2caddr);
    ut_wire->write(*iop++);
    if (amode == 2) {
      ut_wire->write(*iop++);
    }
    ut_wire->write(*iop++);
    ut_wire->endTransmission(true);
  } else {
    // spi mode
  }
  return iop;
}


int16_t uDisplay::besttwoavg( int16_t x , int16_t y , int16_t z ) {
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;

  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;

  return (reta);
}

uint16_t uDisplay::ut_XPT2046(uint16_t z_th) {
  uint16_t result = 0;
  if (ut_spi) {
    int16_t data[6];
		ut_spi->beginTransaction(ut_spiSettings);
		digitalWrite(ut_spi_cs, LOW);
		ut_spi->transfer(0xB1 /* Z1 */);
		int16_t z1 = ut_spi->transfer16(0xC1 /* Z2 */) >> 3;
		int16_t z = z1 + 4095;
		int16_t z2 = ut_spi->transfer16(0x91 /* X */) >> 3;
		z -= z2;
		if (z >= z_th) {
			ut_spi->transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
			data[0] = ut_spi->transfer16(0xD1 /* Y */) >> 3;
			data[1] = ut_spi->transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
			data[2] = ut_spi->transfer16(0xD1 /* Y */) >> 3;
			data[3] = ut_spi->transfer16(0x91 /* X */) >> 3;
      result = 1;
		}
		else {
      data[0] = data[1] = data[2] = data[3] = 0;
    }
    data[4] = ut_spi->transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
		data[5] = ut_spi->transfer16(0) >> 3;
		digitalWrite(ut_spi_cs, HIGH);
		ut_spi->endTransaction();

    uint16_t x = besttwoavg( data[0], data[2], data[4] );
	  uint16_t y = besttwoavg( data[1], data[3], data[5] );

    ut_array[0] = x >> 8;
    ut_array[1] = x;
    ut_array[2] = y >> 8;
    ut_array[3] = y;
	}
  return result;
}

int16_t uDisplay::ut_execute(uint8_t *ut_code) {
int16_t result = 0;
uint8_t iob, len;
uint16_t wval;

  while (*ut_code != UT_END) {
    iob = *ut_code++;
    switch (iob) {
      case UT_RD:
        // read 1 byte
        ut_code = ut_rd(ut_code, 1, 1);
        break;

      case UT_RDM:
        // read multiple bytes
        ut_code = ut_rd(ut_code, 2, 1);
        break;

      case UT_RDW:
        // read 1 byte
        ut_code = ut_rd(ut_code, 1, 2);
        break;

      case UT_RDWM:
        // read multiple bytes
        ut_code = ut_rd(ut_code, 2, 2);
        break;

      case UT_WR:
        ut_code = ut_wr(ut_code, 1);
        break;

       case UT_WRW:
        ut_code = ut_wr(ut_code, 2);
        break;

      case UT_CP:
        // compare
        iob = *ut_code++;
        result = (iob == ut_array[0]);
        break;

      case UT_CPM:
        // compare multiple
        len = *ut_code++;
        result = 0;
        for (uint32_t cnt = 0; cnt < len; cnt++) {
          iob = *ut_code++;
          result |= (iob == ut_array[0]);
        }
        break;

      case UT_CPR:
        // compare
        iob = *ut_code++;
        result = (iob == result);
        break;

      case UT_RTF:
        // return when false
        if (result == 0) {
          return false;
        }
        break;

      case UT_RTT:
         // return when true
        if (result > 0) {
          return false;
        }
        break;

      case UT_MVB:
        // move byte from index to high or low result
        wval = *ut_code++;
        iob = *ut_code++;
        if (wval == 0) {
          result &= 0xff00;
          result |= ut_array[iob]; 
        } else {
          result &= 0x00ff;
          result |= (ut_array[iob] << 8); 
        }
        break;

      case UT_MV:
        // move
        // source
        result = *ut_code++;
        iob = *ut_code++;
        if (iob == 1) {
          result = ut_array[result];
        } else if (iob == 2) {
          iob = result;
          result = ut_array[iob] << 8;
          result |= ut_array[iob + 1];
        } else {
          iob = result;
          result = ut_array[iob + 1] << 8;
          result |= ut_array[iob];
        }
        result &= 0xfff;
        break;

      case UT_AND:
        // and 
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        result &= wval;
        break;
      
      case UT_SCALE:
        {
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        result -= wval;
        uint32_t lval = (uint32_t)*ut_code++ << 24;
        lval |= (uint32_t)*ut_code++ << 16;
        lval |= (uint32_t)*ut_code++ << 8;
        lval |= (uint32_t)*ut_code++;
        float fval = *(float*)&lval;
        fval *= (float)result;  
        result = fval;
        }
        break;

      case UT_LIM:
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        if (result > wval) {
          result = wval;
        }
        break;

      case UT_RT:
        // result
        return result;
        break;

      case UT_GSRT:
#ifdef USE_ESP32_S3      
        { uint32_t val = get_sr_touch(SIMPLERS_XP, SIMPLERS_XM, SIMPLERS_YP, SIMPLERS_YM);
        if (val == 0) {
          return false;
        }
        uint16_t xp = val >> 16;
        uint16_t yp = val;

        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        if (xp > wval && yp > wval) {
          ut_array[0] = val >> 24;
          ut_array[1] = val >> 16;
          ut_array[2] = val >> 8;
          ut_array[3] = val;
          return true;
        }
        return false;
        }
#endif // USE_ESP32_S3
        break;

      case UT_XPT:
        wval = *ut_code++ << 8;
        wval |= *ut_code++;
        result = ut_XPT2046(wval);
        break;

      case UT_DBG:
        // debug show result
        //Serial.printf("UTDBG: %d\n", result);
        wval = *ut_code++;
        AddLog(LOG_LEVEL_INFO, PSTR("UTDBG %d: %02x : %02x,%02x,%02x,%02x"), wval, result, ut_array[0], ut_array[1], ut_array[2], ut_array[3]);
        break;

      case UT_END:
        break;
    }
  }
  return result;
}
#endif // USE_UNIVERSAL_TOUCH

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
      uint16_t slen = strlen(lp);
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

// since ardunio transferBits is completely disfunctional
// we use our own hardware driver for 9 bit spi
void uDisplay::hw_write9(uint8_t val, uint8_t dc) {

    if (spi_dc < -1) {
      // RA8876 mode
      if (!dc) {
        uspi->write(RA8876_CMD_WRITE);
        uspi->write(val);
      } else {
        uspi->write(RA8876_DATA_WRITE);
        uspi->write(val);
      }
    } else {
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
}

#else
#include "spi_register.h"
void uDisplay::hw_write9(uint8_t val, uint8_t dc) {

    if (spi_dc < -1) {
      // RA8876 mode
      if (!dc) {
        uspi->write(RA8876_CMD_WRITE);
        uspi->write(val);
      } else {
        uspi->write(RA8876_DATA_WRITE);
        uspi->write(val);
      }
    } else {
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

uint8_t uDisplay::writeReg16(uint8_t reg, uint16_t wval) {
  hw_write9(reg, 0);
  hw_write9(wval, 1);
  hw_write9(reg + 1, 0);
  hw_write9(wval >> 8, 1);
  return 0;
}

uint8_t uDisplay::readData(void) {
  uspi->write(RA8876_DATA_READ);
  uint8_t val = uspi->transfer(0);
  return val;
}

uint8_t uDisplay::readStatus(void) {
  uspi->write(RA8876_STATUS_READ);
  uint8_t val = uspi->transfer(0);
  return val;
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
  ulcd_data8(val);
  SPI_CS_HIGH
  SPI_END_TRANSACTION
}

void uDisplay::spi_command_EPD(uint8_t val) {
  SPI_BEGIN_TRANSACTION
  SPI_CS_LOW
  ulcd_command(val);
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
  } else if (ep_mode == 3) {
    ClearFrameMemory(0xFF);
    Updateframe_EPD();
  } else {
    ClearFrame_42();
  }
  if (p == DISPLAY_INIT_PARTIAL) {
    delay_sync(lutptime * 10);
  } else {
    delay_sync(lutftime * 10);
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
  for (index = 0; index < MAX_LUTS; index++) {
    spi_command_EPD(lut_cmd[index]);
    for (count = 0; count < lut_cnt[index]; count++) {
        spi_data8_EPD(lut_array[index][count]);
    }
  }
}

void uDisplay::DisplayFrame_42(void) {

    spi_command_EPD(saw_1);
    for(int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(0xFF);
    }
    delay(2);

    spi_command_EPD(saw_2);
    for(int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(framebuffer[i]^0xff);
    }
    delay(2);

    SetLuts();

    spi_command_EPD(saw_3);
    delay_sync(100);

#ifdef UDSP_DEBUG
    Serial.printf("EPD Diplayframe\n");
#endif
}




void uDisplay::ClearFrame_42(void) {
    spi_command_EPD(saw_1);
    for (uint16_t j = 0; j < gys; j++) {
        for (uint16_t i = 0; i < gxs; i++) {
            spi_data8_EPD(0xFF);
        }
    }

    spi_command_EPD(saw_2);
    for (uint16_t j = 0; j < gys; j++) {
        for (uint16_t i = 0; i < gxs; i++) {
            spi_data8_EPD(0xFF);
        }
    }

   spi_command_EPD(saw_3);
   delay_sync(100);
#ifdef UDSP_DEBUG
   Serial.printf("EPD Clearframe\n");
#endif
}

void uDisplay::SetLut(const unsigned char* lut) {
    //spi_command_EPD(WRITE_LUT_REGISTER);
    spi_command_EPD(lut_cmd[0]);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < lutfsize; i++) {
        spi_data8_EPD(lut[i]);
    }
}

void uDisplay::Updateframe_EPD(void) {
  if (ep_mode == 1 || ep_mode == 3) {
    switch (ep_update_mode) {
      case DISPLAY_INIT_PARTIAL:
        if (epc_part_cnt) {
          send_spi_cmds(epcoffs_part, epc_part_cnt);
        }
        break;
      case DISPLAY_INIT_FULL:
        if (epc_full_cnt) {
          send_spi_cmds(epcoffs_full, epc_full_cnt);
        }
        break;
      default:
        SetFrameMemory(framebuffer, 0, 0, gxs, gys);
        DisplayFrame_29();
    }
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
    int x_start1 = (x_start >> 3) & 0xFF;
    int x_end1 = (x_end >> 3) & 0xFF;
    int y_start1 = y_start & 0xFF;
    int y_start2 = (y_start >> 8) & 0xFF;
    int y_end1 = y_end & 0xFF;
    int y_end2 = (y_end >> 8) & 0xFF;
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */

    if (ep_mode == 3) {
        spi_command_EPD(SET_RAM_X_ADDRESS_START_END_POSITION);
        spi_data8_EPD(x_start1);
        spi_data8_EPD(x_end1);
        spi_command_EPD(SET_RAM_Y_ADDRESS_START_END_POSITION);
        spi_data8_EPD(y_end1);
        spi_data8_EPD(y_end2);
        spi_data8_EPD(y_start1);
        spi_data8_EPD(y_start2);
    } else {
        spi_command_EPD(SET_RAM_X_ADDRESS_START_END_POSITION);
        spi_data8_EPD(x_start1);
        spi_data8_EPD(x_end1);
        spi_command_EPD(SET_RAM_Y_ADDRESS_START_END_POSITION);
        spi_data8_EPD(y_start1);
        spi_data8_EPD(y_start2);
        spi_data8_EPD(y_end1);
        spi_data8_EPD(y_end2);
    }
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
    int x1;
    int y1;
    int y2;

    if (ep_mode == 3) {
        x1 = (x >> 3) & 0xFF;
        y--;
        y1 = y & 0xFF;
        y2 = (y >> 8) & 0xFF;
    } else {
        x1 = (x >> 3) & 0xFF;
        y1 = y & 0xFF;
        y2 = (y >> 8) & 0xFF;
    }

    spi_command_EPD(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    spi_data8_EPD(x1);
    spi_command_EPD(SET_RAM_Y_ADDRESS_COUNTER);
    spi_data8_EPD(y1);
    spi_data8_EPD(y2);
}

#if 0
void uDisplay::Send_EP_Data() {
  for (int i = 0; i < gys / 8 * gys; i++) {
      spi_data8_EPD(framebuffer[i]^0xff);
  }
}
#else
void uDisplay::Send_EP_Data() {
  uint16_t image_width = gxs & 0xFFF8;
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t x_end = gxs - 1;
  uint16_t y_end = gys - 1;

  for (uint16_t j = 0; j < y_end - y + 1; j++) {
    for (uint16_t i = 0; i < (x_end - x + 1) / 8; i++) {
        spi_data8_EPD(framebuffer[i + j * (image_width / 8)]^0xff);
    }
  }
}
#endif

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
  }
#endif
  uspi->beginTransaction(s);
}

void uDisplay::endTransaction(void) {
  uspi->endTransaction();
}


// ESP 32 DMA section , derived from TFT_eSPI
#ifdef ESP32

/***************************************************************************************
** Function name:           initDMA
** Description:             Initialise the DMA engine - returns true if init OK
***************************************************************************************/
bool uDisplay::initDMA(int32_t ctrl_cs)
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
    .spics_io_num = ctrl_cs,
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
  if (!lvgl_param.async_dma) {
    dmaWait();
  }
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
  if (!lvgl_param.async_dma) {
    dmaWait();
  }
}

#ifdef USE_ESP32_S3
void uDisplay::calcClockDiv(uint32_t* div_a, uint32_t* div_b, uint32_t* div_n, uint32_t* clkcnt, uint32_t baseClock, uint32_t targetFreq) {
    uint32_t diff = INT32_MAX;
    *div_n = 256;
    *div_a = 63;
    *div_b = 62;
    *clkcnt = 64;
    uint32_t start_cnt = std::min<uint32_t>(64u, (baseClock / (targetFreq * 2) + 1));
    uint32_t end_cnt = std::max<uint32_t>(2u, baseClock / 256u / targetFreq);
    if (start_cnt <= 2) { end_cnt = 1; }
    for (uint32_t cnt = start_cnt; diff && cnt >= end_cnt; --cnt)
    {
      float fdiv = (float)baseClock / cnt / targetFreq;
      uint32_t n = std::max<uint32_t>(2u, (uint32_t)fdiv);
      fdiv -= n;

      for (uint32_t a = 63; diff && a > 0; --a)
      {
        uint32_t b = roundf(fdiv * a);
        if (a == b && n == 256) {
          break;
        }
        uint32_t freq = baseClock / ((n * cnt) + (float)(b * cnt) / (float)a);
        uint32_t d = abs((int)targetFreq - (int)freq);
        if (diff <= d) { continue; }
        diff = d;
        *clkcnt = cnt;
        *div_n = n;
        *div_b = b;
        *div_a = a;
        if (b == 0 || a == b) {
          break;
        }
      }
    }
    if (*div_a == *div_b)
    {
        *div_b = 0;
        *div_n += 1;
    }
  }

void uDisplay::_alloc_dmadesc(size_t len) {
    if (_dmadesc) heap_caps_free(_dmadesc);
    _dmadesc_size = len;
    _dmadesc = (lldesc_t*)heap_caps_malloc(sizeof(lldesc_t) * len, MALLOC_CAP_DMA);
}

void uDisplay::_setup_dma_desc_links(const uint8_t *data, int32_t len) {
    static constexpr size_t MAX_DMA_LEN = (4096-4);
/*
    if (_dmadesc_size * MAX_DMA_LEN < len) {
      _alloc_dmadesc(len / MAX_DMA_LEN + 1);
    }
    lldesc_t *dmadesc = _dmadesc;

    while (len > MAX_DMA_LEN) {
      len -= MAX_DMA_LEN;
      dmadesc->buffer = (uint8_t *)data;
      data += MAX_DMA_LEN;
      *(uint32_t*)dmadesc = MAX_DMA_LEN | MAX_DMA_LEN << 12 | 0x80000000;
      dmadesc->next = dmadesc + 1;
      dmadesc++;
    }
    *(uint32_t*)dmadesc = ((len + 3) & ( ~3 )) | len << 12 | 0xC0000000;
    dmadesc->buffer = (uint8_t *)data;
    dmadesc->next = nullptr;
    */
  }

#define WAIT_LCD_NOT_BUSY while (*reg_lcd_user & LCD_CAM_LCD_START) {}


void uDisplay::pb_beginTransaction(void) {
    auto dev = _dev;
    dev->lcd_clock.val = _clock_reg_value;
    // int clk_div = std::min(63u, std::max(1u, 120*1000*1000 / (_cfg.freq_write+1)));
    // dev->lcd_clock.lcd_clk_sel = 2; // clock_select: 1=XTAL CLOCK / 2=240MHz / 3=160MHz
    // dev->lcd_clock.lcd_clkcnt_n = clk_div;
    // dev->lcd_clock.lcd_clk_equ_sysclk = 0;
    // dev->lcd_clock.lcd_ck_idle_edge = true;
    // dev->lcd_clock.lcd_ck_out_edge = false;

    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
    // dev->lcd_misc.lcd_cd_idle_edge = 1;
    // dev->lcd_misc.lcd_cd_cmd_set = 0;
    // dev->lcd_misc.lcd_cd_dummy_set = 0;
    // dev->lcd_misc.lcd_cd_data_set = 0;

    dev->lcd_user.val = 0;
    // dev->lcd_user.lcd_byte_order = false;
    // dev->lcd_user.lcd_bit_order = false;
    // dev->lcd_user.lcd_8bits_order = false;

    dev->lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG;

    _cache_flip = _cache[0];
  }

void uDisplay::pb_endTransaction(void) {
    auto dev = _dev;
    while (dev->lcd_user.val & LCD_CAM_LCD_START) {}
}

void uDisplay::pb_wait(void) {
    auto dev = _dev;
    while (dev->lcd_user.val & LCD_CAM_LCD_START) {}
}

bool uDisplay::pb_busy(void) {
    auto dev = _dev;
    return (dev->lcd_user.val & LCD_CAM_LCD_START);
}

bool uDisplay::pb_writeCommand(uint32_t data, uint_fast8_t bit_length) {
  auto dev = _dev;
  auto reg_lcd_user = &(dev->lcd_user.val);
  dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  if (interface == _UDSP_PAR8) {
      // 8bit bus
      auto bytes = bit_length >> 3;
      do {
        dev->lcd_cmd_val.lcd_cmd_value = data;
        data >>= 8;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      } while (--bytes);
      return true;
  } else {
      dev->lcd_cmd_val.val = data;
      WAIT_LCD_NOT_BUSY
      *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      return true;
  }
}

void uDisplay::pb_writeData(uint32_t data, uint_fast8_t bit_length) {
  auto dev = _dev;
  auto reg_lcd_user = &(dev->lcd_user.val);
  dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
  auto bytes = bit_length >> 3;

  if (interface == _UDSP_PAR8) {
    uint8_t shift = (bytes - 1) * 8;
    for (uint32_t cnt = 0; cnt < bytes; cnt++) {
      dev->lcd_cmd_val.lcd_cmd_value = (data >> shift) & 0xff;
      shift -= 8;
      WAIT_LCD_NOT_BUSY
      *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    }
    return;

  } else {
    if (bytes == 1 || bytes == 4) {
      uint8_t shift = (bytes - 1) * 8;
      for (uint32_t cnt = 0; cnt < bytes; cnt++) {
        dev->lcd_cmd_val.lcd_cmd_value = (data >> shift) & 0xff;
        shift -= 8;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      }
      return;
    }

    dev->lcd_cmd_val.val = data;
    WAIT_LCD_NOT_BUSY
    *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    return;
  }
}

void uDisplay::pb_pushPixels(uint16_t* data, uint32_t length, bool swap_bytes, bool use_dma) {
  auto dev = _dev;
  auto reg_lcd_user = &(dev->lcd_user.val);
  dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

  if (interface == _UDSP_PAR8) {
    if (swap_bytes) {
      for (uint32_t cnt = 0; cnt < length; cnt++) {
        dev->lcd_cmd_val.lcd_cmd_value = *data;
        while (*reg_lcd_user & LCD_CAM_LCD_START) {}
        *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        dev->lcd_cmd_val.lcd_cmd_value = *data >> 8;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        data++;
      }
    } else {
      for (uint32_t cnt = 0; cnt < length; cnt++) {
        dev->lcd_cmd_val.lcd_cmd_value = *data >> 8;
        while (*reg_lcd_user & LCD_CAM_LCD_START) {}
        *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        dev->lcd_cmd_val.lcd_cmd_value = *data;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        data++;
      }
    }
  } else {
    if (swap_bytes) {
      uint16_t iob;
      for (uint32_t cnt = 0; cnt < length; cnt++) {
        iob = *data++;
        iob = (iob << 8) | (iob >> 8);
        dev->lcd_cmd_val.lcd_cmd_value = iob;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      }
    } else {
      for (uint32_t cnt = 0; cnt < length; cnt++) {
        dev->lcd_cmd_val.lcd_cmd_value = *data++;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      }
    }
  }
}


void uDisplay::pb_writeBytes(const uint8_t* data, uint32_t length, bool use_dma) {

/*
    uint32_t freq = spi_speed * 1000000;
    uint32_t slow = (freq< 4000000) ? 2 : (freq < 8000000) ? 1 : 0;

    auto dev = _dev;
    do {
      auto reg_lcd_user = &(dev->lcd_user.val);
      dev->lcd_misc.lcd_cd_cmd_set  = 0;
      dev->lcd_cmd_val.lcd_cmd_value = data[0] | data[1] << 16;
      uint32_t cmd_val = data[2] | data[3] << 16;
      while (*reg_lcd_user & LCD_CAM_LCD_START) {}
      *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      if (use_dma) {
        if (slow) { ets_delay_us(slow); }
        _setup_dma_desc_links(&data[4], length - 4);
        gdma_start(_dma_chan, (intptr_t)(_dmadesc));
        length = 0;
      } else {
        size_t len = length;
        if (len > CACHE_SIZE) {
          len = (((len - 1) % CACHE_SIZE) + 4) & ~3u;
        }
        memcpy(_cache_flip, &data[4], (len-4+3)&~3);
        _setup_dma_desc_links((const uint8_t*)_cache_flip, len-4);
        gdma_start(_dma_chan, (intptr_t)(_dmadesc));
        length -= len;
        data += len;
        _cache_flip = _cache[(_cache_flip == _cache[0])];
      }
      dev->lcd_cmd_val.lcd_cmd_value = cmd_val;
      dev->lcd_misc.lcd_cd_data_set = 0;
      *reg_lcd_user = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;
      while (*reg_lcd_user & LCD_CAM_LCD_START) {}
      *reg_lcd_user = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
    } while (length);
*/
}


void uDisplay::_send_align_data(void) {
    _has_align_data = false;
    auto dev = _dev;
    dev->lcd_cmd_val.lcd_cmd_value = _align_data;
    auto reg_lcd_user = &(dev->lcd_user.val);
    while (*reg_lcd_user & LCD_CAM_LCD_START) {}
    *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}


void uDisplay::cs_control(bool level) {
    auto pin = par_cs;
    if (pin < 0) return;
    if (level) {
      gpio_hi(pin);
    }
    else {
      gpio_lo(pin);
    }
}

void uDisplay::_pb_init_pin(bool read) {
    if (read) {
      if (interface == _UDSP_PAR8) {
        for (size_t i = 0; i < 8; ++i) {
          gpio_ll_output_disable(&GPIO, (gpio_num_t)par_dbl[i]);
        }
      } else {
        for (size_t i = 0; i < 8; ++i) {
          gpio_ll_output_disable(&GPIO, (gpio_num_t)par_dbl[i]);
        }
        for (size_t i = 0; i < 8; ++i) {
          gpio_ll_output_disable(&GPIO, (gpio_num_t)par_dbh[i]);
        }
      }
    }
    else {
      auto idx_base = LCD_DATA_OUT0_IDX;
      if (interface == _UDSP_PAR8) {
        for (size_t i = 0; i < 8; ++i) {
          gpio_matrix_out(par_dbl[i], idx_base + i, 0, 0);
        }
      } else {
        for (size_t i = 0; i < 8; ++i) {
          gpio_matrix_out(par_dbl[i], idx_base + i, 0, 0);
        }
        for (size_t i = 0; i < 8; ++i) {
          gpio_matrix_out(par_dbh[i], idx_base + 8 + i, 0, 0);
        }
      }
    }
}

/* read analog value from pin for simple digitizer
X+ = d1
X- = CS
Y+ = RS
Y- = D0

define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

*/
uint32_t uDisplay::get_sr_touch(uint32_t _xp, uint32_t _xm, uint32_t _yp, uint32_t _ym) {
  uint32_t aval = 0;
  uint16_t xp,yp;
  if (pb_busy()) return 0;

  _pb_init_pin(true);
  gpio_matrix_out(par_rs, 0x100, 0, 0);

  pinMode(_ym, INPUT_PULLUP); // d0
  pinMode(_yp, INPUT_PULLUP); // rs

  pinMode(_xm, OUTPUT); // cs
  pinMode(_xp, OUTPUT); // d1
  digitalWrite(_xm, HIGH); // cs
  digitalWrite(_xp, LOW); // d1

  xp = 4096 - analogRead(_ym); // d0

  pinMode(_xm, INPUT_PULLUP); // cs
  pinMode(_xp, INPUT_PULLUP); // d1

  pinMode(_ym, OUTPUT); // d0
  pinMode(_yp, OUTPUT); // rs
  digitalWrite(_ym, HIGH); // d0
  digitalWrite(_yp, LOW); // rs

  yp = 4096 - analogRead(_xp); // d1

  aval = (xp << 16) | yp;

  pinMode(_yp, OUTPUT); // rs
  pinMode(_xm, OUTPUT); // cs
  pinMode(_ym, OUTPUT); // d0
  pinMode(_xp, OUTPUT); // d1
  digitalWrite(_yp, HIGH); // rs
  digitalWrite(_xm, HIGH); // cs

  _pb_init_pin(false);
  gpio_matrix_out(par_rs, LCD_DC_IDX, 0, 0);

  return aval;
}




#if 0
void TFT_eSPI::startWrite(void)
{
  begin_tft_write();
  lockTransaction = true; // Lock transaction for all sequentially run sketch functions
  inTransaction = true;
}

/***************************************************************************************
** Function name:           endWrite
** Description:             end transaction with CS high
***************************************************************************************/
void TFT_eSPI::endWrite(void)
{
  lockTransaction = false; // Release sketch induced transaction lock
  inTransaction = false;
  DMA_BUSY_CHECK;          // Safety check - user code should have checked this!
  end_tft_write();         // Release SPI bus
}
#endif


#endif // USE_ESP32_S3

#endif // ESP32
