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

  // Free panel config union
  if (panel_config) {
    free(panel_config);
    panel_config = nullptr;
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
  bp_mode.data = 0;
  dsp_off = 0xff;
  dsp_on = 0xff;
  // busy_pin = -1;  // MOVED to EPDPanelConfig.busy_pin
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
  lut_num = 0;
  lvgl_param.data = 0;
  lvgl_param.flushlines = 40;
  rot_t[0] = 0;
  rot_t[1] = 1;
  rot_t[2] = 2;
  rot_t[3] = 3;
  interface = 0;
  
  // Allocate panel_config once at the beginning
  panel_config = (PanelConfigUnion*)calloc(1, sizeof(PanelConfigUnion));
  // Set EPD timing defaults (will be used if EPD mode is detected)
  panel_config->epd.lut_full_time = 350;
  panel_config->epd.lut_partial_time = 35;
  panel_config->epd.update_time = 10;
  
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
            // special case RGB with software SPI init clk,mosi,cs,reset
            lp1++;
            if (interface == _UDSP_RGB) { 
                lp1++;
                SPIControllerConfig spi_cfg = {
                    .bus_nr = 4,
                    .cs = -1,
                    .clk = (int8_t)next_val(&lp1),
                    .mosi = (int8_t)next_val(&lp1),
                    .dc = -1,
                    .miso = -1,
                    .speed = spi_speed
                };
                spi_cfg.cs = (int8_t)next_val(&lp1);
                spec_init = _UDSP_SPI;
                reset = next_val(&lp1);

                spiController = new SPIController(spi_cfg);
              // spiSettings = spiController->getSPISettings();
              // busy_pin = spi_cfg.miso; // update for timing

              if (reset >= 0) {
                pinMode(reset, OUTPUT);
                digitalWrite(reset, HIGH);
                delay(50);
                reset_pin(50, 200);
              }
#ifdef UDSP_DEBUG
              AddLog(LOG_LEVEL_DEBUG, "UDisplay: SSPI_MOSI:%d SSPI_SCLK:%d SSPI_CS:%d DSP_RESET:%d", spiController->spi_config.mosi, spiController->spi_config.clk, spiController->spi_config.dc, reset);
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
            uint8_t lut_size = next_val(&lp1);
            uint8_t lut_cmd_val = next_hex(&lp1);
            
            // Store directly in EPD config
            panel_config->epd.lut_siz[lut_num - 1] = lut_size;
            panel_config->epd.lut_array_data[lut_num - 1] = (uint8_t*)malloc(lut_size);
            panel_config->epd.lut_cmd[lut_num - 1] = lut_cmd_val;
          } else {
            lut_num = 0;
            lp1++;
            uint16_t lut_size = next_val(&lp1);
            uint8_t lut_cmd_val = next_hex(&lp1);
            
            // Store directly in EPD config
            panel_config->epd.lut_full_data = (uint8_t*)malloc(lut_size);
            panel_config->epd.lutfsize = 0;  // Will be filled during :L data parsing
            panel_config->epd.lut_cmd[0] = lut_cmd_val;
          }
        } else if (section == 'l') {
          lp1++;
          uint16_t lut_size = next_val(&lp1);
          uint8_t lut_cmd_val = next_hex(&lp1);
          
          // Store directly in EPD config
          panel_config->epd.lut_partial_data = (uint8_t*)malloc(lut_size);
          panel_config->epd.lutpsize = 0;  // Will be filled during :l data parsing
          panel_config->epd.lut_cmd[0] = lut_cmd_val;
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
              SPIControllerConfig spi_cfg = {
                  .bus_nr = (uint8_t)next_val(&lp1),
                  .cs = (int8_t)next_val(&lp1),
                  .clk = (int8_t)next_val(&lp1),
                  .mosi = (int8_t)next_val(&lp1),
                  .dc = (int8_t)next_val(&lp1)
              };
              bpanel = next_val(&lp1);
              reset = next_val(&lp1);
              spi_cfg.miso = (int8_t)next_val(&lp1);
              spi_cfg.speed = next_val(&lp1);
              spiController = new SPIController(spi_cfg);
              section = 0;
            } else if (!strncmp(ibuff, "PAR", 3)) {
#if defined(UDISPLAY_I80)
              uint8_t bus = next_val(&lp1);
              if (bus == 8) {
                interface = _UDSP_PAR8;
              } else {
                interface = _UDSP_PAR16;
              }
              reset = next_val(&lp1);
              
              // Parse control pins directly into I80 config
              panel_config->i80.cs_pin = next_val(&lp1);
              panel_config->i80.dc_pin = next_val(&lp1);
              panel_config->i80.wr_pin = next_val(&lp1);
              panel_config->i80.rd_pin = next_val(&lp1);
              bpanel = next_val(&lp1);

              // Parse data pins directly into I80 config
              for (uint32_t cnt = 0; cnt < 8; cnt ++) {
                panel_config->i80.data_pins_low[cnt] = next_val(&lp1);
              }

              if (interface == _UDSP_PAR16) {
                for (uint32_t cnt = 0; cnt < 8; cnt ++) {
                  panel_config->i80.data_pins_high[cnt] = next_val(&lp1);
                }
              }
              spi_speed = next_val(&lp1);
#endif // UDISPLAY_I80
              section = 0;
            }  else if (!strncmp(ibuff, "RGB", 3)) {
#ifdef SOC_LCD_RGB_SUPPORTED
              interface = _UDSP_RGB;
              // RGB needs DMA-capable memory - reallocate panel_config
              free(panel_config);
              panel_config = (PanelConfigUnion*)heap_caps_calloc(1, sizeof(PanelConfigUnion), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
              // Parse pin configuration directly into union
              panel_config->rgb.de_gpio_num = (gpio_num_t)next_val(&lp1);
              panel_config->rgb.vsync_gpio_num = (gpio_num_t)next_val(&lp1);
              panel_config->rgb.hsync_gpio_num = (gpio_num_t)next_val(&lp1);
              panel_config->rgb.pclk_gpio_num = (gpio_num_t)next_val(&lp1);
              bpanel = next_val(&lp1);
              // Parse data pins directly into RGB config
              // Note: byte order may be swapped later based on lvgl_param.swap_color
              for (uint32_t cnt = 0; cnt < 8; cnt++) {
                  panel_config->rgb.data_gpio_nums[cnt + 8] = next_val(&lp1);
              }
              for (uint32_t cnt = 0; cnt < 8; cnt++) {
                  panel_config->rgb.data_gpio_nums[cnt] = next_val(&lp1);
              }
              spi_speed = next_val(&lp1);
#endif //SOC_LCD_RGB_SUPPORTED
            } else if (!strncmp(ibuff, "DSI", 3)) {
#ifdef SOC_MIPI_DSI_SUPPORTED
              interface = _UDSP_DSI;
              // Parse DSI-specific parameters directly into union
              panel_config->dsi.dsi_lanes = next_val(&lp1);
              panel_config->dsi.te_pin = next_val(&lp1);
              bpanel = next_val(&lp1);
              panel_config->dsi.reset_pin = next_val(&lp1);
              panel_config->dsi.ldo_channel = next_val(&lp1);
              panel_config->dsi.ldo_voltage_mv = next_val(&lp1);
              panel_config->dsi.pixel_clock_hz = next_val(&lp1);
              panel_config->dsi.lane_speed_mbps = next_val(&lp1);
              panel_config->dsi.rgb_order = next_val(&lp1);
              panel_config->dsi.data_endian = next_val(&lp1);
              
              // Set display dimensions
              panel_config->dsi.width = gxs;
              panel_config->dsi.height = gys;
              panel_config->dsi.bpp = bpp;
              
              section = 0;
#ifdef UDSP_DEBUG
              AddLog(LOG_LEVEL_DEBUG, "UDisplay: DSI interface - Lanes:%d TE:%d BL:%d LDO:%d@%dmV Clock:%dHz Speed:%dMbps RGB_Order:%d Endian:%d",
                    panel_config->dsi.dsi_lanes, panel_config->dsi.te_pin, bpanel,
                    panel_config->dsi.ldo_channel, panel_config->dsi.ldo_voltage_mv,
                    panel_config->dsi.pixel_clock_hz, panel_config->dsi.lane_speed_mbps,
                    panel_config->dsi.rgb_order, panel_config->dsi.data_endian);
#endif
#endif //SOC_MIPI_DSI_SUPPORTED
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
            } else if (interface == _UDSP_DSI) {
              // DSI - parse current line and accumulate bytes
              // Don't reset dsp_ncmds - accumulate across all :I lines
              uint16_t line_bytes = 0;
              while (1) {
                if (dsp_ncmds >= sizeof(dsp_cmds)) {
                  AddLog(LOG_LEVEL_ERROR, "DSI: Init command buffer full at %d bytes", dsp_ncmds);
                  break;
                }
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  dsp_cmds[dsp_ncmds++] = strtol(ibuff, 0, 16);
                  line_bytes++;
                } else {
                  break;
                }
              }
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
            if (!panel_config->epd.epcoffs_full) {
              panel_config->epd.epcoffs_full = dsp_ncmds;
              panel_config->epd.epc_full_cnt = 0;
            }
            while (1) {
              if (panel_config->epd.epc_full_cnt >= sizeof(dsp_cmds)) break;
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                dsp_cmds[panel_config->epd.epcoffs_full + panel_config->epd.epc_full_cnt++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
            }
            break;
          case 'p':
            // epaper partial update cmds
            if (!panel_config->epd.epcoffs_part) {
              panel_config->epd.epcoffs_part = dsp_ncmds + panel_config->epd.epc_full_cnt;
              panel_config->epd.epc_part_cnt = 0;
            }
            while (1) {
              if (panel_config->epd.epc_part_cnt >= sizeof(dsp_cmds)) break;
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                dsp_cmds[panel_config->epd.epcoffs_part + panel_config->epd.epc_part_cnt++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
            }
            break;

          case 'V':
#if SOC_LCD_RGB_SUPPORTED
          if (interface == _UDSP_RGB){
            // Parse timing directly into union
            panel_config->rgb.timings.flags.hsync_idle_low = (next_val(&lp1) == 0) ? 1 : 0;
            panel_config->rgb.timings.hsync_front_porch = next_val(&lp1);
            panel_config->rgb.timings.hsync_pulse_width = next_val(&lp1);
            panel_config->rgb.timings.hsync_back_porch = next_val(&lp1);
            panel_config->rgb.timings.flags.vsync_idle_low = (next_val(&lp1) == 0) ? 1 : 0;
            panel_config->rgb.timings.vsync_front_porch = next_val(&lp1);
            panel_config->rgb.timings.vsync_pulse_width = next_val(&lp1);
            panel_config->rgb.timings.vsync_back_porch = next_val(&lp1);
            panel_config->rgb.timings.flags.pclk_active_neg = next_val(&lp1);
            // Set fixed flags (not in descriptor)
            panel_config->rgb.timings.flags.de_idle_high = 0;
            panel_config->rgb.timings.flags.pclk_idle_high = 0;
          }
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
          if (interface == _UDSP_DSI && panel_config->dsi.timing.h_front_porch == 0) {
            AddLog(1, "DSI: Parsing :V timing line");
            panel_config->dsi.timing.h_front_porch = next_val(&lp1);
            panel_config->dsi.timing.v_front_porch = next_val(&lp1);
            panel_config->dsi.timing.h_back_porch = next_val(&lp1);
            panel_config->dsi.timing.h_sync_pulse = next_val(&lp1);
            panel_config->dsi.timing.v_sync_pulse = next_val(&lp1);
            panel_config->dsi.timing.v_back_porch = next_val(&lp1);
            AddLog(1, "DSI: Parsed timing - HFP:%d VFP:%d HBP:%d HSW:%d VSW:%d VBP:%d",
                  panel_config->dsi.timing.h_front_porch, panel_config->dsi.timing.v_front_porch,
                  panel_config->dsi.timing.h_back_porch, panel_config->dsi.timing.h_sync_pulse,
                  panel_config->dsi.timing.v_sync_pulse, panel_config->dsi.timing.v_back_porch);
          }
#endif //SOC_MIPI_DSI_SUPPORTED
            break;
          case 'o':
            dsp_off = next_hex(&lp1);
            break;

          case 'O':
            dsp_on = next_hex(&lp1);
            break;
          case 'R':
            // Parse directly into SPI config (used by SPI panels only)
            if (interface == _UDSP_SPI) {
              panel_config->spi.cmd_memory_access = next_hex(&lp1);
              panel_config->spi.cmd_startline = next_hex(&lp1);
            } 
#ifdef UDISPLAY_I80
            else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
              panel_config->i80.cmd_madctl = next_hex(&lp1);
              panel_config->i80.cmd_startline = next_hex(&lp1);
            }
#endif
            else {
              madctrl = next_hex(&lp1);
              startline = next_hex(&lp1);
            }
            break;
          case '0':
            if (interface != _UDSP_RGB) {
              if (interface == _UDSP_SPI) {
                // Parse directly into SPI config
                panel_config->spi.rot_cmd[0] = next_hex(&lp1);
                panel_config->spi.x_addr_offset[0] = next_hex(&lp1);
                panel_config->spi.y_addr_offset[0] = next_hex(&lp1);
              } 
#ifdef UDISPLAY_I80
              else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
                // Parse directly into I80 config
                panel_config->i80.rot_cmd[0] = next_hex(&lp1);
                panel_config->i80.x_addr_offset[0] = next_hex(&lp1);
                panel_config->i80.y_addr_offset[0] = next_hex(&lp1);
              }
#endif
              else {
                rot[0] = next_hex(&lp1);
                x_addr_offs[0] = next_hex(&lp1);
                y_addr_offs[0] = next_hex(&lp1);
              }
            }
            rot_t[0] = next_hex(&lp1);
            break;
          case '1':
            if (interface != _UDSP_RGB) {
              if (interface == _UDSP_SPI) {
                panel_config->spi.rot_cmd[1] = next_hex(&lp1);
                panel_config->spi.x_addr_offset[1] = next_hex(&lp1);
                panel_config->spi.y_addr_offset[1] = next_hex(&lp1);
              } 
#ifdef UDISPLAY_I80
              else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
                // Parse directly into I80 config
                panel_config->i80.rot_cmd[1] = next_hex(&lp1);
                panel_config->i80.x_addr_offset[1] = next_hex(&lp1);
                panel_config->i80.y_addr_offset[1] = next_hex(&lp1);
              }
#endif
              else {
                rot[1] = next_hex(&lp1);
                x_addr_offs[1] = next_hex(&lp1);
                y_addr_offs[1] = next_hex(&lp1);
              }
            }
            rot_t[1] = next_hex(&lp1);
            break;
          case '2':
            if (interface != _UDSP_RGB) {
              if (interface == _UDSP_SPI) {
                panel_config->spi.rot_cmd[2] = next_hex(&lp1);
                panel_config->spi.x_addr_offset[2] = next_hex(&lp1);
                panel_config->spi.y_addr_offset[2] = next_hex(&lp1);
              } 
#ifdef UDISPLAY_I80
              else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
                // Parse directly into I80 config
                panel_config->i80.rot_cmd[2] = next_hex(&lp1);
                panel_config->i80.x_addr_offset[2] = next_hex(&lp1);
                panel_config->i80.y_addr_offset[2] = next_hex(&lp1);
              }
