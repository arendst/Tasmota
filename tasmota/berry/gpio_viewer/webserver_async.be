#
# webserber_async.be - implements a generic async non-blocking HTTP server
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

# restrictions for now:
# 
# Listen to all interfaces
# - GET only
# - no HTTPS
# - support for limited headers
# - HTTP 1.0 only

#@ solidify:webserver_async
#@ solidify:Webserver_async_cnx

#############################################################
# class Webserver_async_cnx
#############################################################
class Webserver_async_cnx
  var server                                      # link to server object
  var cnx                                         # holds the tcpclientasync instance
  var close_after_send                            # if true, close after sending
  var fastloop_cb                                 # cb for fastloop
  var buf_in                                      # incoming buffer
  var buf_in_offset
  var buf_out
  var phase                                       # parsing phase: 0/ status line, 1/ headers, 2/ payload
  # request
  var req_verb
  var req_uri
  var req_version
  var header_host
  # response
  var resp_headers
  var resp_version
  var chunked                                     # if true enable chunked encoding (default true)
  var cors                                        # if true send CORS headers (default false)
  # bytes objects to be reused
  var payload1
  # conversion
  static var CODE_TO_STRING = {
    100: "Continue",
    200: "OK",
    204: "No Content",
    301: "Moved Permanently",
    400: "Bad Request",
    401: "Unauthorized",
    403: "Payment Required",
    404: "Not Found",
    500: "Internal Server Error",
    501: "Not Implemented"
  }

  #############################################################
  # init
  def init(server, cnx)
    self.server = server
    self.cnx = cnx
    self.buf_in = ''
    self.buf_in_offset = 0
    self.buf_out = bytes()
    self.phase = 0
    # util
    self.payload1 = bytes()
    self.close_after_send = false
    # response
    self.resp_headers = ''
    self.resp_version = 1       # HTTP 1.1      # TODO
    self.chunked = true
    self.cors = false
    # register cb
    self.fastloop_cb = def () self.loop() end
    tasmota.add_fast_loop(self.fastloop_cb)
  end

  def set_chunked(chunked)
    self.chunked = bool(chunked)
  end

  def set_cors(cors)
    self.cors = bool(cors)
  end

  #############################################################
  # test if connected
  def connected()
    return self.cnx ? self.cnx.connected() : false
  end

  #############################################################
  # test if out buffer is empty, meaning all was sent
  def buf_out_empty()
    return size(self.buf_out) == 0
  end

  #############################################################
  # write bytes or string
  #
  # v must be bytes()
  def _write(v)
    if (size(v) == 0)   return end

    var buf_out = self.buf_out
    var buf_out_sz = size(buf_out)
    buf_out.resize(buf_out_sz + size(v))
    buf_out.setbytes(buf_out_sz, v)

    self._send()                    # try sending this now
  end

  #############################################################
  # closing web server
  def close()
    tasmota.log(f"WEB: closing cnx", 3)
    if (self.cnx != nil)    self.cnx.close()    end
    self.cnx = nil
  end

  #############################################################
  # called by fastloop
  def loop()
    if self.cnx == nil    # marked for deletion
      # mark as closed with self.cnx == nil
      tasmota.remove_fast_loop(self.fastloop_cb)
      self.fastloop_cb = nil
      return
    end

    self._send()      # try sending outgoing

    var cnx = self.cnx
    if (cnx == nil)   return      end   # it's possible that it was closed after _send()

    # any incoming data?
    if cnx.available() > 0
      var buf_in_new = cnx.read()
      if (!self.buf_in)
        self.buf_in = buf_in_new
      else
        self.buf_in += buf_in_new
      end
    end

    # parse incoming if any
    if (self.buf_in)
      self.parse()
    end
  end

  #############################################################
  # try sending what we can immediately
  def _send()
    # any data waiting to go out?
    var cnx = self.cnx
    if (cnx == nil)     return    end
    var buf_out = self.buf_out
    if size(buf_out) > 0
      if cnx.listening()
        var sent = cnx.write(buf_out)
        if sent > 0
          # we did sent something
          if sent >= size(buf_out)
            # all sent
            self.buf_out.clear()
          else
            # remove the first bytes already sent
            self.buf_out.setbytes(0, buf_out, sent)
            self.buf_out.resize(size(buf_out) - sent)
          end
        end
      end
    else
      # empty buffer, do the cleaning
      self.buf_out.clear()
      self.buf_in_offset = 0

      if self.close_after_send
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
    # tasmota.log(f"WEB: incoming {bytes().fromstring(self.buf_in).tohex()}", 3)
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
      tasmota.log(f"WEB: HTTP verb: {self.req_verb} URI: '{self.req_uri}' Version:{self.req_version}", 3)
      self.parse_http_headers()
    elif size(self.buf_in) > 100        # if no match and we still have 100 bytes, then it fails
      tasmota.log("WEB: error invalid request", 3)
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

          self.event_http_headers_end()     # no more headers
          self.phase = 2
          self.parse_http_payload()         # continue to parsing payload
        end
        if size(self.buf_in) > 1024       # we don't accept a single header larger than 1KB
          tasmota.log("WEB: error header is bigger than 1KB", 3)
          self.close()
          self.buf_in = ''
        end
        return
      end
    end


    self.close()
    self.buf_in = ''
  end

  #############################################################
  # event_http_header
  #
  # Received header
  def event_http_header(header_key, header_value)
    # tasmota.log(f"WEB: header key '{header_key}' = '{header_value}'")

    if (header_key == "Host")
      self.header_host = header_value
    end
  end

  #############################################################
  # event_http_headers_end
  #
  # All headers are received
  def event_http_headers_end()
  end

  #############################################################
  # parse incoming payload (if any)
  def parse_http_payload()
    # tasmota.log(f"WEB: parsing payload '{bytes().fromstring(self.buf_in).tohex()}'")
    # dispatch request before parsing payload
    self.server.dispatch(self, self.req_uri, self.req_verb)
  end


  #############################################################
  # Responses
  #############################################################
  # parse incoming payload (if any)
  def send_header(name, value, first)
    if first
      self.resp_headers = f"{name}: {value}\r\n{self.resp_headers}"
    else
      self.resp_headers = f"{self.resp_headers}{name}: {value}\r\n"
    end
  end

  def send(code, content_type, content)
    var response = f"HTTP/1.{self.resp_version} {code} {self.code_to_string(code)}\r\n"
    if (content_type == nil)    content_type = "text/html"    end
    self.send_header("Content-Type", content_type, true)

    # force chunked TODO
    self.send_header("Accept-Ranges", "none")
    if self.chunked
      self.send_header("Transfer-Encoding", "chunked")
    end
    # cors
    if self.cors
      self.send_header("Access-Control-Allow-Origin", "*")
      self.send_header("Access-Control-Allow-Methods", "*")
      self.send_header("Access-Control-Allow-Headers", "*")
    end
    # others
    self.send_header("Connection", "close")

    response += self.resp_headers
    response += "\r\n"
    self.resp_headers = nil

    # send
    self.write_raw(response)

    if (content)    self.write(content)   end
  end

  static def code_to_string(code)
    return _class.CODE_TO_STRING.find(code, "UNKNOWN")
  end

  #############################################################
  # async write
  def write(v)
    if type(v) == 'string'          # if string, convert to bytes
      v = bytes().fromstring(v)
    end

    # use chunk encoding
    if self.chunked
      var payload1 = self.payload1
      payload1.clear()
      payload1 .. f"{size(v):X}\r\n"
      payload1 .. v
      payload1 .. "\r\n"

      # var chunk = f"{size(v):X}\r\n{v}\r\n"
      # tasmota.log(f"WEB: sending chunk '{payload1.tohex()}'")
      self._write(payload1)
    else
      self._write(v)
    end
  end

  #############################################################
  # async write
  def write_raw(v)
    if (size(v) == 0)   return end

    if type(v) == 'string'          # if string, convert to bytes
      v = bytes().fromstring(v)
    end

    self._write(v)
  end

  
  def content_stop()
    self.write('')
    self.close_after_send = true
  end
