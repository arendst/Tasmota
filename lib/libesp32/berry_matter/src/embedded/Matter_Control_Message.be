#
# Matter_Control_Message.be - suppport for Matter Control Messages (flag C = 1)
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

#@ solidify:Matter_Control_Message,weak

#################################################################################
# Class Matter_Control_Message
#
# Control message have C flag = 1
# Used primarily for Message Counter Synchronization Protocol (MCSP)
#################################################################################
class Matter_Control_Message
  var responder                       # reference to the caller, sending packets
  var device                          # root device object

  def init(responder)
    import crypto
    self.responder = responder
    self.device = responder.device
  end

  def process_incoming_control_message(msg)

    log("MTR: received control message " + matter.inspect(msg), 3)
    if   msg.opcode == 0x00
      return self.parse_MsgCounterSyncReq(msg)
    elif msg.opcode == 0x01
      return self.parse_MsgCounterSyncRsp(msg)
    else
      log(format("MTR: >????????? Unknown OpCode (control message) %02X", msg.opcode), 2)
      return false
    end

    return false
  end

  #############################################################
  # MsgCounterSyncReq
  #
  # Not yet implemented
  def parse_MsgCounterSyncReq(msg)
    var session = msg.session
    log(format("MTR: >MCSyncReq * Not implemented %s", msg.raw[msg.app_payload_idx..].tohex()), 2)
    return false      # we don't explicitly ack the message
  end

  #############################################################
  # MsgCounterSyncRsp
  #
  # Not yet implemented
  def parse_MsgCounterSyncRsp(msg)
    var session = msg.session
    log(format("MTR: >MCSyncRsp * Not implemented %s", msg.raw[msg.app_payload_idx..].tohex()), 2)
    return false      # we don't explicitly ack the message
  end

end
matter.Control_Message = Matter_Control_Message
