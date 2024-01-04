/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef ASYNCWEBSERVERHANDLERIMPL_H_
#define ASYNCWEBSERVERHANDLERIMPL_H_

#include <string>
#ifdef ASYNCWEBSERVER_REGEX
#include <regex>
#endif

#include "stddef.h"
#include <time.h>

class AsyncStaticWebHandler: public AsyncWebHandler {
   using File = fs::File;
   using FS = fs::FS;
  private:
    bool _getFile(AsyncWebServerRequest *request);
    bool _fileExists(AsyncWebServerRequest *request, const String& path);
    uint8_t _countBits(const uint8_t value) const;
  protected:
    FS _fs;
    String _uri;
    String _path;
    String _default_file;
    String _cache_control;
    String _last_modified;
    AwsTemplateProcessor _callback;
    bool _isDir;
    bool _gzipFirst;
    uint8_t _gzipStats;
  public:
    AsyncStaticWebHandler(const char* uri, FS& fs, const char* path, const char* cache_control);
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    AsyncStaticWebHandler& setIsDir(bool isDir);
    AsyncStaticWebHandler& setDefaultFile(const char* filename);
    AsyncStaticWebHandler& setCacheControl(const char* cache_control);
    AsyncStaticWebHandler& setLastModified(const char* last_modified);
    AsyncStaticWebHandler& setLastModified(struct tm* last_modified);
  #ifdef ESP8266
    AsyncStaticWebHandler& setLastModified(time_t last_modified);
    AsyncStaticWebHandler& setLastModified(); //sets to current time. Make sure sntp is runing and time is updated
  #endif
    AsyncStaticWebHandler& setTemplateProcessor(AwsTemplateProcessor newCallback) {_callback = newCallback; return *this;}
};

class AsyncCallbackWebHandler: public AsyncWebHandler {
  private:
  protected:
    String _uri;
    WebRequestMethodComposite _method;
    ArRequestHandlerFunction _onRequest;
    ArUploadHandlerFunction _onUpload;
    ArBodyHandlerFunction _onBody;
    bool _isRegex;
  public:
    AsyncCallbackWebHandler() : _uri(), _method(HTTP_ANY), _onRequest(NULL), _onUpload(NULL), _onBody(NULL), _isRegex(false) {}
    void setUri(const String& uri){ 
      _uri = uri; 
      _isRegex = uri.startsWith("^") && uri.endsWith("$");
    }
    void setMethod(WebRequestMethodComposite method){ _method = method; }
    void onRequest(ArRequestHandlerFunction fn){ _onRequest = fn; }
    void onUpload(ArUploadHandlerFunction fn){ _onUpload = fn; }
    void onBody(ArBodyHandlerFunction fn){ _onBody = fn; }

    virtual bool canHandle(AsyncWebServerRequest *request) override final{

      if(!_onRequest)
        return false;

      if(!(_method & request->method()))
        return false;

#ifdef ASYNCWEBSERVER_REGEX
      if (_isRegex) {
        std::regex pattern(_uri.c_str());
        std::smatch matches;
        std::string s(request->url().c_str());
        if(std::regex_search(s, matches, pattern)) {
          for (size_t i = 1; i < matches.size(); ++i) { // start from 1
            request->_addPathParam(matches[i].str().c_str());
          }
        } else {
          return false;
        }
      } else 
#endif
      if (_uri.length() && _uri.startsWith("/*.")) {
         String uriTemplate = String (_uri);
         uriTemplate = uriTemplate.substring(uriTemplate.lastIndexOf("."));
         if (!request->url().endsWith(uriTemplate))
           return false;
      }
      else
      if (_uri.length() && _uri.endsWith("*")) {
        String uriTemplate = String(_uri);
	uriTemplate = uriTemplate.substring(0, uriTemplate.length() - 1);
        if (!request->url().startsWith(uriTemplate))
          return false;
      }
      else if(_uri.length() && (_uri != request->url() && !request->url().startsWith(_uri+"/")))
        return false;

      request->addInterestingHeader("ANY");
      return true;
    }
  
    virtual void handleRequest(AsyncWebServerRequest *request) override final {
      if((_username != "" && _password != "") && !request->authenticate(_username.c_str(), _password.c_str()))
        return request->requestAuthentication();
      if(_onRequest)
        _onRequest(request);
      else
        request->send(500);
    }
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final {
      if((_username != "" && _password != "") && !request->authenticate(_username.c_str(), _password.c_str()))
        return request->requestAuthentication();
      if(_onUpload)
        _onUpload(request, filename, index, data, len, final);
    }
    virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) override final {
      if((_username != "" && _password != "") && !request->authenticate(_username.c_str(), _password.c_str()))
        return request->requestAuthentication();
      if(_onBody)
        _onBody(request, data, len, index, total);
    }
    virtual bool isRequestHandlerTrivial() override final {return _onRequest ? false : true;}
};

#endif /* ASYNCWEBSERVERHANDLERIMPL_H_ */
