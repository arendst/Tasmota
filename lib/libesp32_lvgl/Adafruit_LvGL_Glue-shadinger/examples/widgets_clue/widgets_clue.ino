// A more functional example for LittlevGL on Adafruit CLUE. Lacking
// touchscreen input, interaction options are limited (but not impossible).
// In this case we'll pretend like it's a status display for something --
// the left and right buttons switch between three different tabs, each
// displaying different information. The code's a bit more complex than
// the hello_clue example, so best get that working before trying this.

// Prior Adafruit_LvGL_Glue users: see hello_changes example for updates!

#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>
#include <Adafruit_ST7789.h>

#define TFT_ROTATION   1 // Landscape orientation on CLUE
#define TFT_SPI     SPI1 // CLUE display peripheral & pins
#define TFT_CS        31
#define TFT_DC        32
#define TFT_RST       33
#define TFT_BACKLIGHT 34

Adafruit_ST7789    tft(&TFT_SPI, TFT_CS, TFT_DC, TFT_RST);
Adafruit_LvGL_Glue glue;

lv_obj_t          *tabview,        // LittlevGL tabview object
                  *gauge,          // Gauge object (on first of three tabs)
                  *chart,          // Chart object (second tab)
                  *canvas;         // Canvas object (third tab)
uint8_t            active_tab = 0, // Index of currently-active tab (0-2)
                   prev_tab   = 0; // Index of previously-active tab
lv_chart_series_t *series;         // 'Series' data for the bar chart

