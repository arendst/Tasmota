// Copyright 2017 Jonny Graham, 2018 David Conran
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRFujitsuAC ac(kIrLed);

void printState() {
  // Display the settings.
  Serial.println("Fujitsu A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < ac.getStateLength(); i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

void setup() {
  ac.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See ir_Fujitsu.cpp for all the options.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
  // See `fujitsu_ac_remote_model_t` in `ir_Fujitsu.h` for a list of models.
  ac.setModel(ARRAH2E);
  ac.setSwing(kFujitsuAcSwingOff);
  ac.setMode(kFujitsuAcModeCool);
  ac.setFanSpeed(kFujitsuAcFanHigh);
  ac.setTemp(24);  // 24C
  ac.setCmd(kFujitsuAcCmdTurnOn);
}

void loop() {
  // Now send the IR signal.
  Serial.println("Sending IR command to A/C ...");
#if SEND_FUJITSU_AC
  ac.send();
#else  // SEND_FUJITSU_AC
  Serial.println("Can't send because SEND_FUJITSU_AC has been disabled.");
#endif  // SEND_FUJITSU_AC
  printState();
  delay(5000);
}
