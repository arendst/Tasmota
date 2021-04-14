/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
 *
 * Write an image into a header file using a 3...2...1...0 format per pixel,
 * for 4 bits color (16 colors - well, greys.) MSB first.  At 80 MHz, screen
 * clears execute in 1.075 seconds and images are drawn in 1.531 seconds.
 */
#define ESP32

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "epd_driver.h"

#include "Arduino.h"
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include "ArduinoJson.h"
#include <HTTPClient.h>

#include <WiFi.h>
#include "time.h"
#include <SPI.h>

#include "owm_credentials.h"
//#include "common_functions.h"
#include "forecast_record.h"
#include "lang.h"

#define SCREEN_WIDTH   EPD_WIDTH
#define SCREEN_HEIGHT  EPD_HEIGHT


//################  VERSION  ##################################################
String version = "1.0 / 9.7in";     // Programme version, see change log at end
//################ VARIABLES ##################################################

enum alignment {LEFT, RIGHT, CENTER};
#define GxEPD_WHITE 0xFF
#define GxEPD_BLACK 0x00
#define GRAY 0x88
#define LRAY 0xBB
#define DRAY 0x44
#define autoscale_on  true
#define autoscale_off false
#define barchart_on   true
#define barchart_off  false

boolean LargeIcon = true, SmallIcon = false;
#define Large  28           // For icon drawing, needs to be odd number for best effect
#define Small  8            // 6  For icon drawing, needs to be odd number for best effect
const byte MaxEvents = 10;  // For event reporting, the maximum that can be recorded
String     Time_str = "--:--:--";
String     Date_str = "-- -- ----";
String     EventMessage[MaxEvents]; // strings for Time, Date and Error reporting
const byte EventThreshold = 2; // Change to 1 to view all messages on e-paper screen
int        wifi_signal, CurrentHour = 0, CurrentMin = 0, CurrentSec = 0, EventCnt = 0;

//################ PROGRAM VARIABLES and OBJECTS ##########################################
#define max_readings 8

Forecast_record_type  WxConditions[1];
Forecast_record_type  WxForecast[max_readings];

float pressure_readings[max_readings]    = {0};
float temperature_readings[max_readings] = {0};
float humidity_readings[max_readings]    = {0};
float rain_readings[max_readings]        = {0};
float snow_readings[max_readings]        = {0};

long SleepDuration = 60; // Sleep time in minutes, aligned to the nearest minute boundary, so if 30 will always update at 00 or 30 past the hour
int  WakeupTime    = 6;  // Don't wakeup until after 07:00 to save battery power
int  SleepTime     = 23; // Sleep after (23+1) 00:00 to save battery power
long StartTime = 0, SleepTimer = 0;
long Delta         = 12; // correction factor to compensate the ESP32 speed
// Prevents display at xx:59:yy and then xx:00:yy (one minute later) to save power
String LocalIP;

uint8_t *fb;

//fonts

#include "opensans8.h"
#include "opensans8b.h"
#include "opensans12.h"
#include "opensans12b.h"
#include "opensans16.h"
#include "opensans16b.h"
#include "opensans24.h"
#include "opensans24b.h"

EpdFont currentFont;


bool obtainWeatherData(WiFiClient& client, const String& RequestType);
float mm_to_inches(float value_mm);
float hPa_to_inHg(float value_hPa);
int JulianDate(int d, int m, int y);
float SumOfPrecip(float DataArray[], int readings);
String TitleCase(String text);
double NormalizedMoonPhase(int d, int m, int y);
void Convert_Readings_to_Imperial();
bool DecodeWeather(WiFiClient& json, String Type);
String ConvertUnixTime(int unix_time);

void DisplayWeather();
void DisplayGeneralInfoSection();
void DisplayMainWeatherSection(int x, int y);
void DisplayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius);
String WindDegToDirection(float winddirection);
void DisplayTemperatureSection(int x, int y, int twidth, int tdepth);
void DisplayForecastTextSection(int x, int y , int fwidth, int fdepth);
void DisplayForecastWeather(int x, int y, int index);
void DisplayPressureSection(int x, int y, int pwidth, int pdepth, float pressure, String slope);
void DisplayAstronomySection(int x, int y);
void DrawMoon(int x, int y, int dd, int mm, int yy, String hemisphere);
String MoonPhase(int d, int m, int y, String hemisphere);
void DisplayForecastSection(int x, int y);
void DisplayConditionsSection(int x, int y, String IconName, bool IconSize);
void arrow(int x, int y, int asize, float aangle, int pwidth, int plength);
void DisplayStatusSection(int x, int y, int rssi);
void DrawRSSI(int x, int y, int rssi);
boolean SetupTime();
boolean UpdateLocalTime() ;
void DrawBattery(int x, int y);
void addcloud(int x, int y, int scale, int linesize);
void addraindrop(int x, int y, int scale);
void addsnow(int x, int y, int scale, bool IconSize);
void addtstorm(int x, int y, int scale);
void addsun(int x, int y, int scale, bool IconSize);
void addfog(int x, int y, int scale, int linesize, bool IconSize);
void Sunny(int x, int y, bool IconSize, String IconName);
void MostlySunny(int x, int y, bool IconSize, String IconName) ;
void MostlyCloudy(int x, int y, bool IconSize, String IconName);
void Cloudy(int x, int y, bool IconSize, String IconName);
void Rain(int x, int y, bool IconSize, String IconName);
void ExpectRain(int x, int y, bool IconSize, String IconName);
void ChanceRain(int x, int y, bool IconSize, String IconName);
void Tstorms(int x, int y, bool IconSize, String IconName);
void Snow(int x, int y, bool IconSize, String IconName);
void Fog(int x, int y, bool IconSize, String IconName);
void Haze(int x, int y, bool IconSize, String IconName);
void CloudCover(int x, int y, int CCover);
void Visibility(int x, int y, String Visi);
void addmoon(int x, int y, int scale, bool IconSize);
void Nodata(int x, int y, bool IconSize, String IconName);
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode);
void drawString(int x, int y, String text, alignment align);
void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, alignment align);
void ReportEvent(String EventMessage[]);
void DisplayPrecipitationSection(int x, int y, int pwidth, int pdepth);
void DisplayTime();
//void VerboseRecordOfResetReason(RESET_REASON reason);

void fillCircle(int x, int y, int r, uint8_t color);
void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawCircle(int x0, int y0, int r, uint8_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color);
void drawPixel(int x, int y, uint8_t color) ;
void setFont(EpdFont const&font);
bool getRandomImage(WiFiClient & client);
void drawImage(WiFiClient & client);
bool decodeImage(WiFiClient& json);


void Delay(uint32_t millis) { vTaskDelay(millis / portTICK_PERIOD_MS); }

uint32_t Millis() { return esp_timer_get_time() / 1000; }

void BeginSleep() {
  epd_poweroff();
  long SleepTimer = SleepDuration * 60; //Some ESP32 are too fast to maintain accurate time
  esp_sleep_enable_timer_wakeup(SleepTimer * 1000000LL);
#ifdef BUILTIN_LED
  pinMode(BUILTIN_LED, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
  digitalWrite(BUILTIN_LED, HIGH);
#endif
  Serial.println("Entering " + String(SleepTimer) + "-secs of sleep time");
  Serial.println("Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
  Serial.println("Starting deep-sleep period...");
  epd_deinit();
  esp_deep_sleep_start();  // Sleep for e.g. 30 minutes
}

boolean SetupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, "time.nist.gov"); //(gmtOffset_sec, daylightOffset_sec, ntpServer)
  setenv("TZ", Timezone, 1);  //setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
  tzset(); // Set the TZ environment variable
  delay(100);
  bool TimeStatus = UpdateLocalTime();
  return TimeStatus;
}

