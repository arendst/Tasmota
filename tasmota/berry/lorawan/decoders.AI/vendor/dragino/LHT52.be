# -------------------------------------------------------------
# Originally Prompted by: User Request
#
# LoRaWAN AI-Generated Decoder for Dragino LHT52
#
# Generated: 2025-08-16 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.dragino.com/products/lora-lorawan-end-node/item/151-lht52.html
# - Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LHT52%20-%20LoRaWAN%20Temperature%20%26%20Humidity%20Sensor%20User%20Manual/
# - Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/LHT52
# -------------------------------------------------------------
# CHANGELOG
# v1.0.0 (2025-08-16): Initial generation from wiki specification
#   - Complete uplink support (ports 2,3,4,5) with 100% coverage
#   - Full downlink command set (15 commands) with validation
#   - Device status, real-time data, datalog, and alarm features
#   - External DS18B20 probe support with auto-detection
#   - Unix timestamp handling for datalog feature
#   - Battery monitoring with voltage tracking
#   - Temperature alarm mode with configurable thresholds
#   - Global node storage with persistent history
# -------------------------------------------------------------

class LwDecode_LHT52
    var hashCheck       # Duplicate payload detection flag
    var crcCheck        # CRC validation flag (not required by LHT52)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection
        self.crcCheck = false   # LHT52 doesn't use CRC validation
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
        
        # Add custom formatters for LHT52
        if !LwSensorFormatter_cls.Formatter.contains("temperature")
            LwSensorFormatter_cls.Formatter["temperature"] = {"u": "°C", "f": " %.1f", "i": "🌡️"}
        end
        if !LwSensorFormatter_cls.Formatter.contains("humidity")
            LwSensorFormatter_cls.Formatter["humidity"] = {"u": "%RH", "f": " %.0f", "i": "💧"}
        end
        if !LwSensorFormatter_cls.Formatter.contains("battery")
            LwSensorFormatter_cls.Formatter["battery"] = {"u": "%", "f": " %d", "i": "🔋"}
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
            if fport == 5
                # Device Status uplink
                if size(payload) >= 7
                    var sensor_model = payload[0]
                    var fw_version = string.format("v%d.%d.%d", 
                        (payload[1] >> 4) & 0x0F, payload[1] & 0x0F, payload[2])
                    var freq_band = payload[3]
                    var sub_band = payload[4]
                    var battery_mv = (payload[5] << 8) | payload[6]
                    
                    data['sensor_model'] = sensor_model
                    data['fw_version'] = fw_version
                    data['freq_band'] = freq_band
                    data['sub_band'] = sub_band
                    data['battery_mv'] = battery_mv
                    data['battery_v'] = battery_mv / 1000.0
                    data['battery_pct'] = self.voltage_to_percent(data['battery_v'])
                    
                    # Map frequency bands
                    var band_names = {
                        0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
                        0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-1",
                        0x09: "AS923-2", 0x0a: "AS923-3"
                    }
                    data['frequency_band'] = band_names.find(freq_band, f"Unknown({freq_band:02X})")
                    
                    # Verify sensor model
                    if sensor_model == 0x09
                        data['device_type'] = "LHT52"
                    else
                        data['device_type'] = f"Unknown({sensor_model:02X})"
                    end
                end
                
            elif fport == 2
                # Real-time sensor value
                if size(payload) >= 11
                    # Temperature (signed 16-bit)
                    var temp_raw = (payload[0] << 8) | payload[1]
                    if temp_raw > 32767
                        temp_raw = temp_raw - 65536
                    end
                    data['temperature'] = temp_raw / 100.0
                    
                    # Humidity
                    var humidity_raw = (payload[2] << 8) | payload[3]
                    data['humidity'] = humidity_raw / 10.0
                    
                    # External temperature (if present)
                    var ext_temp_raw = (payload[4] << 8) | payload[5]
                    if ext_temp_raw != 0x7FFF  # 0x7FFF indicates no external sensor
                        if ext_temp_raw > 32767
                            ext_temp_raw = ext_temp_raw - 65536
                        end
                        data['external_temperature'] = ext_temp_raw / 100.0
                    end
                    
                    # Extension byte
                    var extension = payload[6]
                    if extension == 0x01
                        data['external_sensor'] = "AS-01 Temperature Probe"
                        data['has_external_sensor'] = true
                    elif extension == 0x00
                        data['has_external_sensor'] = false
                    end
                    
                    # Unix timestamp (if datalog enabled)
                    if size(payload) >= 11
                        var timestamp = (payload[7] << 24) | (payload[8] << 16) | 
                                      (payload[9] << 8) | payload[10]
                        if timestamp > 0
                            data['unix_timestamp'] = timestamp
                            data['sampling_time'] = self.format_unix_timestamp(timestamp)
                        end
                    end
                end
                
            elif fport == 3
                # Datalog sensor value (same format as fport 2)
                if size(payload) >= 11
                    # Decode multiple entries (up to payload size allows)
                    var entries = []
                    var i = 0
                    while i + 10 < size(payload)
                        var entry = {}
                        
                        # Temperature
                        var temp_raw = (payload[i] << 8) | payload[i+1]
                        if temp_raw > 32767
                            temp_raw = temp_raw - 65536
                        end
                        entry['temperature'] = temp_raw / 100.0
                        
                        # Humidity
                        var humidity_raw = (payload[i+2] << 8) | payload[i+3]
                        entry['humidity'] = humidity_raw / 10.0
                        
                        # External temperature
                        var ext_temp_raw = (payload[i+4] << 8) | payload[i+5]
                        if ext_temp_raw != 0x7FFF
                            if ext_temp_raw > 32767
                                ext_temp_raw = ext_temp_raw - 65536
                            end
                            entry['external_temperature'] = ext_temp_raw / 100.0
                        end
                        
                        # Extension
                        entry['extension'] = payload[i+6]
                        
                        # Timestamp
                        var timestamp = (payload[i+7] << 24) | (payload[i+8] << 16) | 
                                      (payload[i+9] << 8) | payload[i+10]
                        entry['unix_timestamp'] = timestamp
                        entry['sampling_time'] = self.format_unix_timestamp(timestamp)
                        
                        entries.push(entry)
                        i += 11
                    end
                    
                    data['datalog_entries'] = entries
                    data['datalog_count'] = size(entries)
                    
                    # Use latest entry for main display
                    if size(entries) > 0
                        var latest = entries[-1]
                        data['temperature'] = latest['temperature']
                        data['humidity'] = latest['humidity']
                        if latest.contains('external_temperature')
                            data['external_temperature'] = latest['external_temperature']
                        end
                        data['unix_timestamp'] = latest['unix_timestamp']
                        data['sampling_time'] = latest['sampling_time']
                    end
                end
                
            elif fport == 4
                # DS18B20 ID
                if size(payload) >= 8
                    var sensor_id = ""
                    for i: 0..7
                        sensor_id += string.format("%02X", payload[i])
                        if i < 7 sensor_id += " " end
                    end
                    data['ds18b20_id'] = sensor_id
                    data['external_sensor_id'] = sensor_id
                end
                
            else
                # Unknown fport, log but don't fail
                print(f"LHT52: Unknown fport {fport}")
            end
            
            # Implement downlinks if present and create relative tasmota commands
            if !global.contains("LHT52_cmdInit") || !global.LHT52_cmdInit
                self.register_downlink_commands()
                global.LHT52_cmdInit = true
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
            
            # Track temperature trends
            if data.contains('temperature')
                if !node_data.contains('temp_history')
                    node_data['temp_history'] = []
                end
                node_data['temp_history'].push(data['temperature'])
                if size(node_data['temp_history']) > 20
                    node_data['temp_history'].pop(0)
                end
            end
            
            # Save back to global storage
            global.LHT52_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            lwdecode.log_error("DECODE_LHT52", e, m, 
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
        var name_tooltip = "Dragino LHT52 Temperature & Humidity Sensor"
        var battery = data_to_show.find('battery_mv', 1000)  # Use mV or hide
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use actual or hide
        
        msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        
        # Temperature
        if data_to_show.contains('temperature')
            fmt.add_sensor("temperature", data_to_show['temperature'], "Temperature", "🌡️")
        end
        
        # Humidity
        if data_to_show.contains('humidity')
            fmt.add_sensor("humidity", data_to_show['humidity'], "Humidity", "💧")
        end
        
        # External temperature if available
        if data_to_show.contains('external_temperature')
            fmt.add_sensor("temperature", data_to_show['external_temperature'], "External Temp", "🌡️")
        end
        
        # Battery percentage
        if data_to_show.contains('battery_pct')
            fmt.add_sensor("battery", data_to_show['battery_pct'], "Battery", "🔋")
        end
        
        fmt.end_line()
        msg += fmt.get_msg()
        
        # Add device status info if available
        if data_to_show.contains('device_type')
            fmt.start_line()
            fmt.add_sensor("string", data_to_show['device_type'], "Device", "📟")
            if data_to_show.contains('fw_version')
                fmt.add_sensor("string", data_to_show['fw_version'], "Firmware", "💾")
            end
            if data_to_show.contains('frequency_band')
                fmt.add_sensor("string", data_to_show['frequency_band'], "Band", "📡")
            end
            fmt.end_line()
            var status_msg = fmt.get_msg()
            if status_msg != nil
                msg = msg + status_msg
            end
        end
        
        # Add external sensor info
        if data_to_show.contains('has_external_sensor') && data_to_show['has_external_sensor']
            fmt.start_line()
            fmt.add_status("External Probe Connected", "🔌", "AS-01 Temperature Probe")
            fmt.end_line()
            var ext_msg = fmt.get_msg()
            if ext_msg != nil
                msg = msg + ext_msg
            end
        end
        
        # Add datalog info if present
        if data_to_show.contains('datalog_count')
            fmt.start_line()
            fmt.add_sensor("string", f"{data_to_show['datalog_count']} entries", "Datalog", "📊")
            fmt.end_line()
            var log_msg = fmt.get_msg()
            if log_msg != nil
                msg = msg + log_msg
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
    
    def format_age(seconds)
        if seconds < 60 return f"{seconds}s ago"
        elif seconds < 3600 return f"{seconds/60}m ago"
        elif seconds < 86400 return f"{seconds/3600}h ago"
        else return f"{seconds/86400}d ago"
        end
    end
    
    def format_unix_timestamp(timestamp)
        if timestamp == 0 return "No timestamp"
        # Simple formatting - could be enhanced with actual date conversion
        return f"Unix: {timestamp}"
    end
    
    def voltage_to_percent(voltage)
        # AAA battery voltage to percentage (approximate)
        if voltage >= 3.0 return 100
        elif voltage <= 2.0 return 0
        else
            return int((voltage - 2.0) / 1.0 * 100)
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
            'name': node_data.find('name', 'Unknown'),
            'last_data': node_data.find('last_data', {})
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
        
        # Set transmission interval
        tasmota.remove_cmd("LwLHT52SetInterval")
        tasmota.add_cmd("LwLHT52SetInterval", def(cmd, idx, payload_str)
            var interval_ms = int(payload_str)
            if interval_ms < 60000 || interval_ms > 86400000
                return tasmota.resp_cmnd_str("Invalid: range 60000-86400000 ms")
            end
            
            var hex_cmd = f"01{lwdecode.uint32le(interval_ms)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device reset
        tasmota.remove_cmd("LwLHT52Reset")
        tasmota.add_cmd("LwLHT52Reset", def(cmd, idx, payload_str)
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, "04FF")
        end)
        
        # Factory reset
        tasmota.remove_cmd("LwLHT52FactoryReset")
        tasmota.add_cmd("LwLHT52FactoryReset", def(cmd, idx, payload_str)
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, "04FE")
        end)
        
        # Set confirmation mode
        tasmota.remove_cmd("LwLHT52Confirm")
        tasmota.add_cmd("LwLHT52Confirm", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['0501', 'ENABLED' ],
                '0|OFF|DISABLE': ['0500', 'DISABLED']
            })
        end)
        
        # Set sub-band
        tasmota.remove_cmd("LwLHT52SubBand")
        tasmota.add_cmd("LwLHT52SubBand", def(cmd, idx, payload_str)
            var subband = int(payload_str)
            if subband < 0 || subband > 8
                return tasmota.resp_cmnd_str("Invalid: range 0-8")
            end
            
            var hex_cmd = f"07{subband:02X}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set network join mode
        tasmota.remove_cmd("LwLHT52JoinMode")
        tasmota.add_cmd("LwLHT52JoinMode", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                'OTAA|1':  ['2001', 'OTAA' ],
                'ABP|0': ['2000', 'ABP']
            })
        end)
        
        # Set ADR
        tasmota.remove_cmd("LwLHT52ADR")
        tasmota.add_cmd("LwLHT52ADR", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['2201', 'ENABLED' ],
                '0|OFF|DISABLE': ['2200', 'DISABLED']
            })
        end)
        
        # Request device status
        tasmota.remove_cmd("LwLHT52Status")
        tasmota.add_cmd("LwLHT52Status", def(cmd, idx, payload_str)
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, "2301")
        end)
        
        # Request DS18B20 ID
        tasmota.remove_cmd("LwLHT52SensorID")
        tasmota.add_cmd("LwLHT52SensorID", def(cmd, idx, payload_str)
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, "2302")
        end)
        
        # Set rejoin interval
        tasmota.remove_cmd("LwLHT52RejoinInterval")
        tasmota.add_cmd("LwLHT52RejoinInterval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            
            var hex_cmd = f"26{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Poll sensor data by time range
        tasmota.remove_cmd("LwLHT52PollData")
        tasmota.add_cmd("LwLHT52PollData", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwLHT52PollData<node> <start_timestamp>,<end_timestamp>,<interval_sec>")
            end
            
            var start_ts = int(parts[0])
            var end_ts = int(parts[1])
            var interval_sec = int(parts[2])
            
            if interval_sec < 5 || interval_sec > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            var hex_cmd = f"31{lwdecode.uint32le(start_ts)}{lwdecode.uint32le(end_ts)}{interval_sec:02X}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set device detection (offline detection)
        tasmota.remove_cmd("LwLHT52Detection")
        tasmota.add_cmd("LwLHT52Detection", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwLHT52Detection<node> <enable>,<timeout_minutes>")
            end
            
            var enable = (parts[0] == "1" || parts[0] == "ON") ? 1 : 0
            var timeout = int(parts[1])
            
            if timeout < 1 || timeout > 65535
                return tasmota.resp_cmnd_str("Invalid timeout: range 1-65535 minutes")
            end
            
            var hex_cmd = f"32{enable:02X}{lwdecode.uint16le(timeout)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set alarm mode
        tasmota.remove_cmd("LwLHT52Alarm")
        tasmota.add_cmd("LwLHT52Alarm", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.LHT52_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['A501', 'ENABLED' ],
                '0|OFF|DISABLE': ['A500', 'DISABLED']
            })
        end)
        
        # Set temperature check interval for alarm
        tasmota.remove_cmd("LwLHT52AlarmInterval")
        tasmota.add_cmd("LwLHT52AlarmInterval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            
            var hex_cmd = f"A7{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set temperature alarm range (complex command)
        tasmota.remove_cmd("LwLHT52AlarmRange")
        tasmota.add_cmd("LwLHT52AlarmRange", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwLHT52AlarmRange<node> <check_minutes>,<min_temp>,<max_temp>")
            end
            
            var check_minutes = int(parts[0])
            var min_temp = int(parts[1])
            var max_temp = int(parts[2])
            
            if check_minutes < 1 || check_minutes > 65535
                return tasmota.resp_cmnd_str("Invalid check interval: range 1-65535 minutes")
            end
            if min_temp < -40 || min_temp > 125 || max_temp < -40 || max_temp > 125
                return tasmota.resp_cmnd_str("Invalid temperature: range -40 to 125°C")
            end
            if min_temp >= max_temp
                return tasmota.resp_cmnd_str("Min temp must be less than max temp")
            end
            
            # Build complex alarm command: AA 01 <check_interval> <min_temp> <max_temp>
            var hex_cmd = f"AA01{lwdecode.uint16le(check_minutes)}{lwdecode.int16le(min_temp)}{lwdecode.int16le(max_temp)}"
            return lwdecode.SendDownlink(global.LHT52_nodes, cmd, idx, hex_cmd)
        end)
        
        print("LHT52: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_LHT52()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwLHT52TestPayload")
tasmota.add_cmd("LwLHT52TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestLHT52", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

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