#endif
              else {
                rot[2] = next_hex(&lp1);
                x_addr_offs[2] = next_hex(&lp1);
                y_addr_offs[2] = next_hex(&lp1);
              }
            }
            rot_t[2] = next_hex(&lp1);
            break;
          case '3':
            if (interface != _UDSP_RGB) {
              if (interface == _UDSP_SPI) {
                panel_config->spi.rot_cmd[3] = next_hex(&lp1);
                panel_config->spi.x_addr_offset[3] = next_hex(&lp1);
                panel_config->spi.y_addr_offset[3] = next_hex(&lp1);
              } 
#ifdef UDISPLAY_I80
              else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
                // Parse directly into I80 config
                panel_config->i80.rot_cmd[3] = next_hex(&lp1);
                panel_config->i80.x_addr_offset[3] = next_hex(&lp1);
                panel_config->i80.y_addr_offset[3] = next_hex(&lp1);
              }
#endif
              else {
                rot[3] = next_hex(&lp1);
                x_addr_offs[3] = next_hex(&lp1);
                y_addr_offs[3] = next_hex(&lp1);
              }
            }
            rot_t[3] = next_hex(&lp1);
            break;
          case 'A':
            if (interface == _UDSP_I2C || bpp == 1) {
              // Parse directly into I2C config
              panel_config->i2c.cmd_set_addr_x = next_hex(&lp1);
              panel_config->i2c.page_start = next_hex(&lp1);
              panel_config->i2c.page_end = next_hex(&lp1);
              panel_config->i2c.cmd_set_addr_y = next_hex(&lp1);
              panel_config->i2c.col_start = next_hex(&lp1);
              panel_config->i2c.col_end = next_hex(&lp1);
              panel_config->i2c.cmd_write_ram = next_hex(&lp1);
              // Also keep in legacy vars for now
              saw_1 = panel_config->i2c.cmd_set_addr_x;
              i2c_page_start = panel_config->i2c.page_start;
              i2c_page_end = panel_config->i2c.page_end;
              saw_2 = panel_config->i2c.cmd_set_addr_y;
              i2c_col_start = panel_config->i2c.col_start;
              i2c_col_end = panel_config->i2c.col_end;
              saw_3 = panel_config->i2c.cmd_write_ram;
            } else if (interface == _UDSP_SPI) {
              // Parse directly into SPI config
              panel_config->spi.cmd_set_addr_x = next_hex(&lp1);
              panel_config->spi.cmd_set_addr_y = next_hex(&lp1);
              panel_config->spi.cmd_write_ram = next_hex(&lp1);
              panel_config->spi.address_mode = next_val(&lp1);
            }
