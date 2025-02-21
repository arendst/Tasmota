## CheapPower Module

This fetches electricity prices and chooses the cheapest future time slots.
Currently, the following price data sources have been implemented:
* `cheap_power_dk_no.tapp`: Denmark, Norway (Nord Pool)
* `cheap_power_elering.tapp`: Estonia, Finland, Latvia, Lithuania (Nord Pool via [Elering](https://elering.ee/en), ex VAT)
* `cheap_power_fi.tapp`: Finland (Nord Pool)
* `cheap_power_se.tapp`: Sweden (Nord Pool); assuming Swedish time zone
* `cheap_power_smard.tapp`: https://smard.de (Central Europe; local time)
* `cheap_power_uk_octopus.tapp`: United Kingdom (Octopus Energy)

### Usage:

* copy the `cheap_power_*.tapp` for your data source to the file system
* Invoke the Tasmota command `BrRestart` or restart the entire firmware
* Invoke the Tasmota command `CheapPower1`, `CheapPower2`, … to
   * download prices for some time into the future
   * automatically choose the cheapest future time slots (default: 1)
   * to schedule `Power1 ON`, `Power2 ON`, … at the chosen slots
   * to install a Web UI in the main menu

### Timer Installation:
```
# Europe/Helsinki time zone; see https://tasmota.github.io/docs/Timezone-Table/
Backlog0 Timezone 99; TimeStd 0,0,10,1,4,120; TimeDst 0,0,3,1,3,180

# Detach Switch1 from Power1
Backlog0 SwitchMode1 15; SwitchTopic1 0
Backlog0 WebButton1 boiler; WebButton2 heat
## Power off after 900 seconds (15 minutes)
#PulseTime1 1000
# Power off after 3600 seconds (60 minutes, 1 hour)
PulseTime1 3700

Rule1 ON Clock#Timer DO CheapPower1 … ENDON
Timer {"Enable":1,"Mode":0,"Time":"18:00","Window":0,"Days":"1111111","Repeat":1,"Output":1,"Action":3}
Rule1 1
Timers 1
```
The download schedule can be adjusted in the timer configuration menu.
The prices for the next day will typically be updated in the afternoon
or evening of the previous day.

In case the prices cannot be downloaded, the download will be retried
in 1, 2, 4, 8, 16, 32, 64, 64, 64, … minutes until it succeeds.

### Additional Parameters

* `cheap_power_fi.tapp` (Finland): none
* `cheap_power_elering.tapp`: price zone FI, EE, LT, LV
* `cheap_power_dk_no.tapp` (Denmark, Norway): price zone DK1, DK2, NO1 to NO5
* `cheap_power_se.tapp` (Sweden): price zone SE1 to SE4
* `cheap_power_smard.tapp`: zone AT, BE, CH, CZ, DE, FR, HU, IT, LU, NL, PL, SI
```
CheapPower1 DE
```
* `cheap_power_uk_octopus.tapp` (United Kingdom): tariff name and price zone:
```
CheapPower AGILE-24-10-01 B
```
The default number of active price slots per day is 1. It can be set in the
web user interface.

### Web User Interface

The user interface in the main menu consists of the following buttons:
* ⏮ moves the first time slot earlier (or wraps from the beginning to the end)
* ⏭ moves the first time slot later (or wraps from the end to the beginning)
* ⏯ pauses (switches off) or chooses the optimal slots
* 🔄 requests the prices to be downloaded and the optimal slots to be chosen
* ➖ decreases the number of slots (minimum: 1)
* ➕ increases the number of slots (maximum: currently available price slots)

The status output above the buttons may indicate that the output
is paused until further command or price update:
```
(0≤1)0.299‥2.38 ¢/kWh	⭘
```
Or it may indicate the price and duration until the next active slot:
```
(1≤1)0.299‥2.38 ¢/kWh (0.299)	⭙ 5:05
```
Or it may indicate the price of the currently active time slot:
```
(1≤1)0.299‥2.38 ¢/kWh (0.299)	⭙
```
The first number indicates the number of remaining scheduled time slots,
and the second number indicates the maximum number of time slots per day
(default 1).

The two quantities around the ‥ are the minimum and maximum known prices
from the current time onwards.

The scheduled slots are also indicated by red bars in the line graph
of the available current and future prices.

### Application Contents

```bash
for i in */cheap_power.be
do
  zip ../cheap_power_${i%/*}.tapp -j -0 autoexec.be \
  cheap_power_base.be binary_heap.be "$i"
done
```
