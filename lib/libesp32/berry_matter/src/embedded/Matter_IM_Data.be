#
# Matter_IM_Data.be - suppport for Matter Interation Model messages structure
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

#@ solidify:Matter_IM_base,weak
#@ solidify:Matter_IM_Message_base,weak

#@ solidify:matter.AttributePathIB,weak
#@ solidify:matter.ClusterPathIB,weak
#@ solidify:matter.DataVersionFilterIB,weak
#@ solidify:matter.AttributeDataIB,weak
#@ solidify:matter.AttributeReportIB,weak
#@ solidify:matter.EventFilterIB,weak
#@ solidify:matter.EventPathIB,weak
#@ solidify:matter.EventDataIB,weak
#@ solidify:matter.EventReportIB,weak
#@ solidify:matter.CommandPathIB,weak
#@ solidify:matter.CommandDataIB,weak
#@ solidify:matter.InvokeResponseIB,weak
#@ solidify:matter.CommandStatusIB,weak
#@ solidify:matter.EventStatusIB,weak
#@ solidify:matter.AttributeStatusIB,weak
#@ solidify:matter.StatusIB,weak
#@ solidify:matter.StatusResponseMessage,weak
#@ solidify:matter.ReadRequestMessage,weak
#@ solidify:matter.ReadRequestMessage_solo,weak
#@ solidify:matter.ReportDataMessage,weak
#@ solidify:matter.SubscribeRequestMessage,weak
#@ solidify:matter.SubscribeResponseMessage,weak
#@ solidify:matter.WriteRequestMessage,weak
#@ solidify:matter.WriteResponseMessage,weak
#@ solidify:matter.TimedRequestMessage,weak
#@ solidify:matter.InvokeRequestMessage,weak
#@ solidify:matter.InvokeRequestMessage_solo,weak
#@ solidify:matter.InvokeResponseMessage,weak

#################################################################################
# Base class for all IM containers
#################################################################################
class Matter_IM_base
  def tostring()
    return "<" + classname(self) + ":" + matter.inspect(self) + ">"
  end

  # arr: array (list) of TLV values
  # cl: class for the TLV decoder
  def from_TLV_array(arr, cl)
    if arr == nil     return nil end
    var r = []
    for v:arr
      r.push(cl().from_TLV(v))
    end
    return r
  end

  # add an array of objects to TLV
  # s: current container to add to
  # tag: tag for the array (inner items don't have tags)
  # arr: the array of objects supporting `to_TLV()`
  #
  # if arr is `nil`, nothing happens
  def to_TLV_array(s, tag, arr)
    if arr == nil     return nil end
    var l = s.add_array(tag)     # create the list container and add to the current container
    for v:arr
      l.add_obj(nil, v)
    end
  end
end

#################################################################################
# Matter_AttributePathIB class
#################################################################################
class Matter_AttributePathIB : Matter_IM_base
  var tag_compression             # false if none
  var node                        # u64 as bytes
  var endpoint                    # u16
  var cluster                     # u32
  var attribute                   # u32
  var list_index                  # ?

  def tostring()
    try
      var s = ""
      if self.node    s += format("node=%s ", self.node) end
      s += (self.endpoint != nil ? format("[%02X]", self.endpoint) : "[**]")
      s += (self.cluster != nil ? format("%04X/", self.cluster) : "****/")
      s += (self.attribute != nil ? format("%04X", self.attribute) : "****")
      return s
    except .. as e, m
      return "Exception> " + str(e) + ", " + str(m)
    end
  end

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.tag_compression = val.findsubval(0)
    self.node = val.findsubval(1)
    self.endpoint = val.findsubval(2)
    self.cluster = val.findsubval(3)
    self.attribute = val.findsubval(4)
    self.list_index = val.findsubval(5)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_list()
    s.add_TLV(0, TLV.BOOL, self.tag_compression)
    s.add_TLV(1, TLV.U8, self.node)
    s.add_TLV(2, TLV.U2, self.endpoint)
    s.add_TLV(3, TLV.U4, self.cluster)
    s.add_TLV(4, TLV.U4, self.attribute)
    s.add_TLV(5, TLV.U2, self.list_index)
    return s
  end
end
matter.AttributePathIB = Matter_AttributePathIB


