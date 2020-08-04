/*
 * Demo for RF remote switch receiver. 
 * This example is for the new KaKu / Home Easy type of remotes!
 *
 * For details, see NewRemoteReceiver.h!
 *
 * With this sketch you can control a LED connected to digital pin 4,
 * after the sketch learned the code. After start, the LED starts to blink,
 * until a valid code has been received. The led stops blinking. Now you
 * can control the LED with the remote.
 *
 * Note: only unit-switches are supported in this sketch, no group or dim.
 *
 * Arduino only!
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


int pin = 0; // int for Receive pin.
int led = 4; // pin for Led.

boolean codeLearned = false;
unsigned long learnedAddress;
byte learnedUnit;

void setup() {
    

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(16);      // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setPA(10);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  
  ELECHOUSE_cc1101.SetRx();    // set Receive on
  
  // LED-pin as output
  pinMode(led, OUTPUT);

  // Init a new receiver on interrupt pin 0, minimal 2 identical repeats, and callback set to processCode.
  NewRemoteReceiver::init(pin, 2, processCode);
}

void loop() {
  // Blink led until a code has been learned
  if (!codeLearned) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
}

// Callback function is called only when a valid code is received.
void processCode(NewRemoteCode receivedCode) {
  // A code has been received.
  // Do we already know the code?
  if (!codeLearned) {
    // No! Let's learn the received code.
    learnedAddress = receivedCode.address;
    learnedUnit = receivedCode.unit;
    codeLearned = true;    
  } else {
    // Yes!    
    // Is the received code identical to the learned code?
    if (receivedCode.address == learnedAddress && receivedCode.unit == learnedUnit) {
      // Yes!
      // Switch the LED off if the received code was "off".
      // Anything else (on, dim, on_with_dim) will switch the LED on.
      if (receivedCode.switchType == NewRemoteCode::off) {
        digitalWrite(13, LOW);
      } else {
        digitalWrite(13, HIGH);
      }
    }
  }
}
