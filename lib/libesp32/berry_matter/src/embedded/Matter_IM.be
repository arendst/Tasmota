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

#@ solidify:Matter_Response_container,weak
#@ solidify:Matter_IM,weak

#################################################################################
# Matter_Response_container
#
# Used to store all the elements of the reponse to an attribute or command
#################################################################################
class Matter_Response_container
  var endpoint
  var cluster
  var attribute
  var command
  var status
end
matter.Response_container = Matter_Response_container

#################################################################################
# Matter_IM class
#################################################################################
class Matter_IM
  var responder

  def init(responder)
    self.responder = responder
  end

  def process_incoming(msg, remote_ip, remote_port)
    # messages are always TLV, decode payload
    tasmota.log("MTR: received IM message " + matter.inspect(msg), 3)

    var val = matter.TLV.parse(msg.raw, msg.app_payload_idx)

    tasmota.log("MTR: IM TLV: " + str(val), 3)

    var InteractionModelRevision = val.findsubval(0xFF)
    tasmota.log("MTR: InteractionModelRevision=" + (InteractionModelRevision != nil ? str(InteractionModelRevision) : "nil"), 3)

    var opcode = msg.opcode
    if   opcode == 0x01   # Status Response
      return self.process_status_response(msg, val, remote_ip, remote_port)
    elif opcode == 0x02   # Read Request
      return self.process_read_request(msg, val, remote_ip, remote_port)
    elif opcode == 0x03   # Subscribe Request
      return self.subscribe_request(msg, val, remote_ip, remote_port)
    elif opcode == 0x04   # Subscribe Response
      return self.subscribe_response(msg, val, remote_ip, remote_port)
    elif opcode == 0x05   # Report Data
      return self.report_data(msg, val, remote_ip, remote_port)
    elif opcode == 0x06   # Write Request
      return self.process_write_request(msg, val, remote_ip, remote_port)
    elif opcode == 0x07   # Write Response
      return self.process_write_response(msg, val, remote_ip, remote_port)
    elif opcode == 0x08   # Invoke Request
      return self.process_invoke_request(msg, val, remote_ip, remote_port)
    elif opcode == 0x09   # Invoke Response
      return self.process_invoke_response(msg, val, remote_ip, remote_port)
    elif opcode == 0x0A   # Timed Request
      return self.process_timed_request(msg, val, remote_ip, remote_port)
    end

    return false
  end

  #############################################################
  # process IM 0x01 Status Response
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_status_response(msg, val, remote_ip, remote_port)
    import string
    var status = val.findsubval(0, 0xFF)
    tasmota.log(string.format("MTR: Status Response = 0x%02X", status), 3)
    return true
  end

  #############################################################
  # process IM 0x02 Read Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_read_request(msg, val, remote_ip, remote_port)
    # structure is `ReadRequestMessage` 10.6.2 p.558
    tasmota.log("MTR: IM:read_request processing start", 3)

    var query = matter.ReadRequestMessage().from_TLV(val)
    if query.attributes_requests != nil
      # prepare the response
      var ret = matter.ReportDataMessage()
      ret.suppress_response = true
      ret.attribute_reports = []

      # TODO - we need to implement Concrete path expansion here

      for q:query.attributes_requests
        var attr_name = matter.get_attribute_name(q.cluster, q.attribute)
        tasmota.log("MTR: Read Attribute " + str(q) + (attr_name ? " (" + attr_name + ")" : ""), 2)
        var res = self.responder.device.read_attribute(msg, q.endpoint, q.cluster, q.attribute)
        if res != nil
          var a1 = matter.AttributeReportIB()
          # a1.attribute_status = matter.AttributeStatusIB()
          # a1.attribute_status.path = matter.AttributePathIB()
          # a1.attribute_status.status = matter.StatusIB()
          # a1.attribute_status.path.endpoint = 0
          # a1.attribute_status.path.cluster = q.cluster
          # a1.attribute_status.path.attribute = q.attribute
          # a1.attribute_status.status.status = matter.SUCCESS
          a1.attribute_data = matter.AttributeDataIB()
          a1.attribute_data.data_version = 1
          a1.attribute_data.path = matter.AttributePathIB()
          a1.attribute_data.path.endpoint = 0
          a1.attribute_data.path.cluster = q.cluster
          a1.attribute_data.path.attribute = q.attribute
          a1.attribute_data.data = res

          ret.attribute_reports.push(a1)
        end
      end

      tasmota.log("MTR: ReportDataMessage=" + str(ret), 3)
      tasmota.log("MTR: ReportDataMessageTLV=" + str(ret.to_TLV()), 3)

      var resp = msg.build_response(0x05 #-Report Data-#, true)
      resp.encode(ret.to_TLV().encode())    # payload in cleartext
      resp.encrypt()

      self.responder.send_response(resp.raw, remote_ip, remote_port, resp.message_counter)
    end

    return true
  end

  #############################################################
  # process IM 0x08 Invoke Request
  #
  # val is the TLV structure
  # returns `true` if processed, `false` if silently ignored,
  # or raises an exception
  def process_invoke_request(msg, val, remote_ip, remote_port)
    import string
    # structure is `ReadRequestMessage` 10.6.2 p.558
    tasmota.log("MTR: IM:invoke_request processing start", 3)
    var ctx = matter.Response_container()

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
        if cmd_name == nil  cmd_name = string.format("0x%04X/0x02X", ctx.cluster, ctx.command) end
        tasmota.log(string.format("MTR: >Received_cmd  %s from [%s]:%i", cmd_name, remote_ip, remote_port), 2)
        var res = self.responder.device.invoke_request(msg, q.command_fields, ctx)
        var a1 = matter.InvokeResponseIB()
        if res != nil
          a1.command = matter.CommandDataIB()
          a1.command.command_path = matter.CommandPathIB()
          a1.command.command_path.endpoint = ctx.endpoint
          a1.command.command_path.cluster = ctx.cluster
          a1.command.command_path.command = ctx.command
          a1.command.command_fields = res
          ret.invoke_responses.push(a1)

          cmd_name = matter.get_command_name(ctx.cluster, ctx.command)
          if cmd_name == nil  cmd_name = string.format("0x%04X/0x%02X", ctx.cluster, ctx.command) end
          tasmota.log(string.format("MTR: <Replied_cmd   %s", cmd_name), 2)
        elif ctx.status != nil
          a1.status = matter.CommandStatusIB()
          a1.status.command_path = matter.CommandPathIB()
          a1.status.command_path.endpoint = ctx.endpoint
          a1.status.command_path.cluster = ctx.cluster
          a1.status.command_path.command = ctx.command
          a1.status.status = matter.StatusIB()
          a1.status.status.status = ctx.status
          ret.invoke_responses.push(a1)
        else
          # ignore if content is nil and status is undefined
        end
      end

      tasmota.log("MTR: invoke_responses="+str(ret.invoke_responses), 3)
      if size(ret.invoke_responses) > 0
        tasmota.log("MTR: InvokeResponse=" + str(ret), 3)
        tasmota.log("MTR: InvokeResponseTLV=" + str(ret.to_TLV()), 3)

        var resp = msg.build_response(0x09 #-Invoke Response-#, true)
        resp.encode(ret.to_TLV().encode())    # payload in cleartext
        resp.encrypt()

        self.responder.send_response(resp.raw, remote_ip, remote_port, resp.message_counter)
      elif msg.x_flag_r                   # nothing to respond, check if we need a standalone ack
        var resp = msg.build_standalone_ack()
        resp.encode()
        resp.encrypt()
        # no ecnryption required for ACK
        self.responder.send_response(resp.raw, remote_ip, remote_port, resp.message_counter)
      end
    end
  end

  #############################################################
  # process IM 0x03 Subscribe Request
  #
  def subscribe_request(msg, val, remote_ip, remote_port)
    import string
    var query = matter.SubscribeRequestMessage().from_TLV(val)
    tasmota.log("MTR: received SubscribeRequestMessage=" + str(query), 3)
    return false
  end

  #############################################################
  # process IM 0x04 Subscribe Response
  #
  def subscribe_response(msg, val, remote_ip, remote_port)
    import string
    var query = matter.SubscribeResponseMessage().from_TLV(val)
    tasmota.log("MTR: received SubscribeResponsetMessage=" + str(query), 3)
    return false
  end

  #############################################################
  # process IM 0x05 ReportData
  #
  def report_data(msg, val, remote_ip, remote_port)
    import string
    var query = matter.ReportDataMessage().from_TLV(val)
    tasmota.log("MTR: received ReportDataMessage=" + str(query), 3)
    return false
  end

  #############################################################
  # process IM 0x06 Write Request
  #
  def process_write_request(msg, val, remote_ip, remote_port)
    import string
    var query = matter.WriteRequestMessage().from_TLV(val)
    tasmota.log("MTR: received WriteRequestMessage=" + str(query), 3)
    return false
  end

  #############################################################
  # process IM 0x07 Write Response
  #
  def process_write_response(msg, val, remote_ip, remote_port)
    import string
    var query = matter.WriteResponseMessage().from_TLV(val)
    tasmota.log("MTR: received WriteResponseMessage=" + str(query), 3)
    return false
  end

  #############################################################
  # process IM 0x09 Invoke Response
  #
  def process_invoke_response(msg, val, remote_ip, remote_port)
    import string
    var query = matter.InvokeResponseMessage().from_TLV(val)
    tasmota.log("MTR: received InvokeResponseMessage=" + str(query), 3)
    return false
  end

  #############################################################
  # process IM 0x0A Timed Request
  #
  def process_timed_request(msg, val, remote_ip, remote_port)
    import string
    var query = matter.TimedRequestMessage().from_TLV(val)
    tasmota.log("MTR: received TimedRequestMessage=" + str(query), 3)

    tasmota.log(string.format("MTR: >Received_IM   TimedRequest=%i from [%s]:%i", query.timeout, remote_ip, remote_port), 2)
    
    # Send success status report
    var sr  = matter.StatusResponseMessage()
    sr.status = matter.SUCCESS
    var resp = msg.build_response(0x01 #-Status Response-#, true #-reliable-#)
    resp.encode(sr.to_TLV().encode())    # payload in cleartext
    resp.encrypt()
    self.responder.send_response(resp.raw, remote_ip, remote_port, resp.message_counter)

    return true
  end

  #############################################################
  # placeholder, nothing to run for now
  def every_second()
  end
end
matter.IM = Matter_IM

#-

# Unit tests


-#