#################################################################################
# ClusterPathIB class
#################################################################################
class Matter_ClusterPathIB : Matter_IM_base
  var node                        # u64 as bytes
  var endpoint                    # u16
  var cluster                     # u32

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.node = val.findsubval(0)
    self.endpoint = val.findsubval(1)
    self.cluster = val.findsubval(2)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_list()
    s.add_TLV(0, TLV.U8, self.node)
    s.add_TLV(1, TLV.U2, self.endpoint)
    s.add_TLV(2, TLV.U4, self.cluster)
    return s
  end
end
matter.ClusterPathIB = Matter_ClusterPathIB

#################################################################################
# DataVersionFilterIB class
#################################################################################
class Matter_DataVersionFilterIB : Matter_IM_base
  var path                        # false if none
  var data_version                # u64 as bytes

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.path = matter.ClusterPathIB().from_TLV(val.findsub(0))
    self.data_version = val.findsubval(1) # u32
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.path)
    s.add_TLV(1, TLV.U4, self.data_version)
    return s
  end
end
matter.DataVersionFilterIB = Matter_DataVersionFilterIB

#################################################################################
# AttributeDataIB class
#################################################################################
class Matter_AttributeDataIB : Matter_IM_base
  var data_version                # u32
  var path                        # AttributePathIB
  var data                        # any TLV

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.data_version = val.findsubval(0) # u32
    self.path = matter.AttributePathIB().from_TLV(val.findsub(1))
    self.data = val.findsubval(2) # any
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U4, self.data_version)
    s.add_obj(1, self.path)
    s.add_obj(2, self.data)
    return s
  end
end
matter.AttributeDataIB = Matter_AttributeDataIB

#################################################################################
# AttributeReportIB class
#################################################################################
class Matter_AttributeReportIB : Matter_IM_base
  var attribute_status            # AttributeStatusIB
  var attribute_data              # AttributeDataIB

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.attribute_status = matter.AttributeStatusIB().from_TLV(val.findsub(0))
    self.attribute_data = matter.AttributeDataIB().from_TLV(val.findsub(1))
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.attribute_status)
    s.add_obj(1, self.attribute_data)
    return s
  end
end
matter.AttributeReportIB = Matter_AttributeReportIB

#################################################################################
# EventFilterIB class
#################################################################################
class Matter_EventFilterIB : Matter_IM_base
  var node                        # u64
  var event_min                   # u64

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.node = val.findsubval(0)
    self.event_min = val.findsubval(1)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U8, self.node)
    s.add_TLV(1, TLV.U8, self.event_min)
    return s
  end
end
matter.EventFilterIB = Matter_EventFilterIB

#################################################################################
# EventPathIB class
#################################################################################
class Matter_EventPathIB : Matter_IM_base
  var node                        # u64 as bytes
  var endpoint                    # u16
  var cluster                     # u32
  var event                       # u32
  var is_urgent                   # bool

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.node = val.findsubval(0)
    self.endpoint = val.findsubval(1)
    self.cluster = val.findsubval(2)
    self.event = val.findsubval(3)
    self.is_urgent = val.findsubval(4)
    return self
  end

  def to_TLV(s)
    var TLV = matter.TLV
    if s == nil     s = TLV.Matter_TLV_list() end
    s.add_TLV(0, TLV.U8, self.node)
    s.add_TLV(1, TLV.U2, self.endpoint)
    s.add_TLV(2, TLV.U4, self.cluster)
    s.add_TLV(3, TLV.U4, self.event)
    s.add_TLV(4, TLV.BOOL, self.is_urgent)
    return s
  end
end
matter.EventPathIB = Matter_EventPathIB


