/**
 * @file epd_driver.h
 * A driver library for drawing to an EPD.
 */
#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_EPD_DISPLAY_TYPE_ED047TC1
#define CONFIG_EPD_BOARD_REVISION_LILYGO_T5_47

#pragma once
#include "esp_attr.h"
#include "epd_internals.h"

#include <stdbool.h>
#include <stdint.h>

/// An area on the display.
typedef struct {
  /// Horizontal position.
  int x;
  /// Vertical position.
  int y;
  /// Area / image width, must be positive.
  int width;
  /// Area / image height, must be positive.
  int height;
} EpdRect;

/// Possible failures when drawing.
enum EpdDrawError {
  EPD_DRAW_SUCCESS = 0x0,
  /// No valid framebuffer packing mode was specified.
  EPD_DRAW_INVALID_PACKING_MODE = 0x1,

  /// No lookup table implementation for this mode / packing.
  EPD_DRAW_LOOKUP_NOT_IMPLEMENTED = 0x2,

  /// The string to draw is invalid.
  EPD_DRAW_STRING_INVALID = 0x4,

  /// The string was not empty, but no characters where drawable.
  EPD_DRAW_NO_DRAWABLE_CHARACTERS = 0x8,

  /// Allocation failed
  EPD_DRAW_FAILED_ALLOC = 0x10,

  /// A glyph could not be drawn, and not fallback was present.
  EPD_DRAW_GLYPH_FALLBACK_FAILED = 0x20,

  /// The specified crop area is invalid.
  EPD_DRAW_INVALID_CROP = 0x40,

  /// No such mode is available with the current waveform.
  EPD_DRAW_MODE_NOT_FOUND = 0x80,

  /// The waveform info file contains no applicable temperature range.
  EPD_DRAW_NO_PHASES_AVAILABLE = 0x100,

  /// An invalid combination of font flags was used.
  EPD_DRAW_INVALID_FONT_FLAGS = 0x200,
};

/// Global EPD driver options.
enum EpdInitOptions {
    /// Use the default options.
    EPD_OPTIONS_DEFAULT = 0,
    /// Use a small look-up table of 1024 bytes.
    /// The EPD driver will use less space, but performance may be worse.
    EPD_LUT_1K = 1,
    /// Use a 64K lookup table. (default)
    /// Best performance, but permanently occupies a 64k block of internal memory.
    EPD_LUT_64K = 2,

    /// Use a small feed queue of 8 display lines.
    /// This uses less memory, but may impact performance.
    EPD_FEED_QUEUE_8 = 4,
    /// Use a feed queue of 32 display lines. (default)
    /// Best performance, but larger memory footprint.
    EPD_FEED_QUEUE_32 = 8,
};

/// The image drawing mode.
enum EpdDrawMode {
  /// An init waveform.
  /// This is currently unused, use `epd_clear()` instead.
  MODE_INIT = 0x0,
  /// Direct Update: Go from any color to black for white only.
  MODE_DU = 0x1,
  /// Go from any grayscale value to another with a flashing update.
  MODE_GC16 = 0x2,
  /// Faster version of `MODE_GC16`.
  /// Not available with default epdiy waveforms.
  MODE_GC16_FAST = 0x3,
  /// Animation Mode: Fast, monochrom updates.
  /// Not available with default epdiy waveforms.
  MODE_A2 = 0x4,
  /// Go from any grayscale value to another with a non-flashing update.
  MODE_GL16 = 0x5,
  /// Faster version of `MODE_GL16`.
  /// Not available with default epdiy waveforms.
  MODE_GL16_FAST = 0x6,
  /// A 4-grayscale version of `MODE_DU`.
  /// Not available with default epdiy waveforms.
  MODE_DU4 = 0x7,
  /// Arbitrary transitions for 4 grayscale values.
  /// Not available with default epdiy waveforms.
  MODE_GL4 = 0xA,
  /// Not available with default epdiy waveforms.
  MODE_GL16_INV = 0xB,

