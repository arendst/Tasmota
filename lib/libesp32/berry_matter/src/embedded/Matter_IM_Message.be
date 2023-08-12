#
# Matter_IM_Message.be - suppport for Matter Interaction Model messages responses
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

#@ solidify:Matter_IM_Message,weak
#@ solidify:Matter_IM_Status,weak
#@ solidify:Matter_IM_InvokeResponse,weak
#@ solidify:Matter_IM_WriteResponse,weak
#@ solidify:Matter_IM_ReportData,weak
#@ solidify:Matter_IM_ReportDataSubscribed,weak
#@ solidify:Matter_IM_SubscribedHeartbeat,weak
#@ solidify:Matter_IM_SubscribeResponse,weak

#################################################################################
# Matter_IM_Message
#
# Superclass for all IM responses
#################################################################################
class Matter_IM_Message
  static var MSG_TIMEOUT = 5000       # 5s
  var expiration                      # expiration time for the reporting 
  var resp                            # response Frame object
  var ready                           # bool: ready to send (true) or wait (false)
  var finish                          # if true, the message is removed from the queue
  var data                            # TLV data of the response (if any)
  var last_counter                    # counter value of last sent packet (to match ack)

  # build a response message stub
  def init(msg, opcode, reliable)
    self.reset(msg, opcode, reliable)
  end

  def reset(msg, opcode, reliable)
    self.resp = msg.build_response(opcode, reliable)
    self.ready = true                # by default send immediately
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT
    self.last_counter = 0            # avoid `nil` value
    self.finish = false
    self.data = nil
  end

  # the message is being removed due to expiration
  def reached_timeout()
  end

  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def ack_received(msg)
    # tasmota.log("MTR: IM_Message ack_received exch="+str(self.resp.exchange_id), 3)
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT     # give more time
    return false
  end

  # Status Report OK received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    # tasmota.log(format("MTR: IM_Message status_ok_received exch=%i", self.resp.exchange_id), 3)
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT     # give more time
    if msg
      self.resp = msg.build_response(self.resp.opcode, self.resp.x_flag_r, self.resp)   # update packet
    end
    self.ready = true
    return true
  end

  # we received an ACK error, do any necessary cleaning
  def status_error_received(msg)
  end

  # get the exchange-id for this message
  def get_exchangeid()
    return self.resp.exchange_id
  end

  # default responder for data
  def send_im(responder)
    # tasmota.log(format("MTR: IM_Message send_im exch=%i ready=%i", self.resp.exchange_id, self.ready ? 1 : 0), 3)
    if !self.ready   return false  end
    # import debug
    var resp = self.resp
    var data_tlv = self.data.to_TLV()
    # matter.profiler.log(str(data_tlv))
    var data_raw = data_tlv.tlv2raw()    # payload in cleartext
    # matter.profiler.log(data_raw.tohex())
    resp.encode_frame(data_raw)    # payload in cleartext
    resp.encrypt()
    if tasmota.loglevel(4)
      tasmota.log(format("MTR: <snd       (%6i) id=%i exch=%i rack=%s", resp.session.local_session_id, resp.message_counter, resp.exchange_id, resp.ack_message_counter), 4)
    end
    # tasmota.log("MTR: Perf/Send = " + str(debug.counters()), 4)
    responder.send_response_frame(resp)
    self.last_counter = resp.message_counter
    self.finish = true              # by default we remove the packet after it is sent
  end

end
matter.IM_Message = Matter_IM_Message

