/*
 * irsndArduinoExt.cpp.h - Arduino extensions to the original irsnd.c
 *
 *  Copyright (C) 2020  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of IRMP https://github.com/ukw100/IRMP.
 *
 *  IRMP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#if defined(ARDUINO)

#undef _IRMP_H_                 // We are in IRSND now! Remove old symbol set from former including irmp.c.h if we use receive and send in the same user program.
#include "IRTimer.cpp.h"        // include code for timer
#include "IRFeedbackLed.cpp.h"  // include code for Feedback LED
#include "irmpprotocols.c.h"    // include protocol strings and array of strings

#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
uint_fast8_t irsnd_output_pin;

/*
 * Initialize, and activate feedback LED function
 * @param aFeedbackLedPin - if 0 feedback function is disabled
 */
void irsnd_init(uint_fast8_t aIrsndOutputPin, uint_fast8_t aFeedbackLedPin, bool aIrmpLedFeedbackPinIsActiveLow)
{
    irsnd_output_pin = aIrsndOutputPin;
    irmp_irsnd_LedFeedbackPin = aFeedbackLedPin;
    irmp_irsnd_LedFeedbackPinIsActiveLow = aIrmpLedFeedbackPinIsActiveLow;

    /*
     * enable feedback LED if (aFeedbackLedPin != 0)
     */
    irmp_irsnd_LEDFeedback(aFeedbackLedPin);

    // Do not call irsnd_init_and_store_timer() here, it is done at irsnd_send_data().
    pinMode(irsnd_output_pin, OUTPUT);
#  ifdef IRMP_MEASURE_TIMING
    pinModeFast(IRMP_TIMING_TEST_PIN, OUTPUT);
#  endif
}

/*
 * Initialize, and activate feedback LED function
 * @param aFeedbackLedPin - if 0 feedback function is disabled
 */
void irsnd_init(uint_fast8_t aIrsndOutputPin, uint_fast8_t aFeedbackLedPin)
{
    irsnd_init(aIrsndOutputPin, aFeedbackLedPin, irmp_irsnd_LedFeedbackPinIsActiveLow);
}

/*
 * Initialize, use  feedback LED by using 0 as led pin
 */
void irsnd_init(uint_fast8_t aIrsndOutputPin)
{
    irsnd_init(aIrsndOutputPin, irmp_irsnd_LedFeedbackPin, irmp_irsnd_LedFeedbackPinIsActiveLow);
#  if defined(IRMP_FEEDBACK_LED_PIN)
    // set pin if we have one at hand
    irmp_irsnd_LedFeedbackPin = IRMP_FEEDBACK_LED_PIN;
#  endif
}

#else // defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
/*
 * Init function for defined pins
 */
void irsnd_init(void)
{
    // Do not call irsnd_init_and_store_timer() here, it is done at irsnd_send_data().
    pinModeFast(IRSND_OUTPUT_PIN, OUTPUT);
#  ifdef IRMP_MEASURE_TIMING
    pinModeFast(IRMP_TIMING_TEST_PIN, OUTPUT);
#  endif
}
#endif // defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)

static void irsnd_set_freq(IRSND_FREQ_TYPE freq __attribute__((unused)))
{
    // not supported by now
}

