/*
xsns_91mcp.ino  MCP23017 support driver
version 0.1.1 updated debug



Switch MCP23017_out8	{ mqtt=">[fms:cmnd/boiler/sensor17:command:*:8?${command}], <[fms:stat/boiler/RESULT:state:JSONPATH($.MCP23017.out8)]" }
String MCP23017_in0 "[%d]"	{mqtt="<[fms:tele/boiler/SENSOR:state:JSONPATH($.MCP23017.in0)]"}
String MCP23017_in1 "[%d]"	{mqtt="<[fms:tele/boiler/SENSOR:state:JSONPATH($.MCP23017.in1)]"}
String MCP23017_in10 "[%d]"	 {mqtt="<[fms:tele/boiler/SENSOR:state:JSONPATH($.MCP23017.out10)]"}

MQTT topic and value: "topic pin?Status"
pin from 1 to 15
status ON or "OFF"
все что не  ON  интерпетируется как OFF
"cmnd/boiler/sensor17 8?ON"

не доделано:
- только однин расширительн портов
- нельзя конфугурировать направление портов и их значение по умолчанию

*/

#define XSNS_91 17

#ifdef USE_I2C
#ifdef USE_MCP23017

#include <Adafruit_MCP23017.h>


/*
GPB0	1
GPB1	2
GPB2	3
GPB3	4
GPB4	5
GPB5	6
GPB6	7
GPB7	8
VDD	9
VSS	10
NC/CS	11
SCL/SCK	12
SDA/SI	13
NC/SO	14

A0	15
A1	16
A2	17
RESET	18
INTB	19
INTA	20
GPA0	21
GPA1	22
GPA2	23
GPA3	24
GPA4	25
GPA5	26
GPA6	27
GPA7	28

addr 0 = A2 low , A1 low , A0 low  000
addr 1 = A2 low , A1 low , A0 high 001
addr 2 = A2 low , A1 high , A0 low  010
addr 3 = A2 low , A1 high , A0 high  011
addr 4 = A2 high , A1 low , A0 low  100
addr 5 = A2 high , A1 low , A0 high  101
addr 6 = A2 high , A1 high , A0 low  110
addr 7 = A2 high, A1 high, A0 high 111

Connect pin #12 of the expander to Analog 5 (i2c clock)
Connect pin #13 of the expander to Analog 4 (i2c data)
Connect pins #15, 16 and 17 of the expander to ground (address selection)
Connect pin #9 of the expander to 5V (power)
Connect pin #10 of the expander to ground (common ground)
Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

// Input #0 is on pin 21 so connect a button or switch from there to ground
*/
// порты лучше прижать явно к земле или к питанию
#define MAX_MCP_DEV 1
//#define MCP_DEV1_PINS 65535 // все входные 0 - in 1 - out старший разряд GPB младщий GPA
#define PIN_ESP_INT 14 // Прерывания с MCP будут обрабатываться с помощью этого PIN-кода
#define D_MCP23017 "MCP23017"
#define D_OUTPUT "out"
#define D_INPUT "in"



#define D_CMND_MCP23017 "MCP23017"

/*
   если несколько расширителей то повторяем
  надо понять как грузит память
*/
Adafruit_MCP23017 mcp0;
//Adafruit_MCP23017 mcp1;
//Adafruit_MCP23017 mcp2;
// Adafruit_MCP23017 mcp3;
// Adafruit_MCP23017 mcp4;
// Adafruit_MCP23017 mcp5;
// Adafruit_MCP23017 mcp6;
// Adafruit_MCP23017 mcp7;


uint8_t mcp_devices;
uint8_t mcp_addr[MAX_MCP_DEV]; //адресa устройств
uint16_t mcp_pins_direct[MAX_MCP_DEV]; // конфигурация потров
uint16_t mcp_pins_default_status[MAX_MCP_DEV]; // значение по умолчанию потров

uint8_t Interupt_count; // количество не опубликованных событий по прерыванию надо вызывать обработчик MqttFastChange
uint8_t Interupt_pin[10];
uint8_t Interupt_val[10];
boolean mqqt_dsxflg;

