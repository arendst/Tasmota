/*
  xdrv_81_0_esp32_webcam_CSI_isp.ino - ESP32-P4 CSI ISP Configuration

  Copyright (C) 2025  Christian Baars and Theo Arends

  Runtime AE & CCM Implementation by Martin Macák - HexaMaster

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
 * - Run AE processing loop (called from xdrv_81 core at 4Hz)
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
#include "driver/isp_ae.h"
#include "driver/isp_demosaic.h"

/*********************************************************************************************/

struct WcIspNormConfig {
  struct {
    bool present;
    float gamma_param;
    uint8_t y[16];
  } gamma;

  struct {
    bool present;
    int contrast_0_255;
    int saturation_0_255;
    int hue;
    int brightness;
  } color;

  struct {
    bool present;
    int target;
    int target_low;
    int target_high;
    bool weight_present;
    uint8_t weight[ISP_AE_BLOCK_X_NUM][ISP_AE_BLOCK_Y_NUM];
  } agc;

  struct {
    bool present;
    float table[3][3];
  } ccm;

  struct {
    bool present;
    uint8_t count;

    struct {
      uint16_t gain;
      uint8_t level;
      uint8_t matrix[3][3];
    } profile[8];
  } bf;
};

static WcIspNormConfig isp_norm_cfg;

// AWB state - file-static, used by init and C processing loop
static struct {
  isp_awb_ctlr_t  handle;
  bool            enabled;

  float           base_ccm[3][3];
  float           gain_r, gain_b;
  float           min_r_step;
  float           min_b_step;
  uint32_t        min_counted;

  float           last_applied_gain_r;
  float           last_applied_gain_b;

  bool            runtime_ccm_valid;
  volatile bool   stats_ready;
  volatile uint32_t stats_seq;
  volatile uint32_t cb_count;
  uint32_t        last_consumed_seq;
  isp_awb_stat_result_t latest_stats;
} isp_awb_state = {
  NULL, false,
  {{1,0,0},{0,1,0},{0,0,1}},
  1.0f, 1.0f,
  0.031f, 0.031f,
  2000,
  1.0f, 1.0f,
  false,
  false, 0, 0, 0, {}
};

static struct {
  isp_ae_ctlr_t handle;
  bool enabled;
  volatile bool stats_ready;
  volatile uint32_t stats_seq;
  volatile uint32_t cb_count;
  isp_ae_result_t latest_result;
  uint8_t weight[ISP_AE_BLOCK_X_NUM][ISP_AE_BLOCK_Y_NUM];
  int target;
  int target_low;
  int target_high;
  int last_luma;
  uint32_t last_apply_ms;
  uint32_t last_consumed_seq;
} isp_ae_state = { NULL, false, false, 0, 0, {}, {{0}}, 79, 62, 105, 0, 0, 0 };

static struct {
  bool enabled;
  int last_profile_idx;
  uint32_t last_apply_ms;
} isp_bf_state = { false, -1, 0 };

static struct {
  bool valid;
  uint16_t vts;
  uint16_t exposure_lines;
  uint16_t analog_gain;   // OV02C10 raw, 0x10 = 1x
  uint16_t digital_gain;  // OV02C10 raw, 0x0400 = 1x
} isp_sensor_ae_state = { false, 0, 0, 0x10, 0x0400 };
/*********************************************************************************************/
// Forward declarations

