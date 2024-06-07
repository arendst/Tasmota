#
# Matter_TCP_async.be - implements a generic async non-blocking TCP connection
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

#@ solidify:Matter_TCP_async,weak

# Status:
# nil = uninitialized
#   0 = starting    (tcp_connected == nil) - still on-going
#   1 = connected   (tcp_connected == true)
#   2 = finished    (tcp_connected == false and result in response)
#  -1 = DNS failed  (tcp_connected == false)
#  -2 = refused     (tcp_connecetd == false)
#  -3 = timeout     (tcp_connecetd == false)
#  -4 = error       communication error

class Matter_TCP_async
  var addr                                        # (string) remote address
  var port                                        # (int) remote port
  var timeout                                     # (int) timeout in ms
  var tcp                                         # instance of tcpclientasync
  var time_start                                  # (int) timestamp when starting connection
  var tcp_connected                               # (bool or nil) is tcp connected, true/false/nil (nil is in-progress)
  var status                                      # (int) status, see above
  static var TIMEOUT = 1000                       # default timeout: 1000ms

  var fast_loop                                   # (opt) either nil or a closure

  #############################################################
  # constructor
  #
  # fastloop: (opt) if 'true' uses fastloop instead of every_50ms
  def init(addr, port, timeout, fastloop)
    if timeout == nil   timeout = self.TIMEOUT  end
    if port == nil      port = 80 end
    self.addr = str(addr)
    self.port = int(port)
    self.timeout = timeout
    self.tcp = tcpclientasync()
    if fastloop
      self.fast_loop = / -> self.loop()         # create a closure to use with fastloop
    end
  end

  #############################################################
  # begin
  #
  # returns true if everything is ok, and connection started
  # returns false if DNS failed
  # returns nil if no network
  def begin()
    self.reset()

    if !tasmota.wifi()['up'] && !tasmota.eth()['up']    return nil    end       # no network
    self.time_start = tasmota.millis()
    self.status = 0

    if (self.tcp.connect(self.addr, self.port))
      if self.fast_loop
        tasmota.remove_fast_loop(self.fast_loop)
        tasmota.add_fast_loop(self.fast_loop)
      else
        tasmota.add_driver(self)
      end
      return true
    else
      log(format("BRY: failed to resolve [%s]:%i", self.addr, self.port), 3)
      self.close()
      self.status = -1
      self.tcp_connected = false
      self.event_dnsfailed()
      return false
    end
  end

  #############################################################
  # Set a new timeout, or `nil` for default (1s)
  def set_timeout(timeout)
    if timeout == nil   timeout = self.TIMEOUT  end
    self.timeout = timeout
  end
  def get_timeout()   return self.timeout       end

  #############################################################
  # Reset the instance to send a open a new connection
  # This allows to reuse the same object.
  def reset()
    self.tcp.close()
    self.tcp_connected = nil
  end

  #############################################################
  # send_http
  def close()
    self.tcp.close()
    if self.fast_loop
      tasmota.remove_fast_loop(self.fast_loop)
    else
      tasmota.remove_driver(self)
    end
    # if it was connected, send event
    if self.tcp_connected == true
      self.event_closed()
    end
    self.tcp_connected = false
  end

  def every_50ms()
    self.loop()
  end

  #############################################################
  # Main loop, check evolution and trigger events
  #############################################################
  def loop()
    var prev_connected = self.tcp_connected
    if self.status == nil   self.close()  return  end   # safeguard, this is invalid state

    self.tcp_connected = self.tcp.connected()
    # if not yet connected, check if we finally got a response
    if prev_connected == nil                          # not yet connected
      if self.tcp_connected == true
        # we finally got connection
        self.status = 1
        self.event_established()
        # and continue below
      elif self.tcp_connected == false
        # connection failed
        self.status = -2
        self.event_refused()
        self.close()
        return
      elif (tasmota.millis() - self.time_start) > self.timeout
        # connection timeout
        self.status = -3
        self.tcp_connected = false        # force to false
        self.event_timeout()
      end
    end

    if (tasmota.millis() - self.time_start) > self.timeout
      self.close()
      self.status = -3
      self.event_timeout()
      return
    end

    if self.tcp_connected == false
      if prev_connected == true
        self.event_closed()
      end
      self.close()                # close implicitly removes from loop
      return
    end

    # reading has priority because we don't want to lose data
    if self.tcp.available()
      self.event_available()
    end

    # can we send data
    if self.tcp.listening()
      self.event_listening()
    end
  end

  def write(content)
    if self.tcp_connected
      return self.tcp.write(content)
    end
    return 0
  end

  def read()
    if self.tcp_connected
      return self.tcp.read()
    end
    return nil
  end

  def readbytes()
    if self.tcp_connected
      return self.tcp.readbytes()
    end
    return nil
  end

  def available()
    if self.tcp_connected
      return self.tcp.available()
    end
    return 0
  end

  def listening()
    if self.tcp_connected
      return self.tcp.listening()
    end
    return false
  end

  #############################################################
  # Events: method should be overriden
  #############################################################
  # event_established:  connection was established
  # event_dnsfailed:    dns resolution failed
  # event_refused:      connection was refused by peer
  # event_timeout:      connection timed out
  # event_closed:       connection was normally closed (after being established)
  # event_available:   incoming data is ready to be retrieved with `read()` or `readbytes()`
  # event_listening:    outgoing buffer is empty, ready to send data with `write()`
  def event_established()     end
  def event_dnsfailed()       end
  def event_refused()         end
  def event_timeout()         end
  def event_closed()          end
  def event_available()       end
  def event_listening()       end

end
matter.TCP_async = Matter_TCP_async
