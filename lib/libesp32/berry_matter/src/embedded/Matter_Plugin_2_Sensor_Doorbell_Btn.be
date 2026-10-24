#
# Matter_Plugin_Sensor_Doorbell_Btn.be - Doorbell (0x0148) mapped to a Tasmota Button
#
# Copyright (C) 2026
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

#################################################################################
# Matter 1.6.1 Device Specification - Doorbell (0x0148)
#################################################################################
# Device Type: Doorbell (0x0148)
# Device Type Revision: 2 (Matter 1.6.1 Device Library, Doorbell.xml)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0003: Identify (M) - inherited from Matter_Plugin_Device
# - 0x001E: Binding - storage/read/write of the binding table only (see NOTES)
# - 0x003B: Switch (M) with feature MS (Momentary Switch) mandatory
#
# CLUSTERS (Client):
# - 0x0556: Chime (M) - NOT IMPLEMENTED in this plug-in (see NOTES)
#
# NOTES:
# - Phase 1 (this file): Switch cluster only. Button presses emit
#   InitialPress and ShortRelease events. No multi-press.
# - FeatureMap is 0x06 (MS + MSR). The device type only mandates MS, but the
#   Switch cluster's own event conformance ties ShortRelease (0x03) to the
#   MSR feature, so MSR must also be set since this plug-in emits it.
# - MultiPressMax (0x0002) is not in the Switch cluster attribute list, and
#   the MultiPress events are not exposed.
# - The class derives from Matter_Plugin_Device, not from the generic switch
#   plug-in: consolidate_clusters() merges the parent's cluster map, so
#   inheriting the generic switch would keep MultiPressMax in the list.
# - Binding (0x001E) is implemented as a foundation for a future Chime
#   client: a controller can read/write the binding table (list of
#   {node, group, endpoint, cluster, fabric_index}) and it persists across
#   reboots in this endpoint's plugins_config entry. Nothing yet ACTS on
#   the stored bindings: no outbound Invoke is sent to a bound Chime server.
# - The Chime client (0x0556) itself requires outbound Invoke support
#   (CASE-initiator session establishment, operational discovery, and
#   Invoke Request origination/response handling), none of which exist in
#   the Berry Matter stack yet. It is therefore not declared as a cluster
#   here, and no command is ever sent when the button is pressed.
#################################################################################

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Doorbell_Btn,weak

