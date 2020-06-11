/*
  xdrv_40_telegram.ino - telegram for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef USE_TELEGRAM
/*********************************************************************************************\
 * Telegram bot
 *
 * Supported commands:
 * TGToken <token>    - Add your BotFather created bot token (default none)
 * TGChatId <chat_id> - Add your BotFather created bot chat id (default none)
 * TGPoll <seconds>   - Telegram receive poll time (default 10 seconds)
 * TGState 0          - Disable telegram sending (default)
 * TGState 1          - Enable telegram sending
 * TGState 2          - Disable telegram listener (default)
 * TGState 3          - Enable telegram listener
 * TGState 4          - Disable telegram response echo (default)
 * TGState 5          - Enable telegram response echo
 * TGSend <data>      - If telegram sending is enabled AND a chat id is present then send data
 *
 * Tested with defines
 * #define USE_TELEGRAM                             // Support for Telegram protocol
 * #define USE_TELEGRAM_FINGERPRINT "\xB2\x72\x47\xA6\x69\x8C\x3C\x69\xF9\x58\x6C\xF3\x60\x02\xFB\x83\xFA\x8B\x1F\x23" // Telegram api.telegram.org TLS public key fingerpring
\*********************************************************************************************/

#define XDRV_40                    40

#define TELEGRAM_SEND_RETRY        4    // Retries
#define TELEGRAM_LOOP_WAIT         10   // Seconds

#ifdef USE_MQTT_TLS_CA_CERT
  static const uint32_t tls_rx_size = 2048;   // since Telegram CA is bigger than 1024 bytes, we need to increase rx buffer
  static const uint32_t tls_tx_size = 1024;
#else
  static const uint32_t tls_rx_size = 1024;
  static const uint32_t tls_tx_size = 1024;
#endif

#include "WiFiClientSecureLightBearSSL.h"
BearSSL::WiFiClientSecure_light *telegramClient = nullptr;

static const uint8_t Telegram_Fingerprint[] PROGMEM = USE_TELEGRAM_FINGERPRINT;

struct {
  String message[3][6];  // amount of messages read per time  (update_id, name_id, name, lastname, chat_id, text)
  uint8_t state = 0;
  uint8_t index = 0;
  uint8_t retry = 0;
  uint8_t poll = TELEGRAM_LOOP_WAIT;
  uint8_t wait = 0;
  bool send_enable = false;
  bool recv_enable = false;
  bool echo_enable = false;
  bool recv_busy = false;
} Telegram;

bool TelegramInit(void) {
  bool init_done = false;
  if (strlen(SettingsText(SET_TELEGRAM_TOKEN))) {
    if (!telegramClient) {
      telegramClient = new BearSSL::WiFiClientSecure_light(tls_rx_size, tls_tx_size);
#ifdef USE_MQTT_TLS_CA_CERT
      telegramClient->setTrustAnchor(&GoDaddyCAG2_TA);
#else
      telegramClient->setPubKeyFingerprint(Telegram_Fingerprint, Telegram_Fingerprint, false); // check server fingerprint
#endif

  	  Telegram.message[0][0]="0";   // Number of received messages
	    Telegram.message[1][0]="";
      Telegram.message[0][1]="0";   // Code of last read Message

      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Started"));
    }

    init_done = true;
  }
  return init_done;
}

/**************************************************************************************************
 * function to achieve connection to api.telegram.org and send command to telegram                *
 * (Argument to pass: URL to address to Telegram)                                                 *
 **************************************************************************************************/
String TelegramConnectToTelegram(String command) {
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Cmnd %s"), command.c_str());

  if (!TelegramInit()) { return ""; }

  String response = "";
  uint32_t tls_connect_time = millis();

  if (telegramClient->connect("api.telegram.org", 443)) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Connected in %d ms, max ThunkStack used %d"),
      millis() - tls_connect_time, telegramClient->getMaxThunkStackUse());

    telegramClient->println("GET /"+command);

    String a = "";
    char c;
    int ch_count=0;
    uint32_t now = millis();
    bool avail = false;
    while (millis() -now < 1500) {
      while (telegramClient->available()) {
        char c = telegramClient->read();
        if (ch_count < 700) {
          response = response + c;
          ch_count++;
        }
        avail = true;
      }
      if (avail) {
        break;
      }
    }

    telegramClient->stop();
  }

  return response;
}

/***************************************************************
 * GetUpdates - function to receive all messages from telegram *
 * (Argument to pass: the last+1 message to read)             *
 ***************************************************************/
