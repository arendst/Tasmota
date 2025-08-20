#
# LoRaWAN AI-Generated Decoder for Milesight WS301
#
# Generated: 2025-08-20 | Version: 1.3.0 | Revision: 003
#            by "LoRaWAN Decoder AI Generation Template", v2.2.6
#
# Homepage:  https://www.milesight-iot.com/lorawan/sensor/ws301/
# Userguide: https://resource.milesight.com/milesight/iot/document/ws301-user-guide-en.pdf
# Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# 
# v1.3.0 (2025-08-20): Complete regeneration with framework v2.2.6
# v1.2.0 (2025-08-20): Enhanced door state tracking and security features
# v1.1.0 (2025-08-16): Added comprehensive test scenarios
# v1.0.0 (2025-08-15): Initial generation from PDF specification

class LwDecode_WS301
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
        if !global.contains("WS301_nodes")
            global.WS301_nodes = {}
        end
        if !global.contains("WS301_cmdInit")
            global.WS301_cmdInit = false
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload, simulated)
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
            data['simulated'] = simulated
            
            # Retrieve node history from global storage
            var node_data = global.WS301_nodes.find(node, {})
            
            # Parse multi-channel payload
            var i = 0
            while i < size(payload) - 1
                var channel_id = payload[i]
                var channel_type = payload[i+1]
                i += 2
                
                # Device Information Channels (0xFF)
                if channel_id == 0xFF
                    if channel_type == 0x01 && i < size(payload)
                        # Protocol Version
                        data['protocol_version'] = payload[i]
                        i += 1
                        
                    elif channel_type == 0x08 && i + 5 < size(payload)
                        # Device Serial Number (6 bytes)
                        var sn_hex = ""
                        for j: 0..5
                            sn_hex += string.format("%02X", payload[i+j])
                        end
                        data['device_sn'] = sn_hex
                        i += 6
                        
                    elif channel_type == 0x09 && i + 1 < size(payload)
                        # Hardware Version
                        var hw_major = payload[i]
                        var hw_minor = payload[i+1]
                        data['hw_version'] = string.format("V%d.%d", hw_major, hw_minor)
                        i += 2
                        
                    elif channel_type == 0x0A && i + 1 < size(payload)
                        # Software Version
                        var sw_major = payload[i]
                        var sw_minor = payload[i+1]
                        data['sw_version'] = string.format("V%d.%d", sw_major, sw_minor)
                        i += 2
                        
                    elif channel_type == 0x0B && i < size(payload)
                        # Power On Event
                        data['power_on_event'] = true
                        data['device_reset'] = true
                        i += 1
                        
                    elif channel_type == 0x0F && i < size(payload)
                        # Device Type
                        var device_type = payload[i]
                        if device_type == 0x00
                            data['device_class'] = "Class A"
                        elif device_type == 0x01
                            data['device_class'] = "Class B"
                        elif device_type == 0x02
                            data['device_class'] = "Class C"
                        else
                            data['device_class'] = string.format("Unknown (%02X)", device_type)
                        end
                        i += 1
                    else
                        # Unknown FF channel type
                        break
                    end
                    
                # Battery Level (0x01, 0x75)
                elif channel_id == 0x01 && channel_type == 0x75 && i < size(payload)
                    data['battery_level'] = payload[i]
                    data['battery_v'] = payload[i] / 100.0 * 3.6  # Approximate voltage
                    i += 1
                    
                # Magnet Status (0x03, 0x00)
                elif channel_id == 0x03 && channel_type == 0x00 && i < size(payload)
                    var door_state = payload[i]
                    data['door_state'] = door_state
                    data['door_open'] = door_state == 0x01
                    
                    # Track door state changes
                    var prev_state = node_data.find('last_door_state', nil)
                    if prev_state != nil && prev_state != door_state
                        data['door_state_changed'] = true
                        node_data['door_change_count'] = node_data.find('door_change_count', 0) + 1
                        node_data['last_door_change'] = tasmota.rtc()['local']
                    end
                    node_data['last_door_state'] = door_state
                    i += 1
                    
                # Tamper Status (0x04, 0x00)
                elif channel_id == 0x04 && channel_type == 0x00 && i < size(payload)
                    var tamper_state = payload[i]
                    data['tamper_detected'] = tamper_state == 0x01
                    data['device_installed'] = tamper_state == 0x00
                    
                    # Track tamper events
                    if tamper_state == 0x01
                        node_data['tamper_count'] = node_data.find('tamper_count', 0) + 1
                        node_data['last_tamper'] = tasmota.rtc()['local']
                    end
                    i += 1
                    
                else
                    # Unknown channel
                    print(string.format("WS301: Unknown channel ID=0x%02X Type=0x%02X", channel_id, channel_type))
                    break
                end
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store battery trend if available
            if data.contains('battery_level')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                # Keep last 10 battery readings
                node_data['battery_history'].push(data['battery_level'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Implement downlinks if present and create relative tasmota commands
            if !global.contains("WS301_cmdInit") || !global.WS301_cmdInit
                self.register_downlink_commands()
                global.WS301_cmdInit = true
            end

            # Save back to global storage
            global.WS301_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(string.format("WS301: Decode error - %s: %s", e, m))
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
            var node_data = global.WS301_nodes.find(self.node, {})
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
            name = string.format("WS301-%s", self.node)
        end
        var name_tooltip = "Milesight WS301"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        
        fmt.start_line()
        
        # Door state with appropriate emoji
        if data_to_show.contains('door_open')
            var door_emoji = data_to_show['door_open'] ? "🔓" : "🔒"
            var door_text = data_to_show['door_open'] ? "Open" : "Closed"
            fmt.add_sensor("string", door_text, "Door State", door_emoji)
        end
        
        # Battery level
        if data_to_show.contains('battery_level')
            fmt.add_sensor("string", string.format("%d%%", data_to_show['battery_level']), "Battery", "🔋")
        end
        
        # Environmental data if available
        if data_to_show.contains('temperature')
            fmt.add_sensor("string", string.format("%.1f°C", data_to_show['temperature']), "Temp", "🌡️")
        end
        
        if data_to_show.contains('humidity')
            fmt.add_sensor("string", string.format("%.0f%%", data_to_show['humidity']), "Humidity", "💧")
        end
        
        fmt.next_line()
        
        # Installation and tamper status
        if data_to_show.contains('device_installed')
            var install_emoji = data_to_show['device_installed'] ? "✅" : "❌"
            var install_text = data_to_show['device_installed'] ? "Installed" : "Tamper"
            fmt.add_sensor("string", install_text, "Installation", install_emoji)
        end
        
        # Device reset indicator
        if data_to_show.contains('device_reset') && data_to_show['device_reset']
            fmt.add_sensor("string", "Reset", "Device Event", "🔄")
        end
        
        # Power on event indicator
        if data_to_show.contains('power_on_event') && data_to_show['power_on_event']
            fmt.add_sensor("string", "Power On", "Device Event", "⚡")
        end
        
        # Device information display
        if data_to_show.contains('hw_version')
            fmt.add_sensor("string", data_to_show['hw_version'], "Hardware", "🔧")
        end
        
        fmt.end_line()
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.start_line()
                fmt.add_sensor("string", self.format_age(age), "Last Seen", "⏱️")
                fmt.end_line()
            end
        end
        
        msg += fmt.get_msg()

        return msg
    end
    
    def format_age(seconds)
        if seconds < 60 return string.format("%ds ago", seconds)
        elif seconds < 3600 return string.format("%dm ago", seconds/60)
        elif seconds < 86400 return string.format("%dh ago", seconds/3600)
        else return string.format("%dd ago", seconds/86400)
        end
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.WS301_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown'),
            'door_change_count': node_data.find('door_change_count', 0),
            'last_door_change': node_data.find('last_door_change', 0),
            'tamper_count': node_data.find('tamper_count', 0),
            'last_tamper': node_data.find('last_tamper', 0)
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.WS301_nodes.contains(node_id)
            global.WS301_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Reporting Interval command
        tasmota.remove_cmd("LwWS301SetInterval")
        tasmota.add_cmd("LwWS301SetInterval", def(cmd, idx, payload_str)
            # Format: LwWS301SetInterval<slot> <minutes>
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 1080
                return tasmota.resp_cmnd_str("Invalid: range 1-1080 minutes")
            end
            
            # Convert minutes to seconds and build hex command (little endian)
            var seconds = minutes * 60
            var hex_cmd = string.format("FF03%02X%02X", seconds & 0xFF, (seconds >> 8) & 0xFF)
            return lwdecode.SendDownlink(global.WS301_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device reboot command (using power on command)
        tasmota.remove_cmd("LwWS301Reboot")
        tasmota.add_cmd("LwWS301Reboot", def(cmd, idx, payload_str)
            # Format: LwWS301Reboot<slot>
            # Send power on event trigger (if supported)
            var hex_cmd = "FF0BFF"  # Power on command
            return lwdecode.SendDownlink(global.WS301_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS301: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS301()

# Node management commands
tasmota.remove_cmd("LwWS301NodeStats")
tasmota.add_cmd("LwWS301NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwWS301ClearNode")
tasmota.add_cmd("LwWS301ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwWS301TestUI<slot> <scenario>
tasmota.remove_cmd("LwWS301TestUI")
tasmota.add_cmd("LwWS301TestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "normal":    "017564030000040000",                    # Door closed, installed, 100% battery
        "open":      "017564030001040000",                    # Door open, installed, 100% battery  
        "tamper":    "017564030000040001",                    # Door closed, tamper detected, 100% battery
        "low":       "01750A030001040000",                    # Door open, low battery (10%), installed
        "info":      "FF0BFFFF0101FF086538B2232131FF090140FF0A0114FF0F00",  # Device info
        "startup":   "FF0BFF017564030000040000"           # Power on + sensor data
    }
    
    var hex_payload = test_scenarios.find(payload_str != nil ? payload_str : 'nil', 'not_found')
    
    if hex_payload == 'not_found'
      var scenarios_list = ""
      for key: test_scenarios.keys()
        scenarios_list += key + " "
      end
      return tasmota.resp_cmnd_str(string.format("Available scenarios: %s", scenarios_list))
    end
    
    var rssi = -75
    var fport = 85

    return tasmota.cmd(string.format('LwSimulate%d %d,%d,%s', idx, rssi, fport, hex_payload))
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
