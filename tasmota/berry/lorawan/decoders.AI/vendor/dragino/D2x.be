#
# LoRaWAN AI-Generated Decoder for Dragino D2x Prompted by ZioFabry 
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.2.8
#
# Homepage:  http://wiki.dragino.com/xwiki/bin/view/Main/User Manual for LoRaWAN End Nodes/D20-LBD22-LBD23-LB_LoRaWAN_Temperature_Sensor_User_Manual/
# Userguide: http://wiki.dragino.com/xwiki/bin/view/Main/User Manual for LoRaWAN End Nodes/D20-LBD22-LBD23-LB_LoRaWAN_Temperature_Sensor_User_Manual/
# Decoder:   Official Dragino documentation
# 
# v1.0.0 (2025-08-20): Initial generation from MAP specification

class LwDecode_D2x
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
        if !global.contains("D2x_nodes")
            global.D2x_nodes = {}
        end
        if !global.contains("D2x_cmdInit")
            global.D2x_cmdInit = false
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
            var node_data = global.D2x_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 5
                # Device Status
                if size(payload) >= 6
                    data['sensor_model'] = payload[0]
                    data['fw_version'] = f"v{payload[1]}.{payload[2]}"
                    
                    var freq_band = payload[3]
                    var freq_bands = {
                        0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
                        0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-1",
                        0x09: "AS923-2", 0x0a: "AS923-3", 0x0b: "CN470", 0x0c: "EU433",
                        0x0d: "KR920", 0x0e: "MA869"
                    }
                    data['frequency_band'] = freq_bands.find(freq_band, f"Unknown ({freq_band:02X})")
                    data['sub_band'] = payload[4]
                    
                    var battery_mv = size(payload) > 6 ? ((payload[5] << 8) | payload[6]) : (payload[5] << 8)
                    data['battery_v'] = battery_mv / 1000.0
                    data['battery_mv'] = battery_mv
                end
                
            elif fport == 2
                # Sensor Data
                if size(payload) >= 11
                    var battery_mv = (payload[0] << 8) | payload[1]
                    data['battery_v'] = battery_mv / 1000.0
                    data['battery_mv'] = battery_mv
                    
                    # Temperature Red/White (main probe)
                    var temp_raw = (payload[2] << 8) | payload[3]
                    if temp_raw != 0x7FFF
                        if temp_raw > 32767
                            temp_raw = temp_raw - 65536  # Convert to signed
                        end
                        data['temp_red_white'] = temp_raw / 10.0
                    end
                    
                    # Skip reserved bytes [4-5]
                    
                    # Alarm/MOD/PA8 flags
                    var flags = payload[6]
                    data['alarm_flag'] = (flags & 0x01) != 0
                    data['pa8_level'] = ((flags & 0x80) != 0) ? "Low" : "High"
                    data['mod'] = (flags >> 2) & 0x1F
                    
                    # Temperature White (D22/D23)
                    var temp_white_raw = (payload[7] << 8) | payload[8]
                    if temp_white_raw != 0x7FFF
                        if temp_white_raw > 32767
                            temp_white_raw = temp_white_raw - 65536
                        end
                        data['temp_white'] = temp_white_raw / 10.0
                    end
                    
                    # Temperature Black (D23)
                    var temp_black_raw = (payload[9] << 8) | payload[10]
                    if temp_black_raw != 0x7FFF
                        if temp_black_raw > 32767
                            temp_black_raw = temp_black_raw - 65536
                        end
                        data['temp_black'] = temp_black_raw / 10.0
                    end
                end
                
            elif fport == 3
                # Datalog
                if size(payload) >= 11
                    # Temperature Black
                    var temp_black_raw = (payload[0] << 8) | payload[1]
                    if temp_black_raw != 0x7FFF
                        if temp_black_raw > 32767
                            temp_black_raw = temp_black_raw - 65536
                        end
                        data['temp_black'] = temp_black_raw / 10.0
                    end
                    
                    # Temperature White
                    var temp_white_raw = (payload[2] << 8) | payload[3]
                    if temp_white_raw != 0x7FFF
                        if temp_white_raw > 32767
                            temp_white_raw = temp_white_raw - 65536
                        end
                        data['temp_white'] = temp_white_raw / 10.0
                    end
                    
                    # Temperature Red/White
                    var temp_red_raw = (payload[4] << 8) | payload[5]
                    if temp_red_raw != 0x7FFF
                        if temp_red_raw > 32767
                            temp_red_raw = temp_red_raw - 65536
                        end
                        data['temp_red_white'] = temp_red_raw / 10.0
                    end
                    
                    # Flags
                    var flags = payload[6]
                    data['no_ack_flag'] = (flags & 0x01) != 0
                    data['poll_flag'] = (flags & 0x02) != 0
                    data['pa8_level'] = ((flags & 0x80) != 0) ? "Low" : "High"
                    
                    # Unix timestamp
                    var timestamp = (payload[7] << 24) | (payload[8] << 16) | (payload[9] << 8) | payload[10]
                    data['timestamp'] = timestamp
                    data['datalog'] = true
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
            
            # Track alarm events
            if data.contains('alarm_flag') && data['alarm_flag']
                node_data['alarm_count'] = node_data.find('alarm_count', 0) + 1
                node_data['last_alarm'] = tasmota.rtc()['local']
            end
            
            # Initialize downlink commands
            if !global.contains("D2x_cmdInit") || !global.D2x_cmdInit
                self.register_downlink_commands()
                global.D2x_cmdInit = true
            end

            # Save back to global storage
            global.D2x_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"D2x: Decode error - {e}: {m}")
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
            var node_data = global.D2x_nodes.find(self.node, {})
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
            name = f"D2x-{self.node}"
        end
        var name_tooltip = "Dragino D2x Temperature Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Temperature readings
        if data_to_show.contains('temp_red_white')
            fmt.add_sensor("string", f"{data_to_show['temp_red_white']:.1f}°C", "Red/White Probe", "🌡️")
        end
        
        if data_to_show.contains('temp_white')
            fmt.add_sensor("string", f"{data_to_show['temp_white']:.1f}°C", "White Probe", "🌡️")
        end
        
        if data_to_show.contains('temp_black')
            fmt.add_sensor("string", f"{data_to_show['temp_black']:.1f}°C", "Black Probe", "⚫")
        end
        
        # Battery voltage
        if data_to_show.contains('battery_v')
            fmt.add_sensor("volt", data_to_show['battery_v'], "Battery", "🔋")
        end
        
        # Status indicators
        var has_status = false
        if data_to_show.contains('alarm_flag') && data_to_show['alarm_flag']
            fmt.next_line()
            fmt.add_status("Alarm", "⚠️", "Temperature alarm triggered")
            has_status = true
        end
        
        if data_to_show.contains('datalog') && data_to_show['datalog']
            if !has_status
                fmt.next_line()
                has_status = true
            end
            fmt.add_status("Datalog", "📊", "Historical data")
        end
        
        if data_to_show.contains('pa8_level')
            if !has_status
                fmt.next_line()
                has_status = true
            end
            var pa8_icon = data_to_show['pa8_level'] == "Low" ? "🔻" : "🔺"
            fmt.add_sensor("string", data_to_show['pa8_level'], "PA8 Level", pa8_icon)
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
        var node_data = global.D2x_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'alarm_count': node_data.find('alarm_count', 0),
            'last_alarm': node_data.find('last_alarm', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.D2x_nodes.contains(node_id)
            global.D2x_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set transmit interval
        tasmota.remove_cmd("LwD2xSetInterval")
        tasmota.add_cmd("LwD2xSetInterval", def(cmd, idx, payload_str)
            # Format: LwD2xSetInterval<slot> <seconds>
            var interval = int(payload_str)
            if interval < 30 || interval > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 30-16777215 seconds")
            end
            
            # Build hex command: 01 + 3 bytes
            var hex_cmd = f"01{(interval >> 16) & 0xFF:02X}{(interval >> 8) & 0xFF:02X}{interval & 0xFF:02X}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Get device status
        tasmota.remove_cmd("LwD2xGetStatus")
        tasmota.add_cmd("LwD2xGetStatus", def(cmd, idx, payload_str)
            # Format: LwD2xGetStatus<slot>
            var hex_cmd = "2601"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set alarm threshold for all probes
        tasmota.remove_cmd("LwD2xSetAlarmAll")
        tasmota.add_cmd("LwD2xSetAlarmAll", def(cmd, idx, payload_str)
            # Format: LwD2xSetAlarmAll<slot> <min_temp>,<max_temp>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwD2xSetAlarmAll<slot> <min_temp>,<max_temp>")
            end
            
            var min_temp = int(parts[0])
            var max_temp = int(parts[1])
            
            if min_temp < -128 || min_temp > 127 || max_temp < -128 || max_temp > 127
                return tasmota.resp_cmnd_str("Invalid: range -128 to 127°C")
            end
            
            # Convert to signed bytes
            if min_temp < 0 min_temp = 256 + min_temp end
            if max_temp < 0 max_temp = 256 + max_temp end
            
            var hex_cmd = f"0B{min_temp:02X}{max_temp:02X}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set alarm threshold for single probe
        tasmota.remove_cmd("LwD2xSetAlarmProbe")
        tasmota.add_cmd("LwD2xSetAlarmProbe", def(cmd, idx, payload_str)
            # Format: LwD2xSetAlarmProbe<slot> <min_temp>,<max_temp>,<probe>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwD2xSetAlarmProbe<slot> <min>,<max>,<probe>")
            end
            
            var min_temp = int(parts[0])
            var max_temp = int(parts[1])
            var probe = int(parts[2])
            
            if min_temp < -128 || min_temp > 127 || max_temp < -128 || max_temp > 127
                return tasmota.resp_cmnd_str("Invalid temp: range -128 to 127°C")
            end
            
            if probe < 1 || probe > 3
                return tasmota.resp_cmnd_str("Invalid probe: 1=Red, 2=White, 3=Black")
            end
            
            # Convert to signed bytes
            if min_temp < 0 min_temp = 256 + min_temp end
            if max_temp < 0 max_temp = 256 + max_temp end
            
            var hex_cmd = f"0B{min_temp:02X}{max_temp:02X}{probe:02X}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set alarm interval
        tasmota.remove_cmd("LwD2xSetAlarmInterval")
        tasmota.add_cmd("LwD2xSetAlarmInterval", def(cmd, idx, payload_str)
            # Format: LwD2xSetAlarmInterval<slot> <minutes>
            var interval = int(payload_str)
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            
            var hex_cmd = f"0D{lwdecode.uint16be(interval)}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Get alarm settings
        tasmota.remove_cmd("LwD2xGetAlarm")
        tasmota.add_cmd("LwD2xGetAlarm", def(cmd, idx, payload_str)
            # Format: LwD2xGetAlarm<slot>
            var hex_cmd = "0E01"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set interrupt mode
        tasmota.remove_cmd("LwD2xSetInterrupt")
        tasmota.add_cmd("LwD2xSetInterrupt", def(cmd, idx, payload_str)
            # Format: LwD2xSetInterrupt<slot> <mode>
            return lwdecode.SendDownlinkMap(global.D2x_nodes, cmd, idx, payload_str, {
                '0|DISABLED': ['06000000', 'Disabled'],
                '1|FALLING':  ['06000001', 'Falling Edge'],
                '2|RISING':   ['06000002', 'Rising Edge'],
                '3|BOTH':     ['06000003', 'Both Edges']
            })
        end)
        
        # Set power output duration
        tasmota.remove_cmd("LwD2xSetPowerOut")
        tasmota.add_cmd("LwD2xSetPowerOut", def(cmd, idx, payload_str)
            # Format: LwD2xSetPowerOut<slot> <milliseconds>
            var duration = int(payload_str)
            if duration < 0 || duration > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535 ms")
            end
            
            var hex_cmd = f"07{lwdecode.uint16be(duration)}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        print("D2x: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_D2x()

# Node management commands
tasmota.remove_cmd("LwD2xNodeStats")
tasmota.add_cmd("LwD2xNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwD2xClearNode")
tasmota.add_cmd("LwD2xClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwD2xTestUI<slot> <scenario>
tasmota.remove_cmd("LwD2xTestUI")
tasmota.add_cmd("LwD2xTestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "normal":    "0CE40064007F00007FFF7FFF",      # Normal: 3.3V, 10.0°C, no alarms
        "multi":     "0CE40064007F0000012C01F4",      # Multi-probe: 10.0°C, 30.0°C, 50.0°C
        "alarm":     "0CE40190007F0100012C01F4",      # Alarm: 40.0°C with alarm flag
        "low":       "0BB80064007F00007FFF7FFF",      # Low battery: 3.0V
        "cold":      "0CE4FF9C007F00007FFF7FFF",      # Cold: -10.0°C
        "status":    "19010001000CE4",                # Device status: v1.0, EU868, 3.3V
        "datalog":   "01F4012C0064010A12345678"       # Datalog: temps + timestamp
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
    var fport = 2
    
    # Adjust fport for specific scenarios
    if payload_str == "status"
        fport = 5
    elif payload_str == "datalog"
        fport = 3
    end

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
