//Leo_passthru
// Allows Leonardo to pass serial data between fingerprint reader and Windows.
//
// Red connects to +5V
// Black connects to Ground
// Green goes to Digital 0
// White goes to Digital 1

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(57600);
  Serial.begin(57600);
}

void loop() {
  
  while (Serial.available())
    Serial1.write(Serial.read());
  while (Serial1.available())
    Serial.write(Serial1.read());
}