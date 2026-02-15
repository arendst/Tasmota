/*
  xsns_81_seesaw_encoder - Adafruit I2C QT Rotary Encoder support for Tasmota

  Copyright (C) 2025  Allen Schober

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

#ifdef USE_I2C
#ifdef USE_SEESAW_ENCODER

/*********************************************************************************************\
 * SEESAW_ENCODER - Adafruit I2C QT Rotary Encoder with NeoPixel and Button
 *
 * I2C Address: 0x36, 0x37, 0x38, 0x39 (though Adafruit hw is configurable from 0x36 to 0x3D)
 *
 * The Adafruit I2C QT Rotary Encoder features:
 *   - Quadrature rotary encoder with detents
 *   - Push button (seesaw pin 24)
 *   - RGB NeoPixel LED (seesaw pin 6)
 *   - Controlled via seesaw firmware over I2C
 *
 * Implementation Note:
 *   Add #define SEESAW_ENCODER_LIKE_ROTARY to have driver follow the same patterns
 *   as tasmota_support/support_rotary.ino (ROTARY_V1) and to have consistent behavior
 *   across GPIO rotary encoders and this I2C rotary encoder. See support_rotary.ino
 *   for rotary encoder operation. Add #define SEESAW_ENCODER_HIDE_WEB_DISPLAY to not show
 *   state in web UI if using USE_WEBSERVER (state will still be available in JSON and rules).
 *
 * Device Numbering:
 *   - Encoders are numbered based on detection order by default (SeeEnc-1, SeeEnc-2, etc.)
 *   - Detection scans addresses 0x36-0x39 in order
 *   - A single encoder at any address will always be named SeeEnc (no number)
 *   - #define SEESAW_ENCODER_PERSISTENT_NAMING to use I2C address-based naming
 *     (e.g., SeeEnc-36 instead of SeeEnc-1) for consistent naming across restarts
 *
 * Commands:
 *   - SeeEncSet<x> <position> - Set encoder position (e.g., SeeEncSet1 0)
 *   - SeeEncColor<x> <rrggbb> - Set NeoPixel color in hex (e.g., SeeEncColor1 FF0000 for red)
 *   - SeeEncColor<x> <r>,<g>,<b> - Set NeoPixel color as RGB values (e.g., SeeEncColor1 255,0,0)
 * 
 * Light Control (when USE_LIGHT enabled):
 *   - First two encoders detected control lights similar to GPIO rotary encoders
 *   - SeeEnc1 (first encoder detected):
 *     * Button released: Dimmer control (RGB or all channels)
 *     * Button pressed: Color (RGB) or Color Temperature control
 *   - SeeEnc2 (second encoder detected):
 *     * Button released: Dimmer CW control
 *     * Button pressed: Color Temperature control
 *   - Configuration via existing SetOptions:
 *     * SetOption43 (steps): Change Rotary Max Steps (default 10)
 *     * SetOption98 (0/1): Direct light control (0, default) or rules mode (1)
 *     * SetOption113 (0/1): Power on with low dimmer when rotated while off
 *
 * Button Behavior:
 *   - Click alone (no rotation): Toggle relay (1st encoder -> relay 1, 2nd encoder -> relay 2)
 *   - Press during rotation: Modify rotation behavior (color/CT instead of dimmer). Clicks
 *     during rotation are ignored to prevent unwanted toggles.
\*********************************************************************************************/

// Have Seesaw I2C Encoder behave like a GPIO Rotary Encoder for light control and button handling
// #define SEESAW_ENCODER_LIKE_ROTARY

// Encoder Pin configuration
#define SEESAW_ENCODER_BUTTON_PIN      24
#define SEESAW_ENCODER_NEOPIXEL_PIN    6

#define SEESAW_ENCODER_TIMEOUT         2                 // 2 * Handler() call which is usually 2 * 0.05 seconds

