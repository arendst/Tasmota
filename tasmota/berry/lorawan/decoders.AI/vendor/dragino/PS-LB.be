# -------------------------------------------------------------
# Originally Prompted by: LoRaWAN Decoder AI Generation Request
#
# LoRaWAN AI-Generated Decoder for Dragino PS-LB
#
# Generated: 2025-08-16 | Version: 1.1.0 | Revision: 002
#            by "LoRaWAN Decoder AI Generation Template", v2.1.10
#
# Official Links
# - Homepage:  https://www.dragino.com/products/lora-lorawan-end-node/item/151-ps-lb.html
# - Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/PS-LB%20--%20LoRaWAN%20Pressure%20Sensor/
# - Decoder:   https://github.com/dragino/dragino-end-node-decoder
# -------------------------------------------------------------
# CHANGELOG
# v1.1.0 (2025-08-16): Complete regeneration with framework v2.1.10
#   - Enhanced uplink coverage: all fport types with improved validation
#   - Complete downlink command implementation with proper hex formatting
#   - Improved pressure probe model conversion algorithms
#   - Enhanced ROC (Report on Change) feature with proper flag handling
#   - Optimized datalog parsing with Unix timestamp conversion
#   - Better multi-collection mode support for voltage/current sampling
#   - Advanced battery monitoring with trend analysis
#   - Enhanced web UI with custom formatters and status indicators
#   - Improved global node storage with persistence across reloads
#   - Memory optimization for ESP32 constraints
#   - Better error handling and validation throughout
# v1.0.0 (2025-08-16): Initial generation from wiki specification
#   - Complete uplink coverage: device status, sensor data, datalog, ROC
#   - All downlink commands: TDC, interrupt, output control, probe config, ROC, datalog
#   - Pressure probe model conversion with water depth and pressure calculations
#   - Report on Change (ROC) feature with wave and threshold alarm modes
#   - Datalog polling with Unix timestamp handling
#   - Multi-collection mode for voltage/current sampling
#   - Battery monitoring and trend tracking
#   - Enhanced web UI with pressure units and status indicators
# -------------------------------------------------------------

