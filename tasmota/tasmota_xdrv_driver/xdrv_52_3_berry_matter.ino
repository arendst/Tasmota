/*
  xdrv_52_3_berry_matter.ino - Berry support for Matter UI

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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

// Mappgin from internal light and a generic `light_state` Berry class

#ifdef USE_BERRY

// Convert seconds to a string representing days, hours or minutes present in the n-value.
// The string will contain the most coarse time only, rounded down (61m == 01h, 01h37m == 01h).
// Inputs:
// - seconds: uint32_t representing some number of seconds
// Outputs:
// - char for unit (d for day, h for hour, m for minute)
// - the hex color to be used to display the text
//
extern "C" uint32_t matter_convert_seconds_to_dhm(uint32_t seconds,  char *unit, uint32_t *color, bbool days) {
  *color = WebColor(COL_TEXT);
  static const uint32_t conversions[3] = {24 * 3600, 3600, 60};
  static const char     units[3] = { 'd', 'h', 'm'};   // day, hour, minute
  static const uint32_t color_threshold_hours[2] = {24 * 3600, 3600};               // 0 - 1 hour - 1 day
  static const uint32_t color_threshold_days[2] = {7 * 24 * 3600, 2 * 24 * 3600};    // 0 - 2 days - 7 days

  uint32_t color_text_8 = WebColor(COL_TEXT);    // color of text on 8 bits
  uint8_t color_text_8_r = (color_text_8 & 0xFF0000) >> 16;
  uint8_t color_text_8_g = (color_text_8 & 0x00FF00) >> 8;
  uint8_t color_text_8_b = (color_text_8 & 0x0000FF);

  uint32_t color_back_8 = WebColor(COL_BACKGROUND);    // color of background on 8 bits
  uint8_t color_back_8_r = (color_back_8 & 0xFF0000) >> 16;
  uint8_t color_back_8_g = (color_back_8 & 0x00FF00) >> 8;
  uint8_t color_back_8_b = (color_back_8 & 0x0000FF);

  int32_t colors[3] = {
    ((changeUIntScale( 6, 0, 16, color_back_8_r, color_text_8_r) & 0xFF) << 16U) |   //  6/16 of text
    ((changeUIntScale( 6, 0, 16, color_back_8_g, color_text_8_g) & 0xFF) <<  8U) |   //  6/16 of text
    ( changeUIntScale( 6, 0, 16, color_back_8_b, color_text_8_r) & 0xFF),            //  6/16 of text

    ((changeUIntScale(10, 0, 16, color_back_8_r, color_text_8_r) & 0xFF) << 16U) |   // 10/16 of text
    ((changeUIntScale(10, 0, 16, color_back_8_g, color_text_8_g) & 0xFF) <<  8U) |   // 10/16 of text
    ( changeUIntScale(10, 0, 16, color_back_8_b, color_text_8_r) & 0xFF),            // 10/16 of text

    (color_text_8_r << 16U) |
    (color_text_8_g <<  8U) |
    (color_text_8_b)
  };

  *color = (uint32_t)colors[2];
  for (uint32_t i = 0; i < 2; i++) {
    if (seconds > (days ? color_threshold_days[i] : color_threshold_hours[i])) {
      *color = (uint32_t)colors[i];
      break;
    }
  }
  for(uint32_t i = 0; i < 3; ++i) {
    *unit = units[i];
    if (seconds > conversions[i]) {    // always pass even if 00m
      return seconds / conversions[i];
    }
  }
  return 0;
}

#endif  // USE_BERRY
