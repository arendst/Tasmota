/*
  xdrv_81_0_esp32_webcam_CSI_isp.ino - ESP32-P4 CSI ISP Configuration

  Copyright (C) 2025  Christian Baars and Theo Arends

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

/*********************************************************************************************\
 * ESP32-P4 Hardware ISP Configuration
 * 
 * Pure C++ implementation - no Berry dependencies
 * 
 * Responsibilities:
 * - Load and parse /isp.json from filesystem
 * - Apply ISP settings: CCM, Gamma, Sharpen, Color, BF (Bilateral Filter)
 * - Initialize and manage AWB (Auto White Balance) controller
 * - Run AWB processing loop (called from xdrv_81 core at 4Hz)
 * 
 * Graceful degradation:
 * - If isp.json is missing or malformed, ISP runs with bare minimum configuration
 * - Individual section failures are logged but don't stop other sections
\*********************************************************************************************/

#ifdef ESP32
#ifdef USE_CSI_WEBCAM

#include "esp_idf_version.h"
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)

#include "driver/isp.h"
#include "driver/isp_bf.h"
#include "driver/isp_sharpen.h"
#include "driver/isp_color.h"
#include "driver/isp_ccm.h"
#include "driver/isp_gamma.h"
#include "driver/isp_awb.h"

/*********************************************************************************************/

// AWB state - file-static, used by init and C processing loop
static struct {
  isp_awb_ctlr_t  handle;
  bool            enabled;
  float           base_ccm[3][3];   // Base CCM matrix (before AWB correction)
  float           gain_r, gain_b;   // Current WB correction gains
  float           min_r_step;       // Minimum gain delta to trigger update
  float           min_b_step;
  uint32_t        min_counted;      // Minimum white patches before updating
} isp_awb_state = { NULL, false, {{1,0,0},{0,1,0},{0,0,1}}, 1.0f, 1.0f, 0.031f, 0.031f, 2000 };

/*********************************************************************************************/
// Forward declarations

void WcIspApplyCCM(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyGamma(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplySharpen(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyColor(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyBF(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspInitAWB(isp_proc_handle_t handle, JsonParserObject &sensor, int width, int height);

/*********************************************************************************************/

// Main entry point: Load isp.json and apply all ISP settings
// Called from WcSetup() in xdrv_81_0 after ISP handle is created
// Returns true if configuration was applied, false otherwise
bool WcIspApplyConfig(isp_proc_handle_t handle, const char* sensor_name, int width, int height) {
  if (!handle) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP handle is NULL"));
    return false;
  }
  
  // Check if file exists
  if (!TfsFileExists("/isp.json")) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: No isp.json found, using bare ISP"));
    return false;
  }
  
  // Get file size and allocate buffer
  size_t file_size = TfsFileSize("/isp.json");
  if (file_size == 0 || file_size > 2047) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: isp.json invalid size: %d"), file_size);
    return false;
  }
  
  size_t buf_size = file_size + 1;  // +1 for null terminator
  char *buf = (char*)malloc(buf_size);
  if (!buf) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate %d bytes for JSON"), buf_size);
    return false;
  }
  
  // Load file directly into buffer
  memset(buf, 0, buf_size);
  if (!TfsLoadFile("/isp.json", (uint8_t*)buf, file_size)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to load isp.json"));
    free(buf);
    return false;
  }
  
  // Parse JSON
  JsonParser parser(buf);
  JsonParserToken root = parser.getRoot();
  if (!root) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: isp.json parse failed"));
    free(buf);
    return false;
  }
  
  JsonParserObject root_obj = root.getObject();
  if (!root_obj) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: isp.json root is not an object"));
    free(buf);
    return false;
  }
  
  // Find sensor section
  JsonParserToken sensor_tok = root_obj[sensor_name];
  if (!sensor_tok || !sensor_tok.isObject()) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: No ISP section for sensor %s"), sensor_name);
    free(buf);
    return false;
  }
  
  JsonParserObject sensor = sensor_tok.getObject();
  
  // Apply all ISP settings (each function handles its own errors gracefully)
  WcIspApplyCCM(handle, sensor);
  WcIspApplyGamma(handle, sensor);
  WcIspApplySharpen(handle, sensor);
  WcIspApplyColor(handle, sensor);
  WcIspApplyBF(handle, sensor);
  WcIspInitAWB(handle, sensor, width, height);
  
  free(buf);
  return true;
}