/****************************************************************************/
void MCP23017Detect()
{
  Serial.println("Start mcp Search...");
  Wire.begin();
  uint8_t error, aa;
  for(uint8_t addresses = 32; addresses < 40; addresses++ ) {
      Wire.beginTransmission(addresses);
      error = Wire.endTransmission();
      if (error == 0) {

          String stringOne =  String(addresses, HEX);
          mcp_addr[mcp_devices++]=addresses-32;
          //Serial.print("found Address: 0x"); Serial.print(stringOne); Serial.print("\tID: "); Serial.println(mcp_addr[mcp_devices]);
          snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT,  D_MCP23017, addresses);
          //AddLog(LOG_LEVEL_DEBUG);
          AddLog(LOG_LEVEL_INFO);
      }
  }
  if (mcp_devices == 0) {
      Serial.println("No mcp devices found\n");
    //return;
  }
  else {
      Serial.print("done mcp Scanning.\t Found devices: ");
      Serial.println(mcp_devices);
  }
  delay(3000);
  if (mcp_devices > 0) {
    Adafruit_MCP23017 mcp0;
    mcp0.begin(mcp_addr[aa++]);
  }
  // if (mcp_devices > 1) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp1.begin(mcp_addr[aa++]);
  // }
  // if (mcp_devices > 2) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp2.begin(mcp_addr[aa++]);
  // }
  // if (mcp_devices > 3) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp3.begin(mcp_addr[aa++]);
  // }
  // if (mcp_devices > 4) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp4.begin(mcp_addr[aa++]);
  // }
  // if (mcp_devices > 5) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp5.begin(mcp_addr[aa++]);
  // }
  // if (mcp_devices > 6) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp6.begin(mcp_addr[aa++]);
  // }
  // if (mcp_devices > 7) {
  //   Adafruit_MCP23017 mcp1;
  //   mcp7.begin(mcp_addr[aa++]);
  // }

}

void MCP23017Init(){
  //mcp_pins[0]=0;//65535;
  int8_t d=0;
  mcp_pins_direct[0]=65280;
  mcp_pins_default_status[0]=0;

  for (int _bit=0; _bit<16; _bit++){
    //проверка установленного бита
    //boolean direct;
    //direct=mcp_pins[0]&1<<a;
    //Serial.print("Pin: "); Serial.print(_bit);
    if (mcp_pins_direct[d]&1<<_bit){
      mcp0.pinMode(_bit, OUTPUT); //GB0 pin 1 on chip
      //mcp0.digitalWrite(_bit, LOW);
      if (mcp_pins_default_status[d]&1<<_bit){
        mcp0.digitalWrite(_bit, HIGH);
      }
      else {
        mcp0.digitalWrite(_bit, LOW);
      }
      //Serial.println("\tSet: OUTPUT");
      snprintf_P(log_data, sizeof(log_data), PSTR(D_MCP23017 " id:%d" D_PORT ": %d " D_OUTPUT), d, _bit);
      AddLog(LOG_LEVEL_INFO);

    }
    else {
      mcp0.pinMode(_bit, INPUT); //GBA0 pin 21 on chip
      mcp0.pullUp(_bit, HIGH);
      mcp0.setupInterruptPin(_bit, CHANGE); // разрешаем прерывание по изменению статуса
      //Serial.println("\tSet: INPUT");//; Serial.println(mcp_pins[0]&1<<_bit);
      snprintf_P(log_data, sizeof(log_data), PSTR(D_MCP23017 " id:%d" D_PORT ": %d " D_INPUT), d, _bit);
      AddLog(LOG_LEVEL_INFO);

    }
  }
  mcp0.setupInterrupts( true , false , LOW); // подключаемся к INTA
  attachInterrupt(PIN_ESP_INT, MCP_Interupt, FALLING);
  //Serial.println("Setup.finished..");

}

void MCP_Interupt(){
  // Get more information from the MCP from the INT
//  MCP_MqttFastPublic(mcp0.getLastInterruptPin(),mcp0.getLastInterruptPinValue(), false);

// если замыкается два пина, но приходит одно прерывание
  Interupt_count++;
  Interupt_pin[Interupt_count]=mcp0.getLastInterruptPin();
  Interupt_val[Interupt_count]=mcp0.getLastInterruptPinValue();

}


void MCP_MqttFastPublic(uint8_t pin, uint8_t val, boolean ending) {
  if (mqqt_dsxflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(","), pin, val);
  }
  else{
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MCP23017 "\":{"));
  }
  if (mcp_pins_direct[0]&1<<pin){
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_OUTPUT "%d\":%d"), mqtt_data, pin, val);
  }
  else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_INPUT "%d\":%d"), mqtt_data, pin, val);
  }
  if (ending) snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);



}
void MqttFastChange(){
  if (Interupt_count>0){
    uint8_t curentc_count=0;
    while (curentc_count < Interupt_count) {
      curentc_count++;
      MCP_MqttFastPublic(Interupt_pin[curentc_count],Interupt_val[curentc_count], false);
      //mqqt_dsxflg=1;
      /* code */
    }
    Interupt_count=0;
    mqqt_dsxflg = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
    // а теперь публикуем mqqt
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
    //MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);

    // snprintf_P(log_data, sizeof(mqtt_data), PSTR("%s"), mqtt_data);
    // AddLog(LOG_LEVEL_DEBUG); // а теперь публикуем log
  }
}