  /// Go from a white screen to arbitrary grayscale, quickly.
  /// Exclusively available with epdiy waveforms.
  MODE_EPDIY_WHITE_TO_GL16 = 0x10,
  /// Go from a black screen to arbitrary grayscale, quickly.
  /// Exclusively available with epdiy waveforms.
  MODE_EPDIY_BLACK_TO_GL16 = 0x11,

  /// Monochrome mode. Only supported with 1bpp buffers.
  MODE_EPDIY_MONOCHROME = 0x20,

  MODE_UNKNOWN_WAVEFORM = 0x3F,

  // Framebuffer packing modes
  /// 1 bit-per-pixel framebuffer with 0 = black, 1 = white.
  /// MSB is left is the leftmost pixel, LSB the rightmost pixel.
  MODE_PACKING_8PPB = 0x40,
  /// 4 bit-per pixel framebuffer with 0x0 = black, 0xF = white.
  /// The upper nibble corresponds to the left pixel.
  /// A byte cannot wrap over multiple rows, images of uneven width
  /// must add a padding nibble per line.
  MODE_PACKING_2PPB = 0x80,
  /// A difference image with one pixel per byte.
  /// The upper nibble marks the "from" color,
  /// the lower nibble the "to" color.
  MODE_PACKING_1PPB_DIFFERENCE = 0x100,
  // reserver for 4PPB mode

  /// Assert that the display has a uniform color, e.g. after initialization.
  /// If `MODE_PACKING_2PPB` is specified, a optimized output calculation can be used.
  /// Draw on a white background
  PREVIOUSLY_WHITE = 0x200,
  /// See `PREVIOUSLY_WHITE`.
  /// Draw on a black background
  PREVIOUSLY_BLACK = 0x400,
};

/// The default draw mode (non-flashy refresh, whith previously white screen).
#define EPD_MODE_DEFAULT (MODE_GL16 | PREVIOUSLY_WHITE)

/// Font drawing flags
enum EpdFontFlags {
  /// Draw a background.
  ///
  /// Take the background into account
  /// when calculating the size.
  EPD_DRAW_BACKGROUND = 0x1,

  /// Left-Align lines
  EPD_DRAW_ALIGN_LEFT = 0x2,
  /// Right-align lines
  EPD_DRAW_ALIGN_RIGHT = 0x4,
  /// Center-align lines
  EPD_DRAW_ALIGN_CENTER = 0x8,
};

/// Font properties.
typedef struct {
  /// Foreground color
  uint8_t fg_color : 4;
  /// Background color
  uint8_t bg_color : 4;
  /// Use the glyph for this codepoint for missing glyphs.
  uint32_t fallback_glyph;
  /// Additional flags, reserved for future use
  enum EpdFontFlags flags;
} EpdFontProperties;

/** Initialize the ePaper display */
void epd_init(enum EpdInitOptions options);

/** Deinit the ePaper display */
void epd_deinit();

/** Enable display power supply. */
void epd_poweron();

/** Disable display power supply. */
void epd_poweroff();

/** Clear the whole screen by flashing it. */
void epd_clear();

/**
 * Clear an area by flashing it.
 *
 * @param area: The area to clear.
 */
void epd_clear_area(EpdRect area);

/**
 * Clear an area by flashing it.
 *
 * @param area: The area to clear.
 * @param cycles: The number of black-to-white clear cycles.
 * @param cycle_time: Length of a cycle. Default: 50 (us).
 */
void epd_clear_area_cycles(EpdRect area, int cycles, int cycle_time);

/**
 * @returns Rectancle representing the whole screen area.
 */
EpdRect epd_full_screen();

/**
 * Draw a picture to a given framebuffer.
 *
 * @param image_area: The area to copy to. `width` and `height` of the area
 *   must correspond to the image dimensions in pixels.
 * @param image_data: The image data, as a buffer of 4 bit wide brightness
 * values. Pixel data is packed (two pixels per byte). A byte cannot wrap over
 * multiple rows, images of uneven width must add a padding nibble per line.
 * @param framebuffer: The framebuffer object,
 *   which must be `EPD_WIDTH / 2 * EPD_HEIGHT` large.
 */
void epd_copy_to_framebuffer(EpdRect image_area, const uint8_t *image_data,
                             uint8_t *framebuffer);

