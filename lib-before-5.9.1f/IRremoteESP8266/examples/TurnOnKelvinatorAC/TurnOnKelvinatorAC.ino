
#include <IRKelvinator.h>

IRKelvinatorAC kelvir(D1);  // IR led controlled by Pin D1.

void printState() {
  // Display the settings.
  Serial.println("Kelvinator A/C remote is in the following state:");
  Serial.printf("  Basic\n  Power: %d,  Mode: %d, Temp: %dC, Fan Speed: %d\n",
                kelvir.getPower(), kelvir.getMode(), kelvir.getTemp(),
                kelvir.getFan());
  Serial.printf("  Options\n  X-Fan: %d,  Light: %d, Ion Filter: %d\n",
                kelvir.getXFan(), kelvir.getLight(), kelvir.getIonFilter());
  Serial.printf("  Swing (V): %d, Swing (H): %d, Turbo: %d, Quiet: %d\n",
                kelvir.getSwingVertical(), kelvir.getSwingHorizontal(),
                kelvir.getTurbo(), kelvir.getQuiet());
  // Display the encoded IR sequence.
  unsigned char* ir_code = kelvir.getRaw();
  Serial.print("IR Code: 0x");
  for (int i = 0; i < KELVINATOR_STATE_LENGTH; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

void setup(){
  kelvir.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See IRKelvinator.cpp for all the options.
  // Most things default to off.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
  kelvir.on();
  kelvir.setFan(1);
  kelvir.setMode(KELVINATOR_COOL);
  kelvir.setTemp(26);
  kelvir.setSwingVertical(false);
  kelvir.setSwingHorizontal(true);
  kelvir.setXFan(true);
  kelvir.setIonFilter(false);
  kelvir.setLight(true);
}

void loop() {
  // Now send the IR signal.
  Serial.println("Sending IR command to A/C ...");
  kelvir.send();
  printState();
  delay(5000);
}
