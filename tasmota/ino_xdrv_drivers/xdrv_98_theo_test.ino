/*
  xdrv_98_theo_test.ino - Tests for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//#define USE_THEO_TEST

#ifdef USE_THEO_TEST
/*********************************************************************************************\
 * Test support
\*********************************************************************************************/
#warning **** THEO_TEST is enabled ****

#define XDRV_98             98

const char kTheoTestCommands[] PROGMEM = "Theo|"  // Prefix
  "Test";

void (* const TheoTestCommand[])(void) PROGMEM = {
  &CmndTheoTest };

void CmndTheoTest(void) {
  EspDumpPartitionTable();
  ResponseCmndDone();
}

/*
void CmndTheoTest(void) {
  system_show_malloc();
  ResponseCmndDone();
}
*/
/*
void CmndTheoTest(void) {
  if (XdrvMailbox.payload < 2) {XdrvMailbox.payload = 2;}
  Sspm->error_led_blinks = XdrvMailbox.payload;
  ResponseCmndDone();
}
*/
/*
void CmndTheoTest(void) {
  // Test changeUIntScale(x, params[0], params[1], params[2], params[3]) range
  uint32_t params[4];
  uint32_t paramcount = ParseParameters(4, params);

  String result = String(params[0]);
  String source = String(params[2]);
  uint32_t last_value = params[2];
  for (uint32_t i = params[0]; i < params[1]; i++) {
    uint32_t value = changeUIntScale(i, params[0], params[1], params[2], params[3]);
    if (value != last_value) {
      result += ",";
      result += i;

      source += ",";
      source += value;
    }
    last_value = value;
  }
  Response_P(PSTR("{\"RangeSteps\":[%s],\"SourceSteps\":[%s]}"), result.c_str(), source.c_str());
}
*/
/*
void CmndTheoTest(void) {
  Response_P(PSTR("{\"DNS1\":\"%_I\",\"DNS2\":\"%_I\"}"), (uint32_t)WiFi.dnsIP(), (uint32_t)WiFi.dnsIP(1));
}
*/

/*
void CmndTheoTest(void) {
  String status = "{\"Status\":{\"Module\":18,\"DeviceName\":\"Wemos10\",\"FriendlyName\":[\"Wemos10\"],\"Topic\":\"demo\",\"ButtonTopic\":\"0\",\"Power\":0,\"PowerOnState\":3,\"LedState\":1,\"LedMask\":\"FFFF\",\"SaveData\":1,\"SaveState\":1,\"SwitchTopic\":\"0\",\"SwitchMode\":[0,0,0,0,0,0,0,0],\"ButtonRetain\":0,\"SwitchRetain\":0,\"SensorRetain\":0,\"PowerRetain\":0,\"InfoRetain\":0,\"StateRetain\":0},\"StatusPRM\":{\"Baudrate\":115200,\"SerialConfig\":\"8N1\",\"GroupTopic\":\"tasmotas\",\"OtaUrl\":\"http://domus1/api/arduino/tasmota.bin.gz\",\"RestartReason\":\"Software/System restart\",\"Uptime\":\"0T00:02:10\",\"StartupUTC\":\"2021-05-24T14:13:38\",\"Sleep\":50,\"CfgHolder\":4617,\"BootCount\":305,\"BCResetTime\":\"2021-01-24T15:29:49\",\"SaveCount\":651,\"SaveAddress\":\"F8000\"},\"StatusFWR\":{\"Version\":\"9.4.0.4(theo)\",\"BuildDateTime\":\"2021-05-24T16:07:30\",\"Boot\":31,\"Core\":\"2_7_4_9\",\"SDK\":\"2.2.2-dev(38a443e)\",\"CpuFrequency\":80,\"Hardware\":\"ESP8266EX\",\"CR\":\"310/699\"},\"StatusLOG\":{\"SerialLog\":3,\"WebLog\":2,\"MqttLog\":0,\"SysLog\":0,\"LogHost\":\"domus1\",\"LogPort\":514,\"SSId\":[\"indebuurt_IoT\",\"\"],\"TelePeriod\":300,\"Resolution\":\"958B90C0\",\"SetOption\":[\"00228009\",\"2805C8000100060000005A0A000000000000\",\"00004081\",\"00006008\",\"00000000\"]},\"StatusMEM\":{\"ProgramSize\":532,\"Free\":468,\"Heap\":24,\"ProgramFlashSize\":1024,\"FlashSize\":4096,\"FlashChipId\":\"1625C2\",\"FlashFrequency\":40,\"FlashMode\":3,\"Features\":[\"00000809\",\"0F108587\",\"84240001\",\"04041ECE\",\"010003C0\",\"00006081\",\"00004000\",\"00001000\",\"00000024\"],\"Drivers\":\"1,2,3,4,6,7,9,10,12,22,24,45,99\",\"Sensors\":\"2,3,5,6,8,9,10,15,31\"},\"StatusNET\":{\"Hostname\":\"demo\",\"IPAddress\":\"192.168.2.153\",\"Gateway\":\"192.168.2.254\",\"Subnetmask\":\"255.255.255.0\",\"DNSServer\":\"192.168.2.27\",\"Mac\":\"B4:E6:2D:18:00:F6\",\"Webserver\":2,\"WifiConfig\":4,\"WifiPower\":17.0},\"StatusMQT\":{\"MqttHost\":\"domus1\",\"MqttPort\":1883,\"MqttClientMask\":\"DVES_%06X\",\"MqttClient\":\"DVES_1800F6\",\"MqttUser\":\"DVES_USER\",\"MqttCount\":1,\"MAX_PACKET_SIZE\":2048,\"KEEPALIVE\":30,\"SOCKET_TIMEOUT\":4},\"StatusTIM\":{\"UTC\":\"2021-05-24T14:15:48\",\"Local\":\"2021-05-24T16:15:48\",\"StartDST\":\"2021-03-28T02:00:00\",\"EndDST\":\"2021-10-31T03:00:00\",\"Timezone\":99,\"Sunrise\":\"05:19\",\"Sunset\":\"21:37\"},\"StatusSNS\":{\"Time\":\"2021-05-24T16:15:48\",\"DS18B20\":{\"Id\":\"01144A0CB2AA\",\"Temperature\":23.69},\"HTU21\":{\"Temperature\":24.50,\"Humidity\":41.8,\"DewPoint\":10.67},\"TempUnit\":\"C\"},\"StatusSTS\":{\"Time\":\"2021-05-24T16:15:48\",\"Uptime\":\"0T00:02:10\",\"UptimeSec\":130,\"Heap\":24,\"SleepMode\":\"Dynamic\",\"Sleep\":50,\"LoadAvg\":26,\"MqttCount\":1,\"POWER\":\"OFF\",\"Wifi\":{\"AP\":1,\"SSId\":\"indebuurt_IoT\",\"BSSId\":\"18:E8:29:CA:17:C1\",\"Channel\":11,\"RSSI\":100,\"Signal\":-38,\"LinkCount\":1,\"Downtime\":\"0T00:00:03\"}}}";

  MqttPublishPayloadPrefixTopic_P(STAT, XdrvMailbox.command, status.c_str());

  ResponseCmndDone();
}
*/

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv98(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kTheoTestCommands, TheoTestCommand);
      break;
  }
  return result;
}

#endif  // USE_THEO_TEST