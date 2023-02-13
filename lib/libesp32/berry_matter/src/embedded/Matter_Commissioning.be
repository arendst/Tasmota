#
# Matter_Commissioning.be - suppport for Matter Commissioning process
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

#@ solidify:Matter_Commisioning_Context,weak

#################################################################################
# Class Matter_Commisioning_Context
#################################################################################
class Matter_Commisioning_Context
# static Matter_Context_Prefix = "Matter PAKE V1 Commissioning" # spec is wrong
  static var Matter_Context_Prefix = "CHIP PAKE V1 Commissioning" # from CHIP code
  static var SEKeys_Info = "SessionKeys"
  static var S2K_Info = "Sigma2"
  static var S3K_Info = "Sigma3"
  static var TBEData2_Nonce = "NCASE_Sigma2N"
  static var TBEData3_Nonce = "NCASE_Sigma3N"

  var responder                       # reference to the caller, sending packets
  var device                          # root device object
  var spake
  var future_initiator_session_id
  var future_local_session_id
  # used by TT hash
  var PBKDFParamRequest, PBKDFParamResponse
  # PAKE
  var y                               # 32 bytes random known only by verifier
  var pA, pB, cA, cB
  var Ke
  # CASE
  var ResponderEph_priv, ResponderEph_pub
  var initiatorEph_pub
  # Session data
  var session_timestamp
  var I2RKey, R2IKey, AttestationChallenge
  # is commissioning window open
  var window_open

  def init(responder)
    import crypto
    self.responder = responder
    self.device = responder.device
    # generate y once
    self.y = crypto.random(32)

    self.window_open = true         # auto-commissioning for now
  end

  def process_incoming(msg)
    #
    if !self.window_open
      tasmota.log("MTR: commissioning not open", 2)
      return false
    end

    tasmota.log("MTR: received message " + matter.inspect(msg), 3)
    if msg.opcode == 0x20
      return self.parse_PBKDFParamRequest(msg)
    elif msg.opcode == 0x22
      return self.parse_Pake1(msg)
    elif msg.opcode == 0x24
      return self.parse_Pake3(msg)
    elif msg.opcode == 0x30
      return self.parse_Sigma1(msg)
    elif msg.opcode == 0x32
      return self.parse_Sigma3(msg)
    end

    return false
  end

  def parse_PBKDFParamRequest(msg)
    import crypto
    # sanity checks
    if msg.opcode != 0x20 || msg.local_session_id != 0 || msg.protocol_id != 0
      raise "protocol_error", "invalid PBKDFParamRequest message"
    end
    var pbkdfparamreq = matter.PBKDFParamRequest().parse(msg.raw, msg.app_payload_idx)
    msg.session.set_mode(matter.Session.__PASE)

    self.PBKDFParamRequest = msg.raw[msg.app_payload_idx..]

    # sanity check for PBKDFParamRequest
    if pbkdfparamreq.passcodeId != 0  raise "protocol_error", "non-zero passcode id" end

    # record the initiator_session_id
    self.future_initiator_session_id = pbkdfparamreq.initiator_session_id
    self.future_local_session_id = self.device.sessions.gen_local_session_id()

    # prepare response
    var pbkdfparamresp = matter.PBKDFParamResponse()

    pbkdfparamresp.initiatorRandom = pbkdfparamreq.initiatorRandom
    # generate 32 bytes random
    pbkdfparamresp.responderRandom = crypto.random(32)
    pbkdfparamresp.responderSessionId = self.future_local_session_id
    pbkdfparamresp.pbkdf_parameters_salt = self.device.salt
    pbkdfparamresp.pbkdf_parameters_iterations = self.device.iterations
    tasmota.log("MTR: pbkdfparamresp: " + str(matter.inspect(pbkdfparamresp)), 3)
    var pbkdfparamresp_raw = pbkdfparamresp.encode()
    tasmota.log("MTR: pbkdfparamresp_raw: " + pbkdfparamresp_raw.tohex(), 3)

    self.PBKDFParamResponse = pbkdfparamresp_raw

    var resp = msg.build_response(0x21 #-PBKDR Response-#, true)
    var raw = resp.encode(pbkdfparamresp_raw)

    self.responder.send_response(raw, msg.remote_ip, msg.remote_port, resp.message_counter)
  end

  def parse_Pake1(msg)
    import crypto
    # sanity checks
    if msg.opcode != 0x22 || msg.local_session_id != 0 || msg.protocol_id != 0
      raise "protocol_error", "invalid Pake1 message"
    end
    var pake1 = matter.Pake1().parse(msg.raw, msg.app_payload_idx)

    self.pA = pake1.pA
    tasmota.log("MTR: received pA=" + self.pA.tohex(), 3)

    
    tasmota.log("MTR: spake: " + matter.inspect(self.spake), 3)
    # instanciate SPAKE
    self.spake = crypto.SPAKE2P_Matter(self.device.w0, self.device.w1, self.device.L)
    # compute pB
    self.spake.compute_pB(self.y)
    self.pB = self.spake.pB
    tasmota.log("MTR: y=" + self.y.tohex(), 3)
    tasmota.log("MTR: pb=" + self.pB.tohex(), 3)

    # compute ZV
    self.spake.compute_ZV_verifier(self.pA)
    tasmota.log("MTR: Z=" + self.spake.Z.tohex(), 3)
    tasmota.log("MTR: V=" + self.spake.V.tohex(), 3)

    var context = crypto.SHA256()
    context.update(bytes().fromstring(self.Matter_Context_Prefix))
    context.update(self.PBKDFParamRequest)
    context.update(self.PBKDFParamResponse)
    var context_hash = context.out()

    tasmota.log("MTR: Context=" + context_hash.tohex(), 3)

    # add pA
    self.spake.pA = self.pA

    self.spake.set_context(context_hash)
    self.spake.compute_TT_hash(true)      # `true` to indicate it's Matter variant to SPAKE2+

    tasmota.log("MTR: ------------------------------", 3)
    tasmota.log("MTR: Context = " + self.spake.Context.tohex(), 3)
    tasmota.log("MTR: A       = " + self.spake.A.tohex(), 3)
    tasmota.log("MTR: B       = " + self.spake.B.tohex(), 3)
    tasmota.log("MTR: M       = " + self.spake.M.tohex(), 3)
    tasmota.log("MTR: N       = " + self.spake.N.tohex(), 3)
    tasmota.log("MTR: pA      = " + self.spake.pA.tohex(), 3)
    tasmota.log("MTR: pB      = " + self.spake.pB.tohex(), 3)
    tasmota.log("MTR: Z       = " + self.spake.Z.tohex(), 3)
    tasmota.log("MTR: V       = " + self.spake.V.tohex(), 3)
    tasmota.log("MTR: w0      = " + self.spake.w0.tohex(), 3)
    tasmota.log("MTR: ------------------------------", 3)

    tasmota.log("MTR: Kmain   =" + self.spake.Kmain.tohex(), 3)

    tasmota.log("MTR: KcA     =" + self.spake.KcA.tohex(), 3)
    tasmota.log("MTR: KcB     =" + self.spake.KcB.tohex(), 3)
    tasmota.log("MTR: K_shared=" + self.spake.K_shared.tohex(), 3)
    tasmota.log("MTR: Ke      =" + self.spake.Ke.tohex(), 3)
    self.cB = self.spake.cB
    self.Ke = self.spake.Ke
    tasmota.log("MTR: cB=" + self.cB.tohex(), 3)

    var pake2 = matter.Pake2()
    pake2.pB = self.pB
    pake2.cB = self.cB
    tasmota.log("MTR: pake2: " + matter.inspect(pake2), 3)
    var pake2_raw = pake2.encode()
    tasmota.log("MTR: pake2_raw: " + pake2_raw.tohex(), 3)


    # now package the response message
    var resp = msg.build_response(0x23 #-pake-2-#, true)  # no reliable flag
    var raw = resp.encode(pake2_raw)

    self.responder.send_response(raw, msg.remote_ip, msg.remote_port, resp.message_counter)
  end

  def parse_Pake3(msg)
    import crypto
    # sanity checks
    if msg.opcode != 0x24 || msg.local_session_id != 0 || msg.protocol_id != 0
      raise "protocol_error", "invalid Pake3 message"
    end
    var pake3 = matter.Pake3().parse(msg.raw, msg.app_payload_idx)

    self.cA = pake3.cA
    tasmota.log("MTR: received cA=" + self.cA.tohex(), 3)

    # check the value against computed
    if self.cA != self.spake.cA   raise "protocol_error", "invalid cA received" end

    # send PakeFinished and compute session key
    self.session_timestamp = tasmota.rtc()['utc']
    var session_keys = crypto.HKDF_SHA256().derive(self.Ke, bytes(), bytes().fromstring(self.SEKeys_Info), 48)
    self.I2RKey = session_keys[0..15]
    self.R2IKey = session_keys[16..31]
    self.AttestationChallenge = session_keys[32..47]

    tasmota.log("MTR: ******************************", 3)
    tasmota.log("MTR: session_keys=" + session_keys.tohex(), 3)
    tasmota.log("MTR: I2RKey      =" + self.I2RKey.tohex(), 3)
    tasmota.log("MTR: R2IKey      =" + self.R2IKey.tohex(), 3)
    tasmota.log("MTR: AC          =" + self.AttestationChallenge.tohex(), 3)
    tasmota.log("MTR: ******************************", 3)

    # now package the response message
    var resp = msg.build_response(0x40 #-StatusReport-#, false)  # no reliable flag

    var status_raw = bytes()
    status_raw.add(0x00, 2)      # GeneralCode = SUCCESS
    status_raw.add(0x0000, 4)    # ProtocolID = 0 (PROTOCOL_ID_SECURE_CHANNEL)
    status_raw.add(0x0000, 4)    # ProtocolCode = 0 (SESSION_ESTABLISHMENT_SUCCESS)

    var raw = resp.encode(status_raw)

    self.responder.send_response(raw, msg.remote_ip, msg.remote_port, nil)
    self.responder.add_session(self.future_local_session_id, self.future_initiator_session_id, self.I2RKey, self.R2IKey, self.AttestationChallenge, self.session_timestamp)
  end

  def find_session_by_destination_id(destinationId, initiatorRandom)
    import crypto
    # Validate Sigma1 Destination ID, p.162
    # traverse all existing sessions
    tasmota.log("MTR: SEARCHING: destinationId=" + destinationId.tohex(), 3)
    for session:self.device.sessions.sessions
      if session.noc == nil || session.fabric == nil || session.deviceid == nil     continue end
      # compute candidateDestinationId, Section 4.13.2.4.1, “Destination Identifier”
      var destinationMessage = initiatorRandom + session.get_ca_pub() + session.get_fabric() + session.get_deviceid()
      var key = session.get_ipk_group_key()
      tasmota.log("MTR: SIGMA1: destinationMessage=" + destinationMessage.tohex(), 3)
      tasmota.log("MTR: SIGMA1: key_ipk=" + key.tohex(), 3)
      var h = crypto.HMAC_SHA256(key)
      h.update(destinationMessage)
      var candidateDestinationId = h.out()
      tasmota.log("MTR: SIGMA1: candidateDestinationId=" + candidateDestinationId.tohex(), 3)
      if candidateDestinationId == destinationId
        return session
      end
    end
    return nil
  end

  def parse_Sigma1(msg)
    import crypto
    # sanity checks
    if msg.opcode != 0x30 || msg.local_session_id != 0 || msg.protocol_id != 0
      raise "protocol_error", "invalid Pake1 message"
    end
    var sigma1 = matter.Sigma1().parse(msg.raw, msg.app_payload_idx)

    self.initiatorEph_pub = sigma1.initiatorEphPubKey

    # find session
    var is_resumption = (sigma1.resumptionID != nil && sigma1.initiatorResumeMIC != nil)

    # Check that it's a resumption
    var session
    if is_resumption
      session = self.device.sessions.find_session_by_resumption_id(sigma1.resumptionID)
    else
      session = self.find_session_by_destination_id(sigma1.destinationId, sigma1.initiatorRandom)
    end
    if session == nil   raise "valuer_error", "StatusReport(GeneralCode: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: NO_SHARED_TRUST_ROOTS)" end
    session.source_node_id = msg.source_node_id
    session.set_mode(matter.Session.__CASE)

    if msg.session
      self.device.sessions.remove_session(msg.session)  # drop the temporary session that was created
    end
    msg.session = session
    session._future_initiator_session_id = sigma1.initiator_session_id    # update initiator_session_id
    session._future_local_session_id = self.device.sessions.gen_local_session_id()
    self.future_local_session_id = session._future_local_session_id


    # Check that it's a resumption
    if   is_resumption && session.shared_secret != nil
      # Resumption p.169
      var s1rk_salt = sigma1.initiatorRandom + sigma1.resumptionID
      var s1rk_info = bytes().fromstring("Sigma1_Resume")
      var s1rk = crypto.HKDF_SHA256().derive(session.shared_secret, s1rk_salt, s1rk_info, 16)

      var Resume1MIC_Nonce = bytes().fromstring("NCASE_SigmaR1")
      var encrypted = sigma1.initiatorResumeMIC[0..-17]
      var tag = sigma1.initiatorResumeMIC[-16..]
      var ec = crypto.AES_CCM(s1rk, Resume1MIC_Nonce, bytes(), size(encrypted), 16)
      var Resume1MICPayload = ec.decrypt(encrypted)
      var decrypted_tag = ec.tag()

      tasmota.log("****************************************", 3)
      tasmota.log("MTR: * s1rk              = " + s1rk.tohex(), 3)
      tasmota.log("MTR: * tag               = " + tag.tohex(), 3)
      tasmota.log("MTR: * Resume1MICPayload = " + Resume1MICPayload.tohex(), 3)
      tasmota.log("MTR: * decrypted_tag     = " + decrypted_tag.tohex(), 3)
      tasmota.log("****************************************", 3)
      if tag == decrypted_tag
        # Generate and Send Sigma2_Resume
        session.resumption_id = crypto.random(16)     # generate a new resumption id

        # compute S2RK
        var s2rk_info = bytes().fromstring("Sigma2_Resume") + session.resumption_id
        var s2rk_salt = sigma1.initiatorRandom + sigma1.resumptionID
        var s2rk = crypto.HKDF_SHA256().derive(session.shared_secret, s2rk_salt, s2rk_info, 16)

        # compute Resume2MIC
        var aes = crypto.AES_CCM(s2rk, bytes().fromstring("NCASE_SigmaR2"), bytes(), 0, 16)
        var Resume2MIC = aes.tag()

        var sigma2resume = matter.Sigma2Resume()
        sigma2resume.resumptionID = session.resumption_id
        sigma2resume.responderSessionID = session._future_local_session_id
        sigma2resume.sigma2ResumeMIC = Resume2MIC

        # # compute session key, p.178
        var session_keys = crypto.HKDF_SHA256().derive(session.shared_secret #- input key -#,
                                                      sigma1.initiatorRandom + sigma1.resumptionID #- salt -#,
                                                      bytes().fromstring("SessionResumptionKeys") #- info -#,
                                                      48)
        var i2r = session_keys[0..15]
        var r2i = session_keys[16..31]
        var ac = session_keys[32..47]
        var session_timestamp = tasmota.rtc()['utc']

        tasmota.log("MTR: ******************************", 3)
        tasmota.log("MTR: I2RKey      =" + i2r.tohex(), 3)
        tasmota.log("MTR: R2IKey      =" + r2i.tohex(), 3)
        tasmota.log("MTR: AC          =" + ac.tohex(), 3)
        tasmota.log("MTR: ******************************", 3)

        var sigma2resume_raw = sigma2resume.encode()
        session._Msg1 = nil
        tasmota.log("MTR: sigma2resume_raw: " + sigma2resume_raw.tohex(), 3)

        # now package the response message
        var resp = msg.build_response(0x33 #-sigma-2-resume-#, true)
        var raw = resp.encode(sigma2resume_raw)
    
        self.responder.send_response(raw, msg.remote_ip, msg.remote_port, resp.message_counter)

        session.close()
        session.set_keys(i2r, r2i, ac, session_timestamp)
        session.set_persist(true)     # keep session on flash
        session.set_no_expiration()   # never expire
        session.save()

        return true
      else
        sigma1.resumptionID = nil
        # fall through normal sigma1 (non-resumption)
      end
    end

    if sigma1.resumptionID == nil || sigma1.initiatorResumeMIC == nil
      # Compute Sigma2, p.162
      session.resumption_id = crypto.random(16)
      self.ResponderEph_priv = crypto.random(32)
      self.ResponderEph_pub = crypto.EC_P256().public_key(self.ResponderEph_priv)
      var responderRandom = crypto.random(32)

      session.shared_secret = crypto.EC_P256().shared_key(self.ResponderEph_priv, sigma1.initiatorEphPubKey)

      var sigma2_tbsdata = matter.TLV.Matter_TLV_struct()
      sigma2_tbsdata.add_TLV(1, matter.TLV.B2, session.get_noc())
      sigma2_tbsdata.add_TLV(2, matter.TLV.B2, session.get_icac())
      sigma2_tbsdata.add_TLV(3, matter.TLV.B2, self.ResponderEph_pub)
      sigma2_tbsdata.add_TLV(4, matter.TLV.B2, sigma1.initiatorEphPubKey)

      var TBSData2Signature = crypto.EC_P256().ecdsa_sign_sha256(session.get_pk(), sigma2_tbsdata.encode())

      var sigma2_tbedata = matter.TLV.Matter_TLV_struct()
      sigma2_tbedata.add_TLV(1, matter.TLV.B2, session.get_noc())
      sigma2_tbedata.add_TLV(2, matter.TLV.B2, session.get_icac())
      sigma2_tbedata.add_TLV(3, matter.TLV.B2, TBSData2Signature)
      sigma2_tbedata.add_TLV(4, matter.TLV.B2, session.resumption_id)

      # compute TranscriptHash = Crypto_Hash(message = Msg1)
      tasmota.log("****************************************", 3)
      session._Msg1 = sigma1.Msg1
      tasmota.log("MTR: * MSG1          = " + session._Msg1.tohex(), 3)
      var TranscriptHash = crypto.SHA256().update(session._Msg1).out()

      # Compute S2K, p.175
      var s2k_info = bytes().fromstring(self.S2K_Info)
      var s2k_salt = session.get_ipk_group_key() + responderRandom + self.ResponderEph_pub + TranscriptHash

      var s2k = crypto.HKDF_SHA256().derive(session.shared_secret, s2k_salt, s2k_info, 16)
      tasmota.log("MTR: * SharedSecret  = " + session.shared_secret.tohex(), 3)
      tasmota.log("MTR: * s2k_salt      = " + s2k_salt.tohex(), 3)
      tasmota.log("MTR: * s2k           = " + s2k.tohex(), 3)

      var sigma2_tbedata_raw = sigma2_tbedata.encode()
      # // `AES_CCM.init(secret_key:bytes(16 or 32), iv:bytes(7..13), aad:bytes(), data_len:int, tag_len:int) -> instance`

      var aes = crypto.AES_CCM(s2k, bytes().fromstring(self.TBEData2_Nonce), bytes(), size(sigma2_tbedata_raw), 16)
      var TBEData2Encrypted = aes.encrypt(sigma2_tbedata_raw) + aes.tag()
      tasmota.log("MTR: * TBEData2Enc   = " + TBEData2Encrypted.tohex(), 3)
      tasmota.log("****************************************", 3)

      var sigma2 = matter.Sigma2()
      sigma2.responderRandom = responderRandom
      sigma2.responderSessionId = self.future_local_session_id
      sigma2.responderEphPubKey = self.ResponderEph_pub
      sigma2.encrypted2 = TBEData2Encrypted
      tasmota.log("MTR: sigma2: " + matter.inspect(sigma2), 3)
      var sigma2_raw = sigma2.encode()
      session._Msg2 = sigma2_raw
      tasmota.log("MTR: sigma2_raw: " + sigma2_raw.tohex(), 3)
  
      # now package the response message
      var resp = msg.build_response(0x31 #-sigma-2-#, true)  # no reliable flag
      var raw = resp.encode(sigma2_raw)
  
      self.responder.send_response(raw, msg.remote_ip, msg.remote_port, resp.message_counter)
      return true
    end

    return true
  end

  def parse_Sigma3(msg)
    import crypto
    # sanity checks
    if msg.opcode != 0x32 || msg.local_session_id != 0 || msg.protocol_id != 0
      raise "protocol_error", "invalid Pake1 message"
    end
    var session = msg.session
    var sigma3 = matter.Sigma3().parse(msg.raw, msg.app_payload_idx)

    tasmota.log("****************************************", 3)
    # compute TranscriptHash = Crypto_Hash(message = Msg1 || Msg2)
    var TranscriptHash = crypto.SHA256().update(session._Msg1).update(session._Msg2).out()
    tasmota.log("MTR: * session       = " + str(session), 3)
    tasmota.log("MTR: session.ipk_epoch_key " + str(session.ipk_epoch_key), 3)
    tasmota.log("MTR: session.fabric_compressed " + str(session.fabric_compressed), 3)
    tasmota.log("MTR: * ipk_group_key = " + session.get_ipk_group_key().tohex(), 3)
    tasmota.log("MTR: * TranscriptHash= " + TranscriptHash.tohex(), 3)

    var s3k_info = bytes().fromstring(self.S3K_Info)
    var s3k = crypto.HKDF_SHA256().derive(session.shared_secret, session.get_ipk_group_key() + TranscriptHash, s3k_info, 16)

    tasmota.log("****************************************", 3)
    # self.ipk_epoch_key == nil || self.fabric_compressed")
    tasmota.log("MTR: * s3k_salt      = " + (session.get_ipk_group_key() + TranscriptHash).tohex(), 3)
    tasmota.log("MTR: * s3k           = " + s3k.tohex(), 3)
    tasmota.log("****************************************", 3)

    # decrypt
    var encrypted = sigma3.TBEData3Encrypted[0..-17]
    var tag = sigma3.TBEData3Encrypted[-16..]
    var ec = crypto.AES_CCM(s3k, bytes().fromstring(self.TBEData3_Nonce), bytes(), size(encrypted), 16)
    var TBEData3 = ec.decrypt(encrypted)
    var TBETag3 = ec.tag()
    tasmota.log("MTR: * TBEData3      = " + TBEData3.tohex(), 3)
    tasmota.log("MTR: * TBETag3       = " + TBETag3.tohex(), 3)
    tasmota.log("MTR: * tag_sent      = " + tag.tohex(), 3)
    tasmota.log("****************************************", 3)

    if TBETag3 != tag   raise "value_error", "tag do not match"  end

    var TBEData3TLV = matter.TLV.parse(TBEData3)
    var initiatorNOC = TBEData3TLV.findsubval(1)
    var initiatorICAC = TBEData3TLV.findsubval(2)
    var ec_signature = TBEData3TLV.findsubval(3)
    # Success = Crypto_VerifyChain(certificates = [TBEData3.initiatorNOC, TBEData3.initiatorICAC, TrustedRCAC]), when TBEData3.initiatorICAC is present
    # TODO
    var initiatorNOCTLV = matter.TLV.parse(initiatorNOC)
    tasmota.log("MTR: initiatorNOCTLV = " + str(initiatorNOCTLV), 3)
    var initiatorNOCPubKey = initiatorNOCTLV.findsubval(9)
    var initiatorNOCListDN = initiatorNOCTLV.findsub(6)
    var initiatorFabricId = initiatorNOCListDN.findsubval(17)
    if type(initiatorFabricId) == 'int'   initiatorFabricId = int64(initiatorFabricId) end
    session.peer_node_id = initiatorFabricId.tobytes()
    tasmota.log("MTR: initiatorFabricId="+str(session.peer_node_id), 3)

    var sigma3_tbs = matter.TLV.Matter_TLV_struct()
    sigma3_tbs.add_TLV(1, matter.TLV.B1, initiatorNOC)
    sigma3_tbs.add_TLV(2, matter.TLV.B1, initiatorICAC)
    sigma3_tbs.add_TLV(3, matter.TLV.B1, self.initiatorEph_pub)
    sigma3_tbs.add_TLV(4, matter.TLV.B1, self.ResponderEph_pub) 
    var sigma3_tbs_raw = sigma3_tbs.encode()

    tasmota.log("MTR: * initiatorNOCPubKey      = " + initiatorNOCPubKey.tohex(), 3)
    tasmota.log("MTR: * ec_signature      = " + ec_signature.tohex(), 3)
    tasmota.log("****************************************", 3)

    # `crypto.EC_P256().ecdsa_verify_sha256(public_key:bytes(65), message:bytes(), hash:bytes()) -> bool`
    var sigma3_tbs_valid = crypto.EC_P256().ecdsa_verify_sha256(initiatorNOCPubKey, sigma3_tbs_raw, ec_signature)

    if !sigma3_tbs_valid    raise "value_error", "sigma3_tbs does not have a valid signature" end

    # All good, compute new keys
    tasmota.log("MTR: Sigma3 verified, computing new keys", 3)

    TranscriptHash = crypto.SHA256().update(session._Msg1).update(session._Msg2).update(sigma3.Msg3).out()
    # we can now free _Msg1 and _Msg2
    session._Msg1 = nil
    session._Msg2 = nil

    tasmota.log("MTR: ******************************", 3)
    tasmota.log("MTR: shared_secret =" + session.shared_secret.tohex(), 3)
    tasmota.log("MTR: ipk + hash    =" + (session.get_ipk_group_key() + TranscriptHash).tohex(), 3)
    # compute session key
    var session_keys = crypto.HKDF_SHA256().derive(session.shared_secret #- input key -#,
                                                   session.get_ipk_group_key() + TranscriptHash #- salt -#,
                                                   bytes().fromstring(self.SEKeys_Info) #- info -#,
                                                   48)
    var i2r = session_keys[0..15]
    var r2i = session_keys[16..31]
    var ac = session_keys[32..47]
    var session_timestamp = tasmota.rtc()['utc']

    tasmota.log("MTR: ******************************", 3)
    tasmota.log("MTR: I2RKey      =" + i2r.tohex(), 3)
    tasmota.log("MTR: R2IKey      =" + r2i.tohex(), 3)
    tasmota.log("MTR: AC          =" + ac.tohex(), 3)
    tasmota.log("MTR: ******************************", 3)

    # Send success status report
    var resp = msg.build_response(0x40 #-StatusReport-#, true)  # reliable flag

    var status_raw = bytes()
    status_raw.add(0x00, 2)      # GeneralCode = SUCCESS
    status_raw.add(0x0000, 4)    # ProtocolID = 0 (PROTOCOL_ID_SECURE_CHANNEL)
    status_raw.add(0x0000, 4)    # ProtocolCode = 0 (SESSION_ESTABLISHMENT_SUCCESS)

    var raw = resp.encode(status_raw)

    self.responder.send_response(raw, msg.remote_ip, msg.remote_port, resp.message_counter)

    session.close()
    session.set_keys(i2r, r2i, ac, session_timestamp)
    session.set_persist(true)     # keep session on flash
    session.set_no_expiration()   # never expire
    session.save()

    return true
  end

  #############################################################
  # placeholder, nothing to run for now
  def every_second()
  end
end
matter.Commisioning_Context = Matter_Commisioning_Context

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
