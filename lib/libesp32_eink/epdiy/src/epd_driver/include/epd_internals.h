/**
 * @file epd_internals.h
 * @brief Internal definitions and auxiliary data types.
 *
 * Unless you want to extend the library itself (Which you are very welcome to do),
 * you will most likely not need to know about this file.
 */

#ifndef EPD_INTERNALS_H
#define EPD_INTERNALS_H

#include <stdbool.h>
#include <stdint.h>

/// minimal draw time in ms for a frame layer,
/// which will allow all particles to set properly.
#ifndef MINIMUM_FRAME_TIME
#define MINIMUM_FRAME_TIME 12
#endif

/// Frame draw time for monochrome mode in 1/10 us.
#define MONOCHROME_FRAME_TIME 120

#if defined(CONFIG_EPD_DISPLAY_TYPE_ED097OC4) ||                               \
    defined(CONFIG_EPD_DISPLAY_TYPE_ED097TC2) ||                               \
    defined(CONFIG_EPD_DISPLAY_TYPE_ED097OC4_LQ)
/// Width of the display area in pixels.
#define EPD_WIDTH 1200
/// Height of the display area in pixels.
#define EPD_HEIGHT 825
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED133UT2)
#define EPD_WIDTH 1600
#define EPD_HEIGHT 1200
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED060SC4)
/// Width of the display area in pixels.
#define EPD_WIDTH 800
/// Height of the display area in pixels.
#define EPD_HEIGHT 600
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED060SCT)
/// Width of the display area in pixels.
#define EPD_WIDTH 800
/// Height of the display area in pixels.
#define EPD_HEIGHT 600
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED060XC3)
/// Width of the display area in pixels.
#define EPD_WIDTH 1024
/// Height of the display area in pixels.
#define EPD_HEIGHT 758
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED047TC1)
/// Width of the display area in pixels.
#define EPD_WIDTH 960
/// Height of the display area in pixels.
#define EPD_HEIGHT 540
#else
#error "no display type defined!"
#endif


typedef struct {
  int phases;
  const uint8_t* luts;
  /// If we have timing information for the individual
  /// phases, this is an array of the on-times for each phase.
  /// Otherwise, this is NULL.
  const int* phase_times;
} EpdWaveformPhases;

typedef struct {
  uint8_t type;
  uint8_t temp_ranges;
  EpdWaveformPhases const **range_data;
} EpdWaveformMode;

typedef struct {
  int min;
  int max;
} EpdWaveformTempInterval;

typedef struct {
  uint8_t num_modes;
  uint8_t num_temp_ranges;
  EpdWaveformMode const **mode_data;
  EpdWaveformTempInterval const *temp_intervals;
} EpdWaveform;




extern const EpdWaveform ed047tc2;
extern const EpdWaveform epdiy_ED060SC4;
extern const EpdWaveform epdiy_ED097OC4;
extern const EpdWaveform epdiy_ED047TC1;
extern const EpdWaveform epdiy_ED097TC2;
extern const EpdWaveform epdiy_ED060XC3;
extern const EpdWaveform epdiy_ED060SCT;
extern const EpdWaveform epdiy_ED133UT2;

#if defined(CONFIG_EPD_DISPLAY_TYPE_ED047TC1)
//#define EPD_BUILTIN_WAVEFORM &epdiy_ED047TC1
#define EPD_BUILTIN_WAVEFORM &ed047tc2
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED060SC4)
#define EPD_BUILTIN_WAVEFORM &epdiy_ED060SC4
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED060XC3)
#define EPD_BUILTIN_WAVEFORM &epdiy_ED060XC3
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED060SCT)
#define EPD_BUILTIN_WAVEFORM &epdiy_ED060SCT
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED097OC4) || defined(CONFIG_EPD_DISPLAY_TYPE_ED097OC4_LQ)
#define EPD_BUILTIN_WAVEFORM &epdiy_ED097OC4
#elif defined(CONFIG_EPD_DISPLAY_TYPE_ED097TC2)
#define EPD_BUILTIN_WAVEFORM &epdiy_ED097TC2
#elif defined (CONFIG_EPD_DISPLAY_TYPE_ED133UT2)
#define EPD_BUILTIN_WAVEFORM &epdiy_ED133UT2
#endif

/// Font data stored PER GLYPH
typedef struct {
  uint8_t width;            ///< Bitmap dimensions in pixels
  uint8_t height;           ///< Bitmap dimensions in pixels
  uint8_t advance_x;        ///< Distance to advance cursor (x axis)
  int16_t left;             ///< X dist from cursor pos to UL corner
  int16_t top;              ///< Y dist from cursor pos to UL corner
  uint16_t compressed_size; ///< Size of the zlib-compressed font data.
  uint32_t data_offset;     ///< Pointer into EpdFont->bitmap
} EpdGlyph;

/// Glyph interval structure
typedef struct {
  uint32_t first;  ///< The first unicode code point of the interval
  uint32_t last;   ///< The last unicode code point of the interval
  uint32_t offset; ///< Index of the first code point into the glyph array
} EpdUnicodeInterval;

/// Data stored for FONT AS A WHOLE
typedef struct {
  const uint8_t *bitmap;            ///< Glyph bitmaps, concatenated
  const EpdGlyph *glyph;            ///< Glyph array
  const EpdUnicodeInterval *intervals; ///< Valid unicode intervals for this font
  uint32_t interval_count;    ///< Number of unicode intervals.
  bool compressed;            ///< Does this font use compressed glyph bitmaps?
  uint8_t advance_y;          ///< Newline distance (y axis)
  int ascender;               ///< Maximal height of a glyph above the base line
  int descender;              ///< Maximal height of a glyph below the base line
} EpdFont;


#endif // EPD_INTERNALS_H