#ifdef UDISPLAY_I80
            else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
              // Parse directly into I80 config
              panel_config->i80.cmd_set_addr_x = next_hex(&lp1);
              panel_config->i80.cmd_set_addr_y = next_hex(&lp1);
              panel_config->i80.cmd_write_ram = next_hex(&lp1);
              panel_config->i80.sa_mode = next_val(&lp1);
            }
#endif
            else {
              saw_1 = next_hex(&lp1);
              saw_2 = next_hex(&lp1);
              saw_3 = next_hex(&lp1);
              sa_mode = next_val(&lp1);
            }
            break;
          case 'a':
            if (interface == _UDSP_SPI) {
              // Parse directly into SPI config
              panel_config->spi.cmd_set_addr_x = next_hex(&lp1);
              panel_config->spi.cmd_set_addr_y = next_hex(&lp1);
              panel_config->spi.cmd_write_ram = next_hex(&lp1);
            } else {
              saw_1 = next_hex(&lp1);
              saw_2 = next_hex(&lp1);
              saw_3 = next_hex(&lp1);
            }
            break;
          case 'P':
            col_mode = next_val(&lp1);  // Keep for legacy code
            if (interface == _UDSP_SPI) {
              panel_config->spi.col_mode = col_mode;
            }
#ifdef UDISPLAY_I80
            else if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
              panel_config->i80.color_mode = col_mode;
            }