class Matter_Plugin_Sensor_Doorbell_Btn : Matter_Plugin_Device
  static var TYPE = "doorbell_btn"                  # name of the plug-in in json
  static var DISPLAY_NAME = "Doorbell/Button"       # display name of the plug-in

  static var SCHEMA = "button|"                     # arg name
                      "l:Button|"                    # label (display name)
                      "t:i|"                        # type: int
                      "h:Button<x> number|"         # hint
                      "r:1"                         # required
  # Identify (0x0003) is inherited from Matter_Plugin_Device.
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x001E: [0],                                    # Binding - Binding attribute only
    0x003B: [0, 1],                                 # Switch 1.12 - no MultiPressMax
  })
  static var TYPES = { 0x0148: 2 }                  # Doorbell - Matter 1.6.1 Device Library Rev 2
  static var MAX_BINDINGS = 10                      # arbitrary cap, same order of magnitude as GroupKeyMap

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_position
  var bindings                                      # list of maps: node/group/endpoint/cluster/fabric_index
  var _bindings_write_staged                        # staged list during a chunked write transaction
  var _bindings_write_failed                        # true if any chunk of the current write failed
  var _bindings_write_exchange_id                   # exchange id of the in-progress write transaction
  var _bindings_write_session                       # session of the in-progress write transaction

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.tasmota_switch_index = int(config.find('button', 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
    self.shadow_position = 0             # CurrentPosition is not nullable
    self.bindings = config.find('bindings', [])
  end

  #############################################################
  # Model
  #
  def set_position(position)
    if position != self.shadow_position
      self.shadow_position = position
      self.attribute_updated(0x003B, 0x0001)
    end
  end

  #############################################################
  # read_attribute
  #
  # Switch cluster is handled here. Other clusters are delegated
  # to the parent class.
  def read_attribute(session, ctx, tlv_solo)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if cluster == 0x001E
      if attribute == 0x0000            # Binding - list[TargetStruct]
        return self.binding_list_to_tlv()
      end
    end
    if cluster == 0x003B
      if   attribute == 0x0000          # NumberOfPositions
        return tlv_solo.set(0x06 #-TLV.U4-#, 2)
      elif attribute == 0x0001          # CurrentPosition
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_position)

      elif attribute == 0xFFFA          # EventList: InitialPress, ShortRelease only
        var events = matter.TLV.Matter_TLV_array()
        for event_id : [0x01, 0x03]
          events.add_TLV(nil, 0x06 #-TLV.U4-#, event_id)
        end
        return events
      elif attribute == 0xFFFC          # FeatureMap: MS + MSR (ShortRelease needs MSR)
        return tlv_solo.set(0x06 #-TLV.U4-#, 0x06)
      end
    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # write_attribute
  #
  # Binding cluster is handled here. Other clusters are delegated
  # to the parent class.
  def write_attribute(session, ctx, write_data)
    if ctx.cluster == 0x001E
      if ctx.attribute == 0x0000        # Binding - list[TargetStruct]
        try
          return self.write_bindings_operation(session.get_fabric(), ctx, write_data)
        except .. as e, m
          log("MTR: invalid Binding write: " + str(e) + "|" + str(m), 2)
          ctx.status = 0x8D #-matter.INVALID_DATA_TYPE-#
          return false
        end
      end
    end
    return super(self).write_attribute(session, ctx, write_data)
  end

  #############################################################
  # begin_write_request / end_write_request
  #
  # Open or resume the Binding list-write transaction for this exchange.
  # The transaction survives MoreChunkedMessages, mirroring
  # Matter_Plugin_1_Root's GroupKeyMap write transaction handling.
  def begin_write_request(msg)
    var exchange_id = msg != nil ? msg.exchange_id : nil
    var session = msg != nil ? msg.session : nil
    if exchange_id != nil && self._bindings_write_exchange_id == exchange_id &&
       self._bindings_write_session == session
      return
    end
    self._bindings_write_staged = nil
    self._bindings_write_failed = false
    self._bindings_write_exchange_id = exchange_id
    self._bindings_write_session = session
  end

  # Close a WriteRequest transaction. On the final message this commits a
  # staged list whose last operation appeared in an earlier protocol message.
  def end_write_request(msg, more_chunked_messages)
    if more_chunked_messages   return end

    if self._bindings_write_staged != nil && !self._bindings_write_failed
      self.bindings = self._bindings_write_staged
      self.persist_bindings()
      self.attribute_updated(0x001E, 0x0000)
    end

    self._bindings_write_staged = nil
    self._bindings_write_exchange_id = nil
    self._bindings_write_session = nil
  end

  #############################################################
  # binding_list_to_tlv
  #
  # Encode the persisted binding table for the Binding attribute.
  def binding_list_to_tlv()
    var TLV = matter.TLV
    var result = TLV.Matter_TLV_array()
    for entry : self.bindings
      var out = result.add_struct(nil)
      if entry.find('node') != nil      out.add_TLV(1, 0x07 #-TLV.U8-#, entry.find('node')) end
      if entry.find('group') != nil     out.add_TLV(2, 0x05 #-TLV.U2-#, entry.find('group')) end
      if entry.find('endpoint') != nil  out.add_TLV(3, 0x05 #-TLV.U2-#, entry.find('endpoint')) end
      if entry.find('cluster') != nil   out.add_TLV(4, 0x06 #-TLV.U4-#, entry.find('cluster')) end
      out.add_TLV(0xFE, 0x04 #-TLV.U1-#, entry.find('fabric_index', 1))
    end
    return result
  end

  #############################################################
  # normalize_binding_entry
  #
  # Validate a single incoming TargetStruct and convert it to JSON-safe
  # storage. Exactly one of Node or Group must be present, per spec.
  def normalize_binding_entry(encoded, fabric)
    var node = encoded.findsubval(1)
    var group = encoded.findsubval(2)
    var endpoint = encoded.findsubval(3)
    var cluster = encoded.findsubval(4)
    var fabric_index = encoded.findsubval(0xFE)

    if (node == nil) == (group == nil)              # exactly one of the two
      return nil
    end
    if fabric_index != nil && fabric_index != 0 && fabric != nil && fabric_index != fabric.get_fabric_index()
      return nil
    end
    var normalized = {}
    if node != nil       normalized['node'] = node end
    if group != nil       normalized['group'] = group end
    if endpoint != nil    normalized['endpoint'] = endpoint end
    if cluster != nil     normalized['cluster'] = cluster end
    normalized['fabric_index'] = fabric != nil ? fabric.get_fabric_index() : (fabric_index ? fabric_index : 1)
    return normalized
  end

  #############################################################
  # write_bindings_operation
  #
  # Apply whole-list, append, replace, or delete semantics to a staged
  # Binding list so chunked writes are committed atomically, mirroring
  # Matter_Plugin_1_Root's write_group_key_map_operation.
  def write_bindings_operation(fabric, ctx, write_data)
    if self._bindings_write_failed
      ctx.status = 0x80 #-matter.INVALID_ACTION-#
      return false
    end
    var staged = self._bindings_write_staged
    if staged == nil   staged = self.bindings.copy() end

    if !ctx.list_index_present
      var replacement = []
      if !isinstance(write_data, list) || size(write_data) > self.MAX_BINDINGS
        self._bindings_write_failed = true
        ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
        return false
      end
      for encoded : write_data
        var norm = self.normalize_binding_entry(encoded, fabric)
        if norm == nil
          self._bindings_write_failed = true
          ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
          return false
        end
        replacement.push(norm)
      end
      staged = replacement
    elif ctx.list_index_is_null
      if size(staged) >= self.MAX_BINDINGS
        self._bindings_write_failed = true
        ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
        return false
      end
      var norm = self.normalize_binding_entry(ctx.write_tlv, fabric)
      if norm == nil
        self._bindings_write_failed = true
        ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
        return false
      end
      staged.push(norm)
    else
      var index = ctx.list_index
      if index == nil || index < 0 || index >= size(staged)
        self._bindings_write_failed = true
        ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
        return false
      end
      if ctx.write_tlv != nil && ctx.write_tlv.typ == 0x14 #-TLV.NULL-#
        staged.remove(index)
      else
        var norm = self.normalize_binding_entry(ctx.write_tlv, fabric)
        if norm == nil
          self._bindings_write_failed = true
          ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
          return false
        end
        staged[index] = norm
      end
    end

    self._bindings_write_staged = staged
    if ctx.list_write_final
      self.bindings = staged
      self._bindings_write_staged = nil
      self.persist_bindings()
      self.attribute_updated(ctx.cluster, ctx.attribute)
    end
    return true
  end

  #############################################################
  # persist_bindings
  #
  # Save the binding table into this endpoint's own plugins_config entry
  # so it survives a reboot, the same way the user-facing SCHEMA fields do.
  def persist_bindings()
    var ep_str = str(self.endpoint)
    var conf = self.device.plugins_config.find(ep_str)
    if conf == nil   conf = {}   self.device.plugins_config[ep_str] = conf   end
    conf['bindings'] = self.bindings
    self.device.plugins_persist = true
    self.device.save_param()
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON
  def append_state_json()
    return f',"Switch":{self.shadow_position}'
  end

  #####################################################################
  # button_handler
  #
  # Args:
  #   - button: (int) button number (base 1)
  #   - mode: (int) 0=static report every second, 1=button state changed (immediate), 2=multi-press status (ignored)
  #   - state: 1=button pressed, 0=button released
  def button_handler(button, mode, state, press_counter)
    if button != self.tasmota_switch_index return end
    # Update position on regular and state-change reports
    if (mode == 0) || (mode == 1)
      self.set_position(state)
    end
    # Multi-press (mode 2) is not declared for this device type
    if mode == 1
      if state              # InitialPress
        self.publish_event(0x003B, 0x01, 1 #-matter.EVENT_INFO-#, matter.TLV.Matter_TLV_item().set(0x06 #-matter.TLV.U4-#, 1))
      else                  # ShortRelease
        self.publish_event(0x003B, 0x03, 1 #-matter.EVENT_INFO-#, matter.TLV.Matter_TLV_item().set(0x06 #-matter.TLV.U4-#, 1))
      end
    end
  end

end
matter.Plugin_Sensor_Doorbell_Btn = Matter_Plugin_Sensor_Doorbell_Btn
