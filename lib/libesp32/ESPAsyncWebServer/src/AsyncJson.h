// AsyncJson.h
/*
  Async Response to use with ArduinoJson and AsyncWebServer
  Written by Andrew Melvin (SticilFace) with help from me-no-dev and BBlanchon.

  Example of callback in use

   server.on("/json", HTTP_ANY, [](AsyncWebServerRequest * request) {

    AsyncJsonResponse * response = new AsyncJsonResponse();
    JsonObject& root = response->getRoot();
    root["key1"] = "key number one";
    JsonObject& nested = root.createNestedObject("nested");
    nested["key1"] = "key number one";

    response->setLength();
    request->send(response);
  });

  --------------------

  Async Request to use with ArduinoJson and AsyncWebServer
  Written by Arsène von Wyss (avonwyss)

  Example

  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/rest/endpoint");
  handler->onRequest([](AsyncWebServerRequest *request, JsonVariant &json) {
    JsonObject& jsonObj = json.as<JsonObject>();
    // ...
  });
  server.addHandler(handler);
  
*/
#ifndef ASYNC_JSON_H_
#define ASYNC_JSON_H_
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <Print.h>

#if ARDUINOJSON_VERSION_MAJOR == 5
  #define ARDUINOJSON_5_COMPATIBILITY
#else
  #ifndef DYNAMIC_JSON_DOCUMENT_SIZE
    #define DYNAMIC_JSON_DOCUMENT_SIZE  1024
  #endif
#endif

constexpr const char* JSON_MIMETYPE = "application/json";

/*
 * Json Response
 * */

class ChunkPrint : public Print {
  private:
    uint8_t* _destination;
    size_t _to_skip;
    size_t _to_write;
    size_t _pos;
  public:
    ChunkPrint(uint8_t* destination, size_t from, size_t len)
      : _destination(destination), _to_skip(from), _to_write(len), _pos{0} {}
    virtual ~ChunkPrint(){}
    size_t write(uint8_t c){
      if (_to_skip > 0) {
        _to_skip--;
        return 1;
      } else if (_to_write > 0) {
        _to_write--;
        _destination[_pos++] = c;
        return 1;
      }
      return 0;
    }
    size_t write(const uint8_t *buffer, size_t size)
    {
      return this->Print::write(buffer, size);
    }
};

class AsyncJsonResponse: public AsyncAbstractResponse {
  protected:

#ifdef ARDUINOJSON_5_COMPATIBILITY
    DynamicJsonBuffer _jsonBuffer;
#else
    DynamicJsonDocument _jsonBuffer;
#endif

    JsonVariant _root;
    bool _isValid;

  public:    

#ifdef ARDUINOJSON_5_COMPATIBILITY
    AsyncJsonResponse(bool isArray=false): _isValid{false} {
      _code = 200;
      _contentType = JSON_MIMETYPE;
      if(isArray)
        _root = _jsonBuffer.createArray();
      else
        _root = _jsonBuffer.createObject();
    }
#else
    AsyncJsonResponse(bool isArray=false, size_t maxJsonBufferSize = DYNAMIC_JSON_DOCUMENT_SIZE) : _jsonBuffer(maxJsonBufferSize), _isValid{false} {
      _code = 200;
      _contentType = JSON_MIMETYPE;
      if(isArray)
        _root = _jsonBuffer.createNestedArray();
      else
        _root = _jsonBuffer.createNestedObject();
    }
#endif

    ~AsyncJsonResponse() {}
    JsonVariant & getRoot() { return _root; }
    bool _sourceValid() const { return _isValid; }
    size_t setLength() {

#ifdef ARDUINOJSON_5_COMPATIBILITY      
      _contentLength = _root.measureLength();
#else
      _contentLength = measureJson(_root);
#endif

      if (_contentLength) { _isValid = true; }
      return _contentLength;
    }

   size_t getSize() { return _jsonBuffer.size(); }

    size_t _fillBuffer(uint8_t *data, size_t len){
      ChunkPrint dest(data, _sentLength, len);

#ifdef ARDUINOJSON_5_COMPATIBILITY      
      _root.printTo( dest ) ;
#else
      serializeJson(_root, dest);
#endif
      return len;
    }
};

