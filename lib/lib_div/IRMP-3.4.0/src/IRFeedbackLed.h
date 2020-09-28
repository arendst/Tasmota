/*
 * IRFeedbackLed.h
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

// NO GUARD here, we have the GUARD below with #ifdef _IRSND_H_ and #ifdef _IRMP_H_.

#if defined(ARDUINO)
#ifndef IR_FEEDBACK_LED_H
#define IR_FEEDBACK_LED_H

/*
 * Set IRMP_FEEDBACK_LED_PIN to a reasonable value
 * defining it to 0 disables LED feedback function otherwise LED_BUILTIN (if available) is taken as feedback LED.
 */
#if !defined(IRMP_FEEDBACK_LED_PIN)
#  if defined(LED_BUILTIN)
#define IRMP_FEEDBACK_LED_PIN LED_BUILTIN
#  else
#warning IRMP_FEEDBACK_LED_PIN and LED_BUILTIN  not defined. Pin 5 is used for feedback output if enabled. You can change this in IRFeedbackLed.h.
#define IRMP_FEEDBACK_LED_PIN 5 // choose an abitrary pin
#  endif
#endif

void irmp_irsnd_LEDFeedback(bool aEnableBlinkLed);          // set the mode
void irmp_irsnd_SetFeedbackLED(bool aSwitchLedOn);          // set set pin
constexpr auto irmp_blink13 = irmp_irsnd_LEDFeedback;       // alias for irmp_blink13
constexpr auto irmp_LEDFeedback = irmp_irsnd_LEDFeedback;   // alias for irmp_LEDFeedback
constexpr auto irsnd_LEDFeedback = irmp_irsnd_LEDFeedback;  // alias for irsnd_LEDFeedback

#endif // IR_FEEDBACK_LED_H
#endif // defined(ARDUINO)
