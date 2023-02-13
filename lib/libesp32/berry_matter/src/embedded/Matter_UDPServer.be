#
# Matter_UDPServer.be - implements IPv6 UDP communication for Matter
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

# Matter_UDPServer class
#
# For receiving and outgoing messages on UDP
#

import matter

#@ solidify:Matter_UDPPacket_sent,weak
#@ solidify:Matter_UDPServer,weak

#################################################################################
# Matter_UDPPacket_sent class
#
# A packet that needs to be resent if not acknowledged by the other party
#################################################################################
class Matter_UDPPacket_sent
  static var RETRY_MS = 500       # retry every 500 ms  
  static var RETRIES = 4          # retry every 500 ms  
  var raw                         # bytes() to be sent
  var addr                        # ip_address (string)
  var port                        # port (int)
  var msg_id                      # (int) message identifier that needs to be acknowledged
  var retries                     # how many retries are allowed, when `0` drop and log
  var next_try                    # timestamp (millis) when to try again

  def init(raw, addr, port, id)
    self.raw = raw
    self.addr = addr
    self.port = port
    self.msg_id = id
    self.retries = self.RETRIES
    self.next_try = tasmota.millis() + self.RETRY_MS
  end

  def send(udp_socket)
    import string
    var ok = udp_socket.send(self.addr ? self.addr : udp_socket.remote_ip, self.port ? self.port : udp_socket.remote_port, self.raw)
    if ok
      tasmota.log(string.format("MTR: sending packet to '[%s]:%i'", self.addr, self.port), 3)
    else
      tasmota.log(string.format("MTR: failed to send packet to '[%s]:%i'", self.addr, self.port), 2)
    end
  end
end
matter.UDPPacket_sent = Matter_UDPPacket_sent

#################################################################################
# Matter_UDPServer class
#
#################################################################################
class Matter_UDPServer
  static var MAX_PACKETS_READ = 4   # read at most 4 packets per tick
  var address, port                 # local address and port
  var listening                     # true if active
  var udp_socket
  var dispatch_cb                   # callback to call when a message is received
  var packets_sent                  # map of packets sent to be acknowledged

  #############################################################
  def init(address, port)
    self.address = address ? address : ""
    self.port = port ? port : 5540
    self.listening = false
    self.packets_sent = {}
  end

  #############################################################
  # start the server
  # raises an exception if something is wrong
  # registers as device handle
  #
  # `cb`: callback to call when a message is received
  def start(cb)
    if !self.listening
      self.udp_socket = udp()
      var ok = self.udp_socket.begin(self.address, self.port)
      if !ok    raise "network_error", "could not open UDP server" end
      self.listening = true
      self.dispatch_cb = cb
      tasmota.add_driver(self)
    end
  end

  #############################################################
  # stop the server
  # remove driver
  def stop()
    if self.listening
      self.udp_socket.stop()
      self.listening = false
      tasmota.remove_driver(self)
    end
  end

  #############################################################
  def every_50ms()
    import string
    var packet_read = 0
    if self.udp_socket == nil  return end
    var packet = self.udp_socket.read()
    while packet != nil
      # self.packet = packet
      packet_read += 1
      var from_addr = self.udp_socket.remote_ip
      var from_port = self.udp_socket.remote_port
      tasmota.log(string.format("MTR: UDP received from [%s]:%i", from_addr, from_port), 4)
      if self.dispatch_cb
        self.dispatch_cb(packet, from_addr, from_port)
      end
      # are we reading new packets?
      if packet_read < self.MAX_PACKETS_READ
        packet = self.udp_socket.read()
      else
        packet = nil
      end
    end
    self.resend_packets()               # resend any packet
  end

  #############################################################
  def resend_packets()
    for packet:self.packets_sent
      if tasmota.time_reached(packet.next_try)
        tasmota.log("MTR: resending packet id=" + str(packet.msg_id), 3)
        packet.send(self.udp_socket)         # resend
        packet.retries -= 1
        if packet.retries <= 0
          self.packets_sent.remove(packet.msg_id)
        else
          packet.next_try = tasmota.millis() + packet.RETRY_MS
        end
      end
    end
  end

  #############################################################
  # just received acknowledgment, remove packet from sender
  def packet_ack(id)
    if id == nil   return end
    if self.packets_sent.contains(id)
      self.packets_sent.remove(id)
      tasmota.log("MTR: removed packet from sending list id=" + str(id), 3)
    end
  end

  #############################################################
  def send_response(raw, addr, port, id)
    var packet = matter.UDPPacket_sent(raw, addr, port, id)
    packet.send(self.udp_socket)    # send
    if id
      self.packets_sent[id] = packet
    end
  end

  #############################################################
  # placeholder, nothing to run for now
  def every_second()
  end
end
matter.UDPServer = Matter_UDPServer

#-

import matter
var udps = matter.UDPServer()
udps.listen()

-#