struct SeesawEncoder : public SeesawDevice {
  SeesawEncoder(uint8_t addr) : SeesawDevice(addr),
    position(0), delta(0), pixel_color(0),
    timeout(0), rel_position(0), flags(0) {
    type = SEESAW_TYPE_ENCODER;
    abs_position[0] = 0;
    abs_position[1] = 0;
  }

  virtual void Init() override {
    // Enable encoder interrupt
    Seesaw::Write8(address, SEESAW_ENCODER_BASE, SEESAW_ENCODER_INTENSET, 0x01);

    // Set initial encoder position to 0
    SetEncoderPosition(0);

    // Configure button pin (24) as input with pullup
    uint32_t pin_mask = (uint32_t)1 << SEESAW_ENCODER_BUTTON_PIN;
    // Convert 32-bit pin mask to 4-byte buffer (big-endian)
    uint8_t pin_buf[4] = {
      (uint8_t)(pin_mask >> 24),
      (uint8_t)(pin_mask >> 16),
      (uint8_t)(pin_mask >> 8),
      (uint8_t)(pin_mask & 0xFF)
    };
    // Set as input (clear direction bit)
    Seesaw::Write(address, SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, pin_buf, 4);
    // Enable pullup
    Seesaw::Write(address, SEESAW_GPIO_BASE, SEESAW_GPIO_PULLENSET, pin_buf, 4);
    // Set pin high (for pullup)
    Seesaw::Write(address, SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_SET, pin_buf, 4);

    // Check if NeoPixel module is available (bit 14 = 0x4000)
    //   if (options & (1UL << SEESAW_NEOPIXEL_BASE)) {
    //     AddLog(LOG_LEVEL_INFO, PSTR("SEE: NeoPixel module IS available"));
    //   } else {
    //     AddLog(LOG_LEVEL_ERROR, PSTR("SEE: NeoPixel module NOT available in firmware!"));
    //   }
    // }

    // NeoPixel Init Step 1: updateType - Set speed to 800KHz (NEO_KHZ800 = 0x0000, so is800KHz = true = 1)
    bool speed_ok = Seesaw::Write8(address, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_SPEED, 1);

    // NeoPixel Init Step 2: updateLength - Set buffer length to numBytes
    uint16_t num_bytes = 1 * 3;  // 1 pixel * 3 bytes = 3 bytes
    uint8_t len_buf[2] = {
      (uint8_t)(num_bytes >> 8),    // high byte
      (uint8_t)(num_bytes & 0xFF)   // low byte
    };
    bool len_ok = Seesaw::Write(address, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF_LENGTH, len_buf, 2);

    // NeoPixel Init Step 3: setPin - Set the NeoPixel output pin to 6
    bool pin_ok = Seesaw::Write8(address, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_PIN, SEESAW_ENCODER_NEOPIXEL_PIN);

    // NeoPixel Init Step 4: Initialize NeoPixel to off
    SetPixelColor(0x000000);

    // Read initial encoder state
    position = GetEncoderPosition();

    // Read initial button state
    if (GetButton()) {
      flags |= FLAG_BUTTON | FLAG_BUTTON_PREV;
    }

    valid = true;

#ifdef DEBUG_SEESAW_ENCODER
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: Init Encoder ADDR=%02X POS=%d BTN=%d PXL_SPD_OK=%d PXL_LEN_OK=%d PXL_PIN_OK=%d"),
           address, position, (flags & FLAG_BUTTON) ? 1 : 0, speed_ok, len_ok, pin_ok);
#endif

#if defined(SEESAW_ENCODER_LIKE_ROTARY) && defined(USE_LIGHT)
    // Initialize rotary settings if needed
    InitRotarySettings();
#endif // SEESAW_ENCODER_LIKE_ROTARY && USE_LIGHT
  }

  virtual void Read() override {
    // Read encoder delta (change since last read)
    delta = GetEncoderDelta();

    // Read encoder position
    position = GetEncoderPosition();

    // Read button state
    bool button_was_pressed = (flags & FLAG_BUTTON);
    bool button_now_pressed = GetButton();
    flags &= ~(FLAG_BUTTON | FLAG_BUTTON_PREV);  // Clear both flags
    if (button_was_pressed) { flags |= FLAG_BUTTON_PREV; }  // set previous state
    if (button_now_pressed) { flags |= FLAG_BUTTON; }  // set new current state

#ifdef DEBUG_SEESAW_ENCODER
    if (delta != 0 || (flags & FLAG_BUTTON) != (flags & FLAG_BUTTON_PREV)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: READ ADDR=%02X POS=%d DELTA=%d BTN=%d"),
             address, position, delta, (flags & FLAG_BUTTON) ? 1 : 0);
    }