/*
 * Called from irsnd_ISR to set the IR output
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR irsnd_on(void)
#elif defined(ESP32)
void IRAM_ATTR irsnd_on(void)
#else
void irsnd_on(void)
#endif
{
    if (!irsnd_is_on)
    {
        if (irmp_irsnd_LedFeedbackEnabled)
        {
#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
            irmp_irsnd_SetFeedbackLED(true);
#else
#  if defined(IRMP_FEEDBACK_LED_PIN)
#    if defined(FEEDBACK_LED_IS_ACTIVE_LOW)
            // If the built in LED on the board is active LOW
            digitalWriteFast(IRMP_FEEDBACK_LED_PIN, LOW);
#    else
            digitalWriteFast(IRMP_FEEDBACK_LED_PIN, HIGH);
#    endif
#  endif
#endif
        }
        irsnd_is_on = TRUE; // evaluated at ISR
    }
}

#if defined(ESP8266)
void ICACHE_RAM_ATTR irsnd_off(void)
#elif defined(ESP32)
void IRAM_ATTR irsnd_off(void)
#else
void irsnd_off(void)
#endif
{
    if (irsnd_is_on)
    {
        // Manage feedback LED

        if (irmp_irsnd_LedFeedbackEnabled)
        {
#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
            irmp_irsnd_SetFeedbackLED(false);
#else
#  if defined(IRMP_FEEDBACK_LED_PIN)
#    if defined(FEEDBACK_LED_IS_ACTIVE_LOW)
            // If the built in LED on the board is active LOW
            digitalWriteFast(IRMP_FEEDBACK_LED_PIN, HIGH);
#    else
            digitalWriteFast(IRMP_FEEDBACK_LED_PIN, LOW);
#    endif
#  endif
#endif
        }
        irsnd_is_on = FALSE;
    }
}

/*
 * irsnd_used_protocol_index holds the protocol numbers (from irsndprotocols.h)
 * for the included protocol name entries of the irsnd_used_protocol_names array below
 * E.g. irmp_used_protocol_index=2,7 and irmp_used_protocol_names="NEC","RC5".
 *
 * Both arrays together are generally smaller than the complete irmp_protocol_names array
 * allowing them to be used on ATtinies even if program code for access is bigger.
 * Flash size is more than 100 bytes less (for 15 protocols) using these arrays.
 */
