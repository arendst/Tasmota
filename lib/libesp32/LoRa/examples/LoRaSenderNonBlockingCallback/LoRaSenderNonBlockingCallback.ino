#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender non-blocking Callback");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.onTxDone(onTxDone);
}

void loop() {
  if (runEvery(5000)) { // repeat every 5000 millis

    Serial.print("Sending packet non-blocking: ");
    Serial.println(counter);

    // send in async / non-blocking mode
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket(true); // true = async / non-blocking mode

    counter++;
  }
}

void onTxDone() {
  Serial.println("TxDone");
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
