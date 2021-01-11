/*
  xdrv_04_light_utils.ino - Converter functions for lights

  Copyright (C) 2020  Theo Arends

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

// This file is compiled even if `USE_LIGHT` is not defined and provides
// general purpose converter fucntions


// New version of Gamma correction compute
// Instead of a table, we do a multi-linear approximation, which is close enough
// At low levels, the slope is a bit higher than actual gamma, to make changes smoother
// Internal resolution is 10 bits.

typedef struct gamma_table_t {
  uint16_t to_src;
  uint16_t to_gamma;
} gamma_table_t;

const gamma_table_t gamma_table[] = {   // don't put in PROGMEM for performance reasons
  {    1,      1 },
  {    4,      1 },
  {  209,     13 },
  {  312,     41 },
  {  457,    106 },
  {  626,    261 },
  {  762,    450 },
  {  895,    703 },
  { 1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};

// simplified Gamma table for Fade, cheating a little at low brightness
const gamma_table_t gamma_table_fast[] = {
  {   384,    192 },
  {   768,    576 },
  {  1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};

// For reference, below are the computed gamma tables, via ledGamma()
// for 8 bits output:
//   0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//   1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,
//   3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,
//   6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 11,
//  11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18,
//  18, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
//  25, 26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 36, 37, 38,
//  39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53,
//  54, 55, 56, 57, 58, 59, 60, 61, 61, 62, 63, 64, 65, 67, 68, 69,
//  71, 72, 73, 75, 76, 78, 79, 80, 82, 83, 85, 86, 87, 89, 90, 91,
//  93, 94, 95, 97, 98,100,101,102,104,105,107,108,109,111,112,114,
// 116,118,120,122,124,125,127,129,131,133,135,137,139,141,143,144,
// 146,148,150,152,154,156,158,160,162,164,166,168,170,171,173,175,
// 178,180,183,185,188,190,193,195,198,200,203,205,208,210,213,215,
// 218,220,223,225,228,230,233,235,238,240,243,245,248,250,253,255
//
// and for 10 bits output:
//    0,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
//    5,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,
//    8,   8,   9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,
//   12,  12,  13,  13,  13,  14,  15,  16,  17,  18,  20,  21,  22,  23,  24,  25,
//   26,  27,  28,  29,  30,  31,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,
//   45,  47,  49,  50,  52,  54,  56,  58,  59,  61,  63,  65,  67,  68,  70,  72,
//   74,  76,  77,  79,  81,  83,  84,  86,  88,  90,  92,  93,  95,  97,  99, 101,
//  102, 104, 106, 110, 113, 117, 121, 124, 128, 132, 135, 139, 143, 146, 150, 154,
//  158, 162, 166, 169, 173, 177, 180, 184, 188, 191, 195, 199, 202, 206, 210, 213,
//  217, 221, 224, 228, 232, 235, 239, 243, 246, 250, 254, 257, 261, 267, 272, 278,
//  283, 289, 294, 300, 305, 311, 317, 322, 328, 333, 339, 344, 350, 356, 361, 367,
//  372, 378, 383, 389, 394, 400, 406, 411, 417, 422, 428, 433, 439, 444, 450, 458,
//  465, 473, 480, 488, 496, 503, 511, 518, 526, 534, 541, 549, 557, 564, 572, 579,
//  587, 595, 602, 610, 617, 627, 635, 642, 650, 657, 665, 673, 680, 688, 695, 703,
//  713, 723, 733, 743, 753, 763, 773, 783, 793, 803, 813, 823, 833, 843, 853, 863,
//  873, 883, 893, 903, 913, 923, 933, 943, 953, 963, 973, 983, 993,1003,1013,1023
//
// Output for Dimmer 0..100 values
//   0,  1,  2,  3,  3,  4,  4,  5,  5,  6,  7,  7,  8,  8,  9,
//  10, 10, 11, 12, 12, 13, 15, 17, 21, 23, 26, 28, 31, 34, 37,
//  40, 43, 49, 52, 58, 61, 67, 70, 76, 79, 84, 90, 93, 99,102,
// 110,117,128,135,146,158,166,177,184,195,202,213,221,232,239,
// 250,261,272,289,300,317,328,344,356,372,389,400,417,428,444,
// 458,480,496,518,534,557,579,595,617,635,657,673,695,713,743,
// 773,793,823,843,873,893,923,943,973,993,1023


/*********************************************************************************************\
 * Color converters
\*********************************************************************************************/

