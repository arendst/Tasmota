/**
 * @file "epd_highlevel.h"
 * @brief High-level API for drawing to e-paper displays.
 *
 * The functions in this library provide a simple way to manage updates of e-paper display.
 * To use it, follow the steps below:
 *
 * 1. First, we declare a global object that manages the display state.
 *
 * 		EpdiyHighlevelState hl;
 *
 * 2. Then, the driver and framebuffers must be initialized.
 *
 * 		epd_init(EPD_LUT_1K);
 * 		hl = epd_hl_init(EPD_BUILTIN_WAVEFORM);
 *
 * 3. Now, we can draw to the allocated framebuffer,
 *    using the draw and text functions defined in `epd_driver.h`.
 *    This will not yet update the display, but only its representation in memory.
 *
 * 		// A reference to the framebuffer
 * 		uint8_t* fb = epd_hl_get_framebuffer(&hl);
 *
 * 		// draw a black rectangle
 * 		EpdRect some_rect = {
 * 			.x = 100,
 * 			.y = 100,
 * 			.width = 100,
 * 			.height = 100
 * 		};
 * 		epd_fill_rect(some_rect, 0x0, fb);
 *
 * 		// write a message
 * 		int cursor_x = 100;
 * 		int cursor_y = 300;
 * 		epd_write_default(&FiraSans, "Hello, World!", &cursor_x, &cursor_y, fb);
 *
 * 		// finally, update the display!
 * 		int temperature = 25;
 * 		epd_poweron();
 * 		EpdDrawError err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
 * 		epd_poweroff();
 *
 * 	That's it! For many application, this will be enough.
 * 	For special applications and requirements, have a
 * 	closer look at the `epd_driver.h` header.
 */

 #ifdef __cplusplus
 extern "C" {
 #endif

#include "epd_driver.h"

/// Holds the internal state of the high-level API.
typedef struct {
  /// The "front" framebuffer object.
  uint8_t* front_fb;
  /// The "back" framebuffer object.
  uint8_t* back_fb;
  /// Buffer for holding the interlaced difference image.
  uint8_t* difference_fb;
  /// Tainted lines based on the last difference calculation.
  bool* dirty_lines;
  /// The waveform information to use.
  const EpdWaveform* waveform;
} EpdiyHighlevelState;


/**
 * Initialize a state object.
 * This allocates two framebuffers and an update buffer for
 * the display in the external PSRAM.
 * In order to keep things simple, a chip reset is triggered if this fails.
 *
 * @param waveform: The waveform to use for updates.
 * 		If you did not create your own, this will be `EPD_BUILTIN_WAVEFORM`.
 * @returns An initialized state object.
 */
EpdiyHighlevelState epd_hl_init(const EpdWaveform* waveform);

/// Get a reference to the front framebuffer.
/// Use this to draw on the framebuffer before updating the screen with `epd_hl_update_screen()`.
uint8_t* epd_hl_get_framebuffer(EpdiyHighlevelState* state);

/**
 * Update the EPD screen to match the content of the front frame buffer.
 * Prior to this, power to the display must be enabled via `epd_poweron()`
 * and should be disabled afterwards if no immediate additional updates follow.
 *
 * @param state: A reference to the `EpdiyHighlevelState` object used.
 * @param mode: The update mode to use.
 * 		Additional mode settings like the framebuffer format or
 * 		previous display state are determined by the driver and must not be supplied here.
 * 		In most cases, one of `MODE_GC16` and `MODE_GL16` should be used.
 * @param temperature: Environmental temperature of the display in °C.
 * @returns `EPD_DRAW_SUCCESS` on sucess, a combination of error flags otherwise.
 */
enum EpdDrawError epd_hl_update_screen(EpdiyHighlevelState* state, enum EpdDrawMode mode, int temperature);

/**
 * Update an area of the screen to match the content of the front framebuffer.
 * Supplying a small area to update can speed up the update process.
 * Prior to this, power to the display must be enabled via `epd_poweron()`
 * and should be disabled afterwards if no immediate additional updates follow.
 *
 * @param state: A reference to the `EpdiyHighlevelState` object used.
 * @param mode: See `epd_hl_update_screen()`.
 * @param temperature: Environmental temperature of the display in °C.
 * @param area: Area of the screen to update.
 * @returns `EPD_DRAW_SUCCESS` on sucess, a combination of error flags otherwise.
 */
enum EpdDrawError epd_hl_update_area(EpdiyHighlevelState* state, enum EpdDrawMode mode, int temperature, EpdRect area);

/**
 * Reset the front framebuffer to a white state.
 *
 * @param state: A reference to the `EpdiyHighlevelState` object used.
 */
void epd_hl_set_all_white(EpdiyHighlevelState* state);

/**
 * Bring the display to a fully white state and get rid of any
 * remaining artifacts.
 */
void epd_fullclear(EpdiyHighlevelState* state, int temperature);

#ifdef __cplusplus
}
#endif
