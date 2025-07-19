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
 * TmFingerprint 1    - Use default fingerprint 4E 7F F5 6D 1E 29 40 58 AB 84 DE 63 69 7B CD DF 44 2E D2 F6 as defined by USE_TELEGRAM_FINGERPRINT
 * TmFingerprint <fp> - Set fingerprint
 * TmToken <token>    - Add your BotFather created bot token (default none)
 * TmChatId <chat_id> - Add your BotFather created bot chat id (default none)
 * TmPoll <seconds>   - Telegram receive poll time (default 10 seconds)
 * TmState 0          - Disable telegram sending (default)
 * TmState 1          - Enable telegram sending (needs valid TmChatId)
 * TmState 2          - Disable telegram listener (default)
 * TmState 3          - Enable telegram listener
 * TmState 4          - Disable telegram response echo (default)
 * TmState 5          - Enable telegram response echo
 * TmState 6          - Enable telegram auto fingerprint fix (default)
 * TmState 7          - Disable telegram auto fingerprint fix but present new fingerprint to user
 * TmSend <data>      - If telegram sending is enabled AND a chat id is present then send data
 *
 * Tested with defines
 * #define USE_TELEGRAM                             // Support for Telegram protocol
 * #define USE_TELEGRAM_FINGERPRINT "\x4E\x7F\xF5\x6D\x1E\x29\x40\x58\xAB\x84\xDE\x63\x69\x7B\xCD\xDF\x44\x2E\xD2\xF6" // Telegram api.telegram.org TLS public key fingerpring
\*********************************************************************************************/

#define XDRV_40                     40

#ifndef TELEGRAM_LOOP_WAIT
#define TELEGRAM_LOOP_WAIT          10    // Seconds
#endif

#define TELEGRAM_SEND_RETRY         4     // Retries
#define TELEGRAM_MAX_MESSAGES       2

static const uint32_t tls_rx_size = 512;  // Max allowed packet size sent from server (Telegram)
static const uint32_t tls_tx_size = 512;

#include "WiFiClientSecureLightBearSSL.h"
BearSSL::WiFiClientSecure_light *telegramClient = nullptr;

static const uint8_t Telegram_Fingerprint[] PROGMEM = USE_TELEGRAM_FINGERPRINT;

typedef struct {
  String text;
  String chat_id;
//  String from_first_name;
//  String from_last_name;
//  uint32_t from_id = 0;
  uint32_t update_id = 0;
} TelegramMessage;

typedef struct {
  TelegramMessage message[TELEGRAM_MAX_MESSAGES];
  uint8_t fingerprint[20];
  uint32_t next_update_id = 0;
  uint8_t message_count = 0;   // Amount of messages read per time
  uint8_t state = 0;
  uint8_t index = 0;
  uint8_t retry = 0;
  uint8_t poll = TELEGRAM_LOOP_WAIT;
  uint8_t wait = 0;
  bool recv_busy = false;
  bool skip = true;           // Skip first telegram if restarted
} Telegram_t;
Telegram_t* Telegram = nullptr;

void TelegramInit(void) {
  Telegram = (Telegram_t*)calloc(sizeof(Telegram_t), 1);    // Need calloc to reset registers to 0/false
}

bool TelegramStart(void) {
  if (!strlen(SettingsText(SET_TELEGRAM_TOKEN))) { return false; }
  if (telegramClient) { return true; }

  telegramClient = new BearSSL::WiFiClientSecure_light(tls_rx_size, tls_tx_size);
  HexToBytes(SettingsText(SET_TELEGRAM_FINGERPRINT), Telegram->fingerprint, sizeof(Telegram->fingerprint));
  telegramClient->setPubKeyFingerprint(Telegram->fingerprint, Telegram->fingerprint, false); // check server fingerprint

  Telegram->message_count = 0;     // Number of received messages
  Telegram->next_update_id = 0;    // Code of last read Message
  Telegram->message[0].text = "";

  AddLog(LOG_LEVEL_INFO, PSTR("TGM: (Re)started"));
  return true;
}

void TelegramStop(void) {
  if (telegramClient) {
    telegramClient->stop();
    delete telegramClient;
    telegramClient = nullptr;
    AddLog(LOG_LEVEL_INFO, PSTR("TGM: Stopped"));
  }
}

