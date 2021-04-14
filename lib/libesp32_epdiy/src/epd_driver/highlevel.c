/**
 * High-level API implementation for epdiy.
 */

#include "epd_highlevel.h"
#include <assert.h>
#include <esp_types.h>
#include <string.h>
#include <esp_heap_caps.h>
#include <esp_log.h>

static bool already_initialized = 0;

const static int fb_size = EPD_WIDTH / 2 * EPD_HEIGHT;

EpdiyHighlevelState epd_hl_init(const EpdWaveform* waveform) {
  assert(!already_initialized);
  assert(waveform != NULL);

  EpdiyHighlevelState state;
  state.front_fb = heap_caps_malloc(fb_size, MALLOC_CAP_SPIRAM);
  assert(state.front_fb != NULL);
  state.back_fb = heap_caps_malloc(fb_size, MALLOC_CAP_SPIRAM);
  assert(state.back_fb != NULL);
  state.difference_fb = heap_caps_malloc(2 * fb_size, MALLOC_CAP_SPIRAM);
  assert(state.difference_fb != NULL);
  state.dirty_lines = malloc(EPD_HEIGHT * sizeof(bool));
  assert(state.dirty_lines != NULL);
  state.waveform = waveform;

  memset(state.front_fb, 0xFF, fb_size);
  memset(state.back_fb, 0xFF, fb_size);

  already_initialized = true;
  return state;
}


uint8_t* epd_hl_get_framebuffer(EpdiyHighlevelState* state) {
  assert(state != NULL);
  return state->front_fb;
}

enum EpdDrawError epd_hl_update_screen(EpdiyHighlevelState* state, enum EpdDrawMode mode, int temperature) {
  return epd_hl_update_area(state, mode, temperature, epd_full_screen());
}

enum EpdDrawError epd_hl_update_area(EpdiyHighlevelState* state, enum EpdDrawMode mode, int temperature, EpdRect area) {
  assert(state != NULL);

  bool previously_white = false;
  bool previously_black = false;

  //FIXME: use crop information here, if available
  EpdRect diff_area = epd_difference_image_cropped(
	  state->front_fb,
	  state->back_fb,
	  area,
	  state->difference_fb,
	  state->dirty_lines,
      &previously_white,
      &previously_black
  );

  if (diff_area.height == 0 || diff_area.width == 0) {
      return EPD_DRAW_SUCCESS;
  }

  enum EpdDrawError err;
  if (previously_white) {
      err = epd_draw_base(epd_full_screen(), state->front_fb, diff_area, MODE_PACKING_2PPB | PREVIOUSLY_WHITE | mode, temperature, state->dirty_lines, state->waveform);
  } else if (previously_black) {
      err = epd_draw_base(epd_full_screen(), state->front_fb, diff_area, MODE_PACKING_2PPB | PREVIOUSLY_BLACK | mode, temperature, state->dirty_lines, state->waveform);
  } else {
      err = epd_draw_base(epd_full_screen(), state->difference_fb, diff_area, MODE_PACKING_1PPB_DIFFERENCE | mode, temperature, state->dirty_lines, state->waveform);
  }

  for (int l=diff_area.y; l < diff_area.y + diff_area.height; l++) {
	if (state->dirty_lines[l]) {
	  memcpy(
		state->back_fb + EPD_WIDTH / 2 * l,
		state->front_fb + EPD_WIDTH / 2 * l,
		EPD_WIDTH / 2
	  );
	}
  }
  return err;
}


void epd_hl_set_all_white(EpdiyHighlevelState* state) {
  assert(state != NULL);
  memset(state->front_fb, 0xFF, fb_size);
}

void epd_fullclear(EpdiyHighlevelState* state, int temperature) {
  assert(state != NULL);
  epd_hl_set_all_white(state);
  enum EpdDrawError err = epd_hl_update_screen(state, MODE_GC16, temperature);
  assert(err == EPD_DRAW_SUCCESS);
  epd_clear();
}