#endif
            break;
          case 'i':
            inv_off = next_hex(&lp1);
            inv_on = next_hex(&lp1);
            if (interface == _UDSP_SPI) {
              panel_config->spi.cmd_invert_off = inv_off;
              panel_config->spi.cmd_invert_on = inv_on;
            } else if (interface == _UDSP_I2C) {
              panel_config->i2c.cmd_invert_off = inv_off;
              panel_config->i2c.cmd_invert_on = inv_on;
            }
            break;
          case 'D':
            dim_op = next_hex(&lp1);
            break;
          case 'L':
            if (!lut_num) {
              if (!panel_config->epd.lut_full_data) {
                break;
              }
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  panel_config->epd.lut_full_data[panel_config->epd.lutfsize++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
              }
              // Set pointers for compatibility
              panel_config->epd.lut_full = panel_config->epd.lut_full_data;
              panel_config->epd.lut_full_len = panel_config->epd.lutfsize;
            } else {
              uint8_t index = lut_num - 1;
              if (!panel_config->epd.lut_array_data[index]) {
                break;
              }
              while (1) {
                if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                  panel_config->epd.lut_array_data[index][panel_config->epd.lut_cnt_data[index]++] = strtol(ibuff, 0, 16);
                } else {
                  break;
                }
                if (panel_config->epd.lut_cnt_data[index] >= panel_config->epd.lut_siz[index]) break;
              }
              // Set pointers for compatibility
              panel_config->epd.lut_array = (const uint8_t**)panel_config->epd.lut_array_data;
              panel_config->epd.lut_cnt = panel_config->epd.lut_cnt_data;
            }
            break;
          case 'l':
            if (!panel_config->epd.lut_partial_data) {
              break;
            }
            while (1) {
              if (!str2c(&lp1, ibuff, sizeof(ibuff))) {
                panel_config->epd.lut_partial_data[panel_config->epd.lutpsize++] = strtol(ibuff, 0, 16);
              } else {
                break;
              }
            }
            // Set pointers for compatibility
            panel_config->epd.lut_partial = panel_config->epd.lut_partial_data;
            panel_config->epd.lut_partial_len = panel_config->epd.lutpsize;
            break;
          case 'T':
            // Parse timing directly into EPD config
            panel_config->epd.lut_full_time = next_val(&lp1);
            panel_config->epd.lut_partial_time = next_val(&lp1);
            panel_config->epd.update_time = next_val(&lp1);
            break;
          case 'B':
            lvgl_param.flushlines = next_val(&lp1);
            lvgl_param.data = next_val(&lp1);