end

#############################################################
# class Webserver_dispatcher
#############################################################
class Webserver_dispatcher
  var uri_prefix                                  # prefix string, must start with '/'
  var verb                                        # verb to match, or nil for ANY
  var cb_obj
  var cb_mth

  def init(uri, cb_obj, cb_mth, verb)
    self.uri_prefix = uri
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
        self.cb_mth(self.cb_obj, cnx, uri, verb)
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
class webserver_async
  var local_port                                  # listening port, 80 is already used by Tasmota
  var server                                      # instance of `tcpserver`
  var fastloop_cb                                 # closure used by fastloop
  var timeout                                     # default timeout for tcp connection
  var connections                                 # list of active connections
  # var timeout                                     # timeout in ms
  # var auth                                        # web authentication string (Basic Auth) or `nil`, in format `user:password` as bade64
  # var cmd                                         # GET url command
  var dispatchers
  # copied in each connection
  var chunked                                     # if true enable chunked encoding (default true)
  var cors                                        # if true send CORS headers (default false)
  #
  var payload1, payload2                          # temporary object bytes() to avoid reallocation

  static var TIMEOUT = 1000                       # default timeout: 1000ms
  static var HTTP_REQ = "^(\\w+) (\\S+) HTTP\\/(\\d\\.\\d)\r\n"
  static var HTTP_HEADER_REGEX = "([A-Za-z0-9-]+): (.*?)\r\n"       # extract a header with its 2 parts
  static var HTTP_BODY_REGEX   = "\r\n"                             # end of headers

  #############################################################
  # init
  def init(port, timeout)
    if (timeout == nil)   timeout = self.TIMEOUT    end
    self.connections = []
    self.dispatchers = []
    self.server = tcpserver(port)                 # throws an exception if port is not available
    self.chunked = true
    self.cors = false
    self.payload1 = bytes(100)              # reserve 100 bytes by default
    self.payload2 = bytes(100)              # reserve 100 bytes by default
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
      global._re_http_srv         = re.compile(self.HTTP_REQ)
      global._re_http_srv_header  = re.compile(self.HTTP_HEADER_REGEX)
      global._re_http_srv_body   = re.compile(self.HTTP_BODY_REGEX)
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
  static def bytes_format_int(b, i, default)
    b.clear()
    if (i == nil)   b .. default    return  end
    var negative = false
    # sanity check
    if (i < 0)    i = -i    negative = true   end
    if (i < 0)    return    end     # special case for MININT
    if (i == 0)   b.resize(1)   b[0] = 0x30   return  end   # return bytes("30")

    b.resize(11)    # max size for 32 bits ints '-2147483647'
    var len = 0
    while i > 0
      var digit = i % 10
      b[len] = 0x30 + digit
      len += 1
      i = (i / 10)
    end
    if negative
      b[len] = 0x2D
      len += 1
    end
    # reverse order
    b.resize(len)
    b.reverse()
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
        # tasmota.log("WEB: does not appear to be connected")
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
      var cnx = Webserver_async_cnx(self, self.server.acceptasync())
      cnx.set_chunked(self.chunked)
      cnx.set_cors(self.cors)
      self.connections.push(cnx)
    end
  end

  #############################################################
  # add to dispatcher
  def on(prefix, obj, mth, verb)
    var dispatcher = Webserver_dispatcher(prefix, obj, mth, verb)
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

return webserver_async

#- Test

var web = webserver_async(888)

def send_more(cnx, i)
  cnx.write(f"<p>Hello world {i}</p>")
  if i < 10
    tasmota.set_timer(1000, def () send_more(cnx, i+1) end)
  else
    cnx.content_stop()
  end
end

def f(obj, cnx, uri, verb)
  cnx.send(200, "text/html")
  cnx.write("<html><body>")
  send_more(cnx, 0)
  # cnx.write("Hello world")
  # cnx.content_stop()
end

web.on("/hello", nil, f)

-#