#################################################################################
# Matter_IM_Status
#
# Send a simple Status Message
#################################################################################
class Matter_IM_Status : Matter_IM_Message

  def init(msg, status)
    super(self).init(msg, 0x01 #-Status Response-#, true #-reliable-#)
    var sr  = matter.StatusResponseMessage()
    sr.status = status
    self.data = sr
  end
end
matter.IM_Status = Matter_IM_Status

#################################################################################
# Matter_IM_InvokeResponse
#
# Send a simple Status Message
#################################################################################
class Matter_IM_InvokeResponse : Matter_IM_Message

  def init(msg, data)
    super(self).init(msg, 0x09 #-Invoke Response-#, true)
    self.data = data
  end
end
matter.IM_InvokeResponse = Matter_IM_InvokeResponse

#################################################################################
# Matter_IM_WriteResponse
#
# Send a simple Status Message
#################################################################################
class Matter_IM_WriteResponse : Matter_IM_Message

  def init(msg, data)
    super(self).init(msg, 0x07 #-Write Response-#, true)
    self.data = data
  end

end
matter.IM_WriteResponse = Matter_IM_WriteResponse

#################################################################################
# Matter_IM_ReportData
#
# Report Data for a Read Request
#################################################################################
class Matter_IM_ReportData : Matter_IM_Message
  static var MAX_MESSAGE = 1200       # max bytes size for a single TLV worklaod
                                      # the maximum MTU is 1280, which leaves 80 bytes for the rest of the message
                                      # section 4.4.4 (p. 114)

  def init(msg, data)
    super(self).init(msg, 0x05 #-Report Data-#, true)
    self.data = data
  end

  # default responder for data
  def send_im(responder)
    # tasmota.log(format("MTR: IM_ReportData send_im exch=%i ready=%i", self.resp.exchange_id, self.ready ? 1 : 0), 3)
    if !self.ready   return false  end
    var resp = self.resp                          # response frame object
    var data = self.data                          # TLV data of the response (if any)
    var was_chunked = data.more_chunked_messages  # is this following a chunked packet?

    # the message were grouped by right-sized binaries upfront, we just need to send one block at time
    var elements = 1                # number of elements added

    # tasmota.log(format("MTR: exch=%i elements=%i msg_sz=%i total=%i", self.get_exchangeid(), elements, msg_sz, sz_attribute_reports), 3)
    var next_elemnts
    if data.attribute_reports != nil
      next_elemnts = data.attribute_reports[elements .. ]
      data.attribute_reports = data.attribute_reports[0 .. elements - 1]
      data.more_chunked_messages = (size(next_elemnts) > 0)
    else
      data.more_chunked_messages = false
    end

    if was_chunked
      # tasmota.log(format("MTR: .Read_Attr next_chunk exch=%i", self.get_exchangeid()), 4)
    end
    if data.more_chunked_messages
      if !was_chunked
        # tasmota.log(format("MTR: .Read_Attr first_chunk exch=%i", self.get_exchangeid()), 4)
      end
      # tasmota.log("MTR: sending TLV" + str(data), 4)
    end

    # print(">>>>> send elements before encode")
    var raw_tlv = self.data.to_TLV()
    # print(">>>>> send elements before encode 2")
    var encoded_tlv = raw_tlv.tlv2raw(bytes(self.MAX_MESSAGE))    # takes time
    # print(">>>>> send elements before encode 3")
    resp.encode_frame(encoded_tlv)    # payload in cleartext, pre-allocate max buffer
    # print(">>>>> send elements after encode")
    resp.encrypt()
    # print(">>>>> send elements after encrypt")
    # tasmota.log(format("MTR: <snd       (%6i) id=%i exch=%i rack=%s", resp.session.local_session_id, resp.message_counter, resp.exchange_id, resp.ack_message_counter), 4)
    responder.send_response_frame(resp)
    self.last_counter = resp.message_counter

    if next_elemnts != nil && size(next_elemnts) > 0
      data.attribute_reports = next_elemnts
      # tasmota.log(format("MTR: to_be_sent_later size=%i exch=%i", size(data.attribute_reports), resp.exchange_id), 4)
      self.ready = false    # wait for Status Report before continuing sending
      # keep alive
    else
      self.finish = true         # finished, remove
    end
  end

end
matter.IM_ReportData = Matter_IM_ReportData


#################################################################################
# Matter_IM_ReportDataSubscribed
#
# Main difference is that we are the spontaneous initiator
#################################################################################
class Matter_IM_ReportDataSubscribed : Matter_IM_ReportData
  var sub                         # subscription object
  var report_data_phase           # true during reportdata

  def init(message_handler, session, data, sub)
    self.resp = matter.Frame.initiate_response(message_handler, session, 0x05 #-Report Data-#, true)
    self.data = data
    self.ready = true                # by default send immediately
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT
    #
    self.sub = sub
    self.report_data_phase = true
  end

  def reached_timeout()
    self.sub.remove_self()
  end

  # ack received, confirm the heartbeat
  def ack_received(msg)
    # tasmota.log(format("MTR: IM_ReportDataSubscribed ack_received sub=%i", self.sub.subscription_id), 3)
    super(self).ack_received(msg)
    if !self.report_data_phase
      # if ack is received while all data is sent, means that it finished without error
      if self.sub.is_keep_alive   # only if keep-alive, for normal reports, re_arm is called at last StatusReport
        self.sub.re_arm()           # signal that we can proceed to next sub report
      end
      return true                       # proceed to calling send() which removes the message
    else
      return false                      # do nothing
    end
  end

  # we received an ACK error, remove subscription
  def status_error_received(msg)
    # tasmota.log(format("MTR: IM_ReportDataSubscribed status_error_received sub=%i exch=%i", self.sub.subscription_id, self.resp.exchange_id), 3)
    self.sub.remove_self()
  end

  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    # tasmota.log(format("MTR: IM_ReportDataSubscribed status_ok_received sub=%i exch=%i", self.sub.subscription_id, self.resp.exchange_id), 3)
    if self.report_data_phase
      return super(self).status_ok_received(msg)
    else
      self.sub.re_arm()                       # always re_arm at last StatusReport. The only case where it does not happen is during keep-alive, hence we need to lookg for Ack (see above)
      super(self).status_ok_received(nil)
      return false                            # let the caller to the ack
    end
  end

  # returns true if transaction is complete (remove object from queue)
  # default responder for data
  def send_im(responder)
    # tasmota.log(format("MTR: IM_ReportDataSubscribed send sub=%i exch=%i ready=%i", self.sub.subscription_id, self.resp.exchange_id, self.ready ? 1 : 0), 3)
    # tasmota.log(format("MTR: ReportDataSubscribed::send_im size(self.data.attribute_reports)=%i ready=%s report_data_phase=%s", size(self.data.attribute_reports), str(self.ready), str(self.report_data_phase)), 3)
    if !self.ready   return false  end
    if size(self.data.attribute_reports) > 0      # do we have still attributes to send
      if self.report_data_phase
        super(self).send_im(responder)
        # tasmota.log(format("MTR: ReportDataSubscribed::send_im called super finish=%i", self.finish), 3)
        if !self.finish return end              # ReportData needs to continue
        # ReportData is finished
        self.report_data_phase = false
        self.ready = false
        self.finish = false                     # while a ReadReport would stop here, we continue for subscription
      else
        # send a simple ACK
        var resp = self.resp.build_standalone_ack(false)
        resp.encode_frame()
        resp.encrypt()
        if tasmota.loglevel(4)
          tasmota.log(format("MTR: <Ack       (%6i) ack=%i id=%i", resp.session.local_session_id, resp.ack_message_counter, resp.message_counter), 4)
        end
        responder.send_response_frame(resp)
        self.last_counter = resp.message_counter
        self.finish = true
      end

    else
      # simple heartbeat ReportData
      if self.report_data_phase
        super(self).send_im(responder)
        self.report_data_phase = false
      else
        self.finish = true
      end
    end
  end
end
matter.IM_ReportDataSubscribed = Matter_IM_ReportDataSubscribed

#################################################################################
# Matter_IM_SubscribedHeartbeat
#
# Send a subscription heartbeat, which is an empty DataReport with no SuppressResponse
#
# Main difference is that we are the spontaneous initiator
#################################################################################
class Matter_IM_SubscribedHeartbeat : Matter_IM_ReportData
  var sub                         # subscription object

  def init(message_handler, session, data, sub)
    self.resp = matter.Frame.initiate_response(message_handler, session, 0x05 #-Report Data-#, true)
    self.data = data
    self.ready = true                # by default send immediately
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT
    #
    self.sub = sub
  end

  def reached_timeout()
    self.sub.remove_self()
  end

  # ack received, confirm the heartbeat
  def ack_received(msg)
    # tasmota.log(format("MTR: Matter_IM_SubscribedHeartbeat ack_received sub=%i", self.sub.subscription_id), 3)
    super(self).ack_received(msg)
    self.finish = true
    return true                       # proceed to calling send() which removes the message
  end

  # we received an ACK error, remove subscription
  def status_error_received(msg)
    # tasmota.log(format("MTR: Matter_IM_SubscribedHeartbeat status_error_received sub=%i exch=%i", self.sub.subscription_id, self.resp.exchange_id), 3)
    self.sub.remove_self()
    return false                            # let the caller to the ack
  end

  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    # tasmota.log(format("MTR: Matter_IM_SubscribedHeartbeat status_ok_received sub=%i exch=%i", self.sub.subscription_id, self.resp.exchange_id), 3)
    return false                            # let the caller to the ack
  end

  # default responder for data
  def send_im(responder)
    # tasmota.log(format("MTR: Matter_IM_SubscribedHeartbeat send sub=%i exch=%i ready=%i", self.sub.subscription_id, self.resp.exchange_id, self.ready ? 1 : 0), 3)
    if !self.ready   return false  end

    super(self).send_im(responder)
    self.ready = false
  end
end
matter.IM_SubscribedHeartbeat = Matter_IM_SubscribedHeartbeat

#################################################################################
# Matter_IM_SubscribeResponse
#
# Report Data for a Read Request
#################################################################################
class Matter_IM_SubscribeResponse : Matter_IM_ReportData
  var sub                         # subscription object
  var report_data_phase           # true during reportdata

  def init(msg, data, sub)
    super(self).init(msg, data)
    self.sub = sub
    self.report_data_phase = true
  end

  # default responder for data
  def send_im(responder)
    # tasmota.log(format("MTR: Matter_IM_SubscribeResponse send sub=%i ready=%i", self.sub.subscription_id, self.ready ? 1 : 0), 3)
    if !self.ready   return false  end
    if self.report_data_phase
      super(self).send_im(responder)
      if self.finish
        # finished reporting of data, we still need to send SubscribeResponseMessage after next StatusReport
        self.report_data_phase = false
        self.finish = false       # we continue to subscribe response
      end
      self.ready = false    # wait for Status Report before continuing sending

    else
      # send the final SubscribeReponse
      var resp = self.resp
      var sr = matter.SubscribeResponseMessage()
      sr.subscription_id = self.sub.subscription_id
      sr.max_interval = self.sub.max_interval

      self.resp.opcode = 0x04 #- Subscribe Response -#
      resp.encode_frame(sr.to_TLV().tlv2raw())    # payload in cleartext
      resp.encrypt()
      responder.send_response_frame(resp)
      self.last_counter = resp.message_counter
      # tasmota.log(format("MTR: Send SubscribeResponseMessage sub=%i id=%i", self.sub.subscription_id, resp.message_counter), 3)
      self.sub.re_arm()
      self.finish = true          # remove exchange
    end
  end

  # Status ok received
  def status_ok_received(msg)
    # tasmota.log(format("MTR: IM_SubscribeResponse status_ok_received sub=%i exch=%i ack=%i last_counter=%i", self.sub.subscription_id, self.resp.exchange_id, msg.ack_message_counter ? msg.ack_message_counter : 0 , self.last_counter), 3)
    # once we receive ack, open flow for subscriptions
    if tasmota.loglevel(3)
      tasmota.log(format("MTR: >Sub_OK    (%6i) sub=%i", msg.session.local_session_id, self.sub.subscription_id), 3)
    end
    return super(self).status_ok_received(msg)
  end
    
end
matter.IM_SubscribeResponse = Matter_IM_SubscribeResponse