uint8_t StartWiFi() {
  Serial.print("\r\nConnecting to: "); Serial.println(String(ssid));
  IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  }
  else Serial.println("WiFi connection *** FAILED ***");
  return connectionStatus;
}

void StopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}


void loop() {

  printf("current temperature: %f\n", epd_ambient_temperature());
  Delay(300);

  Delay(200000);
}

void setup() {
  StartTime = millis();
  Serial.begin(115200);
  if (StartWiFi() == WL_CONNECTED && SetupTime() == true) {
    //if ((CurrentHour >= WakeupTime && CurrentHour <= SleepTime)) {
      byte Attempts = 1;
      bool RxWeather = false, RxForecast = false;
      WiFiClient client;   // wifi client object
      while ((RxWeather == false || RxForecast == false) && Attempts <= 2) { // Try up-to 2 time for Weather and Forecast data
        if (RxWeather  == false) RxWeather  = obtainWeatherData(client, "weather");
        if (RxForecast == false) RxForecast = obtainWeatherData(client, "forecast");
        Attempts++;
      }
      if (true || (RxWeather && RxForecast)) { // Only if received both Weather or Forecast proceed
        StopWiFi(); // Reduces power consumption


        epd_poweron();
        volatile uint32_t t1 = Millis();
        epd_clear();
        volatile uint32_t t2 = Millis();
        printf("EPD clear took %dms.\n", t2 - t1);

        //epd_poweroff();
        //epd_poweron();


        //drawImage(client);
        DisplayWeather();
        //DisplayTime();

        t1 = Millis();
        int temperature = epd_ambient_temperature();
        if (temperature <= 0) temperature = 25;
        enum EpdDrawMode mode = (enum EpdDrawMode)(MODE_GC16 | MODE_PACKING_2PPB | PREVIOUSLY_WHITE);
        epd_draw_base(epd_full_screen(), fb, epd_full_screen(), mode, temperature, NULL, EPD_BUILTIN_WAVEFORM);
        t2 = Millis();
        epd_poweroff();

        //display.display(false); // Full screen update mode
      }
    //}
  }
  BeginSleep();
}

void epd_task(void *pvParameters) {
  epd_init(EPD_LUT_1K);

  ESP_LOGW("main", "allocating...\n");

  fb = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
  memset(fb, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

  while (1) {
    loop();
  };
}

 extern "C" {
 void app_main() {
  ESP_LOGW("main", "Hello World!\n");

  heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
  heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);

  xTaskCreatePinnedToCore(&epd_task, "epd task", 10000, NULL, 2, NULL, 1);

  initArduino();
  setup();
}
}

void drawImage(WiFiClient & client) {
  getRandomImage(client);
}

void Convert_Readings_to_Imperial() {
  WxConditions[0].Pressure = hPa_to_inHg(WxConditions[0].Pressure);
  WxForecast[1].Rainfall   = mm_to_inches(WxForecast[1].Rainfall);
  WxForecast[1].Snowfall   = mm_to_inches(WxForecast[1].Snowfall);
}

bool DecodeWeather(WiFiClient& json, String Type) {
  Serial.print(F("\nCreating object...and "));
  // allocate the JsonDocument
  DynamicJsonDocument doc(64 * 1024);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  Serial.println(" Decoding " + Type + " data");
  if (Type == "weather") {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    WxConditions[0].lon         = root["coord"]["lon"].as<float>();              Serial.println(" Lon: "+String(WxConditions[0].lon));
    WxConditions[0].lat         = root["coord"]["lat"].as<float>();              Serial.println(" Lat: "+String(WxConditions[0].lat));
    WxConditions[0].Main0       = root["weather"][0]["main"].as<char*>();        Serial.println("Main: "+String(WxConditions[0].Main0));
    WxConditions[0].Forecast0   = root["weather"][0]["description"].as<char*>(); Serial.println("For0: "+String(WxConditions[0].Forecast0));
    WxConditions[0].Forecast1   = root["weather"][1]["description"].as<char*>(); Serial.println("For1: "+String(WxConditions[0].Forecast1));
    WxConditions[0].Forecast2   = root["weather"][2]["description"].as<char*>(); Serial.println("For2: "+String(WxConditions[0].Forecast2));
    WxConditions[0].Icon        = root["weather"][0]["icon"].as<char*>();        Serial.println("Icon: "+String(WxConditions[0].Icon));
    WxConditions[0].Temperature = root["main"]["temp"].as<float>();              Serial.println("Temp: "+String(WxConditions[0].Temperature));
    WxConditions[0].Pressure    = root["main"]["pressure"].as<float>();          Serial.println("Pres: "+String(WxConditions[0].Pressure));
    WxConditions[0].Humidity    = root["main"]["humidity"].as<float>();          Serial.println("Humi: "+String(WxConditions[0].Humidity));
    WxConditions[0].Low         = root["main"]["temp_min"].as<float>();          Serial.println("TLow: "+String(WxConditions[0].Low));
    WxConditions[0].High        = root["main"]["temp_max"].as<float>();          Serial.println("THig: "+String(WxConditions[0].High));
    WxConditions[0].Windspeed   = root["wind"]["speed"].as<float>();             Serial.println("WSpd: "+String(WxConditions[0].Windspeed));
    WxConditions[0].Winddir     = root["wind"]["deg"].as<float>();               Serial.println("WDir: "+String(WxConditions[0].Winddir));
    WxConditions[0].Cloudcover  = root["clouds"]["all"].as<int>();               Serial.println("CCov: "+String(WxConditions[0].Cloudcover)); // in % of cloud cover
    WxConditions[0].Visibility  = root["visibility"].as<int>();                  Serial.println("Visi: "+String(WxConditions[0].Visibility)); // in metres
    WxConditions[0].Rainfall    = root["rain"]["1h"].as<float>();                Serial.println("Rain: "+String(WxConditions[0].Rainfall));
    WxConditions[0].Snowfall    = root["snow"]["1h"].as<float>();                Serial.println("Snow: "+String(WxConditions[0].Snowfall));
    WxConditions[0].Country     = root["sys"]["country"].as<char*>();            Serial.println("Ctry: "+String(WxConditions[0].Country));
    WxConditions[0].Sunrise     = root["sys"]["sunrise"].as<int>();              Serial.println("SRis: "+String(WxConditions[0].Sunrise));
    WxConditions[0].Sunset      = root["sys"]["sunset"].as<int>();               Serial.println("SSet: "+String(WxConditions[0].Sunset));
    WxConditions[0].Timezone    = root["timezone"].as<int>();                    Serial.println("TZon: "+String(WxConditions[0].Timezone));  }
  if (Type == "forecast") {
    //Serial.println(json);
    Serial.print(F("\nReceiving Forecast period - ")); //------------------------------------------------
    JsonArray list                  = root["list"];
    for (byte r = 0; r < max_readings; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      WxForecast[r].Dt                = list[r]["dt"].as<char*>();
      WxForecast[r].Temperature       = list[r]["main"]["temp"].as<float>();              Serial.println("Temp: "+String(WxForecast[r].Temperature));
      WxForecast[r].Low               = list[r]["main"]["temp_min"].as<float>();          Serial.println("TLow: "+String(WxForecast[r].Low));
      WxForecast[r].High              = list[r]["main"]["temp_max"].as<float>();          Serial.println("THig: "+String(WxForecast[r].High));
      WxForecast[r].Pressure          = list[r]["main"]["pressure"].as<float>();          Serial.println("Pres: "+String(WxForecast[r].Pressure));
      WxForecast[r].Humidity          = list[r]["main"]["humidity"].as<float>();          Serial.println("Humi: "+String(WxForecast[r].Humidity));
      WxForecast[r].Forecast0         = list[r]["weather"][0]["main"].as<char*>();        Serial.println("For0: "+String(WxForecast[r].Forecast0));
      WxForecast[r].Forecast1         = list[r]["weather"][1]["main"].as<char*>();        Serial.println("For1: "+String(WxForecast[r].Forecast1));
      WxForecast[r].Forecast2         = list[r]["weather"][2]["main"].as<char*>();        Serial.println("For2: "+String(WxForecast[r].Forecast2));
      WxForecast[r].Icon              = list[r]["weather"][0]["icon"].as<char*>();        Serial.println("Icon: "+String(WxForecast[r].Icon));
      WxForecast[r].Description       = list[r]["weather"][0]["description"].as<char*>(); Serial.println("Desc: "+String(WxForecast[r].Description));
      WxForecast[r].Cloudcover        = list[r]["clouds"]["all"].as<int>();               Serial.println("CCov: "+String(WxForecast[r].Cloudcover)); // in % of cloud cover
      WxForecast[r].Windspeed         = list[r]["wind"]["speed"].as<float>();             Serial.println("WSpd: "+String(WxForecast[r].Windspeed));
      WxForecast[r].Winddir           = list[r]["wind"]["deg"].as<float>();               Serial.println("WDir: "+String(WxForecast[r].Winddir));
      WxForecast[r].Rainfall          = list[r]["rain"]["3h"].as<float>();                Serial.println("Rain: "+String(WxForecast[r].Rainfall));
      WxForecast[r].Snowfall          = list[r]["snow"]["3h"].as<float>();                Serial.println("Snow: "+String(WxForecast[r].Snowfall));
      WxForecast[r].Period            = list[r]["dt_txt"].as<char*>();                    Serial.println("Peri: "+String(WxForecast[r].Period));
    }
    //------------------------------------------
    float pressure_trend = WxForecast[0].Pressure - WxForecast[2].Pressure; // Measure pressure slope between ~now and later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  WxConditions[0].Trend = "+";
    if (pressure_trend < 0)  WxConditions[0].Trend = "-";
    if (pressure_trend == 0) WxConditions[0].Trend = "0";

    if (Units == "I") Convert_Readings_to_Imperial();
  }
  return true;
}
//#########################################################################################
String ConvertUnixTime(int unix_time) {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  if (Units == "M") {
    strftime(output, sizeof(output), "%H:%M %d/%m/%y", now_tm);
  }
  else {
    strftime(output, sizeof(output), "%I:%M%P %m/%d/%y", now_tm);
  }
  return output;
}
//#########################################################################################
//WiFiClient client; // wifi client object

