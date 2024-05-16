/*
  RadioLib LoRaWAN End Device ABP Example

  This example sets up a LoRaWAN node using ABP (activation
  by personalization). Before you start, you will have to
  register your device at https://www.thethingsnetwork.org/
  After your device is registered, you can run this example.
  The device will start uploading data directly,
  without having to join the network.

  NOTE: LoRaWAN v1.1 requires storing parameters persistently!
        RadioLib does this by using EEPROM (persistent storage), 
        by default starting at address 0 and using 448 bytes.
        If you already use EEPROM in your application,
        you will have to either avoid this range, or change it
        by setting a different start address by changing the value of
        RADIOLIB_HAL_PERSISTENT_STORAGE_BASE macro, either
        during build or in src/BuildOpt.h.
  
  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1262 has the following pin order:
// Module(NSS/CS, DIO1, RESET, BUSY)
// SX1262 radio = new Module(8, 14, 12, 13);

// SX1278 has the following pin order:
// Module(NSS/CS, DIO0, RESET, DIO1)
SX1278 radio = new Module(10, 2, 9, 3);

// create the node instance on the EU-868 band
// using the radio module and the encryption key
// make sure you are using the correct band
// based on your geographical location!
LoRaWANNode node(&radio, &EU868);

// for fixed bands with subband selection
// such as US915 and AU915, you must specify
// the subband that matches the Frequency Plan
// that you selected on your LoRaWAN console
/*
  LoRaWANNode node(&radio, &US915, 2);
*/

void setup() {
  Serial.begin(9600);

  // initialize radio (SX1262 / SX1278 / ... ) with default settings
  Serial.print(F("[Radio] Initializing ... "));
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // device address - this number can be anything
  // when adding new end device in TTN, you can generate this number,
  // or you can set any value you want, provided it is unique
  uint32_t devAddr = 0x12345678;

  // select some encryption keys which will be used to secure the communication
  // there are two of them - network key and application key
  // because LoRaWAN uses AES-128, the key MUST be 16 bytes (or characters) long

  // network key is the ASCII string "topSecretKey1234"
  uint8_t nwkSKey[] = { 0x74, 0x6F, 0x70, 0x53, 0x65, 0x63, 0x72, 0x65,
                        0x74, 0x4B, 0x65, 0x79, 0x31, 0x32, 0x33, 0x34 };

  // application key is the ASCII string "aDifferentKeyABC"
  uint8_t appSKey[] = { 0x61, 0x44, 0x69, 0x66, 0x66, 0x65, 0x72, 0x65,
                        0x6E, 0x74, 0x4B, 0x65, 0x79, 0x41, 0x42, 0x43 };

  // network key 2 is the ASCII string "topSecretKey5678"
  uint8_t fNwkSIntKey[] = { 0x61, 0x44, 0x69, 0x66, 0x66, 0x65, 0x72, 0x65,
                            0x6E, 0x74, 0x4B, 0x65, 0x35, 0x36, 0x37, 0x38 };

  // network key 3 is the ASCII string "aDifferentKeyDEF"
  uint8_t sNwkSIntKey[] = { 0x61, 0x44, 0x69, 0x66, 0x66, 0x65, 0x72, 0x65,
                            0x6E, 0x74, 0x4B, 0x65, 0x79, 0x44, 0x45, 0x46 };

  // prior to LoRaWAN 1.1.0, only a single "nwkKey" is used
  // when connecting to LoRaWAN 1.0 network, "appKey" will be disregarded
  // and can be set to NULL


  // if using EU868 on ABP in TTN, you need to set the SF for RX2 window manually
	/*
    node.rx2.drMax = 3;
  */

  // on EEPROM-enabled boards, after the device has been activated,
  // the session can be restored without rejoining after device power cycle
  // this is intrinsically done when calling `beginABP()` with the same keys
  // in that case, the function will not need to transmit a JoinRequest

  // to start a LoRaWAN v1.0 session, 
  // the user can remove the fNwkSIntKey and sNwkSIntKey
  /*
    state = node.beginABP(devAddr, nwkSKey, appSKey);
  */

  // start the device by directly providing the encryption keys and device address
  Serial.print(F("[LoRaWAN] Attempting over-the-air activation ... "));
  state = node.beginABP(devAddr, nwkSKey, appSKey, fNwkSIntKey, sNwkSIntKey);
  if(state >= RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

}

// counter to keep track of transmitted packets
int count = 0;

void loop() {
  // send uplink to port 10
  Serial.print(F("[LoRaWAN] Sending uplink packet ... "));
  String strUp = "Hello!" + String(count++);
  String strDown;
  int state = node.sendReceive(strUp, 10, strDown);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("received a downlink!"));

    // print data of the packet (if there are any)
    Serial.print(F("[LoRaWAN] Data:\t\t"));
    if(strDown.length() > 0) {
      Serial.println(strDown);
    } else {
      Serial.println(F("<MAC commands only>"));
    }

    // print RSSI (Received Signal Strength Indicator)
    Serial.print(F("[LoRaWAN] RSSI:\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print SNR (Signal-to-Noise Ratio)
    Serial.print(F("[LoRaWAN] SNR:\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    Serial.print(F("[LoRaWAN] Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));
  
  } else if(state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println(F("no downlink!"));
  
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // on EEPROM enabled boards, you should save the current session
  // by calling "saveSession" which allows retrieving the session after reboot or deepsleep
  node.saveSession();

  // wait before sending another packet
  uint32_t minimumDelay = 60000;                  // try to send once every minute
  uint32_t interval = node.timeUntilUplink();     // calculate minimum duty cycle delay (per law!)
  uint32_t delayMs = max(interval, minimumDelay); // cannot send faster than duty cycle allows

  delay(delayMs);
}