void TelegramGetUpdates(String offset) {
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: getUpdates"));

  if (!TelegramInit()) { return; }

  String _token = SettingsText(SET_TELEGRAM_TOKEN);
  String command = "bot" + _token + "/getUpdates?offset=" + offset;
  String response = TelegramConnectToTelegram(command);       //recieve reply from telegram.org

  // {"ok":true,"result":[]}
  // or
  // {"ok":true,"result":[
  //  {"update_id":973125394,
  //   "message":{"message_id":25,
  //              "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
  //              "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
  //              "date":1591877503,
  //              "text":"M1"
  //             }
  //  },
  //  {"update_id":973125395,
  //   "message":{"message_id":26,
  //              "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
  //              "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
  //              "date":1591877508,
  //              "text":"M2"
  //             }
  //  }
  // ]}
  // or
  // {"ok":true,"result":[
  //  {"update_id":973125396,
  //   "message":{"message_id":29,
  //              "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
  //              "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
  //              "date":1591879753,
  //              "text":"/power toggle",
  //              "entities":[{"offset":0,"length":6,"type":"bot_command"}]
  //             }
  //  }
  // ]}

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("TLG: Response %s"), response.c_str());

  // parsing of reply from Telegram into separate received messages
  int i = 0;                //messages received counter
  if (response != "") {

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Sent Update request messages up to %s"), offset.c_str());

    String a = "";
    int ch_count = 0;
    String c;
    for (uint32_t n = 1; n < response.length() +1; n++) {   //Search for each message start
      ch_count++;
      c = response.substring(n -1, n);
      a = a + c;
      if (ch_count > 8) {
        if (a.substring(ch_count -9) == "update_id") {
          if (i > 1) { break; }
          Telegram.message[i][0] = a.substring(0, ch_count -11);
          a = a.substring(ch_count-11);
          i++;
          ch_count = 11;
        }
      }
    }
    if (1 == i) {
      Telegram.message[i][0] = a.substring(0, ch_count);   //Assign of parsed message into message matrix if only 1 message)
    }
    if (i > 1) { i = i -1; }
  }
  //check result of parsing process
  if (response == "") {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Failed to update"));
    return;
  }
  if (0 == i) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: No new messages"));
    Telegram.message[0][0] = "0";
  } else {
    Telegram.message[0][0] = String(i);   //returns how many messages are in the array
    for (int b = 1; b < i+1; b++) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Msg %d %s"), b, Telegram.message[b][0].c_str());
    }

    TelegramAnalizeMessage();
  }
}

void TelegramAnalizeMessage(void) {
  for (uint32_t i = 1; i < Telegram.message[0][0].toInt() +1; i++) {
    Telegram.message[i][5] = "";

    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(Telegram.message[i][0]);
    if (root.success()) {
      Telegram.message[i][0] = root["update_id"].as<String>();
      Telegram.message[i][1] = root["message"]["from"]["id"].as<String>();
      Telegram.message[i][2] = root["message"]["from"]["first_name"].as<String>();
      Telegram.message[i][3] = root["message"]["from"]["last_name"].as<String>();
      Telegram.message[i][4] = root["message"]["chat"]["id"].as<String>();
      Telegram.message[i][5] = root["message"]["text"].as<String>();
    }

    int id = Telegram.message[Telegram.message[0][0].toInt()][0].toInt() +1;
    Telegram.message[0][1] = id;  // Write id of last read message

    for (int j = 0; j < 6; j++)	{
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("TLG: Parsed%d \"%s\""), j, Telegram.message[i][j].c_str());
    }
  }
}

bool TelegramSendMessage(String chat_id, String text) {
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: sendMessage"));

  if (!TelegramInit()) { return false; }

  bool sent = false;
  if (text != "") {
    String _token = SettingsText(SET_TELEGRAM_TOKEN);
    String command = "bot" + _token + "/sendMessage?chat_id=" + chat_id + "&text=" + text;
    String response = TelegramConnectToTelegram(command);

    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("TLG: Response %s"), response.c_str());

    if (response.startsWith("{\"ok\":true")) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: Message sent"));
      sent = true;
    }

  }

  return sent;
}

/*
void TelegramSendGetMe(void) {
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TLG: getMe"));

  if (!TelegramInit()) { return; }

  String _token = SettingsText(SET_TELEGRAM_TOKEN);
  String command = "bot" + _token + "/getMe";
  String response = TelegramConnectToTelegram(command);

  // {"ok":true,"result":{"id":1179906608,"is_bot":true,"first_name":"Tasmota","username":"tasmota_bot","can_join_groups":true,"can_read_all_group_messages":false,"supports_inline_queries":false}}

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("TLG: Response %s"), response.c_str());
}
*/

String TelegramExecuteCommand(const char *svalue) {
  String response = "";

  uint32_t curridx = web_log_index;
  ExecuteCommand(svalue, SRC_CHAT);
  if (web_log_index != curridx) {
    uint32_t counter = curridx;
    response = F("{");
    bool cflg = false;
    do {
      char* tmp;
      size_t len;
      GetLog(counter, &tmp, &len);
      if (len) {
        // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
        char* JSON = (char*)memchr(tmp, '{', len);
        if (JSON) { // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
          size_t JSONlen = len - (JSON - tmp);
          if (JSONlen > sizeof(mqtt_data)) { JSONlen = sizeof(mqtt_data); }
          char stemp[JSONlen];
          strlcpy(stemp, JSON +1, JSONlen -2);
          if (cflg) { response += F(","); }
          response += stemp;
          cflg = true;
        }
      }
      counter++;
      counter &= 0xFF;
      if (!counter) counter++;  // Skip 0 as it is not allowed
    } while (counter != web_log_index);
    response += F("}");
  } else {
    response = F("{\"" D_RSLT_WARNING "\":\"" D_ENABLE_WEBLOG_FOR_RESPONSE "\"}");
  }

  return response;
}

