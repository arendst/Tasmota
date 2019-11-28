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

void setup() {
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send.
  // See state_t, opmode_t, fanspeed_t, swingv_t, & swingh_t in IRsend.h for
  // all the various options.
  ac.next.protocol = decode_type_t::DAIKIN;  // Set a protocol to use.
  ac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  ac.next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
  ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  ac.next.degrees = 25;  // 25 degrees.
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
  ac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  ac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  ac.next.light = false;  // Turn off any LED/Lights/Display that we can.
  ac.next.beep = false;  // Turn off any beep from the A/C if we can.
  ac.next.econo = false;  // Turn off any economy modes if we can.
  ac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  ac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  ac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  ac.next.sleep = -1;  // Don't set any sleep time or modes.
  ac.next.clean = false;  // Turn off any Cleaning options if we can.
  ac.next.clock = -1;  // Don't set any current time if we can avoid it.
  ac.next.power = false;  // Initially start with the unit off.

  Serial.println("Try to turn on & off every supported A/C type ...");
}

void loop() {
  // For every protocol the library has ...
  for (int i = 1; i < kLastDecodeType; i++) {
    decode_type_t protocol = (decode_type_t)i;
    // If the protocol is supported by the IRac class ...
    if (ac.isProtocolSupported(protocol)) {
      Serial.println("Protocol " + String(protocol) + " / " +
                     typeToString(protocol) + " is supported.");
      ac.next.protocol = protocol;  // Change the protocol used.
      ac.next.power = true;  // We want to turn on the A/C unit.
      Serial.println("Sending a message to turn ON the A/C unit.");
      ac.sendAc();  // Have the IRac class create and send a message.
      delay(5000);  // Wait 5 seconds.
      ac.next.power = false;  // Now we want to turn the A/C off.
      Serial.println("Send a message to turn OFF the A/C unit.");
      ac.sendAc();  // Send the message.
      delay(1000);  // Wait 1 second.
    }
  }
  Serial.println("Starting from the begining again ...");
}
