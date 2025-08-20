#
# LoRaWAN AI-Generated Decoder for Micropelt MLR003 Prompted by ZioFabry
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.3
#
# Homepage:  https://micropelt.atlassian.net/wiki/spaces/MH/pages/19300575/MLR003RiEU61-07+Title+Page
# Userguide: https://www.thethingsnetwork.org/device-repository/devices/micropelt/mlr003/
# Decoder:   https://raw.githubusercontent.com/TheThingsNetwork/lorawan-devices/master/vendor/micropelt/mlr003.js
# 
# v1.0.0 (2025-08-20): Initial generation from TTN device repository

class LwDecode_MLR003
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
        if !global.contains("MLR003_nodes")
            global.MLR003_nodes = {}
        end
        if !global.contains("MLR003_cmdInit")
            global.MLR003_cmdInit = false
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
            var node_data = global.MLR003_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 1 && size(payload) >= 11
                # Main device data
                data['valve_position'] = payload[0]
                data['flow_sensor_raw'] = payload[1] * 0.5
                data['flow_temperature'] = payload[2] * 0.5
                data['ambient_sensor_raw'] = payload[3] * 0.25
                data['ambient_temperature'] = payload[4] * 0.25
                
                # Status flags from byte 5
                var flags = payload[5]
                data['temperature_drop_detection'] = (flags >> 7) & 0x01
                data['energy_storage'] = (flags >> 6) & 0x01
                data['harvesting_active'] = (flags >> 5) & 0x01
                data['ambient_sensor_failure'] = (flags >> 4) & 0x01
                data['flow_sensor_failure'] = (flags >> 3) & 0x01
                data['radio_communication_error'] = (flags >> 2) & 0x01
                data['received_signal_strength'] = (flags >> 1) & 0x01
                data['motor_error'] = flags & 0x01
                
                data['storage_voltage'] = payload[6] * 0.02
                data['current_consumed'] = payload[7] * 10
                data['current_generated'] = payload[8] * 10
                
                # Status flags from byte 9
                var flags2 = payload[9]
                data['operating_condition'] = (flags2 >> 7) & 0x01
                data['storage_fully_charged'] = (flags2 >> 6) & 0x01
                data['zero_error'] = (flags2 >> 5) & 0x01
                data['calibration_ok'] = (flags2 >> 4) & 0x01
                
                # User mode
                var user_mode = flags2 & 0x7
                var mode_text = "Unknown"
                if user_mode == 0 mode_text = "Valve_Position"
                elif user_mode == 1 mode_text = "RESERVED"
                elif user_mode == 2 mode_text = "SP_Ambient_Temperature"
                elif user_mode == 3 mode_text = "Detecting_Opening_Point"
                elif user_mode == 4 mode_text = "Slow_Harvesting"
                elif user_mode == 5 mode_text = "Temperature_Drop"
                elif user_mode == 6 mode_text = "Freeze_Protect"
                elif user_mode == 7 mode_text = "Forced_Heating"
                end
                data['user_mode'] = mode_text
                
                # User value (depends on mode)
                var user_value = payload[10]
                if user_mode == 0 || user_mode == 6 || user_mode == 7
                    data['user_value'] = user_value  # Valve position or percent
                elif user_mode == 2
                    data['user_value'] = user_value * 0.5  # Temperature
                elif user_mode == 3 || user_mode == 4
                    data['user_value'] = user_value * 0.25  # Special modes
                end
                
                # Optional used temperature
                if size(payload) >= 12
                    data['used_temperature'] = payload[11] * 0.25
                end
                
            elif fport == 2 && size(payload) >= 6
                # Device version
                var rev_major = payload[0] & 0xF
                var rev_minor = (payload[0] >> 4) & 0xF
                data['rev'] = f"{rev_major}.{rev_minor:x}"
                
                var hw_major = payload[1] & 0xF
                var hw_minor = (payload[1] >> 4) & 0xF
                data['hw_version'] = f"{hw_major}.{hw_minor}"
                
                data['fw_version'] = f"20{payload[2]}.{payload[3]}.{payload[4]}.{payload[5]}"
                
            elif fport == 3 && size(payload) >= 1
                # Motor range
                var range_map = {
                    0: 2.56048, 3: 0.624, 4: 0.832, 5: 1.040, 6: 1.248,
                    7: 1.456, 8: 1.664, 9: 1.872, 10: 2.080, 11: 2.288, 12: 2.496
                }
                data['motor_range'] = range_map.find(payload[0], 0)
                
            elif fport == 4 && size(payload) >= 1
                # Spreading factor
                data['spreading_factor'] = payload[0] == 0 ? "SF7" : (payload[0] == 1 ? "SF8" : "Invalid")
                
            elif fport == 5 && size(payload) >= 2
                # Opening point detection
                data['opening_percent_found'] = (payload[0] >> 7) & 0x01
                data['opening_percent_value'] = payload[0] & 0x7F
                
                var status_map = {
                    0: "Slow Harvesting switched OFF",
                    1: "No Slow Harvesting. Recent Target % not equal to 0",
                    2: "Hot Water expected to be OFF",
                    3: "Cannot detect opening Point",
                    4: "No Slow Harvesting. Battery High",
                    5: "No Slow Harvesting. Insufficient harvesting current",
                    6: "Detecting Opening Point",
                    7: "Slow Harvesting"
                }
                data['opd_status'] = status_map.find(payload[1] & 0x07, "Invalid Status")
                
            elif fport == 6 && size(payload) >= 1
                # Temperature drop detection
                var action = (payload[0] >> 6) & 0x03
                var action_text = "Invalid"
                if action == 0 action_text = "No Action"
                elif action == 2 action_text = "Close to 0% for 30 minutes"
                elif action == 3 action_text = "Close to 0% for 60 minutes"
                end
                data['tdd_action'] = action_text
                data['tdd_beep'] = ((payload[0] >> 5) & 0x01) == 1
                data['tdd_period'] = ((payload[0] >> 4) & 0x01) == 1 ? 2 : 1
                data['temp_drop_detected'] = (payload[0] & 0x01) == 1
                
            elif fport == 7 && size(payload) >= 7
                # PID parameters
                data['p_coefficient'] = payload[0]
                data['i_coefficient'] = payload[1] * 0.02
                data['d_coefficient'] = payload[2] * 0.2
                data['closed_percent'] = payload[4]
                data['d_coefficient_closed'] = payload[5] * 0.2
                data['offset_percent'] = payload[6]
                
            elif fport == 8 && size(payload) >= 1
                # Flow offset
                var offset = payload[0]
                if offset >= 128 offset = offset - 256 end
                data['flow_offset'] = offset * 0.25
                
            elif fport == 9 && size(payload) >= 2
                # External temperature sensor
                data['sensor_expiry_minutes'] = payload[0] == 0 ? "No Expiry" : (payload[0] * 5)
                data['temperature_source'] = payload[1] == 0 ? "Internal" : "External"
                
            elif fport == 15 && size(payload) >= 1
                # Device status
                var status = payload[0]
                data['device_on'] = ((status >> 7) & 0x01) == 1
                data['recalibration_status'] = ((status >> 6) & 0x01) == 1
                data['operating_status'] = (status & 0x01) == 0
                
            else
                print(f"MLR003: Unknown fport {fport} or invalid payload size")
                return nil
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store voltage trend
            if data.contains('storage_voltage')
                if !node_data.contains('voltage_history')
                    node_data['voltage_history'] = []
                end
                node_data['voltage_history'].push(data['storage_voltage'])
                if size(node_data['voltage_history']) > 10
                    node_data['voltage_history'].pop(0)
                end
            end
            
            # Register downlink commands
            if !global.contains("MLR003_cmdInit") || !global.MLR003_cmdInit
                self.register_downlink_commands()
                global.MLR003_cmdInit = true
            end

            # Save back to global storage
            global.MLR003_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"MLR003: Decode error - {e}: {m}")
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
            var node_data = global.MLR003_nodes.find(self.node, {})
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
            name = f"MLR003-{self.node}"
        end
        var name_tooltip = "Micropelt Thermostatic Radiator Valve"
        var battery = data_to_show.find('storage_voltage', 1000)  # Use 1000 to hide (no battery)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Show valve position if available
        if data_to_show.contains('valve_position')
            fmt.add_sensor("string", f"{data_to_show['valve_position']}%", "Valve Position", "🔧")
        end
        
        # Show temperatures
        if data_to_show.contains('ambient_temperature')
            fmt.add_sensor("temp", data_to_show['ambient_temperature'], "Room Temp", "🌡️")
        end
        
        if data_to_show.contains('flow_temperature')
            fmt.add_sensor("temp", data_to_show['flow_temperature'], "Flow Temp", "🌊")
        end
        
        # Show user mode and value
        if data_to_show.contains('user_mode')
            var mode = data_to_show['user_mode']
            var mode_emoji = "⚙️"
            if mode == "SP_Ambient_Temperature" mode_emoji = "🌡️"
            elif mode == "Valve_Position" mode_emoji = "🔧"
            elif mode == "Detecting_Opening_Point" mode_emoji = "🔍"
            elif mode == "Slow_Harvesting" mode_emoji = "🔋"
            end
            
            var value_str = ""
            if data_to_show.contains('user_value')
                var value = data_to_show['user_value']
                if mode == "SP_Ambient_Temperature"
                    value_str = f"{value}°C"
                else
                    value_str = f"{value}%"
                end
            end
            fmt.add_sensor("string", value_str, mode, mode_emoji)
        end
        
        # Show energy harvesting status
        if data_to_show.contains('harvesting_active')
            var harvest_emoji = data_to_show['harvesting_active'] ? "⚡" : "🔋"
            var harvest_text = data_to_show['harvesting_active'] ? "Active" : "Inactive"
            fmt.add_sensor("string", harvest_text, "Energy Harvesting", harvest_emoji)
        end
        
        # Show errors/warnings
        var has_errors = false
        if data_to_show.find('motor_error', 0) == 1
            fmt.next_line()
            fmt.add_status("Motor Error", "⚠️", "Motor malfunction detected")
            has_errors = true
        end
        
        if data_to_show.find('ambient_sensor_failure', 0) == 1 || data_to_show.find('flow_sensor_failure', 0) == 1
            if !has_errors fmt.next_line() end
            fmt.add_status("Sensor Error", "❌", "Sensor failure detected")
            has_errors = true
        end
        
        if data_to_show.find('calibration_ok', 1) == 0
            if !has_errors fmt.next_line() end
            fmt.add_status("Cal Error", "🔧", "Calibration required")
            has_errors = true
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                if !has_errors fmt.next_line() end
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
        var node_data = global.MLR003_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'voltage_history': node_data.find('voltage_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.MLR003_nodes.contains(node_id)
            global.MLR003_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Main configuration (Port 1)
        tasmota.remove_cmd("LwMLR003Config")
        tasmota.add_cmd("LwMLR003Config", def(cmd, idx, payload_str)
            # Format: LwMLR003Config<slot> <mode>,<setValue>,<roomTemp>,<safetyMode>,<safetyValue>,<interval>,<refRun>
            var parts = string.split(payload_str, ',')
            if size(parts) != 7
                return tasmota.resp_cmnd_str("Usage: mode,setValue,roomTemp,safetyMode,safetyValue,interval,refRun")
            end
            
            var mode = parts[0]  # "Ambient_Temperature" or "Valve_Position"
            var set_value = int(parts[1])
            var room_temp = int(parts[2])
            var safety_mode = parts[3]
            var safety_value = int(parts[4])
            var interval = int(parts[5])
            var ref_run = int(parts[6])
            
            # Validate inputs
            if mode != "Ambient_Temperature" && mode != "Valve_Position"
                return tasmota.resp_cmnd_str("Invalid mode: use Ambient_Temperature or Valve_Position")
            end
            
            if room_temp < 0 || room_temp > 40
                return tasmota.resp_cmnd_str("Room temperature range: 0-40°C")
            end
            
            # Build payload according to TTN encoder
            var hex_cmd = ""
            
            # Byte 1: Set value
            if mode == "Ambient_Temperature"
                if set_value < 0 || set_value > 40
                    return tasmota.resp_cmnd_str("Set value range for ambient: 0-40°C")
                end
                hex_cmd += f"{(set_value * 2):02X}"
            else
                if set_value < 0 || set_value > 100
                    return tasmota.resp_cmnd_str("Set value range for valve: 0-100%")
                end
                hex_cmd += f"{set_value:02X}"
            end
            
            # Byte 2: Room temperature
            hex_cmd += f"{(room_temp * 4):02X}"
            
            # Byte 3: Safety value
            if safety_mode == "Ambient_Temperature"
                if safety_value < 0 || safety_value > 40
                    return tasmota.resp_cmnd_str("Safety value range for ambient: 0-40°C")
                end
                hex_cmd += f"{(safety_value * 2):02X}"
            else
                if safety_value < 0 || safety_value > 100
                    return tasmota.resp_cmnd_str("Safety value range for valve: 0-100%")
                end
                hex_cmd += f"{safety_value:02X}"
            end
            
            # Byte 4: Radio interval and modes
            var radio_bits = 0
            if interval == 5 radio_bits = 1
            elif interval == 10 radio_bits = 0
            elif interval == 60 radio_bits = 2
            elif interval == 120 radio_bits = 3
            elif interval == 480 radio_bits = 4
            else return tasmota.resp_cmnd_str("Invalid interval: 5,10,60,120,480")
            end
            
            var user_bits = mode == "Ambient_Temperature" ? 2 : 0
            var safety_bits = safety_mode == "Ambient_Temperature" ? 0 : 2
            hex_cmd += f"{((radio_bits << 4) | (user_bits << 2) | safety_bits):02X}"
            
            # Byte 5: Reserved
            hex_cmd += "00"
            
            # Byte 6: Reference run
            if ref_run < 0 || ref_run > 1
                return tasmota.resp_cmnd_str("Reference run: 0 or 1")
            end
            hex_cmd += f"{(ref_run << 7):02X}"
            
            return lwdecode.SendDownlink(global.MLR003_nodes, cmd, idx, hex_cmd)
        end)
        
        # Motor range configuration (Port 3)
        tasmota.remove_cmd("LwMLR003MotorRange")
        tasmota.add_cmd("LwMLR003MotorRange", def(cmd, idx, payload_str)
            # Format: LwMLR003MotorRange<slot> <range_mm>
            var range_map = {
                "2.56048": "00", "0.624": "03", "0.832": "04", "1.040": "05",
                "1.248": "06", "1.456": "07", "1.664": "08", "1.872": "09",
                "2.080": "0A", "2.288": "0B", "2.496": "0C"
            }
            var hex_val = range_map.find(payload_str, nil)
            if hex_val == nil
                return tasmota.resp_cmnd_str("Invalid range. Valid: 0.624-2.56048mm")
            end
            return lwdecode.SendDownlink(global.MLR003_nodes, cmd, idx, hex_val)
        end)
        
        # Spreading factor (Port 4)
        tasmota.remove_cmd("LwMLR003SpreadFactor")
        tasmota.add_cmd("LwMLR003SpreadFactor", def(cmd, idx, payload_str)
            # Format: LwMLR003SpreadFactor<slot> <SF7|SF8>
            var hex_cmd = ""
            if payload_str == "SF7" hex_cmd = "00"
            elif payload_str == "SF8" hex_cmd = "01"
            else return tasmota.resp_cmnd_str("Invalid SF: use SF7 or SF8")
            end
            return lwdecode.SendDownlink(global.MLR003_nodes, cmd, idx, hex_cmd)
        end)
        
        # Room temperature (Port 10)
        tasmota.remove_cmd("LwMLR003RoomTemp")
        tasmota.add_cmd("LwMLR003RoomTemp", def(cmd, idx, payload_str)
            # Format: LwMLR003RoomTemp<slot> <temperature>
            var temp = int(payload_str)
            if temp < 0 || temp > 40
                return tasmota.resp_cmnd_str("Temperature range: 0-40°C")
            end
            var hex_cmd = f"{(temp * 4):02X}"
            return lwdecode.SendDownlink(global.MLR003_nodes, cmd, idx, hex_cmd)
        end)
        
        # Beep command (Port 11)
        tasmota.remove_cmd("LwMLR003Beep")
        tasmota.add_cmd("LwMLR003Beep", def(cmd, idx, payload_str)
            # Format: LwMLR003Beep<slot> <count>
            var beeps = int(payload_str)
            if beeps < 0 || beeps > 255
                return tasmota.resp_cmnd_str("Beep count range: 0-255")
            end
            var hex_cmd = f"{beeps:02X}"
            return lwdecode.SendDownlink(global.MLR003_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device control (Port 15)
        tasmota.remove_cmd("LwMLR003Control")
        tasmota.add_cmd("LwMLR003Control", def(cmd, idx, payload_str)
            # Format: LwMLR003Control<slot> <operate>,<recal>,<turnoff>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: <operate>,<recal>,<turnoff> (0/1 each)")
            end
            
            var operate = int(parts[0])
            var recal = int(parts[1])
            var turnoff = int(parts[2])
            
            if operate < 0 || operate > 1 || recal < 0 || recal > 1 || turnoff < 0 || turnoff > 1
                return tasmota.resp_cmnd_str("Values must be 0 or 1")
            end
            
            var hex_cmd = f"{((operate << 7) | (recal << 6) | turnoff):02X}"
            return lwdecode.SendDownlink(global.MLR003_nodes, cmd, idx, hex_cmd)
        end)
        
        print("MLR003: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_MLR003()

# Node management commands
tasmota.remove_cmd("LwMLR003NodeStats")
tasmota.add_cmd("LwMLR003NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwMLR003ClearNode")
tasmota.add_cmd("LwMLR003ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Test command with realistic scenarios
tasmota.remove_cmd("LwMLR003TestUI")
tasmota.add_cmd("LwMLR003TestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "normal":        "475E4E74572088542CED522E59",     # Normal operation
        "high_temp":     "475E5A74732088542CED522E59",     # High flow temperature
        "low_battery":   "475E4E74572088242CED522E59",     # Low storage voltage
        "motor_error":   "475E4E74572088543CED522E59",     # Motor error
        "sensor_fail":   "475E4E74572098542CED522E59",     # Sensor failure
        "valve_mode":    "645E4E74572088540CED522E59",     # Valve position mode
        "detecting":     "475E4E74572088541CED522E59",     # Detecting opening point
        "slow_harvest":  "475E4E74572088542CED522E59",     # Slow harvesting
        "version":       "C231180903",                     # Device version (port 2)
        "motor_range":   "09",                             # Motor range (port 3)
        "spread_factor": "01",                             # SF8 (port 4)
        "opd_status":    "0002"                            # Opening point detection (port 5)
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
    var fport = 1  # Default to main data
    
    # Override fport for specific scenarios
    if payload_str == "version" fport = 2
    elif payload_str == "motor_range" fport = 3
    elif payload_str == "spread_factor" fport = 4
    elif payload_str == "opd_status" fport = 5
    end

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
