/*
  xdrv_93_esp32_gnss_ntrip.ino - NTRIP Server and Caster
  Copyright (C) 2025 Catalin Sanda

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef ESP32
#ifdef USE_GNSS
#ifdef USE_NTRIP

/*********************************************************************/
/*                          HEADERS & INCLUDES                       */
/*********************************************************************/
#include <algorithm>
#include <vector>
#include <base64.hpp>
#include <AsyncTCP.h>
#ifdef USE_AUX_WEBSERVER
#include <ESPAsyncWebServer.h>
#endif

#define XDRV_93 93
#define D_CMND_NTRIP "NTRIP"
#define MESSAGE_STATISTICS_COUNT 10
#define NTRIP_CLIENTS 2

/*********************************************************************/
/*                           GLOBAL DEFINES                          */
/*********************************************************************/

#define CLIENT_TIMEOUT 300000
#define MAX_STREAMING_CLIENTS 5

/*********************************************************************/
/*                             STRUCTS                               */
/*********************************************************************/

struct __message_statistics_entry
{
  uint16_t message_type;
  uint32_t message_count;
};

struct __ntrip_server_settings
{
  uint16_t port;
  bool enabled;
  char host[33];
  char mountpoint[33];
  char username[33];
  char password[33];
};

struct __ntrip_caster_settings
{
  bool enabled;
  char mountpoint[33];
  char username[33];
  char password[33];
};

struct __ntrip_settings
{
  uint32_t crc32;
  uint32_t version;
  __ntrip_server_settings server_settings[NTRIP_CLIENTS];
  __ntrip_caster_settings caster_settings;
};

/*********************************************************************/
/*                          GLOBAL VARIABLES                         */
/*********************************************************************/

static __message_statistics_entry message_statistics[MESSAGE_STATISTICS_COUNT];
static bool ntrip_settings_initialized = false;

/**
 * @brief Main settings object for NTRIP
 */
static __ntrip_settings NtripSettings;

/**
 * @brief Count of RTCM bytes forwarded by the caster (to local clients)
 */
static uint32_t rtcmBytesCasterForwarded = 0;

/**
 * @brief Count of RTCM bytes forwarded by the server (device -> remote caster(s))
 */
static uint32_t rtcmBytesServerForwarded = 0;

/*********************************************************************/
/*                   CLI FORWARD FUNCTIONS                           */
/*********************************************************************/
void CmndNTRIP(void);
const char kNTRIPCommands[] PROGMEM = "|" D_CMND_NTRIP;
void (*const NTRIPCommand[])(void) PROGMEM = {
    &CmndNTRIP};

/*********************************************************************/
/*                   LOAD/SAVE SETTINGS FUNCTIONS                    */
/*********************************************************************/
void NtripSettingsSave(void)
{
  // Calculate CRC32 of everything except the CRC field
  uint32_t crc32 = GetCfgCrc32((uint8_t *)&NtripSettings + 4, sizeof(NtripSettings) - 4);

  if (crc32 != NtripSettings.crc32)
  {
    NtripSettings.crc32 = crc32;

    char filename[20];
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_93);

    AddLog(LOG_LEVEL_DEBUG, PSTR("NTRIP: Saving settings to file %s"), filename);

#ifdef USE_UFILESYS
    if (!TfsSaveFile(filename, (const uint8_t *)&NtripSettings, sizeof(NtripSettings)))
    {
      AddLog(LOG_LEVEL_INFO, D_ERROR_FILE_NOT_FOUND);
    }
#else
    AddLog(LOG_LEVEL_INFO, D_ERROR_FILESYSTEM_NOT_READY);
#endif // USE_UFILESYS
  }
}

void NtripSettingsLoad(void)
{
  char filename[20];
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_93);

#ifdef USE_UFILESYS
  if (TfsLoadFile(filename, (uint8_t *)&NtripSettings, sizeof(NtripSettings)))
  {
    uint32_t crc32 = GetCfgCrc32((uint8_t *)&NtripSettings + 4, sizeof(NtripSettings) - 4);
    if (crc32 != NtripSettings.crc32)
    {
      // CRC mismatch - initialize with defaults
      memset(&NtripSettings, 0, sizeof(NtripSettings));
      NtripSettings.version = 1;
    }
  }
  else
  {
    // File not found or load error - initialize with defaults
    memset(&NtripSettings, 0, sizeof(NtripSettings));
    NtripSettings.version = 1;
  }
#endif // USE_UFILESYS
}

/*********************************************************************/
/*                     NTRIP MESSAGE STATISTICS                      */
/*********************************************************************/

void InitializeMessageStatistics()
{
  memset(message_statistics, 0, sizeof(message_statistics));
}

void UpdateMessageStatistics(uint16_t msg_type)
{
  // We have a fixed array size for storing stats on known message types
  for (int i = 0; i < MESSAGE_STATISTICS_COUNT - 1; i++)
  {
    if (message_statistics[i].message_type == msg_type)
    {
      message_statistics[i].message_count++;
      return;
    }
    if (message_statistics[i].message_type == 0)
    {
      message_statistics[i].message_type = msg_type;
      message_statistics[i].message_count = 1;
      return;
    }
  }
  // If we didn't find an empty or matching slot, increment the "Other" slot
  message_statistics[MESSAGE_STATISTICS_COUNT - 1].message_count++;
}

char *ext_vsnprintf_malloc_P_wrapper(const char *fmt_P, ...)
{
  va_list args;
  va_start(args, fmt_P);
  char *result = ext_vsnprintf_malloc_P(fmt_P, args);
  va_end(args);
  return result;
}

/*********************************************************************/
/*                          NTRIP CLIENT CLASS                       */
/*        (Device acting as a client to remote NTRIP caster)         */
/*********************************************************************/

class NTRIPClient
{
public:
  static constexpr uint32_t RECONNECT_DELAY = 30000; // 30 seconds

  NTRIPClient()
      : connected(false), enabled(false), connecting(false), retryCount(0), nextRetryTime(0), lastConnectAttempt(0), shouldReconnect(false), port(0)
  {
    client = new AsyncClient();
    setupClientCallbacks();
    memset(host, 0, sizeof(host));
    memset(mountpoint, 0, sizeof(mountpoint));
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
  }

  ~NTRIPClient()
  {
    if (client)
    {
      cleanupClient();
      delete client;
    }
  }

