#
# LoRaWAN AI-Generated Decoder for Dragino LHT65 Prompted by ZioFabry
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.0
#
# Homepage:  https://www.dragino.com/products/lora-lorawan-end-node/item/151-lht65.html
# Userguide: https://www.dragino.com/downloads/downloads/LHT65/UserManual/LHT65_Temperature_Humidity_Sensor_UserManual_v1.8.5.pdf
# Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/LHT65
# 
# v1.0.0 (2025-08-20): Initial generation from cached MAP specification

class LwDecode_LHT65
    var hashCheck
    var name
    var node
    var last_data
    var last_update
    var lwdecode

    def init()
        self.hashCheck = true
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        import global
        if !global.contains("LHT65_nodes")
            global.LHT65_nodes = {}
        end
        if !global.contains("LHT65_cmdInit")
            global.LHT65_cmdInit = false
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        if payload == nil || size(payload) < 1
            return nil
        end
        
        try
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            var node_data = global.LHT65_nodes.find(node, {})
            
            if fport == 2  # Standard Payload
                if size(payload) >= 11
                    # Battery (bytes 0-1)
                    var bat_raw = (payload[1] << 8) | payload[0]
                    data['battery_status'] = (bat_raw >> 14) & 0x03
                    data['battery_mv'] = bat_raw & 0x3FFF
                    data['battery_v'] = data['battery_mv'] / 1000.0
                    data['battery_level'] = self.get_battery_status_text(data['battery_status'])
                    
                    # Built-in Temperature (bytes 2-3, signed)
                    var temp_raw = (payload[3] << 8) | payload[2]
                    if temp_raw > 32767
                        temp_raw = temp_raw - 65536
                    end
                    data['temperature'] = temp_raw / 100.0
                    
                    # Built-in Humidity (bytes 4-5)
                    var hum_raw = (payload[5] << 8) | payload[4]
                    data['humidity'] = hum_raw / 10.0
                    
                    # External sensor type (byte 6)
                    data['ext_type'] = payload[6]
                    data['ext_name'] = self.get_ext_sensor_name(data['ext_type'])
                    
                    # External sensor data (bytes 7-10)
                    if data['ext_type'] == 0x01  # E1 Temperature
                        var ext_temp_raw = (payload[8] << 8) | payload[7]
                        if ext_temp_raw != 0x7FFF  # Not disconnected
                            if ext_temp_raw > 32767
                                ext_temp_raw = ext_temp_raw - 65536
                            end
                            data['ext_temperature'] = ext_temp_raw / 100.0
                        else
                            data['ext_disconnected'] = true
                        end
                        
                    elif data['ext_type'] == 0x04  # E4 Interrupt
                        var status_byte = payload[7]
                        data['cable_connected'] = (status_byte & 0x80) != 0
                        data['interrupt_triggered'] = (status_byte & 0x40) != 0
                        data['pin_level'] = (status_byte & 0x01) != 0
                        
                    elif data['ext_type'] == 0x05  # E5 Illumination
                        var illum_raw = (payload[8] << 8) | payload[7]
                        data['illuminance'] = illum_raw
                        data['cable_connected'] = (payload[9] & 0x80) != 0
                        
                    elif data['ext_type'] == 0x06  # E6 ADC
                        var adc_raw = (payload[8] << 8) | payload[7]
                        data['adc_voltage'] = adc_raw
                        data['cable_connected'] = (payload[9] & 0x80) != 0
                        
                    elif data['ext_type'] == 0x07  # E7 Counting 16-bit
                        var count_raw = (payload[8] << 8) | payload[7]
                        data['event_count'] = count_raw
                        data['cable_connected'] = (payload[9] & 0x80) != 0
                        
                    elif data['ext_type'] == 0x08  # E7 Counting 32-bit
                        var count_raw = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
                        data['event_count'] = count_raw
                        
                    elif data['ext_type'] == 0x09  # E1 with timestamp
                        var ext_temp_raw = (payload[8] << 8) | payload[7]
                        if ext_temp_raw > 32767
                            ext_temp_raw = ext_temp_raw - 65536
                        end
                        data['ext_temperature'] = ext_temp_raw / 100.0
                        
                        var builtin_temp_raw = (payload[10] << 8) | payload[9]
                        if builtin_temp_raw > 32767
                            builtin_temp_raw = builtin_temp_raw - 65536
                        end
                        data['builtin_temp_alt'] = builtin_temp_raw / 100.0
                        data['has_timestamp'] = true
                        
                        # Note: Full timestamp parsing would need full 11-byte payload
                        
                    end
                end
            end
            
            # Update node storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Battery trend tracking
            if data.contains('battery_v')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                node_data['battery_history'].push(data['battery_v'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Temperature trend tracking
            if data.contains('temperature')
                if !node_data.contains('temp_history')
                    node_data['temp_history'] = []
                end
                node_data['temp_history'].push(data['temperature'])
                if size(node_data['temp_history']) > 10
                    node_data['temp_history'].pop(0)
                end
            end
            
            if !global.contains("LHT65_cmdInit") || !global.LHT65_cmdInit
                self.register_downlink_commands()
                global.LHT65_cmdInit = true
            end

            global.LHT65_nodes[node] = node_data
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"LHT65: Decode error - {e}: {m}")
            return nil
        end
    end
    
    def add_web_sensor()
        import global
        
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.LHT65_nodes.find(self.node, {})
            data_to_show = node_data.find('last_data', {})
            last_update = node_data.find('last_update', 0)
        end
        
        if size(data_to_show) == 0 return nil end
        
        import string
        var msg = ""
        var fmt = LwSensorFormatter_cls()
        
        var name = self.name
        if name == nil || name == ""
            name = f"LHT65-{self.node}"
        end
        var name_tooltip = "Dragino LHT65"
        var battery = data_to_show.find('battery_v', 1000)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        var simulated = data_to_show.find('simulated', false)
        
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Main sensor line
        if data_to_show.contains('temperature')
            fmt.add_sensor("string", f"{data_to_show['temperature']:.1f}°C", "Temperature", "🌡️")
        end
        
        if data_to_show.contains('humidity')
            fmt.add_sensor("string", f"{data_to_show['humidity']:.1f}%", "Humidity", "💧")
        end
        
        # External sensor data
        if data_to_show.contains('ext_temperature')
            fmt.add_sensor("string", f"{data_to_show['ext_temperature']:.1f}°C", "External Temp", "🔗")
        elif data_to_show.contains('illuminance')
            fmt.add_sensor("string", f"{data_to_show['illuminance']}lx", "Light", "☀️")
        elif data_to_show.contains('adc_voltage')
            fmt.add_sensor("string", f"{data_to_show['adc_voltage']}mV", "ADC", "📏")
        elif data_to_show.contains('event_count')
            fmt.add_sensor("string", f"{data_to_show['event_count']}", "Count", "🔢")
        end
        
        # External sensor status line
        if data_to_show.contains('ext_name') && data_to_show['ext_name'] != "None"
            fmt.next_line()
            fmt.add_sensor("string", data_to_show['ext_name'], "External Sensor", "🔗")
            
            if data_to_show.contains('cable_connected')
                var cable_icon = data_to_show['cable_connected'] ? "🔌" : "❌"
                var cable_text = data_to_show['cable_connected'] ? "Connected" : "Disconnected"
                fmt.add_sensor("string", cable_text, "Cable", cable_icon)
            end
            
            if data_to_show.contains('interrupt_triggered') && data_to_show['interrupt_triggered']
                fmt.add_sensor("string", "Triggered", "Interrupt", "⚡")
            end
            
            if data_to_show.contains('ext_disconnected') && data_to_show['ext_disconnected']
                fmt.add_sensor("string", "No Sensor", "Status", "❌")
            end
        end
        
        # Battery status line
        if data_to_show.contains('battery_level')
            if !data_to_show.contains('ext_name') || data_to_show['ext_name'] == "None"
                fmt.next_line()
            end
            fmt.add_sensor("string", data_to_show['battery_level'], "Battery", "🔋")
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600
                fmt.next_line()
                fmt.add_status(self.format_age(age), "⏱️", nil)
            end
        end
        
        fmt.end_line()
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
    
    def get_battery_status_text(status)
        if status == 0 return "Ultra Low"
        elif status == 1 return "Low"
        elif status == 2 return "OK"
        elif status == 3 return "Good"
        else return "Unknown"
        end
    end
    
    def get_ext_sensor_name(ext_type)
        if ext_type == 0x00 return "None"
        elif ext_type == 0x01 return "E1 Temperature"
        elif ext_type == 0x04 return "E4 Interrupt"
        elif ext_type == 0x05 return "E5 Illumination"
        elif ext_type == 0x06 return "E6 ADC"
        elif ext_type == 0x07 return "E7 Count16"
        elif ext_type == 0x08 return "E7 Count32"
        elif ext_type == 0x09 return "E1 Timestamp"
        else return f"Unknown(0x{ext_type:02X})"
        end
    end
    
    def get_node_stats(node_id)
        import global
        var node_data = global.LHT65_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'battery_history': node_data.find('battery_history', []),
            'temp_history': node_data.find('temp_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    def clear_node_data(node_id)
        import global
        if global.LHT65_nodes.contains(node_id)
            global.LHT65_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    def register_downlink_commands()
        import string
        
        # Set Transmit Interval
        tasmota.remove_cmd("LwLHT65SetInterval")
        tasmota.add_cmd("LwLHT65SetInterval", def(cmd, idx, payload_str)
            var interval = int(payload_str)
            if interval < 1 || interval > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 1-16777215 seconds")
            end
            
            var hex_cmd = f"01{(interval >> 16) & 0xFF:02X}{(interval >> 8) & 0xFF:02X}{interval & 0xFF:02X}"
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set External Sensor Mode
        tasmota.remove_cmd("LwLHT65ExtSensor")
        tasmota.add_cmd("LwLHT65ExtSensor", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            var sensor_type = int(parts[0])
            
            if sensor_type < 1 || sensor_type > 9
                return tasmota.resp_cmnd_str("Invalid sensor type: 1-9")
            end
            
            var hex_cmd = f"A2{sensor_type:02X}"
            
            if size(parts) > 1
                var param = int(parts[1])
                hex_cmd += f"{param:02X}"
                
                if size(parts) > 2 && sensor_type == 6  # ADC timeout
                    var timeout = int(parts[2])
                    hex_cmd += f"{(timeout >> 8) & 0xFF:02X}{timeout & 0xFF:02X}"
                end
            end
            
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Enable/Disable DS18B20 Probe ID
        tasmota.remove_cmd("LwLHT65ProbeID")
        tasmota.add_cmd("LwLHT65ProbeID", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.LHT65_nodes, cmd, idx, payload_str, { 
                '1|ENABLE':  ['A801', 'Probe ID Enabled'],
                '0|DISABLE': ['A800', 'Probe ID Disabled']
            })
        end)
        
        # Set System Time
        tasmota.remove_cmd("LwLHT65SetTime")
        tasmota.add_cmd("LwLHT65SetTime", def(cmd, idx, payload_str)
            var timestamp = int(payload_str)
            if timestamp < 0
                return tasmota.resp_cmnd_str("Invalid timestamp")
            end
            
            var hex_cmd = f"30{lwdecode.uint32be(timestamp)}"
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Time Sync Mode
        tasmota.remove_cmd("LwLHT65TimeSync")
        tasmota.add_cmd("LwLHT65TimeSync", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.LHT65_nodes, cmd, idx, payload_str, { 
                '1|AUTO':    ['2801', 'Auto Sync'],
                '0|MANUAL':  ['2800', 'Manual Sync']
            })
        end)
        
        # Clear Flash Record
        tasmota.remove_cmd("LwLHT65ClearFlash")
        tasmota.add_cmd("LwLHT65ClearFlash", def(cmd, idx, payload_str)
            var hex_cmd = "A301"
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Poll Sensor Data
        tasmota.remove_cmd("LwLHT65Poll")
        tasmota.add_cmd("LwLHT65Poll", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwLHT65Poll<slot> <start_ts>,<end_ts>,<interval>")
            end
            
            var start_ts = int(parts[0])
            var end_ts = int(parts[1])
            var interval = int(parts[2])
            
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            var hex_cmd = f"31{lwdecode.uint32be(start_ts)}{lwdecode.uint32be(end_ts)}{interval:02X}"
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        print("LHT65: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_LHT65()

# Node management commands
tasmota.remove_cmd("LwLHT65NodeStats")
tasmota.add_cmd("LwLHT65NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwLHT65ClearNode")
tasmota.add_cmd("LwLHT65ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Test UI command
tasmota.remove_cmd("LwLHT65TestUI")
tasmota.add_cmd("LwLHT65TestUI", def(cmd, idx, payload_str)
    var test_scenarios = {
        "normal":       "C00A0C0A580200000000",      # Good battery, 26.2°C, 60.0%, no external
        "ext_temp":     "C00A0C0A58020A1A0100",      # With E1 temperature sensor 28.2°C
        "illumination": "C00A0C0A580205E8030080",    # E5 illumination 1000lx, cable connected
        "adc":          "C00A0C0A580206E8030080",    # E6 ADC 1000mV, cable connected
        "counting":     "C00A0C0A580207E8030080",    # E7 counting 1000 events, cable connected
        "interrupt":    "C00A0C0A58020481000000",    # E4 interrupt triggered, cable connected
        "low_battery":  "400A0C0A580200000000",      # Ultra low battery
        "disconnected": "C00A0C0A580201FF7F0000"     # E1 sensor disconnected (0x7FFF)
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

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
