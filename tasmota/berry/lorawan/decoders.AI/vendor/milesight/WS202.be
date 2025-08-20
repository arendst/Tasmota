#
# LoRaWAN AI-Generated Decoder for Milesight WS202 Prompted by ZioFabry 
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.2.8
#
# Homepage:  https://resource.milesight.com/milesight/iot/document/ws202-user-guide-en.pdf
# Userguide: https://resource.milesight.com/milesight/iot/document/ws202-user-guide-en.pdf
# Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# 
# v1.0.0 (2025-08-20): Initial generation from MAP specification

class LwDecode_WS202
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
        if !global.contains("WS202_nodes")
            global.WS202_nodes = {}
        end
        if !global.contains("WS202_cmdInit")
            global.WS202_cmdInit = false
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
            var node_data = global.WS202_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 85
                var i = 0
                while i < size(payload)
                    if i + 1 >= size(payload) break end
                    
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # Device information channels
                    if channel_id == 0xFF && channel_type == 0x01
                        # Protocol version
                        if i < size(payload)
                            data['protocol_version'] = f"V{payload[i]}"
                            i += 1
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x08
                        # Device serial number
                        if i + 5 < size(payload)
                            var serial = ""
                            for j: i..(i+5)
                                serial += string.format("%02X", payload[j])
                            end
                            data['serial_number'] = serial
                            i += 6
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x09
                        # Hardware version
                        if i + 1 < size(payload)
                            data['hw_version'] = f"V{payload[i]}.{payload[i+1]}"
                            i += 2
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x0A
                        # Software version
                        if i + 1 < size(payload)
                            data['sw_version'] = f"V{payload[i]}.{payload[i+1]}"
                            i += 2
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x0B
                        # Power on event
                        if i < size(payload)
                            data['power_on_event'] = payload[i] == 0xFF
                            i += 1
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x0F
                        # Device type
                        if i < size(payload)
                            var device_type = payload[i]
                            if device_type == 0x00
                                data['device_class'] = "Class A"
                            elif device_type == 0x01
                                data['device_class'] = "Class B"
                            elif device_type == 0x02
                                data['device_class'] = "Class C"
                            else
                                data['device_class'] = f"Unknown ({device_type})"
                            end
                            i += 1
                        end
                        
                    # Sensor data channels
                    elif channel_id == 0x01 && channel_type == 0x75
                        # Battery level
                        if i < size(payload)
                            data['battery_pct'] = payload[i]
                            data['battery_v'] = 100000 + payload[i]  # Framework format for percentage
                            i += 1
                        end
                        
                    elif channel_id == 0x03 && channel_type == 0x00
                        # PIR status
                        if i < size(payload)
                            var pir_status = payload[i]
                            data['pir_status'] = pir_status == 0x01 ? "Occupied" : "Vacant"
                            data['pir_occupied'] = pir_status == 0x01
                            i += 1
                        end
                        
                    elif channel_id == 0x04 && channel_type == 0x00
                        # Light status
                        if i < size(payload)
                            var light_status = payload[i]
                            data['light_status'] = light_status == 0x01 ? "Bright" : "Dark"
                            data['light_bright'] = light_status == 0x01
                            i += 1
                        end
                        
                    else
                        # Unknown channel, try to skip safely
                        print(f"WS202: Unknown channel ID={channel_id:02X} Type={channel_type:02X}")
                        break
                    end
                end
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store battery trend if available
            if data.contains('battery_pct')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                # Keep last 10 battery readings
                node_data['battery_history'].push(data['battery_pct'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Store reset count if detected
            if data.contains('power_on_event') && data['power_on_event']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Initialize downlink commands
            if !global.contains("WS202_cmdInit") || !global.WS202_cmdInit
                self.register_downlink_commands()
                global.WS202_cmdInit = true
            end

            # Save back to global storage
            global.WS202_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"WS202: Decode error - {e}: {m}")
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
            var node_data = global.WS202_nodes.find(self.node, {})
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
            name = f"WS202-{self.node}"
        end
        var name_tooltip = "Milesight WS202"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # PIR and Light status
        if data_to_show.contains('pir_status')
            var pir_icon = data_to_show['pir_occupied'] ? "🚶" : "🏠"
            var pir_tooltip = f"PIR: {data_to_show['pir_status']}"
            fmt.add_sensor("string", data_to_show['pir_status'], pir_tooltip, pir_icon)
        end
        
        if data_to_show.contains('light_status')
            var light_icon = data_to_show['light_bright'] ? "☀️" : "🌙"
            var light_tooltip = f"Light: {data_to_show['light_status']}"
            fmt.add_sensor("string", data_to_show['light_status'], light_tooltip, light_icon)
        end
        
        # Battery percentage if available
        if data_to_show.contains('battery_pct')
            fmt.add_sensor("string", f"{data_to_show['battery_pct']}%", "Battery Level", "🔋")
        end
        
        # Device events
        var has_events = false
        if data_to_show.contains('power_on_event') && data_to_show['power_on_event']
            fmt.next_line()
            fmt.add_status("Power On", "⚡", "Device power on event")
            has_events = true
        end
        
        # Device info if available
        if data_to_show.contains('device_class')
            if !has_events
                fmt.next_line()
                has_events = true
            end
            fmt.add_sensor("string", data_to_show['device_class'], "LoRaWAN Class", "📡")
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                if !has_events
                    fmt.next_line()
                    has_events = true
                end
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
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.WS202_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.WS202_nodes.contains(node_id)
            global.WS202_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set reporting interval command
        tasmota.remove_cmd("LwWS202SetInterval")
        tasmota.add_cmd("LwWS202SetInterval", def(cmd, idx, payload_str)
            # Format: LwWS202SetInterval<slot> <interval_seconds>
            var interval = int(payload_str)
            if interval < 60 || interval > 64800
                return tasmota.resp_cmnd_str("Invalid: range 60-64800 seconds")
            end
            
            # Build hex command per PDF specification: FF03 + 2 bytes little endian
            var hex_cmd = f"FF03{lwdecode.uint16le(interval)}"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        # Reboot device command
        tasmota.remove_cmd("LwWS202Reboot")
        tasmota.add_cmd("LwWS202Reboot", def(cmd, idx, payload_str)
            # Format: LwWS202Reboot<slot>
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS202: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS202()

# Node management commands
tasmota.remove_cmd("LwWS202NodeStats")
tasmota.add_cmd("LwWS202NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwWS202ClearNode")
tasmota.add_cmd("LwWS202ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwWS202TestUI<slot> <scenario>
tasmota.remove_cmd("LwWS202TestUI")
tasmota.add_cmd("LwWS202TestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "normal":    "017564030000040000",      # Normal: 100% battery, vacant, dark
        "occupied":  "017564030001040001",      # Occupied: 100% battery, occupied, bright
        "vacant":    "017564030000040001",      # Vacant bright: 100% battery, vacant, bright
        "low":       "01750A030001040000",      # Low battery: 10% battery, occupied, dark
        "info":      "FF0BFF FF0101 FF086538B2232131 FF090100 FF0A0101 FF0F00",  # Device info
        "config":    "017550030000040001"       # Config response: 80% battery, vacant, bright
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
    var fport = 85

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
