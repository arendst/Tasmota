#include "Arduino.h"
#include "wifi_info.h"
#include "hap.h"

extern "C" void homekit_main();

void setup() {
	Serial.begin(115200);
	Serial.println("Boot OK");
	wifi_connect();
	// Useful apis: (see hap.h)
	// hap_reset_to_factory();
	// hap_reset_homekit_data();
	// hap_reset_pairings();
	homekit_main();
	vTaskDelete(NULL);
}

void loop() {
}