const uint8_t irsnd_used_protocol_index[] PROGMEM =
{
#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1
    IRMP_SIRCS_PROTOCOL,
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
    IRMP_NEC_PROTOCOL,
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
    IRMP_SAMSUNG_PROTOCOL,
#endif
#if IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1
    IRMP_MATSUSHITA_PROTOCOL,
#endif
#if IRSND_SUPPORT_KASEIKYO_PROTOCOL == 1
    IRMP_KASEIKYO_PROTOCOL,
#endif
#if IRSND_SUPPORT_RECS80_PROTOCOL == 1
    IRMP_RECS80_PROTOCOL,
#endif
#if IRSND_SUPPORT_RC5_PROTOCOL == 1
    IRMP_RC5_PROTOCOL,
#endif
#if IRSND_SUPPORT_DENON_PROTOCOL == 1
    IRMP_DENON_PROTOCOL,
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
    IRMP_RC6_PROTOCOL,
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
    IRMP_SAMSUNG32_PROTOCOL,
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
    IRMP_APPLE_PROTOCOL,
#endif
#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1
    IRMP_RECS80EXT_PROTOCOL,
#endif
#if IRSND_SUPPORT_NUBERT_PROTOCOL == 1
    IRMP_NUBERT_PROTOCOL,
#endif
#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
    IRMP_BANG_OLUFSEN_PROTOCOL,
#endif
#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1
    IRMP_GRUNDIG_PROTOCOL,
#endif
#if IRSND_SUPPORT_NOKIA_PROTOCOL == 1
    IRMP_NOKIA_PROTOCOL,
#endif
#if IRSND_SUPPORT_SIEMENS_PROTOCOL  == 1
    IRMP_SIEMENS_PROTOCOL,
#endif
#if IRSND_SUPPORT_FDC_PROTOCOL == 1
    IRMP_FDC_PROTOCOL,
#endif
#if IRSND_SUPPORT_RCCAR_PROTOCOL == 1
    IRMP_RCCAR_PROTOCOL,
#endif
#if IRSND_SUPPORT_JVC_PROTOCOL == 1
    IRMP_JVC_PROTOCOL,
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
    IRMP_RC6A_PROTOCOL,
#endif
#if IRSND_SUPPORT_NIKON_PROTOCOL == 1
    IRMP_NIKON_PROTOCOL,
#endif
#if IRSND_SUPPORT_RUWIDO_PROTOCOL == 1
    IRMP_RUWIDO_PROTOCOL,
#endif
#if IRSND_SUPPORT_IR60_PROTOCOL == 1
    IRMP_IR60_PROTOCOL,
#endif
#if IRSND_SUPPORT_KATHREIN_PROTOCOL == 1
    IRMP_KATHREIN_PROTOCOL,
#endif
#if IRSND_SUPPORT_NETBOX_PROTOCOL == 1
    IRMP_NETBOX_PROTOCOL,
#endif
#if IRSND_SUPPORT_NEC16_PROTOCOL == 1
    IRMP_NEC16_PROTOCOL,
#endif
#if IRSND_SUPPORT_NEC42_PROTOCOL == 1
    IRMP_NEC42_PROTOCOL,
#endif
#if IRSND_SUPPORT_LEGO_PROTOCOL == 1
    IRMP_LEGO_PROTOCOL,
#endif
#if IRSND_SUPPORT_THOMSON_PROTOCOL == 1
    IRMP_THOMSON_PROTOCOL,
#endif
#if IRSND_SUPPORT_BOSE_PROTOCOL == 1
    IRMP_BOSE_PROTOCOL,
#endif
#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1
    IRMP_A1TVBOX_PROTOCOL,
#endif
#if IRSND_SUPPORT_ORTEK_PROTOCOL == 1
    IRMP_ORTEK_PROTOCOL,
#endif
#if IRMP_SUPPORT_TELEFUNKEN_PROTOCOL == 1
    IRMP_TELEFUNKEN_PROTOCOL,
#endif
#if IRSND_SUPPORT_ROOMBA_PROTOCOL == 1
    IRMP_ROOMBA_PROTOCOL,
#endif
#if IRSND_SUPPORT_RCMM_PROTOCOL  == 1
    IRMP_RCMM12_PROTOCOL,
#endif
#if IRSND_SUPPORT_SPEAKER_PROTOCOL == 1
    IRMP_SPEAKER_PROTOCOL,
#endif
#if IRSND_SUPPORT_LGAIR_PROTOCOL == 1
    IRMP_LGAIR_PROTOCOL,
#endif
#if IRSND_SUPPORT_SAMSUNG48_PROTOCOL == 1
    IRMP_SAMSUNG48_PROTOCOL,
#endif
#if IRSND_SUPPORT_MERLIN_PROTOCOL == 1
    IRMP_MERLIN_PROTOCOL,
#endif
#if IRSND_SUPPORT_PENTAX_PROTOCOL == 1
    IRMP_PENTAX_PROTOCOL,
#endif
#if IRSND_SUPPORT_FAN_PROTOCOL == 1
    IRMP_FAN_PROTOCOL,
#endif
#if IRSND_SUPPORT_S100_PROTOCOL == 1
    IRMP_S100_PROTOCOL,
#endif
#if IRSND_SUPPORT_ACP24_PROTOCOL  == 1
    IRMP_ACP24_PROTOCOL,
#endif
#if IRSND_SUPPORT_TECHNICS_PROTOCOL == 1
    IRMP_TECHNICS_PROTOCOL,
#endif
#if IRSND_SUPPORT_PANASONIC_PROTOCOL == 1
    IRMP_PANASONIC_PROTOCOL,
#endif
#if IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL == 1
    IRMP_MITSU_HEAVY_PROTOCOL,
#endif
#if IRSND_SUPPORT_VINCENT_PROTOCOL == 1
    IRMP_VINCENT_PROTOCOL,
#endif
#if IRSND_SUPPORT_SAMSUNGAH_PROTOCOL == 1
    IRMP_SAMSUNGAH_PROTOCOL,
#endif
#if IRSND_SUPPORT_IRMP16_PROTOCOL == 1
    IRMP_IRMP16_PROTOCOL,
#endif
#if IRSND_SUPPORT_GREE_PROTOCOL == 1
    IRMP_GREE_PROTOCOL,
#endif
#if IRSND_SUPPORT_RCII_PROTOCOL == 1
    IRMP_RCII_PROTOCOL,
#endif
#if IRSND_SUPPORT_METZ_PROTOCOL == 1
    IRMP_METZ_PROTOCOL,
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
    IRMP_ONKYO_PROTOCOL,
#endif
#if IRSND_SUPPORT_RF_GEN24_PROTOCOL == 1
    RF_GEN24_PROTOCOL,
#endif
#if IRMP_SUPPORT_RF_X10_PROTOCOL == 1
    RF_X10_PROTOCOL,
#endif
#if IRMP_SUPPORT_RF_MEDION_PROTOCOL == 1
    RF_MEDION_PROTOCOL
#endif
#if IRSND_SUPPORT_MELINERA_PROTOCOL == 1
    IRMP_MELINERA_PROTOCOL
#endif
};

