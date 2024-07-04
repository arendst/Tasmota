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
#@ solidify:Matter_IM_ReportData_Pull,weak
#@ solidify:Matter_IM_ReportDataSubscribed_Pull,weak
#@ solidify:Matter_IM_SubscribedHeartbeat,weak
#@ solidify:Matter_IM_SubscribeResponse_Pull,weak

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
  var finishing                       # we have sent all packet, just wait for a final Ack
  var finished                        # if true, the message is removed from the queue
  var data                            # TLV data of the response (if any)
  var last_counter                    # counter value of last sent packet (to match ack)

  #################################################################################
  # build a response message stub
  #
  # Args:
  #   - msg: the message object
  #   - opcode: (int) the Matter opcode of the response
  #   - reliable: (bool) if true, then we send the response as reliable, i.e. we expect a Ack to confirm it was received
  def init(msg, opcode, reliable)
    self.reset(msg, opcode, reliable)
  end

  #################################################################################
  def reset(msg, opcode, reliable)
    self.resp = (msg != nil) ? msg.build_response(opcode, reliable) : nil # is nil for spontaneous reports
    self.ready = true                # by default send immediately
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT
    self.last_counter = 0            # avoid `nil` value
    self.finishing = false
    self.finished = false
    self.data = nil
  end

  #################################################################################
  # the message is being removed due to expiration
  def reached_timeout()
    # log(f"MTR: IM_Message reached_timeout exch={self.resp.exchange_id}", 3)
  end

  #################################################################################
  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def ack_received(msg)
    # log(f"MTR: IM_Message ack_received exch={self.resp.exchange_id} {self.finishing=} {self.finished=}", 3)
    if self.finishing                 # if finishing, we are waiting for final Ack before removing from queue
      self.finished = true            # remove exchange 
    else
      self.expiration = tasmota.millis() + self.MSG_TIMEOUT     # else give more time to the timer
    end
    return false                      # return false to indicate that we didn't answer ourselves
  end

  #################################################################################
  # Status Report OK received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    # log(f"MTR: IM_Message status_ok_received exch={self.resp.exchange_id}", 3)
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT     # give more time
    if msg
      self.resp = msg.build_response(self.resp.opcode, self.resp.x_flag_r, self.resp)   # update packet
    end
    self.ready = true
    return true
  end

  #################################################################################
  # we received an ACK error, do any necessary cleaning
  #
  # Arg:
  #   - msg: the message received
  #
  # No return value
  def status_error_received(msg)
  end

  #################################################################################
  # get the exchange-id for this message
  #
  # No return value
  def get_exchangeid()
    return self.resp.exchange_id
  end

  #################################################################################
  # default responder for data
  #
  # This is the main entry point for seding the next response.
  # This is called only when `ready` is `true`
  #
  # Arg:
  #   - responder: instance of MessageHandler to create the response object
  def send_im(responder)
    # log(f"MTR: IM_Message send_im exch={self.resp.exchange_id}", 3)
    # if !self.ready   return false  end    # we're not supposed to be called if ready is false - dead code
    var resp = self.resp
    var data_raw = self.data.to_TLV().tlv2raw()    # payload in cleartext
    resp.encode_frame(data_raw)    # payload in cleartext
    resp.encrypt()
    if tasmota.loglevel(4)
      log(f"MTR: <snd       ({resp.session.local_session_id:6i}) id={resp.message_counter} exch={resp.exchange_id} rack={resp.ack_message_counter}", 4)
    end
    responder.send_response_frame(resp)
    self.last_counter = resp.message_counter
    self.finishing = true              # we wait for final ack
  end

end
matter.IM_Message = Matter_IM_Message

