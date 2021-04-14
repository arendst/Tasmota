#include "lut.h"
#include "display_ops.h"
#include "esp_log.h"
#include "freertos/task.h"
#include <string.h>

/*
 * Build Lookup tables and translate via LUTs.
 * WARNING: These functions must only ever write to internal memory,
 *          Since we disable the PSRAM workaround here for performance reasons.
 */

/* Python script for generating the 1bpp lookup table:
 * for i in range(256):
     number = 0;
     for b in range(8):
         if not (i & (b << 1)):
             number |= 1 << (2*b)
     print ('0x%04x,'%number)
 */
const static uint32_t lut_1bpp_black[256] = {
    0x5555, 0x5554, 0x5551, 0x5550, 0x5545, 0x5544, 0x5541, 0x5540, 0x5515,
    0x5514, 0x5511, 0x5510, 0x5505, 0x5504, 0x5501, 0x5500, 0x5455, 0x5454,
    0x5451, 0x5450, 0x5445, 0x5444, 0x5441, 0x5440, 0x5415, 0x5414, 0x5411,
    0x5410, 0x5405, 0x5404, 0x5401, 0x5400, 0x5155, 0x5154, 0x5151, 0x5150,
    0x5145, 0x5144, 0x5141, 0x5140, 0x5115, 0x5114, 0x5111, 0x5110, 0x5105,
    0x5104, 0x5101, 0x5100, 0x5055, 0x5054, 0x5051, 0x5050, 0x5045, 0x5044,
    0x5041, 0x5040, 0x5015, 0x5014, 0x5011, 0x5010, 0x5005, 0x5004, 0x5001,
    0x5000, 0x4555, 0x4554, 0x4551, 0x4550, 0x4545, 0x4544, 0x4541, 0x4540,
    0x4515, 0x4514, 0x4511, 0x4510, 0x4505, 0x4504, 0x4501, 0x4500, 0x4455,
    0x4454, 0x4451, 0x4450, 0x4445, 0x4444, 0x4441, 0x4440, 0x4415, 0x4414,
    0x4411, 0x4410, 0x4405, 0x4404, 0x4401, 0x4400, 0x4155, 0x4154, 0x4151,
    0x4150, 0x4145, 0x4144, 0x4141, 0x4140, 0x4115, 0x4114, 0x4111, 0x4110,
    0x4105, 0x4104, 0x4101, 0x4100, 0x4055, 0x4054, 0x4051, 0x4050, 0x4045,
    0x4044, 0x4041, 0x4040, 0x4015, 0x4014, 0x4011, 0x4010, 0x4005, 0x4004,
    0x4001, 0x4000, 0x1555, 0x1554, 0x1551, 0x1550, 0x1545, 0x1544, 0x1541,
    0x1540, 0x1515, 0x1514, 0x1511, 0x1510, 0x1505, 0x1504, 0x1501, 0x1500,
    0x1455, 0x1454, 0x1451, 0x1450, 0x1445, 0x1444, 0x1441, 0x1440, 0x1415,
    0x1414, 0x1411, 0x1410, 0x1405, 0x1404, 0x1401, 0x1400, 0x1155, 0x1154,
    0x1151, 0x1150, 0x1145, 0x1144, 0x1141, 0x1140, 0x1115, 0x1114, 0x1111,
    0x1110, 0x1105, 0x1104, 0x1101, 0x1100, 0x1055, 0x1054, 0x1051, 0x1050,
    0x1045, 0x1044, 0x1041, 0x1040, 0x1015, 0x1014, 0x1011, 0x1010, 0x1005,
    0x1004, 0x1001, 0x1000, 0x0555, 0x0554, 0x0551, 0x0550, 0x0545, 0x0544,
    0x0541, 0x0540, 0x0515, 0x0514, 0x0511, 0x0510, 0x0505, 0x0504, 0x0501,
    0x0500, 0x0455, 0x0454, 0x0451, 0x0450, 0x0445, 0x0444, 0x0441, 0x0440,
    0x0415, 0x0414, 0x0411, 0x0410, 0x0405, 0x0404, 0x0401, 0x0400, 0x0155,
    0x0154, 0x0151, 0x0150, 0x0145, 0x0144, 0x0141, 0x0140, 0x0115, 0x0114,
    0x0111, 0x0110, 0x0105, 0x0104, 0x0101, 0x0100, 0x0055, 0x0054, 0x0051,
    0x0050, 0x0045, 0x0044, 0x0041, 0x0040, 0x0015, 0x0014, 0x0011, 0x0010,
    0x0005, 0x0004, 0x0001, 0x0000};