  void begin(const __ntrip_server_settings &settings)
  {
    bool settingsChanged = false;

    settingsChanged |= (enabled != settings.enabled);
    settingsChanged |= (strcmp(host, settings.host) != 0);
    settingsChanged |= (port != settings.port);
    settingsChanged |= (strcmp(mountpoint, settings.mountpoint) != 0);
    settingsChanged |= (strcmp(username, settings.username) != 0);
    settingsChanged |= (strcmp(password, settings.password) != 0);

    enabled = settings.enabled;
    if (!enabled)
    {
      disconnect();
      return;
    }

    strlcpy(host, settings.host, sizeof(host));
    port = settings.port;
    strlcpy(mountpoint, settings.mountpoint, sizeof(mountpoint));
    strlcpy(username, settings.username, sizeof(username));
    strlcpy(password, settings.password, sizeof(password));

    if (settingsChanged)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("NTRIPC: Settings changed, scheduling reconnect"));
      retryCount = 0;
      shouldReconnect = true;
      nextRetryTime = 0;
      disconnect();
    }
  }

  void stop()
  {
    enabled = false;
    disconnect();
  }

  void sendData(const uint8_t *data, size_t length)
  {
    if (!enabled || !connected || !client || !client->connected())
    {
      return;
    }
    client->write(reinterpret_cast<const char *>(data), length);
  }

  bool isConnected() const
  {
    return connected;
  }

  void checkConnection()
  {
    if (!enabled)
    {
      return;
    }
    uint32_t now = millis();

    if (!connected && !connecting && (now >= nextRetryTime) && shouldReconnect)
    {
      initiateConnect();
    }
  }

private:
  AsyncClient *client;
  volatile bool connected;
  bool enabled;
  volatile bool connecting;
  uint32_t retryCount;
  uint32_t nextRetryTime;
  uint32_t lastConnectAttempt;
  volatile bool shouldReconnect;

  char host[33];
  uint16_t port;
  char mountpoint[33];
  char username[33];
  char password[33];

  void disconnect()
  {
    connected = false;
    connecting = false;

    if (client && (client->connected() || client->connecting()))
    {
      client->close(true);
    }
    cleanupClient();
    setupClientCallbacks();
  }

  void cleanupClient()
  {
    if (!client)
      return;
    client->onConnect(nullptr);
    client->onDisconnect(nullptr);
    client->onError(nullptr);
    client->onData(nullptr);
  }

  void setupClientCallbacks()
  {
    if (!client)
      return;

    client->onConnect([this](void *obj, AsyncClient *c)
                      { handleConnect(c); });
    client->onDisconnect([this](void *obj, AsyncClient *c)
                         { handleDisconnect(c); });
    client->onError([this](void *obj, AsyncClient *c, int8_t error)
                    { handleError(c, error); });
    client->onData([this](void *obj, AsyncClient *c, void *data, size_t len)
                   { handleData(c, (uint8_t *)data, len); });

    client->setRxTimeout(3);
    client->setAckTimeout(3000);
  }

  void initiateConnect()
  {
    if (!enabled || connecting || connected)
    {
      return;
    }
    lastConnectAttempt = millis();
    retryCount++;
    connecting = true;
    shouldReconnect = false;

    AddLog(LOG_LEVEL_INFO, PSTR("NTRIPC: Connecting to %s:%d (attempt %d)"), host, port, retryCount);

    if (!client->connect(host, port))
    {
      handleError(client, -1);
      return;
    }
  }

  void handleConnect(AsyncClient *c)
  {
    if (c != client)
      return;

    connecting = false;
    connected = true;
    shouldReconnect = false;
    retryCount = 0;
    nextRetryTime = 0;

    AddLog(LOG_LEVEL_INFO, PSTR("NTRIPC: Connected to %s:%d"), host, port);

    char request[512];
    createNTRIPRequest(request, sizeof(request));
    client->write(request, strlen(request));
  }

  void handleDisconnect(AsyncClient *c)
  {
    if (c != client)
      return;

    connected = false;
    connecting = false;
    shouldReconnect = true;

    AddLog(LOG_LEVEL_INFO, PSTR("NTRIPC: Disconnected from %s:%d"), host, port);

    if (enabled)
    {
      nextRetryTime = millis() + RECONNECT_DELAY;
    }
  }

  void handleError(AsyncClient *c, int8_t error)
  {
    if (c != client)
      return;

    connected = false;
    connecting = false;
    shouldReconnect = true;

    AddLog(LOG_LEVEL_ERROR, PSTR("NTRIPC: Connection error %d"), error);

    if (enabled)
    {
      nextRetryTime = millis() + RECONNECT_DELAY;
      AddLog(LOG_LEVEL_INFO, PSTR("NTRIPC: Will retry in %d seconds"), RECONNECT_DELAY / 1000);
    }
  }

  void handleData(AsyncClient *c, uint8_t *data, size_t len)
  {
    if (c != client || len == 0)
      return;

    char *response = (char *)malloc(len + 1);
    if (!response)
      return;

    memcpy(response, data, len);
    response[len] = '\0';

    if (strstr(response, "200"))
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("NTRIPC: Server accepted connection"));
    }
    else if (strstr(response, "401"))
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("NTRIPC: Authentication failed"));
      disconnect();
      shouldReconnect = true;
      nextRetryTime = millis() + RECONNECT_DELAY;
    }

    free(response);
  }

  void createNTRIPRequest(char *buffer, size_t bufferSize)
  {
    char auth[192] = {0};

    if (strlen(username) > 0)
    {
      char credentials[65] = {0};
      snprintf_P(credentials, sizeof(credentials), PSTR("%s:%s"), username, password);
      size_t credentials_len = strnlen(credentials, sizeof(credentials));

      unsigned int encoded_len = encode_base64_length(credentials_len);
      if (encoded_len > (sizeof(credentials) * 4 / 3 + 2))
      {
        AddLog(LOG_LEVEL_ERROR, PSTR("NTRIPC: Credentials too long"));
        buffer[0] = '\0';
        return;
      }

      char encoded[65 * 4 / 3 + 2] = {0};
      encode_base64((unsigned char *)credentials, credentials_len, (unsigned char *)encoded);

      snprintf_P(auth, sizeof(auth), PSTR("Authorization: Basic %s\r\n"), encoded);
    }

    // Example request for NTRIP server
    snprintf_P(buffer, bufferSize,
               PSTR("POST /%s HTTP/1.1\r\n"
                    "User-Agent: NTRIP Tasmota/1.0\r\n"
                    "Host: %s\r\n"
                    "%s"
                    "Content-Type: application/x-rtcm\r\n"
                    "Ntrip-Version: Ntrip/2.0\r\n"
                    "\r\n"),
               mountpoint,
               host,
               auth);
  }
};

