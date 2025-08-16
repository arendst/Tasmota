# -------------------------------------------------------------
# Originally Prompted by: User Request
#
# LoRaWAN AI-Generated Decoder for Dragino LDS02
#
# Generated: 2025-08-16 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.1.8
#
# Official Links
# - Homepage:  https://www.dragino.com/products/lora-lorawan-end-device/item/137-lds02.html
# - Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/LDS02%20-%20LoRaWAN%20Door%20Sensor%20User%20Manual/
# - Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/LDS02
# -------------------------------------------------------------
# CHANGELOG
# v1.0.0 (2025-08-16): Initial generation from PDF specification
# -------------------------------------------------------------

class LwDecode_LDS02
    var hashCheck       # Duplicate payload detection flag (true = skip duplicates)
    var crcCheck        # CRC validation flag (if required by specs)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    
    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.crcCheck = false   # No CRC validation per PDF specs
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("LDS02_nodes")
            global.LDS02_nodes = {}
        end
        if !global.contains("LDS02_cmdInit")
            global.LDS02_cmdInit = false
        end
        
        # Add custom formatters for this sensor
        LwSensorFormatter_cls.Formatter["door_events"] = {"u": " events", "f": " %d", "i": "🔢"}
        LwSensorFormatter_cls.Formatter["duration"] = {"u": "min", "f": " %d", "i": "⏱️"}
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
            var node_data = global.LDS02_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 10
                # Sensor Data Uplink (10 bytes)
                if size(payload) >= 10
                    # Battery & Door Status (bytes 0-1)
                    var bat_door_raw = (payload[1] << 8) | payload[0]
                    
                    # Battery voltage (lower 14 bits)
                    var battery_mv = bat_door_raw & 0x3FFF
                    data['battery_v'] = battery_mv / 1000.0
                    
                    # Door status (bit 15)
                    var door_bit = (bat_door_raw & 0x8000) >> 15
                    data['door_open'] = door_bit == 1
                    data['door_status'] = door_bit == 1 ? "Open" : "Closed"
                    
                    # Mode (byte 2)
                    var mode = payload[2]
                    data['mode'] = mode
                    data['mode_name'] = mode == 0x01 ? "Normal" : f"Mode-{mode:02X}"
                    
                    # Total Door Open Events (bytes 3-5)
                    var total_events = (payload[5] << 16) | (payload[4] << 8) | payload[3]
                    data['total_open_events'] = total_events
                    
                    # Last Door Open Duration (bytes 6-8)
                    var last_duration = (payload[8] << 16) | (payload[7] << 8) | payload[6]
                    data['last_open_duration'] = last_duration
                    
                    # Alarm Status (byte 9)
                    var alarm = payload[9] & 0x01
                    data['alarm'] = alarm == 1
                    data['alarm_status'] = alarm == 1 ? "Timeout alarm" : "No alarm"
                    
                    # Calculate door state change
                    var last_door_state = node_data.find('last_door_open', nil)
                    if last_door_state != nil && last_door_state != data['door_open']
                        data['state_changed'] = true
                        data['event_type'] = data['door_open'] ? "Opened" : "Closed"
                    else
                        data['state_changed'] = false
                        data['event_type'] = "Keep-alive"
                    end
                    
                    # Store door state history
                    node_data['last_door_open'] = data['door_open']
                end
                
            elif fport == 7
                # EDC Mode Data Uplink (5 bytes)
                if size(payload) >= 5
                    # Battery & EDC Mode (bytes 0-1)
                    var bat_edc_raw = (payload[1] << 8) | payload[0]
                    
                    # Battery voltage (lower 14 bits)
                    var battery_mv = bat_edc_raw & 0x3FFF
                    data['battery_v'] = battery_mv / 1000.0
                    
                    # EDC Mode (bit 15)
                    var edc_bit = (bat_edc_raw & 0x8000) >> 15
                    data['edc_mode'] = edc_bit
                    data['edc_mode_name'] = edc_bit == 1 ? "Open count" : "Close count"
                    
                    # Event Count (bytes 2-4)
                    var event_count = (payload[4] << 16) | (payload[3] << 8) | payload[2]
                    data['event_count'] = event_count
                    
                    # Mark as EDC mode
                    data['is_edc_mode'] = true
                end
            end
            
            # Register downlink commands if not already done
            if !global.contains("LDS02_cmdInit") || !global.LDS02_cmdInit
                self.register_downlink_commands()
                global.LDS02_cmdInit = true
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
            
            # Store door event trends
            if data.contains('total_open_events')
                if !node_data.contains('events_history')
                    node_data['events_history'] = []
                end
                node_data['events_history'].push(data['total_open_events'])
                if size(node_data['events_history']) > 20
                    node_data['events_history'].pop(0)
                end
            end
            
            # Track alarm events
            if data.find('alarm', false)
                node_data['alarm_count'] = node_data.find('alarm_count', 0) + 1
                node_data['last_alarm'] = tasmota.rtc()['local']
            end
            
            # Save back to global storage
            global.LDS02_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            lwdecode.log_error("DECODE_LDS02", e, m, 
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
            var node_data = global.LDS02_nodes.find(self.node, {})
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
            name = f"LDS02-{self.node}"
        end
        var name_tooltip = "Dragino LDS02 Door Sensor"
        var battery = data_to_show.find('battery_v', 1000)  # Use 1000 if no battery
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)  # Use 1000 if no RSSI
        
        msg += lwdecode.header(name, name_tooltip, battery * 1000, battery_last_seen, rssi, last_update)
        
        # Build display using emoji formatter
        fmt.start_line()
        
        # Door status (primary indicator)
        if data_to_show.contains('door_open')
            var door_open = data_to_show['door_open']
            var door_icon = door_open ? "🔓" : "🔒"
            var door_text = door_open ? "Open" : "Closed"
            fmt.add_status(door_text, door_icon, "Door status")
        end
        
        # Event information for normal mode
        if !data_to_show.find('is_edc_mode', false)
            # Total events
            if data_to_show.contains('total_open_events')
                var events = data_to_show['total_open_events']
                fmt.add_sensor("door_events", events, "Total openings", "🔢")
            end
            
            # Last open duration
            if data_to_show.contains('last_open_duration') && data_to_show['last_open_duration'] > 0
                var duration = data_to_show['last_open_duration']
                fmt.add_sensor("duration", duration, "Last open time", "⏱️")
            end
            
            # Alarm status
            if data_to_show.find('alarm', false)
                fmt.add_status("Alarm", "⚠️", "Timeout alarm active")
            end
            
            # Event type indicator
            if data_to_show.contains('event_type')
                var event_type = data_to_show['event_type']
                var event_icon = "📅"
                if event_type == "Opened"
                    event_icon = "🔓"
                elif event_type == "Closed"
                    event_icon = "🔒"
                end
                fmt.add_status(event_type, event_icon, "Event trigger type")
            end
        else
            # EDC mode display
            fmt.next_line()
            fmt.add_status("EDC", "🔄", "Event-driven counting mode")
            
            # EDC mode type
            if data_to_show.contains('edc_mode_name')
                var mode_name = data_to_show['edc_mode_name']
                var mode_icon = data_to_show['edc_mode'] == 1 ? "🔓" : "🔒"
                fmt.add_status(mode_name, mode_icon, "Counting mode")
            end
            
            # Event count
            if data_to_show.contains('event_count')
                var count = data_to_show['event_count']
                fmt.add_sensor("door_events", count, "Event count", "🔢")
            end
        end
        
        fmt.end_line()
        msg += fmt.get_msg()
        
        # Add age info if data is old
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
        var node_data = global.LDS02_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'alarm_count': node_data.find('alarm_count', 0),
            'last_alarm': node_data.find('last_alarm', 0),
            'battery_history': node_data.find('battery_history', []),
            'events_history': node_data.find('events_history', []),
            'last_door_open': node_data.find('last_door_open', nil),
            'name': node_data.find('name', 'Unknown')
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.LDS02_nodes.contains(node_id)
            global.LDS02_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set Transmit Interval (0x01)
        tasmota.remove_cmd("LwLDS02Interval")
        tasmota.add_cmd("LwLDS02Interval", def(cmd, idx, payload_str)
            # Format: LwLDS02Interval<node> <seconds>
            var interval = int(payload_str)
            if interval < 1 || interval > 4294967295
                return tasmota.resp_cmnd_str("Invalid: range 1-4294967295 seconds")
            end
            
            # Build hex command: 01 + 4 bytes little endian
            var hex_cmd = f"01{interval & 0xFF:02X}{(interval >> 8) & 0xFF:02X}{(interval >> 16) & 0xFF:02X}{(interval >> 24) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set EDC Mode (0x02)
        tasmota.remove_cmd("LwLDS02EDC")
        tasmota.add_cmd("LwLDS02EDC", def(cmd, idx, payload_str)
            # Format: LwLDS02EDC<node> <mode>,<count>
            # Example: LwLDS02EDC1 1,10 (open count mode, trigger every 10 events)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwLDS02EDC<node> <mode>,<count>")
            end
            
            var mode = int(parts[0])
            var count = int(parts[1])
            
            # Validate parameters
            if mode < 0 || mode > 1
                return tasmota.resp_cmnd_str("Invalid mode: 0=close_count, 1=open_count")
            end
            if count < 1 || count > 4294967295
                return tasmota.resp_cmnd_str("Invalid count: range 1-4294967295")
            end
            
            # Build hex command: 02 + mode(1) + count(4, little endian)
            var hex_cmd = f"02{mode:02X}{count & 0xFF:02X}{(count >> 8) & 0xFF:02X}{(count >> 16) & 0xFF:02X}{(count >> 24) & 0xFF:02X}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Reset Device (0x04)
        tasmota.remove_cmd("LwLDS02Reset")
        tasmota.add_cmd("LwLDS02Reset", def(cmd, idx, payload_str)
            # Format: LwLDS02Reset<node>
            var hex_cmd = "04FF"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Confirmed Mode (0x05)
        tasmota.remove_cmd("LwLDS02Confirm")
        tasmota.add_cmd("LwLDS02Confirm", def(cmd, idx, payload_str)
            # Format: LwLDS02Confirm<node> <unconfirmed|confirmed>
            return lwdecode.SendDownlinkMap(global.LDS02_nodes, cmd, idx, payload_str, { 
                'UNCONFIRMED|0|OFF': ['0500', 'Unconfirmed'],
                'CONFIRMED|1|ON': ['0501', 'Confirmed']
            })
        end)
        
        # Clear Counting (0xA6)
        tasmota.remove_cmd("LwLDS02Clear")
        tasmota.add_cmd("LwLDS02Clear", def(cmd, idx, payload_str)
            # Format: LwLDS02Clear<node>
            var hex_cmd = "A601"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Enable/Disable Alarm (0xA7)
        tasmota.remove_cmd("LwLDS02Alarm")
        tasmota.add_cmd("LwLDS02Alarm", def(cmd, idx, payload_str)
            # Format: LwLDS02Alarm<node> <enable|disable>
            return lwdecode.SendDownlinkMap(global.LDS02_nodes, cmd, idx, payload_str, { 
                'ENABLE|1|ON': ['A701', 'Enabled'],
                'DISABLE|0|OFF': ['A700', 'Disabled']
            })
        end)
        
        # Control ADR/DR (0xA8)
        tasmota.remove_cmd("LwLDS02ADR")
        tasmota.add_cmd("LwLDS02ADR", def(cmd, idx, payload_str)
            # Format: LwLDS02ADR<node> <adr_enable>,<data_rate>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwLDS02ADR<node> <adr_enable>,<data_rate>")
            end
            
            var adr = int(parts[0])
            var dr = int(parts[1])
            
            # Validate parameters
            if adr < 0 || adr > 1
                return tasmota.resp_cmnd_str("Invalid ADR: 0=disable, 1=enable")
            end
            if dr < 0 || dr > 15
                return tasmota.resp_cmnd_str("Invalid DR: range 0-15")
            end
            
            # Build hex command: A8 + adr(1) + dr(1)
            var hex_cmd = f"A8{adr:02X}{dr:02X}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Alarm Timeout (0xA9)
        tasmota.remove_cmd("LwLDS02Timeout")
        tasmota.add_cmd("LwLDS02Timeout", def(cmd, idx, payload_str)
            # Format: LwLDS02Timeout<node> <status>,<timeout_seconds>
            # Example: LwLDS02Timeout1 1,30 (monitor open status, 30 second timeout)
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwLDS02Timeout<node> <status>,<timeout_s>")
            end
            
            var status = int(parts[0])
            var timeout = int(parts[1])
            
            # Validate parameters
            if status < 0 || status > 1
                return tasmota.resp_cmnd_str("Invalid status: 0=disable, 1=open_timeout")
            end
            if timeout < 0 || timeout > 65535
                return tasmota.resp_cmnd_str("Invalid timeout: range 0-65535 seconds")
            end
            
            # Build hex command: A9 + status(1) + timeout(2, big endian)
            var hex_cmd = f"A9{status:02X}{(timeout >> 8) & 0xFF:02X}{timeout & 0xFF:02X}"
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set Count Value (0xAA)
        tasmota.remove_cmd("LwLDS02SetCount")
        tasmota.add_cmd("LwLDS02SetCount", def(cmd, idx, payload_str)
            # Format: LwLDS02SetCount<node> <count> or LwLDS02SetCount<node> <mode>,<count>
            var parts = string.split(payload_str, ',')
            var hex_cmd = ""
            
            if size(parts) == 1
                # Normal mode: AA + count(3, big endian)
                var count = int(parts[0])
                if count < 0 || count > 16777215
                    return tasmota.resp_cmnd_str("Invalid count: range 0-16777215")
                end
                hex_cmd = f"AA{(count >> 16) & 0xFF:02X}{(count >> 8) & 0xFF:02X}{count & 0xFF:02X}"
                
            elif size(parts) == 2
                # EDC mode: AA + mode(1) + count(3, big endian)
                var mode = int(parts[0])
                var count = int(parts[1])
                
                if mode < 0 || mode > 1
                    return tasmota.resp_cmnd_str("Invalid mode: 0=close, 1=open")
                end
                if count < 0 || count > 16777215
                    return tasmota.resp_cmnd_str("Invalid count: range 0-16777215")
                end
                
                hex_cmd = f"AA{mode:02X}{(count >> 16) & 0xFF:02X}{(count >> 8) & 0xFF:02X}{count & 0xFF:02X}"
            else
                return tasmota.resp_cmnd_str("Usage: LwLDS02SetCount<node> <count> or <mode>,<count>")
            end
            
            return lwdecode.SendDownlink(global.LDS02_nodes, cmd, idx, hex_cmd)
        end)
        
        print("LDS02: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_LDS02()

# Test command registration (recreated on each load)
tasmota.remove_cmd("LwLDS02TestPayload")
tasmota.add_cmd("LwLDS02TestPayload", def(cmd, idx, payload_str)
    # Parse hex string to bytes
    var test_payload = bytes(payload_str)
    
    # Force driver load by LwDecode framework
    var result = LwDeco.decodeUplink("TestLDS02", "test_node", -85, idx, test_payload)
    
    if result != nil
        import json
        tasmota.resp_cmnd(json.dump(result))
    else
        tasmota.resp_cmnd_error()
    end
end)

# Node management commands
tasmota.remove_cmd("LwLDS02NodeStats")
tasmota.add_cmd("LwLDS02NodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwLDS02ClearNode")
tasmota.add_cmd("LwLDS02ClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)
