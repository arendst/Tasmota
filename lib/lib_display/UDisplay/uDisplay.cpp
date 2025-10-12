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
#include "uDisplay_config.h"
#include "uDisplay_spi.h"

#ifdef ESP32
#include "esp8266toEsp32.h"
#endif
#include "tasmota_options.h"


//#define UDSP_DEBUG

#ifndef UDSP_LBSIZE
#define UDSP_LBSIZE 256
#endif

uDisplay::~uDisplay(void) {
#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: dealloc");
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
    //AddLog(LOG_LEVEL_DEBUG, ">> %s\n",lp1);
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
              AddLog(LOG_LEVEL_DEBUG, "UDisplay: SSPI_MOSI:%d SSPI_SCLK:%d SSPI_CS:%d DSP_RESET:%d", spi_mosi, spi_clk, spi_cs, reset);
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
              AddLog(LOG_LEVEL_DEBUG, "UDisplay: I2C_INIT bus:%d addr:%02x", wire_n, i2caddr);
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
                  AddLog(LOG_LEVEL_DEBUG, "UDisplay: reg=%02x val=%02x", dsp_cmds[0], dsp_cmds[1]);
#endif
                } else {
                  delay(dsp_cmds[0]);
#ifdef UDSP_DEBUG
                  AddLog(LOG_LEVEL_DEBUG, "UDisplay: delay=%d ms", dsp_cmds[0]);
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
#ifdef ESP32
            lvgl_param.use_dma = false; // temporary fix to disable DMA due to a problem in esp-idf 5.3
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
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: Device:%s xs:%d ys:%d bpp:%d", dname, gxs, gys, bpp);

  if (interface == _UDSP_SPI) {
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Nr:%d CS:%d CLK:%d MOSI:%d DC:%d TS_CS:%d TS_RST:%d TS_IRQ:%d", 
       spi_nr, spi_cs, spi_clk, spi_mosi, spi_dc, ut_spi_cs, ut_reset, ut_irq);
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: BPAN:%d RES:%d MISO:%d SPED:%d Pixels:%d SaMode:%d DMA-Mode:%d opts:%02x,%02x,%02x SetAddr:%x,%x,%x", 
       bpanel, reset, spi_miso, spi_speed*1000000, col_mode, sa_mode, lvgl_param.use_dma, saw_3, dim_op, startline, saw_1, saw_2, saw_3);
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Rot 0: %x,%x - %d - %d", madctrl, rot[0], x_addr_offs[0], y_addr_offs[0]);

    if (ep_mode == 1) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: LUT_Partial:%d-%d-%x-%d-%d LUT_Full:%d-%d-%x-%d-%d", 
       lut_siz_partial, lutpsize, lut_cmd[0], epcoffs_part, epc_part_cnt, 
       lut_siz_full, lutfsize, lut_cmd[0], epcoffs_full, epc_full_cnt);
    }
    if (ep_mode == 2) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: LUT_SIZE 1:%d 2:%d 3:%d 4:%d 5:%d", 
       lut_cnt[0], lut_cnt[1], lut_cnt[2], lut_cnt[3], lut_cnt[4]);
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: LUT_CMDS %02x-%02x-%02x-%02x-%02x", 
       lut_cmd[0], lut_cmd[1], lut_cmd[2], lut_cmd[3], lut_cmd[4]);
    }
  }
  if (interface == _UDSP_I2C) {
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Addr:%02x SCL:%d SDA:%d", i2caddr, i2c_scl, i2c_sda);

    AddLog(LOG_LEVEL_DEBUG, "UDisplay: SPA:%x pa_sta:%x pa_end:%x SCA:%x ca_sta:%x ca_end:%x WRA:%x", 
       saw_1, i2c_page_start, i2c_page_end, saw_2, i2c_col_start, i2c_col_end, saw_3);
  }

  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
#ifdef USE_ESP32_S3
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: par mode:%d res:%d cs:%d rs:%d wr:%d rd:%d bp:%d", 
       interface, reset, par_cs, par_rs, par_wr, par_rd, bpanel);

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: par d%d:%d", cnt, par_dbl[cnt]);
    }

    if (interface == _UDSP_PAR16) {
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        AddLog(LOG_LEVEL_DEBUG, "UDisplay: par d%d:%d", cnt + 8, par_dbh[cnt]);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: par freq:%d", spi_speed);
#endif // USE_ESP32_S3

  }
  if (interface == _UDSP_RGB) {
#ifdef USE_ESP32_S3

    AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb de:%d vsync:%d hsync:%d pclk:%d bp:%d", de, vsync, hsync, pclk, bpanel);

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb d%d:%d", cnt, par_dbl[cnt]);
    }
    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb d%d:%d", cnt + 8, par_dbh[cnt]);
    }

    AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb freq:%d hsync_pol:%d hsync_fp:%d hsync_pw:%d hsync_bp:%d vsync_pol:%d vsync_fp:%d vsync_pw:%d vsync_bp:%d pclk_neg:%d",
       spi_speed, hsync_polarity, hsync_front_porch, hsync_pulse_width, hsync_back_porch,
       vsync_polarity, vsync_front_porch, vsync_pulse_width, vsync_back_porch, pclk_active_neg);

#endif // USE_ESP32_S3
  }
#endif

#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: Dsp class init complete");
#endif
}