// Timestamp when the last frame draw was started.
// This is used to enforce a minimum frame draw time, allowing
// all pixels to set.
static uint64_t last_frame_start = 0;

inline int min(int x, int y) { return x < y ? x : y; }
inline int max(int x, int y) { return x > y ? x : y; }

// status tracker for row skipping
uint32_t skipping;

// output a row to the display.
void IRAM_ATTR write_row(uint32_t output_time_dus) {
  epd_output_row(output_time_dus);
  skipping = 0;
}

// skip a display row
void IRAM_ATTR skip_row(uint8_t pipeline_finish_time) {
  // output previously loaded row, fill buffer with no-ops.
  if (skipping < 2) {
    memset(epd_get_current_buffer(), 0x00, EPD_LINE_BYTES);
    epd_output_row(pipeline_finish_time);
  } else {
    epd_skip();
  }
  skipping++;
}

void IRAM_ATTR reorder_line_buffer(uint32_t *line_data) {
  for (uint32_t i = 0; i < EPD_LINE_BYTES / 4; i++) {
    uint32_t val = *line_data;
    *(line_data++) = val >> 16 | ((val & 0x0000FFFF) << 16);
  }
}

static void IRAM_ATTR bit_shift_buffer_right(uint8_t *buf, uint32_t len,
                                             int shift) {
  uint8_t carry = 0xFF << (8 - shift);
  for (uint32_t i = 0; i < len; i++) {
    uint8_t val = buf[i];
    buf[i] = (val >> shift) | carry;
    carry = val << (8 - shift);
  }
}

static void IRAM_ATTR nibble_shift_buffer_right(uint8_t *buf, uint32_t len) {
  uint8_t carry = 0xF;
  for (uint32_t i = 0; i < len; i++) {
    uint8_t val = buf[i];
    buf[i] = (val << 4) | carry;
    carry = (val & 0xF0) >> 4;
  }
}

///////////////////////////// Looking up EPD Pixels
//////////////////////////////////

static void IRAM_ATTR calc_epd_input_1bpp(const uint32_t *line_data,
                                          uint8_t *epd_input,
                                          const uint8_t *lut) {

  uint32_t *wide_epd_input = (uint32_t *)epd_input;
  uint8_t *data_ptr = (uint8_t *)line_data;
  uint32_t *lut_32 = (uint32_t *)lut;
  // this is reversed for little-endian, but this is later compensated
  // through the output peripheral.
  for (uint32_t j = 0; j < EPD_WIDTH / 16; j++) {
    uint8_t v1 = *(data_ptr++);
    uint8_t v2 = *(data_ptr++);
    wide_epd_input[j] = (lut_32[v1] << 16) | lut_32[v2];
  }
}

static void IRAM_ATTR
calc_epd_input_4bpp_lut_64k(const uint32_t *line_data, uint8_t *epd_input,
                            const uint8_t *conversion_lut) {

  uint32_t *wide_epd_input = (uint32_t *)epd_input;
  const uint16_t *line_data_16 = (const uint16_t *)line_data;

  // this is reversed for little-endian, but this is later compensated
  // through the output peripheral.
  for (uint32_t j = 0; j < EPD_WIDTH / 16; j++) {

    uint16_t v1 = *(line_data_16++);
    uint16_t v2 = *(line_data_16++);
    uint16_t v3 = *(line_data_16++);
    uint16_t v4 = *(line_data_16++);
    uint32_t pixel = conversion_lut[v1] << 16 | conversion_lut[v2] << 24 |
                     conversion_lut[v3] | conversion_lut[v4] << 8;
    wide_epd_input[j] = pixel;
  }
}

