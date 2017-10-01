/*****************************************************************************************************\
 * User specific configuration parameters to override user_config.h
 * 
 * ATTENTION: - Changes to most PARAMETER defines will only override flash settings if you change
 *              define CFG_HOLDER. 
 *            - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *            - You still need to update user_config.h for major defines MODULE and USE_MQTT_TLS.
 *            - Changing MODULE defines are not being tested for validity as they are in user_config.h.
 *            - Most parameters can be changed online using commands via MQTT, WebConsole or serial.
 *            - So I see no use in this but anyway, your on your own.
\*****************************************************************************************************/

// za sonoff slozhi flash size: 1MB / 64, zashtoto default e 512/64 i ne mozhe da se update-va OTA

// Examples
//#ifdef CFG_HOLDER
//#undef CFG_HOLDER
//#endif
//#define CFG_HOLDER             0x20161231

//#ifdef STA_SSID1
//#undef STA_SSID1
//#endif
//#define STA_SSID1              "yourssid1"

// dobavi dolnoto w webserver.h
//    if (Maxdevice) {
//      if (strlen(webServer->arg("o").c_str())) {
//        // mitko start 
//        if (strlen(webServer->arg("s").c_str())) {
//          do_cmnd_power(atoi(webServer->arg("o").c_str()), atoi(webServer->arg("s").c_str()));
//        } else
//        // mitko end

#ifdef OTA_URL
#undef OTA_URL
#endif


#ifdef STA_SSID1
#undef STA_SSID1
#endif
#define STA_SSID1              "mitko.com"      // [Ssid1] Wifi SSID
#ifdef STA_PASS1
#undef STA_PASS1
#endif
#define STA_PASS1              "0000000099"  // [Password1] Wifi password
#ifdef STA_SSID2
#undef STA_SSID2
#endif
#define STA_SSID2              "mitko.com gd iot"      // [Ssid2] Optional alternate AP Wifi SSID
#ifdef STA_PASS2
#undef STA_PASS2
#endif
#define STA_PASS2              "0000000099"  // [Password2] Optional alternate AP Wifi password

#ifdef WIFI_CONFIG_TOOL
#undef WIFI_CONFIG_TOOL
#endif
#define WIFI_CONFIG_TOOL       WIFI_MANAGER // connect to 192.168.4.1


// slozhi dolnia red w sonoff.h sled void mqtt_reconnect()
// if (strlen(sysCfg.mqtt_host) == 0) { addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: WARNING - Server not configured")); mqttcounter = 60; return; } // mitko - da ne se mychi da se wyrzhe s mqtt, ako ne e konfiguriran
// ili v PubSubClient.h namali MQTT_SOCKET_TIMEOUT ot 15 na 2
#ifdef MQTT_HOST
#undef MQTT_HOST
#endif
#define MQTT_HOST            "192.168.1.3"     // [MqttHost] - ako e prazno, ne prawi opit za reconnect, zashtoto inache spi dokato se wyrzhe

// vizh switchmode: 0 = default, 1 = follow, 2 = inverse follow, s i 4 pushbutton (inverted)
//#define USE_WALL_SWITCH                   // Enable the use of a standard wall switch to control the relay
//#define OTA_URL              "http://fw.tivi.bg:80/arduino/" PROJECT ".ino.sw.bin" // switch
//#define APP_NAME             "Sonoff 8266 module for tivi.bg (switch) " __DATE__

// za th10/16 s datchik samo za temp
//#define SEND_TELEMETRY_DS18B20            // Enable sending single temperature telemetry
//#define OTA_URL              "http://fw.tivi.bg:80/arduino/" PROJECT ".ino.ds.bin" // s temperaturen datchik
//#define APP_NAME             "Sonoff 8266 module for tivi.bg (temperature) " __DATE__

