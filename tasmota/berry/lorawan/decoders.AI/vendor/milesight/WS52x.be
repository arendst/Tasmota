#
# LoRaWAN AI-Generated Decoder for Milesight WS52x
#
# Generated: 2025-08-20 | Version: 1.4.0 | Revision: 0
#            by "LoRaWAN Decoder AI Generation Template", v2.2.6
#
# Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws52x
# Userguide: WS52x LoRaWAN Application Guide v1.1
# Decoder:   Official Milesight Decoder v1.0.3
# 
# v1.4.0 (2025-08-20): Framework v2.2.6 regeneration with enhanced error handling
# v1.3.0 (2025-08-19): Framework v2.2.1 debug version generation
# v1.2.0 (2025-08-17): Framework v2.1.13 regeneration
# v1.1.0 (2025-08-16): Framework v2.1.10 regeneration
# v1.0.0 (2025-08-15): Initial generation from PDF specification

class LwDecode_WS52x
    var hashCheck      # Duplicate payload detection flag (true = skip duplicates)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update

    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("WS52x_nodes")
            global.WS52x_nodes = {}
        end
        if !global.contains("WS52x_cmdInit")
            global.WS52x_cmdInit = false
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload, simulated)
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
            data['simulated'] = simulated
            
            # Retrieve node history from global storage
            var node_data = global.WS52x_nodes.find(node, {})
            
            # Decode based on fport 85 (standard WS52x data port)
            if fport == 85
                var i = 0
                while i < size(payload)
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # Power monitoring channels
                    if channel_id == 0x03 && channel_type == 0x74  # Voltage
                        var voltage = ((payload[i+1] << 8) | payload[i]) / 10.0
                        data['voltage'] = voltage
                        i += 2
                        
                    elif channel_id == 0x04 && channel_type == 0x80  # Active Power (signed)
                        var power = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        if power > 2147483647  # Convert to signed 32-bit
                            power = power - 4294967296
                        end
                        data['active_power'] = power
                        i += 4
                        
                    elif channel_id == 0x05 && channel_type == 0x81  # Power Factor
                        data['power_factor'] = payload[i]
                        i += 1
                        
                    elif channel_id == 0x06 && channel_type == 0x83  # Energy
                        var energy = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        data['energy_wh'] = energy
                        i += 4
                        
                    elif channel_id == 0x07 && channel_type == 0xC9  # Current
                        var current = (payload[i+1] << 8) | payload[i]
                        data['current'] = current
                        i += 2
                        
                    elif channel_id == 0x08 && channel_type == 0x70  # Socket State
                        data['socket_state'] = (payload[i] == 0x01) ? "ON" : "OFF"
                        data['socket_on'] = (payload[i] == 0x01)
                        i += 1
                        
                    # Device information channels
                    elif channel_id == 0xFF && channel_type == 0x01  # Protocol Version
                        data['protocol_version'] = payload[i]
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0x09  # Hardware Version
                        data['hw_version'] = f"{payload[i]}.{payload[i+1]}"
                        i += 2
                        
                    elif channel_id == 0xFF && channel_type == 0x0A  # Software Version
                        data['sw_version'] = f"{payload[i]}.{payload[i+1]}"
                        i += 2
                        
                    elif channel_id == 0xFF && channel_type == 0x0B  # Power On Event
                        data['power_on_event'] = true
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0x0F  # Device Class
                        var classes = ["Class A", "Class B", "Class C"]
                        data['device_class'] = payload[i] < size(classes) ? classes[payload[i]] : f"Unknown({payload[i]})"
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0x16  # Serial Number
                        var serial = ""
                        for j: 0..7
                            serial += f"{payload[i+j]:02X}"
                        end
                        data['serial_number'] = serial
                        i += 8
                        
                    elif channel_id == 0xFF && channel_type == 0x24  # OC Alarm Config
                        data['oc_alarm_enabled'] = (payload[i] == 0x01)
                        data['oc_alarm_threshold'] = payload[i+1]
                        i += 2
                        
                    elif channel_id == 0xFF && channel_type == 0x25  # Button Lock Config
                        var lock_value = (payload[i+1] << 8) | payload[i]
                        data['button_locked'] = (lock_value == 0x8000)
                        i += 2
                        
                    elif channel_id == 0xFF && channel_type == 0x26  # Power Recording Config
                        data['power_recording'] = (payload[i] == 0x01)
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0x2F  # LED Config
                        data['led_enabled'] = (payload[i] == 0x01)
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0x30  # OC Protection Config
                        data['oc_protection_enabled'] = (payload[i] == 0x01)
                        data['oc_protection_threshold'] = payload[i+1]
                        i += 2
                        
                    elif channel_id == 0xFF && channel_type == 0x3F  # Power Outage Event
                        data['power_outage_event'] = true
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0xFE  # Reset Event
                        var reset_types = ["POR", "BOR", "WDT", "CMD"]
                        data['device_reset'] = true
                        data['reset_type'] = payload[i] < size(reset_types) ? reset_types[payload[i]] : f"Unknown({payload[i]})"
                        i += 1
                        
                    elif channel_id == 0xFF && channel_type == 0xFF  # TSL Version
                        data['tsl_version'] = f"{payload[i]}.{payload[i+1]}"
                        i += 2
                        
                    # Configuration channels
                    elif channel_id == 0xFE && channel_type == 0x02  # Reporting Interval
                        var interval = (payload[i+1] << 8) | payload[i]
                        data['report_interval_min'] = interval
                        i += 2
                        
                    # Acknowledgment channels
                    elif channel_id == 0xFE && channel_type == 0x03  # Interval ACK
                        var ack_interval = (payload[i+1] << 8) | payload[i]
                        data['interval_ack'] = ack_interval
                        i += 2
                        
                    elif channel_id == 0xFE && channel_type == 0x10  # Reboot ACK
                        data['reboot_ack'] = true
                        i += 1
                        
                    elif channel_id == 0xFE && channel_type == 0x22  # Delay Task ACK
                        var task_seconds = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        data['delay_task_ack'] = task_seconds
                        i += 4
                        
                    elif channel_id == 0xFE && channel_type == 0x23  # Delete Task ACK
                        var task_number = (payload[i+1] << 8) | payload[i]
                        data['delete_task_ack'] = task_number
                        i += 2
                        
                    else
                        # Unknown channel - log and try to continue
                        print(f"WS52x: Unknown channel ID={channel_id:02X} Type={channel_type:02X}")
                        break
                    end
                end
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Track energy consumption history
            if data.contains('energy_wh')
                if !node_data.contains('energy_history')
                    node_data['energy_history'] = []
                end
                node_data['energy_history'].push(data['energy_wh'])
                if size(node_data['energy_history']) > 10
                    node_data['energy_history'].pop(0)
                end
            end
            
            # Track socket state changes
            if data.contains('socket_on')
                var prev_state = node_data.find('last_socket_state', nil)
                if prev_state != nil && prev_state != data['socket_on']
                    node_data['socket_changes'] = node_data.find('socket_changes', 0) + 1
                end
                node_data['last_socket_state'] = data['socket_on']
            end
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Initialize downlink commands once
            if !global.contains("WS52x_cmdInit") || !global.WS52x_cmdInit
                self.register_downlink_commands()
                global.WS52x_cmdInit = true
            end

            # Save back to global storage
            global.WS52x_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"WS52x: Decode error - {e}: {m}")
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
            var node_data = global.WS52x_nodes.find(self.node, {})
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
            name = f"WS52x-{self.node}"
        end
        var name_tooltip = "Milesight WS52x Smart Power Socket"
        var battery = 1000  # Use 1000 if no battery (mains powered)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Socket state with voltage
        var socket_state = data_to_show.find('socket_on')
        if socket_state != nil
            if socket_state
                fmt.add_status("ON", "🟢", "Socket is ON")
            else
                fmt.add_status("OFF", "🔴", "Socket is OFF")
            end
        end
        
        # Voltage
        if data_to_show.contains('voltage')
            fmt.add_sensor("volt", data_to_show['voltage'], "Voltage", "⚡")
        end
        
        # Current
        if data_to_show.contains('current')
            fmt.add_sensor("milliamp", data_to_show['current'], "Current", "🔌")
        end
        
        # Active Power
        if data_to_show.contains('active_power')
            fmt.add_sensor("power", data_to_show['active_power'], "Power", "💡")
        end
        
        # Power Factor
        if data_to_show.contains('power_factor')
            fmt.add_sensor("power_factor%", data_to_show['power_factor'], "Power Factor", "📊")
        end
        
        # Energy on next line if available
        if data_to_show.contains('energy_wh')
            fmt.next_line()
            fmt.add_sensor("energy", data_to_show['energy_wh'], "Energy", "🏠")
            
            # Show energy increase if we have history
            var node_data = global.WS52x_nodes.find(self.node, {})
            var energy_history = node_data.find('energy_history', [])
            if size(energy_history) > 1
                var energy_delta = energy_history[-1] - energy_history[-2]
                if energy_delta > 0
                    fmt.add_sensor("energy", energy_delta, "Delta", "📈")
                end
            end
        end
        
        # Device information if available
        if data_to_show.contains('sw_version')
            fmt.next_line()
            fmt.add_status(f"v{data_to_show['sw_version']}", "📟", "Software Version")
        end
        
        # Configuration status
        if data_to_show.contains('oc_protection_enabled') && data_to_show['oc_protection_enabled']
            fmt.add_status("OC Prot", "🛡️", f"Over-current protection: {data_to_show.find('oc_protection_threshold', 0)}A")
        end
        
        if data_to_show.contains('button_locked') && data_to_show['button_locked']
            fmt.add_status("Locked", "🔒", "Button locked")
        end
        
        # Events
        if data_to_show.contains('power_on_event') && data_to_show['power_on_event']
            fmt.next_line()
            fmt.add_status("Power On", "⚡", "Power-on event detected")
        end
        
        if data_to_show.contains('power_outage_event') && data_to_show['power_outage_event']
            fmt.add_status("Outage", "⚠️", "Power outage detected")
        end
        
        if data_to_show.contains('device_reset') && data_to_show['device_reset']
            fmt.add_status(f"Reset({data_to_show.find('reset_type', 'Unknown')})", "🔄", "Device reset detected")
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
        var node_data = global.WS52x_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'energy_history': node_data.find('energy_history', []),
            'socket_changes': node_data.find('socket_changes', 0),
            'last_socket_state': node_data.find('last_socket_state', nil),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.WS52x_nodes.contains(node_id)
            global.WS52x_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Socket Control (ON/OFF)
        tasmota.remove_cmd("LwWS52xControl")
        tasmota.add_cmd("LwWS52xControl", def(cmd, idx, payload_str)
            # Format: LwWS52xControl<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['08FF', 'ON' ],     # Maps "1" or "ON" to hex 08FF
                '0|OFF': ['0800', 'OFF']      # Maps "0" or "OFF" to hex 0800
            })
        end)
        
        # Set Reporting Interval
        tasmota.remove_cmd("LwWS52xInterval")
        tasmota.add_cmd("LwWS52xInterval", def(cmd, idx, payload_str)
            # Format: LwWS52xInterval<slot> <minutes>
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            
            # Build hex command (little endian)
            var hex_cmd = f"FE02{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Over-Current Alarm Configuration
        tasmota.remove_cmd("LwWS52xOCAlarm")
        tasmota.add_cmd("LwWS52xOCAlarm", def(cmd, idx, payload_str)
            # Format: LwWS52xOCAlarm<slot> <enabled>,<threshold>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwWS52xOCAlarm<slot> <enabled>,<threshold>")
            end
            
            var enabled = (parts[0] == "1" || string.toupper(parts[0]) == "ON") ? 1 : 0
            var threshold = int(parts[1])
            
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30A")
            end
            
            var hex_cmd = f"FF24{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Over-Current Protection Configuration
        tasmota.remove_cmd("LwWS52xOCProtection")
        tasmota.add_cmd("LwWS52xOCProtection", def(cmd, idx, payload_str)
            # Format: LwWS52xOCProtection<slot> <enabled>,<threshold>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwWS52xOCProtection<slot> <enabled>,<threshold>")
            end
            
            var enabled = (parts[0] == "1" || string.toupper(parts[0]) == "ON") ? 1 : 0
            var threshold = int(parts[1])
            
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30A")
            end
            
            var hex_cmd = f"FF30{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Button Lock Control
        tasmota.remove_cmd("LwWS52xButtonLock")
        tasmota.add_cmd("LwWS52xButtonLock", def(cmd, idx, payload_str)
            # Format: LwWS52xButtonLock<slot> <locked|unlocked|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|LOCKED':    ['FF250080', 'LOCKED'],     # Button locked
                '0|UNLOCKED':  ['FF250000', 'UNLOCKED']    # Button unlocked
            })
        end)
        
        # LED Mode Control
        tasmota.remove_cmd("LwWS52xLED")
        tasmota.add_cmd("LwWS52xLED", def(cmd, idx, payload_str)
            # Format: LwWS52xLED<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['FF2F01', 'ON' ],     # LED enabled
                '0|OFF': ['FF2F00', 'OFF']      # LED disabled
            })
        end)
        
        # Power Recording Control
        tasmota.remove_cmd("LwWS52xPowerRecording")
        tasmota.add_cmd("LwWS52xPowerRecording", def(cmd, idx, payload_str)
            # Format: LwWS52xPowerRecording<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['FF2601', 'ON' ],     # Power recording enabled
                '0|OFF': ['FF2600', 'OFF']      # Power recording disabled
            })
        end)
        
        # Reset Energy Counter
        tasmota.remove_cmd("LwWS52xResetEnergy")
        tasmota.add_cmd("LwWS52xResetEnergy", def(cmd, idx, payload_str)
            # Format: LwWS52xResetEnergy<slot>
            var hex_cmd = "FF2700"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Status Enquiry
        tasmota.remove_cmd("LwWS52xStatus")
        tasmota.add_cmd("LwWS52xStatus", def(cmd, idx, payload_str)
            # Format: LwWS52xStatus<slot>
            var hex_cmd = "FF2800"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device Reboot
        tasmota.remove_cmd("LwWS52xReboot")
        tasmota.add_cmd("LwWS52xReboot", def(cmd, idx, payload_str)
            # Format: LwWS52xReboot<slot>
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Delay Task
        tasmota.remove_cmd("LwWS52xDelayTask")
        tasmota.add_cmd("LwWS52xDelayTask", def(cmd, idx, payload_str)
            # Format: LwWS52xDelayTask<slot> <seconds>
            var seconds = int(payload_str)
            if seconds < 0 || seconds > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 0-4294967295 seconds")
            end
            
            var hex_cmd = f"FE22{lwdecode.uint32le(seconds)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Delete Task
        tasmota.remove_cmd("LwWS52xDeleteTask")
        tasmota.add_cmd("LwWS52xDeleteTask", def(cmd, idx, payload_str)
            # Format: LwWS52xDeleteTask<slot> <task_number>
            var task_number = int(payload_str)
            if task_number < 0 || task_number > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535")
            end
            
            var hex_cmd = f"FE23{lwdecode.uint16le(task_number)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS52x: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS52x()

# Node management commands
tasmota.remove_cmd("LwWS52xNodeStats")
tasmota.add_cmd("LwWS52xNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwWS52xClearNode")
tasmota.add_cmd("LwWS52xClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Command usage: LwWS52xTestUI<slot> <scenario>
tasmota.remove_cmd("LwWS52xTestUI")
tasmota.add_cmd("LwWS52xTestUI", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        "normal":    "037410F0048000000064058164068300127A8007C900FE08701",      # Normal operation: 240V, 100W, 100%, 4730Wh, 420mA, ON
        "low":       "0374005A048000000019058132068300012710070032080700",      # Low values: 90V, 25W, 50%, 4721Wh, 50mA, OFF
        "high":      "037413880480000003E8058164068300129F4C07C93E8080701",    # High values: 500V, 1000W, 100%, 5MB0Wh, 1000mA, ON
        "alert":     "0374108C048000000320058150068300127A8007C902BC080701FF3F01", # Alert: 420V, 800W, 80%, 4730Wh, 700mA, ON, power outage
        "config":    "FF0A0103FF2401050AFF2F01FF2600FE020A00",                  # Config: SW v1.3, OC alarm 5A, LED on, power rec off, 10min interval
        "info":      "FF0101FF09010AFF0A0103FF0F00FF16123456789ABCDEF0"         # Device info: Protocol v1, HW v1.10, SW v1.3, Class A, Serial
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
    var fport = 85

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
