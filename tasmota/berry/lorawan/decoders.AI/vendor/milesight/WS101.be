#
# LoRaWAN AI-Generated Decoder for Milesight WS101 Prompted by Human 
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 0
#            by \"LoRaWAN Decoder AI Generation Template\", v2.2.8
#
# Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws101
# Userguide: https://resource.milesight.com/milesight/iot/document/ws101-user-guide-en.pdf
# Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# 
# v1.0.0 (2025-08-20): Initial generation from PDF specification

class LwDecode_WS101
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
        if !global.contains(\"WS101_nodes\")
            global.WS101_nodes = {}
        end
        if !global.contains(\"WS101_cmdInit\")
            global.WS101_cmdInit = false
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
            var node_data = global.WS101_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 85
                var i = 0
                while i < size(payload)
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    if channel_id == 0xFF && channel_type == 0x2E  # Button Message
                        if i < size(payload)
                            var button_type = payload[i]
                            if button_type == 0x01
                                data['button_event'] = \"Short Press\"
                                data['button_mode'] = 1
                            elif button_type == 0x02
                                data['button_event'] = \"Long Press\"
                                data['button_mode'] = 2
                            elif button_type == 0x03
                                data['button_event'] = \"Double Press\"
                                data['button_mode'] = 3
                            else
                                data['button_event'] = f\"Unknown ({button_type:02X})\"
                                data['button_mode'] = button_type
                            end
                            i += 1
                        end
                        
                    elif channel_id == 0x01 && channel_type == 0x75  # Battery Level
                        if i < size(payload)
                            data['battery_pct'] = payload[i]
                            # Convert to voltage approximation for compatibility
                            data['battery_v'] = 2.4 + (payload[i] / 100.0) * 1.2
                            i += 1
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x01  # Protocol Version
                        if i < size(payload)
                            data['protocol_version'] = f\"V{payload[i]}\"
                            i += 1
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x08  # Device Serial Number
                        if i + 5 < size(payload)
                            var sn = \"\"
                            for j: 0..5
                                sn += f\"{payload[i+j]:02X}\"
                            end
                            data['serial_number'] = sn
                            i += 6
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x09  # Hardware Version
                        if i + 1 < size(payload)
                            var major = payload[i+1]
                            var minor = payload[i]
                            data['hw_version'] = f\"V{major}.{minor}\"
                            i += 2
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x0A  # Software Version
                        if i + 1 < size(payload)
                            var major = payload[i+1]
                            var minor = payload[i]
                            data['sw_version'] = f\"V{major}.{minor}\"
                            i += 2
                        end
                        
                    elif channel_id == 0xFF && channel_type == 0x0B  # Power On Event
                        data['power_on'] = true
                        data['device_reset'] = true
                        # No additional data for power on
                        
                    elif channel_id == 0xFF && channel_type == 0x0F  # Device Class
                        if i < size(payload)
                            var class_val = payload[i]
                            if class_val == 0x00
                                data['device_class'] = \"Class A\"
                            elif class_val == 0x01
                                data['device_class'] = \"Class B\"
                            elif class_val == 0x02
                                data['device_class'] = \"Class C\"
                            else
                                data['device_class'] = f\"Unknown ({class_val:02X})\"
                            end
                            i += 1
                        end
                        
                    else
                        # Unknown channel, try to skip
                        print(f\"WS101: Unknown channel: ID={channel_id:02X} Type={channel_type:02X}\")
                        break
                    end
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
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Store button press count
            if data.contains('button_event')
                node_data['button_count'] = node_data.find('button_count', 0) + 1
                node_data['last_button'] = tasmota.rtc()['local']
            end
            
            # Implement downlinks if present and create relative tasmota commands
            if !global.contains(\"WS101_cmdInit\") || !global.WS101_cmdInit
                self.register_downlink_commands()
                global.WS101_cmdInit = true
            end

            # Save back to global storage
            global.WS101_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f\"WS101: Decode error - {e}: {m}\")
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
            var node_data = global.WS101_nodes.find(self.node, {})
            data_to_show = node_data.find('last_data', {})
            last_update = node_data.find('last_update', 0)
        end
        
        if size(data_to_show) == 0 return nil end
        
        import string
        var msg = \"\"
        var fmt = LwSensorFormatter_cls()
        
        # MANDATORY: Add header line with device info
        var name = self.name
        if name == nil || name == \"\"
            name = f\"WS101-{self.node}\"
        end
        var name_tooltip = \"Milesight WS101 Smart Button\"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        var simulated = data_to_show.find('simulated', false) # Simulated payload indicator
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        
        fmt.start_line()
        
        # Display button event prominently
        if data_to_show.contains('button_event')
            var button_emoji = \"🔵\"
            if data_to_show['button_mode'] == 1
                button_emoji = \"🟢\"  # Short press - green
            elif data_to_show['button_mode'] == 2
                button_emoji = \"🔴\"  # Long press - red  
            elif data_to_show['button_mode'] == 3
                button_emoji = \"🟡\"  # Double press - yellow
            end
            fmt.add_sensor(\"string\", data_to_show['button_event'], \"Button Action\", button_emoji)
        end
        
        # Show battery percentage if available
        if data_to_show.contains('battery_pct')
            fmt.add_sensor(\"string\", f\"{data_to_show['battery_pct']}%\", \"Battery Level\", \"🔋\")
        end
        
        # Show device info compactly
        if data_to_show.contains('sw_version')
            fmt.add_sensor(\"string\", data_to_show['sw_version'], \"Software Version\", \"📊\")
        end
        
        # Power on indication
        if data_to_show.contains('power_on') && data_to_show['power_on']
            fmt.next_line()
            fmt.add_status(\"Power On\", \"⚡\", \"Device powered on or reset\")
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.next_line()
                fmt.add_status(self.format_age(age), \"⏱️\", nil)
            end
        end
        
        fmt.end_line()
        
        # ONLY get_msg() return a string that can be used with +=
        msg += fmt.get_msg()

        return msg
    end
    
    def format_age(seconds)
        if seconds < 60 return f\"{seconds}s ago\"
        elif seconds < 3600 return f\"{seconds/60}m ago\"
        elif seconds < 86400 return f\"{seconds/3600}h ago\"
        else return f\"{seconds/86400}d ago\"
        end
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.WS101_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'button_count': node_data.find('button_count', 0),
            'last_button': node_data.find('last_button', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.WS101_nodes.contains(node_id)
            global.WS101_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Reporting Interval Command
        tasmota.remove_cmd(\"LwWS101Interval\")
        tasmota.add_cmd(\"LwWS101Interval\", def(cmd, idx, payload_str)
            # Format: LwWS101Interval<slot> <seconds>
            var seconds = int(payload_str)
            if seconds < 60 || seconds > 64800
                return tasmota.resp_cmnd_str(\"Invalid: range 60-64800 seconds\")
            end
            
            # Build hex command per PDF specification (little endian)
            var hex_cmd = f\"FF03{lwdecode.uint16le(seconds)}\"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd, f\"{seconds}s\")
        end)
        
        # Reboot Device Command
        tasmota.remove_cmd(\"LwWS101Reboot\")
        tasmota.add_cmd(\"LwWS101Reboot\", def(cmd, idx, payload_str)
            # Format: LwWS101Reboot<slot>
            var hex_cmd = \"FF10FF\"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd, \"Reboot\")
        end)
        
        # LED Indicator Control
        tasmota.remove_cmd(\"LwWS101LED\")
        tasmota.add_cmd(\"LwWS101LED\", def(cmd, idx, payload_str)
            # Format: LwWS101LED<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS101_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['FF2F01', 'LED ON'],
                '0|OFF|DISABLE': ['FF2F00', 'LED OFF']
            })
        end)
        
        # Double Press Mode Control
        tasmota.remove_cmd(\"LwWS101DoublePress\")
        tasmota.add_cmd(\"LwWS101DoublePress\", def(cmd, idx, payload_str)
            # Format: LwWS101DoublePress<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS101_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['FF7401', 'Double Press ON'],
                '0|OFF|DISABLE': ['FF7400', 'Double Press OFF']
            })
        end)
        
        # Buzzer Control
        tasmota.remove_cmd(\"LwWS101Buzzer\")
        tasmota.add_cmd(\"LwWS101Buzzer\", def(cmd, idx, payload_str)
            # Format: LwWS101Buzzer<slot> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS101_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['FF3E01', 'Buzzer ON'],
                '0|OFF|DISABLE': ['FF3E00', 'Buzzer OFF']
            })
        end)
        
        print(\"WS101: Downlink commands registered\")
    end
end

# Global instance
LwDeco = LwDecode_WS101()

# Node management commands
tasmota.remove_cmd(\"LwWS101NodeStats\")
tasmota.add_cmd(\"LwWS101NodeStats\", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str(\"Node not found\")
    end
end)

tasmota.remove_cmd(\"LwWS101ClearNode\")
tasmota.add_cmd(\"LwWS101ClearNode\", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str(\"Node not found\")
    end
end)

# Command usage: LwWS101TestUI<slot> <scenario>
tasmota.remove_cmd(\"LwWS101TestUI\")
tasmota.add_cmd(\"LwWS101TestUI\", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        \"normal\":    \"017564FF2E01\",                    # Normal: 100% battery + short press
        \"low\":       \"017528FF2E02\",                    # Low: 40% battery + long press
        \"high\":      \"017564FF2E03\",                    # High: 100% battery + double press
        \"alert\":     \"017514FF2E02\",                    # Alert: 20% battery + long press (emergency)
        \"config\":    \"FF010AFF080112345678FF090100FF0A0102FF0F00\", # Config: device info
        \"info\":      \"017564FF2E01FF010AFF080112345678\"  # Info: battery + button + device info
    }
    
    var hex_payload = test_scenarios.find(payload_str ? payload_str : 'nil', 'not_found')
    
    if hex_payload == 'not_found'
      var scenarios_list = \"\"
      for key: test_scenarios.keys()
        scenarios_list += key + \" \"
      end
      return tasmota.resp_cmnd_str(format(\"Available scenarios: %s\", scenarios_list))
    end
    
    var rssi = -75
    var fport = 85

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
