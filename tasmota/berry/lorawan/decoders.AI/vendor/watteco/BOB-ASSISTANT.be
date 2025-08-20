#
# LoRaWAN AI-Generated Decoder for Watteco Bob Assistant Prompted by ZioFabry
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.3
#
# Homepage:  https://www.watteco.com/products/bob-assistant-and-movee-solutions/
# Userguide: https://www.youtube.com/watch?v=MTV40U8OKg0
# Decoder:   https://raw.githubusercontent.com/TheThingsNetwork/lorawan-devices/master/vendor/watteco/bob-assistant.js
# 
# v1.0.0 (2025-08-20): Initial generation from TTN device repository

class LwDecode_BOB_ASSISTANT
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
        if !global.contains("BOB_ASSISTANT_nodes")
            global.BOB_ASSISTANT_nodes = {}
        end
        if !global.contains("BOB_ASSISTANT_cmdInit")
            global.BOB_ASSISTANT_cmdInit = false
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        # Validate inputs
        if payload == nil || size(payload) < 3
            return nil
        end
        
        try
            # Store device info
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            # Retrieve node history from global storage
            var node_data = global.BOB_ASSISTANT_nodes.find(node, {})
            
            # Decode based on frame type (first byte)
            var frame_type = payload[0]
            
            if frame_type == 0x72 || frame_type == 0x52
                # Report Frame
                data['frame_type'] = "Report"
                
                if size(payload) >= 27
                    # Calculate report period
                    var report_period = payload[6]
                    if report_period <= 0x3B
                        data['report_period_min'] = report_period
                    else
                        data['report_period_min'] = (report_period - 59) * 60
                    end
                    
                    # Calculate operating time
                    data['operating_time_hours'] = payload[2] * 2.0 / 127
                    
                    # Basic measurements
                    data['battery_percent'] = payload[17] * 100.0 / 127
                    data['anomaly_level'] = payload[1] * 100.0 / 127
                    data['temperature'] = payload[5] - 30
                    data['alarm_count'] = payload[4]
                    
                    # Vibration level calculation
                    var vib_raw = payload[8] * 128 + payload[9] + payload[10] / 100.0
                    data['vibration_level'] = vib_raw / 10.0 / 121.45
                    
                    # Peak frequency
                    data['peak_freq_index'] = payload[11] + 1
                    
                    # Anomaly levels for different time periods
                    data['anomaly_20_24h'] = payload[18]
                    data['anomaly_50_24h'] = payload[19]
                    data['anomaly_80_24h'] = payload[20]
                    data['anomaly_20_30d'] = payload[21]
                    data['anomaly_50_30d'] = payload[22]
                    data['anomaly_80_30d'] = payload[23]
                    data['anomaly_20_6mo'] = payload[24]
                    data['anomaly_50_6mo'] = payload[25]
                    data['anomaly_80_6mo'] = payload[26]
                end
                
            elif frame_type == 0x61
                # Alarm Frame
                data['frame_type'] = "Alarm"
                
                if size(payload) >= 40
                    data['temperature'] = payload[2] - 30
                    data['anomaly_level'] = payload[1] * 100.0 / 127
                    
                    # Vibration level calculation
                    var vib_raw = payload[4] * 128 + payload[5] + payload[6] / 100.0
                    data['vibration_level'] = vib_raw / 10.0 / 121.45
                    
                    # FFT data
                    var fft_data = []
                    for i:8..39
                        if i < size(payload)
                            fft_data.push(payload[i] * data['vibration_level'] / 127.0)
                        end
                    end
                    data['fft_count'] = size(fft_data)
                    data['fft_max'] = 0
                    for val:fft_data
                        if val > data['fft_max'] data['fft_max'] = val end
                    end
                end
                
            elif frame_type == 0x6C
                # Learning Frame
                data['frame_type'] = "Learning"
                
                if size(payload) >= 40
                    data['temperature'] = payload[6] - 30
                    data['learning_percentage'] = payload[1]
                    data['learning_from_scratch'] = payload[7]
                    
                    # Vibration level calculation
                    var vib_raw = payload[2] * 128 + payload[3] + payload[4] / 100.0
                    data['vibration_level'] = vib_raw / 10.0 / 121.45
                    
                    # Peak frequency
                    data['peak_freq_index'] = payload[5] + 1
                    data['peak_frequency_hz'] = (payload[5] + 1) * 800.0 / 256
                    
                    # FFT data
                    var fft_data = []
                    for i:8..39
                        if i < size(payload)
                            fft_data.push(payload[i] * data['vibration_level'] / 127.0)
                        end
                    end
                    data['fft_count'] = size(fft_data)
                    data['fft_max'] = 0
                    for val:fft_data
                        if val > data['fft_max'] data['fft_max'] = val end
                    end
                end
                
            elif frame_type == 0x53
                # State Frame
                data['frame_type'] = "State"
                
                if size(payload) >= 3
                    var state_code = payload[1]
                    var state_text = "Unknown"
                    if state_code == 100 state_text = "Sensor start"
                    elif state_code == 101 state_text = "Sensor stop"
                    elif state_code == 125 state_text = "Machine stop"
                    elif state_code == 126 state_text = "Machine start"
                    end
                    data['state'] = state_text
                    data['battery_percent'] = payload[2] * 100.0 / 127
                end
                
            else
                print(f"BOB-ASSISTANT: Unknown frame type {frame_type:02X}")
                return nil
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store battery trend
            if data.contains('battery_percent')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                node_data['battery_history'].push(data['battery_percent'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
            end
            
            # Store vibration trend
            if data.contains('vibration_level')
                if !node_data.contains('vibration_history')
                    node_data['vibration_history'] = []
                end
                node_data['vibration_history'].push(data['vibration_level'])
                if size(node_data['vibration_history']) > 20
                    node_data['vibration_history'].pop(0)
                end
            end
            
            # Count alarms
            if data['frame_type'] == "Alarm"
                node_data['alarm_count'] = node_data.find('alarm_count', 0) + 1
                node_data['last_alarm'] = tasmota.rtc()['local']
            end
            
            # Save back to global storage
            global.BOB_ASSISTANT_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"BOB-ASSISTANT: Decode error - {e}: {m}")
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
            var node_data = global.BOB_ASSISTANT_nodes.find(self.node, {})
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
            name = f"BOB-ASSISTANT-{self.node}"
        end
        var name_tooltip = "Watteco Vibration Sensor"
        var battery = data_to_show.find('battery_percent', 1000)  # Use 1000 to hide if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        var frame_type = data_to_show.find('frame_type', 'Unknown')
        
        # Show vibration level if available
        if data_to_show.contains('vibration_level')
            fmt.add_sensor("vibration", data_to_show['vibration_level'], "Vibration", "📳")
        end
        
        # Show temperature
        if data_to_show.contains('temperature')
            fmt.add_sensor("temp", data_to_show['temperature'], "Temperature", "🌡️")
        end
        
        # Show frame type specific data
        if frame_type == "Report"
            if data_to_show.contains('anomaly_level')
                var anomaly = data_to_show['anomaly_level']
                var anomaly_emoji = anomaly > 50 ? "🚨" : (anomaly > 20 ? "⚠️" : "✅")
                fmt.add_sensor("string", f"{anomaly:1.0f}%", "Anomaly", anomaly_emoji)
            end
            
            if data_to_show.contains('operating_time_hours')
                fmt.add_sensor("string", f"{data_to_show['operating_time_hours']:1.1f}h", "Runtime", "⏱️")
            end
            
        elif frame_type == "Alarm"
            fmt.add_status("ALARM", "🚨", "Vibration alarm triggered")
            
            if data_to_show.contains('fft_count')
                fmt.add_sensor("string", f"{data_to_show['fft_count']}", "FFT Points", "📊")
            end
            
        elif frame_type == "Learning"
            if data_to_show.contains('learning_percentage')
                fmt.add_sensor("string", f"{data_to_show['learning_percentage']}%", "Learning", "🧠")
            end
            
            if data_to_show.contains('peak_frequency_hz')
                fmt.add_sensor("string", f"{data_to_show['peak_frequency_hz']:1.1f}Hz", "Peak Freq", "🔊")
            end
            
        elif frame_type == "State"
            if data_to_show.contains('state')
                var state = data_to_show['state']
                var state_emoji = "🔄"
                if state == "Sensor start" || state == "Machine start" state_emoji = "🟢"
                elif state == "Sensor stop" || state == "Machine stop" state_emoji = "🔴"
                end
                fmt.add_status(state, state_emoji, "Device state")
            end
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.next_line()
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
        var node_data = global.BOB_ASSISTANT_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'battery_history': node_data.find('battery_history', []),
            'vibration_history': node_data.find('vibration_history', []),
            'alarm_count': node_data.find('alarm_count', 0),
            'last_alarm': node_data.find('last_alarm', 0),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.BOB_ASSISTANT_nodes.contains(node_id)
            global.BOB_ASSISTANT_nodes.remove(node_id)
            return true
        end
        return false
    end
end

# Global instance
LwDeco = LwDecode_BOB_ASSISTANT()

# Node management commands
tasmota.remove_cmd("LwBOB_ASSISTANTNodeStats")
tasmota.add_cmd("LwBOB_ASSISTANTNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwBOB_ASSISTANTClearNode")
tasmota.add_cmd("LwBOB_ASSISTANTClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Test command with realistic scenarios
tasmota.remove_cmd("LwBOB_ASSISTANTTestUI")
tasmota.add_cmd("LwBOB_ASSISTANTTestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "report":        "72017F7C00370200023D1B107F0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",  # Report frame
        "alarm":         "61507F1900255A0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",    # Alarm frame
        "learning":      "6C000217500037017F1E14100C0B0809060704060509060505040305050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050502", # Learning frame
        "sensor_start":  "53647F",                                                                                                                                                                                             # Sensor start
        "sensor_stop":   "53657F",                                                                                                                                                                                             # Sensor stop
        "machine_start": "537E7F",                                                                                                                                                                                             # Machine start
        "machine_stop":  "537D7F",                                                                                                                                                                                             # Machine stop
        "high_vibration":"72017F7C00370200023D1B50FF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",  # High vibration report
        "low_battery":   "720150500037020002205050FF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"   # Low battery report
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

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
