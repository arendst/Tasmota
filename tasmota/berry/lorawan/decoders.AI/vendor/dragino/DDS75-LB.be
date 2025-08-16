# -------------------------------------------------------------
# Originally Prompted by: User Request
#
# LoRaWAN AI-Generated Decoder for Dragino DDS75-LB/LS
#
# Generated: 2025-08-16 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.dragino.com/products/lora-lorawan-end-device/item/154-dds75.html
# - Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/DDS75-LB_LoRaWAN_Distance_Detection_Sensor_User_Manual/
# - Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/
# -------------------------------------------------------------
# CHANGELOG
# v1.0.0 (2025-08-16): Initial generation from PDF specification
# -------------------------------------------------------------

class LwDecode_DDS75_LB
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # No CRC validation per PDF specs
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("DDS75_LB_nodes")
            global.DDS75_LB_nodes = {}
        end
        if !global.contains("DDS75_LB_cmdInit")
            global.DDS75_LB_cmdInit = false
        end
        
        # Standard formatters for DDS75-LB (emojis defined in formatters)
        LwSensorFormatter_cls.Formatter["distance"] = {"u": "mm", "f": " %d", "i": "📏"}
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
            var node_data = global.DDS75_LB_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 1
                # Periodic Data Uplink (8 bytes)
                if size(payload) >= 8
                    # Battery Voltage (bytes 0-1)
                    var battery_raw = (payload[1] << 8) | payload[0]
                    data['battery_v'] = battery_raw / 1000.0
                    
                    # Distance (bytes 2-3)
                    var distance_raw = (payload[3] << 8) | payload[2]
                    if distance_raw == 0x0000
                        data['distance_mm'] = 0
                        data['distance_status'] = "No sensor detected"
                        data['sensor_error'] = true
                    elif distance_raw == 0x0014
                        data['distance_mm'] = 20
                        data['distance_status'] = "Invalid reading"
                        data['sensor_error'] = true
                    else
                        data['distance_mm'] = distance_raw
                        data['distance_status'] = "Valid"
                        data['sensor_error'] = false
                    end
                    
                    # Convert to meters for display
                    data['distance_m'] = data['distance_mm'] / 1000.0
                    
                    # Digital Interrupt (byte 4)
                    var interrupt = payload[4]
                    if interrupt == 0x01
                        data['interrupt'] = true
                        data['trigger_type'] = "Interrupt"
                    else
                        data['interrupt'] = false
                        data['trigger_type'] = "Periodic"
                    end
                    
                    # DS18B20 Temperature (bytes 5-6) - Optional
                    if size(payload) >= 7
                        var temp_raw = (payload[6] << 8) | payload[5]
                        if (temp_raw & 0xFC00) != 0
                            # Negative temperature
                            data['temperature'] = (temp_raw - 65536) / 10.0
                        else
                            # Positive temperature
                            data['temperature'] = temp_raw / 10.0
                        end
                    end
                    
                    # Sensor Flag (byte 7)
                    if size(payload) >= 8
                        var sensor_flag = payload[7]
                        data['sensor_detected'] = sensor_flag == 0x01
                        if !data['sensor_detected']
                            data['sensor_error'] = true
                        end
                    end
                end
                
            elif fport == 5
                # Device Status Uplink (7 bytes)
                if size(payload) >= 7
                    # Sensor Model (byte 0)
                    var model = payload[0]
                    if model == 0x27
                        data['device_model'] = "DDS75-LB/LS"
                    else
                        data['device_model'] = f"Unknown-{model:02X}"
                    end
                    
                    # Firmware Version (bytes 1-2)
                    var fw_raw = (payload[2] << 8) | payload[1]
                    var fw_major = (fw_raw >> 8) & 0xFF
                    var fw_minor = fw_raw & 0xFF
                    data['firmware'] = f"v{fw_major}.{fw_minor}"
                    
                    # Frequency Band (byte 3)
                    var freq_band = payload[3]
                    var bands = {
                        0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
                        0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-2",
                        0x09: "AS923-3", 0x0A: "AS923-4", 0x0B: "CN470", 0x0C: "EU433",
                        0x0D: "KR920", 0x0E: "MA869"
                    }
                    data['frequency_band'] = bands.find(freq_band, f"Unknown-{freq_band:02X}")
                    
                    # Sub-band (byte 4)
                    data['sub_band'] = payload[4]
                    
                    # Battery Info (bytes 5-6)
                    var battery_raw = (payload[6] << 8) | payload[5]
                    data['battery_v'] = battery_raw / 1000.0
                    
                    # Mark as device info uplink
                    data['device_info'] = true
                end
            end
            
            # Register downlink commands if not already done
            if !global.contains("DDS75_LB_cmdInit") || !global.DDS75_LB_cmdInit
                self.register_downlink_commands()
                global.DDS75_LB_cmdInit = true
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
            
            # Store distance trend
            if data.contains('distance_mm') && !data.find('sensor_error', false)
                if !node_data.contains('distance_history')
                    node_data['distance_history'] = []
                end
                node_data['distance_history'].push(data['distance_mm'])
                if size(node_data['distance_history']) > 20
                    node_data['distance_history'].pop(0)
                end
            end
            
            # Track sensor errors
            if data.find('sensor_error', false)
                node_data['error_count'] = node_data.find('error_count', 0) + 1
                node_data['last_error'] = tasmota.rtc()['local']
            end
            
            # Save back to global storage
            global.DDS75_LB_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            lwdecode.log_error("DECODE_DDS75_LB", e, m, 
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
            var node_data = global.DDS75_LB_nodes.find(self.node, {})
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
        var name_tooltip = "Dragino DDS75-LB/LS Distance Detection Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        
        msg += lwdecode.header(name, name_tooltip, battery * 1000, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        
        # Distance measurement (primary sensor)
        if data_to_show.contains('distance_mm')
            var distance_mm = data_to_show['distance_mm']
            if data_to_show.find('sensor_error', false)
                var status = data_to_show.find('distance_status', "Error")
                fmt.add_status(status, "⚠️", "Sensor error or invalid reading")
            else
                # Show distance in appropriate units
                if distance_mm >= 1000
                    var distance_m = distance_mm / 1000.0
                    fmt.add_sensor("distance", distance_m, "Distance", nil)
                else
                    fmt.add_sensor("distance", distance_mm, "Distance", nil)
                end
            end
        end
        
        # Temperature if available
        if data_to_show.contains('temperature')
            fmt.add_sensor("temperature", data_to_show['temperature'], "Temperature", nil)
        end
        
        # Trigger type indicator
        if data_to_show.contains('interrupt')
            if data_to_show['interrupt']
                fmt.add_status("INT", "🔔", "Interrupt triggered uplink")
            else
                fmt.add_status("PER", "⏰", "Periodic uplink")
            end
        end
        
        # Device info mode
        if data_to_show.find('device_info', false)
            fmt.next_line()
            fmt.add_status(data_to_show.find('device_model', 'DDS75L'), "📟", nil)
            fmt.add_status(data_to_show.find('firmware', 'v?.?'), "🔧", "Firmware version")
            fmt.add_status(data_to_show.find('frequency_band', 'Unknown'), "📡", "LoRaWAN frequency band")
        end
        
        # Sensor detection status
        if data_to_show.contains('sensor_detected')
            if !data_to_show['sensor_detected']
                fmt.next_line()
                fmt.add_status("No sensor", "❌", "Ultrasonic sensor not detected")
            end
        end
        
        fmt.end_line()
        msg += fmt.get_msg()
        
        # Add age info if data is old
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
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.DDS75_LB_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'error_count': node_data.find('error_count', 0),
            'last_error': node_data.find('last_error', 0),
            'battery_history': node_data.find('battery_history', []),
            'distance_history': node_data.find('distance_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.DDS75_LB_nodes.contains(node_id)
            global.DDS75_LB_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Transmit Interval (0x01)
        tasmota.remove_cmd("LwDDS75LBInterval")
        tasmota.add_cmd("LwDDS75LBInterval", def(cmd, idx, payload_str)
            # Format: LwDDS75LBInterval<node> <seconds>
            var interval = int(payload_str)
            if interval < 10 || interval > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 10-16777215 seconds")
            end
            
            # Build hex command: 01 + 3 bytes little endian
            var hex_cmd = f"01{interval & 0xFF:02X}{(interval >> 8) & 0xFF:02X}{(interval >> 16) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.DDS75_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Interrupt Mode (0x06)
        tasmota.remove_cmd("LwDDS75LBInterrupt")
        tasmota.add_cmd("LwDDS75LBInterrupt", def(cmd, idx, payload_str)
            # Format: LwDDS75LBInterrupt<node> <disable|rising_edge>
            return lwdecode.SendDownlinkMap(global.DDS75_LB_nodes, cmd, idx, payload_str, { 
                'DISABLE|OFF|0': ['06000000', 'Disabled'],
                'RISING|RISING_EDGE|3': ['06000003', 'Rising Edge']
            })
        end)
        
        # Set Delta Detect Mode (0xFB)
        tasmota.remove_cmd("LwDDS75LBDelta")
        tasmota.add_cmd("LwDDS75LBDelta", def(cmd, idx, payload_str)
            # Format: LwDDS75LBDelta<node> <mode>,<interval>,<threshold>,<count>
            # Example: LwDDS75LDelta1 2,1,30,10
            var parts = string.split(payload_str, ',')
            if size(parts) != 4
                return tasmota.resp_cmnd_str("Usage: LwDDS75LBDelta<node> <mode>,<interval_s>,<threshold_cm>,<count>")
            end
            
            var mode = int(parts[0])
            var interval = int(parts[1])
            var threshold = int(parts[2])
            var count = int(parts[3])
            
            # Validate parameters
            if mode < 1 || mode > 2
                return tasmota.resp_cmnd_str("Invalid mode: 1=normal, 2=delta")
            end
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str("Invalid interval: range 1-65535 seconds")
            end
            if threshold < 1 || threshold > 65535
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-65535 cm")
            end
            if count < 5 || count > 20
                return tasmota.resp_cmnd_str("Invalid count: range 5-20 samples")
            end
            
            # Build hex command: FB + mode + interval(2) + threshold(2) + count(1)
            var hex_cmd = f"FB{mode:02X}{interval:04X}{threshold:04X}{count:02X}"
            return lwdecode.SendDownlink(global.DDS75_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request Device Status (0x26)
        tasmota.remove_cmd("LwDDS75LBStatus")
        tasmota.add_cmd("LwDDS75LBStatus", def(cmd, idx, payload_str)
            # Format: LwDDS75LBStatus<node>
            var hex_cmd = "2601"
            return lwdecode.SendDownlink(global.DDS75_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Poll Sensor Data (0x31)
        tasmota.remove_cmd("LwDDS75LBPoll")
        tasmota.add_cmd("LwDDS75LBPoll", def(cmd, idx, payload_str)
            # Format: LwDDS75LBPoll<node> <start_timestamp>,<end_timestamp>,<interval_s>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwDDS75LBPoll<node> <start_unix>,<end_unix>,<interval_s>")
            end
            
            var start_ts = int(parts[0])
            var end_ts = int(parts[1])
            var interval = int(parts[2])
            
            # Validate parameters
            if start_ts >= end_ts
                return tasmota.resp_cmnd_str("Invalid: start timestamp must be < end timestamp")
            end
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            # Build hex command: 31 + start_ts(4) + end_ts(4) + interval(1)
            var hex_cmd = f"31{start_ts:08X}{end_ts:08X}{interval:02X}"
            return lwdecode.SendDownlink(global.DDS75_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        print("DDS75LB: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_DDS75_LB()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwDDS75LBTestPayload")
tasmota.add_cmd("LwDDS75LBTestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestDDS75LB", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

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