#################################################################################
# EventDataIB class
#################################################################################
class Matter_EventDataIB : Matter_IM_base
  var path                        # 
  var event_number                # u64 as bytes
  var priority                    # u8
  # one of
  var epoch_timestamp             # u64
  var system_timestamp            # u64
  var delta_epoch_timestamp       # u64
  var delta_system_timestamp      # u64
  # data
  var data                        # any TLV

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.path = matter.EventPathIB().from_TLV(val.findsub(0))
    self.event_number = val.findsubval(1) # u64
    self.priority = val.findsubval(2) # u64
    self.epoch_timestamp = val.findsubval(3) # u64
    self.system_timestamp = val.findsubval(4) # u64
    self.delta_epoch_timestamp = val.findsubval(5) # u64
    self.delta_system_timestamp = val.findsubval(6) # u64
    self.data = val.findsubval(7) # any
    return self
  end

  def to_TLV(s)
    var TLV = matter.TLV
    if s == nil   s = TLV.Matter_TLV_struct() end
    if self.path
      self.path.to_TLV(s.add_list(0))
    end
    s.add_TLV(1, TLV.U8, self.event_number)
    s.add_TLV(2, TLV.U1, self.priority)
    s.add_TLV(3, TLV.U8, self.epoch_timestamp)
    s.add_TLV(4, TLV.U8, self.system_timestamp)
    s.add_TLV(5, TLV.U8, self.delta_epoch_timestamp)
    s.add_TLV(6, TLV.U8, self.delta_system_timestamp)
    s.add_obj(7, self.data)
    return s
  end
end
matter.EventDataIB = Matter_EventDataIB


#################################################################################
# EventReportIB class
#################################################################################
class Matter_EventReportIB : Matter_IM_base
  var event_status                # EventStatusIB
  var event_data                  # EventDataIB

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.event_status = matter.EventStatusIB().from_TLV(val.findsub(0))
    self.event_data = matter.EventDataIB().from_TLV(val.findsub(1))
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.event_status)
    s.add_obj(1, self.event_data)
    return s
  end
end
matter.EventReportIB = Matter_EventReportIB

#################################################################################
# CommandPathIB class
#################################################################################
class Matter_CommandPathIB : Matter_IM_base
  var endpoint                    # u16
  var cluster                     # u32
  var command                     # u32

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.endpoint = val.findsubval(0)
    self.cluster = val.findsubval(1)
    self.command = val.findsubval(2)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_list()
    s.add_TLV(0, TLV.U2, self.endpoint)
    s.add_TLV(1, TLV.U4, self.cluster)
    s.add_TLV(2, TLV.U4, self.command)
    return s
  end
end
matter.CommandPathIB = Matter_CommandPathIB

#################################################################################
# CommandDataIB class
#################################################################################
class Matter_CommandDataIB : Matter_IM_base
  var command_path                # CommandPathIB
  var command_fields              # any

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.command_path = matter.CommandPathIB().from_TLV(val.findsub(0))
    self.command_fields = val.findsub(1)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.command_path)
    s.add_obj(1, self.command_fields)
    return s
  end
end
matter.CommandDataIB = Matter_CommandDataIB


#################################################################################
# InvokeResponseIB class
#################################################################################
class Matter_InvokeResponseIB : Matter_IM_base
  var command                     # CommandDataIB
  var status                      # CommandStatusIB

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.command = matter.CommandDataIB().from_TLV(val.findsub(0))
    self.status = matter.CommandStatusIB().from_TLV(val.findsub(1))
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.command)
    s.add_obj(1, self.status)
    return s
  end
end
matter.InvokeResponseIB = Matter_InvokeResponseIB


#################################################################################
# CommandStatusIB class
#################################################################################
class Matter_CommandStatusIB : Matter_IM_base
  var command_path                # CommandPathIB
  var status                      # StatusIB

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.command_path = matter.CommandPathIB().from_TLV(val.findsub(0))
    self.status = matter.StatusIB().from_TLV(val.findsub(1))
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.command_path)
    s.add_obj(1, self.status)
    return s
  end
end
matter.CommandStatusIB = Matter_CommandStatusIB

#################################################################################
# EventStatusIB class
#################################################################################
class Matter_EventStatusIB : Matter_IM_base
  var path                        # EventPathIB
  var status                      # StatusIB

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.path = matter.EventPathIB().from_TLV(val.findsub(0))
    self.status = matter.StatusIB().from_TLV(val.findsub(1))
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.path)
    s.add_obj(1, self.status)
    return s
  end
end
matter.EventStatusIB = Matter_EventStatusIB

