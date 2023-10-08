/*
  xdrv_52_3_berry_webserver.ino - Berry scripting language, webserver module

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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

// also includes tcp_client

#ifdef USE_BERRY

#ifdef USE_WEBCLIENT

#include <berry.h>
#include "HttpClientLight.h"
#include "be_sys.h"

// Tasmota extension
extern File * be_get_arduino_file(void *hfile);

String wc_UrlEncode(const String& text) {
  const char hex[] = "0123456789ABCDEF";

	String encoded = "";
	int len = text.length();
	int i = 0;
	while (i < len)	{
		char decodedChar = text.charAt(i++);

    if (('a' <= decodedChar && decodedChar <= 'z') ||
        ('A' <= decodedChar && decodedChar <= 'Z') ||
        ('0' <= decodedChar && decodedChar <= '9') ||
        ('=' == decodedChar)) {
      encoded += decodedChar;
		} else {
      encoded += '%';
			encoded += hex[decodedChar >> 4];
			encoded += hex[decodedChar & 0xF];
    }

	}
	return encoded;
}

/*********************************************************************************************\
 * Int constants
 *********************************************************************************************/
// const be_const_member_t webserver_constants[] = {
//     { "BUTTON_CONFIGURATION", BUTTON_CONFIGURATION },
//     { "BUTTON_INFORMATION", BUTTON_INFORMATION },
//     { "BUTTON_MAIN", BUTTON_MAIN },
//     { "BUTTON_MANAGEMENT", BUTTON_MANAGEMENT },
//     { "BUTTON_MODULE", BUTTON_MODULE },
//     { "HTTP_ADMIN", HTTP_ADMIN },
//     { "HTTP_ANY", HTTP_ANY },
//     { "HTTP_GET", HTTP_GET },
//     { "HTTP_MANAGER", HTTP_MANAGER },
//     { "HTTP_MANAGER_RESET_ONLY", HTTP_MANAGER_RESET_ONLY },
//     { "HTTP_OFF", HTTP_OFF },
//     { "HTTP_OPTIONS", HTTP_OPTIONS },
//     { "HTTP_POST", HTTP_POST },
//     { "HTTP_USER", HTTP_USER },
// };

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 *
 * import webclient
 *
