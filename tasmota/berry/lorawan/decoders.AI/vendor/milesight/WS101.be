# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight WS101
#
# Generated: 2025-08-16 | Version: 1.1.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.9
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws101
# - Userguide: https://resource.milesight-iot.com/milesight/iot/document/ws101-user-guide.pdf
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# v1.1.0 (2025-08-16): Regenerated from MAP file with template v2.1.9
#                      - Enhanced framework integration
#                      - Added Lw command prefix requirement
#                      - Improved global storage handling
#                      - Added comprehensive button press tracking
#                      - Enhanced error handling
# v1.0.0 (2025-08-15): Initial generation from specifications
# -------------------------------------------------------------

class LwDecode_WS101
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
        if !global.contains("WS101_nodes")
            global.WS101_nodes = {}
        end
        if !global.contains("WS101_cmdInit")
            global.WS101_cmdInit = false
        end
        
        # Expand framework with button control formatters
        LwSensorFormatter_cls.Formatter["button"] = {"u": "", "f": "", "i": "🔘"}
        LwSensorFormatter_cls.Formatter["press_type"] = {"u": "", "f": "", "i": "👆"}
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
            var node_data = global.WS101_nodes.find(node, {})
            
            # Decode based on fport 85 (WS101 uses single port)
            if fport == 85
                var i = 0
                while i < size(payload)
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # Button Press Type (0x01, 0x00) - 1 byte
                    if channel_id == 0x01 && channel_type == 0x00 && i < size(payload)
                        var press_type = payload[i]
                        data['button_pressed'] = true
                        if press_type == 0x01
                            data['press_type'] = "SINGLE"
                        elif press_type == 0x02
                            data['press_type'] = "DOUBLE"
                        elif press_type == 0x03
                            data['press_type'] = "LONG"
                        else
                            data['press_type'] = f"UNKNOWN({press_type})"
                        end
                        data['press_value'] = press_type
                        i += 1
                        
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
            
            # Track button press history
            if data.contains('button_pressed') && data['button_pressed']
                if !node_data.contains('button_history')
                    node_data['button_history'] = []
                end
                var press_event = {
                    'type': data.find('press_type', 'UNKNOWN'),
                    'timestamp': tasmota.rtc()['local'],
                    'value': data.find('press_value', 0)
                }
                node_data['button_history'].push(press_event)
                # Keep last 20 button presses
                if size(node_data['button_history']) > 20
                    node_data['button_history'].pop(0)
                end
                # Update press counters
                var counter_key = f"press_{data.find('press_type', 'unknown').tolower()}_count"
                node_data[counter_key] = node_data.find(counter_key, 0) + 1
            end
            
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
            if !global.contains("WS101_cmdInit") || !global.WS101_cmdInit
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
            lwdecode.log_error("DECODE_WS101", e, m, 
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
            var node_data = global.WS101_nodes.find(self.node, {})
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
            name = f"WS101-{self.node}"
        end
        var name_tooltip = "Milesight WS101 Smart Button"
        var battery = data_to_show.find('battery_v', 1000)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        
        msg = msg + lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build button event display
        fmt.start_line()
        
        # Button press status with dynamic display
        if data_to_show.contains('button_pressed') && data_to_show['button_pressed']
            var press_type = data_to_show.find('press_type', 'UNKNOWN')
            var press_emoji = "🔘"
            if press_type == "SINGLE"
                press_emoji = "👆"
            elif press_type == "DOUBLE"
                press_emoji = "👆👆"
            elif press_type == "LONG"
                press_emoji = "👆⏱️"
            end
            fmt.add_sensor("string", press_type, "Button Press", press_emoji)
        else
            # Show last known state or ready status
            fmt.add_sensor("string", "READY", "Button State", "🔘")
        end
        
        fmt.end_line()
        var sensor_msg = fmt.get_msg()
        if sensor_msg != nil
            msg = msg + sensor_msg
        end
        
        # Add button statistics from global storage if available
        if self.node != nil
            var node_data = global.WS101_nodes.find(self.node, {})
            var has_stats = node_data.contains('press_single_count') ||
                           node_data.contains('press_double_count') ||
                           node_data.contains('press_long_count')
            
            if has_stats
                fmt.start_line()
                var single_count = node_data.find('press_single_count', 0)
                var double_count = node_data.find('press_double_count', 0)
                var long_count = node_data.find('press_long_count', 0)
                
                if single_count > 0
                    fmt.add_sensor("string", f"{single_count}", "Single", "👆")
                end
                if double_count > 0
                    fmt.add_sensor("string", f"{double_count}", "Double", "👆👆")
                end
                if long_count > 0
                    fmt.add_sensor("string", f"{long_count}", "Long", "👆⏱️")
                end
                
                fmt.end_line()
                var stats_msg = fmt.get_msg()
                if stats_msg != nil
                    msg = msg + stats_msg
                end
            end
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
        var node_data = global.WS101_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'battery_history': node_data.find('battery_history', []),
            'button_history': node_data.find('button_history', []),
            'press_single_count': node_data.find('press_single_count', 0),
            'press_double_count': node_data.find('press_double_count', 0),
            'press_long_count': node_data.find('press_long_count', 0),
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
        
        # Set Reporting Interval
        tasmota.remove_cmd("LwWS101Interval")
        tasmota.add_cmd("LwWS101Interval", def(cmd, idx, payload_str)
            var seconds = int(payload_str)
            if seconds < 60 || seconds > 65535
                return tasmota.resp_cmnd_str("Invalid: range 60-65535 seconds")
            end
            var hex_cmd = f"FE02{lwdecode.uint16le(seconds)}"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd)
        end)
        
        # Button Mode Configuration
        tasmota.remove_cmd("LwWS101Mode")
        tasmota.add_cmd("LwWS101Mode", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.WS101_nodes, cmd, idx, payload_str, { 
                'SINGLE|1':  ['FF2101', 'SINGLE'],
                'DOUBLE|2':  ['FF2102', 'DOUBLE'],
                'LONG|3':    ['FF2103', 'LONG']
            })
        end)
        
        # Device Reboot
        tasmota.remove_cmd("LwWS101Reboot")
        tasmota.add_cmd("LwWS101Reboot", def(cmd, idx, payload_str)
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS101: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS101()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwWS101TestPayload")
tasmota.add_cmd("LwWS101TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestWS101", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("LwWS101NodeStats")
tasmota.add_cmd("LwWS101NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwWS101ClearNode")
tasmota.add_cmd("LwWS101ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
