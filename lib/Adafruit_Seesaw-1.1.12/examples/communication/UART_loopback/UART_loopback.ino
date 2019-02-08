//This example takes UART data given to the seesaw, reads it and then loops it back

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup()
{
  Serial.begin(9600);
  
  if(!ss.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");

  //enable interrupt
  ss.enableSercomDataRdyInterrupt();
}

void loop()
{
  if(!digitalRead(3)){
    char c = ss.readSercomData();
    Serial.print(c); //print to arduino console

    //delay after reading data
    delayMicroseconds(100);
    ss.print(c); //send back to the seesaw to print
  }
}
