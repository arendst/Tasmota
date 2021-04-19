// A more interesting example for LittlevGL on Adafruit TFT FeatherWings,
// showing use of the touchscreen. Code's a little more complex than the
// hello_featherwing example, so best get that working before trying this.
// By default, as written, on a 320x240 TFT FeatherWing the example is a
// pretend calculator keypad, while 480x320 TFT has a whole keyboard
// (though you'll probably need a stylus). These just seemed the right
// level of detail for their respective screens, but feel free to override
// and try either for yourself. If display is scrambled, check that
// correct FeatherWing type is selected below (set BIG_FEATHERWING to 0
// or 1 as needed).

// Prior Adafruit_LvGL_Glue users: see hello_changes example for updates!

// Feather M0 is no longer recommended for LittlevGL use. The calculator
// almost works (hangs after a few button presses) and keyboard won't run
// at all. A Feather M4 or other >32K RAM device is recommended.

#define BIG_FEATHERWING 0 // Set this to 1 for 3.5" (480x320) FeatherWing!

#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>
#include <Adafruit_STMPE610.h>

#define DEMO_CALC 0
#define DEMO_TEXT 1

#ifdef ESP32
   #define TFT_CS   15
   #define TFT_DC   33
   #define STMPE_CS 32
#else
   #define TFT_CS    9
   #define TFT_DC   10
   #define STMPE_CS  6
#endif
#define TFT_RST     -1

#if BIG_FEATHERWING
  #include <Adafruit_HX8357.h>
  Adafruit_HX8357  tft(TFT_CS, TFT_DC, TFT_RST);
  #define DEMO DEMO_TEXT // On big TFT, do text/keyboard example
#else
  #include <Adafruit_ILI9341.h>
  Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
  #define DEMO DEMO_CALC // Smaller TFT, do keypad example
#endif

Adafruit_STMPE610  ts(STMPE_CS);
Adafruit_LvGL_Glue glue;

#if DEMO == DEMO_CALC

// "Pretend" calculator example. Please, PLEASE...do NOT implement the whole
// calculator and submit as a pull request, because it will NOT be merged!
// This sketch is meant only to be illustrative and not functional, just
// showing LittlevGL + Adafruit display/touch tied together with a modest
// amount of code. Even a simple but working calc would require WAY more
// code, distracting from that core idea (and is a waste of hardware).
// Daiso has better calculators for $1.50.

#define TFT_ROTATION 0 // Portrait orientation

lv_obj_t   *digits_label = NULL; // LittlevGL label object showing digits
String      digits     = "0";    // Current digits string value
bool        hasDecimal = false;  // Only allow one decimal point
const char *buttons[]  = {       // Button matrix labels
  "7", "8", "9", "/", "\n",
  "4", "5", "6", "x", "\n",
  "1", "2", "3", "-", "\n",
  "0", ".", "=", "+", "" };

// This function processes events from the button matrix
void button_event_handler(lv_obj_t *obj, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED) {
    const char *txt = lv_btnmatrix_get_active_btn_text(obj);
    if(txt) { // NULL if pressed in frame area outside buttons
      if(txt[0] == '.') {
        // Decimal button pressed. Add decimal point to "digits" string
        // if it's not too long and there's no decimal present yet...
        if((digits.length() < 15) && !hasDecimal) {
          digits    += '.';
          hasDecimal = true;
        }
      } else if((txt[0] >= '0') && (txt[0] <= '9')) {
        // Number button (0-9) pressed. If there's nothing currently
        // being displayed, take the digit literally. Otherwise, append
        // the new digit if the "digits" string is not too long.
        if(digits.equals("0")) {
          digits = txt[0];
        } else if(digits.length() < 15) {
          digits += txt[0];
        }
      } else {
        // Any other button, just reset the calculator display.
        // It's all just pretend.
        digits     = "0";
        hasDecimal = false;
      }
      if(digits_label != NULL) {
        lv_label_set_text(digits_label, digits.c_str());
      }
    }
  }
}

