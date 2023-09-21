#
# Matter_Commissioning.be - suppport for Matter Commissioning process PASE and CASE
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
  
  def init(responder)
    import crypto
    self.responder = responder
    self.device = responder.device
  end

  #############################################################
  def add_session(local_session_id, initiator_session_id, i2r, r2i, ac)
    # create session object
    tasmota.log(format("MTR: add_session local_session_id=%i initiator_session_id=%i", local_session_id, initiator_session_id), 4)
    
    var session = self.device.sessions.create_session(local_session_id, initiator_session_id)
    session.set_keys(i2r, r2i, ac)
  end

  def process_incoming(msg)
    #
    if !self.device.is_commissioning_open() && msg.opcode >= 0x20 && msg.opcode <= 0x24
      tasmota.log("MTR: commissioning not open", 2)
      return false
    end

    # tasmota.log("MTR: received message " + matter.inspect(msg), 4)
    if   msg.opcode == 0x10
      # don't need to do anything, the message is acked already before this call
    elif msg.opcode == 0x20
      return self.parse_PBKDFParamRequest(msg)
    elif msg.opcode == 0x22
      return self.parse_Pake1(msg)
    elif msg.opcode == 0x24
      return self.parse_Pake3(msg)
    elif msg.opcode == 0x30
      return self.parse_Sigma1(msg)
    elif msg.opcode == 0x32
      return self.parse_Sigma3(msg)
    elif msg.opcode == 0x40
      return self.parse_StatusReport(msg)
    else
      tasmota.log(format("MTR: >????????? Unknown OpCode (secure channel) %02X", msg.opcode), 2)
      return false
    end

    return false
  end

  #################################################################################
  # send_status_report
  #
  # send a StatusReport message (unencrypted)
  #
  # Usage:
  # # StatusReport(GeneralCode: SUCCESS, ProtocolId: SECURE_CHANNEL, ProtocolCode: SESSION_ESTABLISHMENT_SUCCESS)
  # var raw = send_status_report(0x00, 0x0000, 0x0000)
  # self.responder.send_response(raw, msg.remote_ip, msg.remote_port, nil)
  def send_status_report(msg, general_code, protocol_id, protocol_code, reliable)
    # now package the response message
    var resp = msg.build_response(0x40 #-StatusReport-#, reliable)

    var status_raw = bytes()
    status_raw.add(general_code, 2)
    status_raw.add(protocol_id, 4)
    status_raw.add(protocol_code, 4)

    var raw = resp.encode_frame(status_raw)

    self.responder.send_response_frame(resp)
  end

  def parse_PBKDFParamRequest(msg)
    import crypto
    var session = msg.session
    # sanity checks
    if msg.opcode != 0x20 || msg.local_session_id != 0 || msg.protocol_id != 0
      tasmota.log("MTR: invalid PBKDFParamRequest message", 2)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end
    var pbkdfparamreq = matter.PBKDFParamRequest().parse(msg.raw, msg.app_payload_idx)
    msg.session.set_mode_PASE()

    session.__Msg1 = msg.raw[msg.app_payload_idx..]

    # sanity check for PBKDFParamRequest
    if pbkdfparamreq.passcodeId != 0
      tasmota.log("MTR: non-zero passcode id", 2)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end

    # record the initiator_session_id
    session.__future_initiator_session_id = pbkdfparamreq.initiator_session_id
    session.__future_local_session_id = self.device.sessions.gen_local_session_id()
    tasmota.log(format("MTR: +Session   (%6i) from '[%s]:%i'", session.__future_local_session_id, msg.remote_ip, msg.remote_port), 3)

    # prepare response
    var pbkdfparamresp = matter.PBKDFParamResponse()

    pbkdfparamresp.initiatorRandom = pbkdfparamreq.initiatorRandom
    # generate 32 bytes random
    pbkdfparamresp.responderRandom = crypto.random(32)
    pbkdfparamresp.responderSessionId = session.__future_local_session_id
    pbkdfparamresp.pbkdf_parameters_salt = self.device.commissioning_salt
    pbkdfparamresp.pbkdf_parameters_iterations = self.device.commissioning_iterations
    # tasmota.log("MTR: pbkdfparamresp: " + str(matter.inspect(pbkdfparamresp)), 4)
    var pbkdfparamresp_raw = pbkdfparamresp.tlv2raw()
    # tasmota.log("MTR: pbkdfparamresp_raw: " + pbkdfparamresp_raw.tohex(), 4)

    session.__Msg2 = pbkdfparamresp_raw

    var resp = msg.build_response(0x21 #-PBKDR Response-#, true)
    var raw = resp.encode_frame(pbkdfparamresp_raw)

    self.responder.send_response_frame(resp)
    return true
  end

  def parse_Pake1(msg)
    import crypto

    var session = msg.session
    # sanity checks
    if msg.opcode != 0x22 || msg.local_session_id != 0 || msg.protocol_id != 0
      tasmota.log("MTR: invalid Pake1 message", 3)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end
    var pake1 = matter.Pake1().parse(msg.raw, msg.app_payload_idx)

    var pA = pake1.pA
    # tasmota.log("MTR: received pA=" + pA.tohex(), 4)
    
    # instanciate SPAKE
    # for testing purpose, we don't send `w1` to make sure
    var spake = crypto.SPAKE2P_Matter(self.device.commissioning_w0, nil, self.device.commissioning_L)

    # generate `y` nonce (not persisted)
    var y = crypto.random(32)                     # 32 bytes random known only by verifier

    # compute pB
    spake.compute_pB(y)
    # tasmota.log("MTR: y=" + y.tohex(), 4)
    # tasmota.log("MTR: pb=" + spake.pB.tohex(), 4)

    # compute ZV
    spake.compute_ZV_verifier(pA)
    # tasmota.log("MTR: Z=" + spake.Z.tohex(), 4)
    # tasmota.log("MTR: V=" + spake.V.tohex(), 4)

    var context = crypto.SHA256()
    context.update(bytes().fromstring(self.Matter_Context_Prefix))
    context.update(session.__Msg1)
    context.update(session.__Msg2)
    var context_hash = context.out()

    # tasmota.log("MTR: Context=" + context_hash.tohex(), 4)

    # add pA
    spake.pA = pA

    spake.set_context(context_hash)
    spake.compute_TT_hash(true)      # `true` to indicate it's Matter variant to SPAKE2+

    # tasmota.log("MTR: ------------------------------", 4)
    # tasmota.log("MTR: Context = " + spake.Context.tohex(), 4)
    # tasmota.log("MTR: M       = " + spake.M.tohex(), 4)
    # tasmota.log("MTR: N       = " + spake.N.tohex(), 4)
    # tasmota.log("MTR: pA      = " + spake.pA.tohex(), 4)
    # tasmota.log("MTR: pB      = " + spake.pB.tohex(), 4)
    # tasmota.log("MTR: Z       = " + spake.Z.tohex(), 4)
    # tasmota.log("MTR: V       = " + spake.V.tohex(), 4)
    # tasmota.log("MTR: w0      = " + spake.w0.tohex(), 4)
    # tasmota.log("MTR: ------------------------------", 4)

    # tasmota.log("MTR: Kmain   =" + spake.Kmain.tohex(), 4)

    # tasmota.log("MTR: KcA     =" + spake.KcA.tohex(), 4)
    # tasmota.log("MTR: KcB     =" + spake.KcB.tohex(), 4)
    # tasmota.log("MTR: K_shared=" + spake.K_shared.tohex(), 4)
    # tasmota.log("MTR: Ke      =" + spake.Ke.tohex(), 4)
    # tasmota.log("MTR: cB=" + spake.cB.tohex(), 4)

    var pake2 = matter.Pake2()
    pake2.pB = spake.pB
    pake2.cB = spake.cB
    # tasmota.log("MTR: pake2: " + matter.inspect(pake2), 4)
    var pake2_raw = pake2.tlv2raw()
    # tasmota.log("MTR: pake2_raw: " + pake2_raw.tohex(), 4)

    session.__spake_cA = spake.cA
    session.__spake_Ke = spake.Ke

    # now package the response message
    var resp = msg.build_response(0x23 #-pake-2-#, true)  # no reliable flag
    var raw = resp.encode_frame(pake2_raw)

    # log the fact that a new commissioning is starting
    tasmota.log(format("MTR: New Commissioning (PASE id=%i) from [%s]:%i", session.__future_local_session_id, session._ip, session._port), 2)

    self.responder.send_response_frame(resp)
    return true
  end

  def parse_Pake3(msg)
    import crypto
    var session = msg.session
    # sanity checks
    if msg.opcode != 0x24 || msg.local_session_id != 0 || msg.protocol_id != 0
      tasmota.log("MTR: invalid Pake3 message", 3)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end
    var pake3 = matter.Pake3().parse(msg.raw, msg.app_payload_idx)

    var cA = pake3.cA
    # tasmota.log("MTR: received cA=" + cA.tohex(), 4)

    # check the value against computed
    if cA != session.__spake_cA
      tasmota.log("MTR: invalid cA received", 3)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end

    # send PakeFinished and compute session key
    var created = tasmota.rtc_utc()
    var session_keys = crypto.HKDF_SHA256().derive(session.__spake_Ke, bytes(), bytes().fromstring(self.SEKeys_Info), 48)
    var I2RKey = session_keys[0..15]
    var R2IKey = session_keys[16..31]
    var AttestationChallenge = session_keys[32..47]

    # tasmota.log("MTR: ******************************", 4)
    # tasmota.log("MTR: session_keys=" + session_keys.tohex(), 4)
    # tasmota.log("MTR: I2RKey      =" + I2RKey.tohex(), 4)
    # tasmota.log("MTR: R2IKey      =" + R2IKey.tohex(), 4)
    # tasmota.log("MTR: AC          =" + AttestationChallenge.tohex(), 4)
    # tasmota.log("MTR: ******************************", 4)

    # StatusReport(GeneralCode: SUCCESS, ProtocolId: SECURE_CHANNEL, ProtocolCode: SESSION_ESTABLISHMENT_SUCCESS)
    self.send_status_report(msg, 0x00, 0x0000, 0x0000, false)

    self.add_session(session.__future_local_session_id, session.__future_initiator_session_id, I2RKey, R2IKey, AttestationChallenge, created)
    return true
  end

  def find_fabric_by_destination_id(destinationId, initiatorRandom)
    import crypto
    # Validate Sigma1 Destination ID, p.162
    # traverse all existing fabrics
    if tasmota.loglevel(4)
      tasmota.log("MTR: SEARCHING: destinationId=" + destinationId.tohex(), 4)
    end
    for fabric : self.device.sessions.fabrics
      if fabric.noc == nil || fabric.fabric_id == nil || fabric.device_id == nil     continue end
      # compute candidateDestinationId, Section 4.13.2.4.1, "Destination Identifier"
      var destinationMessage = initiatorRandom + fabric.get_ca_pub() + fabric.fabric_id + fabric.device_id
      var key = fabric.get_ipk_group_key()
      # tasmota.log("MTR: SIGMA1: destinationMessage=" + destinationMessage.tohex(), 4)
      # tasmota.log("MTR: SIGMA1: key_ipk=" + key.tohex(), 4)
      var h = crypto.HMAC_SHA256(key)
      h.update(destinationMessage)
      var candidateDestinationId = h.out()
      if tasmota.loglevel(4)
        tasmota.log("MTR: SIGMA1: candidateDestinationId=" + candidateDestinationId.tohex(), 4)
      end
      if candidateDestinationId == destinationId
        return fabric
      end
    end
    # TODO if there is only 1 fabric, we can try to use it anyways
    # if size(self.device.sessions.fabrics) == 1
    #   tasmota.log("MTR: *** Could not find fabric, trying only fabric in store", 2)
    #   return self.device.sessions.fabrics[0]
    # end
    return nil
  end

  def parse_Sigma1(msg)
    import crypto
    var session = msg.session
    # sanity checks
    if msg.opcode != 0x30 || msg.local_session_id != 0 || msg.protocol_id != 0
      # tasmota.log("MTR: invalid Sigma1 message", 3)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end
    var sigma1 = matter.Sigma1().parse(msg.raw, msg.app_payload_idx)
    # tasmota.log(format("MTR: sigma1=%s", matter.inspect(sigma1)), 4)

    session.__initiator_pub = sigma1.initiatorEphPubKey

    # find session
    var is_resumption = (sigma1.resumptionID != nil && sigma1.initiatorResumeMIC != nil)
    # tasmota.log(format("MTR: is_resumption=%i", is_resumption ? 1 : 0), 4)
    # TODO disable resumption until fixed
    is_resumption = false

    # Check that it's a resumption
    var session_resumption
    if is_resumption
      session_resumption = self.device.sessions.find_session_by_resumption_id(sigma1.resumptionID)
      # tasmota.log(format("MTR: session_resumption found session=%s session_resumption=%s", matter.inspect(session), matter.inspect(session_resumption)), 4)
      if session_resumption == nil || session_resumption._fabric == nil
        is_resumption = false
      end
    end
    
    # Check that it's a resumption
    if   is_resumption
      # Resumption p.169
      var s1rk_salt = sigma1.initiatorRandom + sigma1.resumptionID
      var s1rk_info = bytes().fromstring("Sigma1_Resume")
      var s1rk = crypto.HKDF_SHA256().derive(session_resumption.shared_secret, s1rk_salt, s1rk_info, 16)

      var Resume1MIC_Nonce = bytes().fromstring("NCASE_SigmaS1")
      var encrypted = sigma1.initiatorResumeMIC[0..-17]
      var tag = sigma1.initiatorResumeMIC[-16..]
      var ec = crypto.AES_CCM(s1rk, Resume1MIC_Nonce, bytes(), size(encrypted), 16)
      var Resume1MICPayload = ec.decrypt(encrypted)
      var decrypted_tag = ec.tag()

      # tasmota.log("****************************************", 4)
      # tasmota.log("MTR: * s1rk              = " + s1rk.tohex(), 4)
      # tasmota.log("MTR: * tag               = " + tag.tohex(), 4)
      # tasmota.log("MTR: * Resume1MICPayload = " + Resume1MICPayload.tohex(), 4)
      # tasmota.log("MTR: * decrypted_tag     = " + decrypted_tag.tohex(), 4)
      # tasmota.log("****************************************", 4)
      if tag == decrypted_tag
        session._fabric = session_resumption._fabric
        session._source_node_id = msg.source_node_id
        session.set_mode_CASE()
        session.__future_initiator_session_id = sigma1.initiator_session_id    # update initiator_session_id
        session.__future_local_session_id = self.device.sessions.gen_local_session_id()
        tasmota.log(format("MTR: +Session   (%6i) from '[%s]:%i'", session.__future_local_session_id, msg.remote_ip, msg.remote_port), 3)

        # Generate and Send Sigma2_Resume
        session.shared_secret = session_resumption.shared_secret
        session.resumption_id = crypto.random(16)     # generate a new resumption id

        # compute S2RK
        var s2rk_info = bytes().fromstring("Sigma2_Resume")
        var s2rk_salt = sigma1.initiatorRandom + session.resumption_id
        var s2rk = crypto.HKDF_SHA256().derive(session.shared_secret, s2rk_salt, s2rk_info, 16)


        # compute Resume2MIC
        var aes = crypto.AES_CCM(s2rk, bytes().fromstring("NCASE_SigmaS2"), bytes(), 0, 16)
        var Resume2MIC = aes.tag()

        var sigma2resume = matter.Sigma2Resume()
        sigma2resume.resumptionID = session.resumption_id
        sigma2resume.responderSessionID = session.__future_local_session_id
        sigma2resume.sigma2ResumeMIC = Resume2MIC

        # tasmota.log("****************************************", 4)
        # tasmota.log("MTR: * s2rk              = " + s2rk.tohex(), 4)
        # tasmota.log("MTR: * s2rk_salt         = " + s2rk_salt.tohex(), 4)
        # tasmota.log("MTR: * new_resumption_id = " + session.resumption_id.tohex(), 4)
        # tasmota.log("MTR: * responderSessionID= " + str(session.__future_local_session_id), 4)
        # tasmota.log("MTR: * sigma2ResumeMIC   = " + Resume2MIC.tohex(), 4)
        # tasmota.log("****************************************", 4)
        # # compute session key, p.178
        var session_keys = crypto.HKDF_SHA256().derive(session.shared_secret #- input key -#,
                                                      sigma1.initiatorRandom + session.resumption_id #- salt -#,
                                                      bytes().fromstring("SessionResumptionKeys") #- info -#,
                                                      48)
        var i2r = session_keys[0..15]
        var r2i = session_keys[16..31]
        var ac = session_keys[32..47]
        var created = tasmota.rtc_utc()

        # tasmota.log("MTR: ******************************", 4)
        # tasmota.log("MTR: I2RKey      =" + i2r.tohex(), 4)
        # tasmota.log("MTR: R2IKey      =" + r2i.tohex(), 4)
        # tasmota.log("MTR: AC          =" + ac.tohex(), 4)
        # tasmota.log("MTR: ******************************", 4)

        var sigma2resume_raw = sigma2resume.tlv2raw()
        session.__Msg1 = nil
        # tasmota.log("MTR: sigma2resume: " + matter.inspect(sigma2resume), 4)
        # tasmota.log("MTR: sigma2resume_raw: " + sigma2resume_raw.tohex(), 4)

        # now package the response message
        var resp = msg.build_response(0x33 #-sigma-2-resume-#, true)
        var raw = resp.encode_frame(sigma2resume_raw)
    
        self.responder.send_response_frame(resp)

        session.close()
        session.set_keys(i2r, r2i, ac, created)
        
        # CASE Session completed, persist it
        session._breadcrumb = 0       # clear breadcrumb
        session.counter_snd_next()    # force a first counter. It's important it's used before set_persist(true) to not have a double save
        session.set_persist(true)     # keep session on flash
        session.set_no_expiration()   # never expire
        session.persist_to_fabric()
        session.save()

        return true
      else
        is_resumption = false
        # fall through normal sigma1 (non-resumption)
      end
    end

    if !is_resumption
      # new CASE session, assign to existing fabric
      var fabric = self.find_fabric_by_destination_id(sigma1.destinationId, sigma1.initiatorRandom)
      session._fabric = fabric

      if session == nil || session._fabric == nil
        tasmota.log("MTR: StatusReport(GeneralCode: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: NO_SHARED_TRUST_ROOTS)", 3)
        self.send_status_report(msg, 0x01, 0x0000, 0x0001, false)
        return false
      end
      session._source_node_id = msg.source_node_id
      session.set_mode_CASE()
  
      session.__future_initiator_session_id = sigma1.initiator_session_id    # update initiator_session_id
      session.__future_local_session_id = self.device.sessions.gen_local_session_id()
      tasmota.log(format("MTR: +Session   (%6i) from '[%s]:%i'", session.__future_local_session_id, msg.remote_ip, msg.remote_port), 3)

      # tasmota.log("MTR: fabric="+matter.inspect(session._fabric), 4)
      # tasmota.log("MTR: no_private_key="+session.get_pk().tohex(), 4)
      # tasmota.log("MTR: noc           ="+session.get_noc().tohex(), 4)
      # if fabric.get_icac()
      #   tasmota.log("MTR: icac          ="+fabric.get_icac().tohex(), 4)
      # end
      # tasmota.log("MTR: root_ca_cert  ="+fabric.get_ca().tohex(), 4)

      # Compute Sigma2, p.162
      session.resumption_id = crypto.random(16)     # generate a new resumption id
      session.__responder_priv = crypto.random(32)
      session.__responder_pub = crypto.EC_P256().public_key(session.__responder_priv)
      # tasmota.log("MTR: ResponderEph_priv  ="+session.__responder_priv.tohex(), 4)
      # tasmota.log("MTR: ResponderEph_pub  ="+session.__responder_pub.tohex(), 4)
      var responderRandom = crypto.random(32)

      session.shared_secret = crypto.EC_P256().shared_key(session.__responder_priv, sigma1.initiatorEphPubKey)
      # tasmota.log("MTR: * shared_secret  = " + session.shared_secret.tohex(), 4)

      var sigma2_tbsdata = matter.TLV.Matter_TLV_struct()
      sigma2_tbsdata.add_TLV(1, matter.TLV.B2, fabric.get_noc())
      sigma2_tbsdata.add_TLV(2, matter.TLV.B2, fabric.get_icac())
      sigma2_tbsdata.add_TLV(3, matter.TLV.B2, session.__responder_pub)
      sigma2_tbsdata.add_TLV(4, matter.TLV.B2, sigma1.initiatorEphPubKey)

      var TBSData2Signature = crypto.EC_P256().ecdsa_sign_sha256(fabric.get_pk(), sigma2_tbsdata.tlv2raw())
      # tasmota.log("****************************************", 4)
      # tasmota.log("MTR: * fabric.get_pk  = " + str(fabric.get_pk()), 4)
      # tasmota.log("MTR: * sigma2_tbsdata  = " + str(sigma2_tbsdata), 4)
      # tasmota.log("MTR: * TBSData2Signature  = " + TBSData2Signature.tohex(), 4)

      var sigma2_tbedata = matter.TLV.Matter_TLV_struct()
      sigma2_tbedata.add_TLV(1, matter.TLV.B2, fabric.get_noc())
      sigma2_tbedata.add_TLV(2, matter.TLV.B2, fabric.get_icac())
      sigma2_tbedata.add_TLV(3, matter.TLV.B2, TBSData2Signature)
      sigma2_tbedata.add_TLV(4, matter.TLV.B2, session.resumption_id)

      # compute TranscriptHash = Crypto_Hash(message = Msg1)
      # tasmota.log("****************************************", 4)
      session.__Msg1 = sigma1.Msg1
      # tasmota.log("MTR: * resumptionid  = " + session.resumption_id.tohex(), 4)
      # tasmota.log("MTR: * MSG1          = " + session.__Msg1.tohex(), 4)
      var TranscriptHash = crypto.SHA256().update(session.__Msg1).out()
      # tasmota.log("MTR: TranscriptHash =" + TranscriptHash.tohex(), 4)

      # Compute S2K, p.175
      var s2k_info = bytes().fromstring(self.S2K_Info)
      var s2k_salt = fabric.get_ipk_group_key() + responderRandom + session.__responder_pub + TranscriptHash

      var s2k = crypto.HKDF_SHA256().derive(session.shared_secret, s2k_salt, s2k_info, 16)
      # tasmota.log("MTR: * SharedSecret  = " + session.shared_secret.tohex(), 4)
      # tasmota.log("MTR: * s2k_salt      = " + s2k_salt.tohex(), 4)
      # tasmota.log("MTR: * s2k           = " + s2k.tohex(), 4)

      var sigma2_tbedata_raw = sigma2_tbedata.tlv2raw()
      # tasmota.log("MTR: * TBEData2Raw   = " + sigma2_tbedata_raw.tohex(), 4)
      # // `AES_CCM.init(secret_key:bytes(16 or 32), iv:bytes(7..13), aad:bytes(), data_len:int, tag_len:int) -> instance`

      var aes = crypto.AES_CCM(s2k, bytes().fromstring(self.TBEData2_Nonce), bytes(), size(sigma2_tbedata_raw), 16)
      var TBEData2Encrypted = aes.encrypt(sigma2_tbedata_raw) + aes.tag()
      # tasmota.log("MTR: * TBEData2Enc   = " + TBEData2Encrypted.tohex(), 4)
      # tasmota.log("****************************************", 4)

      var sigma2 = matter.Sigma2()
      sigma2.responderRandom = responderRandom
      sigma2.responderSessionId = session.__future_local_session_id
      sigma2.responderEphPubKey = session.__responder_pub
      sigma2.encrypted2 = TBEData2Encrypted
      # tasmota.log("MTR: sigma2: " + matter.inspect(sigma2), 4)
      var sigma2_raw = sigma2.tlv2raw()
      session.__Msg2 = sigma2_raw
      # tasmota.log("MTR: sigma2_raw: " + sigma2_raw.tohex(), 4)
  
      # now package the response message
      var resp = msg.build_response(0x31 #-sigma-2-#, true)  # no reliable flag
      var raw = resp.encode_frame(sigma2_raw)
  
      # log the fact that a new connection is starting
      tasmota.log(format("MTR: New Connection (CASE id=%i) from [%s]:%i", session.__future_local_session_id, session._ip, session._port), 2)
      
      self.responder.send_response_frame(resp)
      return true
    end

    return true
  end

  def parse_Sigma3(msg)
    import crypto
    # sanity checks
    if msg.opcode != 0x32 || msg.local_session_id != 0 || msg.protocol_id != 0
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end
    var session = msg.session
    var sigma3 = matter.Sigma3().parse(msg.raw, msg.app_payload_idx)

    # tasmota.log("****************************************", 4)
    # compute TranscriptHash = Crypto_Hash(message = Msg1 || Msg2)
    var TranscriptHash = crypto.SHA256().update(session.__Msg1).update(session.__Msg2).out()
    # tasmota.log("MTR: * session       = " + str(session), 4)
    # tasmota.log("MTR:   .ipk_epoch_key=" + str(session.get_ipk_epoch_key()), 4)
    # tasmota.log("MTR:   .fabric_compr = " + str(session.get_fabric_compressed()), 4)
    # tasmota.log("MTR: * ipk_group_key = " + session.get_ipk_group_key().tohex(), 4)
    # tasmota.log("MTR: * TranscriptHash= " + TranscriptHash.tohex(), 4)

    var s3k_info = bytes().fromstring(self.S3K_Info)
    var s3k = crypto.HKDF_SHA256().derive(session.shared_secret, session.get_ipk_group_key() + TranscriptHash, s3k_info, 16)

    # tasmota.log("****************************************", 4)
    # tasmota.log("MTR: * s3k_salt      = " + (session.get_ipk_group_key() + TranscriptHash).tohex(), 4)
    # tasmota.log("MTR: * s3k           = " + s3k.tohex(), 4)
    # tasmota.log("****************************************", 4)

    # decrypt
    var encrypted = sigma3.TBEData3Encrypted[0..-17]
    var tag = sigma3.TBEData3Encrypted[-16..]
    var ec = crypto.AES_CCM(s3k, bytes().fromstring(self.TBEData3_Nonce), bytes(), size(encrypted), 16)
    var TBEData3 = ec.decrypt(encrypted)
    var TBETag3 = ec.tag()
    # tasmota.log("MTR: * TBEData3      = " + TBEData3.tohex(), 4)
    # tasmota.log("MTR: * TBETag3       = " + TBETag3.tohex(), 4)
    # tasmota.log("MTR: * tag_sent      = " + tag.tohex(), 4)
    # tasmota.log("****************************************", 4)

    if TBETag3 != tag
      tasmota.log("MTR: Tag don't match", 3)
      tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 3)
      self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      return false
    end

    var TBEData3TLV = matter.TLV.parse(TBEData3)
    # tasmota.log("MTR: * TBEData3TLV   = " + str(TBEData3TLV), 4)
    var initiatorNOC = TBEData3TLV.findsubval(1)
    var initiatorICAC = TBEData3TLV.findsubval(2)
    var ec_signature = TBEData3TLV.findsubval(3)
    # tasmota.log("MTR: * initiatorNOC  = " + str(initiatorNOC), 4)
    # tasmota.log("MTR: * initiatorICAC = " + str(initiatorICAC), 4)
    # tasmota.log("MTR: * ec_signature  = " + str(ec_signature), 4)
    # Success = Crypto_VerifyChain(certificates = [TBEData3.initiatorNOC, TBEData3.initiatorICAC, TrustedRCAC]), when TBEData3.initiatorICAC is present
    # TODO
    var initiatorNOCTLV = matter.TLV.parse(initiatorNOC)
    # tasmota.log("MTR: initiatorNOCTLV = " + str(initiatorNOCTLV), 4)
    var initiatorNOCPubKey = initiatorNOCTLV.findsubval(9)
    var initiatorNOCListDN = initiatorNOCTLV.findsub(6)
    var initiatorFabricId = initiatorNOCListDN.findsubval(17)
    if type(initiatorFabricId) == 'int'   session.peer_node_id = int64.fromu32(initiatorFabricId).tobytes() else session.peer_node_id = initiatorFabricId.tobytes() end
    # tasmota.log("MTR: initiatorFabricId="+str(session.peer_node_id), 4)

    var sigma3_tbs = matter.TLV.Matter_TLV_struct()
    sigma3_tbs.add_TLV(1, matter.TLV.B1, initiatorNOC)
    sigma3_tbs.add_TLV(2, matter.TLV.B1, initiatorICAC)
    sigma3_tbs.add_TLV(3, matter.TLV.B1, session.__initiator_pub)
    sigma3_tbs.add_TLV(4, matter.TLV.B1, session.__responder_pub)
    # tasmota.log("MTR: * sigma3_tbs    = " + str(sigma3_tbs), 4)
    var sigma3_tbs_raw = sigma3_tbs.tlv2raw()
    # tasmota.log("MTR: * sigma3_tbs_raw= " + sigma3_tbs_raw.tohex(), 4)

    # tasmota.log("MTR: * initiatorNOCPubKey= " + initiatorNOCPubKey.tohex(), 4)
    # tasmota.log("MTR: * ec_signature      = " + ec_signature.tohex(), 4)
    # tasmota.log("****************************************", 4)

    # `crypto.EC_P256().ecdsa_verify_sha256(public_key:bytes(65), message:bytes(), hash:bytes()) -> bool`
    var sigma3_tbs_valid = crypto.EC_P256().ecdsa_verify_sha256(initiatorNOCPubKey, sigma3_tbs_raw, ec_signature)

    if !sigma3_tbs_valid
      tasmota.log("MTR: sigma3_tbs does not have a valid signature", 2)
      tasmota.log("MTR: ******************* Invalid signature, trying anyways", 2)
      # tasmota.log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 2)
      # self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
      # return false
    else
      # All good, compute new keys
      tasmota.log("MTR: Sigma3 verified, computing new keys", 3)
    end

    TranscriptHash = crypto.SHA256().update(session.__Msg1).update(session.__Msg2).update(sigma3.Msg3).out()
    # tasmota.log("MTR: * __Msg1            = " + session.__Msg1.tohex(), 4)
    # tasmota.log("MTR: * __Msg2            = " + session.__Msg2.tohex(), 4)
    # tasmota.log("MTR: * __Msg3            = " + sigma3.Msg3.tohex(), 4)
    # tasmota.log("MTR: * TranscriptHash    = " + TranscriptHash.tohex(), 4)
    # we can now free __Msg1 and __Msg2
    session.__Msg1 = nil
    session.__Msg2 = nil

    # tasmota.log("MTR: ******************************", 4)
    # tasmota.log("MTR: shared_secret =" + session.shared_secret.tohex(), 4)
    # tasmota.log("MTR: ipk + hash    =" + (session.get_ipk_group_key() + TranscriptHash).tohex(), 4)
    # compute session key
    var session_keys = crypto.HKDF_SHA256().derive(session.shared_secret #- input key -#,
                                                   session.get_ipk_group_key() + TranscriptHash #- salt -#,
                                                   bytes().fromstring(self.SEKeys_Info) #- info -#,
                                                   48)
    var i2r = session_keys[0..15]
    var r2i = session_keys[16..31]
    var ac = session_keys[32..47]
    var created = tasmota.rtc_utc()

    # tasmota.log("MTR: ******************************", 4)
    # tasmota.log("MTR: I2RKey      =" + i2r.tohex(), 4)
    # tasmota.log("MTR: R2IKey      =" + r2i.tohex(), 4)
    # tasmota.log("MTR: AC          =" + ac.tohex(), 4)
    # tasmota.log("MTR: ******************************", 4)

    # StatusReport(GeneralCode: SUCCESS, ProtocolId: SECURE_CHANNEL, ProtocolCode: SESSION_ESTABLISHMENT_SUCCESS)
    self.send_status_report(msg, 0x00, 0x0000, 0x0000, true)

    session.close()
    session.set_keys(i2r, r2i, ac, created)
    
    # CASE Session completed, persist it
    session._breadcrumb = 0          # clear breadcrumb
    session.counter_snd_next()    # force a first counter. It's important it's used before set_persist(true) to not have a double save
    session.set_persist(true)     # keep session on flash
    session.set_no_expiration()   # never expire
    session.persist_to_fabric()
    session.save()

    return true
  end

  #############################################################
  # placeholder, nothing to run for now
  def parse_StatusReport(msg)
    var session = msg.session
    tasmota.log("MTR: >Status    "+msg.raw[msg.app_payload_idx..].tohex(), 3)
    return false      # we don't explicitly ack the message
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
