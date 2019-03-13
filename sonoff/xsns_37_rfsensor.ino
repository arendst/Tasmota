/*
  xsns_37_rfsensor.ino - RF sensor receiver for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_RF_SENSOR
/*********************************************************************************************\
 * RF receiver based on work by Paul Tonkes (www.nodo-domotica.nl)
 *
 * Supported 434MHz receiver is Aurel RX-4M50RR30SF
 * Supported 868MHz receiver is Aurel RX-AM8SF
 *
 * Connect one of above receivers with a 330 Ohm resistor to any GPIO
 *
 * USE_THEO_V2    Add support for 434MHz Theo V2 sensors as documented on https://sidweb.nl
 * USE_ALECTO_V2  Add support for 868MHz Alecto V2 sensors like ACH2010, WS3000 and DKW2012 weather stations
\*********************************************************************************************/

#define XSNS_37                   37

//#define USE_THEO_V2                      // Add support for 434MHz Theo V2 sensors as documented on https://sidweb.nl
//#define USE_ALECTO_V2                    // Add support for 868MHz Alecto V2 sensors like ACH2010, WS3000 and DKW2012

#define RFSNS_VALID_WINDOW        1800   // Number of seconds for sensor to respond (1800 = 30 minutes)

#define RFSNS_LOOPS_PER_MILLI     1900   // (345 voor 16MHz ATMega) Voor 80MHz NodeMCU (ESP-12E). Getest met TheoV2 Protocol.
#define RFSNS_RAW_BUFFER_SIZE     180    // (256) Maximum number of RF pulses that can be captured
#define RFSNS_MIN_RAW_PULSES      112    // (16) =8 bits. Minimaal aantal ontvangen bits*2 alvorens cpu tijd wordt besteed aan decodering, etc.
                                         //   Zet zo hoog mogelijk om CPU-tijd te sparen en minder 'onzin' te ontvangen.
#define RFSNS_MIN_PULSE_LENGTH    300    // (50) Pulsen korter dan deze tijd uSec. worden als stoorpulsen beschouwd.
#define RFSNS_RAWSIGNAL_SAMPLE    50     // Sample grootte / Resolutie in uSec waarmee ontvangen Rawsignalen pulsen worden opgeslagen
#define RFSNS_SIGNAL_TIMEOUT      10     // Pulse timings in mSec. Beyond this value indicate end of message
#define RFSNS_SIGNAL_REPEAT_TIME  500    // (500) Tijd in mSec. waarbinnen hetzelfde event niet nogmaals via RF mag binnenkomen. Onderdrukt ongewenste herhalingen van signaal

typedef struct RawSignalStruct                   // Variabelen geplaatst in struct zodat deze later eenvoudig kunnen worden weggeschreven naar SDCard
{
  int  Number;                           // aantal bits, maal twee omdat iedere bit een mark en een space heeft.
  uint8_t Repeats;                          // Aantal maal dat de pulsreeks verzonden moet worden bij een zendactie.
  uint8_t Multiply;                         // Pulses[] * Multiply is de echte tijd van een puls in microseconden
  unsigned long Time;                    // Tijdstempel wanneer signaal is binnengekomen (millis())
  uint8_t Pulses[RFSNS_RAW_BUFFER_SIZE+2];  // Tabel met de gemeten pulsen in microseconden gedeeld door rfsns_raw_signal->Multiply. Dit scheelt helft aan RAM geheugen.
                                         // Om legacy redenen zit de eerste puls in element 1. Element 0 wordt dus niet gebruikt.
} raw_signal_t;

raw_signal_t *rfsns_raw_signal = NULL;
uint8_t rfsns_rf_bit;
uint8_t rfsns_rf_port;
uint8_t rfsns_any_sensor = 0;

/*********************************************************************************************\
 * Fetch signals from RF pin
\*********************************************************************************************/

