# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight WS52x
#
# Generated: 2025-08-15 | Version: 1.1.0 | Revision: 2
#            by "LoRaWAN Decoder AI Generation Template", v2.1.7
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws52x
# - Userguide: https://resource.milesight-iot.com/milesight/iot/document/ws52x-user-guide.pdf
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# v1.1.0 (2025-08-15): Regenerated with AI template v2.1.7
#                      - Enhanced framework integration
#                      - Added formatter expansions
#                      - Improved global storage handling
#                      - Added comprehensive downlink commands
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
        
        # Expand framework with power monitoring formatters
        LwSensorFormatter_cls.Formatter["current"] = {"u": "mA", "f": " %d", "i": "🔌"}
        LwSensorFormatter_cls.Formatter["power_factor"] = {"u": "%", "f": " %d", "i": "📊"}
        LwSensorFormatter_cls.Formatter["socket"] = {"u": "", "f": "", "i": "⚫"}
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
                    
                    # Voltage (0x03, 0x74) - 2 bytes, 0.1V resolution
                    if channel_id == 0x03 && channel_type == 0x74 && i + 1 < size(payload)
                        var voltage = ((payload[i+1] << 8) | payload[i]) / 10.0
                        data['voltage'] = voltage
                        i += 2
                        
                    # Active Power (0x04, 0x80) - 4 bytes signed, 1W resolution
                    elif channel_id == 0x04 && channel_type == 0x80 && i + 3 < size(payload)
                        var power = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        # Handle signed 32-bit
                        if power > 2147483647
                            power = power - 4294967296
                        end
                        data['active_power'] = power
                        i += 4
                        
                    # Power Factor (0x05, 0x81) - 1 byte, 1% resolution
                    elif channel_id == 0x05 && channel_type == 0x81 && i < size(payload)
                        data['power_factor'] = payload[i]
                        i += 1
                        
                    # Energy (0x06, 0x83) - 4 bytes, 1Wh resolution
                    elif channel_id == 0x06 && channel_type == 0x83 && i + 3 < size(payload)
                        var energy = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                        data['energy'] = energy
                        i += 4
                        
                    # Current (0x07, 0xC9) - 2 bytes, 1mA resolution
                    elif channel_id == 0x07 && channel_type == 0xC9 && i + 1 < size(payload)
                        var current = (payload[i+1] << 8) | payload[i]
                        data['current'] = current
                        i += 2
                        
                    # Socket State (0x08, 0x70) - 1 byte
                    elif channel_id == 0x08 && channel_type == 0x70 && i < size(payload)
                        data['socket_state'] = payload[i] == 0x01
                        data['socket_text'] = payload[i] == 0x01 ? "ON" : "OFF"
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
                        elif channel_type == 0x24 && i + 1 < size(payload)  # OC Alarm Config
                            data['oc_alarm_enabled'] = payload[i] == 0x01
                            data['oc_alarm_threshold'] = payload[i+1]
                            i += 2
                        elif channel_type == 0x25 && i + 1 < size(payload)  # Button Lock Config
                            var lock_state = (payload[i+1] << 8) | payload[i]
                            data['button_locked'] = lock_state == 0x0080
                            i += 2
                        elif channel_type == 0x26 && i < size(payload)  # Power Recording Config
                            data['power_recording'] = payload[i] == 0x01
                            i += 1
                        elif channel_type == 0x2F && i < size(payload)  # LED Config
                            data['led_enabled'] = payload[i] == 0x01
                            i += 1
                        elif channel_type == 0x30 && i + 1 < size(payload)  # OC Protection Config
                            data['oc_protection_enabled'] = payload[i] == 0x01
                            data['oc_protection_threshold'] = payload[i+1]
                            i += 2
                        elif channel_type == 0x3F && i < size(payload)  # Power Outage Event
                            data['power_outage'] = payload[i] == 0x01
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
                            var task_time = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                            data['delay_task_ack'] = task_time
                            i += 4
                        elif channel_type == 0x23 && i + 1 < size(payload)  # Delete Task ACK
                            var task_num = (payload[i+1] << 8) | payload[i]
                            data['delete_task_ack'] = task_num
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
            
            # Track energy consumption trend
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
            print(f"LwDecode_WS52x error: {m}")
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
        var name_tooltip = "Milesight WS52x Smart Socket"
        var battery = 1000  # Hide battery (mains powered)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        
        msg = msg + lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build power monitoring display
        fmt.start_line()
        
        # Socket state with dynamic emoji
        if data_to_show.contains('socket_state')
            var socket_emoji = data_to_show['socket_state'] ? "🟢" : "⚫"
            var socket_text = data_to_show.find('socket_text', "UNKNOWN")
            fmt.add_sensor("string", socket_text, "Socket State", socket_emoji)
        end
        
        # Voltage
        if data_to_show.contains('voltage')
            fmt.add_sensor("volt", data_to_show['voltage'], "Voltage", nil)
        end
        
        # Current
        if data_to_show.contains('current')
            fmt.add_sensor("current", data_to_show['current'], "Current", nil)
        end
        
        # Continue to next line for power measurements
        fmt.next_line()
        
        # Active Power
        if data_to_show.contains('active_power')
            fmt.add_sensor("power", data_to_show['active_power'], "Active Power", nil)
        end
        
        # Power Factor
        if data_to_show.contains('power_factor')
            fmt.add_sensor("power_factor", data_to_show['power_factor'], "Power Factor", nil)
        end
        
        # Energy (only if significant)
        if data_to_show.contains('energy') && data_to_show['energy'] > 0
            fmt.add_sensor("energy", data_to_show['energy'], "Total Energy", nil)
        end
        
        fmt.end_line()
        var sensor_msg = fmt.get_msg()
        if sensor_msg != nil
            msg = msg + sensor_msg
        end
        
        # Add alert/status line for special events
        var has_alerts = false
        if data_to_show.find('device_reset', false) || 
           data_to_show.find('power_outage', false) ||
           data_to_show.find('oc_alarm_enabled', false)
            fmt.start_line()
            has_alerts = true
            
            if data_to_show.find('device_reset', false)
                var reset_text = data_to_show.find('reset_type', 'Reset')
                fmt.add_status(reset_text, "🔄", "Device Reset")
            end
            
            if data_to_show.find('power_outage', false)
                fmt.add_status("Outage", "⚠️", "Power Outage Event")
            end
            
            if data_to_show.find('oc_alarm_enabled', false)
                fmt.add_status("OC Alert", "🚨", "Overcurrent Alarm")
            end
            
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
        var node_data = global.WS52x_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
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
        tasmota.remove_cmd("WS52xControl")
        tasmota.add_cmd("WS52xControl", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['08FF', 'ON'],
                '0|OFF': ['0800', 'OFF']
            })
        end)
        
        # Set Reporting Interval
        tasmota.remove_cmd("WS52xInterval")
        tasmota.add_cmd("WS52xInterval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            var hex_cmd = f"FE02{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # OC Alarm Configuration
        tasmota.remove_cmd("WS52xOCAlarm")
        tasmota.add_cmd("WS52xOCAlarm", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: WS52xOCAlarm<node> <enabled(0/1)>,<threshold(1-30)>")
            end
            
            var enabled = int(parts[0])
            var threshold = int(parts[1])
            
            if enabled < 0 || enabled > 1
                return tasmota.resp_cmnd_str("Invalid enabled: 0 or 1")
            end
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30A")
            end
            
            var hex_cmd = f"FF24{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # OC Protection Configuration
        tasmota.remove_cmd("WS52xOCProtection")
        tasmota.add_cmd("WS52xOCProtection", def(cmd, idx, payload_str)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: WS52xOCProtection<node> <enabled(0/1)>,<threshold(1-30)>")
            end
            
            var enabled = int(parts[0])
            var threshold = int(parts[1])
            
            if enabled < 0 || enabled > 1
                return tasmota.resp_cmnd_str("Invalid enabled: 0 or 1")
            end
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30A")
            end
            
            var hex_cmd = f"FF30{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Button Lock Control
        tasmota.remove_cmd("WS52xLock")
        tasmota.add_cmd("WS52xLock", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|LOCK|LOCKED':     ['FF250080', 'LOCKED'],
                '0|UNLOCK|UNLOCKED': ['FF250000', 'UNLOCKED']
            })
        end)
        
        # LED Control
        tasmota.remove_cmd("WS52xLED")
        tasmota.add_cmd("WS52xLED", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':   ['FF2F01', 'ENABLED'],
                '0|OFF|DISABLE': ['FF2F00', 'DISABLED']
            })
        end)
        
        # Power Recording Control
        tasmota.remove_cmd("WS52xRecording")
        tasmota.add_cmd("WS52xRecording", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':   ['FF2601', 'ENABLED'],
                '0|OFF|DISABLE': ['FF2600', 'DISABLED']
            })
        end)
        
        # Reset Energy Counter
        tasmota.remove_cmd("WS52xResetEnergy")
        tasmota.add_cmd("WS52xResetEnergy", def(cmd, idx, payload_str)
            var hex_cmd = "FF2700"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Status Enquiry
        tasmota.remove_cmd("WS52xStatus")
        tasmota.add_cmd("WS52xStatus", def(cmd, idx, payload_str)
            var hex_cmd = "FF2800"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device Reboot
        tasmota.remove_cmd("WS52xReboot")
        tasmota.add_cmd("WS52xReboot", def(cmd, idx, payload_str)
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Delay Task
        tasmota.remove_cmd("WS52xDelayTask")
        tasmota.add_cmd("WS52xDelayTask", def(cmd, idx, payload_str)
            var seconds = int(payload_str)
            if seconds < 0 || seconds > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 0-4294967295 seconds")
            end
            var hex_cmd = f"FE22{lwdecode.uint32le(seconds)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Delete Task
        tasmota.remove_cmd("WS52xDeleteTask")
        tasmota.add_cmd("WS52xDeleteTask", def(cmd, idx, payload_str)
            var task_num = int(payload_str)
            if task_num < 0 || task_num > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535")
            end
            var hex_cmd = f"FE23{lwdecode.uint16le(task_num)}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        print("WS52x: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_WS52x()

# Test command registration (recreated on each load)
tasmota.remove_cmd("WS52xTestPayload")
tasmota.add_cmd("WS52xTestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestWS52x", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("WS52xNodeStats")
tasmota.add_cmd("WS52xNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("WS52xClearNode")
tasmota.add_cmd("WS52xClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
