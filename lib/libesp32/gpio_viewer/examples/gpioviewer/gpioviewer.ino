/***
This example is intended to demonstrate the use of the GPIO Viewer Library.

Tutorial : https://youtu.be/UxkOosaNohU
Documentation : https://github.com/thelastoutpostworkshop/gpio_viewer
***/

#include <gpio_viewer.h> // Must me the first include in your project
GPIOViewer gpio_viewer;

void setup()
{
  Serial.begin(115200);

  // Comment the next line, If your code aleady include connection to Wifi
  gpio_viewer.connectToWifi("Your SSID network", "Your WiFi Password");
  // gpio_viewer.setPort(5555);                 // You can set the http port, if not set default port is 8080

  // Your own setup code start here

  // Must be at the end of your setup
  // gpio_viewer.setSamplingInterval(25); // You can set the sampling interval in ms, if not set default is 100ms
  gpio_viewer.begin();
}

// You don't need to change your loop function
void loop() {

}
// The rest of your code here