#ifdef USE_AUX_WEBSERVER
/*********************************************************************/
/*                    FORWARD DECLARATIONS (Caster)                  */
/*********************************************************************/
void rtcmInitializeCasterEndpoint(AsyncWebServer *ntrip_web_server);
void handleRTCMRequest(AsyncWebServerRequest *request);
static void handleCasterRequest(AsyncWebServerRequest *request);
#endif

/*********************************************************************/
/*                  NTRIP CLIENTS MANAGEMENT                         */
/*********************************************************************/

static NTRIPClient ntripClients[NTRIP_CLIENTS];

static void initializeNTRIPClients()
{
  for (uint8_t i = 0; i < NTRIP_CLIENTS; i++)
  {
    ntripClients[i].begin(NtripSettings.server_settings[i]);
  }
  AddLog(LOG_LEVEL_INFO, PSTR("NTRIPC: Initialized NTRIP clients"));
}

static void CheckNTRIPClients()
{
  for (uint8_t i = 0; i < NTRIP_CLIENTS; i++)
  {
    ntripClients[i].checkConnection();
  }
}

static void NtripProcessNewSettings()
{
  for (uint8_t i = 0; i < NTRIP_CLIENTS; i++)
  {
    ntripClients[i].begin(NtripSettings.server_settings[i]);
  }

#ifdef USE_AUX_WEBSERVER
  rtcmInitializeCasterEndpoint(nullptr);
#endif
}

/*********************************************************************/
/*                          CASTER LOGIC                             */
/*   (Device acting as a local caster to which remote clients connect)*/
/*********************************************************************/
#ifdef USE_AUX_WEBSERVER // Only include caster code if using the auxiliary webserver

static AsyncWebServer *web_server = nullptr;
static AsyncWebHandler *caster_handler = nullptr;
static char caster_mountpoint[34] = {0};

/**
 * RemoteNTRIPClient: track connected remote clients
 */
struct RemoteNTRIPClient
{
  AsyncClient *client;
  uint32_t lastDataTime;
  bool isActive;
  String mountpoint;
};

static std::vector<RemoteNTRIPClient> remoteNtripClients;
static uint32_t remoteNtripClientsTotal = 0;

/*********************************************************************/
/*               NTRIP SOURCE TABLE & CASTER HTTP HANDLER            */
/*********************************************************************/


// TODO(catalinsanda): based on stats generate the list of NTRIP messages and their frequency
static const char NTRIP_SOURCE_TABLE[] PROGMEM =
    "SOURCETABLE 200 OK\r\n"
    "Server: NTRIP Tasmota32\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "STR;%s;RTCM 3.2;1004(1),1005/1007(5),1074(1),1077(1),1084(1),1087(1),1094(1),1097(1),1124(1),1127(1);2;GPS+GLO+GAL+BDS;ESP32;ESP;0;;none;B;N;0;\r\n"
    "ENDSOURCETABLE\r\n";

char *ext_snprintf_malloc_P_wrapper(const char *fmt_P, ...)
{
  va_list args;
  va_start(args, fmt_P);
  char *result = (char *)ext_vsnprintf_P(nullptr, 0, fmt_P, args);
  va_end(args);
  return result;
}

class NTRIPAsyncStreamResponse : public AsyncWebServerResponse
{
private:
  AsyncClient *_client = nullptr;

public:
  NTRIPAsyncStreamResponse()
  {
    _code = 200;
    _contentLength = 0;
    _contentType = "application/octet-stream";
    _sendContentLength = false;
    _chunked = false;
  }

  ~NTRIPAsyncStreamResponse()
  {
    if (_client)
    {
      handleDisconnect(_client);
    }
  }

  bool _sourceValid() const override
  {
    return true;
  }

  void _respond(AsyncWebServerRequest *request) override
  {
    _client = request->client();
    String header;
    _assembleHead(header, request->version());
    _client->write(header.c_str(), header.length());
    _state = RESPONSE_CONTENT;

    // Add client to the caster's tracking list
    RemoteNTRIPClient newClient = {
        _client,
        millis(),
        true,
        request->url().substring(1) // remove leading '/'
    };
    remoteNtripClients.push_back(newClient);
    remoteNtripClientsTotal = remoteNtripClients.size();
  }

  size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time) override
  {
    return len;
  }

  static void handleDisconnect(AsyncClient *client)
  {
    AddLog(LOG_LEVEL_INFO, PSTR("NTRIP: Client disconnected"));
    for (auto &cli : remoteNtripClients)
    {
      if (cli.client == client)
      {
        cli.isActive = false;
        break;
      }
    }
  }
};

static void handleCasterRequest(AsyncWebServerRequest *request)
{
  if (!NtripSettings.caster_settings.enabled || !caster_handler)
  {
    request->send(503, "text/plain", "NTRIP Caster not enabled");
    return;
  }

  String mountpoint = request->url();
  mountpoint.remove(0, 1); // remove leading slash

  if (mountpoint != NtripSettings.caster_settings.mountpoint)
  {
    // If user just requested "/" -> send source table
    if (mountpoint == "")
    {
      char *sourceTable = ext_snprintf_malloc_P_wrapper(NTRIP_SOURCE_TABLE,
                                                        NtripSettings.caster_settings.mountpoint);
      if (sourceTable)
      {
        request->send(200, "text/plain", sourceTable);
        free(sourceTable);
      }
      else
      {
        request->send(500, "text/plain", "Internal Server Error");
      }
      return;
    }
    request->send(404, "text/plain", "Mountpoint not found");
    return;
  }

  if (strcmp(caster_mountpoint + 1, mountpoint.c_str()) != 0)
  {
    request->send(409, "text/plain", "Mountpoint configuration mismatch");
    return;
  }

  // Check authentication if credentials are configured
  if (strlen(NtripSettings.caster_settings.username) > 0)
  {
    if (!request->authenticate(NtripSettings.caster_settings.username,
                               NtripSettings.caster_settings.password))
    {
      request->requestAuthentication();
      return;
    }
  }

  if (remoteNtripClients.size() >= MAX_STREAMING_CLIENTS)
  {
    request->send(503, "text/plain", "Maximum number of clients reached");
    return;
  }

  AsyncWebServerResponse *response = new NTRIPAsyncStreamResponse();
  if (!response)
  {
    AddLog(LOG_LEVEL_ERROR, PSTR("NTRIP: Failed to create response handler"));
    request->send(500, "text/plain", "Failed to create response handler");
    return;
  }

  if (!request->client())
  {
    delete response;
    AddLog(LOG_LEVEL_ERROR, PSTR("NTRIP: Invalid client connection"));
    request->send(500, "text/plain", "Invalid client connection");
    return;
  }

  request->client()->setNoDelay(true);
  response->addHeader("Content-Type", "application/octet-stream");
  response->addHeader("Connection", "close");

  request->send(response);
  AddLog(LOG_LEVEL_INFO, PSTR("NTRIP: Client connected to Caster from %s:%d"),
         request->client()->remoteIP().toString().c_str(), request->client()->remotePort());
}

