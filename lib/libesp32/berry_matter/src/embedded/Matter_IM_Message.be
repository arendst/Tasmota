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

#@ solidify:Matter_Path,weak
#@ solidify:Matter_IM_Message,weak
#@ solidify:Matter_IM_Status,weak
#@ solidify:Matter_IM_InvokeResponse,weak
#@ solidify:Matter_IM_WriteResponse,weak
#@ solidify:Matter_IM_ReportData,weak
#@ solidify:Matter_IM_ReportDataSubscribed,weak
#@ solidify:Matter_IM_SubscribeResponse,weak

#################################################################################
# Matter_Path
#
# Used to store all the elements of the reponse to an attribute or command
#################################################################################
class Matter_Path
  var endpoint                # endpoint or `nil` if expansion
  var cluster                 # cluster or `nil` if expansion
  var attribute               # attribute or `nil` if expansion
  var command                 # command
  var status                  # status to be returned (matter.SUCCESS or matter.<ERROR>)

  def tostring()
    try
      import string
      var s = ""
      s += (self.endpoint  != nil ? string.format("[%02X]", self.endpoint) : "[**]")
      s += (self.cluster   != nil ? string.format("%04X/", self.cluster) : "****/")
      s += (self.attribute != nil ? string.format("%04X", self.attribute) : "")
      s += (self.command   != nil ? string.format("%04X", self.command) : "")
      if self.attribute == nil && self.command == nil     s += "****" end
      return s
    except .. as e, m
      return "Exception> " + str(e) + ", " + str(m)
    end
  end

