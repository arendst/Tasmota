#
# LoRaWAN AI-Generated Decoder for Dragino DDS75-LB Prompted by ZioFabry 
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.2.8
#
# Homepage:  https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/DDS75-LB_LoRaWAN_Distance_Detection_Sensor_User_Manual/
# Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/DDS75-LB_LoRaWAN_Distance_Detection_Sensor_User_Manual/
# Decoder:   Official Dragino documentation
# 
# v1.0.0 (2025-08-20): Initial generation from MAP specification

class LwDecode_DDS75LB
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
        if !global.contains("DDS75LB_nodes")
            global.DDS75LB_nodes = {}
        end
        if !global.contains("DDS75LB_cmdInit")
            global.DDS75LB_cmdInit = false
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
            var node_data = global.DDS75LB_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 1
                # Periodic Data (8 bytes)
                if size(payload) >= 8
                    # Battery voltage (0-1)
                    var battery_mv = (payload[1] << 8) | payload[0]
                    data['battery_v'] = battery_mv / 1000.0
                    data['battery_mv'] = battery_mv
                    
                    # Distance (2-3)
                    var distance_raw = (payload[3] << 8) | payload[2]
                    if distance_raw == 0x0000
                        data['distance_error'] = "No ultrasonic sensor detected"
                        data['sensor_detected'] = false
                    elif distance_raw == 0x0014
                        data['distance_error'] = "Invalid distance (<280mm)"
                        data['sensor_detected'] = true
                    else
                        data['distance_mm'] = distance_raw
                        data['distance_m'] = distance_raw / 1000.0
                        data['sensor_detected'] = true
                    end
                    
                    # Digital interrupt (4)
                    data['interrupt_triggered'] = payload[4] == 0x01
                    
                    # DS18B20 Temperature (5-6) - optional
                    if size(payload) > 6
                        var temp_raw = (payload[6] << 8) | payload[5]
                        if temp_raw != 0x7FFF
                            if temp_raw & 0xFC00
                                data['temperature'] = (temp_raw - 65536) / 10.0
                            else
                                data['temperature'] = temp_raw / 10.0
                            end
                        end
                    end
                    
                    # Sensor flag (7)
                    if size(payload) > 7
                        data['ultrasonic_sensor'] = payload[7] == 0x01 ? "Detected" : "Not detected"
                    end
                end
                
            elif fport == 5
                # Device Status (7 bytes)
                if size(payload) >= 7
                    data['sensor_model'] = payload[0]
                    data['fw_version'] = f"v{((payload[2] << 8) | payload[1]):04X}"
                    
                    var freq_band = payload[3]
                    var freq_bands = {
                        0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
                        0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-2",
                        0x09: "AS923-3", 0x0A: "AS923-4", 0x0B: "CN470", 0x0C: "EU433",
                        0x0D: "KR920", 0x0E: "MA869"
                    }
                    data['frequency_band'] = freq_bands.find(freq_band, f"Unknown ({freq_band:02X})")
                    data['sub_band'] = payload[4]
                    
                    var battery_mv = (payload[6] << 8) | payload[5]
                    data['battery_v'] = battery_mv / 1000.0
                    data['battery_mv'] = battery_mv
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
                node_data['battery_history'].push(data['battery_v'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Track distance trends
            if data.contains('distance_mm')
                if !node_data.contains('distance_history')
                    node_data['distance_history'] = []
                end
                node_data['distance_history'].push(data['distance_mm'])
                if size(node_data['distance_history']) > 10
                    node_data['distance_history'].pop(0)
                end
            end
            
            # Track interrupt events
            if data.contains('interrupt_triggered') && data['interrupt_triggered']
                node_data['interrupt_count'] = node_data.find('interrupt_count', 0) + 1
                node_data['last_interrupt'] = tasmota.rtc()['local']
            end
            
            # Initialize downlink commands
            if !global.contains("DDS75LB_cmdInit") || !global.DDS75LB_cmdInit
                self.register_downlink_commands()
                global.DDS75LB_cmdInit = true
            end

            # Save back to global storage
            global.DDS75LB_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"DDS75LB: Decode error - {e}: {m}")
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
            var node_data = global.DDS75LB_nodes.find(self.node, {})
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
            name = f"DDS75LB-{self.node}"
        end
        var name_tooltip = "Dragino DDS75-LB Distance Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Distance measurement
        if data_to_show.contains('distance_mm')
            fmt.add_sensor("string", f"{data_to_show['distance_mm']}mm", "Distance", "📏")
        elif data_to_show.contains('distance_error')
            fmt.add_sensor("string", data_to_show['distance_error'], "Distance Error", "❌")
        end
        
        # Temperature if available
        if data_to_show.contains('temperature')
            fmt.add_sensor("string", f"{data_to_show['temperature']:.1f}°C", "Temperature", "🌡️")
        end
        
        # Battery voltage
        if data_to_show.contains('battery_v')
            fmt.add_sensor("volt", data_to_show['battery_v'], "Battery", "🔋")
        end
        
        # Status indicators
        var has_status = false
        if data_to_show.contains('interrupt_triggered') && data_to_show['interrupt_triggered']
            fmt.next_line()
            fmt.add_status("Interrupt", "🔔", "Digital interrupt triggered")
            has_status = true
        end
        
        if data_to_show.contains('sensor_detected') && !data_to_show['sensor_detected']
            if !has_status
                fmt.next_line()
                has_status = true
            end
            fmt.add_status("No Sensor", "❌", "Ultrasonic sensor not detected")
        end
        
        if data_to_show.contains('ultrasonic_sensor')
            if !has_status
                fmt.next_line()
                has_status = true
            end
            var sensor_icon = data_to_show['ultrasonic_sensor'] == "Detected" ? "✅" : "❌"
            fmt.add_sensor("string", data_to_show['ultrasonic_sensor'], "Ultrasonic Sensor", sensor_icon)
        end
        
        # Device info if available
        if data_to_show.contains('frequency_band')
            if !has_status
                fmt.next_line()
                has_status = true
            end
            fmt.add_sensor("string", data_to_show['frequency_band'], "LoRaWAN Band", "📡")
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                if !has_status
                    fmt.next_line()
                    has_status = true
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
        var node_data = global.DDS75LB_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'interrupt_count': node_data.find('interrupt_count', 0),
            'last_interrupt': node_data.find('last_interrupt', 0),
            'battery_history': node_data.find('battery_history', []),
            'distance_history': node_data.find('distance_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.DDS75LB_nodes.contains(node_id)
            global.DDS75LB_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set transmit interval
        tasmota.remove_cmd("LwDDS75LBSetInterval")
        tasmota.add_cmd("LwDDS75LBSetInterval", def(cmd, idx, payload_str)
            # Format: LwDDS75LBSetInterval<slot> <seconds>
            var interval = int(payload_str)
            if interval < 10 || interval > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 10-16777215 seconds")
            end
            
            # Build hex command: 01 + 3 bytes little endian
            var hex_cmd = f"01{interval & 0xFF:02X}{(interval >> 8) & 0xFF:02X}{(interval >> 16) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.DDS75LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set interrupt mode
        tasmota.remove_cmd("LwDDS75LBSetInterrupt")
        tasmota.add_cmd("LwDDS75LBSetInterrupt", def(cmd, idx, payload_str)
            # Format: LwDDS75LBSetInterrupt<slot> <disable|rising>
            return lwdecode.SendDownlinkMap(global.DDS75LB_nodes, cmd, idx, payload_str, {
                'DISABLE|0': ['06000000', 'Disabled'],
                'RISING|1|ENABLE': ['06000003', 'Rising Edge']
            })
        end)
        
        # Set delta detect mode
        tasmota.remove_cmd("LwDDS75LBSetDelta")
        tasmota.add_cmd("LwDDS75LBSetDelta", def(cmd, idx, payload_str)
            # Format: LwDDS75LBSetDelta<slot> <mode>,<interval>,<threshold>,<samples>
            var parts = string.split(payload_str, ',')
            if size(parts) != 4
                return tasmota.resp_cmnd_str("Usage: LwDDS75LBSetDelta<slot> <mode>,<interval>,<threshold>,<samples>")
            end
            
            var mode = int(parts[0])
            var interval = int(parts[1])
            var threshold = int(parts[2])
            var samples = int(parts[3])
            
            if mode < 1 || mode > 2
                return tasmota.resp_cmnd_str("Invalid mode: 1=normal, 2=delta")
            end
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str("Invalid interval: range 1-65535 seconds")
            end
            if threshold < 1 || threshold > 65535
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-65535 cm")
            end
            if samples < 5 || samples > 20
                return tasmota.resp_cmnd_str("Invalid samples: range 5-20")
            end
            
            var hex_cmd = f"FB{mode:02X}{lwdecode.uint16be(interval)}{lwdecode.uint16be(threshold)}{samples:02X}"
            return lwdecode.SendDownlink(global.DDS75LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request device status
        tasmota.remove_cmd("LwDDS75LBGetStatus")
        tasmota.add_cmd("LwDDS75LBGetStatus", def(cmd, idx, payload_str)
            # Format: LwDDS75LBGetStatus<slot>
            var hex_cmd = "2601"
            return lwdecode.SendDownlink(global.DDS75LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Poll sensor data
        tasmota.remove_cmd("LwDDS75LBPoll")
        tasmota.add_cmd("LwDDS75LBPoll", def(cmd, idx, payload_str)
            # Format: LwDDS75LBPoll<slot> <start_time>,<end_time>,<interval>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwDDS75LBPoll<slot> <start_unix>,<end_unix>,<interval_sec>")
            end
            
            var start_time = int(parts[0])
            var end_time = int(parts[1])
            var interval = int(parts[2])
            
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            var hex_cmd = f"31{lwdecode.uint32be(start_time)}{lwdecode.uint32be(end_time)}{interval:02X}"
            return lwdecode.SendDownlink(global.DDS75LB_nodes, cmd, idx, hex_cmd)
        end)
        
        print("DDS75LB: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_DDS75LB()

# Node management commands
tasmota.remove_cmd("LwDDS75LBNodeStats")
tasmota.add_cmd("LwDDS75LBNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwDDS75LBClearNode")
tasmota.add_cmd("LwDDS75LBClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwDDS75LBTestUI<slot> <scenario>
tasmota.remove_cmd("LwDDS75LBTestUI")
tasmota.add_cmd("LwDDS75LBTestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "normal":    "C60E340300001C14501",      # Normal: 3.78V, 1076mm, temp 22.0°C
        "close":     "C60E1C0100001C14501",      # Close object: 3.78V, 284mm
        "far":       "C60E2C1700001C14501",      # Far object: 3.78V, 5932mm
        "interrupt": "C60E340301001C14501",      # Interrupt triggered: 3.78V, 1076mm
        "no_sensor": "C60E000000001C14500",      # No sensor: 3.78V, no ultrasonic
        "invalid":   "C60E140000001C14501",      # Invalid reading: 3.78V, <280mm
        "low":       "B809340300001C14501",      # Low battery: 2.5V, 1076mm
        "status":    "2701001000C60E",           # Device status: v0.10, EU868, 3.78V
        "cold":      "C60E3403000038FE501",      # Cold temp: 3.78V, 1076mm, -20.0°C
        "hot":       "C60E3403000058020501"      # Hot temp: 3.78V, 1076mm, 60.0°C
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
    var fport = 1
    
    # Adjust fport for status scenario
    if payload_str == "status"
        fport = 5
    end

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
