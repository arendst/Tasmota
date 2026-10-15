
// inspired by https://github.com/MoritzLerch/tesla-pv-display
#ifndef Powerwall_h
#define Powerwall_h


#define PW_RETRIES 2

#define PWL_LOGLVL LOG_LEVEL_DEBUG

// include libraries from email client
// standard ssl does not work at all
ESP_SSLClient ssl_client;
WiFiClientImpl basic_client;

class Powerwall {
   private:
    String powerwall_ip;
    String tesla_email;
    String tesla_password;
    String authCookie;
    String cts1;
    String cts2;
    
   public:
    Powerwall();
    String getAuthCookie();
    String GetRequest(String url, String authCookie);
    String GetRequest(String url);
    String AuthCookie();
    String Pwl_test(String);
};


#ifndef POWERWALL_IP_CONFIG
    #define POWERWALL_IP_CONFIG "192.168.188.60"
#endif

#ifndef TESLA_EMAIL
    #define TESLA_EMAIL "email"
#endif

#ifndef TESLA_PASSWORD
    #define TESLA_PASSWORD "password"
#endif

#ifndef TESLA_POWERWALL_CTS1
    #define TESLA_POWERWALL_CTS1 "cts1"
#endif

#ifndef TESLA_POWERWALL_CTS2
    #define TESLA_POWERWALL_CTS2 "cts2"
#endif

Powerwall::Powerwall() {
    powerwall_ip   = POWERWALL_IP_CONFIG;
    tesla_email    = TESLA_EMAIL;
    tesla_password = TESLA_PASSWORD;
    authCookie     = "";
    cts1 = TESLA_POWERWALL_CTS1;
    cts2 = TESLA_POWERWALL_CTS2;
}

String Powerwall::AuthCookie() {
    return authCookie;
}

String Powerwall::Pwl_test(String ip) {
    AddLog(PWL_LOGLVL, PSTR("PWL: try to open %s"), ip.c_str());

    ssl_client.setInsecure();
  /** Call setDebugLevel(level) to set the debug
  * esp_ssl_debug_none = 0
  * esp_ssl_debug_error = 1
  * esp_ssl_debug_warn = 2
  * esp_ssl_debug_info = 3
  * esp_ssl_debug_dump = 4
  */
    ssl_client.setDebugLevel(0);

  // Set the receive and transmit buffers size in bytes for memory allocation (512 to 16384).
  // For server that does not support SSL fragment size negotiation, leave this setting the default value
  // by not set any buffer size or set the rx buffer size to maximum SSL record size (16384) and 512 for tx buffer size.  
    //ssl_client.setBufferSizes(1024 /* rx */, 512 /* tx */);
  
  // Assign the basic client
  // Due to the basic_client pointer is assigned, to avoid dangling pointer, basic_client should be existed 
  // as long as it was used by ssl_client for transportation.
    ssl_client.setClient(&basic_client);

    int retry = 0;
    while (retry < PW_RETRIES) {
        int32_t res = ssl_client.connect(ip.c_str(), 443);
        if (res) {
            break;
        }
        delay(100);
        retry++;
    }

    if (retry >= PW_RETRIES) {
        AddLog(PWL_LOGLVL, PSTR("PWL: failed"));
    } else {
        AddLog(PWL_LOGLVL, PSTR("PWL: connected"));
    }

    ssl_client.stop();

    return "\n";
}


void pHexdump(uint8_t *sbuff, uint32_t slen) {
  char cbuff[slen*3+10];
  char *cp = cbuff;
  *cp++ = '>';
  *cp++ = ' ';
  for (uint32_t cnt = 0; cnt < slen; cnt ++) {
    sprintf_P(cp, PSTR("%02x "), sbuff[cnt]);
    cp += 3;
  }
  AddLog(PWL_LOGLVL, PSTR("PWL: response: %s"), cbuff);

}


/**
 * This function returns a string with the authToken based on the basic login endpoint of
 * the powerwall in combination with the credentials from the secrets.h
 * @returns authToken to be used in an authCookie
 */