bool decodeImage(WiFiClient& json) {
  Serial.print(F("\nIMG creating object...and "));
  // allocate the JsonDocument
  DynamicJsonDocument doc(1200 * 825);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("IMG deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonArray array = doc.as<JsonArray>();
  Serial.println("IMG Decoding ...");
  uint8_t img_data[EPD_WIDTH * EPD_HEIGHT / 2];
  int i = 0;
  for(JsonVariant v : array) {
    img_data[i] = v.as<uint16_t>();
    i++;
  }
  //img_data = root["data"].as<JsonArray>();

  EpdRect area = {
      .x = 0,
      .y = 0,
      .width = 1200,
      .height = 825,
  };
  epd_copy_to_framebuffer(area, (uint8_t *)img_data, fb);
  return true;
}

bool getRandomImage(WiFiClient & client) {
  client.stop(); // close connection before sending a new request
  HTTPClient http;
  const char imgServer[] = "bb.org.ua";
  String uri = "/get_random_image.php";
  http.begin(client, imgServer, 80, uri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (!decodeImage(http.getStream())) return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}

bool obtainWeatherData(WiFiClient & client, const String & RequestType) {
  const String units = (Units == "M" ? "metric" : "imperial");
  client.stop(); // close connection before sending a new request
  HTTPClient http;
  String uri = "/data/2.5/" + RequestType + "?q=" + City + "," + Country + "&APPID=" + apikey + "&mode=json&units=" + units + "&lang=" + Language;
  if(RequestType != "weather")
  {
    uri += "&cnt=" + String(max_readings);
  }
  //http.begin(uri,test_root_ca); //HTTPS example connection
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (!DecodeWeather(http.getStream(), RequestType)) return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}

float mm_to_inches(float value_mm)
{
  return 0.0393701 * value_mm;
}

float hPa_to_inHg(float value_hPa)
{
  return 0.02953 * value_hPa;
}

int JulianDate(int d, int m, int y) {
  int mm, yy, k1, k2, k3, j;
  yy = y - (int)((12 - m) / 10);
  mm = m + 9;
  if (mm >= 12) mm = mm - 12;
  k1 = (int)(365.25 * (yy + 4712));
  k2 = (int)(30.6001 * mm + 0.5);
  k3 = (int)((int)((yy / 100) + 49) * 0.75) - 38;
  // 'j' for dates in Julian calendar:
  j = k1 + k2 + d + 59 + 1;
  if (j > 2299160) j = j - k3; // 'j' is the Julian date at 12h UT (Universal Time) For Gregorian calendar:
  return j;
}

float SumOfPrecip(float DataArray[], int readings) {
  float sum = 0;
  for (int i = 0; i <= readings; i++) {
    sum += DataArray[i];
  }
  return sum;
}

String TitleCase(String text){
  if (text.length() > 0) {
    String temp_text = text.substring(0,1);
    temp_text.toUpperCase();
    return temp_text + text.substring(1); // Title-case the string
  }
  else return text;
}

double NormalizedMoonPhase(int d, int m, int y) {
  int j = JulianDate(d, m, y);
  //Calculate the approximate phase of the moon
  double Phase = (j + 4.867) / 29.53059;
  return (Phase - (int) Phase);
}

void DisplayWeather() {                          // 9.7" e-paper display is 1200x825 resolution
    DisplayStatusSection(990, 20, wifi_signal); // Wi-Fi signal strength and Battery voltage
    DisplayGeneralInfoSection();                 // Top line of the display

    DisplayDisplayWindSection(1000, 210, WxConditions[0].Winddir, WxConditions[0].Windspeed, 130);
    DisplayAstronomySection(920, 720);             // Astronomy section Sun rise/set, Moon phase and Moon icon
    DisplayMainWeatherSection(137, 130);          // Centre section of display for Location, temperature, Weather report, current Wx Symbol and wind direction
    DisplayForecastSection(10, 330);             // 3hr forecast boxes
}


void DisplayGeneralInfoSection() {
  setFont(OpenSans8B);
  drawString(4, 2, City, LEFT);
  // Uncomment the next line if the display of IP- and MAC-Adddress is wanted
  //drawString(SCREEN_WIDTH - 150, 20, "IP=" + LocalIP + ",  MAC=" + WiFi.macAddress() ,RIGHT);
  drawLine(5, 30, SCREEN_WIDTH - 8, 30, 0xAA);
  drawString(200, 2, Date_str, LEFT);
  drawString(400, 2, TXT_UPDATED + Time_str, LEFT);
}

void DisplayMainWeatherSection(int x, int y) {  // (x=500, y=190)
  DisplayConditionsSection(x + 3, y + 50, WxConditions[0].Icon, LargeIcon);
  DisplayTemperatureSection(x + 230, y - 30, 180, 170);
  DisplayPressureSection(x + 160, y + 70, 180, 170,  WxConditions[0].Pressure, WxConditions[0].Trend);
  DisplayPrecipitationSection(x + 268, y - 8, 181, 170);
  //DisplayForecastTextSection(x + 147, y + 22, 548, 90);
}

void DisplayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius) {
  arrow(x, y, Cradius - 22, angle, 18, 33); // Show wind direction on outer circle of width and length
  setFont(OpenSans12);
  int dxo, dyo, dxi, dyi;
  drawCircle(x, y, Cradius, GxEPD_BLACK);     // Draw compass circle
  drawCircle(x, y, Cradius + 1, GxEPD_BLACK); // Draw compass circle
  drawCircle(x, y, Cradius * 0.7, GxEPD_BLACK); // Draw compass inner circle
  for (float a = 0; a < 360; a = a + 22.5) {
    dxo = Cradius * cos((a - 90) * PI / 180);
    dyo = Cradius * sin((a - 90) * PI / 180);
    if (a == 45)  drawString(dxo + x + 27, dyo + y - 20, TXT_NE, CENTER);
    if (a == 135) drawString(dxo + x + 27, dyo + y - 2,  TXT_SE, CENTER);
    if (a == 225) drawString(dxo + x - 43, dyo + y - 2,  TXT_SW, CENTER);
    if (a == 315) drawString(dxo + x - 43, dyo + y - 20, TXT_NW, CENTER);
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    drawLine(dxo + x, dyo + y, dxi + x, dyi + y, GxEPD_BLACK);
    dxo = dxo * 0.7;
    dyo = dyo * 0.7;
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    drawLine(dxo + x, dyo + y, dxi + x, dyi + y, GxEPD_BLACK);
  }
  drawString(x - 3, y - Cradius - 30, TXT_N, CENTER);
  drawString(x - 5, y + Cradius + 18, TXT_S, CENTER);
  drawString(x - Cradius - 27, y - 11, TXT_W, CENTER);
  drawString(x + Cradius + 15, y - 11, TXT_E, CENTER);
  drawString(x - 12, y - 57, WindDegToDirection(angle), CENTER);
  drawString(x + 3, y + 50, String(angle, 0) + "°", CENTER);
  setFont(OpenSans24B);
  drawString(x + 3, y - 16, String(windspeed, 1), CENTER);
  //setFont(OpenSans12);
  //drawString(x + 16, y -12, (Units == "M" ? "m/s" : "mph"), LEFT);
}

String WindDegToDirection(float winddirection) {
  if (winddirection >= 348.75 || winddirection < 11.25)  return TXT_N;
  if (winddirection >=  11.25 && winddirection < 33.75)  return TXT_NNE;
  if (winddirection >=  33.75 && winddirection < 56.25)  return TXT_NE;
  if (winddirection >=  56.25 && winddirection < 78.75)  return TXT_ENE;
  if (winddirection >=  78.75 && winddirection < 101.25) return TXT_E;
  if (winddirection >= 101.25 && winddirection < 123.75) return TXT_ESE;
  if (winddirection >= 123.75 && winddirection < 146.25) return TXT_SE;
  if (winddirection >= 146.25 && winddirection < 168.75) return TXT_SSE;
  if (winddirection >= 168.75 && winddirection < 191.25) return TXT_S;
  if (winddirection >= 191.25 && winddirection < 213.75) return TXT_SSW;
  if (winddirection >= 213.75 && winddirection < 236.25) return TXT_SW;
  if (winddirection >= 236.25 && winddirection < 258.75) return TXT_WSW;
  if (winddirection >= 258.75 && winddirection < 281.25) return TXT_W;
  if (winddirection >= 281.25 && winddirection < 303.75) return TXT_WNW;
  if (winddirection >= 303.75 && winddirection < 326.25) return TXT_NW;
  if (winddirection >= 326.25 && winddirection < 348.75) return TXT_NNW;
  return "?";
}

void DisplayTemperatureSection(int x, int y, int twidth, int tdepth) {
  //setFont(OpenSans24/*24b*/);
  setFont(OpenSans24B);
  drawString(x, y, String(WxConditions[0].Temperature, 1) + "°C", CENTER); // Show current Temperature
  setFont(OpenSans16);
  drawString(x, y + 40, String(WxConditions[0].High, 0) + "° | " + String(WxConditions[0].Low, 0) + "°", CENTER); // Show forecast high and Low
}

void DisplayForecastTextSection(int x, int y , int fwidth, int fdepth) {
  String Wx_Description;
  setFont(OpenSans24/*18*/);
  if (Language == "DE")
    Wx_Description = WxConditions[0].Forecast0;
  else {
    Wx_Description = WxConditions[0].Main0;
    if (WxConditions[0].Forecast0 != "") Wx_Description += " (" + WxConditions[0].Forecast0;
  }
  if (WxConditions[0].Forecast1 != "") Wx_Description += ", " + WxConditions[0].Forecast1;
  if (WxConditions[0].Forecast2 != "") Wx_Description += ", " + WxConditions[0].Forecast2;
  if (Wx_Description.indexOf("(") > 0) Wx_Description += ")";
  int MsgWidth = 43; // Using proportional fonts, so be aware of making it too wide!
  if (Language == "DE") drawStringMaxWidth(x + 30, y + 40, MsgWidth, Wx_Description, LEFT); // Leave German text in original format, 28 character screen width at this font size
  else                  drawStringMaxWidth(x + 30, y + 40, MsgWidth, TitleCase(Wx_Description), LEFT); // 28 character screen width at this font size
}

void DisplayPressureSection(int x, int y, int pwidth, int pdepth, float pressure, String slope) {
  pressure = pressure * 0.750062; //convert to mmhg
  setFont(OpenSans12/*24b*/);
  drawString(x, y, String(pressure, (Units == "M"?0:1)) + (Units == "M" ? "mm" : "in"), LEFT);
}

void DisplayForecastWeather(int x, int y, int index) {
  int fwidth = 103;
  x = x + fwidth * (index - 1);
  DisplayConditionsSection(x + fwidth / 2, y + 90, WxForecast[index].Icon, SmallIcon);
  setFont(OpenSans8B);
  drawString(x + fwidth / 2 - 10, y + 30, String(WxForecast[index].Period.substring(11, 16)), CENTER);
  drawString(x + fwidth / 2 + 0, y + 130, String(WxForecast[index].High, 0) + "°/" + String(WxForecast[index].Low, 0) + "°", CENTER);
}

void DisplayPrecipitationSection(int x, int y, int pwidth, int pdepth) {
  setFont(OpenSans12);
  if (WxForecast[1].Rainfall >= 0.005) { // Ignore small amounts
    drawString(x, y + 80, String(WxForecast[1].Rainfall, 2) + (Units == "M" ? "mm" : "in"), LEFT); // Only display rainfall total today if > 0
    addraindrop(x + 102, y + 84, 7);
  }
  if (WxForecast[1].Snowfall >= 0.005)  {// Ignore small amounts
    drawString(x, y + 110, String(WxForecast[1].Snowfall, 2) + (Units == "M" ? "mm" : "in") + " **", LEFT); // Only display snowfall total today if > 0
  }
}

void DisplayAstronomySection(int x, int y) {
  setFont(OpenSans12B);
  drawString(x + 14, y + 34, ConvertUnixTime(WxConditions[0].Sunrise).substring(0, 5) + " " + TXT_SUNRISE, LEFT);
  drawString(x + 14, y + 64, ConvertUnixTime(WxConditions[0].Sunset).substring(0, 5) + " " + TXT_SUNSET, LEFT);
  time_t now = time(NULL);
  struct tm * now_utc  = gmtime(&now);
  const int day_utc = now_utc->tm_mday;
  const int month_utc = now_utc->tm_mon + 1;
  const int year_utc = now_utc->tm_year + 1900;
  //drawString(x + 14, y + 94, MoonPhase(day_utc, month_utc, year_utc, Hemisphere), LEFT);
  DrawMoon(x + 155, y - 20, day_utc, month_utc, year_utc, Hemisphere);
}

void DrawMoon(int x, int y, int dd, int mm, int yy, String hemisphere) {
  const int diameter = 75;
  double Phase = NormalizedMoonPhase(dd, mm, yy);
  hemisphere.toLowerCase();
  if (hemisphere == "south") Phase = 1 - Phase;
  // Draw dark part of moon
  fillCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, GxEPD_BLACK);
  const int number_of_lines = 90;
  for (double Ypos = 0; Ypos <= number_of_lines / 2; Ypos++) {
    double Xpos = sqrt(number_of_lines / 2 * number_of_lines / 2 - Ypos * Ypos);
    // Determine the edges of the lighted part of the moon
    double Rpos = 2 * Xpos;
    double Xpos1, Xpos2;
    if (Phase < 0.5) {
      Xpos1 = -Xpos;
      Xpos2 = Rpos - 2 * Phase * Rpos - Xpos;
    }
    else {
      Xpos1 = Xpos;
      Xpos2 = Xpos - 2 * Phase * Rpos + Rpos;
    }
    // Draw light part of moon
    double pW1x = (Xpos1 + number_of_lines) / number_of_lines * diameter + x;
    double pW1y = (number_of_lines - Ypos)  / number_of_lines * diameter + y;
    double pW2x = (Xpos2 + number_of_lines) / number_of_lines * diameter + x;
    double pW2y = (number_of_lines - Ypos)  / number_of_lines * diameter + y;
    double pW3x = (Xpos1 + number_of_lines) / number_of_lines * diameter + x;
    double pW3y = (Ypos + number_of_lines)  / number_of_lines * diameter + y;
    double pW4x = (Xpos2 + number_of_lines) / number_of_lines * diameter + x;
    double pW4y = (Ypos + number_of_lines)  / number_of_lines * diameter + y;
    drawLine(pW1x, pW1y, pW2x, pW2y, GxEPD_WHITE);
    drawLine(pW3x, pW3y, pW4x, pW4y, GxEPD_WHITE);
  }
  drawCircle(x + diameter - 1, y + diameter, diameter / 2, GxEPD_BLACK);
}

