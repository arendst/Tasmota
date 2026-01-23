#include "uDisplay.h"
#include "uDisplay_config.h"

void udisp_bpwr(uint8_t on);

void udisp_dimm(uint8_t dim);

// input value is 0..15
// void uDisplay::dim(uint8_t dim) {
//   dim8(((uint32_t)dim * 255) / 15);
// }

// ===== Power Management =====
#define AW_PWMRES 1024

void uDisplay::HandeBP(int8_t on) {

    if (bpanel < 0) {
        return;
    }
    if (on < 0) {
        // initial default
#ifdef ESP32
        if (!bp_mode.bp_nopwm) {
          analogWrite(bpanel, 32);
        } else {
          pinMode(bpanel, OUTPUT);
          digitalWrite(bpanel, HIGH);         
        }
#else
        pinMode(bpanel, OUTPUT);
        digitalWrite(bpanel, HIGH);
#endif // ESP32
        return;
    }
#ifdef ESP32
    if (on) { 
        if (!bp_mode.bp_nopwm) {
            if (!bp_mode.bp_invert) {
                analogWrite(bpanel, dimmer10_gamma);
            } else {
                analogWrite(bpanel, AW_PWMRES - dimmer10_gamma);
            }
        } else {
            if (!bp_mode.bp_invert) {
                digitalWrite(bpanel, HIGH);
            } else {
                digitalWrite(bpanel, LOW);
            }
        }
    } else {
        if (!bp_mode.bp_nopwm) {
            if (!bp_mode.bp_invert) {
                analogWrite(bpanel, 0);
            }   else {
                analogWrite(bpanel, AW_PWMRES - 1);
            }
        } else {
             if (!bp_mode.bp_invert) {
                digitalWrite(bpanel, LOW);
            } else {
                digitalWrite(bpanel, HIGH);
            }           
        }
    }
#else
    if (!bp_mode.bp_invert) {
        digitalWrite(bpanel, on);
    } else {
        digitalWrite(bpanel, !on);
    }
#endif
}

void uDisplay::DisplayOnff(int8_t on) {
    if (ep_mode) {
        return;
    }
    if (pwr_cbp) {
        pwr_cbp(on);
    }
    if (universal_panel->displayOnff(on)) {
        return;
    }
    HandeBP(on);
}

// ===== Brightness/Dimming Control =====
// dim is 0..255
void uDisplay::dim10(uint8_t dim, uint16_t dim_gamma) {
    dimmer8 = dim;
    dimmer10_gamma = dim_gamma;
    
    if (ep_mode) {
        return;
    }

#ifdef ESP32
    if (bpanel >= 0) {
        if (bp_mode.bp_nopwm) {
            return;
        }
        if (!bp_mode.bp_invert) {
            analogWrite(bpanel, dimmer10_gamma);
        } else {
            analogWrite(bpanel, AW_PWMRES - dimmer10_gamma);
        }
    } else if (dim_cbp) {
        dim_cbp(dim);
    }
#endif
    
    if (interface == _UDSP_SPI) {
        if (dim_op != 0xff) {
            spiController->beginTransaction();
            spiController->csLow();
            spiController->writeCommand(dim_op);
            spiController->writeData8(dimmer8);
            spiController->csHigh();
            spiController->endTransaction();
        }
    }
}

// ===== Display Inversion =====

void uDisplay::invertDisplay(boolean i) {
    if (universal_panel) {
        universal_panel->invertDisplay(i);
    }
}

// ===== Splash Screen =====

void uDisplay::Splash(void) {
    if (splash_font < 0) return;

    if (ep_mode) {
        Updateframe();
        if (universal_panel) {
            EPDPanel* epd = static_cast<EPDPanel*>(universal_panel);
            epd->delay_sync(panel_config->epd.update_time * 10);
        }
    }
    
    setTextFont(splash_font);
    setTextSize(splash_size);
    DrawStringAt(splash_xp, splash_yp, dname, fg_col, 0);
    Updateframe();

    AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: draw splash"));
}