/*********************************************************************/
/*                  INITIALIZE CASTER ENDPOINT FUNCTION              */
/*********************************************************************/
void rtcmInitializeCasterEndpoint(AsyncWebServer *ntrip_web_server)
{
  if (ntrip_web_server)
  {
    web_server = ntrip_web_server;
  }
  if (!ntrip_settings_initialized || !web_server)
  {
    return;
  }

  char mountpoint[34];
  snprintf_P(mountpoint, sizeof(mountpoint), PSTR("/%s"), NtripSettings.caster_settings.mountpoint);

  // If caster is disabled, remove any existing handler
  if (!NtripSettings.caster_settings.enabled)
  {
    if (caster_handler)
    {
      web_server->removeHandler(caster_handler);
      caster_handler = nullptr;
      memset(caster_mountpoint, 0, sizeof(caster_mountpoint));
      AddLog(LOG_LEVEL_INFO, PSTR("NTRIP: Removed Caster mountpoint"));
    }
    return;
  }

  // If mountpoint changed
  if (strncmp(caster_mountpoint, mountpoint, std::min(sizeof(caster_mountpoint), sizeof(mountpoint))) != 0)
  {
    // Remove old handler if it exists
    if (caster_handler)
    {
      web_server->removeHandler(caster_handler);
      caster_handler = nullptr;
      memset(caster_mountpoint, 0, sizeof(caster_mountpoint));
      AddLog(LOG_LEVEL_INFO, PSTR("NTRIP: Removed old Caster mountpoint"));

      // Disconnect existing clients
      for (auto &client : remoteNtripClients)
      {
        if (client.client && client.client->connected())
        {
          client.client->close();
        }
      }
      remoteNtripClients.clear();
      remoteNtripClientsTotal = 0;

      // Delay to allow cleanup
      delay(100);
    }

    caster_handler = &web_server->on(mountpoint, HTTP_GET, handleCasterRequest);
    strncpy(caster_mountpoint, mountpoint, sizeof(caster_mountpoint));
    AddLog(LOG_LEVEL_INFO, PSTR("NTRIP: Added new Caster mountpoint at %s"), mountpoint);
  }
  else
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NTRIP: Caster endpoint not changed"));
  }
}

/*********************************************************************/
/*                    BROADCAST RTCM DATA TO CLIENTS                 */
/*********************************************************************/
static void broadcastRTCMData(const uint8_t *buffer, size_t length)
{
  if (length == 0 || remoteNtripClients.empty())
  {
    return;
  }

  uint32_t currentTime = millis();

  // Remove inactive or timed-out clients
  remoteNtripClients.erase(
      std::remove_if(
          remoteNtripClients.begin(), remoteNtripClients.end(),
          [currentTime](const RemoteNTRIPClient &client) -> bool
          {
            return (!client.isActive ||
                    ((currentTime - client.lastDataTime) > CLIENT_TIMEOUT) ||
                    !client.client->connected());
          }),
      remoteNtripClients.end());

  // Broadcast to remaining active clients
  for (auto &client : remoteNtripClients)
  {
    if (client.isActive && client.client->connected() && client.client->canSend())
    {
      client.client->write(reinterpret_cast<const char *>(buffer), length);
      client.lastDataTime = currentTime;
      rtcmBytesCasterForwarded += length;
    }
  }

  remoteNtripClientsTotal = remoteNtripClients.size();
}

#endif // USE_AUX_WEBSERVER

/*********************************************************************/
/*                   PROCESS INCOMING RTCM MESSAGES                  */
/*********************************************************************/
static void ProcessRTCMMessage(const uint8_t *data, size_t length)
{
  // Basic RTCM validity checks
  if (length < 6 || data[0] != 0xD3)
  {
    return;
  }
  uint16_t msg_length = ((data[1] & 0x03) << 8) | data[2];
  if ((msg_length + 6) != length)
  {
    return; // invalid
  }

  // The RTCM message type is in bits [msg_offset..]
  uint16_t msg_type = (data[3] << 4) | ((data[4] & 0xF0) >> 4);

  AddLog(LOG_LEVEL_DEBUG, PSTR("NTRIP: RTCM type %d, length %d"), msg_type, msg_length);
  UpdateMessageStatistics(msg_type);

  // Forward to remote servers (the device is the client)
  for (uint8_t i = 0; i < NTRIP_CLIENTS; i++)
  {
    if (ntripClients[i].isConnected())
    {
      ntripClients[i].sendData(data, length);
      rtcmBytesServerForwarded += length;
    }
  }

// Forward to local caster clients
#ifdef USE_AUX_WEBSERVER
  broadcastRTCMData(data, length);
#endif
}

/*********************************************************************/
/*           TASMOTA WEBSERVER HOOKS (INTERFACE/CONFIG UI)           */
/*********************************************************************/
#ifdef USE_WEBSERVER

/*********************************************************************/
/*         WEBPAGE STRINGS AND FORMS FOR NTRIP CONFIGURATION         */
/*********************************************************************/
static const char HTTP_BTN_MENU_NTRIP[] PROGMEM =
    "<p><form action='ntrip' method='get'><button>" D_CONFIGURE_NTRIP "</button></form></p>";

static const char HTTP_NTRIP_SCRIPT[] PROGMEM =
    "<script>"
    "function toggleNtripFields(checkbox, fields) {"
    "  fields.forEach(field => {"
    "    const element = document.getElementById(field);"
    "    if (element) {"
    "      element.disabled = !checkbox.checked;"
    "      if (!checkbox.checked) {"
    "        element.value = '';"
    "      }"
    "    }"
    "  });"
    "}"
    "</script>";

static const char HTTP_FORM_NTRIP[] PROGMEM =
    "<fieldset style='min-width:530px;background:#444;color:white;padding:20px;'>"
    "<legend style='text-align:left;'><b>&nbsp;" D_NTRIP_PARAMETERS "&nbsp;</b></legend>"
    "<style>"
    ".ntrip-input:disabled {"
    "  background-color: #222 !important;"
    "  border-color: #444 !important;"
    "  color: #666 !important;"
    "  cursor: not-allowed;"
    "  opacity: 0.7;"
    "}"
    ".ntrip-input:not(:disabled):hover {"
    "  border-color: #777;"
    "  background-color: #3a3a3a;"
    "}"
    "</style>"
    "<form method='post' action='ntrip'>"
    "%s"
    "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
    "</form>"
    "</fieldset>";