#################################################################################
# Matter_IM_Status
#
# Send a simple Status Message
#################################################################################
class Matter_IM_Status : Matter_IM_Message

  #################################################################################
  def init(msg, status)
    super(self).init(msg, 0x01 #-Status Response-#, true #-reliable-#)
    var sr = matter.StatusResponseMessage()
    sr.status = status
    self.data = sr              # prepare the context in `self.data` that will be sent
  end
end
matter.IM_Status = Matter_IM_Status

#################################################################################
# Matter_IM_InvokeResponse
#
# Send a simple Status Message
#################################################################################
class Matter_IM_InvokeResponse : Matter_IM_Message

  #################################################################################
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

  #################################################################################
  def init(msg, data)
    super(self).init(msg, 0x07 #-Write Response-#, true)
    self.data = data
  end

end
matter.IM_WriteResponse = Matter_IM_WriteResponse

#################################################################################
# Matter_IM_ReportData_Pull
#
# Report Data for a Read Request
#
# This version pull the attributes in lazy mode, only when response is computed
#################################################################################
class Matter_IM_ReportData_Pull : Matter_IM_Message
  static var MAX_MESSAGE = 1200       # max bytes size for a single TLV worklaod
                                      # the maximum MTU is 1280, which leaves 80 bytes for the rest of the message
                                      # section 4.4.4 (p. 114)
  # note: `self.data` (bytes or nil) is containing any remaining responses that could not fit in previous packets
  var generator_or_arr                # a PathGenerator or an array of PathGenerator
  var event_generator_or_arr          # a EventGenerator, or array of EventGenerator, or nil
  var subscription_id                 # if not `nil`, subscription_id in response
  var suppress_response               # if not `nil`, suppress_response attribute
  var data_ev                         # left-overs of events, mirroring of data for attributes

  #################################################################################
  #
  # Args
  #   - msg: message object
  #   - ctx_generator_or_arr: generator(s) for attributes (array, single instance, or nil)
  #   - event_generator_or_arr: generator(s) for events (array, single instance, or nil)
  def init(msg, ctx_generator_or_arr, event_generator_or_arr)
    super(self).init(msg, 0x05 #-Report Data-#, true)
    self.generator_or_arr = ctx_generator_or_arr
    self.event_generator_or_arr = event_generator_or_arr
  end

  #################################################################################
  # Status Report OK received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    super(self).status_ok_received(msg)
    if !self.finishing
      msg.session._message_handler.send_encrypted_ack(msg, false #-not reliable-#)
    end
    return true
  end

  #################################################################################
  # set_subscription_id
  #
  # Sets the SubscriptionId of the exchange (to be used by subclasses)
  def set_subscription_id(subscription_id)
    self.subscription_id = subscription_id
  end

  #################################################################################
  # set_suppress_response
  #
  # Sets the SuppressReponse attribute (bool or nil)
  #
  # If false, the controller must respond with StatusReport
  # If true, the controller must not respond (only a Ack to confirm reliable message)
  # `nil` means the default value which is `false` as per Matter spec
  def set_suppress_response(suppress_response)
    self.suppress_response = suppress_response
  end

  #################################################################################
  # default responder for data
  def send_im(responder)
    # log(format(">>>: Matter_IM_ReportData_Pull send_im exch=%i ready=%i", self.resp.exchange_id, self.ready ? 1 : 0), 3)
    var resp = self.resp                                # response frame object
    var data = (self.data != nil) ? self.data : bytes() # bytes() object of the TLV encoded response
    self.data = nil                                     # we remove the data that was saved for next packet

    var not_full = true                                 # marker used to exit imbricated loops
    var debug = responder.device.debug                  # set debug flag in local variable to ease access below

    var data_ev = (self.data_ev != nil) ? self.data_ev : ((self.event_generator_or_arr != nil) ? bytes() : nil)  # bytes for events or nil if no event generators
                                                        # if event_generator_or_arr != nil then data_ev contains a bytes() object

    ########## Attributes
    while not_full && (self.generator_or_arr != nil)
      # get the current generator (first element of list or single object)
      var current_generator = isinstance(self.generator_or_arr, list) ? self.generator_or_arr[0] : self.generator_or_arr
      # log(f">>>: ReportData_Pull send_im start {current_generator.path_in_endpoint}/{current_generator.path_in_cluster}/{current_generator.path_in_attribute}",3)

      var ctx
      while not_full && (ctx := current_generator.next_attribute())   # 'not_full' must be first to avoid removing an item when we don't want
        # log(f">>>: ReportData_Pull {ctx=}", 3)
        var force_log = current_generator.is_direct() || debug
        var elt_bytes = responder.im.read_single_attribute_to_bytes(current_generator.get_pi(), ctx, resp.session, force_log)   # TODO adapt no_log
        if (elt_bytes == nil)   continue    end         # silently ignored, iterate to next
        # check if we overflow
        if (size(data) + size(elt_bytes) > self.MAX_MESSAGE)
          self.data = elt_bytes                         # save response for later
          not_full = false
        else
          data.append(elt_bytes)                        # append response since we have enough room
        end
      end

      # if we are here, then we exhausted the current generator, and we need to move to the next one
      if not_full
        # log(f">>>: ReportData_Pull remove current generator",3)
        if isinstance(self.generator_or_arr, list)
          self.generator_or_arr.remove(0)               # remove first element
          if size(self.generator_or_arr) == 0
            self.generator_or_arr = nil                 # empty array so we put nil
          end
        else
          self.generator_or_arr = nil                   # there was a single entry, so replace with nil
        end
      end

    end

    # do we have left-overs from events, i.e. self.data_ev is non-empty bytes()
    if not_full && (self.data_ev != nil) && (size(self.data_ev) > 0)
      # try to add self.data_ev
      if (size(data) + size(self.data_ev) > self.MAX_MESSAGE)
        not_full = false                              # skip until next iteration
        data_ev = nil                                 # don't output any value
      else
        self.data_ev = nil                            # we could add it, so remove from left-overs
      end
    end

    ########## Events
    while not_full && (self.event_generator_or_arr != nil)
      # get the current generator (first element of list or single object)
      var current_generator = isinstance(self.event_generator_or_arr, list) ? self.event_generator_or_arr[0] : self.event_generator_or_arr
      # now events
      var ev
      while not_full && (ev := current_generator.next_event())   # 'not_full' must be first to avoid removing an item when we don't want
        # conditional logging
        if debug && tasmota.loglevel(3)
          var data_str = ''
          if (ev.data0 != nil)   data_str = " - " + str(ev.data0)             end
          if (ev.data1 != nil)   data_str += ", " + str(ev.data1)     end
          if (ev.data2 != nil)   data_str += ", " + str(ev.data2)     end
          log(f"MTR: >Read_Event({resp.session.local_session_id:6i}|{ev.event_no:8s}) [{ev.endpoint:02X}]{ev.cluster:04X}/{ev.event_id:02X}{data_str}", 3)
        end
        # send bytes
        var ev_bytes = ev.to_raw_bytes()
        if (size(data) + size(data_ev) + size(ev_bytes) > self.MAX_MESSAGE)
          self.data_ev = ev_bytes                     # save for next iteration
          not_full = false
        else
          data_ev.append(ev_bytes)                    # append response since we have enough room
        end
      end

      # if we are here, then we exhausted the current generator, and we need to move to the next one
      if not_full
        if isinstance(self.event_generator_or_arr, list)
          self.event_generator_or_arr.remove(0)               # remove first element
          if size(self.event_generator_or_arr) == 0
            self.event_generator_or_arr = nil                 # empty array so we put nil
          end
        else
          self.event_generator_or_arr = nil                   # there was a single entry, so replace with nil
        end
      end

    end

    ########## Response
    # prepare the response
    # Manually craft payload
    var more_chunked_messages = (self.data != nil) || (self.data_ev != nil)
    var raw = bytes(self.MAX_MESSAGE)

    # open struct ReportDataMessage
    raw.add(0x15, 1)                  # add 15
    # open sturct EventDataIB
    if (self.subscription_id != nil)
      raw.add(0x2500, -2)               # add 2500
      raw.add(self.subscription_id, 2)  # add subscription_id as 16 bits
    end
    # do we have attributes?
    if (data != nil && size(data) > 0)
      raw.add(0x3601, -2)             # add 3601
      raw.append(data)
      raw.add(0x18, 1)           # add 18
    end
    # do we have events?
    if (data_ev != nil && size(data_ev) > 0)
      raw.add(0x3602, -2)             # add 3601
      raw.append(data_ev)
      raw.add(0x18, 1)           # add 18
    end
    # MoreChunkedMessages
    if more_chunked_messages    # we got more data to send
      raw.add(0x2903, -2)             # add 2903
    else
      raw.add(0x2803, -2)             # add 2803
    end
    # SuppressResponse
    if (self.suppress_response != nil)
      if self.suppress_response
        raw.add(0x2904, -2)             # add 2904
      else
        raw.add(0x2804, -2)             # add 2804
      end
    end
    # InteractionModelRevision
    raw.add(0x24FF, -2)               # add 24FF
    raw.add(0x01, 1)                  # add 01
    # close struct ReportDataMessage
    raw.add(0x18, 1)                  # add 18
    # log(f">>>: {raw.tohex()}", 3)

    # ##### Previous code
    # var ret = matter.ReportDataMessage()
    # ret.subscription_id = self.subscription_id
    # ret.suppress_response = self.suppress_response
    # if (data != nil && size(data) > 0)
    #   ret.attribute_reports = [data]
    # end
    # if (data_ev != nil && size(data_ev) > 0)
    #   ret.event_reports = [data_ev]
    # end
    # ret.more_chunked_messages = (self.data != nil) || (self.data_ev != nil)    # we got more data to send
    # var encoded_tlv = ret.to_TLV().tlv2raw(bytes(self.MAX_MESSAGE))    # takes time
    # resp.encode_frame(encoded_tlv)    # payload in cleartext, pre-allocate max buffer
    # log(f">>>: {encoded_tlv.tohex()}", 3)
    # ##### Previous code

    resp.encode_frame(raw)    # payload in cleartext, pre-allocate max buffer
    resp.encrypt()
    # log(format("MTR: <snd       (%6i) id=%i exch=%i rack=%s", resp.session.local_session_id, resp.message_counter, resp.exchange_id, resp.ack_message_counter), 4)
    responder.send_response_frame(resp)
    self.last_counter = resp.message_counter

    if more_chunked_messages                 # we have more to send
      self.ready = false    # wait for Status Report before continuing sending
      # keep alive
    else
      # log(f">>>: ReportData_Pull finished",3)
      self.finishing = true         # finishing, remove after final Ack
    end

  end

end
matter.IM_ReportData_Pull = Matter_IM_ReportData_Pull

#################################################################################
# Matter_IM_ReportDataSubscribed_Pull
#
# Main difference is that we are the spontaneous initiator
#################################################################################
class Matter_IM_ReportDataSubscribed_Pull : Matter_IM_ReportData_Pull
  #   inherited from Matter_IM_Message
  # static var MSG_TIMEOUT = 5000       # 5s
  # var expiration                      # expiration time for the reporting 
  # var resp                            # response Frame object
  # var ready                           # bool: ready to send (true) or wait (false)
  # var finished                          # if true, the message is removed from the queue
  # var data                            # TLV data of the response (if any)
  # var last_counter                    # counter value of last sent packet (to match ack)
  #   inherited from Matter_IM_ReportData_Pull
  # static var MAX_MESSAGE = 1200       # max bytes size for a single TLV worklaod
  # var generator_or_arr                # a PathGenerator or an array of PathGenerator
  # var event_generator_or_arr          # a EventGenerator, or array of EventGenerator, or nil
  # var subscription_id                 # if not `nil`, subscription_id in response
  var sub                         # subscription object
  var report_data_phase           # true during reportdata

  #################################################################################
  def init(message_handler, session, ctx_generator_or_arr, event_generator_or_arr, sub)
    super(self).init(nil, ctx_generator_or_arr, event_generator_or_arr)     # send msg=nil to avoid creating a reponse
    # we need to initiate a new virtual response, because it's a spontaneous message
    self.resp = matter.Frame.initiate_response(message_handler, session, 0x05 #-Report Data-#, true)
    #
    self.sub = sub
    self.report_data_phase = true
    self.set_subscription_id(sub.subscription_id)
    self.set_suppress_response(false)
  end

  #################################################################################
  def reached_timeout()
    # log(f"MTR: IM_ReportDataSubscribed_Pull reached_timeout()", 3)
    self.sub.remove_self()
  end

  #################################################################################
  # ack received, confirm the heartbeat
  def ack_received(msg)
    # log(f"MTR: IM_ReportDataSubscribed_Pull ack_received sub={self.sub.subscription_id}", 3)
    super(self).ack_received(msg)
    if !self.report_data_phase
      # if ack is received while all data is sent, means that it finished without error
      if self.sub.is_keep_alive   # only if keep-alive, for normal reports, re_arm is called at last StatusReport
        self.sub.re_arm()           # signal that we can proceed to next sub report
      end
      return false                      # proceed to calling send() which removes the message
    else
      return false                      # do nothing
    end
  end

  #################################################################################
  # we received an ACK error, remove subscription
  def status_error_received(msg)
    # log(f"MTR: IM_ReportDataSubscribed_Pull status_error_received sub={self.sub.subscription_id} exch={self.resp.exchange_id}", 3)
    self.sub.remove_self()
  end

  #################################################################################
  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    # log(f"MTR: IM_ReportDataSubscribed_Pull status_ok_received sub={self.sub.subscription_id} exch={self.resp.exchange_id}", 3)
    if self.report_data_phase
      return super(self).status_ok_received(msg)
    else
      self.sub.re_arm()                       # always re_arm at last StatusReport. The only case where it does not happen is during keep-alive, hence we need to lookg for Ack (see above)
      super(self).status_ok_received(nil)
      return false                            # let the caller to the ack
    end
  end

  #################################################################################
  # returns true if transaction is complete (remove object from queue)
  # default responder for data
  def send_im(responder)
    # log(format("MTR: IM_ReportDataSubscribed_Pull send sub=%i exch=%i ready=%i", self.sub.subscription_id, self.resp.exchange_id, self.ready ? 1 : 0), 3)
    if !self.ready   return false  end

    if (self.generator_or_arr != nil) || (self.event_generator_or_arr != nil)             # do we have still attributes or events to send
      if self.report_data_phase
        super(self).send_im(responder)
        # log(format("MTR: ReportDataSubscribed::send_im called super finished=%i", self.finished), 3)
        if !self.finishing return end              # ReportData needs to continue
        # ReportData is finished
        self.report_data_phase = false
        self.ready = false
        self.finished = false                     # while a ReadReport would stop here, we continue for subscription
      else
        # send a simple ACK
        var resp = self.resp.build_standalone_ack(false)
        resp.encode_frame()
        resp.encrypt()
        if tasmota.loglevel(4)
          log(format("MTR: <Ack       (%6i) ack=%i id=%i", resp.session.local_session_id, resp.ack_message_counter, resp.message_counter), 4)
        end
        responder.send_response_frame(resp)
        self.last_counter = resp.message_counter
        # self.finished = true
        self.sub.re_arm()           # signal that we can proceed to next sub report
      end

    else
      # simple heartbeat ReportData
      if self.report_data_phase
        super(self).send_im(responder)
        self.report_data_phase = false
      else
        # self.finished = true
        self.sub.re_arm()           # signal that we can proceed to next sub report
      end
    end
  end
end
matter.IM_ReportDataSubscribed_Pull = Matter_IM_ReportDataSubscribed_Pull

#################################################################################
# Matter_IM_SubscribedHeartbeat
#
# Send a subscription heartbeat, which is an empty DataReport with no SuppressResponse
#
# Main difference is that we are the spontaneous initiator
#################################################################################
class Matter_IM_SubscribedHeartbeat : Matter_IM_ReportData_Pull
  var sub                         # subscription object

  #################################################################################
  def init(message_handler, session, sub)
    super(self).init(nil, nil #-no ctx_generator_or_arr-#, nil #-no event_generator_or_arr-#)     # send msg=nil to avoid creating a reponse
    # we need to initiate a new virtual response, because it's a spontaneous message
    self.resp = matter.Frame.initiate_response(message_handler, session, 0x05 #-Report Data-#, true)
    #
    self.sub = sub
    self.set_subscription_id(sub.subscription_id)
    self.set_suppress_response(true)                # as per Matter definition, heartbeat requires no StatusReport, only a simple Ack
  end

  #################################################################################
  # reached_timeout
  #
  # The heartbeat was not acked within 5 seconds, and after all retries,
  # then the controller is not expecting any more answers,
  # remove the subscription 
  def reached_timeout()
    # log(f"MTR: IM_SubscribedHeartbeat reached_timeout()", 3)
    self.sub.remove_self()
  end

  #################################################################################
  # ack received, confirm the heartbeat and remove the packet from the queue
  def ack_received(msg)
    # log(format("MTR: IM_SubscribedHeartbeat ack_received sub=%i", self.sub.subscription_id), 3)
    super(self).ack_received(msg)
    return false                            # no further response
  end

  #################################################################################
  # we received an ACK error, remove subscription
  def status_error_received(msg)
    # log(format("MTR: IM_SubscribedHeartbeat status_error_received sub=%i exch=%i", self.sub.subscription_id, self.resp.exchange_id), 3)
    self.sub.remove_self()
    return false                            # let the caller to the ack
  end

  #################################################################################
  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    # log(format("MTR: IM_SubscribedHeartbeat status_ok_received sub=%i exch=%i", self.sub.subscription_id, self.resp.exchange_id), 3)
    return false                            # let the caller to the ack
  end

  #################################################################################
  # default responder for data
  def send_im(responder)
    # log(format("MTR: IM_SubscribedHeartbeat send sub=%i exch=%i ready=%i", self.sub.subscription_id, self.resp.exchange_id, self.ready ? 1 : 0), 3)
    super(self).send_im(responder)
    self.ready = false
  end
end
matter.IM_SubscribedHeartbeat = Matter_IM_SubscribedHeartbeat

#################################################################################
# Matter_IM_SubscribeResponse_Pull
#
# Report Data for a Read Request - pull (lazy) mode
#################################################################################
class Matter_IM_SubscribeResponse_Pull : Matter_IM_ReportData_Pull
  # inherited
  # static var MAX_MESSAGE = 1200       # max bytes size for a single TLV worklaod
  # var generator_or_arr                # a PathGenerator or an array of PathGenerator
  var sub                         # subscription object
  var report_data_phase           # true during reportdata

  #################################################################################
  def init(msg, ctx_generator_or_arr, event_generator_or_arr, sub)
    super(self).init(msg, ctx_generator_or_arr, event_generator_or_arr)
    self.sub = sub
    self.report_data_phase = true
    self.set_subscription_id(sub.subscription_id)
  end

  #################################################################################
  # default responder for data
  def send_im(responder)
    # log(format("MTR: Matter_IM_SubscribeResponse send sub=%i ready=%i report_data_phase=%s", self.sub.subscription_id, self.ready ? 1 : 0, self.report_data_phase), 3)
    if self.report_data_phase
      super(self).send_im(responder)
      if self.finishing
        # finished reporting of data, we still need to send SubscribeResponseMessage after next StatusReport
        self.report_data_phase = false
        self.finishing = false       # we continue to subscribe response
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
      # log(f"MTR: Send SubscribeResponseMessage sub={self.sub.subscription_id} id={resp.message_counter}", 3)
      self.sub.re_arm()
      self.finishing = true          # remove exchange
    end
  end

  # Status ok received
  def status_ok_received(msg)
    # log(format("MTR: IM_SubscribeResponse status_ok_received sub=%i exch=%i ack=%i last_counter=%i finished=%s", self.sub.subscription_id, self.resp.exchange_id, msg.ack_message_counter ? msg.ack_message_counter : 0 , self.last_counter, self.finished), 3)
    # once we receive ack, open flow for subscriptions
    if tasmota.loglevel(3)
      log(format("MTR: >Sub_OK    (%6i) sub=%i", msg.session.local_session_id, self.sub.subscription_id), 3)
    end
    return super(self).status_ok_received(msg)
    if !self.report_data_phase
      self.finishing = true
    end
  end

end
matter.IM_SubscribeResponse_Pull = Matter_IM_SubscribeResponse_Pull
