# EmberZNet NCP UART EZSP firmware

## EmberZNet NCP UART EZSP firmware signed for Sonoff ZBBridge

- `ncp-uart-sw_6.7.6_115200.ota` - original stable version for EZSP v8 compatible hosts, contains a bug that can cause battery of IKEA and Philips remotes to drain
- `ncp-uart-sw-6.7.8_115200.ota` - NEW: recommended stable version for EZSP v8 compatible hosts.

## Archived

- `ncp-uart-sw_6.5.5_115200.ota` - legacy version for EZSP v7 compatible hosts.

## EmberZNet and EZSP Protocol Versions

Silicon Labs do not currently have a consolidated list of changes by EmberZNet SDK or EZSP protocol version. The EZSP additions, changes and deletions have only ever been listed in the [Zigbee EmberZNet Release Notes](https://www.silabs.com/search#q=Zigbee%20EmberZNet%20Release%20Notes&t=All&sort=relevancy) (EmberZNet SDK) under the "New items" section as well as the matching UG100 EZSP Reference Guide included with each EmberZNet SDK release.

The largest change was between EZSP v4 (first added in EmberZNet 4.7.2 SDK) and EZSP v5 that was added in EmberZNet 5.9.0 SDK which requires the Legacy Frame ID 0xFF. The change from EZSP v5 to EZSP v6 was done in EmberZNet 6.0.0 SDK. The change from EZSP v6 to EZSP v7 was in EmberZNet 6.4.0 SDK. EmberZNet 6.7.0 SDK added EZSP v8 (and Secure EZSP Protocol Version 2).

Perhaps more important to know is that EZSP v5, v6 and v7 (EmberZNet 6.6.x.x) use the same framing format, but EmberZNet 6.7.x.x/EZSP v8 introduced new framing format and expanded command id field from 8 bits to 16 bits.

See Silabs [Zigbee & Thread Knowledge Base Articles List](https://silabs-prod.adobecqms.net/community/wireless/zigbee-and-thread/knowledge-base.entry.html/2020/04/01/zigbee_thread_knowledgebasearticleslist-ih5r) for background information.