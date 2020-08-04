/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2 on arduino and digital pin 1 on ESP8266.
* 
* 
*Detected codes example:
 code: 8233372 Period: 273
 unit: 1
 groupBit: 0
 switchType: 0
* 
*  https://github.com/1technophile/NewRemoteSwitch
*  https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
*  ----------------------------------------------------------
*  Mod by Little Satan. Have Fun!
*  ----------------------------------------------------------
* 
*/
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <NewRemoteReceiver.h>

int pin; // int for Receive pin.

void setup() {
  Serial.begin(115200);

#ifdef ESP32
pin = 4;  // for esp32! Receiver on GPIO pin 4. 
#elif ESP8266
pin = 4;  // for esp8266! Receiver on pin 4 = D2.
#else
pin = 0;  // for Arduino! Receiver on interrupt 0 => that is pin #2
#endif  

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  
  ELECHOUSE_cc1101.SetRx();     // set Receive on
  
  NewRemoteReceiver::init(pin, 2, showCode);
  Serial.println("Receiver initialized");    
}

void loop() {

}

// Callback function is called only when a valid code is received.
void showCode(unsigned int period, unsigned long address, unsigned long groupBit, unsigned long unit, unsigned long switchType) {

  // Print the received code.
  Serial.print("Code: ");
  Serial.print(address);
  Serial.print(" Period: ");
  Serial.println(period);
  Serial.print(" unit: ");
  Serial.println(unit);
  Serial.print(" groupBit: ");
  Serial.println(groupBit);
  Serial.print(" switchType: ");
  Serial.println(switchType);

}
