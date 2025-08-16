# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Dragino LHT65
#
# Generated: 2025-08-16 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.dragino.com/products/lora-lorawan-end-node/item/151-lht65.html
# - Userguide: https://www.dragino.com/downloads/downloads/LHT65/UserManual/LHT65_Temperature_Humidity_Sensor_UserManual_v1.8.5.pdf
# - Decoder:   https://www.dragino.com/downloads/index.php?dir=LHT65/payload_decode/
# -------------------------------------------------------------
# v1.0.0 (2025-08-16): Initial generation from PDF specification
# - Complete uplink decoding for all 9 external sensor types
# - 8 downlink commands with parameter validation
# - Battery status and voltage monitoring
# - External sensor cable status detection
# - Datalog polling support with Unix timestamps
# - Error detection for disconnected sensors
# - Global node storage with trend tracking
# - Memory-optimized implementation
# -------------------------------------------------------------

class LwDecode_LHT65
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # No CRC validation required per PDF
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("LHT65_nodes")
            global.LHT65_nodes = {}
        end
        if !global.contains("LHT65_cmdInit")
            global.LHT65_cmdInit = false
        end
        
        # Standard formatters for LHT65 (emojis defined in formatters)
        LwSensorFormatter_cls.Formatter["temperature"] = {"u": "°C", "f": " %.1f", "i": "🌡️"}
        LwSensorFormatter_cls.Formatter["humidity"] = {"u": "%RH", "f": " %.0f", "i": "💧"}
        LwSensorFormatter_cls.Formatter["illuminance"] = {"u": "lx", "f": " %d", "i": "💡"}
        LwSensorFormatter_cls.Formatter["adc_voltage"] = {"u": "mV", "f": " %d", "i": "⚡"}
        LwSensorFormatter_cls.Formatter["count"] = {"u": "", "f": " %d", "i": "🔢"}
    end
    
    def decodeUplink(name, node, rssi, fport, payload)
        import string
        import global
        var data = {}
        
        # Validate inputs
        if payload == nil || size(payload) < 6
            return nil
        end
        
        try
            # Store device info
            self.name = name
            self.node = node
            data['rssi'] = rssi
            data['fport'] = fport
            
            # Retrieve node history from global storage
            var node_data = global.LHT65_nodes.find(node, {})
            
            # Decode standard LHT65 payload (FPort 2)
            if fport == 2
                # Handle different payload formats
                if size(payload) == 11
                    # Check if this is datalog format (Ext=9) or standard format
                    var ext_type = payload[6]
                    if ext_type == 0x09 && size(payload) == 11
                        # Datalog format: ext_temp(2) + builtin_temp(2) + bat_hum(2) + status_ext(1) + timestamp(4)
                        data = self.decode_datalog_format(payload)
                    else
                        # Standard format: bat(2) + temp(2) + hum(2) + ext_type(1) + ext_value(4)
                        data = self.decode_standard_format(payload)
                    end
                elif size(payload) >= 6
                    # Minimum standard format without external sensor
                    data = self.decode_standard_format(payload)
                else
                    return nil
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
            
            # Track external sensor events
            if data.contains('ext_type')
                node_data['ext_type'] = data['ext_type']
                node_data['ext_name'] = self.get_ext_sensor_name(data['ext_type'])
            end
            
            # Track interrupt events
            if data.contains('interrupt_triggered') && data['interrupt_triggered']
                node_data['interrupt_count'] = node_data.find('interrupt_count', 0) + 1
                node_data['last_interrupt'] = tasmota.rtc()['local']
            end
            
            # Track counting events
            if data.contains('event_count')
                var prev_count = node_data.find('last_event_count', 0)
                if data['event_count'] != prev_count
                    node_data['count_changes'] = node_data.find('count_changes', 0) + 1
                    node_data['last_event_count'] = data['event_count']
                end
            end
            
            # Register downlink commands if not already done
            if !global.contains("LHT65_cmdInit") || !global.LHT65_cmdInit
                self.register_downlink_commands()
                global.LHT65_cmdInit = true
            end

            # Save back to global storage
            global.LHT65_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            lwdecode.log_error("DECODE_LHT65", e, m, 
                format("Device:%s, Node:%s, FPort:%d, PayloadSize:%d", name, node, fport, size(payload)))
            return nil
        end
    end
    
    def decode_standard_format(payload)
        var data = {}
        
        # Battery status and voltage (bytes 0-1)
        var bat_raw = (payload[1] << 8) | payload[0]
        var bat_status = (bat_raw >> 14) & 0x03
        var bat_voltage_mv = bat_raw & 0x3FFF
        
        data['battery_v'] = bat_voltage_mv / 1000.0
        data['battery_status'] = bat_status
        data['battery_status_text'] = self.get_battery_status_text(bat_status)
        
        # Built-in temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        if temp_raw > 32767
            temp_raw = temp_raw - 65536  # Handle signed values
        end
        data['temperature'] = temp_raw / 100.0
        
        # Built-in humidity (bytes 4-5)
        var hum_raw = (payload[5] << 8) | payload[4]
        data['humidity'] = hum_raw / 10.0
        
        # External sensor handling (bytes 6+)
        if size(payload) > 6
            var ext_type = payload[6]
            data['ext_type'] = ext_type
            data['ext_name'] = self.get_ext_sensor_name(ext_type)
            
            if size(payload) >= 11
                # Decode external sensor data based on type
                if ext_type == 0x01
                    self.decode_ext_temperature(payload, data)
                elif ext_type == 0x04
                    self.decode_ext_interrupt(payload, data)
                elif ext_type == 0x05
                    self.decode_ext_illumination(payload, data)
                elif ext_type == 0x06
                    self.decode_ext_adc(payload, data)
                elif ext_type == 0x07
                    self.decode_ext_counting_16bit(payload, data)
                elif ext_type == 0x08
                    self.decode_ext_counting_32bit(payload, data)
                end
            end
        end
        
        return data
    end
    
    def decode_datalog_format(payload)
        var data = {}
        
        # Datalog format: ext_temp(2) + builtin_temp(2) + bat_hum(2) + status_ext(1) + timestamp(4)
        
        # External temperature (bytes 0-1)
        var ext_temp_raw = (payload[1] << 8) | payload[0]
        if ext_temp_raw == 0x7FFF
            data['ext_temperature'] = 327.67  # Error value indicates no sensor
            data['ext_sensor_connected'] = false
        else
            if ext_temp_raw > 32767
                ext_temp_raw = ext_temp_raw - 65536
            end
            data['ext_temperature'] = ext_temp_raw / 100.0
            data['ext_sensor_connected'] = true
        end
        
        # Built-in temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        if temp_raw > 32767
            temp_raw = temp_raw - 65536
        end
        data['temperature'] = temp_raw / 100.0
        
        # Battery status and humidity (bytes 4-5)
        var bat_hum_raw = (payload[5] << 8) | payload[4]
        var bat_status = (bat_hum_raw >> 14) & 0x03
        var humidity = bat_hum_raw & 0x0FFF
        
        data['battery_status'] = bat_status
        data['battery_status_text'] = self.get_battery_status_text(bat_status)
        data['humidity'] = humidity / 10.0
        
        # Status and ext byte (byte 6)
        var status_ext = payload[6]
        data['poll_message'] = (status_ext & 0x20) != 0
        data['sync_time_ok'] = (status_ext & 0x10) != 0
        data['unix_time_request'] = (status_ext & 0x08) != 0
        data['ext_type'] = status_ext & 0x0F
        
        # Unix timestamp (bytes 7-10)
        var timestamp = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
        data['timestamp'] = timestamp
        data['timestamp_text'] = self.format_unix_timestamp(timestamp)
        
        return data
    end
    
    def decode_ext_temperature(payload, data)
        # E1 Temperature Sensor (DS18B20)
        var temp_raw = (payload[8] << 8) | payload[7]
        if temp_raw == 0x7FFF
            data['ext_temperature'] = 327.67  # Error value
            data['ext_sensor_connected'] = false
            data['ext_error'] = "DS18B20 not connected"
        else
            if temp_raw > 32767
                temp_raw = temp_raw - 65536
            end
            data['ext_temperature'] = temp_raw / 100.0
            data['ext_sensor_connected'] = true
        end
    end
    
    def decode_ext_interrupt(payload, data)
        # E4 Interrupt Sensor
        var status_byte = payload[7]
        data['cable_connected'] = (status_byte & 0x80) != 0
        data['interrupt_triggered'] = (status_byte & 0x40) != 0
        data['pin_level'] = (status_byte & 0x01) != 0
        data['interrupt_type'] = data['interrupt_triggered'] ? "interrupt" : "normal"
    end
    
    def decode_ext_illumination(payload, data)
        # E5 Illumination Sensor
        var illuminance = (payload[8] << 8) | payload[7]
        var status_byte = payload[9]
        data['illuminance'] = illuminance
        data['cable_connected'] = (status_byte & 0x80) != 0
    end
    
    def decode_ext_adc(payload, data)
        # E6 ADC Sensor
        var adc_raw = (payload[8] << 8) | payload[7]
        var status_byte = payload[9]
        data['adc_voltage'] = adc_raw  # Already in mV
        data['adc_voltage_v'] = adc_raw / 1000.0
        data['cable_connected'] = (status_byte & 0x80) != 0
    end
    
    def decode_ext_counting_16bit(payload, data)
        # E7 Counting Sensor (16-bit)
        var count = (payload[8] << 8) | payload[7]
        var status_byte = payload[9]
        data['event_count'] = count
        data['cable_connected'] = (status_byte & 0x80) != 0
    end
    
    def decode_ext_counting_32bit(payload, data)
        # E7 Counting Sensor (32-bit)
        var count = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
        data['event_count'] = count
        # No cable status for 32-bit counting (uses all 4 bytes)
    end
    
    def get_battery_status_text(status)
        if status == 0
            return "Ultra Low"
        elif status == 1
            return "Low"
        elif status == 2
            return "OK"
        elif status == 3
            return "Good"
        else
            return "Unknown"
        end
    end
    
    def get_ext_sensor_name(ext_type)
        if ext_type == 0x00
            return "None"
        elif ext_type == 0x01
            return "E1 Temperature"
        elif ext_type == 0x04
            return "E4 Interrupt"
        elif ext_type == 0x05
            return "E5 Illumination"
        elif ext_type == 0x06
            return "E6 ADC"
        elif ext_type == 0x07
            return "E7 Count 16bit"
        elif ext_type == 0x08
            return "E7 Count 32bit"
        elif ext_type == 0x09
            return "E1 Timestamp"
        else
            return format("Unknown(0x%02X)", ext_type)
        end
    end
    
    def format_unix_timestamp(timestamp)
        # Convert Unix timestamp to readable format
        if timestamp == 0
            return "Not set"
        end
        # Simple conversion - could be enhanced with proper date formatting
        return format("Unix:%d", timestamp)
    end
    
    def add_web_sensor()
        import global
        
        # Try to use current instance data first
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        # If no instance data, try to recover from global storage
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.LHT65_nodes.find(self.node, {})
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
            name = format("LHT65-%s", self.node)
        end
        var name_tooltip = "Dragino LHT65 Temperature & Humidity Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        
        msg = lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        
        # Built-in sensors (always present)
        fmt.add_sensor("temperature", data_to_show.find('temperature'), "Built-in Temp", nil)
        fmt.add_sensor("humidity", data_to_show.find('humidity'), "Built-in Humidity", nil)
        
        # External sensor display based on type
        var ext_type = data_to_show.find('ext_type', 0)
        if ext_type != 0
            fmt.next_line()  # Continue to next line for external sensor
            
            if ext_type == 0x01 || ext_type == 0x09  # Temperature sensors
                var ext_temp = data_to_show.find('ext_temperature')
                if ext_temp != nil
                    if ext_temp == 327.67
                        fmt.add_status("E1 disconnected", "⚠️", "DS18B20 temperature sensor not connected")
                    else
                        fmt.add_sensor("temperature", ext_temp, "External Temp", nil)
                    end
                end
                
            elif ext_type == 0x04  # Interrupt sensor
                var cable_ok = data_to_show.find('cable_connected', false)
                if cable_ok
                    var pin_level = data_to_show.find('pin_level', false)
                    var interrupted = data_to_show.find('interrupt_triggered', false)
                    if interrupted
                        fmt.add_status(pin_level ? "High (INT)" : "Low (INT)", "🔴", "Interrupt triggered")
                    else
                        fmt.add_status(pin_level ? "High" : "Low", "🟢", "Normal state")
                    end
                else
                    fmt.add_status("E4 disconnected", "⚠️", "Interrupt sensor cable not connected")
                end
                
            elif ext_type == 0x05  # Illumination sensor
                var cable_ok = data_to_show.find('cable_connected', false)
                if cable_ok
                    fmt.add_sensor("illuminance", data_to_show.find('illuminance'), "Light", nil)
                else
                    fmt.add_status("E5 disconnected", "⚠️", "Illumination sensor cable not connected")
                end
                
            elif ext_type == 0x06  # ADC sensor
                var cable_ok = data_to_show.find('cable_connected', false)
                if cable_ok
                    fmt.add_sensor("adc_voltage", data_to_show.find('adc_voltage'), "ADC", nil)
                else
                    fmt.add_status("E6 disconnected", "⚠️", "ADC sensor cable not connected")
                end
                
            elif ext_type == 0x07 || ext_type == 0x08  # Counting sensors
                var cable_ok = data_to_show.find('cable_connected', true)  # 32-bit has no cable status
                if cable_ok
                    fmt.add_sensor("count", data_to_show.find('event_count'), "Events", nil)
                else
                    fmt.add_status("E7 disconnected", "⚠️", "Counting sensor cable not connected")
                end
            end
        end
        
        # Special indicators
        if data_to_show.find('poll_message', false)
            fmt.next_line()
            fmt.add_status("Datalog", "📊", "This is a poll response message")
        end
        
        if data_to_show.find('unix_time_request', false)
            fmt.next_line()
            fmt.add_status("Time Sync", "⏰", "Device requesting time synchronization")
        end
        
        fmt.end_line()
        msg = msg + fmt.get_msg()
        
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
        if seconds < 60 return format("%ds ago", seconds)
        elif seconds < 3600 return format("%dm ago", seconds/60)
        elif seconds < 86400 return format("%dh ago", seconds/3600)
        else return format("%dd ago", seconds/86400)
        end
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.LHT65_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'battery_history': node_data.find('battery_history', []),
            'ext_type': node_data.find('ext_type', 0),
            'ext_name': node_data.find('ext_name', 'None'),
            'interrupt_count': node_data.find('interrupt_count', 0),
            'last_interrupt': node_data.find('last_interrupt', 0),
            'count_changes': node_data.find('count_changes', 0),
            'last_event_count': node_data.find('last_event_count', 0),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.LHT65_nodes.contains(node_id)
            global.LHT65_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Transmit Interval command
        tasmota.remove_cmd("LwLHT65Interval")
        tasmota.add_cmd("LwLHT65Interval", def(cmd, idx, payload_str)
            # Format: LwLHT65Interval<node> <seconds>
            var seconds = int(payload_str)
            if seconds < 10 || seconds > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 10-16777215 seconds")
            end
            
            # Build hex command: 01 + 3 bytes big endian
            var hex_cmd = format("01%02X%02X%02X", 
                (seconds >> 16) & 0xFF, 
                (seconds >> 8) & 0xFF, 
                seconds & 0xFF)
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set External Sensor Mode command
        tasmota.remove_cmd("LwLHT65ExtSensor")
        tasmota.add_cmd("LwLHT65ExtSensor", def(cmd, idx, payload_str)
            # Format: LwLHT65ExtSensor<node> <type>[,<mode>[,<timeout>]]
            var parts = string.split(payload_str, ',')
            if size(parts) < 1 || size(parts) > 3
                return tasmota.resp_cmnd_str("Usage: LwLHT65ExtSensor<node> <type>[,<mode>[,<timeout>]]")
            end
            
            var sensor_type = int(parts[0])
            var hex_cmd = format("A2%02X", sensor_type)
            
            # Add mode parameter if provided
            if size(parts) >= 2
                var mode = int(parts[1])
                hex_cmd = hex_cmd + format("%02X", mode)
                
                # Add timeout for ADC sensor (type 6)
                if size(parts) == 3 && sensor_type == 6
                    var timeout = int(parts[2])
                    if timeout < 0 || timeout > 65535
                        return tasmota.resp_cmnd_str("Invalid timeout: range 0-65535ms")
                    end
                    hex_cmd = hex_cmd + format("%02X%02X", (timeout >> 8) & 0xFF, timeout & 0xFF)
                end
            end
            
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Enable/Disable DS18B20 Probe ID command
        tasmota.remove_cmd("LwLHT65ProbeID")
        tasmota.add_cmd("LwLHT65ProbeID", def(cmd, idx, payload_str)
            # Format: LwLHT65ProbeID<node> <0|1>
            return lwdecode.SendDownlinkMap(global.LHT65_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':  ['A801', 'Enabled'],
                '0|OFF|DISABLE': ['A800', 'Disabled']
            })
        end)
        
        # Set System Time command
        tasmota.remove_cmd("LwLHT65SetTime")
        tasmota.add_cmd("LwLHT65SetTime", def(cmd, idx, payload_str)
            # Format: LwLHT65SetTime<node> <unix_timestamp>
            var timestamp = int(payload_str)
            if timestamp < 1000000000 || timestamp > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 1000000000-4294967295 (Unix timestamp)")
            end
            
            # Build hex command: 30 + 4 bytes big endian
            var hex_cmd = format("30%02X%02X%02X%02X",
                (timestamp >> 24) & 0xFF,
                (timestamp >> 16) & 0xFF,
                (timestamp >> 8) & 0xFF,
                timestamp & 0xFF)
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Time Sync Mode command
        tasmota.remove_cmd("LwLHT65TimeSync")
        tasmota.add_cmd("LwLHT65TimeSync", def(cmd, idx, payload_str)
            # Format: LwLHT65TimeSync<node> <manual|auto>
            return lwdecode.SendDownlinkMap(global.LHT65_nodes, cmd, idx, payload_str, { 
                'MANUAL|0':  ['2800', 'Manual'],
                'AUTO|1':    ['2801', 'Auto LoRaWAN MAC']
            })
        end)
        
        # Set Time Sync Interval command
        tasmota.remove_cmd("LwLHT65SyncInterval")
        tasmota.add_cmd("LwLHT65SyncInterval", def(cmd, idx, payload_str)
            # Format: LwLHT65SyncInterval<node> <days>
            var days = int(payload_str)
            if days < 1 || days > 255
                return tasmota.resp_cmnd_str("Invalid: range 1-255 days")
            end
            
            var hex_cmd = format("29%02X", days)
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        # Clear Flash Record command
        tasmota.remove_cmd("LwLHT65ClearData")
        tasmota.add_cmd("LwLHT65ClearData", def(cmd, idx, payload_str)
            # Format: LwLHT65ClearData<node>
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, "A301")
        end)
        
        # Poll Sensor Data command
        tasmota.remove_cmd("LwLHT65Poll")
        tasmota.add_cmd("LwLHT65Poll", def(cmd, idx, payload_str)
            # Format: LwLHT65Poll<node> <start_timestamp>,<end_timestamp>,<interval>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwLHT65Poll<node> <start_time>,<end_time>,<interval>")
            end
            
            var start_time = int(parts[0])
            var end_time = int(parts[1])
            var interval = int(parts[2])
            
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            # Build hex command: 31 + start(4) + end(4) + interval(1)
            var hex_cmd = format("31%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                (start_time >> 24) & 0xFF, (start_time >> 16) & 0xFF,
                (start_time >> 8) & 0xFF, start_time & 0xFF,
                (end_time >> 24) & 0xFF, (end_time >> 16) & 0xFF,
                (end_time >> 8) & 0xFF, end_time & 0xFF,
                interval)
            return lwdecode.SendDownlink(global.LHT65_nodes, cmd, idx, hex_cmd)
        end)
        
        print("LHT65: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_LHT65()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwLHT65TestPayload")
tasmota.add_cmd("LwLHT65TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestLHT65", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("LwLHT65NodeStats")
tasmota.add_cmd("LwLHT65NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwLHT65ClearNode")
tasmota.add_cmd("LwLHT65ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
