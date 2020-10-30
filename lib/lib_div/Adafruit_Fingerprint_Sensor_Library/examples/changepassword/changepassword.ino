/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor
  Designed specifically to work with the Adafruit Fingerprint sensor
  ----> http://www.adafruit.com/products/751
  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(2, 3);

// Using sensor without password
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Using sensor with password
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial, 1337);

void setup()  
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  
  Serial.begin(9600);
  Serial.println("Adafruit fingerprint sensor, change password example");

  // set the data rate for the sensor serial port
  finger.begin(19200);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

  Serial.print("Set password... ");
  uint8_t p = finger.setPassword(1337);
  if (p == FINGERPRINT_OK) {
    Serial.println("OK"); // Password is set
  } else {
    Serial.println("ERROR"); // Failed to set password
  }
}

void loop()
{

}