#ifdef ESP32
            // if(interface != _UDSP_SPI) // maybe test this later
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
            bp_mode.data = next_hex(&lp1);
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

  // EPD mode detection - only for SPI interface
  if (interface == _UDSP_SPI) {
    if (panel_config && panel_config->epd.lutfsize && panel_config->epd.lutpsize) {
      // 2 table mode
      ep_mode = 1;
    }

    if (panel_config && panel_config->epd.lut_cnt_data[0] > 0 && 
        panel_config->epd.lut_cnt_data[1] == panel_config->epd.lut_cnt_data[2] && 
        panel_config->epd.lut_cnt_data[1] == panel_config->epd.lut_cnt_data[3] && 
        panel_config->epd.lut_cnt_data[1] == panel_config->epd.lut_cnt_data[4]) {
      // 5 table mode
      ep_mode = 2;
    }

    if (panel_config && (panel_config->epd.epcoffs_full || panel_config->epd.epcoffs_part) && 
        !(panel_config->epd.lutfsize || panel_config->epd.lutpsize)) {
      // no lutfsize or lutpsize, but epcoffs_full or epcoffs_part
      ep_mode = 3;
    }
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

#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: Device:%s xs:%d ys:%d bpp:%d", dname, gxs, gys, bpp);

  if (interface == _UDSP_SPI) {
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Nr:%d CS:%d CLK:%d MOSI:%d DC:%d TS_CS:%d TS_RST:%d TS_IRQ:%d", 
       spiController->spi_config.bus_nr, spiController->spi_config.cs, spiController->spi_config.clk, spiController->spi_config.mosi, spiController->spi_config.dc, ut_spi_cs, ut_reset, ut_irq);
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: BPAN:%d RES:%d MISO:%d SPED:%d Pixels:%d SaMode:%d DMA-Mode:%d opts:%02x,%02x,%02x SetAddr:%x,%x,%x", 
       bpanel, reset, spiController->spi_config.miso, spiController->spi_config.speed*1000000, col_mode, sa_mode, lvgl_param.use_dma, saw_3, dim_op, startline, saw_1, saw_2, saw_3);
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Rot 0: %x,%x - %d - %d", madctrl, rot[0], x_addr_offs[0], y_addr_offs[0]);

    if (ep_mode == 1 && panel_config) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: LUT_Partial:%d-%x-%d-%d LUT_Full:%d-%x-%d-%d", 
       panel_config->epd.lutpsize, panel_config->epd.lut_cmd[0], panel_config->epd.epcoffs_part, panel_config->epd.epc_part_cnt, 
       panel_config->epd.lutfsize, panel_config->epd.lut_cmd[0], panel_config->epd.epcoffs_full, panel_config->epd.epc_full_cnt);
    }
    if (ep_mode == 2 && panel_config) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: LUT_SIZE 1:%d 2:%d 3:%d 4:%d 5:%d", 
       panel_config->epd.lut_cnt_data[0], panel_config->epd.lut_cnt_data[1], panel_config->epd.lut_cnt_data[2], 
       panel_config->epd.lut_cnt_data[3], panel_config->epd.lut_cnt_data[4]);
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: LUT_CMDS %02x-%02x-%02x-%02x-%02x", 
       panel_config->epd.lut_cmd[0], panel_config->epd.lut_cmd[1], panel_config->epd.lut_cmd[2], 
       panel_config->epd.lut_cmd[3], panel_config->epd.lut_cmd[4]);
    }
  }
  if (interface == _UDSP_I2C) {
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: Addr:%02x SCL:%d SDA:%d", i2caddr, i2c_scl, i2c_sda);

    AddLog(LOG_LEVEL_DEBUG, "UDisplay: SPA:%x pa_sta:%x pa_end:%x SCA:%x ca_sta:%x ca_end:%x WRA:%x", 
       saw_1, i2c_page_start, i2c_page_end, saw_2, i2c_col_start, i2c_col_end, saw_3);
  }

  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