String MoonPhase(int d, int m, int y, String hemisphere) {
  int c, e;
  double jd;
  int b;
  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  c   = 365.25 * y;
  e   = 30.6  * m;
  jd  = c + e + d - 694039.09;     /* jd is total days elapsed */
  jd /= 29.53059;                        /* divide by the moon cycle (29.53 days) */
  b   = jd;                              /* int(jd) -> b, take integer part of jd */
  jd -= b;                               /* subtract integer part to leave fractional part of original jd */
  b   = jd * 8 + 0.5;                /* scale fraction from 0-8 and round by adding 0.5 */
  b   = b & 7;                           /* 0 and 8 are the same phase so modulo 8 for 0 */
  if (hemisphere == "south") b = 7 - b;
  if (b == 0) return TXT_MOON_NEW;              // New;              0%  illuminated
  if (b == 1) return TXT_MOON_WAXING_CRESCENT;  // Waxing crescent; 25%  illuminated
  if (b == 2) return TXT_MOON_FIRST_QUARTER;    // First quarter;   50%  illuminated
  if (b == 3) return TXT_MOON_WAXING_GIBBOUS;   // Waxing gibbous;  75%  illuminated
  if (b == 4) return TXT_MOON_FULL;             // Full;            100% illuminated
  if (b == 5) return TXT_MOON_WANING_GIBBOUS;   // Waning gibbous;  75%  illuminated
  if (b == 6) return TXT_MOON_THIRD_QUARTER;    // Third quarter;   50%  illuminated
  if (b == 7) return TXT_MOON_WANING_CRESCENT;  // Waning crescent; 25%  illuminated
  return "";
}