void WcIspApplyCCM(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyGamma(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplySharpen(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyColor(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyBF(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspApplyDemosaic(isp_proc_handle_t handle, JsonParserObject &sensor);
void WcIspInitAWB(isp_proc_handle_t handle, JsonParserObject &sensor, int width, int height);
void WcIspInitAE(isp_proc_handle_t handle, JsonParserObject &sensor, int width, int height);
void WcIspStartAE(void);
void WcIspAeProcess(void);
void WcIspDeinitAE(void);
void WcIspStartAWB(void);
static void WcIspApplyRuntimeCcm(void);

void WcIspResetNormConfig(void);
void WcIspParseNormConfig(JsonParserObject &sensor);
bool WcIspParseMatrix9(JsonParserToken mat_tok, float out[3][3]);
bool WcIspParseWeight25(JsonParserToken weight_tok, uint8_t out[ISP_AE_BLOCK_X_NUM][ISP_AE_BLOCK_Y_NUM]);
void WcIspBuildGammaYFromParam(float gamma_param, uint8_t out[16]);
void WcIspSetDefaultGammaY(uint8_t out[16]);

bool WcIspApplyBfProfile(isp_proc_handle_t handle, int idx);
void WcIspApplyRuntimeBf(void);
int WcIspSelectRuntimeBfProfile(uint16_t gain_x1);
uint16_t WcIspGetCurrentTotalGainX1(void);
void WcIspMirrorAeSeed(uint16_t vts, uint16_t exposure_lines, uint16_t analog_gain, uint16_t digital_gain);
static void WcIspMirrorAeApplyStep(int step);
/*********************************************************************************************/

static bool WcIspCheckRet(const char *tag, esp_err_t ret) {
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: %s failed (0x%x)"), tag, ret);
    return false;
  }
  return true;
}

static uint8_t WcClampU8(int v, int lo, int hi) {
  if (v < lo) return (uint8_t)lo;
  if (v > hi) return (uint8_t)hi;
  return (uint8_t)v;
}

static int WcClampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static float WcClampFloat(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

void WcIspResetNormConfig(void) {
  memset(&isp_norm_cfg, 0, sizeof(isp_norm_cfg));

  isp_norm_cfg.gamma.present = false;
  isp_norm_cfg.gamma.gamma_param = 1.0f;
  WcIspSetDefaultGammaY(isp_norm_cfg.gamma.y);

  isp_norm_cfg.color.contrast_0_255 = 255;
  isp_norm_cfg.color.saturation_0_255 = 255;
  isp_norm_cfg.color.hue = 0;
  isp_norm_cfg.color.brightness = 0;

  isp_norm_cfg.bf.present = false;
  isp_norm_cfg.bf.count = 0;

  isp_norm_cfg.agc.target = 79;
  isp_norm_cfg.agc.target_low = 62;
  isp_norm_cfg.agc.target_high = 105;
  for (int x = 0; x < ISP_AE_BLOCK_X_NUM; x++) {
    for (int y = 0; y < ISP_AE_BLOCK_Y_NUM; y++) {
      isp_norm_cfg.agc.weight[x][y] = 1;
    }
  }

  isp_norm_cfg.ccm.table[0][0] = 1.0f;
  isp_norm_cfg.ccm.table[1][1] = 1.0f;
  isp_norm_cfg.ccm.table[2][2] = 1.0f;


}

bool WcIspParseMatrix9(JsonParserToken mat_tok, float out[3][3]) {
  if (!mat_tok || !mat_tok.isArray()) {
    return false;
  }

  JsonParserArray mat = mat_tok.getArray();
  int i = 0;
  for (auto v : mat) {
    if (i >= 9) break;
    out[i / 3][i % 3] = v.getFloat(0.0f);
    i++;
  }

  return i == 9;
}

bool WcIspParseWeight25(JsonParserToken weight_tok, uint8_t out[ISP_AE_BLOCK_X_NUM][ISP_AE_BLOCK_Y_NUM]) {
  if (!weight_tok || !weight_tok.isArray()) {
    return false;
  }

  JsonParserArray arr = weight_tok.getArray();
  int idx = 0;
  for (auto v : arr) {
    if (idx >= (ISP_AE_BLOCK_X_NUM * ISP_AE_BLOCK_Y_NUM)) break;
    int x = idx % ISP_AE_BLOCK_X_NUM;
    int y = idx / ISP_AE_BLOCK_X_NUM;
    out[x][y] = WcClampU8(v.getInt(1), 0, 255);
    idx++;
  }

  return idx == (ISP_AE_BLOCK_X_NUM * ISP_AE_BLOCK_Y_NUM);
}

void WcIspSetDefaultGammaY(uint8_t out[16]) {
  for (int i = 0; i < 16; i++) {
    out[i] = (uint8_t)((i * 255) / 15);
  }
}


void WcIspBuildGammaYFromParam(float gamma_param, uint8_t out[16]) {
  gamma_param = WcClampFloat(gamma_param, 0.05f, 5.0f);

  for (int i = 0; i < 16; i++) {
    float x = (float)((i * 255) / 15) / 255.0f;
    float y = powf(x, gamma_param) * 255.0f;
    int yi = (int)(y + 0.5f);
    out[i] = WcClampU8(yi, 0, 255);
  }

  out[0] = 0;
  out[15] = 255;
}


void WcIspParseNormConfig(JsonParserObject &sensor) {
  bool saturation_found = false;
  bool agc_weight_found = false;

  JsonParserToken ext_tok = sensor["ext"];
  if (ext_tok && ext_tok.isObject()) {
    JsonParserObject ext = ext_tok.getObject();
    isp_norm_cfg.color.hue = WcClampInt(ext.getInt("hue", 0), 0, 360);
    isp_norm_cfg.color.brightness = WcClampInt(ext.getInt("brightness", 0), -128, 127);
    isp_norm_cfg.color.present = true;
  }

  JsonParserToken aen_tok = sensor["aen"];
  if (aen_tok && aen_tok.isObject()) {
    JsonParserObject aen = aen_tok.getObject();

    JsonParserToken gamma_tok = aen["gamma"];
    if (gamma_tok && gamma_tok.isObject()) {
      JsonParserObject gamma_obj = gamma_tok.getObject();

      JsonParserToken gp_tok = gamma_obj["gamma_param"];
      if (gp_tok) {
        isp_norm_cfg.gamma.gamma_param = gamma_obj.getFloat("gamma_param", 1.0f);
        WcIspBuildGammaYFromParam(isp_norm_cfg.gamma.gamma_param, isp_norm_cfg.gamma.y);
        isp_norm_cfg.gamma.present = true;
      }
    }

    JsonParserToken con_tok = aen["contrast"];
    if (con_tok && con_tok.isArray()) {
      JsonParserArray con_arr = con_tok.getArray();
      int best_idx = WcIspFindNearest(con_arr, "gain", 1);
      if (best_idx >= 0 && con_arr[best_idx].isObject()) {
        JsonParserObject obj = con_arr[best_idx].getObject();
        isp_norm_cfg.color.contrast_0_255 = WcClampInt(obj.getInt("value", 255), 0, 255);
        isp_norm_cfg.color.present = true;
      }
    }

    JsonParserToken sat_legacy_tok = aen["saturation"];
    if (!saturation_found && sat_legacy_tok && sat_legacy_tok.isArray()) {
      JsonParserArray sat_arr = sat_legacy_tok.getArray();
      int best_idx = WcIspFindNearest(sat_arr, "gain", 1);
      if (best_idx >= 0 && sat_arr[best_idx].isObject()) {
        JsonParserObject obj = sat_arr[best_idx].getObject();
        isp_norm_cfg.color.saturation_0_255 = WcClampInt(obj.getInt("value", 255), 0, 255);
        isp_norm_cfg.color.present = true;
        saturation_found = true;
      }
    }
  }

  JsonParserToken acc_tok = sensor["acc"];
  if (acc_tok && acc_tok.isObject()) {
    JsonParserObject acc = acc_tok.getObject();

    JsonParserToken sat_tok = acc["saturation"];
    if (sat_tok && sat_tok.isArray()) {
      JsonParserArray sat_arr = sat_tok.getArray();
      int best_idx = WcIspFindNearest(sat_arr, "color_temp", 0);
      if (best_idx >= 0 && sat_arr[best_idx].isObject()) {
        JsonParserObject obj = sat_arr[best_idx].getObject();
        isp_norm_cfg.color.saturation_0_255 = WcClampInt(obj.getInt("value", 255), 0, 255);
        isp_norm_cfg.color.present = true;
        saturation_found = true;
      }
    }

  JsonParserToken adn_tok = sensor["adn"];
  if (adn_tok && adn_tok.isObject()) {
    JsonParserObject adn = adn_tok.getObject();

    JsonParserToken bf_tok = adn["bf"];
    if (bf_tok && bf_tok.isArray()) {
      JsonParserArray bf_arr = bf_tok.getArray();
      int count = 0;

      for (auto item_tok : bf_arr) {
        if (count >= 8) break;
        if (!item_tok.isObject()) continue;

        JsonParserObject item = item_tok.getObject();
        JsonParserToken param_tok = item["param"];
        if (!param_tok || !param_tok.isObject()) continue;

        JsonParserObject param = param_tok.getObject();

        isp_norm_cfg.bf.profile[count].gain =
            (uint16_t)WcClampInt(item.getInt("gain", 1), 1, 65535);
        isp_norm_cfg.bf.profile[count].level =
            WcClampU8(param.getInt("level", 5), 2, 20);

        JsonParserToken mat_tok = param["matrix"];
        if (!mat_tok || !mat_tok.isArray()) {
          continue;
        }

        JsonParserArray mat = mat_tok.getArray();
        int i = 0;
        for (auto v : mat) {
          if (i >= 9) break;
          isp_norm_cfg.bf.profile[count].matrix[i / 3][i % 3] =
              WcClampU8((int)v.getFloat(0), 0, 255);
          i++;
        }

        if (i == 9) {
          count++;
        }
      }

      if (count > 0) {
        isp_norm_cfg.bf.present = true;
        isp_norm_cfg.bf.count = count;
      }
    }
  }

    JsonParserToken ccm_tok = acc["ccm"];
    if (ccm_tok && ccm_tok.isObject()) {
      JsonParserObject ccm_obj = ccm_tok.getObject();

      JsonParserToken table_tok = ccm_obj["table"];
      if (table_tok && table_tok.isArray()) {
        JsonParserArray table = table_tok.getArray();
        int best_idx = WcIspFindNearest(table, "color_temp", 0);
        if (best_idx >= 0 && table[best_idx].isObject()) {
          JsonParserObject best = table[best_idx].getObject();
          if (WcIspParseMatrix9(best["matrix"], isp_norm_cfg.ccm.table)) {
            isp_norm_cfg.ccm.present = true;
          }
        }
      }
    }
  }

  JsonParserToken agc_tok = sensor["agc"];
  if (agc_tok && agc_tok.isObject()) {
    JsonParserObject agc = agc_tok.getObject();
    JsonParserToken luma_adj_tok = agc["luma_adjust"];
    if (luma_adj_tok && luma_adj_tok.isObject()) {
      JsonParserObject luma_adj = luma_adj_tok.getObject();

      isp_norm_cfg.agc.target = luma_adj.getInt("target", isp_norm_cfg.agc.target);
      isp_norm_cfg.agc.target_low = luma_adj.getInt("target_low", isp_norm_cfg.agc.target_low);
      isp_norm_cfg.agc.target_high = luma_adj.getInt("target_high", isp_norm_cfg.agc.target_high);

      if (WcIspParseWeight25(luma_adj["weight"], isp_norm_cfg.agc.weight)) {
        isp_norm_cfg.agc.weight_present = true;
        agc_weight_found = true;
      }

      isp_norm_cfg.agc.present = true;
    }
  }

  if (!agc_weight_found) {
    JsonParserToken ian_tok = sensor["ian"];
    if (ian_tok && ian_tok.isObject()) {
      JsonParserObject ian = ian_tok.getObject();
      JsonParserToken luma_tok = ian["luma"];
      if (luma_tok && luma_tok.isObject()) {
        JsonParserObject luma = luma_tok.getObject();
        JsonParserToken ae_tok = luma["ae"];
        if (ae_tok && ae_tok.isObject()) {
          JsonParserObject ae_obj = ae_tok.getObject();
          if (WcIspParseWeight25(ae_obj["weight"], isp_norm_cfg.agc.weight)) {
            isp_norm_cfg.agc.weight_present = true;
          }
        }
      }
    }
  }

  if (isp_norm_cfg.agc.target_low < 1) isp_norm_cfg.agc.target_low = 1;
  if (isp_norm_cfg.agc.target_low > 255) isp_norm_cfg.agc.target_low = 255;
  if (isp_norm_cfg.agc.target_high < 1) isp_norm_cfg.agc.target_high = 1;
  if (isp_norm_cfg.agc.target_high > 255) isp_norm_cfg.agc.target_high = 255;
  if (isp_norm_cfg.agc.target_high < isp_norm_cfg.agc.target_low) {
    isp_norm_cfg.agc.target_high = isp_norm_cfg.agc.target_low;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP norm cfg parsed (gamma=%d sat=%d agc_w=%d ccm=%d bf=%d)"),
         isp_norm_cfg.gamma.present ? 1 : 0,
         isp_norm_cfg.color.saturation_0_255,
         isp_norm_cfg.agc.weight_present ? 1 : 0,
         isp_norm_cfg.ccm.present ? 1 : 0,
         isp_norm_cfg.bf.count);
}

static void WcIspApplyRuntimeCcm() {
  isp_proc_handle_t isp = Wc.core.isp_handle;
  if (!isp) return;
  if (!isp_norm_cfg.ccm.present) return;

  bool gain_changed =
      (!isp_awb_state.runtime_ccm_valid) ||
      (fabsf(isp_awb_state.gain_r - isp_awb_state.last_applied_gain_r) >= 0.001f) ||
      (fabsf(isp_awb_state.gain_b - isp_awb_state.last_applied_gain_b) >= 0.001f);

  if (!gain_changed) {
    return;
  }

  esp_isp_ccm_config_t ccm_cfg;
  memset(&ccm_cfg, 0, sizeof(ccm_cfg));
  ccm_cfg.saturation = true;

  // base CCM + AWB trim
  for (int c = 0; c < 3; c++) {
    ccm_cfg.matrix[0][c] = isp_norm_cfg.ccm.table[0][c] * isp_awb_state.gain_r;
    ccm_cfg.matrix[1][c] = isp_norm_cfg.ccm.table[1][c];
    ccm_cfg.matrix[2][c] = isp_norm_cfg.ccm.table[2][c] * isp_awb_state.gain_b;
  }

  if (!WcIspCheckRet("ISP runtime CCM update", esp_isp_ccm_configure(isp, &ccm_cfg))) {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: CCM runtime gains R=%.3f B=%.3f"),
         isp_awb_state.gain_r,
         isp_awb_state.gain_b);

  isp_awb_state.runtime_ccm_valid = true;
  isp_awb_state.last_applied_gain_r = isp_awb_state.gain_r;
  isp_awb_state.last_applied_gain_b = isp_awb_state.gain_b;
}

static bool IRAM_ATTR WcIspAeOnStatisticsDone(isp_ae_ctlr_t ae_ctlr, const esp_isp_ae_env_detector_evt_data_t *edata, void *user_data) {
  if (!edata) {
    return false;
  }

  isp_ae_state.stats_seq++;
  isp_ae_state.latest_result = edata->ae_result;
  isp_ae_state.stats_seq++;
  isp_ae_state.stats_ready = true;
  isp_ae_state.cb_count++;

  return false;
}

static bool IRAM_ATTR WcIspAwbOnStatisticsDone(isp_awb_ctlr_t awb_ctlr, const esp_isp_awb_evt_data_t *edata, void *user_data) {
  if (!edata) {
    return false;
  }

  isp_awb_state.stats_seq++;
  isp_awb_state.latest_stats = edata->awb_result;
  isp_awb_state.stats_seq++;
  isp_awb_state.stats_ready = true;
  isp_awb_state.cb_count++;

  return false;
}
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
  if (file_size == 0 || file_size > 4095) {
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
  WcIspResetNormConfig();
  WcIspParseNormConfig(sensor);
  WcIspApplyDemosaic(handle, sensor);
  WcIspApplyGamma(handle, sensor);
  WcIspApplySharpen(handle, sensor);
  WcIspApplyColor(handle, sensor);
  WcIspApplyBF(handle, sensor);
  WcIspApplyCCM(handle, sensor);
  WcIspInitAWB(handle, sensor, width, height);
  WcIspInitAE(handle, sensor, width, height);
  
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
  (void)sensor;

  if (!isp_norm_cfg.ccm.present) {
    return;
  }

  esp_isp_ccm_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.saturation = true;

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      cfg.matrix[r][c] = isp_norm_cfg.ccm.table[r][c];
      isp_awb_state.base_ccm[r][c] = isp_norm_cfg.ccm.table[r][c];
    }
  }

  if (!WcIspCheckRet("ISP CCM configure", esp_isp_ccm_configure(handle, &cfg))) {
    return;
  }
  if (!WcIspCheckRet("ISP CCM enable", esp_isp_ccm_enable(handle))) {
    return;
  }

    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP CCM profile matrix applied"));
}

/*********************************************************************************************/

void WcIspApplyDemosaic(isp_proc_handle_t handle, JsonParserObject &sensor) {
  JsonParserToken adn_tok = sensor["adn"];
  if (!adn_tok || !adn_tok.isObject()) {
    return;
  }

  JsonParserObject adn = adn_tok.getObject();
  JsonParserToken dm_tok = adn["demosaic"];
  if (!dm_tok || !dm_tok.isArray()) {
    return;
  }

  JsonParserArray dm_arr = dm_tok.getArray();

  int best_idx = WcIspFindNearest(dm_arr, "gain", 1);
  if (best_idx < 0) {
    return;
  }

  JsonParserToken best_tok = dm_arr[best_idx];
  if (!best_tok.isObject()) {
    return;
  }

  JsonParserObject best = best_tok.getObject();
  float grad_ratio = WcClampFloat(best.getFloat("gradient_ratio", 1.25f), 0.0f, 15.996f);

  esp_isp_demosaic_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.grad_ratio.integer = (uint8_t)grad_ratio;
  cfg.grad_ratio.decimal = (uint8_t)((grad_ratio - (int)grad_ratio) * 256.0f);

  if (!WcIspCheckRet("ISP Demosaic configure", esp_isp_demosaic_configure(handle, &cfg))) {
    return;
  }
  if (!WcIspCheckRet("ISP Demosaic enable", esp_isp_demosaic_enable(handle))) {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Demosaic applied (gain=1, grad=%.3f)"), grad_ratio);
}
/*********************************************************************************************/

bool WcIspApplyBfProfile(isp_proc_handle_t handle, int idx) {
  if (!handle) return false;
  if (!isp_norm_cfg.bf.present) return false;
  if (idx < 0 || idx >= isp_norm_cfg.bf.count) return false;

  esp_isp_bf_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.denoising_level = isp_norm_cfg.bf.profile[idx].level;
  cfg.padding_mode = ISP_BF_EDGE_PADDING_MODE_SRND_DATA;

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      cfg.bf_template[r][c] = isp_norm_cfg.bf.profile[idx].matrix[r][c];
    }
  }

  if (!WcIspCheckRet("ISP BF configure", esp_isp_bf_configure(handle, &cfg))) {
    return false;
  }

  return true;
}

uint16_t WcIspGetCurrentTotalGainX1(void) {
  if (!isp_sensor_ae_state.valid) {
    return 1;
  }

  // total gain ~= (analog_gain / 16.0) * (digital_gain / 1024.0)
  uint32_t num = (uint32_t)isp_sensor_ae_state.analog_gain *
                 (uint32_t)isp_sensor_ae_state.digital_gain;

  uint32_t gain_x1 = (num + (16U * 1024U / 2U)) / (16U * 1024U);

  if (gain_x1 < 1U) gain_x1 = 1U;
  if (gain_x1 > 65535U) gain_x1 = 65535U;

  return (uint16_t)gain_x1;
}

int WcIspSelectRuntimeBfProfile(uint16_t gain_x1) {
  if (!isp_norm_cfg.bf.present || isp_norm_cfg.bf.count == 0) {
    return -1;
  }

  int idx = 0;
  for (int i = 0; i < isp_norm_cfg.bf.count; i++) {
    if (gain_x1 >= isp_norm_cfg.bf.profile[i].gain) {
      idx = i;
    } else {
      break;
    }
  }
  return idx;
}

void WcIspMirrorAeSeed(uint16_t vts, uint16_t exposure_lines, uint16_t analog_gain, uint16_t digital_gain) {
  isp_sensor_ae_state.valid = true;
  isp_sensor_ae_state.vts = vts;
  isp_sensor_ae_state.exposure_lines = exposure_lines;
  isp_sensor_ae_state.analog_gain = analog_gain;
  isp_sensor_ae_state.digital_gain = digital_gain;
}

static void WcIspMirrorAeApplyStep(int step) {
  if (!isp_sensor_ae_state.valid) return;
  if (step == 0) return;

  int exp_delta = 16;
  int again_delta = 4;
  int dgain_delta = 0x40;

  if (step == 2 || step == -2) {
    exp_delta = 48;
    again_delta = 8;
    dgain_delta = 0x80;
  } else if (step == 3 || step == -3) {
    exp_delta = 96;
    again_delta = 16;
    dgain_delta = 0x100;
  }

  uint16_t max_exp = (isp_sensor_ae_state.vts > 8) ? (isp_sensor_ae_state.vts - 8) : 4;
  if (max_exp < 4) max_exp = 4;

  if (step > 0) {
    if (isp_sensor_ae_state.exposure_lines < max_exp) {
      uint32_t v = isp_sensor_ae_state.exposure_lines + exp_delta;
      if (v > max_exp) v = max_exp;
      isp_sensor_ae_state.exposure_lines = (uint16_t)v;
    } else if (isp_sensor_ae_state.analog_gain < 0xF8) {
      uint32_t v = isp_sensor_ae_state.analog_gain + again_delta;
      if (v > 0xF8) v = 0xF8;
      isp_sensor_ae_state.analog_gain = (uint16_t)v;
    } else {
      uint32_t v = isp_sensor_ae_state.digital_gain + dgain_delta;
      if (v > 0x3FFF) v = 0x3FFF;
      isp_sensor_ae_state.digital_gain = (uint16_t)v;
    }
  } else {
    if (isp_sensor_ae_state.digital_gain > 0x0400) {
      int32_t v = (int32_t)isp_sensor_ae_state.digital_gain - dgain_delta;
      if (v < 0x0400) v = 0x0400;
      isp_sensor_ae_state.digital_gain = (uint16_t)v;
    } else if (isp_sensor_ae_state.analog_gain > 0x10) {
      int32_t v = (int32_t)isp_sensor_ae_state.analog_gain - again_delta;
      if (v < 0x10) v = 0x10;
      isp_sensor_ae_state.analog_gain = (uint16_t)v;
    } else {
      int32_t v = (int32_t)isp_sensor_ae_state.exposure_lines - exp_delta;
      if (v < 4) v = 4;
      isp_sensor_ae_state.exposure_lines = (uint16_t)v;
    }
  }
}

void WcIspApplyRuntimeBf(void) {
  if (!isp_bf_state.enabled) return;
  if (!isp_norm_cfg.bf.present) return;

  isp_proc_handle_t isp = Wc.core.isp_handle;
  if (!isp) return;

  uint16_t gain_x1 = WcIspGetCurrentTotalGainX1();
  int idx = WcIspSelectRuntimeBfProfile(gain_x1);
  if (idx < 0) return;

  if (idx == isp_bf_state.last_profile_idx) {
    return;
  }

  uint32_t now = millis();
  if ((now - isp_bf_state.last_apply_ms) < 120) {
    return;
  }

  if (!WcIspApplyBfProfile(isp, idx)) {
    return;
  }

  isp_bf_state.last_profile_idx = idx;
  isp_bf_state.last_apply_ms = now;

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Runtime BF -> idx=%d gain=%u level=%u"),
         idx,
         gain_x1,
         isp_norm_cfg.bf.profile[idx].level);
}

/*********************************************************************************************/

void WcIspStartAE(void) {
  if (!isp_ae_state.enabled || !isp_ae_state.handle) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AE start skipped - not ready"));
    return;
  }

  isp_ae_state.stats_ready = false;
  isp_ae_state.stats_seq = 0;
  isp_ae_state.cb_count = 0;
  isp_ae_state.last_consumed_seq = 0;
  isp_ae_state.last_luma = 0;
  memset(&isp_ae_state.latest_result, 0, sizeof(isp_ae_state.latest_result));

  esp_err_t ret = esp_isp_ae_controller_start_continuous_statistics(isp_ae_state.handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP AE continuous start failed (0x%x)"), ret);
    return;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ISP AE continuous started AFTER stream start"));
}

void WcIspInitAE(isp_proc_handle_t handle, JsonParserObject &sensor, int width, int height) {
  memset(isp_ae_state.weight, 0, sizeof(isp_ae_state.weight));
  for (int x = 0; x < ISP_AE_BLOCK_X_NUM; x++) {
    for (int y = 0; y < ISP_AE_BLOCK_Y_NUM; y++) {
      isp_ae_state.weight[x][y] = 1;
    }
  }

  isp_ae_state.target = 79;
  isp_ae_state.target_low = 62;
  isp_ae_state.target_high = 105;
  isp_ae_state.last_luma = 0;
  isp_ae_state.last_apply_ms = 0;
  isp_ae_state.stats_ready = false;
  isp_ae_state.stats_seq = 0;
  isp_ae_state.cb_count = 0;
  isp_ae_state.last_consumed_seq = 0;
  memset(&isp_ae_state.latest_result, 0, sizeof(isp_ae_state.latest_result));

  (void)sensor;

  isp_ae_state.target = isp_norm_cfg.agc.target;
  isp_ae_state.target_low = isp_norm_cfg.agc.target_low;
  isp_ae_state.target_high = isp_norm_cfg.agc.target_high;

  for (int x = 0; x < ISP_AE_BLOCK_X_NUM; x++) {
    for (int y = 0; y < ISP_AE_BLOCK_Y_NUM; y++) {
      isp_ae_state.weight[x][y] = isp_norm_cfg.agc.weight[x][y];
    }
  }

  if (isp_ae_state.target_low < 1) isp_ae_state.target_low = 1;
  if (isp_ae_state.target_low > 255) isp_ae_state.target_low = 255;
  if (isp_ae_state.target_high < 1) isp_ae_state.target_high = 1;
  if (isp_ae_state.target_high > 255) isp_ae_state.target_high = 255;
  if (isp_ae_state.target_high < isp_ae_state.target_low) {
    isp_ae_state.target_high = isp_ae_state.target_low;
  }

  if (isp_ae_state.handle) {
    esp_isp_ae_controller_stop_continuous_statistics(isp_ae_state.handle);
    esp_isp_ae_controller_disable(isp_ae_state.handle);
    esp_isp_del_ae_controller(isp_ae_state.handle);
    isp_ae_state.handle = NULL;
    isp_ae_state.enabled = false;
  }

  esp_isp_ae_config_t ae_cfg;
  memset(&ae_cfg, 0, sizeof(ae_cfg));
  ae_cfg.sample_point = ISP_AE_SAMPLE_POINT_AFTER_DEMOSAIC;

  ae_cfg.window.top_left.x = 0;
  ae_cfg.window.top_left.y = 0;
  ae_cfg.window.btm_right.x = width;
  ae_cfg.window.btm_right.y = height;

  isp_ae_ctlr_t ctlr = NULL;
  esp_err_t ret = esp_isp_new_ae_controller(handle, &ae_cfg, &ctlr);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP AE init failed (0x%x)"), ret);
    return;
  }

  esp_isp_ae_env_detector_evt_cbs_t ae_cbs = {
    .on_env_statistics_done = WcIspAeOnStatisticsDone,
    .on_env_change = NULL,
  };

  ret = esp_isp_ae_env_detector_register_event_callbacks(ctlr, &ae_cbs, NULL);
  if (ret != ESP_OK) {
    esp_isp_del_ae_controller(ctlr);
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP AE cb reg failed (0x%x)"), ret);
    return;
  }

  ret = esp_isp_ae_controller_enable(ctlr);
  if (ret != ESP_OK) {
    esp_isp_del_ae_controller(ctlr);
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP AE enable failed (0x%x)"), ret);
    return;
  }

  // AE only after stream on

  isp_ae_state.handle = ctlr;
  isp_ae_state.enabled = true;

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AE initialized (prepared, waiting for stream) target=%d low=%d high=%d win=%dx%d"),
         isp_ae_state.target, isp_ae_state.target_low, isp_ae_state.target_high, width, height);
}

