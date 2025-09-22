#
# leds_panel.be - implements a real-time mirroring of the WS2812 leds on the main page
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
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

class leds_panel
  var port
  var web
  var sampling_interval
  var p1, p_leds
  var strip                     # strip object
  var h, w, cell_size, cell_space

  static var EXT_NAME = "leds_panel"
  static var SAMPLING = 100
  static var PORT = 8886      # default port 8886
  static var HTML_WIDTH = 290
  static var HTML_HEAD1 = 
    "<!DOCTYPE HTML><html><head>"
  static var HTML_URL_F = 
    "<script>"
    "var event_url='http://%s:%i/leds_feed';"
    "</script>"
  static var HTML_HEAD2 = 
    '<script>'
    'var source = new EventSource(event_url);'
    'var source = new EventSource(event_url);'
    'wl = f => window.addEventListener("load", f);'
    'eb = s => document.getElementById(s);'
    'var led_canvas, led_strip = {"w":0,"h":0,"clsz":0,"clsp":0,"rev":0,"alt":0}, led_canvas_ctx;'

    # reverse_gamma
    'function ledGammaReverse_8(v) {'
        'const table = [[40,1],[80,3],[98,10],[124,26],[158,65],[190,112],[223, 175],[255, 255]];'
        'let p_src=0,p_gamma=0;'
        
        'for (const [src, gamma] of table) {'
            'if (v <= gamma) {'
                'return Math.round(p_src + (v - p_gamma) * (src - p_src) / (gamma - p_gamma));'
            '}'
            'p_src=src;'
            'p_gamma=gamma;'
        '}'
        'return 255;'
    '}'

    'function initEventSource() {'
      'source.addEventListener("leds",function (e) {'
       ' var j = JSON.parse(e.data);'
        'if("w" in j){led_strip.w=j.w;}'
        'if("h" in j){led_strip.h=j.h;}'
        'if("rev" in j){led_strip.rev=j.rev;}'
        'if("clsz" in j){led_strip.clsz=j.clsz;}'
        'if("clsp" in j){led_strip.clsp=j.clsp;}'
        'if("alt" in j){led_strip.alt=j.alt;}'
        'if("hex" in j){drawLED(j.hex);}'
	    '}, false);'
    '};'

    'function jd() {'
      'led_canvas = eb("canvas");'
      'led_canvas_ctx = led_canvas.getContext("2d");'
      'initEventSource();'
    '}'
    'wl(jd);'

   ' function drawLED(msg){'
      'const rows = led_strip.h;'
      'const cols = led_strip.w;'
      'let leds_left = Math.min(msg.length / 6, rows * cols);'
      'const alt = led_strip.alt;'
      'let left2right = true;'
  
      'let cell_space = led_strip.clsp;'
      'let cell_size = led_strip.clsz;'
  
      'led_canvas.height = rows * cell_size;'
      'led_canvas.width = cols * cell_size;'
  
      'const width = led_canvas.width - cell_size;'
      'for(let row = 0; row < rows; row += 1){'
        'for (let i = 0; i < cols && leds_left > 0; i += 1) {'
          'const rgb = msg.substring((i+(row*cols)) * 6,((i+(row*cols))*6)+6);'
          'const r = ledGammaReverse_8(parseInt(rgb.slice(0, 2), 16));'
          'const g = ledGammaReverse_8(parseInt(rgb.slice(2, 4), 16));'
          'const b = ledGammaReverse_8(parseInt(rgb.slice(4, 6), 16));'
          'led_canvas_ctx.fillStyle = `#${r.toString(16).padStart(2, "0")}${g.toString(16).padStart(2, "0")}${b.toString(16).padStart(2, "0")}`;'
          ' if(left2right){'
            'led_canvas_ctx.fillRect((i * cell_size) + 1 , (row * cell_size) + 1, cell_size-cell_space, cell_size-cell_space);'
          '} else {'
            'led_canvas_ctx.fillRect((width - (i * cell_size) ) + 1 , (row * cell_size) + 1, cell_size-cell_space, cell_size-cell_space);'
          '}'
          'leds_left -= 1;'
        '}'
        'if (alt) {'
          'left2right = !left2right;'
        '}'
      '}'
    '}'
    '</script>'
    '</head>'
    '<body>'
      '<style>body{margin:0px;}</style>'
  static var HTML_CONTENT =
     '<table style="width:100%;border:0px;margin:0px;">'
        '<tbody>'
          '<tr><td>'
            '<canvas id="canvas" width="290" height="10" style="display:block;margin-left:auto;margin-right:auto;"></canvas>'
          '</td></tr>'
        '</tbody>'
    '</table>'
  static var HTML_END =
    '</body>'
    '</html>'
  
  def init(port)
    import gpio
    if !gpio.pin_used(gpio.WS2812, 0)
      log("LED: no leds configured, can't start leds_panel", 3)
      return
    end
    if (port == nil)  port = self.PORT   end
    self.port = port

    # start network part
    if tasmota.is_network_up()
      self.init_network()
    else
      tasmota.when_network_up(/ -> self.init_network())
    end
  end

  def init_network()
    self.web = global.webserver_async(self.port)
    self.sampling_interval = self.SAMPLING

    self.strip = Leds()
    self.p1 = bytes(100)
    self.p_leds = self.strip.pixels_buffer()

    self.web.set_chunked(true)
    self.web.set_cors(true)
    self.web.on("/leds_feed", self, self.send_info_feed)        # feed with leds values
    self.web.on("/leds", self, self.send_info_page)             # display leds page

    tasmota.add_driver(self, self.EXT_NAME)                     # also register as `leds_panel` extension
  end

  #################################################################################
  # unload
  #
  # Uninstall the extension and deallocate all resources
  #################################################################################
  def unload()
    self.close()                                                # stop server
    tasmota.remove_driver(self)                                 # remove driver, normally already done by tasmota.unload_ext
    global.undef("webserver_async")                             # free `webserver_async` if it was loaded as part of this file
  end

  #################################################################################
  # stop
  #
  # Stop server
  #################################################################################
  def close()
    tasmota.remove_driver(self)
    if self.web
      self.web.close()
    end
  end

  def update()
    self.p_leds = self.strip.pixels_buffer(self.p_leds)   # update buffer
    self.h = tasmota.settings.light_pixels_height_1 + 1
    self.w = self.strip.pixel_count() / (tasmota.settings.light_pixels_height_1 + 1)
    self.cell_size = tasmota.int(self.HTML_WIDTH / self.w, 4, 25)
    self.cell_space = (self.cell_size <= 6) ? 1 : 2
  end

  def send_info_page(cnx, uri, verb)
    import string
    
    self.update()
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
    cnx.write(self.HTML_CONTENT)
    cnx.write(self.HTML_END)

    cnx.content_stop()
  end
  
  static class feeder
    var app                                   # overarching app (debug_panel)
    var cnx                                   # connection object

    def init(app, cnx)
      self.app = app
      self.cnx = cnx
      tasmota.add_driver(self)
    end

    def close()
      tasmota.remove_driver(self)
    end

    def every_100ms()
      self.send_feed()
    end

    def send_feed()
      var cnx = self.cnx
      if !cnx.connected()
        self.close()
        return
      end

      var server = self.cnx.server
      if cnx.buf_out_empty()
        # if out buffer is not empty, do not send any new information
        var app = self.app
        app.update()
        var payload1 = app.p1
        var p_leds = app.p_leds

        payload1.clear()
        payload1 .. "id:"
        server.bytes_append_int(payload1, tasmota.millis())
        payload1 .. "\r\nevent:leds\r\ndata:"

        payload1 .. '{"w":'
        server.bytes_append_int(payload1, app.w)
        payload1 .. ',"h":'
        server.bytes_append_int(payload1, app.h)
        payload1 .. ',"clsz":'
        server.bytes_append_int(payload1, app.cell_size)
        payload1 .. ',"clsp":'
        server.bytes_append_int(payload1, app.cell_space)
        payload1 .. ',"rev":'
        server.bytes_append_int(payload1, tasmota.settings.light_pixels_reverse)
        payload1 .. ',"alt":'
        server.bytes_append_int(payload1, tasmota.settings.light_pixels_alternate)
        payload1 .. ',"hex":"'
        payload1.appendhex(p_leds)
        payload1 .. '"}\r\n\r\n'
        cnx.write(payload1)
      end
    end

  end

  def send_info_feed(cnx, uri, verb)
    cnx.set_chunked(false)     # no chunking since we use EventSource
    cnx.send(200, "text/event-stream")
    #
    var feed = feeder(self, cnx)
    feed.send_feed()          # send first values immediately
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
      var height = self.h * self.cell_size + 10
      webserver.content_send(
        f'<table style="width:100%;">'
          '<tbody>'
            '<tr>'
              '<td>'
                '<fieldset style="background-color:var(--c_bg);">'
                  '<legend style="text-align:left;">'
                    '<label>'
                      '<input type="checkbox" id="ledchk">&nbsp;Leds mirroring&nbsp;'
                    '</label>'
                  '</legend>'
                  '<iframe id="led_iframe" src="about:blank" hidden="true" '
                    'style="color:#eaeaea;border:0px none;height:{height}px;width:300px;margin:0px 8px 0px 8px;padding:0px 0px;">'
                  '</iframe>'
                '</fieldset>'            
              '</td>'
            '</tr>'
          '</tbody>'
        '</table>'
        '<script>'
          'const leduri="http://{ip}:{self.port}/leds";'
        '</script>'
      )
      webserver.content_send(
        '<script>'
        'function ledm(){'
          'ledchk=eb("ledchk");'
          # checkbox event
          'ledchk.addEventListener("change",(event)=>{'
            'const iframe=document.getElementById("led_iframe");'
            'if(ledchk.checked){'
              # When checked, reload the original content
              'iframe.src=leduri;'
              'iframe.hidden=false;'
            '}else{'
              # When unchecked, replace iframe with itself to unload it
              'iframe.src="about:blank";'
              'iframe.hidden=true;'
            '}'
          '});'
        '}'
        'wl(ledm);'
        '</script>'
      )
    end
  end

end

return leds_panel()