#if defined(UDISPLAY_I80)
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: par mode:%d res:%d cs:%d rs:%d wr:%d rd:%d bp:%d", 
       interface, reset, panel_config->i80.cs_pin, panel_config->i80.dc_pin, 
       panel_config->i80.wr_pin, panel_config->i80.rd_pin, bpanel);

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: par d%d:%d", cnt, panel_config->i80.data_pins_low[cnt]);
    }

    if (interface == _UDSP_PAR16) {
      for (uint32_t cnt = 0; cnt < 8; cnt ++) {
        AddLog(LOG_LEVEL_DEBUG, "UDisplay: par d%d:%d", cnt + 8, panel_config->i80.data_pins_high[cnt]);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, "UDisplay: par freq:%d", spi_speed);
#endif // UDISPLAY_I80

  }
  if (interface == _UDSP_RGB) {
#ifdef SOC_LCD_RGB_SUPPORTED

    AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb de:%d vsync:%d hsync:%d pclk:%d bp:%d", panel_config->rgb.de_gpio_num, panel_config->rgb.vsync_gpio_num, panel_config->rgb.hsync_gpio_num, panel_config->rgb.pclk_gpio_num, bpanel);

    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb d%d:%d", cnt, panel_config->rgb.data_gpio_nums[cnt]);
    }
    for (uint32_t cnt = 0; cnt < 8; cnt ++) {
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb d%d:%d", cnt + 8, panel_config->rgb.data_gpio_nums[cnt + 8]);
    }

      AddLog(LOG_LEVEL_DEBUG, "UDisplay: rgb freq:%d hsync_idle_low:%d hsync_fp:%d hsync_pw:%d hsync_bp:%d vsync_idle_low:%d vsync_fp:%d vsync_pw:%d vsync_bp:%d pclk_neg:%d",
        spiController->spi_config.speed, panel_config->rgb.timings.flags.hsync_idle_low, panel_config->rgb.timings.hsync_front_porch, panel_config->rgb.timings.hsync_pulse_width, 
        panel_config->rgb.timings.hsync_back_porch, panel_config->rgb.timings.flags.vsync_idle_low,  panel_config->rgb.timings.vsync_front_porch, 
        panel_config->rgb.timings.vsync_pulse_width, panel_config->rgb.timings.vsync_back_porch, panel_config->rgb.timings.flags.pclk_active_neg);

#endif // SOC_LCD_RGB_SUPPORTED
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
    spiController->csLow();
    iob = dsp_cmds[cmd_offset++];
    index++;
    spiController->writeCommand(iob);
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
      spiController->writeData8(iob);
    }
    spiController->csHigh();
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
    spiController->csLow();
    iob = dsp_cmds[cmd_offset++];
    index++;
    if ((ep_mode == 1 || ep_mode == 3) && iob >= EP_RESET) {
      // epaper pseudo opcodes
      if (!universal_panel) return;
      EPDPanel* epd = static_cast<EPDPanel*>(universal_panel);
      
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
          epd->setLut(epd->cfg.lut_full_data, epd->cfg.lutfsize);
          epd->setUpdateMode(DISPLAY_INIT_FULL);
          break;
        case EP_LUT_PARTIAL:
          epd->setLut(epd->cfg.lut_partial_data, epd->cfg.lutpsize);
          epd->setUpdateMode(DISPLAY_INIT_PARTIAL);
          break;
        case EP_WAITIDLE:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
          }
          epd->delay_sync(iob * 10);
          break;
        case EP_SET_MEM_AREA:
          epd->setMemoryArea(0, 0, gxs - 1, gys - 1);
          break;
        case EP_SET_MEM_PTR:
          epd->setMemoryPointer(0, 0);
          break;
        case EP_SEND_DATA:
          epd->sendEPData();
          break;
        case EP_CLR_FRAME:
          epd->clearFrameMemory(0xFF);
          break;
        case EP_SEND_FRAME:
          epd->setFrameMemory(framebuffer);
          break;
        case EP_BREAK_RR_EQU:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
            if (iob == ESP_ResetInfoReason()) {
              epd->setUpdateMode(DISPLAY_INIT_PARTIAL);
              goto exit;
            }
          }
          break;
        case EP_BREAK_RR_NEQ:
          if (args & 1) {
            iob = dsp_cmds[cmd_offset++];
            index++;
            if (iob != ESP_ResetInfoReason()) {
              epd->setUpdateMode(DISPLAY_INIT_PARTIAL);
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
      if (spiController->spi_config.dc == -2) {
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
      spiController->writeCommand(iob);
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
          spiController->writeData8(iob);
        } else {
          spiController->writeCommand(iob);
        }
      }
      spiController->csHigh();
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

    if (wire) {
      // Populate remaining I2C config fields (most already parsed directly into union)
      panel_config->i2c.width = gxs;
      panel_config->i2c.height = gys;
      panel_config->i2c.bpp = bpp;
      panel_config->i2c.i2c_address = i2caddr;
      panel_config->i2c.wire = wire;
      panel_config->i2c.cmd_display_on = dsp_on;
      panel_config->i2c.cmd_display_off = dsp_off;
      panel_config->i2c.init_commands = dsp_cmds;
      panel_config->i2c.init_commands_count = dsp_ncmds;
      
      universal_panel = new i2c_panel(panel_config->i2c, frame_buffer);
    }
  }

