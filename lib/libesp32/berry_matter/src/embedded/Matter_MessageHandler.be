#
# Matter_MessageHandler.be - suppport for Matter Message handler, dispatches incoming messages and sends outgoing messages
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

#@ solidify:Matter_MessageHandler,weak

class Matter_MessageHandler
  # callback for sending messages
  var device              # `tansport.msg_send(raw:bytes() [,...]) -> bool` true if succeeded

  # handlers
  var commissioning
  var im                  # handler for Interaction Model

  #############################################################
  def init(device)
    self.device = device
    self.commissioning = matter.Commisioning_Context(self)
    self.im = matter.IM(device)
  end

  #############################################################
  # Called when a message is received
  #
  # Find or create a session for the message
  # and dispacth to appropriate handler
  #
  def msg_received(raw, addr, port)
    import string
    var ret = false

    try
      tasmota.log("MTR: MessageHandler::msg_received raw="+raw.tohex(), 4)
      var frame = matter.Frame(self, raw, addr, port)

      var ok = frame.decode_header()
      if !ok      return false end

      # do we need decryption?
      if frame.local_session_id == 0 && frame.sec_sesstype == 0
        #############################################################
        ### unencrypted session, handled by commissioning
        var session = self.device.sessions.find_session_source_id_unsecure(frame.source_node_id, 90)    # 90 seconds max
        tasmota.log("MTR: find session by source_node_id = " + str(frame.source_node_id) + " session_id = " + str(session.local_session_id), 3)
        if addr     session._ip = addr     end
        if port     session._port = port   end
        session._message_handler = self
        frame.session = session
        
        # check if it's a duplicate
        if !session._counter_insecure_rcv.validate(frame.message_counter, false)
          tasmota.log(string.format("MTR: rejected duplicate unencrypted message = %i ref = %i", frame.message_counter, session._counter_insecure_rcv.val()), 3)
          return false
        end

        if !frame.decode_payload()    return false end
        self.device.packet_ack(frame.ack_message_counter)       # acknowledge packet
        if frame.opcode != 0x10                                 # don't show `MRP_Standalone_Acknowledgement`
          var op_name = matter.get_opcode_name(frame.opcode)
          if !op_name   op_name = string.format("0x%02X", frame.opcode) end
          tasmota.log(string.format("MTR: >Received  %s from [%s]:%i", op_name, addr, port), 2)
        end
        self.commissioning.process_incoming(frame)
        return true
      else
        #############################################################
        # encrypted message
        tasmota.log(string.format("MTR: decode header: local_session_id=%i message_counter=%i", frame.local_session_id, frame.message_counter), 3)

        var session = self.device.sessions.get_session_by_local_session_id(frame.local_session_id)
        if session == nil
          tasmota.log("MTR: unknown local_session_id="+str(frame.local_session_id), 2)
          tasmota.log("MTR: frame="+matter.inspect(frame), 3)
          return false
        end
        if addr     session._ip = addr     end
        if port     session._port = port   end
        session._message_handler = self
        frame.session = session   # keep a pointer of the session in the message
       
        # check if it's a duplicate
        if !session.counter_rcv.validate(frame.message_counter, true)
          tasmota.log("MTR: rejected duplicate encrypted message = " + str(frame.message_counter) + " counter=" + str(session.counter_rcv.val()), 3)
          return false
        end
        
        var cleartext = frame.decrypt()
        if !cleartext     return false end

        # packet is good, put back content in raw
        frame.raw = frame.raw[0 .. frame.payload_idx - 1]   # remove encrypted payload
        frame.raw .. cleartext                          # add cleartext

        # continue decoding
        tasmota.log(string.format("MTR: idx=%i clear=%s", frame.payload_idx, frame.raw.tohex()), 4)
        frame.decode_payload()
        tasmota.log("MTR: decrypted message: protocol_id:"+str(frame.protocol_id)+" opcode="+str(frame.opcode)+" exchange_id="+str(frame.exchange_id & 0xFFFF), 3)

        self.device.packet_ack(frame.ack_message_counter)      # acknowledge packet

        # dispatch according to protocol_id
        var protocol_id = frame.protocol_id
        if protocol_id == 0x0000    # PROTOCOL_ID_SECURE_CHANNEL
          # it should not be encrypted
          tasmota.log("MTR: PROTOCOL_ID_SECURE_CHANNEL " + matter.inspect(frame), 3)
          if frame.opcode == 0x10                             # MRPStandaloneAcknowledgement
            ret = self.im.process_incoming_ack(frame)
            if ret
              self.im.send_enqueued(self)
            end
          end
          ret = true
        elif protocol_id == 0x0001  # PROTOCOL_ID_INTERACTION_MODEL
          # dispatch to IM Protocol Messages
          ret = self.im.process_incoming(frame)
          # if `ret` is true, we have something to send
          if ret
            self.im.send_enqueued(self)

          elif frame.x_flag_r                   # nothing to respond, check if we need a standalone ack
            var resp = frame.build_standalone_ack()
            resp.encode_frame()
            resp.encrypt()
            self.send_response(resp.raw, resp.remote_ip, resp.remote_port, resp.message_counter)
          end
          ret = true

        # -- PROTOCOL_ID_BDX is used for file transfer between devices, not used in Tasmota
        # elif protocol_id == 0x0002  # PROTOCOL_ID_BDX -- BDX not handled at all in Tasmota
        #   tasmota.log("MTR: PROTOCOL_ID_BDX not yet handled", 2)
        #   return false # ignore for now TODO
        # -- PROTOCOL_ID_USER_DIRECTED_COMMISSIONING is only used by devices, as a device we will not receive any
        # elif protocol_id == 0x0003  # PROTOCOL_ID_USER_DIRECTED_COMMISSIONING
        #   tasmota.log("MTR: PROTOCOL_ID_USER_DIRECTED_COMMISSIONING not yet handled", 2)
        #   return false # ignore for now TODO
        else
          tasmota.log("MTR: ignoring unhandled protocol_id:"+str(protocol_id), 3)
        end

      end

      return ret
    except .. as e, m
      tasmota.log("MTR: MessageHandler::msg_received exception: "+str(e)+";"+str(m))
      import debug
      debug.traceback()
      return false
    end
  end

  #############################################################
  def send_response(raw, addr, port, id)
    self.device.msg_send(raw, addr, port, id)
  end

  #############################################################
  def add_session(local_session_id, initiator_session_id, i2r, r2i, ac, created)
    import string
    # create session object
    tasmota.log(string.format("MTR: add_session local_session_id=%i initiator_session_id=%i", local_session_id, initiator_session_id), 3)
    
    var session = self.device.sessions.create_session(local_session_id, initiator_session_id)
    session.set_keys(i2r, r2i, ac, created)
  end

  #############################################################
  # placeholder, nothing to run for now
  def every_second()
    self.commissioning.every_second()
    self.im.every_second()
  end

  #############################################################
  # dispatch every 250ms click to sub-objects that need it
  def every_250ms()
    self.im.every_250ms()
  end

end
matter.MessageHandler = Matter_MessageHandler