bool RfSnsFetchSignal(uint8_t DataPin, bool StateSignal)
{
  uint8_t Fbit = digitalPinToBitMask(DataPin);
  uint8_t Fport = digitalPinToPort(DataPin);
  uint8_t FstateMask = (StateSignal ? Fbit : 0);

  if ((*portInputRegister(Fport) & Fbit) == FstateMask) {                   // Als er signaal is
    const unsigned long LoopsPerMilli = RFSNS_LOOPS_PER_MILLI;

    // Als het een herhalend signaal is, dan is de kans groot dat we binnen hele korte tijd weer in deze
    // routine terugkomen en dan midden in de volgende herhaling terecht komen. Daarom wordt er in dit
    // geval gewacht totdat de pulsen voorbij zijn en we met het capturen van data beginnen na een korte
    // rust tussen de signalen. Op deze wijze wordt het aantal zinloze captures teruggebracht.

    unsigned long PulseLength = 0;
    if (rfsns_raw_signal->Time) {                                           //  Eerst een snelle check, want dit bevindt zich in een tijdkritisch deel...
      if (rfsns_raw_signal->Repeats && (rfsns_raw_signal->Time + RFSNS_SIGNAL_REPEAT_TIME) > millis()) {  // ...want deze check duurt enkele micro's langer!
        PulseLength = micros() + RFSNS_SIGNAL_TIMEOUT *1000;                // Wachttijd
        while (((rfsns_raw_signal->Time + RFSNS_SIGNAL_REPEAT_TIME) > millis()) && (PulseLength > micros())) {
          if ((*portInputRegister(Fport) & Fbit) == FstateMask) {
            PulseLength = micros() + RFSNS_SIGNAL_TIMEOUT *1000;
          }
        }
        while (((rfsns_raw_signal->Time + RFSNS_SIGNAL_REPEAT_TIME) > millis()) && ((*portInputRegister(Fport) & Fbit) != FstateMask));
      }
    }

    int RawCodeLength = 1;                                                  // We starten bij 1, dit om legacy redenen. Vroeger had element 0 een speciaal doel.
    bool Ftoggle = false;
    unsigned long numloops = 0;
    unsigned long maxloops = RFSNS_SIGNAL_TIMEOUT * LoopsPerMilli;
    rfsns_raw_signal->Multiply = RFSNS_RAWSIGNAL_SAMPLE;                    // Ingestelde sample groote.
    do {                                                                    // lees de pulsen in microseconden en plaats deze in de tijdelijke buffer rfsns_raw_signal
      numloops = 0;
      while(((*portInputRegister(Fport) & Fbit) == FstateMask) ^ Ftoggle) { // while() loop *A*
        if (numloops++ == maxloops) { break; }                              // timeout opgetreden
      }
      PulseLength = (numloops *1000) / LoopsPerMilli;                       // Bevat nu de pulslengte in microseconden
      if (PulseLength < RFSNS_MIN_PULSE_LENGTH) { break; }
      Ftoggle = !Ftoggle;
      rfsns_raw_signal->Pulses[RawCodeLength++] = PulseLength / (unsigned long)rfsns_raw_signal->Multiply;  // sla op in de tabel rfsns_raw_signal
    }
    while(RawCodeLength < RFSNS_RAW_BUFFER_SIZE && numloops <= maxloops);   // Zolang nog ruimte in de buffer, geen timeout en geen stoorpuls

    if ((RawCodeLength >= RFSNS_MIN_RAW_PULSES) && (RawCodeLength < RFSNS_RAW_BUFFER_SIZE -1)) {
      rfsns_raw_signal->Repeats = 0;                                        // Op dit moment weten we nog niet het type signaal, maar de variabele niet ongedefinieerd laten.
      rfsns_raw_signal->Number = RawCodeLength -1;                          // Aantal ontvangen tijden (pulsen *2)
      rfsns_raw_signal->Pulses[rfsns_raw_signal->Number] = 0;               // Laatste element bevat de timeout. Niet relevant.
      rfsns_raw_signal->Time = millis();
      return true;
    }
    else
      rfsns_raw_signal->Number = 0;
  }

  return false;
}

