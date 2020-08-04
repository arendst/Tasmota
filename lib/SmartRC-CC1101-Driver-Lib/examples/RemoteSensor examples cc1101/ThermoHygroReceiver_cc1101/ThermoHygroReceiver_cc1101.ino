/*
 * This sketch receives and decodes data from a 433MHz thermo/hygro weather sensor.
 * The received data (temperature, humidity, channel) is echo
 *
 * Setup:
 * - Connect digital output of a 433MHz receiver to digital pin 2 of Arduino
 * - Enable the serial monitor at 115200 baud. 
 *
 * Need library:
 * https://github.com/mattwire/arduino-dev/tree/master/libraries/RemoteSensor
 * https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
 *
 */
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <SensorReceiver.h>

int pin;

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
  ELECHOUSE_cc1101.SetRx();       // set Receive on
  
  // Init the receiver on interrupt pin 0 (digital pin 2).
  // Set the callback to function "showTempHumi", which is called
  // whenever valid sensor data has been received.
  SensorReceiver::init(pin, showTempHumi);
}

void loop() {
  // Empty! However, you can do other stuff here if you like.
}

void showTempHumi(byte *data) {
  // is data a ThermoHygro-device?
  if ((data[3] & 0x1f) == 0x1e) {
    // Yes!

    byte channel, randomId;
    int temp;
    byte humidity;

    // Decode the data
    SensorReceiver::decodeThermoHygro(data, channel, randomId, temp, humidity);

    // Print temperature. Note: temp is 10x the actual temperature!
    Serial.print("Temperature: ");
    Serial.print(temp / 10); // units
    Serial.print('.');
    Serial.print(temp % 10); // decimal

    // Print humidity
    Serial.print(" deg, Humidity: ");
    Serial.print(humidity);
    Serial.print("% REL");

    // Print channel
    Serial.print(", Channel: ");
    Serial.println(channel, DEC);   
  }
}
