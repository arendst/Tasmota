#include <OneWire.h>

/*
 * DS2408 8-Channel Addressable Switch
 *
 * Writte by Glenn Trewitt, glenn at trewitt dot org
 *
 * Some notes about the DS2408:
 *   - Unlike most input/output ports, the DS2408 doesn't have mode bits to
 *       set whether the pins are input or output.  If you issue a read command,
 *       they're inputs.  If you write to them, they're outputs.
 *   - For reading from a switch, you should use 10K pull-up resisters.
 */

OneWire net(10);  // on pin 10


void PrintBytes(const uint8_t* addr, uint8_t count, bool newline=false) {
  for (uint8_t i = 0; i < count; i++) {
    Serial.print(addr[i]>>4, HEX);
    Serial.print(addr[i]&0x0f, HEX);
  }
  if (newline)
    Serial.println();
}


void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte addr[8];

  if (!net.search(addr)) {
    Serial.print("No more addresses.\n");
    net.reset_search();
    delay(1000);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.print("CRC is not valid!\n");
    return;
  }

  if (addr[0] != 0x29) {
    PrintBytes(addr, 8);
    Serial.print(" is not a DS2408.\n");
    return;
  }

  Serial.print("  Reading DS2408 ");
  PrintBytes(addr, 8);
  Serial.println();

  uint8_t buf[13];  // Put everything in the buffer so we can compute CRC easily.
  buf[0] = 0xF0;    // Read PIO Registers
  buf[1] = 0x88;    // LSB address
  buf[2] = 0x00;    // MSB address
  net.write_bytes(buf, 3);
  net.read_bytes(buf+3, 10);     // 3 cmd bytes, 6 data bytes, 2 0xFF, 2 CRC16
  net.reset();

  if (!OneWire::check_crc16(buf, 11, &buf[11])) {
    Serial.print("CRC failure in DS2408 at ");
    PrintBytes(addr, 8, true);
    return;
  }
  Serial.print("  DS2408 data = ");
  // First 3 bytes contain command, register address.
  Serial.println(buf[3], BIN);
}

