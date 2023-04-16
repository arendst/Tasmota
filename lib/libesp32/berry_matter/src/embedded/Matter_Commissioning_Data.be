#
# Matter_Commissioning_Data.be - suppport for Matter Commissioning messages structure
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

#@ solidify:Matter_PBKDFParamRequest,weak
#@ solidify:Matter_PBKDFParamResponse,weak
#@ solidify:Matter_Pake1,weak
#@ solidify:Matter_Pake2,weak
#@ solidify:Matter_Pake3,weak
#@ solidify:Matter_Sigma1,weak
#@ solidify:Matter_Sigma2,weak
#@ solidify:Matter_Sigma2Resume,weak
#@ solidify:Matter_Sigma3,weak

#################################################################################
# class PBKDFParamRequest encaspulating pbkdfparamreq-struct (p. 151)
#################################################################################
class Matter_PBKDFParamRequest
  var initiatorRandom
  var initiator_session_id
  var passcodeId
  var hasPBKDFParameters
  var SLEEPY_IDLE_INTERVAL
  var SLEEPY_ACTIVE_INTERVAL

  def parse(b, idx)
    if idx == nil    idx = 0 end
    var val = matter.TLV.parse(b, idx)

    self.initiatorRandom = val.getsubval(1)
    self.initiator_session_id = val.getsubval(2)
    self.passcodeId = val.getsubval(3)
    self.hasPBKDFParameters = val.getsubval(4)
    var initiatorSEDParams = val.findsub(5)
    if initiatorSEDParams != nil
      self.SLEEPY_IDLE_INTERVAL = initiatorSEDParams.findsubval(1)
      self.SLEEPY_ACTIVE_INTERVAL = initiatorSEDParams.findsubval(2)
    end
    return self
  end
end
matter.PBKDFParamRequest = Matter_PBKDFParamRequest

#################################################################################
# class PBKDFParamResponse encaspulating pbkdfparamresp-struct (p. 153)
#################################################################################
class Matter_PBKDFParamResponse
  var initiatorRandom
  var responderRandom
  var responderSessionId
  var pbkdf_parameters_iterations
  var pbkdf_parameters_salt
  var SLEEPY_IDLE_INTERVAL
  var SLEEPY_ACTIVE_INTERVAL

  def tlv2raw(b)
    var s = matter.TLV.Matter_TLV_struct()
    # initiatorRandom
    s.add_TLV(1, matter.TLV.B1, self.initiatorRandom)
    s.add_TLV(2, matter.TLV.B1, self.responderRandom)
    s.add_TLV(3, matter.TLV.U2, self.responderSessionId)
    var s_pbkdf = s.add_struct(4)
    s_pbkdf.add_TLV(1, matter.TLV.U4, self.pbkdf_parameters_iterations)
    s_pbkdf.add_TLV(2, matter.TLV.B1, self.pbkdf_parameters_salt)
    if self.SLEEPY_IDLE_INTERVAL != nil || self.SLEEPY_ACTIVE_INTERVAL != nil
      var s2 = s.add_struct(5)
      s2.add_TLV(1, matter.TLV.U4, self.SLEEPY_IDLE_INTERVAL)
      s2.add_TLV(2, matter.TLV.U4, self.SLEEPY_ACTIVE_INTERVAL)
    end
    return s.tlv2raw(b)
  end
end
matter.PBKDFParamResponse = Matter_PBKDFParamResponse

#################################################################################
# class Pake1
#################################################################################
class Matter_Pake1
  var pA

  def parse(b, idx)
    if idx == nil    idx = 0 end
    var val = matter.TLV.parse(b, idx)
    # tasmota.log("MTR: parsed TLV: " + str(val), 4)

    self.pA = val.getsubval(1)
    return self
  end
end
matter.Pake1 = Matter_Pake1

#################################################################################
# class Pake1
#################################################################################
class Matter_Pake2
  var pB                # 65 bytes
  var cB                # 32 bytes
  
  def tlv2raw(b)
    var s = matter.TLV.Matter_TLV_struct()
    #
    s.add_TLV(1, matter.TLV.B1, self.pB)
    s.add_TLV(2, matter.TLV.B1, self.cB)
    return s.tlv2raw(b)
  end
end
matter.Pake2 = Matter_Pake2

# class Pake3
class Matter_Pake3
  var cA

  def parse(b, idx)
    if idx == nil    idx = 0 end
    var val = matter.TLV.parse(b, idx)
    # tasmota.log("MTR: parsed TLV: " + str(val), 4)

    self.cA = val.getsubval(1)
    return self
  end