class PrettyAsyncJsonResponse: public AsyncJsonResponse {	
public:
#ifdef ARDUINOJSON_5_COMPATIBILITY
	PrettyAsyncJsonResponse (bool isArray=false) : AsyncJsonResponse{isArray} {}
#else
	PrettyAsyncJsonResponse (bool isArray=false, size_t maxJsonBufferSize = DYNAMIC_JSON_DOCUMENT_SIZE) : AsyncJsonResponse{isArray, maxJsonBufferSize} {}
#endif
	size_t setLength () {
#ifdef ARDUINOJSON_5_COMPATIBILITY
		_contentLength = _root.measurePrettyLength ();
#else
		_contentLength = measureJsonPretty(_root);
#endif
		if (_contentLength) {_isValid = true;}
		return _contentLength;
	}
	size_t _fillBuffer (uint8_t *data, size_t len) {
		ChunkPrint dest (data, _sentLength, len);
#ifdef ARDUINOJSON_5_COMPATIBILITY
		_root.prettyPrintTo (dest);
#else
		serializeJsonPretty(_root, dest);
#endif
		return len;
	}
};

typedef std::function<void(AsyncWebServerRequest *request, JsonVariant &json)> ArJsonRequestHandlerFunction;

class AsyncCallbackJsonWebHandler: public AsyncWebHandler {
private:
protected:
  const String _uri;
  WebRequestMethodComposite _method;
  ArJsonRequestHandlerFunction _onRequest;
  size_t _contentLength;
#ifndef ARDUINOJSON_5_COMPATIBILITY   
  const size_t maxJsonBufferSize;
#endif
  size_t _maxContentLength;
public:
#ifdef ARDUINOJSON_5_COMPATIBILITY      
  AsyncCallbackJsonWebHandler(const String& uri, ArJsonRequestHandlerFunction onRequest) 
  : _uri(uri), _method(HTTP_POST|HTTP_PUT|HTTP_PATCH), _onRequest(onRequest), _maxContentLength(16384) {}
#else
  AsyncCallbackJsonWebHandler(const String& uri, ArJsonRequestHandlerFunction onRequest, size_t maxJsonBufferSize=DYNAMIC_JSON_DOCUMENT_SIZE) 
  : _uri(uri), _method(HTTP_POST|HTTP_PUT|HTTP_PATCH), _onRequest(onRequest), maxJsonBufferSize(maxJsonBufferSize), _maxContentLength(16384) {}
#endif
  
  void setMethod(WebRequestMethodComposite method){ _method = method; }
  void setMaxContentLength(int maxContentLength){ _maxContentLength = maxContentLength; }
  void onRequest(ArJsonRequestHandlerFunction fn){ _onRequest = fn; }

  virtual bool canHandle(AsyncWebServerRequest *request) override final{
    if(!_onRequest)
      return false;

    if(!(_method & request->method()))
      return false;

    if(_uri.length() && (_uri != request->url() && !request->url().startsWith(_uri+"/")))
      return false;

    if ( !request->contentType().equalsIgnoreCase(JSON_MIMETYPE) )
      return false;

    request->addInterestingHeader("ANY");
    return true;
  }

  virtual void handleRequest(AsyncWebServerRequest *request) override final {
    if(_onRequest) {
      if (request->_tempObject != NULL) {

#ifdef ARDUINOJSON_5_COMPATIBILITY    
        DynamicJsonBuffer jsonBuffer;
        JsonVariant json = jsonBuffer.parse((uint8_t*)(request->_tempObject));
        if (json.success()) {
#else
        DynamicJsonDocument jsonBuffer(this->maxJsonBufferSize);
        DeserializationError error = deserializeJson(jsonBuffer, (uint8_t*)(request->_tempObject));
        if(!error) {
          JsonVariant json = jsonBuffer.as<JsonVariant>();
#endif

          _onRequest(request, json);
          return;
        }
      }
      request->send(_contentLength > _maxContentLength ? 413 : 400);
    } else {
      request->send(500);
    }
  }
  virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final {
  }
  virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) override final {
    if (_onRequest) {
      _contentLength = total;
      if (total > 0 && request->_tempObject == NULL && total < _maxContentLength) {
        request->_tempObject = malloc(total);
      }
      if (request->_tempObject != NULL) {
        memcpy((uint8_t*)(request->_tempObject) + index, data, len);
      }
    }
  }
  virtual bool isRequestHandlerTrivial() override final {return _onRequest ? false : true;}
};
#endif