// new version with only integer computing
// brightness is not needed, it is controlled via Dimmer
void RgbToHsb(uint8_t ir, uint8_t ig, uint8_t ib, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri) {
  uint32_t r = ir;
  uint32_t g = ig;
  uint32_t b = ib;
  uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255
  uint32_t min = (r < g && r < b) ? r : (g < b) ? g : b;   // 0..255
  uint32_t d = max - min;   // 0..255

  uint16_t hue = 0;   // hue value in degrees ranges from 0 to 359
  uint8_t sat = 0;    // 0..255
  uint8_t bri = max;  // 0..255

  if (d != 0) {
    sat = changeUIntScale(d, 0, max, 0, 255);
    if (r == max) {
      hue = (g > b) ?       changeUIntScale(g-b,0,d,0,60) : 360 - changeUIntScale(b-g,0,d,0,60);
    } else if (g == max) {
      hue = (b > r) ? 120 + changeUIntScale(b-r,0,d,0,60) : 120 - changeUIntScale(r-b,0,d,0,60);
    } else {
      hue = (r > g) ? 240 + changeUIntScale(r-g,0,d,0,60) : 240 - changeUIntScale(g-r,0,d,0,60);
    }
    hue = hue % 360;    // 0..359
  }

  if (r_hue) *r_hue = hue;
  if (r_sat) *r_sat = sat;
  if (r_bri) *r_bri = bri;
  //AddLog_P(LOG_LEVEL_DEBUG_MORE, "RgbToHsb rgb (%d %d %d) hsb (%d %d %d)", r, g, b, hue, sat, bri);
}

void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
  uint32_t r = 255;  // default to white
  uint32_t g = 255;
  uint32_t b = 255;
  // we take brightness at 100%, brightness should be set separately
  hue = hue % 360;  // normalize to 0..359

  if (sat > 0) {
    uint32_t i = hue / 60;   // quadrant 0..5
    uint32_t f = hue % 60;   // 0..59
    uint32_t q = 255 - changeUIntScale(f, 0, 60, 0, sat);  // 0..59
    uint32_t p = 255 - sat;
    uint32_t t = 255 - changeUIntScale(60 - f, 0, 60, 0, sat);

    switch (i) {
      case 0:
        //r = 255;
        g = t;
        b = p;
        break;
      case 1:
        r = q;
        //g = 255;
        b = p;
        break;
      case 2:
        r = p;
        //g = 255;
        b = t;
        break;
      case 3:
        r = p;
        g = q;
        //b = 255;
        break;
      case 4:
        r = t;
        g = p;
        //b = 255;
        break;
      default:
        //r = 255;
        g = p;
        b = q;
        break;
      }
    }
  if (r_r)  *r_r = r;
  if (r_g)  *r_g = g;
  if (r_b)  *r_b = b;
}

#define POW FastPrecisePowf

//
// Matrix 3x3 multiplied to a 3 vector, result in a 3 vector
//
void mat3x3(const float *mat33, const float *vec3, float *res3) {
  for (uint32_t i = 0; i < 3; i++) {
    const float * v = vec3;
    *res3 = 0.0f;
    for (uint32_t j = 0; j < 3; j++) {
      *res3 += *mat33++ * *v++;
    }
    res3++;
  }
}

