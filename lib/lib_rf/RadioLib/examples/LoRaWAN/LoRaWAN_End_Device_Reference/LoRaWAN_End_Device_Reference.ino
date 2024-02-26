/*
  RadioLib LoRaWAN End Device Reference Example

  This example joins a LoRaWAN network and will send
  uplink packets. Before you start, you will have to
  register your device at https://www.thethingsnetwork.org/
  After your device is registered, you can run this example.
  The device will join the network and start uploading data.

  Also, most of the possible and available functions are
  shown here for reference.

  LoRaWAN v1.1 requires the use of EEPROM (persistent storage).
  Please refer to the 'persistent' example once you are familiar
  with LoRaWAN.
  Running this examples REQUIRES you to check "Resets DevNonces"
  on your LoRaWAN dashboard. Refer to the network's 
  documentation on how to do this.

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

  // application identifier - pre-LoRaWAN 1.1.0, this was called appEUI
  // when adding new end device in TTN, you will have to enter this number
  // you can pick any number you want, but it has to be unique
  uint64_t joinEUI = 0x12AD1011B0C0FFEE;

  // device identifier - this number can be anything
  // when adding new end device in TTN, you can generate this number,
  // or you can set any value you want, provided it is also unique
  uint64_t devEUI = 0x70B3D57ED005E120;

  // select some encryption keys which will be used to secure the communication
  // there are two of them - network key and application key
  // because LoRaWAN uses AES-128, the key MUST be 16 bytes (or characters) long

  // network key is the ASCII string "topSecretKey1234"
  uint8_t nwkKey[] = { 0x74, 0x6F, 0x70, 0x53, 0x65, 0x63, 0x72, 0x65,
                       0x74, 0x4B, 0x65, 0x79, 0x31, 0x32, 0x33, 0x34 };

  // application key is the ASCII string "aDifferentKeyABC"
  uint8_t appKey[] = { 0x61, 0x44, 0x69, 0x66, 0x66, 0x65, 0x72, 0x65,
                       0x6E, 0x74, 0x4B, 0x65, 0x79, 0x41, 0x42, 0x43 };

  // prior to LoRaWAN 1.1.0, only a single "nwkKey" is used
  // when connecting to LoRaWAN 1.0 network, "appKey" will be disregarded
  // and can be set to NULL


  // now we can start the activation
  // this can take up to 10 seconds, and requires a LoRaWAN gateway in range
  // a specific starting-datarate can be selected in dynamic bands (e.g. EU868):
  /* 
    uint8_t joinDr = 4;
    state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey, joinDr);
  */
  Serial.print(F("[LoRaWAN] Attempting over-the-air activation ... "));
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

  if(state >= RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
    delay(2000);	// small delay between joining and uplink
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  Serial.print("[LoRaWAN] DevAddr: ");
  Serial.println(node.getDevAddr(), HEX);

  // on EEPROM-enabled boards, after the device has been activated,
  // the session can be restored without rejoining after device power cycle
  // this is intrinsically done when calling `beginOTAA()` with the same keys
  // or if you 'lost' the keys or don't want them included in your sketch
  // you can call `restore()`
  /*
    Serial.print(F("[LoRaWAN] Resuming previous session ... "));
    state = node.restore();
    if(state >= RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while(true);
    }
  */

  // disable the ADR algorithm
  node.setADR(false);

  // set a fixed datarate
  node.setDatarate(5);
  // in order to set the datarate persistent across reboot/deepsleep, use the following:
  /*
    node.setDatarate(5, true);  
  */

  // enable CSMA
  // this tries to minimize packet loss by searching for a free channel
  // before actually sending an uplink 
  node.setCSMA(6, 2, true);

  // enable or disable the dutycycle
  // the second argument specific allowed airtime per hour in milliseconds
  // 1250 = TTN FUP (30 seconds / 24 hours)
  // if not called, this corresponds to setDutyCycle(true, 0)
  // setting this to 0 corresponds to the band's maximum allowed dutycycle by law
  node.setDutyCycle(true, 1250);

  // enable or disable the dwell time limits
  // the second argument specifies the allowed airtime per uplink in milliseconds
  // unless specified, this argument is set to 0
  // setting this to 0 corresponds to the band's maximum allowed dwell time by law
  node.setDwellTime(true, 400);
}