void DisplayForecastSection(int x, int y) {
  setFont(OpenSans12/*9*/);
  int f = 1;
  do {
    DisplayForecastWeather(x, y, f);
    f++;
  } while (f < max_readings);
  // Pre-load temporary arrays with with data - because C parses by reference
  int r = 1;
  do {
    if (Units == "I") pressure_readings[r] = WxForecast[r].Pressure * 0.02953;   else pressure_readings[r] = WxForecast[r].Pressure;
    if (Units == "I") rain_readings[r]     = WxForecast[r].Rainfall * 0.0393701; else rain_readings[r]     = WxForecast[r].Rainfall;
    if (Units == "I") snow_readings[r]     = WxForecast[r].Snowfall * 0.0393701; else snow_readings[r]     = WxForecast[r].Snowfall;
    temperature_readings[r] = WxForecast[r].Temperature;
    humidity_readings[r]    = WxForecast[r].Humidity;
    r++;
  } while (r <= max_readings);

  int gwidth = 230, gheight = 150;
  int gx = (SCREEN_WIDTH - gwidth * 4) / 5 + 7;
  int gy = (SCREEN_HEIGHT - gheight - 50);
  int gap = gwidth + gx;
  //setFont(OpenSans12/*18*/);
  //drawString(SCREEN_WIDTH / 2 - 50, gy - 50, TXT_FORECAST_VALUES, CENTER); // Based on a graph height of 60
  //setFont(OpenSans12);
  // (x,y,width,height,MinValue, MaxValue, Title, Data Array, AutoScale, ChartMode)
  //DrawGraph(gx + 0 * gap, gy, gwidth, gheight, 900, 1050, Units == "M" ? TXT_PRESSURE_HPA : TXT_PRESSURE_IN, pressure_readings, max_readings, autoscale_on, barchart_off);
  DrawGraph(gx + 0 * gap, gy, gwidth, gheight, 10, 30,    Units == "M" ? TXT_TEMPERATURE_C : TXT_TEMPERATURE_F, temperature_readings, max_readings, autoscale_on, barchart_off);
  //DrawGraph(gx + 1 * gap, gy, gwidth, gheight, 0, 100,   TXT_HUMIDITY_PERCENT, humidity_readings, max_readings, autoscale_off, barchart_off);
  if (SumOfPrecip(rain_readings, max_readings) >= SumOfPrecip(snow_readings, max_readings))
    DrawGraph(gx + 1 * gap + 5, gy, gwidth, gheight, 0, 30, Units == "M" ? TXT_RAINFALL_MM : TXT_RAINFALL_IN, rain_readings, max_readings, autoscale_on, barchart_on);
  else DrawGraph(gx + 1 * gap + 5, gy, gwidth, gheight, 0, 30, Units == "M" ? TXT_SNOWFALL_MM : TXT_SNOWFALL_IN, snow_readings, max_readings, autoscale_on, barchart_on);
}

