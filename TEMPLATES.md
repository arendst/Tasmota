<picture>
  <source media="(prefers-color-scheme: dark)" srcset="./tools/logo/TASMOTA_FullLogo_Vector_White.svg">
  <img alt="Logo" src="./tools/logo/TASMOTA_FullLogo_Vector.svg" align="right" height="76">
</picture>

# Templates

Find below the available templates as of August 2024. More template information can be found in the [Tasmota Device Templates Repository](http://blakadder.github.io/templates)

## Adapter Board
```
3DStar ESP-OpenTherm v1.1    {"NAME":"3DS_OpenTherm","GPIO":[0,0,0,0,0,0,0,0,4960,0,4928,0,0,0],"FLAG":0,"BASE":18}
KinCony Ethernet IO Expansion Board  {"NAME":"KC868-AM","GPIO":[1,1,1,1,640,1,1,1,1,1,1,224,608,1,5600,1,0,1,1,5568,0,1,1,1,0,0,0,0,3200,1,1,3232,1,0,0,1],"FLAG":0,"BASE":1}
WifInfo - Teleinfo Server    {"NAME":"WifInfo","GPIO":[1376,1,1,5152,640,608,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":18}
ZiGate-Ethernet              {"NAME":"ZIGATE-ETH","GPIO":[1,1,1,1,1,1,0,0,1,0,1,1,3840,576,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0],"FLAG":0,"BASE":18,"CMND":"EthClockMode 3 | EthAddress 1"}
```

## Air Purifier
```
Alfawise                     {"NAME":"alfawise P2","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
BlitzHome AP1                {"NAME":"BH-AP1","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
BlitzHome AP2501             {"NAME":"BH-AP2501","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Kogan 5-Stage 3S             {"NAME":"Kogan Air Purifier 3S","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Air Quality Sensor
```
AirGradient Pro Version DIY  {"NAME":"AirGradient Pro","GPIO":[1600,1,1632,0,640,608,1,1,1664,1,1696,1,1,1],"FLAG":0,"BASE":18}
Tuya PM Box                  {"NAME":"PM Box","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 99,91 | TuyaMCU 71,2 | TuyaMCU 73,3 | HumRes 1 | TempRes 1 "}
```

## Aroma Diffuser
```
Asakuki 500ml                {"NAME":"Asakuki","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 12,11 | TuyaMCU 21,111 | TuyaMCU 24,108 | TuyaMCU 61,103 | TuyaMCU 62,110"}
BlitzWolf 400ml              {"NAME":"BW-FUN3","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Brilex 400ml                 {"NAME":"BrilexDiffuser","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Essential Oil 400ml          {"NAME":"XD800W","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
GD-30W 300ml                 {"NAME":"GD-30W","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Gernems 400 ml               {"NAME":"YX-088","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1|TuyaMCU 61,2|TuyaMCU 62,3|TuyaMCU 12,5|TuyaMCU 63,6|TuyaMCU 21,7|TuyaMCU 24,8|TuyaRGB 3|DimmerRange 1,255|TuyaEnumList 1,2|TuyaEnumList 2,2|TuyaEnumList 3,1"}
Kbaybo 300ml                 {"NAME":"K-H25","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Kogan 500ml                  {"NAME":"Kogan Difuser","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Maxcio 400ml                 {"NAME":"MaxcioDiffuser","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Mirabella Genio 250ml Glass  {"NAME":"Genio Diffuser","GPIO":[0,0,0,0,416,417,0,0,0,480,225,418,224,1],"FLAG":0,"BASE":18}
Wood Grain 550ML             {"NAME":"MY-KCL01800FB","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Bluetooth Gateway
```
Arlec Smart Home Hub         {"NAME":"Arlec Hub","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
GL.iNet POE Ethernet         {"NAME":"GL-S10 v1.0","GPIO":[32,0,0,0,0,0,0,0,321,608,288,640,0,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,544,33,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"EthType 0 | EthAddress 1 | EthClockMode 3"}
```

## CCT
```
3W KLG 320lm                 {"NAME":"3W_KLG","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
AICase 800lm                 {"NAME":"AICase Smart L","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
AiYaTo 12W                   {"NAME":"AiYaTo-CW","GPIO":[0,0,0,0,416,0,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Ajax Online 380lm            {"NAME":"AjaxOnline","GPIO":[32,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":38}
Ajax Online 7W Vintage       {"NAME":"AjaxOnline-7W","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Anoopsyche 9W 800lm          {"NAME":"Anoop-CW-WW","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 1350lm PAR38     {"NAME":"Arlec GLD302HA","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 9.5W 806lm       {"NAME":"Arlec GLD110HA","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":48}
Arlec Smart 9.5W 806lm       {"NAME":"Arlec CCT","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":48}
Arlec Smart R80 9.5W 806lm   {"NAME":"Arlec R80","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":48}
AZzardo Led Vintage          {"NAME":"Azzardo AZ3210 LightBulb Vintage","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf A70 9W 900lm       {"NAME":"BW-LT29","GPIO":[0,0,0,0,0,0,0,0,0,449,0,416,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20696 9W 900lm  {"NAME":"Brilliant20696","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20697 9W 900lm  {"NAME":"Brilliant20696","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 4.5W 400lm    {"NAME":"Brilliant20764","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 4.5W 400lm    {"NAME":"BS-GU10-20887","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Bulbrite Solana A19 Edison Filament  {"NAME":"BulbBrite01","GPIO":[0,0,0,0,0,0,0,0,416,0,417,0,0,0],"FLAG":0,"BASE":18}
Bulbrite Solana G25 5.5W 600lm Filament  {"NAME":"BulbBrite01","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Bulbrite Solana ST18 16W 140lm Filament  {"NAME":"BulbBrite02","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Calex 5W 345lm 2200-4000K Reflector  {"NAME":"Calex GU10 CCT 5w","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Calex G125 7,5W 1055lm       {"NAME":"Calex G125 E27","GPIO":[0,0,0,0,0,2912,0,0,417,0,416,2976,2944,0],"FLAG":0,"BASE":18}
Calex G125 7.5W 1055lm Globe  {"NAME":"Calex G125 E27","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 10W        {"NAME":"CSH-B22WW10W","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 10W        {"NAME":"CSH-E27WW10W","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 5W         {"NAME":"CSH-E14WW5W","GPIO":[0,0,0,0,416,417,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 5W         {"NAME":"Connect CSH-GU10WW5W","GPIO":[0,0,0,0,416,417,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome GU5.3 5W   {"NAME":"Connect CSH-GU53WW5W","GPIO":[0,0,0,0,416,417,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Deltaco SH-LE14W 470lm       {"NAME":"SH-LE14W","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Deltaco SH-LE27W 810lm       {"NAME":"SH-LE27W","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
DORESshop A60 720lm Filament  {"NAME":"DORESshop-A60","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Energizer A19 10W Multi-White  {"NAME":"Energizer CCT ","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Energizer A19 10W Smart White  {"NAME":"Energizer CCT ","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Euri Lighting A19 10W 800lm  {"NAME":"Euri Lighting ","GPIO":[0,0,0,0,416,417,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Geeni LUX 1050lm             {"NAME":"Geeni-1050-WW","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,4704],"FLAG":0,"BASE":18}
Globe A19 10W 800lm          {"NAME":"GlobeCCT","GPIO":[0,0,0,0,417,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Globe G25 Edison 500lm       {"NAME":"Globe 34920","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Globe ST19 Edison 500lm      {"NAME":"Globe 34919","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Hama 806lm                   {"NAME":"Hama 00176550","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Hama 4.5W 300lm              {"NAME":"Hama LED-Bulb GU10","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Hama 4.5W 350lm              {"NAME":"Hama LED-Bulb","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Hykker SL-0392 650lm         {"NAME":"Hykker 7W","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Iotton 9W 700lm              {"NAME":"Iotton Light","GPIO":[0,0,0,0,416,417,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
iQtech 10W 900lm             {"NAME":"iQ-Tech CCT 10W 900LM","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
iView 10W 1050lm             {"NAME":"iView ISB1000-D","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Kogan 10W Cool & Warm White 1050lm  {"NAME":"Kogan 10W CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":48}
Kogan 4.5W 330lm 110         {"NAME":"Kogan White/Wa","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Kogan 5W                     {"NAME":"Kogan Co/Wa","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Kruidvat A60 9W 806lm        {"NAME":"Kruidvat E27 806 Lumens","GPIO":[0,0,0,0,416,449,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":48}
Laser 10W 1000lm             {"NAME":"Laser 10W CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":48}
Laser 10W 1000lm             {"NAME":"Laser 10W CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
LE LampUX 380lm Candle       {"NAME":"LE Bulb","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
LE LampUX 4.5W 410lm         {"NAME":"LE LampUX","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":48}
ledscom.de 4.5W 430lm        {"NAME":"GX53","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":0}
Lenovo 800lm                 {"NAME":"LenovoLB","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Lenovo SE-141DC              {"NAME":"LenovoSE-141DC","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":48}
Lohas ZN051 5W 420lm         {"NAME":"Lohas ZN051","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN070 720lm            {"NAME":"Lohas ZN070","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
LSC 10W 806lm                {"NAME":"LSC E27 806lumens","GPIO":[0,0,0,0,416,449,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":48}
LSC A55 14W 1400lm           {"NAME":"LSC CT E27","GPIO":[0,0,0,0,416,449,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":48}
Lumiman A19 7.5W 800lm       {"NAME":"Lumiman LM520","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Luminea ZX-2831              {"NAME":"Luminea CCT","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
LVWIT A60 6.5W 806lm Filament  {"NAME":"LVWIT-E27-WiFi-6.5","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
LVWIT E14 Smart Wifi LED Bulb, 470Lm, 4.5W  {"NAME":"LVWIT 4.5w CCT","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Mimoodz A21 10.5W 1050lm     {"NAME":"Mimoodz","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 6W 500lm     {"NAME":"GenioGU10","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"GenioBulbCCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"GenioBulbCCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"GenioBulbCCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio G95 5W 470lm  {"NAME":"Genio Filament","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio SES 5.5W 470lm  {"NAME":"Mirabella Candle","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis 4.5W 350lm Candle      {"NAME":"WIFILW13WTE14","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 5.5W 350lm Twisted Filament  {"NAME":"WIFILT10GDA60","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 800lm              {"NAME":"WIFILW10WTE27","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 9W 800lm           {"NAME":"WIFILW10WTB22","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 9W 800lm           {"NAME":"WIFILW30WTE27","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis C10 350lm              {"NAME":"WIFILW10WTE14","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Nedis G125 5.5W 350lm Twisted Filament  {"NAME":"WIFILF10GDG125","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis PAR16 330lm            {"NAME":"Nedis WIFILW30","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis PAR16 4,5W 380lm       {"NAME":"Nedis WIFILW10WTGU10","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis PAR16 4.5W 330lm 110   {"NAME":"WIFILW30","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Nous P2                      {"NAME":"NOUS-P2","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":37}
Philips Zhirui Candle 250lm  {"NAME":"Xiaomi Philips","GPIO":[0,0,0,0,0,0,0,0,417,0,0,416,0,0],"FLAG":0,"BASE":48}
Phillips Zhirui 450lm        {"NAME":"Xiaomi Philips","GPIO":[0,0,0,0,0,0,0,0,417,0,0,416,0,0],"FLAG":0,"BASE":48}
Polux ST64 5.5W 470lm        {"NAME":"basic","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Prokord 10W 1050lm           {"NAME":"PSH-E2745-CCT","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,4704],"FLAG":0,"BASE":18}
Prokord 5W 1050lm            {"NAME":"Prokord-PSH-GU1045-CCT","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Shelly Duo 800lm             {"NAME":"Shelly DUO","GPIO":[0,0,0,0,417,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
SmartDGM L-WT9W1 9W 800lm    {"NAME":"L-WT9W1","GPIO":[0,0,0,0,0,416,0,0,160,417,0,0,0,0],"FLAG":0,"BASE":18}
Smitch 10W                   {"NAME":"Smitch SB0310 - B22","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Solimo 12W 1080lm            {"NAME":"Solimo WCCT 12","GPIO":[0,0,0,0,160,416,0,0,161,417,162,0,0,0],"FLAG":0,"BASE":18}
Spectrum Smart 5W 410lm Candle  {"NAME":"lightbulb","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Status 9W 806lm              {"NAME":"Status Smart","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Sulion Dante G100 10W 1055lm  {"NAME":"Sulion Bombilla G100","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Sulion Edisson Filament ST64 8W 600lm  {"NAME":"Sulion Bombilla ST64","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Sulion Morgan C37 5W 470lm   {"NAME":"Sulion Bombilla C37","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Swisstone 806lm              {"NAME":"SwisstoneSH330","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Swisstone SH 310             {"NAME":"Swisstone SH 310","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Treatlife A19 9W 800lm       {"NAME":"Treatlife SL20","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
V-Tac PAR16 4.5W 300lm 110   {"NAME":"V-TAC VT-5174","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Vestaiot BR30 800lm          {"NAME":"Vesta BR30 CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Wipro Garnet NS9100 810lm    {"NAME":"Wipro 9W CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":48}
Wyze A19 800lm               {"NAME":"Wyze Bulb","GPIO":[5728,0,0,0,0,0,0,0,0,416,417,0,0,0],"FLAG":0,"BASE":48}
Wyze A19 800lm               {"NAME":"WLPA19v2","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,0,417,416,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
```

## Camera
```
Ai-Thinker                   {"NAME":"AITHINKER CAM","GPIO":[4992,1,672,1,416,5088,1,1,1,6720,736,704,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,576,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
Freenove ESP32-WROVER.DEV    {"NAME":"Freenove-WROVER-Cam","GPIO":[1,1,1,1,5088,5089,0,0,1,1,1,1,1,1,5090,5091,1,4992,5184,5152,1,5120,5024,5056,0,0,0,0,1,1,5094,5095,5092,1,1,5093],"FLAG":0,"BASE":1}
M5Stack ESP32CAM Camera Module OV2640  {"NAME":"M5Cam","GPIO":[0,1,672,1,416,5091,1,1,1,6720,320,4960,1,1,5093,5095,0,5184,5024,5056,0,5120,5152,4992,0,0,0,0,5088,1,5090,5089,5094,0,0,5092],"FLAG":0,"BASE":2}
M5Stack PSRAM Timer Camera (OV3660)  {"NAME":"M5STACK CAM","GPIO":[0,0,0,0,0,5091,1,0,0,0,0,4960,1,0,5093,5095,0,5184,5120,5056,0,5024,5152,4992,0,0,0,0,5088,0,5090,5089,5094,0,0,5092],"FLAG":0,"BASE":2}
OV5640 ESP32                 {"NAME":"ESP32-OV5640-Cam","GPIO":[32,1,5088,1,8832,4960,1,1,5091,8864,5089,4992,1,1,5120,8800,0,8960,5024,5056,0,288,5184,5092,0,0,0,0,1,5093,5094,5090,5152,0,0,5095],"FLAG":0,"BASE":1}
Wireles Tag Camera           {"NAME":"WT-ESP32-CAM","GPIO":[4992,1,1,1,1,5088,0,0,1,1,1,1,1,0,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,0,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
```

## Ceiling Light
```
BAZZ 14" RGBCCT              {"NAME":"WF19129W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf 24W                {"NAME":"BW-LT20","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,1],"FLAG":0,"BASE":18}
BlitzWolf RGB Atmosphere and CCT  {"NAME":"BW-CLT1","GPIO":[0,0,0,0,419,0,0,0,418,420,417,416,0,0],"FLAG":0,"BASE":18,"CMND":"SO37 128"}
BrilliantSmart 12W 290mm Salisbury CCT  {"NAME":"Brilliant Smart Salisbury","GPIO":[0,0,0,0,0,0,0,0,449,0,416,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 24W 410mm Salisbury CCT  {"NAME":"Salisbury","GPIO":[0,0,0,0,0,0,0,0,449,0,416,0,0,0],"FLAG":0,"BASE":48}
BrilliantSmart Cordia 24W CCT  {"NAME":"Cordia","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Calex 429250                 {"NAME":"Calex_LED","GPIO":[0,0,0,0,0,0,0,0,0,449,416,0,0,0],"FLAG":0,"BASE":18}
Fcmila 48W RGBCCT            {"NAME":"XDD-48W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Globe Electric Brushed Nickel 11" Flush Mount  {"NAME":"Globe 60839","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Hama Glitter Effect, 27cm Square  {"NAME":"Hama LED Ceiling Light","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,1],"FLAG":0,"BASE":18}
HeyLight Plafoniera 30W CCT  {"NAME":"HeyLight Ceiling Light","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Hykolity 13" RGBCCT Flush Mount  {"NAME":"Hykolity","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lamptan 3 Lights in 1 36W    {"NAME":"Lamptan Smart Wi-Fi Ceiling Lamp LUMINA 36W","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":54}
LE lampUX 15W RGBCCT         {"NAME":"LE lampUX 15W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LightZone MeLiTec            {"NAME":"LightZone MeLiTec D114 Light ","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Lohas ZN026CL10 RGBCCT       {"NAME":"Lohas LED Lamp","GPIO":[0,0,0,0,417,416,0,0,419,418,420,0,0,0],"FLAG":0,"BASE":18}
LOLAsmart Uranus White 70 cm  {"NAME":"lola smart","GPIO":[0,0,0,1088,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LSC 20W 1400lm White Ambiance  {"NAME":"LSC RGBCW LED","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Luminea 24W CCT              {"NAME":"Luminea NX6205-944","GPIO":[0,0,0,0,0,0,0,0,449,0,416,0,0,0],"FLAG":0,"BASE":48}
LVL 300mm Round              {"NAME":"LVL 300m Round 24W Ceiling LED","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":48}
Nedis CCT 800lm              {"NAME":"NEDIS WIFILAW10WT","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis RGBCCT 1200lm          {"NAME":"Nedis RGB","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Oeegoo 15W RGBCCT            {"NAME":"Oeegoo RGBCCT","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Offdarks Star Fine RGBCCT 60W 40mm  {"NAME":"Offdarks","GPIO":[0,0,0,0,417,416,0,0,419,420,418,0,0,0],"FLAG":0,"BASE":18}
Polux UFO RGBCCT 48W         {"NAME":"Polux 313924","GPIO":[0,0,0,0,416,417,0,0,418,419,420,0,0,0],"FLAG":0,"BASE":18}
SMRTLite LED Panel           {"NAME":"SMRTLite","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Spectrum Smart Nymphea 36W CCT  {"NAME":"Nymphea_36w","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Taloya 12" 24W CCT Main and RGB Ambient  {"NAME":"Taloya GA300-24W-AI-WEMG","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Utorch 24W 2000lm CCT        {"NAME":"Utorch PZE-911","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":1}
Utorch 24W 2000lm CCT        {"NAME":"Utorch UT40","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":1}
Verve 24W Cream Fabric Shade CCT  {"NAME":"Verve ACL603HA","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,20 | TuyaMCU 21,22 | TuyaMCU 23,23 | DimmerRange 0,1000"}
Verve Design Angie 18W RGB Ring  {"NAME":"ACL12HA Light","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Verve Design Charlie 22W CCT  {"NAME":"Verve ACL01HA","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":48}
Verve Design Hana 24W CCT    {"NAME":"Verve ACL03HA","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":48}
Verve Design Hana 36W CCT    {"NAME":"Verve ACL04HA","GPIO":[0,0,0,0,0,0,0,0,449,0,416,0,0,0],"FLAG":0,"BASE":48}
Yeelight Jiaoyue 260mm 10W CCT  {"NAME":"Yeelight LED Ceiling Light 260","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,417,0,416,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
```

## Contact Sensor
```
D06                          {"NAME":"D06 Door Sensor","GPIO":[0,3200,0,3232,224,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Digoo DG-ZXD21 Door Detector  {"NAME":"Digoo ZXD21","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Earykong TYMC-1 Door Window  {"NAME":"TYMC-1","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
TY01 Door Window             {"NAME":"TY01","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Zemismart Door Window        {"NAME":"Zemismart","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
```

## Curtain Module
```
Athom                        {"NAME":"Athom Curtain Module","GPIO":[0,0,0,160,161,32,0,0,224,576,225,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO80 1 | ShutterMode 1 | SwitchDebounce 300 | ButtonDebounce 300"}
BlitzWolf                    {"NAME":"BlitzWolf SS6 2 Gang","GPIO":[0,0,32,0,480,0,0,0,161,160,224,225,0,0],"FLAG":0,"BASE":18}
LoraTap In-Wall              {"NAME":"SC500W","GPIO":[0,0,0,576,160,161,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
LoraTap In-Wall              {"NAME":"SC511WSC","GPIO":[0,1,0,320,32,34,0,0,224,33,226,225,0,0],"FLAG":0,"BASE":18}
MS-108 In-Wall               {"NAME":"MS-108","GPIO":[0,0,0,0,161,160,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
MS-108WR RF Curtain Module   {"NAME":"MS-108WR","GPIO":[1,1,1,544,32,33,1,1,225,32,224,1,1,1],"FLAG":0,"BASE":18}
Nous                         {"NAME":" Smart WiFi Curtain Module L12T","GPIO":[1,160,1,161,225,224,1,1,544,1,32,1,1,1],"FLAG":0,"BASE":18}
QS-WIFI-C01-RF               {"NAME":"Shutter-QS-WIFI-C01","GPIO":[0,0,1,0,288,0,0,0,32,33,224,225,0,0],"FLAG":0,"BASE":18}
wesmartify Smart Home Aktor Shut It TASMOTA  {"NAME":"ITzy","GPIO":[0,0,0,0,225,224,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Curtain Switch
```
Anccy                        {"NAME":"Anccy Shutter","GPIO":[544,0,289,34,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
Anccy Relax                  {"NAME":"Tuya Shutter","GPIO":[544,0,290,161,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Avatto Curtain + Light       {"NAME":"Avatto Curtain + Light","GPIO":[576,0,0,34,226,35,0,0,32,224,225,227,33,0],"FLAG":0,"BASE":18}
Avatto SYS-CS01              {"NAME":"Avatto","GPIO":[320,576,0,33,225,0,0,0,0,224,321,0,32,0],"FLAG":0,"BASE":18}
Currydouble                  {"NAME":"Currydouble","GPIO":[260,258,576,0,0,224,0,0,0,1,5760,225,259,0],"FLAG":0,"BASE":18}
Etersky WF-CS01              {"NAME":"Etersky","GPIO":[544,0,289,34,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
Homecube                     {"NAME":"Jinvoo Curtain","GPIO":[288,0,0,33,225,34,0,0,32,224,0,226,0,4704],"FLAG":0,"BASE":18}
Jinvoo SM-SW101-C            {"NAME":"Jinvoo Curtain","GPIO":[288,0,0,33,225,34,0,0,32,224,0,226,0,4704],"FLAG":0,"BASE":18}
KingArt                      {"NAME":"KING-Q4","GPIO":[544,0,0,162,0,32,0,0,33,225,290,224,0,0],"FLAG":0,"BASE":18}
Konesky                      {"NAME":"KingArt","GPIO":[544,0,289,162,226,32,0,0,33,225,290,224,288,0],"FLAG":0,"BASE":18}
LoraTap SC400W-EU            {"NAME":"Loratap SC400W","GPIO":[544,0,0,34,0,32,0,0,33,225,0,224,0,0],"FLAG":0,"BASE":18}
LoraTap SC411WSC-EU RF Remote  {"NAME":"Loratap","GPIO":[0,0,0,34,226,32,0,0,33,225,0,224,0,0],"FLAG":0,"BASE":18}
Maxcio WF-CS01               {"NAME":"Maxcio","GPIO":[544,0,289,162,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
Moes RF                      {"NAME":"Moes WS-EUR-CW","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
SCS86-03AJAI                 {"NAME":"ESE86-03AJAI","GPIO":[544,227,289,34,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
Teekar 3rd Generation        {"NAME":"Teekar-Tag","GPIO":[0,0,0,32,33,34,0,0,225,0,226,0,224,0],"FLAG":0,"BASE":18}
Teekar SYS-CS 01             {"NAME":"Teekar-Tag","GPIO":[320,0,544,33,225,162,0,0,0,224,321,258,32,0],"FLAG":0,"BASE":18}
Teepao                       {"NAME":"Teepao","GPIO":[576,322,226,33,225,34,0,0,320,224,321,0,32,0],"FLAG":0,"BASE":18}
WF-CS01                      {"NAME":"ShutterSwitch","GPIO":[544,227,289,34,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18,"CMND":"Rule1 1"}
WF-CS02                      {"NAME":"WF-CS02 Tuya","GPIO":[544,0,289,162,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
WuyunElek                    {"NAME":"MILOS Jalousie","GPIO":[1,0,224,225,1,320,0,0,32,33,34,226,0,0],"FLAG":0,"BASE":18}
Zemismart                    {"NAME":"Zemismart","GPIO":[544,0,289,162,290,161,0,0,160,224,226,225,288,0],"FLAG":0,"BASE":18}
Zemismart Backlit            {"NAME":"WF-CS01","GPIO":[544,227,289,34,226,161,0,0,160,224,290,225,288,0],"FLAG":0,"BASE":18}
```

## DIN Relay
```
CBI Astute Smart Controller  {"NAME":"CBI Astute","GPIO":[2624,320,0,0,0,224,0,0,2720,32,2656,0,0,0],"FLAG":0,"BASE":6}
CurrySmarter Power Monitoring 30A  {"NAME":"30A Breaker on Led","GPIO":[0,0,0,0,7584,224,0,0,2720,32,2656,2624,320,0],"FLAG":0,"BASE":18}
EARU DIN Circuit Breaker 1P 32A/50A  {"NAME":"RDCBC-1P","GPIO":[320,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
EARU DIN Circuit Breaker 1P 63A eWeLink version  {"NAME":"RDCBC-1P","GPIO":[32,0,0,0,0,0,0,0,288,224,0,0,0,0],"FLAG":0,"BASE":18}
EARU DIN Circuit Breaker 2P 63/80A eWeLink White  {"NAME":"RDCBC-2P","GPIO":[32,0,0,0,0,0,0,0,288,224,0,0,0,0],"FLAG":0,"BASE":18}
Hoch Circuit Breaker 1P      {"NAME":"HOCH ZJSB9","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Ketotek Single Phase Energy Monitor  {"NAME":"Ketotek KTEM06","GPIO":[0,2272,0,2304,0,0,0,0,0,0,320,0,32,0],"FLAG":0,"BASE":54}
Martin Jerry 30A Circuit Breaker  {"NAME":"30A Breaker","GPIO":[0,0,0,0,7584,224,0,0,2720,32,2656,2624,320,0],"FLAG":0,"BASE":18}
OpenEnergyMonitor WiFi MQTT Thermostat  {"NAME":"MQTT-RELAY","GPIO":[32,0,1,0,0,224,0,0,0,0,0,0,320,0],"FLAG":0,"BASE":18}
RocketController ASTRA Controller  {"NAME":"ASTRA R4A4","GPIO":[1,1,1,1,576,1,1,1,480,1,1,1,3232,3200,1,1,0,640,608,1,0,224,225,1152,0,0,0,0,227,226,160,161,162,0,0,163],"FLAG":0,"BASE":1}
Shelly Pro 1                 {"NAME":"Shelly Pro 1","GPIO":[0,1,0,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350"}
Shelly Pro 1PM               {"NAME":"Shelly Pro 1PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3459,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350"}
Shelly Pro 2                 {"NAME":"Shelly Pro 2","GPIO":[0,1,0,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350 | AdcParam2 2,10000,10000,3350"}
Shelly Pro 2PM               {"NAME":"Shelly Pro 2PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,9569,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3460,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350 | AdcParam2 2,10000,10000,3350"}
Sinotimer                    {"NAME":"TM608","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Sinotimer                    {"NAME":"Sinotimer TM60","GPIO":[0,0,0,0,0,288,0,0,224,160,0,0,0,0],"FLAG":0,"BASE":18}
SMTONOFF 63A                 {"NAME":"SMTONOFF","GPIO":[32,0,0,3104,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":43}
Sohan DIN Circuit Breaker 1P 50A  {"NAME":"RDCBC-1P","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Smart Stackable Power Meter  {"NAME":"Sonoff SPM","GPIO":[0,0,0,0,3200,5536,0,0,672,704,736,0,3232,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,544,0,0,32,0,0,0,0],"FLAG":0,"BASE":1}
TOMZN 1P 63A Circuit Breaker  {"NAME":"TOB8-63WiFi","GPIO":[32,0,0,3104,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":43}
TOMZN 2P 63A/80A Circuit Breaker  {"NAME":"TOWBC-2P-T","GPIO":[0,0,0,0,0,0,0,0,64,224,0,0,288,0],"FLAG":0,"BASE":18}
TOMZN 4P 80A Circuit Breaker  {"NAME":"TOWBC-4P-T","GPIO":[288,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
TOMZN 4P 80A Circuit Breaker  {"NAME":"TOMZN4","GPIO":[32,0,0,0,0,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
Tongou 2P 63A Circuit Breaker  {"NAME":"RCD_CONTACTOR","GPIO":[32,224,0,3104,0,0,0,0,288,544,0,0,0,0],"FLAG":0,"BASE":18}
```

## Dehumidifier
```
Duux Bora                    {"NAME":"Duux Bora","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 73,3 | TuyaMCU 74,4 | TuyaMCU 12,5 | TuyaMCU 13,7 | TuyaMCU 81,13 | TuyaMCU 14,101 | HumRes 0"}
electriQ 12L Portable        {"NAME":"electriQ CD12PW","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Vacplus 50 Pint              {"NAME":"VacPlus Dehumidifier","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
```

## Development Board
```
Adafruit ESP32 Feather V2    {"NAME":"HUZZAH32v2","GPIO":[1376,0,3840,0,1,1,0,0,1,1,1,1,0,0,0,1,608,1,640,0,0,1,1,1,0,1,1,0,1,1,1,4704,1,1,0,1],"FLAG":0,"BASE":1}
Adafruit HUZZAH ESP8266      {"NAME":"Huzzah","GPIO":[32,0,320,0,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":18}
Adafruit HUZZAH32 ESP32 Feather  {"NAME":"HUZZAH32","GPIO":[0,0,0,0,4709,0,1,1,1,288,1,1,1,1,0,1,0,0,608,640,0,4705,4704,1,0,0,0,0,1,1,4706,4710,4708,0,0,4707],"FLAG":0,"BASE":1}
Adafruit QT Py ESP32 Pico    {"NAME":"QTPy ESP32 Pico","GPIO":[32,3200,0,3232,1,1376,0,0,1,1,1,1,0,0,0,608,0,0,640,0,0,1,1,1,0,1,3840,0,1,1,0,0,0,0,0,0],"FLAG":0,"BASE":1}
AZ-Envy Environmental Sensor  {"NAME":"AZ Envy","GPIO":[32,0,320,0,640,608,0,0,0,0,0,0,0,4704],"FLAG":0,"BASE":18}
Coiaca Tasmota               {"NAME":"AWR01t","GPIO":[576,1,1,128,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Coiaca Tasmota Development Board AWR12  {"NAME":"AWR12t","GPIO":[320,1,1,1,1,1,0,0,1,1,1,1,1,1],"FLAG":0,"BASE":18}
Dasduino CONNECT             {"NAME":"Dasduino CONNECT","GPIO":[1,1,1376,1,640,608,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":18}
Dasduino CONNECTPLUS         {"NAME":"Dasduino CONNECTPLUS","GPIO":[32,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,640,608,1,0,1,1,1,0,0,0,0,1376,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Espoir Rev 1.0.0 PoE+        {"NAME":"Espoir","GPIO":[0,0,1,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,5568,5600,1,7968,1,1,1,1],"FLAG":0,"BASE":1}
Freenove ESP32-WROOM         {"NAME":"FREENOVE SP32 WROOM","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0],"FLAG":0,"BASE":1}
KinCony 128 Channel Controller  {"NAME":"KC868-A128","GPIO":[0,1,0,1,609,640,1,1,1,3232,3200,641,608,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,0,0,4705,4707,4706,0,0,4704],"FLAG":0,"BASE":1}
KinCony 64 Channel Controller  {"NAME":"KC868-A64","GPIO":[0,1,0,1,609,640,1,1,1,3232,3200,641,608,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,0,0,4705,4707,4706,0,0,4704],"FLAG":0,"BASE":1}
LC Technology MicroPython Maker  {"NAME":"LC-ESP-Python","GPIO":[1,1,544,1,1,1,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":18}
LilyGo 4G GPS                {"NAME":"LiLyGo A7608EH","GPIO":[1,1,672,1,226,225,0,0,224,6720,736,704,0,0,1,1,0,1,1,1,0,227,3200,3232,0,0,0,0,1,192,1,4864,1,0,0,1],"FLAG":0,"BASE":1}
LilyGO RGB LED Ring Encoder  {"NAME":"T-Encoder","GPIO":[0,0,1,0,1,0,0,0,1,1,1,1,0,0,0,480,6212,0,0,0,0,449,450,448,0,0,0,0,0,0,0,0,3296,3264,32,0],"FLAG":0,"BASE":1,"CMND":"BuzzerPwm 1"}
LilyGO T7 Mini32 V1.5        {"NAME":"LilyGO T7 V1.5","GPIO":[1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,544,0,0,0,1,0,1,1,1,0,0,0,0,0,1,1,4704,1,0,0,1],"FLAG":0,"BASE":1}
LilyGO TTGO ESP32 Ethernet   {"NAME":"T-Internet-POE v1.2","GPIO":[0,1,1,1,1,1,1,1,1,1,1,1,1,1,5600,1,0,1,1,5568,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1,"CMND":"EthType 0|EthClockMode 1|EthAddress 0"}
M5Stack Atom Lite ESP32      {"NAME":"M5Stack Atom Lite","GPIO":[0,0,0,0,0,0,0,0,1056,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1376,0,0,0,0,1,1,1,0,0,0,0,32],"FLAG":0,"BASE":1}
M5Stack AtomU USB-A ESP32    "Not available"
Mostly Robots Wemos D1 4 Channel MOSFET Shield  {"NAME":"MOSFET","GPIO":[1,1,288,1,1,1,1,1,226,225,227,224,1,1],"FLAG":0,"BASE":18}
Olimex ESP32-POE Ethernet    {"NAME":"Olimex ESP32-PoE","GPIO":[1,1,1,1,1,1,0,0,5536,1,1,1,1,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
QuinLED 2 Channel            {"NAME":"QuinLED 2 channel","GPIO":[416,0,417,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Silicognition wESP32         {"NAME":"wESP32","GPIO":[0,0,1,0,1,1,0,0,1,1,1,1,5568,5600,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
TZT ESP8266 Weather Station Kit  {"NAME":"TZT Weather Station","GPIO":[32,0,640,0,1,1184,0,0,1,1,608,1,1,1],"FLAG":0,"BASE":18}
WeAct Studio ESP32 Core      {"NAME":"ESP32CoreBoard","GPIO":[32,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,544,1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Wemos D1 Mini ESP32          {"NAME":"Wemos D1 Mini ESP32","GPIO":[0,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0],"FLAG":0,"BASE":1}
Wemos LOLIN32 Lite V1.0.0 (ESP32)  {"NAME":"Wemos LOLIN32 Lite V1.0.0","GPIO":[1,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0],"FLAG":0,"BASE":1}
Wireless Tag ESP32 Ethernet  {"NAME":"WT32_ETH01","GPIO":[1,1,1,1,1,1,0,0,1,0,1,1,3840,576,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1],"FLAG":0,"BASE":1}
Witty Cloud                  {"NAME":"Witty Cloud","GPIO":[1,1,320,1,32,1,0,0,417,418,1,416,1,4704],"FLAG":0,"BASE":32}
Yison ESP-01/ESP-202         {"NAME":"Yison Dev Board","GPIO":[259,544,258,1,260,261,1,1,416,418,257,417,256,1],"FLAG":0,"BASE":18}
```

## Dimmable
```
4lite WiZ A60 9W ES 806lm    {"NAME":"4lite WiZ LED A60 Smart Bulb Wi-Fi 9W ES 806lm","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Aisirer 9W 806lm             {"NAME":"Aisirer 9W","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Aisirer 9W 806lm             {"NAME":"AISIRER E26","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Aisirer 9W 806lm             {"NAME":"Aisirer 9W","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Arlec 4W 380lm Warm White Fancy Round Globe  {"NAME":"Arlec E14 Smart Bulb","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 4W 380lm Candle  {"NAME":"Arlec Bulb 4W","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 9W 950lm 4000K   {"NAME":"Arlec-GLD124HA","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Avatar 5W Candle             {"NAME":"AVATAR","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
AZzardo ES111 Black 3000K 15W  {"NAME":"Azzardo WiFi ES111 DIMM","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Bright A19 10W 800lm         {"NAME":"Bright Smart Bulbs","GPIO":[0,0,0,0,0,416,0,0,0,160,0,0,0,0],"FLAG":0,"BASE":66}
BrilliantSmart 8.5W 900lm 3000k  {"NAME":"Brilliant20889","GPIO":[0,0,0,0,0,0,0,0,416,0,0,0,0,0],"FLAG":0,"BASE":18}
Cleverio 51396 800lm         {"NAME":"Cleverio E27","GPIO":[0,0,0,0,0,0,0,0,0,0,448,0,0,0],"FLAG":0,"BASE":18}
Digma DiLight E27 W1         {"NAME":"DiLight E27 W1","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
DORESshop B11 600lm Filament  {"NAME":"Doresshop-cand","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
DORESshop ST64 720lm Filament  {"NAME":"DORESshop-ST64","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Feit Electric 800lm          {"NAME":" Feit P_A800_2","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Feit Electric 8.8W 800lm Vintage  {"NAME":"FEIT A1960CL","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Feit Electric BR30 650lm     {"NAME":"Feit BR30 WW","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Feit Electric BR30 650lm     {"NAME":"Feit BR30 CW","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Feit Electric Chandelier Flame Tip Soft White  {"NAME":"FEIT CFC40","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Feit Electric ST19 Soft White  {"NAME":"FEIT A1960FILAG","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Geeni LUX 800                {"NAME":"Geeni GN-BW902-999","GPIO":[0,0,0,0,416,0,0,0,0,160,0,0,0,0],"FLAG":0,"BASE":18}
Geeni LUX 800                {"NAME":"Geeni-LUX-800","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Geeni LUX Edison             {"NAME":"Geeni-Edison","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Globe 10W 800lm 3000K        {"NAME":"Globe 34324","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Globe A19 10W 800lm          {"NAME":"Globe WW 800lm","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Gosund NiteBird A19 8W 2700K  {"NAME":"NiteBird LB1","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Hama 7W 900lm Filament       {"NAME":"Hama Filament","GPIO":[1,1,1,1,1,1,1,1,1,1,448,1,1,1],"FLAG":0,"BASE":18}
Jetstream 9W 800lm           {"NAME":"Jetstream MA19WH","GPIO":[0,0,0,0,416,0,0,0,0,288,0,0,0,0],"FLAG":0,"BASE":18}
KMC 70113 A19 7.5W           {"NAME":"Generic","GPIO":[0,0,0,0,0,0,416,417,0,0,0,0,0,1],"FLAG":0,"BASE":18}
Kogan ST-20 Filament         {"NAME":"Kogan Filament","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,1],"FLAG":0,"BASE":18}
Krisbow Bohlam 14W           {"NAME":"Krisbow SmartL","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":48}
LeDesign 8W ST21 Filament    {"NAME":"Edison Bulb","GPIO":[0,0,0,0,0,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":18}
Lohas LZN127 G25 800lm       {"NAME":"Lohas Globe","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN014 550lm            {"NAME":"Lohas MR16","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN121 8W 720lm         {"NAME":"Lohas LED A60","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN124 980lm            {"NAME":"Lohas LH-ZN124","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
LSC Filament A60             {"NAME":"LSC Filam E27","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
LSC Filament C35             {"NAME":"LSC Filam E14","GPIO":[0,1,0,1,0,0,0,0,417,0,416,0,0,1],"FLAG":0,"BASE":18}
LSC Filament G125            {"NAME":"LSC Filam Huge","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
LSC Filament ST64 5.5W 470lm  {"NAME":"LSC Filam Big","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Luedd 7W 800lm Filament      {"NAME":"Luedd E27","GPIO":[0,0,0,0,0,0,0,0,0,0,448,0,0,0],"FLAG":0,"BASE":18}
Lumary 6W 700lm Edison       {"NAME":"Lumary TS3Y","GPIO":[0,0,0,0,0,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":18}
Luminea 5W 350lm 6500k       {"NAME":"ZX-2894","GPIO":[1,1,1,1,1,416,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":18}
Luminea ZX-2880 A60 800lm    {"NAME":"LAV-110.w","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Luminea ZX-2982 ST64 Filament  {"NAME":"Luminea ZX2982","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
MagicHome 7.5W Warm White    {"NAME":"ZJ-8C2B-CCT-AV1.1","GPIO":[0,0,0,0,0,0,0,0,416,0,0,0,0,0],"FLAG":0,"BASE":18}
Malmbergs A60 8,5W 810lm     {"NAME":"Malmbergs8,5W-W","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Merkury 9W 800lm             {"NAME":"MI-BW320-999W","GPIO":[0,0,0,0,0,0,0,0,0,0,224,0,0,0],"FLAG":0,"BASE":18}
Merkury BR30 750lm           {"NAME":"MI-BW905-999W","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
Merkury MI-BW902-999W 800lm  {"NAME":"MI-BW902-999W","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Merkury MI-BW942-999W 800lm  {"NAME":"MI-BW942-999W","GPIO":[0,0,0,0,416,0,0,0,0,288,0,0,0,0],"FLAG":0,"BASE":18}
Merkury MIC-BW902-999W       {"NAME":"MI-BW902-999W","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Merkury Vintage Edison A19   {"NAME":"Merkury A19 Ed","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 800lm        {"NAME":"GenioBulbCW","GPIO":[0,0,0,0,417,416,0,0,0,419,418,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"GenioB22","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"Mirabella Genio I002606","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm CW  {"NAME":"Genio Bulb CW Dim","GPIO":[0,0,0,0,0,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm WW  {"NAME":"Genio WW","GPIO":[0,0,0,0,0,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio A70 1400lm   {"NAME":"GenioB_CW2744","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 5W 500lm           {"NAME":"WIFILF11WTA60","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 Filament           {"NAME":"WIFILF10WTA60","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 W 800lm Warm White  {"NAME":"WIFILW31WTE27","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 Warm White 9W 800lm  {"NAME":"WIFILW11WTE27","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis G125 Filament          {"NAME":"WIFILF10GDG125","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Nedis PAR16 330lm            {"NAME":"Nedis WIFILW31","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Nedis ST64 5W 500lm Filament  {"NAME":"WIFILF10GDST64","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Positivo Smart Lampada Retro  {"NAME":"POSITIVO SMART LAMPADA RETRO","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Sealight Vintage Edison A19  {"NAME":"SealightEdison","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Shelly Vintage 4W 260lm 2700k  {"NAME":"Shelly Vintage","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Shelly Vintage 7W 750lm 2700k  {"NAME":"Shelly Vintage","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Shelly Vintage 7W 750lm 2700k  {"NAME":"Shelly Vintage","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
SmartDGM 9W 806lm            {"NAME":"L-WB9W1","GPIO":[0,0,0,0,0,416,0,0,160,0,0,0,0,0],"FLAG":0,"BASE":18}
Smitch 10W 6500K             {"NAME":"Smitch Ambience SB-0110","GPIO":[0,0,0,0,416,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Smitch 10W 6500K             {"NAME":"Smitch 10W 6500K Dimmable Bulb (SB0110 - E27)","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":1}
TCP Smart 806lm Warm White   {"NAME":"TCP Smart Clas","GPIO":[0,0,0,0,0,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":1}
TCP Smart 810lm Filament     {"NAME":"TCP Filament","GPIO":[0,0,0,0,0,0,0,0,0,0,448,0,0,0],"FLAG":0,"BASE":18}
TCP Smart 810lm Filament     {"NAME":"TCP Filament","GPIO":[0,0,0,0,0,0,0,0,0,0,448,0,0,0],"FLAG":0,"BASE":18}
WiZ A60 8W 2700K             {"NAME":"WiZ 9290024502","GPIO":[0,0,416,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Xiaomi Philips MUE4088RT     {"NAME":"Xiaomi Philips","GPIO":[0,0,0,0,0,0,0,0,0,0,0,416,0,0],"FLAG":0,"BASE":18}
```

## Dimmer
```
3A Smart Home HGZB-04D       {"NAME":"HGZB-4D","GPIO":[1,1,1,1,1,1,0,0,1,1,290,1,1,0],"FLAG":0,"BASE":54}
Acenx SD03                   {"NAME":"SD03","GPIO":[34,33,0,323,576,322,0,0,321,416,320,96,256,0],"FLAG":0,"BASE":73}
Athom EU                     {"NAME":"Athom Dimmer Switch","GPIO":[576,2272,0,2304,0,0,0,0,0,0,0,0,0,1],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 21,2|Ledtable 0|DimmerRange 10,1000"}
CE Smart Home                {"NAME":"CE-WF500D","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
CE Smart Home CFW500D-3W 3 Way  {"NAME":"CE-WF500D-3W","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
CNSKOU Touch                 {"NAME":"CNSKOU Dimmer Switch","GPIO":[0,0,0,0,0,0,0,0,0,0,290,0,0,0],"FLAG":0,"BASE":54}
Deta Single Gang Touch Dimmer  {"NAME":"Deta 6910HA","GPIO":[0,3200,1,3232,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Eva Logik                    {"NAME":"WF31 Dimmer","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Eva Logik 3 Way              {"NAME":"EL WF31T","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Feit Electric Smart          {"NAME":"Feit DIM/WIFI","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Globe 3 Way                  {"NAME":"Globe Dimmer","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Gosund                       {"NAME":"Gosund Dimmer","GPIO":[1,3200,1,3232,32,0,1,1,320,576,416,1,1,0],"FLAG":0,"BASE":18}
iLintek / Lumary             {"NAME":"L-DS100","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
iSwitch Touch Switch         {"NAME":"iSwitchOZ Dimmer","GPIO":[0,0,0,0,0,0,0,0,0,0,290,0,0,0],"FLAG":0,"BASE":54}
Martin Jerry SD01            {"NAME":"MJ-SD01 Dimmer","GPIO":[34,33,0,323,576,322,0,0,321,416,320,96,256,0],"FLAG":0,"BASE":73}
Martin Jerry Single Pole     {"NAME":"SD01 Dimmer","GPIO":[34,33,0,323,576,322,0,0,321,416,320,96,256,0],"FLAG":0,"BASE":73}
Martin Jerry Single Pole     {"NAME":"MJ-KN01 Dimmer","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Maxcio Rotary                {"NAME":"EDM-1WAA-EU","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Minoston 3-Way               {"NAME":"MS10W","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Moes                         {"NAME":"MOES DS01","GPIO":[1,1,1,1,1,1,0,0,1,2304,1,2272,1,0],"FLAG":0,"BASE":54}
Moes 3 Way Smart Light       {"NAME":"EDM-1WAA-US","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
PS-16-DZ                     {"NAME":"PS-16-DZ","GPIO":[1,3200,1,3232,1,1,0,0,1,288,1,1,1,0],"FLAG":0,"BASE":58}
Stitch                       {"NAME":"Stitch 35558","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 21,3|DimmerRange 21,255"}
Teekar UIW001-1              {"NAME":"Teekar UIW001-","GPIO":[0,3232,416,3200,640,608,0,0,160,0,0,0,0,0],"FLAG":0,"BASE":18}
Tessan 3 Way                 {"NAME":"Tessan-STD02","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"SO97 1 | TuyaMCU 11,1 | TuyaMCU 21,2 | DimmerRange 20,255"}
Tessan MJ-SD02               {"NAME":"MJ-SD02","GPIO":[34,33,0,323,576,322,0,0,321,416,320,96,256,0],"FLAG":0,"BASE":73}
TopGreener TGWF500D          {"NAME":"TopGreener-Dimmer","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
TreatLife 400W               {"NAME":"DS02S Dimmer","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
TreatLife 400W Single Pole   {"NAME":"DS02S Dimmer","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Treatlife Dual Outdoor       {"NAME":"DP12","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"SO97 1 | TuyaMCU 11,1 | TuyaMCU 12,7 | TuyaMCU 21,2 | TuyaMCU 22,8 | SO20 1 | SO54 1"}
TreatLife Light and Fan      {"NAME":"DS03 Fan/Light","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Virage Labs VirageDimmer     {"NAME":"VirageDimmer","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
WF-DS01                      {"NAME":"Dimmer WF-DS01","GPIO":[1,1,1,1,1,1,0,0,1,1,290,1,1,0],"FLAG":0,"BASE":54}
WiFi Dimmer Switch           {"NAME":"PS-16-DZ","GPIO":[0,3200,0,3232,0,0,0,0,0,288,0,0,0,0],"FLAG":0,"BASE":58}
Zemismart KS-7011            {"NAME":"KS-7011 Dimmer","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
```

## Dimmer Module
```
Armtronix AC Dimmer 1-Triac  {"NAME":"ARMTR Dimmer","GPIO":[0,3200,0,3232,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":56}
Armtronix AC Dimmer 2-Triac  {"NAME":"ARMTR Dimmer","GPIO":[0,3200,0,3232,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":56}
BrilliantSmart D350W         {"NAME":"D350W","GPIO":[1,2272,1,2304,1,1,0,0,1,0,1,0,1,0],"FLAG":0,"BASE":54}
BrilliantSmart Jupiter       {"NAME":"BrSm Jupiter","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
EX-Store 2 Kanal RS232 V4    {"NAME":"EXS Dimmer","GPIO":[0,3200,0,3232,0,0,0,0,0,4128,0,0,0,0],"FLAG":0,"BASE":72}
Moes 2 Gang                  {"NAME":"WM-105","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 21,2 | TuyaMCU 12,7 | TuyaMCU 22,8 | DimmerRange 0,1003"}
Moes MS-105-1 v2             {"NAME":"MS-105","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
QS-WiFi-D01-TRIAC 150W       {"NAME":"QS-WiFi-D01-TRIAC","GPIO":[0,3200,0,3232,0,0,0,0,0,352,416,0,0,0],"FLAG":0,"BASE":18}
RJWF-02A                     {"NAME":"RJWF-02A","GPIO":[32,2272,0,2304,0,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":54}
Shelly Dimmer                {"NAME":"Shelly Dimmer 1","GPIO":[0,3200,0,3232,5568,5600,0,0,192,0,193,288,0,4736],"FLAG":0,"BASE":18}
Shelly Dimmer 2              {"NAME":"Shelly Dimmer 2","GPIO":[0,3200,0,3232,5568,5600,0,0,193,0,192,0,320,4736],"FLAG":0,"BASE":18}
Shelly Plus 0-10V Dimmer     {"NAME":"Shelly Plus 0-10V Dimmer","GPIO":[288,0,0,0,192,0,0,0,0,0,0,0,0,0,193,1,0,0,0,0,0,32,416,0,0,0,0,0,4736,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff D1                    {"NAME":"Sonoff D1","GPIO":[1,3200,0,3232,0,0,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":74}
```

## Display
```
Espressif ESP-WROVER-KIT     {"NAME":"ESPWroverKit41","GPIO":[1,1,1,1,1,992,1,1,1,1,1,1,1,1,1024,736,0,800,768,704,0,672,6210,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Heltec WiFi Kit 32           {"NAME":"WiFi Kit 32","GPIO":[1,1,1,1,640,1,1,1,1,1,1,608,3840,1,1,1,0,1,1,1,0,224,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Heltec WiFi Kit 8            {"NAME":"WifiKit8","GPIO":[1,1,1,1,640,608,1,1,1,1,1,1,1024,1],"FLAG":0,"BASE":18}
LilyGO T5 4.7 inch E-paper   {"NAME":"LilyGO T5-4.7","GPIO":[0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,7616,0,0,0,0,0,0,0,0,0,0,33,34,4704,0,0,32],"FLAG":0,"BASE":1}
LilyGo TTGO ESP32            {"NAME":"TTGO T-Journal ESP32 Camera","GPIO":[4928,1,1,1,1,5091,1,1,1,608,640,4960,1,5088,5093,5095,0,5184,5120,5056,0,5024,5152,4992,0,0,0,0,1,1,5090,5089,5094,0,0,5092],"FLAG":0,"BASE":1}
LilyGO TTGO ESP8266 0.91 inch OLED SSD1306  {"NAME":"TTGO-0.91","GPIO":[1,1,640,1,1024,0,0,0,1,1,608,1,0,1],"FLAG":0,"BASE":18}
LilyGO TTGO T-Camera         {"NAME":"T-Camera","GPIO":[1,1,1,1,4992,5120,1,1,1,5089,5090,64,1,1,5024,65,0,640,608,5056,0,5184,1,5152,0,0,0,0,1,1,5088,5091,5095,5094,5093,5092],"FLAG":0,"BASE":1}
LilyGO TTGO T-Display 1.14in  {"NAME":"ESP32-ttgo-display","GPIO":[0,1,1,1,992,6592,1,1,1,1,1,1,6624,1,896,864,0,1,1,1024,0,1,1,1,0,0,0,0,1,1,1,33,1,0,0,1],"FLAG":0,"BASE":1}
LilyGO TTGO T-Watcher        {"NAME":"TTGO T4 v1.3","GPIO":[35,1,672,1,992,1024,1,1,832,768,736,704,1,1,896,0,0,640,608,864,0,0,1,6368,0,0,0,0,6400,1,1,4704,1,32,33,34],"FLAG":0,"BASE":1}
LilyGO TTGO TO ESP8266 OLED SH1106 Weather Station  {"NAME":"TTGO T12","GPIO":[1,1,1,1,608,640,0,0,32,34,33,1,1,1],"FLAG":0,"BASE":18}
Lolin TFT 2.4 Touch Shield   {"NAME":"Lolin TFT 2.4 Touch Shield","GPIO":[224,225,226,1,1,6400,1,1,1,1,1,1,1,7264,736,672,0,1,992,704,0,1,6368,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
M5Stack Core2                {"NAME":"M5Stack Core 2","GPIO":[6210,1,1,1,6720,768,0,0,0,1,1,800,0,0,736,1,0,641,609,704,0,0,0,1,0,0,0,0,640,608,1,1,1,0,672,0],"FLAG":0,"BASE":1}
M5Stack M5Stick              {"NAME":"M5Stick","GPIO":[0,0,0,0,0,0,0,0,0,608,768,0,0,1056,736,544,0,641,609,704,0,640,480,800,6210,0,0,0,0,1024,0,32,0,0,0,0],"FLAG":0,"BASE":1}
M5Stack M5StickC             {"NAME":"M5StickC","GPIO":[1,0,0,0,0,768,1056,576,0,736,0,704,0,0,1024,0,0,641,609,800,0,0,1,0,0,0,0,0,640,608,1,0,1,32,6210,33],"FLAG":0,"BASE":1}
M5Stack Tough                {"NAME":"M5Stack Tough","GPIO":[6210,1,1,1,6720,768,0,0,0,1,1,800,0,0,736,1,0,641,609,704,0,0,0,1,0,0,0,0,640,608,1,1,1,0,672,7968],"FLAG":0,"BASE":1}
ODROID-GO Game Kit           {"NAME":"ODROID-GO","GPIO":[32,3200,544,3232,608,768,0,0,6210,33,416,640,1,1,736,672,0,800,6720,704,0,0,0,34,0,0,0,0,164,165,3328,3329,4866,0,0,35],"FLAG":0,"BASE":3}
OLED Display Module 0.66" for Wemos D1 Mini  {"NAME":"OLED 64x48","GPIO":[1,1,1,1,640,608,0,0,1,1,1,1,1024,1],"FLAG":0,"BASE":18}
Sunton 2.8" 240*320 CYD      {"NAME":"ESP32-2432S028","GPIO":[6210,1,800,0,448,0,1,1,672,704,736,768,449,450,1,1,0,992,1,1,0,737,480,1,0,0,0,0,705,7264,4768,1,0,0,0,673],"FLAG":0,"BASE":1}
Wireless Tag 3.5" Touch      {"NAME":"WT32-SC01","GPIO":[6210,1,1,1,1,1,0,0,1,704,736,768,1,1,640,608,1,800,1024,992,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
```

## Display Switch
```
Lanbon L8 5 in 1 LCD Touch   {"NAME":"Lanbon L8","GPIO":[608,0,0,0,640,992,0,0,224,0,225,0,0,0,1024,736,0,800,768,704,6210,672,416,226,0,0,0,0,417,418,0,2688,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff NSPanel Touch         {"NAME":"NSPanel","GPIO":[0,0,0,0,3872,0,0,0,0,0,32,0,0,0,0,225,0,480,224,1,0,0,0,33,0,0,0,0,0,0,0,0,0,0,4736,0],"FLAG":0,"BASE":1,"CMND":"ADCParam1 2,11200,10000,3950 | Sleep 0 | BuzzerPWM 1"}
```

## Downlight
```
3A 12W RGBW                  {"NAME":"RGBWSpot","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 9W CCT           {"NAME":"Arlec ALD092CHA","GPIO":[0,0,0,0,416,417,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 9W RGB+CCT       {"NAME":"Arlec ALD092RHA","GPIO":[0,0,0,0,419,420,0,0,416,418,417,0,0,0],"FLAG":0,"BASE":18}
Atom 10W RGBCCT              {"NAME":"Atom AT9017/WH/WIZ/TR","GPIO":[0,0,420,0,419,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,416,417,418,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Atom 11W RGBCCT              {"NAME":"Atom AT9012/WH/WIZ","GPIO":[0,0,420,0,419,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,416,417,418,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
BAZZ 4" Recessed             {"NAME":"WFDISK400W","GPIO":[0,0,0,0,420,419,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Bazz 6" RGBCCT Recessed Lighting Conversion Kit  {"NAME":"CON6RGBTNWWF","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart Prism 10W RGBCCT  {"NAME":"Prism","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart Trilogy 9W CCT  {"NAME":"SmartCCTDwnLgt","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":48}
Connect SmartHome RGB        {"NAME":"CSH-240RGB10W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Deta 10W RGBCCT              {"NAME":"Deta DownLight","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Deta 7W 660lm LED            {"NAME":"Deta DownLight","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Feit Electric 6in. RGBW Recessed  {"NAME":"Feit LEDR6/RGB","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":48}
FK-DL101 RGBW                {"NAME":"FK-DL101 RGBW","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Globe 4" 9W                  {"NAME":"Globe RGBCCT Downlight","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Globe 5W 4" Recessed RGBCCT  {"NAME":"GlobeRGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Hyperikon 14W 1000lm 6"      {"NAME":"HyperikonDL6","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
iHomma 6W RGBCCT             {"NAME":"iHomma RGBWW","GPIO":[0,0,0,0,420,419,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
iHomma RGB BT+IR 12W         {"NAME":"iHommaLEDDownl","GPIO":[0,0,0,0,0,419,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Illume 6-inch RGBCCT         {"NAME":"I-SMRGBCCT6","GPIO":[0,0,0,0,407,406,0,0,409,420,408,0,0,0],"FLAG":0,"BASE":18}
Kogan 9W RGBCCT              {"NAME":"Kogan_SMARTLED","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
LEDLite CCT 10W Fire Rated   {"NAME":"LEDLite-LTTD10Wifi","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W CCT       {"NAME":"GenioDLightCCT","GPIO":[0,0,0,0,0,0,0,0,449,0,416,0,0,0],"FLAG":0,"BASE":48}
Mirabella Genio 9W Dimmable  {"NAME":"Genio 1002742 (2022)","GPIO":[0,0,0,0,0,0,0,0,417,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W RGBCCT    {"NAME":"GenioDLightCCT","GPIO":[0,0,0,0,0,0,0,0,4032,0,4064,0,0,0],"FLAG":0,"BASE":18}
Moes 7W RGBCCT               {"NAME":"Moes Downlight","GPIO":[0,0,0,0,419,420,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Philips 5/6 in. RGBCCT       {"NAME":"Philips","GPIO":[0,0,0,0,419,0,0,0,416,420,418,417,0,0],"FLAG":0,"BASE":48}
Seed Alarm 11W RGBW          {"NAME":"Seed Alarm Downlight","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Xiaomi Philips 200lm 3000k-5700k CCT  {"NAME":"xiaomi.philips.downlight","GPIO":[0,0,0,0,0,0,0,0,416,0,0,417,0,0],"FLAG":0,"BASE":18}
Zemismart 4" 10W RGBCCT      {"NAME":"ZemiDownLight4","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Zemismart 4" 10W RGBW        {"NAME":"ZemiDownLight","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Zemismart 6" 14W RGBCCT      {"NAME":"ZemiDownLight6","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
```

## Energy Meter
```
DIY ESP-01 and PZEM-004T     {"NAME":"HW-655 PZEM","GPIO":[0,1504,0,1472,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Hiking Single Phase 65A DIN Rail  {"NAME":"Hiking DDS238-2 WIFI","GPIO":[0,2272,0,2304,0,0,0,0,0,0,320,0,32,0],"FLAG":0,"BASE":54}
ZMAi-90 Digital              {"NAME":"ZMAi-90","GPIO":[0,3200,0,3232,0,0,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
```

## Environment Sensor
```
IOT4                         {"NAME":"IOT4SH01BME680","GPIO":[0,0,0,0,0,0,0,0,640,0,608,0,0,0],"FLAG":0,"BASE":18}
Sonoff SC                    {"NAME":"Sonoff SC","GPIO":[32,3200,1,3232,0,0,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":21}
```

## Fan
```
Anko HEGSM40                 {"NAME":"Anko HEGSM40","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Arlec 117cm Aurora Digital Tower  {"NAME":"ATF4601HA","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Arlec 130cm White 4 Blade DC Ceiling Light and  {"NAME":"Arlec DCF4002WHA","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Arlec 45cm DC Pedestal       {"NAME":"Arlec 45cm Fan","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 61,2 | TuyaMCU 64,3 | TuyaMCU 99,6 | TuyaMCU 12,101"}
Arlec Smart 45cm Smart DC Wall  {"NAME":"Arlec 45cm Fan","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 61,2 | TuyaMCU 64,3 | TuyaMCU 99,6 | TuyaMCU 12,101"}
Duux Whisper Flex Battery Pack  {"NAME":"Duux DXCF12","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 61,2 | TuyaMCU 62,3 | TuyaMCU 12,4 | TuyaMCU 13,5 | TuyaMCU 64,6"}
Duux Whisper Flex Smart      {"NAME":"Duux Fan","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Geek Aire 3D Oscillating Desktop  {"NAME":"Geek Aire Fan","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 12,4 | TuyaMCU 13,5"}
Goldair SleepSmart GCPF315   {"NAME":"Goldair Fan","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Holmes 36" Oscillating Tower  {"NAME":"Generic","GPIO":[1,1,1,1,1,1,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":54,"CMND":"TuyaMcu 11,1 | TuyaMcu 12,5 | WebButton1 Power | WebButton2 Oscillation "}
Lucci Connect Remote Control  {"NAME":"Lucci Fan","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
QuietCool Gable Mount Attic  {"NAME":"QuietCool-AFG-SMT-PRO-2.0","GPIO":[0,0,0,0,0,224,0,0,0,0,0,0,0,0,640,608,0,0,0,225,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Interlock 1|WebButton1 Low|WebButton2 High|SO8 1"}
Sichler Haushaltsgeraete Column  {"NAME":"Sichler Fan","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Technical Pro                {"NAME":"FXA16 Fan","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 12,8"}
Zemismart Bladeless          {"NAME":"Bladeless Fan","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
```

## Fan Controller
```
BrilliantSmart Light and     {"NAME":"Brilliant Fan","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Qiachip 110V/220V Light and  {"NAME":"Qiachip FLC","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 12,9 | TuyaMCU 81,7"}
Sonoff iFan02 Light and Ceiling  {"NAME":"Sonoff iFan02","GPIO":[32,1,0,1,226,225,33,34,224,320,35,227,0,0],"FLAG":0,"BASE":44}
Sonoff iFan03 220V Light and Ceiling  {"NAME":"SonoffiFan03","GPIO":[32,3200,0,3232,0,0,256,512,226,320,225,227,0,0],"FLAG":0,"BASE":71}
Sonoff iFan04-H 220V Light and Ceiling  {"NAME":"iFan04-H","GPIO":[32,3200,0,3232,0,0,256,512,226,320,225,227,0,0],"FLAG":0,"BASE":71}
Sonoff iFan04-L 110V Light and Ceiling  {"NAME":"iFan04-L","GPIO":[32,3200,0,3232,0,0,256,512,226,320,225,227,0,0],"FLAG":0,"BASE":71}
```

## Fan Switch
```
Avatto Light and             {"NAME":"AVATTO SYS-FL01","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Deta Light and               {"NAME":"Deta Fan Speed and Light Controller","GPIO":[33,0,0,576,226,34,0,0,0,225,224,227,32,0],"FLAG":0,"BASE":18}
iSwitch Light and            {"NAME":"iSwitchOZ Light Fan","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Martin Jerry Fan Controller  {"NAME":"US-FC-01","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Treatlife 1.5A 4 Speed Ceiling  {"NAME":"DS02F","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1|SO97 1|SO68 0"}
```

## Garage Door Opener
```
MS-102                       {"NAME":"Garage Opener","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Wofea                        {"NAME":"Wofea Garage Door","GPIO":[1,1,1,1,1,1,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":54}
```

## Gas Sensor
```
Digoo Alarm with Heat and    {"NAME":"DG-ZXGS21","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Natural Gas (CH4) Alarm      {"NAME":"PA-210WYS","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
```

## Heater
```
Devola Designer Glass Panel  {"NAME":"Devola Heater","GPIO":[0,0,0,0,224,1,0,0,0,1824,0,1792,0,0],"FLAG":0,"BASE":18}
EBERG ROT 720 Infrared       {"NAME":"Tuya MCU","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,4 | TuyaMCU 72,2 | TuyaMCU 71,3"}
Klarstein Bornholm Smart 2000W  {"NAME":"Klarstein Bornholm","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Kogan 1500W Panel            {"NAME":"Kogan Panel Heater","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
LSC Smart Connect 500W IR Panel  {"NAME":"LSC Connect Smart IR Panel Heater","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
SmartMi Electric Air         {"NAME":"ZNNFJ07ZM","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,0,3200,3232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
```

## Humidifier
```
Duux Beam Ultrasonic         {"NAME":"Duux Beam","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 12,5 | TuyaMCU 13,21 | TuyaMCU 71,14 | TuyaMCU 74,15 | TuyaMCU 73,16"}
Proscenic                    {"NAME":"Generic","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
```

## IR Controller
```
3DStar ESP IR Blaster 8L     {"NAME":"3DS_IRblaster_xL","GPIO":[0,0,0,0,0,0,0,0,0,0,0,1056,0,0],"FLAG":0,"BASE":18}
3DStar ESP IR Blaster xLR    {"NAME":"3DS_IR Blaster_xLR","GPIO":[0,0,0,0,0,0,0,0,0,1088,0,1056,0,0],"FLAG":0,"BASE":18}
A1 Universal Remote Control  {"NAME":"A1 IR Controller","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
AI Universal Remote          {"NAME":"YTF IR Controller","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
AI Universal Remote Control  {"NAME":"LQ-08","GPIO":[0,0,0,0,0,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Alfawise KS1                 {"NAME":"KS1","GPIO":[1,1792,32,1824,32,1088,0,0,320,0,1056,0,0,4704],"FLAG":0,"BASE":62}
Antsig Universal Remote Controller  {"NAME":"Antsig Smart Wi-Fi IR","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Athom                        {"NAME":"Athom_IR_Remote","GPIO":[32,0,0,0,1056,1088,0,0,0,576,0,0,0,0],"FLAG":0,"BASE":18}
Automate Things IR Bridge    {"NAME":"AT-IRBR-1.0","GPIO":[0,0,0,0,1056,1088,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"Module 0"}
Automate Things IR Bridge    {"NAME":"AT-IRBR-1.4","GPIO":[1088,0,0,0,1056,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"Module 0"}
auvisio S06                  {"NAME":"NX-4519-675","GPIO":[0,0,0,0,288,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":18}
Avatto Temperature and Humidity Sensor and  {"NAME":"S08Pro","GPIO":[0,640,0,608,288,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-RC1             {"NAME":"BW-RC1","GPIO":[0,0,0,0,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Connect SmartHome Universal Remote  {"NAME":"CSH IR Controller","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Cusam CS-IRC-1               {"NAME":"YTF IR Controller","GPIO":[1,1,1,1,288,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Eachen IR DC6                {"NAME":"Eachen IR","GPIO":[0,0,0,0,320,1088,0,0,1,32,1056,0,0,0],"FLAG":0,"BASE":18}
ESP_IR_TR                    {"NAME":"Tasmota IR-Gateway","GPIO":[0,0,0,0,1056,0,0,0,0,0,1088,0,0,0],"FLAG":0,"BASE":18}
Frankever Smart Remote       {"NAME":"FK-UFO-R4","GPIO":[1,1,1,1,288,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Geeklink GK01                {"NAME":"GL IR Blaster","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
HIPER IoT IR2                {"NAME":"hiper_HI-IR2","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Jinvoo AC/TV Box             {"NAME":"Jinvoo IR Controller","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
JS-IR1 Universal Remote      {"NAME":"JS-IR1","GPIO":[1,1,1,1,288,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":18}
KinCony ESP32 433MHz RF and  {"NAME":"KC868-AG","GPIO":[0,0,1056,0,480,0,0,0,0,1152,1,0,1,1,1,1,0,1,1120,1088,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1,"CMND":"BuzzerPwm 1"}
Lenovo SE-741C               {"NAME":"Lenovo IR","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Mirabella Genio I002577      {"NAME":"Genio IR TxRx","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Moes Tuya Remote Control     {"NAME":"Moes IR Controller","GPIO":[1,1,1,1,1,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Nedis Universal Remote Control  {"NAME":"Nedis IR Controller","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
NEO Coolcam Remote Controller  {"NAME":"Neo Coolcam IR","GPIO":[1,3200,1,3232,576,1088,0,0,320,32,1056,0,0,0],"FLAG":0,"BASE":62}
Nivian Smart Infrared Controller  {"NAME":"Nivian NVS-SMARTIR-W2","GPIO":[0,3200,0,3232,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Orvibo Magic Cube            {"NAME":"Orvibo CT10W","GPIO":[0,0,0,0,32,1088,0,0,288,0,1056,289,0,0],"FLAG":0,"BASE":18}
Phlipton Universal Remote    {"NAME":"Phliptron IR","GPIO":[1,3200,1,3232,576,1088,0,0,320,32,1056,0,0,0],"FLAG":0,"BASE":62}
Remote Control               {"NAME":"DC-QRA2","GPIO":[0,0,0,0,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
RM mini                      {"NAME":"RM mini","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Smart Remote                 {"NAME":"IRC","GPIO":[0,0,544,0,0,288,0,0,1088,32,1056,0,0,0],"FLAG":0,"BASE":18}
Smartpoint Smart Remote      {"NAME":"Smartpoint SPCNTRL-WM","GPIO":[0,0,0,0,288,1088,0,0,0,0,1056,0,0,0],"FLAG":0,"BASE":18}
STITCH                       {"NAME":"Stitch 35753","GPIO":[0,0,0,0,288,1088,0,0,0,64,1056,0,0,0],"FLAG":0,"BASE":62}
SZMDLX IR Remote Controller  {"NAME":"SZMDLX WiFi IR","GPIO":[0,0,0,0,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Tellur Remote Control        {"NAME":"Tellur TLL331241","GPIO":[0,0,0,0,1056,1088,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Tuya Smart Remote            {"NAME":"IR Controller","GPIO":[1,1,1,1,1,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
UFO R1 Universal Remote      {"NAME":"UFO-R1","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
Uniplay Universal Remote     {"NAME":"Uniplay IR","GPIO":[1,3200,1,3232,576,1088,0,0,320,32,1056,0,0,0],"FLAG":0,"BASE":62}
YTF Universal Remote         {"NAME":"YTF IR Controller","GPIO":[1,1,1,1,320,1088,0,0,0,32,1056,0,0,0],"FLAG":0,"BASE":62}
```

## IR Remote
```
```

## Illuminance Sensor
```
Tuya                         {"NAME":"Lightsensor","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Kettle
```
Anko 1.7L Cordless Water     {"NAME":"Anko Kettle","GPIO":[1,1,1,1,1,1,0,0,1,2304,1,2272,1,0],"FLAG":0,"BASE":54}
Kogan 1.7L Smart Glass       {"NAME":"Kogan Kettle","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
ProfiCook Glass 1.5L         {"NAME":"ProfiCook PC-WKS 1167 G","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## LED Controller
```
Anncoe C350 RGB              {"NAME":"TUYA LED","GPIO":[0,0,0,0,0,417,0,0,418,32,416,0,0,0],"FLAG":0,"BASE":18}
Anncoe C350 RGBW             {"NAME":"TUYA LED","GPIO":[0,0,0,0,0,417,0,0,418,32,416,419,0,0],"FLAG":0,"BASE":18}
Arilux AL-LC01               {"NAME":"Arilux LC01","GPIO":[32,0,323,0,3136,416,0,0,417,418,0,0,0,0],"FLAG":0,"BASE":37}
Arilux AL-LC06               {"NAME":"Arilux LC06","GPIO":[32,0,0,0,0,0,0,0,417,418,416,420,419,0],"FLAG":0,"BASE":18}
Arilux AL-LC11               {"NAME":"Arilux LC11","GPIO":[32,0,323,0,417,416,0,0,420,419,418,3136,0,0],"FLAG":0,"BASE":38}
Arilux LC-08 RGBCCT          {"NAME":"Arilux LC08","GPIO":[0,0,161,0,417,416,0,0,420,419,418,0,0,0],"FLAG":0,"BASE":18}
Arilux SL-LC 03              {"NAME":"Arilux LC03","GPIO":[0,0,0,0,1088,417,0,0,416,418,0,419,0,0],"FLAG":0,"BASE":34}
Arilux SL-LC 09              {"NAME":"Arilux LC09","GPIO":[0,0,0,0,1152,416,0,0,418,0,417,0,0,0],"FLAG":0,"BASE":18}
Athom 5-24V 16A RGBW         {"NAME":"LS_5PIN_TAS","GPIO":[32,0,576,0,416,0,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Athom RGB                    {"NAME":"LS5050C-TAS","GPIO":[32,0,0,0,417,0,0,0,416,0,418,0,0,0],"FLAG":0,"BASE":18}
Bakeey RGB                   {"NAME":"Bakeey Strip Controller","GPIO":[32,0,0,0,416,420,0,0,418,0,417,419,0,0],"FLAG":0,"BASE":18}
CCT 12-24V                   {"NAME":"WS02","GPIO":[0,0,0,0,0,0,0,0,0,417,416,0,0,0],"FLAG":0,"BASE":18}
CIN-03 96W RGB               {"NAME":"CIN03-03 Strip","GPIO":[0,0,0,0,417,0,0,0,416,0,418,0,0,0],"FLAG":0,"BASE":18}
CIN04 240W                   {"NAME":"CL-CIN04-03","GPIO":[32,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
DD001-MINI(G)-IR-V03         {"NAME":"WIFI-RGB","GPIO":[32,0,0,0,0,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":40}
DD001-MINI(G)-IR-V08         {"NAME":"WIFI-RGB","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Electrodragon ESP LED Strip Board, Mosfet Drive  {"NAME":"LEDBoard RGBW","GPIO":[0,0,0,0,0,0,0,0,418,417,419,416,288,0],"FLAG":0,"BASE":18}
H801                         {"NAME":"H801","GPIO":[1,288,1,1,420,321,0,0,418,417,419,416,0,0],"FLAG":0,"BASE":20}
Hama Adapter and RGB         {"NAME":"HAMA LED-Strip WLAN-Controller","GPIO":[0,0,0,0,417,419,0,0,0,416,418,0,0,0],"FLAG":0,"BASE":18}
Holman Garden Light RGB      {"NAME":"Holman RGB","GPIO":[0,0,0,0,0,0,0,0,417,416,418,0,0,0],"FLAG":0,"BASE":18}
Jinvoo SM-WA104 RGB          {"NAME":"Jinvoo LED Controller","GPIO":[0,0,0,0,256,418,0,0,416,32,417,0,257,0],"FLAG":0,"BASE":18}
Konesky 12V RGB              {"NAME":"RGBwifi","GPIO":[0,0,0,0,416,0,0,0,417,320,418,0,0,0],"FLAG":0,"BASE":18}
LEDEnet                      {"NAME":"LEDEnet","GPIO":[0,1,320,1,3136,420,0,0,417,418,416,419,0,0],"FLAG":0,"BASE":34}
Luminea CCT                  {"NAME":"Luminea ZX-2842","GPIO":[416,0,0,0,0,0,0,0,417,32,0,0,0,0],"FLAG":0,"BASE":18}
Luminea RGBW                 {"NAME":"Luminea ZX-284","GPIO":[419,0,0,0,0,418,0,0,417,32,416,0,0,0],"FLAG":0,"BASE":18}
Luminea ZX-2844-675          {"NAME":"ZX-2844-675","GPIO":[32,0,0,0,417,419,0,0,416,0,418,0,0,0],"FLAG":0,"BASE":18}
Lustreon                     {"NAME":"Lustreon WiFi ","GPIO":[32,0,291,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":38}
Magic UFO RGBW               {"NAME":"Magic UFO","GPIO":[32,0,544,0,0,416,0,0,418,419,417,320,0,0],"FLAG":0,"BASE":18}
MagicHome DC5-28V Single Color  {"NAME":"MagicHome RF","GPIO":[0,0,3168,0,0,0,0,0,416,417,0,0,0,0],"FLAG":0,"BASE":38}
MagicHome RGB                {"NAME":"MagicHome RGB IR","GPIO":[0,0,0,0,1088,416,0,0,417,418,0,0,0,0],"FLAG":0,"BASE":18}
MagicHome RGB                {"NAME":"Magic Home RGB","GPIO":[0,0,0,0,3136,416,0,0,418,0,417,0,3168,0],"FLAG":0,"BASE":18}
MagicHome RGB ZJ-WFMN-A V1.1  {"NAME":"MagicHome RGB","GPIO":[0,0,0,0,0,416,0,0,417,418,0,0,0,0],"FLAG":0,"BASE":34}
MagicHome RGBW               {"NAME":"ESP-IR-B-v2.3","GPIO":[0,0,1088,0,0,417,0,0,416,418,0,419,0,0],"FLAG":0,"BASE":18}
MagicHome RGBW RF            {"NAME":"MagicHome RGBW RF","GPIO":[0,0,320,0,3136,417,0,0,418,419,416,3168,0,0],"FLAG":0,"BASE":18}
MagicHome RGBW ZJ-WFMN-B V1.1  {"NAME":"MagicHome RGBW RF (ZJ-WFMN-B V1.1)","GPIO":[0,0,0,0,3136,417,0,0,418,419,416,0,3168,0],"FLAG":0,"BASE":34}
MagicHome RGBWW w/ RF        {"NAME":"MagicHome RF","GPIO":[0,0,3168,0,416,417,0,0,420,419,418,3136,0,0],"FLAG":0,"BASE":38}
MagicHome RGBWW w/ RF        {"NAME":"MagicHome RF","GPIO":[0,0,0,0,3136,419,0,0,417,418,416,420,3168,0],"FLAG":0,"BASE":18}
MagicHome Single Color 5-28V  {"NAME":"MagicHome","GPIO":[0,0,0,0,0,0,0,0,416,0,0,0,0,0],"FLAG":0,"BASE":18}
MagicHome ZJ-ESP-IR-F V1     {"NAME":"ZJ-ESP-IR-F V1","GPIO":[0,0,0,0,1088,417,0,0,416,418,0,419,0,0],"FLAG":0,"BASE":18}
MiBoxer Dual White           {"NAME":"FUT035W","GPIO":[0,2304,0,2272,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"DimmerRange 0,1000 | TuyaMCU 11,20 | TuyaMCU 21,22 | TuyaMCU 23,23 | TuyaMCU 98,101"}
MiBoxer WL5 LED Controller   {"NAME":"WL5","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"DimmerRange 100,1024 | TuyaMCU 11,20 | TuyaMCU 26,21 | TuyaMCU 21,23 | TuyaMCU 24,24 | TuyaMCU 81,26 | TuyaMCU 98,101"}
Nexlux                       {"NAME":"MagicHome V1.1","GPIO":[0,0,0,0,1088,416,0,0,417,418,0,0,0,0],"FLAG":0,"BASE":34}
Powertech RGB                {"NAME":"Powertech RGB ","GPIO":[0,0,0,0,416,419,0,0,418,420,417,0,0,0],"FLAG":0,"BASE":18}
QS-WIFI-RGBCW                {"NAME":"QS-WIFI-RGBCW","GPIO":[0,0,32,0,544,417,0,0,418,0,289,416,0,0],"FLAG":0,"BASE":18}
RGB 12-24V                   {"NAME":"WS03","GPIO":[0,0,0,0,0,0,0,0,418,417,416,0,0,0],"FLAG":0,"BASE":18}
RGB+CCT 12-24V               {"NAME":"WS05","GPIO":[0,0,0,0,0,420,0,0,418,417,416,419,0,0],"FLAG":0,"BASE":18}
RGBW 12-24V                  {"NAME":"*WS04","GPIO":[0,0,0,0,0,0,0,0,417,418,416,419,0,0],"FLAG":0,"BASE":18}
Robus CCT                    {"NAME":"RobusCCT","GPIO":[0,0,0,0,32,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Shelly RGBW2                 {"NAME":"Shelly RGBW2","GPIO":[0,0,288,0,419,1,0,0,416,32,418,417,0,0],"FLAG":0,"BASE":18}
Spectrum Smart RGBCCT        {"NAME":"Spectrum Smart RGB CCT Controller","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Tuya RGBCCT                  {"NAME":"AP-5CH-1","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Xunata Led Controller High Voltage 110/220V  {"NAME":"KL-LED-WIFI-AC","GPIO":[0,0,0,0,0,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":18}
ZJ-WF-ESP-A v1.1             {"NAME":"RGB2","GPIO":[0,0,0,0,0,0,0,0,417,416,418,0,0,0],"FLAG":0,"BASE":18}
```

## LED Strip
```
Aldi Casalux RGB             {"NAME":"DW-RGB-WI01","GPIO":[1088,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
ARLEC 5m Colour Changing     {"NAME":"ALD557HA","GPIO":[0,0,0,1376,0,0,0,0,0,1088,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 1m CCT LED Strip Light  {"NAME":"ALD155HA","GPIO":[0,0,1088,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 2m LED Colour Changing Strip Light  {"NAME":"Arlec_Light_Strip","GPIO":[1,1,1088,1,416,419,1,1,417,420,418,0,1,1],"FLAG":0,"BASE":18}
Arlec Smart 5m White & Colour Changing  {"NAME":"ALD556HA","GPIO":[0,0,1088,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
B.K. Licht 2m RGB            {"NAME":"RGBW-Strip","GPIO":[0,0,0,0,416,32,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
BAZZ 10 ft RGBW              {"NAME":"BAZZ U183MRGBWWF RGBW LED Strip","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-LT11            {"NAME":"BW-LT11 Strip","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20743 RGB+W   {"NAME":"BrilliantStrip","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Briloner 2256-150 RGB        {"NAME":"Briloner2256-1","GPIO":[1088,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Calex 5m RGB + White         {"NAME":"Calex RGBW IR","GPIO":[1088,0,0,0,416,0,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Cocoon Smart                 {"NAME":"Cocoon Smart","GPIO":[32,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Deltaco 3m RGBCCT            {"NAME":"Deltaco Led Strip","GPIO":[0,0,0,0,416,32,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
electriQ 3m RGBCCT           {"NAME":"ElectricQ wifiRGBWLEDSTR","GPIO":[0,0,0,0,416,420,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Elfeland 10m RGB             {"NAME":"Elfeland RGB","GPIO":[0,0,0,0,0,417,0,0,418,1088,0,416,0,0],"FLAG":0,"BASE":18}
Energizer Multi-Color 6.5ft  {"NAME":"Energizer","GPIO":[0,0,0,0,0,417,0,0,418,0,0,416,0,0],"FLAG":0,"BASE":18}
Geeni Prisma Plus            {"NAME":"Geeni Prisma Plus Strip","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
GoKlug RGB                   {"NAME":"Tuya LED Strip","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Gosund NiteBird 32.8ft RGB   {"NAME":"Gosund SL3","GPIO":[0,0,32,0,417,0,0,0,416,418,4768,0,0,0],"FLAG":0,"BASE":18}
Gosund NiteBird 5m RGB       {"NAME":"gosund SL2","GPIO":[1,1,1,1,35,417,1,1,416,418,1,1,1,1],"FLAG":0,"BASE":18}
Gosund NiteBird RGB          {"NAME":"Gosund LED Strip","GPIO":[0,0,0,0,32,417,0,0,416,418,0,0,0,4768],"FLAG":0,"BASE":18}
HitLights L1012V-MC1         {"NAME":"HitLights RBG","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
HomeMate 10m RGB             {"NAME":"Homemate Strip","GPIO":[0,0,0,0,0,416,0,0,418,32,417,0,0,0],"FLAG":0,"BASE":18}
Hykker 3m RGB                {"NAME":"HYKKER Strip","GPIO":[0,0,0,0,0,416,0,0,418,32,417,0,0,0],"FLAG":0,"BASE":18}
INDARUN RGB String Lights    {"NAME":"STAR301","GPIO":[0,0,0,0,1088,416,0,0,417,418,0,0,0,0],"FLAG":0,"BASE":34}
Kogan RGB + Cool & Warm White 2m  {"NAME":"RGB+W+C Strip","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LE LampUX 16.4ft RGB         {"NAME":"LampUX","GPIO":[0,33,32,0,0,417,0,0,418,1088,0,416,0,0],"FLAG":0,"BASE":18}
LE LampUX 2m RGB TV Backlight  {"NAME":"LE 904102","GPIO":[0,32,33,0,0,417,0,0,418,34,0,416,0,0],"FLAG":0,"BASE":18}
LE LampUX 5m RGB             {"NAME":"LampUX","GPIO":[32,0,0,0,0,417,0,0,418,0,0,416,0,0],"FLAG":0,"BASE":18}
LE LampUX 5m RGB             {"NAME":"LE LampUx","GPIO":[0,0,0,0,0,417,0,0,418,0,0,416,0,0],"FLAG":0,"BASE":34}
LE lampUX 5m RGBW            {"NAME":"LampUX","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Lednify WiZ Connected 5m RGB+W  {"NAME":"Lednify 429336","GPIO":[0,0,420,0,419,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,416,417,418,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Lenovo 2m RGB                {"NAME":"Lenovo LED Strip 2m","GPIO":[32,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Lenovo 5m RGBW               {"NAME":"Lenovo LED Strip 5m","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LePower 32.8ft RGB           {"NAME":"LePower","GPIO":[0,0,0,0,0,417,0,0,418,0,0,416,0,0],"FLAG":0,"BASE":18}
Lohas ZN022 5m RGBW          {"NAME":"LOHAS M5-022","GPIO":[0,0,0,0,417,416,0,0,32,418,0,0,0,0],"FLAG":0,"BASE":18}
LSC RGBW                     {"NAME":"LSC RGBW Strip","GPIO":[1088,0,0,0,416,0,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Lumary RGBCCT                {"NAME":"Lumary LED","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lumary RGBCCT                {"NAME":"Lumary LED","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Maxonar Lightstrip Pro XS-SLD001  {"NAME":"Maxonar LED","GPIO":[0,0,0,0,0,416,0,0,418,32,417,0,0,0],"FLAG":0,"BASE":18}
MegaLight Smart RGB          {"NAME":"MegaLight Smart LED-Stripe","GPIO":[0,0,0,0,417,1088,0,0,418,416,0,0,32,0],"FLAG":0,"BASE":18}
Merkury 6-1/2 ft             {"NAME":"Mercury RGBWWCW String","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Merkury Innovations MI-EW003-999W  {"NAME":"MI-EW003-999W ","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio RGB+CW       {"NAME":"MirabellaStrip","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Monster Smart IlluminEssence  {"NAME":"MI-EW003-999W ","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Polux RGB+NW 2m              {"NAME":"Polux Wi-Fi SM","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":37}
Polux RGBCCT                 {"NAME":"Polux Wi-Fi SM","GPIO":[32,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":37}
Polux RGBCCT 20W 1700lm 5m   {"NAME":"Polux LED 5m","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Powertech 5m RGBW            {"NAME":"Jaycar ST3992 LED Strip","GPIO":[0,0,0,0,416,419,0,0,418,420,417,0,0,0],"FLAG":0,"BASE":18}
Reafoo RGBW 5m               {"NAME":"REAFOO RGBW LS","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Simfonio 5m RGB              {"NAME":"SimfonioLEDStrip","GPIO":[32,0,1088,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Sonoff L1                    {"NAME":"SonoffL1","GPIO":[0,3200,0,3232,0,0,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":70}
Sonoff L1 Lite RGB           {"NAME":"Sonoff L1 Lite","GPIO":[0,3200,0,3232,0,0,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":70}
TBI Pro RGB                  {"NAME":"TBI-PRO RGB Strip Light","GPIO":[32,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Techvilla TE001              {"NAME":"TE-TE001","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Teckin SL02                  {"NAME":"Teckin SL02","GPIO":[1088,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":52}
Teckin SL07 32.8ft RGB       {"NAME":"WIFI RGB","GPIO":[1088,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Teopek Spider Z RGB          {"NAME":"Sonoff L1","GPIO":[0,3200,0,3232,0,0,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":70}
TORCHSTAR CCT 30W Lighting Kit  {"NAME":"Torchstar CCT ","GPIO":[0,0,0,0,288,417,0,0,0,32,0,416,289,0],"FLAG":0,"BASE":18}
Torchstar Safe Lighting Kit  {"NAME":"Torchstar","GPIO":[0,0,0,0,288,0,0,0,0,0,0,416,289,4704],"FLAG":0,"BASE":18}
WiZ 2m Starter Kit           {"NAME":"WiZ 9290025266","GPIO":[0,0,420,0,419,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,416,417,418,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
WOOX 5m RGBW                 {"NAME":"GardenLedstrip1","GPIO":[0,0,0,0,0,417,0,0,418,160,416,0,0,0],"FLAG":0,"BASE":18}
WOOX 5m RGBW                 {"NAME":"Woox R5093","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Yeelight Lightstrip 1S       {"NAME":"Yeelight Lightstrip 1S","GPIO":[0,0,0,0,0,418,0,0,0,416,417,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"SO37 128 | SO92 1"}
Zemismart 3m Extendable RGBW  {"NAME":"Zemismart LED","GPIO":[0,0,0,0,417,416,0,0,0,418,419,0,0,0],"FLAG":0,"BASE":18}
```

## Light
```
Arlec Smart 10W LED Bunker   {"NAME":"DetaBulkhead","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 15W Security Floodlight  {"NAME":"ArlecFlood","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,1],"FLAG":0,"BASE":18}
Arlec Smart 20W Movement Activated Security  {"NAME":"Arlec MAL300HA","GPIO":[0,0,0,0,224,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 40W 4000lm LED Batten  {"NAME":"Arlec Batten","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart Portable Floodlight 10.5W  {"NAME":"Arlec GLD301HA","GPIO":[0,0,0,0,0,416,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart Security         {"NAME":"Arlec MAL315HA","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Arlec Smart Up & Down LED Wall  {"NAME":"Arlec Up Down CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
BAZZ 4 in. RGB Recessed Fixture  {"NAME":"SLMR4RGBWWFW","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
BAZZ 6 in. CCT Recessed Fixture  {"NAME":"SLDSK6TNWWF","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
Brelong USB Charging Night   {"NAME":"Brelong Smart USB Charging Lamp","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,419,0],"FLAG":0,"BASE":18}
Brilex Nightstand Lamp       {"NAME":"Smart Table Lamp","GPIO":[0,0,33,0,416,544,0,0,417,32,418,0,419,0],"FLAG":0,"BASE":18}
BrilliantSmart RGB Garden Kit  {"NAME":"Brilliant Gard","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome CSH-FSTN12  {"NAME":"CSH-FSTN12","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Deta 18W 1900lm T8 Tube      {"NAME":"DETA Smart LED","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
electriQ MOODL Ambiance Lamp  {"NAME":"ElectriQ MOODL","GPIO":[0,4640,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Gosund Table Lamp            {"NAME":"Gosund LB3","GPIO":[0,0,0,0,0,418,0,0,417,419,0,416,0,0],"FLAG":0,"BASE":18}
Hama Wall Light Square, 10 cm, IP 44  {"NAME":"Hama Wifi Wall Light","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,1],"FLAG":0,"BASE":18}
HiFree Table Lamp            {"NAME":"TuyaMCU","GPIO":[2304,1184,2272,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,0],"FLAG":0,"BASE":18}
Hugoai Table Lamp            {"NAME":"HG02","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,20 | TuyaMCU 26,21 | TuyaMCU 21,22 | TuyaMCU 23,23 | TuyaMCU 24,24 | DimmerRange 34,1000"}
Iwoole Table Lamp            {"NAME":"GLOBELAMP","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Lepro Bedroom Lamp           {"NAME":"Lepro 902101-US","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,20 | TuyaMCU 26,21 | TuyaMCU 21,22 | TuyaMCU 23,23 | TuyaMCU 24,24 | DimmerRange 34,1000"}
LSC Multicolor Christmas String  {"NAME":"LSC MC Lights","GPIO":[0,0,0,0,544,32,0,0,3840,0,3872,0,0,0],"FLAG":0,"BASE":18}
LSC Outdoor Up and Down      {"NAME":"LSC RGBCCT Outdoor","GPIO":[0,0,0,0,416,418,0,0,419,417,420,0,0,0],"FLAG":0,"BASE":18}
LSC Sensor LED Flood         {"NAME":"LSC Smart LED Flood Light with Sensor","GPIO":[0,0,0,0,416,417,0,0,0,160,0,0,0,4768],"FLAG":0,"BASE":18}
LSC Smart Mood               {"NAME":"LSC Mood Light","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lumary 18W RGBCCT Recessed Panel  {"NAME":"LumaryDLghtRGB","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Mi LED Desk Lamp             {"NAME":"Mi Desk Lamp","GPIO":[0,0,32,0,416,417,0,0,3264,3296,0,0,0,0],"FLAG":0,"BASE":66}
Mirabella Genio 10 LED Filament Festoon  {"NAME":"GenioFestoon","GPIO":[0,0,0,0,0,0,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 4 Black LED Garden Path  {"NAME":"GenioGardenStr","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio CCT 6 LED 30mm Stainless Steel Deck  {"NAME":"Mirabella Deck CCT","GPIO":[0,0,0,0,0,416,0,0,0,417,0,0,0,0],"FLAG":0,"BASE":18}
MiraBella Genio Colour 6 LED 30mm Stainless Steel Deck  {"NAME":"Genio RGB Deck Lights","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio Rechargable LED Portable Lamp  {"NAME":"MirabellaPortableLamp","GPIO":[32,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Moes RGBCW (8+8+15+15W)      {"NAME":"JL-TD3-4-4-2516-V1.0","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Nous CCT Desk Lamp           {"NAME":"NOUS Smart Desk Lamp S1","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 21,3 | TuyaMCU 23,4 | DimmerRange 25,255"}
Novostella Bloom 20W RGBCW Flood  {"NAME":"Novostella NTF36 Floodlight","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Novostella UT88835 20W Flood  {"NAME":"Novo 20W Flood","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Novostella UT88836 20W Flood  {"NAME":"Novo 20W Flood","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Nue Vision Care Desk Lamp    {"NAME":"Nue Vision Desk Lamp VC18","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 21,3 | TuyaMCU 23,4"}
Philips Wiz 24W LED White Batten  {"NAME":"PHILIPS-wiz-24w","GPIO":[0,0,0,0,417,0,0,0,0,416,0,0,0,0],"FLAG":0,"BASE":18}
Polycab Hohm Avenir 20W Batten  {"NAME":"PolycabBatten","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":18}
RGB Floor Lamp               {"NAME":"Floor Lamp","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Sonoff                       {"NAME":"Sonoff BN-SZ","GPIO":[0,0,0,0,0,0,0,0,416,320,0,0,0,0],"FLAG":0,"BASE":22}
Spotlight 9cm RGB+W 7W       {"NAME":"Spotlight RGBW","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
TCP WPAN Square 600X600mm 36W CCT Panel  {"NAME":"TCPsmart LED Panel","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO92 1|DimmerRange 30,100"}
Teckin FL41                  {"NAME":"Teckin FL41","GPIO":[0,0,0,0,0,32,0,0,0,0,416,0,0,0],"FLAG":0,"BASE":18}
Wipro 20W LED RGB Batten     {"NAME":"Wipro RGBW Tubelight","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,1],"FLAG":0,"BASE":18}
Wipro Next Smart Batten 20W CCT  {"NAME":"WIPROBatten","GPIO":[0,0,0,0,0,416,0,0,0,449,0,0,0,0],"FLAG":0,"BASE":18}
Xiaomi Mi Computer Monitor Light Bar 1S  {"NAME":"Mijia Desk Lamp 1S (MJGJD02YL)","GPIO":[0,0,0,0,3840,0,1,1,0,0,0,0,0,0,0,416,0,417,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"DimmerRange 45,255"}
Xiaomi Mi Desk Lamp Pro      {"NAME":"Mi Desk Lamp Pro","GPIO":[6212,0,416,0,417,0,0,0,3840,0,0,0,160,640,608,0,0,0,0,0,0,0,3296,3264,0,0,0,0,0,32,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"DimmerRange 30,100"}
Xiaomi Mi LED Desk Lamp 1S   {"NAME":"Mi LED Desk Lamp 1S","GPIO":[6212,0,416,0,417,0,0,0,3840,0,0,0,160,640,608,0,0,0,0,0,0,0,3264,3296,0,0,0,0,0,32,0,0,0,0,0,0],"FLAG":3,"BASE":66,"CMND":"DimmerRange 30,100|Fade 1|PowerOnFade 1|Speed 2"}
Yeelight Crystal Pendant     {"NAME":"Yeelight Meteorite","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,420,0,419,0,0,0,0,417,418,0,0,0,0,0,416,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"SO37 128"}
Zemismart Moonlamp           {"NAME":"Zemismart Moonlamp","GPIO":[1,1,1,1,416,419,1,1,417,420,418,1,1,1],"FLAG":0,"BASE":18}
```

## Miscellaneous
```
BlitzWolf Air Fryer          {"NAME":"BW-AF1","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,111 | TuyaMCU 12,101 | TuyaMCU 71,104 | TuyaMCU 81,106 | TempRes 0"}
Blitzwolf UV Pet Water Fountain  {"NAME":"BW-SPF1","GPIO":[0,0,0,0,224,416,0,0,0,32,160,0,576,0],"FLAG":0,"BASE":18}
Coiaca ABCt v2.2 Interface for Boilers  {"NAME":"Coiaca ABCt","GPIO":[320,1216,0,0,224,160,0,0,32,416,0,0,0,0],"FLAG":0,"BASE":18}
Dealdig Robvaccum 8 Vacuum   {"NAME":"WhiteVacuum","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Digital Body Fat Scale       {"NAME":"TuyaScale","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Dual-Pump Automatic Watering System  {"NAME":"WD-01ADE","GPIO":[32,0,0,0,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
EaryKong Doorbell            {"NAME":"Doorbell","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
electriQ 10000 BTU Wall Mounted Heat Pump Air Conditioner  {"NAME":"electriQ IQOOL-SMART12HP","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Eufy RoboVac C30 Robot Vacuum  {"NAME":"Eufy Robovac","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,2 | TuyaMCU 12,101 | TuyaMCU 13,103 | SetOption97 1"}
Fujin Irrigation Timer       {"NAME":"Fujin FJKB022-B4","GPIO":[0,0,0,0,32,0,0,0,288,0,224,0,0,0],"FLAG":0,"BASE":18}
iLONDA Fish Feeder           {"NAME":"Feeder","GPIO":[0,0,0,0,32,320,0,0,352,0,224,0,0,0],"FLAG":0,"BASE":18}
Kogan 4.1kW Portable Air Conditioner (Reverse Cycle)  {"NAME":"Kogan Panel Heater","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Kogan Pet Fountain           {"NAME":"Pet Fountain","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
LED Starry Sky Projector Light  {"NAME":"STAR PROJECTOR","GPIO":[0,0,0,0,416,0,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Liectroux C30B Robot Vacuum  {"NAME":"Liectroux C30B","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Mosquito Killer Lamp         {"NAME":"MosquitoKiller","GPIO":[32,0,0,0,0,0,0,0,416,320,0,0,0,0],"FLAG":0,"BASE":18}
NEO Coolcam Mouse Trap       {"NAME":"NAS-MA01W","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 51,51 | TuyaMCU 99,11"}
PCI-e Desktop PC Remote Control  {"NAME":"PC-Switch-01","GPIO":[32,0,0,0,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":18}
Petoneer Smart Dot Cat Toy   {"NAME":"Petoneer Smart Dot","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Proscenic T21 Air Fryer      {"NAME":"Proscenic T21","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
RainPoint Indoor Water Pump  {"NAME":"RainPoint","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 81,107|TuyaMCU 12,109|TuyaMCU 11,1|TuyaMCU 82,104"}
Sinilink PCIe Computer Remote  {"NAME":"XY-WPCE","GPIO":[1,1,320,1,160,224,0,161,162,0,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO114 1 | Pulsetime 10 | SwitchMode1 2 | SwitchMode2 2 | SwitchMode3 2"}
Sinilink USB Computer Remote  {"NAME":"XY-WPCL","GPIO":[0,0,320,0,0,224,0,32,160,0,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO114 1 | Pulsetime 10 | SwitchMode1 2"}
Sinilink XY-Clock Clock Alarm Module  {"NAME":"XY-Clock","GPIO":[288,0,289,0,0,416,32,33,608,640,0,0,34,0],"FLAG":0,"BASE":18}
Sunbeam LoftTec Electric Blanket  {"NAME":"Sunbeam Heated Blanket","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Ulanzi Smart Pixel Clock     {"NAME":"Ulanzi TC001","GPIO":[0,0,0,0,0,0,0,0,0,0,34,480,0,0,0,0,0,640,608,0,0,0,32,33,0,0,0,0,1376,0,4705,4768,0,0,0,0],"FLAG":0,"BASE":1}
Xystec USB3.0 4 Port Hub     {"NAME":"Xystec USB Hub","GPIO":[0,0,0,0,224,0,0,0,226,227,225,0,0,0],"FLAG":0,"BASE":18}
```

## Module
```
Ants Make 16 Channel ESP32 Relay Driver 5V DC  {"NAME":"AM-022","GPIO":[1,1,1,1,232,237,1,1,228,231,229,1,233,230,234,235,0,238,239,236,0,224,227,226,0,0,0,0,1,225,1,1,1,0,0,1],"FLAG":0,"BASE":1}
DT-Light ESP8285 Lighting    {"NAME":"DMP-L1","GPIO":[1,1,0,1,1,1,0,0,1,1,1,1,1,1],"FLAG":0,"BASE":18}
ESP-01D                      {"NAME":"ESP-01D","GPIO":[1,1,0,1,1,0,0,0,1,0,1,0,0,0],"FLAG":0,"BASE":18}
ESP-01S                      {"NAME":"ESP-01","GPIO":[1,1,1,1,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
ESP-02S TYWE2S Replacement   {"NAME":"ESP-02S","GPIO":[1,1,1,1,1,1,0,0,1,1,1,0,0,1],"FLAG":0,"BASE":18}
ESP-12                       {"NAME":"ESP-12","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,1],"FLAG":0,"BASE":18}
ESP-15F                      {"NAME":"ESP-15F","GPIO":[1,1,0,1,1,1,0,0,0,544,0,0,0,0],"FLAG":0,"BASE":18}
ESP-M2                       {"NAME":"ESP-M2","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,0,1],"FLAG":0,"BASE":18}
ESP-M3                       {"NAME":"ESP-M3","GPIO":[1,1,1,1,1,0,0,0,0,1,1,0,1,0],"FLAG":0,"BASE":18}
ESP-M4                       {"NAME":"ESP-M4","GPIO":[1,1,0,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":18}
ESP8266 5V UART Communication  {"NAME":"ESP-WROOM-5V2L","GPIO":[0,1,0,1,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
LC Technology PSF-B04 Ewelink 4 Channel Switch  {"NAME":"LC-EWL-B04-MB","GPIO":[1,1,1,1,1,1,0,0,1,288,1,1,0,0],"FLAG":0,"BASE":18}
M5Stack M5Stamp Pico         {"NAME":"M5Stamp Pico","GPIO":[1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1376,0,0,0,0,1,1,0,0,1,0,0,32],"FLAG":0,"BASE":1}
MTools 16 Channel ESP32 Relay Driver 5V DC  {"NAME":"16ch Board","GPIO":[1,1,237,1,232,1,1,1,228,231,1,1,233,230,234,235,0,238,239,236,0,224,227,226,0,0,0,0,229,225,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Shelly Universal Input/Output  {"NAME":"Shelly Uni","GPIO":[320,0,0,0,225,1216,0,0,192,193,194,224,0,4864],"FLAG":0,"BASE":18}
Sinilink MODBUS Interface    {"NAME":"XY-WFPOW","GPIO":[0,8768,544,8800,32,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
```

## Motion Sensor
```
DP-WP001 PIR                 {"NAME":"TUYA PIR","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Lenovo Rechargable PIR Motion  {"NAME":"Lenovo PIR","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Mirabella Genio I002576      {"NAME":"GenioPir","GPIO":[32,2272,0,2304,0,0,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":54}
Tuya Alarm PIR               {"NAME":"CT61W","GPIO":[0,0,0,0,0,0,0,0,0,160,480,256,0,0],"FLAG":0,"BASE":18}
```

## Motor
```
Steren Curtain               {"NAME":"Steren_SHOME-155","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54,"CMND":"SO54 1|SO20 1|TuyaMCU 61,1|TuyaMCU 21,2|TuyaMCU 27,3|TuyaMCU 97,5|TuyaMCU 11,6|TuyaMCU 62,7|TuyaMCU 63,8|TuyaMCU 81,9|TuyaMCU 98,10|TuyaMCU 82,11"}
Zemismart BCM300D-TY         {"NAME":"Zemistart_Curt","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Zemismart Blinds Controller  {"NAME":"Zemismart Blind WBR3","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,7|TuyaMCU 21,2"}
Zemismart Curtain            {"NAME":"Zemismart_Curt","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Zemismart Rechargeable Roller Shade  {"NAME":"Zemismart Remote","GPIO":[544,0,288,33,225,32,0,0,34,226,289,224,290,0],"FLAG":0,"BASE":18}
Zemismart Roller Shade       {"NAME":"M2805EIGB","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Zemismart Updated RF Remote Roller Shade  {"NAME":"Zemismart M515EGB","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
```

## Other
```
Kogan SmarterHome 1.9L Pet Water Fountain  {"NAME":"WaterFountain","GPIO":[257,0,259,0,576,32,0,0,224,161,0,480,226,4704],"FLAG":0,"BASE":18}
Tetra Connect Automatic Feeder  {"NAME":"Tetra Connect","GPIO":[0,0,0,0,224,288,0,0,98,96,97,0,289,0],"FLAG":0,"BASE":18}
```

## Outdoor Plug
```
Acenx SOP04-US Dual          {"NAME":"SOP04-US Dual","GPIO":[1,1,1,1,320,321,0,0,224,32,225,1,1,0],"FLAG":0,"BASE":18}
Aicliv 3 Outlet              {"NAME":"AICLIV SOP03US","GPIO":[0,0,0,226,321,0,0,0,224,33,225,0,0,0],"FLAG":0,"BASE":18}
Albohes PC-1606              {"NAME":"Albohes PC1606","GPIO":[32,0,0,0,0,225,33,0,224,0,0,0,0,4704],"FLAG":0,"BASE":39}
Albohes PS-1602              {"NAME":"Albohes PC1606","GPIO":[32,0,0,0,0,225,33,0,224,0,0,0,0,4704],"FLAG":0,"BASE":39}
Amzdest Dual                 {"NAME":"Amzdest c158","GPIO":[0,32,0,225,2720,2656,0,0,224,2624,0,0,0,0],"FLAG":0,"BASE":18}
Amzdest IP55                 {"NAME":"C168 Outdoor","GPIO":[0,0,0,2624,2720,2656,0,0,224,320,225,226,32,0],"FLAG":0,"BASE":18}
Aoycocr X13                  {"NAME":"Aoycocr X13","GPIO":[0,0,320,0,0,0,0,0,225,32,0,224,0,0],"FLAG":0,"BASE":18}
Arlec IP44 Weatherproof      {"NAME":"PC44HA","GPIO":[0,0,0,35,2720,2656,0,0,2592,320,224,0,0,0],"FLAG":0,"BASE":18}
Atomi                        {"NAME":"AtomiSmartPlug","GPIO":[0,0,0,0,320,576,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Blitzwolf 16A IP44 Dual      {"NAME":"Blitzwolf BW-SHP14","GPIO":[0,320,0,32,0,321,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
BN-Link                      {"NAME":"BNC-60/U130T","GPIO":[1,0,1,0,1,320,1,1,224,32,1,0,0,1],"FLAG":0,"BASE":18}
Brennenstuhl WA 3000         {"NAME":"WA 3000 XS02","GPIO":[0,0,0,0,224,32,0,0,576,288,0,0,0,0],"FLAG":0,"BASE":61}
BSEED                        {"NAME":"PA-GEBA-01SWP","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
C119                         {"NAME":"Subosi C119","GPIO":[0,0,0,0,225,320,0,0,224,544,32,0,321,0],"FLAG":0,"BASE":18}
C137 IP55                    {"NAME":"C137 Outdoor","GPIO":[0,32,0,320,2720,2656,0,0,224,2624,225,0,0,1],"FLAG":0,"BASE":18}
C168 IP64                    {"NAME":"C188","GPIO":[320,0,321,0,33,0,0,0,224,32,544,225,0,0],"FLAG":0,"BASE":18}
ChilITec Pilota Casa IP44    {"NAME":"Outdoor IP44","GPIO":[0,0,0,0,0,224,0,0,0,288,32,0,0,0],"FLAG":0,"BASE":18}
Deltaco 13A IP44             {"NAME":"Deltaco SH-OP01","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":55}
Dewenwils Heavy Duty 40A     {"NAME":"Dewenwils HOWT01A","GPIO":[0,0,544,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
ECF-SOP03                    {"NAME":"Outdoor3Outlet","GPIO":[0,0,0,226,320,0,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Ecoolbuy 4 Socket IP44       {"NAME":"ECCOLBUY 4","GPIO":[0,0,0,0,225,226,0,0,224,321,32,0,227,0],"FLAG":0,"BASE":18}
Edimax 2AC                   {"NAME":"EDI SP-1122WTO","GPIO":[0,0,0,0,225,576,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Emax IP44                    {"NAME":"Emax Smart Socket","GPIO":[0,0,0,0,320,0,0,0,224,321,32,0,0,0],"FLAG":0,"BASE":18}
Energizer 2AC Weather Resistant  {"NAME":"Energizer EOX3-1001-BLK","GPIO":[0,0,0,0,320,576,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Etekcity                     {"NAME":"ES015-TB","GPIO":[0,0,0,0,224,225,288,0,2656,2688,32,2592,289,0],"FLAG":0,"BASE":18}
Feit Electric PLUG/WIFI/WP   {"NAME":"Prime Smart ou","GPIO":[0,1,0,1,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Forrinx IP44                 {"NAME":"SH-18","GPIO":[0,0,0,0,320,321,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Forrinx SH-18EU-A            {"NAME":"SH-18EU-A","GPIO":[0,0,0,0,225,288,224,321,32,0,0,0,0,0],"FLAG":0,"BASE":18}
Foxnovo                      {"NAME":"SOP03-US","GPIO":[0,0,0,226,320,321,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Foxnovo IP44 Dual            {"NAME":"SOP06-US","GPIO":[0,0,0,0,320,321,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Geeni Outdoor DUO Dual Outlet  {"NAME":"Geeni Dual Out","GPIO":[32,0,0,0,0,321,0,0,0,320,224,0,225,0],"FLAG":0,"BASE":18}
Globe 2-Outlet               {"NAME":"Globe 50029","GPIO":[0,0,0,321,225,544,0,0,32,224,320,0,0,0],"FLAG":0,"BASE":18}
HA109US                      {"NAME":"HA109US","GPIO":[32,0,0,0,288,289,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
Hama 2,300W/10A              {"NAME":"Hama Outdoor","GPIO":[0,0,0,0,576,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HBN 6 Outlet Yard Stake      {"NAME":"BNC-60/U13WT","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HBN Heavy Duty               {"NAME":"HBN U151T","GPIO":[0,0,0,0,288,289,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
iClever IC-BS06              {"NAME":"iClever Switch","GPIO":[0,0,0,0,544,320,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Jackyled IP55 3AC            {"NAME":"JackyLed 3","GPIO":[0,0,320,0,0,226,0,0,225,321,32,0,224,0],"FLAG":0,"BASE":18}
King-Link C128               {"NAME":"King-Link C128","GPIO":[0,0,322,0,225,320,0,0,226,544,32,224,321,0],"FLAG":0,"BASE":18}
Kogan Energy Meter IP44      {"NAME":"Kogan Smart Sw IP44","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Konyks Pluviose 16A IP55     {"NAME":"Konyks Pluviose","GPIO":[32,0,0,0,0,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
Koolertron                   {"NAME":"C168 Outdoor","GPIO":[0,32,0,320,2720,2656,0,0,224,2624,225,226,0,0],"FLAG":0,"BASE":18}
Ledvance Smart+ 16A          {"NAME":"LEDVANCE Smart Wifi Outdoor Plug","GPIO":[0,0,0,320,2688,2656,0,0,224,32,2624,0,0,0],"FLAG":0,"BASE":18}
Ledvance Smart+ Compact      {"NAME":"LEDVANCE SMART+ Compact Outdoor Plug ","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
LSC Dual Socket              {"NAME":"LSC NFL-022","GPIO":[0,0,0,0,320,32,0,0,0,224,225,0,0,0],"FLAG":0,"BASE":18}
LSC Dual Socket              {"NAME":"LSC Outdoor Dual Socket","GPIO":[320,0,0,32,8673,8672,0,0,0,0,8674,0,8675,0],"FLAG":0,"BASE":18}
Luminea 16A                  {"NAME":"NX-4655-675","GPIO":[0,0,0,0,320,576,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Luminea 2 Outlet             {"NAME":"Luminea","GPIO":[0,0,0,0,225,320,0,0,224,321,32,0,0,1],"FLAG":0,"BASE":18}
Luminea NX-4458              {"NAME":"Luminea NX4458","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":65}
Master                       {"NAME":"Master_IOT-EXTPLUG","GPIO":[32,1,0,1,1,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":1}
Maxcio EOP03-EU              {"NAME":"Maxcio EOP03-EU","GPIO":[0,0,0,0,225,321,0,0,224,288,32,0,322,0],"FLAG":0,"BASE":18}
Maxcio SOP02-US              {"NAME":"Maxcio SOP02US","GPIO":[0,0,0,0,0,544,0,0,224,32,225,0,0,1],"FLAG":0,"BASE":18}
Merkury                      {"NAME":"Merkury Switch","GPIO":[32,0,0,0,0,321,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
Merkury                      {"NAME":"Merkury Switch","GPIO":[0,0,0,0,32,0,0,0,0,224,0,0,0,0],"FLAG":0,"BASE":18}
Meross MSS620 16A IP44       {"NAME":"MSS620","GPIO":[0,320,0,320,225,0,0,0,224,576,32,0,0,0],"FLAG":0,"BASE":18}
Minoston MP22W               {"NAME":"Minoston MP22W","GPIO":[0,0,0,0,320,0,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
Nedis PO120 IP44             {"NAME":"WIFIPO120FWT","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
Obi Stecker IP44             {"NAME":"OBI-Outdoor-Socket2","GPIO":[0,0,0,0,224,32,0,0,576,288,1,1,1,1],"FLAG":0,"BASE":18}
Oittm IP44 15A               {"NAME":"Oittm Outdoor","GPIO":[576,0,320,0,2688,2656,0,0,224,2592,0,0,0,0],"FLAG":0,"BASE":57}
Oittm Outdoor                {"NAME":"Oittm Outdoor","GPIO":[32,0,0,0,0,0,0,0,0,0,320,224,1,0],"FLAG":0,"BASE":18}
Peteme PS-1602               {"NAME":"Peteme Outdoor","GPIO":[32,0,0,0,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Poweradd                     {"NAME":"POWERADD","GPIO":[0,0,0,0,320,321,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Prime                        {"NAME":"CCRCOWF2PK-ESP-02S","GPIO":[1,1,1,1,288,160,1,1,1,224,1,1,1,1],"FLAG":0,"BASE":18}
Prime                        {"NAME":"PRIME CCRCWFI02PK","GPIO":[0,0,0,0,320,544,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Prime RCWFIO 2-Outlet        {"NAME":"Prime Outdoor","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Prime Smart Outlet           {"NAME":"Prime CCWFIO232PK","GPIO":[0,0,0,321,225,576,0,0,32,224,320,0,0,0],"FLAG":0,"BASE":18}
Prokord 2in1                 {"NAME":"PSH-SO-05","GPIO":[0,0,0,0,225,576,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Signstek EOP03-EU            {"NAME":"Signstek EOP03","GPIO":[0,0,0,0,320,321,0,0,224,32,225,0,0,1],"FLAG":0,"BASE":18}
SK03                         {"NAME":"SK03 Outdoor","GPIO":[32,0,0,0,2688,2656,0,0,2624,321,320,224,0,0],"FLAG":0,"BASE":57}
STITCH                       {"NAME":"STITCH 35556","GPIO":[1,1,1,1,225,321,0,0,224,320,32,1,1,0],"FLAG":0,"BASE":18}
Suraielec 40A Heavy Duty     {"NAME":"Suraielec UBTW01B","GPIO":[0,0,0,0,544,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Tate Guard ZUM-26EU-X Dual   {"NAME":"Tate Guard","GPIO":[0,0,0,0,224,288,0,0,225,288,32,0,0,0],"FLAG":0,"BASE":18}
Teckin SS31                  {"NAME":"Teckin SS31","GPIO":[1,1,1,1,320,321,1,1,224,32,225,1,1,1],"FLAG":0,"BASE":18}
Teckin SS33                  {"NAME":"Teckin SS31","GPIO":[0,0,0,226,320,321,0,0,224,32,225,0,0,1],"FLAG":0,"BASE":18}
Teckin SS42                  {"NAME":"Teckin SS42","GPIO":[0,0,0,0,320,321,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Top-Max PS-1602              {"NAME":"PS-1602","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Torchstar LITEdge 2-in-1     {"NAME":"LITEdge Plug","GPIO":[0,0,0,0,320,321,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Treatlife Dimmer             {"NAME":"DP10 Dimmer","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Ucomen                       {"NAME":"PA-GEBA-01SWP","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Ucomen                       {"NAME":"PA-GEH-01SW2","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
WiOn Yard Stake              {"NAME":"WiOn 50053","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
WOOX R4051                   {"NAME":"WOOX R4051","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
WOOX R4052                   {"NAME":"WOOX R4052","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Wyze                         {"NAME":"Wyze Plug Outdoor","GPIO":[0,0,0,0,0,576,0,0,0,0,0,224,321,7713,7712,320,0,0,0,0,0,2624,2656,2720,0,0,0,0,225,0,4704,0,0,0,0,0],"FLAG":0,"BASE":1}
XtendLan IP66 Double         {"NAME":"XtendLan_ZAS4","GPIO":[32,0,0,0,0,225,33,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
```

## Plug
```
16A                          {"NAME":"AWP16L","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
1AC 2USB                     {"NAME":"BSD31","GPIO":[0,0,0,0,0,225,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":18}
2nice                        {"NAME":"2NICE SP111","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,4736],"FLAG":0,"BASE":18}
3Stone Mini                  {"NAME":"3Stone Smart Plug","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
7hSevenOn Home 10020         {"NAME":"7hSevenOn","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
7hSevenOn Home 10022 USB     {"NAME":"7hSevenOn","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Ablue WP1                    {"NAME":"Ablue","GPIO":[321,0,320,0,0,0,0,0,0,32,0,224,0,4704],"FLAG":0,"BASE":18}
Aigoss 16A Mini              {"NAME":"Aigoss Plug","GPIO":[1,1,0,1,288,224,0,0,290,1,32,0,1,1],"FLAG":0,"BASE":51}
Aisirer                      {"NAME":"AWP07L v2","GPIO":[0,32,321,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Aisirer                      {"NAME":"AISIRER AWP08L","GPIO":[0,0,0,0,32,321,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Aisirer AWP07L               {"NAME":"AISIRER AWP07L","GPIO":[320,0,321,0,0,2688,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Aisirer JH-G018              {"NAME":"AISIRER JH-G01","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Aisirer SWA11                {"NAME":"SWA11","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Aisirer UK-1                 {"NAME":"AISIRER","GPIO":[0,32,0,0,0,0,0,0,0,288,224,0,0,4704],"FLAG":0,"BASE":18}
AiYaTo 12W                   {"NAME":"AiYaTo-SWITCH","GPIO":[0,0,0,0,32,544,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Alecto SMART-PLUG20          {"NAME":"Alecto SP20","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Alexfirst TV-ASP801EU        {"NAME":"Alexfirst","GPIO":[32,0,0,0,290,320,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Alfawise                     {"NAME":"PE1606","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Alfawise PE1004T             {"NAME":"PE1004T","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Alfawise PF1006              {"NAME":"PF1006","GPIO":[0,0,32,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Alfawise PME1606             {"NAME":"PME1606","GPIO":[0,0,0,32,2688,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Amped Wireless               {"NAME":"AWP48W","GPIO":[0,0,0,0,320,32,0,0,0,289,224,0,0,0],"FLAG":0,"BASE":18}
Amysen JSM-WF02              {"NAME":"Amysen JSMWF02","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
ANBES ABS-CZ004              {"NAME":"ANBES ABS-CZ00","GPIO":[0,0,0,0,224,2688,0,0,2624,32,2656,225,33,0],"FLAG":0,"BASE":18}
Anncoe C318                  {"NAME":"Anncoe C318","GPIO":[64,3072,1,3104,0,0,0,0,224,289,0,0,0,0],"FLAG":0,"BASE":18}
Anoopsyche AWP08L            {"NAME":"ANOOPSYCHE AWP08L","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Anoopsyche SP-G01            {"NAME":"SP-G01","GPIO":[0,3072,0,3104,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":41}
Anoopsyche SP15              {"NAME":"Anoop SP15","GPIO":[0,0,0,0,320,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Anoopsyche UK1D              {"NAME":"UK1D","GPIO":[0,32,0,0,2688,2656,0,0,2592,288,224,0,0,0],"FLAG":0,"BASE":6}
AOFO 10A                     {"NAME":"AOFO Smart Plug Wifi","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Aoycocr                      {"NAME":"Aoycocr X10S","GPIO":[320,0,321,0,224,2720,0,0,2624,32,2656,0,0,0],"FLAG":0,"BASE":45}
Aoycocr EU5 16A              {"NAME":"Aoycocr","GPIO":[1,0,320,0,0,0,0,0,1,32,0,224,0,0],"FLAG":0,"BASE":17}
Aoycocr EU6S                 {"NAME":"Aoycocr EU6S","GPIO":[1,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":17}
Aoycocr U2S                  {"NAME":"Aoycocr U2S","GPIO":[320,0,321,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Aoycocr U3S                  {"NAME":"Aoycocr U3S","GPIO":[320,1,321,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Aoycocr X5P                  {"NAME":"Aoycocr X5P","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Aoycocr X6                   {"NAME":"Aoycocr X6","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Aquiv S1                     {"NAME":"Aquiv S1","GPIO":[0,0,544,0,320,0,0,0,224,32,0,0,0,1],"FLAG":0,"BASE":18}
Arlec 10m Smart Extension Lead  {"NAME":"Arlec Ext Cord","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Arlec Heavy Duty 20m Extension Lead  {"NAME":"Arlec Ext Cord","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Arlec Plug In Socket         {"NAME":"PC191HA","GPIO":[0,32,0,0,2720,2656,0,0,2624,224,288,0,0,0],"FLAG":0,"BASE":18}
Arlec Plug In Socket         {"NAME":"PC191HAS2 CB2S","GPIO":[0,0,0,32,2656,2624,0,0,224,2720,320,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart                  {"NAME":"Arlec-PC190HA","GPIO":[0,0,0,0,320,0,0,0,224,576,32,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart                  {"NAME":"PC399HA","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Arlec Smart 2.1A USB Charger  {"NAME":"Arlec Single","GPIO":[0,0,0,0,321,0,0,0,224,0,64,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart PC189HA          {"NAME":"Arlec Single","GPIO":[0,0,0,0,321,0,0,0,224,0,64,0,0,0],"FLAG":0,"BASE":18}
Arlec Socket With USB        {"NAME":"PC192HA","GPIO":[0,0,0,32,0,0,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
Arlec Twin PC288HA           {"NAME":"Arlec Twin","GPIO":[0,32,0,225,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Arlec Twin Socket with Energy Meter  {"NAME":"Arlec PC287HA","GPIO":[0,32,0,320,2624,2656,0,0,224,2720,225,0,0,0],"FLAG":0,"BASE":18}
Athom 16A                    {"NAME":"Athom Power Monitoring Plug","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,1],"FLAG":0,"BASE":18}
Athom 16A AU                 {"NAME":"Athom PG05-AU16A","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":1}
Athom 16A AU                 {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A BR V2              {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A Brazil             {"NAME":"Athom PG05-BR16A","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":1}
Athom 16A EU V2              {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A Israel             {"NAME":"Athom Power Monitoring Plug","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,1],"FLAG":0,"BASE":18}
Athom 16A Israel v2          {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A Italy              {"NAME":"Athom PG05-IT16A","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":1}
Athom 16A Switzerland v2     {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A UK                 {"NAME":"Athom PG04-UK16A","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Athom 16A UK V2              {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A US                 {"NAME":"Athom PG03-US16A","GPIO":[0,0,0,32,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Athom 16A US V2              {"NAME":"Athom Plug V2","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Atlantis                     {"NAME":"Atlantis Smart Plug","GPIO":[32,0,0,0,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Atomi AT1217                 {"NAME":"AT1217","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Aubess 16A                   {"NAME":"Aubess 16A Power Monitoring Plug","GPIO":[0,32,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Aukey                        {"NAME":"Aukey SH-PA2","GPIO":[0,0,0,0,576,320,0,0,32,0,224,0,0,0],"FLAG":0,"BASE":18}
Aukey                        {"NAME":"AUKEY SH-PA2","GPIO":[0,0,0,0,576,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Aukey SH-PA1                 {"NAME":"AUKEY SH-PA1","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Aukey SH-PA3                 {"NAME":"Aukey SH-PA3","GPIO":[0,0,576,0,321,32,0,0,225,0,0,224,0,0],"FLAG":0,"BASE":18}
Aunics Presa Smart           {"NAME":"AUNICS","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Aunics Smart IT              {"NAME":"AUNICS","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Avatar                       {"NAME":"Generic","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Avatar                       {"NAME":"Avatar UK 10A","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Avatar AWP01L                {"NAME":"AWP01L","GPIO":[0,0,0,0,0,320,0,0,0,32,224,0,0,0],"FLAG":0,"BASE":18}
Avatar AWP02L-N              {"NAME":"AWP02L-N","GPIO":[576,0,0,0,320,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Avatar AWP03L                {"NAME":"AWP03L","GPIO":[0,0,0,0,0,0,0,0,224,32,320,321,0,0],"FLAG":0,"BASE":18}
Avatar AWP04L                {"NAME":"AWP04L","GPIO":[321,1,1,2624,1,2720,0,0,224,32,2656,320,1,0],"FLAG":0,"BASE":18}
Avatar AWP07L                {"NAME":"AWP07L","GPIO":[320,1,1,1,1,2720,1,1,2624,32,2656,224,1,0],"FLAG":0,"BASE":18}
Avatar AWP08L                {"NAME":"AWP08L","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Avatar AWP12L Capsule 2-in-1  {"NAME":"AWP12L","GPIO":[320,0,0,2624,33,2720,0,0,224,32,2656,225,0,0],"FLAG":0,"BASE":18}
Avatto 10A                   {"NAME":"Avatto NAS-WR01W 10A 2021-12","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Avatto JH-G01E               {"NAME":"AVATTO JH-G01E","GPIO":[0,3072,0,3104,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":41}
Avatto OT06 16A              {"NAME":"Avatto OT06","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
Avatto OT08                  {"NAME":"Avatto OT08","GPIO":[416,0,418,0,417,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Avidsen Home                 {"NAME":"Avidsen HomePlug","GPIO":[0,0,0,0,224,35,0,0,289,288,0,0,0,0],"FLAG":0,"BASE":18}
Awow X5P                     {"NAME":"Awow","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
AWP02L-N                     {"NAME":"AWP02L-N","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
AWP08L 20A                   {"NAME":"AWP08L","GPIO":[0,0,0,32,0,0,0,0,0,0,224,0,0,0],"FLAG":0,"BASE":18}
AzpenHome Smart              {"NAME":"Socket2Me","GPIO":[288,1,1,1,225,1,0,0,224,1,32,1,1,0],"FLAG":0,"BASE":18}
Baco Smart Power Socket      {"NAME":"Balco HE200021","GPIO":[0,0,0,32,2720,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":52}
Bagotte SK-EU-A01            {"NAME":"Bagotte SK-EU-A01","GPIO":[96,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Bakibo TP22Y                 {"NAME":"Bakibo TP22Y","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Bardi 16A                    {"NAME":"BARDI","GPIO":[320,0,0,0,0,2720,0,0,224,32,2656,321,2624,0],"FLAG":0,"BASE":18}
Bauhn ASPU-1019              {"NAME":"Bauhn Smart Pl","GPIO":[0,0,0,0,224,225,0,0,0,320,32,0,0,0],"FLAG":0,"BASE":18}
BAW                          {"NAME":"BAW TPSWIFI-10","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Bawoo                        {"NAME":"Bawoo S120","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Be HiTech 16A                {"NAME":"Be HiTech","GPIO":[0,0,0,288,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Bearware 303492 3AC+2USB     {"NAME":"Bearware 30349","GPIO":[0,320,0,32,225,226,0,0,227,224,544,0,0,0],"FLAG":0,"BASE":18}
Beghelli Dom-e Presa Smart IT  {"NAME":"Beghelli DOM-E Smart Plug","GPIO":[32,0,0,0,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Bestek MRJ1011               {"NAME":"BestekMRJ1011","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":1}
BlitzWolf 1200W Dual         {"NAME":"BlitzWolf SHP3","GPIO":[320,0,321,0,225,2720,0,0,2624,33,2656,224,32,0],"FLAG":0,"BASE":45}
BlitzWolf 16A Dual           {"NAME":"SHP7 v2","GPIO":[33,576,320,2624,2720,2656,0,0,32,321,224,0,225,0],"FLAG":0,"BASE":45}
BlitzWolf BW-SHP10-P         {"NAME":"BW-SHP10 PM","GPIO":[0,3200,0,5056,576,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf SHP1               {"NAME":"BlitzWolf-SHP1","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
BlitzWolf SHP10              {"NAME":"BW-SHP10","GPIO":[0,0,0,0,544,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Blitzwolf SHP11 16A          {"NAME":"BlitzWolf-SHP11","GPIO":[320,0,0,2624,0,2720,0,0,0,32,2656,224,0,0],"FLAG":0,"BASE":18}
BlitzWolf SHP2               {"NAME":"BlitzWolf SHP","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
BlitzWolf SHP3               {"NAME":"BlitzWolf SHP3","GPIO":[33,320,0,2624,2720,2656,0,0,32,0,225,224,0,0],"FLAG":0,"BASE":18}
BlitzWolf SHP4               {"NAME":"BlitzWolf SHP","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
BlitzWolf SHP5               {"NAME":"SHP5","GPIO":[321,3072,320,3104,0,225,0,0,0,0,224,0,32,0],"FLAG":0,"BASE":18}
BlitzWolf SHP6 10A           {"NAME":"BW-SHP6 10A","GPIO":[576,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
BlitzWolf SHP6 15A           {"NAME":"BlitzWolf SHP6","GPIO":[320,1,576,1,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Bluemyth 16A                 {"NAME":"SWA11","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
BN-Link                      {"NAME":"BNC-60/U133TJ","GPIO":[0,288,0,32,2720,2656,0,0,2624,289,224,0,0,0],"FLAG":0,"BASE":18}
BNETA IoT                    {"NAME":"BNETA WifiPlug","GPIO":[32,288,0,0,2688,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Bontec SPEU                  {"NAME":"Bontec SPEU","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Brennenstuhl Ecolor 3AC      {"NAME":"WS EC01 DE","GPIO":[0,0,0,0,32,576,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
Brennenstuhl WA 3000         {"NAME":"WA 3000 XS01","GPIO":[0,0,0,0,224,32,0,0,576,288,0,0,0,0],"FLAG":0,"BASE":61}
Bright                       {"NAME":"Bright Wi-Fi Smart Plug","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Brilliant                    {"NAME":"HK17654S05","GPIO":[32,1,1,1,2688,2656,1,1,2624,320,224,1,1,0],"FLAG":0,"BASE":18}
Brilliant Lighting           {"NAME":"BL20925","GPIO":[0,0,0,32,2688,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":52}
BrilliantSmart 20676 USB Charger  {"NAME":"Brilliant","GPIO":[0,0,0,0,0,224,0,0,0,288,64,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart Double Adaptor  {"NAME":"Brilliant_2Plg","GPIO":[0,0,0,288,224,225,1,1,544,32,33,0,0,0],"FLAG":0,"BASE":18}
BSD01                        {"NAME":"BSD01","GPIO":[1,320,1,1,1,1,0,0,224,32,321,1,1,0],"FLAG":0,"BASE":18}
BSD13                        {"NAME":"TuyaPlugBSD13","GPIO":[0,321,0,0,0,0,0,0,224,32,288,0,0,0],"FLAG":0,"BASE":18}
BSD15                        {"NAME":"TuyaPlugBSD15","GPIO":[1,1,1,1,1,1,0,0,224,32,320,1,1,4704],"FLAG":0,"BASE":18}
BSD25                        {"NAME":"Tuya Wifi Plug","GPIO":[1,1,1,1,320,1,0,0,224,32,1,1,1,0],"FLAG":0,"BASE":18}
BSD29                        {"NAME":"BSD29","GPIO":[0,0,0,2624,2720,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":52}
BSD33 10A                    {"NAME":"BSD33 type 2","GPIO":[0,0,320,0,0,0,0,0,1,32,1,224,1,0],"FLAG":0,"BASE":18}
BSD33 16A                    {"NAME":"Generic","GPIO":[0,1,0,2624,2720,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":18}
BSD34                        {"NAME":"BSD34 Plug","GPIO":[0,0,0,2624,2688,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":18}
BSD34-1-16A                  {"NAME":"BSD34-1 16A","GPIO":[0,289,0,2624,2720,2656,0,0,224,32,288,0,0,0],"FLAG":0,"BASE":18}
BSD48 16A                    {"NAME":"BSD48 Plug","GPIO":[0,288,0,0,0,0,0,0,224,32,321,0,0,0],"FLAG":0,"BASE":18}
Bytech                       {"NAME":"BYTECH B POWERED","GPIO":[0,321,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Calex 429198                 {"NAME":"Calex Pwr Plug","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":1}
Calex Powerplug UK           {"NAME":"Calex UK Plug","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":1}
Calex UK                     {"NAME":"Calex 429202","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,1],"FLAG":0,"BASE":18}
casa NOVA                    {"NAME":"casa-NOVA","GPIO":[0,0,0,0,320,576,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":1}
CE Smart Home                {"NAME":"LITESUN LA-WF7","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":18}
CE Smart Home LA-WF3         {"NAME":"CE LA-WF3","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
CloudFree EU                 {"NAME":"CloudFree P1EU","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Cloudfree Runs Tasmota       {"NAME":"CloudFree 1","GPIO":[1,1,320,1,1,1,0,0,1,32,1,224,1,0],"FLAG":0,"BASE":18}
CloudFree Smart Plug 2       {"NAME":"CloudFree P2","GPIO":[320,0,576,0,224,2720,0,0,2624,32,2656,0,0,0],"FLAG":0,"BASE":45}
Conico SM-PW70               {"NAME":"Conico SM-PW70","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,4704],"FLAG":0,"BASE":18}
Connect SmartHome Dual USB   {"NAME":"CSH-PLGUSB","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":52}
Connex                       {"NAME":"Connex Smart T","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Connex Type F                {"NAME":"Connex CC-P1001","GPIO":[0,0,320,0,0,2688,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Coolseer                     {"NAME":"Coolseer Plug (SK01WE)","GPIO":[0,3200,0,7520,0,32,0,0,0,544,224,0,0,0],"FLAG":0,"BASE":18}
Coosa                        {"NAME":"COOSA","GPIO":[0,0,0,0,321,288,0,0,224,32,1,0,0,0],"FLAG":0,"BASE":1}
Coosa SP1                    {"NAME":"COOSA SP1","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
CooWoo                       {"NAME":"CooWoo AW01","GPIO":[0,0,0,0,288,160,0,0,256,0,0,0,0,0],"FLAG":0,"BASE":18}
CozyLife HomeKit 16A         {"NAME":"CozyLife 16A","GPIO":[0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,2720,0,0,2656,576,0,224,2624,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
CrazyLynX WiFi               {"NAME":"CrazyLynX","GPIO":[0,0,0,0,321,320,0,0,224,32,0,0,0,4704],"FLAG":0,"BASE":18}
Crest Single Power Adaptor   {"NAME":"SHSPM1","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Crest Smart Home Single Power Adaptor with 2 USB  {"NAME":"Medion","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
CurrySmarter 16A             {"NAME":"CurrySmarter 16A","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
CurrySmarter 16A Power Monitoring  {"NAME":"Currysmarter XH-TW2P","GPIO":[0,0,0,2624,32,320,0,0,224,2720,2656,0,0,0],"FLAG":0,"BASE":18}
CYYLTF BIFANS J23            {"NAME":"CYYLTD BIFANS J23","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
CZ100 10A                    {"NAME":"ASZKJ","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
D3D Smart Plug with USB & Power Monitor  {"NAME":"D3D FLHS-ZN04","GPIO":[321,1,320,2624,1,2688,1,1,1,32,2656,224,1,1],"FLAG":0,"BASE":18}
Datto                        {"NAME":"Datto MP10","GPIO":[320,0,0,0,0,2720,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
DE-1 16A                     {"NAME":"DE-1","GPIO":[0,32,0,0,2688,0,0,0,0,288,224,0,0,4704],"FLAG":0,"BASE":18}
DeLock Power Socket Switch MQTT  {"NAME":"DeLock 11826","GPIO":[32,0,0,0,0,0,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":1}
Delock Power Socket Switch MQTT  {"NAME":"Delock 11827","GPIO":[0,0,0,32,2688,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":53}
Deltaco SH-P01               {"NAME":"DELTACO SH-P01","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Deltaco SH-P01E              {"NAME":"DELTACO SH-P01E","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":55}
Deltaco SH-P02               {"NAME":"Deltaco SH-P02","GPIO":[33,0,0,0,2720,2656,0,0,2624,320,224,225,32,0],"FLAG":0,"BASE":18}
Denver 16A                   {"NAME":"Denver SHP-100","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":65}
Denver Dual                  {"NAME":"Denver SHP-200","GPIO":[576,0,0,2624,32,2720,0,0,224,33,2656,225,0,0],"FLAG":0,"BASE":18}
Deta 6930HA                  {"NAME":"DetaSmartPlug","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
DETA Base                    {"NAME":"DetaPlugBase","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Digital LCD BL6523           {"NAME":"BL6523 Smart Meter","GPIO":[0,0,0,0,8000,8032,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Digma DiPlug 100S            {"NAME":"Digma DiPlug 100s","GPIO":[320,0,544,1,0,2720,1,1,1,32,2656,224,0,1],"FLAG":0,"BASE":18}
Digma DiPlug 160M            {"NAME":"DiPlug 160M","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":55}
Digoo DG-SP01                {"NAME":"DG-SP01","GPIO":[0,32,0,224,320,419,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Digoo NX-SP202               {"NAME":"Generic_SP202","GPIO":[288,0,0,2624,65,2720,0,0,225,32,2656,224,0,0],"FLAG":0,"BASE":63}
Dilisens NX-SP201 Mini 2 in 1  {"NAME":"NX-SP201","GPIO":[0,0,2624,0,2720,2656,288,224,33,225,32,0,0,0],"FLAG":0,"BASE":18}
DILISENS SP201               {"NAME":"Dilisens SP201","GPIO":[0,0,2624,0,2688,2656,288,224,33,225,32,0,0,0],"FLAG":0,"BASE":18}
Dunnes Stores                {"NAME":"SmartLifePlug","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Duvik 10A                    {"NAME":"Duvik M008","GPIO":[0,0,0,0,320,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
DWFeng AWP02L-N              {"NAME":"AWP02L-N","GPIO":[1,1,320,1,1,1,0,0,1,32,1,224,1,0],"FLAG":0,"BASE":18}
DWFeng BSD01                 {"NAME":"DWFeng BSD01","GPIO":[1,1,1,1,1,1,1,1,224,32,320,1,1,1],"FLAG":0,"BASE":18}
ECO Plugs CT-065W            {"NAME":"ECO/CT-065W","GPIO":[0,0,576,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
eco4life DPS1101S            {"NAME":"Eco4Life Plug","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Edisio 10A                   {"NAME":"Edisio Wifi Timer","GPIO":[1,1,1,1,1,1,1,1,1,1,32,1,320,1,1,1,1,1,1,1,1,1,224,1,1,1,1,1,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":1}
ednet 84334                  {"NAME":"84334","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
eFamilyCloud ASDFEE174       {"NAME":"eFamily Plug","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
EFUN SH330W                  {"NAME":"EFUNPlug","GPIO":[320,1,1,1,1,1,1,1,1,32,1,224,1,1],"FLAG":0,"BASE":18}
EFUN SH331W                  {"NAME":"Efun-Plug","GPIO":[320,0,576,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
EleLight                     {"NAME":"EleLight PE1004T","GPIO":[0,0,0,0,288,289,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
EletecPro 2                  {"NAME":"EletecPro-2","GPIO":[1,1,1,1,32,1,0,0,289,288,224,1,1,4704],"FLAG":0,"BASE":18}
Emil Lux Wifi-Stecker IP20   {"NAME":"Obi CH Plug","GPIO":[0,0,0,0,320,0,0,0,224,576,32,0,0,0],"FLAG":0,"BASE":18}
Emporia                      {"NAME":"Emporia EMS01","GPIO":[0,0,0,289,224,2720,0,0,2624,32,2656,288,0,0],"FLAG":0,"BASE":18}
Emporia 15A                  {"NAME":"Emporia EMS02","GPIO":[320,0,321,0,224,2720,0,0,2624,32,2656,0,0,0],"FLAG":0,"BASE":18}
Ener-J                       {"NAME":"ENER-J SHA5264","GPIO":[32,0,0,0,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Energeeks                    {"NAME":"EG-EW003MC","GPIO":[0,0,0,32,2720,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":52}
Energeeks 2 USB              {"NAME":"EG-EW005MC","GPIO":[544,1,288,1,1,225,1,1,2592,1,224,1,32,0],"FLAG":0,"BASE":18}
Energizer                    {"NAME":"Energizer EIE3-1001-WHT","GPIO":[32,0,448,0,2688,2656,0,0,2624,450,224,0,449,0],"FLAG":0,"BASE":18}
Energizer Smart Plug         {"NAME":"Energizer Smart Plug","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Epicka                       {"NAME":"Epicka","GPIO":[1,1,1,1,321,320,0,0,224,32,1,1,1,4704],"FLAG":0,"BASE":18}
Eques Elf Smart Plug         {"NAME":"EQP01WTGY","GPIO":[0,0,0,320,0,0,0,0,96,0,0,224,0,0],"FLAG":0,"BASE":18}
Esicoo JSM-WF02              {"NAME":"Esicoo Plug","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Estink C178                  {"NAME":"Estink C178","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Etekcity 8A                  {"NAME":"ESW01-USA","GPIO":[0,0,0,0,224,544,0,0,2656,2688,32,2592,288,0],"FLAG":0,"BASE":55}
ETEKCITY Voltson Smart Wi-Fi Outlet  {"NAME":"ESW15-USA","GPIO":[0,0,0,0,416,224,0,0,2656,2688,32,2592,544,4704],"FLAG":0,"BASE":18}
EU3S                         {"NAME":"AWOW BSD33","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Eva Logik                    {"NAME":"EVA LOGIK Plug","GPIO":[1,32,1,1,1,1,0,0,1,288,224,1,1,0],"FLAG":0,"BASE":18}
Ewelink EU 20A               {"NAME":"Ewelink Smart Socket 20A","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Febite                       {"NAME":"Febite","GPIO":[320,0,0,0,0,2720,0,0,224,32,2656,0,0,0],"FLAG":0,"BASE":1}
Feit Electric PLUG/WIFI      {"NAME":"Feit Wifi Plug","GPIO":[0,0,0,320,0,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
FK-PW901U                    {"NAME":"FK-PW901U","GPIO":[320,1,1,1,1,226,0,0,224,32,227,225,1,0],"FLAG":0,"BASE":18}
FLHS-ZN04                    {"NAME":"FLHS-ZN04","GPIO":[321,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Floureon                     {"NAME":"FLOUREON","GPIO":[0,0,0,0,320,0,1,1,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Fontastic 16A                {"NAME":"P22-2036563-DZB117435","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Fontastic SH01               {"NAME":"Fontastic","GPIO":[1,0,1,0,320,0,0,0,224,32,0,0,0,4704],"FLAG":0,"BASE":18}
Foreet                       {"NAME":"Foreet_120V","GPIO":[0,0,0,0,224,32,0,0,289,288,0,0,0,0],"FLAG":0,"BASE":18}
Fox&Summit FS-IP12PK         {"NAME":"Fox & Summit Plug","GPIO":[0,0,32,0,3840,0,0,0,544,256,480,0,0,0],"FLAG":0,"BASE":18,"CMND":"BuzzerPwm 1"}
FrameXX IO 16A               {"NAME":"FrameXX FXIOWR16","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
FrankEver 15A                {"NAME":"Israel plug","GPIO":[321,0,320,2624,0,2720,0,0,0,32,2656,224,0,0],"FLAG":0,"BASE":45}
FrankEver 16A                {"NAME":"FrankEver FK-PW801ER","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
FrankEver Mini               {"NAME":"FK-PW801US","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Frankever USB                {"NAME":"FK-PW301U","GPIO":[0,0,1,0,576,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
GDTech                       {"NAME":"GDTech Model: MPV2RO-US","GPIO":[320,0,0,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
GDTech W-US001               {"NAME":"GDTech W-US001","GPIO":[1,32,1,1,1,1,0,0,1,320,224,1,1,4704],"FLAG":0,"BASE":18}
GDTech W-US003               {"NAME":"W-US003","GPIO":[0,32,1,1,1,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Geekbes YM-WS-1              {"NAME":"Office Test Pl","GPIO":[1,1,1,1,1,1,1,1,576,32,163,224,1,1],"FLAG":0,"BASE":18}
Geekome Enchufe Inteligente Chile  {"NAME":"Geekhome PG01-CL10A_T","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":1}
Geeni OUTDOOR                {"NAME":"Geeni Outdoor","GPIO":[32,0,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Geeni Spot                   {"NAME":"Geeni Spot","GPIO":[576,0,0,0,320,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Geeni Spot Glo               {"NAME":"Geeni Glo","GPIO":[0,0,0,0,320,0,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Geeni SWITCH                 {"NAME":"Geeni Switch","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Geeni Switch Duo             {"NAME":"Geeni Duo","GPIO":[0,0,0,0,33,225,0,0,32,288,224,0,289,0],"FLAG":0,"BASE":18}
GHome EP2-A                  {"NAME":"GHome EP2-A","GPIO":[576,0,320,0,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Girier 16A                   {"NAME":"Girier - JR-PM01","GPIO":[0,544,0,2624,2720,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":68}
Globe 15A                    {"NAME":"Globe 50329","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Globe 2 Outlet               {"NAME":"Globe 50020","GPIO":[0,576,0,321,33,32,0,0,224,320,225,0,0,0],"FLAG":0,"BASE":18}
Globe Smart                  {"NAME":"GlobeSmartPlug","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
GoldenDot Mini               {"NAME":"GoldenDot Mini","GPIO":[0,32,0,0,0,0,0,0,0,321,224,0,0,0],"FLAG":0,"BASE":52}
GoldenDot with ADC           {"NAME":"W-US003-Power","GPIO":[320,0,0,0,0,0,0,0,0,32,0,224,0,4896],"FLAG":0,"BASE":18}
Goliath 16A                  {"NAME":"GOLIATH AV-SSTE01","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Gosund                       {"NAME":"Gosund EP2","GPIO":[576,1,320,1,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Gosund                       {"NAME":"Gosund SP111","GPIO":[320,0,321,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Gosund                       {"NAME":"SHP5","GPIO":[321,3072,320,3104,1,225,0,0,1,1,224,1,32,0],"FLAG":0,"BASE":18}
Gosund 13A                   {"NAME":"Gosund UP111","GPIO":[0,320,0,32,2720,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":18}
Gosund 2 in 1                {"NAME":"Gosund WP212","GPIO":[321,288,544,0,224,2720,0,0,2624,32,2656,225,33,0],"FLAG":0,"BASE":18}
Gosund Dual                  {"NAME":"Gosund SP211","GPIO":[33,576,320,2624,2720,2656,0,0,32,321,224,0,225,0],"FLAG":0,"BASE":18}
Gosund SP1                   {"NAME":"Gosund SP1 v23","GPIO":[0,321,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":55}
Gosund SP111 v1.1            {"NAME":"Gosund SP111 v1.1","GPIO":[320,0,576,0,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Gosund SP111 v1.4            {"NAME":"Gosund SP111 v1.4","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Gosund SP112                 {"NAME":"Gosund 112v3.4","GPIO":[320,0,321,0,2656,2720,0,0,2624,257,224,0,0,4800],"FLAG":0,"BASE":18}
Gosund WP1                   {"NAME":"Gosund WP1-1","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Gosund WP2                   {"NAME":"Gosund WP2 Plug","GPIO":[33,0,320,0,321,32,0,0,224,576,225,0,0,0],"FLAG":0,"BASE":18}
Gosund WP211                 {"NAME":"Gosund wp211","GPIO":[0,0,0,0,224,0,0,0,0,32,0,225,33,0],"FLAG":0,"BASE":18}
Gosund WP212                 {"NAME":"Gosund_WP212","GPIO":[32,544,321,0,33,0,0,0,225,320,224,0,0,0],"FLAG":0,"BASE":18}
Gosund WP3                   {"NAME":"Gosund WP3 v2","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Gosund WP5                   {"NAME":"Gosund-WP5","GPIO":[0,0,0,0,32,0,0,0,320,321,224,0,0,0],"FLAG":0,"BASE":18}
Gosund WP6                   {"NAME":"Gosund WP6","GPIO":[0,320,0,32,0,0,0,0,0,544,224,0,0,0],"FLAG":0,"BASE":18}
Grefic TE101                 {"NAME":"Grefic TE101","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":1}
GSP1311 Series               {"NAME":"GSP1311","GPIO":[321,0,320,0,0,2720,0,0,2624,32,2656,224,0,1],"FLAG":0,"BASE":18}
Gyman SM-PW701U              {"NAME":"Gyman","GPIO":[1,1,544,1,320,1,0,0,224,32,1,1,1,0],"FLAG":0,"BASE":18}
Hama 16A                     {"NAME":"Hama Plug","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":52}
Hama 16A                     {"NAME":"Hama 00176562","GPIO":[0,0,0,0,32,288,0,0,0,289,224,0,0,0],"FLAG":0,"BASE":18}
Hama 16A 3680W               {"NAME":"Hama Plug","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Hama Basic 10A               {"NAME":"Hama 176567","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Hama Mini                    {"NAME":"HAMA 00176573","GPIO":[0,0,0,32,0,0,0,0,0,576,224,0,0,0],"FLAG":0,"BASE":18}
Hama Mini                    {"NAME":"HAMA 00176575","GPIO":[0,0,0,32,0,2720,0,0,0,576,224,0,0,0],"FLAG":0,"BASE":18}
HaoDeng                      {"NAME":"HaoDeng","GPIO":[0,0,0,0,224,32,0,0,320,289,0,0,0,0],"FLAG":0,"BASE":61}
Hauppauge 01647              {"NAME":"SL-1642","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HBN 13A                      {"NAME":"BNC-50/E75T","GPIO":[0,0,0,0,576,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HBN BNC-60/U152T             {"NAME":"BNC-60/U152T","GPIO":[0,0,0,0,320,0,1,1,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HerePow                      {"NAME":"HerePow ZHX-ZNOC","GPIO":[0,544,0,2624,2720,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":68}
Heygo 02                     {"NAME":"Heygo 02","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HiHome                       {"NAME":"HIhome WPP-16S","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
HiHome                       {"NAME":"HiHome WPP-10S","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
HiHome                       {"NAME":"HIhome WPP-10S","GPIO":[320,0,576,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":49}
HiHome                       {"NAME":"HiHome WPP-16T","GPIO":[32,320,1,1,2720,2656,0,0,33,1,225,2592,224,4704],"FLAG":0,"BASE":18}
HIPER IoT P01                {"NAME":"HIPER IoT P01","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
HIPER IoT P05                {"NAME":"HIPER IoT P05","GPIO":[0,320,0,32,0,0,0,0,0,224,0,0,0,0],"FLAG":0,"BASE":18}
HIPER IoT P09                {"NAME":"HIPER IoT P09","GPIO":[32,0,0,0,0,320,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
hiwild W-US002               {"NAME":"W-US002","GPIO":[0,32,0,0,0,0,0,0,0,288,224,0,576,0],"FLAG":0,"BASE":18}
HLT-309                      {"NAME":"HLT-309","GPIO":[0,0,0,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Hoin 10A                     {"NAME":"NIOH XS-SSC01","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Hombli Smart Socket EU       {"NAME":"Hombli HBSS-0109","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Hombli Socket Duo            {"NAME":"Hombli HBSD-0109","GPIO":[33,0,0,0,0,0,0,0,0,544,224,225,32,0],"FLAG":0,"BASE":18}
Homecube                     {"NAME":"Homecube SP1","GPIO":[0,321,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":55}
HomeMate 16A Heavy Duty      {"NAME":"HMLPG16","GPIO":[0,288,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Houzetek                     {"NAME":"AWP07L","GPIO":[320,0,0,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
HS108                        {"NAME":"HS108","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,4704],"FLAG":0,"BASE":18}
HS108A                       {"NAME":"HS108A","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,4704],"FLAG":0,"BASE":18}
HuaFan QinLu                 {"NAME":"Huafan SS","GPIO":[320,0,0,321,32,256,0,0,2656,2592,2688,0,0,0],"FLAG":0,"BASE":24}
Hykker SE-0792               {"NAME":"Hykker SE-0792","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Hyleton 312                  {"NAME":"MANZOKU HLT312","GPIO":[0,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Hyleton 313                  {"NAME":"Hyleton 313","GPIO":[321,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Hyleton 314                  {"NAME":"hyleton-314","GPIO":[321,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Hyleton 315                  {"NAME":"hyleton-315","GPIO":[0,0,0,0,321,320,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
Hyleton 317                  {"NAME":"hyleton-317","GPIO":[320,0,321,0,322,0,0,0,0,64,0,224,0,0],"FLAG":0,"BASE":18}
Hyleton HLT-311              {"NAME":"HLT-311","GPIO":[544,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Hyrican SmartPlug 16A        {"NAME":"Hyrican TM-MP-EU02","GPIO":[0,0,0,32,2656,2624,0,0,224,2720,320,0,0,0],"FLAG":0,"BASE":18}
iClever IC-BS08              {"NAME":"iClever BS08","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
iDIGITAL                     {"NAME":"Brilliant","GPIO":[0,0,0,0,288,0,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
iGET Security                {"NAME":"iGET Security DP16","GPIO":[320,1,576,1,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Ihommate 16A                 {"NAME":"ZCH-02","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,4704],"FLAG":0,"BASE":18}
Infray 16A                   {"NAME":"AWP08L","GPIO":[32,0,288,0,0,0,0,0,0,0,0,224,0,4704],"FLAG":0,"BASE":18}
Insmart                      {"NAME":"INSMART SP1","GPIO":[320,1,321,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Insmart WP5                  {"NAME":"INSMART","GPIO":[0,0,448,0,0,0,0,0,0,160,0,224,0,0],"FLAG":0,"BASE":18}
Intempo Home Euro 2-Pin      {"NAME":"Intempo EE5010WHTSTKEU","GPIO":[0,0,0,32,0,2720,0,0,0,576,224,0,0,0],"FLAG":0,"BASE":18}
iQtech SmartLife             {"NAME":"iQ-Tech WS020","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
iSwitch                      {"NAME":"Smart Plug XSA","GPIO":[1,32,1,1,1,1,0,0,1,320,224,1,1,0],"FLAG":0,"BASE":18}
Itinftek                     {"NAME":"ITINFTEK SP1","GPIO":[0,320,1,2624,2720,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":18}
ITINFTEK                     {"NAME":"ZEU-003","GPIO":[0,1,0,2624,2720,2656,0,0,224,32,320,0,0,0],"FLAG":0,"BASE":18}
J15                          {"NAME":"J15","GPIO":[0,0,0,0,544,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Jeeo TF-SH330                {"NAME":"Jeeo TF-SH330","GPIO":[320,0,0,0,0,0,0,0,0,32,0,224,0,4704],"FLAG":0,"BASE":18}
Jeeo TF-SH331W               {"NAME":"Jeeo SH331W","GPIO":[320,0,576,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Jetstream MSP150             {"NAME":"JetstreamMSP150","GPIO":[320,0,321,0,224,2720,0,0,2624,32,2656,0,0,0],"FLAG":0,"BASE":45}
JH-G01B1                     {"NAME":"JH-G01B1","GPIO":[0,3072,0,3104,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":41}
JH-G01U                      {"NAME":"JH-G01U","GPIO":[0,0,0,0,224,32,0,0,288,0,0,0,0,0],"FLAG":0,"BASE":18}
Jinli XS-SSA01               {"NAME":"JINLI","GPIO":[0,0,0,0,0,0,0,0,320,32,0,224,0,0],"FLAG":0,"BASE":18}
Jinvoo SM-PW701U             {"NAME":"SM-PW702","GPIO":[0,0,0,0,321,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Jinvoo SM-PW712UA 10A        {"NAME":"SM-PW712UA","GPIO":[0,0,0,576,320,321,0,0,225,32,224,0,0,1],"FLAG":0,"BASE":18}
Jinvoo SM-PW762U             {"NAME":"SM-PW762U","GPIO":[0,0,0,0,576,320,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Jomarto SH0617               {"NAME":"Jomarto SH0617","GPIO":[321,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Jomarto SH1123               {"NAME":"SH1123","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Jula Anslut                  {"NAME":"Anslut 026174","GPIO":[0,3200,0,7520,0,32,0,0,224,0,320,0,0,0],"FLAG":0,"BASE":18}
Jules V (Upgrade Version)    {"NAME":"Jules-V_UV","GPIO":[320,0,0,0,0,2720,0,0,224,32,2656,321,2624,0],"FLAG":0,"BASE":18}
JULES.V NX-SM200 V1.3        {"NAME":"NX-SM200","GPIO":[288,0,0,2624,0,2720,0,0,0,32,2656,224,0,4704],"FLAG":0,"BASE":45}
Jules.V NX-SM300             {"NAME":"NX-SM300","GPIO":[288,0,0,0,0,2592,0,0,224,2656,2688,288,0,0],"FLAG":0,"BASE":6}
Jules.V NX-SP203             {"NAME":"NX-SP203","GPIO":[288,0,0,0,32,2720,1,1,224,33,0,225,0,0],"FLAG":0,"BASE":18}
JuoYou 16A                   {"NAME":"Juoyou ZY-OYD","GPIO":[0,0,0,32,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
JVMAC-EU01                   {"NAME":"JVMAC","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Kaforto KW-US-801            {"NAME":"Kaforto US-801","GPIO":[32,576,0,227,2720,2656,0,0,2624,225,224,226,0,0],"FLAG":0,"BASE":18}
Kauf esphome                 {"NAME":"KAUF Plug","GPIO":[576,0,320,0,224,2720,0,0,2624,32,2656,0,0,0],"FLAG":0,"BASE":18}
Kauf esphome                 {"NAME":"Kauf Plug","GPIO":[0,320,0,32,2720,2656,0,0,321,224,2624,0,0,0],"FLAG":0,"BASE":18}
Kimire S12                   {"NAME":"Kimire S12","GPIO":[1,1,1,32,1,1,0,0,1,320,224,1,1,0],"FLAG":0,"BASE":18}
King-Link KL-US-WF002        {"NAME":"Kinglink-plug","GPIO":[0,0,0,0,0,224,0,0,288,32,0,0,0,0],"FLAG":0,"BASE":18}
Kisslink SP200               {"NAME":"Kisslink SP200","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,4704],"FLAG":0,"BASE":18}
KMC                          {"NAME":"KMC Plug","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
KMC 4AC                      {"NAME":"KMC 4 Outlet","GPIO":[0,320,0,0,2688,2656,0,0,2592,225,226,224,32,0],"FLAG":0,"BASE":36}
KMC 70011                    {"NAME":"KMC 70011","GPIO":[32,0,0,0,2688,2656,0,0,2592,320,224,0,0,0],"FLAG":0,"BASE":36}
Kogan Energy Meter           {"NAME":"Kogan Smart Sw","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Kogan Energy Meter & USB     {"NAME":"KoganPOW","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Konlen USB                   {"NAME":"KONLEN","GPIO":[0,0,0,0,419,416,0,0,418,32,417,224,0,0],"FLAG":0,"BASE":18}
Konyks Priska EU             {"NAME":"Konyks Priska EU","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Konyks Priska Mini 10A       {"NAME":"Konyks 10A","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Konyks Priska+ Mini 16A      {"NAME":"Konyks PRISKA+ MINI","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
Koogeek                      {"NAME":"Koogeek KLSP1","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Koogeek                      {"NAME":"Koogeek-KLUP1","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Koogeek KLSP1                {"NAME":"Koogeek-KLSP1","GPIO":[0,320,0,32,2720,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":18}
Koogeek KLSP2                {"NAME":"KOOGEEK KLSP2","GPIO":[321,3072,320,3104,0,225,0,0,0,0,224,0,32,0],"FLAG":0,"BASE":45}
Koogeek KLWP1                {"NAME":"Koogeek-KLWP1","GPIO":[544,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":1}
Koogeek W-DEXI               {"NAME":"W-DEXI","GPIO":[0,64,0,0,2720,2656,0,0,2592,288,224,0,0,0],"FLAG":0,"BASE":18}
Koogeek W-UKX                {"NAME":"Koogeek W-UKX","GPIO":[0,32,1,1,1,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":1}
Kruger & Matz Connect P2     {"NAME":"KM2200","GPIO":[0,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Kruidvat Smart Dual          {"NAME":"Kruidvat Dual Plug","GPIO":[3840,0,0,0,32,0,0,0,224,33,0,225,0,0],"FLAG":0,"BASE":1}
KULED K63                    {"NAME":"KULED K63","GPIO":[0,0,0,0,224,32,0,0,320,0,0,0,0,0],"FLAG":0,"BASE":18}
Laduo YX-DE01                {"NAME":"YX-DE01","GPIO":[1,32,1,1,1,1,0,0,1,320,224,1,1,0],"FLAG":0,"BASE":18}
LE LampUX Smart Socket       {"NAME":"LE LampUX","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Ledvance Smart+              {"NAME":"Ledvance Plug","GPIO":[0,0,0,320,2688,2656,0,0,224,32,2624,0,0,0],"FLAG":0,"BASE":18}
Ledvance Smart+ CH           {"NAME":"Ledvance Plug CH","GPIO":[0,0,0,288,32,0,0,0,2656,224,2720,0,0,0],"FLAG":0,"BASE":18}
Lenovo SE-341A               {"NAME":"Lenovo SE-341A","GPIO":[0,0,0,0,32,224,0,0,576,0,320,0,0,0],"FLAG":0,"BASE":18}
Lenovo SE-341AA              {"NAME":"Lenovo SE-341AC","GPIO":[0,0,0,0,32,224,0,0,576,0,320,0,0,0],"FLAG":0,"BASE":18}
Lenovo SE-341AC              {"NAME":"Lenovo SE-341AC","GPIO":[0,321,0,32,2720,2656,0,0,320,224,2624,0,0,0],"FLAG":0,"BASE":18}
LESHP KS-501                 {"NAME":"LESHP KS-501","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Lighting Arena               {"NAME":"Lighting Arena Smart Plug","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Lloyd's                      {"NAME":"Lloyds LC-1193","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Local Bytes (UK)             {"NAME":"LocalBytes PM","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
LogiLink                     {"NAME":"LogilinkPA0199","GPIO":[0,0,0,64,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
LogiLink PA0200              {"NAME":"LogilinkPA0200","GPIO":[0,0,0,64,0,0,0,0,0,320,224,0,0,4896],"FLAG":0,"BASE":18}
Lohas Nightlight + USB       {"NAME":"Lohas LED Mini Plug","GPIO":[0,321,0,288,322,226,1,1,224,32,225,0,544,0],"FLAG":0,"BASE":18}
Lombex Actis Pro             {"NAME":"U11-Socket","GPIO":[0,0,0,0,289,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Lonsonho 10A Type E          {"NAME":"Lonsonho10ALed","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
LoraTap SP400W-IT            {"NAME":"LoraTap SP400W","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,1],"FLAG":0,"BASE":18}
LSC Power                    {"NAME":"LSC Smart Plug (CB2S)","GPIO":[0,0,0,0,32,224,0,0,0,288,289,0,0,0],"FLAG":0,"BASE":18}
LSC Smart Connect            {"NAME":"LSC Smart Plug FR","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
LSC Smart Power Plug         {"NAME":"LSC Smart Power Plug with Energy Monitoring","GPIO":[0,2624,1,288,32,544,1,1,2656,224,2720,1,1,1],"FLAG":0,"BASE":18}
Lumiman LM650                {"NAME":"Lumiman LM650","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Luminea                      {"NAME":"CH-1556","GPIO":[0,0,0,32,2720,2656,1,1,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Luminea NX-4491              {"NAME":"Luminea NX-449","GPIO":[320,0,576,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Luminea NX-4541              {"NAME":"NX-4451","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":55}
Luminea ZX-2820              {"NAME":"ZX2820-675","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":65}
Luminea ZX-2858              {"NAME":"ZX2858-675","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":65}
Lunvon 2000W                 {"NAME":"Lunvon Smart Plug","GPIO":[32,0,0,0,0,0,288,224,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Lunvon 3000W                 {"NAME":"Lunvon Smart Plug","GPIO":[32,0,0,0,0,0,288,224,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Marmitek Power SI            {"NAME":"Marmitek PowerSi","GPIO":[0,0,0,32,2720,0,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
Martin Jerry V01             {"NAME":"MJ V01","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Martin Jerry XS-SSA01        {"NAME":"MJ_XS-SSA01","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Maxcio 16A Mini              {"NAME":"MAXCIO RMC021","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":1}
Maxcio W-UK007S              {"NAME":"Maxcio","GPIO":[320,0,1,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Maxcio W-US002S              {"NAME":"W-US002S","GPIO":[0,32,0,544,2688,2656,0,0,2592,288,224,0,0,0],"FLAG":0,"BASE":18}
Maxcio W-US003               {"NAME":"W-US003","GPIO":[1,32,1,1,1,1,0,0,1,225,224,1,1,0],"FLAG":0,"BASE":18}
Maxcio YX-DE02               {"NAME":"Maxcio DE02","GPIO":[0,32,0,224,320,225,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Maxcio YX-DE04               {"NAME":"Maxcio YX-DE04","GPIO":[1,32,1,224,320,419,0,0,416,417,418,1,1,4704],"FLAG":0,"BASE":18}
MaxKare XKJJ-0218            {"NAME":"MaxKare XKJJ-0","GPIO":[0,0,0,0,33,2688,0,0,224,32,2656,225,2592,0],"FLAG":0,"BASE":18}
Maxus Brio Head 16A          {"NAME":"Brio-W-Head16","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Medion Life+ S85225          {"NAME":"Medion","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
MELINK Smart Socket          {"NAME":"MeLink ML1SSO20","GPIO":[0,32,0,32,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Merkury Innovations 4 Outlets  {"NAME":"MI-WW119-199W","GPIO":[320,3200,0,3232,225,32,0,0,224,226,227,0,0,0],"FLAG":0,"BASE":18}
Merkury MI-WW101-199         {"NAME":"merkury WW101","GPIO":[0,0,0,0,0,0,0,0,320,64,0,224,0,0],"FLAG":0,"BASE":18}
Merkury MI-WW102-199L        {"NAME":"MIC-WW102","GPIO":[32,0,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":36}
Merkury MI-WW105-199W        {"NAME":"Merkury Switch","GPIO":[1,1,1,1,576,320,0,0,224,32,1,1,1,4704],"FLAG":0,"BASE":18}
Mini                         {"NAME":"MiniSmartSocket_S03","GPIO":[0,0,0,0,320,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Mini Smart                   {"NAME":"W-US002S","GPIO":[0,32,0,0,2720,0,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":45}
Minleaf W-DEXI               {"NAME":"W-DEXI","GPIO":[0,32,0,0,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio              {"NAME":"GenioGpo","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 1002341      {"NAME":"Genio 1","GPIO":[0,0,320,0,0,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":1}
Mirabella Genio Double Plug with USB Ports  {"NAME":"Genio I002932","GPIO":[0,320,1,32,0,0,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio Dual USB     {"NAME":"Mirabella Genio Wi-Fi Power Plug with Dual USB Port","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio USB          {"NAME":"Mirabella Genio 1002826","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":1}
Mirabella Genio USB Port     {"NAME":"Genio I002341","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":1}
Mistral                      {"NAME":"Mistral Smart ","GPIO":[320,0,0,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Moes 16A                     {"NAME":"MOES WS-UEU16-M","GPIO":[0,3072,0,3104,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":41}
Moes NX-SP203                {"NAME":"Moes NX-SP203","GPIO":[288,0,0,0,32,2720,0,0,224,33,0,225,0,0],"FLAG":0,"BASE":18}
Moes W-DE004S                {"NAME":"Moes DE004S ","GPIO":[321,0,0,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Moes WS-UEU                  {"NAME":"MoesHouse","GPIO":[0,0,0,224,32,0,0,0,321,0,0,0,0,0],"FLAG":0,"BASE":18}
MoKo 2 USB                   {"NAME":"MoKo Plug","GPIO":[0,32,0,0,0,0,0,0,0,321,224,0,0,0],"FLAG":0,"BASE":18}
MoKo YX-WS01A                {"NAME":"MoKo Plug","GPIO":[0,32,0,0,0,0,0,0,0,321,224,0,0,0],"FLAG":0,"BASE":18}
Muvit iO MIOSMP008           {"NAME":"MIOSMP008","GPIO":[0,0,0,32,2656,2624,0,0,224,2720,320,0,0,0],"FLAG":0,"BASE":1}
MXQ LED Nightlight           {"NAME":"MXQ SP06","GPIO":[0,0,0,0,288,192,0,0,225,321,224,0,0,0],"FLAG":0,"BASE":18}
Nanxin                       {"NAME":"NX-SM400","GPIO":[0,0,0,32,2720,2656,0,0,2592,288,224,0,0,0],"FLAG":0,"BASE":18}
Naxa NSH-1000                {"NAME":"Naxa NSH-1000","GPIO":[0,0,0,0,32,0,0,0,321,320,224,0,0,0],"FLAG":0,"BASE":18}
Nedis P110                   {"NAME":"Nedis WIFIP110","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
Nedis P120                   {"NAME":"WIFIP120EWT","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
Nedis P130                   {"NAME":"WIFIP130FWT","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Nedis Schuko Type F 16A      {"NAME":"WIFIP120FWT","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
NEO Coolcam 10A              {"NAME":"Neo Coolcam 10","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
NEO Coolcam 16A              {"NAME":"NEO Coolcam NAS-WR07W-16A","GPIO":[32,0,0,0,2720,2656,1,1,2624,320,224,0,0,1],"FLAG":0,"BASE":18}
NEO Coolcam NAS-WR01W        {"NAME":"NAS-WR01W","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
NEO Coolcam NAS-WR01W 16A    {"NAME":"Neo Coolcam 16","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
Nestler-matho                {"NAME":"N-m 485-1","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
NETVIP XS-SSA01              {"NAME":"XS-SSA01","GPIO":[0,0,0,0,0,0,0,0,320,32,0,224,0,0],"FLAG":0,"BASE":18}
Nexxt Surge Protector 220V Chile  {"NAME":"Nexxt Smart Plug","GPIO":[0,0,0,0,320,1,0,0,32,544,224,0,0,0],"FLAG":0,"BASE":18}
NGS Loop Track 16A           {"NAME":"LOOP","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Nightlight and AC Outlet     {"NAME":"SWN03","GPIO":[32,0,0,0,0,0,1,1,416,0,0,224,0,0],"FLAG":0,"BASE":18}
Nishica SM-PW701I            {"NAME":"SM-PW701I","GPIO":[1,1,1,1,1,1,1,1,224,288,32,1,1,1],"FLAG":0,"BASE":18}
Nivian                       {"NAME":"Nivian Smart Socket","GPIO":[0,0,320,0,0,2688,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Nous A1                      {"NAME":"NOUS A1","GPIO":[320,0,576,0,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Nous A1T 16A                 {"NAME":"NOUS A1T","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
NX-SM112                     {"NAME":"NX-SM112v3","GPIO":[0,0,0,0,2720,2656,0,0,576,32,2592,224,0,0],"FLAG":0,"BASE":45}
NX-SM200                     {"NAME":"NX-SM200","GPIO":[320,0,0,0,0,2720,0,0,224,32,2656,321,2624,0],"FLAG":0,"BASE":18}
NX-SM210                     {"NAME":"NX-SM210","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
NX-SM223                     {"NAME":"Smart Thurmm","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Oakter Oak Plug Plus 16A     {"NAME":"Oakter OakPlug Plus","GPIO":[0,0,0,0,224,0,0,0,544,320,0,0,0,0],"FLAG":0,"BASE":18}
Oakter OakPlug Mini 10A      {"NAME":"Oakter OakPlug Mini","GPIO":[0,0,0,0,224,0,0,0,544,320,0,0,0,0],"FLAG":0,"BASE":18}
Oakter OakPlug Plus (old)    {"NAME":"Oakter OakPlug Plus (old)","GPIO":[0,0,0,0,224,0,0,0,0,320,0,0,544,0],"FLAG":0,"BASE":18}
Obi Stecker                  {"NAME":"Euromate","GPIO":[1,1,1,1,288,224,1,1,289,1,32,1,1,1],"FLAG":0,"BASE":18}
Obi Stecker 2                {"NAME":"OBI Socket 2","GPIO":[0,0,0,0,224,32,0,0,320,289,0,0,0,0],"FLAG":0,"BASE":61}
OFFONG 16A                   {"NAME":"OFFONG P1","GPIO":[0,32,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Oittm Smart                  {"NAME":"Oittm","GPIO":[0,0,0,0,224,320,0,0,32,0,0,0,0,0],"FLAG":0,"BASE":1}
Olliwon                      {"NAME":"Olliwon","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Onearz Connect Smart         {"NAME":"Onearz Power Plug Wifi","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Oneplug                      {"NAME":"ONEPLUG UP111","GPIO":[321,0,0,0,2656,2720,0,0,2624,0,224,0,32,0],"FLAG":0,"BASE":18}
Onestyle SD-WL-02            {"NAME":"JH-G01B1","GPIO":[0,3072,0,3104,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":41}
Orbecco W-US009              {"NAME":"Orbecco Plug","GPIO":[0,32,0,0,0,0,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
Orvibo B25                   {"NAME":"Orvibo B25","GPIO":[0,0,0,0,289,224,0,0,288,0,32,0,0,0],"FLAG":0,"BASE":18}
Oukitel P1                   {"NAME":"Oukitel P1Dual","GPIO":[288,1,0,1,0,0,0,0,225,32,0,224,0,0],"FLAG":0,"BASE":18}
Oukitel P2                   {"NAME":"Oukitel P2","GPIO":[0,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Oukitel X6P                  {"NAME":"OUKITEL X6P","GPIO":[0,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
OxaOxe NX-SM200              {"NAME":"NX-SM200","GPIO":[32,0,0,0,2688,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":18}
Oxaoxe NX-SM800              {"NAME":"NX-SM800","GPIO":[0,0,0,2624,0,2720,0,0,224,32,2656,0,0,0],"FLAG":0,"BASE":45}
OxaOxe NX-SP202              {"NAME":"oxaoxe-dual","GPIO":[33,0,0,0,2720,2656,0,0,2624,320,224,225,32,0],"FLAG":0,"BASE":18}
OxaOxe NX-SP202 v2           {"NAME":"oxaoxe-dold","GPIO":[320,0,0,2624,32,2720,0,0,224,33,2656,225,0,0],"FLAG":0,"BASE":18}
OZWI Smart                   {"NAME":"OZWI Smart Plug","GPIO":[0,0,0,0,288,0,0,0,224,32,544,0,0,0],"FLAG":0,"BASE":18}
Panamalar Nightlight         {"NAME":"Panamalar EWN0","GPIO":[32,0,0,0,0,0,1,1,416,0,0,224,0,0],"FLAG":0,"BASE":18}
Panamalar NX-SM200           {"NAME":"NX-SM200","GPIO":[0,0,0,0,320,2720,0,0,2624,32,2656,224,0,4704],"FLAG":0,"BASE":18}
Polycam Hohm Lanre 16A       {"NAME":"SLV1910001","GPIO":[0,0,0,32,2720,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":18}
Positivo PPW1000             {"NAME":"PPW1000","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Positivo Max                 {"NAME":"PPW1600","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":55}
PowerAdd BIE0091             {"NAME":"BIE0091","GPIO":[32,0,0,0,0,0,0,0,416,0,0,224,0,0],"FLAG":0,"BASE":18}
Powertech                    {"NAME":"Jaycar MS6104","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Powertech                    {"NAME":"Jaycar","GPIO":[320,0,0,0,0,0,0,0,0,160,0,224,0,0],"FLAG":0,"BASE":6}
Powrui 3-Outlet with 4 USB   {"NAME":"POWRUI AHR-077","GPIO":[0,0,0,35,34,33,0,0,225,226,32,224,544,0],"FLAG":0,"BASE":18}
Powrui AW-08                 {"NAME":"POWRUI AW-08","GPIO":[0,0,0,0,32,224,0,0,0,288,321,0,0,4704],"FLAG":0,"BASE":18}
Premier PWIFPLG              {"NAME":"Premier Plug","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,4704],"FLAG":0,"BASE":18}
Prime CCRCWFII113PK          {"NAME":"Prime","GPIO":[0,0,0,0,321,320,0,0,224,96,0,0,0,0],"FLAG":0,"BASE":18}
Prime RCWFII11               {"NAME":"Prime Plug","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
PrimeCables                  {"NAME":"CAB-LA-WF4","GPIO":[0,0,0,320,321,0,0,0,0,0,0,224,32,0],"FLAG":0,"BASE":18}
PrimeCables 2 USB Mini       {"NAME":"CAB-LA-WF4-G2","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Prokord                      {"NAME":"PSH-WS007-EU","GPIO":[0,0,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Prokord 16A                  {"NAME":"PROKORD PSH-WS021-EU","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
QNCX                         {"NAME":"QNCX","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Qnect 16A                    {"NAME":"Qnect QN-WP01","GPIO":[0,0,0,0,0,224,0,0,288,0,32,0,0,0],"FLAG":0,"BASE":18}
qnect 16A                    {"NAME":"QNECT QN-WP01E","GPIO":[0,0,0,32,2688,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Qualitel Mini                {"NAME":"Qualitel HG01WT","GPIO":[320,0,321,0,0,2688,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Refoss P11                   {"NAME":"Refoss-P11","GPIO":[0,0,0,3104,576,32,0,0,320,224,0,0,0,4736],"FLAG":0,"BASE":1}
RenPho RF-SM004              {"NAME":"RenPho RFSM004","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Robaxo                       {"NAME":"Robaxo RSP-025","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
RSH-WS005                    {"NAME":"RSH-WS005 Energy Monitor","GPIO":[320,0,576,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
RSH-WS007-EU                 {"NAME":"RSH-WS007","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
RSmart                       {"NAME":"RSmart Plug","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
S-Link Swapp 16A             {"NAME":"S-Link S01","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
S126                         {"NAME":"tuya-plug","GPIO":[0,0,0,0,0,0,0,0,224,35,0,0,0,0],"FLAG":0,"BASE":8}
SA-001                       {"NAME":"SA-001","GPIO":[32,3072,0,3104,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":41}
SA-P202A                     {"NAME":"SA-P202A","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
SA-P202C 16A                 {"NAME":"Elivco 202C-G","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
SA-P302A                     {"NAME":"KinCam SA-P302A","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Sansui                       {"NAME":"Sansui YSP-1","GPIO":[288,0,289,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Sansui Rewireable            {"NAME":"YSP-2","GPIO":[288,0,289,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
See Switches                 {"NAME":"SEESWITCHES SSPG01WH","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Setti+                       {"NAME":"Setti+ SP301","GPIO":[320,0,576,0,2656,2720,0,0,2624,32,0,224,0,0],"FLAG":0,"BASE":45}
Shelly Plug                  {"NAME":"Shelly Plug EU","GPIO":[0,0,0,0,224,2688,0,0,96,288,289,0,290,0],"FLAG":0,"BASE":18}
Shelly Plug S                {"NAME":"Shelly Plug S","GPIO":[320,1,576,1,1,2720,0,0,2624,32,2656,224,1,4736],"FLAG":0,"BASE":45}
Shelly Plug US               {"NAME":"Shelly Plug US","GPIO":[288,0,321,0,224,2720,0,0,2624,32,2656,544,0,0],"FLAG":0,"BASE":45}
Shelly Plus Plug S           {"NAME":"Shelly Plus Plug S","GPIO":[0,0,0,0,224,0,32,2720,0,0,0,0,0,0,0,2624,0,0,2656,0,0,288,289,0,0,0,0,0,0,4736,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sieges                       {"NAME":"Sieges FLHS-ZN01","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
SilentNight                  {"NAME":"SilentNightPlug","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Silvergear Slimme Stekker    {"NAME":"Silvergear SmartHomePlug","GPIO":[0,0,0,96,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
SimpleHome                   {"NAME":"SimpleHome","GPIO":[289,0,0,0,0,0,0,0,224,320,32,0,0,0],"FLAG":0,"BASE":1}
SimpleHome 2 USB             {"NAME":"SimpleHome","GPIO":[289,0,0,0,2656,2624,0,0,224,2688,32,0,0,0],"FLAG":0,"BASE":1}
Slitinto Mini 2 in 1         {"NAME":"slitinto NX-SP201","GPIO":[0,0,0,2624,32,2720,0,0,224,33,2656,225,0,0],"FLAG":0,"BASE":18}
Slitinto NX-SM110            {"NAME":"Slitinto SM110","GPIO":[0,0,320,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Slitinto NX-SM112            {"NAME":"NX-SM112","GPIO":[576,0,0,2624,0,2720,0,0,0,32,2656,224,0,0],"FLAG":0,"BASE":45}
Slitinto NX-SM112 v2         {"NAME":"Slitinto NX SM","GPIO":[320,1,576,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Slitinto NX-SM200            {"NAME":"NX-SM200","GPIO":[0,0,0,32,2720,2656,0,0,0,288,224,2592,0,0],"FLAG":0,"BASE":45}
Slitinto NX-SM2001           {"NAME":"NX-SM2001","GPIO":[0,0,0,32,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":45}
Slitinto NX-SP202            {"NAME":"Slitinto SP202","GPIO":[32,0,0,0,2720,2656,0,0,2624,288,225,224,65,0],"FLAG":0,"BASE":64}
SM-PW701K                    {"NAME":"SM-PW701K","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Smaho                        {"NAME":"SMAHO WiFi P.","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Smart 15A                    {"NAME":"JH-G09U","GPIO":[0,0,0,0,224,32,0,0,288,0,0,0,0,0],"FLAG":0,"BASE":18}
Smart Plug                   {"NAME":"SO08WP","GPIO":[1,1,1,32,2688,2656,1,1,2624,288,224,1,1,1],"FLAG":0,"BASE":18}
SmartDGM PP-W162             {"NAME":"SmartDGM Plug","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
SmartGrade                   {"NAME":"SmartGrade AC","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
SmartVU Home                 {"NAME":"SHWSP1","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Smitch 16A                   {"NAME":"Smitch SP0602","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Smitch 6A                    {"NAME":"Smitch SP0601","GPIO":[321,1,320,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Sonoff S20                   {"NAME":"Sonoff S20","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":8}
Sonoff S22 TH                {"NAME":"Sonoff S22","GPIO":[32,1,0,1,1,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":4}
Sonoff S26                   {"NAME":"Sonoff S26","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":8}
Sonoff S26 R2                {"NAME":"Sonoff S26R2","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":8}
Sonoff S31                   {"NAME":"Sonoff S31","GPIO":[32,3072,0,3104,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":41}
Sonoff S31 Lite              {"NAME":"Sonoff S31","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":41}
SP10                         {"NAME":"Bilikay SP10","GPIO":[0,416,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
SP111                        {"NAME":"Enjowi SP111-E","GPIO":[32,320,0,0,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":55}
SP201 Dual                   {"NAME":"SP-201","GPIO":[258,0,0,2624,32,2720,0,0,224,33,2656,225,0,0],"FLAG":0,"BASE":45}
SPARKE JH-G01E 10A           {"NAME":"SPARKE JH-G01E","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
SPC Clever 6201B             {"NAME":"SPC Clever Plu","GPIO":[32,0,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
SS01                         {"NAME":"Smart Plug SS0","GPIO":[1,1,1,1,1,1,0,0,224,32,1,1,1,4704],"FLAG":0,"BASE":18}
Steren                       {"NAME":"SHOME-100 V1.0","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Steren                       {"NAME":"SHOME-135","GPIO":[0,288,0,32,2720,2656,0,0,2624,289,224,0,0,0],"FLAG":0,"BASE":18}
STITCH                       {"NAME":"Stitch 27937","GPIO":[32,0,320,0,2688,2656,0,0,2624,0,224,0,321,0],"FLAG":0,"BASE":18}
STITCH                       {"NAME":"Stitch 35511","GPIO":[320,0,321,0,0,2688,0,0,0,32,2656,224,2624,0],"FLAG":0,"BASE":18}
STITCH 15A In-Line           {"NAME":"Stitch 39047","GPIO":[0,288,0,32,2688,2656,0,0,2624,0,224,0,0,0],"FLAG":0,"BASE":18}
STITCH Mini 10A              {"NAME":"STITCH 41730","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Strong Helo EU               {"NAME":"Strong-Helo-PLUG","GPIO":[0,0,0,32,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":65}
Strong Helo Plight           {"NAME":"Strong HELO-PLIGHT-EU","GPIO":[416,0,418,0,417,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":18}
Strong Helo PLUSB 2x USB     {"NAME":"Helo-PLUSB-EU","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,225,8096,0],"FLAG":0,"BASE":18}
SuperNight Dual              {"NAME":"SUPERNIGHT","GPIO":[0,32,0,224,2656,2688,0,0,225,2624,576,0,0,4833],"FLAG":0,"BASE":18}
SWA1                         {"NAME":"SWA1","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
SWA1 FR                      {"NAME":"SWA1","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
SWA1 UK                      {"NAME":"SWA1","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
SWA11 16A                    {"NAME":"SWA11","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
SWA5                         {"NAME":"Linganzh SWA5","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
SWA9                         {"NAME":"SWA9","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Swisstone                    {"NAME":"SwissTone","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Sygonix SY-4276902           {"NAME":"SYGONIX","GPIO":[0,0,0,32,2688,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Sygonix SY-4416210           {"NAME":"Sygonix_sy_rs_201","GPIO":[0,0,0,0,0,0,0,0,0,288,224,0,32,0],"FLAG":0,"BASE":18}
Szshining                    {"NAME":"Szshining ITSP10-2","GPIO":[0,225,0,32,2720,2656,0,0,2624,0,224,0,0,0],"FLAG":0,"BASE":18}
Takida Smart Power           {"NAME":"Takida Smart Plug","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Talo 10A                     {"NAME":"Talo smart plug","GPIO":[32,0,0,0,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":49}
TanTan WP2                   {"NAME":"TanTan WP2","GPIO":[321,320,576,1,224,2720,1,1,2624,32,2656,225,33,1],"FLAG":0,"BASE":18}
TanTan WP3                   {"NAME":"TanTan WP3","GPIO":[0,0,320,0,32,0,0,0,321,0,224,0,0,0],"FLAG":0,"BASE":18}
TCP Smart 13A                {"NAME":"TCP_Plug","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":45}
Teckin SP10                  {"NAME":"Teckin SP10","GPIO":[1,1,320,1,1,1,0,0,1,32,1,224,1,0],"FLAG":0,"BASE":18}
Teckin SP20                  {"NAME":"TECKIN SP20","GPIO":[320,1,321,1,224,2720,0,0,2624,32,2656,0,0,0],"FLAG":0,"BASE":45}
Teckin SP21                  {"NAME":"Teckin SP21","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":45}
Teckin SP22                  {"NAME":"Teckin","GPIO":[0,32,0,321,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":52}
Teckin SP23                  {"NAME":"Teckin SP23","GPIO":[320,1,321,1,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Teckin SP25                  {"NAME":"Teckin SP25","GPIO":[320,1,1,1,1,1,0,0,225,32,1,224,1,4704],"FLAG":0,"BASE":18}
Teckin SP27                  {"NAME":"Teckin SP27","GPIO":[320,1,1,1,1,1,0,0,1,32,1,224,1,0],"FLAG":0,"BASE":18}
Tellur 16A 2 Ports           {"NAME":"Tellur WiFi Smart Socket","GPIO":[0,0,0,2624,96,2688,0,0,224,33,2656,225,0,0],"FLAG":0,"BASE":18}
Tellur 1USB 10A              {"NAME":"Tellur TTL331021","GPIO":[0,0,544,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Tflag NX-SM100               {"NAME":"NX-SM100","GPIO":[320,0,0,0,0,2720,0,0,224,32,2656,321,2624,0],"FLAG":0,"BASE":18}
TH3D EZPlug V1               {"NAME":"EZPlug V1","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":1}
TH3D EZPlug+ V1              {"NAME":"EZPlug+ V1","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":1}
TikLok TL650                 {"NAME":"TikLok Mini","GPIO":[0,0,0,0,321,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Timethinker C338             {"NAME":"C338","GPIO":[32,0,1,0,0,0,0,0,224,288,1,0,0,0],"FLAG":0,"BASE":1}
Timethinker TK04             {"NAME":"TimethinkerEU","GPIO":[1,1,1,1,32,1,0,0,1,288,224,1,0,0],"FLAG":0,"BASE":18}
TimeThinker WS2              {"NAME":"TimeThinkerWS2","GPIO":[0,0,0,0,32,0,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
Tomada KaBuM! Smart          {"NAME":"TOMADA KABUM! SMART","GPIO":[0,0,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
TomaxUSA                     {"NAME":"HKWL-SO07W","GPIO":[32,1,1,1,1,1,0,0,1,1,224,1,320,0],"FLAG":0,"BASE":18}
Topersun                     {"NAME":"Topersun ","GPIO":[0,0,0,0,288,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
TopGreener                   {"NAME":"TGWF115PRM","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":18}
TopGreener 10A               {"NAME":"TGWF115PQM","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":18}
TopGreener TGWF115APM        {"NAME":"TGWF115APM","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":18}
Torchstar LITEdge Smart      {"NAME":"LITEdge Plug","GPIO":[0,32,0,0,0,0,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
TOVA Dual                    {"NAME":"TOVA","GPIO":[320,0,0,2624,33,2720,0,0,0,32,2656,225,224,0],"FLAG":0,"BASE":45}
TP20                         {"NAME":"TP20","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
TP24                         {"NAME":"TP24","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
TP28Y                        {"NAME":"TP28Y","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Treatlife Dimmable           {"NAME":"DP20","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 21,2 | SO20 1 | SO54 1"}
Treatlife Smart              {"NAME":"Treatlife SK50","GPIO":[1,1,1,1,320,576,1,1,224,1,32,1,1,1],"FLAG":0,"BASE":18}
Tuya 16A Nightlight          {"NAME":"Nightlight","GPIO":[225,0,320,0,226,227,0,0,34,64,0,224,0,0],"FLAG":0,"BASE":18}
Tuya 20A                     {"NAME":"Tuya 20A Power Monitoring Plug","GPIO":[1,1,1,32,2720,224,1,1,1,320,289,1,1,1],"FLAG":0,"BASE":18}
U10 Series                   {"NAME":"WIFI-Socket","GPIO":[1,32,1,1,1,1,1,1,1,320,224,1,1,4704],"FLAG":0,"BASE":18}
Ucomen Night Light           {"NAME":"UCOMEN Plug","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
UltraBrite                   {"NAME":"UltraBrite Smart Plug","GPIO":[1,1,1,1,288,289,1,1,224,32,1,1,1,1],"FLAG":0,"BASE":18}
Ultralink UL-P01W            {"NAME":"UL-P01W","GPIO":[0,288,0,32,2720,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Umax U-Smart Duo             {"NAME":"Umax Duo Plug","GPIO":[33,0,0,0,2688,2656,0,0,2624,544,224,225,32,0],"FLAG":0,"BASE":18}
Unlocked Automation 15A      {"NAME":"UA 100","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,1],"FLAG":0,"BASE":18}
UP111                        {"NAME":"UP111","GPIO":[0,576,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
Upstone                      {"NAME":"UPSTONE","GPIO":[1,1,544,1,320,1,0,0,224,32,1,1,1,1],"FLAG":0,"BASE":18}
US212                        {"NAME":"US212","GPIO":[320,0,0,2624,0,2720,0,0,224,32,2656,225,0,0],"FLAG":0,"BASE":18}
Varna Crafts 16A             {"NAME":"VC Plug","GPIO":[544,0,0,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
Vaupan X6P                   {"NAME":"Vaupan 10a X6P","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Vettora Smart Plug           {"NAME":"Vettora","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Vingo                        {"NAME":"Karpal-01","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Vivanco 39625 Smart Home Power Adapter  {"NAME":"Vivianco","GPIO":[0,0,0,32,2688,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Vivitar                      {"NAME":"Vivitar HA1003","GPIO":[576,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Vivitar                      {"NAME":"Vivitar HA-1004","GPIO":[320,0,321,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Vivitar HA-1006              {"NAME":"HA-1006","GPIO":[0,0,0,0,320,0,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
Vivitar HA-1006-AU           {"NAME":"HA-1006-AU","GPIO":[0,0,0,0,320,0,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
Vivitar Plug                 {"NAME":"HA-1005N-AU","GPIO":[0,0,0,0,544,0,0,0,224,64,0,0,0,0],"FLAG":0,"BASE":18}
Vizia 16A                    {"NAME":"Vizia 16A","GPIO":[32,0,0,0,2688,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
W-DE004                      {"NAME":"W-DE004","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,4704],"FLAG":0,"BASE":18}
WAGA life CHCZ02MB           {"NAME":"WAGA CHCZ02MB","GPIO":[321,0,0,2624,0,2720,0,0,224,32,2656,320,0,0],"FLAG":0,"BASE":68}
WAZA 10A                     {"NAME":"WAZA","GPIO":[0,0,0,0,224,32,0,0,320,0,0,0,0,4704],"FLAG":0,"BASE":18}
WAZA JH-G01B                 {"NAME":"Teckin SP27","GPIO":[1,1,1,1,289,1,0,0,224,32,1,1,1,0],"FLAG":0,"BASE":18}
WAZA JH-G01E 10A             {"NAME":"Waza JH-G01E","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,4704],"FLAG":0,"BASE":18}
Waza JH-G01E 16A             {"NAME":"Waza JH-G01E 2","GPIO":[0,0,0,0,0,0,0,0,32,288,224,0,0,4704],"FLAG":0,"BASE":18}
wesmartify essentials 10A    {"NAME":"wesmartify 120111","GPIO":[0,0,0,0,320,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Wily Electronics             {"NAME":"VC Plug","GPIO":[544,0,0,0,0,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
WiOn                         {"NAME":"WiOn","GPIO":[1,0,288,0,0,0,0,0,1,32,0,224,0,0],"FLAG":0,"BASE":17}
WiOn Single Outlet           {"NAME":"WiOn 50050","GPIO":[0,0,576,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Wipro 16A                    {"NAME":"Wip-DSP1160","GPIO":[0,0,0,32,2720,2656,0,0,2624,576,224,0,0,0],"FLAG":0,"BASE":52}
Wisdom Dual                  {"NAME":"ZY-ACU02","GPIO":[0,0,0,544,225,224,0,0,320,32,321,0,0,0],"FLAG":0,"BASE":18}
WiZ 10A                      {"NAME":"WIZ 9290024276","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
WL-SC01                      {"NAME":"WL-SC01","GPIO":[0,0,0,0,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":1}
Woox EU                      {"NAME":"WOOX R6113","GPIO":[0,2624,0,320,32,289,0,0,2656,224,2720,0,0,0],"FLAG":0,"BASE":18}
WOOX R4026                   {"NAME":"WOOX R4026","GPIO":[0,0,0,32,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
WOOX R4785                   {"NAME":"WOOXR4785","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
WOOX R5024                   {"NAME":"WOOX5024","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
WP211                        {"NAME":"YUNTAB WP211","GPIO":[320,0,576,0,225,0,0,0,0,33,0,224,32,0],"FLAG":0,"BASE":18}
WP5                          {"NAME":"WP5","GPIO":[321,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Wsiiroon                     {"NAME":"Wsiiroon Smart Plug","GPIO":[0,0,0,0,32,320,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Wyze                         {"NAME":"WyzePlugWLPP1","GPIO":[0,0,0,0,0,320,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Wyze                         {"NAME":"Wyze Plug ESP32","GPIO":[0,0,0,0,0,576,0,0,0,0,0,32,224,0,0,320,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Xenon                        {"NAME":"SM-PW702","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Xenon 10A                    {"NAME":"Cooligg","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Xiaomi IMILAB                {"NAME":"Mi Smart Plug","GPIO":[32,0,0,0,320,224,0,0,0,576,0,0,0,0],"FLAG":0,"BASE":18}
XS-A11                       {"NAME":"THRUMM XS-A11","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
XS-A12                       {"NAME":"XS-A12","GPIO":[416,0,418,0,417,2720,0,0,2624,32,2656,224,0,0],"FLAG":0,"BASE":45}
XS-A14                       {"NAME":"NETVIP XS-A14","GPIO":[416,0,417,0,0,32,0,0,418,224,0,0,0,0],"FLAG":0,"BASE":18}
XS-A17                       {"NAME":"XS-A18","GPIO":[416,0,417,0,0,418,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":45}
XS-A18                       {"NAME":"XS-A18","GPIO":[416,0,417,0,0,418,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":45}
XS-A23                       {"NAME":"XS-A23","GPIO":[320,1,0,2624,32,2720,0,0,0,33,2656,224,225,0],"FLAG":0,"BASE":45}
XS-SSA01                     {"NAME":"XS-SSA01","GPIO":[1,0,0,1,0,0,0,0,320,32,1,224,1,0],"FLAG":0,"BASE":18}
XS-SSA01 v2                  {"NAME":"XS-SSA01","GPIO":[1,32,1,1,1,1,0,0,320,1,1,224,1,0],"FLAG":0,"BASE":18}
XS-SSA05                     {"NAME":"XS-SSA05","GPIO":[320,0,0,2624,544,2688,0,0,224,32,2656,0,0,4704],"FLAG":0,"BASE":18}
XS-SSA06                     {"NAME":"XS-SSA06","GPIO":[416,0,417,0,0,418,0,0,0,64,0,224,0,0],"FLAG":0,"BASE":18}
Yagala SWA9                  {"NAME":"SWA9","GPIO":[0,0,0,0,288,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Yelomin JH-G01E              {"NAME":"Yelomin","GPIO":[0,3072,0,3104,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":18}
YERON US101                  {"NAME":"YERON_US101","GPIO":[1,1,1,32,2688,2656,0,0,2624,320,224,1,1,0],"FLAG":0,"BASE":18}
YM-WS-1 Mini                 {"NAME":"YM-WS-1-ALT","GPIO":[32,33,34,35,0,0,0,0,0,288,224,0,0,0],"FLAG":0,"BASE":18}
YM-WS-3 16A                  {"NAME":"YM-WS-3","GPIO":[0,32,0,0,0,0,0,0,0,288,224,0,576,4704],"FLAG":0,"BASE":18}
YT-E002                      {"NAME":"YT-E002","GPIO":[576,0,0,0,33,0,0,0,225,32,0,224,0,0],"FLAG":0,"BASE":18}
YT-E003                      {"NAME":"YT-E003-SP202","GPIO":[32,0,0,0,2720,2656,0,0,2624,288,225,224,65,0],"FLAG":0,"BASE":64}
Yuanguo KS-501               {"NAME":"Yuanguo_KS-501","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
YX-DE01                      {"NAME":"YX-DE01","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
YX-WS01                      {"NAME":"Generic","GPIO":[1,32,1,1,1,1,1,1,1,320,224,1,1,1],"FLAG":0,"BASE":18}
YX-WS02                      {"NAME":"Amysen YX-WS02","GPIO":[1,32,1,1,1,1,0,0,1,320,224,1,1,4704],"FLAG":0,"BASE":18}
ZBR-001                      {"NAME":"ZBR-001","GPIO":[32,1,1,1,2688,2656,0,0,2592,320,224,1,321,4704],"FLAG":0,"BASE":18}
ZettaGuard                   {"NAME":"ZettaGuard S25","GPIO":[0,0,0,0,289,224,0,0,288,0,32,0,0,0],"FLAG":0,"BASE":18}
Zhilde Dual USB              {"NAME":"ZLD-12USA-W","GPIO":[0,0,0,0,320,321,0,0,224,225,32,0,0,0],"FLAG":0,"BASE":18}
ZooZee SA101                 {"NAME":"ZooZee","GPIO":[321,1,320,1,1,1,0,0,1,32,1,224,1,0],"FLAG":0,"BASE":18}
ZooZee SA102                 {"NAME":"ZooZee","GPIO":[321,1,320,1,224,2688,0,0,2624,32,2656,1,1,0],"FLAG":0,"BASE":18}
ZooZee SE131                 {"NAME":"ZOOZEE_SF131","GPIO":[0,0,320,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
ZSP-001                      {"NAME":"ZSP-001","GPIO":[32,1,1,1,2688,2656,0,0,2592,321,224,1,320,4704],"FLAG":0,"BASE":18}
```

## Power Strip
```
3AC 4USB                     {"NAME":"C723","GPIO":[0,0,320,0,0,224,0,0,226,320,32,227,225,0],"FLAG":0,"BASE":18}
4 AC Outlets 10A and 4 USB Ports  {"NAME":"SA-P802 Power Strip","GPIO":[544,0,0,0,227,228,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
A0F0 ZLD-44EU-W              {"NAME":"AOFO-4AC-4USB","GPIO":[0,320,0,32,225,224,0,0,226,227,260,0,0,4704],"FLAG":0,"BASE":18}
Acenx 3AC+3USB               {"NAME":"ACENX 3-Outlet","GPIO":[320,291,290,289,0,224,0,0,226,227,225,0,32,0],"FLAG":0,"BASE":18}
AHRise 4+4AC+4USB            {"NAME":"AHRise-083","GPIO":[0,0,0,0,320,32,0,0,225,224,226,227,0,0],"FLAG":0,"BASE":18}
AHRise AHR-085               {"NAME":"AHRise AHR-085","GPIO":[0,0,0,0,32,320,0,0,225,226,224,0,0,0],"FLAG":0,"BASE":18}
Aicliv 4AC 3USB              {"NAME":"Aicliv WiFi","GPIO":[0,0,0,227,226,576,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
Ailink 4AC 4USB              {"NAME":"Ailink","GPIO":[288,321,0,289,256,32,0,0,258,257,259,0,228,0],"FLAG":0,"BASE":4}
Anhaorui 6+6AC 4USB          {"NAME":"Anhaorui ZN-709","GPIO":[320,0,0,160,224,225,0,0,228,229,227,226,0,0],"FLAG":0,"BASE":18,"CMND":"SwitchMode1 5|SO32 10"}
Anncoe 4AC 4USB              {"NAME":"P-EU4U4S","GPIO":[288,0,0,0,226,227,0,0,0,0,225,224,0,0],"FLAG":0,"BASE":18}
Annhome 3AC + 2USB           {"NAME":"1200W WiFi SPS","GPIO":[259,0,0,0,321,288,0,0,224,32,225,226,260,0],"FLAG":0,"BASE":18}
AOFO 3AC+4USB                {"NAME":"AOFO","GPIO":[0,320,0,32,225,224,0,0,0,226,227,0,0,4704],"FLAG":0,"BASE":18}
AOFO 4AC + 4USB              {"NAME":"AOFO C379 4AC+4USB UK ","GPIO":[0,576,0,32,227,226,0,0,224,228,225,0,0,0],"FLAG":0,"BASE":18}
AOFO 4AC 4USB                {"NAME":"AOFO4AC4USB-EU","GPIO":[0,320,0,32,228,227,0,0,225,224,226,0,0,0],"FLAG":0,"BASE":18}
AOFO 4AC+4USB Tuya           {"NAME":"AOFO-4AC-4USB","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,4704],"FLAG":0,"BASE":54}
AOFO 4AC+4USB UK             {"NAME":"AOFO4AC4USB-UK","GPIO":[0,320,0,32,226,227,0,0,225,224,260,0,0,0],"FLAG":0,"BASE":18}
Arlec 4 Outlet               {"NAME":"Arlec PB4KSHA Powerboard","GPIO":[0,32,1,1,1,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":12}
Arlec Smart                  {"NAME":"Arlec PB88UHA","GPIO":[0,320,0,32,225,224,0,0,227,226,0,0,0,1],"FLAG":0,"BASE":18}
Arlec Smart                  {"NAME":"Arlec PB89HA","GPIO":[0,320,0,32,225,224,0,0,227,226,0,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 3 Outlet Power Cube  {"NAME":"Arlec Cube","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
Bauhn ASPBU-1019             {"NAME":"Bauhn 3AC+3USB","GPIO":[0,544,0,0,225,224,0,0,0,226,32,0,0,0],"FLAG":0,"BASE":18}
Bix 3AC 4USB                 {"NAME":"Bix BP-01","GPIO":[259,0,0,0,0,289,0,0,224,35,225,226,288,0],"FLAG":0,"BASE":18}
BlitzWolf BW-SHP9            {"NAME":"BlitzWolf SHP9","GPIO":[576,1,0,1,0,226,0,0,224,32,225,227,0,0],"FLAG":0,"BASE":45}
Brennenstuhl Connect Eco-Line  {"NAME":"WS EL01 DE","GPIO":[34,33,0,32,224,225,0,0,288,0,0,289,576,0],"FLAG":0,"BASE":18}
Brennenstuhl Connect Premium-Line  {"NAME":"WS PL01 DE","GPIO":[34,33,0,32,224,225,0,0,288,0,35,289,576,0],"FLAG":0,"BASE":18}
BrilliantSmart Powerboard with USB Chargers  {"NAME":"B_WiFi-4","GPIO":[320,0,0,321,256,32,0,0,258,257,259,0,228,4704],"FLAG":0,"BASE":18}
Calex 4AC 2USB               {"NAME":"Calex Power Strip 429228","GPIO":[0,320,0,36,225,224,0,0,226,227,228,0,0,0],"FLAG":0,"BASE":18}
Calex 4AC 4USB               {"NAME":"Calex Power Strip 429228","GPIO":[0,0,0,288,224,226,0,0,0,228,227,225,32,0],"FLAG":0,"BASE":18}
CE Smart Home                {"NAME":"CE Power Strip","GPIO":[288,0,0,0,227,228,0,0,225,226,224,0,32,0],"FLAG":0,"BASE":18}
CE Smart Home Garden Stake   {"NAME":"CE Power Stake","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
CRST LTS-4G-W                {"NAME":"CRST LTS-4G-W","GPIO":[544,0,0,0,227,32,0,0,225,226,224,0,0,0],"FLAG":0,"BASE":18}
Curv 4 Plug                  {"NAME":"CURV","GPIO":[0,32,0,228,0,0,1,1,227,225,226,224,0,1],"FLAG":0,"BASE":18}
Deltaco SH-P03USB            {"NAME":"Deltaco SH-P03","GPIO":[320,0,0,0,0,226,0,0,224,32,225,227,0,0],"FLAG":0,"BASE":18}
Digma DiPlug Strip 40        {"NAME":"DiPlug Strip 40","GPIO":[1,320,1,32,225,224,1,1,226,227,260,1,1,1],"FLAG":0,"BASE":18}
Digoo DG-PS01                {"NAME":"Digoo DG-PS01","GPIO":[0,320,0,32,226,225,0,0,0,227,224,0,0,4704],"FLAG":0,"BASE":18}
Ener-J Extension Box with USB  {"NAME":"ENER-J SHA5207","GPIO":[544,0,0,321,225,322,1,1,320,224,227,226,32,1],"FLAG":0,"BASE":18}
Forrinx 3AC 4USB             {"NAME":"SH-20","GPIO":[0,320,0,321,225,224,0,0,32,226,227,0,0,0],"FLAG":0,"BASE":18}
Geekbes 3AC 3USB             {"NAME":"GC801U","GPIO":[291,0,0,320,256,35,0,0,258,257,0,0,227,0],"FLAG":0,"BASE":18}
Geekbes 4AC+4USB             {"NAME":"Geekbes 4xStri","GPIO":[0,320,0,32,225,224,0,0,226,227,228,0,0,4704],"FLAG":0,"BASE":18}
Geeni Surge + Charge 2 USB   {"NAME":"Geeni GN-SW023","GPIO":[288,0,0,0,225,224,0,0,226,227,228,0,32,0],"FLAG":0,"BASE":18}
Geeni SURGE 6-Outlet Surge Protector  {"NAME":"Geeni 6 Strip","GPIO":[320,0,0,0,225,224,0,0,227,228,226,229,32,0],"FLAG":0,"BASE":18}
Geeni SURGE 6-Outlet Surge Protector  {"NAME":"Geeni GNC-SW003-199","GPIO":[320,0,0,32,226,225,0,0,228,229,227,224,0,0],"FLAG":0,"BASE":18}
Geeni Surge Mini             {"NAME":"Geeni-GN-SW004","GPIO":[320,0,0,32,0,0,0,0,225,224,226,0,0,1],"FLAG":0,"BASE":18}
Globe 4 Outlet 2 USB         {"NAME":"PowerBar","GPIO":[320,0,0,0,227,32,0,0,225,226,224,0,0,1],"FLAG":0,"BASE":18}
Gosund 3+3AC 3USB            {"NAME":"Gosund-PM03-T","GPIO":[0,0,576,0,0,259,0,0,224,225,226,0,32,0],"FLAG":0,"BASE":18}
Gosund P1                    {"NAME":"Gosund_P1","GPIO":[0,3072,544,3104,0,259,0,0,225,226,224,0,35,4704],"FLAG":0,"BASE":18}
Gosund P2                    {"NAME":"Gosund-PM03-v2","GPIO":[0,0,576,0,0,0,0,0,224,225,226,259,32,0],"FLAG":0,"BASE":18}
Gosund WP9                   {"NAME":"Gosund WP9","GPIO":[320,291,290,289,0,224,0,0,226,227,225,0,32,0],"FLAG":0,"BASE":18}
Hama 4AC 4USB                {"NAME":"HamaStrip","GPIO":[0,544,0,32,225,224,0,0,226,227,228,0,0,0],"FLAG":0,"BASE":18}
Heyvalue 3AC+3USB            {"NAME":"HeyvalueHLT-330","GPIO":[288,0,289,0,227,256,0,0,257,35,258,0,0,0],"FLAG":0,"BASE":18}
Heyvalue 4AC+4USB            {"NAME":"Heyvalue HLT-331","GPIO":[288,1,1,321,256,32,0,0,258,257,259,1,228,0],"FLAG":0,"BASE":18}
HIPER IoT PS44               {"NAME":"HIPER IoT PS44","GPIO":[0,320,0,32,225,224,0,0,226,227,228,0,0,0],"FLAG":0,"BASE":18}
HLT-333                      {"NAME":"BEYAWL","GPIO":[0,0,320,227,0,0,0,0,0,257,256,258,0,0],"FLAG":0,"BASE":18}
Home Awesome 4AC 4USB        {"NAME":"Home Awesome","GPIO":[288,0,289,0,228,225,0,0,227,32,226,224,0,0],"FLAG":0,"BASE":18}
Houzetek 4AC 3USB            {"NAME":"SA-P402A Houze","GPIO":[0,32,0,320,226,228,0,0,224,227,225,0,0,4704],"FLAG":0,"BASE":18}
Hyleton 330                  {"NAME":"Hyleton-330","GPIO":[321,0,0,320,256,32,0,0,258,257,259,0,228,0],"FLAG":0,"BASE":18}
Hyleton 331                  {"NAME":"HLT-331","GPIO":[288,1,1,321,256,32,0,0,258,257,259,1,322,0],"FLAG":0,"BASE":18}
Hyleton 333                  {"NAME":"HLT-333","GPIO":[288,0,0,321,256,32,0,0,258,257,0,0,227,0],"FLAG":0,"BASE":18}
Hyleton 336                  {"NAME":"HLT-336","GPIO":[288,0,0,321,256,32,0,0,258,257,0,0,259,0],"FLAG":0,"BASE":18}
Idinio 3AC 4USB              {"NAME":"Idinio-140135","GPIO":[0,320,0,32,225,224,0,0,0,226,227,0,0,0],"FLAG":0,"BASE":18}
Jinvoo 4AC+2USB              {"NAME":"JINVOO Model SM-SO306-2A","GPIO":[320,0,0,0,259,32,0,0,257,258,256,0,228,0],"FLAG":0,"BASE":18}
KaBuM! Filtro de Linha       {"NAME":"Kabum Power Strip","GPIO":[0,544,0,32,227,228,0,0,0,225,226,224,320,0],"FLAG":0,"BASE":18}
KMC 5AC 3USB QC              {"NAME":"KMC 5-Outlet","GPIO":[320,0,0,0,228,160,0,0,225,224,226,0,227,0],"FLAG":0,"BASE":18}
KMC Smart Strip 3            {"NAME":"KMC 3-Outlet (20307)","GPIO":[0,320,0,32,0,226,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
Kogan Power Strip USB Ports & Energy Meter  {"NAME":"Generic","GPIO":[64,320,0,227,2720,2656,0,0,2624,225,226,224,0,0],"FLAG":0,"BASE":18}
Konesky Type 1               {"NAME":"Konesky","GPIO":[0,0,0,0,228,225,0,0,227,32,226,224,0,0],"FLAG":0,"BASE":18}
Koogeek KLOE4                {"NAME":"Koogeek KLOE4","GPIO":[0,320,0,32,225,224,0,0,226,227,228,0,0,4704],"FLAG":0,"BASE":18}
Larkkey 4AC 4USB             {"NAME":"LARKKEY Strip","GPIO":[0,544,0,32,225,224,0,0,226,227,228,0,0,0],"FLAG":0,"BASE":18}
LeFun SK2                    {"NAME":"LeFun SK2","GPIO":[0,0,0,32,225,224,0,0,226,227,228,0,0,0],"FLAG":0,"BASE":18}
Lidl Silvercrest Zigbee      {"NAME":"Lidl Silvercrest HG06338","GPIO":[0,288,0,0,225,226,0,0,32,224,576,0,0,0],"FLAG":0,"BASE":18}
LITEdge Smart Power Strip    {"NAME":"LITEEdge Power Strip","GPIO":[227,0,0,0,288,289,0,0,224,32,225,226,228,0],"FLAG":0,"BASE":18}
Luminea 3AC+4USB 16A         {"NAME":"Luminea-NX4473","GPIO":[0,320,0,32,225,224,0,0,0,226,227,0,0,0],"FLAG":0,"BASE":18}
Maxcio ZLD-34EU-W            {"NAME":"MAXCIO","GPIO":[0,320,0,32,225,224,0,0,0,226,227,0,0,4704],"FLAG":0,"BASE":18}
Merkury Innovations Smart Surge  {"NAME":"Merkury Power Strip MIC-SW002-199L","GPIO":[288,0,289,0,228,32,0,0,225,224,226,0,259,0],"FLAG":0,"BASE":18}
Merkury Innovations SmartSurge  {"NAME":"Merkury MI-SW001","GPIO":[288,0,289,0,228,32,0,0,225,224,226,0,227,0],"FLAG":0,"BASE":18}
Meross                       {"NAME":"Meross MSS426","GPIO":[320,0,262,0,229,228,0,0,225,226,224,227,32,0],"FLAG":0,"BASE":18}
Meross 4AC 4USB              {"NAME":"MSS425F","GPIO":[0,544,0,32,225,224,0,0,226,227,260,0,0,0],"FLAG":0,"BASE":18}
Meross MSS425                {"NAME":"Meross MSS425","GPIO":[260,0,0,0,320,0,0,0,224,32,225,226,259,0],"FLAG":0,"BASE":18}
Mirabella Genio 4 Outlet Power Board with 2 USB  {"NAME":"Genio i002340","GPIO":[320,0,0,0,224,225,0,0,226,32,227,228,0,0],"FLAG":0,"BASE":18}
Mirabella Genio Powerboard   {"NAME":"Genio Powerboa","GPIO":[224,288,0,0,226,225,0,0,228,32,229,227,0,0],"FLAG":0,"BASE":18}
Nedis P310                   {"NAME":"Nedis WIFIP310","GPIO":[0,320,0,32,225,224,0,0,0,226,227,0,0,4704],"FLAG":0,"BASE":18}
Nous 15A 3AC 3USB            {"NAME":"NOUS A5T","GPIO":[0,3072,544,3104,0,259,0,0,225,226,224,0,35,4704],"FLAG":1,"BASE":18}
Nozdom 3AC+2USB              {"NAME":"NOZDOM SWB3","GPIO":[544,0,289,0,0,226,0,0,224,32,225,227,0,0],"FLAG":0,"BASE":18}
Pearl CHA1555 3AC 4USB       {"NAME":"Pearl CHA1555","GPIO":[32,0,0,225,2720,2656,0,0,2624,227,226,224,320,0],"FLAG":0,"BASE":18}
Powertech 4AC 2USB           {"NAME":"Ptech MS6107","GPIO":[0,288,0,32,224,225,0,0,227,228,226,0,0,0],"FLAG":0,"BASE":18}
Powrui 4USB 3AC              {"NAME":"POWRUI AHR-087","GPIO":[0,0,0,0,225,226,0,0,32,288,0,224,0,0],"FLAG":0,"BASE":18}
Powrui AHR-079               {"NAME":"Powrui Power S","GPIO":[320,0,0,32,34,224,0,0,226,35,225,227,33,0],"FLAG":0,"BASE":18}
Powrui AHR-081               {"NAME":"POWRUI AHR-081","GPIO":[0,0,0,0,320,32,0,0,225,226,224,0,0,0],"FLAG":0,"BASE":18}
Powrui AW-39                 {"NAME":"Powrui AW-39","GPIO":[320,0,0,0,224,1,0,0,226,227,225,0,160,0],"FLAG":0,"BASE":18}
POWSAV 5+5AC 4USB            {"NAME":"POWSAV 5+5AC+4USB","GPIO":[320,0,0,32,224,225,0,0,228,0,227,226,0,0],"FLAG":0,"BASE":18}
Prokord 4AC 4USB             {"NAME":"PSH-WT003-EU","GPIO":[0,320,0,32,226,227,0,0,225,224,260,0,0,0],"FLAG":0,"BASE":18}
S2199EU                      {"NAME":"S2199EU","GPIO":[0,32,0,288,226,228,0,0,224,227,225,0,0,4704],"FLAG":0,"BASE":18}
SA-P402A                     {"NAME":"SA-P402A","GPIO":[0,32,0,320,226,228,224,227,225,0,0,0,0,4704],"FLAG":0,"BASE":18}
Smart Tech 4AC 4USB          {"NAME":"ST-T-SPS1","GPIO":[0,576,320,0,228,225,0,0,227,32,226,224,0,0],"FLAG":0,"BASE":18}
SmartVU Home 4AC 2USB        {"NAME":"SHWSP4","GPIO":[0,288,0,32,224,225,0,0,227,228,226,0,0,0],"FLAG":0,"BASE":18}
Soundance 3AC 2USB           {"NAME":"Soundance C198","GPIO":[256,0,0,0,320,289,0,0,225,0,226,227,260,0],"FLAG":0,"BASE":18}
STITCH 4 AC, 2 Always-On USB  {"NAME":"MP Stitch 34082","GPIO":[320,0,0,0,227,32,0,0,225,226,224,0,0,0],"FLAG":0,"BASE":18}
Surge Protector 3AC 2USB     {"NAME":"C158","GPIO":[260,0,0,0,261,230,0,0,224,0,225,226,259,0],"FLAG":0,"BASE":18}
SWB1                         {"NAME":"SWB1","GPIO":[288,0,0,0,0,227,0,0,224,32,225,226,0,0],"FLAG":0,"BASE":18}
SWB2 3AC + 2USB              {"NAME":"SWB2","GPIO":[576,1,0,1,0,226,0,0,224,32,225,227,0,0],"FLAG":0,"BASE":18}
Swisstone 4AC 4USB           {"NAME":"Swisstone SH140","GPIO":[0,576,0,32,225,224,0,0,226,227,228,0,0,0],"FLAG":0,"BASE":18}
Sygonix 4AC                  {"NAME":"Sygonix SY-4538254","GPIO":[544,32,0,8678,8676,8679,0,0,8672,8673,8675,8677,8706,0],"FLAG":0,"BASE":18}
TCP Smart 4AC+USB            {"NAME":"TCP WPS4WUK","GPIO":[1,320,0,32,226,227,0,0,225,224,228,0,0,1],"FLAG":0,"BASE":18}
Teckin SS30                  {"NAME":"Teckin SS30","GPIO":[288,1,1,321,256,32,0,0,258,257,259,1,228,0],"FLAG":0,"BASE":18}
Tellur 3AC 4USB              {"NAME":"Tellur","GPIO":[0,320,0,32,225,224,0,0,0,226,227,0,0,4704],"FLAG":0,"BASE":18}
Tessan                       {"NAME":"TESSAN A4L-BK","GPIO":[0,0,0,227,226,0,0,0,224,0,225,0,0,0],"FLAG":0,"BASE":18}
Timeguard 3 Way USB          {"NAME":"Timeguard","GPIO":[0,0,0,0,288,226,0,0,224,32,225,227,0,0],"FLAG":0,"BASE":18}
Tonbux SM-SO301-U            {"NAME":"Tonbux SM-SO30","GPIO":[320,0,0,0,256,0,0,0,258,257,259,0,228,0],"FLAG":0,"BASE":18}
Useelink                     {"NAME":"Useelink","GPIO":[288,0,0,321,256,32,0,0,258,257,259,0,228,0],"FLAG":0,"BASE":18}
Useelink 4AC 2USB            {"NAME":"306 Power Strip","GPIO":[576,0,576,291,259,32,0,0,257,258,256,0,228,0],"FLAG":0,"BASE":18}
Vivitar HA-1007              {"NAME":"Vivitar HA-1007 Power Strip","GPIO":[544,0,0,0,227,228,0,0,225,224,226,0,35,1],"FLAG":0,"BASE":18}
Vivitar HA-1007-AU           {"NAME":"HA-1007-AU","GPIO":[320,32,0,322,256,321,0,0,258,257,259,0,228,0],"FLAG":0,"BASE":18}
wesmartify essentials 3-socket 2 USB  {"NAME":"Essentials Smart Home 3-socket USB Power Strip","GPIO":[0,0,0,0,544,226,0,0,224,32,225,0,0,0],"FLAG":0,"BASE":18}
wesmartify essentials 4AC+4USB  {"NAME":"essential_4_po","GPIO":[320,0,0,0,227,228,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
Wipro Smart Extension        {"NAME":"Generic","GPIO":[321,0,0,0,259,0,0,0,257,258,256,0,228,0],"FLAG":0,"BASE":18}
Wolf Armor 4AC 4USB          {"NAME":"Wolf Armor SP26","GPIO":[0,320,0,32,227,226,0,0,224,228,225,0,0,0],"FLAG":0,"BASE":18}
WOOX Powerstrip EU           {"NAME":"WOOX R5104","GPIO":[0,0,0,32,0,352,0,0,225,355,226,227,224,4896],"FLAG":0,"BASE":18}
WOOX R4028                   {"NAME":"WOOX R4028","GPIO":[0,320,0,32,226,225,0,0,0,227,224,0,0,4704],"FLAG":0,"BASE":18}
WP40                         {"NAME":"WP40","GPIO":[260,0,0,0,320,0,0,0,224,32,225,226,259,0],"FLAG":0,"BASE":18}
Xenon                        {"NAME":"SM-SO306I1","GPIO":[0,0,0,0,256,32,0,0,258,257,259,0,228,0],"FLAG":0,"BASE":18}
Xenon SM-S0301               {"NAME":"SM-SO301","GPIO":[288,1,1,321,256,32,0,0,258,257,259,1,228,0],"FLAG":0,"BASE":18}
Xenon SM-S0301-U             {"NAME":"SM-SO301-U","GPIO":[288,1,1,321,256,32,0,0,258,257,259,1,228,0],"FLAG":0,"BASE":18}
Xenon SM-SO301 v2            {"NAME":"SM-SO301","GPIO":[320,0,0,0,257,32,0,0,259,258,260,0,224,0],"FLAG":0,"BASE":18}
Xenta 3 AC 6 USB             {"NAME":"Xenta-3-Gang","GPIO":[288,0,0,321,256,32,0,0,258,257,0,0,227,0],"FLAG":0,"BASE":18}
XS-A25                       {"NAME":"XS-A25","GPIO":[288,0,289,0,228,225,0,0,227,32,226,224,0,0],"FLAG":0,"BASE":18}
XS-A26                       {"NAME":"XS-A26","GPIO":[288,0,289,0,228,225,0,0,227,32,226,224,0,0],"FLAG":0,"BASE":18}
XS-A34                       {"NAME":"XS-A24","GPIO":[288,0,289,0,228,225,0,0,227,32,226,224,0,0],"FLAG":0,"BASE":18}
Yagala                       {"NAME":"Yagala","GPIO":[288,1,1,321,256,32,0,0,258,257,259,1,228,0],"FLAG":0,"BASE":18}
Yagala SWB3                  {"NAME":"YAGALA SWB3","GPIO":[544,0,289,0,0,226,0,0,224,32,225,227,0,0],"FLAG":0,"BASE":18}
Yagala Z1                    {"NAME":"YAGALA Z1","GPIO":[0,544,0,32,225,227,0,0,226,228,224,0,0,0],"FLAG":0,"BASE":18}
YPS11                        {"NAME":"SM-SO301","GPIO":[320,0,0,0,257,32,0,0,259,258,260,0,224,0],"FLAG":0,"BASE":18}
Yuanguo 4AC+2USB             {"NAME":"YUANGUO","GPIO":[164,35,0,227,34,33,0,0,32,225,224,226,544,0],"FLAG":0,"BASE":1}
Zeoota 4AC 2USB              {"NAME":"Zeoota WP30","GPIO":[0,32,0,227,0,0,0,0,0,225,224,226,0,4896],"FLAG":0,"BASE":18}
Zeoota PS022                 {"NAME":"ZEOOTA 3x plus","GPIO":[0,321,0,320,225,224,0,0,32,226,227,0,0,4704],"FLAG":0,"BASE":18}
Zeoota ZLD-44EU-W            {"NAME":"ZEOOTA-ZLD-44E","GPIO":[0,320,0,32,225,224,0,0,226,227,228,0,0,4704],"FLAG":0,"BASE":18}
ZLD-44EU-W                   {"NAME":"ZLD-44EU-W","GPIO":[32,1,1,1,225,224,33,34,226,227,228,0,0,0],"FLAG":0,"BASE":23}
ZLD-44USA-W                  {"NAME":"ZLD-44USA-W","GPIO":[0,320,0,32,225,224,0,0,226,227,260,0,0,0],"FLAG":0,"BASE":18}
ZLD64-EU-W                   {"NAME":"ZLD64-EU-W","GPIO":[0,320,0,32,225,224,0,0,0,0,226,0,0,0],"FLAG":0,"BASE":18}
```

## Presence Sensor
```
Everything Presence Lite     {"NAME":"Everything Presence Lite","GPIO":[0,0,0,0,0,0,0,0,0,0,544,0,3232,3200,160,161,0,640,608,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Everything Presence One      {"NAME":"Everything Presence One","GPIO":[0,0,0,0,0,0,0,0,0,3200,3232,160,0,1,1,1,0,0,1,1,0,608,640,0,0,0,0,0,544,161,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"SO114 1 | SwitchMode1 1 | SwitchMode2 1"}
Tuya mmWave                  {"NAME":"ZY-M100","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"SO97 1 | TuyaMCU 99,1 | TuyaMCU 75,104"}
```

## RF Gateway
```
Sonoff RF Bridge 433         {"NAME":"Sonoff Bridge","GPIO":[32,3200,1,3232,1,1,0,0,1,320,1,0,0,0],"FLAG":0,"BASE":25}
Virage Labs VirageBridge 433MHz  {"NAME":"VirageBridge","GPIO":[32,3200,1,3232,1,1,0,0,1,320,1,0,0,0],"FLAG":0,"BASE":25}
```

## RGB
```
Cairondin 7W 500lm           {"NAME":"Cairondin","GPIO":[32,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Deltaco SH-LE27RGB 810lm     {"NAME":"SH-LE27RGB","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Halonix Prime Prizm 12W      {"NAME":"Halonix Prizm ","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Jomarto 9W                   {"NAME":"Jomarto Wifi S","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,4704],"FLAG":0,"BASE":18}
LVWIT 5W 350Lm               {"NAME":"LVWITGU10","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Lyasi PT-BW09                {"NAME":"Lyasi PT-BW09","GPIO":[32,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
MoKo YX-L01C-E27 810lm       {"NAME":"MOKO","GPIO":[32,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
muvit IO miobulb001 600lm    {"NAME":"miobulb001","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,1],"FLAG":0,"BASE":18}
Oobest ZN93028 11W           {"NAME":"RGB Bulb 11W","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,4704],"FLAG":0,"BASE":18}
SuperBrightLEDs 3W 70lm 12V  {"NAME":"G4-RGB3W","GPIO":[0,0,0,0,417,0,0,0,418,0,416,0,0,0],"FLAG":0,"BASE":18}
V-Tac P45 4.5W 300lm         {"NAME":"V-TAC VT-5154","GPIO":[0,0,0,0,0,0,0,0,4065,0,4032,0,0,0],"FLAG":0,"BASE":18}
```

## RGBCCT
```
Aigital LE13 800lm           {"NAME":"Aigital 9W RGB","GPIO":[0,0,0,0,420,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
Aisirer 10W 1000lm           {"NAME":"Aisirer RGBCW","GPIO":[160,0,0,0,0,0,0,0,0,4032,4064,0,0,0],"FLAG":0,"BASE":18}
AiYaTo 12W                   {"NAME":"AiYaTo RGBCW","GPIO":[0,0,0,0,419,418,0,0,416,420,417,0,0,0],"FLAG":0,"BASE":18}
AiYaTo 12W                   {"NAME":"AiYaTo RGBCW","GPIO":[0,0,0,0,419,418,0,0,416,420,417,0,0,0],"FLAG":0,"BASE":18}
Alfawise LE12 9W 900LM       {"NAME":"Alfawise LE12 ","GPIO":[0,0,0,0,420,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
Aoycocr JL81 5W 400lm        {"NAME":"AoycocrJLB1","GPIO":[0,0,0,0,418,0,0,0,417,420,416,419,0,0],"FLAG":0,"BASE":18}
Aoycocr Q10CWM BR30 9W 720lm  {"NAME":"AoycocrBR30","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,0],"FLAG":0,"BASE":18}
Arlec 5.5W 470lm             {"NAME":"Arlec GLD360HA","GPIO":[0,0,0,0,0,0,0,0,4067,0,4032,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO37 24"}
Arlec Smart 10W 830lm        {"NAME":"Arlec GLD320HA","GPIO":[0,0,0,0,4067,0,0,0,0,4032,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO37 6"}
Arlec Smart 9.5W 806lm       {"NAME":"Arlec RGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Arlec Smart 9.5W 806lm       {"NAME":"Arlec RGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Athom 12W 1000lm             {"NAME":"LB03-12W-E27-TAS","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO92 1 | DimmerRange 24,100"}
Athom 15W 1400lm             {"NAME":"LB01-15W-E27-TAS","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO92 1 | DimmerRange 24 100"}
Athom 4.5W 300lm             {"NAME":"Athom LB10-5W-TAS","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Athom 7W 600lm               {"NAME":"LB01-7W-B22-TAS","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Athom 7W 600lm               {"NAME":"LB01-7W-E27-TAS","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Aunics 7W 600lm              {"NAME":"Aunics RGBW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Avatar 8W 800lm              {"NAME":"Avatar 8W RGBCW","GPIO":[1,1,1,1,416,419,1,1,417,420,418,1,1,1],"FLAG":0,"BASE":18}
Avatar ALB201W 720lm         {"NAME":"AVATAR ALB201W","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,0],"FLAG":0,"BASE":18}
Avatar ALS18L A60 800lm      {"NAME":"Avatar E14 7W","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":20}
AZzardo 10W                  {"NAME":"Azzardo AZ3213","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18,"CMND":"SetOption37 6"}
B.K. Licht 5.5W 350lm        {"NAME":"BKL1262","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
B.K. Licht 9W 806lm          {"NAME":"BKL1253","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Bakibo TB95 9W 1000lm        {"NAME":"Bakibo A19 9W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Bardi 9W 900lm               {"NAME":"Bardi 9W RGBWW","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":20}
BAZZ BR30 650lm              {"NAME":"BAZZrgb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Beghelli dom-E 14W 1400lm    {"NAME":"Beghelli Smart Bulb","GPIO":[0,0,0,0,416,417,0,0,418,419,420,0,0,0],"FLAG":3,"BASE":18,"CMND":"SO37 13"}
BlitzWolf w/ remote 850lm    {"NAME":"BW-LT27","GPIO":[0,0,0,0,420,417,0,0,418,1088,419,416,0,0],"FLAG":0,"BASE":18}
BNETA 8.5W 800lm             {"NAME":"BNETA IO-WIFI60-E27P","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":48}
BNETA 8.5W 800lm             {"NAME":"OM60/RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Bomcosy 600lm                {"NAME":"Generic","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,4704],"FLAG":0,"BASE":18}
Boxlood 9W 900lm             {"NAME":"Boxlood 9w 900lm RGBCCT","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Calex 429004 A60 806lm       {"NAME":"Calex E27 RGB ","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Calex 429008 B35 5W 470lm    {"NAME":"Calex E14 RGBW","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Calex 5W 350lm Reflector     {"NAME":"Calex RGBW","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Candle 5W 450lm              {"NAME":"Candela","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Cleverio 400lm 5.5W          {"NAME":"Cleverio 51573","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Cleverio 51395 806lm         {"NAME":"CleverioE27RGB","GPIO":[0,0,0,0,2944,2912,0,0,417,2976,416,0,0,0],"FLAG":0,"BASE":18}
Cloudfree 1000lm A21         {"NAME":"CloudFree LBC","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
CMARS 4W Reflector           {"NAME":"RGBWW GU10","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,1],"FLAG":0,"BASE":18}
Connect SmartHome 5W GU5.3   {"NAME":"CSH-GU53RGB5W","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Diolamp E27 9W 800lm         {"NAME":"Diolamp A609WIFI","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18}
Dogain 320lm                 {"NAME":"DOGAIN","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Eagle Lighting 9W 800lm      {"NAME":"Eagle Lighting WXX991","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
EKasa 10W+3W 1050lm          {"NAME":"EKASA T710C-E27","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Emuni TB95 9W 850Lm          {"NAME":"TB95","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Ener-J 800lm                 {"NAME":"ENER-J RGBWWW ","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Energizer 10W 800lm          {"NAME":"OM100/RGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Enklen 7W 600lm              {"NAME":"Enklen E27 7W RGBCW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Euri Lighting 10W 800lm      {"NAME":"Euri Lighting ","GPIO":[0,0,0,0,416,419,0,0,417,420,418,416,0,0],"FLAG":0,"BASE":18}
Extrastar 10w 1000lm         {"NAME":"Extrastar","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
EXUP C37 5W                  {"NAME":"EXUP","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,1],"FLAG":0,"BASE":18}
Feit Electric A19 1600lm     {"NAME":"OM100/RGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Feit Electric A19 800lm      {"NAME":" BPA800/RGBW/AG/2","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Feit Electric A19 800lm      {"NAME":"BPA800/RGBW/AG/2P","GPIO":[0,0,0,0,416,449,0,0,2944,2976,2912,0,0,0],"FLAG":0,"BASE":48}
Feit Electric A19 800lm      {"NAME":"FE-OM60-15K-AG","GPIO":[0,0,0,0,2944,2912,0,0,416,2976,417,0,0,0],"FLAG":0,"BASE":18}
Feit Electric BR30 700lm     {"NAME":"Feit BR30/RGBW","GPIO":[0,0,0,417,2944,2912,0,0,0,2976,416,0,0,0],"FLAG":0,"BASE":18}
Feit Electric OM60/RGBW/CA/AG A19 800lm  {"NAME":"OM60/RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Feit OM60/RGBW/CA/AG(P) 9W 800lm  {"NAME":"OM60/RGBW/CA/AG/P","GPIO":[0,0,0,0,2944,2912,0,0,416,2976,417,0,0,0],"FLAG":0,"BASE":18}
Fitop 10W                    {"NAME":"Fitop 10W RGBCW","GPIO":[0,0,0,0,0,0,0,0,0,4032,4064,0,0,0],"FLAG":0,"BASE":18,"CMND":"SetOption37 6"}
Fitop 9W                     {"NAME":"E27RGBCCT9w","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,1],"FLAG":0,"BASE":18}
Fulighture 9W 810lm          {"NAME":"Fulighture 9W","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Geeni Prisma 10W 1050lm      {"NAME":"Geeni Prisma 1050 RGB","GPIO":[0,0,0,0,2944,2912,0,416,417,2976,0,0,0,0],"FLAG":0,"BASE":18}
Geeni Prisma Plus 800lm      {"NAME":"Geeni Prisma P","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Generic GU10 5W 460lm        {"NAME":"RGBCCT GU10","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Girier 9W                    {"NAME":"GIRIER E27 9W ","GPIO":[0,0,0,0,420,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
Globe A19 10W 800lm          {"NAME":"GlobeRGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Great Value Wiz A19 8.8w 800lm  {"NAME":"GreatValue RGBCCT","GPIO":[0,0,0,0,419,0,0,0,416,420,418,417,0,0],"FLAG":0,"BASE":48}
Havells Glamax 9W 850lm      {"NAME":"HavellsGlamaxTWColors","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18,"CMND":"SetOption92 1"}
Helloify BR30 9W 600lm       {"NAME":"Helloify","GPIO":[1,1,1,1,416,419,1,1,417,420,418,1,1,0],"FLAG":0,"BASE":18}
HeyBulb 11W 1050lm           {"NAME":"HeyBulb","GPIO":[0,0,0,1088,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
HIPER IoT A61                {"NAME":"HIPER IoT A61","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
HIPER IoT B1                 {"NAME":"HIPER B1-RGB","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
HIPER IoT C1                 {"NAME":"Hiper IoT C1 RGB","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Holdpeak BR40 12W 1100lm     {"NAME":"HP-BR40-12W","GPIO":[0,0,0,0,2912,416,0,0,417,0,2944,0,0,0],"FLAG":0,"BASE":18}
Hombli 9W 800lm              {"NAME":"HBEB-0124","GPIO":[0,0,0,0,418,417,0,0,419,416,420,0,0,0],"FLAG":0,"BASE":18}
Infray 9W 900LM              {"NAME":"InfrayRGBCCT","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Intempo 7W                   {"NAME":"Intempo Light Bulb RGB","GPIO":[0,0,0,0,2944,2912,0,0,417,2976,416,0,0,0],"FLAG":0,"BASE":18}
iQtech 9W 800lm              {"NAME":"iQ-Tech RGBCCT 9W 800LM","GPIO":[0,0,0,0,4032,0,0,0,0,0,4065,0,0,0],"FLAG":0,"BASE":18}
Jeeo TF-QPZ13 800lm          {"NAME":"Jeeo","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Julun JL-021 5W Candle       {"NAME":"E14 RGBCCT","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Kauf esphome 10W             {"NAME":"Kauf Bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Kauf esphome A15 5W          {"NAME":"Kauf Bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Kauf esphome A19 7W          {"NAME":"Kauf Bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
KHSUIN BR30 13W 1300lm       {"NAME":"KHSUIN 13W BR30","GPIO":[0,0,0,0,416,420,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Kogan 10W 1050lm             {"NAME":"Kogan RGB+CCT","GPIO":[1,1,1,0,416,419,1,1,417,452,418,1,1,1],"FLAG":0,"BASE":18}
Kohree 600lm                 {"NAME":"Kohree VHP560","GPIO":[0,0,0,0,416,420,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
LangPlus+ A60 7W             {"NAME":"LangPlus+ A60 7W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lasco 9W 700lm               {"NAME":"Lasco 7W","GPIO":[0,0,0,0,2912,416,0,0,0,0,2944,0,0,0],"FLAG":0,"BASE":1}
Laser 10W 1000lm             {"NAME":"LSH-E27RGB10W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Laser 5W 460lm               {"NAME":"LSH-E14RGB5W","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
LE lampUX 8.5W 806lm         {"NAME":"lampUX","GPIO":[0,0,0,0,2944,2912,0,0,417,2976,416,0,0,1],"FLAG":0,"BASE":18}
LE lampUX A19 850lm          {"NAME":"LE RGBWW 60W","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,4704],"FLAG":0,"BASE":18}
LE lampUX A19 9.5W 806lm     {"NAME":"LE lampUX 9W Bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LE lampUX A19 9W 806lm       {"NAME":"lampUX","GPIO":[0,0,0,0,2944,2912,0,0,417,2976,416,0,0,1],"FLAG":0,"BASE":18}
Ledmundo 6W 600lm            {"NAME":"LEDMUNDO 6W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Legelite 5W Candle           {"NAME":"Legelite E12","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Legelite A60 7W              {"NAME":"Legelite A60 7","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Legelite A60 7W 600lm        {"NAME":"Legelite E26","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lenovo 800lm                 {"NAME":"Lenovo SE-241EB","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Lloyd's 5W 400Lm             {"NAME":"LLOYDS LC-1271","GPIO":[160,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Local Bytes 9W 900lm         {"NAME":"localbytes bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN004 8W 680lm         {"NAME":"Lohas B22 R63","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN011 5W 420lm         {"NAME":"LohasZN011","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN014-2 5W 380lm       {"NAME":"Lohas ZN014-2","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN031 650lm            {"NAME":"Lohas ZN031","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN033 9W 810lm         {"NAME":"Lohas RGBWW","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN033 9W 810lm         {"NAME":"Lohas RGBCW","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN036 900 lm           {"NAME":"Lohas RGBCCT","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN037 450lm            {"NAME":"Lohas LH-ZN037","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":20}
Lohas ZN038 BR20 8W 750lm    {"NAME":"Lohas LH-ZN038","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN039 BR40 1450lm      {"NAME":"Lohas ZN039","GPIO":[0,0,0,0,418,416,0,0,420,417,419,0,0,0],"FLAG":0,"BASE":18}
Lohas ZN041 BR30 12W 1080lm  {"NAME":"Lohas ZN041","GPIO":[0,0,0,0,418,416,0,0,420,417,419,0,0,0],"FLAG":0,"BASE":18}
Longlifelamps A60 9W         {"NAME":"LongLifeLamp Nimble 10W","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
LSC 9W 806lm                 {"NAME":"LSC 970714 swapped4ESP","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Lumary 9W 800lm              {"NAME":"Lumary / iLint","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Luminea 5.5W 470lm           {"NAME":"Luminea ZX-2983","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Luminea 9W 800lm             {"NAME":"Luminea RGBW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LVWIT A60 8.5W 806lm         {"NAME":"LVWIT A60 8.5W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LVWIT A70 12W 1521lm         {"NAME":"LVWIT A70 12W","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
LVWIT BR30 8.5W 650lm        {"NAME":"LVWIT BR30 8.5W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LVWIT G45 5W 470Lm           {"NAME":"LVWIT E14 5W G45 RGBWCCT","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Lyhope 7W 650lm              {"NAME":"Lyhope 014BB06","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
MagicHome 7W                 {"NAME":"MagicHome E27","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Makion 7W 600lm              {"NAME":"Makion Smart LED Bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Melery MR16                  {"NAME":"Melery MR16 GU5.3","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 5W 450lm Candle  {"NAME":"MiraBellaGenio","GPIO":[0,0,0,0,0,0,0,0,4068,0,4032,0,0,0],"FLAG":0,"BASE":18}
Moes 9W 800lm                {"NAME":"Moes 9w","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Nishica JBT 9W 806lm         {"NAME":"Nishica","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Novostella 7W 600lm          {"NAME":"Novostella E27","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Novostella HM-LB09 13W 1300lm  {"NAME":"Novostella 13W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Novostella NTB10 9W 900lm    {"NAME":"NTB10","GPIO":[0,0,0,0,420,417,0,0,418,1088,419,416,0,0],"FLAG":0,"BASE":18}
Novostella UT55507 9W 900lm  {"NAME":"Novostella UT55507","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Novostella UT55508 12W 1150lm  {"NAME":"Novostella","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Novostella UT55509 13W 1300lm  {"NAME":"Novostella 13W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Ohlux 5W 450lm Candle        {"NAME":"OHLUX","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Ohlux A19 7W 600lm           {"NAME":"OHLUX2021","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Ohlux BR30 10W 900lm         {"NAME":"OHLUX","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
oobest 7W                    {"NAME":"E27_RGBCW_Bulb","GPIO":[0,0,0,0,417,416,0,0,419,420,418,0,0,0],"FLAG":0,"BASE":18}
PETEME 7.5W 750lm            {"NAME":"PETEME","GPIO":[1,1,0,1,416,419,0,0,417,420,418,1,1,4896],"FLAG":0,"BASE":18}
Phillips Wiz A19 8.8W 800lm RGBCCT Bulb  {"NAME":"WiZ RGBCCT A19 Bulb","GPIO":[0,0,419,0,420,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,416,417,418,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"so92 1"}
Polux 5.5W 400lm             {"NAME":"Polux GU10","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Polux A65 11W 1055lm         {"NAME":"A65 SMDWWCW+RG","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Polux G45 5,5W 475lm         {"NAME":"Polux G45 E14","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Positivo 10W 806lm           {"NAME":"Positivo Bulb","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Powercase 10W 1050lm         {"NAME":"Powercase A95BU22-S","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Powertech SL225X 800lm       {"NAME":"Jaycar SL225X","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
QM Smart Cloud 4W 350lm      {"NAME":"QMSmart RGBCCT","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Qualitel ALS08L 1100lm       {"NAME":"Qualitel ALS08","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Reafoo A26 9W                {"NAME":"ReaFooE26","GPIO":[0,0,0,0,420,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
Reafoo A27 9W 810lm          {"NAME":"ReaFooE27","GPIO":[0,0,0,0,420,419,0,0,416,0,418,417,0,0],"FLAG":0,"BASE":18}
Ruihai 9W 800lm              {"NAME":"Ruihai SB50","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
RYE 5W 450LM Candle          {"NAME":"RYE Candlebra","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Saudio A19 7W 700lm          {"NAME":"X002BU0DOL","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Sealight A19 9W 810lm        {"NAME":"DGO/SEASTAR","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Slitinto 5W Candle           {"NAME":"Slitinto RGBWW","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Slitinto TB95 9W 1000lm      {"NAME":"Slitinto 9W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Smart 9W 800lm               {"NAME":"SmartLED ","GPIO":[0,0,0,0,420,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
SmartLED 9W 400lm            {"NAME":"SmartLED","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Smitch 10W                   {"NAME":"Smitch SB161001 - B22","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Sonoff B1 (R2)               {"NAME":"Sonoff B1","GPIO":[32,0,0,0,0,0,0,0,3008,0,3040,0,0,0],"FLAG":0,"BASE":26}
Spectrum Smart 13W 1500lm    {"NAME":"LED 13W E-27 Spectrum SMART","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Spectrum Smart GLS 9W 850lm  {"NAME":"SPECTR. RGBCCT","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Spectrum Smart GU10 5W 480lm  {"NAME":"SPECTR. RGBCCT","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Steren 10W 800lm             {"NAME":"Steren_SHOME-120_RGBCCT","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Sulion Swan A70 14W 1200lm   {"NAME":"Sulion Bombilla A70","GPIO":[0,0,0,0,416,419,0,0,417,452,418,0,0,0],"FLAG":0,"BASE":18}
Sunco G25 5W 450lm           {"NAME":"Sunco G25","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Sunco PAR20 5W 400lm         {"NAME":"Sunco PAR20","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
Sunco PAR38 13W 1050lm       {"NAME":"Sunco PAR38 RGBCCT","GPIO":[0,0,0,0,417,416,0,0,419,418,420,0,0,0],"FLAG":0,"BASE":18}
Sunlit Smart A60 10W 810lm   {"NAME":"Sunlit A60","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO37 31"}
Techlux A19 9W 806lm         {"NAME":"TECHLUX A19 RGBCW 806lm 9w","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Teckin SB50 v3 A19 800lm     {"NAME":"Teckin SB50v3","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Teckin SB53 1300lm           {"NAME":"Teckin SB53","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Treatlife A19 8W 650lm       {"NAME":"Treatlife RGBW","GPIO":[0,0,0,0,417,416,0,0,420,418,419,0,0,0],"FLAG":0,"BASE":18}
V-TAC 10W 806lm              {"NAME":"V-TAC VT-5119","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18}
V-Tac A60 10W 806lm          {"NAME":"V-Tac A60","GPIO":[0,0,0,0,416,417,0,0,418,419,420,0,0,0],"FLAG":0,"BASE":18,"CMND":"SetOption37 13"}
V-Tac PAR16 4.5W 400lm 100   {"NAME":"V-TAC VT5164","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Vizia 5W GU10                {"NAME":"Vizia RGBWW","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,1],"FLAG":0,"BASE":18}
WdtPro 8W 800lm              {"NAME":"WdtPro","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Wipro Garnet 9W 810lm        {"NAME":"Wipro","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
WiZ BR30 13.8W 750lm         {"NAME":"WiZ RGBCCT PAR30","GPIO":[0,0,0,0,419,0,0,0,417,420,418,416,0,0],"FLAG":0,"BASE":48}
Wiz Colors A19 8.8w 800lm    {"NAME":"Wiz Colors RGBCCT","GPIO":[0,0,0,0,419,0,0,0,416,420,418,417,0,0],"FLAG":0,"BASE":48}
Wiz Colors A21 14.5w 1600lm  {"NAME":"WIZ-922-65","GPIO":[1,1,420,1,419,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,416,417,418,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
WiZ PAR16 4.9W 400lm         {"NAME":"WIZ GU10","GPIO":[0,0,420,0,419,416,0,0,0,0,0,0,0,0,417,418,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Xtricity A19 10W 800lm       {"NAME":"XTRICITY A19 10W 800LM RGBCCT BULB","GPIO":[0,0,0,0,416,419,0,0,417,452,418,2304,2272,0],"FLAG":0,"BASE":48}
Zemismart 5W 480lm           {"NAME":"Zemismart 5W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Zemismart 5W 480lm           {"NAME":"Zemismart-E27-RGBCW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
ZunPulse 9W                  {"NAME":"ZunPulse B22 9W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
ZZHXON 600lm                 {"NAME":"E27_RGB_Bulb","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
```

## RGBIC LED
```
Athom 2812b Controller for   {"NAME":"LS2812B-TAS","GPIO":[32,0,1376,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Athom ESP32 Music Controller for  {"NAME":"Athom LS8P","GPIO":[32,1,224,1,1,1,1,1,1,1,1,7904,1,1377,1376,1,0,1,1,1,0,1088,1,1,0,0,0,0,7872,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Athom High Power 16A Controller for  {"NAME":"LS_4PIN_TAS","GPIO":[32,1376,0,0,0,0,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"Rule1 on Power1#State do power2 2 endon|Rule1 1"}
BlitzWolf IC Smart RGB Controller for  {"NAME":"BW-LT31","GPIO":[0,0,32,1376,0,0,0,0,0,1088,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"SO37 24"}
cod.m WLAN Pixel Controller v0.8  {"NAME":"cod.m Pixel Controller V0.8","GPIO":[0,0,0,0,0,544,0,0,0,0,0,32,1376,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
ESP01 NeoPixel Ring          {"NAME":"ESP-01S-RGB-LED-v1.0","GPIO":[1,1,1376,1,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
H803WF 2048 Pixel 5V-24V     {"NAME":"H803WF","GPIO":[0,0,0,0,3840,3840,0,0,3872,1376,0,3872,0,0],"FLAG":0,"BASE":18}
IOTMCU                       {"NAME":"IOTMCU_ESP-12S-RGB-LED-v1","GPIO":[1,1,1,1,0,1376,0,0,1,1088,32,0,0,0],"FLAG":0,"BASE":18}
LifeSmart Cololight MIX      {"NAME":"ESP32-DevKit","GPIO":[0,0,0,0,1376,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,0,0,4704,0,0,0,0,0,0],"FLAG":0,"BASE":1}
LifeSmart Cololight PRO Hexagonal  {"NAME":"Cololight PRO","GPIO":[0,0,0,0,32,0,0,0,0,33,0,0,1376,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4704,0,0,0,0,0,0],"FLAG":0,"BASE":1}
SP501E WS2812B               {"NAME":"SP501E","GPIO":[0,32,0,1376,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
```

## RGBW
```
3Stone EBE-QPW36 1050lm      {"NAME":"3Stone","GPIO":[0,0,0,0,2912,417,0,0,0,416,2944,0,0,0],"FLAG":0,"BASE":18}
Accewit 7W 650lm             {"NAME":"Accewit Bulb","GPIO":[0,0,0,0,0,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
Aisirer 7W 580lm             {"NAME":"Aisirer RGBW","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Aisirer 7W 580lm             {"NAME":"Aisirer RGBW","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
AL Above Lights 810lm        {"NAME":"AL 810LM","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Aoycocr Q0 750lm             {"NAME":"AoycocrA19","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,0],"FLAG":0,"BASE":18}
Aoycocr Q3CM 230lm           {"NAME":"Aoycocr_GU10","GPIO":[0,0,0,0,0,0,0,0,3008,0,3040,0,0,0],"FLAG":0,"BASE":27}
Aoycocr Q9WM A21 10W 900lm   {"NAME":"Aoycocr Q9WM","GPIO":[0,0,0,0,0,418,0,0,417,419,416,420,0,0],"FLAG":0,"BASE":18}
Authometion 9W 850lm         {"NAME":"LYT8266","GPIO":[1,1,419,1,1,1,1,1,417,416,418,224,1,1],"FLAG":0,"BASE":18}
Avatar ALS08L A19 910lm      {"NAME":"Avatar E27 7W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":20}
Avatar ALS09L A60 900lm      {"NAME":"Avatar E14 9W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":20}
Avatar ALS11L PAR16 500lm    {"NAME":"Avatar_GU10","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Avatar Controls A19 10W 900lm 3000k  {"NAME":"Avatar10WA19","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Avatar Controls A19 10W 900lm 6000k  {"NAME":"Avatar E26 10W","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,0],"FLAG":0,"BASE":18}
AWOW A60 9W 800lm            {"NAME":"AWOW 9W RGBW","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Axtee AI-003 A19 700lm       {"NAME":"Axtee E26 7W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":20}
Bawoo EUWL122130 925lm       {"NAME":"bawoo_bulb","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf 900lm              {"NAME":"BlitzWolf LT21","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,0],"FLAG":0,"BASE":18}
BNeta 4.5W 380lm             {"NAME":"BNeta","GPIO":[0,0,0,0,2944,2912,0,0,416,2976,0,0,0,0],"FLAG":0,"BASE":18}
BriHome 6,5W 500lm           {"NAME":"BRI E27 6,5W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":20}
Brilliant 350lm Candle       {"NAME":"HK17653S72","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20698 9W 800lm  {"NAME":"Brilliant20698","GPIO":[0,0,0,0,2944,2912,0,0,416,2976,0,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20699 9W 800lm  {"NAME":"Brilliant20699","GPIO":[0,0,0,0,2944,2912,0,0,416,2976,0,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20741 9W 750lm  {"NAME":"Brilliant RGB+","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
BrilliantSmart 20888         {"NAME":"BS-GU10-20888","GPIO":[0,0,0,0,2912,417,0,0,416,2976,2944,0,0,0],"FLAG":0,"BASE":18}
BrizLabs A19 9W 806LM        {"NAME":"TCP Smart RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
BrizLabs Candle 4,5W 350lm   {"NAME":"BrizLabs RGBW","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Brizlabs EBE-LZW10 350Lm     {"NAME":"Brizlabs E14","GPIO":[0,0,0,0,2944,2912,0,0,416,2976,0,0,0,1],"FLAG":0,"BASE":18}
BrizLabs EBE-SHW03 380lm     {"NAME":"BrizLabs","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
BTZ1                         {"NAME":"WifiBulb","GPIO":[0,0,0,0,0,416,0,0,418,419,417,0,0,0],"FLAG":0,"BASE":18}
Cleverio 4.5W 350lm          {"NAME":"HK17653S72","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Cocoon DY180363-B 800lm      {"NAME":"Cocoon RGBW","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 10W        {"NAME":"CSH-B22RGB10W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 10W        {"NAME":"CSH-E27RGB10W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 5W         {"NAME":"CSH-E14RGB5W","GPIO":[0,0,0,0,419,420,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Connex A70 10W 1050lm        {"NAME":"Connex 10w RGBWW","GPIO":[0,0,0,417,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Connex Connect A60 6W 470lm  {"NAME":"Connex RGBW Bu","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
DailyComb 7W 600lm           {"NAME":"DailyComb RGBW Bulb","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Elari A60 6W 470lm           {"NAME":"OM60/RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
electriQ 600lm               {"NAME":"ElectricQ B22","GPIO":[0,0,0,0,416,420,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
EleLight 350lm               {"NAME":"EleLight 7wA19","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Esicoo 810lm                 {"NAME":"Esicoo Bulb","GPIO":[0,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
Eurolux A70 10W              {"NAME":"Eurolux A70 RG","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,4704],"FLAG":0,"BASE":18}
Fcmila 10W                   {"NAME":"FCMILA LED E27","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Fcmila 15W 1050lm            {"NAME":"FCMILA B22 15W","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Fcmila 7W                    {"NAME":"FCMILA E27 0.1","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Fcmila Spotlight 460lm       {"NAME":"Fcmila LED 6W","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Feit Electric BR30 650lm     {"NAME":"BR30/RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
FFHL 12W 900lm               {"NAME":"FFHL RGBW Bulb","GPIO":[0,0,0,0,0,416,0,0,418,419,417,0,0,0],"FLAG":0,"BASE":18}
Frankever A21                {"NAME":"FrankEver RGBW Bulb","GPIO":[0,1,5729,2272,1,2304,0,0,832,3008,896,3040,256,0],"FLAG":0,"BASE":27}
Fulighture A60 810lm         {"NAME":"Fulighture A60","GPIO":[0,0,0,0,417,416,0,0,0,418,419,0,0,0],"FLAG":0,"BASE":18}
Fulighture RGBW 15W          {"NAME":"Fulighture RGBW 15W","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":18}
Garsent 10W                  {"NAME":"Garsent 10W RGBW-Bulb","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,4704],"FLAG":0,"BASE":18}
Geeni Prisma Drop BR30 700lm  {"NAME":"GN-BW906-999","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Generic GU10 5W 450lm        {"NAME":"RGBCW GU10","GPIO":[0,1,0,1,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":3}
Gosund 8W 800lm              {"NAME":"Gosund RGBW 8W","GPIO":[0,0,0,0,420,419,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Gosund NiteBird 8W 800lm     {"NAME":"Gosund WB4","GPIO":[0,0,0,0,419,0,0,0,416,417,418,0,420,0],"FLAG":0,"BASE":18}
Gosund WB3 8W 800lm          {"NAME":"Gosund WB3","GPIO":[0,0,0,0,419,0,0,0,416,417,418,0,0,0],"FLAG":0,"BASE":18}
Hama 10W 1050lm              {"NAME":"Hama Bulb RGBW","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Hama 10W 806lm               {"NAME":"Hama Smart WiF","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Hama 4.5W                    {"NAME":"hama E14 RGB","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Hiiten A19 7W 650lm          {"NAME":"Hiiten Bulb","GPIO":[416,0,0,0,2912,417,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Hikenri 7W 640lm             {"NAME":"HIKENRI E27 RGB","GPIO":[32,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Hombli 4.5W 380lm Candle     {"NAME":"Hombli E14 RGB","GPIO":[0,0,0,0,2944,2912,0,0,0,2976,416,0,0,0],"FLAG":0,"BASE":18}
Hykker SL-0492 810lm         {"NAME":"Hykker RBGW 9W","GPIO":[0,0,0,0,0,419,0,0,416,0,418,417,0,0],"FLAG":0,"BASE":18}
iGET Security                {"NAME":"iGET DP23","GPIO":[0,0,0,0,419,0,0,0,416,417,418,0,420,0],"FLAG":0,"BASE":18}
Jetstream 9W 800lm           {"NAME":"Jetstream MA19CL","GPIO":[0,0,0,0,416,0,0,0,2944,2976,2912,0,0,0],"FLAG":0,"BASE":18}
Kainsy 600lm                 {"NAME":"KAINSY","GPIO":[32,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
Kkmoon 9W 800lm              {"NAME":"KKMOON V21","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Koaanw 650lm                 {"NAME":"KOAANW Bulb","GPIO":[0,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
Kogan 10W Ambient 1050lm     {"NAME":"Kogan RGB","GPIO":[0,0,0,0,2912,416,0,0,0,0,2944,0,0,0],"FLAG":0,"BASE":18}
Kogan 4.5W 330lm 110         {"NAME":"Kogan_GU10","GPIO":[0,0,0,0,418,419,0,0,416,0,417,0,0,0],"FLAG":0,"BASE":18}
Kogan Ambient Candle         {"NAME":"Kogan_E14","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Kuled 800lm                  {"NAME":"KULED 60W RGB","GPIO":[0,0,0,0,418,419,0,0,416,0,417,0,0,4704],"FLAG":0,"BASE":18}
Laideyi 7W                   {"NAME":"7W-E14-RGBW-La","GPIO":[0,0,0,0,417,416,0,0,418,0,419,0,0,0],"FLAG":0,"BASE":18}
Lanberg 9W                   {"NAME":"Lanberg E27 9W RGBW","GPIO":[0,0,0,0,1,419,0,0,416,0,418,417,0,0],"FLAG":0,"BASE":18}
Laser 5W 430lm               {"NAME":"LaserSmartHomeGU10","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
LE LampUX BR30 9.5W 750lm    {"NAME":"LE RGBCCT 9.5W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LeCardio 10W 980lm           {"NAME":"LeCardio RGBW","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
LEDLite 10W 800lm            {"NAME":"LEDLite","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Linganzh LE7 6W 600lm        {"NAME":"Linganzh LE7","GPIO":[0,0,0,0,0,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
Linganzh LWE3 7W 800lm       {"NAME":"LINGANZH Smart ","GPIO":[0,0,0,0,420,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
LiteMate A90 810lm           {"NAME":"LiteMate 9W","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
LOFTer 7W 450lm              {"NAME":"Lofter","GPIO":[0,1,1,1,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Lohas 14W 1350lm             {"NAME":"Lohas","GPIO":[0,0,0,0,416,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":18}
Lohas ZN001 810lm            {"NAME":"Lohas RGBW","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":18}
Lohas ZN001-E12 810lm        {"NAME":"Lohas","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":26}
Lohas ZN005 420lm            {"NAME":"Lohas E14 R50","GPIO":[32,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Lohas ZN006 1380lm           {"NAME":"Lohas100 RGBW","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":18}
Lohas ZN012 450lm            {"NAME":"LH-5W-ZN01204","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Lonsonho 900lm               {"NAME":"RGB+W+C Bulb","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
LSC C45 400lm                {"NAME":"LSC RGBCW E14","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
LSC MR16 380lm               {"NAME":"LSC RGBCW GU10","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
LTC 10W                      {"NAME":"LTC LXU403","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Lumiman LM530 7.5W 800lm     {"NAME":"Lumiman LM530","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,4704],"FLAG":0,"BASE":18}
Lumiman LM530 7.5W 800lm     {"NAME":"Lumiman LM530","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,4704],"FLAG":0,"BASE":18}
Lumiman LM530 7.5W 800lm     {"NAME":"LM530","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,4704],"FLAG":0,"BASE":18}
Luminea 4.5W 350lm Candle    {"NAME":"Luminea NX4462 RGB+W","GPIO":[0,0,0,0,418,419,0,0,416,0,417,0,0,0],"FLAG":0,"BASE":18}
Luminea ZX-2832              {"NAME":"Luminea RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,4704],"FLAG":0,"BASE":18}
Luminea ZX-2986 1400lm       {"NAME":"Luminea RGBW","GPIO":[0,0,0,0,416,420,0,0,0,417,418,419,0,0],"FLAG":0,"BASE":18}
LWE3 600lm                   {"NAME":"Linganzh LWE3 ","GPIO":[0,0,0,0,0,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
MagicLight 4.5W 350lm        {"NAME":"4.5W RGBW Bulb","GPIO":[0,0,0,0,0,416,0,0,418,419,417,0,0,0],"FLAG":0,"BASE":18}
Malmbergs G95 8,5W 810lm     {"NAME":"Malmbergs8,5W-RGBW","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Manzoku XS-001 1050lm        {"NAME":"Manzoku RGBW","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Maxcio YX-L01P-E27-2P 9W     {"NAME":"Maxcio YXL01P","GPIO":[32,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
Melery 5W                    {"NAME":"MeleryMR16","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Merkury 75W 1050lm           {"NAME":"MIC-BW904-999W","GPIO":[417,0,0,0,2944,2912,0,0,416,2976,0,0,0,0],"FLAG":0,"BASE":18}
Merkury A21 10W 1050lm       {"NAME":"MI-BW210-999W","GPIO":[417,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":69}
Merkury A21 10W 1050lm       {"NAME":"MI-BW210-999W","GPIO":[0,0,0,0,418,417,0,0,0,416,419,0,0,0],"FLAG":0,"BASE":18}
Merkury BR30 8W 750lm        {"NAME":"MI-BW906-999W","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Merkury MI-BW904-999W 1050lm  {"NAME":"MI-BW904-999W","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Merkury MI-BW904-999W v2 1050lm  {"NAME":"MI-BW210-999W","GPIO":[0,0,0,0,417,416,0,0,2944,2976,2912,0,0,0],"FLAG":0,"BASE":48}
Merkury MI-BW904-999W v3     {"NAME":"MI-BW904-999W","GPIO":[0,0,0,0,416,417,0,0,2944,2976,2912,0,0,0],"FLAG":0,"BASE":69}
Mimoodz A19 6.5W             {"NAME":"Miimoodz RGBCW LED","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18}
Mirabella 9W 800lm           {"NAME":"MiraBellaGenio","GPIO":[0,0,0,0,0,0,0,0,4066,0,4032,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"GenioBulbRGB","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"MiraBellaGenio","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Mirabella Genio 9W 800lm     {"NAME":"MiraBellaGenio","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Mixigoo 950lm                {"NAME":"Mixigoo Bulb RGBCCT","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
MoKo GU10                    {"NAME":"MoKo GU10","GPIO":[1,1,1,1,418,1,1,1,417,420,416,419,1,1],"FLAG":0,"BASE":18}
MoKo JL81 5W 400lm           {"NAME":"MoKo E14","GPIO":[0,0,0,0,0,0,0,0,3008,0,3040,0,0,0],"FLAG":0,"BASE":27}
MOKO YX-L01C-E14 A60 810lm   {"NAME":"MOKO","GPIO":[32,0,0,0,3008,3040,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":27}
Nedis 4.5W 380lm             {"NAME":"Nedis RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Nedis 6W 450lm               {"NAME":"WIFILC10WTB22","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Nedis 6W 470lm               {"NAME":"nedis Bulb","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Nedis A60 800lm              {"NAME":"Nedis RGBW","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,4704],"FLAG":0,"BASE":18}
Nedis C10 350lm              {"NAME":"Nedis WIFILC10","GPIO":[0,0,0,0,418,416,0,0,419,417,420,0,0,4704],"FLAG":0,"BASE":18}
Nedis PAR16 330lm            {"NAME":"Nedis GU10","GPIO":[0,0,0,0,418,416,0,0,419,417,420,0,0,0],"FLAG":0,"BASE":18}
NGteco                       {"NAME":"NGTECO L100","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,0],"FLAG":0,"BASE":18}
Novostella UT55506 10W 1050lm  {"NAME":"Novostella 10W","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Onforu 7W 700lm              {"NAME":"Onforu RGBW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Orbecco 5W 400lm             {"NAME":"Orbecco Bulb","GPIO":[0,0,0,0,0,0,0,0,3008,0,3040,0,0,0],"FLAG":0,"BASE":27}
Positivo 4.5W 350lm          {"NAME":"V-TAC VT5164","GPIO":[0,0,0,0,0,0,0,0,4064,0,4032,0,0,0],"FLAG":0,"BASE":18}
Premier A19 10W              {"NAME":"Premier Light","GPIO":[0,0,0,0,0,418,0,0,417,0,416,419,0,4704],"FLAG":0,"BASE":18}
REPSN G45 5W 500lm           {"NAME":"REPSN RGBW E14","GPIO":[0,0,0,0,0,0,0,0,4032,0,4064,0,0,0],"FLAG":0,"BASE":18}
Riversong Juno 10W           {"NAME":"Juno10","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Rogoei EBE-QPZ04 6.5W 450lm  {"NAME":"EBE-QPZ04","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18}
Saudio 7W 700lm              {"NAME":"X002BU0DOL","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Sengled                      {"NAME":"Sengled RGBW","GPIO":[0,0,0,0,0,0,0,0,417,416,419,418,0,0],"FLAG":0,"BASE":18}
Shelly Duo RGBW 5W 400lm     {"NAME":"Shelly Duo RGBW","GPIO":[0,0,0,0,0,419,0,0,417,416,418,0,0,0],"FLAG":0,"BASE":18}
Shelly Duo RGBW 9W 800lm     {"NAME":"Shelly Duo RGBW","GPIO":[0,0,0,0,0,419,0,0,417,416,418,0,0,0],"FLAG":0,"BASE":18}
Smart 810lm                  {"NAME":"OOOLED 60W RGB","GPIO":[0,0,0,0,418,419,0,0,416,0,417,0,0,4704],"FLAG":0,"BASE":18}
SmartLED 9W 400lm            {"NAME":"SmartLED RGBWW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Smartyfi 600lm               {"NAME":"SMARTYFI 9W","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Smitch 7W                    {"NAME":"Smitch RGB 7W SB-1602","GPIO":[0,0,0,0,2912,419,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Solimo 12W                   {"NAME":"Solimo RGBCCT 12","GPIO":[0,0,0,0,416,420,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Solimo 810lm                 {"NAME":"Solimo RGBWW 9","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
SPC Sirius 380               {"NAME":"SPC Sirius 380","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Swisstone 350lm              {"NAME":"SH 320","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
SwissTone 4.5W 380lm         {"NAME":"SH 360","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,1],"FLAG":0,"BASE":18}
Swisstone 806lm              {"NAME":"SH 340","GPIO":[0,0,0,0,2912,416,0,0,0,2976,2944,0,0,1],"FLAG":0,"BASE":18}
Syska 7W 480lm               {"NAME":"Syska","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Syska 8W                     {"NAME":"Syska SMW-8W-C","GPIO":[0,0,0,0,420,419,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Syska 9W 720lm               {"NAME":"SyskaSmartBulb","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
TCP Smart 5W                 {"NAME":"TAOLGU42OWW25RGBW","GPIO":[0,0,0,0,417,416,0,0,419,418,420,0,0,0],"FLAG":0,"BASE":18}
TCP Smart 9W 806lm           {"NAME":"TCP Smart RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Teckin 7.5W 800lm            {"NAME":"Teckin SB60","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Teckin SB50 800lm            {"NAME":"Teckin SB50","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
Teckin SB50 v2 800lm         {"NAME":"Teckin SB50","GPIO":[0,0,0,0,416,0,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
Teckin SB51 800lm            {"NAME":"Teckin SB51","GPIO":[0,0,0,0,419,0,0,0,417,418,416,0,0,0],"FLAG":0,"BASE":18}
TH3D EZBulb V1               {"NAME":"EZBulb V1","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
TikLOk TL530 A19 7.5W 800lm  {"NAME":"TikLOk WW-CW-L","GPIO":[0,0,0,0,0,416,0,0,417,0,0,0,0,0],"FLAG":0,"BASE":18}
TVLive 7.5W 800lm            {"NAME":"TVLIVE RGBCW","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Utorch LE7 600lm             {"NAME":"Utorch LE7","GPIO":[0,0,0,0,0,417,0,0,418,0,419,416,0,0],"FLAG":0,"BASE":18}
V-Tac A60 11W 1055lm         {"NAME":"V-TAC LED A60 ","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18}
V-Tac A66 15W 1300lm         {"NAME":"V-TAC VT-5117","GPIO":[0,0,0,0,4032,0,0,0,0,0,4064,0,0,0],"FLAG":0,"BASE":18}
V-TAC A95 18W 1350lm         {"NAME":"V-TAC VT-5021","GPIO":[0,0,0,0,416,420,0,0,417,419,418,0,0,0],"FLAG":0,"BASE":18}
V-TAC G45 4.5W 300lm         {"NAME":"V-TAC VT-5124","GPIO":[0,0,0,0,0,0,0,0,4065,0,4032,0,0,0],"FLAG":0,"BASE":18}
Vivitar 1100lm               {"NAME":"Vivitar (Tuya) RGBW Lamp","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Wallfire WF-05               {"NAME":"Wallfire E27","GPIO":[32,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Wipro Garnet NS7001 480lm    {"NAME":"Wipro Garnet NS7001","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
Wixann C37 5W 450lm          {"NAME":"WIXANNE12","GPIO":[0,0,0,0,416,419,0,0,417,420,418,0,0,0],"FLAG":0,"BASE":18}
Woopower 460lm               {"NAME":"Woopower E14","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
WOOX 4W 350lm                {"NAME":"WOOX R4553","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
WOOX R4553 650lm             {"NAME":"WOOX R4553","GPIO":[0,0,0,0,416,419,0,0,417,0,418,0,0,0],"FLAG":0,"BASE":18}
WOOX R5077                   {"NAME":"WOOX R5077","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
Wyze Bulb Color              {"NAME":"Wyze Bulb Color","GPIO":[0,0,0,0,0,0,0,0,0,418,416,419,0,0,0,0,0,0,0,224,0,0,417,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Xiaomi Mi LED Smart Bulb Essential  {"NAME":"Mi LED Smart Bulb Essential","GPIO":[0,0,0,0,418,419,0,0,416,417,0,0,0,0],"FLAG":0,"BASE":18}
Zemismart 5W                 {"NAME":"Zemismart_GU10","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Zemismart 5W 480lm           {"NAME":"Zemismart-E14-RGBW","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Zemismart A19 10W            {"NAME":"Zemism_E27_A19","GPIO":[0,0,0,0,0,0,0,0,0,3008,0,3040,0,0],"FLAG":0,"BASE":27}
Zilotek A19 800lm            {"NAME":"Zilotek RGBW","GPIO":[0,0,0,0,2912,416,0,0,417,2976,2944,0,0,0],"FLAG":0,"BASE":18}
```

## Relay
```
Shelly Pro 3                 {"NAME":"ShellyPro3","GPIO":[1,1,224,1,225,1,1,1,1,1,1,1,1,1,5600,1,0,1,1,5568,0,1,1,1,0,0,0,0,226,1,96,1,4736,97,162,1],"FLAG":0,"BASE":1}
Topgreener                   {"NAME":"Topgreener TGWFRSM1","GPIO":[0,32,0,0,0,0,0,0,0,160,224,0,0,0],"FLAG":0,"BASE":18}
```

## Relay Board
```
2 Channel Tuya               {"NAME":"TY-DIY-S02","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 12,2 | TuyaMCU 13,13 | TuyaMCU 1,101"}
Anmbest 2 Channel Inching Self-locking Switch Module  {"NAME":"Generic","GPIO":[32,1,1,1,1,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Aptinex IOT RelayNode 4 Channel  {"NAME":"APTINEX","GPIO":[0,0,1,0,0,0,0,0,224,225,226,227,0,0],"FLAG":0,"BASE":18}
Armtronix Quad               {"NAME":"Armtronix Wifi Four Relay Board","GPIO":[1,0,0,0,224,1,0,0,225,226,227,0,1,0],"FLAG":0,"BASE":7}
Athom 1Ch Inching/Self-locking  {"NAME":"Athom R01","GPIO":[1,1,1,1,1,224,1,1,1,1,1,1,576,0],"FLAG":0,"BASE":18}
Athom 8Ch Inching/Self-locking 10A  {"NAME":"Athom R08","GPIO":[229,1,1,1,230,231,1,1,226,227,225,228,224,0],"FLAG":0,"BASE":18}
Claudy 5V                    {"NAME":"CLAUDY","GPIO":[0,0,225,0,0,0,0,0,0,0,0,224,0,0],"FLAG":0,"BASE":18}
Devantech 8x16A              {"NAME":"ESP32LR88","GPIO":[0,0,231,0,32,35,0,0,229,230,228,0,33,34,36,37,0,38,39,544,0,225,226,227,0,0,0,0,0,224,3232,3200,0,0,0,0],"FLAG":0,"BASE":1}
Dingtian 16 Channel          {"NAME":"Dingtian DT-R008","GPIO":[1,9408,1,9440,1,1,1,1,1,9760,9729,9856,9792,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,9824,9952,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Dingtian 32 Channel          {"NAME":"Dingtian DT-R008","GPIO":[1,9408,1,9440,1,1,1,1,1,9760,9731,9856,9792,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,9824,9952,1,1,1,0,0,1],"FLAG":0,"BASE":1}
Dingtian 4 Channel           {"NAME":"Dingtian DT-R004","GPIO":[0,9408,225,9440,0,0,0,0,227,9952,0,0,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,226,162,0,163,160,0,0,161],"FLAG":0,"BASE":1}
Dingtian 8 Channel           {"NAME":"Dingtian DT-R008","GPIO":[1,9408,1,9440,1,1,1,1,1,9760,9728,9856,9792,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,9824,9952,1,1,1,0,0,1],"FLAG":0,"BASE":1}
DoHome HomeKit DIY Switch    {"NAME":"DoHome DIY","GPIO":[1,1,0,1,96,544,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":1}
Eachen ST-DC2                {"NAME":"Garage Control","GPIO":[162,0,0,0,226,225,33,0,224,288,163,227,0,4704],"FLAG":0,"BASE":18}
Eachen ST-DC4                {"NAME":"Eachen_ST-DC4","GPIO":[160,1,1,1,226,225,1,1,224,544,1,227,1,0],"FLAG":0,"BASE":54}
Eachen ST-UDC1               {"NAME":"ST-UDC1","GPIO":[160,0,0,0,0,0,0,0,224,320,0,0,0,4704],"FLAG":0,"BASE":18}
Electrodragon Board SPDT     {"NAME":"ED Relay Board","GPIO":[1,1,1,1,1,1,0,0,224,225,1,1,288,4704],"FLAG":0,"BASE":18}
Electrodragon ESP8266        {"NAME":"ElectroDragon","GPIO":[33,1,32,1,1,1,0,0,225,224,1,1,288,4704],"FLAG":0,"BASE":15}
Electrodragon Inductive Load  {"NAME":"ED RelayBoard IL","GPIO":[0,0,1,0,1,1,0,0,224,225,1,1,288,0],"FLAG":0,"BASE":18}
ESP-01 Relay V4.0            {"NAME":"ESP01v4","GPIO":[256,320,0,32,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
ESP-01 Relay V5.0            {"NAME":"ESP01v5","GPIO":[256,320,0,32,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
ESP-01S 5V Relay Module V1.0  {"NAME":"ESP-01S Relay","GPIO":[256,288,1,1,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
ESP-12F 5V/7-28V 1 Channel 30A  {"NAME":"Aideepen","GPIO":[0,0,0,0,0,288,0,0,0,0,0,0,224,0],"FLAG":0,"BASE":18}
ESP-12F 5V/7-28V 4 Channel 30A  {"NAME":"ESP12F_Relay_30A_X4","GPIO":[1,1,1,1,32,1,1,1,226,227,225,1,224,1],"FLAG":0,"BASE":18}
ESP-12F 5V/7-28V 8 Channel   {"NAME":"ESP12F_Relay_X8","GPIO":[229,1,1,1,230,231,0,0,226,227,225,228,224,1],"FLAG":0,"BASE":18}
ESP-12F 5V/7-28V 8 Channel   {"NAME":"ESP12F_Relay_X8_v1.1","GPIO":[230,1,231,229,1,1,1,1,226,225,227,224,228,1],"FLAG":0,"BASE":18}
ESP-12F 5V/7-30V/220V 4 Channel  {"NAME":"ESP12F_Relay_X4","GPIO":[1,1,320,1,1,321,1,1,226,227,225,1,224,1],"FLAG":0,"BASE":18}
ESP-12F 5V/8-80V 2 Channel   {"NAME":"LC-Relay-ESP12-2R-D8","GPIO":[1,1,1,1,224,225,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":18}
ESP-12F DC 5V/12V/24V 16 Channel  {"NAME":"ESP12F_Relay_X16","GPIO":[1,1,1,1,1,7712,1,1,7680,7648,7744,1,1,1],"FLAG":0,"BASE":18}
ESP32 4 Channel              {"NAME":"RobotDyn ESP32R4","GPIO":[0,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,224,225,0,0,0,0,0,227,226,32,33,34,0,0,35],"FLAG":0,"BASE":2}
ESP32SR88 - WIFI 8 x 1A      {"NAME":"ESP32SR88","GPIO":[0,3200,0,3232,226,544,0,0,0,0,0,0,225,224,32,544,33,34,35,36,0,37,38,39,0,229,228,227,0,0,0,0,231,230,0,0],"FLAG":0,"BASE":1}
ESP8266 4 Channel 5V/12V     {"NAME":"4CH Relay","GPIO":[0,0,0,0,0,0,0,0,226,227,225,0,224,0],"FLAG":0,"BASE":18}
eWeLink PSF-B04 5V 7-32V 4 Channel  {"NAME":"eWeLink 4CH","GPIO":[160,0,0,0,226,225,161,162,224,288,163,227,0,0],"FLAG":0,"BASE":18}
Ewelink RF No Neutral 3 Channel  {"NAME":"Ewelink 3 Gang Module","GPIO":[32,0,0,0,225,226,33,34,224,544,0,0,0,0],"FLAG":0,"BASE":18}
Geekcreit 5V DIY 4 Channel Jog Inching Self-Locking  {"NAME":"Geekcreit-4ch","GPIO":[160,0,0,0,226,225,161,162,224,288,163,227,0,0],"FLAG":0,"BASE":18}
Geekcreit Module 220V 10A    {"NAME":"DIY ESP8266 Re","GPIO":[0,0,544,0,224,32,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
HW-622 ESP8266               {"NAME":"HW-622","GPIO":[0,0,544,0,224,32,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
KinCony 16 Channel           {"NAME":"KC868-A16 rev 1.4","GPIO":[32,1,1,1,640,608,1,1,1,1,1,1,1,1,5600,1,0,1,1,5568,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
KinCony 32 Channel MOSFET    {"NAME":"KC868-A32M","GPIO":[0,1,0,1,640,608,1,1,1,3200,1152,3232,1088,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,0,0,4704,4705,4707,0,0,4706],"FLAG":0,"BASE":1}
KinCony 6 Channel            {"NAME":"KC868-A6","GPIO":[0,1,0,1,640,768,1,1,1,1,3232,608,0,1,736,672,0,1,0,704,0,1,0,3200,0,0,0,0,1312,1184,4706,4707,4704,0,0,4705],"FLAG":0,"BASE":1}
KinCony 8 Channel            {"NAME":"KC868-A8","GPIO":[32,0,1120,0,640,608,0,0,0,1,1,1152,0,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1],"FLAG":0,"BASE":1,"CMND":"EthClockMode 3 | EthAddress 0 | EthType 0 | I2CDriver2 1"}
KinCony ESP32 4G 2 Channel   {"NAME":"KC868-A2","GPIO":[0,1,225,1,640,0,1,1,1,1,1184,224,608,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,3200,1312,1,3232,160,0,0,161],"FLAG":0,"BASE":1}
KinCony ESP32 Ethernet RS485 4G GSM  {"NAME":"KC868-A8S","GPIO":[1,1,480,1,640,608,1,1,1376,1,1312,1,1152,1,5600,1,0,1,1,5568,0,1,1,1,0,0,0,0,3232,3200,1,1,1,0,0,1],"FLAG":0,"BASE":1}
KinCony ESP32 SD Card DS3231 RTC Sensor  {"NAME":"KC868-ASR","GPIO":[32,1,1,1,0,225,1,1,1,1,1,0,0,1,480,224,0,1,289,288,0,1,640,608,0,0,0,0,1312,1184,1,0,1,0,0,1],"FLAG":0,"BASE":1}
KinCony Ethernet 16 Channel  {"NAME":"KC868-A16S","GPIO":[0,1,0,1,640,608,1,1,1,0,0,0,1152,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,3232,3200,4705,4706,4704,0,0,4707],"FLAG":0,"BASE":1}
KinCony Ethernet 32 Channel  {"NAME":"KC868-A32","GPIO":[0,1,0,1,641,609,1,1,1,608,3200,640,3232,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,0,0,4705,4707,4706,0,0,4704],"FLAG":0,"BASE":1}
KinCony Ethernet CAN Bus 8 Channel  {"NAME":"KC868-A8M","GPIO":[0,1,0,1,640,608,1,1,1,3200,1312,3232,1152,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,0,0,4704,4705,4706,0,0,4707],"FLAG":0,"BASE":1}
KinCony GSM 4 Channel        {"NAME":"KC868-A4S","GPIO":[0,1,0,1,640,1312,1,1,1,1,1184,0,608,1,5600,0,0,1,0,5568,0,1,0,0,0,0,0,0,3200,3232,1,0,4704,4705,4706,4707],"FLAG":0,"BASE":1}
KinCony IR+RF 4 Channel      {"NAME":"KC868-A4","GPIO":[32,0,227,0,224,225,0,0,0,1312,1,226,0,0,480,1152,0,1120,1056,1088,0,1,1,1,0,0,0,0,4706,4707,4704,4705,1,0,0,1],"FLAG":0,"BASE":1}
KRIDA 4 Channel 10A Electromagnetic  {"NAME":"4CH Relay","GPIO":[0,0,0,0,0,0,0,0,226,225,227,224,0,0],"FLAG":0,"BASE":18}
KRIDA 8 Channel 10A Electromagnetic  {"NAME":"8CH Relay","GPIO":[230,1,231,229,1,1,1,1,226,225,227,224,228,1],"FLAG":0,"BASE":18}
LC Technology 12V 4 Channel  {"NAME":"LC Technology 4CH Relay","GPIO":[224,0,225,0,226,227,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
LC Technology 5V 2 Channel   {"NAME":"LC-ESP01-2R-5V","GPIO":[0,3200,0,3232,0,0,0,0,224,225,0,0,0,0],"FLAG":0,"BASE":18}
LC Technology 5V 4 Channel   {"NAME":"LC-Tech_4CH ","GPIO":[288,1,32,1,0,0,0,0,224,225,226,227,0,0],"FLAG":0,"BASE":18}
LC Technology 5V/7-30V 8 Channel  {"NAME":"ESP32_Relay_X8","GPIO":[0,0,0,0,0,0,0,0,225,224,226,0,0,0,0,0,0,0,0,0,0,229,228,227,0,0,0,0,231,230,0,0,0,0,0,0],"FLAG":0,"BASE":1}
LC Technology 5V/8-80V 1 Channel  {"NAME":"LC-Relay-ESP12-1R-MV","GPIO":[1,1,544,1,1,224,1,1,1,1,1,1,321,1],"FLAG":0,"BASE":18}
LC Technology 5V/8-80V 1 Channel  {"NAME":"LC-Relay-ESP12-1R-D8","GPIO":[1,1,544,1,1,224,1,1,1,1,1,1,321,1],"FLAG":0,"BASE":18}
LC Technology AC90V-250V 1 Channel  {"NAME":"ESP32_Relay_AC_X1","GPIO":[1,1,1,1,1,1,1,1,1,1,1,1,224,1,1,1,0,1,1,544,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
LC Technology DC5-60V 1 Channel  {"NAME":"ESP32_Relay_X1","GPIO":[1,1,1,1,1,1,1,1,1,1,1,1,224,1,1,1,0,1,1,544,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
LC Technology DC5-60V 2 Channel  {"NAME":"ESP32_Relay_X2","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,224,225,0,0,0,0,0,544,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
LC Technology DC5-60V 4 Channel  {"NAME":"ESP32_Relay_X4","GPIO":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,544,0,226,227,1,0,0,0,0,224,225,1,1,1,0,0,1],"FLAG":0,"BASE":1}
LC Technology ESP8266 5V     {"NAME":"ESP8266-01S","GPIO":[224,3200,0,3232,0,0,0,0,0,0,0,0,0,4704],"FLAG":0,"BASE":18}
LilyGo T-Relay 5V 8 Channel  {"NAME":"LilyGo ESP32 Relay 8","GPIO":[1,1,1,1,1,231,1,1,227,226,1,1,1,1,230,229,0,228,1,1,0,544,1,1,0,0,0,0,225,224,1,1,1,0,0,1],"FLAG":0,"BASE":1}
LilyGO TTGO 4 Channel ESP32  {"NAME":"T-Relay ESP32","GPIO":[0,0,1,0,1,227,0,0,1,1,1,1,0,0,226,225,0,224,1,1,0,544,1,1,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
LinkNode R4                  {"NAME":"LinkNode R4","GPIO":[0,0,0,0,0,0,0,0,224,225,226,0,227,0],"FLAG":0,"BASE":18}
LinkNode R8                  {"NAME":"LinkNode R8","GPIO":[0,0,0,0,228,229,0,231,226,227,225,230,224,0],"FLAG":0,"BASE":18}
Mhcozy 5V                    {"NAME":"Portail","GPIO":[160,0,0,0,0,0,0,0,224,320,0,0,0,4704],"FLAG":0,"BASE":18}
Mhcozy 5V 2 Channel          {"NAME":"MHCOZY RF 2ch","GPIO":[32,0,0,0,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
RobotDyn 40A                 {"NAME":"RobotDyn ESP8266R40A","GPIO":[1,1,0,1,1,0,0,0,0,0,224,0,0,0],"FLAG":0,"BASE":18}
Sinilink 6-32V Real Time Clock  {"NAME":"Sinilink XY-WFBJ","GPIO":[0,0,544,0,7584,224,0,0,608,640,32,0,288,0],"FLAG":0,"BASE":18}
Sinilink DC5V Module         {"NAME":"Sinilink XY-WF5V","GPIO":[0,0,0,0,224,1,0,0,32,288,0,0,1,0],"FLAG":0,"BASE":18}
Sinilink DC6-36V Module      {"NAME":"Sinilink XY-WF5V","GPIO":[0,0,0,0,224,1,0,0,32,288,0,0,1,0],"FLAG":0,"BASE":18}
Sinilink MOS                 {"NAME":"Sinilink MOS","GPIO":[0,0,576,0,224,1,0,0,32,288,0,0,0,0],"FLAG":0,"BASE":18}
Sinilink Temperature Sensor  {"NAME":"XY-WFWT","GPIO":[1,1,320,1,225,224,1,1,608,640,32,1,33,1],"FLAG":0,"BASE":18}
Sinilink Thermostat          {"NAME":"XY-WFT1","GPIO":[0,0,544,0,480,224,0,0,320,1,32,0,0,4736],"FLAG":0,"BASE":18,"CMND":"BuzzerPwm 1"}
SiSAH Retro Fit 4 Channel    {"NAME":"Sisah 4ch","GPIO":[0,0,0,160,224,161,0,0,162,225,226,227,163,0],"FLAG":0,"BASE":18}
Sonoff 1 Channel Inching/Self-Locking  {"NAME":"1 Channel","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":12}
Sonoff RE5V1C 5V Inching/Selflock  {"NAME":"Sonoff RE5V1C","GPIO":[32,1,1,1,1,1,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Sonoff SV                    {"NAME":"Sonoff SV","GPIO":[32,1,0,1,1,1,0,0,224,320,1,0,0,4704],"FLAG":0,"BASE":3}
Yunshan 7-30V 10A            {"NAME":"Yunshan 10A","GPIO":[32,1,288,1,224,161,0,0,225,0,0,0,0,0],"FLAG":0,"BASE":18}
```

## Relay Module
```
Mini Smart Switch            {"NAME":"Mini Smart Switch","GPIO":[1,1,1,1,320,160,1,1,224,32,1,1,1,1],"FLAG":0,"BASE":18}
Smarteefi Retrofit 1 Node 16A  {"NAME":"Smarteefi NS11","GPIO":[2624,0,0,0,160,2720,0,0,0,2656,224,0,288,0],"FLAG":0,"BASE":18}
Tuya Mini Switch             {"NAME":"Tuya Mini Switch","GPIO":[0,1,0,32,320,0,0,0,0,224,160,0,0,0],"FLAG":0,"BASE":1}
```

## Sensor
```
Bresser 7-Kanal Thermo-/Hygrometer with Outdoor  {"NAME":"Bresser","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54,"CMND":"SO97 1 | TuyaMCU 73,2 | TuyaMCU 71,102"}
Genesense IoT Controller     {"NAME":"GNS24","GPIO":[32,1,1312,1,256,320,1,1,256,1216,160,3840,1,4704],"FLAG":0,"BASE":18}
Kaiweets Air Quality         {"NAME":"Kaiweets EH-8","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Shelly 3EM Power Monitoring Module  {"NAME":"Shelly 3EM","GPIO":[1,1,288,1,32,8065,0,0,640,8064,608,224,8096,0],"FLAG":0,"BASE":18}
Tuya Air Detector 6 in 1     {"NAME":"DCR-KQG","GPIO":[1,2272,544,2304,1,1,1,1,1,1,1,1,1,1],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 80,2 | TuyaMCU 71,18 | TuyaMCU 73,19 | TuyaMCU 99,20 | TuyaMCU 76,21 | TuyaMCU 77,22 | HumRes 1 | TempRes 1 "}
```

## Siren
```
Connex Smart Indoor          {"NAME":"Connex Siren","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
NEO Coolcam Temperature and Humidity 3in1 Alarm  {"NAME":"Neo Siren 3in1","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Smoke Sensor
```
Nedis Smoke Detector         {"NAME":"Nedis WIFIDS10WT","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Smoke Alarm                  {"NAME":"YG400A","GPIO":[1,2272,1,2304,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
VisorTech                    {"NAME":"VisorTech RWM-200","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Socket Adapter
```
Blitzwolf E27                {"NAME":"BlitzWolf LT-30","GPIO":[0,0,0,0,320,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Elegant Choice E27/E26       {"NAME":"name","GPIO":[0,0,0,0,0,0,0,0,0,0,0,224,0,0],"FLAG":0,"BASE":18}
Slampher                     {"NAME":"Slampher","GPIO":[32,1,0,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":9}
SmartBase E0260              {"NAME":"SmartBaseE0260","GPIO":[0,0,0,0,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Timeguard Lamp Holder        {"NAME":"Timeguard WFLH","GPIO":[0,0,0,0,576,320,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
```

## Soil Sensor
```
DIY MORE ESP32 DHT11         {"NAME":"DIYMORESOILDHT11","GPIO":[1,1,1,1,1,1,1,1,1,1,1,1,544,1,1,1,0,1,1184,1,0,1,1,1,0,0,0,0,4864,288,4865,1,1,0,0,1],"FLAG":0,"BASE":1}
```

## Soldering Iron
```
T13 100W PD3.0 Portable      {"NAME":"PTS200","GPIO":[0,0,33,0,34,0,0,0,6210,0,32,512,0,0,0,0,0,640,608,0,0,224,4704,0,0,0,0,0,0,4737,0,0,0,0,0,0],"FLAG":0,"BASE":1}
```

## Switch
```
3 Way Smart Light            {"NAME":"KS-602F","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
3A Smart Home                {"NAME":"3A Smart Home ","GPIO":[544,0,290,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
4 Gang Light Touch           {"NAME":"03O","GPIO":[32,0,0,0,226,225,33,34,224,260,35,227,0,0],"FLAG":0,"BASE":7}
AGL 2 Gang                   {"NAME":"AGL WiFi 02","GPIO":[0,0,544,0,0,33,0,0,225,224,0,0,32,0],"FLAG":0,"BASE":18}
AGL 3 Gang                   {"NAME":"AGL WiFi 03","GPIO":[0,0,544,0,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
AmmToo 433Mhz 2 Gang US Light  {"NAME":"Dual band touch switch","GPIO":[0,0,0,32,33,0,0,0,0,224,225,0,0,0],"FLAG":0,"BASE":18}
Aoycocr SW1                  {"NAME":"Aoycocr SW1","GPIO":[576,1,321,1,1,1,1,1,320,32,1,224,1,1],"FLAG":0,"BASE":18}
APPIO 1 Gang Switch          {"NAME":"Appio-9608","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,288,0],"FLAG":0,"BASE":18}
Appio 3 Gang Touch           {"NAME":"Appio-9611","GPIO":[0,0,0,0,224,33,0,0,34,226,32,225,288,0],"FLAG":0,"BASE":18}
Athom 1 Gang                 {"NAME":"Athom SW011EU","GPIO":[576,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Athom 1 Gang                 {"NAME":"Athom SW031US","GPIO":[576,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Athom 1 Gang No Neutral      {"NAME":"Athom SW111EU","GPIO":[576,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Athom 1 Gang No Neutral Touch  {"NAME":"Athom SW33-1US","GPIO":[576,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":18}
Athom 2 Gang                 {"NAME":"Athom SW012EU","GPIO":[576,289,0,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Athom 2 Gang                 {"NAME":"Athom SW032US","GPIO":[576,289,0,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Athom 2 Gang No Neutral      {"NAME":"Athom SW112EU","GPIO":[576,289,0,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Athom 2 Gang No Neutral Touch  {"NAME":"Athom SW33-2US","GPIO":[576,289,0,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":18}
Athom 3 Gang Key             {"NAME":"Athom SW01-TAS-3EU","GPIO":[576,290,1,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Athom 3 Gang No Neutral Touch  {"NAME":"Athom SW33-3US","GPIO":[576,290,0,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Athom 3 Gang Touch           {"NAME":"Athom SW03-TAS-3US","GPIO":[576,290,0,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Athom 3 Gang Touch           {"NAME":"Athom SW11-TAS-3EU","GPIO":[576,290,1,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Athom 4 Gang No Neutral Touch  {"NAME":"Athom SW33-4US","GPIO":[576,0,0,33,225,34,0,0,32,224,227,226,35,0],"FLAG":0,"BASE":18}
Athom 4 Gang Touch           {"NAME":"Athom SW03-TAS-4US","GPIO":[576,0,0,33,225,34,0,0,32,224,227,226,35,0],"FLAG":0,"BASE":18}
Athom 4 Gang Touch           {"NAME":"Athom SW11-TAS-4EU","GPIO":[576,0,0,33,225,34,0,0,32,224,227,226,35,0],"FLAG":0,"BASE":18}
Athom US Key Switch 1 Gang   {"NAME":"Athom SW13","GPIO":[576,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Avatto 1 Gang                {"NAME":"AVATTO 1 Gang","GPIO":[0,320,576,0,0,32,0,0,320,0,0,224,0,0],"FLAG":0,"BASE":18}
Avatto 2 Gang                {"NAME":"Avatto KS-601","GPIO":[0,0,288,0,0,32,0,0,224,225,0,0,33,0],"FLAG":0,"BASE":18}
Avatto 3 Gang                {"NAME":"AVATTO 2 Gang","GPIO":[0,0,576,32,225,0,0,0,321,224,320,0,33,0],"FLAG":0,"BASE":18}
Avatto 4 Gang                {"NAME":"AVATTO 4 Gang","GPIO":[576,0,0,32,224,33,0,0,34,226,227,225,35,0],"FLAG":0,"BASE":18}
Bakeey 10A RGB Scene         {"NAME":"RGB Switch","GPIO":[0,0,320,0,417,418,0,0,289,32,416,224,0,0],"FLAG":0,"BASE":18}
Bardi Smart Wallswitch 1 Gang  {"NAME":"Bardi 1 Gang","GPIO":[321,320,544,0,0,32,0,0,0,0,0,224,0,0],"FLAG":0,"BASE":18}
Bardi Smart Wallswitch 2 Gang  {"NAME":"BARDI 2 Gang","GPIO":[320,0,544,33,225,0,0,0,288,224,321,0,32,0],"FLAG":0,"BASE":18}
Bardi Smart Wallswitch 3 Gang  {"NAME":"BARDI 3 Gang","GPIO":[320,321,544,34,226,33,0,0,288,224,322,225,32,0],"FLAG":0,"BASE":18}
BAZZ SWTCHWFW1               {"NAME":"BAZZ KS-602S","GPIO":[32,0,0,0,0,0,224,288,256,320,0,0,0,0],"FLAG":0,"BASE":18}
Bingoelec 1 Gang Touch       {"NAME":"Bingoelec W601","GPIO":[0,544,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-SS3 1 Gang      {"NAME":"BW-SS3-1G-EU","GPIO":[288,0,0,32,0,0,0,0,0,224,0,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-SS3 2 Gang      {"NAME":"BW-SS3-2G-EU","GPIO":[544,1,1,1,225,33,1,1,32,224,1,1,1,1],"FLAG":0,"BASE":18}
BlitzWolf BW-SS3 3 Gang      {"NAME":"BlitzWolf SS3","GPIO":[576,0,0,161,225,162,0,0,160,224,0,226,0,0],"FLAG":0,"BASE":18}
BSEED 2 Gang 1 Way           {"NAME":"BSEED Switch 2Ch","GPIO":[0,0,0,33,225,0,0,0,32,224,0,288,289,0],"FLAG":0,"BASE":18}
CD303 3 Gang Touch           {"NAME":"Touch Switch 3","GPIO":[290,321,1,34,226,33,1,1,32,224,1,225,288,1],"FLAG":0,"BASE":18}
Cinlinele 4 Gang             {"NAME":"Cinlinele","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
CloudFree Light              {"NAME":"CloudFree SW1","GPIO":[32,1,0,1,0,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":1}
CloudFree Motion Light       {"NAME":"CloudFree SWM1","GPIO":[32,0,0,0,0,160,0,0,224,576,161,0,0,0],"FLAG":0,"BASE":18}
Connect SmartHome 2 Gang Wall  {"NAME":"CSH-SWTCH2","GPIO":[0,0,288,0,0,33,0,0,225,224,0,0,32,0],"FLAG":0,"BASE":18}
Connect SmartHome 3 Gang Wall Switch ()  {"NAME":"CSH-SWTCH3","GPIO":[0,0,288,0,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
Csmart US-B2                 {"NAME":"Csmart US-B2","GPIO":[0,544,0,33,32,0,0,0,0,225,224,0,0,0],"FLAG":0,"BASE":18}
Delock WLAN EASY-USB         {"NAME":"Delock 11828","GPIO":[0,0,0,32,0,0,0,0,0,576,224,0,0,0],"FLAG":0,"BASE":53}
Deta 1 Gang                  {"NAME":"Deta 1G Switch","GPIO":[0,0,0,0,544,0,0,0,0,224,0,0,64,0],"FLAG":0,"BASE":18}
Deta 2 Gang                  {"NAME":"DETA 2G Switch","GPIO":[0,0,0,0,544,0,0,0,65,224,225,0,64,0],"FLAG":0,"BASE":18}
Deta 3 Gang                  {"NAME":"DETA 3G Switch","GPIO":[544,0,0,66,65,224,0,0,226,0,225,0,64,0],"FLAG":0,"BASE":18}
Deta 4 Gang                  {"NAME":"Deta 4G Switch","GPIO":[576,0,0,34,33,224,0,0,226,35,225,227,32,0],"FLAG":0,"BASE":18}
DETA Mechanism               {"NAME":"Deta 6014HA","GPIO":[0,0,0,0,0,0,0,0,320,0,32,0,224,0],"FLAG":0,"BASE":18}
Dierya Touch Panel 2 Gang    {"NAME":"CD301","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Dierya Touch Panel 2 Gang    {"NAME":"CD302","GPIO":[544,0,0,33,225,0,0,0,32,224,321,0,320,0],"FLAG":0,"BASE":18}
Dierya Touch Panel 3 Gang    {"NAME":"CD303","GPIO":[576,289,0,34,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
Digoo DG-S811 3 Gang         {"NAME":"DIGOO Switch","GPIO":[0,0,0,0,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
DS-101 1 Gang                {"NAME":"Smart Life Switch","GPIO":[0,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":18}
DS-101 2 Gang                {"NAME":"Smart Life Switch","GPIO":[576,289,0,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":18}
DS-101 3 Gang                {"NAME":"DS-101 3 gang","GPIO":[576,322,0,33,226,34,0,0,32,225,321,224,320,0],"FLAG":0,"BASE":18}
DS-101 4 Gang Switch         {"NAME":"DS-101 4 Gang","GPIO":[544,0,0,33,225,34,0,0,32,224,227,226,35,0],"FLAG":0,"BASE":18}
DS-102 1 Gang                {"NAME":"DS-102 1 Gang","GPIO":[576,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":18}
DS-102 2 Gang                {"NAME":"DS-102 2 Gang","GPIO":[576,321,0,32,225,33,0,0,0,224,320,1,0,0],"FLAG":0,"BASE":18}
DS-102 3 Gang                {"NAME":"DS-102 3 Gang","GPIO":[576,322,0,33,225,34,0,0,32,224,321,226,320,0],"FLAG":0,"BASE":18}
DS-121 2 Gang No Neutral     {"NAME":"2 Gang Switch","GPIO":[576,321,0,32,225,33,0,0,0,224,320,0,0,0],"FLAG":0,"BASE":18}
DS-121 4 Gang No Neutral     {"NAME":"2 Gang Switch","GPIO":[576,321,0,32,225,33,0,0,0,224,320,0,0,0],"FLAG":0,"BASE":18}
Eachen 3CH                   {"NAME":"Eachen Smart Switch 3CH","GPIO":[34,0,0,0,224,225,33,32,226,320,0,0,0,0],"FLAG":0,"BASE":18}
Eachen CD303 3 Gang          {"NAME":"ID Components","GPIO":[544,289,0,162,224,161,0,0,160,225,290,226,288,1],"FLAG":0,"BASE":18}
Eachen SWT-2Gang             {"NAME":"ID Components","GPIO":[544,1,1,1,1,161,1,1,160,224,289,225,288,1],"FLAG":0,"BASE":18}
Earda 3-Way                  {"NAME":"ESW-1WAA-US","GPIO":[160,0,0,0,320,0,0,0,0,224,0,0,0,0],"FLAG":0,"BASE":18}
EJLINK RF433 2 Gang          {"NAME":"EJLINK EK02","GPIO":[32,0,0,0,0,225,33,0,224,544,0,0,0,0],"FLAG":0,"BASE":18}
Enjowi WF-SK301              {"NAME":"Tuya 3 Channel","GPIO":[0,0,0,0,226,33,0,0,32,224,34,225,544,0],"FLAG":0,"BASE":18}
Esooli 1 Gang                {"NAME":"Esooli 1G","GPIO":[0,288,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":1}
Esooli 2 Gang 1 Way          {"NAME":"Esooli 2 gang","GPIO":[0,288,0,32,33,0,0,0,0,224,225,0,0,0],"FLAG":0,"BASE":1}
Etekcity                     {"NAME":"EtekCityESWL01","GPIO":[0,1,0,1,288,289,0,0,0,224,96,0,0,4704],"FLAG":0,"BASE":18}
Etekcity 3-way               {"NAME":"Etekcity 3Way","GPIO":[0,0,0,0,226,256,0,0,192,225,161,0,0,0],"FLAG":0,"BASE":18}
Eva Logik 3-Way              {"NAME":"WF30 Switch","GPIO":[0,0,0,0,33,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
FrankEver 4 Gang             {"NAME":"FrankEver Wifi Smart Switch","GPIO":[0,0,0,32,224,33,1,1,34,226,227,225,35,0],"FLAG":0,"BASE":18}
Freecube                     {"NAME":"Freecube","GPIO":[288,0,289,0,0,0,0,0,290,32,0,224,0,0],"FLAG":0,"BASE":18}
Geeni TAP 3-Way              {"NAME":"Geeni 3-Way","GPIO":[544,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
Generic 2 Gang               {"NAME":"KING-Tuya-key","GPIO":[0,0,0,0,0,225,0,0,224,32,0,0,33,0],"FLAG":0,"BASE":18}
Girier 3 Gang Touch          {"NAME":"Girier JR-DES01","GPIO":[576,290,0,32,225,34,0,0,33,224,288,226,288,0],"FLAG":0,"BASE":18}
Girier EK01 RF433MHz 1 Gang  {"NAME":"Girier EK01","GPIO":[544,0,0,0,224,0,0,0,0,0,0,0,32,0],"FLAG":0,"BASE":18}
Girier EK02 RF433MHz 2 Gang  {"NAME":"Girier EK02","GPIO":[544,0,0,0,0,32,0,0,33,224,225,0,0,0],"FLAG":0,"BASE":18}
Girier EK03 RF433MHz 3 Gang  {"NAME":"EK03","GPIO":[544,0,0,0,225,32,0,0,34,224,226,0,33,0],"FLAG":0,"BASE":18}
Girier JRSWR-US01 No Neutral 1 Gang  {"NAME":"Tuya 1 Channel","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,544,0],"FLAG":0,"BASE":18}
Girier JRSWR-US01 No Neutral 3 Gang  {"NAME":"Girier JRSWR-U","GPIO":[0,0,0,0,224,33,0,0,34,226,32,225,544,0],"FLAG":0,"BASE":18}
Girier RF433 1 Gang No Neutral  {"NAME":"Girier","GPIO":[0,544,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Girier RF433 2 Gang No Neutral  {"NAME":"W602","GPIO":[0,0,0,0,225,0,0,0,32,224,33,0,544,0],"FLAG":0,"BASE":18}
Girier RF433 3 Gang No Neutral  {"NAME":"W603","GPIO":[0,0,0,0,226,33,0,0,32,224,34,225,544,1],"FLAG":0,"BASE":18}
GoKlug Glass Touch 1 Gang    {"NAME":"GoKlug 1x","GPIO":[320,321,0,0,0,160,0,0,0,0,0,224,0,0],"FLAG":0,"BASE":18}
GoKlug Glass Touch 2 Gang    {"NAME":"GoKlug 2x","GPIO":[576,0,0,193,224,0,0,0,320,225,321,0,192,1],"FLAG":0,"BASE":18}
Gosund 2 Gang                {"NAME":"GosundSW9","GPIO":[32,0,288,0,289,33,0,0,225,576,224,0,0,0],"FLAG":0,"BASE":18}
Gosund Single Pole           {"NAME":"Gosund SW1","GPIO":[32,0,321,0,0,0,0,0,0,0,224,0,320,0],"FLAG":0,"BASE":18}
Gosund Single Pole           {"NAME":"Gosund SW5","GPIO":[32,0,320,0,0,0,0,0,0,0,224,0,576,0],"FLAG":0,"BASE":18}
Gosund SW6 3-Way             {"NAME":"Gosund SW6","GPIO":[161,0,320,0,160,0,0,0,0,226,225,224,576,0],"FLAG":0,"BASE":18}
Hama Flush-mounted 2 Gang    {"NAME":"Hama WiFiTouch","GPIO":[544,0,0,0,0,33,0,0,32,225,0,224,0,0],"FLAG":0,"BASE":45}
HBN Wall-Mounted Timer       {"NAME":"HBN Timer Switch","GPIO":[0,0,0,0,290,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Hidin US Light               {"NAME":"Hidin MFA03","GPIO":[32,0,0,0,288,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":18}
Innens 1 Gang 1 Way          {"NAME":"Innens 1 Gang 1 Way","GPIO":[0,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Innens RF433 2 Gang 1 Way    {"NAME":"Innens Light Switch 2G","GPIO":[0,0,289,0,0,33,0,0,32,225,0,224,0,0],"FLAG":0,"BASE":18}
Jinvoo SM-SW101-1            {"NAME":"SM-SW101-1","GPIO":[288,0,0,33,0,0,0,0,32,224,0,0,0,4704],"FLAG":0,"BASE":18}
Jinvoo SM-SW101-2            {"NAME":"SM-SW101-2","GPIO":[288,0,0,33,225,0,0,0,32,224,0,0,0,4704],"FLAG":0,"BASE":18}
Jinvoo SM-SW101-3            {"NAME":"Jinvoo Wall Sw","GPIO":[288,0,0,33,225,34,0,0,32,224,0,226,0,4704],"FLAG":0,"BASE":18}
KaBuM! 15A 3 Gang Touch      {"NAME":"Kabum Switch 3","GPIO":[0,0,0,0,226,33,0,0,32,224,34,225,576,0],"FLAG":0,"BASE":18}
Kauf RGB Wall                {"NAME":"Kauf SRF10","GPIO":[448,1,450,1,450,449,1,1,449,32,448,224,1,1],"FLAG":0,"BASE":18}
Ketotek                      {"NAME":"KETOTEK KTSS1NU","GPIO":[0,0,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
KingArt 1 Gang               {"NAME":"KING-L1","GPIO":[0,0,0,0,0,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
KingArt 3 Gang               {"NAME":"KING-L3","GPIO":[0,0,0,0,226,225,0,0,224,164,165,0,167,0],"FLAG":0,"BASE":18}
KMC 70008                    {"NAME":"KMC 70008","GPIO":[32,1,1,0,0,1,0,0,0,320,224,1,1,0],"FLAG":0,"BASE":18}
Koaanw CD302-EU-1            {"NAME":"CD302-EU-1","GPIO":[0,0,0,0,544,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Koaanw CD302-EU-2            {"NAME":"CD302-EU-2","GPIO":[544,0,0,0,0,33,0,0,32,224,321,225,320,0],"FLAG":0,"BASE":18}
Koaanw CD302-EU-3            {"NAME":"CD302-EU-3","GPIO":[576,289,0,34,226,33,0,0,32,224,290,225,288,0],"FLAG":0,"BASE":18}
KS-601 2-way                 {"NAME":"2way Switch","GPIO":[1,1,576,1,1,193,0,0,225,224,1,192,1,0],"FLAG":0,"BASE":18}
KS-602                       {"NAME":"Gosund KS-602","GPIO":[32,0,0,0,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":18}
KS-605                       {"NAME":"KS-605","GPIO":[32,0,0,0,0,0,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
KS-611 3 Gang                {"NAME":"KS-611 3 Gang","GPIO":[0,0,576,0,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
KTNN-KG-T100 2 Gang Switch   {"NAME":"Sonoff T1 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Kuled K36                    {"NAME":"KULED-B","GPIO":[160,1,1,1,1,1,224,288,256,320,1,1,1,0],"FLAG":0,"BASE":18}
Kuled KS602S                 {"NAME":"KULED","GPIO":[32,1,1,1,1,1,0,0,224,320,1,1,1,0],"FLAG":0,"BASE":18}
Kygne CD-301                 {"NAME":"KYGNE Touch","GPIO":[0,0,0,0,288,289,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":10}
Laghten SS02S                {"NAME":"Laghten SS02S","GPIO":[0,0,0,0,288,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
LCARE Modular 2 Gang         {"NAME":"2SW1-In","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
LerLink 1 Gang               {"NAME":"LerLink X801","GPIO":[0,0,0,0,32,0,0,0,224,320,0,0,0,1],"FLAG":0,"BASE":18}
LerLink 1 Gang No Neutral    {"NAME":"LerLink X801-L","GPIO":[0,0,0,0,32,0,0,0,224,320,0,0,0,1],"FLAG":0,"BASE":18}
Lerlink 2 Gang               {"NAME":"Lerlink X802A","GPIO":[0,0,0,33,32,0,0,0,224,288,225,0,0,0],"FLAG":0,"BASE":18}
LerLink 2 Gang No Neutral    {"NAME":"Lonsonho 2gang","GPIO":[0,0,0,33,32,0,0,0,224,288,225,0,0,0],"FLAG":0,"BASE":18}
Lerlink 3 Gang               {"NAME":"X803A","GPIO":[0,0,0,33,32,34,0,0,224,259,225,226,288,0],"FLAG":0,"BASE":18}
Lerlink 3 Gang               {"NAME":"X803A","GPIO":[0,0,0,33,32,34,0,0,224,288,225,226,0,0],"FLAG":0,"BASE":18}
Lerlink 3 Gang No Neutral    {"NAME":"X803K-L 3 Gang","GPIO":[0,0,320,0,32,34,33,0,224,0,225,226,0,0],"FLAG":0,"BASE":18}
Lidl Silvercrest             {"NAME":"Lidl Silvercrest HG06337","GPIO":[0,0,0,0,0,0,0,0,32,320,224,0,0,0],"FLAG":0,"BASE":18}
Lightstory WT02S             {"NAME":"WT02S","GPIO":[0,0,0,0,321,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":50}
Linkind 2-Way                {"NAME":"Linkind WS240010008","GPIO":[0,0,0,0,0,224,0,0,0,0,288,0,0,0,0,0,0,0,0,0,0,576,321,0,0,0,0,0,33,32,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Linkind Dimmer               {"NAME":"Linkind Dimmer WS240010108","GPIO":[6213,8448,0,0,640,0,0,0,0,0,288,0,0,0,0,0,0,0,608,0,0,0,544,0,0,0,0,0,33,32,0,0,0,0,0,0],"FLAG":0,"BASE":1}
LoGiX Multi Button RGB Scene Controller  {"NAME":"Logix3","GPIO":[1,1,1,1,34,33,1,1,1376,32,1312,1,1,1],"FLAG":0,"BASE":18}
Lonsonho SK3-01              {"NAME":"Tuya 1 Channel","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,288,0],"FLAG":0,"BASE":18}
Lonsonho SK3-02              {"NAME":"Tuya 2 Channel","GPIO":[0,0,0,0,225,0,0,0,32,224,33,0,288,0],"FLAG":0,"BASE":18}
Lonsonho SK3-03              {"NAME":"Tuya 3-ch v2","GPIO":[544,322,0,33,225,34,0,0,32,224,321,226,320,0],"FLAG":0,"BASE":18}
LoraTap WH100W-US 20A        {"NAME":"LoraTap Boiler","GPIO":[0,0,0,0,0,0,0,0,32,224,0,0,320,0],"FLAG":0,"BASE":18}
Luminea LHC-101.on           {"NAME":"LHC-101.on","GPIO":[544,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Luminea LHC-102.on           {"NAME":"LHC-102.on","GPIO":[544,0,289,0,0,33,0,0,32,224,0,225,288,0],"FLAG":0,"BASE":18}
Luminea LHC-103.on           {"NAME":"LHC-103.on","GPIO":[544,0,289,34,290,33,0,0,32,224,226,225,288,0],"FLAG":0,"BASE":18}
LVT No Neutral Glass Touch   {"NAME":"LVT-5486 - 1 Gang","GPIO":[0,544,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
LX-WIFI-00M 4 Gang           {"NAME":"LX-WIFI-00M","GPIO":[32,228,1,1,226,225,33,34,224,0,35,227,0,0],"FLAG":0,"BASE":7}
MakeGood 2 Gang              {"NAME":"MakeGood 2 Gang","GPIO":[0,0,0,0,0,0,0,0,0,0,290,0,0,0],"FLAG":0,"BASE":54}
MakeGood 4 Gang              {"NAME":"MakeGood 4 Gang","GPIO":[0,0,0,0,0,0,0,0,0,0,290,0,0,0],"FLAG":0,"BASE":54}
Markevina KS-602S            {"NAME":"Markevina KS-6","GPIO":[32,1,0,1,0,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
Martin Jerry S01 15A         {"NAME":"MJ-S01 Switch","GPIO":[0,0,0,0,320,321,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Martin Jerry ST01 3 Way      {"NAME":"MJ 3Way Switch","GPIO":[1,1,1,1,288,289,0,0,224,160,544,1,0,0],"FLAG":0,"BASE":18}
Maxcio IT Wall 3 Gang Light  {"NAME":"Maxcio 3 Gang Switch","GPIO":[544,0,0,0,32,33,0,0,225,226,224,0,34,0],"FLAG":0,"BASE":18}
Merkury MI-WW107-199W        {"NAME":"MI-WW107-199W","GPIO":[288,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
Micmi K38                    {"NAME":"KS-605","GPIO":[32,0,0,0,0,0,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Milfra                       {"NAME":"Milfra KS-602H","GPIO":[32,0,0,0,0,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
Milfra 1 Gang                {"NAME":"Milfra MFC01","GPIO":[290,320,1,1,0,33,0,0,224,0,1,0,32,0],"FLAG":0,"BASE":8}
Milfra 1 Gang                {"NAME":"Milfra TB21","GPIO":[288,322,1,1,1,1,0,0,1,224,1,0,32,0],"FLAG":0,"BASE":8}
Milfra 2 Gang                {"NAME":"Milfra MFC01-2","GPIO":[290,0,0,0,0,33,0,0,225,224,0,0,32,0],"FLAG":0,"BASE":18}
Milfra 2 Gang                {"NAME":"Milfra TB12","GPIO":[576,320,289,290,34,33,0,0,225,224,480,0,32,0],"FLAG":0,"BASE":18}
Milfra 3 Gang                {"NAME":"Milfra TB23","GPIO":[320,320,289,290,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":8}
Milfra Mini Smart            {"NAME":"Milfra TB31","GPIO":[1,1,1,1,1,32,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":1}
Milfra Motion Sensor Dual Light  {"NAME":"Milfra MFA06","GPIO":[32,0,0,0,33,160,0,0,224,576,162,0,225,0],"FLAG":0,"BASE":18}
Milfra Motion Sensor Light   {"NAME":"Milfra MFA05","GPIO":[32,0,0,0,0,160,0,0,224,320,0,0,289,0],"FLAG":0,"BASE":18}
Milfra Vibration Touch       {"NAME":"Milfra MFA01","GPIO":[160,0,0,0,288,0,0,0,224,544,480,0,0,0],"FLAG":0,"BASE":18}
Minitiger 1 Gang             {"NAME":"minitiger 1 Gang","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Minitiger 1 Gang v2          {"NAME":"MiniTiger1BandNoNeutral","GPIO":[0,320,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Minitiger 2 Gang             {"NAME":"minitiger 2 Gang","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Minitiger 2 Gang No Neutral  {"NAME":"Tuya 2 Gang (Ali.Minitiger))","GPIO":[544,289,0,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Minitiger 2 Gang v2          {"NAME":"Minitiger2Band","GPIO":[0,320,0,32,33,0,0,0,0,224,225,0,0,0],"FLAG":0,"BASE":18}
Minitiger 3 Gang             {"NAME":"Minitiger3Band","GPIO":[0,288,0,32,34,33,0,0,225,224,226,289,0,0],"FLAG":0,"BASE":18}
Minitiger 4 Gang             {"NAME":"Minitiger 4 Gang","GPIO":[32,0,0,0,226,225,33,34,224,576,35,227,0,0],"FLAG":0,"BASE":18}
Moes 2 Gang                  {"NAME":"Moes WS-EU2-W","GPIO":[544,0,289,0,0,33,0,0,32,224,0,225,288,1],"FLAG":0,"BASE":18}
Moes 2-Way Multi-Control 1 Gang  {"NAME":"Moes WS-EU-SK1-W","GPIO":[544,0,0,32,224,0,0,0,0,0,288,0,0,1],"FLAG":0,"BASE":18}
Moes 3 Gang                  {"NAME":"Moes WS-EU3-W","GPIO":[544,0,290,33,225,34,0,0,32,224,289,226,288,1],"FLAG":0,"BASE":18}
Moes 3-Way                   {"NAME":"Moes 3-Way","GPIO":[1,1,1,1,224,321,0,0,257,161,160,1,1,0],"FLAG":0,"BASE":18}
Moes BS-US-W Boiler          {"NAME":"BS-US-W","GPIO":[290,0,0,32,224,0,0,0,0,0,288,0,291,0],"FLAG":0,"BASE":18}
Moes Light and Fan           {"NAME":"Moes WF-FL01","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Moes Push Button 3 Way Light  {"NAME":"Moes ESW-1WAA-US","GPIO":[32,0,0,0,288,0,0,0,0,224,161,0,0,0],"FLAG":0,"BASE":18}
Moes RF433 2 Gang Switch     {"NAME":"WS-EUB2-WR","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Moes RF433 3 Gang            {"NAME":"WS-EUB3-WR","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Moes RF433 3 Gang Touch      {"NAME":"WS-EU-RF","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 11,1 | TuyaMCU 12,2 | TuyaMCU 13,3"}
Moes SS01S-1                 {"NAME":"Moes Switch","GPIO":[1,1,1,1,320,0,0,0,224,32,1,1,1,0],"FLAG":0,"BASE":18}
Moes WS-EU1-LB 1 Gang No Neutral  {"NAME":"Moes WS-EU1-LB","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,544,0],"FLAG":0,"BASE":18}
Moes WS-EU2-LW 2 Gang No Neutral  {"NAME":"Tuya 2 Channel","GPIO":[0,0,0,0,225,0,0,0,32,224,33,0,544,0],"FLAG":0,"BASE":18}
Moes WS-EU3-LW 3 Gang No Neutral  {"NAME":"Tuya 3 Channel","GPIO":[0,0,0,0,224,34,0,0,33,225,32,226,544,0],"FLAG":0,"BASE":18}
Moes WS-EUY1 1 Gang          {"NAME":"WS-EUY1-W","GPIO":[544,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Moes WS-EUY2 2 Gang          {"NAME":"WS-EUY2-W","GPIO":[544,0,289,0,0,33,0,0,32,224,0,225,288,0],"FLAG":0,"BASE":18}
Moes WS-EUY3 3 Gang          {"NAME":"WS-EUY3-W","GPIO":[544,0,290,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Moes WS-US1-W 1 Gang         {"NAME":"WS-US1-W","GPIO":[290,0,0,32,224,0,0,0,0,0,288,0,291,0],"FLAG":0,"BASE":18}
Moes WS-US2-W 2 Gang         {"NAME":"WS-US2-W ","GPIO":[288,0,289,0,0,33,0,0,32,224,0,225,290,0],"FLAG":0,"BASE":18}
Moes WS-US3-W 3 Gang         {"NAME":"Tuya Moes 3 Ch","GPIO":[544,0,290,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Moes WT02S                   {"NAME":"Moes WT02S","GPIO":[0,0,0,0,320,576,0,0,224,160,0,0,0,1],"FLAG":0,"BASE":18}
MoKo 2 Gang                  {"NAME":"Moko2CH","GPIO":[544,0,289,0,0,33,0,0,32,224,0,225,288,0],"FLAG":0,"BASE":18}
MoKo Scene Life              {"NAME":"Moko Smart Swi","GPIO":[576,0,0,0,418,417,0,0,320,32,416,224,0,0],"FLAG":0,"BASE":18}
MoKo Smart Life              {"NAME":"Moko Switch (Single)","GPIO":[544,0,0,32,224,0,0,0,0,0,320,0,0,0],"FLAG":0,"BASE":59}
NaamaSmart KS602             {"NAME":"KS-602","GPIO":[32,0,0,0,0,0,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Nedis Dual                   {"NAME":"SM-SW102U-2","GPIO":[576,0,0,33,225,0,0,0,32,224,0,0,0,4704],"FLAG":0,"BASE":18}
NEO Coolcam 2Ch Touch Light  {"NAME":"Neo NAS-SC01W-2","GPIO":[0,0,0,0,225,0,0,0,32,224,33,0,544,0],"FLAG":0,"BASE":18}
Nexete DS-123                {"NAME":"DS-123","GPIO":[544,321,1,32,224,33,0,0,1,225,320,1,1,0],"FLAG":0,"BASE":18}
Nexete DS-123 Single         {"NAME":"DS-123","GPIO":[544,0,1,33,0,32,0,0,1,224,320,1,1,0],"FLAG":0,"BASE":18}
Novadigital Interruptor Touch Led 1 Boto  {"NAME":"Nova Digital Switch 1 Gang","GPIO":[544,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
PNI SmartHome 1 Key          {"NAME":"Tuya Switch 1 key","GPIO":[544,1,1,32,224,1,1,1,1,1,288,1,1,1],"FLAG":0,"BASE":18}
PNI SmartHome 2 Keys         {"NAME":"Tuya switch 2 key","GPIO":[544,0,289,0,0,32,0,0,33,225,0,224,288,0],"FLAG":0,"BASE":18}
Prosto                       {"NAME":"Prosto WFS-T10","GPIO":[0,0,0,0,0,224,0,0,320,0,64,0,0,0],"FLAG":0,"BASE":18}
Push Button 1/2/3/4 Gang     {"NAME":"DS-122","GPIO":[321,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":18}
Q-touch 1 Gang               {"NAME":"Qtouch","GPIO":[289,0,0,32,0,0,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":1}
Q-touch 433MHz 1 Gang        {"NAME":"Qtouch 1G","GPIO":[32,0,0,0,0,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
Q-touch 433MHz 1 Gang No Neutral  {"NAME":"QWP_W1_WIFI-TOUCH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Q-touch 433MHz 2 Gang No Neutral  {"NAME":"QWP_W2_WIFI-TOUCH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
QNCX Smart Breaker           {"NAME":"QNCX Smart breaker","GPIO":[0,0,0,0,160,224,0,0,0,320,0,0,0,0],"FLAG":0,"BASE":18}
Qualitel 1 Gang              {"NAME":"Qualitel 1 Gang","GPIO":[544,0,0,160,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Qualitel 2-Gang              {"NAME":"Qualitel 2 Gang","GPIO":[544,0,289,0,0,161,0,0,160,224,0,225,288,0],"FLAG":0,"BASE":18}
Qualitel 3 Gang              {"NAME":"Qualitel 3 Gang","GPIO":[544,0,290,161,225,162,0,0,160,224,289,226,288,0],"FLAG":0,"BASE":18}
Refoss AC/DC                 {"NAME":"Refoss-R10","GPIO":[1,1,0,1,224,192,0,0,2656,2720,32,1,2624,4736],"FLAG":0,"BASE":18}
RY-RSM104 Light Touch        {"NAME":"RY-RSM104","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
Sainko 1-Way                 {"NAME":"SAINKO 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sainko 2 Way                 {"NAME":"Sainko 2-Way","GPIO":[0,1,320,0,0,32,0,0,224,225,0,0,33,0],"FLAG":0,"BASE":18}
SANA SASW-03                 {"NAME":"SANA SASW-03","GPIO":[290,0,0,34,226,33,0,0,32,224,0,225,0,0],"FLAG":0,"BASE":18}
SANA SW02-02                 {"NAME":"SW02-02","GPIO":[258,1,1,33,225,0,0,0,32,224,1,0,1,0],"FLAG":0,"BASE":18}
SANA SW02-03                 {"NAME":"SW02-03","GPIO":[320,1,1,34,226,33,0,0,32,224,1,225,1,0],"FLAG":0,"BASE":18}
SavEnergy 3 Gang             {"NAME":"SavEnergy 3-sw","GPIO":[291,544,290,33,226,34,1,1,32,225,289,224,288,1],"FLAG":0,"BASE":18}
Semicom LM-HP/GEVD-W         {"NAME":"WaterHeater","GPIO":[576,320,0,0,0,32,0,0,0,0,0,224,0,0],"FLAG":0,"BASE":18}
Sesoo WIFI-EU-SK3-01         {"NAME":"Sensoo SK3-01","GPIO":[0,0,321,0,0,32,0,0,0,0,0,224,288,0],"FLAG":0,"BASE":18}
Sesoo WIFI-EU-SK3-02         {"NAME":"Sesoo SK3-02","GPIO":[0,0,321,0,225,0,0,0,32,224,33,0,288,0],"FLAG":0,"BASE":18}
Sesoo WIFI-US-SK3-04         {"NAME":"Tuya 4 Channel","GPIO":[288,1,1,34,226,32,0,0,35,227,225,224,33,0],"FLAG":0,"BASE":18}
Shawader 1Ch Touch Light     {"NAME":"ShawaderTuya1C","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,288,0],"FLAG":0,"BASE":18}
Shawader 2Ch Touch Light     {"NAME":"ShawaderTuya2C","GPIO":[0,0,0,0,224,0,0,0,33,225,32,0,288,0],"FLAG":0,"BASE":18}
Shawader 3Ch Touch Light     {"NAME":"ShawaderTuya3C","GPIO":[0,3872,0,66,64,65,0,0,225,226,224,0,0,0],"FLAG":0,"BASE":18}
SK-A801-01-US 1 Gang         {"NAME":"jsankou US Switch 1 Gang","GPIO":[544,0,0,0,0,0,0,0,32,256,0,0,0,0],"FLAG":0,"BASE":18}
SK-W803-01-US 3 Gang         {"NAME":"jsankou US Switch 3 Gang","GPIO":[544,0,0,33,257,34,0,0,32,256,0,258,0,0],"FLAG":0,"BASE":18}
Smart 3 Gang Touch           {"NAME":"ETERSKY KS601 3 Gang","GPIO":[0,0,544,0,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
Smart 3 in 1 Wall            {"NAME":"SW02-03","GPIO":[320,1,1,34,226,33,0,0,32,224,321,225,322,544],"FLAG":0,"BASE":18}
SmartGrade                   {"NAME":"SmartGrade AC 5005","GPIO":[0,0,0,32,2688,2656,0,0,2624,544,224,0,0,0],"FLAG":0,"BASE":18}
Smartlife Opard CD302        {"NAME":"CD302","GPIO":[0,0,0,0,288,321,0,0,256,32,0,0,0,0],"FLAG":0,"BASE":18}
SmartPlex 3 Gang             {"NAME":"Tuya 3 Channel","GPIO":[1,1,1,1,224,33,0,0,34,226,32,225,1,0],"FLAG":0,"BASE":18}
SmartVU Home Double Touch Light  {"NAME":"SHWSW2","GPIO":[544,0,288,0,0,32,0,0,33,225,0,224,289,0],"FLAG":0,"BASE":18}
SmartVU Home Touch Light     {"NAME":"SHWSW1","GPIO":[290,0,0,32,224,0,0,0,0,0,289,0,0,0],"FLAG":0,"BASE":18}
SmartVU Home Triple Touch Light  {"NAME":"SHWSW3","GPIO":[544,0,288,34,226,32,0,0,33,224,290,225,289,0],"FLAG":0,"BASE":18}
Smatrul 1 Gang RF No Neutral  {"NAME":"SMATRUL 1 GANG","GPIO":[0,544,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Smatrul 2 Gang RF No Neutral  {"NAME":"SMATRUL 2 GANG","GPIO":[0,544,0,32,33,0,0,0,0,224,225,0,0,0],"FLAG":0,"BASE":18}
Smatrul 5A RF433MHz 1 Gang Touch  {"NAME":"TMC01-EU","GPIO":[0,320,0,0,0,160,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Smatrul 5A RF433MHz 4 Gang Touch  {"NAME":"TMW4-01(EU)","GPIO":[0,0,0,33,35,32,0,0,34,224,225,226,227,0],"FLAG":0,"BASE":18}
Smatrul 5A RF433MHz 4 Gang Touch  {"NAME":"Smatrul RF433MHz 3 Gang Touch Switch (TMW4-01(EU))","GPIO":[0,0,0,160,162,161,0,0,225,224,226,0,0,0],"FLAG":0,"BASE":18}
Smatrul Infrared Sensor      {"NAME":"WHS-2","GPIO":[0,0,0,160,288,0,0,0,0,224,0,0,0,0],"FLAG":0,"BASE":18,"CMND":"SwitchMode1 4 | SO13 1"}
Sonoff IW101                 {"NAME":"Sonoff IW101","GPIO":[32,3072,0,3104,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":41}
Sonoff SwitchMan M5-1C 1 Gang  {"NAME":"Sonoff SwitchMan M5-1C-86","GPIO":[32,0,0,0,288,576,0,0,0,0,0,0,0,0,416,0,0,0,0,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff SwitchMan M5-2C 2 Gang  {"NAME":"Sonoff SwitchMan 2C","GPIO":[0,0,0,0,32,576,0,0,0,0,0,33,0,0,416,225,0,0,0,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff SwitchMan M5-3C 3 Gang  {"NAME":"Sonoff M5-3C","GPIO":[33,0,0,0,32,576,0,0,0,0,0,34,0,0,416,225,0,0,226,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff T1 EU 1 Gang          {"NAME":"Sonoff T1 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff T1 EU 2 Gang          {"NAME":"Sonoff T1 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff T1 UK 1 Gang          {"NAME":"Sonoff T1 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff T1 UK 2 Gang          {"NAME":"Sonoff T1 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff T1 UK 3 Gang          {"NAME":"Sonoff T1 3CH","GPIO":[32,1,1,1,226,225,33,34,224,320,0,0,0,0],"FLAG":0,"BASE":30}
Sonoff T1 US 1 Gang          {"NAME":"Sonoff T1 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff T1 US 2 Gang          {"NAME":"Sonoff T1 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff T1 US 3 Gang          {"NAME":"Sonoff T1 3CH","GPIO":[32,1,1,1,226,225,33,34,224,320,0,0,0,0],"FLAG":0,"BASE":30}
Sonoff Touch EU              {"NAME":"Sonoff Touch","GPIO":[32,1,0,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":10}
Sonoff Touch US              {"NAME":"Sonoff Touch","GPIO":[32,1,0,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":10}
Sonoff TX T0 EU 1 Gang       {"NAME":"Sonoff T0 TX 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff TX T0 EU 2 Gang       {"NAME":"Sonoff T0 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff TX T0 US 3 Gang       {"NAME":"TX T0US3C","GPIO":[32,1,0,1,226,225,33,34,224,576,0,0,0,0],"FLAG":0,"BASE":30}
Sonoff TX T1 EU 1 Gang       {"NAME":"Sonoff T1 TX 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff TX T1 EU 2 Gang       {"NAME":"Sonoff T1 TX 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff TX T2 EU 1 Gang       {"NAME":"Sonoff T2 TX 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff TX T2 EU 2 Gang       {"NAME":"Sonoff T2 TX 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff TX T2 UK 2 Gang       {"NAME":"Sonoff T1 TX 2CH","GPIO":[32,1,1,1,0,225,33,0,224,448,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff TX T3 EU 1 Gang       {"NAME":"Sonoff T3 TX 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff TX T3 EU 2 Gang       {"NAME":"Sonoff T3 TX 2CH","GPIO":[32,1,1,1,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":29}
Sonoff TX T3 EU 3 Gang       {"NAME":"TX T3EU3C","GPIO":[32,1,0,1,226,225,33,34,224,576,0,0,0,0],"FLAG":0,"BASE":30}
Sonoff TX T3 US 3 Gang       {"NAME":"TX T3US3C","GPIO":[32,1,0,1,226,225,33,34,224,576,0,0,0,0],"FLAG":0,"BASE":30}
Sonoff TX T4 EU No Neutral 1 Gang  {"NAME":"Sonoff T4 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Sonoff TX Ultimate 1 Gang    {"NAME":"TX Ultimate 1","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,0,224,3232,0,480,3200,0,0,0,3840,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate 2 Gang    {"NAME":"TX Ultimate 2","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,225,224,3232,0,480,3200,0,0,0,3840,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate 3 Gang    {"NAME":"TX Ultimate 3","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,225,224,3232,0,480,3200,0,0,0,3840,226,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate 4 Gang    {"NAME":"TX Ultimate 4","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,225,224,3232,0,480,3200,227,0,0,3840,226,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate US 1 Gang  {"NAME":"TX Ultimate 1","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,0,224,3232,0,480,3200,0,0,0,3840,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate US 2 Gang  {"NAME":"TX Ultimate 2","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,225,224,3232,0,480,3200,0,0,0,3840,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate US 3 Gang  {"NAME":"TX Ultimate 3","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,225,224,3232,0,480,3200,0,0,0,3840,226,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
Sonoff TX Ultimate US 4 Gang  {"NAME":"TX Ultimate 1","GPIO":[0,0,7808,0,7840,3872,0,0,0,1376,0,7776,0,0,224,3232,0,480,3200,0,0,0,3840,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"Backlog Pixels 28"}
SPC Hera                     {"NAME":"SPC HERA","GPIO":[544,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
SRL 2 Gang                   {"NAME":"SRL 4WW Switch","GPIO":[0,0,0,0,0,33,0,0,32,224,0,225,0,0],"FLAG":0,"BASE":18}
SRL 4 Gang                   {"NAME":"SRL 4WW Switch","GPIO":[0,0,0,34,226,33,0,0,32,224,227,225,35,0],"FLAG":0,"BASE":18}
SS118-01K1                   {"NAME":"SS118-01K1","GPIO":[1,1,1,32,224,1,0,0,1,1,320,1,1,0],"FLAG":0,"BASE":18}
SS86-AI 3-Gang               {"NAME":"SS86-AI 3 Gang","GPIO":[544,0,322,33,225,34,0,0,32,224,321,226,320,0],"FLAG":0,"BASE":18}
SSMS118-01A1 Scene Light Smart  {"NAME":"RGB Switch","GPIO":[257,0,259,161,418,417,0,0,258,160,416,224,0,0],"FLAG":0,"BASE":18}
Steren Apagador              {"NAME":"SHOME-115","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Steren Apagador Doble        {"NAME":"STEREN SHOME-116","GPIO":[0,0,288,0,0,32,0,0,224,225,0,0,33,0],"FLAG":0,"BASE":18}
Steren Apagador Triple       {"NAME":"STEREN SHOME-117","GPIO":[0,0,288,0,34,32,0,0,224,225,226,0,33,0],"FLAG":0,"BASE":18}
STITCH                       {"NAME":"Tuya WF15S ","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
TCP Smart 1 Gang             {"NAME":"TCP 1 Gang 1 Way","GPIO":[544,0,0,32,224,0,0,0,0,0,320,0,0,0],"FLAG":0,"BASE":18}
Teckin 2 Gang                {"NAME":"Teckin SR43","GPIO":[0,0,288,0,0,32,0,0,224,225,0,0,33,0],"FLAG":0,"BASE":18}
Teckin SR-41 Single Pole     {"NAME":"Teckin SR-41","GPIO":[32,0,0,0,0,0,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Teekar 10 Way 1 Gang         {"NAME":"Teekar 10way","GPIO":[160,161,162,35,164,165,0,0,166,167,0,0,0,0],"FLAG":0,"BASE":18}
Teekar Wi-Fi Light 1 Gang    {"NAME":"TeeKar Touch","GPIO":[0,0,1,0,1,224,0,0,0,1,32,0,1,0],"FLAG":0,"BASE":18}
Teepao Smart-Rollladen-Schalter  {"NAME":"Teepao","GPIO":[576,322,226,33,225,34,0,0,320,224,321,0,32,0],"FLAG":0,"BASE":18}
Tellur 1 Port 1800W 10A      {"NAME":"Tellur 1CH","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,544,0],"FLAG":0,"BASE":18}
Tellur 2 Ports 1800W 10A     {"NAME":"Tellur 2CH","GPIO":[0,0,0,0,225,0,0,0,32,224,33,0,544,0],"FLAG":0,"BASE":18}
Tonbux AMZ180648-2           {"NAME":"Tonbux","GPIO":[32,1,1,1,1,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":1}
TopGreener Scene Controller  {"NAME":"TGWF15RM","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
TopGreener TGWF15S           {"NAME":"TopGreener-Switch","GPIO":[0,0,0,0,0,0,0,0,0,2304,0,2272,0,0],"FLAG":0,"BASE":54}
Touch 3 Gang                 {"NAME":"Switch 3-Gang","GPIO":[0,0,0,0,226,33,0,0,32,224,34,225,544,0],"FLAG":0,"BASE":18}
Touch Light Switch 1 Gang    {"NAME":"Smart Touch Light Switch ","GPIO":[0,0,0,0,0,32,1,1,0,0,0,224,320,0],"FLAG":0,"BASE":18}
TreatLife                    {"NAME":"TL SS01S Swtch","GPIO":[0,0,0,0,288,576,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Treatlife 3-Way              {"NAME":"Treatlife SS01 3-Way","GPIO":[0,0,0,0,224,576,0,0,225,33,160,0,0,0],"FLAG":0,"BASE":18}
TreatLife 3-Way              {"NAME":"Treatlife 3-Way","GPIO":[0,0,0,0,224,576,0,0,225,33,160,0,0,0],"FLAG":0,"BASE":18}
TreatLife Single Pole ON/OFF  {"NAME":"Treatlife SS02","GPIO":[0,0,0,0,288,576,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Tuya 20A                     {"NAME":"DS-161","GPIO":[544,0,0,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Tuya 3 Gang                  {"NAME":"KING-Tuya-key","GPIO":[0,0,0,0,226,225,0,0,224,32,34,0,33,0],"FLAG":0,"BASE":18}
Tuya T1 2 Gang No Neutral    {"NAME":"Tuya T1 Wifi 2 Gang No Neutral Switch","GPIO":[32,0,0,0,0,225,33,0,224,544,0,0,0,0],"FLAG":0,"BASE":18}
Tuya T1 3 Gang No Neutral    {"NAME":"Tuya T1 Wifi 3 Gang No Neutral Switch","GPIO":[32,0,0,0,226,225,33,34,224,544,0,0,0,0],"FLAG":0,"BASE":18}
TY-US-L1-W                   {"NAME":"TY-US-L1-W","GPIO":[0,0,0,0,0,32,0,0,0,224,0,0,576,0],"FLAG":0,"BASE":18}
TY-US-L3-W                   {"NAME":"TY-US-L3-W","GPIO":[0,0,0,0,224,33,0,0,34,226,32,225,576,1],"FLAG":0,"BASE":18}
Useelink 1 Gang              {"NAME":"USEELINK-SW-1G","GPIO":[288,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
Useelink 2 Gang              {"NAME":"SM-SW101-2","GPIO":[576,288,0,33,225,0,0,0,32,224,0,0,0,4704],"FLAG":0,"BASE":18}
Useelink 3 Gang              {"NAME":"Useelink 3 Gang Wall Switch","GPIO":[576,288,0,33,225,32,0,0,34,226,289,224,288,0],"FLAG":0,"BASE":18}
Vaticas 1                    {"NAME":"Vaticas","GPIO":[0,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
VHome RF433 1 Gang           {"NAME":"VH-TW-CL-01","GPIO":[0,0,0,0,0,32,0,0,0,0,0,224,544,1],"FLAG":0,"BASE":18}
VHome RF433 2 Gang           {"NAME":"VX-TW-CL-002","GPIO":[0,0,0,0,224,0,0,0,33,225,32,0,544,1],"FLAG":0,"BASE":18}
vhome RF433 3 Gang           {"NAME":"VH-TB-US-003","GPIO":[0,0,0,0,224,33,0,0,34,226,32,225,576,1],"FLAG":0,"BASE":18}
Virage Labs VirageSwitch Light  {"NAME":"VirageSwitch","GPIO":[32,0,0,0,0,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
Wall Touch Switch 2 Gang     {"NAME":"VOVOWAY 120-WIFI-RF 2GANG","GPIO":[0,0,289,0,225,0,0,0,32,224,33,0,288,0],"FLAG":0,"BASE":18}
WiFi Smart Switch 2 Gang     {"NAME":"Kingart N2","GPIO":[32,1,0,1,0,225,33,0,224,0,0,0,0,1],"FLAG":0,"BASE":18}
WiFi Smart Switch 3 Gang     {"NAME":"KingArt-3CH","GPIO":[32,1,0,1,226,225,33,34,224,288,0,0,0,1],"FLAG":0,"BASE":18}
WS-US-03                     {"NAME":"WS-US-03","GPIO":[32,1,1,1,226,225,33,34,224,320,0,0,0,0],"FLAG":0,"BASE":30}
Xenon SM-SW102U 2 Gang       {"NAME":"SM-SW102U-2","GPIO":[288,0,0,33,225,0,0,0,32,224,0,0,0,4704],"FLAG":0,"BASE":18}
Xenon SM-SW202               {"NAME":"SM-SW202","GPIO":[0,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Yagusmart 2 Gang Wall Light  {"NAME":"DS-101 2 Gang","GPIO":[544,321,0,64,225,65,0,0,0,224,320,0,0,0],"FLAG":0,"BASE":18}
Yagusmart 3 Gang             {"NAME":"DS-101 3 gang","GPIO":[576,290,0,33,225,34,0,0,32,224,289,226,288,0],"FLAG":0,"BASE":18}
Yapmor 1-gang                {"NAME":"YAPMOR 1CH","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":28}
Youngzuth 2in1               {"NAME":"SW02 2W","GPIO":[288,0,0,32,224,0,0,0,33,225,0,0,0,0],"FLAG":0,"BASE":18}
Youngzuth 3in1               {"NAME":"SW02 3W","GPIO":[320,0,0,34,226,33,0,0,32,224,0,225,0,0],"FLAG":0,"BASE":18}
Zamel Foot                   {"NAME":"Zamel PNW-01","GPIO":[0,0,0,0,32,224,0,0,0,0,0,0,288,0],"FLAG":0,"BASE":18}
Zemismart KS-811 1 Gang      {"NAME":"KS-811 Single","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Zemismart KS-811 2 Gang      {"NAME":"KS-811 Dual","GPIO":[0,0,576,0,0,33,0,0,225,224,0,0,32,0],"FLAG":0,"BASE":18}
Zemismart KS-811 3 Gang      {"NAME":"KS-811 Triple","GPIO":[0,0,320,0,34,33,0,0,225,224,226,0,32,0],"FLAG":0,"BASE":18}
Zemismart RF Remote No Neutral 2 Gang  {"NAME":"DS-104-2","GPIO":[0,0,0,0,225,0,0,0,32,224,33,0,320,0],"FLAG":0,"BASE":18}
ZemiSmart SS118-01K2         {"NAME":"SS118-01K2","GPIO":[0,0,0,32,224,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Zemismart WF-BS01            {"NAME":"WF-BS01","GPIO":[289,0,0,32,224,0,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Zemismart ZM-L01E            {"NAME":"ZSmart ZM-L01E","GPIO":[1,1,1,1,1,1,0,0,1,224,1,1,32,0],"FLAG":0,"BASE":18}
Zemismart ZM-L02E            {"NAME":"ZSmart ZM-L02E","GPIO":[1,1,1,1,1,32,0,0,33,224,225,1,1,0],"FLAG":0,"BASE":18}
Zemismart ZM-L03E            {"NAME":"ZSmart ZM-L03E","GPIO":[288,289,0,0,226,32,0,0,34,224,225,0,33,0],"FLAG":0,"BASE":18}
ZUCZUG 1 Gang                {"NAME":"2ph105626a x1","GPIO":[0,288,0,0,0,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":1}
ZUCZUG 2 Gang                {"NAME":"2ph105626a x2","GPIO":[0,288,0,32,33,0,0,0,0,224,225,0,0,0],"FLAG":0,"BASE":1}
ZUCZUG 3 Gang                {"NAME":"2ph105626a x3","GPIO":[0,288,0,32,34,33,0,0,225,224,226,0,0,0],"FLAG":0,"BASE":1}
```

## Switch Module
```
2 CH Smart Switch            {"NAME":"Generic","GPIO":[32,1,1,1,1,225,33,1,224,288,1,1,1,1],"FLAG":0,"BASE":18}
AGL Modulo Relay 01 Canal    {"NAME":"AGL-Basic","GPIO":[0,1,0,0,224,32,0,0,0,0,320,0,0,0],"FLAG":0,"BASE":18}
Albohes 2 Channel            {"NAME":"Albohes SH-08","GPIO":[0,3200,33,3232,321,320,0,0,224,544,32,0,225,1],"FLAG":0,"BASE":18}
Athom 10A                    {"NAME":"CB01-TAS-1","GPIO":[0,0,0,32,320,0,0,0,0,224,0,0,0,1],"FLAG":0,"BASE":18}
Athom 2Ch Inching/Self-locking  {"NAME":"Athom R02","GPIO":[1,1,1,1,225,224,1,1,1,1,1,1,576,0],"FLAG":0,"BASE":18}
Athom 3-Way Mini Relay       {"NAME":"RS01-TAS-1","GPIO":[0,0,0,32,576,0,0,0,0,224,160,0,0,0],"FLAG":0,"BASE":18}
Athom 4Ch Inching/Self-locking 10A  {"NAME":"Athom R04","GPIO":[1,1,1,1,32,576,1,1,226,227,225,1,224,0],"FLAG":0,"BASE":18}
Athom 4Ch Inching/Self-locking 30A  {"NAME":"Athom R04-30A","GPIO":[1,1,1,1,32,576,1,1,226,227,225,1,224,0],"FLAG":0,"BASE":18}
ATMS1601 230VAC DIN Timer/Switch  {"NAME":"ATMS1601","GPIO":[1,1,1,1,544,320,1,1,224,32,1,1,1,1],"FLAG":0,"BASE":18}
Aubess Power Monitor Switch 16A  {"NAME":"Aubess with (BL0942)","GPIO":[0,3200,0,7520,0,0,0,0,160,0,224,0,0,0],"FLAG":0,"BASE":18}
BH OnOfre Dual Rev5 Silent Edition  {"NAME":"bhonofre","GPIO":[0,0,0,0,225,224,0,0,160,161,0,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-SS1             {"NAME":"BW-SS1","GPIO":[1,1,1,1,544,224,0,0,1,32,1,1,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-SS5 1 Gang      {"NAME":"BlitzWolf SS5 1 Gang","GPIO":[0,0,0,0,288,0,0,0,160,224,0,0,0,0],"FLAG":0,"BASE":18}
BlitzWolf BW-SS5 2 Gang      {"NAME":"BlitzWolf SS5 2 Gang","GPIO":[0,0,32,0,480,0,0,0,161,160,224,225,0,0],"FLAG":0,"BASE":18}
BlitzWolf SS4                {"NAME":"BlitzWolf SS4 Two Gang","GPIO":[0,0,0,0,320,224,0,0,225,32,0,0,0,0],"FLAG":0,"BASE":18}
Canwing CW-001               {"NAME":"Canwing CW-001","GPIO":[32,1,0,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Century Aoke Smart Switch    {"NAME":"CenturyAoke","GPIO":[0,1,0,1,224,0,0,0,32,320,1,0,0,0],"FLAG":0,"BASE":18}
Deta 6000HA Smart Inline Switch  {"NAME":"DETA-6000HA","GPIO":[0,32,0,0,0,0,0,0,0,320,224,0,0,0],"FLAG":0,"BASE":18}
dewenwils Outdoor Timer Box  {"NAME":"Dewenwils50054","GPIO":[0,0,290,0,0,0,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
EDM Low Voltage              {"NAME":"HVAC Switch","GPIO":[0,0,0,0,288,321,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
eMylo 2 Channel              {"NAME":"eMylo XL9252WI","GPIO":[0,1,0,0,320,225,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
eMylo Single Channel Switch  {"NAME":"eMylo XL9251WI","GPIO":[0,1,0,0,320,0,0,0,224,0,35,0,0,0],"FLAG":0,"BASE":18}
eMylo SS-8839-02             {"NAME":"SS-8839-02","GPIO":[0,1,0,1,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
eMylo SS-8839-03             {"NAME":"SS-8839-03","GPIO":[0,1,0,1,288,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
Ener-J Outdoor Switch        {"NAME":"Ener-J Smart WiFi Outdoor Relay","GPIO":[32,0,0,0,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":18}
eWelink No Neutral           {"NAME":"SA-018","GPIO":[0,0,0,0,160,0,0,0,224,288,0,0,0,0],"FLAG":0,"BASE":18}
EX Store 2 Kanal V5          {"NAME":"EXS Relay V5","GPIO":[1,1,1,1,1,1,0,0,224,225,258,288,259,0],"FLAG":0,"BASE":16}
Geekcreit 2 Channel AC 85V-250V  {"NAME":"Geekcreit 2ch","GPIO":[32,0,0,0,0,225,33,0,224,288,0,0,0,4704],"FLAG":0,"BASE":18}
Gosund SW3                   {"NAME":"Gosund WP3","GPIO":[0,0,0,32,0,0,0,0,320,321,224,0,0,0],"FLAG":0,"BASE":18}
HomCloud QS-WIFI-S05-EC 1 Channel  {"NAME":"QS-WIFI-S05-EC","GPIO":[0,3200,0,5056,288,224,0,0,0,160,0,0,0,0],"FLAG":0,"BASE":18}
HomeMate 4 Node In-wall      {"NAME":"HomeMate Wifi 4N ","GPIO":[1,1,1,160,224,163,1,1,161,225,226,227,162,1],"FLAG":0,"BASE":18}
L-5A01                       {"NAME":"L-5A01","GPIO":[32,1,0,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
LoraTap 10A                  {"NAME":"LoraTap RR400W","GPIO":[0,0,0,0,544,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
LoraTap RR500W               {"NAME":"LoraTap RR500W","GPIO":[544,0,0,0,160,0,0,0,32,224,0,0,320,0],"FLAG":0,"BASE":18}
LoveAnna AC85-250V 10A       {"NAME":"2xSwitch No RF LoveAnna","GPIO":[32,0,0,0,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Luani HVIO                   {"NAME":"Luani HVIO","GPIO":[0,1,1,1,224,225,0,0,160,161,1,288,0,4704],"FLAG":0,"BASE":35}
Luminea                      {"NAME":"Luminea NX-4651","GPIO":[0,0,0,0,288,0,0,0,160,224,0,0,0,0],"FLAG":0,"BASE":18}
MHCOZY DC 7-32V              {"NAME":"MHCOZY RF 1CH","GPIO":[32,0,0,0,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Milfra Smart                 {"NAME":"Milfra Smart Module TB41","GPIO":[576,0,0,225,2688,2656,0,0,2592,193,480,224,192,0],"FLAG":0,"BASE":18}
Mini Smart Switch            {"NAME":"SmartSwitch-MK601","GPIO":[0,0,0,160,32,224,0,0,0,0,288,0,0,0],"FLAG":0,"BASE":18}
Moes                         {"NAME":"Moes MS-104B","GPIO":[0,0,32,0,480,0,0,0,161,160,224,225,0,0],"FLAG":0,"BASE":18}
Moes 10A                     {"NAME":"Moes MS-101","GPIO":[0,0,0,0,0,320,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Moes Mini 3 Gang 1/2 Way     {"NAME":"Moes MS-104C","GPIO":[0,0,0,34,32,33,0,0,224,225,226,0,0,0],"FLAG":0,"BASE":18}
Nedis 10A                    {"NAME":"Nedis WIFIPS10WT","GPIO":[0,0,0,0,224,0,0,0,32,321,0,288,0,0],"FLAG":0,"BASE":18}
Nous 1 Channel Touch         {"NAME":"NOUS L1T","GPIO":[544,0,1,32,0,0,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Nous 1/2 Channel             {"NAME":"NOUS L13T Smart Switch Module","GPIO":[1,161,1,160,225,224,1,1,544,1,32,1,1,1],"FLAG":0,"BASE":18}
Nous 2 Channel Touch         {"NAME":"NOUS L2T","GPIO":[544,289,1,32,225,33,0,0,0,224,288,0,0,0],"FLAG":0,"BASE":1}
Nova Digital Basic 1 MS101   {"NAME":"NovaDigBasic1","GPIO":[0,1,0,1,320,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
ONiOFF Smart Switch          {"NAME":"ONOFF SmartSwitch","GPIO":[1,320,1,1,1,32,0,0,224,1,1,0,0,0],"FLAG":0,"BASE":1}
PPA Contatto Wi-Fi           {"NAME":"PPA Contatto","GPIO":[0,0,32,0,224,162,0,0,288,225,0,0,0,0],"FLAG":0,"BASE":18}
PS-1604 16A                  {"NAME":"PS-1604 16A","GPIO":[32,1,1,1,1,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":1}
QS-WIFI-S03                  {"NAME":"QS-WIFI-S03","GPIO":[32,1,1,1,1,0,0,0,192,224,0,0,0,0],"FLAG":0,"BASE":1}
QS-WIFI-S05                  {"NAME":"QS-WIFI-S05","GPIO":[32,1,1,1,1,0,0,0,192,224,0,0,0,0],"FLAG":0,"BASE":1}
Qualitel Breaker             {"NAME":"Qualitel Smart Switch","GPIO":[0,0,0,0,224,0,0,0,32,321,288,0,0,0],"FLAG":0,"BASE":18}
Shelly 1                     {"NAME":"Shelly 1","GPIO":[1,1,0,1,224,192,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":46}
Shelly 1L No Neutral         {"NAME":"Shelly 1L","GPIO":[320,0,0,0,192,224,0,0,0,0,193,0,0,4736],"FLAG":0,"BASE":18}
Shelly 1PM                   {"NAME":"Shelly 1PM","GPIO":[320,0,0,0,192,2720,0,0,0,0,0,224,0,4736],"FLAG":0,"BASE":18}
Shelly 2                     {"NAME":"Shelly 2","GPIO":[0,2752,0,2784,224,225,0,0,160,0,161,2816,0,0],"FLAG":0,"BASE":47}
Shelly 2.5                   {"NAME":"Shelly 2.5","GPIO":[320,0,0,0,224,193,0,0,640,192,608,225,3456,4736],"FLAG":0,"BASE":18}
Shelly EM                    {"NAME":"Shelly EM","GPIO":[0,0,0,0,0,0,0,0,640,3457,608,224,8832,1],"FLAG":0,"BASE":18}
Shelly i3 Action and Scenes Activation Device  {"NAME":"Shelly i3","GPIO":[0,0,0,0,0,320,0,0,193,194,192,0,0,4736],"FLAG":0,"BASE":18}
Shelly Plus 1                {"NAME":"Shelly Plus 1 ","GPIO":[288,0,0,0,192,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,32,224,0,0,0,0,0,4736,4705,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Shelly Plus 1PM              {"NAME":"Shelly Plus 1PM","GPIO":[0,0,0,0,192,2720,0,0,0,0,0,0,0,0,2656,0,0,0,0,2624,0,32,224,0,0,0,0,0,4736,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350"}
Shelly Plus 2PM              {"NAME":"Shelly Plus 2PM PCB v0.1.9","GPIO":[320,0,0,0,34,192,0,0,225,224,0,0,0,0,193,0,0,0,0,0,0,608,640,3458,0,0,0,0,0,9472,0,4736,0,0,0,0],"FLAG":0,"BASE":1}
Shelly Plus i4               {"NAME":"Shelly Plus i4","GPIO":[0,0,0,0,0,0,0,0,192,0,193,0,0,0,0,0,0,0,0,0,0,0,195,194,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"SwitchMode1 1 | SwitchMode2 1 | SwitchMode3 1 | SwitchMode4 1 | SwitchTopic 0 | SetOption114 1"}
Sinilink USB                 {"NAME":"XY-WFUSB","GPIO":[1,1,0,1,32,224,0,0,0,0,320,0,544,0],"FLAG":0,"BASE":18}
Smarsecur Smart Switch       {"NAME":"ESP-02S","GPIO":[0,0,0,32,0,0,0,0,0,0,224,0,0,0],"FLAG":0,"BASE":18}
Smart Home SS-8839-01        {"NAME":"SS-8839-01","GPIO":[0,1,0,1,224,0,0,0,32,321,0,320,0,0],"FLAG":0,"BASE":18}
Smart Switch                 {"NAME":"FL-S124-V1.0","GPIO":[1,1,1,1,32,224,1,1,1,320,1,1,1,1],"FLAG":0,"BASE":18}
Sonoff 4CH (R2)              {"NAME":"Sonoff 4CH","GPIO":[32,1,1,1,226,225,33,34,224,320,35,227,0,0],"FLAG":0,"BASE":7}
Sonoff 4CH Pro (R2)          {"NAME":"Sonoff 4CH Pro","GPIO":[32,1,1,1,226,225,33,34,224,320,35,227,0,0],"FLAG":0,"BASE":23}
Sonoff 4CHPROR3              {"NAME":"Sonoff 4CHPROR3","GPIO":[32,1,1,1,226,225,33,34,224,320,35,227,0,0],"FLAG":0,"BASE":23}
Sonoff 4CHR3                 {"NAME":"Sonoff 4CHR3","GPIO":[32,1,1,1,226,225,33,34,224,320,35,227,0,0],"FLAG":0,"BASE":7}
Sonoff Basic                 {"NAME":"Sonoff Basic","GPIO":[32,1,0,1,0,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":1}
Sonoff Basic R2              {"NAME":"Sonoff BASICR2","GPIO":[32,1,1,1,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Basic R3              {"NAME":"Basic R3","GPIO":[32,1,0,1,0,0,1,1,224,320,0,0,1,0],"FLAG":0,"BASE":1}
Sonoff Dual                  {"NAME":"Sonoff Dual","GPIO":[0,3200,0,3232,1,0,0,0,0,320,1,0,0,0],"FLAG":0,"BASE":5}
Sonoff Dual R2               {"NAME":"Sonoff Dual R2","GPIO":[1,1,0,1,0,225,1,32,224,320,0,0,0,0],"FLAG":0,"BASE":39}
Sonoff Dual R3               {"NAME":"Sonoff Dual R3","GPIO":[32,0,0,0,0,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,7296,7328,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Dual R3 Lite          {"NAME":"Sonoff Dual R3 Lite","GPIO":[32,0,0,0,0,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,0,0,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Dual R3 v2            {"NAME":"Sonoff Dual R3 v2","GPIO":[32,0,0,0,0,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,3200,8128,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Mini                  {"NAME":"Sonoff Mini","GPIO":[32,0,0,0,160,0,0,0,224,320,0,0,1,0],"FLAG":0,"BASE":1}
Sonoff Mini Extreme          {"NAME":"Sonoff MINIR4","GPIO":[32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,576,0,0,0,0,0,0,224,160,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Mini R2               {"NAME":"Sonoff MINIR2","GPIO":[32,0,0,0,160,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Pow                   {"NAME":"Sonoff Pow","GPIO":[32,0,0,0,0,2592,0,0,224,2656,2688,288,0,0],"FLAG":0,"BASE":6}
Sonoff POW Elite 16A         {"NAME":"Sonoff POWR316D","GPIO":[32,0,0,0,0,576,0,0,0,224,9280,0,3104,0,320,0,0,0,0,0,0,9184,9248,9216,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff POW Elite 20A         {"NAME":"Sonoff POWR320D","GPIO":[32,0,9313,0,9312,576,0,0,0,0,9280,0,3104,0,320,0,0,0,0,0,0,9184,9248,9216,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff POW Origin 16A        {"NAME":"Sonoff POWR316","GPIO":[32,0,0,0,0,576,0,0,0,224,0,0,3104,0,320,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff Pow R2                {"NAME":"Sonoff Pow R2","GPIO":[32,3072,0,3104,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":43}
Sonoff POW R3 25A 5500W      {"NAME":"Sonoff POWR3","GPIO":[32,3072,0,3104,0,0,0,0,256,320,0,1,1,1],"FLAG":0,"BASE":43}
Sonoff RF                    {"NAME":"Sonoff RF","GPIO":[32,1,1,1,1,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":2}
Sonoff TH Elite 16A Temperature and Humidity Monitoring  {"NAME":"Sonoff THR316D","GPIO":[32,0,0,0,225,9280,0,0,0,321,0,576,320,9184,9216,0,0,224,0,9248,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff TH Elite 20A Temperature and Humidity Monitoring  {"NAME":"Sonoff THR320D","GPIO":[32,0,0,0,226,9280,0,0,0,321,0,576,320,9184,9216,9312,0,0,9313,9248,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff TH Origin 16A         {"NAME":"Sonoff THR316","GPIO":[32,0,0,0,0,0,0,0,0,321,0,576,320,0,0,0,0,224,0,0,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff TH Origin 20A         {"NAME":"Sonoff THR320","GPIO":[32,0,0,0,0,0,0,0,0,321,0,576,320,0,0,9312,0,0,9313,0,0,1,0,3840,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff TH10/TH16             {"NAME":"Sonoff TH","GPIO":[32,1,0,1,1,0,0,0,224,320,1,0,0,0],"FLAG":0,"BASE":4}
Splatura USB Device Power Switch  {"NAME":"Splatura USB","GPIO":[0,0,288,0,0,0,0,0,0,224,0,96,0,0],"FLAG":0,"BASE":18}
SS-8839-02                   {"NAME":"SS-8839-02","GPIO":[0,1,0,1,320,0,0,0,224,0,32,0,0,0],"FLAG":0,"BASE":18}
SS311KWS RF Kinetic Switch and WiFi  {"NAME":"SS311KWS","GPIO":[0,0,0,0,288,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
SUPLA inCan by Espablo       {"NAME":"Supla Espablo","GPIO":[0,1,1312,1,32,224,0,0,1,225,1,0,288,4704],"FLAG":0,"BASE":31}
SW-R03                       {"NAME":"SW-R03","GPIO":[0,0,0,0,0,0,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Switch Module 2x5A           {"NAME":"QS-WIFI-S04-2C","GPIO":[1,1,32,1,480,0,0,0,161,160,224,225,0,0],"FLAG":0,"BASE":18}
Syrotech                     {"NAME":"Syrotech SY-LS80","GPIO":[0,0,0,0,224,32,0,0,0,320,320,0,0,0],"FLAG":0,"BASE":18}
Tinxy 1 Node 16A for AC/Geyser  {"NAME":"Tnxy16A","GPIO":[32,0,0,0,160,224,0,0,288,0,416,0,0,0],"FLAG":0,"BASE":18}
Tinxy Single Node 7A         {"NAME":"Tnxy07A","GPIO":[32,0,0,0,160,224,0,0,288,0,0,0,0,0],"FLAG":0,"BASE":18}
WL-SW01_10                   {"NAME":"WL-SW01_10","GPIO":[32,3232,0,3200,0,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Woox Integrational Switch    {"NAME":"WOOXR4967","GPIO":[0,0,0,1,320,224,0,0,0,32,0,0,0,0],"FLAG":0,"BASE":18}
Yeelight Dual Control        {"NAME":"YLAI002","GPIO":[0,0,0,0,0,0,0,0,96,0,224,0,0,320,320,160,0,160,0,0,0,0,0,225,0,0,0,0,162,163,0,0,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"SO127 1"}
Yuntong Smart                {"NAME":"Yuntong Smart","GPIO":[0,0,0,0,224,0,0,0,96,320,0,576,0,0],"FLAG":0,"BASE":1}
Zemismart ERC309 Kinetic     {"NAME":"Kinetic Switch","GPIO":[1,1,1,1,1,1,0,0,1,2304,1,2272,1,0],"FLAG":0,"BASE":54}
```

## Temperature Sensor
```
Coiaca Humidity and          {"NAME":"Coiaca AWR01THERMt","GPIO":[576,1216,0,32,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
DS18B20 ESP01 DIY            {"NAME":"ESP-01-01S-DS18B20-v1.0","GPIO":[1,1,1312,1,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
ESP01 DHT11 DIY              {"NAME":"ESP01S DHT11","GPIO":[1,1,1184,1,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
IOT4SH01DS                   {"NAME":"IOT4SH01DS","GPIO":[1,1,1,1,1,1,0,0,1,1312,1,1,1,1],"FLAG":0,"BASE":18}
Shelly Add-on                {"NAME":"Shelly 1 Temp ","GPIO":[1344,0,0,1312,224,192,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":46}
Shelly Plus Add-On           {"NAME":"Shelly Plus 1","GPIO":[1344,1312,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,224,0,0,0,0,0,4736,4705,0,0,0,0,0,0],"FLAG":0,"BASE":1}
```

## Thermostat
```
Floor Heating or Water/Gas Boiler  {"NAME":"ME81H Thermostat_8266","GPIO":[1,1,1,1,1,1,0,0,1,1,1,1,1,0],"FLAG":0,"BASE":54}
Lytko 101                    {"NAME":"Lytko 101","GPIO":[1,7584,1312,1,1792,1824,0,0,1,1,1,224,1,4736],"FLAG":0,"BASE":18}
Moes Floor Heating or Water/Gas Boiler Wall  {"NAME":"WHT-HY609-GB-WH-MS","GPIO":[0,2304,0,2272,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"tuyamcu 11,1 | tuyamcu 71,3 | tuyamcu 72,2 | tuyamcu 12,102"}
Mysa V1 Electric Baseboard Heater  {"NAME":"Mysa Thermostat","GPIO":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,0,0,0,0,0,0,640,608,0,0,0,0,0,0],"FLAG":0,"BASE":1}
```

## Valve
```
Garden Water Timer BQ05      {"NAME":"BQ05","GPIO":[32,0,0,0,0,0,0,0,224,544,0,0,0,4704],"FLAG":0,"BASE":18}
Hoenyzy DN20 3/4             {"NAME":"DN20 Valve","GPIO":[288,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
Jinvoo SM-AW713              {"NAME":"Jinvoo Valve","GPIO":[0,0,0,0,0,288,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Jinvoo SM-PW713              {"NAME":"Jinvoo Valve","GPIO":[0,0,0,0,224,288,0,0,32,289,0,0,0,4704],"FLAG":0,"BASE":18}
Moes Gas-Water               {"NAME":"MoesHouse WV-B","GPIO":[0,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
Owfeel EN71                  {"NAME":"SmartValve","GPIO":[288,0,0,0,0,0,0,0,32,224,0,0,0,0],"FLAG":0,"BASE":18}
Steren Valvula               {"NAME":"Steren_SHOME-150","GPIO":[0,0,0,0,0,0,0,0,544,0,32,0,224,0],"FLAG":0,"BASE":18}
Tuya Gas                     {"NAME":"Valve RQF-1T","GPIO":[0,0,0,0,0,0,0,0,0,0,32,0,224,0],"FLAG":0,"BASE":18}
Tuya Gas/Water               {"NAME":"Valve FM101","GPIO":[320,0,0,0,224,0,0,0,0,0,32,0,225,0],"FLAG":0,"BASE":18}
```

## Wall Outlet
```
2AC 1USB                     {"NAME":"SM-SW801U","GPIO":[0,0,0,0,288,32,0,0,224,0,225,0,226,0],"FLAG":0,"BASE":18}
Aigostar P40                 {"NAME":"Aigostar 8433325212278","GPIO":[0,0,0,0,544,288,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Aseer THWFS01                {"NAME":"ASEER-THWFS01","GPIO":[320,33,544,323,2720,2656,0,0,2624,225,321,224,32,0],"FLAG":0,"BASE":18}
Athom                        {"NAME":"Athom SK01","GPIO":[0,0,0,3104,0,32,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Athom 16A UK                 {"NAME":"Athom SK03-TAS","GPIO":[0,0,0,3104,0,32,0,0,224,576,0,0,0,0],"FLAG":0,"BASE":18}
Bestten LO-2-W               {"NAME":"BESTTEN LO-2-W","GPIO":[0,0,0,0,576,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
BingoElec 16A                {"NAME":"BingoElecPower","GPIO":[0,0,0,0,288,289,1,1,224,32,0,0,1,1],"FLAG":0,"BASE":18}
BlitzWolf SHP8               {"NAME":"SHP8","GPIO":[0,320,0,32,2720,2656,0,0,2624,289,224,0,0,0],"FLAG":0,"BASE":64}
BSEED Smart Socket           {"NAME":"BSEED Socket","GPIO":[0,0,0,0,544,288,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
CE Smart Home                {"NAME":"CE Smart Wall","GPIO":[1,1,1,1,544,32,0,0,224,1,1,1,1,1],"FLAG":0,"BASE":18}
CE Smart Home LQ-2-W3        {"NAME":"LITESUN","GPIO":[0,0,0,0,544,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
DETA Double GPO + USB        {"NAME":"DETA 6920HA","GPIO":[0,0,0,3104,32,288,0,0,33,224,225,0,0,0],"FLAG":0,"BASE":18}
Deta Double Power Point      {"NAME":"DETA 2G GPO","GPIO":[0,0,0,0,544,0,0,0,65,224,225,0,64,0],"FLAG":0,"BASE":18}
DETA Outdoor Double Powerpoint  {"NAME":"DETA 6294HA","GPIO":[0,0,0,3104,32,288,0,0,33,224,225,0,0,0],"FLAG":0,"BASE":18}
Deta Single Power Point      {"NAME":"DETA 1G GPO","GPIO":[0,0,0,3104,64,576,0,0,0,224,0,0,0,0],"FLAG":0,"BASE":18}
Ener-J 13A Twin Wall Sockets with USB  {"NAME":"Ener-J 2-Gang ","GPIO":[32,0,0,0,0,224,33,0,225,320,0,0,0,0],"FLAG":0,"BASE":18}
GHome USB Charger            {"NAME":"GHome Smart WO2-1","GPIO":[320,0,0,0,0,257,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Globe Double Receptacle 15A  {"NAME":"Globe 50024","GPIO":[0,0,0,0,320,32,0,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Gosund                       {"NAME":"Gosund WO1","GPIO":[320,0,576,0,2656,2720,0,0,2624,321,225,224,0,4704],"FLAG":0,"BASE":18}
Gosund USB Charger           {"NAME":"Gosund WO2","GPIO":[320,0,576,0,0,257,0,0,0,32,0,224,0,0],"FLAG":0,"BASE":18}
Hevolta Glasense             {"NAME":"Hevolta Socket","GPIO":[0,0,0,0,288,289,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Kapok T16                    {"NAME":"tiltech-t16","GPIO":[0,320,0,32,192,0,0,0,224,225,0,0,0,0],"FLAG":0,"BASE":18}
Kesen KS-604                 {"NAME":"KS-604","GPIO":[1,1,288,1,1,33,0,0,225,224,1,1,32,0],"FLAG":0,"BASE":18}
Kesen KS-604S                {"NAME":"KS-604S","GPIO":[1,1,258,1,1,33,0,0,225,224,1,1,32,4704],"FLAG":0,"BASE":18}
Keygma KS-15TW               {"NAME":"Keygma KS-15T","GPIO":[0,0,0,0,0,320,0,0,224,160,0,0,0,0],"FLAG":0,"BASE":18}
Knightsbridge Dual Waterproof  {"NAME":"Knightsbridge Dual Socket","GPIO":[321,544,320,544,225,2720,0,0,2624,33,2656,224,32,0],"FLAG":0,"BASE":18}
KS-621                       {"NAME":"KS-621","GPIO":[32,0,0,0,2688,2656,0,0,2592,288,0,224,65,1],"FLAG":0,"BASE":18}
Makegood MG-AUWF01           {"NAME":"MG-AUWF01","GPIO":[320,161,544,323,2720,2656,0,0,2624,225,321,224,160,0],"FLAG":0,"BASE":18}
Makegood MG-UKWSG01          {"NAME":"Aseer 2-Gang","GPIO":[321,160,544,323,2720,2656,0,0,2624,224,320,225,161,0],"FLAG":0,"BASE":18}
Makegood MG-UKWSW/B          {"NAME":"Aseer 1-Gang","GPIO":[320,0,544,321,2720,2656,0,0,2624,0,0,224,160,0],"FLAG":0,"BASE":18}
Master Contacto WiFi de pared  {"NAME":"Master_IOT-WLSOCKET","GPIO":[32,0,0,0,0,225,33,0,224,320,0,0,0,0],"FLAG":0,"BASE":18}
Milfra UK Double USB Chager Twin  {"NAME":"Milfra TBU02","GPIO":[0,0,0,0,288,33,0,0,257,256,258,0,32,0],"FLAG":0,"BASE":18}
Moes 16A                     {"NAME":"WK-EU(FR/UK)16M","GPIO":[0,288,0,32,2720,2656,0,0,2624,224,0,0,0,0],"FLAG":0,"BASE":18}
Moes WWK Glass Panel         {"NAME":"Smart Socket","GPIO":[0,0,0,0,288,289,0,0,224,32,0,0,0,0],"FLAG":0,"BASE":18}
Oittm 120                    {"NAME":"Oittm WS01","GPIO":[32,0,0,0,0,2592,0,0,224,2656,2688,288,0,0],"FLAG":0,"BASE":18}
PFS Presa Smart              {"NAME":"PFS_PresaSmart","GPIO":[1,1,1,1,288,289,1,1,224,32,0,1,1,1],"FLAG":0,"BASE":18}
PS-1607                      {"NAME":"PS-1607","GPIO":[32,0,0,0,0,225,33,0,224,0,0,0,0,0],"FLAG":0,"BASE":18}
Smanergy KA10                {"NAME":"KA10","GPIO":[0,320,0,32,2720,2656,0,0,2624,289,224,0,0,0],"FLAG":0,"BASE":64}
Sonoff IW100                 {"NAME":"Sonoff IW100","GPIO":[32,3072,0,3104,0,0,0,0,224,544,0,0,0,0],"FLAG":0,"BASE":41}
Sonoff S55                   {"NAME":"Sonoff S55","GPIO":[32,1,0,1,1,0,0,0,224,320,0,0,0,0],"FLAG":0,"BASE":1}
Steren Dual Plug and USB Charger  {"NAME":"Steren_SHOME-118","GPIO":[0,576,0,32,33,288,0,0,224,225,289,0,0,0],"FLAG":0,"BASE":18}
Steren Dual Plug and USB Charger Wall Outlet (SHOME-118)  {"NAME":"Steren_SHOME-118","GPIO":[576,0,0,32,320,33,0,0,225,224,321,226,0,0],"FLAG":0,"BASE":18}
T16E Dual USB 10A            {"NAME":"T16E 10A","GPIO":[0,0,0,32,2720,2656,0,0,2624,320,224,0,0,0],"FLAG":0,"BASE":18}
TCP Smart Dual               {"NAME":"TCP TAUWIS2GUK","GPIO":[33,0,0,0,0,224,32,0,225,544,0,0,0,0],"FLAG":0,"BASE":18}
Teckin SR40                  {"NAME":"RF-SR40-US","GPIO":[576,0,0,32,320,33,0,0,225,224,321,226,0,0],"FLAG":0,"BASE":18}
TopGreener                   {"NAME":"TGWF15RM","GPIO":[0,320,0,32,2720,2656,0,0,2624,321,224,0,0,0],"FLAG":0,"BASE":55}
TopGreener Dual USB          {"NAME":"TGWF215U2A","GPIO":[0,320,0,32,2720,2656,0,0,2624,225,224,321,0,0],"FLAG":0,"BASE":18}
Vigica VGSPK00815            {"NAME":"VIGICA outlet","GPIO":[32,1,1,1,1,225,33,1,224,1,1,1,1,4704],"FLAG":0,"BASE":18}
Virage Labs ViragePlug       {"NAME":"ViragePlug","GPIO":[544,0,0,32,320,33,0,0,225,224,320,226,0,0],"FLAG":0,"BASE":18}
Woox Dual                    {"NAME":"Woox R4053","GPIO":[33,0,0,0,0,224,32,0,225,320,0,0,0,0],"FLAG":0,"BASE":18}
Xenon                        {"NAME":"Xenon SM-PM801-K1","GPIO":[0,320,0,32,2720,2656,0,0,2624,288,224,0,0,0],"FLAG":0,"BASE":18}
Xenon 2AC 1USB               {"NAME":"Xenon SM-PW801-U1","GPIO":[0,0,0,0,288,32,0,0,224,0,225,0,226,0],"FLAG":0,"BASE":18}
```

## Water Sensor
```
Nedis SmartLife Water Detector  {"NAME":"Nedis WIFIDW10WT","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54,"CMND":"TuyaMCU 51,21"}
W06                          {"NAME":"W06 Water Sensor","GPIO":[0,3200,0,3232,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
Y09                          {"NAME":"Y09","GPIO":[0,2272,0,2304,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":54}
```

## Zigbee Gateway
```
Athom ESP32 Ethernet         {"NAME":"Athom ZG01","GPIO":[32,0,0,0,0,0,0,0,5472,0,5504,544,0,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,1,5792,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Eachen eWeLink ZbBridge Pro Ethernet  {"NAME":"ZB-GW03-V1.2","GPIO":[0,0,3552,0,3584,0,0,0,5793,5792,320,544,5536,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,608,640,32,0,0,0,0,0],"FLAG":0,"BASE":1}
Sonoff ZBBridge              {"NAME":"Sonoff ZbBridge","GPIO":[320,3552,0,3584,5312,0,0,0,640,576,608,0,32,0],"FLAG":0,"BASE":75}
Sonoff ZBBridge Pro          {"NAME":"Sonoff Zigbee Pro","GPIO":[0,0,576,0,480,0,0,0,0,1,1,5792,0,0,0,3552,0,320,5793,3584,0,640,608,32,0,0,0,0,0,1,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Tube's CC2652P2 Ethernet     {"NAME":"Tube ZB CC2652","GPIO":[0,0,0,3840,0,3584,0,0,0,0,0,0,5536,3552,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,3840,5792,0,0,0,0,0,0],"FLAG":0,"BASE":1}
Tube's EFR32 Ethernet        {"NAME":"Tube ZB EFR32","GPIO":[0,0,0,3840,0,3552,1,0,0,0,0,0,5536,3584,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,5793,5792,0,0,0,0,0,0],"FLAG":0,"BASE":1}
TubesZB CC2652P2 Zigbee to PoE Coordinator 2022  {"NAME":"TubesZB CC2652 PoE Coordinator 2022","GPIO":[1,1,8864,1,5793,3584,0,0,5536,5792,8832,8800,3552,0,5600,1,1,1,1,5568,1,1,1,1,0,0,0,0,1,1,32,1,1,1,1,1],"FLAG":0,"BASE":1}
```