class LwDecode_PS_LB
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (disabled for PS-LB)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # PS-LB does not use CRC validation
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("PS_LB_nodes")
            global.PS_LB_nodes = {}
        end
        if !global.contains("PS_LB_cmdInit")
            global.PS_LB_cmdInit = false
        end
        
        # Register custom formatters for pressure sensor
        LwSensorFormatter_cls.Formatter["pressure_mpa"] = {"u": "MPa", "f": " %.3f", "i": "📊"}
        LwSensorFormatter_cls.Formatter["current"] = {"u": "mA", "f": " %.3f", "i": "🔌"}
        LwSensorFormatter_cls.Formatter["water_depth"] = {"u": "m", "f": " %.2f", "i": "💧"}
        LwSensorFormatter_cls.Formatter["pressure_kpa"] = {"u": "kPa", "f": " %.1f", "i": "📊"}
        LwSensorFormatter_cls.Formatter["unix_time"] = {"u": "", "f": " %s", "i": "⏰"}
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
            var node_data = global.PS_LB_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 5
                # Device Status
                if size(payload) >= 6
                    data = self.decode_device_status(payload, data)
                end
                
            elif fport == 2
                # Sensor Data or ROC Data
                if size(payload) >= 9
                    data = self.decode_sensor_data(payload, data, node_data)
                end
                
            elif fport == 7
                # Multiple sensor data
                if size(payload) >= 2
                    data = self.decode_multi_data(payload, data)
                end
                
            elif fport == 3
                # Datalog data
                if size(payload) >= 11
                    data = self.decode_datalog(payload, data)
                end
            else
                # Unknown fport - still log payload size for debugging
                data['unknown_fport'] = true
                data['payload_size'] = size(payload)
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
                
                # Calculate battery trend
                if size(node_data['battery_history']) >= 3
                    var trend = self.calculate_battery_trend(node_data['battery_history'])
                    data['battery_trend'] = trend
                end
            end
            
            # Store probe configuration for conversions
            if data.contains('probe_model')
                node_data['probe_model'] = data['probe_model']
                node_data['probe_type'] = data.find('probe_type', 'unknown')
            end
            
            # Track ROC events
            if data.find('roc_triggered', false)
                node_data['roc_count'] = node_data.find('roc_count', 0) + 1
                node_data['last_roc'] = tasmota.rtc()['local']
            end
            
            # Initialize downlink commands if needed
            if !global.contains("PS_LB_cmdInit") || !global.PS_LB_cmdInit
                self.register_downlink_commands()
                global.PS_LB_cmdInit = true
            end

            # Save back to global storage
            global.PS_LB_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            lwdecode.log_error("DECODE_PS_LB", e, m, 
                format("Device:%s, Node:%s, FPort:%d, PayloadSize:%d", name, node, fport, size(payload)))
            return nil
        end
    end
    
    def decode_device_status(payload, data)
        # Device Status (FPORT=5): sensor_model(1) + fw_version(2) + freq_band(1) + sub_band(1) + battery(2)
        data['sensor_model'] = payload[0]
        data['device_type'] = "PS-LB"
        
        # Firmware version (assuming first byte is major, second is minor.patch)
        var fw_byte1 = payload[1]
        var fw_byte2 = payload[2]
        data['fw_version'] = format("%d.%d", fw_byte1, fw_byte2)
        
        # Frequency band
        var freq_bands = {
            0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
            0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-1",
            0x09: "AS923-2", 0x0a: "AS923-3", 0x0b: "CN470", 0x0c: "EU433",
            0x0d: "KR920", 0x0e: "MA869"
        }
        data['frequency_band'] = freq_bands.find(payload[3], format("Unknown_%02X", payload[3]))
        data['sub_band'] = payload[4]
        
        # Battery voltage
        var battery_mv = (payload[5] << 8) | payload[6]
        data['battery_v'] = battery_mv / 1000.0
        data['battery_pct'] = self.voltage_to_percent(data['battery_v'])
        
        return data
    end
    
    def decode_sensor_data(payload, data, node_data)
        # Sensor Data (FPORT=2): probe_model(2) + idc_input(2) + vdc_input(2) + battery(2) + status(1)
        
        # Probe model configuration
        var probe_model = (payload[0] << 8) | payload[1]
        data['probe_model'] = probe_model
        
        # Current input (4-20mA from pressure probe)
        var idc_raw = (payload[2] << 8) | payload[3]
        data['idc_input_ma'] = idc_raw / 1000.0
        
        # Voltage input (0-30V)
        var vdc_raw = (payload[4] << 8) | payload[5]
        data['vdc_input_v'] = vdc_raw / 1000.0
        
        # Battery voltage
        var battery_mv = (payload[6] << 8) | payload[7]
        data['battery_v'] = battery_mv / 1000.0
        data['battery_pct'] = self.voltage_to_percent(data['battery_v'])
        
        # Status flags
        var status = payload[8]
        data['in1_level'] = (status & 0x08) != 0
        data['in2_level'] = (status & 0x04) != 0
        data['int_level'] = (status & 0x02) != 0
        data['int_status'] = (status & 0x01) != 0
        
        # Check for ROC flags (bits 4-7)
        if (status & 0xF0) != 0
            data['roc_idc_decrease'] = (status & 0x80) != 0
            data['roc_idc_increase'] = (status & 0x40) != 0
            data['roc_vdc_decrease'] = (status & 0x20) != 0
            data['roc_vdc_increase'] = (status & 0x10) != 0
            data['roc_triggered'] = true
        end
        
        # Convert probe readings to physical values
        self.convert_probe_values(data, node_data)
        
        return data
    end
    
    def decode_multi_data(payload, data)
        # Multiple Data (FPORT=7): data_count(2) + voltage_values(n*2)
        var data_count = (payload[0] << 8) | payload[1]
        data['data_count'] = data_count
        
        var voltage_values = []
        var i = 2
        while i < size(payload) - 1
            var voltage = ((payload[i] << 8) | payload[i+1]) / 1000.0
            voltage_values.push(voltage)
            i += 2
        end
        data['voltage_values'] = voltage_values
        data['multi_mode'] = "voltage_collection"
        
        # Calculate statistics for voltage collection
        if size(voltage_values) > 0
            var min_v = voltage_values[0]
            var max_v = voltage_values[0]
            var sum_v = 0.0
            
            for v : voltage_values
                if v < min_v min_v = v end
                if v > max_v max_v = v end
                sum_v += v
            end
            
            data['voltage_min'] = min_v
            data['voltage_max'] = max_v
            data['voltage_avg'] = sum_v / size(voltage_values)
        end
        
        return data
    end
    
    def decode_datalog(payload, data)
        # Datalog Data (FPORT=3): multiple 11-byte entries
        var entries = []
        var i = 0
        
        while i <= size(payload) - 11
            var entry = {}
            
            # Parse each 11-byte entry
            entry['probe_model'] = (payload[i] << 8) | payload[i+1]
            entry['vdc_input_v'] = ((payload[i+2] << 8) | payload[i+3]) / 1000.0
            entry['idc_input_ma'] = ((payload[i+4] << 8) | payload[i+5]) / 1000.0
            
            var status = payload[i+6]
            entry['in1_level'] = (status & 0x08) != 0
            entry['in2_level'] = (status & 0x04) != 0
            entry['int_level'] = (status & 0x02) != 0
            entry['int_status'] = (status & 0x01) != 0
            
            # Unix timestamp
            var timestamp = (payload[i+7] << 24) | (payload[i+8] << 16) | (payload[i+9] << 8) | payload[i+10]
            entry['unix_timestamp'] = timestamp
            entry['timestamp_str'] = self.format_unix_time(timestamp)
            
            entries.push(entry)
            i += 11
        end
        
        data['datalog_entries'] = entries
        data['datalog_count'] = size(entries)
        data['datalog_mode'] = true
        
        # Calculate datalog time span
        if size(entries) > 1
            var first_ts = entries[0]['unix_timestamp']
            var last_ts = entries[-1]['unix_timestamp']
            data['datalog_span_hours'] = (last_ts - first_ts) / 3600.0
        end
        
        return data
    end
    
    def convert_probe_values(data, node_data)
        # Convert 4-20mA current to pressure/water depth based on probe model
        var probe_model = data.find('probe_model', 0)
        var current_ma = data.find('idc_input_ma', 0)
        
        if current_ma < 3.8 || current_ma > 20.5
            # Invalid current range (allow small tolerance)
            data['probe_error'] = true
            data['probe_error_msg'] = format("Current %.1fmA out of 4-20mA range", current_ma)
            return
        end
        
        # Extract probe type and range from model
        var probe_type = (probe_model >> 8) & 0xFF
        var probe_range = probe_model & 0xFF
        
        if probe_type == 0x00
            # Water depth mode (aa=00, bb=depth_meters)
            var max_depth = probe_range
            if max_depth > 0
                # 4mA = 0m, 20mA = max_depth
                var depth = (current_ma - 4.0) / 16.0 * max_depth
                data['water_depth_m'] = depth
                data['probe_type'] = "water_depth"
                data['probe_range'] = format("%dm", max_depth)
            end
            
        elif probe_type == 0x01
            # Pressure mode (aa=01, bb=pressure_type)
            var pressure_types = {
                0x01: 1.0,    # Type A: 0-1MPa
                0x02: 2.5,    # Type B: 0-2.5MPa
                0x03: 4.0,    # Type C: 0-4MPa
                0x04: 6.0,    # Type D: 0-6MPa
                0x05: 10.0,   # Type E: 0-10MPa
                0x06: 16.0,   # Type F: 0-16MPa
                0x07: 25.0,   # Type G: 0-25MPa
                0x08: 40.0,   # Type H: 0-40MPa
                0x09: 60.0,   # Type I: 0-60MPa
                0x0A: 100.0,  # Type J: 0-100MPa
                0x0B: 160.0,  # Type K: 0-160MPa
                0x0C: 250.0   # Type L: 0-250MPa
            }
            var max_pressure = pressure_types.find(probe_range, 1.0)
            # 4mA = 0MPa, 20mA = max_pressure
            var pressure = (current_ma - 4.0) / 16.0 * max_pressure
            data['pressure_mpa'] = pressure
            data['probe_type'] = "pressure"
            data['probe_range'] = format("%.1fMPa", max_pressure)
            
        elif probe_type == 0x02
            # Differential pressure mode (aa=02, bb=range_type)
            var diff_ranges = {
                0x01: 100,     # 0-100Pa
                0x02: 200,     # 0-200Pa
                0x03: 300,     # 0-300Pa
                0x04: 1000,    # 0-1KPa
                0x05: 2000,    # 0-2KPa
                0x06: 3000,    # 0-3KPa
                0x07: 4000,    # 0-4KPa
                0x08: 5000,    # 0-5KPa
                0x09: 10000,   # 0-10KPa
                0x0A: 200,     # -100~100Pa
                0x0B: 400,     # -200~200Pa
                0x0C: 2000     # -1~1KPa
            }
            var max_pressure_pa = diff_ranges.find(probe_range, 100)
            
            # Handle bipolar ranges
            if probe_range >= 0x0A
                # Bipolar range: 4mA = -max/2, 20mA = +max/2
                var pressure_pa = (current_ma - 12.0) / 8.0 * (max_pressure_pa / 2.0)
                data['diff_pressure_pa'] = pressure_pa
                data['probe_range'] = format("±%.0fPa", max_pressure_pa/2.0)
            else
                # Unipolar range: 4mA = 0Pa, 20mA = max_pressure_pa
                var pressure_pa = (current_ma - 4.0) / 16.0 * max_pressure_pa
                data['diff_pressure_pa'] = pressure_pa
                data['probe_range'] = format("0-%.0fPa", max_pressure_pa)
            end
            data['probe_type'] = "differential"
        else
            # Unknown probe type
            data['probe_type'] = "unknown"
            data['probe_error'] = true
            data['probe_error_msg'] = format("Unknown probe type 0x%02X", probe_type)
        end
    end
    
    def calculate_battery_trend(history)
        # Calculate simple linear trend for battery voltage
        if size(history) < 3 return "stable" end
        
        var recent = history[-3..-1]  # Last 3 readings
        var increasing = 0
        var decreasing = 0
        
        for i : 1..size(recent)-1
            if recent[i] > recent[i-1]
                increasing += 1
            elif recent[i] < recent[i-1]
                decreasing += 1
            end
        end
        
        if increasing > decreasing return "increasing"
        elif decreasing > increasing return "decreasing"
        else return "stable"
        end
    end
    
    def add_web_sensor()
        import global
        
        # Try to use current instance data first
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        # If no instance data, try to recover from global storage
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.PS_LB_nodes.find(self.node, {})
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
            name = format("PS-LB-%s", self.node)
        end
        var name_tooltip = "Dragino PS-LB Pressure Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        
        msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        
        # Pressure/water depth measurement (primary sensor value)
        if data_to_show.contains('water_depth_m')
            fmt.add_sensor("water_depth", data_to_show['water_depth_m'], "Water Depth", "💧")
        elif data_to_show.contains('pressure_mpa')
            fmt.add_sensor("pressure_mpa", data_to_show['pressure_mpa'], "Pressure", "📊")
        elif data_to_show.contains('diff_pressure_pa')
            var pressure_kpa = data_to_show['diff_pressure_pa'] / 1000.0
            fmt.add_sensor("pressure_kpa", pressure_kpa, "Diff Pressure", "🌬️")
        end
        
        # Current input (4-20mA probe signal)
        if data_to_show.contains('idc_input_ma')
            fmt.add_sensor("current", data_to_show['idc_input_ma'], "Probe Current", "🔌")
        end
        
        # Voltage input
        if data_to_show.contains('vdc_input_v')
            fmt.add_sensor("volt", data_to_show['vdc_input_v'], "VDC Input", "⚡")
        end
        
        # Multi-collection statistics
        if data_to_show.contains('voltage_avg')
            fmt.next_line()
            fmt.add_sensor("volt", data_to_show['voltage_avg'], "Avg Voltage", "📊")
            fmt.add_sensor("volt", data_to_show['voltage_min'], "Min", "⬇️")
            fmt.add_sensor("volt", data_to_show['voltage_max'], "Max", "⬆️")
        end
        
        # ROC status indicators
        if data_to_show.find('roc_triggered', false)
            fmt.next_line()
            var roc_status = []
            if data_to_show.find('roc_idc_increase', false)
                roc_status.push("I↑")
            end
            if data_to_show.find('roc_idc_decrease', false)
                roc_status.push("I↓")
            end
            if data_to_show.find('roc_vdc_increase', false)
                roc_status.push("V↑")
            end
            if data_to_show.find('roc_vdc_decrease', false)
                roc_status.push("V↓")
            end
            if size(roc_status) > 0
                fmt.add_status(string.join(roc_status, " "), "🔄", "Report on Change triggered")
            end
        end
        
        # Digital inputs and interrupt status
        if data_to_show.contains('in1_level') || data_to_show.contains('in2_level') || data_to_show.find('int_status', false)
            fmt.next_line()
            var in1 = data_to_show.find('in1_level', false) ? "H" : "L"
            var in2 = data_to_show.find('in2_level', false) ? "H" : "L"
            fmt.add_status(format("IN1:%s IN2:%s", in1, in2), "🔢", "Digital inputs")
            
            if data_to_show.find('int_status', false)
                fmt.add_status("INT", "🔔", "Interrupt triggered")
            end
        end
        
        # Device info (for device status messages)
        if data_to_show.contains('fw_version')
            fmt.next_line()
            fmt.add_status(data_to_show['fw_version'], "💾", "Firmware version")
            fmt.add_status(data_to_show.find('frequency_band', 'Unknown'), "📡", "Frequency band")
        end
        
        # Probe configuration and status
        if data_to_show.contains('probe_type')
            var probe_type = data_to_show['probe_type']
            var probe_icon = probe_type == "water_depth" ? "💧" : (probe_type == "pressure" ? "📊" : "🌬️")
            fmt.add_status(string.toupper(probe_type), probe_icon, "Probe type")
            
            if data_to_show.contains('probe_range')
                fmt.add_status(data_to_show['probe_range'], "🎯", "Probe range")
            end
        end
        
        # Battery trend
        if data_to_show.contains('battery_trend')
            var trend = data_to_show['battery_trend']
            var trend_icon = trend == "increasing" ? "📈" : (trend == "decreasing" ? "📉" : "➡️")
            fmt.add_status(string.toupper(trend), trend_icon, "Battery trend")
        end
        
        # Datalog info
        if data_to_show.contains('datalog_count')
            fmt.next_line()
            fmt.add_status(format("%d entries", data_to_show['datalog_count']), "📚", "Datalog entries")
            
            if data_to_show.contains('datalog_span_hours')
                var span = data_to_show['datalog_span_hours']
                if span < 1
                    fmt.add_status(format("%.0fm", span * 60), "⏱️", "Data span")
                else
                    fmt.add_status(format("%.1fh", span), "⏱️", "Data span")
                end
            end
        end
        
        # Error indicators
        if data_to_show.find('probe_error', false)
            fmt.next_line()
            var error_msg = data_to_show.find('probe_error_msg', "Probe Error")
            fmt.add_status(error_msg, "⚠️", "Probe error detected")
        end
        
        if data_to_show.find('unknown_fport', false)
            fmt.add_status(format("Unknown FPort %d", data_to_show['fport']), "❓", "Unknown message type")
        end
        
        fmt.end_line()
        msg += fmt.get_msg()
        
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
    
    def format_unix_time(timestamp)
        if timestamp == 0 return "Never" end
        
        # Simple Unix timestamp formatting
        var days_since_epoch = timestamp / 86400
        var year = 1970 + int(days_since_epoch / 365.25)
        var day_seconds = timestamp % 86400
        var hour = int(day_seconds / 3600)
        var minute = int((day_seconds % 3600) / 60)
        
        return format("%d-%02d:%02d", year, hour, minute)
    end
    
    def voltage_to_percent(voltage)
        # Battery voltage to percentage (Li-SOCI2 battery characteristics)
        if voltage >= 3.6 return 100
        elif voltage >= 3.3 return int((voltage - 3.3) / 0.3 * 80) + 20
        elif voltage >= 2.8 return int((voltage - 2.8) / 0.5 * 20)
        else return 0
    end
    
    # Get node statistics
    def get_node_stats(node_id)
        import global
        var node_data = global.PS_LB_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'battery_history': node_data.find('battery_history', []),
            'probe_model': node_data.find('probe_model', 0),
            'probe_type': node_data.find('probe_type', 'unknown'),
            'roc_count': node_data.find('roc_count', 0),
            'last_roc': node_data.find('last_roc', 0),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.PS_LB_nodes.contains(node_id)
            global.PS_LB_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Transmit Interval (TDC)
        tasmota.remove_cmd("LwPS_LBSetInterval")
        tasmota.add_cmd("LwPS_LBSetInterval", def(cmd, idx, payload_str)
            # Format: LwPS_LBSetInterval<node> <seconds>
            var interval = int(payload_str)
            if interval < 30 || interval > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 30-16777215 seconds")
            end
            
            # Build hex command: 01 + 3 bytes interval (big endian)
            var hex_cmd = format("01%06X", interval)
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Interrupt Mode
        tasmota.remove_cmd("LwPS_LBSetInterrupt")
        tasmota.add_cmd("LwPS_LBSetInterrupt", def(cmd, idx, payload_str)
            # Format: LwPS_LBSetInterrupt<node> <mode>
            # 0=disabled, 1=falling, 2=rising, 3=both
            return lwdecode.SendDownlinkMap(global.PS_LB_nodes, cmd, idx, payload_str, { 
                '0|DISABLED':  ['06000000', 'DISABLED'],
                '1|FALLING':   ['06000001', 'FALLING'],
                '2|RISING':    ['06000002', 'RISING'],
                '3|BOTH':      ['06000003', 'BOTH']
            })
        end)
        
        # Set Output Control (3V3/5V/12V)
        tasmota.remove_cmd("LwPS_LBSetOutput")
        tasmota.add_cmd("LwPS_LBSetOutput", def(cmd, idx, payload_str)
            # Format: LwPS_LBSetOutput<node> <type>,<duration_ms>
            # type: 1=3V3, 2=5V, 3=12V
            # duration: 0=normally_closed, 65535=normally_open, other=delay_ms
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwPS_LBSetOutput<node> <type>,<duration_ms>")
            end
            
            var output_type = int(parts[0])
            var duration = int(parts[1])
            
            if output_type < 1 || output_type > 3
                return tasmota.resp_cmnd_str("Invalid type: 1=3V3, 2=5V, 3=12V")
            end
            
            if duration < 0 || duration > 180000
                return tasmota.resp_cmnd_str("Invalid duration: 0-180000ms")
            end
            
            # Build hex command: 07 + type(1) + duration(4 bytes big endian)
            var hex_cmd = format("07%02X%08X", output_type, duration)
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Probe Model Configuration
        tasmota.remove_cmd("LwPS_LBSetProbe")
        tasmota.add_cmd("LwPS_LBSetProbe", def(cmd, idx, payload_str)
            # Format: LwPS_LBSetProbe<node> <probe_model_hex>
            # Example: 0003 = water depth 3m, 0101 = pressure type A
            var probe_model = int(payload_str, 16)
            if probe_model < 0 || probe_model > 0xFFFF
                return tasmota.resp_cmnd_str("Invalid probe model: use hex format (e.g., 0003, 0101)")
            end
            
            var hex_cmd = format("08%04X", probe_model)
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Report on Change (ROC) Mode
        tasmota.remove_cmd("LwPS_LBSetROC")
        tasmota.add_cmd("LwPS_LBSetROC", def(cmd, idx, payload_str)
            # Format: LwPS_LBSetROC<node> <mode>,<interval>,<idc_threshold>,<vdc_threshold>
            var parts = string.split(payload_str, ',')
            if size(parts) != 4
                return tasmota.resp_cmnd_str("Usage: LwPS_LBSetROC<node> <mode>,<interval_s>,<idc_uA>,<vdc_mV>")
            end
            
            var mode = int(parts[0])
            var interval = int(parts[1])
            var idc_threshold = int(parts[2])
            var vdc_threshold = int(parts[3])
            
            if mode < 0 || mode > 3
                return tasmota.resp_cmnd_str("Invalid mode: 0=disabled, 1=wave, 2=wave+refresh, 3=threshold")
            end
            
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str("Invalid interval: range 1-65535 seconds")
            end
            
            # Build hex command: 09 + mode(1) + interval(2) + idc_threshold(2) + vdc_threshold(2)
            var hex_cmd = format("09%02X%04X%04X%04X", mode, interval, idc_threshold, vdc_threshold)
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request Device Status
        tasmota.remove_cmd("LwPS_LBStatus")
        tasmota.add_cmd("LwPS_LBStatus", def(cmd, idx, payload_str)
            # Format: LwPS_LBStatus<node>
            var hex_cmd = "2601"
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Multi-Collection Mode
        tasmota.remove_cmd("LwPS_LBSetMulti")
        tasmota.add_cmd("LwPS_LBSetMulti", def(cmd, idx, payload_str)
            # Format: LwPS_LBSetMulti<node> <mode>,<interval>,<count>
            # mode: 0=disabled, 1=VDC, 2=IDC
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwPS_LBSetMulti<node> <mode>,<interval_s>,<count>")
            end
            
            var mode = int(parts[0])
            var interval = int(parts[1])
            var count = int(parts[2])
            
            if mode < 0 || mode > 2
                return tasmota.resp_cmnd_str("Invalid mode: 0=disabled, 1=VDC, 2=IDC")
            end
            
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str("Invalid interval: range 1-65535 seconds")
            end
            
            if count < 1 || count > 120
                return tasmota.resp_cmnd_str("Invalid count: range 1-120")
            end
            
            var hex_cmd = format("AE%02X%04X%02X", mode, interval, count)
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Poll Datalog
        tasmota.remove_cmd("LwPS_LBPollLog")
        tasmota.add_cmd("LwPS_LBPollLog", def(cmd, idx, payload_str)
            # Format: LwPS_LBPollLog<node> <start_timestamp>,<end_timestamp>,<interval>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwPS_LBPollLog<node> <start_ts>,<end_ts>,<interval_s>")
            end
            
            var start_ts = int(parts[0])
            var end_ts = int(parts[1])
            var interval = int(parts[2])
            
            if interval < 5 || interval > 255
                return tasmota.resp_cmnd_str("Invalid interval: range 5-255 seconds")
            end
            
            var hex_cmd = format("31%08X%08X%02X", start_ts, end_ts, interval)
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Clear Flash Record
        tasmota.remove_cmd("LwPS_LBClearLog")
        tasmota.add_cmd("LwPS_LBClearLog", def(cmd, idx, payload_str)
            # Format: LwPS_LBClearLog<node>
            var hex_cmd = "A301"
            return lwdecode.SendDownlink(global.PS_LB_nodes, cmd, idx, hex_cmd)
        end)
        
        print("PS-LB: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_PS_LB()

# Test command registration (recreated on each load)
# Command usage: LwPS_LBTestPayload<node> <hex_payload>
#                LwPS_LBTestPayload<node> <fport>,<hex_payload>
#                LwPS_LBTestPayload<node> <rssi>,<fport>,<hex_payload>
# Default: fport=<node>, rssi=-85

tasmota.remove_cmd("LwPS_LBTestPayload")
tasmota.add_cmd("LwPS_LBTestPayload", def(cmd, idx, payload_str)
    # Parse parameters: payload_str can be "hex", "fport,hex", or "rssi,fport,hex"
    var parts = string.split(payload_str, ',')
    var rssi = -85          # Default RSSI
    var fport = idx         # Default fport = node index
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
    var result = LwDeco.decodeUplink(format("PS-LB-%d", idx), idx, rssi, fport, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("LwPS_LBNodeStats")
tasmota.add_cmd("LwPS_LBNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwPS_LBClearNode")
tasmota.add_cmd("LwPS_LBClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
