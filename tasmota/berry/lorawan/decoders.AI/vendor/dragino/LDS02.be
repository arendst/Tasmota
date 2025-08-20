#
# LoRaWAN AI-Generated Decoder for Dragino LDS02 Prompted by ZioFabry
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.0
#
# Homepage:  https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/
# Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/
# Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/LDS02
# 
# v1.0.0 (2025-08-20): Initial generation from cached MAP specification

class LwDecode_LDS02
    var hashCheck      # Duplicate payload detection flag (true = skip duplicates)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    var lwdecode       # global instance of the driver

    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("LDS02_nodes")
            global.LDS02_nodes = {}
        end
        if !global.contains("LDS02_cmdInit")
            global.LDS02_cmdInit = false
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        # Validate inputs
        if payload == nil || size(payload) < 1
            return nil
        end
        
        try
            # Store device info
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            # Retrieve node history from global storage
            var node_data = global.LDS02_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 10  # Sensor Data
                if size(payload) >= 10
                    # Battery & Door Status (bytes 0-1)
                    var raw_bat_door = (payload[1] << 8) | payload[0]
                    data['battery_mv'] = raw_bat_door & 0x3FFF
                    data['battery_v'] = data['battery_mv'] / 1000.0
                    data['door_status'] = (raw_bat_door & 0x8000) != 0
                    data['door_state'] = data['door_status'] ? "Open" : "Closed"
                    
                    # Mode (byte 2)
                    data['mode'] = payload[2] == 1 ? "Normal" : "Unknown"
                    
                    # Total Door Open Events (bytes 3-5)
                    data['total_events'] = (payload[5] << 16) | (payload[4] << 8) | payload[3]
                    
                    # Last Door Open Duration (bytes 6-8)
                    data['last_duration_min'] = (payload[8] << 16) | (payload[7] << 8) | payload[6]
                    
                    # Alarm Status (byte 9)
                    if size(payload) > 9
                        data['alarm_active'] = (payload[9] & 0x01) != 0
                        data['alarm_status'] = data['alarm_active'] ? "Timeout Alarm" : "No Alarm"
                    end
                end
                
            elif fport == 7  # EDC Mode Data
                if size(payload) >= 5
                    # Battery & EDC Mode (bytes 0-1)
                    var raw_bat_edc = (payload[1] << 8) | payload[0]
                    data['battery_mv'] = raw_bat_edc & 0x3FFF
                    data['battery_v'] = data['battery_mv'] / 1000.0
                    data['edc_mode'] = (raw_bat_edc & 0x8000) != 0
                    data['edc_type'] = data['edc_mode'] ? "Open Count" : "Close Count"
                    
                    # Event Count (bytes 2-4)
                    data['event_count'] = (payload[4] << 16) | (payload[3] << 8) | payload[2]
                end
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store battery trend if available
            if data.contains('battery_v')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                # Keep last 10 battery readings
                node_data['battery_history'].push(data['battery_v'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Track door events
            if data.contains('total_events')
                var prev_events = node_data.find('last_total_events', 0)
                if data['total_events'] > prev_events
                    node_data['recent_activity'] = true
                    node_data['last_event_time'] = tasmota.rtc()['local']
                end
                node_data['last_total_events'] = data['total_events']
            end
            
            # Initialize downlink commands
            if !global.contains("LDS02_cmdInit") || !global.LDS02_cmdInit
                self.register_downlink_commands()
                global.LDS02_cmdInit = true
            end

            # Save back to global storage
            global.LDS02_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"LDS02: Decode error - {e}: {m}")
            return nil
        end
    end
    
    def add_web_sensor()
        import global
        
        # Try to use current instance data first
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        # If no instance data, try to recover from global storage
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.LDS02_nodes.find(self.node, {})
            data_to_show = node_data.find('last_data', {})
            last_update = node_data.find('last_update', 0)
        end
        
        if size(data_to_show) == 0 return nil end
        
        import string
        var msg = ""
        var fmt = LwSensorFormatter_cls()
        
        # MANDATORY: Add header line with device info
        var name = self.name
        if name == nil || name == ""
            name = f"LDS02-{self.node}"
        end
        var name_tooltip = "Dragino LDS02"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Main sensor line
        if data_to_show.contains('door_state')
            var door_icon = data_to_show['door_status'] ? "🔓" : "🔒"
            fmt.add_sensor("string", data_to_show['door_state'], "Door Status", door_icon)
        end
        
        if data_to_show.contains('total_events')
            fmt.add_sensor("string", f"{data_to_show['total_events']}", "Total Events", "📊")
        end
        
        if data_to_show.contains('last_duration_min')
            var duration = data_to_show['last_duration_min']
            var duration_text = duration > 0 ? f"{duration}min" : "0min"
            fmt.add_sensor("string", duration_text, "Last Open Duration", "⏱️")
        end
        
        # EDC mode info line (if present)
        if data_to_show.contains('edc_type')
            fmt.next_line()
            fmt.add_sensor("string", data_to_show['edc_type'], "EDC Mode", "⚙️")
            if data_to_show.contains('event_count')
                fmt.add_sensor("string", f"{data_to_show['event_count']}", "EDC Count", "🔢")
            end
        end
        
        # Battery and status line
        if data_to_show.contains('battery_v') || data_to_show.contains('alarm_status')
            if !data_to_show.contains('edc_type')
                fmt.next_line()
            end
            
            if data_to_show.contains('battery_v')
                var batt_pct = self.voltage_to_percent(data_to_show['battery_v'])
                fmt.add_sensor("string", f"{batt_pct}%", "Battery", "🔋")
            end
            
            if data_to_show.contains('alarm_status') && data_to_show['alarm_active']
                fmt.add_sensor("string", "Alarm", "Timeout Alert", "🚨")
            end
            
            if data_to_show.contains('mode') && data_to_show['mode'] != "Normal"
                fmt.add_sensor("string", data_to_show['mode'], "Mode", "⚙️")
            end
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.next_line()
                fmt.add_status(self.format_age(age), "⏱️", nil)
            end
        end
        
        fmt.end_line()
        
        # ONLY get_msg() return a string that can be used with +=
        msg += fmt.get_msg()

        return msg
    end
    
    def format_age(seconds)
        if seconds < 60 return f"{seconds}s ago"
        elif seconds < 3600 return f"{seconds/60}m ago"
        elif seconds < 86400 return f"{seconds/3600}h ago"
        else return f"{seconds/86400}d ago"
        end
    end
    
    def voltage_to_percent(voltage)
        # AAA battery curve approximation (2.1V - 3.0V range)
        if voltage >= 3.0 return 100
        elif voltage <= 2.1 return 0
        else
            # Linear approximation for AAA batteries
            return int((voltage - 2.1) / 0.9 * 100)
        end
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.LDS02_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'last_total_events': node_data.find('last_total_events', 0),
            'last_event_time': node_data.find('last_event_time', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.LDS02_nodes.contains(node_id)
            global.LDS02_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Transmit Interval command
        tasmota.remove_cmd("LwLDS02SetInterval")
        tasmota.add_cmd("LwLDS02SetInterval", def(cmd, idx, payload_str)
            # Format: LwLDS02SetInterval<slot> <seconds>
            var interval = int(payload_str)
            if interval < 1 || interval > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 1-4294967295 seconds")
            end
            
            # Build hex command: 01 + 32-bit little endian interval
            var hex_cmd = f"01{lwdecode.uint32le(interval)}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Reset Device command
        tasmota.remove_cmd("LwLDS02Reset")
        tasmota.add_cmd("LwLDS02Reset", def(cmd, idx, payload_str)
            # Format: LwLDS02Reset<slot>
            var hex_cmd = "04FF"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Enable/Disable Alarm command
        tasmota.remove_cmd("LwLDS02Alarm")
        tasmota.add_cmd("LwLDS02Alarm", def(cmd, idx, payload_str)
            # Format: LwLDS02Alarm<slot> <enable|disable|1|0>
            return lwdecode.SendDownlinkMap(global.LDS02_nodes, cmd, idx, payload_str, { 
                '1|ENABLE':  ['A701', 'Alarm Enabled'],
                '0|DISABLE': ['A700', 'Alarm Disabled']
            })
        end)
        
        # Clear Counting command
        tasmota.remove_cmd("LwLDS02Clear")
        tasmota.add_cmd("LwLDS02Clear", def(cmd, idx, payload_str)
            # Format: LwLDS02Clear<slot>
            var hex_cmd = "A601"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Confirmed Mode command
        tasmota.remove_cmd("LwLDS02Confirm")
        tasmota.add_cmd("LwLDS02Confirm", def(cmd, idx, payload_str)
            # Format: LwLDS02Confirm<slot> <confirmed|unconfirmed|1|0>
            return lwdecode.SendDownlinkMap(global.LDS02_nodes, cmd, idx, payload_str, { 
                '1|CONFIRMED':    ['0501', 'Confirmed Mode'],
                '0|UNCONFIRMED':  ['0500', 'Unconfirmed Mode']
            })
        end)
        
        # Set EDC Mode command
        tasmota.remove_cmd("LwLDS02EDC")
        tasmota.add_cmd("LwLDS02EDC", def(cmd, idx, payload_str)
            # Format: LwLDS02EDC<slot> <mode>,<count>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwLDS02EDC<slot> <open|close>,<count>")
            end
            
            var mode_str = string.toupper(parts[0])
            var count = int(parts[1])
            
            if count < 1 || count > 4294967295
                return tasmota.resp_cmnd_str("Invalid count: range 1-4294967295")
            end
            
            var mode = mode_str == "OPEN" ? 1 : 0
            var hex_cmd = f"02{mode:02X}{lwdecode.uint32le(count)}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Alarm Timeout command
        tasmota.remove_cmd("LwLDS02Timeout")
        tasmota.add_cmd("LwLDS02Timeout", def(cmd, idx, payload_str)
            # Format: LwLDS02Timeout<slot> <seconds>
            var timeout = int(payload_str)
            if timeout < 0 || timeout > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535 seconds")
            end
            
            var status = timeout > 0 ? 1 : 0
            var hex_cmd = f"A9{status:02X}{lwdecode.uint16be(timeout)}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Count Value command
        tasmota.remove_cmd("LwLDS02SetCount")
        tasmota.add_cmd("LwLDS02SetCount", def(cmd, idx, payload_str)
            # Format: LwLDS02SetCount<slot> <count> or LwLDS02SetCount<slot> <mode>,<count>
            var parts = string.split(payload_str, ',')
            var count, mode_byte
            
            if size(parts) == 1
                # Normal mode
                count = int(parts[0])
                mode_byte = ""
            elif size(parts) == 2
                # EDC mode
                var mode_str = string.toupper(parts[0])
                count = int(parts[1])
                mode_byte = mode_str == "OPEN" ? "01" : "00"
            else
                return tasmota.resp_cmnd_str("Usage: LwLDS02SetCount<slot> <count> or <mode>,<count>")
            end
            
            if count < 0 || count > 16777215
                return tasmota.resp_cmnd_str("Invalid count: range 0-16777215")
            end
            
            # Build hex command: AA + mode (if EDC) + 24-bit big endian count
            var count_hex = f"{(count >> 16) & 0xFF:02X}{(count >> 8) & 0xFF:02X}{count & 0xFF:02X}"
            var hex_cmd = f"AA{mode_byte}{count_hex}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        print("LDS02: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_LDS02()

# Node management commands
tasmota.remove_cmd("LwLDS02NodeStats")
tasmota.add_cmd("LwLDS02NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwLDS02ClearNode")
tasmota.add_cmd("LwLDS02ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwLDS02TestUI<slot> <scenario>
tasmota.remove_cmd("LwLDS02TestUI")
tasmota.add_cmd("LwLDS02TestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "closed":     "880B019300002500000F",      # Door closed, 147 events, 37min duration ✅
        "open":       "52D101AB00001E00000F",      # Door open, 171 events, 30min duration ✅
        "alarm":      "52D101940000003C0001",      # Door open with timeout alarm ✅
        "low_battery":"880801590000150000000F",    # Low battery (2.1V), closed door ✅
        "edc_open":   "608C0A00000000",            # EDC mode - open count = 10 ✅
        "edc_close":  "600C140000000",             # EDC mode - close count = 20 ✅
        "new_device": "880B01000000000000000F"       # New device, no events yet ✅
    }
    
    var hex_payload = test_scenarios.find(payload_str ? payload_str : 'nil', 'not_found')
    
    if hex_payload == 'not_found'
      var scenarios_list = ""
      for key: test_scenarios.keys()
        scenarios_list += key + " "
      end
      return tasmota.resp_cmnd_str(format("Available scenarios: %s", scenarios_list))
    end
    
    var rssi = -75
    var fport = payload_str == "edc_open" || payload_str == "edc_close" ? 7 : 10

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