end
matter.Path = Matter_Path

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
  var data                            # TLV data of the response (if any)

  # build a response message stub
  def init(msg, opcode, reliable)
    self.resp = msg.build_response(opcode, reliable)
    self.ready = true
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT
  end

  # the message is being removed due to expiration
  def reached_timeout()
  end

  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def ack_received(msg)
    self.expiration = tasmota.millis() + self.MSG_TIMEOUT     # give more time
    return false
  end

  # Status Report OK received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
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

  # return true if transaction is complete (remove object from queue)
  # default responder for data
  def send(responder)
    var resp = self.resp
    resp.encode_frame(self.data.to_TLV().encode())    # payload in cleartext
    resp.encrypt()
    responder.send_response(resp.raw, resp.remote_ip, resp.remote_port, resp.message_counter)
    return true
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
    self.ready = true                # send immediately
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
    self.ready = true                # send immediately
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
    self.ready = true                # send immediately
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

  def init(msg, data)
    super(self).init(msg, 0x05 #-Report Data-#, true)
    self.data = data
    self.ready = true                # send immediately
  end

  # return true if transaction is complete (remove object from queue)
  # default responder for data
  def send(responder)
    import string
    var resp = self.resp                          # response frame object
    var data = self.data                          # TLV data of the response (if any)
    var was_chunked = data.more_chunked_messages  # is this following a chunked packet?

    # compute the acceptable size
    var msg_sz = 0                  # message size up to now
    var elements = 0                # number of elements added
    if size(data.attribute_reports) > 0
      msg_sz = data.attribute_reports[0].to_TLV().encode_len()
      elements = 1
    end
    while msg_sz < self.MAX_MESSAGE && elements < size(data.attribute_reports)
      var next_sz = data.attribute_reports[elements].to_TLV().encode_len()
      if msg_sz + next_sz < self.MAX_MESSAGE
        msg_sz += next_sz
        elements += 1
      else  
        break
      end
    end


    tasmota.log(string.format("MTR: elements=%i msg_sz=%i total=%i", elements, msg_sz, size(data.attribute_reports)), 3)
    var next_elemnts = data.attribute_reports[elements .. ]
    data.attribute_reports = data.attribute_reports[0 .. elements - 1]
    data.more_chunked_messages = (size(next_elemnts) > 0)

    if was_chunked
      tasmota.log(string.format("MTR: Read_Attr  next_chunk exch=%i", self.get_exchangeid()), 3)
    end
    if data.more_chunked_messages
      if !was_chunked
        tasmota.log(string.format("MTR: Read_Attr  first_chunk exch=%i", self.get_exchangeid()), 3)
      end
      # tasmota.log("MTR: sending TLV" + str(data), 4)
    end

    # print(">>>>> send elements before encode")
    var raw_tlv = self.data.to_TLV()
    # print(">>>>> send elements before encode 2")
    var encoded_tlv = raw_tlv.encode(bytes(self.MAX_MESSAGE))    # takes time
    # print(">>>>> send elements before encode 3")
    resp.encode_frame(encoded_tlv)    # payload in cleartext, pre-allocate max buffer
    # print(">>>>> send elements after encode")
    resp.encrypt()
    # print(">>>>> send elements after encrypt")
    responder.send_response(resp.raw, resp.remote_ip, resp.remote_port, resp.message_counter)

    if size(next_elemnts) > 0
      data.attribute_reports = next_elemnts
      # tasmota.log("MTR: to_be_sent_later TLV" + str(data), 3)
      return false          # keep alive
    else
      return true           # finished, remove
    end
  end

end
matter.IM_ReportData = Matter_IM_ReportData


#################################################################################
# Matter_IM_ReportDataSubscribed
#
# Main difference is that we are the spontaneous inititor
#################################################################################
class Matter_IM_ReportDataSubscribed : Matter_IM_ReportData
  var sub                         # subscription object
  var report_data_phase           # true during reportdata

  def init(message_handler, session, data, sub)
    self.resp = matter.Frame.initiate_response(message_handler, session, 0x05 #-Report Data-#, true)
    self.data = data
    self.ready = true                # send immediately
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
    super(self).ack_received(msg)
    if !self.report_data_phase
      # if ack is received while all data is sent, means that it finished without error
      self.ready = true
      self.sub.re_arm()           # signal that we can proceed to next sub report
      return true                       # proceed to calling send() which removes the message
    else
      return false                      # do nothing
    end
  end

  # we received an ACK error, remove subscription
  def status_error_received(msg)
    self.sub.remove_self()
  end

  # ack received for previous message, proceed to next (if any)
  # return true if we manage the ack ourselves, false if it needs to be done upper
  def status_ok_received(msg)
    if self.report_data_phase
      return super(self).status_ok_received(msg)
    else
      super(self).status_ok_received(nil)
      return false                            # let the caller to the ack
    end
  end

  # returns true if transaction is complete (remove object from queue)
  # default responder for data
  def send(responder)
    if size(self.data.attribute_reports) > 0
      if self.report_data_phase
        var ret = super(self).send(responder)
        if !ret   return false end              # ReportData needs to continue
        # ReportData is finished
        self.report_data_phase = false
        return false
      else
        # send a simple ACK
        var resp = self.resp.build_standalone_ack()
        resp.encode_frame()
        resp.encrypt()
        responder.send_response(resp.raw, resp.remote_ip, resp.remote_port, resp.message_counter)
        return true                             # we received a ack(), just finish
      end

    else
      # simple heartbeat ReportData
      if self.report_data_phase
        super(self).send(responder)
        self.report_data_phase = false
        return false                             # don't expect any response
      else
        return true                              # we're done, remove message
      end
    end
  end
end
matter.IM_ReportDataSubscribed = Matter_IM_ReportDataSubscribed

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

  # return true if transaction is complete (remove object from queue)
  # default responder for data
  def send(responder)
    if self.report_data_phase
      var ret = super(self).send(responder)
      if !ret   return false end              # ReportData needs to continue
      # ReportData is finished
      self.report_data_phase = false
      return false

    else
      # send the final SubscribeReponse
      var resp = self.resp
      var sr = matter.SubscribeResponseMessage()
      sr.subscription_id = self.sub.subscription_id
      sr.max_interval = self.sub.max_interval

      self.resp.opcode = 0x04 #- Subscribe Response -#
      resp.encode_frame(sr.to_TLV().encode())    # payload in cleartext
      resp.encrypt()
      responder.send_response(resp.raw, resp.remote_ip, resp.remote_port, resp.message_counter)
      self.sub.re_arm()
      return true
    end
  end

  # Status ok received
  def status_ok_received(msg)
    # once we receive ack, open flow for subscriptions
    return super(self).status_ok_received(msg)
  end
    
end
matter.IM_SubscribeResponse = Matter_IM_SubscribeResponse
