/*
  xdrv_40_telegram.ino - telegram for Tasmota

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

#ifdef USE_TELEGRAM
/*********************************************************************************************\
 * Telegram bot
 *
 * Supported commands:
 * TmToken <token>    - Add your BotFather created bot token (default none)
 * TmChatId <chat_id> - Add your BotFather created bot chat id (default none)
 * TmPoll <seconds>   - Telegram receive poll time (default 10 seconds)
 * TmState 0          - Disable telegram sending (default)
 * TmState 1          - Enable telegram sending (needs valid TmChatId)
 * TmState 2          - Disable telegram listener (default)
 * TmState 3          - Enable telegram listener
 * TmState 4          - Disable telegram response echo (default)
 * TmState 5          - Enable telegram response echo
 * TmSend <data>      - If telegram sending is enabled AND a chat id is present then send data
 *
 * Tested with defines
 * #define USE_TELEGRAM                             // Support for Telegram protocol
 * #define USE_TELEGRAM_FINGERPRINT "\xB2\x72\x47\xA6\x69\x8C\x3C\x69\xF9\x58\x6C\xF3\x60\x02\xFB\x83\xFA\x8B\x1F\x23" // Telegram api.telegram.org TLS public key fingerpring
\*********************************************************************************************/

#define XDRV_40                    40

#ifndef TELEGRAM_LOOP_WAIT
#define TELEGRAM_LOOP_WAIT         10   // Seconds
#endif

#define TELEGRAM_SEND_RETRY        4    // Retries
#define TELEGRAM_MAX_MESSAGES      2

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

typedef struct {
  String text;
//  String from_first_name;
//  String from_last_name;
//  uint32_t from_id = 0;
  uint32_t update_id = 0;
  int32_t chat_id = 0;
} TelegramMessage;

struct {
  TelegramMessage message[TELEGRAM_MAX_MESSAGES];
  uint32_t next_update_id = 0;
  uint8_t message_count = 0;   // Amount of messages read per time
  uint8_t state = 0;
  uint8_t index = 0;
  uint8_t retry = 0;
  uint8_t poll = TELEGRAM_LOOP_WAIT;
  uint8_t wait = 0;
  bool send_enable = false;
  bool recv_enable = false;
  bool echo_enable = false;
  bool recv_busy = false;
  bool skip = true;           // Skip first telegram if restarted
} Telegram;

bool TelegramInit(void) {
  bool init_done = false;
  if (strlen(SettingsText(SET_TELEGRAM_TOKEN))) {
    if (!telegramClient) {
      telegramClient = new BearSSL::WiFiClientSecure_light(tls_rx_size, tls_tx_size);
#ifdef USE_MQTT_TLS_CA_CERT
      telegramClient->setTrustAnchor(&GoDaddyCAG2_TA, 1);
#else
      telegramClient->setPubKeyFingerprint(Telegram_Fingerprint, Telegram_Fingerprint, false); // check server fingerprint
#endif
      Telegram.message_count = 0;     // Number of received messages
      Telegram.next_update_id = 0;    // Code of last read Message
      Telegram.message[0].text = "";

      AddLog_P(LOG_LEVEL_INFO, PSTR("TGM: Started"));
    }
    init_done = true;
  }
  return init_done;
}