const char * const irsnd_used_protocol_names[] PROGMEM =
{
#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1
    proto_sircs,
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
    proto_nec,
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
    proto_samsung,
#endif
#if IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1
    proto_matsushita,
#endif
#if IRSND_SUPPORT_KASEIKYO_PROTOCOL == 1
    proto_kaseikyo,
#endif
#if IRSND_SUPPORT_RECS80_PROTOCOL == 1
    proto_recs80,
#endif
#if IRSND_SUPPORT_RC5_PROTOCOL == 1
    proto_rc5,
#endif
#if IRSND_SUPPORT_DENON_PROTOCOL == 1
    proto_denon,
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
    proto_rc6,
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
    proto_samsung32,
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
    proto_apple,
#endif
#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1
    proto_recs80ext,
#endif
#if IRSND_SUPPORT_NUBERT_PROTOCOL == 1
    proto_nubert,
#endif
#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
    proto_bang_olufsen,
#endif
#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1
    proto_grundig,
#endif
#if IRSND_SUPPORT_NOKIA_PROTOCOL == 1
    proto_nokia,
#endif
#if IRSND_SUPPORT_SIEMENS_PROTOCOL  == 1
    proto_siemens,
#endif
#if IRSND_SUPPORT_FDC_PROTOCOL == 1
    proto_fdc,
#endif
#if IRSND_SUPPORT_RCCAR_PROTOCOL == 1
    proto_rccar,
#endif
#if IRSND_SUPPORT_JVC_PROTOCOL == 1
    proto_jvc,
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
    proto_rc6a,
#endif
#if IRSND_SUPPORT_NIKON_PROTOCOL == 1
    proto_nikon,
#endif
#if IRSND_SUPPORT_RUWIDO_PROTOCOL == 1
    proto_ruwido,
#endif
#if IRSND_SUPPORT_IR60_PROTOCOL == 1
    proto_ir60,
#endif
#if IRSND_SUPPORT_KATHREIN_PROTOCOL == 1
    proto_kathrein,
#endif
#if IRSND_SUPPORT_NETBOX_PROTOCOL == 1
    proto_netbox,
#endif
#if IRSND_SUPPORT_NEC16_PROTOCOL == 1
    proto_nec16,
#endif
#if IRSND_SUPPORT_NEC42_PROTOCOL == 1
    proto_nec42,
#endif
#if IRSND_SUPPORT_LEGO_PROTOCOL == 1
    proto_lego,
#endif
#if IRSND_SUPPORT_THOMSON_PROTOCOL == 1
    proto_thomson,
#endif
#if IRSND_SUPPORT_BOSE_PROTOCOL == 1
    proto_bose,
#endif
#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1
    proto_a1tvbox,
#endif
#if IRSND_SUPPORT_ORTEK_PROTOCOL == 1
    proto_ortek,
#endif
#if IRSND_SUPPORT_TELEFUNKEN_PROTOCOL == 1
    proto_telefunken,
#endif
#if IRSND_SUPPORT_ROOMBA_PROTOCOL == 1
    proto_roomba,
#endif
#if IRSND_SUPPORT_RCMM_PROTOCOL  == 1
    proto_rcmm12,
#endif
#if IRSND_SUPPORT_SPEAKER_PROTOCOL == 1
    proto_speaker,
#endif
#if IRSND_SUPPORT_LGAIR_PROTOCOL == 1
    proto_lgair,
#endif
#if IRSND_SUPPORT_SAMSUNG48_PROTOCOL == 1
    proto_samsung48,
#endif
#if IRSND_SUPPORT_MERLIN_PROTOCOL == 1
    proto_merlin,
#endif
#if IRSND_SUPPORT_PENTAX_PROTOCOL == 1
    proto_pentax,
#endif
#if IRSND_SUPPORT_FAN_PROTOCOL == 1
    proto_fan,
#endif
#if IRSND_SUPPORT_S100_PROTOCOL == 1
    proto_s100,
#endif
#if IRSND_SUPPORT_ACP24_PROTOCOL  == 1
    proto_acp24,
#endif
#if IRSND_SUPPORT_TECHNICS_PROTOCOL == 1
    proto_technics,
#endif
#if IRSND_SUPPORT_PANASONIC_PROTOCOL == 1
    proto_panasonic,
#endif
#if IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL == 1
    proto_mitsu_heavy,
#endif
#if IRSND_SUPPORT_VINCENT_PROTOCOL == 1
    proto_vincent,
#endif
#if IRSND_SUPPORT_SAMSUNGAH_PROTOCOL == 1
    proto_samsungah,
#endif
#if IRSND_SUPPORT_IRMP16_PROTOCOL == 1
    proto_irmp16,
#endif
#if IRSND_SUPPORT_GREE_PROTOCOL == 1
    proto_gree,
#endif
#if IRSND_SUPPORT_RCII_PROTOCOL == 1
    proto_rcii,
#endif
#if IRSND_SUPPORT_METZ_PROTOCOL == 1
    proto_metz,
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
    proto_onkyo,
#endif
#if IRSND_SUPPORT_RF_GEN24_PROTOCOL == 1
    proto_rf_gen24,
#endif
#if IRSND_SUPPORT_RF_X10_PROTOCOL == 1
    proto_rf_x10,
#endif
#if IRSND_SUPPORT_RF_MEDION_PROTOCOL == 1
    proto_rf_medion
#endif
#if IRSND_SUPPORT_MELINERA_PROTOCOL == 1
    proto_melinera
#endif
};