end
matter.Pake3 = Matter_Pake3

#################################################################################
# class Sigma1, p.160
#################################################################################
class Matter_Sigma1
  var initiatorRandom       # bytes(32)
  var initiator_session_id    # U16
  var destinationId         # bytes(32)
  var initiatorEphPubKey    # bytes(65)
  # var initiatorSEDParams    # (opt) sed-parameter-struct
  var SLEEPY_IDLE_INTERVAL
  var SLEEPY_ACTIVE_INTERVAL
  var resumptionID          # (opt) bytes(16)
  var initiatorResumeMIC    # (opt) bytes(16)
  var Msg1
  
  def parse(b, idx)
    if idx == nil    idx = 0 end
    var val = matter.TLV.parse(b, idx)
    self.Msg1 = b[idx..]
    # tasmota.log("MTR: Sigma1 TLV=" + str(val), 4)

    self.initiatorRandom = val.getsubval(1)
    self.initiator_session_id = val.getsubval(2)
    self.destinationId = val.getsubval(3)
    self.initiatorEphPubKey = val.getsubval(4)
    var initiatorSEDParams = val.findsub(5)
    if initiatorSEDParams != nil
      self.SLEEPY_IDLE_INTERVAL = initiatorSEDParams.findsubval(1)
      self.SLEEPY_ACTIVE_INTERVAL = initiatorSEDParams.findsubval(2)
    end
    self.resumptionID = val.findsubval(6)
    self.initiatorResumeMIC = val.findsubval(7)
    return self
  end
end
matter.Sigma1 = Matter_Sigma1

#################################################################################
# class Sigma2
#################################################################################
class Matter_Sigma2
  var responderRandom       # bytes(32)
  var responderSessionId    # U16
  var responderEphPubKey    # bytes(65)
  var encrypted2            # bytes()
  var SLEEPY_IDLE_INTERVAL
  var SLEEPY_ACTIVE_INTERVAL
  
  def tlv2raw(b)
    var s = matter.TLV.Matter_TLV_struct()
    # initiatorRandom
    s.add_TLV(1, matter.TLV.B1, self.responderRandom)
    s.add_TLV(2, matter.TLV.U2, self.responderSessionId)
    s.add_TLV(3, matter.TLV.B1, self.responderEphPubKey)
    s.add_TLV(4, matter.TLV.B1, self.encrypted2)
    if self.SLEEPY_IDLE_INTERVAL != nil || self.SLEEPY_ACTIVE_INTERVAL != nil
      var s2 = s.add_struct(5)
      s2.add_TLV(1, matter.TLV.U4, self.SLEEPY_IDLE_INTERVAL)
      s2.add_TLV(2, matter.TLV.U4, self.SLEEPY_ACTIVE_INTERVAL)
    end
    return s.tlv2raw(b)
  end
end
matter.Sigma2 = Matter_Sigma2

#################################################################################
# class Sigma2Resume, p.170
#################################################################################
class Matter_Sigma2Resume
  var resumptionID          # bytes(16)
  var sigma2ResumeMIC       # bytes(16)
  var responderSessionID    # u16
  var SLEEPY_IDLE_INTERVAL
  var SLEEPY_ACTIVE_INTERVAL
  
  def tlv2raw(b)
    var s = matter.TLV.Matter_TLV_struct()
    # initiatorRandom
    s.add_TLV(1, matter.TLV.B1, self.resumptionID)
    s.add_TLV(2, matter.TLV.B1, self.sigma2ResumeMIC)
    s.add_TLV(3, matter.TLV.U2, self.responderSessionID)
    if self.SLEEPY_IDLE_INTERVAL != nil || self.SLEEPY_ACTIVE_INTERVAL != nil
      var s2 = s.add_struct(4)
      s2.add_TLV(1, matter.TLV.U4, self.SLEEPY_IDLE_INTERVAL)
      s2.add_TLV(2, matter.TLV.U4, self.SLEEPY_ACTIVE_INTERVAL)
    end
    return s.tlv2raw(b)
  end
end
matter.Sigma2Resume = Matter_Sigma2Resume

#################################################################################
# class Sigma3, p.160
#################################################################################
class Matter_Sigma3
  var TBEData3Encrypted     # bytes()
  var Msg3
  
  def parse(b, idx)
    if idx == nil    idx = 0 end
    var val = matter.TLV.parse(b, idx)
    self.Msg3 = b[idx..]
    # tasmota.log("MTR: Sigma3 TLV=" + str(val), 4)

    self.TBEData3Encrypted = val.getsubval(1)
    return self
  end
