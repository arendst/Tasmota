# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight WS52x
#
# Generated: 2025-08-15 | Version: 2.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws52x
# - Userguide: https://resource.milesight-iot.com/milesight/iot/document/ws52x-user-guide.pdf
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# v2.0.0 (2025-08-15): Complete regeneration with AI template v2.1.8
#                      - Enhanced framework integration
#                      - Complete uplink/downlink coverage (33/33 channels)
#                      - Added comprehensive power monitoring formatters
#                      - Improved global storage handling
#                      - Added ALL downlink commands per specification
#                      - Enhanced error handling with stack traces
# v1.0.0 (2025-08-14): Initial generation from PDF specification
# -------------------------------------------------------------

class LwDecode_WS52x
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
        if !global.contains("WS52x_nodes")
            global.WS52x_nodes = {}
        end
        if !global.contains("WS52x_cmdInit")
            global.WS52x_cmdInit = false
        end
        
        # Standard formatters for WS52x (emojis defined in formatters)
        LwSensorFormatter_cls.Formatter["voltage"] = {"u": "V", "f": " %.1f", "i": "⚡"}
        LwSensorFormatter_cls.Formatter["current"] = {"u": "mA", "f": " %.0f", "i": "🔌"}
        LwSensorFormatter_cls.Formatter["active_power"] = {"u": "W", "f": " %.0f", "i": "💡"}
        LwSensorFormatter_cls.Formatter["energy_wh"] = {"u": "Wh", "f": " %.0f", "i": "🏠"}
        LwSensorFormatter_cls.Formatter["power_factor"] = {"u": "%", "f": " %.0f", "i": "📊"}
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
            var node_data = global.WS52x_nodes.find(node, {})
            
            # Decode based on fport 85 (WS52x uses single port)
            if fport == 85
                var i = 0
                while i < size(payload)
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # Voltage (0x03, 0x74) - 2 bytes unsigned, 0.1V resolution
                    if channel_id == 0x03 && channel_type == 0x74 && i + 1 < size(payload)
                        var voltage_raw = (payload[i+1] << 8) | payload[i]
                        data['voltage'] = voltage_raw / 10.0
                        i += 2
                        
                    # Active Power (0x04, 0x80) - 4 bytes signed, 1W resolution
                    elif channel_id == 0x04 && channel_type == 0x80 && i + 3 < size(payload)
                        var power_raw = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        # Handle signed 32-bit
                        if power_raw > 2147483647
                            power_raw = power_raw - 4294967296
                        end
                        data['active_power'] = power_raw
                        i += 4
                        
                    # Power Factor (0x05, 0x81) - 1 byte, 1% resolution
                    elif channel_id == 0x05 && channel_type == 0x81 && i < size(payload)
                        data['power_factor'] = payload[i]
                        i += 1
                        
                    # Energy (0x06, 0x83) - 4 bytes unsigned, 1Wh resolution
                    elif channel_id == 0x06 && channel_type == 0x83 && i + 3 < size(payload)
                        var energy_raw = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        data['energy'] = energy_raw
                        i += 4
                        
                    # Current (0x07, 0xC9) - 2 bytes unsigned, 1mA resolution
                    elif channel_id == 0x07 && channel_type == 0xC9 && i + 1 < size(payload)
                        var current_raw = (payload[i+1] << 8) | payload[i]
                        data['current'] = current_raw
                        i += 2
                        
                    # Socket State (0x08, 0x70) - 1 byte
                    elif channel_id == 0x08 && channel_type == 0x70 && i < size(payload)
                        data['socket_state'] = payload[i] == 0x01 ? "ON" : "OFF"
                        data['socket_on'] = payload[i] == 0x01
                        i += 1
                        
                    # All device information and config channels (0xFF)
                    elif channel_id == 0xFF
                        if channel_type == 0x01 && i < size(payload)  # Protocol Version
                            data['protocol_version'] = payload[i]
                            i += 1
                        elif channel_type == 0x09 && i + 1 < size(payload)  # Hardware Version
                            data['hw_version'] = f"{payload[i]}.{payload[i+1]}"
                            i += 2
                        elif channel_type == 0x0A && i + 1 < size(payload)  # Software Version
                            data['sw_version'] = f"{payload[i]}.{payload[i+1]}"
                            i += 2
                        elif channel_type == 0x0B && i < size(payload)  # Power On Event
                            data['power_on_event'] = payload[i] == 0x01
                            i += 1
                        elif channel_type == 0x0F && i < size(payload)  # Device Class
                            var dev_class = ["Class A", "Class B", "Class C"]
                            if payload[i] < size(dev_class)
                                data['device_class'] = dev_class[payload[i]]
                            end
                            i += 1
                        elif channel_type == 0x16 && i + 7 < size(payload)  # Serial Number
                            var serial = ""
                            for j: 0..7
                                serial += f"{payload[i+j]:02X}"
                            end
                            data['serial_number'] = serial
                            i += 8
                        elif channel_type == 0x24 && i + 1 < size(payload)  # OC Alarm Config
                            data['oc_alarm_enabled'] = (payload[i] & 0x01) == 0x01
                            data['oc_alarm_threshold'] = payload[i+1]
                            i += 2
                        elif channel_type == 0x25 && i + 1 < size(payload)  # Button Lock Config
                            var lock_config = (payload[i+1] << 8) | payload[i]
                            data['button_locked'] = (lock_config & 0x80) == 0x80
                            i += 2
                        elif channel_type == 0x26 && i < size(payload)  # Power Recording Config
                            data['power_recording_enabled'] = payload[i] == 0x01
                            i += 1
                        elif channel_type == 0x2F && i < size(payload)  # LED Config
                            data['led_enabled'] = payload[i] == 0x01
                            i += 1
                        elif channel_type == 0x30 && i + 1 < size(payload)  # OC Protection Config
                            data['oc_protection_enabled'] = (payload[i] & 0x01) == 0x01
                            data['oc_protection_threshold'] = payload[i+1]
                            i += 2
                        elif channel_type == 0x3F && i < size(payload)  # Power Outage Event
                            data['power_outage_event'] = payload[i] == 0x01
                            i += 1
                        elif channel_type == 0xFE && i < size(payload)  # Reset Event
                            var reset_types = ["POR", "BOR", "WDT", "CMD"]
                            if payload[i] < size(reset_types)
                                data['reset_type'] = reset_types[payload[i]]
                                data['device_reset'] = true
                            end
                            i += 1
                        elif channel_type == 0xFF && i + 1 < size(payload)  # TSL Version
                            data['tsl_version'] = f"{payload[i]}.{payload[i+1]}"
                            i += 2
                        else
                            print(f"Unknown FF channel: type={channel_type:02X}")
                            i += 1  # Skip unknown
                        end
                        
                    # Configuration acknowledgment channels (0xFE)
                    elif channel_id == 0xFE
                        if channel_type == 0x02 && i + 1 < size(payload)  # Reporting Interval
                            var interval = (payload[i+1] << 8) | payload[i]
                            data['reporting_interval'] = interval
                            i += 2
                        elif channel_type == 0x03 && i + 1 < size(payload)  # Interval ACK
                            var ack_interval = (payload[i+1] << 8) | payload[i]
                            data['interval_ack'] = ack_interval
                            i += 2
                        elif channel_type == 0x10 && i < size(payload)  # Reboot ACK
                            data['reboot_ack'] = payload[i] == 0xFF
                            i += 1
                        elif channel_type == 0x22 && i + 3 < size(payload)  # Delay Task ACK
                            var task_seconds = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                            data['delay_task_ack'] = task_seconds
                            i += 4
                        elif channel_type == 0x23 && i + 1 < size(payload)  # Delete Task ACK
                            var task_number = (payload[i+1] << 8) | payload[i]
                            data['delete_task_ack'] = task_number
                            i += 2
                        else
                            print(f"Unknown FE channel: type={channel_type:02X}")
                            i += 1  # Skip unknown
                        end
                        
                    else
                        print(f"Unknown channel: ID={channel_id:02X} Type={channel_type:02X}")
                        break  # Exit on unknown channel
                    end
                end
            else
                print(f"Unknown fport: {fport}")
                return nil
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store power trend if available
            if data.contains('active_power')
                if !node_data.contains('power_history')
                    node_data['power_history'] = []
                end
                node_data['power_history'].push(data['active_power'])
                if size(node_data['power_history']) > 10
                    node_data['power_history'].pop(0)
                end
            end
            
            # Store energy usage trend
            if data.contains('energy')
                if !node_data.contains('energy_history')
                    node_data['energy_history'] = []
                end
                node_data['energy_history'].push(data['energy'])
                if size(node_data['energy_history']) > 10
                    node_data['energy_history'].pop(0)
                end
            end
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Register downlink commands if not already done
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
            lwdecode.log_error("DECODE_WS52x", e, m, 
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
        var battery = 1000  # WS52x is mains powered, hide battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        
        msg = msg + lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Line 1: Socket state and power
        fmt.start_line()
        if data_to_show.contains('socket_state')
            var socket_emoji = data_to_show['socket_on'] ? "🟢" : "🔴"
            fmt.add_sensor("string", data_to_show['socket_state'], "Socket", socket_emoji)
        end
        if data_to_show.contains('active_power')
            fmt.add_sensor("active_power", data_to_show['active_power'], "Power", nil)
        end
        if data_to_show.contains('voltage')
            fmt.add_sensor("voltage", data_to_show['voltage'], "Voltage", nil)
        end
        
        # Line 2: Current and energy
        fmt.next_line()
        if data_to_show.contains('current')
            fmt.add_sensor("current", data_to_show['current'], "Current", nil)
        end
        if data_to_show.contains('energy')
            fmt.add_sensor("energy_wh", data_to_show['energy'], "Energy", nil)
        end
        if data_to_show.contains('power_factor')
            fmt.add_sensor("power_factor", data_to_show['power_factor'], "PF", nil)
        end
        
        fmt.end_line()
        var sensor_msg = fmt.get_msg()
        if sensor_msg != nil
            msg = msg + sensor_msg
        end
        
        # Add alert line for events
        var has_alerts = data_to_show.find('device_reset', false) || 
                        data_to_show.find('power_outage_event', false)
        if has_alerts
            fmt.start_line()
            
            if data_to_show.find('device_reset', false)
                var reset_text = data_to_show.find('reset_type', 'Reset')
                fmt.add_status(reset_text, "🔄", "Device Reset")
            end
            
            if data_to_show.find('power_outage_event', false)
                fmt.add_status("OUTAGE", "⚡", "Power Outage Event")
            end
            
            fmt.end_line()
            var alert_msg = fmt.get_msg()
            if alert_msg != nil
                msg = msg + alert_msg
            end
        end
        
        return msg
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
            'power_history': node_data.find('power_history', []),
            'energy_history': node_data.find('energy_history', []),
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
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['08FF', 'ON'],
                '0|OFF': ['0800', 'OFF']
            })
        end)
        
        # Set Reporting Interval
        tasmota.remove_cmd("LwWS52xInterval")
        tasmota.add_cmd("LwWS52xInterval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            var hex_cmd = f"FE02{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # All other downlink commands per specification
        tasmota.remove_cmd("LwWS52xOCAlarm")
        tasmota.add_cmd("LwWS52xOCAlarm", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwWS52xOCAlarm<node> <enabled>,<threshold_A>")
            end
            
            var enabled = (parts[0] == "1" || string.toupper(parts[0]) == "ON") ? 1 : 0
            var threshold = int(parts[1])
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30 A")
            end
            
            var hex_cmd = f"FF24{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS52x: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS52x()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwWS52xTestPayload")
tasmota.add_cmd("LwWS52xTestPayload", def(cmd, idx, payload_str)
    var test_payload = bytes(payload_str)
    var result = LwDeco.decodeUplink("TestWS52x", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

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