#ifdef USE_THEO_V2
/*********************************************************************************************\
 * Theo V2 protocol
 * Dit protocol zorgt voor ontvangst van Theo sensoren met protocol V2
 *
 * Auteur             : Theo Arends
 * Support            : www.sidweb.nl
 * Datum              : 17 Apr 2014
 * Versie             : 0.1 - Initiele versie
 **********************************************************************************************
 * Technische informatie:
 *
 * Theo Sensor V2 type 1 Message Format (7 Bytes, 57 bits):
 *   Checksum Type Chl BsVoltag Temperature       Light
 * S AAAAAAAA BBBBBCCC DEFFFFFF GGGGGGGG GGGGGGGG HHHHHHHH HHHHHHHH
 * idx: 0        1        2        3        4        5        6
 *
 * Theo Sensor V2 type 2 Message Format (7 Bytes, 57 bits):
 *   Checksum Type Chl BsVoltag Temperature       Humidity
 * S AAAAAAAA BBBBBCCC DEFFFFFF GGGGGGGG GGGGGGGG HHHHHHHH HHHHHHHH
 * idx: 0        1        2        3        4        5        6
\*********************************************************************************************/

#define RFSNS_THEOV2_MAX_CHANNEL    2      // Max number of ATTiny sensor channels supported

#define RFSNS_THEOV2_PULSECOUNT     114
#define RFSNS_THEOV2_RF_PULSE_MID   1000   // PWM: Pulsen langer zijn '1'

typedef struct {
  uint32_t time;
  int16_t temp;
  uint16_t lux;
  uint8_t volt;
} theo_v2_t1_t;

typedef struct {
  uint32_t time;
  int16_t temp;
  uint16_t hum;
  uint8_t volt;
} theo_v2_t2_t;

theo_v2_t1_t *rfsns_theo_v2_t1 = NULL;
theo_v2_t2_t *rfsns_theo_v2_t2 = NULL;

void RfSnsInitTheoV2(void)
{
  rfsns_theo_v2_t1 = (theo_v2_t1_t*)malloc(RFSNS_THEOV2_MAX_CHANNEL * sizeof(theo_v2_t1_t));
  rfsns_theo_v2_t2 = (theo_v2_t2_t*)malloc(RFSNS_THEOV2_MAX_CHANNEL * sizeof(theo_v2_t2_t));
  rfsns_any_sensor++;
}

void RfSnsAnalyzeTheov2(void)
{
  if (rfsns_raw_signal->Number != RFSNS_THEOV2_PULSECOUNT) { return; }

  uint8_t Checksum;  // 8 bits Checksum over following bytes
  uint8_t Channel;   // 3 bits channel
  uint8_t Type;      // 5 bits type
  uint8_t Voltage;   // 8 bits Vcc like 45 = 4.5V, bit 8 is batt low
  int Payload1;      // 16 bits
  int Payload2;      // 16 bits

  uint8_t b, bytes, bits, id;

  uint8_t idx = 3;
  uint8_t chksum = 0;
  for (bytes = 0; bytes < 7; bytes++) {
    b = 0;
    for (bits = 0; bits <= 7; bits++)
    {
      if ((rfsns_raw_signal->Pulses[idx] * rfsns_raw_signal->Multiply) > RFSNS_THEOV2_RF_PULSE_MID) {
        b |= 1 << bits;
      }
      idx += 2;
    }
    if (bytes > 0) { chksum += b; }  // bereken checksum

    switch (bytes) {
    case 0:
      Checksum = b;
      break;
    case 1:
      id = b;
      Channel = b & 0x7;
      Type = (b >> 3) & 0x1f;
      break;
    case 2:
      Voltage = b;
      break;
    case 3:
      Payload1 = b;
      break;
    case 4:
      Payload1 = (b << 8) | Payload1;
      break;
    case 5:
      Payload2 = b;
      break;
    case 6:
      Payload2 = (b << 8) | Payload2;
      break;
    }
  }

  if (Checksum != chksum) { return; }
  if ((Channel == 0) || (Channel > RFSNS_THEOV2_MAX_CHANNEL)) { return; }
  Channel--;

  rfsns_raw_signal->Repeats = 1;  // het is een herhalend signaal. Bij ontvangst herhalingen onderdukken

  int Payload3 = Voltage & 0x3f;

  switch (Type) {
  case 1:   // Temp / Lux
    rfsns_theo_v2_t1[Channel].time = LocalTime();
    rfsns_theo_v2_t1[Channel].volt = Payload3;
    rfsns_theo_v2_t1[Channel].temp = Payload1;
    rfsns_theo_v2_t1[Channel].lux = Payload2;
    break;
  case 2:  // Temp / Hum
    rfsns_theo_v2_t2[Channel].time = LocalTime();
    rfsns_theo_v2_t2[Channel].volt = Payload3;
    rfsns_theo_v2_t2[Channel].temp = Payload1;
    rfsns_theo_v2_t2[Channel].hum = Payload2;
    break;
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RFS: TheoV2, ChkCalc %d, Chksum %d, id %d, Type %d, Ch %d, Volt %d, BattLo %d, Pld1 %d, Pld2 %d"),
    chksum, Checksum, id, Type, Channel +1, Payload3, (Voltage & 0x80) >> 7, Payload1, Payload2);
}