#endif
  }

  virtual void Handler() override {
    // Handler logic mirrors support_rotary.ino RotaryHandler() (lines 195-278)
    // to provide consistent behavior between GPIO and I2C rotary encoders

    if (timeout) {
      timeout--;
      if (!timeout) {
#ifdef USE_LIGHT
        if (!Settings->flag4.rotary_uses_rules) {  // SetOption98 - Use rules instead of light control
          ResponseLightState(0);
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_STATE));
        }
#endif  // USE_LIGHT
      }
    }

    // Use stored button state (read once in Read())
    bool button_current = !!(flags & FLAG_BUTTON);  // Convert to boolean 0 or 1
    bool button_prev = !!(flags & FLAG_BUTTON_PREV); // Convert to boolean 0 or 1

    // Reset changed flag when button released
    if (button_prev && !button_current) {
      if (flags & FLAG_CHANGED) {
        flags &= ~FLAG_CHANGED;
      }
    }

    // Check for rotation or button change
    if (delta == 0 && button_current == button_prev) { return; }

    timeout = SEESAW_ENCODER_TIMEOUT;   // Prevent fast direction changes within 100ms

    int32_t current_delta = delta;
    delta = 0;  // Clear delta after reading

    // Postpone flash writes during rapid rotation
    // Mirrors support_rotary.ino line 218-220
    if (Settings->save_data && (TasmotaGlobal.save_data_counter < 2)) {
      TasmotaGlobal.save_data_counter = 3;
    }

    // Button is pressed: set color temperature
    if (button_current) { flags |= FLAG_CHANGED; }

#ifdef SEESAW_ENCODER_LIKE_ROTARY
    // Button click handling - toggle relay on a click without rotation
    // On button click - track if rotation occurs
    if (current_delta != 0 && button_current) {
      flags |= FLAG_ROTATION_OCCURRED;
    }
    // On button release - check if it was just a click (no rotation)
    if (button_prev && !button_current) {
      if (!(flags & FLAG_CHANGED) && !(flags & FLAG_ROTATION_OCCURRED)) {
        // Button released without being used for rotation - toggle relay
        uint8_t relay_index = device_index + 1;  // device_index is 0-based
        ExecuteCommandPower(relay_index, POWER_TOGGLE, SRC_BUTTON);
        // Early return to prevent light control code from interfering with toggle
        return;
      }
      // Reset rotation flag for next button press
      flags &= ~FLAG_ROTATION_OCCURRED;
    }

