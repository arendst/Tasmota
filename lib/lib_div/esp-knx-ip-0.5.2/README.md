# ESP-KNX-IP #

This is a library for the ESP8266 to enable KNXnet/IP communication. It uses UDP multicast on 224.0.23.12:3671.
It is intended to be used with the Arduino platform for the ESP8266.

## How to use ##

The library is under development. API may change multiple times in the future.

API documentation is available [here](https://github.com/envy/esp-knx-ip/wiki/API)

A simple example:

```c++
#include <esp-knx-ip.h>

const char* ssid = "my-ssid";  //  your network SSID (name)
const char* pass = "my-pw";    // your network password

config_id_t my_GA;
config_id_t param_id;

int8_t some_var = 0;

void setup()
{
	// Register a callback that is called when a configurable group address is receiving a telegram
	knx.register_callback("Set/Get callback", my_callback);
	knx.register_callback("Write callback", my_other_callback);

	int default_val = 21;
	param_id = knx.config_register_int("My Parameter", default_val);

	// Register a configurable group address for sending out answers
	my_GA = knx.config_register_ga("Answer GA");

	knx.load(); // Try to load a config from EEPROM

	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

	knx.start(); // Start everything. Must be called after WiFi connection has been established
}

void loop()
{
	knx.loop();
}


void my_callback(message_t const &msg, void *arg)
{
	switch (msg.ct)
	{
	case KNX_CT_WRITE:
		// Save received data
		some_var = knx.data_to_1byte_int(msg.data);
		break;
	case KNX_CT_READ:
		// Answer with saved data
		knx.answer1ByteInt(msg.received_on, some_var);
		break;
	}
}

void my_other_callback(message_t const &msg, void *arg)
{
	switch (msg.ct)
	{
	case KNX_CT_WRITE:
		// Write an answer somewhere else
		int value = knx.config_get_int(param_id);
		address_t ga = knx.config_get_ga(my_GA);
		knx.answer1ByteInt(ga, (int8_t)value);
		break;
	}
}

```

## How to configure (buildtime) ##

Open the `esp-knx-ip.h` and take a look at the config options at the top inside the block marked `CONFIG`

## How to configure (runtime) ##

Simply visit the IP of your ESP with a webbrowser. You can configure the following:
* KNX physical address
* Which group address should trigger which callback
* Which group address are to be used by the program (e.g. for status replies)

The configuration is dynamically generated from the code.
