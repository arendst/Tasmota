# JSON value pair description

## Example
```
{"Name":"SDM230","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A}
```

## Modbus config parameters
```
  Name          - Name of energy monitoring device(s)
  Baud          - Baudrate of device modbus interface - optional. default is 9600
  Config        - Serial config parameters like 8N1 - 8 databits, No parity, 1 stop bit
  Poll          - Time between modbus requests - optional. default is 200 milliseconds
  Address       - Modbus device address entered as decimal (1) or hexadecimal (0x01) or up to three addresses ([1,2,3]) - optional. default = 1
  Function      - Modbus function code to access registers - optional. default = 4
```
## Tasmota default embedded register names
```
  Voltage       - Voltage register entered as decimal or hexadecimal for one phase (0x0000) or up to three phases ([0x0000,0x0002,0x0004]) or
                  Additional defined parameters
                  Value pair description:
                  {"R":0,"T":0,"F":0}
                  R - Modbus register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164])
                  T - Datatype - optional. default is 0 - float:
                      0 - float
                      1 = 2-byte signed
                      2 = 4-byte signed
                      3 = 2-byte unsigned
                      4 = 4-byte unsigned
                      5 = not used
                      6 = 4-byte signed with swapped words
                      7 = not used
                      8 = 4-byte unsigned with swapped words
                  F - Register factor positive for multiplication or negative for division - optional. default is 0 - no action
                      -4 - divide by 10000
                      -3 - divide by 1000
                      -2 - divide by 100
                      -1 - divide by 10
                      0 - no action
                      1 - multiply by 10
                      2 - multiply by 100
                      3 - multiply by 1000
                      4 - multiply by 10000
  Current       - Current register entered as decimal or hexadecimal for one phase (0x0006) or up to three phases ([0x0006,0x0008,0x000A]) or
                  See additional defines like voltage.
  Power         - Active power register entered as decimal or hexadecimal for one phase (0x000C) or up to three phases ([0x000C,0x000E,0x0010]) or
                  See additional defines like voltage.
  ApparentPower - Apparent power register entered as decimal or hexadecimal for one phase (0x000C) or up to three phases ([0x000C,0x000E,0x0010]) or
                  See additional defines like voltage.
  ReactivePower - Reactive power register entered as decimal or hexadecimal for one phase (0x0018) or up to three phases ([0x0018,0x001A,0x001C]) or
                  See additional defines like voltage.
  Factor        - Power factor register entered as decimal or hexadecimal for one phase (0x001E) or up to three phases ([0x001E,0x0020,0x0022]) or
                  See additional defines like voltage.
  Frequency     - Frequency register entered as decimal or hexadecimal for one phase (0x0046) or up to three phases ([0x0046,0x0048,0x004A]) or
                  See additional defines like voltage.
  Total         - Total active energy register entered as decimal or hexadecimal for one phase (0x0156) or up to three phases ([0x015A,0x015C,0x015E]) or
                  See additional defines like voltage.
  ExportActive  - Export active energy register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164]) or
                  See additional defines like voltage.
```
## Optional user defined registers
```
  User          - Additional user defined registers
                  Value pair description:
                  "User":{"R":0x0024,"T":0,"F":0,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}
                  R - Modbus register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164])
                  T - Datatype - optional. default is 0 - float:
                      0 - float
                      1 = 2-byte signed
                      2 = 4-byte signed
                      3 = 2-byte unsigned
                      4 = 4-byte unsigned
                      5 = not used
                      6 = 4-byte signed with swapped words
                      7 = not used
                      8 = 4-byte unsigned with swapped words
                  F - Register factor positive for multiplication or negative for division - optional. default is 0 - no action
                      -4 - divide by 10000
                      -3 - divide by 1000
                      -2 - divide by 100
                      -1 - divide by 10
                      0 - no action
                      1 - multiply by 10
                      2 - multiply by 100
                      3 - multiply by 1000
                      4 - multiply by 10000
                  J - JSON register name (preferrably without spaces like "PhaseAngle") - mandatory. It needs to be different from the Tasmota default embedded register names
                  G - GUI register name - optional. If not defined the register will not be shown in the GUI
                  U - GUI unit name - optional. default is none
                  D - Number of decimals for floating point presentation (0 to 20) or a code correspondig to Tasmota resolution command settings:
                      21 - VoltRes (V)
                      22 - AmpRes (A)
                      23 - WattRes (W, VA, var)
                      24 - EnergyRes (kWh, kVAh, kvarh)
                      25 - FreqRes (Hz)
                      26 - TempRes (C, F)
                      27 - HumRes (%)
                      28 - PressRes (hPa, mmHg)
                      29 - WeightRes (Kg)
```