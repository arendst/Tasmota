# OpenTherm Arduino/ESP8266/ESP32 Library

This library provides implementation of OpenTherm protocol.

OpenTherm Library is based on OpenTherm protocol specification v2.2 and works with all OpenTherm compatible boilers. Library can be easily installed into Arduino IDE and compiled for Arduino, ESP8266/ESP32 and other similar controllers. 

OpenTherm protocol requires simple low voltage twowire connection to boiler, but voltage levels (7..15V) still much higher than Arduino/ESP8266 levels, which requires [OpenTherm Adapter](http://ihormelnyk.com/opentherm_adapter).

This version of library uses interrupts to achieve better stability and synchronization with boiler.

## Using OpenTherm Library you will be able:
- control your boiler remotely (get status, switch on/off heating/water heating, set water temperature and much more)
- make custom thermostat

## Configuration and Usage:
```c
#include <OpenTherm.h>
```
You have to choose 2 controller GPIO pins which will be used for communication and connected to [OpenTherm Adapter](http://ihormelnyk.com/opentherm_adapter). Controller(Arduino/ESP8266) input pin should support interrupts.
Controller output pin should be connected to OpenTherm Adapter input pin and vise versa.
```c
const int inPin = 2;
const int outPin = 3;
```
Define OpenTherm class instance using constructor which accepts as arguments pin numbers:
```c
OpenTherm ot(inPin, outPin);
```
Define interrupts handler function for specified above instance:
```c
void handleInterrupt() {
	ot.handleInterrupt();
}
```
Use begin function to initialize OpenTherm instance, specify interrupts handler function as argument
```c
void setup()
{
    ot.begin(handleInterrupt);
}
```
According to OpenTherm Protocol specification master (controller) must communicate at least every 1 sec. So lets make some requests in loop function:
```c
void loop()
{	
    //Set/Get Boiler Status
    ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
    //Set Boiler Temperature to 64 degrees C
    ot.setBoilerTemperature(64);
    //Get Boiler Temperature
    float temperature = ot.getBoilerTemperature();
    delay(1000);
}
```

In details [OpenTherm Library](http://ihormelnyk.com/opentherm_library) described [here](http://ihormelnyk.com/opentherm_library).

## OpenTherm Adapter Schematic
![opentherm adapter schmetic](http://ihormelnyk.com/Content/Pages/opentherm_adapter/opentherm_adapter_schematic.png)

## Arduino UNO Connection
![opentherm adapter arduino](http://ihormelnyk.com/Content/Pages/opentherm_adapter/opentherm_adapter_arduino_connection.png)

## License
Copyright (c) 2018 [Ihor Melnyk](http://ihormelnyk.com). Licensed under the [MIT license](/LICENSE?raw=true).