if (interface == _UDSP_SPI) {

    HandeBP(-1);

    spiController->beginTransaction();
 
    if (reset >= 0) {
      pinMode(reset, OUTPUT);
      digitalWrite(reset, HIGH);
      delay(50);
      reset_pin(50, 200);
#ifdef UDSP_DEBUG
      AddLog(LOG_LEVEL_DEBUG, "UDisplay: resetting device");
#endif
    }
    
    if (ep_mode) {
        // Populate remaining EPD config fields (LUT data already parsed into union)
        panel_config->epd.width = gxs;
        panel_config->epd.height = gys;
        panel_config->epd.bpp = bpp;
        panel_config->epd.ep_mode = ep_mode;
        // Timing values already set in panel_config->epd (either defaults or from :T section)
        panel_config->epd.reset_pin = reset;
        panel_config->epd.busy_pin = spiController->spi_config.miso;
        panel_config->epd.invert_colors = true; // IF_INVERT_COLOR was hardcoded to 1
        panel_config->epd.invert_framebuffer = true; // TODO: maybe use lvgl_param.invert_bw for per-display config?
        panel_config->epd.busy_invert = (bool)lvgl_param.busy_invert;
        
        // Set callback for sending command sequences
        panel_config->epd.send_cmds_callback = [this](uint16_t offset, uint16_t count) {
            this->send_spi_cmds(offset, count);
        };
        
        // Create EPD panel BEFORE sending init commands (send_spi_cmds needs universal_panel)
        universal_panel = new EPDPanel(panel_config->epd, spiController, frame_buffer);
        send_spi_cmds(0, dsp_ncmds);
        
        // After descriptor init commands, do initial EPD setup
        EPDPanel* epd = static_cast<EPDPanel*>(universal_panel);
        epd->resetDisplay();
        if (epd->cfg.lut_full && epd->cfg.lut_full_len > 0) {
            epd->setLut(epd->cfg.lut_full, epd->cfg.lut_full_len);
        }
        epd->clearFrameMemory(0xFF);
        epd->displayFrame();
        
        // Send full update command sequence if available
        if (epd->cfg.epc_full_cnt) {
            send_spi_cmds(epd->cfg.epcoffs_full, epd->cfg.epc_full_cnt);
        }
        
        // Set update mode to partial for subsequent updates
        epd->setUpdateMode(DISPLAY_INIT_PARTIAL);
    } else {   
        AddLog(2,"SPI Panel!");
        // Populate remaining SPI config fields (most already parsed directly into union)
        panel_config->spi.width = gxs;
        panel_config->spi.height = gys;
        panel_config->spi.bpp = bpp;
        panel_config->spi.cmd_display_on = dsp_on;
        panel_config->spi.cmd_display_off = dsp_off;
        panel_config->spi.reset_pin = reset;
        panel_config->spi.bpanel = bpanel;
        panel_config->spi.all_commands_mode = allcmd_mode;
        
        send_spi_cmds(0, dsp_ncmds);  // Send init commands for regular SPI
      
        universal_panel = new SPIPanel(panel_config->spi, spiController, frame_buffer);
#ifdef ESP32
        spiController->initDMA(panel_config->spi.width, lvgl_param.flushlines, lvgl_param.data);
#endif
    }

    spiController->endTransaction();
    
    // EPD LUT initialization is now handled inside EPDPanel constructor
    // so we don't need to call Init_EPD here anymore
}

