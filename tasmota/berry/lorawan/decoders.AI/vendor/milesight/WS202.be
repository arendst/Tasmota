# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight WS202
#
# Generated: 2025-08-15 | Version: 1.1.0 | Revision: 2
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws202
# - Userguide: https://resource.milesight-iot.com/milesight/iot/document/ws202-user-guide.pdf
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# v1.1.0 (2025-08-15): Regenerated with AI template v2.1.8
#                      - Enhanced framework integration
#                      - Added formatter expansions
#                      - Improved global storage handling
#                      - Added comprehensive downlink commands
# v1.0.0 (2025-08-14): Initial generation from PDF specification
# -------------------------------------------------------------

class LwDecode_WS202
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
        if !global.contains("WS202_nodes")
            global.WS202_nodes = {}
        end
        if !global.contains("WS202_cmdInit")
            global.WS202_cmdInit = false
        end
        
        # Expand framework with motion sensor formatters
        LwSensorFormatter_cls.Formatter["illuminance"] = {"u": "lux", "f": " %d", "i": "💡"}
        LwSensorFormatter_cls.Formatter["motion"] = {"u": "", "f": "", "i": "🚶"}
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
            var node_data = global.WS202_nodes.find(node, {})
            
            # Decode based on fport 85 (WS202 uses single port)
            if fport == 85
                var i = 0
                while i < size(payload)
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # PIR Motion (0x00, 0x00) - 1 byte
                    if channel_id == 0x00 && channel_type == 0x00 && i < size(payload)
                        data['pir_motion'] = payload[i] == 0x01
                        data['motion_text'] = payload[i] == 0x01 ? "MOTION" : "NO MOTION"
                        i += 1
                        
                    # Light Sensor (0x01, 0x65) - 2 bytes
                    elif channel_id == 0x01 && channel_type == 0x65 && i + 1 < size(payload)
                        var illuminance = (payload[i+1] << 8) | payload[i]
                        data['illuminance'] = illuminance
                        data['light_text'] = illuminance > 20 ? "BRIGHT" : "DARK"
                        i += 2
                        
                    # Battery (0x01, 0x75) - 1 byte
                    elif channel_id == 0x01 && channel_type == 0x75 && i < size(payload)
                        data['battery_pct'] = payload[i]
                        data['battery_v'] = 2.0 + (payload[i] / 100.0) * 1.2  # Estimate voltage
                        i += 1
                        
                    # Device information channels (0xFF)
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
                        elif channel_type == 0x22 && i + 1 < size(payload)  # PIR Sensitivity
                            data['pir_sensitivity'] = (payload[i+1] << 8) | payload[i]
                            i += 2
                        elif channel_type == 0x23 && i + 1 < size(payload)  # PIR Retry
                            data['pir_retry'] = (payload[i+1] << 8) | payload[i]
                            i += 2
                        elif channel_type == 0x2C && i + 1 < size(payload)  # Light Threshold
                            data['light_threshold'] = (payload[i+1] << 8) | payload[i]
                            i += 2
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
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
            end
            
            # Register downlink commands if not already done
            if !global.contains("WS202_cmdInit") || !global.WS202_cmdInit
                self.register_downlink_commands()
                global.WS202_cmdInit = true
            end

            # Save back to global storage
            global.WS202_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"LwDecode_WS202 error: {m}")
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
            var node_data = global.WS202_nodes.find(self.node, {})
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
            name = f"WS202-{self.node}"
        end
        var name_tooltip = "Milesight WS202 PIR & Light Sensor"
        var battery = data_to_show.find('battery_v', 1000)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        
        msg = msg + lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build compact motion and light display
        fmt.start_line()
        
        # Motion status with dynamic emoji
        if data_to_show.contains('pir_motion')
            var motion_emoji = data_to_show['pir_motion'] ? "🚶" : "🚫"
            var motion_text = data_to_show.find('motion_text', "UNKNOWN")
            fmt.add_sensor("string", motion_text, "Motion Detection", motion_emoji)
        end
        
        # Light level
        if data_to_show.contains('illuminance')
            fmt.add_sensor("illuminance", data_to_show['illuminance'], "Light Level", nil)
        end
        
        # Light status with emoji (if light level available)
        if data_to_show.contains('light_text')
            var light_emoji = data_to_show['illuminance'] > 20 ? "🌞" : "🌕"
            fmt.add_sensor("string", data_to_show['light_text'], "Light Status", light_emoji)
        end
        
        fmt.end_line()
        var sensor_msg = fmt.get_msg()
        if sensor_msg != nil
            msg = msg + sensor_msg
        end
        
        # Add alert line for reset events
        if data_to_show.find('device_reset', false)
            fmt.start_line()
            var reset_text = data_to_show.find('reset_type', 'Reset')
            fmt.add_status(reset_text, "🔄", "Device Reset")
            fmt.end_line()
            var alert_msg = fmt.get_msg()
            if alert_msg != nil
                msg = msg + alert_msg
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
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.WS202_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'battery_history': node_data.find('battery_history', []),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.WS202_nodes.contains(node_id)
            global.WS202_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Reporting Interval
        tasmota.remove_cmd("LwWS202Interval")
        tasmota.add_cmd("LwWS202Interval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            var hex_cmd = f"FE02{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        # PIR Sensitivity Configuration
        tasmota.remove_cmd("LwWS202PIRSensitivity")
        tasmota.add_cmd("LwWS202PIRSensitivity", def(cmd, idx, payload_str)
            var sensitivity = int(payload_str)
            if sensitivity < 1 || sensitivity > 10
                return tasmota.resp_cmnd_str("Invalid: range 1-10")
            end
            var hex_cmd = f"FF22{lwdecode.uint16le(sensitivity)}"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        # PIR Retry Configuration
        tasmota.remove_cmd("LwWS202PIRRetry")
        tasmota.add_cmd("LwWS202PIRRetry", def(cmd, idx, payload_str)
            var retry = int(payload_str)
            if retry < 1 || retry > 255
                return tasmota.resp_cmnd_str("Invalid: range 1-255")
            end
            var hex_cmd = f"FF23{lwdecode.uint16le(retry)}"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        # Light Threshold Configuration
        tasmota.remove_cmd("LwWS202LightThreshold")
        tasmota.add_cmd("LwWS202LightThreshold", def(cmd, idx, payload_str)
            var threshold = int(payload_str)
            if threshold < 0 || threshold > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535 lux")
            end
            var hex_cmd = f"FF2C{lwdecode.uint16le(threshold)}"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device Reboot
        tasmota.remove_cmd("LwWS202Reboot")
        tasmota.add_cmd("LwWS202Reboot", def(cmd, idx, payload_str)
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS202_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS202: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS202()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwWS202TestPayload")
tasmota.add_cmd("LwWS202TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestWS202", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("LwWS202NodeStats")
tasmota.add_cmd("LwWS202NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwWS202ClearNode")
tasmota.add_cmd("LwWS202ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