void MCP23017Show(boolean json){

  char stemp[10];

  byte dsxflg = 0;
  for (byte _bit = 0; _bit < 16; _bit++) {
    int16_t mcp_value = mcp0.digitalRead(_bit);
///
    if (json) {
      if (!dsxflg  ) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_MCP23017 "\":{"), mqtt_data);
        stemp[0] = '\0';
      }
      dsxflg++;
      if (mcp_pins_direct[0]&1<<_bit){
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_OUTPUT "%d\":%d"), mqtt_data, stemp, _bit, mcp_value);
      }
      else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_INPUT "%d\":%d"), mqtt_data, stemp, _bit, mcp_value);
      }
      strcpy(stemp, ",");
  #ifdef USE_WEBSERVER
    } else {
      //snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MCP_INPUT, mqtt_data, "MCP23017", i, mcp_value);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, D_MCP23017, _bit, mcp_value);

  #endif  // USE_WEBSERVER
    }
  //
  }
  if (json) {
    if (dsxflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
  }
}

bool CommandMCP23017() {
  boolean serviced = true;
  int8_t d=0;
  /*
  Serial.println("run CommandMCP23017()");
  Serial.print("  index: ");      Serial.print(XdrvMailbox.index);
  Serial.print("  data_len: ");   Serial.print(XdrvMailbox.data_len);
  Serial.print("  payload: ");    Serial.print(XdrvMailbox.payload);
  Serial.print("  topic:");       Serial.print(XdrvMailbox.topic);
  Serial.print("  data: ");       Serial.println(XdrvMailbox.data);
*/
  if (XdrvMailbox.data_len) {
    char *value_command = strtok(XdrvMailbox.data, "?");
    value_command = strtok(NULL, "?");
    uint8_t channal = (uint8_t) XdrvMailbox.payload;
#ifdef LVA_DEBUG
    Serial.println("channal: "+ String(channal));
    Serial.println("value_command: "+ String(value_command));
#endif
    if (mcp_pins_direct[d]&1<< channal) {
          if (!strcasecmp(value_command, "ON")){
            mcp0.digitalWrite(channal, HIGH);
            //Serial.println("pin set to 1");
            MCP_MqttFastPublic(channal, 1, true);

            Interupt_count++;
            Interupt_pin[Interupt_count]=channal;
            Interupt_val[Interupt_count]=1;
          }
          else {
            mcp0.digitalWrite(channal, LOW);
            //Serial.println("pin set to 0");
            MCP_MqttFastPublic(channal, 0, true);
            Interupt_count++;
            Interupt_pin[Interupt_count]=channal;
            Interupt_val[Interupt_count]=0;

          }

    }
    else {
        serviced = false;
        Serial.println("ERROR pin in");
    }
  }

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/



boolean Xsns91(byte function) {
  boolean result = false;
  //Serial.println("run Xsns17()");

  if (i2c_flg) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        MqttFastChange();
        break;
      case FUNC_INIT:
        MCP23017Init();
        break;
      case FUNC_COMMAND:
        //Serial.println("run Xsns17()");
        if (XSNS_91 == XdrvMailbox.index) {
            result = CommandMCP23017();
        }
        break;
      case FUNC_JSON_APPEND:
        MCP23017Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        MCP23017Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

/*

itch LivingRoom_Light "Living Room Light" <light> (LR,gLight)
    { mqtt=">[broker:cmnd/sonoff-A00EEA/POWER:command:*:default],
            <[broker:stat/sonoff-A00EEA/POWER:state:default]" }

работает из брокера
топик cmnd/boiler/POWER1 или cmnd/boiler/POWER2
зданчение даты 1 или 0 а так же ON или OFF

возращает
20:44:27 MQT: stat/boiler/RESULT = {"POWER2":"ON"}
20:44:27 MQT: stat/boiler/POWER2 = ON

команда с консоли просто "POWER1 OFF" или "POWER1 ON"


20:59:28 RSL: Group 0, Index 2, Command POWER, Data 1
20:59:28 MQT: stat/boiler/RESULT = {"POWER2":"ON"}
20:59:28 MQT: stat/boiler/POWER2 = ON
20:59:33 RSL: Group 0, Index 2, Command POWER, Data 1
20:59:33 MQT: stat/boiler/RESULT = {"POWER2":"ON"}
20:59:33 MQT: stat/boiler/POWER2 = ON
20:59:37 RSL: Group 0, Index 2, Command POWER, Data 0
20:59:37 MQT: stat/boiler/RESULT = {"POWER2":"OFF"}
20:59:37 MQT: stat/boiler/POWER2 = OFF

*/







#endif  // USE_MCP23017
#endif  // USE_I2C
