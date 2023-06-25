#
# Matter_IM.be - suppport for Matter Interaction Model
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

#@ solidify:Matter_IM,weak

#################################################################################
# Matter_IM class
#################################################################################
class Matter_IM
  var device
  var subs_shop                        # subscriptions shop

  var send_queue                  # list of IM_Message queued for sending as part of exchange-id

  def init(device)
    self.device = device
    self.send_queue = []
    self.subs_shop = matter.IM_Subscription_Shop(self)
  end

  def process_incoming(msg)
    # messages are always TLV, decode payload
    # tasmota.log("MTR: received IM message " + matter.inspect(msg), 3)

    var val = matter.TLV.parse(msg.raw, msg.app_payload_idx)

    # tasmota.log("MTR: IM TLV: " + str(val), 3)

    var InteractionModelRevision = val.findsubval(0xFF)
    # tasmota.log("MTR: InteractionModelRevision=" + (InteractionModelRevision != nil ? str(InteractionModelRevision) : "nil"), 4)

    var opcode = msg.opcode
    if   opcode == 0x01   # Status Response
      return self.process_status_response(msg, val)
    elif opcode == 0x02   # Read Request
      self.send_ack_now(msg)
      return self.process_read_request(msg, val)
    elif opcode == 0x03   # Subscribe Request
      self.send_ack_now(msg)
      return self.subscribe_request(msg, val)
    elif opcode == 0x04   # Subscribe Response
      return self.subscribe_response(msg, val)
    elif opcode == 0x05   # Report Data
      return self.report_data(msg, val)
    elif opcode == 0x06   # Write Request
      self.send_ack_now(msg)
      return self.process_write_request(msg, val)
    elif opcode == 0x07   # Write Response
      return self.process_write_response(msg, val)
    elif opcode == 0x08   # Invoke Request
      self.send_ack_now(msg)
      return self.process_invoke_request(msg, val)
    elif opcode == 0x09   # Invoke Response
      return self.process_invoke_response(msg, val)
    elif opcode == 0x0A   # Timed Request
      return self.process_timed_request(msg, val)
    end

    return false
  end

  #############################################################
  # check whether the ack received is of interest to any
  # current exchange
  #
  # return `true` if handled
  def process_incoming_ack(msg)
    # check if there is an exchange_id interested in receiving this
    var message = self.find_sendqueue_by_exchangeid(msg.exchange_id)
    # tasmota.log(format("MTR: process_incoming_ack exch=%i message=%i", msg.exchange_id, message != nil ? 1 : 0), 4)
    if message
      return message.ack_received(msg)                # dispatch to IM_Message
    end
    return false
  end

  #############################################################
  # send Ack response now and don't enqueue it
  #
  # returns `true` if packet could be sent
  def send_ack_now(msg)
    msg.session._message_handler.send_encrypted_ack(msg, false #-not reliable-#)
  end

  #############################################################
  # send enqueued responses
  #
  # self.send_queue is a list of <Matter_IM_Message>
  # 
  def send_enqueued(responder)
    var idx = 0
    while idx < size(self.send_queue)
      var message = self.send_queue[idx]

      if !message.finish && message.ready
        message.send_im(responder)         # send message
      end

      if message.finish
        tasmota.log("MTR: remove IM message exch="+str(message.resp.exchange_id), 4)
        self.send_queue.remove(idx)
      else
        idx += 1
      end
    end
  end

  #############################################################
  # find in send_queue by exchangeid
  #
  def find_sendqueue_by_exchangeid(exchangeid)
    if exchangeid == nil    return nil end
    var idx = 0
    while idx < size(self.send_queue)
      var message = self.send_queue[idx]
      if message.get_exchangeid() == exchangeid
        return message
      end      
      idx += 1
    end
    return nil
  end

  #############################################################
  # find in send_queue by exchangeid
  #
  def remove_sendqueue_by_exchangeid(exchangeid)
    if exchangeid == nil    return end
    var idx = 0
    while idx < size(self.send_queue)
      if self.send_queue[idx].get_exchangeid() == exchangeid
        self.send_queue.remove(idx)
      else
        idx += 1
      end
    end
  end

  #############################################################
  # Remove any queued message that expired
  #
  def expire_sendqueue()
    var idx = 0
    while idx < size(self.send_queue)
      var message = self.send_queue[idx]
      if tasmota.time_reached(message.expiration)
        message.reached_timeout()
        self.send_queue.remove(idx)
      else
        idx += 1
      end
    end
    return nil
  end

  #############################################################
  # process IM 0x01 Status Response
  #
  # val is the TLV structure
  # or raises an exception
  # return true if we handled the response and ack, false instead
  def process_status_response(msg, val)
    var status = val.findsubval(0, 0xFF)
    var message = self.find_sendqueue_by_exchangeid(msg.exchange_id)
    if status == matter.SUCCESS
      if message
        return message.status_ok_received(msg)         # re-arm the sending of next packets for the same exchange
      else
        tasmota.log(format("MTR: >OK        (%6i) exch=%i not found", msg.session.local_session_id, msg.exchange_id), 4)      # don't show 'SUCCESS' to not overflow logs with non-information
      end
    else
      # error
      tasmota.log(format("MTR: >Status    ERROR = 0x%02X", status), 3)
      if message
        message.status_error_received(msg)
        self.remove_sendqueue_by_exchangeid(msg.exchange_id)
      end
    end
    return false      # we did not ack the message, do it at higher level
  end

  #############################################################
  # Inner code shared between read_attributes and subscribe_request
  #
  # query: `ReadRequestMessage` or `SubscribeRequestMessage`
  def _inner_process_read_request(session, query, no_log)

    ### Inner function to be iterated upon
    # ret is the ReportDataMessage list to send back
    # ctx is the context with endpoint/cluster/attribute
    # direct is true if error is reported, false if error is silently ignored
    #
    # if `pi` is nil, just report the status for ctx.status
    #
    # should return true if answered, false if passing to next handler
    def read_single_attribute(ret, pi, ctx, direct)
      var TLV = matter.TLV
      var attr_name = matter.get_attribute_name(ctx.cluster, ctx.attribute)
      attr_name = attr_name ? " (" + attr_name + ")" : ""
      # Special case to report unsupported item, if pi==nil
      var res = (pi != nil) ? pi.read_attribute(session, ctx) : nil
      var found = true                # stop expansion since we have a value
      var a1_raw                      # this is the bytes() block we need to add to response (or nil)
      if res != nil
        var res_str = str(res)        # get the value with anonymous tag before it is tagged, for logging

        var a1 = matter.AttributeReportIB()
        a1.attribute_data = matter.AttributeDataIB()
        a1.attribute_data.data_version = 1
        a1.attribute_data.path = matter.AttributePathIB()
        a1.attribute_data.path.endpoint = ctx.endpoint
        a1.attribute_data.path.cluster = ctx.cluster
        a1.attribute_data.path.attribute = ctx.attribute
        a1.attribute_data.data = res

        var a1_tlv = a1.to_TLV()
        var a1_len = a1_tlv.encode_len()
        var a1_bytes = bytes(a1_len)        # pre-size bytes() to the actual size
        a1_raw = a1_tlv.tlv2raw(a1_bytes)
        # tasmota.log(format("MTR: guessed len=%i actual=%i '%s'", a1_len, size(a1_raw), a1_raw.tohex()), 2)

        if !no_log
          tasmota.log(format("MTR: >Read_Attr (%6i) %s%s - %s", session.local_session_id, str(ctx), attr_name, res_str), 3)
        end          
      elif ctx.status != nil
        if direct                           # we report an error only if a concrete direct read, not with wildcards
          var a1 = matter.AttributeReportIB()
          a1.attribute_status = matter.AttributeStatusIB()
          a1.attribute_status.path = matter.AttributePathIB()
          a1.attribute_status.status = matter.StatusIB()
          a1.attribute_status.path.endpoint = ctx.endpoint
          a1.attribute_status.path.cluster = ctx.cluster
          a1.attribute_status.path.attribute = ctx.attribute
          a1.attribute_status.status.status = ctx.status

          var a1_tlv = a1.to_TLV()
          var a1_len = a1_tlv.encode_len()
          var a1_bytes = bytes(a1_len)        # pre-size bytes() to the actual size
          a1_raw = a1_tlv.tlv2raw(a1_bytes)

          tasmota.log(format("MTR: >Read_Attr (%6i) %s%s - STATUS: 0x%02X %s", session.local_session_id, str(ctx), attr_name, ctx.status, ctx.status == matter.UNSUPPORTED_ATTRIBUTE ? "UNSUPPORTED_ATTRIBUTE" : ""), 3)
        end
      else
        tasmota.log(format("MTR: >Read_Attr (%6i) %s%s - IGNORED", session.local_session_id, str(ctx), attr_name), 3)
        # ignore if content is nil and status is undefined
        found = false
      end

      # check if we still have enough room in last block
      if a1_raw         # do we have bytes to add, and it's not zero size
        if size(ret.attribute_reports) == 0
          ret.attribute_reports.push(a1_raw)      # push raw binary instead of a TLV
        else    # already blocks present, see if we can add to the latest, or need to create a new block
          var last_block = ret.attribute_reports[-1]
          if size(last_block) + size(a1_raw) <= matter.IM_ReportData.MAX_MESSAGE
            # add to last block
            last_block .. a1_raw
          else
            ret.attribute_reports.push(a1_raw)      # push raw binary instead of a TLV
          end
        end
      end

      return found          # return true if we had a match
    end

    var endpoints = self.device.get_active_endpoints()
    # structure is `ReadRequestMessage` 10.6.2 p.558
    var ctx = matter.Path()

    # prepare the response
    var ret = matter.ReportDataMessage()
    # ret.suppress_response = true
    ret.attribute_reports = []

    for q:query.attributes_requests
      # need to do expansion here
      ctx.endpoint = q.endpoint
      ctx.cluster = q.cluster
      ctx.attribute = q.attribute
      ctx.status = matter.UNSUPPORTED_ATTRIBUTE   #default error if returned `nil`
      
      # expand endpoint
      if ctx.endpoint == nil || ctx.cluster == nil || ctx.attribute == nil
        # we need expansion, log first
        if ctx.cluster != nil && ctx.attribute != nil
          var attr_name = matter.get_attribute_name(ctx.cluster, ctx.attribute)
          tasmota.log(format("MTR: >Read_Attr (%6i) %s", session.local_session_id, str(ctx) + (attr_name ? " (" + attr_name + ")" : "")), 3)
        else
          tasmota.log(format("MTR: >Read_Attr (%6i) %s", session.local_session_id, str(ctx)), 3)
        end
        
      end

      # implement concrete expansion
      self.device.process_attribute_expansion(ctx,
      / pi, ctx, direct -> read_single_attribute(ret, pi, ctx, direct)
      )
    end

    # tasmota.log("MTR: ReportDataMessage=" + str(ret), 3)
    # tasmota.log("MTR: ReportDataMessageTLV=" + str(ret.to_TLV()), 3)

    return ret
  end

  #############################################################
  # process IM 0x02 Read Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_read_request(msg, val)
    var query = matter.ReadRequestMessage().from_TLV(val)
    if query.attributes_requests != nil
      var ret = self._inner_process_read_request(msg.session, query)
      self.send_report_data(msg, ret)
    end

    return true
  end

  #############################################################
  # process IM 0x03 Subscribe Request
  #
  def subscribe_request(msg, val)
    var query = matter.SubscribeRequestMessage().from_TLV(val)

    if !query.keep_subscriptions
      self.subs_shop.remove_by_session(msg.session)      # if `keep_subscriptions`, kill all subscriptions from current session
    end

    # tasmota.log("MTR: received SubscribeRequestMessage=" + str(query), 3)

    var sub = self.subs_shop.new_subscription(msg.session, query)

    # expand a string with all attributes requested
    var attr_req = []
    var ctx = matter.Path()
    for q:query.attributes_requests
      ctx.endpoint = q.endpoint
      ctx.cluster = q.cluster
      ctx.attribute = q.attribute
      attr_req.push(str(ctx))
    end
    tasmota.log(format("MTR: >Subscribe (%6i) %s (min=%i, max=%i, keep=%i) sub=%i",
                              msg.session.local_session_id, attr_req.concat(" "), sub.min_interval, sub.max_interval, query.keep_subscriptions ? 1 : 0, sub.subscription_id), 3)

    var ret = self._inner_process_read_request(msg.session, query, true #-no_log-#)
    # ret is of type `Matter_ReportDataMessage`
    ret.subscription_id = sub.subscription_id     # enrich with subscription id TODO
    self.send_subscribe_response(msg, ret, sub)
    return true
  end

  #############################################################
  # process IM 0x08 Invoke Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_invoke_request(msg, val)
    # structure is `ReadRequestMessage` 10.6.2 p.558
    # tasmota.log("MTR: IM:invoke_request processing start", 4)
    var ctx = matter.Path()

    var query = matter.InvokeRequestMessage().from_TLV(val)
    if query.invoke_requests != nil
      # prepare the response
      var ret = matter.InvokeResponseMessage()
      ret.suppress_response = false
      ret.invoke_responses = []

      for q:query.invoke_requests
        ctx.endpoint = q.command_path.endpoint
        ctx.cluster = q.command_path.cluster
        ctx.command = q.command_path.command
        ctx.status = matter.UNSUPPORTED_COMMAND   #default error if returned `nil`

        var cmd_name = matter.get_command_name(ctx.cluster, ctx.command)
        var ctx_str = str(ctx)                    # keep string before invoking, it is modified by response
        var res = self.device.invoke_request(msg.session, q.command_fields, ctx)
        var params_log = (ctx.log != nil) ? "(" + str(ctx.log) + ") " : ""
        tasmota.log(format("MTR: >Command   (%6i) %s %s %s", msg.session.local_session_id, ctx_str, cmd_name ? cmd_name : "", params_log), ctx.endpoint != 0 ? 2 : 3 #- don't log for endpoint 0 -# )
        ctx.log = nil
        var a1 = matter.InvokeResponseIB()
        if res == true || ctx.status == matter.SUCCESS      # special case, just respond ok
          a1.status = matter.CommandStatusIB()
          a1.status.command_path = matter.CommandPathIB()
          a1.status.command_path.endpoint = ctx.endpoint
          a1.status.command_path.cluster = ctx.cluster
          a1.status.command_path.command = ctx.command
          a1.status.status = matter.StatusIB()
          a1.status.status.status = matter.SUCCESS
          ret.invoke_responses.push(a1)
          tasmota.log(format("MTR: <Replied   (%6i) OK exch=%i", msg.session.local_session_id, msg.exchange_id), 3)
        elif res != nil
          a1.command = matter.CommandDataIB()
          a1.command.command_path = matter.CommandPathIB()
          a1.command.command_path.endpoint = ctx.endpoint
          a1.command.command_path.cluster = ctx.cluster
          a1.command.command_path.command = ctx.command
          a1.command.command_fields = res
          ret.invoke_responses.push(a1)

          cmd_name = matter.get_command_name(ctx.cluster, ctx.command)
          tasmota.log(format("MTR: <Replied   (%6i) %s %s", msg.session.local_session_id, str(ctx), cmd_name ? cmd_name : ""), 3)
        elif ctx.status != nil
          a1.status = matter.CommandStatusIB()
          a1.status.command_path = matter.CommandPathIB()
          a1.status.command_path.endpoint = ctx.endpoint
          a1.status.command_path.cluster = ctx.cluster
          a1.status.command_path.command = ctx.command
          a1.status.status = matter.StatusIB()
          a1.status.status.status = ctx.status
          ret.invoke_responses.push(a1)
          tasmota.log(format("MTR: <Replied   (%6i) Status=0x%02X exch=%i", msg.session.local_session_id, ctx.status, msg.exchange_id), 3)
        else
          tasmota.log(format("MTR: _Ignore    (%6i) exch=%i", msg.session.local_session_id, msg.exchange_id), 3)
          # ignore if content is nil and status is undefined
        end
      end

      # tasmota.log("MTR: invoke_responses="+str(ret.invoke_responses), 4)
      if size(ret.invoke_responses) > 0
        # tasmota.log("MTR: InvokeResponse=" + str(ret), 4)
        # tasmota.log("MTR: InvokeResponseTLV=" + str(ret.to_TLV()), 3)

        self.send_invoke_response(msg, ret)
      else
        return false        # we don't send anything, hence the responder sends a simple packet ack
      end
      return true
    end
  end

  #############################################################
  # process IM 0x04 Subscribe Response
  #
  def subscribe_response(msg, val)
    var query = matter.SubscribeResponseMessage().from_TLV(val)
    # tasmota.log("MTR: received SubscribeResponsetMessage=" + str(query), 4)
    return false
  end

  #############################################################
  # process IM 0x05 ReportData
  #
  def report_data(msg, val)
    var query = matter.ReportDataMessage().from_TLV(val)
    # tasmota.log("MTR: received ReportDataMessage=" + str(query), 4)
    return false
  end

  #############################################################
  # process IM 0x06 Write Request
  #
  def process_write_request(msg, val)
    var query = matter.WriteRequestMessage().from_TLV(val)
    # tasmota.log("MTR: received WriteRequestMessage=" + str(query), 3)

    var suppress_response = query.suppress_response
    # var timed_request = query.timed_request   # TODO not supported
    # var more_chunked_messages = query.more_chunked_messages # TODO not supported

    var endpoints = self.device.get_active_endpoints()

    ### Inner function to be iterated upon
    # ret is the ReportDataMessage list to send back
    # ctx is the context with endpoint/cluster/attribute
    # val is the TLV object containing the value
    # direct is true if error is reported, false if error is silently ignored
    #
    # if `pi` is nil, just report the status for ctx.status
    #
    # should return true if answered, false if failed
    def write_single_attribute(ret, pi, ctx, write_data, direct)
      var attr_name = matter.get_attribute_name(ctx.cluster, ctx.attribute)
      attr_name = attr_name ? " (" + attr_name + ")" : ""
      # tasmota.log(format("MTR: Read Attribute " + str(ctx) + (attr_name ? " (" + attr_name + ")" : ""), 2)
      # Special case to report unsupported item, if pi==nil
      ctx.status = matter.UNSUPPORTED_WRITE
      var res = (pi != nil) ? pi.write_attribute(msg.session, ctx, write_data) : nil
      if res    ctx.status = matter.SUCCESS   end     # if the cb returns true, the request was processed
      if ctx.status != nil
        if direct
          var a1 = matter.AttributeStatusIB()
          a1.path = matter.AttributePathIB()
          a1.status = matter.StatusIB()
          a1.path.endpoint = ctx.endpoint
          a1.path.cluster = ctx.cluster
          a1.path.attribute = ctx.attribute
          a1.status.status = ctx.status

          ret.write_responses.push(a1)
          tasmota.log(format("MTR: Write_Attr %s%s - STATUS: 0x%02X %s", str(ctx), attr_name, ctx.status, ctx.status == matter.SUCCESS ? "SUCCESS" : ""), (ctx.endpoint != 0) ? 2 : 3)
          return true
        end
      else
        tasmota.log(format("MTR: Write_Attr %s%s - IGNORED", str(ctx), attr_name), 3)
        # ignore if content is nil and status is undefined
      end
    end

    # structure is `ReadRequestMessage` 10.6.2 p.558
    # tasmota.log("MTR: IM:write_request processing start", 4)
    var ctx = matter.Path()

    if query.write_requests != nil
      # prepare the response
      var ret = matter.WriteResponseMessage()
      # ret.suppress_response = true
      ret.write_responses = []

      for q:query.write_requests      # q is AttributeDataIB
        var path = q.path
        var write_data = q.data
        # need to do expansion here
        ctx.endpoint = path.endpoint
        ctx.cluster = path.cluster
        ctx.attribute = path.attribute
        ctx.status = matter.UNSUPPORTED_ATTRIBUTE   #default error if returned `nil`
        
        # return an error if the expansion is illegal
        if ctx.cluster == nil || ctx.attribute == nil
          # force INVALID_ACTION reporting
          ctx.status = matter.INVALID_ACTION
          write_single_attribute(ret, nil, ctx, nil, true)
          continue
        end

        # expand endpoint
        if ctx.endpoint == nil
          # we need expansion, log first
          var attr_name = matter.get_attribute_name(ctx.cluster, ctx.attribute)
          tasmota.log("MTR: Write_Attr " + str(ctx) + (attr_name ? " (" + attr_name + ")" : ""), 3)
        end

        # implement concrete expansion
        self.device.process_attribute_expansion(ctx,
        / pi, ctx, direct -> write_single_attribute(ret, pi, ctx, write_data, direct)
        )
      end

      # tasmota.log("MTR: ReportWriteMessage=" + str(ret), 4)
      # tasmota.log("MTR: ReportWriteMessageTLV=" + str(ret.to_TLV()), 3)

      # send the reponse that may need to be chunked if too large to fit in a single UDP message
      if !suppress_response
        self.send_write_response(msg, ret)
      end
    end
    return true
  end

  #############################################################
  # process IM 0x07 Write Response
  #
  def process_write_response(msg, val)
    var query = matter.WriteResponseMessage().from_TLV(val)
    # tasmota.log("MTR: received WriteResponseMessage=" + str(query), 4)
    return false
  end

  #############################################################
  # process IM 0x09 Invoke Response
  #
  def process_invoke_response(msg, val)
    var query = matter.InvokeResponseMessage().from_TLV(val)
    # tasmota.log("MTR: received InvokeResponseMessage=" + str(query), 4)
    return false
  end

  #############################################################
  # process IM 0x0A Timed Request
  #
  def process_timed_request(msg, val)
    var query = matter.TimedRequestMessage().from_TLV(val)
    # tasmota.log("MTR: received TimedRequestMessage=" + str(query), 3)

    tasmota.log(format("MTR: >Command   (%6i) TimedRequest=%i", msg.session.local_session_id, query.timeout), 3)
    
    # Send success status report
    self.send_status(msg, matter.SUCCESS)

    return true
  end

  #############################################################
  # send regular update for data subscribed
  #
  def send_subscribe_update(sub)
    var session = sub.session

    # create a fake read request to feed to the ReportData
    var fake_read = matter.ReadRequestMessage()
    fake_read.fabric_filtered = false
    fake_read.attributes_requests = []

    for ctx: sub.updates
      var p1 = matter.AttributePathIB()
      p1.endpoint = ctx.endpoint
      p1.cluster = ctx.cluster
      p1.attribute = ctx.attribute
      fake_read.attributes_requests.push(p1)
    end

    tasmota.log(format("MTR: <Sub_Data  (%6i) sub=%i", session.local_session_id, sub.subscription_id), 3)
    sub.is_keep_alive = false             # sending an actual data update

    var ret = self._inner_process_read_request(session, fake_read)
    ret.suppress_response = false
    ret.subscription_id = sub.subscription_id

    var report_data_msg = matter.IM_ReportDataSubscribed(session._message_handler, session, ret, sub)
    self.send_queue.push(report_data_msg)           # push message to queue
    self.send_enqueued(session._message_handler)    # and send queued messages now
  end
  
  #############################################################
  # send regular update for data subscribed
  #
  def send_subscribe_heartbeat(sub)
    var session = sub.session
    
    tasmota.log(format("MTR: <Sub_Alive (%6i) sub=%i", session.local_session_id, sub.subscription_id), 3)
    sub.is_keep_alive = true              # sending keep-alive

    # prepare the response
    var ret = matter.ReportDataMessage()
    ret.suppress_response = true
    ret.subscription_id = sub.subscription_id

    var report_data_msg = matter.IM_SubscribedHeartbeat(session._message_handler, session, ret, sub)
    self.send_queue.push(report_data_msg)           # push message to queue
    self.send_enqueued(session._message_handler)    # and send queued messages now
  end
  
  #############################################################
  # send_status
  #
  def send_status(msg, status)
    self.send_queue.push(matter.IM_Status(msg, status))
  end

  #############################################################
  # send_invoke_response
  #
  def send_invoke_response(msg, data)
    self.send_queue.push(matter.IM_InvokeResponse(msg, data))
  end

  #############################################################
  # send_invoke_response
  #
  def send_write_response(msg, data)
    self.send_queue.push(matter.IM_WriteResponse(msg, data))
  end

  #############################################################
  # send_report_data
  #
  def send_report_data(msg, data)
    self.send_queue.push(matter.IM_ReportData(msg, data))
  end

  #############################################################
  # send_report_data
  #
  def send_subscribe_response(msg, data, sub)
    self.send_queue.push(matter.IM_SubscribeResponse(msg, data, sub))
  end

  #############################################################
  # placeholder, nothing to run for now
  def every_second()
    self.expire_sendqueue()
  end

  #############################################################
  # dispatch every 250ms click to sub-objects that need it
  def every_250ms()
    self.subs_shop.every_250ms()
  end

end
matter.IM = Matter_IM

#-

# Unit tests


-#