static const char HTTP_FORM_NTRIP_SERVER[] PROGMEM =
    "<fieldset style='margin:10px 0;padding:15px;border:1px solid #666;'>"
    "<legend style='text-align:left; display:flex; align-items:center; gap:8px;'>"
    "<span style='color:white;'>%s</span>" // Title
    "<input type='checkbox' id='ntrip_server_enable_%d' name='ntrip_server_enable_%d' "
    "onchange='toggleNtripFields(this, [\"ntrip_serv_host_%d\",\"ntrip_serv_port_%d\",\"ntrip_serv_mount_%d\",\"ntrip_serv_user_%d\",\"ntrip_serv_pass_%d\"])' "
    "%s " // checked if enabled
    "style='margin:0;'/>"
    "</legend>"
    "<div style='display:grid; grid-template-columns:100px 1fr 100px 1fr 100px 1fr; gap:10px; align-items:center; margin-bottom:10px;'>"
    "<label for='ntrip_serv_host_%d' style='color:white;'>" D_NTRIP_HOST "</label>"
    "<input type='text' id='ntrip_serv_host_%d' name='ntrip_serv_host_%d' value='%s' %s maxlength='32' "
    "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
    "<label for='ntrip_serv_port_%d' style='color:white;'>" D_NTRIP_PORT "</label>"
    "<input type='number' id='ntrip_serv_port_%d' name='ntrip_serv_port_%d' value='%d' %s min='1' max='65535' placeholder='2101' "
    "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
    "<label for='ntrip_serv_mount_%d' style='color:white;'>" D_NTRIP_MOUNT "</label>"
    "<input type='text' id='ntrip_serv_mount_%d' name='ntrip_serv_mount_%d' value='%s' %s maxlength='32' "
    "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
    "</div>"
    "<div style='display:grid; grid-template-columns:100px 1fr 100px 1fr; gap:10px; align-items:center;'>"
    "<label for='ntrip_serv_user_%d' style='color:white;'>" D_NTRIP_USERNAME "</label>"
    "<input type='text' id='ntrip_serv_user_%d' name='ntrip_serv_user_%d' value='%s' %s maxlength='32' "
    "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
    "<label for='ntrip_serv_pass_%d' style='color:white;'>" D_NTRIP_PASSWORD "</label>"
    "<input type='text' id='ntrip_serv_pass_%d' name='ntrip_serv_pass_%d' value='%s' %s maxlength='32' "
    "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
    "</div>"
    "</fieldset>";

#ifdef USE_AUX_WEBSERVER
static const char HTTP_FORM_NTRIP_CASTER[] PROGMEM =
    "<fieldset style='margin:10px 0;padding:15px;border:1px solid #666;'>"
    "<legend style='text-align:left; display:flex; align-items:center; gap:8px;'>"
    "<span style='color:white;'>" D_NTRIP_CASTER "</span>"
    "<input type='checkbox' id='ntrip_caster_enable' name='ntrip_caster_enable' "
    "onchange='toggleNtripFields(this, [\"ntrip_cast_mount\",\"ntrip_cast_user\",\"ntrip_cast_pass\"])' "
    "%s "
    "style='margin:0;'/>"
    "</legend>"
    "<div style='display:grid; grid-template-columns:100px 1fr 100px 1fr 100px 1fr; gap:10px; align-items:center; margin-bottom:10px;'>"
    "<label for='ntrip_cast_port' style='color:white;'>" D_NTRIP_PORT "</label>"
    "<input type='number' id='ntrip_cast_port' value='" STR(AUX_WEBSERVER_PORT) "' disabled "
                                                                                "style='width:100%%; background:#333; color:#666; border:1px solid #555; padding:5px; cursor:not-allowed;' "
                                                                                "title='Port is fixed to the auxiliary webserver port (" STR(AUX_WEBSERVER_PORT) ") set at compile time'/>"
                                                                                                                                                                 "<label for='ntrip_cast_mount' style='color:white;'>" D_NTRIP_MOUNT "</label>"
                                                                                                                                                                 "<input type='text' id='ntrip_cast_mount' name='ntrip_cast_mount' value='%s' %s maxlength='32' "
                                                                                                                                                                 "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
                                                                                                                                                                 "</div>"
                                                                                                                                                                 "<div style='display:grid; grid-template-columns:100px 1fr 100px 1fr; gap:10px; align-items:center;'>"
                                                                                                                                                                 "<label for='ntrip_cast_user' style='color:white;'>" D_NTRIP_USERNAME "</label>"
                                                                                                                                                                 "<input type='text' id='ntrip_cast_user' name='ntrip_cast_user' value='%s' %s maxlength='32' "
                                                                                                                                                                 "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
                                                                                                                                                                 "<label for='ntrip_cast_pass' style='color:white;'>" D_NTRIP_PASSWORD "</label>"
                                                                                                                                                                 "<input type='text' id='ntrip_cast_pass' name='ntrip_cast_pass' value='%s' %s maxlength='32' "
                                                                                                                                                                 "style='width:100%%; background:#333; color:white; border:1px solid #555; padding:5px; transition:all 0.3s ease;' class='ntrip-input'/>"
                                                                                                                                                                 "</div>"
                                                                                                                                                                 "<br>"
                                                                                                                                                                 "</fieldset>";
#endif // USE_AUX_WEBSERVER