/**
 * Look up 4 pixels of a differential image.
 */
static inline uint8_t
lookup_differential_pixels(uint32_t in, const uint8_t *conversion_lut) {
  uint8_t out = conversion_lut[in & 0xFF];
  in = in >> 8;
  out |= (conversion_lut + 0x100)[in & 0xFF];
  in = in >> 8;
  out |= (conversion_lut + 0x200)[in & 0xFF];
  in = in >> 8;
  out |= (conversion_lut + 0x300)[in];
  return out;
}

/**
 * Calculate EPD input for a difference image with one pixel per byte.
 */
static void IRAM_ATTR calc_epd_input_1ppB(const uint32_t *ld,
                                          uint8_t *epd_input,
                                          const uint8_t *conversion_lut) {

  // this is reversed for little-endian, but this is later compensated
  // through the output peripheral.
  for (uint32_t j = 0; j < EPD_WIDTH / 4; j += 4) {
    epd_input[j + 2] = lookup_differential_pixels(*(ld++), conversion_lut);
    epd_input[j + 3] = lookup_differential_pixels(*(ld++), conversion_lut);
    epd_input[j + 0] = lookup_differential_pixels(*(ld++), conversion_lut);
    epd_input[j + 1] = lookup_differential_pixels(*(ld++), conversion_lut);
  }
}

/**
 * Look up 4 pixels in a 1K LUT with fixed "from" value.
 */
static inline uint8_t lookup_pixels_4bpp_1k(uint16_t in,
                                            const uint8_t *conversion_lut,
                                            uint8_t from) {
  uint8_t v;
  uint8_t out;
  v = ((in << 4) | from);
  out = conversion_lut[v & 0xFF];
  v = ((in & 0xF0) | from);
  out |= (conversion_lut + 0x100)[v & 0xFF];
  in = in >> 8;
  v = ((in << 4) | from);
  out |= (conversion_lut + 0x200)[v & 0xFF];
  v = ((in & 0xF0) | from);
  out |= (conversion_lut + 0x300)[v];
  return out;
}

/**
 * Calculate EPD input for a 4bpp buffer, but with a difference image LUT.
 * This is used for small-LUT mode.
 */
static void IRAM_ATTR calc_epd_input_4bpp_1k_lut(const uint32_t *ld,
                                                 uint8_t *epd_input,
                                                 const uint8_t *conversion_lut,
                                                 uint8_t from) {

  uint16_t *ptr = (uint16_t *)ld;
  // this is reversed for little-endian, but this is later compensated
  // through the output peripheral.
  for (uint32_t j = 0; j < EPD_WIDTH / 4; j += 4) {
    epd_input[j + 2] = lookup_pixels_4bpp_1k(*(ptr++), conversion_lut, from);
    epd_input[j + 3] = lookup_pixels_4bpp_1k(*(ptr++), conversion_lut, from);
    epd_input[j + 0] = lookup_pixels_4bpp_1k(*(ptr++), conversion_lut, from);
    epd_input[j + 1] = lookup_pixels_4bpp_1k(*(ptr++), conversion_lut, from);
  }
}

static void IRAM_ATTR calc_epd_input_4bpp_1k_lut_white(
    const uint32_t *ld, uint8_t *epd_input, const uint8_t *conversion_lut) {
  calc_epd_input_4bpp_1k_lut(ld, epd_input, conversion_lut, 0xF);
}

static void IRAM_ATTR calc_epd_input_4bpp_1k_lut_black(
    const uint32_t *ld, uint8_t *epd_input, const uint8_t *conversion_lut) {
  calc_epd_input_4bpp_1k_lut(ld, epd_input, conversion_lut, 0x0);
}