// Canvas object is an image in memory. Although LittlevGL supports several
// internal image formats, only the TRUE_COLOR variety allows drawing
// operations. Fortunately nRF52840 has gobs of RAM for this. The canvas
// buffer is global because it's referenced both in the setup and main loop.
#define CANVAS_WIDTH  200 // Dimensions in pixels
#define CANVAS_HEIGHT 150
lv_color_t canvas_buffer[
  LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
lv_draw_line_dsc_t draw_dsc; // Drawing style (for canvas) is similarly global
 
void lvgl_setup(void) {
  // Create a tabview object, by default this covers the full display.
  tabview = lv_tabview_create(lv_disp_get_scr_act(NULL), NULL);
  // The CLUE display has a lot of pixels and can't refresh very fast.
  // To show off the tabview animation, let's slow it down to 1 second.
  lv_tabview_set_anim_time(tabview, 1000);

  // Because they're referenced any time an object is drawn, styles need
  // to be permanent in scope; either declared globally (outside all
  // functions), or static. The styles used on tabs are never modified after
  // they're used here, so let's use static on those...
  static lv_style_t tab_style, tab_background_style, indicator_style;

  // This is the background style "behind" the tabs. This is what shows
  // through for "off" (inactive) tabs -- a vertical green gradient,
  // minimal padding around edges (zero at bottom).
  lv_style_init(&tab_background_style);
  lv_style_set_bg_color(&tab_background_style, LV_STATE_DEFAULT, lv_color_hex(0x408040));
  lv_style_set_bg_grad_color(&tab_background_style, LV_STATE_DEFAULT, lv_color_hex(0x304030));
  lv_style_set_bg_grad_dir(&tab_background_style, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
  lv_style_set_pad_top(&tab_background_style, LV_STATE_DEFAULT, 2);
  lv_style_set_pad_left(&tab_background_style, LV_STATE_DEFAULT, 2);
  lv_style_set_pad_right(&tab_background_style, LV_STATE_DEFAULT, 2);
  lv_style_set_pad_bottom(&tab_background_style, LV_STATE_DEFAULT, 0);
  lv_obj_add_style(tabview, LV_TABVIEW_PART_TAB_BG, &tab_background_style);

  // Style for tabs. Active tab is white with opaque background, inactive
  // tabs are transparent so the background shows through (only the white
  // text is seen). A little top & bottom padding reduces scrunchyness.
  lv_style_init(&tab_style);
  lv_style_set_pad_top(&tab_style, LV_STATE_DEFAULT, 3);
  lv_style_set_pad_bottom(&tab_style, LV_STATE_DEFAULT, 10);
  lv_style_set_bg_color(&tab_style, LV_STATE_CHECKED, LV_COLOR_WHITE);
  lv_style_set_bg_opa(&tab_style, LV_STATE_CHECKED, LV_OPA_100);
  lv_style_set_text_color(&tab_style, LV_STATE_CHECKED, LV_COLOR_GRAY);
  lv_style_set_bg_opa(&tab_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_text_color(&tab_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_add_style(tabview, LV_TABVIEW_PART_TAB_BTN, &tab_style);

  // Style for the small indicator bar that appears below the active tab.
  lv_style_init(&indicator_style);
  lv_style_set_bg_color(&indicator_style, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_size(&indicator_style, LV_STATE_DEFAULT, 5);
  lv_obj_add_style(tabview, LV_TABVIEW_PART_INDIC, &indicator_style);

  // Back to creating widgets...

  // Add three tabs to the tabview
  lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Gauge");
  lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Chart");
  lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Canvas");

  // And then add stuff in each tab...

  // The first tab holds a gauge. To keep the demo simple, let's just use
  // the default style and range (0-100). See LittlevGL docs for options.
  gauge = lv_gauge_create(tab1, NULL);
  lv_obj_set_size(gauge, 186, 186);
  lv_obj_align(gauge, NULL, LV_ALIGN_CENTER, 0, 0);

  // Second tab, make a chart...
  chart = lv_chart_create(tab2, NULL);
  lv_obj_set_size(chart, 200, 180);
  lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_COLUMN);
  // For simplicity, we'll stick with the chart's default 10 data points:
  series = lv_chart_add_series(chart, LV_COLOR_RED);
  lv_chart_init_points(chart, series, 0);
  // Make each column shift left as new values enter on right:
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

  // Third tab is a canvas, which we'll fill with random colored lines.
  // LittlevGL draw functions only work on TRUE_COLOR canvas.
  canvas = lv_canvas_create(tab3, NULL);
  lv_canvas_set_buffer(canvas, canvas_buffer,
    CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
  lv_obj_align(canvas, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_canvas_fill_bg(canvas, LV_COLOR_WHITE, LV_OPA_100);

  // Set up canvas line-drawing style based on defaults.
  // Later we'll change color settings when drawing each line.
  lv_draw_line_dsc_init(&draw_dsc);
}

void setup(void) {
  Serial.begin(115200);

  // Initialize display BEFORE glue setup
  tft.init(240, 240);
  tft.setRotation(TFT_ROTATION);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  // Initialize glue, passing in address of display
  LvGLStatus status = glue.begin(&tft);
  if(status != LVGL_OK) {
    Serial.printf("Glue error %d\r\n", (int)status);
    for(;;);
  }

  lvgl_setup(); // Call UI-building function above

  // Enable "A" and "B" buttons as inputs
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
}

uint32_t prev_time = -1;

void loop(void) {

  // Read left/right button inputs. Debouncing could be done better,
  // but this'll do for a quick simple demo...
  if(digitalRead(PIN_BUTTON1) == LOW) {
    if(active_tab > 0) {
      active_tab--;
    }
    while(digitalRead(PIN_BUTTON1) == LOW); // Wait for button release
  } else if(digitalRead(PIN_BUTTON2) == LOW) {
    if(active_tab < 2) {
      active_tab++;
    }
    while(digitalRead(PIN_BUTTON2) == LOW); // Wait for button release
  }

  // Change active tab if button pressings happened
  if(active_tab != prev_tab) {
    lv_tabview_set_tab_act(tabview, active_tab, true);
    prev_tab = active_tab;
  }

  // Make the gauge sweep a full sine wave over time
  lv_gauge_set_value(gauge, 0, (int)(50.5 + sin(millis() / 1000.0) * 50.0));

  // About 2X a second, things happen on the other two tabs...
  uint32_t new_time = millis() / 500; // Current half-second
  if(new_time != prev_time) {         // freshly elapsed
    prev_time = new_time;

    // Add a new random item to the bar chart (old value shift left)
    lv_chart_set_next(chart, series, random(100));
    lv_chart_refresh(chart);

    // Add a random line to the canvas
    lv_point_t points[2];
    points[0].x = random(CANVAS_WIDTH);
    points[0].y = random(CANVAS_HEIGHT);
    points[1].x = random(CANVAS_WIDTH);
    points[1].y = random(CANVAS_HEIGHT);
    draw_dsc.color.ch.red   = random();
    draw_dsc.color.ch.green = random();
    draw_dsc.color.ch.blue  = random();
    lv_canvas_draw_line(canvas, points, 2, &draw_dsc);
    // This forces the canvas to update (otherwise changes aren't
    // seen unless leaving and returning to the canvas tab):
    lv_canvas_set_buffer(canvas, canvas_buffer,
      CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
  }

  lv_task_handler(); // Call LittleVGL task handler periodically
  delay(5);
}