/*********************************************************************************************/

// Helper: Find nearest entry in array by numeric key
// Returns index of nearest entry, or -1 if not found
int WcIspFindNearest(JsonParserArray &arr, const char* key, int target_value) {
  if (!arr) return -1;
  
  int best_idx = -1;
  int best_dist = INT_MAX;
  int idx = 0;
  
  for (auto entry_tok : arr) {
    if (!entry_tok.isObject()) {
      idx++;
      continue;
    }
    
    JsonParserObject entry = entry_tok.getObject();
    JsonParserToken val_tok = entry[key];
    if (!val_tok) {
      idx++;
      continue;
    }
    
    int val = val_tok.getInt();
    int dist = abs(val - target_value);
    if (dist < best_dist) {
      best_dist = dist;
      best_idx = idx;
    }
    idx++;
  }
  
  return best_idx;
}

/*********************************************************************************************/

// Apply CCM (Color Correction Matrix)
// JSON: {"acc": {"ccm": {"table": [{"color_temp": 6500, "matrix": [flat 9 floats]}, ...]}}
void WcIspApplyCCM(isp_proc_handle_t handle, JsonParserObject &sensor) {
  // Navigate: sensor["acc"]["ccm"]["table"]
  JsonParserToken acc_tok = sensor["acc"];
  if (!acc_tok || !acc_tok.isObject()) return;
  JsonParserObject acc = acc_tok.getObject();
  
  JsonParserToken ccm_tok = acc["ccm"];
  if (!ccm_tok || !ccm_tok.isObject()) return;
  JsonParserObject ccm_obj = ccm_tok.getObject();
  
  JsonParserToken table_tok = ccm_obj["table"];
  if (!table_tok || !table_tok.isArray()) return;
  JsonParserArray table = table_tok.getArray();
  
  // Find nearest color temperature to 6500K (daylight)
  int best_idx = WcIspFindNearest(table, "color_temp", 6500);
  if (best_idx < 0) return;
  
  JsonParserToken best_tok = table[best_idx];
  if (!best_tok.isObject()) return;
  JsonParserObject best = best_tok.getObject();
  
  JsonParserToken mat_tok = best["matrix"];
  if (!mat_tok || !mat_tok.isArray()) return;
  JsonParserArray mat = mat_tok.getArray();
  
  // Flat 9-element array
  esp_isp_ccm_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  int i = 0;
  for (auto val : mat) {
    if (i >= 9) break;
    float fval = val.getFloat(0.0f);
    cfg.matrix[i/3][i%3] = fval;
    isp_awb_state.base_ccm[i/3][i%3] = fval;
    i++;
  }
  
  esp_isp_ccm_configure(handle, &cfg);
  esp_isp_ccm_enable(handle);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP CCM applied"));
}

/*********************************************************************************************/

uint32_t WcIspGammaInterp(uint32_t x) {
  uint32_t seg = (x * 15) / 255;
  if (seg >= 15) return Wc.core.isp_gamma_y[15];
  uint32_t x0 = seg * 255 / 15;
  uint32_t x1 = (seg + 1) * 255 / 15;
  uint32_t y0 = Wc.core.isp_gamma_y[seg];
  uint32_t y1 = Wc.core.isp_gamma_y[seg + 1];
  uint32_t dx = x1 - x0;
  return dx ? y0 + (y1 - y0) * (x - x0) / dx : y0;
}

