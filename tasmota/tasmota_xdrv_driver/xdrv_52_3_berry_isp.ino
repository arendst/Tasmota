/*
  xdrv_52_3_berry_isp.ino - Berry scripting language, native functions

  Copyright (C) 2025 Christian Baars & Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


/*
  xdrv_52_3_berry_isp.ino - Native Berry ISP Implementation for ESP-IDF 5.5+
  FIXED: 
  - Replaced designated initializers with direct assignment to fix ordering errors.
  - Corrected Gamma point assignment (setting .y and .x members).
*/

#ifdef USE_BERRY
#include <berry.h>

#ifdef USE_CSI_WEBCAM
// Global ISP Processor Handle (Must be shared with xdrv_81)
#include "driver/isp.h"
isp_proc_handle_t tasmota_wc_isp_handle = NULL; // we do not have stable IDF 5.5 yet, but we need this symbol

#ifdef PLACEHOLDER_UNTIL_IDF55_ISMATURED //USE_CSI_WEBCAM

#include "driver/isp.h"
#include "driver/isp_bf.h"
#include "driver/isp_sharpen.h"
#include "driver/isp_color.h"
#include "driver/isp_ccm.h"
#include "driver/isp_gamma.h"
#include <cstring>


extern "C" {

  // Helper: Parse Berry list to flat float array
  static int ParseFloatList(bvm *vm, int index, float* out, int max_len) {
      if (!be_islist(vm, index)) return 0;
      int count = be_data_size(vm, index);
      if (count > max_len) count = max_len;
      be_pushvalue(vm, index);
      for (int i = 0; i < count; i++) {
          be_pushint(vm, i);
          be_getindex(vm, -2);
          out[i] = be_isnumber(vm, -1) ? be_toreal(vm, -1) : 0.0f;
          be_pop(vm, 1);
      }
      be_pop(vm, 1);
      return count;
  }

  // ISP.init(width, height)
  int be_ISP_init(bvm *vm) {
      int width = be_toint(vm, 1);
      int height = be_toint(vm, 2);
      
      if (tasmota_wc_isp_handle) {
          be_pushbool(vm, true);
          return 1;
      }

      // FIXED: Use memset + direct assignment to bypass C++ designated initializer ordering issues
      esp_isp_processor_cfg_t isp_config;
      memset(&isp_config, 0, sizeof(isp_config));
      
      isp_config.clk_hz = 80 * 1000 * 1000;
      isp_config.clk_src = ISP_CLK_SRC_DEFAULT; 
      isp_config.input_data_source = ISP_INPUT_DATA_SOURCE_CSI;
      isp_config.input_data_color_type = ISP_COLOR_RAW8;
      isp_config.output_data_color_type = ISP_COLOR_YUV422;
      isp_config.h_res = (uint32_t)width;
      isp_config.v_res = (uint32_t)height;
      isp_config.has_line_start_packet = false;
      isp_config.has_line_end_packet = false;

      esp_err_t ret = esp_isp_new_processor(&isp_config, &tasmota_wc_isp_handle);
      if (ret == ESP_OK) {
          esp_isp_enable(tasmota_wc_isp_handle);
      }
      
      be_pushbool(vm, (ret == ESP_OK));
      return 1;
  }

  // ISP.set_ccm(matrix_list)
  int be_ISP_set_ccm(bvm *vm) {
      if (!tasmota_wc_isp_handle) return 0;
      
      esp_isp_ccm_config_t cfg = {0};
      float temp_matrix[9];
      ParseFloatList(vm, 1, temp_matrix, 9);
      
      int idx = 0;
      for(int r=0; r<3; r++) {
          for(int c=0; c<3; c++) {
              cfg.matrix[r][c] = temp_matrix[idx++];
          }
      }
      
      esp_isp_ccm_configure(tasmota_wc_isp_handle, &cfg);
      esp_isp_ccm_enable(tasmota_wc_isp_handle);
      return 0;
  }

  // ISP.set_gamma(curve_points_list)
  // curve_points_list: List of 16 Y-values (Output values)
  int be_ISP_set_gamma(bvm *vm) {
      if (!tasmota_wc_isp_handle) return 0;
      
      float temp_points[16];
      ParseFloatList(vm, 1, temp_points, 16);
      
      isp_gamma_curve_points_t pts;
      memset(&pts, 0, sizeof(pts));

      // P4 Gamma Curve has 16 points. 
      // We map the 16 input values to Y, and generate linear X steps.
      // Assuming 8-bit input (0-255), step is 16.
      for(int i=0; i<16; i++) {
          pts.pt[i].x = i * 16;                     // Auto-generate Input X (0, 16, 32...)
          pts.pt[i].y = (uint32_t)temp_points[i];   // FIXED: Assign to .y member
      }
      
      esp_isp_gamma_configure(tasmota_wc_isp_handle, COLOR_COMPONENT_R, &pts);
      esp_isp_gamma_configure(tasmota_wc_isp_handle, COLOR_COMPONENT_G, &pts);
      esp_isp_gamma_configure(tasmota_wc_isp_handle, COLOR_COMPONENT_B, &pts);
      esp_isp_gamma_enable(tasmota_wc_isp_handle);
      
      return 0;
  }

  // ISP.set_sharpen(h_thresh, l_thresh, h_coeff, m_coeff, template_list)
  int be_ISP_set_sharpen(bvm *vm) {
      if (!tasmota_wc_isp_handle) return 0;
      
      esp_isp_sharpen_config_t cfg;
      memset(&cfg, 0, sizeof(cfg));
      
      cfg.h_thresh = be_toint(vm, 1);
      cfg.l_thresh = be_toint(vm, 2);
      
      float h_val = be_toreal(vm, 3);
      cfg.h_freq_coeff.integer = (uint8_t)h_val;
      cfg.h_freq_coeff.decimal = (uint8_t)((h_val - (int)h_val) * 256);

      float m_val = be_toreal(vm, 4);
      cfg.m_freq_coeff.integer = (uint8_t)m_val;
      cfg.m_freq_coeff.decimal = (uint8_t)((m_val - (int)m_val) * 256);
      
      float temp[9];
      ParseFloatList(vm, 5, temp, 9);
      int idx = 0;
      for(int r=0; r<3; r++) {
          for(int c=0; c<3; c++) {
              cfg.sharpen_template[r][c] = (uint8_t)temp[idx++];
          }
      }
      
      cfg.padding_mode = ISP_SHARPEN_EDGE_PADDING_MODE_SRND_DATA;
      
      esp_isp_sharpen_configure(tasmota_wc_isp_handle, &cfg);
      esp_isp_sharpen_enable(tasmota_wc_isp_handle);
      return 0;
  }

  // ISP.set_color(contrast, saturation, hue, brightness)
  int be_ISP_set_color(bvm *vm) {
      if (!tasmota_wc_isp_handle) return 0;
      
      esp_isp_color_config_t cfg;
      memset(&cfg, 0, sizeof(cfg));
      
      float contrast = be_toreal(vm, 1);
      cfg.color_contrast.integer = (uint8_t)contrast;
      cfg.color_contrast.decimal = (uint8_t)((contrast - (int)contrast) * 256);
      
      float saturation = be_toreal(vm, 2);
      cfg.color_saturation.integer = (uint8_t)saturation;
      cfg.color_saturation.decimal = (uint8_t)((saturation - (int)saturation) * 256);
      
      cfg.color_hue = (be_top(vm) >= 3) ? be_toint(vm, 3) : 0;
      cfg.color_brightness = (be_top(vm) >= 4) ? be_toint(vm, 4) : 0;
      
      esp_isp_color_configure(tasmota_wc_isp_handle, &cfg);
      esp_isp_color_enable(tasmota_wc_isp_handle);
      return 0;
  }

  // ISP.set_bf(denoising_level, template_list)
  int be_ISP_set_bf(bvm *vm) {
      if (!tasmota_wc_isp_handle) return 0;
      
      esp_isp_bf_config_t cfg;
      memset(&cfg, 0, sizeof(cfg));
      
      cfg.denoising_level = be_toint(vm, 1);
      
      if (be_top(vm) >= 2 && be_islist(vm, 2)) {
          float temp[9];
          ParseFloatList(vm, 2, temp, 9);
          int idx = 0;
          for(int r=0; r<3; r++) {
              for(int c=0; c<3; c++) {
                  cfg.bf_template[r][c] = (uint8_t)temp[idx++];
              }
          }
      }
      
      cfg.padding_mode = ISP_BF_EDGE_PADDING_MODE_SRND_DATA;
      
      esp_isp_bf_configure(tasmota_wc_isp_handle, &cfg);
      esp_isp_bf_enable(tasmota_wc_isp_handle);
      return 0;
  }

} // extern C

#endif //PLACEHOLDER_UNTIL_IDF55_ISMATURED
#endif // USE_CSI_WEBCAM
#endif // USE_BERRY