void WcIspAeProcess(void) {

  if (!isp_ae_state.enabled || !isp_ae_state.handle) {
    return;
  }
  if (!isp_ae_state.stats_ready) {
    return;
  }

  uint32_t seq1 = isp_ae_state.stats_seq;
  if (seq1 & 1) {
    return;
  }

  isp_ae_result_t result = isp_ae_state.latest_result;

  uint32_t seq2 = isp_ae_state.stats_seq;
  if (seq1 != seq2 || (seq2 & 1)) {
    return;
  }

  if (seq2 == isp_ae_state.last_consumed_seq) {
    return;
  }
  isp_ae_state.last_consumed_seq = seq2;

  uint32_t weighted_sum = 0;
  uint32_t weight_sum = 0;

  for (int x = 0; x < ISP_AE_BLOCK_X_NUM; x++) {
    for (int y = 0; y < ISP_AE_BLOCK_Y_NUM; y++) {
      uint32_t w = isp_ae_state.weight[x][y];
      weighted_sum += (uint32_t)result.luminance[x][y] * w;
      weight_sum += w;
    }
  }

  if (weight_sum == 0) {
    return;
  }

  isp_ae_state.last_luma = weighted_sum / weight_sum;

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: AE cb=%u luma=%d target=%d band=%d..%d"),
         isp_ae_state.cb_count,
         isp_ae_state.last_luma,
         isp_ae_state.target,
         isp_ae_state.target_low,
         isp_ae_state.target_high);

  if (isp_ae_state.last_luma >= isp_ae_state.target_low &&
      isp_ae_state.last_luma <= isp_ae_state.target_high) {
    return;
  }

  uint32_t now = millis();
  if ((now - isp_ae_state.last_apply_ms) < 150) {
    return;
  }

  int err = isp_ae_state.target - isp_ae_state.last_luma;
  int step = 0;

  if (err > 32) {
    step = 3;
  } else if (err > 16) {
    step = 2;
  } else if (err > 4) {
    step = 1;
  } else if (err < -32) {
    step = -3;
  } else if (err < -16) {
    step = -2;
  } else if (err < -4) {
    step = -1;
  }

  if (step == 0) {
    return;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: AE step=%d"), step);

  int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("ae"), step, nullptr, 0);

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: AE dispatch result=%d"), berry_result);

  if (berry_result != 0) {
    WcIspMirrorAeApplyStep(step);
    isp_ae_state.last_apply_ms = now;
  }
}

