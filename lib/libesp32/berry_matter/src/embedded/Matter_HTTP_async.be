#
# Matter_HTTP_async.be - implements a generic async non-blocking TCP connection
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

import matter

#@ solidify:Matter_HTTP_async,weak

# dummy declaration for solidification
class Matter_TCP_async end

class Matter_HTTP_async : Matter_TCP_async
  # var addr                                        # remote address
  # var port                                        # remote port
  # var timeout                                     # timeout in ms
  # var tcp                                         # instance of tcpclientasync
  # var time_start                                  # timestamp when starting connection
  # var tcp_connected                               # is tcp connected, true/false/nil (nil is in-progress)
  var cmd                                         # GET url command
  var response                                    # aggrated response
  var response_offset                             # offset to parse in response
  var status_code                                 # status code looking for 200
  var payload                                     # actual payload of the response (string)
  # var status                                      # status, see above
  var http_status                                 # status: 0/ in progress, 1/ finished ok, -1/ failed, -2/ timeout
  # static var TIMEOUT = 1000                       # default timeout: 500ms
  var phase                                       # parsing phase: 0/ status line, 1/ headers, 2/ payload chunked
  var is_chunked                                  # true if the content is chunked
  var chunk_size                                  # nil or int, size of the current chunk
  static var HTTP_GET = "GET %s HTTP/1.1\r\nHost %s:%s\r\nConnection: close\r\n\r\n"    # see https://stackoverflow.com/questions/6686261/what-at-the-bare-minimum-is-required-for-an-http-request

  static var HTTP_STATUS_REGEX = "HTTP/1\\.[0-1] (\\d+) .*?\r\n"    # extract stattus code from first line
  static var HTTP_HEADER_REGEX = "([A-Za-z0-9-]+): (.*?)\r\n"       # extract a header with its 2 parts
  static var HTTP_BODY_REGEX   = "\r\n"                             # end of headers
  static var HTTP_CHUNK_REGEX   = "\r\n([A-Fa-f0-9]+)[ \t]*.*?\r\n"              # extract the length of a chunk
  static var SPINLOCK = 5                         # delay in ms for spinlock

  #############################################################
  # init
  def init(addr, port, timeout, fastloop)
    super(self).init(addr, port, timeout, fastloop)
    self.compile_re()
  end

  #############################################################
  # compile once for all the regex
  def compile_re()
    import re
    if !global.contains("_re_http_status")
      global._re_http_status = re.compile(self.HTTP_STATUS_REGEX)
      global._re_http_header = re.compile(self.HTTP_HEADER_REGEX)
      global._re_http_body   = re.compile(self.HTTP_BODY_REGEX)
      global._re_http_chunk  = re.compile(self.HTTP_CHUNK_REGEX)
    end
  end

  #############################################################
  # begin
  #
  # returns true if everything is ok, and connection started
  # returns false if DNS failed
  # returns nil if no network
  def begin(cmd)
    var ret = super(self).begin()
    self.cmd = cmd
    return ret
  end

  #############################################################
  # begin_sync
  #
  # Synchronous (blocking version)
  #
  # returns nil if something went wrong
  # returns the payload as string
  def begin_sync(cmd, timeout)
    var timeout_old = self.timeout
    if timeout != nil   self.set_timeout(timeout)    end
    var payload = nil
    var spinlock = self.SPINLOCK

    # try
    var ret = self.begin(cmd)
    if ret      # true just means that DNS was ok and that nothing went wrong
      while self.http_status == 0
        self.loop()
        tasmota.delay(spinlock)
      end
    end

    self.set_timeout(timeout_old)
    return self.http_status > 0 ? self.payload : nil
  end

  def reset()
    super(self).reset()
    # free buffers
    self.cmd = nil
    self.response = nil
    self.response_offset = 0
    self.payload = ""
    self.phase = 0
    self.http_status = 0
    self.is_chunked = false
    self.chunk_size = nil
  end

  #############################################################
  # parse_http_response
  #
  # Parse incoming HTTP response from self.response
  def parse_http_response()
    if self.phase == 0
      self.parse_http_status_line()
    elif self.phase == 1
      self.parse_http_headers()
    elif self.phase == 2
      self.parse_http_payload()
    end
    # if phase == 3 (payload raw) we don't parse anything and work for tcp connection to be closed
  end

  #############################################################
  # parse_http_status_line
  #
  # Parse incoming HTTP status line
  def parse_http_status_line()
    var m = global._re_http_status.match2(self.response, self.response_offset)
    # Ex: [17, '200']
    if m
      self.response_offset = m[0]         # move to following bytes
      self.status_code = int(m[1])
      self.phase = 1                      # proceed to parsing headers
      self.parse_http_headers()
    elif size(self.response) > 100        # if no match and we still have 100 bytes, then it fails
      self.close()
      self.status = -4
    end
  end

  #############################################################
  # parse_http_status_line
  #
  # Parse incoming HTTP status line
  def parse_http_headers()
    while true
      # print("parse_http_headers", "self.response_offset=", self.response_offset)
      var m = global._re_http_header.match2(self.response, self.response_offset)
      # print("m=", m)
      # Ex: [32, 'Content-Type', 'application/json']
      if m
        self.event_http_header(m[1], m[2])
        self.response_offset += m[0]
      else  # no more headers
        var m2 = global._re_http_body.match2(self.response, self.response_offset)
        if m2
          # end of headers
          # we keep \r\n which is used by pattern
          self.event_http_headers_end()     # no more headers
          self.phase = 2
          self.parse_http_payload()         # continue to parsing payload
        end
        if size(self.response) > 1024       # we don't accept a single header larger than 1KB
          self.close()
          self.status = -4
          self.http_status = -1
          self.event_http_failed()
        end
        return
      end
    end
  end

  #############################################################
  # parse_http_status_line
  #
  # Parse incoming HTTP status line
  def parse_http_payload()
    if self.is_chunked
      while true
        # if no current chunk size
        if self.chunk_size == nil
          var m = global._re_http_chunk.match2(self.response, self.response_offset)
          if m
            self.response_offset += m[0]
            self.chunk_size = int('0x'+m[1])
            # if chunk size is zero, finished
            if self.chunk_size == 0
              self.status = 2   # finished
              self.response = ''    # free space
              self.response_offset = 0
              self.close()
              # if self.http_status == 0    # this is now handled by the close event
              #   self.http_status = 1
              #   self.event_http_finished()
              # end
              return
            end
          end
        end
        # do we have a chunk size
        if self.chunk_size != nil
          # print("chunk_size", self.chunk_size, size(self.response) - self.response_offset)
          if self.chunk_size <= size(self.response) - self.response_offset
            # we have a complete chunk in the buffer
            self.payload += self.response[self.response_offset .. self.response_offset + self.chunk_size - 1]
            self.response = self.response[self.response_offset + self.chunk_size .. ]    # truncate to save space
            self.response_offset = 0
            self.chunk_size = nil
            # print(bytes().fromstring(self.response).tohex())
          else
            return          # return until more data is received
          end
        else
          return
        end
      end
    else
      # non-chunked
      self.payload += self.response[self.response_offset..]
      self.response = ""
      self.response_offset = 0
    end
  end

  #############################################################
  # send_http
  #
  # Send http request
  def send_http()
    import string
    # "GET %s HTTP/1.1\r\nHost %s:%s\r\nConnection: close\r\n\r\n"
    self.response = ""

    # special formatting for IPv6
    var addr = self.addr
    if string.find(addr, ':') >= 0
      addr = "[" + addr + "]"       # IPv6 must be enclosed in brakets
    end

    var req = format(self.HTTP_GET, self.cmd, addr, self.port)
    var ret = self.write(req)
    if ret != size(req)
      # print("Could not send","size=",size(req),"ret=",ret)
      self.close()
      self.status = -4
      self.http_status = -1
      self.event_http_failed()
    end
  end


  #############################################################
  # Events: method should be overriden
  #############################################################
  #   From TCP
  # event_established:  connection was established
  # event_dnsfailed:    dns resolution failed
  # event_refused:      connection was refused by peer
  # event_timeout:      connection timed out
  # event_closed:       connection was normally closed (after being established)
  # event_available:   incoming data is ready to be retrieved with `read()` or `readbytes()`
  # event_listening:    outgoing buffer is empty, ready to send data with `write()`
  #   From HTTP
  # event_http_status_code: received HTTP status code (stored in `status_code`)
  # event_http_header:      received HTTP header (not stored except `is_chunked`)
  # event_http_headers_end: all HTTP headers received
  #   Always finishes with one of:
  # event_http_finished:    HTTP finished, result is in `payload`
  # event_http_failed:      HTTP connection failed (not a timeout)
  # event_http_timeout:     HTTP timeout

  #############################################################
  # event_established
  #
  # When connection is established, we send the GET request in
  # a single write, and we don't need to send anything more
  def event_established()
    self.send_http()
  end

  #############################################################
  # event_available
  #
  # Data received
  def event_available()
    self.receive()
  end

  #############################################################
  # event_http_status_code
  #
  # Received status_code
  def event_http_status_code(status_code, status_message)
    self.status_code = status_code
  end

  #############################################################
  # event_http_header
  #
  # Received header
  def event_http_header(header_key, header_value)
    import string
    header_key = string.tolower(header_key)
    header_value = string.tolower(header_value)
    # print("header=", header_key, header_value)
    if   header_key == 'transfer-encoding' && string.tolower(header_value) == 'chunked'
      self.is_chunked = true
    end
  end

  #############################################################
  # event_http_headers_end
  #
  # All headers are received
  def event_http_headers_end()
    # print("event_http_headers_end")
    # truncate to save space
    if self.response_offset > 0
      self.response = self.response[self.response_offset .. ]
      self.response_offset = 0
    end
  end

  #############################################################
  # receive
  #
  # Receive and collate
  def receive()
    if self.tcp_connected != true     return    end      # not connected

    if (tasmota.millis() - self.time_start) > self.timeout
      # connection timeout
      # print("Connected timeout in", tasmota.millis() - self.time_start, "ms")
      self.status = -3
      self.close()
      self.http_status = -2
      self.event_http_timeout()
    else

      var avail = self.tcp.available()
      var new_data = 0
      while avail > 0
        var s = self.tcp.read()
        # print("read size=", size(s), "avail=", avail, "in", tasmota.millis() - self.time_start, "ms")
        self.response += s
        new_data += size(s)
        avail = self.tcp.available()
      end

      if new_data > 0
        # print("read size=", new_data, "in", tasmota.millis() - self.time_start, "ms")
        self.parse_http_response()          # try to parse response
      end

    end
  end

  #############################################################
  # event_http_finished
  #
  # Transaction finished sucessfully, result in self.payload
  def event_http_finished()   end
  def event_closed()
    if self.http_status == 0
      self.http_status = 1
      self.event_http_finished()
    end
  end

  #############################################################
  # event_http_failed
  #
  # Transaction finished sucessfully, result in self.payload
  def event_http_failed()     end
  def event_refused()         self.http_status = -1   self.event_http_failed()  end
  #############################################################
  # event_http_timeout
  #
  # Transaction finished sucessfully, result in self.payload
  def event_http_timeout()    end
  def event_timeout()         self.http_status = -2   self.event_http_timeout()  end

end
matter.HTTP_async = Matter_HTTP_async

#-

# Example

tcp = matter.HTTP_async("192.168.2.200", 80, 1000)
tcp.begin("/cm?cmnd=Status%208")
tcp.begin("/")

# Synchronous examples
tcp = matter.HTTP_async("192.168.2.200", 80, 1000)
print(tcp.begin_sync("/cm?cmnd=Status%208", 500))
print(tcp.begin_sync("/", 500))

-#