\*********************************************************************************************/
extern "C" {
  // Berry: ``
  //
  int32_t wc_init(struct bvm *vm);
  int32_t wc_init(struct bvm *vm) {
    WiFiClient * wcl = new WiFiClient();
    be_pushcomptr(vm, (void*) wcl);
    be_setmember(vm, 1, ".w");
    HTTPClientLight * cl = new HTTPClientLight();
    cl->setUserAgent(USE_BERRY_WEBCLIENT_USERAGENT);
    cl->setConnectTimeout(USE_BERRY_WEBCLIENT_TIMEOUT);   // set default timeout
    be_pushcomptr(vm, (void*) cl);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  int32_t wc_tcp_init(struct bvm *vm);
  int32_t wc_tcp_init(struct bvm *vm) {
    int32_t argc = be_top(vm);
    WiFiClient * wcl;
    if (argc >= 2 && be_iscomptr(vm, 2)) {
      wcl = (WiFiClient*) be_tocomptr(vm, 2);
    } else {
      wcl = new WiFiClient();
    }
    be_pushcomptr(vm, (void*) wcl);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  HTTPClientLight * wc_getclient(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    void *p = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return (HTTPClientLight*) p;
  }

  WiFiClient * wc_getwificlient(struct bvm *vm) {
    be_getmember(vm, 1, ".w");
    void *p = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return (WiFiClient*) p;
  }

  // same but using `.p` argument
  WiFiClient * wc_getwificlient_p(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    void *p = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return (WiFiClient*) p;
  }

  int32_t wc_deinit(struct bvm *vm);
  int32_t wc_deinit(struct bvm *vm) {
    // int32_t argc = be_top(vm); // Get the number of arguments
    HTTPClientLight * cl = wc_getclient(vm);
    if (cl != nullptr) { delete cl; }
    be_pushnil(vm);
    be_setmember(vm, 1, ".p");
    WiFiClient * wcl = wc_getwificlient(vm);
    if (wcl != nullptr) { delete wcl; }
    be_setmember(vm, 1, ".w");
    be_return_nil(vm);
  }

  int32_t wc_tcp_deinit(struct bvm *vm);
  int32_t wc_tcp_deinit(struct bvm *vm) {
    WiFiClient * wcl = wc_getwificlient_p(vm);
    if (wcl != nullptr) { delete wcl; }
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  // wc.url_encode(string) -> string  (static method)
  int32_t wc_urlencode(struct bvm *vm);
  int32_t wc_urlencode(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * s = be_tostring(vm, 1);
      String url = wc_UrlEncode(String(s));
      be_pushstring(vm, url.c_str());
      be_return(vm);  /* return self */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // wc.begin(url:string) -> self
  int32_t wc_begin(struct bvm *vm);
  int32_t wc_begin(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc == 1 || !be_tostring(vm, 2)) { be_raise(vm, "attribute_error", "missing URL as string"); }
    const char * url = be_tostring(vm, 2);
    HTTPClientLight * cl = wc_getclient(vm);
    // open connection
    if (!cl->begin(url)) {
      be_raise(vm, "value_error", "unsupported protocol");
    }
    be_pushvalue(vm, 1);
    be_return(vm);  /* return self */
  }

  // tcp.connect(address:string, port:int [, timeout_ms:int]) -> bool
  int32_t wc_tcp_connect(struct bvm *vm);
  int32_t wc_tcp_connect(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 3 && be_isstring(vm, 2) && be_isint(vm, 3)) {
      WiFiClient * tcp = wc_getwificlient_p(vm);
      const char * address = be_tostring(vm, 2);
      int32_t port = be_toint(vm, 3);
      int32_t timeout = USE_BERRY_WEBCLIENT_TIMEOUT;   // default timeout of 2 seconds
      if (argc >= 4) {
        timeout = be_toint(vm, 4);
      }
      // open connection
      IPAddress ipaddr;
      bool success = WifiHostByName(address, ipaddr);
      if (success) {
        success = tcp->connect(ipaddr, port, timeout);
      }
      be_pushbool(vm, success);
      be_return(vm);  /* return self */
    }
    be_raise(vm, "attribute_error", NULL);
  }

  // wc.close(void) -> nil
  int32_t wc_close(struct bvm *vm);
  int32_t wc_close(struct bvm *vm) {
    HTTPClientLight * cl = wc_getclient(vm);
    cl->end();
    be_return_nil(vm);
  }

  // tcp.close(void) -> nil
  int32_t wc_tcp_close(struct bvm *vm);
  int32_t wc_tcp_close(struct bvm *vm) {
    WiFiClient * tcp = wc_getwificlient_p(vm);
    tcp->stop();
    be_return_nil(vm);
  }

  // tcp.available(void) -> int
  int32_t wc_tcp_available(struct bvm *vm);
  int32_t wc_tcp_available(struct bvm *vm) {
    WiFiClient * tcp = wc_getwificlient_p(vm);
    int32_t available = tcp->available();
    be_pushint(vm, available);
    be_return(vm);
  }

  // wc.wc_set_timeouts([http_timeout_ms:int, tcp_timeout_ms:int]) -> self
  int32_t wc_set_timeouts(struct bvm *vm);
  int32_t wc_set_timeouts(struct bvm *vm) {
    int32_t argc = be_top(vm);
    HTTPClientLight * cl = wc_getclient(vm);
    if (argc >= 2) {
      cl->setTimeout(be_toint(vm, 2));
    }
    if (argc >= 3) {
      cl->setConnectTimeout(be_toint(vm, 3));
    }
    be_pushvalue(vm, 1);
    be_return(vm);  /* return self */
  }

  // wc.set_useragent(user_agent:string) -> self
  int32_t wc_set_useragent(struct bvm *vm);
  int32_t wc_set_useragent(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
      HTTPClientLight * cl = wc_getclient(vm);
      const char * useragent = be_tostring(vm, 2);
      cl->setUserAgent(String(useragent));
      be_pushvalue(vm, 1);
      be_return(vm);  /* return self */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // wc.set_follow_redirects(bool) -> self
  int32_t wc_set_follow_redirects(struct bvm *vm);
  int32_t wc_set_follow_redirects(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isbool(vm, 2)) {
      HTTPClientLight * cl = wc_getclient(vm);
      bbool follow = be_tobool(vm, 2);
      cl->setFollowRedirects(follow ? HTTPC_STRICT_FOLLOW_REDIRECTS : HTTPC_DISABLE_FOLLOW_REDIRECTS);
      be_pushvalue(vm, 1);
      be_return(vm);  /* return self */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // wc.collect_headers( [header:string]+ ) -> self
  int32_t wc_collect_headers(struct bvm *vm);
  int32_t wc_collect_headers(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2) {
      size_t header_len = argc-1;
      const char** header_array = (const char**) be_os_malloc((header_len) * sizeof(const char*));
      if (!header_array) { be_throw(vm, BE_MALLOC_FAIL); }

      for (int32_t i = 0; i < header_len; i++) {
        header_array[i] = be_tostring(vm, i + 2);
      }
      HTTPClientLight * cl = wc_getclient(vm);
      cl->collectHeaders(header_array, header_len);

      be_os_free(header_array);
    }
    be_pushvalue(vm, 1);
    be_return(vm);  /* return self */
  }

  // wc.get_header(header_name:string) -> string
  int32_t wc_get_header(struct bvm *vm);
  int32_t wc_get_header(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
      HTTPClientLight * cl = wc_getclient(vm);
      const char * header_name = be_tostring(vm, 2);
      String ret = cl->header(header_name);
      be_pushstring(vm, ret.c_str());
      be_return(vm);  /* return self */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // wc.wc_set_auth(auth:string | (user:string, password:string)) -> self
  int32_t wc_set_auth(struct bvm *vm);
  int32_t wc_set_auth(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2) && (argc < 3 || be_isstring(vm, 3))) {
      HTTPClientLight * cl = wc_getclient(vm);
      const char * user = be_tostring(vm, 2);
      if (argc == 2) {
        cl->setAuthorization(user);
      } else {
        const char * password = be_tostring(vm, 3);
        cl->setAuthorization(user, password);
      }
      be_pushvalue(vm, 1);
      be_return(vm);  /* return self */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // wc.addheader(name:string, value:string [, first:bool=false [, replace:bool=true]]) -> nil
  int32_t wc_addheader(struct bvm *vm);
  int32_t wc_addheader(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 3 && (be_isstring(vm, 2) || be_isstring(vm, 2))) {
      HTTPClientLight * cl = wc_getclient(vm);

      const char * name = be_tostring(vm, 2);
      const char * value = be_tostring(vm, 3);
      bool first = false;
      bool replace = true;
      if (argc >= 4) {
        first = be_tobool(vm, 4);
      }
      if (argc >= 5) {
        replace = be_tobool(vm, 5);
      }
      // do the call
      cl->addHeader(String(name), String(value), first, replace);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // cw.connected(void) -> bool
  int32_t wc_connected(struct bvm *vm);
  int32_t wc_connected(struct bvm *vm) {
    HTTPClientLight * cl = wc_getclient(vm);
    be_pushbool(vm, cl->connected());
    be_return(vm);  /* return code */
  }

  // tcp.connected(void) -> bool
  int32_t wc_tcp_connected(struct bvm *vm);
  int32_t wc_tcp_connected(struct bvm *vm) {
    WiFiClient * tcp = wc_getwificlient_p(vm);
    be_pushbool(vm, tcp->connected());
    be_return(vm);  /* return code */
  }

  // tcp.write(bytes | string) -> int
  int32_t wc_tcp_write(struct bvm *vm);
  int32_t wc_tcp_write(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && (be_isstring(vm, 2) || be_isbytes(vm, 2))) {
      WiFiClient * tcp = wc_getwificlient_p(vm);
      const char * buf = nullptr;
      size_t buf_len = 0;
      if (be_isstring(vm, 2)) {  // string
        buf = be_tostring(vm, 2);
        buf_len = strlen(buf);
      } else { // bytes
        buf = (const char*) be_tobytes(vm, 2, &buf_len);
      }
      size_t bw = tcp->write(buf, buf_len);
      be_pushint(vm, bw);
      be_return(vm);  /* return code */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // tcp.read() -> string
  int32_t wc_tcp_read(struct bvm *vm);
  int32_t wc_tcp_read(struct bvm *vm) {
    WiFiClient * tcp = wc_getwificlient_p(vm);
    int32_t max_read = -1;      // by default read as much as we can
    if (be_top(vm) >= 2 && be_isint(vm, 2)) {
      max_read = be_toint(vm, 2);
    }
    int32_t btr = tcp->available();
    if (btr <= 0) {
      be_pushstring(vm, "");
    } else {
      if ((max_read >= 0) && (btr > max_read)) {
        btr = max_read;
      }
      char * buf = (char*) be_pushbuffer(vm, btr);
      int32_t btr2 = tcp->read((uint8_t*) buf, btr);
      be_pushnstring(vm, buf, btr2);
    }
    be_return(vm);  /* return code */
  }

  // tcp.readbytes() -> bytes
  int32_t wc_tcp_readbytes(struct bvm *vm);
  int32_t wc_tcp_readbytes(struct bvm *vm) {
    WiFiClient * tcp = wc_getwificlient_p(vm);
    int32_t max_read = -1;      // by default read as much as we can
    if (be_top(vm) >= 2 && be_isint(vm, 2)) {
      max_read = be_toint(vm, 2);
    }
    int32_t btr = tcp->available();
    if (btr <= 0) {
      be_pushbytes(vm, nullptr, 0);
    } else {
      if ((max_read >= 0) && (btr > max_read)) {
        btr = max_read;
      }
      uint8_t * buf = (uint8_t*) be_pushbuffer(vm, btr);
      int32_t btr2 = tcp->read(buf, btr);
      be_pushbytes(vm, buf, btr2);
    }
    be_return(vm);  /* return code */
  }

  void wc_errorCodeMessage(int32_t httpCode, uint32_t http_connect_time) {
    if (httpCode < 0) {
      if (httpCode <= -1000) {
        AddLog(LOG_LEVEL_INFO, D_LOG_HTTP "TLS connection error %d after %d ms", -httpCode - 1000, millis() - http_connect_time);
      } else if (httpCode == -1) {
        AddLog(LOG_LEVEL_INFO, D_LOG_HTTP "Connection timeout after %d ms", httpCode, millis() - http_connect_time);
      } else {
        AddLog(LOG_LEVEL_INFO, D_LOG_HTTP "Connection error %d after %d ms", httpCode, millis() - http_connect_time);
      }
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Connected in %d ms, stack low mark %d"),
        millis() - http_connect_time, uxTaskGetStackHighWaterMark(nullptr));
    }
  }

  // cw.GET(void) -> httpCode:int
  int32_t wc_GET(struct bvm *vm);
  int32_t wc_GET(struct bvm *vm) {
    HTTPClientLight * cl = wc_getclient(vm);
    uint32_t http_connect_time = millis();
    int32_t httpCode = cl->GET();
    wc_errorCodeMessage(httpCode, http_connect_time);
    be_pushint(vm, httpCode);
    be_return(vm);  /* return code */
  }

  // Combined function for POST/PUT/PATCH/DELETE
  enum {
    wc_POST_op,
    wc_PUT_op,
    wc_PATCH_op,
    wc_DELETE_op
  };
  int32_t wc_PostPutPatchDelete(struct bvm *vm, int32_t op);
  int32_t wc_PostPutPatchDelete(struct bvm *vm, int32_t op) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && (be_isstring(vm, 2) || be_isbytes(vm, 2))) {
      HTTPClientLight * cl = wc_getclient(vm);
      const char * buf = nullptr;
      size_t buf_len = 0;
      if (be_isstring(vm, 2)) {  // string
        buf = be_tostring(vm, 2);
        buf_len = strlen(buf);
      } else { // bytes
        buf = (const char*) be_tobytes(vm, 2, &buf_len);
      }
      uint32_t http_connect_time = millis();
      int32_t httpCode;
      switch (op) {
        case wc_PUT_op:
          httpCode = cl->PUT((uint8_t*)buf, buf_len);
          break;
        case wc_PATCH_op:
          httpCode = cl->PATCH((uint8_t*)buf, buf_len);
          break;
        case wc_DELETE_op:
          httpCode = cl->DELETE((uint8_t*)buf, buf_len);
          break;
        case wc_POST_op:
        default:
          httpCode = cl->POST((uint8_t*)buf, buf_len);
          break;
      }
      wc_errorCodeMessage(httpCode, http_connect_time);
      be_pushint(vm, httpCode);
      be_return(vm);  /* return code */
    }
    be_raise(vm, kTypeError, nullptr);
  }


  // wc.POST(string | bytes) -> httpCode:int
  int32_t wc_POST(struct bvm *vm);
  int32_t wc_POST(struct bvm *vm) {
    return wc_PostPutPatchDelete(vm, wc_POST_op);
  }

  // wc.PUT(string | bytes) -> httpCode:int
  int32_t wc_PUT(struct bvm *vm);
  int32_t wc_PUT(struct bvm *vm) {
    return wc_PostPutPatchDelete(vm, wc_PUT_op);
  }

  // wc.PATCH(string | bytes) -> httpCode:int
  int32_t wc_PATCH(struct bvm *vm);
  int32_t wc_PATCH(struct bvm *vm) {
    return wc_PostPutPatchDelete(vm, wc_PATCH_op);
  }

  // wc.DELETE(string | bytes) -> httpCode:int
  int32_t wc_DELETE(struct bvm *vm);
  int32_t wc_DELETE(struct bvm *vm) {
    return wc_PostPutPatchDelete(vm, wc_DELETE_op);
  }

  int32_t wc_getstring(struct bvm *vm);
  int32_t wc_getstring(struct bvm *vm) {
    HTTPClientLight * cl = wc_getclient(vm);
    int32_t sz = cl->getSize();
    // abort if we exceed 32KB size, things will not go well otherwise
    if (sz >= 32767) {
      be_raise(vm, "value_error", "response size too big (>32KB)");
    }
    String payload = cl->getString();
    be_pushstring(vm, payload.c_str());
    cl->end();  // free allocated memory ~16KB
    be_return(vm);  /* return code */
  }

  int32_t wc_writefile(struct bvm *vm);
  int32_t wc_writefile(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
      HTTPClientLight * cl = wc_getclient(vm);
      const char * fname = be_tostring(vm, 2);

      void * f = be_fopen(fname, "w");
      int ret = -1;
      if (f) {
        File * fptr = be_get_arduino_file(f);
        ret = cl->writeToStream(fptr);
      }
      be_fclose(f);
      be_pushint(vm, ret);
      be_return(vm);  /* return code */
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t wc_getsize(struct bvm *vm);
  int32_t wc_getsize(struct bvm *vm) {
    HTTPClientLight * cl = wc_getclient(vm);
    be_pushint(vm, cl->getSize());
    be_return(vm);  /* return code */
  }
}

extern size_t FlashWriteSubSector(uint32_t address_start, const uint8_t *data, size_t size);

const uint32_t STREAM_FLASH_PROGRESS_THRESHOLD_KB = 100;    // display log entry every 100kB
const uint32_t STREAM_FLASH_PROGRESS_THRESHOLD = STREAM_FLASH_PROGRESS_THRESHOLD_KB * 1024;

class StreamFlash: public Stream
{
public:
  StreamFlash(uint32_t addr) : addr_start(addr), offset(0) {};

  size_t write(const uint8_t *buffer, size_t size) override {
    // AddLog(LOG_LEVEL_INFO, "FLASH: addr=%p  hex=%*_H  size=%i", addr_start + offset, 32, buffer, size);
    if (size > 0) {
#if ESP_IDF_VERSION_MAJOR < 5     // TODO later
      esp_err_t ret = spi_flash_write(addr_start + offset, buffer, size);
      if (ret != ESP_OK)  { return 0; }  // error
      offset += size;

      // shall we display a progress indicator?
      if (((offset - size) / STREAM_FLASH_PROGRESS_THRESHOLD) != (offset / STREAM_FLASH_PROGRESS_THRESHOLD)) {
        AddLog(LOG_LEVEL_DEBUG, D_LOG_UPLOAD "Progress %d kB", offset / 1024);
      }
#endif
    }
    return size;
  }
  size_t write(uint8_t data) override {
    write(&data, 1);
    return 1;
  }

  int available() override { return 0; }
  int read() override { return -1; }
  int peek() override { return -1; }
  void flush() override { }

protected:
  uint32_t addr_start;      // start address
  uint32_t offset;          // how many bytes have already been written
};

extern "C" {
  int32_t wc_writeflash(struct bvm *vm);
  int32_t wc_writeflash(struct bvm *vm) {
#if ESP_IDF_VERSION_MAJOR < 5
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isint(vm, 2)) {
      HTTPClientLight * cl = wc_getclient(vm);
      uint32_t addr = be_toint(vm, 2);
      if (addr < 0x10000 || addr >= 0x400000) {
        be_raisef(vm, "value_error", "invalid flash address 0x04X", addr);
      }
      if (addr & (SPI_FLASH_SEC_SIZE-1) != 0) {
        be_raisef(vm, "value_error", "invalid flash address, must be at %iKB boundary 0x%04X", SPI_FLASH_SEC_SIZE/1024, addr);
      }
      int32_t size = cl->getSize();
      if (size <= 0 || addr+size >= 0x400000) {
        be_raisef(vm, "value_error", "invalid flash size 0x%04X", size);
      }

      int32_t size_rounded_4k = (size + SPI_FLASH_SEC_SIZE - 1) & ~(SPI_FLASH_SEC_SIZE - 1);

      //  erase region
      esp_err_t ret;
      AddLog(LOG_LEVEL_DEBUG, D_LOG_UPLOAD "erasing flash at 0x%04X length 0x%04X", addr, size_rounded_4k);
      ret = spi_flash_erase_range(addr, size_rounded_4k);
      if (ret != ESP_OK) {
        be_raisef(vm, "internal_error", "unable to erase flash region (%i)", ret);
      }

      StreamFlash flash_writer(addr);
      AddLog(LOG_LEVEL_DEBUG, D_LOG_UPLOAD "writing flash at 0x%04X size 0x%04X", addr, size);
      int32_t written = cl->writeToStream(&flash_writer);
      if (written <= 0) {
        be_raisef(vm, "internal_error", "unable to write flash (%i)", written);
      }
      if (written != size) {
        be_raisef(vm, "internal_error", "failed, written %i bytes vs %i", written, size);
      }
      AddLog(LOG_LEVEL_DEBUG, D_LOG_UPLOAD "flash writing succesful");

      be_pushint(vm, written);
      be_return(vm);  /* return code */
    }
#endif
    be_raise(vm, kTypeError, nullptr);
  }
}


// a stream which writes to the Bytes object on the top of the stack
class StreamBeBytesWriter: public Stream
{
public:
  StreamBeBytesWriter(bvm *vm_in, int increment = 1024) : vm(vm_in), offset(0), incr(increment) {};

  size_t write(const uint8_t *buffer, size_t size) override {
    // we need size, not len, so can;t just get len with be_tobytes
    be_getmember(vm, -1, ".size");
    int32_t signed_size = be_toint(vm, -1);
    be_pop(vm, 1);  /* bytes() instance is at top */

    // if it won't fit, make the bytes object bigger
    if (offset + size > signed_size){
      int newsize = offset + size + incr;
      AddLog(LOG_LEVEL_INFO, "BE: realloc bytes in StreamBeBytesWriter newsize=%i", newsize);
      be_getmember(vm, -1, "resize");
      be_pushvalue(vm, -2);
      be_pushint(vm, size);
      be_call(vm, 2); /* call b.resize(size) */
      be_pop(vm, 3);  /* bytes() instance is at top */      

      // checkw e got it, because Berry just maxes out?
      be_getmember(vm, -1, ".size");
      signed_size = be_toint(vm, -1);
      be_pop(vm, 1);  /* bytes() instance is at top */
      if (offset + size > signed_size){
        // what should we raise here???
        be_raise(vm, "alloc_error", "did not get enough extra bytes");
      }
    }

    // AddLog(LOG_LEVEL_INFO, "FLASH: addr=%p  hex=%*_H  size=%i", addr_start + offset, 32, buffer, size);
    if (offset + size > signed_size){
      AddLog(LOG_LEVEL_ERROR, "BERRYWC: buffer overrun");
      return size;
    }

    char *bytebuf = (char*) be_tobytes(vm, -1, NULL); /* we get the address of the internam buffer of size 'size' */
    if (!bytebuf){
      AddLog(LOG_LEVEL_ERROR, "BERRYWC: buffer null??");
      return size;
    }

    // stream in our chunk
    memcpy(bytebuf + offset, buffer, size);
    offset += size;

    // set the len
    be_pushint(vm, offset);
    be_setmember(vm, -2, ".len");
    be_pop(vm, 1);
    return size;
  }
  size_t write(uint8_t data) override {
    write(&data, 1);
    return 1;
  }

  int available() override { return 0; }
  int read() override { return -1; }
  int peek() override { return -1; }
  void flush() override { }

protected:
  bvm *vm;                // the berry VM
  uint32_t offset;       // how many bytes have already been written
  size_t incr;           // amount to add to size if it does not fit.
};

extern "C" {
  int32_t wc_getbytes(struct bvm *vm);
  int32_t wc_getbytes(struct bvm *vm) {
    HTTPClientLight * cl = wc_getclient(vm);
    int32_t sz = cl->getSize();
    // abort if we exceed 32KB size, things will not go well otherwise
    if (sz >= 32767) {
      be_raise(vm, "value_error", "response size too big (>32KB)");
    }
    // default to 1K starter if contetn-length not present
    if (sz < 0) sz = 1024;
    // create a bytes object at top of stack.
    // the streamwriter knows how to get it. 
    uint8_t * buf = (uint8_t*) be_pushbytes(vm, nullptr, sz);
    StreamBeBytesWriter memory_writer(vm);
    int32_t written = cl->writeToStream(&memory_writer);
    cl->end();  // free allocated memory ~16KB
    be_return(vm);  /* return code */
  }
}

#endif // USE_WEBCLIENT
#endif  // USE_BERRY
