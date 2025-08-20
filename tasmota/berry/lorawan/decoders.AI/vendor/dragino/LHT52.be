#
# LoRaWAN AI-Generated Decoder for Dragino LHT52 Prompted by ZioFabry
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.0
#
# Homepage:  https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/
# Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/
# Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/LHT52
# 
# v1.0.0 (2025-08-20): Initial generation from cached MAP specification

class LwDecode_LHT52
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
        if !global.contains("LHT52_nodes")
            global.LHT52_nodes = {}
        end
        if !global.contains("LHT52_cmdInit")
            global.LHT52_cmdInit = false
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
            var node_data = global.LHT52_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 5  # Device Status
                if size(payload) >= 7
                    data['sensor_model'] = payload[0]
                    data['fw_version'] = f"v{payload[1]}.{payload[2]}"
                    data['frequency_band'] = payload[3]
                    data['subband'] = payload[4]
                    data['battery_mv'] = (payload[6] << 8) | payload[5]
                    data['battery_v'] = data['battery_mv'] / 1000.0
                end
                
            elif fport == 2  # Real-time Sensor Value
                if size(payload) >= 7
                    # Temperature (signed 16-bit)
                    var temp_raw = (payload[1] << 8) | payload[0]
                    if temp_raw > 32767
                        temp_raw = temp_raw - 65536
                    end
                    data['temperature'] = temp_raw / 100.0
                    
                    # Humidity
                    var hum_raw = (payload[3] << 8) | payload[2]
                    data['humidity'] = hum_raw / 10.0
                    
                    # External Temperature (if present)
                    if size(payload) >= 7
                        var ext_temp_raw = (payload[5] << 8) | payload[4]
                        if ext_temp_raw != 0x7FFF  # Not disconnected
                            if ext_temp_raw > 32767
                                ext_temp_raw = ext_temp_raw - 65536
                            end
                            data['ext_temperature'] = ext_temp_raw / 100.0
                        end
                        
                        if size(payload) >= 7
                            data['extension'] = payload[6]
                            data['has_external'] = data['extension'] == 0x01
                        end
                    end
                    
                    # Unix timestamp (if present)
                    if size(payload) >= 11
                        data['timestamp'] = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
                        data['timestamp_str'] = self.format_timestamp(data['timestamp'])
                    end
                end
                
            elif fport == 3  # Datalog Sensor Value
                if size(payload) >= 11
                    # Same format as real-time but from historical data
                    var temp_raw = (payload[1] << 8) | payload[0]
                    if temp_raw > 32767
                        temp_raw = temp_raw - 65536
                    end
                    data['temperature'] = temp_raw / 100.0
                    
                    var hum_raw = (payload[3] << 8) | payload[2]
                    data['humidity'] = hum_raw / 10.0
                    
                    var ext_temp_raw = (payload[5] << 8) | payload[4]
                    if ext_temp_raw != 0x7FFF
                        if ext_temp_raw > 32767
                            ext_temp_raw = ext_temp_raw - 65536
                        end
                        data['ext_temperature'] = ext_temp_raw / 100.0
                    end
                    
                    data['extension'] = payload[6]
                    data['timestamp'] = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
                    data['timestamp_str'] = self.format_timestamp(data['timestamp'])
                    data['is_datalog'] = true
                end
                
            elif fport == 4  # DS18B20 ID
                if size(payload) >= 8
                    var sensor_id = ""
                    for i: 0..7
                        sensor_id += string.format("%02X", payload[i])
                    end
                    data['ds18b20_id'] = sensor_id
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
            
            # Track temperature trends
            if data.contains('temperature')
                if !node_data.contains('temp_history')
                    node_data['temp_history'] = []
                end
                node_data['temp_history'].push(data['temperature'])
                if size(node_data['temp_history']) > 10
                    node_data['temp_history'].pop(0)
                end
            end
            
            # Initialize downlink commands
            if !global.contains("LHT52_cmdInit") || !global.LHT52_cmdInit
                self.register_downlink_commands()
                global.LHT52_cmdInit = true
            end

            # Save back to global storage
            global.LHT52_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"LHT52: Decode error - {e}: {m}")
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
            var node_data = global.LHT52_nodes.find(self.node, {})
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
            name = f"LHT52-{self.node}"
        end
        var name_tooltip = "Dragino LHT52"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Main sensor line
        if data_to_show.contains('temperature')
            fmt.add_sensor("string", f"{data_to_show['temperature']:.1f}°C", "Temperature", "🌡️")
        end
        
        if data_to_show.contains('humidity')
            fmt.add_sensor("string", f"{data_to_show['humidity']:.1f}%", "Humidity", "💧")
        end
        
        if data_to_show.contains('ext_temperature')
            fmt.add_sensor("string", f"{data_to_show['ext_temperature']:.1f}°C", "External Temp", "🔗")
        end
        
        # Device info line (if present)
        if data_to_show.contains('fw_version') || data_to_show.contains('frequency_band')
            fmt.next_line()
            if data_to_show.contains('fw_version')
                fmt.add_sensor("string", data_to_show['fw_version'], "Firmware", "💿")
            end
            if data_to_show.contains('frequency_band')
                var band_name = self.get_band_name(data_to_show['frequency_band'])
                fmt.add_sensor("string", band_name, "Band", "📡")
            end
            if data_to_show.contains('subband') && data_to_show['subband'] > 0
                fmt.add_sensor("string", f"SB{data_to_show['subband']}", "Sub-band", "📶")
            end
        end
        
        # Status line
        var has_status = false
        if data_to_show.contains('is_datalog') && data_to_show['is_datalog']
            if !data_to_show.contains('fw_version')
                fmt.next_line()
            end
            fmt.add_sensor("string", "Datalog", "Historical Data", "📊")
            has_status = true
        end
        
        if data_to_show.contains('ds18b20_id')
            if !has_status && !data_to_show.contains('fw_version')
                fmt.next_line()
            end
            fmt.add_sensor("string", "DS18B20", "External Sensor", "🔗")
            has_status = true
        end
        
        if data_to_show.contains('timestamp_str')
            if !has_status && !data_to_show.contains('fw_version')
                fmt.next_line()
            end
            fmt.add_sensor("string", data_to_show['timestamp_str'], "Logged At", "⏰")
            has_status = true
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                if !has_status && !data_to_show.contains('fw_version')
                    fmt.next_line()
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
    
    def format_timestamp(timestamp)
        if timestamp == 0 return "No timestamp" end
        # Simple timestamp formatting
        var date = tasmota.time_dump(timestamp)
        return f"{date['month']:02d}/{date['day']:02d} {date['hour']:02d}:{date['min']:02d}"
    end
    
    def get_band_name(band_code)
        if band_code == 1 return "EU868"
        elif band_code == 2 return "US915"
        elif band_code == 3 return "AS923"
        elif band_code == 4 return "AU915"
        elif band_code == 5 return "KR920"
        elif band_code == 6 return "IN865"
        elif band_code == 7 return "CN470"
        elif band_code == 8 return "EU433"
        else return f"Band{band_code}"
        end
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.LHT52_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'battery_history': node_data.find('battery_history', []),
            'temp_history': node_data.find('temp_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.LHT52_nodes.contains(node_id)
            global.LHT52_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set TDC Time command
        tasmota.remove_cmd("LwLHT52SetInterval")
        tasmota.add_cmd("LwLHT52SetInterval", def(cmd, idx, payload_str)
            # Format: LwLHT52SetInterval<slot> <milliseconds>
            var interval = int(payload_str)
            if interval < 1 || interval > 86400000
                return tasmota.resp_cmnd_str("Invalid: range 1-86400000 ms")
            end
            
            # Build hex command: 01 + 32-bit little endian interval
            var hex_cmd = f"01{lwdecode.uint32le(interval)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Reset Device command
        tasmota.remove_cmd("LwLHT52Reset")
        tasmota.add_cmd("LwLHT52Reset", def(cmd, idx, payload_str)
            # Format: LwLHT52Reset<slot>
            var hex_cmd = "04FF"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Factory Reset command
        tasmota.remove_cmd("LwLHT52Factory")
        tasmota.add_cmd("LwLHT52Factory", def(cmd, idx, payload_str)
            # Format: LwLHT52Factory<slot>
            var hex_cmd = "04FE"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Confirmation Mode command
        tasmota.remove_cmd("LwLHT52Confirm")
        tasmota.add_cmd("LwLHT52Confirm", def(cmd, idx, payload_str)
            # Format: LwLHT52Confirm<slot> <enable|disable|1|0>
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                '1|ENABLE':  ['0501', 'Confirmed Mode'],
                '0|DISABLE': ['0500', 'Unconfirmed Mode']
            })
        end)
        
        # Set Sub-band command
        tasmota.remove_cmd("LwLHT52Subband")
        tasmota.add_cmd("LwLHT52Subband", def(cmd, idx, payload_str)
            # Format: LwLHT52Subband<slot> <0-8>
            var subband = int(payload_str)
            if subband < 0 || subband > 8
                return tasmota.resp_cmnd_str("Invalid: range 0-8")
            end
            
            var hex_cmd = f"07{subband:02X}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set ADR command
        tasmota.remove_cmd("LwLHT52ADR")
        tasmota.add_cmd("LwLHT52ADR", def(cmd, idx, payload_str)
            # Format: LwLHT52ADR<slot> <enable|disable|1|0>
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                '1|ENABLE':  ['2201', 'ADR Enabled'],
                '0|DISABLE': ['2200', 'ADR Disabled']
            })
        end)
        
        # Request Device Status command
        tasmota.remove_cmd("LwLHT52Status")
        tasmota.add_cmd("LwLHT52Status", def(cmd, idx, payload_str)
            # Format: LwLHT52Status<slot>
            var hex_cmd = "2301"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request DS18B20 ID command
        tasmota.remove_cmd("LwLHT52SensorID")
        tasmota.add_cmd("LwLHT52SensorID", def(cmd, idx, payload_str)
            # Format: LwLHT52SensorID<slot>
            var hex_cmd = "2302"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Alarm Mode command
        tasmota.remove_cmd("LwLHT52Alarm")
        tasmota.add_cmd("LwLHT52Alarm", def(cmd, idx, payload_str)
            # Format: LwLHT52Alarm<slot> <enable|disable|1|0>
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                '1|ENABLE':  ['A501', 'Alarm Enabled'],
                '0|DISABLE': ['A500', 'Alarm Disabled']
            })
        end)
        
        # Set Temperature Check Interval command
        tasmota.remove_cmd("LwLHT52TempCheck")
        tasmota.add_cmd("LwLHT52TempCheck", def(cmd, idx, payload_str)
            # Format: LwLHT52TempCheck<slot> <minutes>
            var interval = int(payload_str)
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            
            var hex_cmd = f"A7{lwdecode.uint16be(interval)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Poll Sensor Data command
        tasmota.remove_cmd("LwLHT52Poll")
        tasmota.add_cmd("LwLHT52Poll", def(cmd, idx, payload_str)
            # Format: LwLHT52Poll<slot> <start_ts>,<end_ts>,<interval>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwLHT52Poll<slot> <start_ts>,<end_ts>,<interval>")
            end
            
            var start_ts = int(parts[0])
            var end_ts = int(parts[1])
            var interval = int(parts[2])
            
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            var hex_cmd = f"31{lwdecode.uint32le(start_ts)}{lwdecode.uint32le(end_ts)}{interval:02X}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        print("LHT52: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_LHT52()

# Node management commands
tasmota.remove_cmd("LwLHT52NodeStats")
tasmota.add_cmd("LwLHT52NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwLHT52ClearNode")
tasmota.add_cmd("LwLHT52ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwLHT52TestUI<slot> <scenario>
tasmota.remove_cmd("LwLHT52TestUI")
tasmota.add_cmd("LwLHT52TestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "device_info": "09010001003A0B",              # Device status: v1.0, EU868, 2874mV
        "normal":      "BC09600270000001",            # 25.0°C, 61.0%RH, no external
        "external":    "BC096002700A1001",            # With DS18B20 at 26.6°C
        "cold":        "E4F56002F0F801",              # -2.6°C, 61.0%RH, external -2.0°C
        "hot":         "D00F64027F1F01",              # 40.0°C, 61.2%RH, external 80.9°C
        "datalog":     "BC096002700A100160E26300",    # Historical data with timestamp
        "sensor_id":   "2801234567890ABC"             # DS18B20 sensor ID response
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
    var fport = 2  # Default to sensor data
    
    # Determine correct fport for scenario
    if payload_str == "device_info"
        fport = 5
    elif payload_str == "datalog"
        fport = 3
    elif payload_str == "sensor_id"
        fport = 4
    end

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
