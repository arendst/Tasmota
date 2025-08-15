# -------------------------------------------------------------
# Originally Prompted by: ZioFabry
#
# LoRaWAN AI-Generated Decoder for Milesight AM300
#
# Generated: 2025-08-15 | Version: 1.1.0 | Revision: 2
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.milesight.com/iot/product/lorawan-sensor/am300
# - Userguide: https://resource.milesight-iot.com/milesight/iot/document/am300-user-guide.pdf
# - Decoder:   https://github.com/Milesight-IoT/SensorDecoders
# -------------------------------------------------------------
# v1.1.0 (2025-08-15): Regenerated with AI template v2.1.8
#                      - Enhanced framework integration
#                      - Added comprehensive air quality formatters
#                      - Improved global storage handling
#                      - Added comprehensive downlink commands
# v1.0.0 (2025-08-14): Initial generation from PDF specification
# -------------------------------------------------------------

class LwDecode_AM300
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
        if !global.contains("AM300_nodes")
            global.AM300_nodes = {}
        end
        if !global.contains("AM300_cmdInit")
            global.AM300_cmdInit = false
        end
        
        # Expand framework with comprehensive air quality formatters
        LwSensorFormatter_cls.Formatter["temperature"] = {"u": "°C", "f": " %.1f", "i": "🌡️"}
        LwSensorFormatter_cls.Formatter["humidity"] = {"u": "%RH", "f": " %.0f", "i": "💧"}
        LwSensorFormatter_cls.Formatter["co2"] = {"u": "ppm", "f": " %d", "i": "🌬️"}
        LwSensorFormatter_cls.Formatter["tvoc"] = {"u": "ppb", "f": " %d", "i": "🏭"}
        LwSensorFormatter_cls.Formatter["pressure"] = {"u": "hPa", "f": " %.0f", "i": "📊"}
        LwSensorFormatter_cls.Formatter["pm25"] = {"u": "μg/m³", "f": " %d", "i": "🌫️"}
        LwSensorFormatter_cls.Formatter["pm10"] = {"u": "μg/m³", "f": " %d", "i": "💨"}
        LwSensorFormatter_cls.Formatter["hcho"] = {"u": "mg/m³", "f": " %.3f", "i": "🧪"}
        LwSensorFormatter_cls.Formatter["o3"] = {"u": "ppm", "f": " %.3f", "i": "⚗️"}
        LwSensorFormatter_cls.Formatter["illuminance"] = {"u": "lux", "f": " %d", "i": "💡"}
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
            var node_data = global.AM300_nodes.find(node, {})
            
            # Decode based on fport 85 (AM300 uses single port)
            if fport == 85
                var i = 0
                while i < size(payload)
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # Temperature (0x03, 0x67) - 2 bytes signed, 0.1°C resolution
                    if channel_id == 0x03 && channel_type == 0x67 && i + 1 < size(payload)
                        var temp_raw = (payload[i+1] << 8) | payload[i]
                        # Handle signed 16-bit
                        if temp_raw > 32767
                            temp_raw = temp_raw - 65536
                        end
                        data['temperature'] = temp_raw / 10.0
                        i += 2
                        
                    # Humidity (0x04, 0x68) - 1 byte, 0.5% resolution
                    elif channel_id == 0x04 && channel_type == 0x68 && i < size(payload)
                        data['humidity'] = payload[i] / 2.0
                        i += 1
                        
                    # CO2 (0x07, 0x7D) - 2 bytes, ppm
                    elif channel_id == 0x07 && channel_type == 0x7D && i + 1 < size(payload)
                        var co2 = (payload[i+1] << 8) | payload[i]
                        data['co2'] = co2
                        i += 2
                        
                    # TVOC (0x08, 0x7D) - 2 bytes, ppb
                    elif channel_id == 0x08 && channel_type == 0x7D && i + 1 < size(payload)
                        var tvoc = (payload[i+1] << 8) | payload[i]
                        data['tvoc'] = tvoc
                        i += 2
                        
                    # Barometric Pressure (0x09, 0x73) - 2 bytes, 0.1 hPa resolution
                    elif channel_id == 0x09 && channel_type == 0x73 && i + 1 < size(payload)
                        var pressure = ((payload[i+1] << 8) | payload[i]) / 10.0
                        data['pressure'] = pressure
                        i += 2
                        
                    # PM2.5 (0x0A, 0x7E) - 2 bytes, μg/m³
                    elif channel_id == 0x0A && channel_type == 0x7E && i + 1 < size(payload)
                        var pm25 = (payload[i+1] << 8) | payload[i]
                        data['pm25'] = pm25
                        i += 2
                        
                    # PM10 (0x0B, 0x7E) - 2 bytes, μg/m³
                    elif channel_id == 0x0B && channel_type == 0x7E && i + 1 < size(payload)
                        var pm10 = (payload[i+1] << 8) | payload[i]
                        data['pm10'] = pm10
                        i += 2
                        
                    # HCHO (0x0C, 0x7F) - 2 bytes, 0.001 mg/m³ resolution
                    elif channel_id == 0x0C && channel_type == 0x7F && i + 1 < size(payload)
                        var hcho = ((payload[i+1] << 8) | payload[i]) / 1000.0
                        data['hcho'] = hcho
                        i += 2
                        
                    # O3 (0x0D, 0x7F) - 2 bytes, 0.001 ppm resolution
                    elif channel_id == 0x0D && channel_type == 0x7F && i + 1 < size(payload)
                        var o3 = ((payload[i+1] << 8) | payload[i]) / 1000.0
                        data['o3'] = o3
                        i += 2
                        
                    # Light Sensor (0x0E, 0x65) - 2 bytes, lux
                    elif channel_id == 0x0E && channel_type == 0x65 && i + 1 < size(payload)
                        var illuminance = (payload[i+1] << 8) | payload[i]
                        data['illuminance'] = illuminance
                        i += 2
                        
                    # PIR Motion (0x0F, 0x00) - 1 byte
                    elif channel_id == 0x0F && channel_type == 0x00 && i < size(payload)
                        data['pir_motion'] = payload[i] == 0x01
                        data['motion_text'] = payload[i] == 0x01 ? "OCCUPIED" : "VACANT"
                        i += 1
                        
                    # Buzzer Status (0x10, 0x01) - 1 byte
                    elif channel_id == 0x10 && channel_type == 0x01 && i < size(payload)
                        data['buzzer_status'] = payload[i] == 0x01
                        data['buzzer_text'] = payload[i] == 0x01 ? "ON" : "OFF"
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
            if !global.contains("AM300_cmdInit") || !global.AM300_cmdInit
                self.register_downlink_commands()
                global.AM300_cmdInit = true
            end

            # Save back to global storage
            global.AM300_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"LwDecode_AM300 error: {m}")
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
            var node_data = global.AM300_nodes.find(self.node, {})
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
            name = f"AM300-{self.node}"
        end
        var name_tooltip = "Milesight AM300 8-in-1 Air Quality Monitor"
        var battery = data_to_show.find('battery_v', 1000)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        
        msg = msg + lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update)
        
        # Line 1: Basic environmental conditions
        fmt.start_line()
        if data_to_show.contains('temperature')
            fmt.add_sensor("temperature", data_to_show['temperature'], "Temperature", nil)
        end
        if data_to_show.contains('humidity')
            fmt.add_sensor("humidity", data_to_show['humidity'], "Humidity", nil)
        end
        if data_to_show.contains('pressure')
            fmt.add_sensor("pressure", data_to_show['pressure'], "Pressure", nil)
        end
        
        # Line 2: Air quality gases
        fmt.next_line()
        if data_to_show.contains('co2')
            fmt.add_sensor("co2", data_to_show['co2'], "CO2", nil)
        end
        if data_to_show.contains('tvoc')
            fmt.add_sensor("tvoc", data_to_show['tvoc'], "TVOC", nil)
        end
        if data_to_show.contains('hcho')
            fmt.add_sensor("hcho", data_to_show['hcho'], "HCHO", nil)
        end
        
        # Line 3: Particulate matter and other sensors
        fmt.next_line()
        if data_to_show.contains('pm25')
            fmt.add_sensor("pm25", data_to_show['pm25'], "PM2.5", nil)
        end
        if data_to_show.contains('pm10')
            fmt.add_sensor("pm10", data_to_show['pm10'], "PM10", nil)
        end
        if data_to_show.contains('o3')
            fmt.add_sensor("o3", data_to_show['o3'], "O3", nil)
        end
        
        # Line 4: Motion, Light, Buzzer (if present)
        var has_sensors_4 = data_to_show.contains('pir_motion') || 
                           data_to_show.contains('illuminance') || 
                           data_to_show.contains('buzzer_status')
        if has_sensors_4
            fmt.next_line()
            
            if data_to_show.contains('pir_motion')
                var motion_emoji = data_to_show['pir_motion'] ? "🟢" : "⚫"
                var motion_text = data_to_show.find('motion_text', "UNKNOWN")
                fmt.add_sensor("string", motion_text, "Occupancy", motion_emoji)
            end
            
            if data_to_show.contains('illuminance')
                fmt.add_sensor("illuminance", data_to_show['illuminance'], "Light", nil)
            end
            
            if data_to_show.contains('buzzer_status')
                var buzzer_emoji = data_to_show['buzzer_status'] ? "🔊" : "🔇"
                var buzzer_text = data_to_show.find('buzzer_text', "UNKNOWN")
                fmt.add_sensor("string", buzzer_text, "Buzzer", buzzer_emoji)
            end
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
        var node_data = global.AM300_nodes.find(node_id, nil)
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
        if global.AM300_nodes.contains(node_id)
            global.AM300_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Reporting Interval
        tasmota.remove_cmd("LwAM300Interval")
        tasmota.add_cmd("LwAM300Interval", def(cmd, idx, payload_str)
            var minutes = int(payload_str)
            if minutes < 1 || minutes > 65535
                return tasmota.resp_cmnd_str("Invalid: range 1-65535 minutes")
            end
            var hex_cmd = f"FE02{lwdecode.uint16le(minutes)}"
            return lwdecode.SendDownlink(global.AM300_nodes, cmd, idx, hex_cmd)
        end)
        
        # Buzzer Control
        tasmota.remove_cmd("LwAM300Buzzer")
        tasmota.add_cmd("LwAM300Buzzer", def(cmd, idx, payload_str)
            return lwdecode.SendDownlinkMap(global.AM300_nodes, cmd, idx, payload_str, { 
                '1|ON|ENABLE':   ['FF2801', 'ENABLED'],
                '0|OFF|DISABLE': ['FF2800', 'DISABLED']
            })
        end)
        
        # Device Reboot
        tasmota.remove_cmd("LwAM300Reboot")
        tasmota.add_cmd("LwAM300Reboot", def(cmd, idx, payload_str)
            var hex_cmd = "FF10FF"
            return lwdecode.SendDownlink(global.AM300_nodes, cmd, idx, hex_cmd)
        end)
        
        print("AM300: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_AM300()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwAM300TestPayload")
tasmota.add_cmd("LwAM300TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestAM300", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("LwAM300NodeStats")
tasmota.add_cmd("LwAM300NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwAM300ClearNode")
tasmota.add_cmd("LwAM300ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
