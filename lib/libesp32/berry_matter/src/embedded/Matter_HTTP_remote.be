#
# Matter_HTTP_remote.be - implements an interface to query remotely Tasmota device via HTTP
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

#@ solidify:Matter_HTTP_remote,weak

# dummy declaration for solidification
class Matter_HTTP_async end

class Matter_HTTP_remote : Matter_HTTP_async
  var cb                                            # call cb(http_status, payload)

  #############################################################
  # init
  def init(addr, port, timeout, fastloop)
    super(self).init(addr, port, timeout, fastloop)
  end

  def set_cb(cb)
    self.cb = cb
  end

  #############################################################
  # begin
  #
  # returns true if everything is ok, and connection started
  # returns false if DNS failed
  # returns nil if no network
  def begin(cmd)
    import string
    tasmota.log(string.format("MTR: HTTP async request 'http://%s:%i/%s'", self.addr, self.port, cmd), 3)
    return super(self).begin(cmd)
  end

  #############################################################
  # begin_sync
  #
  # Synchronous (blocking version)
  #
  # returns nil if something went wrong
  # returns the payload as string
  def begin_sync(cmd, timeout)
    import string
    tasmota.log(string.format("MTR: HTTP sync request 'http://%s:%i/%s'", self.addr, self.port, cmd), 3)
    return super(self).begin_sync(cmd, timeout)
  end

  def event_http_finished()
    import string
    var payload_short = (self.payload != nil) ? self.payload : 'nil'
    if size(payload_short) > 30   payload_short = payload_short[0..29] + '...'   end
    tasmota.log(string.format("MTR: HTTP response in %i ms: '%s'", tasmota.millis() - self.time_start, payload_short), 3)

    if self.cb
      self.cb(self.http_status, self.payload)
    end
  end
  def event_http_failed()
    tasmota.log("MTR: HTTP failed", 3)
    if self.cb
      self.cb(self.http_status, nil)
    end
  end
  def event_http_timeout()
    tasmota.log("MTR: HTTP timeout", 3)
    if self.cb
      self.cb(self.http_status, nil)
    end
  end

end
matter.HTTP_remote = Matter_HTTP_remote