// special init for GC displays
void uDisplay::send_spi_icmds(uint16_t cmd_size) {
uint16_t index = 0;
uint16_t cmd_offset = 0;


#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: start send icmd table");
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
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: cmd, args %02x, %d", iob, args & 0x7f);
#endif
    for (uint32_t cnt = 0; cnt < (args & 0x7f); cnt++) {
      iob = dsp_cmds[cmd_offset++];
      index++;
#ifdef UDSP_DEBUG
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: %02x", iob);
#endif
      ulcd_data8(iob);
    }
    SPI_CS_HIGH
    if (args & 0x80) {  // delay after the command
      delay_arg(args);
    }
    if (index >= cmd_size) break;
  }
#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: end send icmd table");
#endif
  return;
}


void uDisplay::send_spi_cmds(uint16_t cmd_offset, uint16_t cmd_size) {
uint16_t index = 0;
#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: start send cmd table");
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
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: cmd, args %02x, %d", iob, args & 0x1f);
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
        AddLog(LOG_LEVEL_DEBUG, "UDisplay: %02x", iob);
      }
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
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: cmd, args %02x, %d", iob, args & 0x1f);
#endif
      for (uint32_t cnt = 0; cnt < (args & 0x1f); cnt++) {
        iob = dsp_cmds[cmd_offset++];
        index++;
#ifdef UDSP_DEBUG
        AddLog(LOG_LEVEL_DEBUG, "%02x ", iob );
#endif
        if (!allcmd_mode) {
          ulcd_data8(iob);
        } else {
          ulcd_command(iob);
        }
      }
      SPI_CS_HIGH
      if (args & 0x80) {  // delay after the command
        delay_arg(args);
      }
    }
    if (index >= cmd_size) break;
  }

exit:
#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: end send cmd table");
#endif
  return;
}

Renderer *uDisplay::Init(void) {
  if (!interface) {   // no valid configuration, abort
    #ifdef UDSP_DEBUG
    AddLog(LOG_LEVEL_INFO, "UDisplay: Dsp Init no valid configuration");
    #endif
    return NULL;
  }

  #ifdef UDSP_DEBUG
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Dsp Init 1 start");
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
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: I2C cmds:%d", dsp_ncmds);
#endif
    for (uint32_t cnt = 0; cnt < dsp_ncmds; cnt++) {
      i2c_command(dsp_cmds[cnt]);
#ifdef UDSP_DEBUG
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: cmd=%x", dsp_cmds[cnt]);
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
        AddLog(LOG_LEVEL_DEBUG, "UDisplay: Dsp busy pin:%d", busy_pin);
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
      AddLog(LOG_LEVEL_INFO, "UDisplay: Dsp RGB requires PSRAM, abort");
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

    // assume that byte swapping of 16-bit color is done only upon request
    // via display.ini and not by callers of pushColor()
    // -> swap bytes by swapping GPIO numbers
    int8_t *par_db8 = lvgl_param.swap_color ? par_dbl : par_dbh;
    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      _panel_config->data_gpio_nums[cnt] = par_db8[cnt];
    }
    par_db8 = lvgl_param.swap_color ? par_dbh : par_dbl;
    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      _panel_config->data_gpio_nums[cnt + 8] = par_db8[cnt];
    }
    lvgl_param.swap_color = 0;

    _panel_config->disp_gpio_num = GPIO_NUM_NC;

    _panel_config->flags.disp_active_low = 0;
    _panel_config->flags.refresh_on_demand = 0;
    _panel_config->flags.fb_in_psram = 1;             // allocate frame buffer in PSRAM

    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(_panel_config, &_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(_panel_handle));

    uint16_t color = random(0xffff);
    ESP_ERROR_CHECK(_panel_handle->draw_bitmap(_panel_handle, 0, 0, 1, 1, &color));

    void * buf = NULL;
    esp_lcd_rgb_panel_get_frame_buffer(_panel_handle, 1, &buf);
    rgb_fb = (uint16_t *)buf;

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
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: cmd, args %02x, %d", iob, args & 0x1f);
    #endif
      for (uint32_t cnt = 0; cnt < (args & 0x1f); cnt++) {
        iob = dsp_cmds[index++];
    #ifdef UDSP_DEBUG
        AddLog(LOG_LEVEL_DEBUG, "UDisplay: %02x", iob);
    #endif
        pb_writeData(iob, 8);
      }
      cs_control(1);
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
          AddLog(LOG_LEVEL_DEBUG, "UDisplay: delay %d ms", delay_ms);
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
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: Dsp Init 1 complete");
#endif
  return this;
}

void uDisplay::DisplayInit(int8_t p, int8_t size, int8_t rot, int8_t font) {
  if (p != DISPLAY_INIT_MODE && ep_mode) {
    ep_update_mode = p;
    if (p == DISPLAY_INIT_PARTIAL) {
      if (lutpsize) {
#ifdef UDSP_DEBUG
        AddLog(LOG_LEVEL_DEBUG, "init partial epaper mode");
#endif
        SetLut(lut_partial);
        Updateframe_EPD();
        delay_sync(lutptime * 10);
      }
      return;
    } else if (p == DISPLAY_INIT_FULL) {
#ifdef UDSP_DEBUG
      AddLog(LOG_LEVEL_DEBUG, "init full epaper mode");
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
    AddLog(LOG_LEVEL_DEBUG, "Dsp Init 2 complete");
#endif
  }
}

void uDisplay::i2c_command(uint8_t val) {
  //AddLog(LOG_LEVEL_DEBUG, "%02x\n",val );
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
    // AddLog(LOG_LEVEL_DEBUG, "m_row=%d m_col=%d xs=%d ys=%d\n", m_row, m_col, xs, ys);

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