#################################################################################
# AttributeStatusIB class
#################################################################################
class Matter_AttributeStatusIB : Matter_IM_base
  var path                        # AttributePathIB
  var status                      # StatusIB

  # decode from TLV
  def from_TLV(val)
    if val == nil   return nil end
    self.path = matter.AttributePathIB().from_TLV(val.findsub(0))
    self.status = matter.StatusIB().from_TLV(val.findsub(1))
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_obj(0, self.path)
    s.add_obj(1, self.status)
    return s
  end
end
matter.AttributeStatusIB = Matter_AttributeStatusIB

#################################################################################
# StatusIB class
#################################################################################
class Matter_StatusIB : Matter_IM_base
  var status                      # u16
  var cluster_status              # u16

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.status = val.findsubval(0)
    self.cluster_status = val.findsubval(1)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U2, self.status)
    s.add_TLV(1, TLV.U2, self.cluster_status)
    return s
  end
end
matter.StatusIB = Matter_StatusIB

#################################################################################
#################################################################################
# Matter_IM_Message_base class
#
# This is the superclass for all high-level messages
#################################################################################
#################################################################################
class Matter_IM_Message_base : Matter_IM_base
  var InteractionModelRevision              # 0xFF

  def init()
    self.InteractionModelRevision = 1
  end
end


#################################################################################
# StatusResponseMessage class
#################################################################################
class Matter_StatusResponseMessage : Matter_IM_Message_base
  var status                      # u32

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.status = val.findsubval(0)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U4, self.status)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.StatusResponseMessage = Matter_StatusResponseMessage

#################################################################################
# ReadRequestMessage class
#################################################################################
class Matter_ReadRequestMessage : Matter_IM_Message_base
  var attributes_requests         # array of AttributePathIB
  var event_requests              # array of EventPathIB
  var event_filters               # array of EventFilterIB
  var fabric_filtered             # bool
  var data_version_filters        # array of DataVersionFilterIB

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.attributes_requests = self.from_TLV_array(val.findsubval(0), matter.AttributePathIB)
    self.event_requests = self.from_TLV_array(val.findsubval(1), matter.EventPathIB)
    self.event_filters = self.from_TLV_array(val.findsubval(2), matter.EventFilterIB)
    self.fabric_filtered = val.findsubval(3)
    self.data_version_filters = self.from_TLV_array(val.findsubval(4), matter.DataVersionFilterIB)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    self.to_TLV_array(s, 0, self.attributes_requests)
    self.to_TLV_array(s, 1, self.event_requests)
    self.to_TLV_array(s, 2, self.event_filters)
    s.add_TLV(3, TLV.BOOL, self.fabric_filtered)
    self.to_TLV_array(s, 4, self.data_version_filters)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.ReadRequestMessage = Matter_ReadRequestMessage


#################################################################################
# ReadRequestMessage class optimized for a simple solo argument
#################################################################################
class Matter_Path end             # for compilation
class Matter_ReadRequestMessage_solo : Matter_Path
  # var endpoint                    # int
  # var cluster                     # int
  # var attribute                   # int
  # var fabric_filtered             # bool or nil

  def from_raw(raw, idx)
    self.reset()
    # must start with 15360017
    var sz = size(raw)
    var val
    if raw.get(idx, -4) != 0x15360017     return nil  end
    idx += 4
    while idx < sz
      # expect 24 xx yy or 25 xx yyyy (little endian)
      var tag = raw.get(idx+1, 1)
      var b0 = raw.get(idx, 1)
      if   b0 == 0x24       # u1
        val = raw.get(idx+2, 1)
        idx += 3
      elif b0 == 0x25       # u2
        val = raw.get(idx+2, 2)
        idx += 4
      else
        break
      end
      if   tag == 2
        self.endpoint = val
      elif tag == 3
        self.cluster = val
      elif tag == 4
        self.attribute = val
      else
        return nil            # unsupported tag
      end
    end
    # expect 18 18
    val = raw.get(idx, -2)
    if val != 0x1818      return nil  end
    idx += 2
    # fabric_filtered: 2803 or 2903
    val = raw.get(idx, -2)
    if   val == 0x2803
      self.fabric_filtered = false
      idx += 2
    elif val == 0x2903
      self.fabric_filtered = true
      idx += 2
    end
    # skip 24FFxx
    val = raw.get(idx, -2)
    if val == 0x24FF    idx += 3    end
    # expect 18
    if raw.get(idx, 1) != 0x18   return nil  end
    idx += 1
    # sanity check
    if self.endpoint == nil ||
       self.cluster == nil ||
       self.attribute == nil ||
       self.fabric_filtered == nil
          return nil
    end
    # all good
    return self
  end

  # Example: read_attr Reachable
  # 153600172402012403392404111818290324FF0118
  # 15 structure
  #   3600 tag 00 array
  #     17 list
  #       2402 tag 02 u1 "endpoint"
  #         01
  #       2403 tag 03 u1 "cluster"
  #         39 57U
  #       2404 tag 04 u1 "attribute"
  #         11 17U
  #       18
  #     18
  #   2903 tag 03 booltrue
  #   24FF tag FF u1
  #     01 1U
  # 18
  # {0 = [[[2 = 1U, 3 = 57U, 4 = 17U]]], 3 = true, 255 = 1U}