void lvgl_setup(void) {

  // Just using the default styles for everything here, keeping
  // it basic. See the official LittlevGL docs and examples for
  // insights on applying styles.

  // The calculator digits are held inside a LvGL container object
  // as this gives us a little more control over positioning.
  lv_obj_t *container = lv_cont_create(lv_scr_act(), NULL);
  lv_cont_set_fit(container, LV_FIT_NONE); // Don't auto fit
  lv_obj_set_size(container, tft.width(), 50); // Full width x 50 px

  // Calculator digits are just a text label inside the container,
  // refreshed whenever the global "digits" string changes.
  digits_label = lv_label_create(container, NULL);
  lv_label_set_text(digits_label, digits.c_str());
  lv_obj_align(digits_label, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 10);
  lv_label_set_long_mode(digits_label, LV_LABEL_LONG_CROP);
  lv_obj_set_size(digits_label, tft.width() - 40, 30);
  lv_label_set_align(digits_label, LV_LABEL_ALIGN_RIGHT);

  // Fill the remaining space with the button matrix.
  lv_obj_t *button_matrix = lv_btnmatrix_create(lv_scr_act(), NULL);
  lv_btnmatrix_set_map(button_matrix, buttons);
  lv_obj_align(button_matrix, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 50);
  lv_obj_set_size(button_matrix, tft.width(), tft.height() - 50);
  lv_obj_set_event_cb(button_matrix, button_event_handler);
}

#else // Keyboard demo

// Keyboard example, lets you enter and edit text in a field. Even on a
// 480x320 TFT it requires a stylus to be even half useful (fingertip
// is possible if very patient), but having the option of a keyboard at
// all on this device is pretty nifty!

#define TFT_ROTATION 1 // Landscape orientation

lv_obj_t  *textarea,
          *keyboard = NULL; // Created/deleted as needed

#if LV_USE_ANIMATION
// Called after keyboard slides closed - deletes keyboard object
void delete_keyboard(lv_anim_t * a) {
  lv_obj_del((lv_obj_t *)a->var);
  keyboard = NULL;
}
#endif

// Called when the close or ok button is pressed on the keyboard
void keyboard_event_handler(lv_obj_t *obj, lv_event_t event) {
  lv_keyboard_def_event_cb(keyboard, event);

  if(event == LV_EVENT_APPLY || event == LV_EVENT_CANCEL) {
#if LV_USE_ANIMATION
    // If animation is enabled, make keyboard slide away
    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y); 
    lv_anim_set_var(&a, keyboard);
    lv_anim_set_time(&a, 300);
    lv_anim_set_values(&a, lv_obj_get_y(keyboard), LV_VER_RES); // start, end
    lv_anim_set_path(&a, &path);
    lv_anim_set_ready_cb(&a, delete_keyboard);
    lv_anim_start(&a);
#else
    lv_obj_del(keyboard);
    keyboard = NULL;
#endif
  }
}

// Other clicks in the text area
void text_area_event_handler(lv_obj_t *obj, lv_event_t event) {
  if(event == LV_EVENT_CLICKED) {

    // Unsure why, but text area has an initial clicked event on
    // creation, causing the keyboard to appear. This is a hacky
    // workaround that just ignores the first click event, so
    // subsequent actual clicks bring up the keyboard.
    static bool first = true;
    if(first) {
      first = false;
      return;
    }

    if(keyboard == NULL) {
      // If not present, create keyboard object at bottom of screen
      keyboard = lv_keyboard_create(lv_scr_act(), NULL);
      lv_obj_set_size(keyboard, tft.width(), tft.height() * 7 / 16);
      lv_obj_align(keyboard, textarea, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
      lv_keyboard_set_textarea(keyboard, textarea);
      lv_keyboard_set_cursor_manage(keyboard, true);
      lv_obj_set_event_cb(keyboard, keyboard_event_handler);

#if LV_USE_ANIMATION
      // If animation is enabled, make keyboard slide into place
      lv_anim_path_t path;
      lv_anim_path_init(&path);
      lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
      lv_anim_t a;
      lv_anim_init(&a);
      lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y); 
      lv_anim_set_var(&a, keyboard);
      lv_anim_set_time(&a, 300);
      lv_anim_set_values(&a, LV_VER_RES, lv_obj_get_y(keyboard)); // start, end
      lv_anim_set_path(&a, &path);
      lv_anim_start(&a);
#endif
    }
  }
}

void lvgl_setup(void) {
  textarea = lv_textarea_create(lv_scr_act(), NULL);
  lv_obj_set_size(textarea, LV_HOR_RES, LV_VER_RES); // Whole screen
  lv_obj_align(textarea, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_textarea_set_text(textarea, "This text is editable.");
  lv_obj_set_event_cb(textarea, text_area_event_handler);
  lv_textarea_set_cursor_pos(textarea, LV_TEXTAREA_CURSOR_LAST);
}

#endif // end calculator / keyboard examples

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

// NOTE TO FUTURE SELF: this sketch is largely similar to the PyPortal
// widgets example. If updating/bugfixing one sketch, make sure the other
// is kept in sync.
