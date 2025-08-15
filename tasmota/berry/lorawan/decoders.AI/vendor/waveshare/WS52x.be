#-------------------------------------------------------------
# LoRaWAN Decoder for Waveshare WS52x
# Generated: 2025-01-15 | Version: 1.0.0 | Revision: 1
# Originally Prompted by: ZioFabry
#-------------------------------------------------------------
# v1.0.0 (2025-01-15): Initial generation from PDF specification
#-------------------------------------------------------------

class LwDecode_WS52x
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # WS52x doesn't require CRC validation
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
            
            # Decode based on fport
            if fport == 10
                # Channel-based sensor data uplink
                var i = 0
                while i < size(payload)
                    if i + 1 >= size(payload)
                        break  # Not enough bytes for channel header
                    end
                    
                    var channel_id = payload[i]
                    var channel_type = payload[i+1]
                    i += 2
                    
                    # Temperature channel
                    if channel_id == 0x01 && channel_type == 0x67
                        if i + 1 < size(payload)
                            var temp_raw = (payload[i+1] << 8) | payload[i]
                            # Handle signed value
                            if temp_raw > 32767
                                temp_raw = temp_raw - 65536
                            end
                            data['temperature'] = temp_raw / 10.0
                            i += 2
                        else
                            break
                        end
                        
                    # Humidity channel
                    elif channel_id == 0x02 && channel_type == 0x68
                        if i < size(payload)
                            data['humidity'] = payload[i] / 2.0
                            i += 1
                        else
                            break
                        end
                        
                    # Barometric pressure channel
                    elif channel_id == 0x03 && channel_type == 0x73
                        if i + 1 < size(payload)
                            data['pressure'] = ((payload[i+1] << 8) | payload[i]) / 10.0
                            i += 2
                        else
                            break
                        end
                        
                    # Gas resistance channel (WS525 only)
                    elif channel_id == 0x04 && channel_type == 0x7D
                        if i + 1 < size(payload)
                            data['gas_resistance'] = ((payload[i+1] << 8) | payload[i]) / 10.0
                            i += 2
                        else
                            break
                        end
                        
                    # Battery channel
                    elif channel_id == 0x05 && channel_type == 0x75
                        if i + 1 < size(payload)
                            var battery_mv = ((payload[i+1] << 8) | payload[i])
                            data['battery_v'] = battery_mv / 1000.0
                            data['battery_pct'] = self.voltage_to_percent(data['battery_v'])
                            i += 2
                        else
                            break
                        end
                        
                    # Gyroscope X channel
                    elif channel_id == 0x06 && channel_type == 0x86
                        if i + 1 < size(payload)
                            var gyro_x = (payload[i+1] << 8) | payload[i]
                            if gyro_x > 32767
                                gyro_x = gyro_x - 65536
                            end
                            data['gyro_x'] = gyro_x / 100.0
                            i += 2
                        else
                            break
                        end
                        
                    # Gyroscope Y channel
                    elif channel_id == 0x07 && channel_type == 0x86
                        if i + 1 < size(payload)
                            var gyro_y = (payload[i+1] << 8) | payload[i]
                            if gyro_y > 32767
                                gyro_y = gyro_y - 65536
                            end
                            data['gyro_y'] = gyro_y / 100.0
                            i += 2
                        else
                            break
                        end
                        
                    # Gyroscope Z channel
                    elif channel_id == 0x08 && channel_type == 0x86
                        if i + 1 < size(payload)
                            var gyro_z = (payload[i+1] << 8) | payload[i]
                            if gyro_z > 32767
                                gyro_z = gyro_z - 65536
                            end
                            data['gyro_z'] = gyro_z / 100.0
                            i += 2
                        else
                            break
                        end
                        
                    # Accelerometer X channel
                    elif channel_id == 0x09 && channel_type == 0x71
                        if i + 1 < size(payload)
                            var accel_x = (payload[i+1] << 8) | payload[i]
                            if accel_x > 32767
                                accel_x = accel_x - 65536
                            end
                            data['accel_x'] = accel_x / 1000.0
                            i += 2
                        else
                            break
                        end
                        
                    # Accelerometer Y channel
                    elif channel_id == 0x0A && channel_type == 0x71
                        if i + 1 < size(payload)
                            var accel_y = (payload[i+1] << 8) | payload[i]
                            if accel_y > 32767
                                accel_y = accel_y - 65536
                            end
                            data['accel_y'] = accel_y / 1000.0
                            i += 2
                        else
                            break
                        end
                        
                    # Accelerometer Z channel
                    elif channel_id == 0x0B && channel_type == 0x71
                        if i + 1 < size(payload)
                            var accel_z = (payload[i+1] << 8) | payload[i]
                            if accel_z > 32767
                                accel_z = accel_z - 65536
                            end
                            data['accel_z'] = accel_z / 1000.0
                            i += 2
                        else
                            break
                        end
                        
                    # IAQ (Indoor Air Quality) channel - WS525 only
                    elif channel_id == 0x0C && channel_type == 0x7F
                        if i + 1 < size(payload)
                            data['iaq'] = (payload[i+1] << 8) | payload[i]
                            # IAQ scale: 0-50 Excellent, 51-100 Good, 101-150 Lightly Polluted,
                            # 151-200 Moderately Polluted, 201-250 Heavily Polluted, 251-350 Severely Polluted, >350 Extremely Polluted
                            i += 2
                        else
                            break
                        end
                        
                    # Light sensor channel - WS523 only
                    elif channel_id == 0x0D && channel_type == 0x65
                        if i + 1 < size(payload)
                            data['light'] = (payload[i+1] << 8) | payload[i]
                            i += 2
                        else
                            break
                        end
                        
                    # UV index channel - WS523 only
                    elif channel_id == 0x0E && channel_type == 0xCA
                        if i < size(payload)
                            data['uv_index'] = payload[i]
                            i += 1
                        else
                            break
                        end
                        
                    # Noise level channel - WS526 only
                    elif channel_id == 0x0F && channel_type == 0xDB
                        if i < size(payload)
                            data['noise_level'] = payload[i]
                            i += 1
                        else
                            break
                        end
                        
                    # Pitch angle channel
                    elif channel_id == 0x10 && channel_type == 0x90
                        if i + 1 < size(payload)
                            var pitch = (payload[i+1] << 8) | payload[i]
                            if pitch > 32767
                                pitch = pitch - 65536
                            end
                            data['pitch'] = pitch / 100.0
                            i += 2
                        else
                            break
                        end
                        
                    # Roll angle channel
                    elif channel_id == 0x11 && channel_type == 0x90
                        if i + 1 < size(payload)
                            var roll = (payload[i+1] << 8) | payload[i]
                            if roll > 32767
                                roll = roll - 65536
                            end
                            data['roll'] = roll / 100.0
                            i += 2
                        else
                            break
                        end
                        
                    # Yaw angle channel
                    elif channel_id == 0x12 && channel_type == 0x90
                        if i + 1 < size(payload)
                            var yaw = (payload[i+1] << 8) | payload[i]
                            if yaw > 32767
                                yaw = yaw - 65536
                            end
                            data['yaw'] = yaw / 100.0
                            i += 2
                        else
                            break
                        end
                        
                    else
                        # Unknown channel, log and skip
                        print(f"WS52x: Unknown channel ID={channel_id:02X} Type={channel_type:02X}")
                        # Try to guess size based on type (most are 2 bytes)
                        if channel_type == 0x68 || channel_type == 0xCA || channel_type == 0xDB
                            i += 1  # Single byte channels
                        else
                            i += 2  # Default to 2 bytes
                        end
                    end
                end
                
            elif fport == 15
                # Configuration/Status uplink (acknowledgment)
                if size(payload) >= 1
                    var cmd_type = payload[0]
                    data['config_ack'] = true
                    data['config_cmd'] = cmd_type
                    
                    if cmd_type == 0x01 && size(payload) >= 3
                        # Reporting interval acknowledgment
                        data['report_interval'] = (payload[2] << 8) | payload[1]
                    elif cmd_type == 0x02 && size(payload) >= 2
                        # Motion detection threshold acknowledgment
                        data['motion_threshold'] = payload[1]
                    elif cmd_type == 0x03 && size(payload) >= 2
                        # Sensor enable/disable acknowledgment
                        data['sensors_enabled'] = payload[1]
                    elif cmd_type == 0xFF
                        # Device reset acknowledgment
                        data['device_reset'] = true
                    end
                end
                
            else
                print(f"WS52x: Unknown fport {fport}")
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
        
        # Primary sensors line
        msg += fmt.start_line()
        if data_to_show.contains('battery_pct')
            fmt.add_sensor("battery", data_to_show['battery_pct'], "%", "🔋")
        end
        if data_to_show.contains('temperature')
            fmt.add_sensor("temperature", data_to_show['temperature'], "°C", "🌡️")
        end
        if data_to_show.contains('humidity')
            fmt.add_sensor("humidity", data_to_show['humidity'], "%", "💧")
        end
        if data_to_show.contains('pressure')
            fmt.add_sensor("pressure", data_to_show['pressure'], "hPa", "🔵")
        end
        if data_to_show.contains('rssi')
            fmt.add_sensor("rssi", data_to_show['rssi'], "dBm", "📶")
        end
        msg += fmt.end_line().get_msg()
        
        # Air quality sensors (if present)
        if data_to_show.contains('iaq') || data_to_show.contains('gas_resistance')
            msg += fmt.start_line()
            if data_to_show.contains('iaq')
                fmt.add_sensor("iaq", data_to_show['iaq'], "", "💨")
            end
            if data_to_show.contains('gas_resistance')
                fmt.add_sensor("gas", data_to_show['gas_resistance'], "kΩ", "⚗️")
            end
            msg += fmt.end_line().get_msg()
        end
        
        # Environmental sensors (if present)
        if data_to_show.contains('light') || data_to_show.contains('uv_index') || data_to_show.contains('noise_level')
            msg += fmt.start_line()
            if data_to_show.contains('light')
                fmt.add_sensor("light", data_to_show['light'], "lux", "💡")
            end
            if data_to_show.contains('uv_index')
                fmt.add_sensor("uv", data_to_show['uv_index'], "", "☀️")
            end
            if data_to_show.contains('noise_level')
                fmt.add_sensor("noise", data_to_show['noise_level'], "dB", "🔊")
            end
            msg += fmt.end_line().get_msg()
        end
        
        # Motion sensors (if present)
        if data_to_show.contains('pitch') || data_to_show.contains('roll') || data_to_show.contains('yaw')
            msg += fmt.start_line()
            if data_to_show.contains('pitch')
                fmt.add_sensor("pitch", data_to_show['pitch'], "°", "↕️")
            end
            if data_to_show.contains('roll')
                fmt.add_sensor("roll", data_to_show['roll'], "°", "↔️")
            end
            if data_to_show.contains('yaw')
                fmt.add_sensor("yaw", data_to_show['yaw'], "°", "🔄")
            end
            msg += fmt.end_line().get_msg()
        end
        
        # Status indicators
        if data_to_show.contains('device_reset') && data_to_show['device_reset']
            msg += fmt.start_line()
                .add_status("Reset detected", "⚠️")
                .end_line()
                .get_msg()
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                msg += fmt.start_line()
                    .add_status(self.format_age(age), "⏱️")
                    .end_line()
                    .get_msg()
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
    
    # Battery voltage to percentage (LiPo/Li-ion curve)
    def voltage_to_percent(voltage)
        if voltage >= 4.2 return 100
        elif voltage <= 3.0 return 0
        else
            # Linear approximation
            return int((voltage - 3.0) / 1.2 * 100)
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
            'battery_history': node_data.find('battery_history', []),
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
        
        # Set reporting interval command
        tasmota.remove_cmd("WS52xInterval")
        tasmota.add_cmd("WS52xInterval", def(cmd, idx, payload_str)
            # Format: WS52xInterval<node> <seconds>
            # Valid range: 1-65535 seconds
            var interval = int(payload_str)
            if interval < 1 || interval > 65535
                return tasmota.resp_cmnd_str(f"Invalid: range 1-65535 seconds")
            end
            
            # Build hex command: 01 + 2 bytes little endian interval
            var hex_cmd = f"01{interval & 0xFF:02X}{(interval >> 8) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set motion detection threshold
        tasmota.remove_cmd("WS52xMotion")
        tasmota.add_cmd("WS52xMotion", def(cmd, idx, payload_str)
            # Format: WS52xMotion<node> <threshold>
            # Valid range: 0-255 (0=disabled)
            var threshold = int(payload_str)
            if threshold < 0 || threshold > 255
                return tasmota.resp_cmnd_str(f"Invalid: range 0-255")
            end
            
            # Build hex command: 02 + threshold byte
            var hex_cmd = f"02{threshold:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Enable/disable sensors
        tasmota.remove_cmd("WS52xSensors")
        tasmota.add_cmd("WS52xSensors", def(cmd, idx, payload_str)
            # Format: WS52xSensors<node> <bitmap>
            # Bitmap: bit 0=temp, 1=humidity, 2=pressure, 3=gas, 4=battery, 5=gyro, 6=accel, 7=angles
            var bitmap = int(payload_str)
            if bitmap < 0 || bitmap > 255
                return tasmota.resp_cmnd_str(f"Invalid: range 0-255 (bitmap)")
            end
            
            # Build hex command: 03 + bitmap byte
            var hex_cmd = f"03{bitmap:02X}"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Device reset command
        tasmota.remove_cmd("WS52xReset")
        tasmota.add_cmd("WS52xReset", def(cmd, idx, payload_str)
            # Format: WS52xReset<node>
            # Send reset command (no parameters needed)
            var hex_cmd = "FF"
            return lwdecode.SendDownlink(global.WS52x_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request device status
        tasmota.remove_cmd("WS52xStatus")
        tasmota.add_cmd("WS52xStatus", def(cmd, idx, payload_str)
            # Format: WS52xStatus<node>
            # Request current configuration
            var hex_cmd = "FE"
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