///////////////////////////// Calculate Lookup Tables
//////////////////////////////////

/**
 * Unpack the waveform data into a lookup table, with bit shifted copies.
 */
static void IRAM_ATTR waveform_lut(const EpdWaveform *waveform, uint8_t *lut,
                                   uint8_t mode, int range, int frame) {
  const uint8_t *p_lut =
      waveform->mode_data[mode]->range_data[range]->luts + (16 * 4 * frame);
  for (uint8_t to = 0; to < 16; to++) {
    for (uint8_t from_packed = 0; from_packed < 4; from_packed++) {
      uint8_t index = (to << 4) | (from_packed * 4);
      uint8_t packed = *(p_lut++);
      lut[index] = (packed >> 6) & 3;
      lut[index + 1] = (packed >> 4) & 3;
      lut[index + 2] = (packed >> 2) & 3;
      lut[index + 3] = (packed >> 0) & 3;
      // printf("%2X%2X%2X%2X (%d)", lut[index], lut[index + 1], lut[index + 2],
      // lut[index + 3], index);
    }
    // printf("\n");
  }
  uint32_t index = 0x100;
  for (uint8_t s = 2; s <= 6; s += 2) {
    for (int i = 0; i < 0x100; i++) {
      lut[index] = lut[index % 0x100] << s;
      index++;
    }
  }
}

/**
 * Build a 16-bit LUT from the waveform if the previous color is
 * known, e.g. all white or all black.
 * This LUT is use to look up 4 pixels at once, as with the epdiy LUT.
 */
static void IRAM_ATTR waveform_lut_static_from(const EpdWaveform *waveform,
                                               uint8_t *lut, uint8_t from,
                                               uint8_t mode, int range,
                                               int frame) {
  const uint8_t *p_lut =
      waveform->mode_data[mode]->range_data[range]->luts + (16 * 4 * frame);

  /// index into the packed "from" row
  uint8_t fi = from >> 2;
  /// bit shift amount for the packed "from" row
  uint8_t fs = 6 - 2 * (from & 3);

  // populate the first 4096 bytes
  uint8_t v1 = 0;
  uint32_t s1 = 0;
  for (uint8_t t2 = 0; t2 < 16; t2++) {
    uint8_t v2 = ((p_lut[(t2 << 2) + fi] >> fs) & 0x03) << 4;
    uint32_t s2 = t2 << 8;
    for (uint8_t t3 = 0; t3 < 16; t3++) {
      uint8_t v3 = ((p_lut[(t3 << 2) + fi] >> fs) & 0x03) << 2;
      uint32_t s3 = t3 << 4;
      for (uint8_t t4 = 0; t4 < 16; t4++) {
        uint8_t v4 = ((p_lut[(t4 << 2) + fi] >> fs) & 0x03) << 0;
        uint32_t s4 = t4;
        lut[s1 | s2 | s3 | s4] = v1 | v2 | v3 | v4;
      }
    }
  }

  // now just copy and the first 4096 bytes and add the upper two bits
  for (uint8_t t1 = 1; t1 < 16; t1++) {
    memcpy(&lut[t1 << 12], lut, 1 << 12);
  }

  for (int i = 0; i < 16; i++) {
    uint32_t v1 = ((p_lut[(i << 2) + fi] >> fs) & 0x03);
    uint32_t mask = (v1 << 30) | (v1 << 22) | (v1 << 14) | (v1 << 6);
    for (int j = 0; j < 16 * 16 * 16 / 4; j++) {
      ((uint32_t *)lut)[(i << 10) + j] |= mask;
    }
  }
}