void RfSnsTheoV2Show(bool json)
{
  bool sensor_once = false;

  for (uint8_t i = 0; i < RFSNS_THEOV2_MAX_CHANNEL; i++) {
    if (rfsns_theo_v2_t1[i].time) {
      char sensor[10];
      snprintf_P(sensor, sizeof(sensor), PSTR("TV2T1C%d"), i +1);
      char voltage[33];
      dtostrfd((float)rfsns_theo_v2_t1[i].volt / 10, 1, voltage);

      if (rfsns_theo_v2_t1[i].time < LocalTime() - RFSNS_VALID_WINDOW) {
        if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_RFRECEIVED "\":\"%s\",\"" D_JSON_VOLTAGE "\":%s}"),
            mqtt_data, sensor, GetDT(rfsns_theo_v2_t1[i].time).c_str(), voltage);
        }
      } else {
        char temperature[33];
        dtostrfd(ConvertTemp((float)rfsns_theo_v2_t1[i].temp / 100), Settings.flag2.temperature_resolution, temperature);

        if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_ILLUMINANCE "\":%d,\"" D_JSON_VOLTAGE "\":%s}"),
            mqtt_data, sensor, temperature, rfsns_theo_v2_t1[i].lux, voltage);
#ifdef USE_DOMOTICZ
          if ((0 == tele_period) && !sensor_once) {
            DomoticzSensor(DZ_TEMP, temperature);
            DomoticzSensor(DZ_ILLUMINANCE, rfsns_theo_v2_t1[i].lux);
            sensor_once = true;
          }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
        } else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, sensor, temperature, TempUnit());
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ILLUMINANCE, mqtt_data, sensor, rfsns_theo_v2_t1[i].lux);
#endif  // USE_WEBSERVER
        }
      }
    }
  }

  sensor_once = false;
  for (uint8_t i = 0; i < RFSNS_THEOV2_MAX_CHANNEL; i++) {
    if (rfsns_theo_v2_t2[i].time) {
      char sensor[10];
      snprintf_P(sensor, sizeof(sensor), PSTR("TV2T2C%d"), i +1);
      char voltage[33];
      dtostrfd((float)rfsns_theo_v2_t2[i].volt / 10, 1, voltage);

      if (rfsns_theo_v2_t2[i].time < LocalTime() - RFSNS_VALID_WINDOW) {
        if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_RFRECEIVED" \":\"%s\",\"" D_JSON_VOLTAGE "\":%s}"),
            mqtt_data, sensor, GetDT(rfsns_theo_v2_t2[i].time).c_str(), voltage);
        }
      } else {
        float temp = ConvertTemp((float)rfsns_theo_v2_t2[i].temp / 100);
        float humi = (float)rfsns_theo_v2_t2[i].hum / 100;
        char temperature[33];
        dtostrfd(temp, Settings.flag2.temperature_resolution, temperature);
        char humidity[33];
        dtostrfd(humi, Settings.flag2.humidity_resolution, humidity);

        if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s,\"" D_JSON_VOLTAGE "\":%s}"),
            mqtt_data, sensor, temperature, humidity, voltage);
          if ((0 == tele_period) && !sensor_once) {
#ifdef USE_DOMOTICZ
            DomoticzTempHumSensor(temperature, humidity);  //
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
            KnxSensor(KNX_TEMPERATURE, temp);
            KnxSensor(KNX_HUMIDITY, humi);
#endif  // USE_KNX
            sensor_once = true;
          }