void irsnd_print_protocol_name(Print *aSerial, uint8_t aProtocolNumber)
{
#  if defined(__AVR__)
    for (uint_fast8_t i = 0; i < sizeof(irsnd_used_protocol_index); ++i)
    {
        if (pgm_read_byte(&irsnd_used_protocol_index[i]) == aProtocolNumber)
        {
            const char* tProtocolStringPtr = (char*) pgm_read_word(&irsnd_used_protocol_names[i]);
            aSerial->print((__FlashStringHelper *) (tProtocolStringPtr));
            break;
        }
    }
#  else
    // no need to save space
    aSerial->print(irmp_protocol_names[aProtocolNumber]);
#  endif
}

void irsnd_data_print(Print *aSerial, IRMP_DATA *aIRMPDataPtr)
{
    /*
     * Print protocol name or number
     */
    aSerial->print(F("Protocol="));
#if IRSND_PROTOCOL_NAMES == 1
    irsnd_print_protocol_name(aSerial, aIRMPDataPtr->protocol);
    aSerial->print(' ');
#else
    aSerial->print(F("0x"));
    aSerial->print(aIRMPDataPtr->protocol, HEX);
#endif

    /*
     * Print address, code and repetition flag
     */
    aSerial->print(F(" Address=0x"));
    aSerial->print(aIRMPDataPtr->address, HEX);
    aSerial->print(F(" Command=0x"));
    aSerial->print(aIRMPDataPtr->command, HEX);
    if (aIRMPDataPtr->flags & IRSND_REPETITION_MASK)
    {
        aSerial->print(F(" Repeats="));
        aSerial->print(aIRMPDataPtr->flags & IRSND_REPETITION_MASK);

    }
    aSerial->println();
}

#endif // defined(ARDUINO)
