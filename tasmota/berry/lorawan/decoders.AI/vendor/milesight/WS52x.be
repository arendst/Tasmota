# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight WS52x
#
# Generated: 2025-08-16 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.10
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws52x
# - Userguide: https://www.milesight.com/iot/product/lorawan-sensor/ws52x
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# CHANGELOG
# v1.0.0 (2025-08-16): Initial generation from PDF specification with debug capabilities
# -------------------------------------------------------------

class LwDecode_WS52x
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    var debug_mode     # Debug mode for enhanced logging
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # WS52x doesn't require CRC validation per spec
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        self.debug_mode = true  # Enable debug for development
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("WS52x_nodes")
            global.WS52x_nodes = {}
        end
        if !global.contains("WS52x_cmdInit")
            global.WS52x_cmdInit = false
        end
        
        # Add custom formatters for WS52x-specific measurements
        LwSensorFormatter_cls.Formatter["power"] = {"u": "W", "f": " %d", "i": "⚡"}
        LwSensorFormatter_cls.Formatter["power_factor"] = {"u": "%", "f": " %d", "i": "📊"}
        LwSensorFormatter_cls.Formatter["energy"] = {"u": "Wh", "f": " %d", "i": "🔋"}
        LwSensorFormatter_cls.Formatter["current"] = {"u": "mA", "f": " %d", "i": "🔌"}
        LwSensorFormatter_cls.Formatter["voltage"] = {"u": "V", "f": " %.1f", "i": "⚡"}
        LwSensorFormatter_cls.Formatter["socket"] = {"u": "", "f": " %s", "i": "🔌"}
        
        if self.debug_mode
            print("WS52x: Decoder initialized with debug mode enabled")
        end
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        # Validate inputs
        if payload == nil || size(payload) < 1
            if self.debug_mode print("WS52x: Invalid payload") end
            return nil
        end
        
        try
            # Store device info
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            if self.debug_mode
                print(f"WS52x: Decoding node={node}, fport={fport}, size={size(payload)}")
                print(f"WS52x: Payload hex: {payload.tohex()}")
            end
            
            # Retrieve node history from global storage
            var node_data = global.WS52x_nodes.find(node, {})
            
            # WS52x uses port 85 for all data
            if fport == 85
                self.decode_data_payload(payload, data)
            else
                if self.debug_mode print(f"WS52x: Unknown fport {fport}") end
                data['unknown_port'] = fport
                data['raw_payload'] = payload.tohex()
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Track reset events
            if data.contains('reset_event') && data['reset_event']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
                if self.debug_mode print(f"WS52x: Reset event detected, count={node_data['reset_count']}") end
            end
            
            # Track power outage events
            if data.contains('power_outage_event') && data['power_outage_event']
                node_data['power_outage_count'] = node_data.find('power_outage_count', 0) + 1
                node_data['last_power_outage'] = tasmota.rtc()['local']
                if self.debug_mode print(f"WS52x: Power outage event detected") end
            end
            
            # Register downlink commands if not done yet
            if !global.contains("WS52x_cmdInit") || !global.WS52x_cmdInit
                self.register_downlink_commands()
                global.WS52x_cmdInit = true
            end

            # Save back to global storage
            global.WS52x_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            if self.debug_mode
                print(f"WS52x: Decoded successfully, channels: {size(data)}")
            end
            
            return data
            
        except .. as e, m
            lwdecode.log_error("DECODE_WS52x", e, m, 
                format("Device:%s, Node:%s, FPort:%d, PayloadSize:%d", name, node, fport, size(payload)))
            return nil
        end
    end
    
    def decode_data_payload(payload, data)
        var i = 0
        var channels_decoded = 0
        
        while i < size(payload)
            if i + 1 >= size(payload) break end
            
            var channel_id = payload[i]
            var channel_type = payload[i+1]
            i += 2
            channels_decoded += 1
            
            if self.debug_mode
                print(f"WS52x: Channel {channels_decoded}: ID=0x{channel_id:02X} Type=0x{channel_type:02X}")
            end
            
            # Voltage (Channel 0x03, Type 0x74)
            if channel_id == 0x03 && channel_type == 0x74
                if i + 1 >= size(payload) break end
                var voltage = (payload[i] | (payload[i+1] << 8)) / 10.0
                data['voltage'] = voltage
                i += 2
                if self.debug_mode print(f"WS52x: Voltage = {voltage}V") end
                
            # Active Power (Channel 0x04, Type 0x80)
            elif channel_id == 0x04 && channel_type == 0x80
                if i + 3 >= size(payload) break end
                var power = payload[i] | (payload[i+1] << 8) | (payload[i+2] << 16) | (payload[i+3] << 24)
                # Convert from unsigned to signed 32-bit
                if power > 2147483647
                    power = power - 4294967296
                end
                data['active_power'] = power
                i += 4
                if self.debug_mode print(f"WS52x: Active Power = {power}W") end
                
            # Power Factor (Channel 0x05, Type 0x81)
            elif channel_id == 0x05 && channel_type == 0x81
                if i >= size(payload) break end
                var power_factor = payload[i]
                data['power_factor'] = power_factor
                i += 1
                if self.debug_mode print(f"WS52x: Power Factor = {power_factor}%") end
                
            # Energy (Channel 0x06, Type 0x83)
            elif channel_id == 0x06 && channel_type == 0x83
                if i + 3 >= size(payload) break end
                var energy = payload[i] | (payload[i+1] << 8) | (payload[i+2] << 16) | (payload[i+3] << 24)
                data['energy'] = energy
                i += 4
                if self.debug_mode print(f"WS52x: Energy = {energy}Wh") end
                
            # Current (Channel 0x07, Type 0xC9)
            elif channel_id == 0x07 && channel_type == 0xC9
                if i + 1 >= size(payload) break end
                var current = payload[i] | (payload[i+1] << 8)
                data['current'] = current
                i += 2
                if self.debug_mode print(f"WS52x: Current = {current}mA") end
                
            # Socket State (Channel 0x08, Type 0x70)
            elif channel_id == 0x08 && channel_type == 0x70
                if i >= size(payload) break end
                var socket_state = payload[i]
                data['socket_state'] = socket_state
                data['socket_state_text'] = socket_state == 0x01 ? "ON" : "OFF"
                i += 1
                if self.debug_mode print(f"WS52x: Socket State = {data['socket_state_text']}") end
                
            # Device Information and Configuration Channels (0xFF prefix)
            elif channel_id == 0xFF
                i = self.decode_device_info_channel(payload, i-1, data, channel_type)
                
            # Reporting and ACK Channels (0xFE prefix)
            elif channel_id == 0xFE
                i = self.decode_reporting_channel(payload, i-1, data, channel_type)
                
            else
                # Unknown channel - log and try to skip
                if self.debug_mode 
                    print(f"WS52x: Unknown channel ID=0x{channel_id:02X} Type=0x{channel_type:02X}")
                end
                data[f'unknown_ch_{channel_id:02X}_{channel_type:02X}'] = "present"
                # Try to continue - this is risky but better than failing
                i += 1  # Skip one byte and hope for the best
            end
        end
        
        if self.debug_mode
            print(f"WS52x: Decoded {channels_decoded} channels total")
        end
    end
    
    def decode_device_info_channel(payload, start_idx, data, channel_type)
        var i = start_idx + 2  # Skip channel_id and channel_type
        
        if channel_type == 0x01  # Protocol Version
            if i < size(payload)
                data['protocol_version'] = payload[i]
                i += 1
                if self.debug_mode print(f"WS52x: Protocol Version = {data['protocol_version']}") end
            end
            
        elif channel_type == 0x09  # Hardware Version
            if i + 1 < size(payload)
                data['hw_version'] = f"{payload[i]}.{payload[i+1]}"
                i += 2
                if self.debug_mode print(f"WS52x: HW Version = {data['hw_version']}") end
            end
            
        elif channel_type == 0x0A  # Software Version
            if i + 1 < size(payload)
                data['sw_version'] = f"{payload[i]}.{payload[i+1]}"
                i += 2
                if self.debug_mode print(f"WS52x: SW Version = {data['sw_version']}") end
            end
            
        elif channel_type == 0x0B  # Power On Event
            data['power_on_event'] = true
            if self.debug_mode print("WS52x: Power On Event detected") end
            
        elif channel_type == 0x0F  # Device Class
            if i < size(payload)
                var dev_class = payload[i]
                data['device_class_code'] = dev_class
                data['device_class'] = dev_class == 0x00 ? "Class A" : (dev_class == 0x01 ? "Class B" : "Class C")
                i += 1
                if self.debug_mode print(f"WS52x: Device Class = {data['device_class']}") end
            end
            
        elif channel_type == 0x16  # Serial Number
            if i + 7 < size(payload)
                var serial = ""
                for j: 0..7
                    serial += f"{payload[i+j]:02X}"
                end
                data['serial_number'] = serial
                i += 8
                if self.debug_mode print(f"WS52x: Serial Number = {serial}") end
            end
            
        elif channel_type == 0x24  # OC Alarm Config
            if i + 1 < size(payload)
                var enabled = payload[i]
                var threshold = payload[i+1]
                data['oc_alarm_enabled'] = enabled == 0x01
                data['oc_alarm_threshold'] = threshold
                i += 2
                if self.debug_mode print(f"WS52x: OC Alarm = {data['oc_alarm_enabled']}, Threshold = {threshold}A") end
            end
            
        elif channel_type == 0x25  # Button Lock Config
            if i + 1 < size(payload)
                var lock_state = (payload[i+1] << 8) | payload[i]
                data['button_locked'] = lock_state == 0x0080
                i += 2
                if self.debug_mode print(f"WS52x: Button Locked = {data['button_locked']}") end
            end
            
        elif channel_type == 0x26  # Power Recording Config
            if i < size(payload)
                data['power_recording_enabled'] = payload[i] == 0x01
                i += 1
                if self.debug_mode print(f"WS52x: Power Recording = {data['power_recording_enabled']}") end
            end
            
        elif channel_type == 0x2F  # LED Config
            if i < size(payload)
                data['led_enabled'] = payload[i] == 0x01
                i += 1
                if self.debug_mode print(f"WS52x: LED Enabled = {data['led_enabled']}") end
            end
            
        elif channel_type == 0x30  # OC Protection Config
            if i + 1 < size(payload)
                var enabled = payload[i]
                var threshold = payload[i+1]
                data['oc_protection_enabled'] = enabled == 0x01
                data['oc_protection_threshold'] = threshold
                i += 2
                if self.debug_mode print(f"WS52x: OC Protection = {data['oc_protection_enabled']}, Threshold = {threshold}A") end
            end
            
        elif channel_type == 0x3F  # Power Outage Event
            data['power_outage_event'] = true
            if self.debug_mode print("WS52x: Power Outage Event detected") end
            
        elif channel_type == 0xFE  # Reset Event
            if i < size(payload)
                var reset_reason = payload[i]
                data['reset_event'] = true
                var reset_reasons = {0x00: "POR", 0x01: "BOR", 0x02: "WDT", 0x03: "CMD"}
                data['reset_reason'] = reset_reasons.find(reset_reason, f"Unknown({reset_reason})")
                i += 1
                if self.debug_mode print(f"WS52x: Reset Event = {data['reset_reason']}") end
            end
            
        elif channel_type == 0xFF  # TSL Version
            if i + 1 < size(payload)
                data['tsl_version'] = f"{payload[i]}.{payload[i+1]}"
                i += 2
                if self.debug_mode print(f"WS52x: TSL Version = {data['tsl_version']}") end
            end
            
        else
            if self.debug_mode print(f"WS52x: Unknown device info type 0x{channel_type:02X}") end
            # Skip 1 byte for unknown types
            i += 1
        end
        
        return i
    end
    
    def decode_reporting_channel(payload, start_idx, data, channel_type)
        var i = start_idx + 2  # Skip channel_id and channel_type
        
        if channel_type == 0x02  # Reporting Interval
            if i + 1 < size(payload)
                var interval = (payload[i+1] << 8) | payload[i]
                data['reporting_interval'] = interval
                i += 2
                if self.debug_mode print(f"WS52x: Reporting Interval = {interval} minutes") end
            end
            
        elif channel_type == 0x03  # Interval ACK
            if i + 1 < size(payload)
                var interval = (payload[i+1] << 8) | payload[i]
                data['interval_ack'] = interval
                i += 2
                if self.debug_mode print(f"WS52x: Interval ACK = {interval} minutes") end
            end
            
        elif channel_type == 0x10  # Reboot ACK
            data['reboot_ack'] = true
            if self.debug_mode print("WS52x: Reboot ACK received") end
            
        elif channel_type == 0x22  # Delay Task ACK
            if i + 3 < size(payload)
                var seconds = (payload[i+3] << 24) | (payload[i+2] << 16) | (payload[i+1] << 8) | payload[i]
                data['delay_task_ack'] = seconds
                i += 4
                if self.debug_mode print(f"WS52x: Delay Task ACK = {seconds} seconds") end
            end
            
        elif channel_type == 0x23  # Delete Task ACK
            if i + 1 < size(payload)
                var task_number = (payload[i+1] << 8) | payload[i]
                data['delete_task_ack'] = task_number
                i += 2
                if self.debug_mode print(f"WS52x: Delete Task ACK = {task_number}") end
            end
            
        else
            if self.debug_mode print(f"WS52x: Unknown reporting type 0x{channel_type:02X}") end
            # Skip 1 byte for unknown types
            i += 1
        end
        
        return i
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
        var battery = 1000  # No battery - hide battery indicator
        var battery_last_seen = 0
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 to hide if no RSSI
        
        msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        
        # Socket state (most important - show first)
        if data_to_show.contains('socket_state_text')
            var socket_status = data_to_show['socket_state_text']
            var socket_emoji = socket_status == "ON" ? "🟢" : "🔴"
            fmt.add_sensor("socket", socket_status, "Socket", socket_emoji)
        end
        
        # Power measurements
        if data_to_show.contains('active_power')
            fmt.add_sensor("power", data_to_show['active_power'], "Power", "⚡")
        end
        
        if data_to_show.contains('voltage')
            fmt.add_sensor("voltage", data_to_show['voltage'], "Voltage", "⚡")
        end
        
        if data_to_show.contains('current')
            fmt.add_sensor("current", data_to_show['current'], "Current", "🔌")
        end
        
        # Continue to next line for secondary measurements
        fmt.next_line()
        
        if data_to_show.contains('energy')
            fmt.add_sensor("energy", data_to_show['energy'], "Energy", "🔋")
        end
        
        if data_to_show.contains('power_factor')
            fmt.add_sensor("power_factor", data_to_show['power_factor'], "PF", "📊")
        end
        
        # Alerts and status indicators
        if data_to_show.contains('reset_event') && data_to_show['reset_event']
            fmt.add_status(data_to_show.find('reset_reason', 'Reset'), "⚠️", "Device was reset")
        end
        
        if data_to_show.contains('power_outage_event') && data_to_show['power_outage_event']
            fmt.add_status("Power Outage", "⚠️", "Power outage detected")
        end
        
        # Configuration status
        if data_to_show.contains('oc_protection_enabled') && data_to_show['oc_protection_enabled']
            fmt.add_status("OC Prot", "🛡️", f"Overcurrent protection active ({data_to_show.find('oc_protection_threshold', 0)}A)")
        end
        
        fmt.end_line()
        msg += fmt.get_msg()
        
        # Add debug info if in debug mode and data is old
        if self.debug_mode && last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 300  # Data older than 5 minutes in debug mode
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
            'power_outage_count': node_data.find('power_outage_count', 0),
            'last_power_outage': node_data.find('last_power_outage', 0),
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
    
    # Set debug mode
    def set_debug_mode(enabled)
        self.debug_mode = enabled
        var status = enabled ? "enabled" : "disabled"
        print(f"WS52x: Debug mode {status}")
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Socket Control Command (ON/OFF) using SendDownlinkMap
        tasmota.remove_cmd("LwWS52xControl")
        tasmota.add_cmd("LwWS52xControl", def(cmd, idx, payload_str)
            # Format: LwWS52xControl<node> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['08FF', 'Socket ON'],     # Maps "1" or "ON" to hex 08FF
                '0|OFF': ['0800', 'Socket OFF']     # Maps "0" or "OFF" to hex 0800
            })
        end)
        
        # Set Reporting Interval
        tasmota.remove_cmd("LwWS52xInterval")
        tasmota.add_cmd("LwWS52xInterval", def(cmd, idx, payload_str)
            # Format: LwWS52xInterval<node> <minutes>
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            
            # Build hex command per PDF specification (little endian)
            var hex_cmd = f"FE02{minutes & 0xFF:02X}{(minutes >> 8) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # OC Alarm Configuration
        tasmota.remove_cmd("LwWS52xOCAlarm")
        tasmota.add_cmd("LwWS52xOCAlarm", def(cmd, idx, payload_str)
            # Format: LwWS52xOCAlarm<node> <enabled>,<threshold_A>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwWS52xOCAlarm<node> <0|1>,<1-30>")
            end
            
            var enabled = int(parts[0])
            var threshold = int(parts[1])
            
            if enabled < 0 || enabled > 1
                return tasmota.resp_cmnd_str("Invalid enabled: use 0 or 1")
            end
            
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30 A")
            end
            
            var hex_cmd = f"FF24{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # OC Protection Configuration
        tasmota.remove_cmd("LwWS52xOCProtection")
        tasmota.add_cmd("LwWS52xOCProtection", def(cmd, idx, payload_str)
            # Format: LwWS52xOCProtection<node> <enabled>,<threshold_A>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwWS52xOCProtection<node> <0|1>,<1-30>")
            end
            
            var enabled = int(parts[0])
            var threshold = int(parts[1])
            
            if enabled < 0 || enabled > 1
                return tasmota.resp_cmnd_str("Invalid enabled: use 0 or 1")
            end
            
            if threshold < 1 || threshold > 30
                return tasmota.resp_cmnd_str("Invalid threshold: range 1-30 A")
            end
            
            var hex_cmd = f"FF30{enabled:02X}{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Button Lock Configuration using SendDownlinkMap
        tasmota.remove_cmd("LwWS52xButtonLock")
        tasmota.add_cmd("LwWS52xButtonLock", def(cmd, idx, payload_str)
            # Format: LwWS52xButtonLock<node> <locked|unlocked|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|LOCKED':   ['FF250080', 'Button Locked'],
                '0|UNLOCKED': ['FF250000', 'Button Unlocked']
            })
        end)
        
        # LED Mode Configuration using SendDownlinkMap
        tasmota.remove_cmd("LwWS52xLED")
        tasmota.add_cmd("LwWS52xLED", def(cmd, idx, payload_str)
            # Format: LwWS52xLED<node> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['FF2F01', 'LED Enabled'],
                '0|OFF': ['FF2F00', 'LED Disabled']
            })
        end)
        
        # Power Recording Configuration using SendDownlinkMap
        tasmota.remove_cmd("LwWS52xPowerRecording")
        tasmota.add_cmd("LwWS52xPowerRecording", def(cmd, idx, payload_str)
            # Format: LwWS52xPowerRecording<node> <on|off|1|0>
            return lwdecode.SendDownlinkMap(global.WS52x_nodes, cmd, idx, payload_str, { 
                '1|ON':  ['FF2601', 'Power Recording Enabled'],
                '0|OFF': ['FF2600', 'Power Recording Disabled']
            })
        end)
        
        # Reset Energy Counter
        tasmota.remove_cmd("LwWS52xResetEnergy")
        tasmota.add_cmd("LwWS52xResetEnergy", def(cmd, idx, payload_str)
            # Format: LwWS52xResetEnergy<node>
            var hex_cmd = "FF2700"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Status Enquiry
        tasmota.remove_cmd("LwWS52xStatus")
        tasmota.add_cmd("LwWS52xStatus", def(cmd, idx, payload_str)
            # Format: LwWS52xStatus<node>
            var hex_cmd = "FF2800"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device Reboot
        tasmota.remove_cmd("LwWS52xReboot")
        tasmota.add_cmd("LwWS52xReboot", def(cmd, idx, payload_str)
            # Format: LwWS52xReboot<node>
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Delay Task (schedule socket operation)
        tasmota.remove_cmd("LwWS52xDelayTask")
        tasmota.add_cmd("LwWS52xDelayTask", def(cmd, idx, payload_str)
            # Format: LwWS52xDelayTask<node> <seconds>
            var seconds = int(payload_str)
            if seconds < 0 || seconds > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 0-4294967295 seconds")
            end
            
            # Build hex command using framework helper (little endian)
            var hex_cmd = "FE22" + lwdecode.uint32le(seconds)
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Delete Task
        tasmota.remove_cmd("LwWS52xDeleteTask")
        tasmota.add_cmd("LwWS52xDeleteTask", def(cmd, idx, payload_str)
            # Format: LwWS52xDeleteTask<node> <task_number>
            var task_number = int(payload_str)
            if task_number < 0 || task_number > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535")
            end
            
            # Build hex command using framework helper (little endian)
            var hex_cmd = "FE23" + lwdecode.uint16le(task_number)
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Debug Control Command
        tasmota.remove_cmd("LwWS52xDebug")
        tasmota.add_cmd("LwWS52xDebug", def(cmd, idx, payload_str)
            # Format: LwWS52xDebug <on|off|1|0>
            var enable = payload_str == "on" || payload_str == "1"
            LwDeco.set_debug_mode(enable)
            var status = enable ? "enabled" : "disabled"
            tasmota.resp_cmnd_str(f"Debug mode {status}")
        end)
        
        print("WS52x: All downlink commands registered successfully")
    end
end

# Global instance
LwDeco = LwDecode_WS52x()

# Test command registration with debug scenarios
tasmota.remove_cmd("LwWS52xTestPayload")
tasmota.add_cmd("LwWS52xTestPayload", def(cmd, idx, payload_str)
    # Parse parameters: payload_str can be "hex", "fport,hex", or "rssi,fport,hex"
    var parts = string.split(payload_str, ',')
    var rssi = -85          # Default RSSI
    var fport = 85          # Default fport for WS52x
    var hex_payload = payload_str
    
    if size(parts) == 1
        # Format: <hex_payload>
        hex_payload = parts[0]
    elif size(parts) == 2
        # Format: <fport>,<hex_payload>
        fport = int(parts[0])
        hex_payload = parts[1]
    elif size(parts) == 3
        # Format: <rssi>,<fport>,<hex_payload>
        rssi = int(parts[0])
        fport = int(parts[1])
        hex_payload = parts[2]
    end
    
    # Parse hex string to bytes
    var test_payload = bytes(hex_payload)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink(f"WS52x-{idx}", idx, rssi, fport, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Test scenarios for UI development
tasmota.remove_cmd("LwWS52xTestReal")
tasmota.add_cmd("LwWS52xTestReal", def(cmd, idx, scenario)
    # Pre-defined test scenarios for UI development
    var test_scenarios = {
        # Scenario 1: Normal operation - socket ON with normal power consumption
        "normal": "037410F004800F27000005810A0683D007C9E003087001",
        
        # Scenario 2: Low power consumption - socket ON but minimal load
        "low": "037408F804800A000000058100068301000000C90A00087001",
        
        # Scenario 3: High power consumption - socket ON with high load
        "high": "0374B40B048088130000058164068340E2010007C9501D087001",
        
        # Scenario 4: Alert scenario - overcurrent protection triggered
        "alert": "037410F004800F27000005810A0683D007C9E003087001FF30010A",
        
        # Scenario 5: Configuration info - device settings
        "config": "FF0101FF090203FF0A0106FF25008000262601FF2F01FF24010A",
        
        # Scenario 6: Device info - version and serial
        "info": "FF0101FF090203FF0A0106FF16AABBCCDDEEFF0011FF01FF0110"
    }
    
    var payload_hex = test_scenarios.find(scenario, nil)
    if payload_hex == nil
        tasmota.resp_cmnd_str("Usage: LwWS52xTestReal<node> <normal|low|high|alert|config|info>")
        return
    end
    
    var test_payload = bytes(payload_hex)
    var result = LwDeco.decodeUplink(f"WS52x-{idx}", idx, -85, 85, test_payload)
    
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
