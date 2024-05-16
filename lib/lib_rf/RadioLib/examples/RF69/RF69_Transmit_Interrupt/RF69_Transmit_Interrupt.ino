/*
   RadioLib RF69 Transmit with Interrupts Example

   This example transmits FSK packets with one second delays
   between them. Each packet contains up to 64 bytes
   of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#rf69sx1231

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// RF69 has the following connections:
// CS pin:    10
// DIO0 pin:  2
// RESET pin: 3
RF69 radio = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//RF69 radio = RadioShield.ModuleA;

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

void setup() {
  Serial.begin(9600);

  // initialize RF69 with default settings
  Serial.print(F("[RF69] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when packet transmission is finished
  radio.setPacketSentAction(setFlag);

  // NOTE: some RF69 modules use high power output,
  //       those are usually marked RF69H(C/CW).
  //       To configure RadioLib for these modules,
  //       you must call setOutputPower() with
  //       second argument set to true.
  /*
    Serial.print(F("[RF69] Setting high power module ... "));
    state = radio.setOutputPower(20, true);
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while (true);
    }
  */

  // start transmitting the first packet
  Serial.print(F("[RF69] Sending first packet ... "));

  // you can transmit C-string or Arduino string up to
  // 64 characters long
  transmissionState = radio.startTransmit("Hello World!");

  // you can also transmit byte array up to 64 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                      0x89, 0xAB, 0xCD, 0xEF};
    state = radio.startTransmit(byteArr, 8);
  */
}

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we sent a packet, set the flag
  transmittedFlag = true;
}

// counter to keep track of transmitted packets
int count = 0;

void loop() {
  // check if the previous transmission finished
  if(transmittedFlag) {
    // reset flag
    transmittedFlag = false;

    if (transmissionState == RADIOLIB_ERR_NONE) {
      // packet was successfully sent
      Serial.println(F("transmission finished!"));

      // NOTE: when using interrupt-driven transmit method,
      //       it is not possible to automatically measure
      //       transmission data rate using getDataRate()

    } else {
      Serial.print(F("failed, code "));
      Serial.println(transmissionState);

    }

    // clean up after transmission is finished
    // this will ensure transmitter is disabled,
    // RF switch is powered down etc.
    radio.finishTransmit();

    // wait a second before transmitting again
    delay(1000);

    // send another one
    Serial.print(F("[RF69] Sending another packet ... "));

    // you can transmit C-string or Arduino string up to
    // 64 characters long
    String str = "Hello World! #" + String(count++);
    transmissionState = radio.startTransmit(str);

    // you can also transmit byte array up to 64 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
      transmissionState = radio.startTransmit(byteArr, 8);
    */
  }
}