void WcIspDeinitAE(void) {
  if (!isp_ae_state.handle) {
    isp_ae_state.enabled = false;
    isp_ae_state.stats_ready = false;
    isp_ae_state.stats_seq = 0;
    isp_ae_state.cb_count = 0;
    isp_ae_state.last_consumed_seq = 0;
    memset(&isp_ae_state.latest_result, 0, sizeof(isp_ae_state.latest_result));
    isp_ae_state.last_luma = 0;
    isp_ae_state.last_apply_ms = 0;
    return;
  }

  esp_isp_ae_controller_stop_continuous_statistics(isp_ae_state.handle);
  esp_isp_ae_controller_disable(isp_ae_state.handle);
  esp_isp_del_ae_controller(isp_ae_state.handle);

  isp_ae_state.handle = NULL;
  isp_ae_state.enabled = false;
  isp_ae_state.stats_ready = false;
  isp_ae_state.stats_seq = 0;
  isp_ae_state.cb_count = 0;
  isp_ae_state.last_consumed_seq = 0;
  memset(&isp_ae_state.latest_result, 0, sizeof(isp_ae_state.latest_result));
  isp_ae_state.last_luma = 0;
  isp_ae_state.last_apply_ms = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AE deinitialized"));
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
void WcIspApplyGamma(isp_proc_handle_t handle, JsonParserObject &sensor) {
  (void)sensor;

  if (!isp_norm_cfg.gamma.present) {
    return;
  }

  for (int i = 0; i < 16; i++) {
    Wc.core.isp_gamma_y[i] = isp_norm_cfg.gamma.y[i];
  }

  isp_gamma_curve_points_t pts = {};
  esp_isp_gamma_fill_curve_points(WcIspGammaInterp, &pts);

  if (!WcIspCheckRet("ISP Gamma configure R", esp_isp_gamma_configure(handle, COLOR_COMPONENT_R, &pts))) {
    return;
  }
  if (!WcIspCheckRet("ISP Gamma configure G", esp_isp_gamma_configure(handle, COLOR_COMPONENT_G, &pts))) {
    return;
  }
  if (!WcIspCheckRet("ISP Gamma configure B", esp_isp_gamma_configure(handle, COLOR_COMPONENT_B, &pts))) {
    return;
  }
  if (!WcIspCheckRet("ISP Gamma enable", esp_isp_gamma_enable(handle))) {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Gamma applied (gamma=%.3f y1=%d y15=%d)"),
         isp_norm_cfg.gamma.gamma_param,
         (int)Wc.core.isp_gamma_y[1],
         (int)Wc.core.isp_gamma_y[15]);
}
/*********************************************************************************************/

// Apply Sharpen Filter
// JSON: {"aen": {"sharpen": [{"gain": 1, "param": {"h_thresh":..., "l_thresh":..., "h_coeff":..., "m_coeff":..., "matrix":[...]}}]}}
void WcIspApplySharpen(isp_proc_handle_t handle, JsonParserObject &sensor) {
  JsonParserToken aen_tok = sensor["aen"];
  if (!aen_tok || !aen_tok.isObject()) return;

  JsonParserObject aen = aen_tok.getObject();
  JsonParserToken sh_tok = aen["sharpen"];
  if (!sh_tok || !sh_tok.isArray()) return;

  JsonParserArray sh_arr = sh_tok.getArray();
  int best_idx = WcIspFindNearest(sh_arr, "gain", 1);
  if (best_idx < 0) return;

  JsonParserToken best_tok = sh_arr[best_idx];
  if (!best_tok.isObject()) return;

  JsonParserObject best = best_tok.getObject();
  JsonParserToken param_tok = best["param"];
  if (!param_tok || !param_tok.isObject()) return;

  JsonParserObject param = param_tok.getObject();

  esp_isp_sharpen_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.h_thresh = WcClampU8(param.getInt("h_thresh", 255), 0, 255);
  cfg.l_thresh = WcClampU8(param.getInt("l_thresh", 128), 0, 255);

  float h_coeff = WcClampFloat(param.getFloat("h_coeff", 1.0f), 0.0f, 15.996f);
  cfg.h_freq_coeff.integer = (uint8_t)h_coeff;
  cfg.h_freq_coeff.decimal = (uint8_t)((h_coeff - (int)h_coeff) * 256.0f);

  float m_coeff = WcClampFloat(param.getFloat("m_coeff", 1.0f), 0.0f, 15.996f);
  cfg.m_freq_coeff.integer = (uint8_t)m_coeff;
  cfg.m_freq_coeff.decimal = (uint8_t)((m_coeff - (int)m_coeff) * 256.0f);

  JsonParserToken mat_tok = param["matrix"];
  if (mat_tok && mat_tok.isArray()) {
    JsonParserArray mat = mat_tok.getArray();
    int i = 0;
    for (auto v : mat) {
      if (i >= 9) break;
      cfg.sharpen_template[i / 3][i % 3] = WcClampU8((int)v.getFloat(0), 0, 255);
      i++;
    }
  }

  cfg.padding_mode = ISP_SHARPEN_EDGE_PADDING_MODE_SRND_DATA;

  if (!WcIspCheckRet("ISP Sharpen configure", esp_isp_sharpen_configure(handle, &cfg))) {
    return;
  }
  if (!WcIspCheckRet("ISP Sharpen enable", esp_isp_sharpen_enable(handle))) {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Sharpen applied (gain=1)"));
}

/*********************************************************************************************/

template<typename T>
static void WcIspSetFixed01(uint8_t src_0_255, T *dst) {
  uint32_t q7 = ((uint32_t)src_0_255 * 128U + 127U) / 255U;  // 0..128
  if (q7 > 128U) q7 = 128U;

  if (q7 >= 128U) {
    dst->integer = 1;
    dst->decimal = 0;
  } else {
    dst->integer = 0;
    dst->decimal = q7;   // 0..127
  }
}

// Apply Color Adjustment
// JSON: {"ext": {"hue":0, "brightness":0}, "aen": {"contrast": [{"gain":1, "value":128}]}}
void WcIspApplyColor(isp_proc_handle_t handle, JsonParserObject &sensor) {
  (void)sensor;

  esp_isp_color_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.color_contrast.integer = 1;
  cfg.color_contrast.decimal = 0;
  cfg.color_saturation.integer = 1;
  cfg.color_saturation.decimal = 0;
  cfg.color_hue = 0;
  cfg.color_brightness = 0;

  WcIspSetFixed01((uint8_t)WcClampInt(isp_norm_cfg.color.contrast_0_255, 0, 255), &cfg.color_contrast);
  WcIspSetFixed01((uint8_t)WcClampInt(isp_norm_cfg.color.saturation_0_255, 0, 255), &cfg.color_saturation);

  cfg.color_hue = WcClampInt(isp_norm_cfg.color.hue, 0, 360);
  cfg.color_brightness = WcClampInt(isp_norm_cfg.color.brightness, -128, 127);

  if (!WcIspCheckRet("ISP Color configure", esp_isp_color_configure(handle, &cfg))) {
    return;
  }
  if (!WcIspCheckRet("ISP Color enable", esp_isp_color_enable(handle))) {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP Color applied (contrast=%d saturation=%d hue=%d brightness=%d)"),
         isp_norm_cfg.color.contrast_0_255,
         isp_norm_cfg.color.saturation_0_255,
         isp_norm_cfg.color.hue,
         isp_norm_cfg.color.brightness);
}

/*********************************************************************************************/

// Apply Bilateral Filter (Denoising)
// JSON: {"adn": {"bf": [{"gain":1, "param": {"level":2, "matrix":[flat 9]}}]}}
void WcIspApplyBF(isp_proc_handle_t handle, JsonParserObject &sensor) {
  (void)sensor;

  if (!isp_norm_cfg.bf.present || isp_norm_cfg.bf.count == 0) {
    isp_bf_state.enabled = false;
    isp_bf_state.last_profile_idx = -1;
    isp_bf_state.last_apply_ms = 0;
    return;
  }

  if (!WcIspApplyBfProfile(handle, 0)) {
    isp_bf_state.enabled = false;
    isp_bf_state.last_profile_idx = -1;
    isp_bf_state.last_apply_ms = 0;
    return;
  }

  if (!WcIspCheckRet("ISP BF enable", esp_isp_bf_enable(handle))) {
    isp_bf_state.enabled = false;
    isp_bf_state.last_profile_idx = -1;
    isp_bf_state.last_apply_ms = 0;
    return;
  }

  isp_bf_state.enabled = true;
  isp_bf_state.last_profile_idx = 0;
  isp_bf_state.last_apply_ms = millis();

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP BF applied (init profile gain=%u level=%u)"),
         isp_norm_cfg.bf.profile[0].gain,
         isp_norm_cfg.bf.profile[0].level);
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

  JsonParserToken range_tok = awb["range"];
  if (!range_tok || !range_tok.isObject()) return;
  JsonParserObject range = range_tok.getObject();

  JsonParserToken green_tok = range["green"];
  JsonParserToken rg_tok = range["rg"];
  JsonParserToken bg_tok = range["bg"];
  if (!green_tok || !green_tok.isObject() || !rg_tok || !rg_tok.isObject() || !bg_tok || !bg_tok.isObject()) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: ISP AWB skipped - incomplete range object"));
    return;
  }

  JsonParserObject green_obj = green_tok.getObject();
  JsonParserObject rg_obj = rg_tok.getObject();
  JsonParserObject bg_obj = bg_tok.getObject();

  int lum_min = WcClampInt(green_obj.getInt("min", 128), 0, 255);
  int lum_max = WcClampInt(green_obj.getInt("max", 240), 0, 255);
  if (lum_max < lum_min) lum_max = lum_min;

  float rg_min = WcClampFloat(rg_obj.getFloat("min", 0.7f), 0.0f, 8.0f);
  float rg_max = WcClampFloat(rg_obj.getFloat("max", 1.3f), 0.0f, 8.0f);
  float bg_min = WcClampFloat(bg_obj.getFloat("min", 0.7f), 0.0f, 8.0f);
  float bg_max = WcClampFloat(bg_obj.getFloat("max", 1.3f), 0.0f, 8.0f);

  int x1 = width / 10;
  int y1 = height / 10;
  int x2 = width - width / 10;
  int y2 = height - height / 10;

  int sp = awb.getInt("sample_point", (int)ISP_AWB_SAMPLE_POINT_AFTER_CCM);
  if (sp != (int)ISP_AWB_SAMPLE_POINT_BEFORE_CCM &&
      sp != (int)ISP_AWB_SAMPLE_POINT_AFTER_CCM) {
    sp = (int)ISP_AWB_SAMPLE_POINT_AFTER_CCM;
  }

  esp_isp_awb_config_t awb_cfg;
  memset(&awb_cfg, 0, sizeof(awb_cfg));
  awb_cfg.sample_point = (isp_awb_sample_point_t)sp;
  awb_cfg.window.top_left.x = x1;
  awb_cfg.window.top_left.y = y1;
  awb_cfg.window.btm_right.x = x2;
  awb_cfg.window.btm_right.y = y2;
  awb_cfg.subwindow = awb_cfg.window;
  awb_cfg.white_patch.luminance.min = (uint32_t)lum_min;
  awb_cfg.white_patch.luminance.max = (uint32_t)lum_max;
  awb_cfg.white_patch.red_green_ratio.min = rg_min;
  awb_cfg.white_patch.red_green_ratio.max = rg_max;
  awb_cfg.white_patch.blue_green_ratio.min = bg_min;
  awb_cfg.white_patch.blue_green_ratio.max = bg_max;

  isp_awb_ctlr_t ctlr = NULL;
  esp_err_t ret = esp_isp_new_awb_controller(handle, &awb_cfg, &ctlr);
  if (!WcIspCheckRet("ISP AWB init", ret)) {
    return;
  }

  esp_isp_awb_cbs_t awb_cbs = {
    .on_statistics_done = WcIspAwbOnStatisticsDone,
  };

  ret = esp_isp_awb_register_event_callbacks(ctlr, &awb_cbs, NULL);
  if (!WcIspCheckRet("ISP AWB cb reg", ret)) {
    esp_isp_del_awb_controller(ctlr);
    return;
  }

  ret = esp_isp_awb_controller_enable(ctlr);
  if (!WcIspCheckRet("ISP AWB enable", ret)) {
    esp_isp_del_awb_controller(ctlr);
    return;
  }

  isp_awb_state.handle = ctlr;
  isp_awb_state.enabled = true;
  isp_awb_state.gain_r = 1.0f;
  isp_awb_state.gain_b = 1.0f;
  isp_awb_state.last_applied_gain_r = 1.0f;
  isp_awb_state.last_applied_gain_b = 1.0f;
  isp_awb_state.runtime_ccm_valid = false;
  isp_awb_state.min_counted = (uint32_t)WcClampInt(awb.getInt("min_counted", 2000), 1, 1000000);
  isp_awb_state.min_r_step = WcClampFloat(awb.getFloat("min_red_gain_step", 0.034f), 0.0f, 1.0f);
  isp_awb_state.min_b_step = WcClampFloat(awb.getFloat("min_blue_gain_step", 0.034f), 0.0f, 1.0f);

  isp_awb_state.stats_ready = false;
  isp_awb_state.stats_seq = 0;
  isp_awb_state.cb_count = 0;
  isp_awb_state.last_consumed_seq = 0;
  memset(&isp_awb_state.latest_stats, 0, sizeof(isp_awb_state.latest_stats));

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AWB initialized"));
}

