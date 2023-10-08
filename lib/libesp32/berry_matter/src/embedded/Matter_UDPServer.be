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
  var raw                         # bytes() to be sent
  var addr                        # ip_address (string)
  var port                        # port (int)
  var msg_id                      # (int) message identifier that needs to be acknowledged, or `nil` if no ack needed
  var exchange_id                 # (int) exchange id, to match ack
  var session_id                  # (int) session id, for logging only
  var retries                     # 0 in first attempts, goes up to RETRIES
  var next_try                    # timestamp (millis) when to try again

  def init(msg)
    # extract information from msg
    self.raw = msg.raw
    self.addr = msg.remote_ip
    self.port = msg.remote_port
    self.msg_id = msg.x_flag_r ? msg.message_counter : nil
    self.exchange_id = (msg.exchange_id != nil) ? msg.exchange_id : 0
    self.session_id = (msg.local_session_id != nil) ?  msg.local_session_id : 0
    # other information
    self.retries = 0
    self.next_try = tasmota.millis() + matter.UDPServer._backoff_time(self.retries)
  end

end
matter.UDPPacket_sent = Matter_UDPPacket_sent

#################################################################################
# Matter_UDPServer class
#
#################################################################################
class Matter_UDPServer
  static var RETRIES = 5            # 6 transmissions max (5 retries) - 1 more than spec `MRP_MAX_TRANSMISSIONS` 4.11.8 p.146
  static var MAX_PACKETS_READ = 4   # read at most 4 packets per tick
  var addr, port                    # local addr and port
  var device
  var listening                     # true if active
  var udp_socket
  var dispatch_cb                   # callback to call when a message is received
  var packets_sent                  # list map of packets sent to be acknowledged
  var loop_cb                       # closure to pass to fast_loop
  var packet                      # reuse the packer `bytes()` object at each iteration

  #############################################################
  # Init UDP Server listening to `addr` and `port` (opt).
  #
  # By default, the server listens to `""` (all addresses) and port `5540`
  def init(device, addr, port)
    self.device = device
    self.addr = addr ? addr : ""
    self.port = port ? port : 5540
    self.listening = false
    self.packets_sent = []
    self.loop_cb = def () self.loop() end
  end

  #############################################################
  # Starts the server.
  # Registers as device handler to Tasmota
  #
  # `cb(packet, from_addr, from_port)`: callback to call when a message is received.
  # Raises an exception if something is wrong.
  def start(cb)
    if !self.listening
      self.udp_socket = udp()
      var ok = self.udp_socket.begin(self.addr, self.port)
      if !ok    raise "network_error", "could not open UDP server" end
      self.listening = true
      self.dispatch_cb = cb
      # tasmota.add_driver(self)
      tasmota.add_fast_loop(self.loop_cb)
    end
  end

  #############################################################
  # Stops the server and remove driver
  def stop()
    if self.listening
      self.udp_socket.stop()
      self.listening = false
      # tasmota.remove_driver(self)
      tasmota.remove_fast_loop(self.loop_cb)
    end
  end

  #############################################################
  # At every tick:
  # Check if a packet has arrived, and dispatch to `cb`.
  # Read at most `MAX_PACKETS_READ (4) packets at each tick to
  # avoid any starvation.
  # Then resend queued outgoing packets.
  def loop()
    # import debug
    var profiler = matter.profiler
    var packet_read = 0
    if self.udp_socket == nil  return end
    var packet = self.udp_socket.read(self.packet)
    while packet != nil
      profiler.start()
      self.packet = packet      # save packet for next iteration
      packet_read += 1
      var from_addr = self.udp_socket.remote_ip
      var from_port = self.udp_socket.remote_port
      if tasmota.loglevel(4)
        tasmota.log(format("MTR: UDP received from [%s]:%i", from_addr, from_port), 4)
      end
      # tasmota.log("MTR: Perf/UDP_received = " + str(debug.counters()), 4)
      if self.dispatch_cb
        profiler.log("udp_loop_dispatch")
        self.dispatch_cb(packet, from_addr, from_port)
      end
      profiler.dump(2)
      # are we reading new packets?
      if packet_read < self.MAX_PACKETS_READ
        packet = self.udp_socket.read()
      else
        packet = nil
      end
    end
    self._resend_packets()               # resend any packet
  end

  def every_50ms()
    self.loop()
  end
  #############################################################
  # Send packet now.
  #
  # Returns `true` if packet was successfully sent.
  def send(packet)
    var ok = self.udp_socket.send(packet.addr ? packet.addr : self.udp_socket.remote_ip, packet.port ? packet.port : self.udp_socket.remote_port, packet.raw)
    
    if ok
      if tasmota.loglevel(4)
        tasmota.log(format("MTR: sending packet to '[%s]:%i'", packet.addr, packet.port), 4)
      end
    else
      if tasmota.loglevel(3)
        tasmota.log(format("MTR: error sending packet to '[%s]:%i'", packet.addr, packet.port), 3)
      end
    end
    return ok
  end

  #############################################################
  # Resend packets if they have not been acknowledged by receiver
  # either with direct Ack packet or ack embedded in another packet.
  # Packets with `id`=`nil` are not resent.
  # <BR>
  # Packets are re-sent at most `RETRIES` (4) times, i.e. sent maximum 5 times.
  # Exponential backoff is added after each resending.
  # <BR>
  # If all retries expired, remove packet and log.
  def _resend_packets()
    var idx = 0
    while idx < size(self.packets_sent)
      var packet = self.packets_sent[idx]
      if tasmota.time_reached(packet.next_try)
        if packet.retries <= self.RETRIES
          tasmota.log("MTR: .          Resending packet id=" + str(packet.msg_id), 4)
          self.send(packet)
          packet.next_try = tasmota.millis() + self._backoff_time(packet.retries)
          packet.retries += 1
          idx += 1
        else
          self.packets_sent.remove(idx)
          tasmota.log(format("MTR: .          (%6i) Unacked packet '[%s]:%i' msg_id=%i", packet.session_id, packet.addr, packet.port, packet.msg_id), 3)
        end
      else
        idx += 1
      end
    end
  end

  #############################################################
  # Just received acknowledgment, remove packet from sender
  def received_ack(msg)
    var id = msg.ack_message_counter
    var exch = msg.exchange_id
    if id == nil   return end
    # tasmota.log("MTR: receveived ACK id="+str(id), 4)
    var idx = 0
    while idx < size(self.packets_sent)
      var packet = self.packets_sent[idx]
      if packet.msg_id == id && packet.exchange_id == exch
        self.packets_sent.remove(idx)
        if tasmota.loglevel(4)
          tasmota.log("MTR: .          Removed packet from sending list id=" + str(id), 4)
        end
      else
        idx += 1
      end
    end
  end

  #############################################################
  # Send a packet, enqueue it if `id` is not `nil`
  def send_UDP(msg)
    var packet = matter.UDPPacket_sent(msg)
    self.send(packet)
    if packet.msg_id
      self.packets_sent.push(packet)
    end
  end

  #############################################################
  # placeholder, nothing to run for now
  def every_second()
  end

  #############################################################
  # Compute exponential backoff as per 4.11.2.1 p.137
  static def _backoff_time(n)
    def power_int(v, n)
      var r = 1
      while n > 0
        r *= v
        n -= 1
      end
      return r
    end

    import math
    var i = 300                                   # SLEEPY_ACTIVE_INTERVAL
    var rand = real(math.rand() & 0xFF) / 255     # 0..1 with reasonable granularity
    var n_power = n > 0 ? n - 1 : 0
    var mrpBackoffTime = i * power_int(1.6, n_power) * (1.0 + rand * 0.25 )
    return int(mrpBackoffTime)
  end
end
matter.UDPServer = Matter_UDPServer
