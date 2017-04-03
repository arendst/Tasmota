
#include <IRMitsubishiAC.h>

IRMitsubishiAC mitsubir(D1);  // IR led controlled by Pin D1.

void printState() {
  // Display the settings.
  Serial.println("Mitsubishi A/C remote is in the following state:");
  Serial.printf("  Power: %d,  Mode: %d, Temp: %dC, Fan Speed: %d, Vane Mode: %d\n",
                mitsubir.getPower(), mitsubir.getMode(), mitsubir.getTemp(),
                mitsubir.getFan(), mitsubir.getVane());
  // Display the encoded IR sequence.
  unsigned char* ir_code = mitsubir.getRaw();
  Serial.print("IR Code: 0x");
  for (int i = 0; i < MITSUBISHI_AC_STATE_LENGTH; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

void setup(){
  mitsubir.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See IRMitsubishiAC.cpp for all the options.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
  mitsubir.on();
  mitsubir.setFan(1);
  mitsubir.setMode(MITSUBISHI_AC_COOL);
  mitsubir.setTemp(26);
  mitsubir.setVane(MITSUBISHI_AC_VANE_AUTO);
}

void loop() {
  // Now send the IR signal.
  Serial.println("Sending IR command to A/C ...");
  mitsubir.send();
  printState();
  delay(5000);
}