String TelegramConnectToTelegram(String command) {
//  AddLog_P(LOG_LEVEL_DEBUG, PSTR("TGM: Cmnd %s"), command.c_str());

  if (!TelegramInit()) { return ""; }

  String response = "";
  uint32_t tls_connect_time = millis();
  if (telegramClient->connect("api.telegram.org", 443)) {

//    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TGM: Connected in %d ms, max ThunkStack used %d"), millis() - tls_connect_time, telegramClient->getMaxThunkStackUse());

    telegramClient->println("GET /"+command);

    char c;
    int ch_count=0;
    uint32_t now = millis();
    bool avail = false;
    while (millis() -now < 1500) {
      while (telegramClient->available()) {
        char c = telegramClient->read();
        if (ch_count < 700) {  // Allow up to two messages
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

void TelegramGetUpdates(uint32_t offset) {
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: getUpdates"));

  if (!TelegramInit()) { return; }

  String _token = SettingsText(SET_TELEGRAM_TOKEN);
  String command = "bot" + _token + "/getUpdates?offset=" + String(offset);
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
  // or
  // {"ok":true,"result":[
  //  {"update_id":14354460,
  //   "message":{"message_id":164,
  //              "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
  //              "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
  //              "date":1602428727,
  //              "text":"Status 1"
  //             }
  //  },
  //  {"update_id":14354461,
  //   "message":{"message_id":165,
  //              "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
  //              "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
  //              "date":1602428731,
  //              "text":"Status 2"
  //             }
  //  }
  // ]}

  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Response %s"), response.c_str());

  JsonParser parser((char*)response.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) {

//    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TGM: Sent Update request messages up to %s"), offset.c_str());

    JsonParserArray arr = root[PSTR("result")];
    uint32_t max_updates = arr.size();
//    if (max_updates > TELEGRAM_MAX_MESSAGES) { max_updates = TELEGRAM_MAX_MESSAGES; }
    if (max_updates > 1) { max_updates = 1; }  // Cannot handle more than one for now
    Telegram.message_count = 0;                // Returns how many messages are in the array
    if (max_updates) {
      for (uint32_t i = 0; i < max_updates; i++) {
        Telegram.message[i].text = "";           // Reset command
        JsonParserObject result = arr[i].getObject();
        if (result) {
          // {"update_id":14354450,
          //  "message":{"message_id":141,
          //             "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
          //             "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
          //             "date":1602346120,
          //             "text":"Status 1"}}
          Telegram.message_count++;   // Returns how many messages are in the array
          Telegram.message[i].update_id = result["update_id"].getUInt();
//          Telegram.message[i].from_id = result["message"].getObject()["from"].getObject()["id"].getUInt();
//          Telegram.message[i].from_first_name = result["message"].getObject()["from"].getObject()["first_name"].getStr();
//          Telegram.message[i].from_last_name = result["message"].getObject()["from"].getObject()["last_name"].getStr();
          Telegram.message[i].chat_id = result["message"].getObject()["chat"].getObject()["id"].getUInt();
          Telegram.message[i].text = result["message"].getObject()["text"].getStr();
        }
        Telegram.next_update_id = Telegram.message[i].update_id +1;  // Write id of last read message

        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Parsed update_id %d, chat_id %d, text \"%s\""), Telegram.message[i].update_id, Telegram.message[i].chat_id, Telegram.message[i].text.c_str());
      }
    } else {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TGM: No new messages"));
    }
  } else {
//    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TGM: Failed to update"));
  }
}

bool TelegramSendMessage(int32_t chat_id, String text) {
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: sendMessage"));

  if (!TelegramInit()) { return false; }

  bool sent = false;
  if (text != "") {
    String _token = SettingsText(SET_TELEGRAM_TOKEN);
    String command = "bot" + _token + "/sendMessage?chat_id=" + String(chat_id) + "&text=" + text;
    String response = TelegramConnectToTelegram(command);

//    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Response %s"), response.c_str());

    if (response.startsWith("{\"ok\":true")) {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TGM: Message sent"));
      sent = true;
    }
  }

  return sent;
}

/*
void TelegramSendGetMe(void) {
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: getMe"));

  if (!TelegramInit()) { return; }

  String _token = SettingsText(SET_TELEGRAM_TOKEN);
  String command = "bot" + _token + "/getMe";
  String response = TelegramConnectToTelegram(command);

  // {"ok":true,"result":{"id":1179906608,"is_bot":true,"first_name":"Tasmota","username":"tasmota_bot","can_join_groups":true,"can_read_all_group_messages":false,"supports_inline_queries":false}}

//  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Response %s"), response.c_str());
}
*/

String TelegramExecuteCommand(const char *svalue) {
  String response = "";

  uint32_t curridx = TasmotaGlobal.log_buffer_pointer;
  TasmotaGlobal.templog_level = LOG_LEVEL_INFO;
  ExecuteCommand(svalue, SRC_CHAT);
  response = F("{");
  bool cflg = false;
  uint32_t index = curridx;
  char* line;
  size_t len;
  while (GetLog(TasmotaGlobal.templog_level, &index, &line, &len)) {
    // [14:49:36.123 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
    char* JSON = (char*)memchr(line, '{', len);
    if (JSON) {  // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
      size_t JSONlen = len - (JSON - line);
      if (JSONlen > sizeof(TasmotaGlobal.mqtt_data)) { JSONlen = sizeof(TasmotaGlobal.mqtt_data); }
      char stemp[JSONlen];
      strlcpy(stemp, JSON +1, JSONlen -2);
      if (cflg) { response += F(","); }
      response += stemp;
      cflg = true;
    }
  }
  response += F("}");
  TasmotaGlobal.templog_level = 0;

  return response;
}

void TelegramLoop(void) {
  if (!TasmotaGlobal.global_state.network_down && (Telegram.recv_enable || Telegram.echo_enable)) {
    switch (Telegram.state) {
      case 0:
        TelegramInit();
        Telegram.state++;
        break;
      case 1:
        TelegramGetUpdates(Telegram.next_update_id);   // Launch API GetUpdates up to xxx message
        Telegram.index = 0;
        Telegram.retry = TELEGRAM_SEND_RETRY;
        Telegram.state++;
        break;
      case 2:
        if (Telegram.echo_enable) {
          if (Telegram.retry && (Telegram.index < Telegram.message_count)) {
            if (TelegramSendMessage(Telegram.message[Telegram.index].chat_id, Telegram.message[Telegram.index].text)) {
              Telegram.index++;
              Telegram.retry = TELEGRAM_SEND_RETRY;
            } else {
              Telegram.retry--;
            }
          } else {
            Telegram.message_count = 0;   // All messages have been replied - reset new messages
            Telegram.wait = Telegram.poll;
            Telegram.state++;
          }
        } else {
          if (Telegram.skip) {  // Skip first update after restart as it may be a restart (again)
            Telegram.skip = false;
          } else {
            if (Telegram.message_count && (Telegram.message[Telegram.index].text.length() > 0)) {
              String logging = TelegramExecuteCommand(Telegram.message[Telegram.index].text.c_str());
              if (logging.length() > 0) {
                TelegramSendMessage(Telegram.message[Telegram.index].chat_id, logging);
              }
            }
          }
          Telegram.message_count = 0;   // All messages have been replied - reset new messages
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

#define D_CMND_TMSTATE "State"
#define D_CMND_TMPOLL "Poll"
#define D_CMND_TMSEND "Send"
#define D_CMND_TMTOKEN "Token"
#define D_CMND_TMCHATID "ChatId"

const char kTelegramCommands[] PROGMEM = "Tm|"  // Prefix
  D_CMND_TMSTATE "|" D_CMND_TMPOLL "|" D_CMND_TMTOKEN "|" D_CMND_TMCHATID "|" D_CMND_TMSEND;

void (* const TelegramCommand[])(void) PROGMEM = {
  &CmndTmState, &CmndTmPoll, &CmndTmToken, &CmndTmChatId, &CmndTmSend };

void CmndTmState(void) {
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
  snprintf_P (TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"%s\":{\"Send\":\"%s\",\"Receive\":\"%s\",\"Echo\":\"%s\"}}"),
    XdrvMailbox.command, GetStateText(Telegram.send_enable), GetStateText(Telegram.recv_enable), GetStateText(Telegram.echo_enable));
}

void CmndTmPoll(void) {
  if ((XdrvMailbox.payload >= 4) && (XdrvMailbox.payload <= 300)) {
    Telegram.poll = XdrvMailbox.payload;
    if (Telegram.poll < Telegram.wait) {
      Telegram.wait = Telegram.poll;
    }
  }
  ResponseCmndNumber(Telegram.poll);
}

void CmndTmToken(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_TELEGRAM_TOKEN, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_TELEGRAM_TOKEN));
}

void CmndTmChatId(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_TELEGRAM_CHATID, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_TELEGRAM_CHATID));
}

void CmndTmSend(void) {
  if (!Telegram.send_enable || !strlen(SettingsText(SET_TELEGRAM_CHATID))) {
    ResponseCmndChar(D_JSON_FAILED);
    return;
  }
  if (XdrvMailbox.data_len > 0) {
    String message = XdrvMailbox.data;
    String chat_id = SettingsText(SET_TELEGRAM_CHATID);
    if (!TelegramSendMessage(chat_id.toInt(), message)) {
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