/*********************************************************************/
/*    INTERNAL HELPERS FOR NTRIP WEB CONFIG (SAVING FORM VALUES)     */
/*********************************************************************/
static void Ntrip_Save_Settings(void)
{
  String stmp;
  bool needs_save = false;
  const uint8_t NUM_SERVERS = 2;

  if (!Webserver)
  {
    return;
  }

  // NTRIP servers
  for (uint8_t server_idx = 0; server_idx < NUM_SERVERS; server_idx++)
  {
    char field_name[32];
    uint8_t server_num = server_idx + 1;

    snprintf(field_name, sizeof(field_name), "ntrip_server_enable_%d", server_num);
    if (Webserver->hasArg(field_name))
    {
      bool old_val = NtripSettings.server_settings[server_idx].enabled;
      if (!old_val)
      {
        needs_save = true;
      }
      NtripSettings.server_settings[server_idx].enabled = true;

      // host
      snprintf(field_name, sizeof(field_name), "ntrip_serv_host_%d", server_num);
      if (Webserver->hasArg(field_name))
      {
        stmp = Webserver->arg(field_name);
        if (strcmp(NtripSettings.server_settings[server_idx].host, stmp.c_str()) != 0)
        {
          needs_save = true;
        }
        strlcpy(NtripSettings.server_settings[server_idx].host, stmp.c_str(),
                sizeof(NtripSettings.server_settings[server_idx].host));
      }
      // port
      snprintf(field_name, sizeof(field_name), "ntrip_serv_port_%d", server_num);
      if (Webserver->hasArg(field_name))
      {
        stmp = Webserver->arg(field_name);
        uint16_t port = stmp.toInt();
        if (NtripSettings.server_settings[server_idx].port != port)
        {
          needs_save = true;
        }
        NtripSettings.server_settings[server_idx].port = port;
      }
      // mount
      snprintf(field_name, sizeof(field_name), "ntrip_serv_mount_%d", server_num);
      if (Webserver->hasArg(field_name))
      {
        stmp = Webserver->arg(field_name);
        if (strcmp(NtripSettings.server_settings[server_idx].mountpoint, stmp.c_str()) != 0)
        {
          needs_save = true;
        }
        strlcpy(NtripSettings.server_settings[server_idx].mountpoint, stmp.c_str(),
                sizeof(NtripSettings.server_settings[server_idx].mountpoint));
      }
      // user
      snprintf(field_name, sizeof(field_name), "ntrip_serv_user_%d", server_num);
      if (Webserver->hasArg(field_name))
      {
        stmp = Webserver->arg(field_name);
        if (strcmp(NtripSettings.server_settings[server_idx].username, stmp.c_str()) != 0)
        {
          needs_save = true;
        }
        strlcpy(NtripSettings.server_settings[server_idx].username, stmp.c_str(),
                sizeof(NtripSettings.server_settings[server_idx].username));
      }
      // pass
      snprintf(field_name, sizeof(field_name), "ntrip_serv_pass_%d", server_num);
      if (Webserver->hasArg(field_name))
      {
        stmp = Webserver->arg(field_name);
        if (strcmp(NtripSettings.server_settings[server_idx].password, stmp.c_str()) != 0)
        {
          needs_save = true;
        }
        strlcpy(NtripSettings.server_settings[server_idx].password, stmp.c_str(),
                sizeof(NtripSettings.server_settings[server_idx].password));
      }
    }
    else
    {
      if (NtripSettings.server_settings[server_idx].enabled)
      {
        needs_save = true;
      }
      NtripSettings.server_settings[server_idx].enabled = false;
    }
  }

#ifdef USE_AUX_WEBSERVER
  // Caster settings
  if (Webserver->hasArg("ntrip_caster_enable"))
  {
    bool old_val = NtripSettings.caster_settings.enabled;
    if (!old_val)
    {
      needs_save = true;
    }
    NtripSettings.caster_settings.enabled = true;

    if (Webserver->hasArg("ntrip_cast_mount"))
    {
      stmp = Webserver->arg("ntrip_cast_mount");
      if (strcmp(NtripSettings.caster_settings.mountpoint, stmp.c_str()) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.caster_settings.mountpoint, stmp.c_str(),
              sizeof(NtripSettings.caster_settings.mountpoint));
    }
    if (Webserver->hasArg("ntrip_cast_user"))
    {
      stmp = Webserver->arg("ntrip_cast_user");
      if (strcmp(NtripSettings.caster_settings.username, stmp.c_str()) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.caster_settings.username, stmp.c_str(),
              sizeof(NtripSettings.caster_settings.username));
    }
    if (Webserver->hasArg("ntrip_cast_pass"))
    {
      stmp = Webserver->arg("ntrip_cast_pass");
      if (strcmp(NtripSettings.caster_settings.password, stmp.c_str()) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.caster_settings.password, stmp.c_str(),
              sizeof(NtripSettings.caster_settings.password));
    }
  }
  else
  {
    if (NtripSettings.caster_settings.enabled)
    {
      needs_save = true;
    }
    NtripSettings.caster_settings.enabled = false;
  }
#else
  if (NtripSettings.caster_settings.enabled)
  {
    needs_save = true;
  }
  NtripSettings.caster_settings.enabled = false;
#endif // USE_AUX_WEBSERVER

  if (needs_save)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NTRIP: Settings changed, saving"));
    NtripSettingsSave();
    NtripProcessNewSettings();
  }
}

/*********************************************************************/
/*            TASMOTA WEB HANDLER FOR NTRIP CONFIG PAGE              */
/*********************************************************************/
static void HandleNtripConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess())
  {
    return;
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_NTRIP));

  if (Webserver->hasArg("save"))
  {
    Ntrip_Save_Settings();
    HandleConfiguration();
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_NTRIP));
  WSContentSendStyle();

  const uint8_t NUM_SERVERS = 2;
  const char *disabled_attr = "disabled";
  const uint16_t DEFAULT_NTRIP_PORT = 2101;

  // Generate the server fields
  char *servers_html = nullptr;
  for (uint8_t server_idx = 0; server_idx < NUM_SERVERS; server_idx++)
  {
    uint8_t server_num = server_idx + 1;
    const __ntrip_server_settings *server = &NtripSettings.server_settings[server_idx];

    char *server_html = ext_vsnprintf_malloc_P_wrapper(
        HTTP_FORM_NTRIP_SERVER,
        // Title
        (server_idx == 0 ? D_NTRIP_SERVER_1 : D_NTRIP_SERVER_2),
        server_num, server_num, // for id / name
        server_num, server_num, server_num, server_num, server_num,
        (server->enabled ? "checked" : ""),
        // host
        server_num, server_num, server_num,
        server->host,
        server->enabled ? "" : disabled_attr,
        // port
        server_num, server_num, server_num,
        server->port ? server->port : DEFAULT_NTRIP_PORT,
        server->enabled ? "" : disabled_attr,
        // mount
        server_num, server_num, server_num,
        server->mountpoint,
        server->enabled ? "" : disabled_attr,
        // user
        server_num, server_num, server_num,
        server->username,
        server->enabled ? "" : disabled_attr,
        // pass
        server_num, server_num, server_num,
        server->password,
        server->enabled ? "" : disabled_attr);

    if (!server_html)
    {
      if (servers_html)
      {
        free(servers_html);
      }
      AddLog(LOG_LEVEL_ERROR, PSTR("Failed to allocate memory for server HTML"));
      return;
    }

    if (servers_html)
    {
      char *new_html = ext_vsnprintf_malloc_P_wrapper(PSTR("%s%s"), servers_html, server_html);
      free(servers_html);
      free(server_html);
      if (!new_html)
      {
        AddLog(LOG_LEVEL_ERROR, PSTR("Failed to combine server HTML"));
        return;
      }
      servers_html = new_html;
    }
    else
    {
      servers_html = server_html;
    }
  }

  char *caster_html = nullptr;