// Apply Gamma Curve
// JSON: {"aen": {"gamma": {"table": [{"luma": 50, "y": [0,...15 values...]}, ...]}}}
void WcIspApplyGamma(isp_proc_handle_t handle, JsonParserObject &sensor) {
  JsonParserToken aen_tok = sensor["aen"];
  if (!aen_tok || !aen_tok.isObject()) {
    return;
  }
  
  JsonParserObject aen = aen_tok.getObject();
  JsonParserToken gamma_tok = aen["gamma"];
  if (!gamma_tok || !gamma_tok.isObject()) {
    return;
  }
  
  JsonParserObject gamma_obj = gamma_tok.getObject();
  JsonParserToken table_tok = gamma_obj["table"];
  if (!table_tok || !table_tok.isArray()) {
    return;
  }
  
  JsonParserArray table = table_tok.getArray();
  
  // Find nearest luma to 50 (normal scene)
  int best_idx = WcIspFindNearest(table, "luma", 50);
  if (best_idx < 0) {
    return;
  }
  
  JsonParserToken best_tok = table[best_idx];
  if (!best_tok.isObject()) {
    return;
  }
  
  JsonParserObject best = best_tok.getObject();
  JsonParserToken y_tok = best["y"];
  if (!y_tok || !y_tok.isArray()) {
    return;
  }
  
  JsonParserArray y_arr = y_tok.getArray();
  
  int i = 0;
  for (auto v : y_arr) {
    if (i >= 16) break;
    Wc.core.isp_gamma_y[i++] = (uint32_t)v.getInt(0);
  }

  isp_gamma_curve_points_t pts = {};
  esp_isp_gamma_fill_curve_points(WcIspGammaInterp, &pts);
  
  // Apply to all color components
  esp_isp_gamma_configure(handle, COLOR_COMPONENT_R, &pts);
  esp_isp_gamma_configure(handle, COLOR_COMPONENT_G, &pts);
  esp_isp_gamma_configure(handle, COLOR_COMPONENT_B, &pts);
  esp_isp_gamma_enable(handle);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Gamma applied (luma=50)"));
}

/*********************************************************************************************/

// Apply Sharpen Filter
// JSON: {"aen": {"sharpen": [{"gain": 1, "param": {"h_thresh":..., "l_thresh":..., "h_coeff":..., "m_coeff":..., "matrix":[...]}}]}}
void WcIspApplySharpen(isp_proc_handle_t handle, JsonParserObject &sensor) {
  JsonParserToken aen_tok = sensor["aen"];
  if (!aen_tok || !aen_tok.isObject()) {
    return;
  }
  
  JsonParserObject aen = aen_tok.getObject();
  JsonParserToken sh_tok = aen["sharpen"];
  if (!sh_tok || !sh_tok.isArray()) {
    return;
  }
  
  JsonParserArray sh_arr = sh_tok.getArray();
  
  // Find nearest gain to 1
  int best_idx = WcIspFindNearest(sh_arr, "gain", 1);
  if (best_idx < 0) {
    return;
  }
  
  JsonParserToken best_tok = sh_arr[best_idx];
  if (!best_tok.isObject()) {
    return;
  }
  
  JsonParserObject best = best_tok.getObject();
  JsonParserToken param_tok = best["param"];
  if (!param_tok || !param_tok.isObject()) {
    return;
  }
  
  JsonParserObject param = param_tok.getObject();
  
  // Parse sharpen parameters
  esp_isp_sharpen_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  
  cfg.h_thresh = param.getInt("h_thresh", 255);
  cfg.l_thresh = param.getInt("l_thresh", 128);
  
  float h_coeff = param.getFloat("h_coeff", 1.0f);
  cfg.h_freq_coeff.integer = (uint8_t)h_coeff;
  cfg.h_freq_coeff.decimal = (uint8_t)((h_coeff - (int)h_coeff) * 256);
  
  float m_coeff = param.getFloat("m_coeff", 1.0f);
  cfg.m_freq_coeff.integer = (uint8_t)m_coeff;
  cfg.m_freq_coeff.decimal = (uint8_t)((m_coeff - (int)m_coeff) * 256);
  
  // Parse 3x3 matrix
  JsonParserToken mat_tok = param["matrix"];
  if (mat_tok && mat_tok.isArray()) {
    JsonParserArray mat = mat_tok.getArray();
    int i = 0;
    for (auto v : mat) {
      if (i >= 9) break;
      cfg.sharpen_template[i/3][i%3] = (uint8_t)v.getFloat(0);
      i++;
    }
  }
  
  cfg.padding_mode = ISP_SHARPEN_EDGE_PADDING_MODE_SRND_DATA;
  
  esp_isp_sharpen_configure(handle, &cfg);
  esp_isp_sharpen_enable(handle);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Sharpen applied (gain=1)"));
}

