# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Dragino D2x Series
#
# Generated: 2025-08-16 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.9
#
# Official Links
# - Homepage:  https://www.dragino.com/products/temperature-humidity-sensor/
# - Userguide: http://wiki.dragino.com/xwiki/bin/view/Main/User Manual for LoRaWAN End Nodes/D20-LBD22-LBD23-LB_LoRaWAN_Temperature_Sensor_User_Manual/
# - Decoder:   https://github.com/dragino/dragino-end-node-decoder/
# -------------------------------------------------------------
# v1.0.0 (2025-08-16): Initial generation from manual specification
#                      - Complete D20/D20S/D22/D23-LB/LS support
#                      - Multi-probe temperature monitoring (1-3 probes)
#                      - Datalog and alarm features
#                      - Device status and configuration
#                      - Enhanced error handling with lwdecode framework
# -------------------------------------------------------------

class LwDecode_D2x
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # No CRC validation required per specs
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
        
        # Standard formatters for D2x (emojis defined in formatters)
        LwSensorFormatter_cls.Formatter["temperature"] = {"u": "°C", "f": " %.1f", "i": "🌡️"}
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
                # Device Status (FPORT=5)
                if size(payload) >= 6
                    data['sensor_model'] = payload[0]
                    data['fw_version'] = f"{payload[1]}.{payload[2]}"
                    data['freq_band'] = self.get_frequency_band(payload[3])
                    data['sub_band'] = payload[4]
                    var battery_raw = (payload[5] << 8) | payload[6] if size(payload) > 5 else (payload[5] << 8)
                    data['battery_mv'] = battery_raw
                    data['battery_v'] = battery_raw / 1000.0
                end
                
            elif fport == 2
                # Sensor Data (FPORT=2)
                if size(payload) >= 11
                    # Battery (bytes 0-1)
                    var battery_raw = (payload[1] << 8) | payload[0]
                    data['battery_mv'] = battery_raw
                    data['battery_v'] = battery_raw / 1000.0
                    
                    # Temperature Red/White (bytes 2-3)
                    var temp1_raw = (payload[3] << 8) | payload[2]
                    data['temp_red_white'] = self.convert_temperature(temp1_raw)
                    
                    # Skip ignore field (bytes 4-5)
                    
                    # Alarm Flag & MOD & PA8 Level (byte 6)
                    var flags = payload[6]
                    data['alarm_flag'] = (flags & 0x01) != 0
                    data['pa8_level'] = (flags & 0x80) != 0 ? "LOW" : "HIGH"
                    data['mod'] = (flags >> 2) & 0x1F
                    data['message_type'] = data['mod'] == 0 ? "SAMPLING" : (data['mod'] == 49 ? "POLL_REPLY" : "OTHER")
                    
                    # Temperature White (bytes 7-8)
                    if size(payload) >= 9
                        var temp2_raw = (payload[8] << 8) | payload[7]
                        data['temp_white'] = self.convert_temperature(temp2_raw)
                    end
                    
                    # Temperature Black (bytes 9-10)
                    if size(payload) >= 11
                        var temp3_raw = (payload[10] << 8) | payload[9]
                        data['temp_black'] = self.convert_temperature(temp3_raw)
                    end
                    
                    # Determine model based on valid temperatures
                    data['model'] = self.determine_model(data)
                end
                
            elif fport == 3
                # Datalog (FPORT=3) - Historical data
                var entries = []
                var i = 0
                while i + 10 < size(payload)
                    var entry = {}
                    
                    # Temperature Black (bytes 0-1)
                    var temp_black_raw = (payload[i+1] << 8) | payload[i]
                    entry['temp_black'] = self.convert_temperature(temp_black_raw)
                    
                    # Temperature White (bytes 2-3)
                    var temp_white_raw = (payload[i+3] << 8) | payload[i+2]
                    entry['temp_white'] = self.convert_temperature(temp_white_raw)
                    
                    # Temperature Red/White (bytes 4-5)
                    var temp_red_white_raw = (payload[i+5] << 8) | payload[i+4]
                    entry['temp_red_white'] = self.convert_temperature(temp_red_white_raw)
                    
                    # Poll message flag & Alarm Flag & PA8 Level (byte 6)
                    var flags = payload[i+6]
                    entry['no_ack'] = (flags & 0x01) != 0
                    entry['poll_reply'] = (flags & 0x02) != 0
                    entry['pa8_level'] = (flags & 0x80) != 0 ? "LOW" : "HIGH"
                    
                    # Unix Timestamp (bytes 7-10)
                    var timestamp = (payload[i+10] << 24) | (payload[i+9] << 16) | 
                                   (payload[i+8] << 8) | payload[i+7]
                    entry['timestamp'] = timestamp
                    entry['datetime'] = self.format_unix_time(timestamp)
                    
                    entries.push(entry)
                    i += 11
                end
                
                data['datalog_entries'] = entries
                data['datalog_count'] = size(entries)
                data['is_datalog'] = true
                
            else
                print(f"Unknown fport: {fport}")
                return nil
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store temperature trends if available
            var temp_values = []
            if data.contains('temp_red_white') && data['temp_red_white'] != 327.67
                temp_values.push(data['temp_red_white'])
            end
            if data.contains('temp_white') && data['temp_white'] != 327.67
                temp_values.push(data['temp_white'])
            end
            if data.contains('temp_black') && data['temp_black'] != 327.67
                temp_values.push(data['temp_black'])
            end
            
            if size(temp_values) > 0
                if !node_data.contains('temperature_history')
                    node_data['temperature_history'] = []
                end
                node_data['temperature_history'].push({
                    'timestamp': tasmota.rtc()['local'],
                    'temperatures': temp_values
                })
                if size(node_data['temperature_history']) > 10
                    node_data['temperature_history'].pop(0)
                end
            end
            
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
            
            # Track alarm events
            if data.contains('alarm_flag') && data['alarm_flag']
                node_data['alarm_count'] = node_data.find('alarm_count', 0) + 1
                node_data['last_alarm'] = tasmota.rtc()['local']
            end
            
            # Register downlink commands if not already done
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
            lwdecode.log_error("DECODE_D2x", e, m, 
                format("Device:%s, Node:%s, FPort:%d, PayloadSize:%d", name, node, fport, size(payload)))
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
            var model = data_to_show.find('model', 'D2x')
            name = f"{model}-{self.node}"
        end
        var name_tooltip = f"Dragino {data_to_show.find('model', 'D2x')} Temperature Sensor"
        var battery = data_to_show.find('battery_v', 1000)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        
        msg = msg + lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Handle different message types
        if data_to_show.contains('is_datalog')
            # Datalog display
            fmt.start_line()
            fmt.add_sensor("string", f"{data_to_show.find('datalog_count', 0)} entries", "Datalog", "💾")
            var latest_entry = data_to_show.find('datalog_entries', [])
            if size(latest_entry) > 0
                var entry = latest_entry[0]
                if entry.contains('temp_red_white') && entry['temp_red_white'] != 327.67
                    fmt.add_sensor("temperature", entry['temp_red_white'], "Temp", nil)
                end
            end
            fmt.end_line()
            
        else
            # Regular sensor display
            fmt.start_line()
            
            # Show available temperature probes
            var temp_count = 0
            if data_to_show.contains('temp_red_white') && data_to_show['temp_red_white'] != 327.67
                fmt.add_sensor("temperature", data_to_show['temp_red_white'], "T1", nil)
                temp_count += 1
            end
            if data_to_show.contains('temp_white') && data_to_show['temp_white'] != 327.67
                fmt.add_sensor("temperature", data_to_show['temp_white'], "T2", nil)
                temp_count += 1
            end
            if data_to_show.contains('temp_black') && data_to_show['temp_black'] != 327.67
                fmt.add_sensor("temperature", data_to_show['temp_black'], "T3", nil)
                temp_count += 1
            end
            
            # Show device model if determined
            if data_to_show.contains('model')
                fmt.add_sensor("string", data_to_show['model'], "Model", "📟")
            end
            
            fmt.end_line()
        end
        
        var sensor_msg = fmt.get_msg()
        if sensor_msg != nil
            msg = msg + sensor_msg
        end
        
        # Add alert line for alarms or special states
        var has_alerts = data_to_show.find('alarm_flag', false)
        if has_alerts
            fmt.start_line()
            fmt.add_status("ALARM", "🚨", "Temperature Alarm")
            var msg_type = data_to_show.find('message_type', 'UNKNOWN')
            if msg_type != "SAMPLING"
                fmt.add_sensor("string", msg_type, "Type", "📡")
            end
            fmt.end_line()
            var alert_msg = fmt.get_msg()
            if alert_msg != nil
                msg = msg + alert_msg
            end
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.start_line()
                fmt.add_status(self.format_age(age), "⏱️", nil)
                fmt.end_line()
                var age_msg = fmt.get_msg()
                if age_msg != nil
                    msg = msg + age_msg
                end
            end
        end
        
        return msg
    end
    
    def convert_temperature(raw_value)
        # Handle invalid temperature marker
        if raw_value == 0x7FFF
            return 327.67  # Invalid/not present marker
        end
        
        # Handle signed 16-bit temperature
        var temp = raw_value
        if temp > 32767
            temp = temp - 65536
        end
        return temp / 10.0
    end
    
    def determine_model(data)
        var valid_temps = 0
        if data.contains('temp_red_white') && data['temp_red_white'] != 327.67
            valid_temps += 1
        end
        if data.contains('temp_white') && data['temp_white'] != 327.67
            valid_temps += 1
        end
        if data.contains('temp_black') && data['temp_black'] != 327.67
            valid_temps += 1
        end
        
        if valid_temps == 1
            return "D20"
        elif valid_temps == 2
            return "D22"
        elif valid_temps == 3
            return "D23"
        else
            return "D2x"
        end
    end
    
    def get_frequency_band(band_code)
        var bands = {
            0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
            0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-1",
            0x09: "AS923-2", 0x0a: "AS923-3", 0x0b: "CN470", 0x0c: "EU433",
            0x0d: "KR920", 0x0e: "MA869"
        }
        return bands.find(band_code, f"UNKNOWN({band_code:02X})")
    end
    
    def format_unix_time(timestamp)
        # Simple timestamp formatting (basic implementation)
        return f"TS:{timestamp}"
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
            'temperature_history': node_data.find('temperature_history', []),
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
        
        # Set Transmit Interval
        tasmota.remove_cmd("LwD2xInterval")
        tasmota.add_cmd("LwD2xInterval", def(cmd, idx, payload_str)
            var seconds = int(payload_str)
            if seconds < 30 || seconds > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 30-16777215 seconds")
            end
            var hex_cmd = f"01{lwdecode.uint24le(seconds)}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Get Device Status
        tasmota.remove_cmd("LwD2xStatus")
        tasmota.add_cmd("LwD2xStatus", def(cmd, idx, payload_str)
            var hex_cmd = "2601"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Alarm Threshold (All Probes)
        tasmota.remove_cmd("LwD2xAlarmAll")
        tasmota.add_cmd("LwD2xAlarmAll", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwD2xAlarmAll<node> <min_temp>,<max_temp>")
            end
            
            var min_temp = int(parts[0])
            var max_temp = int(parts[1])
            if min_temp < -128 || min_temp > 127 || max_temp < -128 || max_temp > 127
                return tasmota.resp_cmnd_str("Invalid: temperature range -128 to 127°C")
            end
            
            # Convert to signed bytes
            var min_byte = min_temp < 0 ? (256 + min_temp) : min_temp
            var max_byte = max_temp < 0 ? (256 + max_temp) : max_temp
            var hex_cmd = f"0B{min_byte:02X}{max_byte:02X}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Alarm Threshold (Single Probe)
        tasmota.remove_cmd("LwD2xAlarmProbe")
        tasmota.add_cmd("LwD2xAlarmProbe", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwD2xAlarmProbe<node> <min>,<max>,<probe_idx>")
            end
            
            var min_temp = int(parts[0])
            var max_temp = int(parts[1])
            var probe_idx = int(parts[2])
            
            if min_temp < -128 || min_temp > 127 || max_temp < -128 || max_temp > 127
                return tasmota.resp_cmnd_str("Invalid: temperature range -128 to 127°C")
            end
            if probe_idx < 1 || probe_idx > 3
                return tasmota.resp_cmnd_str("Invalid: probe index 1-3 (1=Red, 2=White, 3=Black)")
            end
            
            var min_byte = min_temp < 0 ? (256 + min_temp) : min_temp
            var max_byte = max_temp < 0 ? (256 + max_temp) : max_temp
            var hex_cmd = f"0B{min_byte:02X}{max_byte:02X}{probe_idx:02X}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Alarm Interval
        tasmota.remove_cmd("LwD2xAlarmInterval")
        tasmota.add_cmd("LwD2xAlarmInterval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            var hex_cmd = f"0D{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Get Alarm Settings
        tasmota.remove_cmd("LwD2xGetAlarm")
        tasmota.add_cmd("LwD2xGetAlarm", def(cmd, idx, payload_str)
            var hex_cmd = "0E01"
            return lwdecode.SendDownlink(global.D2x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Interrupt Mode
        tasmota.remove_cmd("LwD2xIntMode")
        tasmota.add_cmd("LwD2xIntMode", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.D2x_nodes, cmd, idx, payload_str, { 
                '0|DISABLE': ['06000000', 'DISABLED'],
                '1|FALLING': ['06000001', 'FALLING_EDGE'],
                '2|RISING':  ['06000002', 'RISING_EDGE'],
                '3|BOTH':    ['06000003', 'BOTH_EDGES']
            })
        end)
        
        # Device Reboot (via activation)
        tasmota.remove_cmd("LwD2xReboot")
        tasmota.add_cmd("LwD2xReboot", def(cmd, idx, payload_str)
            # Note: Physical button press required for reboot
            return tasmota.resp_cmnd_str("Physical button press required for device reboot")
        end)
        
        print("D2x: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_D2x()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwD2xTestPayload")
tasmota.add_cmd("LwD2xTestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestD2x", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

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
