# Modbus JSON configurations

Modbus JSON configuration data for use with generic Energy Modbus driver.

## Options to select modbus JSON data

1. Rule driven - copy the required JSON content to any rule buffer preceded with ``rule3 on file#modbus do `` and
  followed with `` endon``.
2. Script driven - copy the required JSON content to a script preceded with ``>y`` and
  followed with ``#``.
3. File system driven - if a file system is present (every ESP32) create a file called ``modbus.json`` and copy the required JSON
  content to it.

## Notes
 - To enter long rules using the serial console and solve error "Serial buffer overrun" you might need to enlarge the serial input buffer with command ``serialbuffer 800``
 - Changes to configurations are activated only after command ``restart``

## Default configurations

```
DDSU6666           {"Name":"DDSU666","Baud":9600,"Config":"8N1","Address":1,"Function":4,"Voltage":0x2000,"Current":0x2002,"Power":0x2004,"ReactivePower":0x2006,"Factor":0x200A,"Frequency":0x200E,"Total":0x4000,"ExportActive":0x400A}

PZEM014            {"Name":"PZEM014","Baud":9600,"Config":"8N1","Address":1,"Function":4,"Voltage":{"R":0,"T":3,"F":-1},"Current":{"R":1,"T":8,"F":-3},"Power":{"R":3,"T":8,"F":-1},"Factor":{"R":8,"T":3,"F":-2},"Frequency":{"R":7,"T":3,"F":-1},"Total":{"R":5,"T":8,"F":-3}}

3 x PZEM014        {"Name":"3 x PZEM014","Baud":9600,"Config":"8N1","Address":[1,2,3],"Function":4,"Voltage":{"R":0,"T":3,"F":-1},"Current":{"R":1,"T":8,"F":-3},"Power":{"R":3,"T":8,"F":-1},"Factor":{"R":8,"T":3,"F":-2},"Frequency":{"R":7,"T":3,"F":-1},"Total":{"R":5,"T":8,"F":-3}}

SDM230             {"Name":"SDM230","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A}

Solax X3MIC        {"Name":"Solax X3MIC","Baud":9600,"Config":"8N1","Address":1,"Function":4,"Voltage":{"R":0x0404,"T":3,"F":-1},"Power":{"R":0x040e,"T":3,"F":0},"Total":{"R":0x0423,"T":8,"F":-3}}

WE517              {"Name":"WE517","Baud":9600,"Config":"8E1","Address":1,"Function":3,"Voltage":[0xE,0x10,0x12],"Current":[0x16,0x18,0x1A],"Power":[0x1E,0x20,0x22],"ReactivePower":[0x26,0x28,0x2A],"Factor":[0x36,0x38,0x3A],"Frequency":0x14,"Total":0x100}
```

## Extended configurations
```
SDM72              {"Name":"SDM72","Baud":9600,"Config":"8N1","Address":0x01,"Function":0x04,"Power":0x0034,"Total":0x0156,"ExportActive":0x004A,"User":[{"R":0x0502,"J":"ImportActive","G":"Import Active","U":"kWh","D":24},{"R":0x0502,"J":"ExportPower","G":"Export Power","U":"W","D":23},{"R":0x0500,"J":"ImportPower","G":"Import Power","U":"W","D":23}]}

SDM120             {"Name":"SDM120","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x0048,"J":"ImportActive","G":"Import Active","U":"kWh","D":24},{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":24},{"R":0x004C,"J":"ImportReactive","G":"Import Reactive","U":"kVArh","D":24},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]}

SDM230             {"Name":"SDM230 with two user registers","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]}

SDM630             {"Name":"SDM630","Baud":9600,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,2,4],"Current":[6,8,10],"Power":[12,14,16],"ApparentPower":[18,20,22],"ReactivePower":[24,26,28],"Factor":[30,32,34],"Frequency":70,"Total":342,"ExportActive":[352,354,356],"User":{"R":[346,348,350],"J":"ImportActive","G":"Import Active","U":"kWh","D":24}}

WE517              {"Name":"WE517","Baud":9600,"Config":"8E1","Address":1,"Function":3,"Voltage":[0xE,0x10,0x12],"Current":[0x16,0x18,0x1A],"Power":[0x1E,0x20,0x22],"ReactivePower":[0x26,0x28,0x2A],"Factor":[0x36,0x38,0x3A],"Frequency":0x14,"Total":0x100,"ExportActive":0x110,"User":[{"J":"ImportActive","G":"Import Active","R":0x108,"U":"kWh","D":24},{"J":"TotalPower","G":"Active Power Total","R":0x1C,"U":"W","D":23}]}
```