void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y) {
  float x = 0.31271f;   // default medium white
  float y = 0.32902f;

  if (i_r + i_b + i_g > 0) {
    float rgb[3] = { (float)i_r, (float)i_g, (float)i_b };
    // https://gist.github.com/popcorn245/30afa0f98eea1c2fd34d
    // Gamma correction
    for (uint32_t i = 0; i < 3; i++) {
      rgb[i] = rgb[i] / 255.0f;
      rgb[i] = (rgb[i] > 0.04045f) ? POW((rgb[i] + 0.055f) / (1.0f + 0.055f), 2.4f) : (rgb[i] / 12.92f);
    }

    // conversion to X, Y, Z
    // Y is also the Luminance
    float XYZ[3];
    static const float XYZ_factors[] = {  0.649926f, 0.103455f, 0.197109f,
                                          0.234327f, 0.743075f, 0.022598f,
                                          0.000000f, 0.053077f, 1.035763f };
    mat3x3(XYZ_factors, rgb, XYZ);

    float XYZ_sum = XYZ[0] + XYZ[1] + XYZ[2];
    x = XYZ[0] / XYZ_sum;
    y = XYZ[1] / XYZ_sum;
    // we keep the raw gamut, one nice thing could be to convert to a narrower gamut
  }
  if (r_x)  *r_x = x;
  if (r_y)  *r_y = y;
}

void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb)
{
  float XYZ[3], rgb[3];
  x = (x > 0.99f ? 0.99f : (x < 0.01f ? 0.01f : x));
  y = (y > 0.99f ? 0.99f : (y < 0.01f ? 0.01f : y));
  float z = 1.0f - x - y;
  XYZ[0] = x / y;
  XYZ[1] = 1.0f;
  XYZ[2] = z / y;

  static const float rgb_factors[] = {  3.2406f, -1.5372f, -0.4986f,
                                       -0.9689f,  1.8758f,  0.0415f,
                                        0.0557f, -0.2040f,  1.0570f };
  mat3x3(rgb_factors, XYZ, rgb);
  float max = (rgb[0] > rgb[1] && rgb[0] > rgb[2]) ? rgb[0] : (rgb[1] > rgb[2]) ? rgb[1] : rgb[2];

  for (uint32_t i = 0; i < 3; i++) {
    rgb[i] = rgb[i] / max; // normalize to max == 1.0
    rgb[i] = (rgb[i] <= 0.0031308f) ? 12.92f * rgb[i] : 1.055f * POW(rgb[i], (1.0f / 2.4f)) - 0.055f; // gamma
  }

  int32_t irgb[3];
  for (uint32_t i = 0; i < 3; i++) {
    irgb[i] = rgb[i] * 255.0f + 0.5f;
  }

  if (rr) { *rr = (irgb[0] > 255 ? 255: (irgb[0] < 0 ? 0 : irgb[0])); }
  if (rg) { *rg = (irgb[1] > 255 ? 255: (irgb[1] < 0 ? 0 : irgb[1])); }
  if (rb) { *rb = (irgb[2] > 255 ? 255: (irgb[2] < 0 ? 0 : irgb[2])); }
}

/*********************************************************************************************\
 * Change scales from 8 bits to 10 bits and vice versa
\*********************************************************************************************/
// 8 to 10 to 8 is guaranteed to give the same result
uint16_t change8to10(uint8_t v) {
  return changeUIntScale(v, 0, 255, 0, 1023);
}
// change from 10 bits to 8 bits, but any non-zero input will be non-zero
uint8_t change10to8(uint16_t v) {
  return (0 == v) ? 0 : changeUIntScale(v, 4, 1023, 1, 255);
}

/*********************************************************************************************\
 * Gamma correction
\*********************************************************************************************/
// Calculate the gamma corrected value for LEDS
uint16_t ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (v <= to_src) {
      return changeUIntScale(v, from_src, to_src, from_gamma, to_gamma);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}
// Calculate the reverse gamma value for LEDS
uint16_t ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (vg <= to_gamma) {
      return changeUIntScale(vg, from_gamma, to_gamma, from_src, to_src);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}

// 10 bits in, 10 bits out
uint16_t ledGamma10_10(uint16_t v) {
  return ledGamma_internal(v, gamma_table);
}

// 10 bits resolution, 8 bits in
uint16_t ledGamma10(uint8_t v) {
  return ledGamma10_10(change8to10(v));
}

// Legacy function
uint8_t ledGamma(uint8_t v) {
  return change10to8(ledGamma10(v));
}

// Fast versions for Fading
uint16_t ledGammaFast(uint16_t v) {
  return ledGamma_internal(v, gamma_table_fast);
}

uint16_t leddGammaReverseFast(uint16_t vg) {
  return ledGammaReverse_internal(vg, gamma_table_fast);
}