String Powerwall::getAuthCookie() {
    AddLog(PWL_LOGLVL, PSTR("PWL: requesting new auth Cookie from %s"), powerwall_ip.c_str());
    String apiLoginURL = "/api/login/Basic";

    ssl_client.setInsecure();
    //ssl_client.setBufferSizes(4096 /* rx */, 512 /* tx */);
    ssl_client.setTimeout(3000);
    ssl_client.setClient(&basic_client);
    ssl_client.setDebugLevel(3);

    int retry = 0;
    while (retry < PW_RETRIES) {
        int32_t res = ssl_client.connect(powerwall_ip.c_str(), 443);
        if (res) {
            break;
        }
        delay(100);
        retry++;
    }

    if (retry >= PW_RETRIES) {
        return ("CONN-FAIL");
    }

    AddLog(PWL_LOGLVL, PSTR("PWL: connected"));

    String dataString = "{\"username\":\"customer\",\"email\":\"" + tesla_email + "\",\"password\":\"" + tesla_password + "\",\"force_sm_off\":false}";

    String payload = String("POST ") + apiLoginURL + " HTTP/1.1\r\n" +
                      "Host: " + powerwall_ip + "\r\n" +
                      "Connection: close" + "\r\n" +
                      "Content-Type: application/json" + "\r\n" +
                      "Content-Length: " + dataString.length() + "\r\n" +
                      "\r\n" + dataString + "\r\n\r\n";

    AddLog(PWL_LOGLVL, PSTR("PWL: payload: %s"),payload.c_str());

    ssl_client.println(payload);

    uint8_t flag = 0; 

    uint8_t string[1200];
    uint32_t dlen;
    uint32_t timeout = 30;
    while (ssl_client.connected()) {
        if (ssl_client.available()) {
            dlen = ssl_client.available();
            AddLog(PWL_LOGLVL, PSTR("PWL: available: %d"), dlen);
            String response = "";
#if 1
            if (!flag) {
                char c = ssl_client.peek();
                AddLog(PWL_LOGLVL, PSTR("PWL: peek: %c"), c);
                if (c != 'H') {
                    AddLog(PWL_LOGLVL, PSTR("PWL: wrong response: %c"), c);
                    ssl_client.stop();
                    return "";
                } else {
                    //basic_client.read(string, 17);
                    //ssl_client.read(string, 17);
                    const char *cp = ssl_client.peekBuffer();
                    //ssl_client.peekBytes(string, 17);
                    //ssl_client.peekConsume(17);
                    //string[17] = 0;
                    //pHexdump(string, 17);
                    AddLog(PWL_LOGLVL, PSTR("PWL: 1. response: %s"), cp);
                    cp = strchr(cp, '{');
                    if (cp) {
                        char *cp1 = strchr(cp, '}');
                        if (cp1) {
                            *(cp1 + 1) = 0;
                            AddLog(PWL_LOGLVL, PSTR("PWL: json: %s"), cp);
                            char str_value[256];
                            str_value[0] = 0;
                            float fv;
                            JsonParser parser((char*)cp);
                            JsonParserObject obj = parser.getRootObject();
                            uint32_t res = JsonParsePath(&obj, "token", '#', &fv, str_value, sizeof(str_value));

                            AddLog(PWL_LOGLVL, PSTR("PWL: token: %s"), str_value);

                            ssl_client.stop();
                            return str_value;
                        }
                    }
                }
                flag = 1;
            }
            response = ssl_client.readStringUntil('\n');
            AddLog(PWL_LOGLVL, PSTR("PWL: response: %s"), response.c_str());
 #else
            ssl_client.read(string, dlen);
            pHexdump(string, dlen);
 #endif
            char *cp = (char*)response.c_str();
            if (!strncmp_P(cp, PSTR("HTTP"), 4)) {
                char *sp = strchr(cp, ' ');
                if (sp) {
                    sp++;
                    uint16_t result = strtol(sp, 0, 10);
                    if (result != 200) {
                        ssl_client.stop();
                        return "";
                    } else {
                       // break;
                    }
                }
            }
            if (response == "\r") {
                break;
            }
        }
        timeout--;
        delay(100);
        AddLog(PWL_LOGLVL, PSTR("PWL: timeout: %d"), timeout);
        if (!timeout) {
            ssl_client.stop();
            return "";
        }
    }

    String jsonInput;
    dlen = ssl_client.available();
    if (ssl_client.connected() && dlen) {
        ssl_client.read(string, dlen);
        string[dlen] = 0;
        jsonInput = (char*)string;
        AddLog(PWL_LOGLVL, PSTR("PWL: jsonInput %s"),jsonInput.c_str());
    }

    char str_value[256];
    str_value[0] = 0;
    float fv;
    JsonParser parser((char*)jsonInput.c_str());
    JsonParserObject obj = parser.getRootObject();
    uint32_t res = JsonParsePath(&obj, "token", '#', &fv, str_value, sizeof(str_value));

    AddLog(PWL_LOGLVL, PSTR("PWL: token: %s"), str_value);

    ssl_client.stop();
    
    return str_value;
}

/**
 * This function does a GET-request on the local powerwall web server.
 * This is mainly used here to do API requests.
 * HTTP/1.0 is used because some responses are so big that this would encounter
 * chunked transfer encoding in HTTP/1.1 (https://en.wikipedia.org/wiki/Chunked_transfer_encoding)
 *
 * @param url relative URL on the Powerwall
 * @param authCookie optional, but recommended
 * @returns content of request
 */