void loop() {
  int state = RADIOLIB_ERR_NONE;

  // set battery fill level - the LoRaWAN network server
  // may periodically request this information
  // 0 = external power source
  // 1 = lowest (empty battery)
  // 254 = highest (full battery)
  // 255 = unable to measure
  uint8_t battLevel = 146;
  node.setDeviceStatus(battLevel);

  // retrieve the last uplink frame counter
  uint32_t fcntUp = node.getFcntUp();

  Serial.print(F("[LoRaWAN] Sending uplink packet ... "));
  String strUp = "Hello!" + String(fcntUp);
  
  // send a confirmed uplink to port 10 every 64th frame
  // and also request the LinkCheck and DeviceTime MAC commands
  if(fcntUp % 64 == 0) {
    node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_LINK_CHECK);
    node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_DEVICE_TIME);
    state = node.uplink(strUp, 10, true);
  } else {
    state = node.uplink(strUp, 10);
  }
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // after uplink, you can call downlink(),
  // to receive any possible reply from the server
  // this function must be called within a few seconds
  // after uplink to receive the downlink!
  Serial.print(F("[LoRaWAN] Waiting for downlink ... "));
  String strDown;

  // you can also retrieve additional information about 
  // uplink or downlink by passing a reference to
  // LoRaWANEvent_t structure
  LoRaWANEvent_t event;
  state = node.downlink(strDown, &event);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));

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

    // print extra information about the event
    Serial.println(F("[LoRaWAN] Event information:"));
    Serial.print(F("[LoRaWAN] Direction:\t"));
    if(event.dir == RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK) {
      Serial.println(F("uplink"));
    } else {
      Serial.println(F("downlink"));
    }
    Serial.print(F("[LoRaWAN] Confirmed:\t"));
    Serial.println(event.confirmed);
    Serial.print(F("[LoRaWAN] Confirming:\t"));
    Serial.println(event.confirming);
    Serial.print(F("[LoRaWAN] Datarate:\t"));
    Serial.println(event.datarate);
    Serial.print(F("[LoRaWAN] Frequency:\t"));
    Serial.print(event.freq, 3);
    Serial.println(F(" MHz"));
    Serial.print(F("[LoRaWAN] Output power:\t"));
    Serial.print(event.power);
    Serial.println(F(" dBm"));
    Serial.print(F("[LoRaWAN] Frame count:\t"));
    Serial.println(event.fcnt);
    Serial.print(F("[LoRaWAN] Port:\t\t"));
    Serial.println(event.port);
    
    Serial.print(radio.getFrequencyError());

    uint8_t margin = 0;
    uint8_t gwCnt = 0;
    if(node.getMacLinkCheckAns(&margin, &gwCnt)) {
      Serial.print(F("[LoRaWAN] LinkCheck margin:\t"));
      Serial.println(margin);
      Serial.print(F("[LoRaWAN] LinkCheck count:\t"));
      Serial.println(gwCnt);
    }

    uint32_t networkTime = 0;
    uint8_t fracSecond = 0;
    if(node.getMacDeviceTimeAns(&networkTime, &fracSecond, true)) {
      Serial.print(F("[LoRaWAN] DeviceTime Unix:\t"));
      Serial.println(networkTime);
      Serial.print(F("[LoRaWAN] LinkCheck second:\t1/"));
      Serial.println(fracSecond);
    }
  
  } else if(state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println(F("timeout!"));
  
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // on EEPROM enabled boards, you can save the current session
  // by calling "saveSession" which allows retrieving the session after reboot or deepsleep
  /*
    node.saveSession();
  */

  // wait before sending another packet
  uint32_t minimumDelay = 60000;                  // try to send once every minute
  uint32_t interval = node.timeUntilUplink();     // calculate minimum duty cycle delay (per law!)
	uint32_t delayMs = max(interval, minimumDelay); // cannot send faster than duty cycle allows

  delay(delayMs);
}