end
matter.ReadRequestMessage_solo = Matter_ReadRequestMessage_solo
# test
# b = bytes('0000153600172402012403392404111818290324FF0118')
# m = matter.ReadRequestMessage_solo()
# mm = m.from_raw(b, 2)
# print(m)


#################################################################################
# InvokeRequestMessage class optimized for a simple solo argument
#################################################################################
class Matter_Path end             # for compilation
class Matter_InvokeRequestMessage_solo : Matter_Path
  var SuppressResponse
  var TimedRequest
  var command_fields
  # var endpoint                    # int
  # var cluster                     # int
  # var attribute                   # int
  # var fabric_filtered             # bool or nil

  def reset()
    var n = nil
    super(self).reset()
    self.SuppressResponse = n
    self.TimedRequest = n
    self.command_fields = n
  end

  def from_raw(raw, idx)
    self.reset()
    # must start with 15360017
    var sz = size(raw)
    var val

    if raw.get(idx, 1) != 0x15            return nil  end
    idx += 1
    # check SuppressResponse (optional)
    val = raw.get(idx, -2)
    if   val == 0x2800 || val == 0x2900
      self.SuppressResponse = (val == 0x2900)
      idx += 2
    end
    # check TimedRequest (optional)
    val = raw.get(idx, -2)
    if   val == 0x2801 || val == 0x2901
      self.SuppressResponse = (val == 0x2901)
      idx += 2
    end
    # start of CommandDataIB
    if raw.get(idx, -2) != 0x3602   return nil end
    idx += 2
    if raw.get(idx, 1)  != 0x15     return nil end
    idx += 1
    if raw.get(idx, -2) != 0x3700   return nil end
    idx += 2
    #
    while idx < sz
      # expect 24 xx yy or 25 xx yyyy (little endian)
      var tag = raw.get(idx+1, 1)
      var b0 = raw.get(idx, 1)
      if   b0 == 0x24       # u1
        val = raw.get(idx+2, 1)
        idx += 3
      elif b0 == 0x25       # u2
        val = raw.get(idx+2, 2)
        idx += 4
      else
        break
      end
      if   tag == 0
        self.endpoint = val
      elif tag == 1
        self.cluster = val
      elif tag == 2
        self.command = val
      else
        return nil            # unsupported tag
      end
    end
    if raw.get(idx, 1)  != 0x18     return nil end
    idx += 1
    # command_fields
    if raw.get(idx, -2) != 0x3501   return nil end
    self.command_fields = matter.TLV.parse(raw, idx)
    idx = self.command_fields.next_idx     # skip structure

    # close
    if raw.get(idx, -2) != 0x1818     return nil end
    idx += 2
    if raw.get(idx, -4) != 0x24FF0118 return nil end

    # all good
    return self
  end

  # Example: command OnOff
  # {0 = false, 1 = false, 2 = [{0 = [[0 = 1U, 1 = 6U, 2 = 0U]], 1 = {}}], 255 = 1U}
  # 1528002801360215370024000124010624020018350118181824FF0118
  # 
  # 15
  #   2800 0 = false SuppressResponse
  #   2801 1 = false TimedRequest
  #   3602 2 = list of CommandDataIB
  #     15
  #       3700 0 = CommandPathIB
  #         240001 0 = 1U endpoint
  #         240106 1 = 6U cluster
  #         240200 2 = 0U command
  #       18
  #       3501 1 = struct
  #       18
  #     18
  #   18
  #   24FF01 FF = 1U
  # 18





  # 15 structure
  #   3600 tag 00 array
  #     17 list
  #       2402 tag 02 u1 "endpoint"
  #         01
  #       2403 tag 03 u1 "cluster"
  #         39 57U
  #       2404 tag 04 u1 "attribute"
  #         11 17U
  #       18
  #     18
  #   2903 tag 03 booltrue
  #   24FF tag FF u1
  #     01 1U
  # 18
  # {0 = [[[2 = 1U, 3 = 57U, 4 = 17U]]], 3 = true, 255 = 1U}