#if SOC_LCD_RGB_SUPPORTED
  if (interface == _UDSP_RGB) {
    if (!UsePSRAM())  {        // RGB is not supported on S3 without PSRAM
      #ifdef UDSP_DEBUG
      AddLog(LOG_LEVEL_INFO, "UDisplay: Dsp RGB requires PSRAM, abort");
      #endif
      return NULL;
    }

    HandeBP(-1);

    panel_config->rgb.clk_src = LCD_CLK_SRC_PLL160M;
    panel_config->rgb.timings.pclk_hz = spi_speed*1000000;
    panel_config->rgb.timings.h_res = gxs;
    panel_config->rgb.timings.v_res = gys;
    panel_config->rgb.data_width = 16; // RGB565 in parallel mode, thus 16bit in width
    panel_config->rgb.sram_trans_align = 8;
    panel_config->rgb.psram_trans_align = 64;

    // Handle byte swapping by swapping the low and high byte pin assignments
    if (lvgl_param.swap_color) {
      for (uint32_t cnt = 0; cnt < 8; cnt++) {
        int8_t temp = panel_config->rgb.data_gpio_nums[cnt];
        panel_config->rgb.data_gpio_nums[cnt] = panel_config->rgb.data_gpio_nums[cnt + 8];
        panel_config->rgb.data_gpio_nums[cnt + 8] = temp;
      }
      lvgl_param.swap_color = 0;
    }

    panel_config->rgb.disp_gpio_num = GPIO_NUM_NC;

    panel_config->rgb.flags.disp_active_low = 0;
    panel_config->rgb.flags.refresh_on_demand = 0;
    panel_config->rgb.flags.fb_in_psram = 1;             // allocate frame buffer in PSRAM

    universal_panel = new RGBPanel(&panel_config->rgb);
    rgb_fb = universal_panel->framebuffer;
    // super->setDrawMode();

  }
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
     if (interface == _UDSP_DSI) {
        // Pass init commands to DSI panel config
        panel_config->dsi.init_commands = dsp_cmds;
        panel_config->dsi.init_commands_count = dsp_ncmds;
               
        // Pass display on/off commands from descriptor
        panel_config->dsi.cmd_display_on = dsp_on;
        panel_config->dsi.cmd_display_off = dsp_off;
        
        universal_panel = new DSIPanel(panel_config->dsi);
        rgb_fb = universal_panel->framebuffer;
                
        HandeBP(-1);
     }
#endif

  if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
  #ifdef UDISPLAY_I80
        // Reset handling
      if (reset >= 0) {
          pinMode(reset, OUTPUT);
          digitalWrite(reset, HIGH);
          delay(50);
          reset_pin(50, 200);
      }
      
      // Populate remaining I80 config fields (most already parsed directly into union)
      // Control and data pins already parsed directly into config during INI parsing
      panel_config->i80.width = gxs;
      panel_config->i80.height = gys;
      panel_config->i80.bpp = bpp;
      panel_config->i80.color_mode = col_mode;
      panel_config->i80.bus_width = (uint8_t)((interface == _UDSP_PAR16) ? 16 : 8);
      panel_config->i80.clock_speed_hz = (uint32_t)spi_speed * 1000000;
      panel_config->i80.allcmd_mode = allcmd_mode;
      // Set sa_mode default if not parsed (old descriptors may not have it)
      if (panel_config->i80.sa_mode == 0) {
        panel_config->i80.sa_mode = sa_mode;  // Use global default (16)
      }
      panel_config->i80.init_commands = dsp_cmds;
      panel_config->i80.init_commands_count = dsp_ncmds;
      
      universal_panel = new I80Panel(panel_config->i80);

      HandeBP(-1);

  #endif
  }
  
  if(!universal_panel){
    return NULL;
  }

#ifdef UDSP_DEBUG
  AddLog(LOG_LEVEL_DEBUG, "UDisplay: Dsp Init 1 complete");
#endif
  return this;
}

void uDisplay::DisplayInit(int8_t p, int8_t size, int8_t rot, int8_t font) {
  if (p != DISPLAY_INIT_MODE && ep_mode) {
    if (p == DISPLAY_INIT_PARTIAL) {
      if (universal_panel) {
        EPDPanel* epd = static_cast<EPDPanel*>(universal_panel);
        epd->setUpdateMode(DISPLAY_INIT_PARTIAL);
        if (epd->cfg.lutpsize) {
#ifdef UDSP_DEBUG
          AddLog(LOG_LEVEL_DEBUG, "init partial epaper mode");
#endif
          epd->setLut(epd->cfg.lut_partial_data, epd->cfg.lutpsize);
          epd->updateFrame();
          epd->delay_sync(epd->cfg.lut_partial_time * 10);
        }
      }
      return;
    } else if (p == DISPLAY_INIT_FULL) {
#ifdef UDSP_DEBUG
      AddLog(LOG_LEVEL_DEBUG, "init full epaper mode");
#endif
      if (universal_panel) {
        EPDPanel* epd = static_cast<EPDPanel*>(universal_panel);
        epd->setUpdateMode(DISPLAY_INIT_FULL);
        if (epd->cfg.lutfsize) {
          epd->setLut(epd->cfg.lut_full_data, epd->cfg.lutfsize);
          epd->updateFrame();
        }
        if (ep_mode == 2) {
          epd->clearFrame_42();
          epd->displayFrame_42();
        }
        epd->delay_sync(epd->cfg.lut_full_time * 10);
      }
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

#define WIRE_MAX 32