String Powerwall::GetRequest(String url, String in_authCookie) {
    

    AddLog(PWL_LOGLVL, PSTR("PWL: cookie %s"), in_authCookie.c_str());

    ssl_client.setInsecure();
    ssl_client.setTimeout(5000);
    ssl_client.setClient(&basic_client);
    //ssl_client.setBufferSizes(4096 /* rx */, 512 /* tx */);
    ssl_client.setBufferSizes(16384, 512);


    if (in_authCookie == "") {
        authCookie = getAuthCookie();
    }

    AddLog(PWL_LOGLVL, PSTR("PWL: doing GET-request to %s - %s"), powerwall_ip.c_str(), url.c_str());

    int retry = 0;

    while ((!ssl_client.connect(powerwall_ip.c_str(), 443)) && (retry < PW_RETRIES)) {
        delay(100);
        //Serial.print(".");
        retry++;
    }

    if (retry >= PW_RETRIES) {
        return ("CONN-FAIL");
    }

    AddLog(PWL_LOGLVL, PSTR("PWL: connected"));

    // HTTP/1.0 is used because of Chunked transfer encoding
    String request = "GET " + url + " HTTP/1.0" + "\r\n" +
                      "Host: " + powerwall_ip + "\r\n" +
                      "Cookie: " + "AuthCookie" + "=" + authCookie + "\r\n" +
                      "Connection: close\r\n\r\n";
    
    ssl_client.println(request);
    
    AddLog(PWL_LOGLVL, PSTR("PWL: request: %s"), request.c_str());

    uint32_t timeout = 500;
    int32_t chunked = 0;
    while (ssl_client.connected()) {
        if (ssl_client.available()) {
            String response = ssl_client.readStringUntil('\n');
            AddLog(PWL_LOGLVL, PSTR("PWL: result %s"), response.c_str());
            if (chunked == -2) {
                // process chunc size
                chunked = strtol(response.c_str(), 0, 16);
                AddLog(PWL_LOGLVL, PSTR("PWL: chunc size %d"), chunked);
                break;
            }
            char *cp =  (char*)response.c_str();
            if (!strncmp_P(cp, PSTR("HTTP"), 4)) {
                char *sp = strchr(cp, ' ');
                if (sp) {
                    sp++;
                    uint16_t result = strtol(sp, 0, 10);
                    AddLog(PWL_LOGLVL, PSTR("PWL: result %d"), result);
                    // in case of error 401, get new cookie
                    if (result == 401) {
                        authCookie = "";
                    } else if (result != 200) {
                        ssl_client.stop();
                        return "\n";
                    }
                }
            }
            if (!strncmp_P(cp, PSTR("Transfer-Encoding: chunked"), 26)) {
                chunked = -1;
                AddLog(PWL_LOGLVL, PSTR("PWL: chunked %d"), chunked);
            }

            if (response == "\r") {
                if (chunked) {
                    // skip
                    chunked = -2;
                } else {
                    break;
                }
            }
        }
        timeout--;
        delay(10);
        if (!timeout) {
            break;
        }
    }

    String result = "\r";

    timeout = 100;
    char *string = (char*)calloc(4096,1);
    if (string) {
        char *cp = string;
        while (ssl_client.connected()) {
            uint16_t dlen;
            dlen = ssl_client.available();
            if (dlen) {
                ssl_client.read((uint8_t*)cp, dlen);
                cp += dlen;
                *cp = 0;
            }
            delay(10);
            timeout--;
            if (!timeout) {
                break;
            }
        }
        AddLog(PWL_LOGLVL, PSTR("PWL: result %s"), string);
        result = string;
        free(string);
    }
    ssl_client.stop();

    // custom replace
    result.replace(cts1, "PW_CTS1");

    result.replace(cts2, "PW_CTS2");

    // shrink data size because it exceeds json parser maxsize
    result.replace("communication_time", "ct");
    result.replace("instant", "i");
    result.replace("apparent", "a");
    result.replace("reactive", "r");

    result.replace("nominal_full_pack_energy", "f_p_e");
    result.replace("nominal_energy_remaining", "n_e_r");
    result.replace("backup_reserve_percent", "b_r_p");

    return result;
}

/**
 * this is getting called if there was no provided authCookie in powerwallGetRequest(String url, String authCookie)
 */
String Powerwall::GetRequest(String url) {
    if (url[0] == '@') {
        if (url[1] == 'D') {
            // define vars
            //AddLog(PWL_LOGLVL, PSTR("PWL: %s - %s - %s"), powerwall_ip.c_str(), tesla_email.c_str(), tesla_password.c_str());
            url = url.substring(2);
            uint16_t pos = strcspn(url.c_str(), ",");
            powerwall_ip = url.substring(0, pos);
            url = url.substring(pos + 1);
            pos = strcspn(url.c_str(), ",");
            tesla_email = url.substring(0, pos);
            tesla_password = url.substring(pos + 1);
            //AddLog(PWL_LOGLVL, PSTR("PWL: %s - %s - %s"), powerwall_ip.c_str(), tesla_email.c_str(), tesla_password.c_str());
            return "";
        } if (url[1] == 'C') {
            url = url.substring(2);
            uint16_t pos = strcspn(url.c_str(), ",");
            cts1 = url.substring(0, pos);
            cts2 = url.substring(pos + 1);
            return "";
        } else {
            url = url.substring(1);
            return Pwl_test(url);
        }
    }
    return (GetRequest(url, getAuthCookie()));
}

#endif