#ifdef USE_WEBSERVER
        } else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, sensor, temperature, TempUnit());
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, sensor, humidity);
#endif  // USE_WEBSERVER
        }
      }
    }
  }
}

#endif  // USE_THEO_V2 ************************************************************************

#ifdef USE_ALECTO_V2
/*********************************************************************************************\
 * Alecto V2 protocol
 * Dit protocol zorgt voor ontvangst van Alecto weerstation buitensensoren
 *
 * Auteur             : Nodo-team (Martinus van den Broek) www.nodo-domotica.nl
 *                      Support ACH2010 en code optimalisatie door forumlid: Arendst
 * Support            : www.nodo-domotica.nl
 * Datum              : 25 Jan 2013
 * Versie             : 1.3
 **********************************************************************************************
 * Technische informatie:
 * DKW2012 Message Format: (11 Bytes, 88 bits):
 * AAAAAAAA AAAABBBB BBBB__CC CCCCCCCC DDDDDDDD EEEEEEEE FFFFFFFF GGGGGGGG GGGGGGGG HHHHHHHH IIIIIIII
 *                         Temperature Humidity Windspd_ Windgust Rain____ ________ Winddir  Checksum
 * A = start/unknown, first 8 bits are always 11111111
 * B = Rolling code
 * C = Temperature (10 bit value with -400 base)
 * D = Humidity
 * E = windspeed (* 0.3 m/s, correction for webapp = 3600/1000 * 0.3 * 100 = 108))
 * F = windgust (* 0.3 m/s, correction for webapp = 3600/1000 * 0.3 * 100 = 108))
 * G = Rain ( * 0.3 mm)
 * H = winddirection (0 = north, 4 = east, 8 = south 12 = west)
 * I = Checksum, calculation is still under investigation
 *
 * WS3000 and ACH2010 systems have no winddirection, message format is 8 bit shorter
 * Message Format: (10 Bytes, 80 bits):
 * AAAAAAAA AAAABBBB BBBB__CC CCCCCCCC DDDDDDDD EEEEEEEE FFFFFFFF GGGGGGGG GGGGGGGG HHHHHHHH
 *                         Temperature Humidity Windspd_ Windgust Rain____ ________ Checksum
 *
 * DCF Time Message Format: (NOT DECODED!)
 * AAAAAAAA BBBBCCCC DDDDDDDD EFFFFFFF GGGGGGGG HHHHHHHH IIIIIIII JJJJJJJJ KKKKKKKK LLLLLLLL MMMMMMMM
 *          11                 Hours   Minutes  Seconds  Year     Month    Day      ?        Checksum
 * B = 11 = DCF
 * C = ?
 * D = ?
 * E = ?
 * F = Hours BCD format (7 bits only for this byte, MSB could be '1')
 * G = Minutes BCD format
 * H = Seconds BCD format
 * I = Year BCD format (only two digits!)
 * J = Month BCD format
 * K = Day BCD format
 * L = ?
 * M = Checksum
\*********************************************************************************************/

#define RFSNS_DKW2012_PULSECOUNT       176
#define RFSNS_ACH2010_MIN_PULSECOUNT   160 // reduce this value (144?) in case of bad reception
#define RFSNS_ACH2010_MAX_PULSECOUNT   160

#define D_ALECTOV2                     "AlectoV2"