#ifdef USE_LIGHT
    // Light control (only first 2 encoders detected)
    // Mirrors support_rotary.ino lines 227-254 (inline logic matching GPIO rotary behavior)
    if (device_index < 2 && !Settings->flag4.rotary_uses_rules) {  // SetOption98 - Use rules instead of light control
      // Check if second encoder exists
      // Matches support_rotary.ino line 228: bool second_rotary = (Encoder[1].pinb >= 0);
      bool second_encoder = (SeesawMgr.GetTypeCount(SEESAW_TYPE_ENCODER) > 1);

      if (device_index == 0) {  // First encoder (lines 229-247 in support_rotary.ino)
        if (button_current) {
          // Color or CT control
          if (second_encoder) {
            // With second encoder: control color only
            LightColorOffset(current_delta * Rotary.color_increment);
          } else {
            // Without second encoder: try CT, fallback to color
            if (!LightColorTempOffset(current_delta * Rotary.ct_increment)) {
              LightColorOffset(current_delta * Rotary.color_increment);
            }
          }
        } else {
          // Dimmer RGBCW or RGB only if second rotary
          uint32_t dimmer_index = second_encoder ? 1 : 0;
          if (!Settings->flag4.rotary_poweron_dimlow || TasmotaGlobal.power) {  // SetOption113
            LightDimmerOffset(dimmer_index, current_delta * Rotary.dimmer_increment);
          } else {
            if (current_delta > 0) {  // Only power on if rotary increase
              LightDimmerOffset(dimmer_index, -LightGetDimmer(dimmer_index) + ROTARY_START_DIM);
            }
          }
        }
      } else {  // Second encoder (lines 248-254 in support_rotary.ino)
        if (button_current) {
          // Color Temperature
          LightColorTempOffset(current_delta * Rotary.ct_increment);
        } else {
          // Dimmer CW
          LightDimmerOffset(2, current_delta * Rotary.dimmer_increment);
        }
      }
      return;  // Skip rules processing for light control mode
    }
#endif  // USE_LIGHT
#endif  // SEESAW_ENCODER_LIKE_ROTARY

    // Position tracking and rules (when not in direct light control mode)
    // Mirrors support_rotary.ino lines 257-273
    abs_position[button_current] += current_delta;
    if (abs_position[button_current] < 0) {
      abs_position[button_current] = 0;
    }
    if (abs_position[button_current] > Settings->param[P_ROTARY_MAX_STEP]) {
      abs_position[button_current] = Settings->param[P_ROTARY_MAX_STEP];
    }

    rel_position += current_delta;
    if (rel_position > Settings->param[P_ROTARY_MAX_STEP]) {
      rel_position = Settings->param[P_ROTARY_MAX_STEP];
    }
    if (rel_position < -(Settings->param[P_ROTARY_MAX_STEP])) {
      rel_position = -(Settings->param[P_ROTARY_MAX_STEP]);
    }

#ifdef DEBUG_SEESAW_ENCODER
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: %s btn=%d delta=%d abs_position[0]=%d abs_position[1]=%d, rel_position=%d"),
           device_name, button_current, current_delta,
           abs_position[0], abs_position[1], rel_position);
#endif

    // Trigger rules (when not in direct light control mode)
    Response_P(PSTR("{\"%s\":{\"Pos1\":%d,\"Pos2\":%d,\"Button\":%d}}"),
               device_name,
               abs_position[0],
               abs_position[1],
               button_current);
    XdrvRulesProcess(0);
  }

  virtual void Show(bool json, const char *name) override {
    // Store name for use in Handler() and debug logging
    strlcpy(device_name, name, sizeof(device_name));

    // Use stored button state (read once in Read())
    bool button_state = (flags & FLAG_BUTTON);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"Pos1\":%d,\"Pos2\":%d,\"Button\":%d,\"Color\":\"%06X\"}"),
        name, abs_position[0], abs_position[1],
        button_state, pixel_color);