end
matter.InvokeRequestMessage_solo = Matter_InvokeRequestMessage_solo

#################################################################################
# ReportDataMessage class
#################################################################################
class Matter_ReportDataMessage : Matter_IM_Message_base
  var subscription_id             # u32
  var attribute_reports           # array of AttributeReportIB
  var event_reports               # array of EventReportIB
  var more_chunked_messages       # bool
  var suppress_response           # bool

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.subscription_id = val.findsubval(0)
    self.attribute_reports = self.from_TLV_array(val.findsubval(1), matter.AttributeReportIB)
    self.event_reports = self.from_TLV_array(val.findsubval(2), matter.EventReportIB)
    self.more_chunked_messages = val.findsubval(3)
    self.suppress_response = val.findsubval(4)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U4, self.subscription_id)
    self.to_TLV_array(s, 1, self.attribute_reports)
    self.to_TLV_array(s, 2, self.event_reports)
    s.add_TLV(3, TLV.BOOL, self.more_chunked_messages)
    s.add_TLV(4, TLV.BOOL, self.suppress_response)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.ReportDataMessage = Matter_ReportDataMessage

#################################################################################
# SubscribeRequestMessage class
#################################################################################
class Matter_SubscribeRequestMessage : Matter_IM_Message_base
  var keep_subscriptions          # bool
  var min_interval_floor          # u16
  var max_interval_ceiling        # u16
  var attributes_requests          # array of AttributePathIB
  var event_requests              # array of EventPathIB
  var event_filters               # array of EventFilterIB
  var fabric_filtered             # bool
  var data_version_filters        # array of DataVersionFilterIB

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.keep_subscriptions = val.findsubval(0, false)
    self.min_interval_floor = val.findsubval(1, 0)
    self.max_interval_ceiling = val.findsubval(2, 60)
    self.attributes_requests = self.from_TLV_array(val.findsubval(3), matter.AttributePathIB)
    self.event_requests = self.from_TLV_array(val.findsubval(4), matter.EventPathIB)
    self.event_filters = self.from_TLV_array(val.findsubval(5), matter.EventFilterIB)
    self.fabric_filtered = val.findsubval(7, false)
    self.data_version_filters = self.from_TLV_array(val.findsubval(8), matter.DataVersionFilterIB)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.BOOL, self.keep_subscriptions)
    s.add_TLV(1, TLV.U2, self.min_interval_floor)
    s.add_TLV(2, TLV.U2, self.max_interval_ceiling)
    self.to_TLV_array(s, 3, self.attributes_requests)
    self.to_TLV_array(s, 4, self.event_requests)
    self.to_TLV_array(s, 5, self.event_filters)
    s.add_TLV(7, TLV.BOOL, self.fabric_filtered)
    self.to_TLV_array(s, 8, self.data_version_filters)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.SubscribeRequestMessage = Matter_SubscribeRequestMessage

#################################################################################
# SubscribeResponseMessage class
#################################################################################
class Matter_SubscribeResponseMessage : Matter_IM_Message_base
  var subscription_id             # u32
  var max_interval                # u16

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.subscription_id = val.findsubval(0)
    self.max_interval = val.findsubval(2)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U4, self.subscription_id)
    s.add_TLV(2, TLV.U2, self.max_interval)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.SubscribeResponseMessage = Matter_SubscribeResponseMessage