/*********************************************************************************************/

// Apply Color Adjustment
// JSON: {"ext": {"hue":0, "brightness":0}, "aen": {"contrast": [{"gain":1, "value":128}]}}
void WcIspApplyColor(isp_proc_handle_t handle, JsonParserObject &sensor) {
  esp_isp_color_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  
  // contrast from aen.contrast[] nearest gain=1
  cfg.color_contrast.integer = 1;
  JsonParserToken aen_tok = sensor["aen"];
  if (aen_tok && aen_tok.isObject()) {
    JsonParserObject aen = aen_tok.getObject();
    JsonParserToken con_tok = aen["contrast"];
    if (con_tok && con_tok.isArray()) {
      JsonParserArray con_arr = con_tok.getArray();
      int best_idx = WcIspFindNearest(con_arr, "gain", 1);
      if (best_idx >= 0) {
        int val = con_arr[best_idx].getObject().getInt("value", 128);
        cfg.color_contrast.integer = (uint8_t)(val >> 7);        // scale 0-255 → ISP fixed-point
        cfg.color_contrast.decimal = (uint8_t)((val & 0x7F) << 1);
      }
    }
  }
  
  // hue + brightness from ext
  JsonParserToken ext_tok = sensor["ext"];
  if (ext_tok && ext_tok.isObject()) {
    JsonParserObject ext = ext_tok.getObject();
    cfg.color_hue        = ext.getInt("hue", 0);
    cfg.color_brightness = ext.getInt("brightness", 0);
  }
  
  // TODO: read from acc.saturation[] once color temperature estimation is available
  cfg.color_saturation.integer = 1;
  
  esp_isp_color_configure(handle, &cfg);
  esp_isp_color_enable(handle);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Color applied"));
}

/*********************************************************************************************/

// Apply Bilateral Filter (Denoising)
// JSON: {"adn": {"bf": [{"gain":1, "param": {"level":2, "matrix":[flat 9]}}]}}
void WcIspApplyBF(isp_proc_handle_t handle, JsonParserObject &sensor) {
  JsonParserToken adn_tok = sensor["adn"];
  if (!adn_tok || !adn_tok.isObject()) return;
  JsonParserObject adn = adn_tok.getObject();
  
  JsonParserToken bf_tok = adn["bf"];
  if (!bf_tok || !bf_tok.isArray()) return;
  JsonParserArray bf_arr = bf_tok.getArray();
  
  int best_idx = WcIspFindNearest(bf_arr, "gain", 1);  // gain=1 = no AGC, normal light
  if (best_idx < 0) return;
  
  JsonParserObject best = bf_arr[best_idx].getObject();
  JsonParserToken param_tok = best["param"];
  if (!param_tok || !param_tok.isObject()) return;
  JsonParserObject param = param_tok.getObject();
  
  esp_isp_bf_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.denoising_level = param.getInt("level", 2);
  
  JsonParserToken mat_tok = param["matrix"];
  if (mat_tok && mat_tok.isArray()) {
    JsonParserArray mat = mat_tok.getArray();
    int i = 0;
    for (auto v : mat) {
      if (i >= 9) break;
      cfg.bf_template[i/3][i%3] = (uint8_t)v.getFloat(0);
      i++;
    }
  }
  cfg.padding_mode = ISP_BF_EDGE_PADDING_MODE_SRND_DATA;
  esp_isp_bf_configure(handle, &cfg);
  esp_isp_bf_enable(handle);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP BF applied"));
}

