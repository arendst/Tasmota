
#include <TasmotaModbus.h>

TasmotaModbus Modbus(14, 12);

void setup() {
  Serial.begin(115200);
  Modbus.Begin(9600);

  Serial.println("\nTasmotaModbus test started");

  Modbus.Send(0x01, 0x04, 0, 8);
}

void loop() {
  if (Modbus.ReceiveReady()) {
    uint8_t buffer[26];

    uint8_t error = Modbus.ReceiveBuffer(buffer, 8);
    if (error) {
      Serial.print("Modbus response error ");
      Serial.println(error);
    } else {
      Serial.print("Modbus received:");
      for (int i = 0; i < (buffer[2]) ? buffer[2] +5 : sizeof(buffer); i++) {
        Serial.print(" ");
        Serial.print(buffer[i], HEX);
      }
    }
  }
}