end
matter.Sigma3 = Matter_Sigma3


#-

# STEP 1
PBKDF_PR = matter.PBKDFParamRequest().parse(bytes("15300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280435052501881325022C011818"))
{'passcodeId': 0, 'initiatorRandom': bytes('D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66'),
'hasPBKDFParameters': 0, 'SLEEPY_IDLE_INTERVAL': 5000, 'SLEEPY_ACTIVE_INTERVAL': 300, 'initiator_session_id': 19461}





# ##########################################################################################
PBKDF_PR = matter.PBKDFParamRequest().parse(bytes("15300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280435052501881325022C011818"))
print(matter.inspect(PBKDF_PR))
{'passcodeId': 0, 'initiatorRandom': bytes('D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66'), 'hasPBKDFParameters': 0, 'SLEEPY_IDLE_INTERVAL': 5000, 'SLEEPY_ACTIVE_INTERVAL': 300, 'initiator_session_id': 19461}

var r = matter.PBKDFParamResponse()
r.initiatorRandom = bytes("112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF00")
r.responderRandom = bytes("112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF00")
r.responderSessionId = 1234
r.pbkdf_parameters_iterations = 1000
r.pbkdf_parameters_salt = bytes("deadbeef11223344deadbeef11223344")
r.SLEEPY_IDLE_INTERVAL = 100
r.SLEEPY_ACTIVE_INTERVAL = 200

bytes('15300120112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF00300220112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF002503D20418')


# ##########################################################################################
var m = matter.Frame()
m.decode(bytes("0400000006747A09347BFD880AC8128005205FC3000015300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280435052501881325022C011818"))

# 'opcode': 32
# 'local_session_id': 0
# 'protocol_id': 0

#{'dest_node_id_8': nil, 'flag_dsiz': 0, 'x_flag_r': 1, 'sec_c': 0, 'app_payload_idx': 22, 'sec_flags': 0, 
#'dest_node_id_2': nil, 'x_flag_sx': 0, 'sec_p': 0, 'message_counter': 159020038, 'x_flag_v': 0, 'flags': 4, 
#'vendor_id': nil, 'x_flag_i': 1, 'source_node_id': bytes('347BFD880AC81280'), 
#'ack_message_counter': nil, 'raw': bytes('0400000006747A09347BFD880AC8128005205FC3000015300120D2DAEE8760C9...'), 
#'exchange_id': 50015, 'opcode': 32, 'local_session_id': 0, 'x_flag_a': 0, 'flag_s': 1, 'x_flags': 5, 'sec_sesstype': 0, 
#'sec_extensions': nil, 'sec_mx': 0, 'protocol_id': 0}

0400000006747A09347BFD880AC8128005205FC3000015300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280435052501881325022C011818

12:09:05.561 MTR: sending packet to '192.168.2.109:5540' 050000000674C82B96B90B530000347BFD880AC8128000215FC3000015300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66300220AA981992DB666B51DEDC0DD67ED6ABBB29AB30E67452C8893166FBF5FD95601D2503010018

050000000674C82B96B90B530000347BFD880AC8128000215FC3000015300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66300220AA981992DB666B51DEDC0DD67ED6ABBB29AB30E67452C8893166FBF5FD95601D2503010018
12:10:29.174 MTR: received message {
    'flag_s': 1, 'flag_dsiz': 1, 'x_flag_r': 1, 'x_flag_sx': 1, 'sec_mx': 0, 'sec_flags': 0, 
    'dest_node_id_2': nil, 'sec_sesstype': 0, 'local_session_id': 0, 'ack_message_counter': -2014389550, 
    'x_flag_v': 1, 'flags': 5, 'vendor_id': 8193, 'x_flag_i': 1, 'source_node_id': bytes('96B90B530000347B'), 
    'app_payload_idx': 51590, 
    'raw': bytes('050000000674C82B96B90B530000347BFD880AC8128000215FC3000015300120...'), 
    'exchange_id': 0, 'opcode': 195, 'message_counter': 734557190, 'sec_p': 0, 'sec_c': 0, 
    'protocol_id': 12309, 'dest_node_id_8': bytes('FD880AC812800021'), 
    'sec_extensions': nil, 'x_flag_a': 1, 'x_flags': 95}

347BFD880AC81280

fe80::1bf1:bd1a:c5ff:818a
b8:27:eb:8b:d2:59

{'mac': 'C8:2B:96:B9:0B:53', 'ip6local': 'fe80::ca2b:96ff:feb9:b53', 'ip': '192.168.1.116'}

-#
