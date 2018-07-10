// Copyright 2017 Jonny Graham
#include <IRsend.h>
#include <ir_Fujitsu.h>

IRFujitsuAC fujitsu(5);  // IR led controlled by Pin D1.

void printState() {
  // Display the settings.
  Serial.println("Fujitsu A/C remote is in the following state:");
  Serial.printf("  Command:%d,  Mode: %d, Temp: %dC, Fan Speed: %d," \
                    " Swing Mode: %d\n",
                fujitsu.getCmd(), fujitsu.getMode(), fujitsu.getTemp(),
                fujitsu.getFanSpeed(), fujitsu.getSwing());
  // Display the encoded IR sequence.
  unsigned char* ir_code = fujitsu.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < FUJITSU_AC_STATE_LENGTH; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

void setup() {
  fujitsu.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See ir_Mitsubishi.cpp for all the options.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
  fujitsu.setCmd(FUJITSU_AC_CMD_TURN_ON);
  fujitsu.setSwing(FUJITSU_AC_SWING_BOTH);
  fujitsu.setMode(FUJITSU_AC_MODE_COOL);
  fujitsu.setFanSpeed(FUJITSU_AC_FAN_HIGH);
  fujitsu.setTemp(24);
}

void loop() {
  // Now send the IR signal.
  Serial.println("Sending IR command to A/C ...");
  fujitsu.send();
  printState();
  delay(5000);
}