#if defined(USE_WEBSERVER) && !defined(SEESAW_ENCODER_HIDE_WEB_DISPLAY)
    } else {
      WSContentSend_PD(PSTR("{s}%s Pos1{m}%d{e}"), name, abs_position[0]);
      WSContentSend_PD(PSTR("{s}%s Pos2{m}%d{e}"), name, abs_position[1]);
      WSContentSend_PD(PSTR("{s}%s Button{m}%d{e}"), name, button_state);
      WSContentSend_PD(PSTR("{s}%s Color{m}#%06X{e}"), name, pixel_color);
#endif // USE_WEBSERVER && !SEESAW_ENCODER_HIDE_WEB_DISPLAY
    }
  }

  virtual bool HandleCommand(const char* cmd, uint32_t len) override {
    // Commands: Set<x> <position>, Color<x> <rrggbb> or <r>,<g>,<b>
    // This is called from the manager with cmd already pointing to the command
    return false;  // Commands handled via Tasmota command interface
  }

  bool SetEncoderPosition(int32_t pos) {
    uint8_t buf[4] = {
      (uint8_t)(pos >> 24),
      (uint8_t)(pos >> 16),
      (uint8_t)(pos >> 8),
      (uint8_t)(pos & 0xFF)
    };
    bool success = Seesaw::Write(address, SEESAW_ENCODER_BASE, SEESAW_ENCODER_POSITION, buf, 4);

    if (success) {
      position = pos;
    }

#ifdef DEBUG_SEESAW_ENCODER
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: WRITE ADDR=%02X val=%d success=%d"),
           address, pos, success);
#endif

    return success;
  }

  bool SetPixelColor(uint32_t color) {
    // Set NeoPixel buffer: pixel index (2 bytes) + GRB color data (3 bytes)
    uint8_t buf[5] = {
      0,                          // index high byte
      0,                          // index low byte
      (uint8_t)(color >> 8),      // G
      (uint8_t)(color >> 16),     // R
      (uint8_t)(color & 0xFF)     // B
    };

    if (!Seesaw::Write(address, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_BUF, buf, 5)) {
      return false;
    }

    // Show the pixel
    bool success = Seesaw::Write(address, SEESAW_NEOPIXEL_BASE, SEESAW_NEOPIXEL_SHOW, nullptr, 0);
    if (success) {
      pixel_color = color;
    }

#ifdef DEBUG_SEESAW_ENCODER
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: COLOR ADDR=%02X color=%06X success=%d"),
           address, color, success);
#endif

    return success;
  }

  static const char id[] PROGMEM;

private:
  int32_t GetEncoderPosition() {
    uint8_t buf[4];
    if (!Seesaw::Read(address, SEESAW_ENCODER_BASE, SEESAW_ENCODER_POSITION, buf, 4)) {
      return 0;
    }
    return ((int32_t)buf[0] << 24) | ((int32_t)buf[1] << 16) | ((int32_t)buf[2] << 8) | (int32_t)buf[3];
  }

  int32_t GetEncoderDelta() {
    uint8_t buf[4];
    if (!Seesaw::Read(address, SEESAW_ENCODER_BASE, SEESAW_ENCODER_DELTA, buf, 4)) {
      return 0;
    }
    return ((int32_t)buf[0] << 24) | ((int32_t)buf[1] << 16) | ((int32_t)buf[2] << 8) | (int32_t)buf[3];
  }

  bool GetButton() {
    uint8_t buf[4];
    if (!Seesaw::Read(address, SEESAW_GPIO_BASE, SEESAW_GPIO_BULK, buf, 4)) {
      return false;
    }
    uint32_t gpio_value = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                          ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    // Button is on pin 24, active low with pullup
    return !(gpio_value & ((uint32_t)1 << SEESAW_ENCODER_BUTTON_PIN));
  }

#if defined(SEESAW_ENCODER_LIKE_ROTARY) && defined(USE_LIGHT)
  void InitRotarySettings() {
    #ifdef ROTARY_V1
    if (Rotary.present) { return; }  // GPIO rotaries already initialized their settings
    #endif

    // No GPIO Rotary present, initialize for Seesaw Encoders
    RotaryInitMaxSteps();
  }
#endif  // SEESAW_ENCODER_LIKE_ROTARY && USE_LIGHT

  // Flag bit definitions
  static const uint8_t FLAG_CHANGED = 0x01;
  static const uint8_t FLAG_ROTATION_OCCURRED = 0x02;
  static const uint8_t FLAG_BUTTON = 0x04;
  static const uint8_t FLAG_BUTTON_PREV = 0x08;

  int32_t position;
  int32_t delta;
  uint32_t pixel_color;
  uint8_t timeout;
  int8_t abs_position[2];
  int8_t rel_position;
  uint8_t flags;  // Bit 0: changed, Bit 1: rotation_occurred, Bit 2: button, Bit 3: button_previous
};