const char kAlectoV2Directions[] PROGMEM = D_TX20_NORTH "|"
                                           D_TX20_NORTH D_TX20_NORTH D_TX20_EAST "|"
                                           D_TX20_NORTH D_TX20_EAST "|"
                                           D_TX20_EAST D_TX20_NORTH D_TX20_EAST "|"
                                           D_TX20_EAST "|"
                                           D_TX20_EAST D_TX20_SOUTH D_TX20_EAST "|"
                                           D_TX20_SOUTH D_TX20_EAST "|"
                                           D_TX20_SOUTH D_TX20_SOUTH D_TX20_EAST "|"
                                           D_TX20_SOUTH "|"
                                           D_TX20_SOUTH D_TX20_SOUTH D_TX20_WEST "|"
                                           D_TX20_SOUTH D_TX20_WEST "|"
                                           D_TX20_WEST D_TX20_SOUTH D_TX20_WEST "|"
                                           D_TX20_WEST "|"
                                           D_TX20_WEST D_TX20_NORTH D_TX20_WEST "|"
                                           D_TX20_NORTH D_TX20_WEST "|"
                                           D_TX20_NORTH D_TX20_NORTH D_TX20_WEST;

typedef struct {
  uint32_t time;
  float temp;
  float rain;
  float wind;
  float gust;
  uint8_t type;
  uint8_t humi;
  uint8_t wdir;
} alecto_v2_t;

alecto_v2_t *rfsns_alecto_v2 = NULL;
uint16_t rfsns_alecto_rain_base = 0;

void RfSnsInitAlectoV2(void)
{
  rfsns_alecto_v2 = (alecto_v2_t*)malloc(sizeof(alecto_v2_t));
  rfsns_any_sensor++;
}

void RfSnsAnalyzeAlectov2()
{
  if (!(((rfsns_raw_signal->Number >= RFSNS_ACH2010_MIN_PULSECOUNT) &&
         (rfsns_raw_signal->Number <= RFSNS_ACH2010_MAX_PULSECOUNT)) || (rfsns_raw_signal->Number == RFSNS_DKW2012_PULSECOUNT))) { return; }

  uint8_t c = 0;
  uint8_t rfbit;
  uint8_t data[9] = { 0 };
  uint8_t msgtype = 0;
  uint8_t rc = 0;
  int temp;
  uint8_t checksum = 0;
  uint8_t checksumcalc = 0;
  uint8_t maxidx = 8;
  unsigned long atime;
  float factor;
  char buf1[16];

  if (rfsns_raw_signal->Number > RFSNS_ACH2010_MAX_PULSECOUNT) { maxidx = 9; }
  // Get message back to front as the header is almost never received complete for ACH2010
  uint8_t idx = maxidx;
  for (uint8_t x = rfsns_raw_signal->Number; x > 0; x = x-2) {
    if (rfsns_raw_signal->Pulses[x-1] * rfsns_raw_signal->Multiply < 0x300) {
      rfbit = 0x80;
    } else {
      rfbit = 0;
    }
    data[idx] = (data[idx] >> 1) | rfbit;
    c++;
    if (c == 8) {
      if (idx == 0) { break; }
      c = 0;
      idx--;
    }
  }

  checksum = data[maxidx];
  checksumcalc = RfSnsAlectoCRC8(data, maxidx);

  msgtype = (data[0] >> 4) & 0xf;
  rc = (data[0] << 4) | (data[1] >> 4);

  if (checksum != checksumcalc) { return; }
  if ((msgtype != 10) && (msgtype != 5)) { return; }

  rfsns_raw_signal->Repeats = 1;  // het is een herhalend signaal. Bij ontvangst herhalingen onderdukken

//  Test set
//  rfsns_raw_signal->Number = RFSNS_DKW2012_PULSECOUNT;  // DKW2012
//  data[8] = 11;                                        // WSW

  factor = 1.22;  // (1.08)
//  atime = rfsns_raw_signal->Time - rfsns_alecto_time;
//  if ((atime > 10000) && (atime < 60000)) factor = (float)60000 / atime;
//  rfsns_alecto_time = rfsns_raw_signal->Time;
//  Serial.printf("atime %d, rfsns_alecto_time %d\n", atime, rfsns_alecto_time);

  rfsns_alecto_v2->time = LocalTime();
  rfsns_alecto_v2->type = (RFSNS_DKW2012_PULSECOUNT == rfsns_raw_signal->Number);
  rfsns_alecto_v2->temp = (float)(((data[1] & 0x3) * 256 + data[2]) - 400) / 10;
  rfsns_alecto_v2->humi = data[3];
  uint16_t rain = (data[6] * 256) + data[7];
  // check if rain unit has been reset!
  if (rain < rfsns_alecto_rain_base) { rfsns_alecto_rain_base = rain; }
  if (rfsns_alecto_rain_base > 0) {
    rfsns_alecto_v2->rain += ((float)rain - rfsns_alecto_rain_base) * 0.30;
  }
  rfsns_alecto_rain_base = rain;
  rfsns_alecto_v2->wind = (float)data[4] * factor;
  rfsns_alecto_v2->gust = (float)data[5] * factor;
  if (rfsns_alecto_v2->type) {
    rfsns_alecto_v2->wdir = data[8] & 0xf;
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RFS: " D_ALECTOV2 ", ChkCalc %d, Chksum %d, rc %d, Temp %d, Hum %d, Rain %d, Wind %d, Gust %d, Dir %d, Factor %s"),
    checksumcalc, checksum, rc, ((data[1] & 0x3) * 256 + data[2]) - 400, data[3], (data[6] * 256) + data[7], data[4], data[5], data[8] & 0xf, dtostrfd(factor, 3, buf1));
}