/**
 * Draw a pixel a given framebuffer.
 *
 * @param x: Horizontal position in pixels.
 * @param y: Vertical position in pixels.
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_draw_pixel(int x, int y, uint8_t color, uint8_t *framebuffer);

/**
 * Draw a horizontal line to a given framebuffer.
 *
 * @param x: Horizontal start position in pixels.
 * @param y: Vertical start position in pixels.
 * @param length: Length of the line in pixels.
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 *  which must be `EPD_WIDTH / 2 * EPD_HEIGHT` bytes large.
 */
void epd_draw_hline(int x, int y, int length, uint8_t color,
                    uint8_t *framebuffer);

/**
 * Draw a horizontal line to a given framebuffer.
 *
 * @param x: Horizontal start position in pixels.
 * @param y: Vertical start position in pixels.
 * @param length: Length of the line in pixels.
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 *  which must be `EPD_WIDTH / 2 * EPD_HEIGHT` bytes large.
 */
void epd_draw_vline(int x, int y, int length, uint8_t color,
                    uint8_t *framebuffer);

void epd_fill_circle_helper(int x0, int y0, int r, int corners, int delta,
                            uint8_t color, uint8_t *framebuffer);

/**
 * Draw a circle with given center and radius
 *
 * @param x: Center-point x coordinate
 * @param y: Center-point y coordinate
 * @param r: Radius of the circle in pixels
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_draw_circle(int x, int y, int r, uint8_t color, uint8_t *framebuffer);

/**
 * Draw a circle with fill with given center and radius
 *
 * @param x: Center-point x coordinate
 * @param y: Center-point y coordinate
 * @param r: Radius of the circle in pixels
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_fill_circle(int x, int y, int r, uint8_t color, uint8_t *framebuffer);

/**
 * Draw a rectanle with no fill color
 *
 * @param rect: The rectangle to draw.
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_draw_rect(EpdRect rect, uint8_t color, uint8_t *framebuffer);

/**
 * Draw a rectanle with fill color
 *
 * @param rect: The rectangle to fill.
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_fill_rect(EpdRect rect, uint8_t color, uint8_t *framebuffer);

/**
 * Draw a line
 *
 * @param    x0  Start point x coordinate
 * @param    y0  Start point y coordinate
 * @param    x1  End point x coordinate
 * @param    y1  End point y coordinate
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_draw_line(int x0, int y0, int x1, int y1, uint8_t color,
                   uint8_t *framebuffer);

/**
 * Draw a triangle with no fill color
 *
 * @param    x0  Vertex #0 x coordinate
 * @param    y0  Vertex #0 y coordinate
 * @param    x1  Vertex #1 x coordinate
 * @param    y1  Vertex #1 y coordinate
 * @param    x2  Vertex #2 x coordinate
 * @param    y2  Vertex #2 y coordinate
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                       uint8_t color, uint8_t *framebuffer);

/**
 * Draw a triangle with color-fill
 *
 * @param    x0  Vertex #0 x coordinate
 * @param    y0  Vertex #0 y coordinate
 * @param    x1  Vertex #1 x coordinate
 * @param    y1  Vertex #1 y coordinate
 * @param    x2  Vertex #2 x coordinate
 * @param    y2  Vertex #2 y coordinate
 * @param color: The gray value of the line (0-255);
 * @param framebuffer: The framebuffer to draw to,
 */
void epd_fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                       uint8_t color, uint8_t *framebuffer);
/**
 * Get the current ambient temperature in °C, if supported by the board.
 * Requires the display to be powered on.
 */
float epd_ambient_temperature();

/**
 * The default font properties.
 */
EpdFontProperties epd_font_properties_default();

/*!
 * Get the text bounds for string, when drawn at (x, y).
 * Set font properties to NULL to use the defaults.
 */
void epd_get_text_bounds(const EpdFont *font, const char *string,
                     const int *x, const int *y,
                     int *x1, int *y1, int *w, int *h,
                     const EpdFontProperties *props);

/**
 * Write text to the EPD.
 */