String TelegramConnectToTelegram(const String &command) {
//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Cmnd '%s'"), command.c_str());

  if (!TelegramStart()) { return ""; }

  String response = "";
  String host = F("api.telegram.org");
  uint32_t tls_connect_time = millis();
  if (telegramClient->connect(host.c_str(), 443)) {

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Connected in %d ms, max ThunkStack used %d"), millis() - tls_connect_time, telegramClient->getMaxThunkStackUse());
    
//    telegramClient->println("GET /"+command);  // Fails after 20210621
    String request = "GET /" + command + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    telegramClient->print(request);
/*
    Response before 20210621:
    {"ok":true,"result":[]}

    Response after 20210621:
    HTTP/1.1 200 OK
    Server: nginx/1.18.0
    Date: Thu, 24 Jun 2021 15:26:20 GMT
    Content-Type: application/json
    Content-Length: 23
    Connection: close
    Strict-Transport-Security: max-age=31536000; includeSubDomains; preload
    Access-Control-Allow-Origin: *
    Access-Control-Allow-Methods: GET, POST, OPTIONS
    Access-Control-Expose-Headers: Content-Length,Content-Type,Date,Server,Connection

    {"ok":true,"result":[]}
*/
    char c;
    bool available = false;
    uint32_t now = millis();
    while (!available && (millis() -now < 1500)) {
      while (telegramClient->available()) {
        c = telegramClient->read();
        if (c == '{') {
          available = true;               // Skip headers (+-400 bytes) and start response at first JSON
        }
        if (available) {
          response += c;
          if (response.length() > 2000) {  // Allow up to 5 messages
            break;
          }
        }
      }
    }

    telegramClient->stop();
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TGM: TLS connection error %d"), telegramClient->getLastError());

    const uint8_t *recv_fingerprint = telegramClient->getRecvPubKeyFingerprint();
    char buf_fingerprint[64];
    if (!Settings->sbflag1.telegram_disable_af) {  // CMND_TMSTATE 6/7 - Disable Telegram auto-fingerprint fix
      // Replace current fingerprint with the fingerprint received
      ToHex_P(recv_fingerprint, 20, buf_fingerprint, sizeof(buf_fingerprint));
      SettingsUpdateText(SET_TELEGRAM_FINGERPRINT, buf_fingerprint);
      TelegramStop();
    } else {
      // Create a printable version of the fingerprint received
      ToHex_P(recv_fingerprint, 20, buf_fingerprint, sizeof(buf_fingerprint), ' ');
      AddLog(LOG_LEVEL_DEBUG, PSTR("TGM: Telegram fingerprint %s"), buf_fingerprint);
    }
  }

  return response;
}

void TelegramGetUpdates(uint32_t offset) {
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: getUpdates"));

  if (!TelegramStart()) { return; }

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

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Response '%s'"), response.c_str());

  JsonParser parser((char*)response.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) {

//    AddLog(LOG_LEVEL_DEBUG, PSTR("TGM: Sent Update request messages up to %s"), offset.c_str());

    JsonParserArray arr = root[PSTR("result")];
    uint32_t max_updates = arr.size();
//    if (max_updates > TELEGRAM_MAX_MESSAGES) { max_updates = TELEGRAM_MAX_MESSAGES; }
    if (max_updates > 1) { max_updates = 1; }  // Cannot handle more than one for now
    Telegram->message_count = 0;                // Returns how many messages are in the array
    if (max_updates) {
      for (uint32_t i = 0; i < max_updates; i++) {
        Telegram->message[i].text = "";           // Reset command
        JsonParserObject result = arr[i].getObject();
        if (result) {
          // {"update_id":14354450,
          //  "message":{"message_id":141,
          //             "from":{"id":139920293,"is_bot":false,"first_name":"Theo","last_name":"Arends","username":"tjatja","language_code":"nl"},
          //             "chat":{"id":139920293,"first_name":"Theo","last_name":"Arends","username":"tjatja","type":"private"},
          //             "date":1602346120,
          //             "text":"Status 1"}}
          Telegram->message_count++;   // Returns how many messages are in the array
          Telegram->message[i].update_id = result["update_id"].getUInt();
//          Telegram->.message[i].from_id = result["message"].getObject()["from"].getObject()["id"].getUInt();
//          Telegram->message[i].from_first_name = result["message"].getObject()["from"].getObject()["first_name"].getStr();
//          Telegram->message[i].from_last_name = result["message"].getObject()["from"].getObject()["last_name"].getStr();
          Telegram->message[i].chat_id = result["message"].getObject()["chat"].getObject()["id"].getStr();
          Telegram->message[i].text = result["message"].getObject()["text"].getStr();
        }
        Telegram->next_update_id = Telegram->message[i].update_id +1;  // Write id of last read message

        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Parsed update_id %d, chat_id %s, text \"%s\""), Telegram->message[i].update_id, Telegram->message[i].chat_id.c_str(), Telegram->message[i].text.c_str());
      }
    } else {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("TGM: No new messages"));
    }
  } else {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("TGM: Failed to update"));
  }
}