#################################################################################
# WriteRequestMessage class
#################################################################################
class Matter_WriteRequestMessage : Matter_IM_Message_base
  var suppress_response           # bool
  var timed_request               # bool
  var write_requests              # array of AttributeDataIB
  var more_chunked_messages       # bool

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.suppress_response = val.findsubval(0)
    self.timed_request = val.findsubval(1)
    self.write_requests = self.from_TLV_array(val.findsubval(2), matter.AttributeDataIB)
    self.more_chunked_messages = val.findsubval(3)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.BOOL, self.suppress_response)
    s.add_TLV(1, TLV.BOOL, self.timed_request)
    self.to_TLV_array(s, 2, self.write_requests)
    s.add_TLV(3, TLV.BOOL, self.more_chunked_messages)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.WriteRequestMessage = Matter_WriteRequestMessage

#################################################################################
# WriteResponseMessage class
#################################################################################
class Matter_WriteResponseMessage : Matter_IM_Message_base
  var write_responses             # array of AttributeStatusIB

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.write_requests = self.from_TLV_array(val.findsubval(0), matter.AttributeStatusIB)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    self.to_TLV_array(s, 0, self.write_responses)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.WriteResponseMessage = Matter_WriteResponseMessage

#################################################################################
# TimedRequestMessage class
#################################################################################
class Matter_TimedRequestMessage : Matter_IM_Message_base
  var timeout                     # u16

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.timeout = val.findsubval(0)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.U2, self.timeout)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.TimedRequestMessage = Matter_TimedRequestMessage

#################################################################################
# InvokeRequestMessage class
#################################################################################
class Matter_InvokeRequestMessage : Matter_IM_Message_base
  var suppress_response           # bool
  var timed_request               # bool
  var invoke_requests             # array of CommandDataIB

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.suppress_response = val.findsubval(0)
    self.timed_request = val.findsubval(1)
    self.invoke_requests = self.from_TLV_array(val.findsubval(2), matter.CommandDataIB)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.BOOL, self.suppress_response)
    s.add_TLV(1, TLV.BOOL, self.timed_request)
    self.to_TLV_array(s, 2, self.invoke_requests)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.InvokeRequestMessage = Matter_InvokeRequestMessage

#################################################################################
# InvokeResponseMessage class
#################################################################################
class Matter_InvokeResponseMessage : Matter_IM_Message_base
  var suppress_response           # bool
  var invoke_responses            # array of InvokeResponseIB

  # decode from TLV
  def from_TLV(val)
    if val == nil     return nil end
    self.suppress_response = val.findsubval(0)
    self.invoke_responses = self.from_TLV_array(val.findsubval(1), matter.InvokeResponseIB)
    return self
  end

  def to_TLV()
    var TLV = matter.TLV
    var s = TLV.Matter_TLV_struct()
    s.add_TLV(0, TLV.BOOL, self.suppress_response)
    self.to_TLV_array(s, 1, self.invoke_responses)
    s.add_TLV(0xFF, TLV.U1, self.InteractionModelRevision)
    return s
  end
end
matter.InvokeResponseMessage = Matter_InvokeResponseMessage


#-

# Unit tests

import matter

m = matter.TLV.parse(bytes("1724020024033024040018"))
assert(m.tostring() == "[[2 = 0U, 3 = 48U, 4 = 0U]]")
#a = matter.IM.AttributePathIB().from_TLV(m)
a = matter.AttributePathIB().from_TLV(m)
assert(str(a) == "[00]0030/0000")
m = a.to_TLV()
assert(m.tostring() == "[[2 = 0U, 3 = 48U, 4 = 0U]]")
assert(m.tlv2raw() == bytes("1724020024033024040018"))


# create DataVersionFilterIB from scratch
c = matter.ClusterPathIB()
c.endpoint = 1
c.cluster = 32
c.to_TLV()
assert(str(c.to_TLV()) == "[[1 = 1U, 2 = 32U]]")

d = matter.DataVersionFilterIB()
d.path = c
d.data_version = 10
assert(str(d.to_TLV()) == '{0 = [[1 = 1U, 2 = 32U]], 1 = 10U}')
assert(d.to_TLV().tlv2raw() == bytes('1537002401012402201824010A18'))