#ifdef USE_AUX_WEBSERVER
  caster_html = ext_vsnprintf_malloc_P_wrapper(
      HTTP_FORM_NTRIP_CASTER,
      (NtripSettings.caster_settings.enabled ? "checked" : ""),
      NtripSettings.caster_settings.mountpoint,
      (NtripSettings.caster_settings.enabled ? "" : disabled_attr),
      NtripSettings.caster_settings.username,
      (NtripSettings.caster_settings.enabled ? "" : disabled_attr),
      NtripSettings.caster_settings.password,
      (NtripSettings.caster_settings.enabled ? "" : disabled_attr));
#else
  caster_html = ext_vsnprintf_malloc_P_wrapper(PSTR(""));
#endif

  if (!caster_html)
  {
    if (servers_html)
      free(servers_html);
    AddLog(LOG_LEVEL_ERROR, PSTR("Failed to allocate memory for caster HTML"));
    return;
  }

  char *all_content = ext_vsnprintf_malloc_P_wrapper(PSTR("%s%s"), servers_html ? servers_html : "", caster_html);
  if (!all_content)
  {
    if (servers_html)
      free(servers_html);
    free(caster_html);
    AddLog(LOG_LEVEL_ERROR, PSTR("Failed to allocate memory for all content"));
    return;
  }

  WSContentSend(HTTP_NTRIP_SCRIPT, strlen_P(HTTP_NTRIP_SCRIPT));
  WSContentSend_P(HTTP_FORM_NTRIP, all_content);

  if (servers_html)
    free(servers_html);
  free(caster_html);
  free(all_content);

  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

#endif // USE_WEBSERVER

/*********************************************************************/
/*               CLI COMMAND HANDLING (CmndNTRIP)                    */
/*********************************************************************/
void CmndNTRIP(void)
{
  char *data = XdrvMailbox.data;
  char *command = strtok(data, " ,");

  // If no arguments, print status
  if (!command)
  {
    Response_P(
        PSTR("{\"NTRIP\":{\"Caster\":{\"enabled\":%d,\"port\":%d,\"mount\":\"%s\",\"user\":\"%s\"},"),
#ifdef USE_AUX_WEBSERVER
        NtripSettings.caster_settings.enabled,
        AUX_WEBSERVER_PORT,
        NtripSettings.caster_settings.mountpoint,
        NtripSettings.caster_settings.username
#else
        0, 0, "", ""
#endif
    );

    ResponseAppend_P(
        PSTR("\"Server1\":{\"enabled\":%d,\"host\":\"%s\",\"port\":%d,\"mount\":\"%s\",\"user\":\"%s\"},"),
        NtripSettings.server_settings[0].enabled,
        NtripSettings.server_settings[0].host,
        NtripSettings.server_settings[0].port,
        NtripSettings.server_settings[0].mountpoint,
        NtripSettings.server_settings[0].username);

    ResponseAppend_P(
        PSTR("\"Server2\":{\"enabled\":%d,\"host\":\"%s\",\"port\":%d,\"mount\":\"%s\",\"user\":\"%s\"}}}"),
        NtripSettings.server_settings[1].enabled,
        NtripSettings.server_settings[1].host,
        NtripSettings.server_settings[1].port,
        NtripSettings.server_settings[1].mountpoint,
        NtripSettings.server_settings[1].username);
    return;
  }

  bool needs_save = false;

  // NTRIP <caster|server1|server2> ...
  if (strcasecmp(command, "caster") == 0)
  {
#ifdef USE_AUX_WEBSERVER
    // Format: NTRIP caster enable,mountpoint,username,password
    char *enable = strtok(nullptr, " ,");
    char *mount = strtok(nullptr, " ,");
    char *user = strtok(nullptr, " ,");
    char *pass = strtok(nullptr, " ,");

    if (!enable)
    {
      // Show current caster status
      Response_P(
          PSTR("{\"NTRIPCaster\":{\"enabled\":%d,\"port\":%d,\"mount\":\"%s\",\"user\":\"%s\"}}"),
          NtripSettings.caster_settings.enabled,
          AUX_WEBSERVER_PORT,
          NtripSettings.caster_settings.mountpoint,
          NtripSettings.caster_settings.username);
      return;
    }

    bool en = (atoi(enable) > 0);
    if (NtripSettings.caster_settings.enabled != en)
    {
      needs_save = true;
    }
    NtripSettings.caster_settings.enabled = en;

    if (mount)
    {
      if (strcmp(NtripSettings.caster_settings.mountpoint, mount) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.caster_settings.mountpoint, mount, sizeof(NtripSettings.caster_settings.mountpoint));
    }
    if (user)
    {
      if (strcmp(NtripSettings.caster_settings.username, user) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.caster_settings.username, user, sizeof(NtripSettings.caster_settings.username));
    }
    if (pass)
    {
      if (strcmp(NtripSettings.caster_settings.password, pass) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.caster_settings.password, pass, sizeof(NtripSettings.caster_settings.password));
    }
#else
    // If no AUX webserver, ignore caster commands or respond with an error
    Response_P(PSTR("{\"error\":\"Caster functionality not enabled\"}"));
    return;
#endif // USE_AUX_WEBSERVER
  }
  else if ((strcasecmp(command, "server1") == 0) || (strcasecmp(command, "server2") == 0))
  {
    // Format: NTRIP serverX enable,host,port,mountpoint,username,password
    uint8_t index = (command[6] - '1');
    if (index >= NTRIP_CLIENTS)
    {
      ResponseCmndError();
      return;
    }

    char *enable = strtok(nullptr, " ,");
    char *host = strtok(nullptr, " ,");
    char *port = strtok(nullptr, " ,");
    char *mount = strtok(nullptr, " ,");
    char *user = strtok(nullptr, " ,");
    char *pass = strtok(nullptr, " ,");

    if (!enable)
    {
      Response_P(
          PSTR("{\"NTRIPServer%d\":{\"enabled\":%d,\"host\":\"%s\",\"port\":%d,\"mount\":\"%s\",\"user\":\"%s\"}}"),
          index + 1,
          NtripSettings.server_settings[index].enabled,
          NtripSettings.server_settings[index].host,
          NtripSettings.server_settings[index].port,
          NtripSettings.server_settings[index].mountpoint,
          NtripSettings.server_settings[index].username);
      return;
    }

    bool en = (atoi(enable) > 0);
    if (NtripSettings.server_settings[index].enabled != en)
    {
      needs_save = true;
    }
    NtripSettings.server_settings[index].enabled = en;

    if (host)
    {
      if (strcmp(NtripSettings.server_settings[index].host, host) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.server_settings[index].host, host, sizeof(NtripSettings.server_settings[index].host));
    }
    if (port)
    {
      uint16_t new_port = atoi(port);
      if (NtripSettings.server_settings[index].port != new_port)
      {
        needs_save = true;
      }
      NtripSettings.server_settings[index].port = new_port;
    }
    if (mount)
    {
      if (strcmp(NtripSettings.server_settings[index].mountpoint, mount) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.server_settings[index].mountpoint, mount, sizeof(NtripSettings.server_settings[index].mountpoint));
    }
    if (user)
    {
      if (strcmp(NtripSettings.server_settings[index].username, user) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.server_settings[index].username, user, sizeof(NtripSettings.server_settings[index].username));
    }
    if (pass)
    {
      if (strcmp(NtripSettings.server_settings[index].password, pass) != 0)
      {
        needs_save = true;
      }
      strlcpy(NtripSettings.server_settings[index].password, pass, sizeof(NtripSettings.server_settings[index].password));
    }
  }
  else
  {
    ResponseCmndError();
    return;
  }

  if (needs_save)
  {
    NtripSettingsSave();
    NtripProcessNewSettings();
  }
  ResponseCmndDone();
}