void RfSnsAlectoResetRain(void)
{
  if ((RtcTime.hour == 0) && (RtcTime.minute == 0) && (RtcTime.second == 5)) {
    rfsns_alecto_v2->rain = 0;  // Reset Rain
  }
}

/*********************************************************************************************\
 * Calculates CRC-8 checksum
 * reference http://lucsmall.com/2012/04/29/weather-station-hacking-part-2/
 *           http://lucsmall.com/2012/04/30/weather-station-hacking-part-3/
 *           https://github.com/lucsmall/WH2-Weather-Sensor-Library-for-Arduino/blob/master/WeatherSensorWH2.cpp
 \*********************************************************************************************/
uint8_t RfSnsAlectoCRC8(uint8_t *addr, uint8_t len)
{
  uint8_t crc = 0;
  while (len--) {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x80;
      crc <<= 1;
      if (mix) { crc ^= 0x31; }
      inbyte <<= 1;
    }
  }
  return crc;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_ALECTOV2[] PROGMEM = "%s"
  "{s}" D_ALECTOV2 " " D_RAIN "{m}%s " D_UNIT_MILLIMETER "{e}"
  "{s}" D_ALECTOV2 " " D_TX20_WIND_SPEED "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
  "{s}" D_ALECTOV2 " " D_TX20_WIND_SPEED_MAX "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}";
const char HTTP_SNS_ALECTOV2_WDIR[] PROGMEM = "%s"
  "{s}" D_ALECTOV2 " " D_TX20_WIND_DIRECTION "{m}%s{e}";
#endif