# decode DataVersionFilterIB from scratch
m = matter.TLV.parse(bytes("1537002401012402201824010A18"))
assert(str(m) == '{0 = [[1 = 1U, 2 = 32U]], 1 = 10U}')

# ReadRequestMessage
m = matter.TLV.parse(bytes("153600172403312504FCFF18172402002403302404001817240200240330240401181724020024033024040218172402002403302404031817240200240328240402181724020024032824040418172403312404031818280324FF0118"))
assert(str(m) == "{0 = [[[3 = 49U, 4 = 65532U]], [[2 = 0U, 3 = 48U, 4 = 0U]], [[2 = 0U, 3 = 48U, 4 = 1U]], [[2 = 0U, 3 = 48U, 4 = 2U]], [[2 = 0U, 3 = 48U, 4 = 3U]], [[2 = 0U, 3 = 40U, 4 = 2U]], [[2 = 0U, 3 = 40U, 4 = 4U]], [[3 = 49U, 4 = 3U]]], 3 = false, 255 = 1U}")
r = matter.ReadRequestMessage().from_TLV(m)
assert(str(r) == "<Matter_ReadRequestMessage:{'attributes_requests': [[**]0031/FFFC, [00]0030/0000, [00]0030/0001, [00]0030/0002, [00]0030/0003, [00]0028/0002, [00]0028/0004, [**]0031/0003], 'data_version_filters': nil, 'event_filters': nil, 'event_requests': nil, 'fabric_filtered': false}>")
t = r.to_TLV()
assert(str(t) == "{0 = [[[3 = 49U, 4 = 65532U]], [[2 = 0U, 3 = 48U, 4 = 0U]], [[2 = 0U, 3 = 48U, 4 = 1U]], [[2 = 0U, 3 = 48U, 4 = 2U]], [[2 = 0U, 3 = 48U, 4 = 3U]], [[2 = 0U, 3 = 40U, 4 = 2U]], [[2 = 0U, 3 = 40U, 4 = 4U]], [[3 = 49U, 4 = 3U]]], 3 = false}")


# ReportDataMessage
r = matter.ReportDataMessage()
r.subscription_id = 1
r.attribute_reports = []
a1 = matter.AttributeReportIB()
a1.attribute_status = matter.AttributeStatusIB()
a1.attribute_status.path = matter.AttributePathIB()
a1.attribute_status.status = matter.StatusIB()
a1.attribute_status.path.endpoint = 0
a1.attribute_status.path.cluster = 0x0030
a1.attribute_status.path.attribute = 0
a1.attribute_status.status.status = 0
a1.attribute_status.status.cluster_status = 0
a1.attribute_data = matter.AttributeDataIB()
a1.attribute_data.data_version = 1
a1.attribute_data.path = matter.AttributePathIB()
a1.attribute_data.path.endpoint = 0
a1.attribute_data.path.cluster = 0x0030
a1.attribute_data.path.attribute = 0
a1.attribute_data.data = matter.TLV.create_TLV(matter.TLV.UTF1, "Tasmota")
assert(str(a1.to_TLV()) == '{0 = {0 = [[2 = 0U, 3 = 48U, 4 = 0U]], 1 = [[0 = 0U, 1 = 0U]]}, 1 = {0 = 1U, 1 = [[2 = 0U, 3 = 48U, 4 = 0U]], 2 = "Tasmota"}}')
r.attribute_reports.push(a1)
#{0 = 1U, 1 = [{0 = {0 = [[2 = 0U, 3 = 48U, 4 = 0U]], 1 = [[0 = 0U, 1 = 0U]]}, 1 = {0 = 1U, 1 = [[2 = 0U, 3 = 48U, 4 = 0U]], 2 = "Tasmota"}}]}
assert(r.to_TLV().tlv2raw() == bytes('1524000136011535003700240200240330240400183701240000240100181835012400013701240200240330240400182C02075461736D6F746118181818'))


# <Matter_AttributeReportIB:{
#   'attribute_data': <Matter_AttributeDataIB:{
#       'data': nil, 'data_version': nil, 'path': nil}>, 
#       'attribute_status': <Matter_AttributeStatusIB:{
#         'path': [00]0030/0000, 'status': <Matter_StatusIB:{
#           'cluster_status': 0, 'status': 0}>}>}>

-#

