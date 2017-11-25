#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

void setup() {

  Serial.begin(115200);

}

void loop() {

  if(sht30.get()==0){
    Serial.print("Temperature in Celsius : ");
    Serial.println(sht30.cTemp);
    Serial.print("Temperature in Fahrenheit : ");
    Serial.println(sht30.fTemp);
    Serial.print("Relative Humidity : ");
    Serial.println(sht30.humidity);
    Serial.println();
  }
  else
  {
    Serial.println("Error!");
  }
  delay(1000);

}