enum EpdDrawError epd_write_string(const EpdFont *font, const char *string, int *cursor_x,
                int *cursor_y, uint8_t *framebuffer,
                const EpdFontProperties *properties);

/**
 * Write a (multi-line) string to the EPD.
 */
enum EpdDrawError epd_write_default(const EpdFont *font, const char *string, int *cursor_x,
                  int *cursor_y, uint8_t *framebuffer);

/**
 * Get the font glyph for a unicode code point.
 */
const EpdGlyph* epd_get_glyph(const EpdFont *font, uint32_t code_point);


/**
 * Darken / lighten an area for a given time.
 *
 * @param area: The area to darken / lighten.
 * @param time: The time in us to apply voltage to each pixel.
 * @param color: 1: lighten, 0: darken.
 */
void epd_push_pixels(EpdRect area, short time, int color);

/**
 * Base function for drawing an image on the screen.
 * If It is very customizable, and the documentation below should be studied carefully.
 * For simple applications, use the epdiy highlevel api in "epd_higlevel.h".
 *
 * @param area: The area of the screen to draw to.
 *      This can be imagined as shifting the origin of the frame buffer.
 * @param data: A full framebuffer of display data.
 *      It's structure depends on the chosen `mode`.
 * @param crop_to: Only draw a part of the frame buffer.
 *      Set to `epd_full_screen()` to draw the full buffer.
 * @param mode: Specifies the Waveform used, the framebuffer format
 *      and additional information, like if the display is cleared.
 * @param temperature: The temperature of the display in °C.
 *      Currently, this is unused by the default waveforms at can be
 *      set to room temperature, e.g. 20-25°C.
 * @param drawn_lines: If not NULL, an array of at least the height of the
 *      image. Every line where the corresponding value in `lines` is `false` will be
 *      skipped.
 * @param waveform: The waveform information to use for drawing.
 *      If you don't have special waveforms, use `EPD_BUILTIN_WAVEFORM`.
 * @returns `EPD_DRAW_SUCCESS` on sucess, a combination of error flags otherwise.
 */
enum EpdDrawError IRAM_ATTR epd_draw_base(EpdRect area,
                            const uint8_t *data,
                            EpdRect crop_to,
                            enum EpdDrawMode mode,
                            int temperature,
                            const bool *drawn_lines,
                            const EpdWaveform *waveform);
/**
 * Calculate a `MODE_PACKING_1PPB_DIFFERENCE` difference image
 * from two `MODE_PACKING_2PPB` (4 bit-per-pixel) buffers.
 * If you're using the epdiy highlevel api, this is handled by the update functions.
 *
 * @param to: The goal image as 4-bpp (`MODE_PACKING_2PPB`) framebuffer.
 * @param from: The previous image as 4-bpp (`MODE_PACKING_2PPB`) framebuffer.
 * @param crop_to: Only calculate the difference for a crop of the input framebuffers.
 *      The `interlaced` will not be modified outside the crop area.
 * @param interlaced: The resulting difference image in `MODE_PACKING_1PPB_DIFFERENCE` format.
 * @param dirty_lines: An array of at least `EPD_HEIGHT`.
 *      The positions corresponding to lines where `to` and `from` differ
 *      are set to `true`, otherwise to `false`.
 * @param previously_white: If not NULL, it is set to `true`
 *      if the considered crop of the `from`-image is completely white.
 * @param previously_black: If not NULL, it is set to `true`
 *      if the considered crop of the `from`-image is completely black.
 * @returns The smallest rectangle containing all changed pixels.
 */
EpdRect epd_difference_image_cropped(
    const uint8_t* to,
    const uint8_t* from,
    EpdRect crop_to,
    uint8_t* interlaced,
    bool* dirty_lines,
    bool* previously_white,
    bool* previously_black
);

/**
 * Simplified version of `epd_difference_image_cropped()`, which considers the
 * whole display frame buffer.
 *
 * See `epd_difference_image_cropped() for details.`
 */
EpdRect epd_difference_image(const uint8_t* to, const uint8_t* from, uint8_t* interlaced, bool* dirty_lines);




#ifdef __cplusplus
}
#endif