// za th10/16 s niakakyw si datchik (temp i hydro)
#define SEND_TELEMETRY_DHT2                // Enable sending temperature and humidity telemetry
// #define SEND_TELEMETRY_I2C                // Enable sending I2C sensor telemetry
#ifdef DHT_TYPE
#undef DHT_TYPE
#endif
#define DHT_TYPE             DHT21       // DHT module type (DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321)
#define OTA_URL              "http://fw.tivi.bg:80/arduino/" PROJECT ".ino.bin" // s temperaturen datchik
#define APP_NAME             "Sonoff 8266 module for tivi.bg (temp+hydro) " __DATE__


// adafruit required #define SEND_TELEMETRY_DHT2               // Enable sending temperature and humi


// wemos d1: pin d7 e inversen i stava 1, kogato I power1, I power2 sa ednowremeno OFF (ne-i)
// veroiatno zashtoto D7 e ednowremenno D11 = GPIO 13
// https://github.com/esp8266/Arduino/blob/master/variants/d1/pins_arduino.h
// #define VER_WEMOSD1
#ifdef VER_WEMOSD1
  // module sonoff_2, 
  // R1: D8/GPIO0 = button, D7(D11)/GPIO13=led, D6(D12)/GPIO12=rel 1, D3/GPIO5=rel 2, D4(D14)/GPIO4=rel 3, D10/GPIO15=rel 4, Switch/GPIO14 = D5 (D13)
  // R2 prognoza: D3/GPIO0 = button, D7/GPIO13=led, D6/GPIO12=rel 1, D1/GPIO5=rel 2, D2/GPIO4=rel 3, D8/GPIO15=rel 4, Switch/GPIO14 = D5
  // triabwa da se izpylni "model 4", inache maj se razpoznawa kato model 2
  #ifdef LED_INVERTED
  #undef LED_INVERTED
  #endif
  #define LED_INVERTED         1            // built-in led pin 2 is inverted, external is not inverted
  #undef LED_PIN
  #define LED_PIN              BUILTIN_LED  // BUILTIN_LED = 2 is D9, but inverted
  #define APP_NAME             "Sonoff 8266 module for tivi.bg (D1) " __DATE__
  #define OTA_URL              "http://fw.tivi.bg:80/arduino/" PROJECT ".ino.d1.bin"
#endif

#ifdef TIME_DST
#undef TIME_DST
#undef TIME_STD
#undef APP_TIMEZONE
#endif
#define TIME_DST               Last, Sun, Mar, 2, +180  // Last sunday in march at 02:00 +120 minutes
#define TIME_STD               Last, Sun, Oct, 3, +120   // Last sunday in october 02:00 +60 minutes
#define APP_TIMEZONE           2



#ifdef SAVE_STATE
#undef SAVE_STATE
#endif
#define SAVE_STATE             0            // [SaveState] Save changed power state to Flash (0 = disable, 1 = enable) - da pestim flasha
// w konzolata napishi savestate 1, za da zapochne da zapiswa. Posle savestate 0, za da spre da zapisva. Mezhduwremenno mozhe da smenish statusa, za da se zapishe.
// poweronstate 0,1,2 ili 3 : 0 = all relay OFF, 1 = all relay ON, 2 = all relay saved state, 3 ili po-goliamo = bitmap na statusa na reletata


#ifdef OTA_URL
// nishto
#else
#define OTA_URL              "http://fw.tivi.bg:80/arduino/" PROJECT ".ino.bin" // default
#endif

#undef MQTT_BUTTON_RETAIN
#undef MQTT_POWER_RETAIN
#undef MQTT_SWITCH_RETAIN
#define MQTT_BUTTON_RETAIN     1                 // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      1                 // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)
#define MQTT_SWITCH_RETAIN     1                 // [SwitchRetain] Switch may send retain flag (0 = off, 1 = on)

#undef APP_SLEEP
#define APP_SLEEP              1                 // [Sleep] Sleep time to lower energy consumption (0 = Off, 1 - 250 mSec)