void DisplayConditionsSection(int x, int y, String IconName, bool IconSize) {
  Serial.println("Icon name: " + IconName);
  if      (IconName == "01d" || IconName == "01n")  Sunny(x, y, IconSize, IconName);
  else if (IconName == "02d" || IconName == "02n")  MostlySunny(x, y, IconSize, IconName);
  else if (IconName == "03d" || IconName == "03n")  Cloudy(x, y, IconSize, IconName);
  else if (IconName == "04d" || IconName == "04n")  MostlySunny(x, y, IconSize, IconName);
  else if (IconName == "09d" || IconName == "09n")  ChanceRain(x, y, IconSize, IconName);
  else if (IconName == "10d" || IconName == "10n")  Rain(x, y, IconSize, IconName);
  else if (IconName == "11d" || IconName == "11n")  Tstorms(x, y, IconSize, IconName);
  else if (IconName == "13d" || IconName == "13n")  Snow(x, y, IconSize, IconName);
  else if (IconName == "50d")                       Haze(x, y, IconSize, IconName);
  else if (IconName == "50n")                       Fog(x, y, IconSize, IconName);
  else                                              Nodata(x, y, IconSize, IconName);
  if (IconSize == LargeIcon) {
    setFont(OpenSans16B/*18*/);
    drawString(x + 360, y - 74, String(WxConditions[0].Humidity, 0) + "%", CENTER);
    if (WxConditions[0].Visibility > 0) Visibility(x - 100, y + 130, String(WxConditions[0].Visibility) + "M");
    if (WxConditions[0].Cloudcover > 0) CloudCover(x + 60, y + 130, WxConditions[0].Cloudcover);
  }
}

void arrow(int x, int y, int asize, float aangle, int pwidth, int plength) {
  float dx = (asize - 10) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize - 10) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  float x1 = 0;         float y1 = plength;
  float x2 = pwidth / 2;  float y2 = pwidth / 2;
  float x3 = -pwidth / 2; float y3 = pwidth / 2;
  float angle = aangle * PI / 180 - 135;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
  fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, GxEPD_BLACK);
}


void DisplayStatusSection(int x, int y, int rssi) {
  setFont(OpenSans8B);
  DrawRSSI(x, y + 4, rssi);
  //DrawBattery(x + 100, y + 4);
}

void DrawRSSI(int x, int y, int rssi) {
  int WIFIsignal = 0;
  int xpos = 1;
  for (int _rssi = -100; _rssi <= rssi; _rssi = _rssi + 20) {
    if (_rssi <= -20)  WIFIsignal = 30; //            <-20dbm displays 5-bars
    if (_rssi <= -40)  WIFIsignal = 24; //  -40dbm to  -21dbm displays 4-bars
    if (_rssi <= -60)  WIFIsignal = 18; //  -60dbm to  -41dbm displays 3-bars
    if (_rssi <= -80)  WIFIsignal = 12; //  -80dbm to  -61dbm displays 2-bars
    if (_rssi <= -100) WIFIsignal = 6;  // -100dbm to  -81dbm displays 1-bar
    fillRect(x + xpos * 8, y - WIFIsignal, 7, WIFIsignal, GxEPD_BLACK);
    xpos++;
  }
  //fillRect(x, y - 1, 5, 1, GxEPD_BLACK);
  //drawString(x + 6,  y + 10, String(rssi) + "dBm", CENTER);
}

boolean UpdateLocalTime() {
  struct tm timeinfo;
  char   time_output[30], day_output[30], update_time[30];
  while (!getLocalTime(&timeinfo, 5000)) { // Wait for 5-sec for time to synchronise
    Serial.println("Failed to obtain time");
    return false;
  }
  CurrentHour = timeinfo.tm_hour;
  CurrentMin  = timeinfo.tm_min;
  CurrentSec  = timeinfo.tm_sec;
  //See http://www.cplusplus.com/reference/ctime/strftime/
  Serial.println("***Current time***");
  Serial.println(&timeinfo, "%a %b %d %Y   %H:%M:%S");      // Displays: Saturday, June 24 2017 14:05:49
  if (Units == "M") {
    if ((Language == "CZ") || (Language == "DE") || (Language == "NL") || (Language == "PL")) {
      sprintf(day_output, "%s, %02u. %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900); // day_output >> So., 23. Juni 2019 <<
    }
    else
    {
      sprintf(day_output, "%s, %02u %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    }
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeinfo);  // Creates: '@ 14:05:49'   and change from 30 to 8 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sprintf(time_output, "%s", update_time);
  }
  else
  {
    strftime(day_output, sizeof(day_output), "%a %b-%d-%Y", &timeinfo); // Creates  'Sat May-31-2019'
    strftime(update_time, sizeof(update_time), "%r", &timeinfo);        // Creates: '@ 02:05:49pm'
    sprintf(time_output, "%s", update_time);
  }
  Date_str = day_output;
  Time_str = time_output;
  return true;
}

void DrawBattery(int x, int y) {
  uint8_t percentage = 100;
  float voltage = analogRead(35) / 4096.0 * 7.46;
  if (voltage > 1 ) { // Only display if there is a valid reading
    Serial.println("Voltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.20) percentage = 0;  // orig 3.5
    drawRect(x + 25, y - 20, 40, 15, GxEPD_BLACK);
    fillRect(x + 65, y - 16, 4, 6, GxEPD_BLACK);
    fillRect(x + 27, y - 18, 36 * percentage / 100.0, 11, GxEPD_BLACK);
    setFont(OpenSans8B/*9*/);
    drawString(x + 5, y - 15, String(percentage) + "%", RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
  }
}

// Symbols are drawn on a relative 10x10grid and 1 scale unit = 1 drawing unit
void addcloud(int x, int y, int scale, int linesize) {
  //Draw cloud outer
  fillCircle(x - scale * 3, y, scale, GxEPD_BLACK);                // Left most circle
  fillCircle(x + scale * 3, y, scale, GxEPD_BLACK);                // Right most circle
  fillCircle(x - scale, y - scale, scale * 1.4, GxEPD_BLACK);    // left middle upper circle
  fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75, GxEPD_BLACK); // Right middle upper circle
  fillRect(x - scale * 3 - 1, y - scale, scale * 6, scale * 2 + 1, GxEPD_BLACK); // Upper and lower lines
  //Clear cloud inner
  fillCircle(x - scale * 3, y, scale - linesize, GxEPD_WHITE);            // Clear left most circle
  fillCircle(x + scale * 3, y, scale - linesize, GxEPD_WHITE);            // Clear right most circle
  fillCircle(x - scale, y - scale, scale * 1.4 - linesize, GxEPD_WHITE);  // left middle upper circle
  fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75 - linesize, GxEPD_WHITE); // Right middle upper circle
  fillRect(x - scale * 3 + 2, y - scale + linesize - 1, scale * 5.9, scale * 2 - linesize * 2 + 2, GxEPD_WHITE); // Upper and lower lines
}

void addraindrop(int x, int y, int scale) {
  fillCircle(x, y, scale / 2, GxEPD_BLACK);
  fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
  x = x + scale * 1.6; y = y + scale / 3;
  fillCircle(x, y, scale / 2, GxEPD_BLACK);
  fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
}

void addrain(int x, int y, int scale, bool IconSize) {
  if (IconSize == SmallIcon) scale *= 1.34;
  for (int d = 0; d < 4; d++) {
    addraindrop(x + scale * (7.8 - d * 1.95) - scale * 5.2, y + scale * 2.1 - scale / 6, scale / 1.6);
  }
}

void addsnow(int x, int y, int scale, bool IconSize) {
  int dxo, dyo, dxi, dyi;
  for (int flakes = 0; flakes < 5; flakes++) {
    for (int i = 0; i < 360; i = i + 45) {
      dxo = 0.5 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.1;
      dyo = 0.5 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.1;
      drawLine(dxo + x + flakes * 1.5 * scale - scale * 3, dyo + y + scale * 2, dxi + x + 0 + flakes * 1.5 * scale - scale * 3, dyi + y + scale * 2, GxEPD_BLACK);
    }
  }
}

void addtstorm(int x, int y, int scale) {
  y = y + scale / 2;
  for (int i = 0; i < 5; i++) {
    drawLine(x - scale * 4 + scale * i * 1.5 + 0, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 0, y + scale, GxEPD_BLACK);
    if (scale != Small) {
      drawLine(x - scale * 4 + scale * i * 1.5 + 1, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 1, y + scale, GxEPD_BLACK);
      drawLine(x - scale * 4 + scale * i * 1.5 + 2, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 2, y + scale, GxEPD_BLACK);
    }
    drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 0, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 0, GxEPD_BLACK);
    if (scale != Small) {
      drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 1, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 1, GxEPD_BLACK);
      drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 2, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 2, GxEPD_BLACK);
    }
    drawLine(x - scale * 3.5 + scale * i * 1.4 + 0, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5, GxEPD_BLACK);
    if (scale != Small) {
      drawLine(x - scale * 3.5 + scale * i * 1.4 + 1, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 1, y + scale * 1.5, GxEPD_BLACK);
      drawLine(x - scale * 3.5 + scale * i * 1.4 + 2, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 2, y + scale * 1.5, GxEPD_BLACK);
    }
  }
}