void IRAM_ATTR provide_out(OutputParams *params) {
  while (true) {
    // line must be able to hold 2-pixel-per-byte or 1-pixel-per-byte data
    uint8_t line[EPD_WIDTH];
    memset(line, 255, EPD_WIDTH);

    xSemaphoreTake(params->start_smphr, portMAX_DELAY);
    EpdRect area = params->area;
    const uint8_t *ptr = params->data_ptr;
    const bool crop = (params->crop_to.width > 0 && params->crop_to.height > 0);

    // number of pixels per byte of input data
    int ppB = 0;
    int bytes_per_line = 0;
    int width_divider = 0;

    if (params->mode & MODE_PACKING_1PPB_DIFFERENCE) {
      ppB = 1;
      bytes_per_line = area.width;
      width_divider = 1;
    } else if (params->mode & MODE_PACKING_2PPB) {
      ppB = 2;
      bytes_per_line = area.width / 2 + area.width % 2;
      width_divider = 2;
    } else if (params->mode & MODE_PACKING_8PPB) {
      ppB = 8;
      bytes_per_line = (area.width / 8 + (area.width % 8 > 0));
      width_divider = 8;
    } else {
      params->error |= EPD_DRAW_INVALID_PACKING_MODE;
    }

    int crop_x = (crop ? params->crop_to.x : 0);
    int crop_y = (crop ? params->crop_to.y : 0);
    int crop_w = (crop ? params->crop_to.width : 0);
    int crop_h = (crop ? params->crop_to.height : 0);

    // Adjust for negative starting coordinates with optional crop
    if (area.x - crop_x < 0) {
      ptr += -(area.x - crop_x) / width_divider;
    }

    if (area.y - crop_y < 0) {
      ptr += -(area.y - crop_y) * bytes_per_line;
    }

    // calculate start and end row with crop
    int min_y = area.y + crop_y;
    int max_y = min(min_y + (crop ? crop_h : area.height), area.height);
    for (int i = 0; i < EPD_HEIGHT; i++) {
      if (i < min_y || i >= max_y) {
        continue;
      }
      if (params->drawn_lines != NULL && !params->drawn_lines[i - area.y]) {
        ptr += bytes_per_line;
        continue;
      }

      uint32_t *lp = (uint32_t *)line;
      bool shifted = false;
      if (area.width == EPD_WIDTH && area.x == 0 && !crop && !params->error) {
        lp = (uint32_t *)ptr;
        ptr += bytes_per_line;
      } else if (!params->error) {
        uint8_t *buf_start = (uint8_t *)line;
        uint32_t line_bytes = bytes_per_line;

        int min_x = area.x + crop_x;
        if (min_x >= 0) {
          buf_start += min_x / width_divider;
        } else {
          // reduce line_bytes to actually used bytes
          // ptr was already adjusted above
          line_bytes += min_x / width_divider;
        }
        line_bytes = min(line_bytes, EPD_WIDTH / width_divider -
                                         (uint32_t)(buf_start - line));
        memcpy(buf_start, ptr, line_bytes);
        ptr += bytes_per_line;

        int cropped_width = (crop ? crop_w : area.width);
        /// consider half-byte shifts in two-pixel-per-Byte mode.
        if (ppB == 2) {
          // mask last nibble for uneven width
          if (cropped_width % 2 == 1 &&
              min_x / 2 + cropped_width / 2 + 1 < EPD_WIDTH) {
            *(buf_start + line_bytes - 1) |= 0xF0;
          }
          if (area.x % 2 == 1 && !(crop_x % 2 == 1) && min_x < EPD_WIDTH) {
            shifted = true;
            uint32_t remaining =
                (uint32_t)line + EPD_WIDTH / 2 - (uint32_t)buf_start;
            uint32_t to_shift = min(line_bytes + 1, remaining);
            // shift one nibble to right
            nibble_shift_buffer_right(buf_start, to_shift);
          }
          // consider bit shifts in bit buffers
        } else if (ppB == 8) {
          // mask last n bits if width is not divisible by 8
          if (cropped_width % 8 != 0 && bytes_per_line + 1 < EPD_WIDTH) {
            uint8_t mask = 0;
            for (int s = 0; s < cropped_width % 8; s++) {
              mask = (mask << 1) | 1;
            }
            *(buf_start + line_bytes - 1) |= ~mask;
          }

          if (min_x % 8 != 0 && min_x < EPD_WIDTH) {
            // shift to right
            shifted = true;
            uint32_t remaining =
                (uint32_t)line + EPD_WIDTH / 8 - (uint32_t)buf_start;
            uint32_t to_shift = min(line_bytes + 1, remaining);
            bit_shift_buffer_right(buf_start, to_shift, min_x % 8);
          }
        }
        lp = (uint32_t *)line;
      }
      xQueueSendToBack(*params->output_queue, lp, portMAX_DELAY);
      if (shifted) {
        memset(line, 255, EPD_WIDTH / width_divider);
      }
    }

    xSemaphoreGive(params->done_smphr);
  }
}

