/*
  RadioLib STM32WLx Blocking Receive Example

  This example listens for LoRa transmissions using STM32WL MCU with
  integrated (SX126x) LoRa radio.

  To successfully receive data, the following settings have to be the same
  on both transmitter and receiver:
  - carrier frequency
  - bandwidth
  - spreading factor
  - coding rate
  - sync word
  - preamble length
   
  This example assumes Nucleo WL55JC1 is used. For other Nucleo boards
  or standalone STM32WL, some configuration such as TCXO voltage and
  RF switch control may have to be adjusted.

  Using blocking receive is not recommended, as it will lead
  to significant amount of timeouts, inefficient use of processor
  time and can some miss packets!
  Instead, interrupt receive is recommended.

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

  // initialize STM32WL with default settings, except frequency
  Serial.print(F("[STM32WL] Initializing ... "));
  int state = radio.begin(868.0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set appropriate TCXO voltage for Nucleo WL55JC1
  state = radio.setTCXO(1.7);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[STM32WL] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  String str;
  int state = radio.receive(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[STM32WL] Data:\t\t"));
    Serial.println(str);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[STM32WL] RSSI:\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("[STM32WL] SNR:\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