void addsun(int x, int y, int scale, bool IconSize) {
  int linesize = 3;
  if (IconSize == SmallIcon) linesize = 1;
  fillRect(x - scale * 2, y, scale * 4, linesize, GxEPD_BLACK);
  fillRect(x, y - scale * 2, linesize, scale * 4, GxEPD_BLACK);
  drawLine(x - scale * 1.3, y - scale * 1.3, x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
  drawLine(x - scale * 1.3, y + scale * 1.3, x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  if (IconSize == LargeIcon) {
    drawLine(1 + x - scale * 1.3, y - scale * 1.3, 1 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    drawLine(2 + x - scale * 1.3, y - scale * 1.3, 2 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    drawLine(3 + x - scale * 1.3, y - scale * 1.3, 3 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    drawLine(1 + x - scale * 1.3, y + scale * 1.3, 1 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    drawLine(2 + x - scale * 1.3, y + scale * 1.3, 2 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    drawLine(3 + x - scale * 1.3, y + scale * 1.3, 3 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  }
  fillCircle(x, y, scale * 1.3, GxEPD_WHITE);
  fillCircle(x, y, scale, GxEPD_BLACK);
  fillCircle(x, y, scale - linesize, GxEPD_WHITE);
}

void addfog(int x, int y, int scale, int linesize, bool IconSize) {
  if (IconSize == SmallIcon) {
    y -= 10;
    linesize = 1;
  }
  for (int i = 0; i < 6; i++) {
    fillRect(x - scale * 3, y + scale * 1.5, scale * 6, linesize, GxEPD_BLACK);
    fillRect(x - scale * 3, y + scale * 2.0, scale * 6, linesize, GxEPD_BLACK);
    fillRect(x - scale * 3, y + scale * 2.5, scale * 6, linesize, GxEPD_BLACK);
  }
}

void Sunny(int x, int y, bool IconSize, String IconName) {
  int scale = Small;
  if (IconSize == LargeIcon) scale = Large;
  else y = y - 3; // Shift up small sun icon
  if (IconName.endsWith("n")) addmoon(x, y + 3, scale, IconSize);
  scale = scale * 1.6;
  addsun(x, y, scale, IconSize);
}

void MostlySunny(int x, int y, bool IconSize, String IconName) {
  int scale = Small, linesize = 3, offset = 5;
  if (IconSize == LargeIcon) {
    scale = Large;
    offset = 10;
  }
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y + offset, scale, IconSize);
  addcloud(x, y + offset, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8 + offset, scale, IconSize);
}

void MostlyCloudy(int x, int y, bool IconSize, String IconName) {
  int scale = Small, linesize = 3;
  if (IconSize == LargeIcon) {
    scale = Large;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, IconSize);
  addcloud(x, y, scale, linesize);
}

void Cloudy(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
    linesize = 1;
    addcloud(x, y, scale, linesize);
  }
  else {
    y += 10;
    if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
    addcloud(x + 30, y - 45, 5, linesize); // Cloud top right
    addcloud(x - 20, y - 30, 7, linesize); // Cloud top left
    addcloud(x, y, scale, linesize);       // Main cloud
  }
}

void Rain(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, IconSize);
}

void ExpectRain(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, IconSize);
}

void ChanceRain(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, IconSize);
}

void Tstorms(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addtstorm(x, y, scale);
}

void Snow(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addsnow(x, y, scale, IconSize);
}

void Fog(int x, int y, bool IconSize, String IconName) {
  int linesize = 3, scale = Large;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y - 5, scale, linesize);
  addfog(x, y - 5, scale, linesize, IconSize);
}

void Haze(int x, int y, bool IconSize, String IconName) {
  int linesize = 3, scale = Large;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addsun(x, y - 5, scale * 1.4, IconSize);
  addfog(x, y - 5, scale * 1.4, linesize, IconSize);
}

void CloudCover(int x, int y, int CCover) {
  addcloud(x - 9, y - 3, Small * 0.8, 2); // Cloud top left
  addcloud(x + 3, y - 3, Small * 0.8, 2); // Cloud top right
  addcloud(x, y,         Small * 0.8, 2); // Main cloud
  setFont(OpenSans12);
  drawString(x + 44, y - 14, String(CCover) + "%", LEFT);
}

void Visibility(int x, int y, String Visi) {
  y = y - 3; //
  float start_angle = 0.52, end_angle = 2.61;
  int r = 14;
  for (float i = start_angle; i < end_angle; i = i + 0.05) {
    drawPixel(x + r * cos(i), y - r / 2 + r * sin(i), GxEPD_BLACK);
    drawPixel(x + r * cos(i), 1 + y - r / 2 + r * sin(i), GxEPD_BLACK);
  }
  start_angle = 3.61; end_angle = 5.78;
  for (float i = start_angle; i < end_angle; i = i + 0.05) {
    drawPixel(x + r * cos(i), y + r / 2 + r * sin(i), GxEPD_BLACK);
    drawPixel(x + r * cos(i), 1 + y + r / 2 + r * sin(i), GxEPD_BLACK);
  }
  fillCircle(x, y, r / 4, GxEPD_BLACK);
  setFont(OpenSans12);
  drawString(x + 14, y - 12, Visi, LEFT);
}

void addmoon(int x, int y, int scale, bool IconSize) {
  if (IconSize == LargeIcon) {
    fillCircle(x - 85, y - 100, uint16_t(scale * 0.8), GxEPD_BLACK);
    fillCircle(x - 57, y - 100, uint16_t(scale * 1.6), GxEPD_WHITE);
  }
  else
  {
    fillCircle(x - 28, y - 37, uint16_t(scale * 1.0), GxEPD_BLACK);
    fillCircle(x - 20, y - 37, uint16_t(scale * 1.6), GxEPD_WHITE);
  }
}

void Nodata(int x, int y, bool IconSize, String IconName) {
  if (IconSize == LargeIcon) setFont(OpenSans24); else setFont(OpenSans12/*9*/);
  drawString(x - 3, y - 10, "?", CENTER);
  setFont(OpenSans12/*9*/);
}

/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data from an array containing data to be graphed.
    The variable 'max_readings' determines the maximum number of data elements for each array. Call it with the following parametric data:
    x_pos-the x axis top-left position of the graph
    y_pos-the y-axis top-left position of the graph, e.g. 100, 200 would draw the graph 100 pixels along and 200 pixels down from the top-left of the screen
    width-the width of the graph in pixels
    height-height of the graph in pixels
    Y1_Max-sets the scale of plotted data, for example 5000 would scale all data to a Y-axis of 5000 maximum
    data_array1 is parsed by value, externally they can be called anything else, e.g. within the routine it is called data_array1, but externally could be temperature_readings
    auto_scale-a logical value (TRUE or FALSE) that switches the Y-axis autoscale On or Off
    barchart_on-a logical value (TRUE or FALSE) that switches the drawing mode between barhcart and line graph
    barchart_colour-a sets the title and graph plotting colour
    If called with Y!_Max value of 500 and the data never goes above 500, then autoscale will retain a 0-500 Y scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale by 1000 steps.
*/
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode) {
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up in units of e.g. 3
#define y_minor_axis 5      // 5 y-axis division markers
  int maxYscale = -10000;
  int minYscale =  10000;
  int last_x, last_y;
  float x2, y2;
  if (auto_scale == true) {
    for (int i = 1; i < readings; i++ ) {
      if (DataArray[i] >= maxYscale) maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale) minYscale = DataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
    Y1Max = round(maxYscale + 0.5);
    if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
    Y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
  drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, GRAY);
  drawString(x_pos - 20 + gwidth / 2, y_pos - 32, title, CENTER);
  // Draw the data
  // x_pos = x_pos + 10;
  for (int gx = 1; gx < readings; gx++) {
    x2 = x_pos + gx * gwidth / (readings - 1) - 1 ; // max_readings is the global variable that sets the maximum data that can be plotted
    y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
    if (barchart_mode) {
      fillRect(last_x + 2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 2, GxEPD_BLACK);
    } else {
      drawLine(last_x, last_y, x2, y2, GxEPD_BLACK);
    }
    last_x = x2;
    last_y = y2;
  }
  //Draw the Y-axis scale
#define number_of_dashes 20
  for (int spacing = 0; spacing <= y_minor_axis; spacing++) {
    for (int j = 0; j < number_of_dashes; j++) { // Draw dashed graph grid lines
      if (spacing < y_minor_axis) drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), GRAY);
    }
    if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 5 || title == TXT_PRESSURE_IN) {
        drawString(x_pos - 10, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
    }
    else
    {
      if (Y1Min < 1 && Y1Max < 10) {
        drawString(x_pos - 10, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
      }
      else {
        drawString(x_pos - 18, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0), RIGHT);
      }
    }
  }
  for (int i = 0; i <= 2; i++) {
    drawString(5 + x_pos + gwidth / 3 * i, y_pos + gheight + 10, String(i)+"d", LEFT);
  }
  //drawString(x_pos + gwidth / 2, y_pos + gheight + 25, TXT_DAYS, CENTER);
}


