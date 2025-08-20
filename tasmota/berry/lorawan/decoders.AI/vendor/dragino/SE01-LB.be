#
# LoRaWAN AI-Generated Decoder for Dragino SE01-LB Prompted by User Request
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.3
#
# Homepage:  https://www.dragino.com/products/agriculture-weather-station/item/277-se01-lb.html
# Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SE01-LB_LoRaWAN_Soil%20Moisture%26EC_Sensor_User_Manual/
# Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/
# 
# v1.0.0 (2025-08-20): Initial generation from wiki specification

class LwDecode_SE01LB
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
        if !global.contains("SE01LB_nodes")
            global.SE01LB_nodes = {}
        end
        if !global.contains("SE01LB_cmdInit")
            global.SE01LB_cmdInit = false
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
            var node_data = global.SE01LB_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 2
                # Main sensor data - multiple modes possible
                if size(payload) == 11
                    # 11-byte payload: interrupt mode (MOD=0 or MOD=1)
                    data = self.decode_11_byte_payload(payload, data)
                elif size(payload) == 15
                    # 15-byte payload: counting mode (MOD=0 or MOD=1)
                    data = self.decode_15_byte_payload(payload, data)
                else
                    print(f"SE01LB: Unexpected payload size {size(payload)} for fport 2")
                    return nil
                end
                
            elif fport == 5
                # Device status
                if size(payload) >= 7
                    data = self.decode_device_status(payload, data)
                else
                    print(f"SE01LB: Invalid device status payload size: {size(payload)}")
                    return nil
                end
                
            elif fport == 3
                # Datalog data
                data = self.decode_datalog(payload, data)
                
            else
                print(f"SE01LB: Unknown fport: {fport}")
                return nil
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
            
            # Store soil measurement trends
            if data.contains('soil_moisture')
                if !node_data.contains('moisture_history')
                    node_data['moisture_history'] = []
                end
                node_data['moisture_history'].push(data['soil_moisture'])
                if size(node_data['moisture_history']) > 10
                    node_data['moisture_history'].pop(0)
                end
            end
            
            # Track count increments (if in counting mode)
            if data.contains('count_value')
                var prev_count = node_data.find('last_count', 0)
                if data['count_value'] > prev_count
                    node_data['count_increment'] = data['count_value'] - prev_count
                end
                node_data['last_count'] = data['count_value']
            end
            
            # Implement downlinks if present and create relative tasmota commands
            if !global.contains("SE01LB_cmdInit") || !global.SE01LB_cmdInit
                self.register_downlink_commands()
                global.SE01LB_cmdInit = true
            end

            # Save back to global storage
            global.SE01LB_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"SE01LB: Decode error - {e}: {m}")
            return nil
        end
    end
    
    def decode_11_byte_payload(payload, data)
        # DS18B20 Temperature (bytes 0-1)
        var ds18b20_raw = (payload[1] << 8) | payload[0]
        if (ds18b20_raw & 0x8000) != 0
            # Negative temperature
            ds18b20_raw = ds18b20_raw - 0x10000
        end
        var ds18b20_temp = ds18b20_raw / 10.0
        if ds18b20_temp > 300  # 327.67°C indicates disconnected sensor
            data['ds18b20_disconnected'] = true
        else
            data['ds18b20_temperature'] = ds18b20_temp
        end
        
        # Battery voltage (bytes 8-9)
        data['battery_voltage'] = (payload[9] << 8) | payload[8]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # MOD & Flags (byte 10)
        var flags = payload[10]
        var mod = (flags >> 7) & 0x01
        var i_flag = (flags >> 6) & 0x01
        var s_flag = (flags >> 5) & 0x01
        
        data['working_mode'] = mod
        data['interrupt_triggered'] = i_flag == 1
        data['sensor_identified'] = s_flag == 1
        
        if mod == 0
            # Default mode - calibrated values
            # Soil Moisture (bytes 2-3)
            var moisture_raw = (payload[3] << 8) | payload[2]
            data['soil_moisture'] = moisture_raw / 100.0
            
            # Soil Temperature (bytes 4-5)
            var soil_temp_raw = (payload[5] << 8) | payload[4]
            if (soil_temp_raw & 0x8000) != 0
                soil_temp_raw = soil_temp_raw - 0x10000
            end
            data['soil_temperature'] = soil_temp_raw / 100.0
            
            # Soil Conductivity (bytes 6-7)
            data['soil_conductivity'] = (payload[7] << 8) | payload[6]
            
        else
            # Original value mode - raw ADC values
            data['soil_conductivity_raw'] = (payload[3] << 8) | payload[2]
            data['soil_moisture_raw'] = (payload[5] << 8) | payload[4]
            data['dielectric_constant_raw'] = (payload[7] << 8) | payload[6]
        end
        
        return data
    end
    
    def decode_15_byte_payload(payload, data)
        # First decode as 11-byte payload
        data = self.decode_11_byte_payload(payload, data)
        
        # Extract count mode flag from byte 10
        var flags = payload[10]
        var count_mod = (flags >> 4) & 0x01
        data['counting_mode'] = count_mod == 1
        
        # Count value (bytes 11-14) - big endian
        var count_value = (payload[11] << 24) | (payload[12] << 16) | (payload[13] << 8) | payload[14]
        data['count_value'] = count_value
        
        return data
    end
    
    def decode_device_status(payload, data)
        # Sensor Model (byte 0)
        data['sensor_model'] = payload[0]
        
        # Firmware Version (bytes 1-2)
        var fw_major = (payload[1] >> 4) & 0x0F
        var fw_minor = payload[1] & 0x0F
        var fw_patch = (payload[2] >> 4) & 0x0F
        data['firmware_version'] = f"{fw_major}.{fw_minor}.{fw_patch}"
        
        # Frequency Band (byte 3)
        var band_map = {
            0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
            0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-1",
            0x09: "AS923-2", 0x0a: "AS923-3", 0x0b: "CN470", 0x0c: "EU433",
            0x0d: "KR920", 0x0e: "MA869"
        }
        data['frequency_band'] = band_map.find(payload[3], f"Unknown({payload[3]:02X})")
        
        # Sub-band (byte 4)
        data['sub_band'] = payload[4]
        
        # Battery voltage (bytes 5-6)
        data['battery_voltage'] = (payload[6] << 8) | payload[5]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        return data
    end
    
    def decode_datalog(payload, data)
        # Datalog entries are 11 bytes each
        var entry_size = 11
        var num_entries = size(payload) / entry_size
        
        data['datalog_entries'] = []
        
        var i = 0
        while i < size(payload) && i + entry_size <= size(payload)
            var entry = {}
            
            # MOD & Level (byte 6 in each entry)
            var mod_flags = payload[i + 6]
            var mod = (mod_flags >> 7) & 0x01
            
            if mod == 0
                # Default mode datalog
                entry['humidity'] = ((payload[i+1] << 8) | payload[i]) / 100.0
                entry['temperature'] = self.decode_signed_16(payload[i+3], payload[i+2]) / 100.0
                entry['soil_conductivity'] = (payload[i+5] << 8) | payload[i+4]
            else
                # Raw mode datalog
                entry['soil_dielectric_raw'] = (payload[i+1] << 8) | payload[i]
                entry['raw_water_soil'] = (payload[i+3] << 8) | payload[i+2]
                entry['raw_conduct_soil'] = (payload[i+5] << 8) | payload[i+4]
            end
            
            # Unix timestamp (bytes 7-10)
            entry['timestamp'] = (payload[i+7] << 24) | (payload[i+8] << 16) | (payload[i+9] << 8) | payload[i+10]
            entry['datetime'] = self.format_unix_timestamp(entry['timestamp'])
            
            data['datalog_entries'].push(entry)
            i += entry_size
        end
        
        data['datalog_count'] = size(data['datalog_entries'])
        
        return data
    end
    
    def decode_signed_16(high_byte, low_byte)
        var value = (high_byte << 8) | low_byte
        if (value & 0x8000) != 0
            value = value - 0x10000
        end
        return value
    end
    
    def format_unix_timestamp(timestamp)
        # Basic timestamp formatting - could be enhanced
        if timestamp == 0
            return "Not set"
        end
        return f"Unix:{timestamp}"
    end
    
    def add_web_sensor()
        import global
        
        # Try to use current instance data first
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        # If no instance data, try to recover from global storage
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.SE01LB_nodes.find(self.node, {})
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
            name = f"SE01LB-{self.node}"
        end
        var name_tooltip = "Dragino SE01-LB Soil Moisture & EC Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Primary soil measurements
        if data_to_show.contains('soil_moisture')
            fmt.add_sensor("humidity", data_to_show['soil_moisture'], "Soil Moisture", "💧")
        end
        
        if data_to_show.contains('soil_temperature')
            fmt.add_sensor("temp", data_to_show['soil_temperature'], "Soil Temp", "🌡️")
        end
        
        if data_to_show.contains('soil_conductivity')
            fmt.add_sensor("conductivity", data_to_show['soil_conductivity'], "EC", "⚡")
        end
        
        # External temperature sensor
        if data_to_show.contains('ds18b20_temperature')
            fmt.next_line()
            fmt.add_sensor("temp", data_to_show['ds18b20_temperature'], "Ext Temp", "🌡️")
        end
        
        # Sensor status and mode info
        if data_to_show.contains('working_mode')
            var mode_str = data_to_show['working_mode'] == 0 ? "Cal" : "Raw"
            fmt.add_sensor("string", mode_str, "Mode", "⚙️")
        end
        
        if data_to_show.contains('counting_mode') && data_to_show['counting_mode']
            if data_to_show.contains('count_value')
                fmt.add_sensor("string", f"{data_to_show['count_value']}", "Count", "🔢")
            end
        end
        
        # Device events and status
        var has_events = false
        if data_to_show.contains('ds18b20_disconnected') && data_to_show['ds18b20_disconnected']
            fmt.next_line()
            fmt.add_sensor("string", "DS18B20 Disc", "Sensor Status", "⚠️")
            has_events = true
        end
        
        if data_to_show.contains('interrupt_triggered') && data_to_show['interrupt_triggered']
            if !has_events
                fmt.next_line()
                has_events = true
            end
            fmt.add_sensor("string", "Triggered", "Interrupt", "📶")
        end
        
        if data_to_show.contains('sensor_identified') && !data_to_show['sensor_identified']
            if !has_events
                fmt.next_line()
                has_events = true
            end
            fmt.add_sensor("string", "No Sensor", "Status", "❌")
        end
        
        # Device info (firmware, band) if available
        if data_to_show.contains('firmware_version')
            fmt.next_line()
            fmt.add_sensor("string", data_to_show['firmware_version'], "Firmware", "💾")
            
            if data_to_show.contains('frequency_band')
                fmt.add_sensor("string", data_to_show['frequency_band'], "Band", "📡")
            end
        end
        
        # Datalog info
        if data_to_show.contains('datalog_count') && data_to_show['datalog_count'] > 0
            fmt.next_line()
            fmt.add_sensor("string", f"{data_to_show['datalog_count']} entries", "Datalog", "📊")
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.next_line()
                fmt.add_sensor("string", self.format_age(age), "Last Seen", "⏱️")
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
        var node_data = global.SE01LB_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'name': node_data.find('name', 'Unknown'),
            'battery_history': node_data.find('battery_history', []),
            'moisture_history': node_data.find('moisture_history', []),
            'last_count': node_data.find('last_count', 0),
            'count_increment': node_data.find('count_increment', 0)
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.SE01LB_nodes.contains(node_id)
            global.SE01LB_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set transmit interval (TDC)
        tasmota.remove_cmd("LwSE01LBInterval")
        tasmota.add_cmd("LwSE01LBInterval", def(cmd, idx, payload_str)
            # Format: LwSE01LBInterval<slot> <seconds>
            var seconds = int(payload_str)
            if seconds < 30 || seconds > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 30-16777215 seconds")
            end
            
            # Build hex command: 01 + 3 bytes (big endian)
            var hex_cmd = f"01{(seconds >> 16) & 0xFF:02X}{(seconds >> 8) & 0xFF:02X}{seconds & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SE01LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device reset
        tasmota.remove_cmd("LwSE01LBReset")
        tasmota.add_cmd("LwSE01LBReset", def(cmd, idx, payload_str)
            # Format: LwSE01LBReset<slot>
            return lwdecode.SendDownlink(global.SE01LB_nodes, cmd, idx, "04FF")
        end)
        
        # Set confirm mode
        tasmota.remove_cmd("LwSE01LBConfirm")
        tasmota.add_cmd("LwSE01LBConfirm", def(cmd, idx, payload_str)
            # Format: LwSE01LBConfirm<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.SE01LB_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['05000001', 'Confirmed' ],
                '0|OFF': ['05000000', 'Unconfirmed']
            })
        end)
        
        # Set interrupt mode
        tasmota.remove_cmd("LwSE01LBInterrupt")
        tasmota.add_cmd("LwSE01LBInterrupt", def(cmd, idx, payload_str)
            # Format: LwSE01LBInterrupt<slot> <0|3>
            return lwdecode.SendDownlinkMap(global.SE01LB_nodes, cmd, idx, payload_str, { 
                '0|OFF|DISABLE':  ['06000000', 'Disabled' ],
                '3|ON|RISING':    ['06000003', 'Rising Edge']
            })
        end)
        
        # Set 5V output duration
        tasmota.remove_cmd("LwSE01LBPower5V")
        tasmota.add_cmd("LwSE01LBPower5V", def(cmd, idx, payload_str)
            # Format: LwSE01LBPower5V<slot> <milliseconds>
            var ms = int(payload_str)
            if ms < 0 || ms > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535 ms")
            end
            
            var hex_cmd = f"07{(ms >> 8) & 0xFF:02X}{ms & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SE01LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set count value
        tasmota.remove_cmd("LwSE01LBSetCount")
        tasmota.add_cmd("LwSE01LBSetCount", def(cmd, idx, payload_str)
            # Format: LwSE01LBSetCount<slot> <count>
            var count = int(payload_str)
            if count < 0 || count > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 0-4294967295")
            end
            
            var hex_cmd = f"09{(count >> 24) & 0xFF:02X}{(count >> 16) & 0xFF:02X}{(count >> 8) & 0xFF:02X}{count & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SE01LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set working mode
        tasmota.remove_cmd("LwSE01LBMode")
        tasmota.add_cmd("LwSE01LBMode", def(cmd, idx, payload_str)
            # Format: LwSE01LBMode<slot> <default|raw|0|1>
            return lwdecode.SendDownlinkMap(global.SE01LB_nodes, cmd, idx, payload_str, { 
                '0|DEFAULT|CAL':  ['0A00', 'Default Mode' ],
                '1|RAW|ORIG':     ['0A01', 'Raw Mode']
            })
        end)
        
        # Set count mode
        tasmota.remove_cmd("LwSE01LBCountMode")
        tasmota.add_cmd("LwSE01LBCountMode", def(cmd, idx, payload_str)
            # Format: LwSE01LBCountMode<slot> <interrupt|counting|0|1>
            return lwdecode.SendDownlinkMap(global.SE01LB_nodes, cmd, idx, payload_str, { 
                '0|INTERRUPT|INT': ['1000', 'Interrupt Mode' ],
                '1|COUNTING|COUNT': ['1001', 'Counting Mode']
            })
        end)
        
        # Request device status
        tasmota.remove_cmd("LwSE01LBStatus")
        tasmota.add_cmd("LwSE01LBStatus", def(cmd, idx, payload_str)
            # Format: LwSE01LBStatus<slot>
            return lwdecode.SendDownlink(global.SE01LB_nodes, cmd, idx, "2601")
        end)
        
        # Poll datalog
        tasmota.remove_cmd("LwSE01LBPollLog")
        tasmota.add_cmd("LwSE01LBPollLog", def(cmd, idx, payload_str)
            # Format: LwSE01LBPollLog<slot> <start_timestamp>,<end_timestamp>,<interval>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwSE01LBPollLog<slot> <start_ts>,<end_ts>,<interval_s>")
            end
            
            var start_ts = int(parts[0])
            var end_ts = int(parts[1])
            var interval = int(parts[2])
            
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            var hex_cmd = f"31{(start_ts >> 24) & 0xFF:02X}{(start_ts >> 16) & 0xFF:02X}{(start_ts >> 8) & 0xFF:02X}{start_ts & 0xFF:02X}"
            hex_cmd += f"{(end_ts >> 24) & 0xFF:02X}{(end_ts >> 16) & 0xFF:02X}{(end_ts >> 8) & 0xFF:02X}{end_ts & 0xFF:02X}"
            hex_cmd += f"{interval:02X}"
            
            return lwdecode.SendDownlink(global.SE01LB_nodes, cmd, idx, hex_cmd)
        end)
        
        print("SE01LB: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_SE01LB()

# Node management commands
tasmota.remove_cmd("LwSE01LBNodeStats")
tasmota.add_cmd("LwSE01LBNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwSE01LBClearNode")
tasmota.add_cmd("LwSE01LBClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwSE01LBTestUI<slot> <scenario>
tasmota.remove_cmd("LwSE01LBTestUI")
tasmota.add_cmd("LwSE01LBTestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        # Normal soil monitoring with moderate values
        "normal":    "D604E4036E19BD0CAF0C5A",      # 21.2°C, 25.36% moisture, 22.54°C soil, 3261 uS/cm, 3.247V
        # Dry soil conditions with low moisture
        "dry":       "C7041901F611E008AF0C4A",      # 19.9°C, 5.45% moisture, 45.70°C soil, 2272 uS/cm, 3.151V  
        # Wet soil conditions with high moisture
        "wet":       "E6051C1EE016CC1EAF0C6A",      # 23.0°C, 82.04% moisture, 58.88°C soil, 7372 uS/cm, 3.191V
        # Low battery warning
        "lowbatt":   "D604E4036E19BD0C200B5A",      # Normal values but 2.848V battery
        # Counting mode with count value
        "counting":  "D604E4036E19BD0CAF0C1AE8030000", # Normal + counting mode + count 1000
        # Raw mode values (MOD=1)
        "raw":       "000045671234ABCDAF0CDA",      # Raw ADC values, MOD=1
        # Device status response (fport=5)
        "status":    "260200010C00AF0C",            # Model 0x26, FW v2.0.0, CN470, subband 0, 3.247V
        # External sensor disconnected
        "disconn":   "CC0CE4036E19BD0CAF0C5A"       # DS18B20 shows 327.6°C (disconnected)
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
    var fport = hex_payload == test_scenarios['status'] ? 5 : 2

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)