void RfSnsAlectoV2Show(bool json)
{
  if (rfsns_alecto_v2->time) {
    if (rfsns_alecto_v2->time < LocalTime() - RFSNS_VALID_WINDOW) {
      if (json) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_ALECTOV2 "\":{\"" D_JSON_RFRECEIVED "\":\"%s\"}"),
          mqtt_data, GetDT(rfsns_alecto_v2->time).c_str());
      }
    } else {
      float temp = ConvertTemp(rfsns_alecto_v2->temp);
      char temperature[33];
      dtostrfd(temp, Settings.flag2.temperature_resolution, temperature);
      float humi = (float)rfsns_alecto_v2->humi;
      char humidity[33];
      dtostrfd(humi, Settings.flag2.humidity_resolution, humidity);
      char rain[33];
      dtostrfd(rfsns_alecto_v2->rain, 2, rain);
      char wind[33];
      dtostrfd(rfsns_alecto_v2->wind, 2, wind);
      char gust[33];
      dtostrfd(rfsns_alecto_v2->gust, 2, gust);
      char wdir[4];
      char direction[20];
      if (rfsns_alecto_v2->type) {
        GetTextIndexed(wdir, sizeof(wdir), rfsns_alecto_v2->wdir, kAlectoV2Directions);
        snprintf_P(direction, sizeof(direction), PSTR(",\"Direction\":\"%s\""), wdir);
      }

      if (json) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_ALECTOV2 "\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s,\"Rain\":%s,\"Wind\":%s,\"Gust\":%s%s}"),
          mqtt_data, temperature, humidity, rain, wind, gust, (rfsns_alecto_v2->type) ? direction : "");
        if (0 == tele_period) {
#ifdef USE_DOMOTICZ
        // Use a rules to send data to Domoticz where also a local BMP280 is connected:
        // on tele-alectov2#temperature do var1 %value% endon on tele-alectov2#humidity do var2 %value% endon on tele-bmp280#pressure do publish domoticz/in {"idx":68,"svalue":"%var1%;%var2%;0;%value%;0"} endon
        // on tele-alectov2#wind do var1 %value% endon on tele-alectov2#gust do publish domoticz/in {"idx":69,"svalue":"0;N;%var1%;%value%;22;24"} endon"}
        // on tele-alectov2#rain do publish domoticz/in {"idx":70,"svalue":"0;%value%"} endon
#endif  // USE_DOMOTICZ
        }
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, D_ALECTOV2, temperature, TempUnit());
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, D_ALECTOV2, humidity);
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ALECTOV2, mqtt_data, rain, wind, gust);
        if (rfsns_alecto_v2->type) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ALECTOV2_WDIR, mqtt_data, wdir);
        }
#endif  // USE_WEBSERVER
      }
    }
  }
}
#endif  // USE_ALECTO_V2 **********************************************************************

void RfSnsInit(void)
{
  rfsns_raw_signal = (raw_signal_t*)(malloc(sizeof(raw_signal_t)));
  if (rfsns_raw_signal) {
    memset(rfsns_raw_signal, 0, sizeof(raw_signal_t));  // Init defaults to 0
#ifdef USE_THEO_V2
    RfSnsInitTheoV2();
#endif
#ifdef USE_ALECTO_V2
    RfSnsInitAlectoV2();
#endif
    if (rfsns_any_sensor) {
      rfsns_rf_bit = digitalPinToBitMask(pin[GPIO_RF_SENSOR]);
      rfsns_rf_port = digitalPinToPort(pin[GPIO_RF_SENSOR]);
      pinMode(pin[GPIO_RF_SENSOR], INPUT);
    } else {
      free(rfsns_raw_signal);
      rfsns_raw_signal = NULL;
    }
  }
}

void RfSnsAnalyzeRawSignal(void)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RFS: Pulses %d"), (int)rfsns_raw_signal->Number);

#ifdef USE_THEO_V2
    RfSnsAnalyzeTheov2();
#endif
#ifdef USE_ALECTO_V2
    RfSnsAnalyzeAlectov2();
#endif
}

void RfSnsEverySecond(void)
{
#ifdef USE_ALECTO_V2
  RfSnsAlectoResetRain();
#endif
}

void RfSnsShow(bool json)
{
#ifdef USE_THEO_V2
  RfSnsTheoV2Show(json);
#endif
#ifdef USE_ALECTO_V2
  RfSnsAlectoV2Show(json);
#endif
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns37(uint8_t function)
{
  bool result = false;

  if ((pin[GPIO_RF_SENSOR] < 99) && (FUNC_INIT == function)) {
    RfSnsInit();
  }
  else if (rfsns_raw_signal) {
    switch (function) {
      case FUNC_LOOP:
        if ((*portInputRegister(rfsns_rf_port) &rfsns_rf_bit) == rfsns_rf_bit) {
          if (RfSnsFetchSignal(pin[GPIO_RF_SENSOR], HIGH)) {
            RfSnsAnalyzeRawSignal();
          }
        }
        sleep = 0;
        break;
      case FUNC_EVERY_SECOND:
        RfSnsEverySecond();
        break;
      case FUNC_JSON_APPEND:
        RfSnsShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        RfSnsShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_RF_SENSOR