/*********************************************************************************************/

// AWB Processing Loop - called from xdrv_81 FUNC_EVERY_250_MSECOND (4Hz)
// Grey-world algorithm: gain_r = avg_G/avg_R, gain_b = avg_G/avg_B
// Applied as diagonal correction on base CCM
void WcIspStartAWB(void) {
  if (!isp_awb_state.enabled || !isp_awb_state.handle) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AWB start skipped - not ready"));
    return;
  }

  isp_awb_state.stats_ready = false;
  isp_awb_state.stats_seq = 0;
  isp_awb_state.cb_count = 0;
  isp_awb_state.last_consumed_seq = 0;
  memset(&isp_awb_state.latest_stats, 0, sizeof(isp_awb_state.latest_stats));

  esp_err_t ret = esp_isp_awb_controller_start_continuous_statistics(isp_awb_state.handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP AWB continuous start failed (0x%x)"), ret);
    return;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ISP AWB continuous started AFTER stream start"));
}


void WcIspAwbProcess(void) {
  if (!isp_awb_state.enabled || !isp_awb_state.handle) return;

  uint32_t seq1 = isp_awb_state.stats_seq;
  if (seq1 & 1) {
    return;
  }

  isp_awb_stat_result_t stats = isp_awb_state.latest_stats;

  uint32_t seq2 = isp_awb_state.stats_seq;
  if (seq1 != seq2 || (seq2 & 1)) {
    return;
  }

  if (seq2 == isp_awb_state.last_consumed_seq) {
    return;
  }
  isp_awb_state.last_consumed_seq = seq2;

  if (stats.white_patch_num < isp_awb_state.min_counted) {
    return;
  }

  float avg_r = (float)stats.sum_r / (float)stats.white_patch_num;
  float avg_g = (float)stats.sum_g / (float)stats.white_patch_num;
  float avg_b = (float)stats.sum_b / (float)stats.white_patch_num;

  if (avg_g < 8.0f) return;
  if (avg_r < 1.0f) avg_r = 1.0f;
  if (avg_b < 1.0f) avg_b = 1.0f;

  // AFTER_CCM => avg_g/avg_r a avg_g/avg_b residuals, not absolute gains

  float residual_r = avg_g / avg_r;
  float residual_b = avg_g / avg_b;

  // limit step correction
  residual_r = WcClampFloat(residual_r, 0.75f, 1.33f);
  residual_b = WcClampFloat(residual_b, 0.75f, 1.33f);

  float target_gain_r = isp_awb_state.gain_r * residual_r;
  float target_gain_b = isp_awb_state.gain_b * residual_b;

  target_gain_r = WcClampFloat(target_gain_r, 0.5f, 2.0f);
  target_gain_b = WcClampFloat(target_gain_b, 0.5f, 2.0f);

  const float alpha = 0.35f;

  float new_gain_r = isp_awb_state.gain_r + (target_gain_r - isp_awb_state.gain_r) * alpha;
  float new_gain_b = isp_awb_state.gain_b + (target_gain_b - isp_awb_state.gain_b) * alpha;

  if (fabsf(new_gain_r - isp_awb_state.gain_r) < isp_awb_state.min_r_step &&
      fabsf(new_gain_b - isp_awb_state.gain_b) < isp_awb_state.min_b_step) {
    return;
  }

  isp_awb_state.gain_r = new_gain_r;
  isp_awb_state.gain_b = new_gain_b;

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: AWB residual R=%.3f B=%.3f -> gain R=%.3f B=%.3f"),
         residual_r, residual_b,
         isp_awb_state.gain_r, isp_awb_state.gain_b);
}


