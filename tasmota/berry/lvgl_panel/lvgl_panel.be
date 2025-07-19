#
# lvgl_panel.be - implements a real-time mirroring of LVGL display on the main page
#
# Copyright (C) 2025  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# make sure we use `webserver_async` if it's already solidified
if !global.contains("webserver_async") || type(global.webserver_async) != 'class'
  class webserver_async
    #############################################################
    # class webserver_async_cnx
    #
    # This instance represents an active connection between
    # the server and a client (TCP connection)
    #############################################################
    static class webserver_async_cnx
      var server                                      # link to server object
      var cnx                                         # holds the tcpclientasync instance
      var close_after_send                            # if true, close after we finished sending the out_buffer
      var fastloop_cb                                 # cb for fastloop
      var buf_in                                      # incoming buffer
      var buf_in_offset
      var buf_out
      var phase                                       # parsing phase: 0/ status line, 1/ headers, 2/ payload
      # request
      var req_verb                                    # verb for request (we support only GET)
      var req_uri                                     # URI for request
      var req_version                                 # HTTP version for request
      var header_host                                 # 'Host' header - useful for redirections
      # response
      var resp_headers                                # (string) aggregate headers
      var chunked                                     # if true enable chunked encoding (default true)
      # conversion
      static var CODE_TO_STRING = {
        # 100: "Continue",
        200: "OK",
        # 204: "No Content",
        301: "Moved Permanently",
        # 400: "Bad Request",
        # 401: "Unauthorized",
        # 403: "Payment Required",      # not sure it's useful in Tasmota context
        404: "Not Found",
        500: "Internal Server Error",
        # 501: "Not Implemented"
      }

      #############################################################
      # init
      #
      # Called when a new connection is received from a client
      # Arg:
      #  - server : main instance of `webserver_async` server
      #  - cnx : instance of `tcpclientasync`
      #
      # By default:
      #   version is HTTP/1.1
      #   response is chunked-encoded
      def init(server, cnx)
        self.server = server
        self.cnx = cnx
        self.buf_in = ''
        self.buf_in_offset = 0
        self.buf_out = bytes()
        self.phase = 0              # 0 = status line
        # util
        self.close_after_send = false
        # response
        self.resp_headers = ''
        self.chunked = true
        # register cb
        self.fastloop_cb = def () self.loop() end   # the closure needs to be kept, to allow removal of fast_loop later
        tasmota.add_fast_loop(self.fastloop_cb)
      end

      #############################################################
      # set_chunked: sets whether the response is chunked encoded
      #   true by default
      #
      def set_chunked(chunked)
        self.chunked = bool(chunked)
      end

      #############################################################
      # connected: returns `true` if the connection is still open
      #
      def connected()
        return self.cnx ? self.cnx.connected() : false
      end

      #############################################################
      # buf_out_empty: returns `true` if out buffer is empty,
      # i.e. all content was sent to client
      #
      def buf_out_empty()
        return size(self.buf_out) == 0
      end

      #############################################################
      # _write: (internal method) write bytes
      #
      # Arg:
      #   v must be bytes()
      #
      def _write(v)
        var sz_v = size(v)
        if (sz_v == 0)   return end         # do nothing if empty content

        var buf_out = self.buf_out          # keep a copy of reference in local variable (avoids multiple dereferencing)
        var buf_out_sz = size(buf_out)
        buf_out.resize(buf_out_sz + sz_v)
        buf_out.setbytes(buf_out_sz, v)

        self._send()                        # try sending `self.buf_out` now
      end

      #############################################################
      # close: close the connection to client
      #
      # Can be called multiple times
      # Does nothing if connection is already closed
      #
      def close()
        # log(f"WEB: closing cnx", 3)
        if (self.cnx != nil)    self.cnx.close()    end
        self.cnx = nil
      end

      #############################################################
      # loop: called by fastloop every 5 ms
      #
      def loop()
        if self.cnx == nil    # if connection is closed, this instance is marked for deletion
          tasmota.remove_fast_loop(self.fastloop_cb)  # remove from fast_loop
          self.fastloop_cb = nil                      # fastloop_cb can be garbage collected
          return
        end

        self._send()                        # try sending any pending output data

        var cnx = self.cnx                  # keep copy 
        if (cnx == nil)   return    end     # it's possible that it was closed after _send()

        # any new incoming data received?
        if cnx.available() > 0
          var buf_in_new = cnx.read()       # read bytes() object
          if (!self.buf_in)                 # use the same instance if none present
            self.buf_in = buf_in_new
          else                              # or append to current incoming buffer
            self.buf_in += buf_in_new
          end
        end

        # parse incoming data if any
        if (self.buf_in)
          self.parse()
        end
      end

      #############################################################
      # _send: (internal method) try sending pendin data out
      #
      # the content is in `self.buf_out`
      #
      def _send()
        # any data waiting to go out?
        var cnx = self.cnx
        if (cnx == nil)   return  end       # abort if connection is closed

        var buf_out = self.buf_out          # keep reference in local variable
        if size(buf_out) > 0
          if cnx.listening()                # is the client ready to receive?
            var sent = cnx.write(buf_out)   # send the buffer, `sent` contains the number of bytes actually sent
            if sent > 0                     # did we succeed in sending anything?
              # we did sent something
              if sent >= size(buf_out)      # the entire buffer was sent, clear it
                # all sent
                self.buf_out.clear()
              else                          # buffer was sent partially, remove what was sent from `out_buf`
                # remove the first bytes already sent
                self.buf_out.setbytes(0, buf_out, sent)     # copy to index 0 (start of buffer), content from the same buffer starting at offset 'sent'
                self.buf_out.resize(size(buf_out) - sent)   # shrink buffer
              end
            end
          end
        else
          # empty buffer, do the cleaning
          # self.buf_out.clear()              # TODO not needed?
          # self.buf_in_offset = 0            # TODO really useful?

          if self.close_after_send          # close connection if we have sent everything
            self.close()
          end
        end
      end

      #############################################################
      # parse incoming
      #
      # pre: self.buf_in is not empty
      # post: self.buf_in has made progress (smaller or '')
      def parse()
        # log(f"WEB: incoming {bytes().fromstring(self.buf_in).tohex()}", 3)
        if self.phase == 0
          self.parse_http_req_line()
        elif self.phase == 1
          self.parse_http_headers()
        elif self.phase == 2
          self.parse_http_payload()
        end
      end

      #############################################################
      # parse incoming request
      #
      # pre: self.buf_in is not empty
      # post: self.buf_in has made progress (smaller or '')
      def parse_http_req_line()
        var m = global._re_http_srv.match2(self.buf_in, self.buf_in_offset)
        # Ex: "GET / HTTP/1.1\r\n"
        if m
          var offset = m[0]
          self.req_verb = m[1]                      # GET/POST...
          self.req_uri = m[2]                       # /
          self.req_version = m[3]                   # "1.0" or "1.1"
          self.phase = 1                            # proceed to parsing headers
          self.buf_in = self.buf_in[offset .. ]     # remove what we parsed
          if tasmota.loglevel(4)
            log(f"WEB: HTTP verb: {self.req_verb} URI: '{self.req_uri}' Version:{self.req_version}", 4)
          end
          self.parse_http_headers()
        elif size(self.buf_in) > 100        # if no match and we still have 100 bytes, then it fails
          log("WEB: error invalid request", 4)
          self.close()
          self.buf_in = ''
        end
      end

      #############################################################
      # parse incoming headers
      def parse_http_headers()
        while true
          # print("parse_http_headers", "self.buf_in_offset=", self.buf_in_offset)
          var m = global._re_http_srv_header.match2(self.buf_in, self.buf_in_offset)
          # print("m=", m)
          # Ex: [32, 'Content-Type', 'application/json']
          if m
            self.event_http_header(m[1], m[2])
            self.buf_in_offset += m[0]
          else  # no more headers
            var m2 = global._re_http_srv_body.match2(self.buf_in, self.buf_in_offset)
            if m2
              # end of headers
              # we keep \r\n which is used by pattern
              self.buf_in = self.buf_in[self.buf_in_offset + m2[0] .. ]   # truncate
              self.buf_in_offset = 0

              # self.event_http_headers_end()     # no more headers
              self.phase = 2
              self.parse_http_payload()         # continue to parsing payload
            end
            if size(self.buf_in) > 1024       # we don't accept a single header larger than 1KB
              log("WEB: error header is bigger than 1KB", 4)
              self.close()
              self.buf_in = ''
            end
            return
          end
        end

      end

      #############################################################
      # event_http_header: method called for each header received
      #
      # Default implementation only stores "Host" header
      # and ignores all other headers
      #
      # Args:
      #   header_key: string
      #   header_value: string
      #
      def event_http_header(header_key, header_value)
        # log(f"WEB: header key '{header_key}' = '{header_value}'")

        if (header_key == "Host")
          self.header_host = header_value
        end
      end

      #############################################################
      # event_http_headers_end: called afte all headers are received
      #
      # By default does nothing
      #
      # def event_http_headers_end()
      # end

      #############################################################
      # parse incoming payload (if any)
      #
      # Calls the server's dispatcher with 'verb' and 'uri'
      #
      # Payload is in `self.buf_in`
      #
      def parse_http_payload()
        # log(f"WEB: parsing payload '{bytes().fromstring(self.buf_in).tohex()}'")
        # dispatch request before parsing payload
        self.server.dispatch(self, self.req_uri, self.req_verb)
      end


      #############################################################
      # Responses
      #############################################################
      # send_header: add header to the response
      #
      # Args:
      #   name: key of header
      #   value: value of header
      #   first: if 'true' prepend, or append if 'false'
      def send_header(name, value, first)
        if first
          self.resp_headers = f"{name}: {value}\r\n{self.resp_headers}"
        else
          self.resp_headers = f"{self.resp_headers}{name}: {value}\r\n"
        end
      end

      #############################################################
      # send: send response to client
      #
      # Args
      #   code: (int) http code (ex: 200)
      #   content_type: (string, opt) MIME type, "text/html" if not specified
      #   content: (bytes or string, opt) first content to send to client (you can send more later)
      #
      def send(code, content_type, content)
        var response = f"HTTP/1.1 {code} {self.CODE_TO_STRING.find(code, 'UNKNOWN')}\r\n"
        self.send_header("Content-Type", content_type ? content_type : "text/html", true)

        self.send_header("Accept-Ranges", "none")
        # chunked encoding?
        if self.chunked
          self.send_header("Transfer-Encoding", "chunked")
        end
        # cors?
        if self.server.cors
          self.send_header("Access-Control-Allow-Origin", "*")
          self.send_header("Access-Control-Allow-Methods", "*")
          self.send_header("Access-Control-Allow-Headers", "*")
        end
        # others
        self.send_header("Connection", "close")

        response += self.resp_headers
        response += "\r\n"
        self.resp_headers = nil

        # send status-line and headers
        self.write_raw(response)

        # send first part of content
        if (content)    self.write(content)   end
      end

      #############################################################
      # write: writes a bytes or string piece of content
      #
      # If chunked encoding is enabled, it is sent as a separate chunk
      #
      # If content is empty, it can be sent as an empty chunk
      # which is an indicator of end-of-content
      #
      def write(v)
        if type(v) == 'string'          # if string, convert to bytes
          v = bytes().fromstring(v)
        end

        # use chunk encoding
        if self.chunked
          var p1 = self.server.p1
          p1.clear()
          p1.append(f"{size(v):X}\r\n")
          p1.append(v)
          p1.append("\r\n")

          # log(f"WEB: sending chunk '{p1.tohex()}'")
          self._write(p1)
        else
          self._write(v)
        end
      end

      #############################################################
      # write_raw: low-level write of string or bytes (without chunk encoding)
      #
      # If content is empty, nothing is sent
      #
      def write_raw(v)
        if (size(v) == 0)   return end

        if type(v) == 'string'          # if string, convert to bytes
          v = bytes().fromstring(v)
        end

        self._write(v)
      end

      #############################################################
      # content_stop: signal that the response is finished
      #
      def content_stop()
        self.write('')                  # send 'end-of-content' for chunked encoding
        self.close_after_send = true    # close connection when everything was sent to client
      end
    end

    #######################################################################
    # class webserver_async_dispatcher
    #
    # Pre-register callbacks triggered when a certain URL is accessed
    #
    # You can register either a pure function or a method and an instance
    #
    # Pure function:
    #   webserver_async_dispatcher(uri_prefix, nil, func, verb)
    #  will call:
    #   func(cnx, uri, verb)
    #
    # Instance and method:
    #   webserver_async_dispatcher(uri_prefix, instance, method, verb)
    #  will call:
    #   insatnce.method(cnx, uri, verb)
    #
    # Args in:
    #   uri_prefix: prefix string for matchin URI, must start with '/'
    #   cb_obj: 'nil' for pure function or instance from which we call a method
    #   cb_mth: pure function or method to call
    #   verb: verb to match, only supported: 'GET' or 'nil' for any
    #
    # Args of callback:
    #   cnx: instance of 'webserver_async_cnx' for the current connection
    #   uri: full uri of request
    #   verb: verb received (currently only GET is supported)
    #######################################################################
    static class webserver_async_dispatcher
      var uri_prefix                                  # prefix string, must start with '/'
      var verb                                        # verb to match, or nil for ANY
      var cb_obj                                      # callback object (sent as first argument if not 'nil')
      var cb_mth                                      # callback function

      def init(uri_prefix, cb_obj, cb_mth, verb)
        self.uri_prefix = uri_prefix
        self.cb_obj = cb_obj
        self.cb_mth = cb_mth
        self.verb = verb
      end

      # return true if matched
      def dispatch(cnx, uri, verb)
        import string
        if string.find(uri, self.uri_prefix) == 0
          var match = false
          if (self.verb == nil) || (self.verb == verb)
            # method is valid
            var cb_obj = self.cb_obj
            if (cb_obj != nil)
              self.cb_mth(self.cb_obj, cnx, uri, verb)
            else
              self.cb_mth(cnx, uri, verb)
            end
            return true
          end
        end
        return false
      end
    end

    #############################################################
    # class webserver_async
    #
    # This is the main class to call
    #############################################################
    var local_port                                  # listening port, 80 is already used by Tasmota
    var server                                      # instance of `tcpserver`
    var fastloop_cb                                 # closure used by fastloop
    # var timeout                                     # default timeout for tcp connection
    var connections                                 # list of active connections
    # var auth                                        # web authentication string (Basic Auth) or `nil`, in format `user:password` as bade64
    # var cmd                                         # GET url command
    var dispatchers
    # copied in each connection
    var chunked                                     # if true enable chunked encoding (default true)
    var cors                                        # if true send CORS headers (default false)
    #
    var p1                                          # temporary object bytes() to avoid reallocation

    # static var TIMEOUT = 1000                       # default timeout: 1000ms
    # static var HTTP_REQ = "^(\\w+) (\\S+) HTTP\\/(\\d\\.\\d)\r\n"
    # static var HTTP_HEADER_REGEX = "([A-Za-z0-9-]+): (.*?)\r\n"       # extract a header with its 2 parts
    # static var HTTP_BODY_REGEX   = "\r\n"                             # end of headers

    #############################################################
    # init
    def init(port, timeout)
      # if (timeout == nil)   timeout = self.TIMEOUT    end
      # if (timeout == nil)   timeout = 1000    end
      self.connections = []
      self.dispatchers = []
      self.server = tcpserver(port)                 # throws an exception if port is not available
      self.chunked = true
      self.cors = false
      self.p1 = bytes(100)              # reserve 100 bytes by default
      # TODO what about max_clients ?
      self.compile_re()
      # register cb
      tasmota.add_driver(self)
      self.fastloop_cb = def () self.loop() end
      tasmota.add_fast_loop(self.fastloop_cb)
    end

    #############################################################
    # compile once for all the regex
    def compile_re()
      import re
      if !global.contains("_re_http_srv")
        # global._re_http_srv         = re.compile(self.HTTP_REQ)
        # global._re_http_srv_header  = re.compile(self.HTTP_HEADER_REGEX)
        # global._re_http_srv_body   = re.compile(self.HTTP_BODY_REGEX)
        global._re_http_srv         = re.compile("^(\\w+) (\\S+) HTTP\\/(\\d\\.\\d)\r\n")
        global._re_http_srv_header  = re.compile("([A-Za-z0-9-]+): (.*?)\r\n")
        global._re_http_srv_body   = re.compile("\r\n")
      end
    end

    #############################################################
    # enable or disable chunked mode (enabled by default)
    def set_chunked(chunked)
      self.chunked = bool(chunked)
    end

    #############################################################
    # enable or disable CORS mode (enabled by default)
    def set_cors(cors)
      self.cors = bool(cors)
    end

    #############################################################
    # Helper function to encode integer as hex (uppercase)
    static def bytes_format_hex(b, i, default)
      b.clear()
      if (i == nil)   b .. default    return  end
      # sanity check
      if (i < 0)    i = -i    end
      if (i < 0)    return    end     # special case for MININT
      if (i == 0)   b.resize(1)   b[0] = 0x30   return  end   # return bytes("30")

      b.resize(8)
      var len = 0
      while i > 0
        var digit = i & 0x0F
        if (digit < 10)
          b[len] = 0x30 + digit
        else
          b[len] = 0x37 + digit # 0x37 = 0x41 ('A') - 10
        end
        len += 1
        i = (i >> 4)
      end
      # reverse order
      b.resize(len)
      b.reverse()
    end

    #############################################################
    # Helper function to encode integer as int
    static def bytes_append_int(b, i, default)
      var sz = size(b)
      if (i == 0)         # just append '0'
        b.resize(sz + 1)
        b[sz] = 0x30
      elif (i != nil)     # we have a non-zero value
        var negative = false
        # sanity check
        if (i < 0)    i = -i    negative = true   end
        if (i < 0)    return b   end     # special case for MININT
    
        if negative
          b.resize(sz + 1)
          b[sz] = 0x2D
          sz += 1
        end
    
        var start = sz
        while i > 0
          var digit = i % 10
          b.resize(sz + 1)
          b[sz] = 0x30 + digit
          sz += 1
          i = (i / 10)
        end
        # reverse order starting where the integer is
        b.reverse(start)

      else                # i is `nil`, append default
        b.append(default)
      end
      return b
    end

    #############################################################
    # closing web server
    def close()
      tasmota.remove_driver(self)
      tasmota.remove_fast_loop(self.fastloop_cb)
      self.fastloop_cb = nil
      self.server.close()

      # close all active connections
      for cnx: self.connections
        cnx.close()
      end
      self.connections = nil      # and free memory
    end

    #############################################################
    # clean connections
    #
    # Remove any connections that is closed or in error
    def clean_connections()
      var idx = 0
      while idx < size(self.connections)
        var cnx = self.connections[idx]
        # remove if not connected
        if !cnx.connected()
          # log("WEB: does not appear to be connected")
          cnx.close()
          self.connections.remove(idx)
        else
          idx += 1
        end
      end
    end

    #############################################################
    # called by fastloop
    def loop()
      self.clean_connections()
      # check if any incoming connection
      while self.server.hasclient()
        # retrieve new client
        var cnx = self.webserver_async_cnx(self, self.server.acceptasync())
        cnx.set_chunked(self.chunked)
        self.connections.push(cnx)
      end
    end

    #############################################################
    # add to dispatcher
    def on(prefix, obj, mth, verb)
      var dispatcher = self.webserver_async_dispatcher(prefix, obj, mth, verb)
      self.dispatchers.push(dispatcher)
    end

    #############################################################
    # add to dispatcher
    def dispatch(cnx, uri, verb)
      var idx = 0
      while idx < size(self.dispatchers)
        if (self.dispatchers[idx].dispatch(cnx, uri, verb))
          return
        end
        idx += 1
      end
      # fallback unsupported request  
      cnx.send(500, "text/plain")
      cnx.write("Unsupported")
      cnx.content_stop()
    end

  end

  # assign the class to a global
  global.webserver_async = webserver_async