/**
 * Set all pixels not in [xmin,xmax) to nop in the current line buffer.
 */
static void IRAM_ATTR mask_line_buffer(int xmin, int xmax) {
  // lower bound to where byte order is not an issue.
  int memset_start = (xmin / 16) * 4;
  int memset_end = min(((xmax + 15) / 16) * 4, EPD_LINE_BYTES);
  uint8_t *lb = epd_get_current_buffer();

  // memset the areas where order is not an issue
  memset(lb, 0, memset_start);
  memset(lb + memset_end, 0, EPD_LINE_BYTES - memset_end);

  const int offset_table[4] = {2, 3, 0, 1};

  // mask unused pixels at the start of the output interval
  uint8_t line_start_mask = 0xFF << (2 * (xmin % 4));
  uint8_t line_end_mask = 0xFF >> (8 - 2 * (xmax % 4));

  // number of full bytes to mask
  int lower_full_bytes = max(0, (xmin / 4 - memset_start));
  int upper_full_bytes = max(0, (memset_end - ((xmax + 3) / 4)));
  assert(lower_full_bytes <= 3);
  assert(upper_full_bytes <= 3);
  assert(memset_end >= 4);

  // mask full bytes
  for (int i = 0; i < lower_full_bytes; i++) {
    lb[memset_start + offset_table[i]] = 0x0;
  }
  for (int i = 0; i < upper_full_bytes; i++) {
    lb[memset_end - 4 + offset_table[3 - i]] = 0x0;
  }

  // mask partial bytes
  if ((memset_start + lower_full_bytes) * 4 < xmin) {
    epd_get_current_buffer()[memset_start + offset_table[lower_full_bytes]] &=
        line_start_mask;
  }
  if ((memset_end - upper_full_bytes) * 4 > xmax) {
    epd_get_current_buffer()[memset_end - 4 +
                             offset_table[3 - upper_full_bytes]] &=
        line_end_mask;
  }
}

void IRAM_ATTR busy_delay(uint32_t cycles);

static enum EpdDrawError calculate_lut(OutputParams *params) {

  enum EpdDrawMode mode = params->mode;
  enum EpdDrawMode selected_mode = mode & 0x3F;

  // two pixel per byte packing with only target color
  if (mode & MODE_PACKING_2PPB && mode & PREVIOUSLY_WHITE &&
      params->conversion_lut_size == (1 << 16)) {
    waveform_lut_static_from(params->waveform, params->conversion_lut, 0x0F,
                             params->waveform_index, params->waveform_range,
                             params->frame);
  } else if (mode & MODE_PACKING_2PPB && mode & PREVIOUSLY_BLACK &&
             params->conversion_lut_size == (1 << 16)) {
    waveform_lut_static_from(params->waveform, params->conversion_lut, 0x00,
                             params->waveform_index, params->waveform_range,
                             params->frame);

    // one pixel per byte with from and to colors
  } else if (mode & MODE_PACKING_1PPB_DIFFERENCE ||
             (mode & MODE_PACKING_2PPB &&
              params->conversion_lut_size == (1 << 10))) {
    waveform_lut(params->waveform, params->conversion_lut,
                 params->waveform_index, params->waveform_range, params->frame);

    // 1bit per pixel monochrome with only target color
  } else if (mode & MODE_PACKING_8PPB &&
             selected_mode == MODE_EPDIY_MONOCHROME) {
    // FIXME: Pack into waveform?
    if (mode & PREVIOUSLY_WHITE) {
      memcpy(params->conversion_lut, lut_1bpp_black, sizeof(lut_1bpp_black));
    } else if (mode & PREVIOUSLY_BLACK) {
      // FIXME: implement!
      // memcpy(params->conversion_lut, lut_1bpp_white, sizeof(lut_1bpp_white));
      return EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
    } else {
      return EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
    }

    // unknown format.
  } else {
    return EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
  }
  return EPD_DRAW_SUCCESS;
}