void drawString(int x, int y, String text, alignment align) {
  char * data  = const_cast<char*>(text.c_str());
  int  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  int w, h;
  int xx = x, yy = y;
  EpdFontProperties fp = epd_font_properties_default();
  epd_get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, &fp);

  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;

  int cursor_y = y + h;
  epd_write_default(&currentFont, data, &x, &cursor_y, fb);
}

void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, alignment align) {
  char * data = const_cast<char*>(text.c_str());
  int  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  int w, h;
  int xx = x, yy = y;

  EpdFontProperties fp = epd_font_properties_default();
  epd_get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, &fp);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;

  if (text.length() > text_width * 2) {
    setFont(OpenSans12/*9*/);
    text_width = 42;
    y = y - 3;
  }
  epd_write_default(&currentFont, data, &x, &y, fb);

  if (text.length() > text_width) {
    y += h + 15;
    String secondLine = text.substring(text_width);
    secondLine.trim(); // Remove any leading spac
    epd_write_default(&currentFont, data, &x, &y, fb);
  }
}


void ReportEvent(String EventMessage[]) {
  int y = int(SCREEN_HEIGHT - 40 * (EventCnt + 1) - 2 * 40);
  int wx = int (SCREEN_WIDTH * 0.1);
  int wy = y + int (SCREEN_WIDTH * 0.1);
  int tbw = int (SCREEN_WIDTH * 0.8);
  int tbh = int ((EventCnt + 1) * 40);
  if (EventCnt > EventThreshold) {
    //display.setPartialWindow(wx, wy, tbw, tbh);

    EpdRect to_clear = {
      .x = wx,
      .y = wy,
      .width = tbw,
      .height = tbh,
    };
    epd_poweron();
    epd_clear_area(to_clear);

    fillRect(wx, wy, tbw, tbh, GxEPD_WHITE);
    drawRect(wx, wy, tbw, tbh, GxEPD_BLACK);
  }
  setFont(OpenSans12/*18*/);
  for (byte Event = 1; Event <= EventCnt; Event++) {
    if (EventCnt > EventThreshold) drawString(wx + 3, wy + 20 + (Event - 1) * 30,
                                   "Evt#" + String(Event < 10 ? "0" : "") + String(Event) + " : " + EventMessage[Event], LEFT);
    Serial.println("Evnt#" + String(Event < 10 ? "0" : "") + String(Event) + " : " + EventMessage[Event]);
  }
  epd_poweroff();
}

/*void VerboseRecordOfResetReason(RESET_REASON reason) {
  switch ( reason)  {
    case 1  : AddToEventLog("Vbat power on reset"); break;
    case 3  : AddToEventLog("Software reset digital core"); break;
    case 4  : AddToEventLog("Legacy watch dog reset digital core"); break;
    case 5  : AddToEventLog("Deep Sleep reset digital core"); break;
    case 6  : AddToEventLog("Reset by SLC module, reset digital core"); break;
    case 7  : AddToEventLog("Timer Group0 Watch dog reset digital core"); break;
    case 8  : AddToEventLog("Timer Group1 Watch dog reset digital core"); break;
    case 9  : AddToEventLog("RTC Watch dog Reset digital core"); break;
    case 10 : AddToEventLog("Instrusion tested to reset CPU"); break;
    case 11 : AddToEventLog("Time Group reset CPU"); break;
    case 12 : AddToEventLog("Software reset CPU"); break;
    case 13 : AddToEventLog("RTC Watch dog Reset CPU"); break;
    case 14 : AddToEventLog("APP CPU reset by PRO CPU"); break;
    case 15 : AddToEventLog("Reset when Vdd voltage is not stable"); break;
    case 16 : AddToEventLog("RTC Watch dog reset digital core and rtc module"); break;
    default : AddToEventLog("Unknown reason");
  }
}*/

void DisplayTime() {
  drawString(850, 510, "16:00", CENTER);
}

void fillCircle(int x, int y, int r, uint8_t color) {
  epd_fill_circle(x, y, r, color, fb);
}

void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color) {
  epd_draw_hline(x0, y0, length, color, fb);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  epd_draw_line(x0, y0, x1, y1, color, fb);
}

void drawCircle(int x0, int y0, int r, uint8_t color) {
  epd_draw_circle(x0, y0, r, color, fb);
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
  EpdRect rect = {
      .x = x,
      .y = y,
      .width = w,
      .height = h
  };
  epd_draw_rect(rect, color, fb);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
  EpdRect rect = {
      .x = x,
      .y = y,
      .width = w,
      .height = h
  };
  epd_fill_rect(rect, color, fb);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color) {
  epd_fill_triangle(x0, y0, x1, y1, x2, y2, color,fb);
}

void drawPixel(int x, int y, uint8_t color) {
  epd_draw_pixel(x, y, color, fb);
}

void setFont(EpdFont const &font) {
  currentFont = font;
}








