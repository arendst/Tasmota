/*
xsns_92_pca9685.ino  PCA9685 support driver
version 0.1




не доделано:
- только однин драйвер PWM

*/

#define XSNS_92 92

#ifdef USE_I2C
#ifdef USE_PCA9685

#include <Adafruit_PWMServoDriver.h>

#define ADDR_PCA9685 0x41


#define D_PCA9685 "PCA9685"
#define D_CMND_PCA9685 "MCP23017"
#define D_PWM "PWM"
/*
   если несколько расширителей то повторяем
  надо понять как грузит память
*/
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(ADDR_PCA9685);


// uint8_t mcp_devices;
// uint8_t mcp_addr[MAX_MCP_DEV]; //адресa устройств
// uint16_t mcp_pins_direct[MAX_MCP_DEV]; // конфигурация потров
// uint16_t mcp_pins_default_status[MAX_MCP_DEV]; // значение по умолчанию потров
//
// uint8_t Interupt_count; // количество не опубликованных событий по прерыванию надо вызывать обработчик MqttFastChange
uint8_t  change_PCA9685_pin;
uint16_t change_PCA9685_val;
boolean  change_PCA9685_flg;

/****************************************************************************/

void PCA9685Init(){

      pwm.begin();
      pwm.reset();
      pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
      pwm.setPWMALL(0,0);
      snprintf_P(log_data, sizeof(log_data), PSTR(D_PCA9685 " adr:%x init"), ADDR_PCA9685);
      AddLog(LOG_LEVEL_INFO);
}

/*
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

*/

/*
}
void MqttFastChange(){
  if (Interupt_count>0){
    uint8_t curentc_count=0;
    while (curentc_count < Interupt_count) {
      curentc_count++;
      MCP_MqttFastPublic(Interupt_pin[curentc_count],Interupt_val[curentc_count], false);
      //mqqt_dsxflg=1;
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

*/

void PCA9685Show(boolean json){

  char stemp[10];
  byte dsxflg = 0;
  for (byte _pin = 0; _pin < 16; _pin++) {
    int16_t pin_value = pwm.readPWM(_pin);
///
    if (json) {
      if (!dsxflg  ) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_PCA9685 "\":{"), mqtt_data);
        stemp[0] = '\0';
      }
      dsxflg++;
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_PWM "%d\":%d"), mqtt_data, stemp, _pin, pin_value);
      strcpy(stemp, ",");
  #ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, D_PWM, _pin, pin_value);

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
void PCA9685_MqttFastReturn(uint8_t pin, uint16_t val) {
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_PCA9685 "\":{"));
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_PWM "%d\":%d"), mqtt_data, pin, val);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
}
void PCA9685_MqttPublicChange() {
  if (change_PCA9685_flg){
    PCA9685_MqttFastReturn(change_PCA9685_pin, change_PCA9685_val);
    change_PCA9685_flg=false;
    //snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
    // а теперь публикуем mqqt
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  }
}

bool PCA9685Command() {
  boolean serviced = true;
  //uint8_t d=0;
  //uint16_t val=0;
  if (XdrvMailbox.data_len) {
    char *value_command = strtok(XdrvMailbox.data, "?");
    value_command = strtok(NULL, "?");
    //val=atol(char *value_command);
    uint8_t pin = (uint8_t) XdrvMailbox.payload;
    uint16_t val = atoi(value_command);
#ifdef LVA_DEBUG
    Serial.println("Pin: "+ String(pin));
    Serial.println("value_command string: "+ String(value_command));
#endif
    if (pin <16 && val < 4096) {
      pwm.setPWM(pin, 0, val);
#ifdef LVA_DEBUG
      Serial.println("pin " + String(pin) + "set to " + String(value_command));
#endif

      PCA9685_MqttFastReturn(pin, val);
      change_PCA9685_pin=pin;
      change_PCA9685_val=val;
      change_PCA9685_flg=true;
    }
    else {
      Serial.println("ERROR");
      Serial.println("value_command string"+ String(value_command));
      Serial.println("value_command int16"+ String(val,HEX));
      serviced = false;
    }
  }
  else {
    serviced = false;
  }

  return serviced;
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/



boolean Xsns92(byte function) {
  boolean result = false;

  //Serial.println("run Xsns92()");

  if (i2c_flg) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        PCA9685_MqttPublicChange();
        break;
      case FUNC_INIT:
        PCA9685Init();
        break;
      case FUNC_COMMAND:
        //Serial.println("run Xsns92()");
        if (XSNS_92 == XdrvMailbox.index) {
            result = PCA9685Command();
        }
        break;
      case FUNC_JSON_APPEND:
        PCA9685Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        PCA9685Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}









#endif  // USE_PCA9685
#endif  // USE_I2C