bool TelegramSendMessage(const String &chat_id, const String &text) {
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: sendMessage"));

  if (!TelegramStart()) { return false; }

  bool sent = false;
  if (text != "") {
    String _token = SettingsText(SET_TELEGRAM_TOKEN);
    String command = "bot" + _token + "/sendMessage?chat_id=" + chat_id + "&text=" + UrlEncode(text);
    String response = TelegramConnectToTelegram(command);

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Response %s"), response.c_str());

    if (response.startsWith("{\"ok\":true")) {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("TGM: Message sent"));
      sent = true;
    }
  }

  return sent;
}

/*
void TelegramSendGetMe(void) {
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: getMe"));

  if (!TelegramStart()) { return; }

  String _token = SettingsText(SET_TELEGRAM_TOKEN);
  String command = "bot" + _token + "/getMe";
  String response = TelegramConnectToTelegram(command);

  // {"ok":true,"result":{"id":1179906608,"is_bot":true,"first_name":"Tasmota","username":"tasmota_bot","can_join_groups":true,"can_read_all_group_messages":false,"supports_inline_queries":false}}

//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TGM: Response %s"), response.c_str());
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
      if (cflg) { response += F(","); }  // Add a comma

//      size_t JSONlen = len - (JSON - line);
//      response.concat(JSON +1, JSONlen -3);  // Add terminating '\0' - Not supported on ESP32
      len -= 2;                          // Skip last '}'
      char save_log_char = line[len];
      line[len] = '\0';                  // Add terminating \'0'
      response.concat(JSON +1);          // Skip first '{'
      line[len] = save_log_char;

      cflg = true;
    }
  }
  response += F("}");
  TasmotaGlobal.templog_level = 0;

  return response;
}

void TelegramLoop(void) {
  if (!TasmotaGlobal.global_state.network_down && (Settings->sbflag1.telegram_recv_enable || Settings->sbflag1.telegram_echo_enable)) {
    switch (Telegram->state) {
      case 0:
        TelegramStart();
        Telegram->state++;
        break;
      case 1:
        TelegramGetUpdates(Telegram->next_update_id);   // Launch API GetUpdates up to xxx message
        Telegram->index = 0;
        Telegram->retry = TELEGRAM_SEND_RETRY;
        Telegram->state++;
        break;
      case 2:
        if (Settings->sbflag1.telegram_echo_enable) {
          if (Telegram->retry && (Telegram->index < Telegram->message_count)) {
            if (TelegramSendMessage(Telegram->message[Telegram->index].chat_id, Telegram->message[Telegram->index].text)) {
              Telegram->index++;
              Telegram->retry = TELEGRAM_SEND_RETRY;
            } else {
              Telegram->retry--;
            }
          } else {
            Telegram->message_count = 0;   // All messages have been replied - reset new messages
            Telegram->wait = Telegram->poll;
            Telegram->state++;
          }
        } else {
          if (Telegram->skip) {  // Skip first update after restart as it may be a restart (again)
            Telegram->skip = false;
          } else {
            if (Telegram->message_count && (Telegram->message[Telegram->index].text.length() > 0)) {
              String logging = TelegramExecuteCommand(Telegram->message[Telegram->index].text.c_str());
              if (logging.length() > 0) {
                TelegramSendMessage(Telegram->message[Telegram->index].chat_id, logging);
              }
            }
          }
          Telegram->message_count = 0;   // All messages have been replied - reset new messages
          Telegram->wait = Telegram->poll;
          Telegram->state++;
        }
        break;
      case 3:
        if (Telegram->wait) {
          Telegram->wait--;
        } else {
          Telegram->state = 1;
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
#define D_CMND_TMFINGERPRINT "Fingerprint"

const char kTelegramCommands[] PROGMEM = "Tm|"  // Prefix
  D_CMND_TMSTATE "|" D_CMND_TMPOLL "|" D_CMND_TMTOKEN "|" D_CMND_TMCHATID "|" D_CMND_TMSEND "|" D_CMND_TMFINGERPRINT;

void (* const TelegramCommand[])(void) PROGMEM = {
  &CmndTmState, &CmndTmPoll, &CmndTmToken, &CmndTmChatId, &CmndTmSend, &CmndTmFingerprint };

void CmndTmFingerprint(void) {
  // TmFingerprint 1
  // TmFingerprint 4E 7F F5 6D 1E 29 40 58 AB 84 DE 63 69 7B CD DF 44 2E D2 F6
  char fingerprint[60];
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(fingerprint))) {
    if (SC_DEFAULT == Shortcut()) {
      memcpy_P(Telegram->fingerprint, Telegram_Fingerprint, sizeof(Telegram->fingerprint)); 
    } else {
      strlcpy(fingerprint, (SC_CLEAR == Shortcut()) ? "" : XdrvMailbox.data, sizeof(fingerprint));
      char *p = fingerprint;
      for (uint32_t i = 0; i < 20; i++) {
        Telegram->fingerprint[i] = strtol(p, &p, 16);
      }
    }
    ToHex_P(Telegram->fingerprint, 20, fingerprint, sizeof(fingerprint));
    SettingsUpdateText(SET_TELEGRAM_FINGERPRINT, fingerprint);
    TasmotaGlobal.restart_flag = 2;
  }
  HexToBytes(SettingsText(SET_TELEGRAM_FINGERPRINT), Telegram->fingerprint, sizeof(Telegram->fingerprint));
  ResponseCmndChar(ToHex_P(Telegram->fingerprint, 20, fingerprint, sizeof(fingerprint), ' '));
}

void CmndTmState(void) {
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 7)) {
      switch (XdrvMailbox.payload) {
      case 0: // Off
      case 1: // On
        Settings->sbflag1.telegram_send_enable = XdrvMailbox.payload &1;
        break;
      case 2: // Off
      case 3: // On
        Settings->sbflag1.telegram_recv_enable = XdrvMailbox.payload &1;
        break;
      case 4: // Off
      case 5: // On
        Settings->sbflag1.telegram_echo_enable = XdrvMailbox.payload &1;
        break;
      case 6: // Off
      case 7: // On
        Settings->sbflag1.telegram_disable_af = XdrvMailbox.payload &1;
        break;
      }
    }
  }
  if (!Settings->sbflag1.telegram_send_enable &&
      !Settings->sbflag1.telegram_recv_enable &&
      !Settings->sbflag1.telegram_echo_enable) {
    TelegramStop();
  }
  Response_P(PSTR("{\"%s\":{\"Send\":\"%s\",\"Receive\":\"%s\",\"Echo\":\"%s\",\"NoAutoFingerprint\":\"%s\"}}"),
    XdrvMailbox.command,
    GetStateText(Settings->sbflag1.telegram_send_enable),
    GetStateText(Settings->sbflag1.telegram_recv_enable),
    GetStateText(Settings->sbflag1.telegram_echo_enable),
    GetStateText(Settings->sbflag1.telegram_disable_af));
}

void CmndTmPoll(void) {
  if ((XdrvMailbox.payload >= 4) && (XdrvMailbox.payload <= 300)) {
    Telegram->poll = XdrvMailbox.payload;
    if (Telegram->poll < Telegram->wait) {
      Telegram->wait = Telegram->poll;
    }
  }
  ResponseCmndNumber(Telegram->poll);
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
  if (!Settings->sbflag1.telegram_send_enable || !strlen(SettingsText(SET_TELEGRAM_CHATID))) {
    ResponseCmndFailed();
    return;
  }
  if (XdrvMailbox.data_len > 0) {
    String message = XdrvMailbox.data;
    String chat_id = SettingsText(SET_TELEGRAM_CHATID);
    if (!TelegramSendMessage(chat_id, message)) {
      ResponseCmndFailed();
      return;
    }
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv40(uint32_t function)
{
  bool result = false;

  if (FUNC_INIT == function) {
    TelegramInit();
  }
  else if (Telegram) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        TelegramLoop();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTelegramCommands, TelegramCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}
#endif  // USE_TELEGRAM
