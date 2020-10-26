/*
 * This is an example showing a simple environment sensor based on a BME280 attached via I2C.
 * This sketch was tested on a WeMos D1 mini
 */

#include <Adafruit_BME280.h>
#include <esp-knx-ip.h>

// WiFi config here
const char* ssid = "myssid";
const char* pass = "mypassword";

#define LED_PIN D4
#define UPDATE_INTERVAL 10000

unsigned long next_change = 0;

float last_temp = 0.0;
float last_hum = 0.0;
float last_pres = 0.0;

config_id_t temp_ga, hum_ga, pres_ga;
config_id_t hostname_id;
config_id_t update_rate_id, send_rate_id;
config_id_t enable_sending_id;
config_id_t enable_reading_id;

Adafruit_BME280 bme;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  hostname_id = knx.config_register_string("Hostname", 20, String("env"));
  enable_sending_id = knx.config_register_bool("Send on update", true);
  update_rate_id = knx.config_register_int("Update rate (ms)", UPDATE_INTERVAL);
  temp_ga = knx.config_register_ga("Temperature", show_periodic_options);
  hum_ga = knx.config_register_ga("Humidity", show_periodic_options);
  pres_ga = knx.config_register_ga("Pressure", show_periodic_options);

  knx.callback_register("Read Temperature", temp_cb);
  knx.callback_register("Read Humidity", hum_cb);
  knx.callback_register("Read Pressure", pres_cb);

  knx.feedback_register_float("Temperature (°C)", &last_temp);
  knx.feedback_register_float("Humidity (%)", &last_hum);
  knx.feedback_register_float("Pressure (hPa)", &last_pres, 0);

  // Load previous config from EEPROM
  knx.load();

  // Init sensor
  if (!bme.begin(0x76)) {  
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  // Init WiFi
  WiFi.hostname(knx.config_get_string(hostname_id));
  WiFi.begin(ssid, pass);

  Serial.println("");
  Serial.print("[Connecting]");
  Serial.print(ssid);

  digitalWrite(LED_PIN, LOW);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }
  digitalWrite(LED_PIN, HIGH);

  // Start knx
  knx.start();

  Serial.println();
  Serial.println("Connected to wifi");
  Serial.println(WiFi.localIP());
}

void loop() {
  knx.loop();
  
  unsigned long now = millis();

  if (next_change < now)
  {
    next_change = now + knx.config_get_int(update_rate_id);

    last_temp = bme.readTemperature();
    last_hum = bme.readHumidity();
    last_pres = bme.readPressure()/100.0f;

    Serial.print("T: ");
    Serial.print(last_temp);
    Serial.print("°C H: ");
    Serial.print(last_hum);
    Serial.print("% P: ");
    Serial.print(last_pres);
    Serial.println("hPa");

    if (knx.config_get_bool(enable_sending_id))
    {
      knx.write_2byte_float(knx.config_get_ga(temp_ga), last_temp);
      knx.write_2byte_float(knx.config_get_ga(hum_ga), last_hum);
      knx.write_2byte_float(knx.config_get_ga(pres_ga), last_pres);
    }
  }

  delay(50);
}

bool show_periodic_options()
{
  return knx.config_get_bool(enable_sending_id);
}

bool enable_reading_callback()
{
  return knx.config_get_bool(enable_reading_id);
}

void temp_cb(message_t const &msg, void *arg)
{
  switch (msg.ct)
  {
    case KNX_CT_READ:
    {
      knx.answer_2byte_float(msg.received_on, last_temp);
      break;
    }
  }
}

void hum_cb(message_t const &msg, void *arg)
{
  switch (msg.ct)
  {
    case KNX_CT_READ:
    {
      knx.answer_2byte_float(msg.received_on, last_hum);
      break;
    }
  }
}

void pres_cb(message_t const &msg, void *arg)
{
  switch (msg.ct)
  {
    case KNX_CT_READ:
    {
      knx.answer_2byte_float(msg.received_on, last_pres);
      break;
    }
  }
}