const char SeesawEncoder::id[] PROGMEM = "ENCODER";

// Factory function implementation
SeesawDevice* SeesawManager::CreateEncoderDevice(uint8_t addr) {
  return new SeesawEncoder(addr);
}

// Helper function to find encoder by command index
// Returns nullptr if index is out of range
// Sets total_count to the total number of encoders found
SeesawEncoder* GetEncoderByIndex(uint8_t cmd_index, uint8_t* total_count = nullptr) {
  uint8_t encoder_index = 0;
  SeesawEncoder* target_encoder = nullptr;

  // Find encoder and count simultaneously
  for (uint8_t i = 0; i < SeesawMgr.GetCount(); i++) {
    SeesawDevice* dev = SeesawMgr.GetDevice(i);
    if (dev && dev->GetType() == SEESAW_TYPE_ENCODER) {
      encoder_index++;
      // Capture the target encoder if index matches
      if (encoder_index == cmd_index && !target_encoder) {
        target_encoder = static_cast<SeesawEncoder*>(dev);
        // Continue counting if total_count is requested
        if (!total_count) {
          return target_encoder;  // Early exit if count not needed
        }
      }
    }
  }

  if (total_count) {
    *total_count = encoder_index;
  }

  // Return encoder if cmd_index was valid (>= 1 and <= encoder_index)
  return (cmd_index >= 1 && cmd_index <= encoder_index) ? target_encoder : nullptr;
}

// Command handlers
#define D_PRFX_SEEENC "SeeEnc"
#define D_CMND_SEEENC_SET "Set"
#define D_CMND_SEEENC_COLOR "Color"

const char kSeeEncCommands[] PROGMEM = D_PRFX_SEEENC "|"
  D_CMND_SEEENC_SET "|" D_CMND_SEEENC_COLOR;

void CmndSeeEncSet(void) {
  // Command format: SeeEncSet<x> <position>
  uint8_t encoder_count = 0;
  SeesawEncoder* encoder = GetEncoderByIndex(XdrvMailbox.index, &encoder_count);

  if (!encoder) {
    ResponseCmndIdxNumber(encoder_count);
    return;
  }

  if (encoder->SetEncoderPosition(XdrvMailbox.payload)) {
    ResponseCmndNumber(XdrvMailbox.payload);
  } else {
    ResponseCmndFailed();
  }
}

void CmndSeeEncColor(void) {
  // Command format: SeeEncColor<x> <rrggbb> or <r>,<g>,<b>
  uint8_t encoder_count = 0;
  SeesawEncoder* encoder = GetEncoderByIndex(XdrvMailbox.index, &encoder_count);

  if (!encoder) {
    ResponseCmndIdxNumber(encoder_count);
    return;
  }

  uint32_t color = 0;

  // Check if input is hex format (RRGGBB) or comma-separated (R,G,B)
  if (strchr(XdrvMailbox.data, ',')) {
    // Parse R,G,B format
    uint8_t r, g, b;
    if (sscanf(XdrvMailbox.data, "%hhu,%hhu,%hhu", &r, &g, &b) == 3) {
      color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    } else {
      ResponseCmndError();
      return;
    }
  } else {
    // Parse hex format
    color = strtoul(XdrvMailbox.data, nullptr, 16);
  }

  if (encoder->SetPixelColor(color)) {
    ResponseCmndIdxChar(XdrvMailbox.data);
  } else {
    ResponseCmndFailed();
  }
}

void (* const SeeEncCommand[])(void) PROGMEM = {
  &CmndSeeEncSet,
  &CmndSeeEncColor };

#endif  // USE_SEESAW_ENCODER
#endif  // USE_I2C