/*********************************************************************/
/*                    JSON AND WEBSENSOR DISPLAYS                    */
/*********************************************************************/
static void RTCMShowJSON()
{
  ResponseAppend_P(PSTR(",\"RTCM\":{\"Messages\":["));
  bool first = true;
  for (int i = 0; i < MESSAGE_STATISTICS_COUNT - 1; i++)
  {
    if (message_statistics[i].message_type > 0)
    {
      if (!first)
      {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("{\"type\":%d,\"count\":%d}"),
                       message_statistics[i].message_type,
                       message_statistics[i].message_count);
      first = false;
    }
  }
  ResponseAppend_P(PSTR("],\"Other\":%d"), message_statistics[MESSAGE_STATISTICS_COUNT - 1].message_count);

#ifdef USE_AUX_WEBSERVER
  ResponseAppend_P(PSTR(",\"Caster\":{\"BytesForwarded\":%d,\"Clients\":%d}"),
                   rtcmBytesCasterForwarded,
                   remoteNtripClientsTotal);
#else
  ResponseAppend_P(PSTR(",\"Caster\":{\"BytesForwarded\":0,\"Clients\":0}"));
#endif

  ResponseAppend_P(PSTR(",\"Server\":{\"BytesForwarded\":%d"), rtcmBytesServerForwarded);
  ResponseAppend_P(PSTR(",\"Status\":["));
  for (uint8_t i = 0; i < NTRIP_CLIENTS; i++)
  {
    if (i > 0)
    {
      ResponseAppend_P(PSTR(","));
    }
    ResponseAppend_P(PSTR("{\"enabled\":%d,\"connected\":%d}"),
                     NtripSettings.server_settings[i].enabled,
                     ntripClients[i].isConnected());
  }
  ResponseAppend_P(PSTR("]}}}")); // close "Status", "Server", "RTCM"
}

#ifdef USE_WEBSERVER
static void RTCMShowWebSensor()
{
  if (message_statistics[0].message_type > 0)
  {
    WSContentSend_PD(PSTR("{s}RTCM Messages{m}{e}"));
    for (int i = 0; i < MESSAGE_STATISTICS_COUNT - 1; i++)
    {
      if (message_statistics[i].message_type > 0)
      {
        WSContentSend_PD(PSTR("{s}&nbsp;&nbsp;• Type %d{m}%d{e}"),
                         message_statistics[i].message_type,
                         message_statistics[i].message_count);
      }
    }
    if (message_statistics[MESSAGE_STATISTICS_COUNT - 1].message_count > 0)
    {
      WSContentSend_PD(PSTR("{s}&nbsp;&nbsp;• Other Types{m}%d{e}"),
                       message_statistics[MESSAGE_STATISTICS_COUNT - 1].message_count);
    }

    // Show NTRIP Caster stats
#ifdef USE_AUX_WEBSERVER
    WSContentSend_PD(PSTR("{s}NTRIP Caster{m}{e}"));
    WSContentSend_PD(PSTR("{s}&nbsp;&nbsp;• Connected Clients{m}%d{e}"), remoteNtripClientsTotal);
    WSContentSend_PD(PSTR("{s}&nbsp;&nbsp;• Bytes Forwarded{m}%d{e}"), rtcmBytesCasterForwarded);
#endif

    // Show NTRIP Server stats
    WSContentSend_PD(PSTR("{s}NTRIP Server{m}{e}"));
    WSContentSend_PD(PSTR("{s}&nbsp;&nbsp;• Bytes Forwarded{m}%d{e}"), rtcmBytesServerForwarded);

    for (uint8_t i = 0; i < NTRIP_CLIENTS; i++)
    {
      if (NtripSettings.server_settings[i].enabled)
      {
        WSContentSend_PD(
            PSTR("{s}&nbsp;&nbsp;• Server %d Status{m}%s{e}"),
            i + 1,
            ntripClients[i].isConnected() ? PSTR("Connected") : PSTR("Disconnected"));
      }
    }
  }
}
#endif // USE_WEBSERVER

/*********************************************************************/
/*                             Xdrv93()                              */
/*********************************************************************/
/**
 * Main entry point for Tasmota driver #93
 */
bool Xdrv93(uint32_t function)
{
  bool result = false;

  switch (function)
  {
  case FUNC_INIT:
    NtripSettingsLoad();
    InitializeMessageStatistics();
    ntrip_settings_initialized = true;
    initializeNTRIPClients();
#ifdef USE_AUX_WEBSERVER
    rtcmInitializeCasterEndpoint(nullptr);
#endif
    result = true;
    break;

  case FUNC_COMMAND:
    result = DecodeCommand(kNTRIPCommands, NTRIPCommand);
    break;

  case FUNC_EVERY_250_MSECOND:
    CheckNTRIPClients();
    result = true;
    break;

  case FUNC_JSON_APPEND:
    RTCMShowJSON();
    result = true;
    break;

  case FUNC_SAVE_SETTINGS:
    NtripSettingsSave();
    result = true;
    break;

#ifdef USE_WEBSERVER
  case FUNC_WEB_SENSOR:
    RTCMShowWebSensor();
    result = true;
    break;

  case FUNC_WEB_ADD_BUTTON:
    WSContentSend_P(HTTP_BTN_MENU_NTRIP);
    result = true;
    break;

  case FUNC_WEB_ADD_HANDLER:
    WebServer_on(PSTR("/ntrip"), HandleNtripConfiguration);
    result = true;
    break;
#endif // USE_WEBSERVER
  }

  return result;
}

#endif // USE_NTRIP
#endif // USE_GNSS
#endif // ESP32
