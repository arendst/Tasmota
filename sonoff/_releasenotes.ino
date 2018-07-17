/*
6.1.1b 20180715
** Change default initial wifi config tool **
Default initial wifi config tool has changed from WPS to Wifi Manager (local Access Point).
Both WPS and Wifi Smartconfig are now optional and are disabled in user_config.h saving a massive 66k of code space.
See _changelog.ino how to enable them again.
Define WIFI_CONFIG_TOOL now contains the default behaviour once a SSID has been configured.
If no SSID is configured making a wifi connection impossible the new define WIFI_CONFIG_NO_SSID will be used.
While define WIFI_CONFIG_NO_SSID is set to WIFI_WPSCONFIG in user_config.h the compiler will check for define USE_WPS and if not enabled
  WIFI_CONFIG_NO_SSID will default to WIFI_MANAGER using the webserver. If define USE_WEBSERVER is also not enabled
  WIFI_CONFIG_NO_SSID will default to WIFI_SMARTCONFIG. If define USE_SMARTCONFIG is also not enabled
  WIFI_CONFIG_NO_SSID will default to a new option WIFI_SERIAL allowing to enter wifi parameters to serial which is always possible.

** Introduction _changelog.ino **
Finally the changes can be found in a changelog where they belong. Release info will now be in _releasenotes.ino
*/