void WcIspAutoProcess(void) {
  if (Wc.core.state != CAM_STREAMING) {
    return;
  }

  WcIspAeProcess();
  WcIspApplyRuntimeBf();
  WcIspAwbProcess();
  WcIspApplyRuntimeCcm();
}
/*********************************************************************************************/

// Deinitialize AWB - called when streaming stops
void WcIspDeinitAWB(void) {
  WcIspDeinitAE();

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

  esp_isp_awb_controller_stop_continuous_statistics(isp_awb_state.handle);
  esp_isp_awb_controller_disable(isp_awb_state.handle);
  esp_isp_del_awb_controller(isp_awb_state.handle);
  isp_awb_state.handle = NULL;
  isp_awb_state.gain_r = 1.0f;
  isp_awb_state.gain_b = 1.0f;
  isp_awb_state.last_applied_gain_r = 1.0f;
  isp_awb_state.last_applied_gain_b = 1.0f;
  isp_awb_state.runtime_ccm_valid = false;
  isp_awb_state.stats_ready = false;
  isp_awb_state.stats_seq = 0;
  isp_awb_state.cb_count = 0;
  isp_awb_state.last_consumed_seq = 0;

  isp_bf_state.enabled = false;
  isp_bf_state.last_profile_idx = -1;
  isp_bf_state.last_apply_ms = 0;

  isp_sensor_ae_state.valid = false;
  isp_sensor_ae_state.vts = 0;
  isp_sensor_ae_state.exposure_lines = 0;
  isp_sensor_ae_state.analog_gain = 0x10;
  isp_sensor_ae_state.digital_gain = 0x0400;

  memset(&isp_awb_state.latest_stats, 0, sizeof(isp_awb_state.latest_stats));
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ISP AWB deinitialized"));
}

#endif // ESP_IDF_VERSION >= 5.5.0
#endif // USE_CSI_WEBCAM
#endif // ESP32