end

class lvgl_panel
  var port
  var web
  var p1                          # bytes() object reused when generating payload
  var feeders

  # static var SAMPLING = 100
  static var PORT = 8881      # default port 8881

  static var HTML_HEAD1 = 
    "<!DOCTYPE HTML><html><head>"
  static var HTML_URL_F = 
    "<script>"
    "var event_url='http://%s:%i/lvgl_feed';"
    "</script>"
  static var HTML_HEAD2 = 
    '<script>'
    'var source = new EventSource(event_url);'
    'wl = f => window.addEventListener("load", f);'
    'eb = s => document.getElementById(s);'
    'var lvgl_canvas, lvgl_info = {"x1":0,"y1":0,"x2":0,"y2":0}, lvgl_canvas_ctx;'

    'function initEventSource() {'
      'source.addEventListener("lvgl",function(e){'
       ' var j=JSON.parse(e.data);'
        'if("x1" in j){lvgl_info.x1=j.x1;}'
        'if("y1" in j){lvgl_info.y1=j.y1;}'
        'if("x2" in j){lvgl_info.x2=j.x2;}'
        'if("y2" in j){lvgl_info.y2=j.y2;}'
        'if("b64" in j){drawPixels(j.b64);}'
	    '}, false);'
    '};'

    'function jd() {'
      'lvgl_canvas=eb("canvas");'
      'lvgl_canvas_ctx=lvgl_canvas.getContext("2d");'
      'initEventSource();'

      # Mouse down event
      'lvgl_canvas.addEventListener("mousedown", (event) => {'
        'event.preventDefault();'
        'const coords=getCanvasCoordinates(event);'
        'sendTS(coords.x,coords.y);'
      '});'

    '}'
    'wl(jd);'

    'function drawPixels(b64){'
      'const p565=atob(b64);'
    
      # Calculate dimensions
      'const width=lvgl_info.x2-lvgl_info.x1+1;'
      'const height=lvgl_info.y2-lvgl_info.y1+1;'
      # 'console.log(`width: (${lvgl_info.x1},${lvgl_info.y1},${lvgl_info.x2},${lvgl_info.y2}) ${width}, height: ${height}, p565.length; ${p565.length}`);'
    
      # Create buffers for the RGB data
      'const imageData=lvgl_canvas_ctx.createImageData(width,height);'
      'const rgb=new Uint8ClampedArray(width*height*4);'

      # 'console.log(`p565.length ${p565.length} allocated ${width * height}`);'
      # Convert RGB565 to RGBA in a single pass
      'for(let i=0;i<p565.length;i+=2){'
        'const p0=p565[i].charCodeAt(0);'
        'const p1=p565[i+1].charCodeAt(0);'
        'const pos=i*2;'
        
        # Extract RGB components and convert to 8-bit in a single operation
        'rgb[pos]=((p1>>3)&0x1F)<<3;'               # R
        'rgb[pos+1]=((p1&0x07)<<5)|((p0>>5)<<2);'   # G
        'rgb[pos+2]=(p0&0x1F)<<3;'                  # B
        'rgb[pos+3]=255;'                           # A
      '}'
      # Update the ImageData
      'imageData.data.set(rgb);'
    
      # Put the image data on the canvas
      'lvgl_canvas_ctx.putImageData(imageData,lvgl_info.x1,lvgl_info.y1);'
    '}'

    ######################
    # virtual touchscreen
    # Function to calculate relative coordinates within canvas
    'function getCanvasCoordinates(event){'
      'const rect=lvgl_canvas.getBoundingClientRect();'
      'return{x:event.clientX-rect.left,y:event.clientY-rect.top};'
    '}'

    # Function to send events to the embedded system
    'function sendTS(x,y){'
      'fetch("/lvgl_touch",{'
        'method:"POST",'
        'headers:{'
          '"Content-Type":"application/json",'
        '},'
        'body:JSON.stringify({x,y})'
      '});'
      # '}).catch(error => console.error("Error:", error));'
    '}'

    '</script>'
    '</head>'
    '<body>'
      '<style>body{margin:0px;}</style>'
  static var HTML_CONTENT =
      '<table style="width:100%%;border:0px;margin:0px;">'
        '<tbody>'
          '<tr><td>'
            '<canvas id="canvas" width="%i" height="%i" style="display:block;margin-left:auto;margin-right:auto;"></canvas>'
          '</td></tr>'
        '</tbody>'
    '</table>'
  static var HTML_END =
    '</body>'
    '</html>'
  
  def init(port)
    if (port == nil)  port = self.PORT   end
    self.port = port
    self.web = global.webserver_async(port)

    self.p1 = bytes(100)
    self.feeders = []

    self.web.set_chunked(true)
    self.web.set_cors(true)
    self.web.on("/lvgl_feed", self, self.send_info_feed)            # feed with lvgl pixels
    self.web.on("/lvgl_touch", self, self.touch_received, "POST")   # virtual touch screen
    self.web.on("/lvgl", self, self.send_info_page)                 # display lvgl page

    import cb
    var paint_cb = cb.gen_cb(def (x1,y1,x2,y2,pixels) self.paint_cb(x1,y1,x2,y2,pixels) end)
    lv.set_paint_cb(paint_cb)

    tasmota.add_driver(self)
  end

  def close()
    tasmota.remove_driver(self)
    self.web.close()
  end

  def update()
  end

  def touch_received(cnx, uri, verb)
    # log(f">>>TS: touch_received {uri=} {verb=} {cnx.buf_in=}")
    cnx.close()
    # Example of events:
    #   {"x":376,"y":258}
    import json
    import display
    var touch = json.load(cnx.buf_in)     # POST payload
    if (touch == nil)
      log(f"LVG: received invalid touch event '{cnx.buf_in}'")
      return
    end

    if (tasmota.loglevel(4))
      log(f"LVG: received touch event '{touch}'")
    end

    display.touch_update(1, touch.find('x', 0), touch.find('y', 0), 0)
  end

  def paint_cb(x1,y1,x2,y2,pixels)
    if (size(self.feeders) == 0)    return    end       # nothing to do if no feeders

    import introspect
    var pixels_count = (x2-x1+1) * (y2-y1+1)
    var pixels_bytes = bytes(introspect.toptr(pixels), pixels_count * 2)
    #log(f">>>>>: {x1=} {x2=} {y1=} {y2=} {pixels_count=} {size(pixels_bytes)=}")

    var bytes_per_line = (x2 - x1 + 1) * 2
    var lines_remaining = (y2 - y1 + 1)
    var lines_per_msg = 2000 / bytes_per_line
    var bytes_per_msg = lines_per_msg * bytes_per_line
    var y = y1
    var offset_bytes = 0

    #log(f">>>>>: {x1=} {x2=} {y1=} {y2=} {bytes_per_line=} {lines_per_msg=} {bytes_per_msg=}")

    while lines_remaining > 0
      # compute the workload
      # var payload = pixels_bytes[offset_bytes .. offset_bytes + bytes_per_msg - 1].tob64()    # string in base64

      var idx = 0
      var lines_to_send = (lines_per_msg > lines_remaining) ? lines_remaining : lines_per_msg
      var bytes_to_send = lines_to_send * bytes_per_line
      while idx < size(self.feeders)
        self.feeders[idx].send_feed(x1, y, x2, y + lines_to_send - 1, pixels_bytes, offset_bytes, bytes_to_send)
        idx += 1
      end

      # move to next message
      offset_bytes += bytes_to_send
      y += lines_to_send
      lines_remaining -= lines_to_send
    end

    # log(f">>>: paint {x1=} {y1=} {x2=} {y2=} {pixels_count=}", 2)
  end

  def add_feed(feed)
    if self.feeders.find(feed) == nil           # make sure it's not already in teh list
      self.feeders.push(feed)
    end
  end

  def remove_feed(feed)
    var idx = self.feeders.find(feed)
    if idx != nil
      self.feeders.remove(idx)
    end
  end

  def send_info_page(cnx, uri, verb)
    import string
    var height = lv.get_ver_res()
    var width = lv.get_hor_res()
    
    var host = cnx.header_host
    var host_split = string.split(host, ':')      # need to make it stronger
    var ip = host_split[0]
    var port = 80
    if size(host_split) > 1
      port = int(host_split[1])
    end

    cnx.send(200, "text/html")
    cnx.write(self.HTML_HEAD1)
    cnx.write(format(self.HTML_URL_F, ip, port))
    cnx.write(self.HTML_HEAD2)
    cnx.write(format(self.HTML_CONTENT, width, height))
    cnx.write(self.HTML_END)

    cnx.content_stop()
  end
  
  static class feeder
    var app                                   # overarching app (debug_panel)
    var cnx                                   # connection object
    var w, h

    def init(app, cnx)
      self.app = app
      self.cnx = cnx
      self.w = lv.get_hor_res()
      self.h = lv.get_ver_res()
      #tasmota.add_driver(self)
    end

    def close()
      self.app.remove_feed(self)
    end

    # payload is max 16KB
    def send_feed(x1, y1, x2, y2, pixels, idx, len)
      var cnx = self.cnx
      if !cnx.connected()
        self.close()
        return nil
      end

      var server = self.cnx.server
      # if cnx.buf_out_empty()
        # if out buffer is not empty, do not send any new information
      var app = self.app
      var p1 = app.p1

      p1.clear()
      p1.append("id:")
      server.bytes_append_int(p1, tasmota.millis())
      p1.append("\r\nevent:lvgl\r\ndata:")

      p1.append('{"x1":')
      server.bytes_append_int(p1, x1)
      p1.append(',"x2":')
      server.bytes_append_int(p1, x2)
      p1.append(',"y1":')
      server.bytes_append_int(p1, y1)
      p1.append(',"y2":')
      server.bytes_append_int(p1, y2)
      p1.append(',"b64":"')
      p1.appendb64(pixels, idx, len)
      p1.append('"}\r\n\r\n')
      cnx.write(p1)
      # end
    end

  end

  def send_info_feed(cnx, uri, verb)
    cnx.set_chunked(false)     # no chunking since we use EventSource
    cnx.send(200, "text/event-stream")
    #
    var feed = feeder(self, cnx)
    self.add_feed(feed)
    lv.scr_act().invalidate()                 # force a screen redraw for any new connection
  end

  def web_add_main_button()
    self.send_iframe_code()
  end

  def send_iframe_code()
    import webserver
    self.update()
    var ip = tasmota.wifi().find('ip')
    if (ip == nil)
      ip = tasmota.eth().find('ip')
    end
    if (ip != nil)
      var height = lv.get_ver_res() + 10
      var width = lv.get_hor_res() + 20
      if (width < 340) width = 340 end
      webserver.content_send(
       f'<table style="width:100%;">'
          '<tbody>'
            '<tr>'
              '<td>'
                '<fieldset style="background-color:{tasmota.webcolor(1)};">'
                  '<legend style="text-align:left;">'
                    '<label>'
                      '<input type="checkbox" id="lvchk">&nbsp;LVGL screen mirroring&nbsp;'
                    '</label>'
                  '</legend>'
                  '<iframe id="lvgl_iframe" src="about:blank" hidden="true" '
                    'style="color:#eaeaea; border:0px none;height:{height}px;width:{width}px;margin:0px 8px 0px 8px;padding:0px 0px;">'
                  '</iframe>'
                '</fieldset>'            
              '</td>'
            '</tr>'
          '</tbody>'
        '</table>'
        '<script>'
          'const lvuri="http://{ip}:{self.port}/lvgl";'
        '</script>'
      )
      webserver.content_send(
        '<script>'
        'function lvg(){'
          'lvchk=eb("lvchk");'
          # checkbox event
          'lvchk.addEventListener("change",(event)=>{'
            'const iframe=document.getElementById("lvgl_iframe");'
            'if(lvchk.checked){'
              # When checked, reload the original content
              'iframe.src=lvuri;'
              'iframe.hidden=false;'
            '}else{'
              # When unchecked, replace iframe with itself to unload it
              'iframe.src="about:blank";'
              'iframe.hidden=true;'
            '}'
          '});'
        '}'
        'wl(lvg);'
        '</script>'
      )
    end
  end

end

return lvgl_panel()
