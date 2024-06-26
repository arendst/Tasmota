#
# Matter_Message.be - suppport for Matter Message Packet/Frame structure, deconding, enconding, encryption & decryption
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

#@ solidify:Matter_Frame,weak

# Matter frame format, see 4.4 (p. 107)
class Matter_Frame
  # context information
  var message_handler             # the message handler instance that decodes and generate responses
  var session                     # the session object associated, or `nil` if no session
  # content of the message
  var raw                         # raw bytes of the packet
  var payload_idx                 # index in raw for beginning of payload
  # Message header
  var flags
  var flag_s                      # Note: Matter booleans are 1/0 not true/false
  var flag_dsiz
  var local_session_id
  var sec_flags
  var sec_p
  var sec_c
  var sec_mx
  var sec_sesstype
  var message_counter
  var source_node_id
  var dest_node_id_2              # if 2 bytes
  var dest_node_id_8              # if 8 bytes
  # Message payload
  var x_flags
  var x_flag_v
  var x_flag_sx
  var x_flag_r
  var x_flag_a
  var x_flag_i
  var opcode
  var exchange_id                 # exchange_id is 16 bits unsigned, we set bit 16 if it's an id generated locally
  var protocol_id
  var vendor_id                   # (opt)
  var ack_message_counter         # (opt)
  var sec_extensions
  # var
  var app_payload_idx             # index where the application payload starts
  # for UDP, remote_ip and remote_port to send back to
  var remote_ip, remote_port

  #############################################################
  # keep track of the message_handler object
  #
  def init(message_handler, raw, addr, port)
    self.message_handler = message_handler
    self.raw = raw
    self.remote_ip = addr
    self.remote_port = port
  end

  #############################################################
  # decode the header of the message and provide session
  # information if decryption is required
  #
  # returns `true` if ok, `false` if message needs to be ignored
  # or raises an exception if something went wrong
  def decode_header()
    var idx = 0
    var raw = self.raw
    # flags
    self.flags = raw.get(0, 1)
    if (self.flags & 0xF8) != 0x00    return false end      # ignore if Matter version if not 1.0
    self.flag_s = raw.getbits(2, 1)
    self.flag_dsiz = raw.getbits(0, 2)
    if self.flag_dsiz == 3            return false end      # ignore if dsiz is unknown

    # security flagse
    self.sec_flags = raw.get(3, 1)
    self.sec_p = raw.getbits(3*8 + 7, 1)
    self.sec_c = raw.getbits(3*8 + 6, 1)
    self.sec_mx = raw.getbits(3*8 + 5, 1)
    self.sec_sesstype = raw.getbits(3*8, 2)
    if self.sec_sesstype > 1          return false end      # ignore if session type is unknown

    #
    self.local_session_id = raw.get(1, 2)
    self.message_counter = raw.get(4, 4)
    idx += 8
    # source node id?
    if self.flag_s
      self.source_node_id = raw[idx .. idx+7]
      idx += 8
    end
    # dest node id?
    if self.flag_dsiz == 1
      self.dest_node_id_8 = raw[idx .. idx+7]
      idx += 8
    elif self.flag_dsiz == 2
      self.dest_node_id_2 = raw.get(idx, 2)
      idx += 2
    end

    # skip MX if any
    if self.sec_mx
      var sz = raw.get(idx, 2)
      idx += sz + 2             # ignore bytes
    end

    self.payload_idx = idx
    return true
  end

  #############################################################
  def decode_payload()
    var idx = self.payload_idx      # retrieve index for payload
    var raw = self.raw

    # Message paylod
    self.x_flags = raw.get(idx, 1)
    # flags
    self.x_flag_v = raw.getbits(idx*8 + 4, 1)
    self.x_flag_sx = raw.getbits(idx*8 + 3, 1)
    self.x_flag_r = raw.getbits(idx*8 + 2, 1)
    self.x_flag_a = raw.getbits(idx*8 + 1, 1)
    self.x_flag_i = raw.getbits(idx*8, 1)

    self.opcode = raw.get(idx+1, 1)
    self.exchange_id = raw.get(idx+2, 2)
    if !self.x_flag_i    self.exchange_id |= 0x10000  end      # special encoding for local exchange_id
    self.protocol_id = raw.get(idx+4, 2)
    idx += 6

    # optional protocol vendor id
    if self.x_flag_v
      self.vendor_id = raw.get(idx, 2)
      idx += 2
    end
    # optional Acknowledged Message Counter (32 bits)
    if self.x_flag_a
      self.ack_message_counter = raw.get(idx, 4)
      idx += 4
    end
    # optional secure extensions
    if self.x_flag_sx
      var sz = raw.get(idx, 2)
      idx += sz + 2                   # skip
    end

    self.app_payload_idx = idx
    return self
  end

  #############################################################
  # Encode the packet as raw bytes
  #
  # If encryption is needed, it is done later
  #
  # Header is built from attributes
  # `payload` is a bytes() buffer for the app payload
  #
  # you can pass a `raw` bytes() object to be used
  def encode_frame(payload, raw)
    if raw == nil
      raw = bytes(16 + (payload ? size(payload) : 0))
    end
    # compute flags
    if self.flags == nil
      self.flags = 0x00
      if self.flag_s      self.flags = self.flags | 0x04 end
      if self.flag_dsiz   self.flags = self.flags | (self.flag_dsiz & 0x03) end
    end
    raw.add(self.flags, 1)
    # local_session_id (mandatory)
    raw.add(self.local_session_id ? self.local_session_id : 0, 2)
    # compute security flags
    if self.sec_flags == nil
      self.sec_flags = 0x00
      if self.sec_p         self.sec_flags = self.sec_flags | 0x80 end
      if self.sec_c         self.sec_flags = self.sec_flags | 0x40 end
      # if self.sec_mx      self.sec_flags = self.sec_flags | 0x20 end    # not supportedd
      if self.sec_sesstype  self.sec_flags = self.sec_flags | (self.sec_sesstype & 0x03) end
    end
    raw.add(self.sec_flags, 1)
    #
    raw.add(self.message_counter, 4)    # mandatory
    #
    if self.flag_s          raw .. self.source_node_id end        # 8 bytes
    if self.flag_dsiz == 0x01 raw .. self.dest_node_id_8 end      # 8 bytes
    if self.flag_dsiz == 0x02 raw.add(self.dest_node_id_2, 2) end # 2 bytes
    # skip message extensions
    self.payload_idx = size(raw)
    # Message payload
    if self.x_flags == nil
      self.x_flags = 0x00
      if self.x_flag_v          self.x_flags = self.x_flags | 0x10 end
      # if self.x_flag_sx         self.x_flags = self.x_flags | 0x08 end  # not implemented
      if self.x_flag_r          self.x_flags = self.x_flags | 0x04 end
      if self.x_flag_a          self.x_flags = self.x_flags | 0x02 end
      if self.x_flag_i          self.x_flags = self.x_flags | 0x01 end
    end
    raw.add(self.x_flags, 1)
    # opcode (mandatory)
    raw.add(self.opcode, 1)
    raw.add((self.exchange_id != nil) ? self.exchange_id & 0xFFFF : 0, 2)
    raw.add(self.protocol_id, 2)
    if self.x_flag_a    raw.add(self.ack_message_counter, 4) end
    # finally payload
    self.app_payload_idx = size(raw)
    if payload
      raw .. payload
    end

    self.raw = raw
    return raw
  end

  #############################################################
  # Generate a Standalone Acknowledgment
  # Uses `PROTOCOL_ID_SECURE_CHANNEL` no ecnryption required
  def build_standalone_ack(reliable)
    # send back response
    var resp = classof(self)(self.message_handler)

    resp.remote_ip = self.remote_ip
    resp.remote_port = self.remote_port

    if self.flag_s
      resp.flag_dsiz = 0x01
      resp.dest_node_id_8 = self.source_node_id
    else
      resp.flag_dsiz = 0x00
    end
    resp.session = self.session         # also copy the session object
    # message counter
    resp.message_counter = self.session.counter_snd_next()
    resp.local_session_id = self.session.initiator_session_id
      
    resp.x_flag_i = (self.x_flag_i ? 0 : 1)     # invert the initiator flag
    resp.opcode = 0x10          # MRP Standalone Acknowledgement
    resp.exchange_id = self.exchange_id
    resp.protocol_id = 0                # PROTOCOL_ID_SECURE_CHANNEL
    resp.x_flag_a = 1           # ACK of previous message
    resp.ack_message_counter = self.message_counter
    resp.x_flag_r = reliable ? 1 : 0
    return resp
  end

  #############################################################
  # Generate response to message with default parameter
  # does not handle encryption which is done in a later step
  #
  # if 'resp' is not nil, update frame
  def build_response(opcode, reliable, resp)
    # send back response
    if resp == nil
      resp = classof(self)(self.message_handler)
    end

    resp.remote_ip = self.remote_ip
    resp.remote_port = self.remote_port

    if self.flag_s
      resp.flag_dsiz = 0x01
      resp.dest_node_id_8 = self.source_node_id
    else
      resp.flag_dsiz = 0x00
    end
    resp.session = self.session         # also copy the session object
    # message counter
    # if self.session && self.session.initiator_session_id != 0
    if self.local_session_id != 0 && self.session && self.session.initiator_session_id != 0
      resp.message_counter = self.session.counter_snd_next()
      resp.local_session_id = self.session.initiator_session_id
    else
      resp.message_counter = self.session._counter_insecure_snd.next()
      resp.local_session_id = 0
    end
      
    resp.x_flag_i = (self.x_flag_i ? 0 : 1)     # invert the initiator flag
    resp.opcode = opcode
    resp.exchange_id = self.exchange_id
    resp.protocol_id = self.protocol_id
    if self.x_flag_r
      resp.x_flag_a = 1           # ACK of previous message
      resp.ack_message_counter = self.message_counter
    end
    resp.x_flag_r = reliable ? 1 : 0

    if resp.local_session_id == 0
      var op_name = matter.get_opcode_name(resp.opcode)
      if !op_name   op_name = format("0x%02X", resp.opcode) end
      log(format("MTR: <Replied   (%6i) %s", resp.session.local_session_id, op_name), 3)
    end
    return resp
  end

  #############################################################
  # Generate a message - we are the initiator
 #
  # if 'resp' is not nil, update frame
  static def initiate_response(message_handler, session, opcode, reliable, resp)
    # send back response
    if resp == nil
      resp = matter.Frame(message_handler)
    end

    resp.remote_ip = session._ip
    resp.remote_port = session._port

    resp.flag_dsiz = 0x00
    resp.session = session         # also copy the session object
    # message counter
    if session && session.initiator_session_id != 0
      resp.message_counter = session.counter_snd_next()
      resp.local_session_id = session.initiator_session_id
    else
      resp.message_counter = session._counter_insecure_snd.next()
      resp.local_session_id = 0
    end
      
    resp.x_flag_i = 1                                     # we are the initiator
    resp.opcode = opcode
    session._exchange_id += 1                            # move to next exchange_id
    resp.exchange_id = session._exchange_id | 0x10000    # special encoding for local exchange_id
    resp.protocol_id = 0x0001                             # PROTOCOL_ID_INTERACTION_MODEL
    resp.x_flag_r = reliable ? 1 : 0

    return resp
  end


  #############################################################
  # decrypt with I2S key
  # return cleartext or `nil` if failed
  #
  # frame.raw is decrypted in-place and the MIC is removed
  # returns true if successful
  def decrypt()
    import crypto
    var session = self.session
    var raw = self.raw
    var payload_idx = self.payload_idx
    var tag_len = 16

    # decrypt the message with `i2r` key
    var i2r = session.get_i2r()

    # check privacy flag, p.127
    if self.sec_p
      # compute privacy key, p.71
      log("MTR: >>>>>>>>>>>>>>>>>>>> Compute Privacy TODO", 2)
      var k = session.get_i2r_privacy()
      var mic = raw[-16..]      # take last 16 bytes as signature
      var n = bytes().add(self.local_session_id, -2) + mic[5..15]   # session in Big Endian
      var m = self.raw[4 .. self.payload_idx-1]
      var m_clear = crypto.AES_CTR(k).decrypt(m, n, 2)
      # replace in-place
      self.raw = self.raw[0..3] + m_clear + m[self.payload_idx .. ]
    end

    # recompute nonce
    var n = self.message_handler._n_bytes     # use cached bytes() object to avoid allocation
    n.clear()
    n.add(self.flags, 1)
    n.add(self.message_counter, 4)
    if self.source_node_id
      n .. self.source_node_id
    else
      if session.peer_node_id
        n .. session.peer_node_id
      end
      n.resize(13)        # add zeros
    end

    # log("MTR: ******************************", 4)
    # log("MTR: raw         =" + raw.tohex(), 4)
    # log("MTR: i2r         =" + i2r.tohex(), 4)
    # log("MTR: p           =" + raw[payload_idx .. -17].tohex(), 4)
    # log("MTR: a           =" + raw[0 .. payload_idx - 1].tohex(), 4)
    # log("MTR: n           =" + n.tohex(), 4)
    # log("MTR: mic         =" + raw[-16..].tohex(), 4)

    # decrypt
    var ret = crypto.AES_CCM.decrypt1(i2r,                    # secret key
                                      n, 0, size(n),          # nonce / IV
                                      raw, 0, payload_idx,    # aad
                                      raw, payload_idx, size(raw) - payload_idx - tag_len,  # encrypted - decrypted in-place
                                      raw, size(raw) - tag_len, tag_len)  # MIC
    if ret
      # succcess
      raw.resize(size(raw) - tag_len)   # remove MIC
    else
      log("MTR: rejected packet due to invalid MIC", 3)
    end
    return ret
  end

  #############################################################
  # encrypt with R2I key
  # return true if ok
  # changes the raw buffer in-place
  def encrypt()
    import crypto
    var raw = self.raw
    var session = self.session
    var payload_idx = self.payload_idx
    var tag_len = 16

    # encrypt the message with `i2r` key
    var r2i = session.get_r2i()

    # recompute nonce
    var n = self.message_handler._n_bytes     # use cached bytes() object to avoid allocation
    n.clear()
    n.add(self.flags, 1)
    n.add(self.message_counter, 4)
    if session.is_CASE() && session.get_device_id()
      n .. session.get_device_id()
    end
    n.resize(13)        # add zeros

    # encrypt
    raw.resize(size(raw) + tag_len)   # make room for MIC
    var ret = crypto.AES_CCM.encrypt1(r2i,                    # secret key
                                      n, 0, size(n),          # nonce / IV
                                      raw, 0, payload_idx,    # aad
                                      raw, payload_idx, size(raw) - payload_idx - tag_len,  # encrypted - decrypted in-place
                                      raw, size(raw) - tag_len, tag_len)  # MIC

  end

  #############################################################
  # compute the node_id for this message, via session and fabric
  #
  # returns bytes(8)
  def get_node_id()
    return self.session ? self.session.get_node_id() : nil
  end

  #############################################################
  # Decode a message we are about to send, to ease debug
  def debug(raw)
    return      # disable logging for now
    var r = matter.Frame(self.message_handler, raw)
    r.decode_header()
    r.decode_payload()
    # log("MTR: sending decode: " + matter.inspect(r), 4)
  end
end
matter.Frame = Matter_Frame
