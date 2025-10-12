#include "uDisplay.h"
#include "uDisplay_config.h"
#include "uDisplay_spi.h"

void udisp_bpwr(uint8_t on);

void udisp_dimm(uint8_t dim);

// input value is 0..15
// void uDisplay::dim(uint8_t dim) {
//   dim8(((uint32_t)dim * 255) / 15);
// }

// ===== Power Management =====

void uDisplay::DisplayOnff(int8_t on) {
    if (ep_mode) {
        return;
    }

    if (pwr_cbp) {
        pwr_cbp(on);
    }

#define AW_PWMRES 1024

    if (interface == _UDSP_I2C) {
        if (on) {
            i2c_command(dsp_on);
        } else {
            i2c_command(dsp_off);
        }
    } else {
        if (on) {
            if (dsp_on != 0xff) ulcd_command_one(dsp_on);
            if (bpanel >= 0) {
#ifdef ESP32
                if (!bpmode) {
                    analogWrite(bpanel, dimmer10_gamma);
                } else {
                    analogWrite(bpanel, AW_PWMRES - dimmer10_gamma);
                }
#else
                if (!bpmode) {
                    digitalWrite(bpanel, HIGH);
                } else {
                    digitalWrite(bpanel, LOW);
                }
#endif
            }
        } else {
            if (dsp_off != 0xff) ulcd_command_one(dsp_off);
            if (bpanel >= 0) {
#ifdef ESP32
                if (!bpmode) {
                    analogWrite(bpanel, 0);
                } else {
                    analogWrite(bpanel, AW_PWMRES - 1);
                }
#else
                if (!bpmode) {
                    digitalWrite(bpanel, LOW);
                } else {
                    digitalWrite(bpanel, HIGH);
                }
#endif
            }
        }
    }
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
        if (!bpmode) {
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
            SPI_BEGIN_TRANSACTION
            SPI_CS_LOW
            ulcd_command(dim_op);
            ulcd_data8(dimmer8);
            SPI_CS_HIGH
            SPI_END_TRANSACTION
        }
    }
}

// ===== Display Inversion =====

void uDisplay::invertDisplay(boolean i) {
    if (ep_mode) {
        return;
    }

    if (interface == _UDSP_SPI || interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
        if (i) {
            ulcd_command_one(inv_on);
        } else {
            ulcd_command_one(inv_off);
        }
    }
    
    if (interface == _UDSP_I2C) {
        if (i) {
            i2c_command(inv_on);
        } else {
            i2c_command(inv_off);
        }
    }
}

// ===== Splash Screen =====

void uDisplay::Splash(void) {
    if (splash_font < 0) return;

    if (ep_mode) {
        Updateframe();
        delay_sync(lut3time * 10);
    }
    
    setTextFont(splash_font);
    setTextSize(splash_size);
    DrawStringAt(splash_xp, splash_yp, dname, fg_col, 0);
    Updateframe();

    AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: draw splash"));
}