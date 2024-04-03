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

  var send_queue                      # list of IM_Message queued for sending as part of exchange-id

  var read_request_solo               # instance of ReadRequestMessage_solo to optimize single reads
  var invoke_request_solo             # instance of InvokeRequestMessage_solo to optimize single reads
  var tlv_solo                        # instance of Matter_TLV_item for simple responses
  
  def init(device)
    self.device = device
    self.send_queue = []
    self.subs_shop = matter.IM_Subscription_Shop(self)
    self.read_request_solo = matter.ReadRequestMessage_solo()
    self.invoke_request_solo = matter.InvokeRequestMessage_solo()
    self.tlv_solo = matter.TLV.Matter_TLV_item()
  end

  def process_incoming(msg)

    var opcode = msg.opcode

    # Fast-Track processing 
    # first pass is optimized for simple frequent messages and avoids complete decoding of TLV
    if   opcode == 0x02   # Read Request
      var read_request_solo = self.read_request_solo.from_raw(msg.raw, msg.app_payload_idx)
      if read_request_solo != nil
        # tasmota.log(f"MTR: process_incoming {read_request_solo=}")
        return self.process_read_request_solo(msg, read_request_solo)
      end
    elif opcode == 0x08   # Invoke Request
      var invoke_request_solo = self.invoke_request_solo.from_raw(msg.raw, msg.app_payload_idx)
      # tasmota.log(f"MTR: {invoke_request_solo=} {msg.raw[msg.app_payload_idx .. ].tohex()} {msg.app_payload_idx=} {msg.raw.tohex()}")
      if invoke_request_solo != nil
        return self.process_invoke_request_solo(msg, invoke_request_solo)
      end
    end

    # tasmota.log("MTR: received IM message " + matter.inspect(msg), 3)
    var val = matter.TLV.parse(msg.raw, msg.app_payload_idx)

    # tasmota.log("MTR: IM TLV: " + str(val), 3)

    # var InteractionModelRevision = val.findsubval(0xFF)
    # tasmota.log("MTR: InteractionModelRevision=" + (InteractionModelRevision != nil ? str(InteractionModelRevision) : "nil"), 4)

    if   opcode == 0x01   # Status Response
      return self.process_status_response(msg, val)
    elif opcode == 0x02   # Read Request
      # self.send_ack_now(msg)      # to improve latency, we don't automatically Ack on invoke request
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
      # self.send_ack_now(msg)      # to improve latency, we don't automatically Ack on invoke request
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
    if msg == nil   return  end
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
  def _inner_process_read_request(session, query, msg, no_log)

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
      var res = (pi != nil) ? pi.read_attribute(session, ctx, self.tlv_solo) : nil
      var found = true                # stop expansion since we have a value
      var a1_raw_or_list              # contains either a bytes() buffer to append, or a list of bytes(), or nil
      if res != nil
        var res_str = ""
        if !no_log
          res_str = res.to_str_val()  # get the value with anonymous tag before it is tagged, for logging
        end

        # check if too big to encode as a single packet
        if (res.is_list || res.is_array) && res.encode_len() > matter.IM_ReportData.MAX_MESSAGE
          # tasmota.log(f"MTR: >>>>>> long response", 3)
          a1_raw_or_list = []         # we return a list of block
          var a1_raw = bytes(48)
          var empty_list = TLV.Matter_TLV_array()
          self.attributedata2raw(a1_raw, ctx, empty_list, false)
          a1_raw_or_list.push(a1_raw)
          # tasmota.log(f"MTR: >>>>>> long response global DELETE {a1_raw.tohex()}", 3)

          for elt:res.val
            a1_raw = bytes(48)
            # var list_item = TLV.Matter_TLV_array()
            # list_item.val.push(elt)
            self.attributedata2raw(a1_raw, ctx, elt, true #- add ListIndex:null -#)
            # tasmota.log(f"MTR: >>>>>> long response global ADD {a1_raw.tohex()}", 3)
            a1_raw_or_list.push(a1_raw)
          end
          # tasmota.log(f"MTR: >>>>>> long response global {a1_raw_or_list}", 3)
        else
          # normal encoding
          # encode directly raw bytes()
          a1_raw_or_list = bytes(48)      # pre-reserve 48 bytes
          self.attributedata2raw(a1_raw_or_list, ctx, res)
        end

        if !no_log
          tasmota.log(f"MTR: >Read_Attr ({session.local_session_id:6i}) {ctx}{attr_name} - {res_str}", 3)
        end          
      elif ctx.status != nil
        if direct                           # we report an error only if a concrete direct read, not with wildcards
          # encode directly raw bytes()
          a1_raw_or_list = bytes(48)      # pre-reserve 48 bytes
          self.attributestatus2raw(a1_raw_or_list, ctx, ctx.status)

          if tasmota.loglevel(3)
            tasmota.log(format("MTR: >Read_Attr (%6i) %s%s - STATUS: 0x%02X %s", session.local_session_id, str(ctx), attr_name, ctx.status, ctx.status == matter.UNSUPPORTED_ATTRIBUTE ? "UNSUPPORTED_ATTRIBUTE" : ""), 3)
          end
        end
      else
        tasmota.log(format("MTR: >Read_Attr (%6i) %s%s - IGNORED", session.local_session_id, str(ctx), attr_name), 3)
        # ignore if content is nil and status is undefined
        found = false
      end

      # a1_raw_or_list if either nil, bytes(), of list(bytes())
      var idx = isinstance(a1_raw_or_list, list) ? 0 : nil      # index in list, or nil if non-list
      while a1_raw_or_list != nil
        var elt = (idx == nil) ? a1_raw_or_list : a1_raw_or_list[idx]   # dereference

        if size(ret.attribute_reports) == 0
          ret.attribute_reports.push(elt)                       # push raw binary instead of a TLV
        else    # already blocks present, see if we can add to the latest, or need to create a new block
          var last_block = ret.attribute_reports[-1]
          if size(last_block) + size(elt) <= matter.IM_ReportData.MAX_MESSAGE
            # add to last block
            last_block .. elt
          else
            ret.attribute_reports.push(elt)      # push raw binary instead of a TLV
          end
        end

        if idx == nil
          a1_raw_or_list = nil                    # stop loop
        else
          idx += 1
          if idx >= size(a1_raw_or_list)
            a1_raw_or_list = nil                    # stop loop
          end
        end
      end

      # check if we still have enough room in last block
      # if a1_raw_or_list         # do we have bytes to add, and it's not zero size
      #   if size(ret.attribute_reports) == 0
      #     ret.attribute_reports.push(a1_raw_or_list)      # push raw binary instead of a TLV
      #   else    # already blocks present, see if we can add to the latest, or need to create a new block
      #     var last_block = ret.attribute_reports[-1]
      #     if size(last_block) + size(a1_raw_or_list) <= matter.IM_ReportData.MAX_MESSAGE
      #       # add to last block
      #       last_block .. a1_raw_or_list
      #     else
      #       ret.attribute_reports.push(a1_raw_or_list)      # push raw binary instead of a TLV
      #     end
      #   end
      # end

      return found          # return true if we had a match
    end

    var endpoints = self.device.get_active_endpoints()
    # structure is `ReadRequestMessage` 10.6.2 p.558
    var ctx = matter.Path()
    ctx.msg = msg

    # prepare the response
    var ret = matter.ReportDataMessage()
    # ret.suppress_response = true
    ret.attribute_reports = []

    for q:query.attributes_requests
      # need to do expansion here
      ctx.endpoint = q.endpoint
      ctx.cluster = q.cluster
      ctx.attribute = q.attribute
      ctx.fabric_filtered = query.fabric_filtered
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
  # path2raw
  #
  # Encodes endpoint/cluster/attribute as `AttributePathIB` elements
  # Takes sub-tag
  #
  #   1 = AttributePathIB
  #     0 = EnableTagCompression bool opt
  #     1 = Node
  #     2 = Endpoint
  #     3 = Cluste
  #     4 = Attribute
  #     5 = ListIndex (opt)
  #
  #     3701 		1 = LIST
  #       2402 01    	2 = 1U (U1)
  #       2403 39 	3 = 0x39U (U1)
  #       2404 11 	4 = 0x11U (U1)
  #       [OPTIONAL ListIndex]
  #       3405      5 = NULL
  #     18
  def path2raw(raw, ctx, sub_tag, list_index_null)
    # open struct
    raw.add(0x37, 1)              # add 37
    raw.add(sub_tag, 1)           # add sub_tag
    # add endpoint
    if ctx.endpoint <= 0xFF       # endpoint is 16 bits max
      raw.add(0x2402, -2)         # add 2402
      raw.add(ctx.endpoint, 1)
    else
      raw.add(0x2502, -2)         # add 2502
      raw.add(ctx.endpoint, 2)
    end
    # add cluster
    if ctx.cluster <= 0xFF        # cluster is 32 bits max
      raw.add(0x2403, -2)         # add 2403
      raw.add(ctx.cluster, 1)
    elif ctx.cluster <= 0xFFFF
      raw.add(0x2503, -2)         # add 2503
      raw.add(ctx.cluster, 2)
    else
      raw.add(0x2603, -2)         # add 2603
      raw.add(ctx.cluster, 4)
    end
    # add attribute
    if ctx.attribute <= 0xFF       # cluster is 32 bits max
      raw.add(0x2404, -2)          # add 2404
      raw.add(ctx.attribute, 1)
    elif ctx.attribute <= 0xFFFF
      raw.add(0x2504, -2)          # add 2504
      raw.add(ctx.attribute, 2)
    else
      raw.add(0x2604, -2)          # add 2604
      raw.add(ctx.attribute, 4)
    end
    # do we add ListIndex: null
    if list_index_null
      raw.add(0x3405, -2)          # add 3405
    end
    # close
    raw.add(0x18, 1)               # add 18
  end

  #############################################################
  # attributedata2raw
  #
  # generate a raw version of AttributeDataIB
  #
  # Typical answer
  #
  # AttributeReportIB
  # 0 = AttributeStatusIB
  # 1 = AttributeDataIB
  #   0 = DataVersion U1
  #   1 = AttributePathIB
  #     0 = EnableTagCompression bool opt
  #     1 = Node
  #     2 = Endpoint
  #     3 = Cluste
  #     4 = Attribute
  #     5 = ListIndex (opt)
  #   2 = Data
  #
  # 153601.15350124000137012402012403392404111829021818.1824FF0118
  # 15350124000137012402012403392404111829021818
  # 1535012400013701
  #   240201240339240411
  # 1829021818
  #
  # 15
  #   3501  	1 = {}
  #     2400 01 	0 = 1U  (U1)
  #     3701 		1 = LIST
  #       2402 01    	2 = 1U (U1)
  #       2403 39 	3 = 0x39U (U1)
  #       2404 11 	4 = 0x11U (U1)
  #       [OPTIONAL ListIndex]
  #       3405      5 = NULL
  #
  #     18
  #     2902		2 = True
  #   18
  # 18
  def attributedata2raw(raw, ctx, val, list_index_null)
    raw.add(0x15350124, -4)             # add 15350124
    raw.add(0x0001, -2)                 # add 0001

    self.path2raw(raw, ctx, 0x01, list_index_null)
    
    # add value with tag 2
    val.tag_sub = 2
    val.tlv2raw(raw)
    # close 2 structs
    raw.add(0x1818, -2)
  end

  #############################################################
  # attributedata2raw
  #
  # generate a raw version of AttributeStatusIB
  #
  #
  # Typical answer
  #
  # AttributeReportIB
  # 0 = AttributeStatusIB
  #   0 = AttributePathIB
  #     0 = EnableTagCompression bool opt
  #     1 = Node
  #     2 = Endpoint
  #     3 = Cluste
  #     4 = Attribute
  #     5 = ListIndex (opt)
  #   1 = StatusIB
  #     0 = Status (u1)
  #     1 = ClusterStatus (u1)
  # 1 = AttributeDataIB
  #
  # 15360115350037002402012403022404031835012400041818181824FF0118
  # 153601 1535003700 - 2402012403022404031835012400041818181824FF0118
  # 
  # 15
  #   3601
  #
  #     15
  #       3500 0 = struct
  #         3700 0 = list
  #           240201 2 = 1U endpoint
  #           240302 3 = 2U cluster
  #           240403 4 = 3U attribute
  #         18
  #         3501 1 = struct
  #           240004 0 = 4U status
  #         18
  #       18
  #     18
  #
  #   18
  #   24FF01
  # 18
  def attributestatus2raw(raw, ctx, status)
    raw.add(0x15, 1)                    # add 15
    raw.add(0x3500, -2)                 # add 3500

    self.path2raw(raw, ctx, 0x00)
    
    raw.add(0x3501, -2)            # add 3501 for status
    # status
    if ctx.status <= 255
      raw.add(0x2400, -2)          # add 2400
      raw.add(ctx.status, 1)
    else
      raw.add(0x2500, -2)          # add 2500
      raw.add(ctx.status, 2)
    end
    # close
    raw.add(0x1818, -2)            # add 1818
    raw.add(0x18, 1)               # add 18
  end

  #############################################################
  # attributedata2raw
  #
  # generate a raw version of InvokeResponseIB()
  # Typical answer
  #
  # 1535013700240011240122240244183501240000181818
  #
  # 0 = CommandDataIB
  #   0 = CommandPathIB
  #     0 = endpoint u2
  #     1 = cluster u4
  #     2 = command u4
  #   1 = <fields>
  # 1 = CommandStatusIB
  #   0 = CommandPathIB
  #     0 = endpoint u2
  #     1 = cluster u4
  #     2 = command u4
  #   1 = StatusIB
  #     0 = status u1
  #     1 = ClusterStatus u1
  #
  # 1535013700240011240122240244183501240000181818
  # 15
  #   3501 1 = struct
  #     3700 0 = list
  #       240011 0 = endpoint
  #       240122 1 = cluster
  #       240244 2 = command
  #     18
  #     3501 1 = struct
  #       240000 0 = 0 (status)
  #     18
  #   18
  # 18
  #
  # 1535003700240011240122240244182401031818
  # 15
  #   3500 0 = struct
  #     3700 0 = list
  #       240011 0 = endpoint
  #       240122 1 = cluster
  #       240244 2 = command
  #     18
  #     240103 1 = <field>
  #   18
  # 18
  def invokeresponse2raw(raw, ctx, val)
    raw.add(0x15, 1)                    # add 15
    if val == nil
      raw.add(0x3501, -2)                 # add 3500
    else
      raw.add(0x3500, -2)                 # add 3500
    end
    raw.add(0x3700, -2)                 # add 3700
    # add endpoint
    if ctx.endpoint <= 0xFF       # endpoint is 16 bits max
      raw.add(0x2400, -2)         # add 2400
      raw.add(ctx.endpoint, 1)
    else
      raw.add(0x2500, -2)         # add 2500
      raw.add(ctx.endpoint, 2)
    end
    # add cluster
    if ctx.cluster <= 0xFF        # cluster is 32 bits max
      raw.add(0x2401, -2)         # add 2401
      raw.add(ctx.cluster, 1)
    elif ctx.cluster <= 0xFFFF
      raw.add(0x2501, -2)         # add 2501
      raw.add(ctx.cluster, 2)
    else
      raw.add(0x2601, -2)         # add 2601
      raw.add(ctx.cluster, 4)
    end
    # add attribute
    if ctx.command <= 0xFF       # cluster is 32 bits max
      raw.add(0x2402, -2)          # add 2402
      raw.add(ctx.command, 1)
    elif ctx.command <= 0xFFFF
      raw.add(0x2502, -2)          # add 2502
      raw.add(ctx.command, 2)
    else
      raw.add(0x2602, -2)          # add 2602
      raw.add(ctx.command, 4)
    end
    raw.add(0x18, 1)               # add 18

    # either value or statuc
    if val == nil
      var status = ctx.status
      if status == nil  status = matter.SUCCESS end
      raw.add(0x3501, -2)         # add 3501
      raw.add(0x2400, -2)         # add 2400
      raw.add(ctx.status, 1)      # add status:1
      raw.add(0x18, 1)            # add 18
    else
      val.tag_sub = 1             # set sub_tag for reponse
      val.tlv2raw(raw)
    end
    # close
    raw.add(0x1818, -2)           # add 1818
  end

  #############################################################
  # process IM 0x02 Read Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_read_request(msg, val)
    matter.profiler.log("read_request_start")
    # matter.profiler.log(str(val))
    var query = matter.ReadRequestMessage().from_TLV(val)
    # matter.profiler.log(str(query))
    if query.attributes_requests != nil
      var ret = self._inner_process_read_request(msg.session, query, msg)
      self.send_report_data(msg, ret)
    end

    return true
  end

  #############################################################
  # process IM 0x02 Read Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_read_request_solo(msg, ctx)
    # prepare fallback error
    ctx.status = matter.INVALID_ACTION
    ctx.msg = msg

    # find pi for this endpoint/cluster/attribute
    var pi = self.device.process_attribute_read_solo(ctx)
    var res = nil
    # matter.profiler.log("read_request_solo pi ok")
    # tasmota.log(f"MTR: process_read_request_solo {pi=}")

    var raw                      # this is the bytes() block we need to add to response (or nil)
    if pi != nil
      ctx.status = matter.UNSUPPORTED_ATTRIBUTE    # new fallback error
      res = pi.read_attribute(msg.session, ctx, self.tlv_solo)
    end
    matter.profiler.log("read_request_solo read done")

    if res != nil

      # check if the payload is a complex structure and too long to fit in a single response packet
      if (res.is_list || res.is_array) && res.encode_len() > matter.IM_ReportData.MAX_MESSAGE
        # revert to standard 
        # the attribute will be read again, but it's hard to avoid it
        res = nil       # indicated to GC that we don't need it again
        tasmota.log(f"MTR:                     Response to big, revert to non-solo", 3)
        var val = matter.TLV.parse(msg.raw, msg.app_payload_idx)
        return self.process_read_request(msg, val)
      end
      # encode directly raw bytes()
      raw = bytes(48)      # pre-reserve 48 bytes

      raw.add(0x15, 1)                    # add 15
      raw.add(0x3601, -2)                 # add 3601

      self.attributedata2raw(raw, ctx, res)

      # add suffix 1824FF0118
      raw.add(0x1824FF01, -4)        # add 1824FF01
      raw.add(0x18, 1)               # add 18

    elif ctx.status != nil
      
      # encode directly raw bytes()
      raw = bytes(48)      # pre-reserve 48 bytes

      raw.add(0x15, 1)                    # add 15
      raw.add(0x3601, -2)                 # add 3601

      self.attributestatus2raw(raw, ctx, ctx.status)
      
      # add suffix 1824FF0118
      raw.add(0x1824FF01, -4)        # add 1824FF01
      raw.add(0x18, 1)               # add 18

    else
      tasmota.log(f"MTR: >Read_Attr ({msg.session.local_session_id:6i}) {ctx} - IGNORED", 3)
      return false
    end

    # send packet
    var resp = msg.build_response(0x05 #-Report Data-#, true)

    var responder = self.device.message_handler
    var msg_raw = msg.raw
    msg_raw.clear()
    resp.encode_frame(raw, msg_raw)    # payload in cleartext
    resp.encrypt()
    if tasmota.loglevel(4)
      tasmota.log(format("MTR: <snd       (%6i) id=%i exch=%i rack=%s", resp.session.local_session_id, resp.message_counter, resp.exchange_id, resp.ack_message_counter), 4)
    end

    responder.send_response_frame(resp)

    # postpone lengthy operations after sending back response
    matter.profiler.log("RESPONSE SENT")

    var attr_name = matter.get_attribute_name(ctx.cluster, ctx.attribute)
    attr_name = attr_name ? " (" + attr_name + ")" : ""

    if res != nil
      if tasmota.loglevel(3)
        var res_str = res.to_str_val()  # get the value with anonymous tag before it is tagged, for logging
        tasmota.log(f"MTR: >Read_Attr1({msg.session.local_session_id:6i}) {ctx}{attr_name} - {res_str}", 3)
        # tasmota.log(f"MTR: {res.tlv2raw().tohex()}", 3)
      end
      # if matter.profiler.active && tasmota.loglevel(3)
      #   tasmota.log(f"MTR:            {raw=}", 3)    # TODO remove before flight
      # end
    elif ctx.status != nil
      var unsupported_attribute = (ctx.status == matter.UNSUPPORTED_ATTRIBUTE ? "UNSUPPORTED_ATTRIBUTE" : "")
      if tasmota.loglevel(3)
        tasmota.log(f"MTR: >Read_Attr1({msg.session.local_session_id:6i}) {ctx}{attr_name} - STATUS: 0x{ctx.status:02X} {unsupported_attribute}", 3)
      end
      # if matter.profiler.active && tasmota.loglevel(3)
      #   tasmota.log(f"MTR:            {raw=}", 3)    # TODO remove before flight
      # end
    else
      if tasmota.loglevel(3)
        tasmota.log(f"MTR: >Read_Attr1({msg.session.local_session_id:6i}) {ctx}{attr_name} - IGNORED", 3)
      end
    end

    # matter.profiler.log("read_request_solo end")
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
    ctx.msg = msg
    for q:query.attributes_requests
      ctx.endpoint = q.endpoint
      ctx.cluster = q.cluster
      ctx.attribute = q.attribute
      attr_req.push(str(ctx))
    end
    tasmota.log(format("MTR: >Subscribe (%6i) %s (min=%i, max=%i, keep=%i) sub=%i fabric_filtered=%s",
                              msg.session.local_session_id, attr_req.concat(" "), sub.min_interval, sub.max_interval, query.keep_subscriptions ? 1 : 0, sub.subscription_id, query.fabric_filtered), 3)
    if query.event_requests != nil && size(query.event_requests) > 0
      tasmota.log(f"MTR: >Subscribe (%6i) event_requests_size={size(query.event_requests)}", 3)
    end

    var ret = self._inner_process_read_request(msg.session, query, msg, true #-no_log-#)
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
    # import debug
    # structure is `ReadRequestMessage` 10.6.2 p.558
    # tasmota.log("MTR: IM:invoke_request processing start", 4)
    matter.profiler.log("invoke_request_start")
    var ctx = matter.Path()
    ctx.msg = msg

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
        matter.profiler.log("COMMAND DONE")
        var params_log = (ctx.log != nil) ? "(" + str(ctx.log) + ") " : ""
        tasmota.log(format("MTR: >Command   (%6i) %s %s %s", msg.session.local_session_id, ctx_str, cmd_name ? cmd_name : "", params_log), 3)
        # tasmota.log("MTR: Perf/Command = " + str(debug.counters()), 4)
        ctx.log = nil
        var raw = bytes(32)
        # var a1 = matter.InvokeResponseIB()
        if res == true || ctx.status == matter.SUCCESS      # special case, just respond ok
          ctx.status = matter.SUCCESS
          self.invokeresponse2raw(raw, ctx, nil)
          ret.invoke_responses.push(raw)
          if tasmota.loglevel(3)
            tasmota.log(f"MTR: <Replied   ({msg.session.local_session_id:6i}) OK exch={msg.exchange_id:i}", 3)
          end
        elif res != nil
          self.invokeresponse2raw(raw, ctx, res)
          ret.invoke_responses.push(raw)

          cmd_name = matter.get_command_name(ctx.cluster, ctx.command)
          if !cmd_name  cmd_name = "" end
          if tasmota.loglevel(3)
            tasmota.log(f"MTR: <Replied   ({msg.session.local_session_id:6i}) {ctx} {cmd_name}", 3)
          end
        elif ctx.status != nil
          self.invokeresponse2raw(raw, ctx, nil)
          ret.invoke_responses.push(raw)
          if tasmota.loglevel(3)
            tasmota.log(f"MTR: <Replied   ({msg.session.local_session_id:6i}) Status=0x{ctx.status:02X} exch={msg.exchange_id:i}", 3)
          end
        else
          if tasmota.loglevel(3)
            tasmota.log(f"MTR: _Ignore    ({msg.session.local_session_id:6i}) exch={msg.exchange_id:i}", 3)
          end
          # ignore if content is nil and status is undefined
        end
      end

      if size(ret.invoke_responses) > 0
        self.send_invoke_response(msg, ret)
      else
        return false        # we don't send anything, hence the responder sends a simple packet ack
      end
      return true
    end
  end

  #############################################################
  # process IM 0x08 Invoke Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_invoke_request_solo(msg, ctx)
    # import debug
    matter.profiler.log("invoke_request_solo_start")
    ctx.msg = msg
    ctx.status = matter.UNSUPPORTED_COMMAND   #default error if returned `nil`

    var cmd_name = matter.get_command_name(ctx.cluster, ctx.command)
    var ctx_str = str(ctx)                    # keep string before invoking, it is modified by response
    var res = self.device.invoke_request(msg.session, ctx.command_fields, ctx)
    matter.profiler.log("COMMAND DONE")
    var params_log = (ctx.log != nil) ? "(" + str(ctx.log) + ") " : ""
    if tasmota.loglevel(3)
      tasmota.log(format("MTR: >Command1  (%6i) %s %s %s", msg.session.local_session_id, ctx_str, cmd_name ? cmd_name : "", params_log), 3)
    end
    # tasmota.log("MTR: Perf/Command = " + str(debug.counters()), 4)
    ctx.log = nil
    var raw = bytes(48)

    # prefix 1528003601
    raw.add(0x15280036, -4)       # add 15280036
    raw.add(0x01, 1)              # add 01
    if res == true || ctx.status == matter.SUCCESS      # special case, just respond ok
      ctx.status = matter.SUCCESS
      self.invokeresponse2raw(raw, ctx, nil)

      if tasmota.loglevel(3)
        tasmota.log(f"MTR: <Replied   ({msg.session.local_session_id:6i}) OK exch={msg.exchange_id:i}", 3)
      end
    elif res != nil
      self.invokeresponse2raw(raw, ctx, res)

      if !cmd_name  cmd_name = "" end
      if tasmota.loglevel(3)
        tasmota.log(f"MTR: <Replied   ({msg.session.local_session_id:6i}) {ctx} {cmd_name}", 3)
      end
    elif ctx.status != nil
      self.invokeresponse2raw(raw, ctx, nil)

      if tasmota.loglevel(3)
        tasmota.log(f"MTR: <Replied   ({msg.session.local_session_id:6i}) Status=0x{ctx.status:02X} exch={msg.exchange_id:i}", 3)
      end
    else
      if tasmota.loglevel(3)
        tasmota.log(f"MTR: _Ignore    ({msg.session.local_session_id:6i}) exch={msg.exchange_id:i}", 3)
      end
      # ignore if content is nil and status is undefined
      return false
    end
    # add suffix 1824FF0118
    raw.add(0x1824FF01, -4)       # add 1824FF01
    raw.add(0x18, 1)              # add 18

    # tasmota.log(f"MTR: raw={raw.tohex()}", 3)
    var resp = msg.build_response(0x09 #-Invoke Response-#, true)
    var responder = self.device.message_handler
    var msg_raw = msg.raw
    msg_raw.clear()
    resp.encode_frame(raw, msg_raw)    # payload in cleartext
    resp.encrypt()
    responder.send_response_frame(resp)
    matter.profiler.log("RESPONSE SENT")

    return true
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
    ctx.msg = msg

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

    var ret = self._inner_process_read_request(session, fake_read, nil #-no msg-#)
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

