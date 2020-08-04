/*
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 *
 * When run, this sketch waits for a valid code from a new-style the receiver,
 * decodes it, and retransmits it after 1 seconds.
 *
 *  
 *  https://github.com/1technophile/NewRemoteSwitch
 *  https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 *  ----------------------------------------------------------
 *  Written by Roman.(Arduino Forum) THX!
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 */

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>

int pinTx;        // int for Receive pin.
int pinRx;        // int for Receive pin.
int Interr = 2;   // Interrupt Numer
int Anz = 3;      // number of retransmissions
int Tw = 0;       // Wait Miliseconds before sending

int debug = 1; // Debugmode ein (1)/aus(0)

void setup() {
Serial.begin(115200);

#ifdef ESP32
pinRx = 4; pinTx = 2;  // for esp32! Receiver on GPIO pin 4. Transmit on GPIO pin 2.
#elif ESP8266
pinRx = 4; pinTx = 5;  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
#else
pinRx = 0; pinTx = 6;  // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
#endif   

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  

    
  ELECHOUSE_cc1101.SetRx();     // set Receive on
  // See example ShowReceivedCode for info on this
  NewRemoteReceiver::init(pinRx, Interr, ReTrans);
  if (debug == 1) {Serial.println("Receiver initialized... ");}}

void loop() {
}

 void ReTrans(unsigned int period, unsigned long address, unsigned long groupBit, unsigned long unit, unsigned long switchType){  // Disable the receiver; otherwise it might pick up the retransmit as well.
  
   if (debug == 1) {
    // Print the received code.
    Serial.print("Receiver: ");
    Serial.print("Code: ");
    Serial.print(address);
    Serial.print(" unit: ");
    Serial.print(unit);
    Serial.print(" switchType: ");
    Serial.print(switchType);
    Serial.print(" Period: ");
    Serial.print(period);
    Serial.print(" groupBit: ");
    Serial.println(groupBit);
   }



   if (debug == 1) {Serial.print("Send: Receiver disable... ");}
    NewRemoteReceiver::disable();

   // Need interrupts for delay()
    interrupts();
  
   if (debug == 1) {Serial.print("Wait... ");}
   
   // Wait 1 seconds before sending.
      delay(Tw);


  
    ELECHOUSE_cc1101.SetTx();  // set Transmit on
    // Create a new transmitter with the received address and period, use digital pin as output pin
    NewRemoteTransmitter transmitter(address, pinTx, period, Anz);
    if (debug == 1) {Serial.print("Send: Addr " + String(address) + " unit " +  String(unit)+" "+ String(switchType)+", period: "+String(period)+" " );}
  
      // On/Off signal received
      bool isOn = switchType == NewRemoteCode::on;
      
       if (groupBit) {
        // Send to the group
        transmitter.sendGroup(isOn);
      }
      else {
        // Send to a single unit
        transmitter.sendUnit(unit, isOn);
      }
  
    if (debug == 1) {Serial.println("Receiver enable!");}
    ELECHOUSE_cc1101.SetRx();  // set Receive on
    NewRemoteReceiver::enable();
}