void TelegramLoop(void) {
  if (!global_state.wifi_down && (Telegram.recv_enable || Telegram.echo_enable)) {
    switch (Telegram.state) {
      case 0:
        TelegramInit();
        Telegram.state++;
        break;
      case 1:
        TelegramGetUpdates(Telegram.message[0][1]);   // launch API GetUpdates up to xxx message
        Telegram.index = 1;
        Telegram.retry = TELEGRAM_SEND_RETRY;
        Telegram.state++;
        break;
      case 2:
        if (Telegram.echo_enable) {
          if (Telegram.retry && (Telegram.index < Telegram.message[0][0].toInt() + 1)) {
            if (TelegramSendMessage(Telegram.message[Telegram.index][4], Telegram.message[Telegram.index][5])) {
              Telegram.index++;
              Telegram.retry = TELEGRAM_SEND_RETRY;
            } else {
              Telegram.retry--;
            }
          } else {
            Telegram.message[0][0] = "";   // All messages have been replied - reset new messages
            Telegram.wait = Telegram.poll;
            Telegram.state++;
          }
        } else {
          if (Telegram.message[0][0].toInt() && (Telegram.message[Telegram.index][5].length() > 0)) {
            String logging = TelegramExecuteCommand(Telegram.message[Telegram.index][5].c_str());
            if (logging.length() > 0) {
              TelegramSendMessage(Telegram.message[Telegram.index][4], logging);
            }
          }
          Telegram.message[0][0] = "";   // All messages have been replied - reset new messages
          Telegram.wait = Telegram.poll;
          Telegram.state++;
        }
        break;
      case 3:
        if (Telegram.wait) {
          Telegram.wait--;
        } else {
          Telegram.state = 1;
        }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_TGSTATE "State"
#define D_CMND_TGPOLL "Poll"
#define D_CMND_TGSEND "Send"
#define D_CMND_TGTOKEN "Token"
#define D_CMND_TGCHATID "ChatId"

const char kTelegramCommands[] PROGMEM = "TG|"  // Prefix
  D_CMND_TGSTATE "|" D_CMND_TGPOLL "|" D_CMND_TGTOKEN "|" D_CMND_TGCHATID "|" D_CMND_TGSEND;

void (* const TelegramCommand[])(void) PROGMEM = {
  &CmndTgState, &CmndTgPoll, &CmndTgToken, &CmndTgChatId, &CmndTgSend };

void CmndTgState(void) {
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
      switch (XdrvMailbox.payload) {
      case 0: // Off
      case 1: // On
        Telegram.send_enable = XdrvMailbox.payload &1;
        break;
      case 2: // Off
      case 3: // On
        Telegram.recv_enable = XdrvMailbox.payload &1;
        break;
      case 4: // Off
      case 5: // On
        Telegram.echo_enable = XdrvMailbox.payload &1;
        break;
      }
    }
  }
  snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":{\"Send\":\"%s\",\"Receive\":\"%s\",\"Echo\":\"%s\"}}"),
    XdrvMailbox.command, GetStateText(Telegram.send_enable), GetStateText(Telegram.recv_enable), GetStateText(Telegram.echo_enable));
}

void CmndTgPoll(void) {
  if ((XdrvMailbox.payload >= 4) && (XdrvMailbox.payload <= 300)) {
    Telegram.poll = XdrvMailbox.payload;
    if (Telegram.poll < Telegram.wait) {
      Telegram.wait = Telegram.poll;
    }
  }
  ResponseCmndNumber(Telegram.poll);
}

void CmndTgToken(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_TELEGRAM_TOKEN, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_TELEGRAM_TOKEN));
}

void CmndTgChatId(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_TELEGRAM_CHATID, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_TELEGRAM_CHATID));
}

void CmndTgSend(void) {
  if (!Telegram.send_enable || !strlen(SettingsText(SET_TELEGRAM_CHATID))) {
    ResponseCmndChar(D_JSON_FAILED);
    return;
  }
  if (XdrvMailbox.data_len > 0) {
    String message = XdrvMailbox.data;
    String chat_id = SettingsText(SET_TELEGRAM_CHATID);
    if (!TelegramSendMessage(chat_id, message)) {
      ResponseCmndChar(D_JSON_FAILED);
      return;
    }
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv40(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      TelegramLoop();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTelegramCommands, TelegramCommand);
      break;
  }
  return result;
}
#endif  // USE_TELEGRAM
