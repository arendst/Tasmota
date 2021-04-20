// Minimal "Hello" example for LittlevGL on Adafruit TFT FeatherWings.
// Requires LittlevGL, Adafruit_LvGL_Glue, Adafruit_STMPE610, Adafruit_GFX
// and Adafruit_ILI9341 (2.4" TFT) or Adafruit_HX8357 (3.5") libraries.
// This example doesn't use any touchscreen input, but it's declared anyway
// so this sketch can be copied-and-pasted to serve as a starting point for
// other projects. If display is scrambled, check that correct FeatherWing
// type is selected below (set BIG_FEATHERWING to 0 or 1 as needed).

// Prior Adafruit_LvGL_Glue users: see hello_changes example for updates!

#define BIG_FEATHERWING 0 // Set this to 1 for 3.5" (480x320) FeatherWing!

#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>
#include <Adafruit_STMPE610.h>

#ifdef ESP32
   #define TFT_CS   15
   #define TFT_DC   33
   #define STMPE_CS 32
#else
   #define TFT_CS    9
   #define TFT_DC   10
   #define STMPE_CS  6
#endif
#define TFT_ROTATION 1 // Landscape orientation on FeatherWing
#define TFT_RST     -1

#if BIG_FEATHERWING
  #include <Adafruit_HX8357.h>
  Adafruit_HX8357  tft(TFT_CS, TFT_DC, TFT_RST);
#else
  #include <Adafruit_ILI9341.h>
  Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
#endif

Adafruit_STMPE610  ts(STMPE_CS);
Adafruit_LvGL_Glue glue;

// This example sketch's LittlevGL UI-building calls are all in this
// function rather than in setup(), so simple programs can just 
// copy-and-paste this sketch as a starting point, then embellish here:
void lvgl_setup(void) {
  // Create simple label centered on screen
  lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(label, "Hello Arduino!");
  lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
}

void setup(void) {
  Serial.begin(115200);

  // Initialize display and touchscreen BEFORE glue setup
  tft.begin();
  tft.setRotation(TFT_ROTATION);
  if(!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    for(;;);
  }

  // Initialize glue, passing in address of display & touchscreen
  LvGLStatus status = glue.begin(&tft, &ts);
  if(status != LVGL_OK) {
    Serial.printf("Glue error %d\r\n", (int)status);
    for(;;);
  }

  lvgl_setup(); // Call UI-building function above
}

void loop(void) {
  lv_task_handler(); // Call LittleVGL task handler periodically
  delay(5);
}