/*********************************************************************************************/

// Initialize AWB (Auto White Balance)
// JSON: {"awb": {"range": {"green":{min,max}, "rg":{min,max}, "bg":{min,max}}, "min_red_gain_step":..., "min_blue_gain_step":..., "min_counted":...}}
void WcIspInitAWB(isp_proc_handle_t handle, JsonParserObject &sensor, int width, int height) {
  JsonParserToken awb_tok = sensor["awb"];
  if (!awb_tok || !awb_tok.isObject()) return;
  JsonParserObject awb = awb_tok.getObject();
  
  if (isp_awb_state.handle) {
    esp_isp_awb_controller_disable(isp_awb_state.handle);
    esp_isp_del_awb_controller(isp_awb_state.handle);
    isp_awb_state.handle = NULL;
    isp_awb_state.enabled = false;
  }
  
  // Read nested range object
  JsonParserToken range_tok = awb["range"];
  if (!range_tok || !range_tok.isObject()) return;
  JsonParserObject range = range_tok.getObject();
  
  JsonParserObject green_obj = range["green"].getObject();
  JsonParserObject rg_obj    = range["rg"].getObject();
  JsonParserObject bg_obj    = range["bg"].getObject();
  
  int lum_min = green_obj.getInt("min", 128);
  int lum_max = green_obj.getInt("max", 240);
  float rg_min = rg_obj.getFloat("min", 0.7f);
  float rg_max = rg_obj.getFloat("max", 1.3f);
  float bg_min = bg_obj.getFloat("min", 0.7f);
  float bg_max = bg_obj.getFloat("max", 1.3f);
  
  int x1 = width / 10;
  int y1 = height / 10;
  int x2 = width - width / 10;
  int y2 = height - height / 10;
  
  esp_isp_awb_config_t awb_cfg;
  memset(&awb_cfg, 0, sizeof(awb_cfg));
  awb_cfg.sample_point = (isp_awb_sample_point_t)awb.getInt("sample_point", 1);
  awb_cfg.window.top_left.x  = x1;
  awb_cfg.window.top_left.y  = y1;
  awb_cfg.window.btm_right.x = x2;
  awb_cfg.window.btm_right.y = y2;
  awb_cfg.subwindow = awb_cfg.window;
  awb_cfg.white_patch.luminance.min          = (uint32_t)lum_min;
  awb_cfg.white_patch.luminance.max          = (uint32_t)lum_max;
  awb_cfg.white_patch.red_green_ratio.min    = rg_min;
  awb_cfg.white_patch.red_green_ratio.max    = rg_max;
  awb_cfg.white_patch.blue_green_ratio.min   = bg_min;
  awb_cfg.white_patch.blue_green_ratio.max   = bg_max;
  
  isp_awb_ctlr_t ctlr = NULL;
  esp_err_t ret = esp_isp_new_awb_controller(handle, &awb_cfg, &ctlr);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP AWB init failed (0x%x)"), ret);
    return;
  }
  ret = esp_isp_awb_controller_enable(ctlr);
  if (ret != ESP_OK) {
    esp_isp_del_awb_controller(ctlr);
    return;
  }
  
  isp_awb_state.handle      = ctlr;
  isp_awb_state.enabled     = true;
  isp_awb_state.gain_r      = 1.0f;
  isp_awb_state.gain_b      = 1.0f;
  isp_awb_state.min_counted = (uint32_t)awb.getInt("min_counted", 2000);
  isp_awb_state.min_r_step  = awb.getFloat("min_red_gain_step", 0.034f);
  isp_awb_state.min_b_step  = awb.getFloat("min_blue_gain_step", 0.034f);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AWB initialized"));
}

/*********************************************************************************************/

