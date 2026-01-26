/*
DS250x add-only programmable memory reader w/SKIP ROM.
 
 The DS250x is a 512/1024bit add-only PROM(you can add data but cannot change the old one) that's used mainly for device identification purposes
 like serial number, mfgr data, unique identifiers, etc. It uses the Maxim 1-wire bus.
 
 This sketch will use the SKIP ROM function that skips the 1-Wire search phase since we only have one device connected in the bus on digital pin 6.
 If more than one device is connected to the bus, it will fail.
 Sketch will not verify if device connected is from the DS250x family since the skip rom function effectively skips the family-id byte readout.
 thus it is possible to run this sketch with any Maxim OneWire device in which case the command CRC will most likely fail.
 Sketch will only read the first page of memory(32bits) starting from the lower address(0000h), if more than 1 device is present, then use the sketch with search functions.
 Remember to put a 4.7K pullup resistor between pin 6 and +Vcc
 
 To change the range or ammount of data to read, simply change the data array size, LSB/MSB addresses and for loop iterations
 
 This example code is in the public domain and is provided AS-IS.
 
 Built with Arduino 0022 and PJRC OneWire 2.0 library http://www.pjrc.com/teensy/td_libs_OneWire.html
 
 created by Guillermo Lovato <glovato@gmail.com>
 march/2011
 
 */

#include <OneWire.h>
OneWire ds(6);                    // OneWire bus on digital pin 6
void setup() {
  Serial.begin (9600);
}

void loop() {
  byte i;                         // This is for the for loops
  boolean present;                // device present var
  byte data[32];                  // container for the data from device
  byte leemem[3] = {              // array with the commands to initiate a read, DS250x devices expect 3 bytes to start a read: command,LSB&MSB adresses
    0xF0 , 0x00 , 0x00   };       // 0xF0 is the Read Data command, followed by 00h 00h as starting address(the beginning, 0000h)
  byte ccrc;                      // Variable to store the command CRC
  byte ccrc_calc;

  present = ds.reset();           // OneWire bus reset, always needed to start operation on the bus, returns a 1/TRUE if there's a device present.
  ds.skip();                      // Skip ROM search

  if (present == TRUE){           // We only try to read the data if there's a device present 
    Serial.println("DS250x device present");
    ds.write(leemem[0],1);        // Read data command, leave ghost power on
    ds.write(leemem[1],1);        // LSB starting address, leave ghost power on
    ds.write(leemem[2],1);        // MSB starting address, leave ghost power on

    ccrc = ds.read();             // DS250x generates a CRC for the command we sent, we assign a read slot and store it's value
    ccrc_calc = OneWire::crc8(leemem, 3);  // We calculate the CRC of the commands we sent using the library function and store it

    if ( ccrc_calc != ccrc) {      // Then we compare it to the value the ds250x calculated, if it fails, we print debug messages and abort
      Serial.println("Invalid command CRC!");
      Serial.print("Calculated CRC:");
      Serial.println(ccrc_calc,HEX);    // HEX makes it easier to observe and compare
      Serial.print("DS250x readback CRC:");
      Serial.println(ccrc,HEX);
      return;                      // Since CRC failed, we abort the rest of the loop and start over
    }
    Serial.println("Data is: ");   // For the printout of the data 
    for ( i = 0; i < 32; i++) {    // Now it's time to read the PROM data itself, each page is 32 bytes so we need 32 read commands
      data[i] = ds.read();         // we store each read byte to a different position in the data array 
      Serial.print(data[i]);       // printout in ASCII
      Serial.print(" ");           // blank space 
    }
    Serial.println();
    delay(5000);                    // Delay so we don't saturate the serial output
  }
  else {                           // Nothing is connected in the bus 
    Serial.println("Nothing connected");
    delay(3000);
  }
}

