void IRAM_ATTR feed_display(OutputParams *params) {
  while (true) {
    xSemaphoreTake(params->start_smphr, portMAX_DELAY);

    skipping = 0;
    EpdRect area = params->area;
    enum EpdDrawMode mode = params->mode;
    int frame_time = params->frame_time;

    params->error |= calculate_lut(params);

    void (*input_calc_func)(const uint32_t *, uint8_t *, const uint8_t *) =
        NULL;
    if (mode & MODE_PACKING_2PPB) {
      if (params->conversion_lut_size == 1024) {
        if (mode & PREVIOUSLY_WHITE) {
          input_calc_func = &calc_epd_input_4bpp_1k_lut_white;
        } else if (mode & PREVIOUSLY_BLACK) {
          input_calc_func = &calc_epd_input_4bpp_1k_lut_black;
        } else {
          params->error |= EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
        }
      } else if (params->conversion_lut_size == (1 << 16)) {
        input_calc_func = &calc_epd_input_4bpp_lut_64k;
      } else {
        params->error |= EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
      }
    } else if (mode & MODE_PACKING_1PPB_DIFFERENCE) {
      input_calc_func = &calc_epd_input_1ppB;
    } else if (mode & MODE_PACKING_8PPB) {
      input_calc_func = &calc_epd_input_1bpp;
    } else {
      params->error |= EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
    }

    // Adjust min and max row for crop.
    const bool crop = (params->crop_to.width > 0 && params->crop_to.height > 0);
    int crop_y = (crop ? params->crop_to.y : 0);
    int min_y = area.y + crop_y;
    int max_y = min(min_y + (crop ? params->crop_to.height : area.height), area.height);

    // interval of the output line that is needed
    // FIXME: only lookup needed parts
    int line_start_x = area.x + (crop ? params->crop_to.x : 0);
    int line_end_x = line_start_x + (crop ? params->crop_to.width : area.width);
    line_start_x = min(max(line_start_x, 0), EPD_WIDTH);
    line_end_x = min(max(line_end_x, 0), EPD_WIDTH);

    uint64_t now = esp_timer_get_time();
    uint64_t diff = (now - last_frame_start) / 1000;
    if (diff < MINIMUM_FRAME_TIME) {
      vTaskDelay(MINIMUM_FRAME_TIME - diff);
    }

    last_frame_start = esp_timer_get_time();

    epd_start_frame();
    for (int i = 0; i < EPD_HEIGHT; i++) {
      if (i < min_y || i >= max_y) {
        skip_row(frame_time);
        continue;
      }
      if (params->drawn_lines != NULL && !params->drawn_lines[i - area.y]) {
        skip_row(frame_time);
        continue;
      }

      uint8_t output[EPD_WIDTH];
      xQueueReceive(*params->output_queue, output, portMAX_DELAY);
      if (!params->error) {
        (*input_calc_func)((uint32_t *)output, epd_get_current_buffer(),
                           params->conversion_lut);
        if (line_start_x > 0 || line_end_x < EPD_WIDTH) {
          mask_line_buffer(line_start_x, line_end_x);
        }
      }
      write_row(frame_time);
    }
    if (!skipping) {
      // Since we "pipeline" row output, we still have to latch out the last
      // row.
      write_row(frame_time);
    }
    epd_end_frame();

    xSemaphoreGive(params->done_smphr);
  }
}