// AWB Processing Loop - called from xdrv_81 FUNC_EVERY_250_MSECOND (4Hz)
// Grey-world algorithm: gain_r = avg_G/avg_R, gain_b = avg_G/avg_B
// Applied as diagonal correction on base CCM
void WcIspAwbProcess(void) {
  if (Wc.core.state != CAM_STREAMING) return;
  if (!isp_awb_state.enabled || !isp_awb_state.handle) return;
  isp_proc_handle_t isp = Wc.core.isp_handle;
  if (!isp) return;

  isp_awb_stat_result_t stats;
  memset(&stats, 0, sizeof(stats));

  esp_err_t ret = esp_isp_awb_controller_get_oneshot_statistics(
    isp_awb_state.handle, 100, &stats);
  if (ret != ESP_OK) {
    return;
  }

  if (stats.white_patch_num < isp_awb_state.min_counted) {
    return;
  }

  float avg_r = (float)stats.sum_r / (float)stats.white_patch_num;
  float avg_g = (float)stats.sum_g / (float)stats.white_patch_num;
  if (avg_g < 8.0f) return; // too dark??
  float avg_b = (float)stats.sum_b / (float)stats.white_patch_num;

  if (avg_r < 1.0f) avg_r = 1.0f;
  if (avg_b < 1.0f) avg_b = 1.0f;

  float new_gain_r = avg_g / avg_r;
  float new_gain_b = avg_g / avg_b;

  // Clamp gains to reasonable range
  if (new_gain_r < 0.5f) new_gain_r = 0.5f;
  if (new_gain_r > 2.0f) new_gain_r = 2.0f;
  if (new_gain_b < 0.5f) new_gain_b = 0.5f;
  if (new_gain_b > 2.0f) new_gain_b = 2.0f;

  // Check if delta exceeds minimum step
  if (fabsf(new_gain_r - isp_awb_state.gain_r) < isp_awb_state.min_r_step &&
      fabsf(new_gain_b - isp_awb_state.gain_b) < isp_awb_state.min_b_step) {
    return;
  }

  isp_awb_state.gain_r = new_gain_r;
  isp_awb_state.gain_b = new_gain_b;

  // Apply: corrected_ccm = diag(gain_r, 1, gain_b) * base_ccm
  esp_isp_ccm_config_t ccm_cfg;
  memset(&ccm_cfg, 0, sizeof(ccm_cfg));
  for (int c = 0; c < 3; c++) {
    ccm_cfg.matrix[0][c] = isp_awb_state.base_ccm[0][c] * isp_awb_state.gain_r;
    ccm_cfg.matrix[1][c] = isp_awb_state.base_ccm[1][c];
    ccm_cfg.matrix[2][c] = isp_awb_state.base_ccm[2][c] * isp_awb_state.gain_b;
  }

  esp_isp_ccm_configure(isp, &ccm_cfg);
}

/*********************************************************************************************/

// Deinitialize AWB - called when streaming stops
void WcIspDeinitAWB(void) {
  if (!isp_awb_state.handle) return;

  isp_awb_state.enabled = false;
  
  memset(isp_awb_state.base_ccm, 0, sizeof(isp_awb_state.base_ccm));
  isp_awb_state.base_ccm[0][0] = 1.0f;
  isp_awb_state.base_ccm[1][1] = 1.0f;
  isp_awb_state.base_ccm[2][2] = 1.0f;

  isp_proc_handle_t isp = Wc.core.isp_handle;
  if (!isp) {
    isp_awb_state.handle = NULL;
    return;
  }

  esp_isp_awb_controller_disable(isp_awb_state.handle);
  esp_isp_del_awb_controller(isp_awb_state.handle);
  isp_awb_state.handle = NULL;
  isp_awb_state.gain_r = 1.0f;
  isp_awb_state.gain_b = 1.0f;
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AWB deinitialized"));
}

#endif // ESP_IDF_VERSION >= 5.5.0
#endif // USE_CSI_WEBCAM
#endif // ESP32
