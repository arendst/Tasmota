/*
   RadioLib STM32WLx Channel Activity Detection Example

   This example uses STM32WLx to scan the current LoRa
   channel and detect ongoing LoRa transmissions.
   Unlike SX127x CAD, SX126x/STM32WLx can detect any part
   of LoRa transmission, not just the preamble.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// no need to configure pins, signals are routed to the radio internally
STM32WLx radio = new STM32WLx_Module();

// set RF switch configuration for Nucleo WL55JC1
// NOTE: other boards may be different!
//       Some boards may not have either LP or HP.
//       For those, do not set the LP/HP entry in the table.
static const uint32_t rfswitch_pins[] =
                         {PC3,  PC4,  PC5};
static const Module::RfSwitchMode_t rfswitch_table[] = {
  {STM32WLx::MODE_IDLE,  {LOW,  LOW,  LOW}},
  {STM32WLx::MODE_RX,    {HIGH, HIGH, LOW}},
  {STM32WLx::MODE_TX_LP, {HIGH, HIGH, HIGH}},
  {STM32WLx::MODE_TX_HP, {HIGH, LOW,  HIGH}},
  END_OF_MODE_TABLE,
};

void setup() {
  Serial.begin(9600);

  // set RF switch control configuration
  // this has to be done prior to calling begin()
  radio.setRfSwitchTable(rfswitch_pins, rfswitch_table);

  // initialize STM32WLx with default settings, except frequency
  Serial.print(F("[STM32WLx] Initializing ... "));
  int state = radio.begin(868.0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when LoRa packet or timeout is detected
  radio.setDio1Action(setFlag);

  // start scanning the channel
  Serial.print(F("[STM32WLx] Starting scan for LoRa preamble ... "));
  state = radio.startChannelScan();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}

// flag to indicate that a packet was detected or CAD timed out
volatile bool scanFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // something happened, set the flag
  scanFlag = true;
}

void loop() {
  // check if the flag is set
  if(scanFlag) {
    // reset flag
    scanFlag = false;

    // check CAD result
    int state = radio.getChannelScanResult();

    if (state == RADIOLIB_LORA_DETECTED) {
      // LoRa packet was detected
      Serial.println(F("[STM32WLx] Packet detected!"));

    } else if (state == RADIOLIB_CHANNEL_FREE) {
      // channel is free
      Serial.println(F("[STM32WLx] Channel is free!"));

    } else {
      // some other error occurred
      Serial.print(F("[STM32WLx] Failed, code "));
      Serial.println(state);

    }

    // start scanning the channel again
    Serial.print(F("[STM32WLx] Starting scan for LoRa preamble ... "));
    state = radio.startChannelScan();
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
  }
}
