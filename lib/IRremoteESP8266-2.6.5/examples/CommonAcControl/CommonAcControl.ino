/* Copyright 2019 David Conran
*
* This example code demonstrates how to use the "Common" IRac class to control
* various air conditions. The IRac class does not support all the features
* for every protocol. Some have more detailed support that what the "Common"
* interface offers, and some only have a limited subset of the "Common" options.
*
* This example code will:
* o Try to turn on, then off every fully supported A/C protocol we know of.
* o It will try to put the A/C unit into Cooling mode at 25C, with a medium
*   fan speed, and no fan swinging.
* Note: Some protocols support multiple models, only the first model is tried.
*
*/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

const uint16_t kIrLed = 4;  // The ESP GPIO pin to use that controls the IR LED.
IRac ac(kIrLed);  // Create a A/C object using GPIO to sending messages with.
stdAc::state_t state;  // Where we will store the desired state of the A/C.
stdAc::state_t prev;  // Where we will store the previous state of the A/C.

void setup() {
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send.
  // See state_t, opmode_t, fanspeed_t, swingv_t, & swingh_t in IRsend.h for
  // all the various options.
  state.protocol = decode_type_t::DAIKIN;  // Set a protocol to use.
  state.model = 1;  // Some A/C's have different models. Let's try using just 1.
  state.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
  state.celsius = true;  // Use Celsius for units of temp. False = Fahrenheit
  state.degrees = 25;  // 25 degrees.
  state.fanspeed = stdAc::fanspeed_t::kMedium;  // Start with the fan at medium.
  state.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  state.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  state.light = false;  // Turn off any LED/Lights/Display that we can.
  state.beep = false;  // Turn off any beep from the A/C if we can.
  state.econo = false;  // Turn off any economy modes if we can.
  state.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  state.turbo = false;  // Don't use any turbo/powerful/etc modes.
  state.quiet = false;  // Don't use any quiet/silent/etc modes.
  state.sleep = -1;  // Don't set any sleep time or modes.
  state.clean = false;  // Turn off any Cleaning options if we can.
  state.clock = -1;  // Don't set any current time if we can avoid it.
  state.power = false;  // Initially start with the unit off.

  prev = state;  // Make sure we have a valid previous state.
}

void loop() {
  // For every protocol the library has ...
  for (int i = 1; i < kLastDecodeType; i++) {
    decode_type_t protocol = (decode_type_t)i;
    // If the protocol is supported by the IRac class ...
    if (ac.isProtocolSupported(protocol)) {
      state.protocol = protocol;  // Change the protocol used.

      Serial.println("Protocol " + String(protocol) + " / " +
                     typeToString(protocol));
      state.power = true;  // We want to turn on the A/C unit.
      // Have the IRac class create and send a message.
      // We need a `prev` state as some A/Cs use toggle messages.
      // e.g. On & Off are the same message. When given the previous state,
      //      it will try to do the correct thing for you.
      ac.sendAc(state, &prev);  // Construct and send the message.
      Serial.println("Sent a message to turn ON the A/C unit.");
      prev = state;  // Copy new state over the previous one.
      delay(5000);  // Wait 5 seconds.
      state.power = false;  // Now we want to turn the A/C off.
      ac.sendAc(state, &prev);  // Construct and send the message.
      Serial.println("Sent a message to turn OFF the A/C unit.");
      prev = state;  // Copy new state over the previous one.
      delay(1000);  // Wait 1 second.
    }
  }
  Serial.println("Starting from the begining again ...");
}
