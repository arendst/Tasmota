# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight WS101 (DEBUG VERSION)
#
# Generated: 2025-08-16 | Version: 2.1.0-debug | Revision: 01
#            by "LoRaWAN Decoder AI Generation Template", v2.1.12
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/ws101
# - Userguide: https://resource.milesight.com/milesight/iot/document/ws101-user-guide-en.pdf
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# CHANGELOG
# v2.1.0-debug (2025-08-16): Debug version with enhanced logging
#                            - Enhanced debug logging throughout decode process
#                            - Payload hex dumps and channel-by-channel processing
#                            - Debug control command for runtime toggling
#                            - Complete node storage and analytics tracking
# v2.0.0 (2025-01-20): Framework upgrade to v2.1.10
# v1.0.0 (2025-01-15): Initial generation from PDF specification
# -------------------------------------------------------------

class LwDecode_WS101
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    var debug_mode     # Debug mode for enhanced logging
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # WS101 does not require CRC validation per specs
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        self.debug_mode = true  # Enable debug for development
        
        if self.debug_mode
            print("WS101: Decoder initialized with debug mode enabled")
        end
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("WS101_nodes")
            global.WS101_nodes = {}
        end
        if !global.contains("WS101_cmdInit")
            global.WS101_cmdInit = false
        end
        
        # Add custom formatters for button-specific data types
        LwSensorFormatter_cls.Formatter["button_type"] = {"u": "", "f": " %s", "i": "🔘"}
        LwSensorFormatter_cls.Formatter["button_count"] = {"u": "x", "f": " %d", "i": "🔢"}
        LwSensorFormatter_cls.Formatter["device_version"] = {"u": "", "f": " v%s", "i": "📦"}
        LwSensorFormatter_cls.Formatter["device_class"] = {"u": "", "f": " %s", "i": "📡"}
        LwSensorFormatter_cls.Formatter["reset_reason"] = {"u": "", "f": " %s", "i": "🔄"}
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        if self.debug_mode
            print(f"WS101: Decoding node={node}, fport={fport}, size={size(payload)}")
            print(f"WS101: Payload hex: {payload.tohex()}")
        end
        
        # Validate inputs
        if payload == nil || size(payload) < 1
            if self.debug_mode
                print("WS101: Invalid payload - nil or empty")
            end
            return nil
        end
        
        try
            # Store device info
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            if self.debug_mode
                print(f"WS101: Stored device info - name={name}, node={node}, rssi={rssi}")
            end
            
            # Retrieve node history from global storage
            var node_data = global.WS101_nodes.find(node, {})
            
            # Decode based on fport (WS101 uses port 85 exclusively)
            if fport == 85
                if self.debug_mode
                    print("WS101: Starting payload decode for port 85")
                end
                data = self.decode_ws101_payload(payload, data, node_data)
                if self.debug_mode
                    import json
                    print(f"WS101: Decode complete, result keys: {data.keys()}")
                end
            else
                if self.debug_mode
                    print(f"WS101: Unsupported fport {fport}")
                end
                return nil
            end
            
            # Update node history in global storage
            node_data['last_data'] = data
            node_data['last_update'] = tasmota.rtc()['local']
            node_data['name'] = name
            
            # Store battery trend if available
            if data.contains('battery_pct')
                if !node_data.contains('battery_history')
                    node_data['battery_history'] = []
                end
                # Keep last 10 battery readings for trend analysis
                node_data['battery_history'].push(data['battery_pct'])
                if size(node_data['battery_history']) > 10
                    node_data['battery_history'].pop(0)
                end
                # Calculate battery trend
                var history = node_data['battery_history']
                if size(history) >= 3
                    var recent_avg = (history[-1] + history[-2] + history[-3]) / 3
                    var older_avg = (history[0] + history[1] + history[2]) / 3
                    node_data['battery_trend'] = recent_avg - older_avg
                    if self.debug_mode
                        var trend_str = node_data['battery_trend'] > 0 ? "improving" : "declining"
                        print(f"WS101: Battery trend {trend_str} ({node_data['battery_trend']:.1f}%)")
                    end
                end
                if self.debug_mode
                    print(f"WS101: Battery = {data['battery_pct']}% ({data['battery_v']:.2f}V)")
                end
            end
            
            # Store reset count if detected
            if data.contains('device_reset') && data['device_reset']
                node_data['reset_count'] = node_data.find('reset_count', 0) + 1
                node_data['last_reset'] = tasmota.rtc()['local']
                if self.debug_mode
                    var reason = data.find('reset_reason', 'Unknown')
                    print(f"WS101: Device reset detected - reason={reason}, count={node_data['reset_count']}")
                end
            end
            
            # Track button press statistics
            if data.contains('button_type') && data.contains('button_press')
                var press_type = data['button_type']
                var press_key = f"{press_type}_count"
                node_data[press_key] = node_data.find(press_key, 0) + 1
                node_data['total_presses'] = node_data.find('total_presses', 0) + 1
                node_data['last_press_type'] = press_type
                node_data['last_press_time'] = tasmota.rtc()['local']
                
                # Button usage analytics
                var rtc_data = tasmota.rtc()
                if rtc_data && rtc_data.contains('hour')
                    var hour = rtc_data['hour']
                    var hourly_key = f"hour_{hour}_presses"
                    node_data[hourly_key] = node_data.find(hourly_key, 0) + 1
                    
                    if self.debug_mode
                        print(f"WS101: Button press - type={press_type}, total={node_data['total_presses']}")
                        print(f"WS101: Hour {hour} presses: {node_data[hourly_key]}")
                    end
                else
                    if self.debug_mode
                        print(f"WS101: Button press - type={press_type}, total={node_data['total_presses']}")
                        print("WS101: RTC hour data not available")
                    end
                end
            end
            
            # Initialize and register downlink commands if needed
            if !global.contains("WS101_cmdInit") || !global.WS101_cmdInit
                self.register_downlink_commands()
                global.WS101_cmdInit = true
            end

            # Save back to global storage
            global.WS101_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            if self.debug_mode
                print(f"WS101: Node storage updated for {node}")
            end
            
            return data
            
        except .. as e, m
            if self.debug_mode
                print(f"WS101: Exception in decode - {e}: {m}")
            end
            print(f"WS101: Decode error - {e}: {m}")
            return nil
        end
    end
    
    def decode_ws101_payload(payload, data, node_data)
        var i = 0
        
        if self.debug_mode
            print(f"WS101: Starting channel decode, payload size: {size(payload)}")
        end
        
        while i < size(payload)
            if i + 1 >= size(payload)
                if self.debug_mode
                    print(f"WS101: Insufficient data at offset {i}, breaking")
                end
                break  # Need at least channel ID and type
            end
            
            var channel_id = payload[i]
            var channel_type = payload[i + 1]
            i += 2
            
            if self.debug_mode
                print(f"WS101: Processing channel ID={channel_id:02X} Type={channel_type:02X} at offset {i-2}")
            end
            
            # Decode based on channel ID and type from MAP specification
            if channel_id == 0x01 && channel_type == 0x00
                # Button Press Type - 1 byte
                if i >= size(payload)
                    if self.debug_mode
                        print("WS101: Button press data missing")
                    end
                    break
                end
                var press_value = payload[i]
                var press_type = ""
                if press_value == 0x01
                    press_type = "Single Press"
                elif press_value == 0x02
                    press_type = "Double Press"
                elif press_value == 0x03
                    press_type = "Long Press"
                else
                    press_type = f"Unknown Press ({press_value})"
                end
                data['button_type'] = press_type
                data['button_press'] = true
                data['button_value'] = press_value
                if self.debug_mode
                    print(f"WS101: Button decoded - {press_type} (raw={press_value})")
                end
                i += 1
                
            elif channel_id == 0x01 && channel_type == 0x75
                # Battery Level - 1 byte (percentage)
                if i >= size(payload)
                    if self.debug_mode
                        print("WS101: Battery data missing")
                    end
                    break
                end
                var battery_pct = payload[i]
                data['battery_pct'] = battery_pct
                data['battery_v'] = self.battery_percent_to_voltage(battery_pct)
                if self.debug_mode
                    print(f"WS101: Battery decoded - {battery_pct}% = {data['battery_v']:.2f}V")
                end
                i += 1
                
            elif channel_id == 0xFF && channel_type == 0x01
                # Protocol Version - 1 byte
                if i >= size(payload)
                    break
                end
                data['protocol_version'] = payload[i]
                if self.debug_mode
                    print(f"WS101: Protocol version = {payload[i]}")
                end
                i += 1
                
            elif channel_id == 0xFF && channel_type == 0x09
                # Hardware Version - 2 bytes
                if i + 1 >= size(payload)
                    break
                end
                var hw_major = payload[i]
                var hw_minor = payload[i + 1]
                data['hw_version'] = f"{hw_major}.{hw_minor}"
                if self.debug_mode
                    print(f"WS101: Hardware version = {data['hw_version']}")
                end
                i += 2
                
            elif channel_id == 0xFF && channel_type == 0x0A
                # Software Version - 2 bytes
                if i + 1 >= size(payload)
                    break
                end
                var sw_major = payload[i]
                var sw_minor = payload[i + 1]
                data['sw_version'] = f"{sw_major}.{sw_minor}"
                if self.debug_mode
                    print(f"WS101: Software version = {data['sw_version']}")
                end
                i += 2
                
            elif channel_id == 0xFF && channel_type == 0x0B
                # Power On Event - 1 byte
                if i >= size(payload)
                    break
                end
                data['power_on_event'] = true
                data['power_on_reason'] = payload[i]
                if self.debug_mode
                    print(f"WS101: Power on event - reason={payload[i]}")
                end
                i += 1
                
            elif channel_id == 0xFF && channel_type == 0x0F
                # Device Class - 1 byte
                if i >= size(payload)
                    break
                end
                var dev_class = payload[i]
                var class_str = ""
                if dev_class == 0x00
                    class_str = "Class A"
                elif dev_class == 0x01
                    class_str = "Class B"
                elif dev_class == 0x02
                    class_str = "Class C"
                else
                    class_str = f"Unknown Class ({dev_class})"
                end
                data['device_class'] = class_str
                if self.debug_mode
                    print(f"WS101: Device class = {class_str}")
                end
                i += 1
                
            elif channel_id == 0xFF && channel_type == 0x16
                # Serial Number - 8 bytes
                if i + 7 >= size(payload)
                    break
                end
                var serial = ""
                for j: 0..7
                    serial += format("%02X", payload[i + j])
                end
                data['serial_number'] = serial
                if self.debug_mode
                    print(f"WS101: Serial number = {serial}")
                end
                i += 8
                
            elif channel_id == 0xFF && channel_type == 0xFE
                # Reset Event - 1 byte
                if i >= size(payload)
                    break
                end
                var reset_reason = payload[i]
                data['device_reset'] = true
                var reason_str = ""
                if reset_reason == 0x00
                    reason_str = "POR"
                elif reset_reason == 0x01
                    reason_str = "BOR"
                elif reset_reason == 0x02
                    reason_str = "WDT"
                elif reset_reason == 0x03
                    reason_str = "CMD"
                else
                    reason_str = f"Unknown Reset ({reset_reason})"
                end
                data['reset_reason'] = reason_str
                if self.debug_mode
                    print(f"WS101: Reset event - {reason_str}")
                end
                i += 1
                
            elif channel_id == 0xFF && channel_type == 0xFF
                # TSL Version - 2 bytes
                if i + 1 >= size(payload)
                    break
                end
                var tsl_major = payload[i]
                var tsl_minor = payload[i + 1]
                data['tsl_version'] = f"{tsl_major}.{tsl_minor}"
                if self.debug_mode
                    print(f"WS101: TSL version = {data['tsl_version']}")
                end
                i += 2
                
            elif channel_id == 0xFE && channel_type == 0x02
                # Reporting Interval - 2 bytes (little endian)
                if i + 1 >= size(payload)
                    break
                end
                var interval = payload[i] | (payload[i + 1] << 8)
                data['reporting_interval'] = interval
                if self.debug_mode
                    print(f"WS101: Reporting interval = {interval}s")
                end
                i += 2
                
            elif channel_id == 0xFE && channel_type == 0x03
                # Interval ACK - 2 bytes (acknowledgment for set interval command)
                if i + 1 >= size(payload)
                    break
                end
                var ack_interval = payload[i] | (payload[i + 1] << 8)
                data['interval_ack'] = ack_interval
                data['config_ack'] = true
                data['ack_type'] = "interval"
                if self.debug_mode
                    print(f"WS101: Interval ACK received - {ack_interval}s")
                end
                i += 2
                
            elif channel_id == 0xFE && channel_type == 0x10
                # Reboot ACK - 1 byte (acknowledgment for reboot command)
                if i >= size(payload)
                    break
                end
                data['reboot_ack'] = true
                data['reboot_status'] = payload[i]
                data['config_ack'] = true
                data['ack_type'] = "reboot"
                if self.debug_mode
                    print(f"WS101: Reboot ACK received - status={payload[i]}")
                end
                i += 1
                
            else
                # Unknown channel - log but try to continue
                if self.debug_mode
                    print(f"WS101: Unknown channel ID={channel_id:02X} Type={channel_type:02X} at offset {i-2}")
                    print("WS101: Attempting to resync by advancing 1 byte")
                end
                # Advance by one byte and try to resync
                i += 1
            end
        end
        
        if self.debug_mode
            print(f"WS101: Channel decode complete, processed {i} bytes")
        end
        
        return data
    end
    
    def battery_percent_to_voltage(percent)
        # Convert battery percentage to estimated voltage for WS101
        # Li battery: 3.6V nominal, 3.0V min, 4.1V max
        if percent <= 0 return 3.0
        elif percent >= 100 return 4.1
        else
            # Non-linear battery discharge curve approximation
            return 3.0 + (percent / 100.0) * 1.1
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
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        
        msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter - single line preferred
        fmt.start_line()
        
        # Show button press type if available
        if data_to_show.contains('button_type')
            fmt.add_sensor("button_type", data_to_show['button_type'], "Button", "🔘")
        end
        
        # Show battery percentage
        if data_to_show.contains('battery_pct')
            fmt.add_sensor("battery", data_to_show['battery_pct'], "Battery", "🔋")
        end
        
        # Show device class if available
        if data_to_show.contains('device_class')
            fmt.add_sensor("device_class", data_to_show['device_class'], "Class", "📡")
        end
        
        fmt.end_line()
        msg += fmt.get_msg()
        
        # Add version info line if available
        if data_to_show.contains('hw_version') || data_to_show.contains('sw_version')
            fmt.start_line()
            if data_to_show.contains('hw_version')
                fmt.add_sensor("device_version", data_to_show['hw_version'], "HW", "📦")
            end
            if data_to_show.contains('sw_version')
                fmt.add_sensor("device_version", data_to_show['sw_version'], "SW", "📦")
            end
            fmt.end_line()
            var version_msg = fmt.get_msg()
            if version_msg != nil
                msg = msg + version_msg
            end
        end
        
        # Add reset event if detected
        if data_to_show.find('device_reset')
            fmt.start_line()
            var reset_reason = data_to_show.find('reset_reason', 'Unknown')
            fmt.add_sensor("reset_reason", reset_reason, "Reset", "🔄")
            fmt.end_line()
            var reset_msg = fmt.get_msg()
            if reset_msg != nil
                msg = msg + reset_msg
            end
        end
        
        # Add acknowledgment status if present
        if data_to_show.find('config_ack')
            fmt.start_line()
            var ack_type = data_to_show.find('ack_type', 'config')
            fmt.add_status(f"ACK: {ack_type}", "✅", "Configuration acknowledged")
            fmt.end_line()
            var ack_msg = fmt.get_msg()
            if ack_msg != nil
                msg = msg + ack_msg
            end
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.start_line()
                fmt.add_status(self.format_age(age), "⏱️", "Last activity")
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
    
    # Get comprehensive node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.WS101_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'reset_count': node_data.find('reset_count', 0),
            'last_reset': node_data.find('last_reset', 0),
            'battery_history': node_data.find('battery_history', []),
            'battery_trend': node_data.find('battery_trend', 0),
            'total_presses': node_data.find('total_presses', 0),
            'Single Press_count': node_data.find('Single Press_count', 0),
            'Double Press_count': node_data.find('Double Press_count', 0),
            'Long Press_count': node_data.find('Long Press_count', 0),
            'last_press_type': node_data.find('last_press_type', 'None'),
            'last_press_time': node_data.find('last_press_time', 0),
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
    
    # Set debug mode
    def set_debug_mode(enabled)
        self.debug_mode = enabled
        var status = enabled ? "enabled" : "disabled"
        print(f"WS101: Debug mode {status}")
    end
    
    # Register comprehensive downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Reporting Interval Command
        # Format: LwWS101Interval<slot> <seconds>
        # Range: 60-65535 seconds (little endian)
        tasmota.remove_cmd("LwWS101Interval")
        tasmota.add_cmd("LwWS101Interval", def(cmd, idx, payload_str)
            var seconds = int(payload_str)
            if seconds < 60 || seconds > 65535
                return tasmota.resp_cmnd_str("Invalid: range 60-65535 seconds")
            end
            
            # Build hex command: FE02 + 16-bit little endian seconds
            var hex_cmd = f"FE02{seconds & 0xFF:02X}{(seconds >> 8) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd)
        end)
        
        # Button Mode Configuration Command
        # Format: LwWS101Mode<slot> <single|double|long|1|2|3>
        tasmota.remove_cmd("LwWS101Mode")
        tasmota.add_cmd("LwWS101Mode", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.WS101_nodes, cmd, idx, payload_str, { 
                'SINGLE|1': ['FF2101', 'Single Press Mode'],
                'DOUBLE|2': ['FF2102', 'Double Press Mode'],
                'LONG|3':   ['FF2103', 'Long Press Mode']
            })
        end)
        
        # Device Reboot Command
        # Format: LwWS101Reboot<slot>
        tasmota.remove_cmd("LwWS101Reboot")
        tasmota.add_cmd("LwWS101Reboot", def(cmd, idx, payload_str)
            # Simple reboot command: FF10FF
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request Device Status Command
        # Format: LwWS101Status<slot>
        tasmota.remove_cmd("LwWS101Status")
        tasmota.add_cmd("LwWS101Status", def(cmd, idx, payload_str)
            # Status request command: FF00 (custom implementation)
            var hex_cmd = "FF00"
            return lwdecode.SendDownlink(global.WS101_nodes, cmd, idx, hex_cmd)
        end)
        
        # Debug control command
        tasmota.remove_cmd("LwWS101Debug")
        tasmota.add_cmd("LwWS101Debug", def(cmd, idx, payload_str)
            var enable = payload_str == "on" || payload_str == "1"
            LwDeco.set_debug_mode(enable)
            var status = enable ? "enabled" : "disabled"
            tasmota.resp_cmnd_str(f"Debug mode {status}")
        end)
        
        if self.debug_mode
            print("WS101: Downlink commands registered (Interval, Mode, Reboot, Status, Debug)")
        end
    end
end

# Global instance
LwDeco = LwDecode_WS101()

# Test command registration (recreated on each load)
# Command usage: LwWS101TestPayload<slot> <hex_payload>
#                LwWS101TestPayload<slot> <fport>,<hex_payload>
#                LwWS101TestPayload<slot> <rssi>,<fport>,<hex_payload>
# Where <slot> is the driver slot (0-15), NOT the device node ID
# Default: fport=85 (WS101 standard), rssi=-85

tasmota.remove_cmd("LwWS101TestPayload")
tasmota.add_cmd("LwWS101TestPayload", def(cmd, idx, payload_str)
    # idx = driver slot number (0-15) assigned by Tasmota
    # Parse parameters: payload_str can be "hex", "fport,hex", or "rssi,fport,hex"
    var parts = string.split(payload_str, ',')
    var rssi = -85          # Default RSSI
    var fport = 85          # Default fport for WS101
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
    
    # Use slot number for device name, but test node ID for protocol
    var device_name = format("WS101-slot%d", idx)
    var node_id = format("test_%d", idx)  # Use slot-based node ID for testing
    
    var result = LwDeco.decodeUplink(device_name, node_id, rssi, fport, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Debug version - TestReal command for UI testing without hardware
# Command usage: LwWS101TestReal<slot> <scenario>
tasmota.remove_cmd("LwWS101TestReal")
tasmota.add_cmd("LwWS101TestReal", def(cmd, idx, payload_str)
    # Predefined realistic test scenarios for UI development
    var test_scenarios = {
        # Define realistic test payloads for different device states
        "normal":    "010001017550",                    # Single press + 80% battery
        "low":       "010001011E",                      # Single press + 30% battery
        "high":      "010002017564",                    # Double press + 100% battery
        "alert":     "FFFE03",                          # Reset event (CMD)
        "config":    "FE022C01",                        # Interval ACK (300s)
        "info":      "FF0A010AFF090203FF1612345678ABCDEF01"  # SW v1.10 + HW v2.3 + Serial
    }
    
    var scenario = payload_str.find(' ') > 0 ? payload_str[0..payload_str.find(' ')-1] : payload_str
    var hex_payload = test_scenarios.find(scenario, test_scenarios["normal"])
    
    if hex_payload == nil
        var scenarios_list = ""
        for key: test_scenarios.keys()
            scenarios_list += key + " "
        end
        return tasmota.resp_cmnd_str(format("Available scenarios: %s", scenarios_list))
    end
    
    var test_payload = bytes(hex_payload)
    var device_name = format("WS101-slot%d", idx)
    var node_id = format("test_%d", idx)
    
    var result = LwDeco.decodeUplink(device_name, node_id, -75, 85, test_payload)
    
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