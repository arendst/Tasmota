#
# LoRaWAN AI-Generated Decoder for Dragino SN50v3-LB Prompted by User Request
#
# Generated: 2025-08-20 | Version: 1.0.0 | Revision: 1
#            by "LoRaWAN Decoder AI Generation Template", v2.3.3
#
# Homepage:  https://www.dragino.com/products/lora-lorawan-end-node/item/260-sn50v3-lb-ls.html
# Userguide: https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SN50v3-LB/
# Decoder:   https://github.com/dragino/dragino-end-node-decoder/tree/main/SN50_v3-LB
# 
# v1.0.0 (2025-08-20): Initial generation from wiki specification

class LwDecode_SN50v3LB
    var hashCheck      # Duplicate payload detection flag (true = skip duplicates)
    var name           # Device name from LoRaWAN
    var node           # Node identifier
    var last_data      # Cached decoded data
    var last_update    # Timestamp of last update
    var lwdecode       # global instance of the driver

    def init()
        self.hashCheck = true   # Enable duplicate detection by default
        self.name = nil
        self.node = nil
        self.last_data = {}
        self.last_update = 0
        
        # Initialize global node storage (survives decoder reload)
        import global
        if !global.contains("SN50v3LB_nodes")
            global.SN50v3LB_nodes = {}
        end
        if !global.contains("SN50v3LB_cmdInit")
            global.SN50v3LB_cmdInit = false
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
            var node_data = global.SN50v3LB_nodes.find(node, {})
            
            # Decode based on fport
            if fport == 5
                # Device status
                if size(payload) >= 7
                    data = self.decode_device_status(payload, data)
                else
                    print(f"SN50v3LB: Invalid device status payload size: {size(payload)}")
                    return nil
                end
                
            elif fport == 2
                # Sensor data - determine working mode from flags byte
                if size(payload) >= 7
                    # Extract working mode from 7th byte (index 6)
                    var flags_byte = payload[6]
                    var working_mode = (flags_byte >> 2) & 0x1F
                    data['working_mode'] = working_mode
                    
                    if working_mode == 0  # MOD=1 (default)
                        data = self.decode_mode_1(payload, data)
                    elif working_mode == 1  # MOD=2 (distance)
                        data = self.decode_mode_2(payload, data)
                    elif working_mode == 2  # MOD=3 (3 ADC + I2C)
                        data = self.decode_mode_3(payload, data)
                    elif working_mode == 3  # MOD=4 (3 DS18B20)
                        data = self.decode_mode_4(payload, data)
                    elif working_mode == 4  # MOD=5 (weight)
                        data = self.decode_mode_5(payload, data)
                    elif working_mode == 5  # MOD=6 (counting)
                        data = self.decode_mode_6(payload, data)
                    elif working_mode == 6  # MOD=7 (three interrupt)
                        data = self.decode_mode_7(payload, data)
                    elif working_mode == 7  # MOD=8 (3 ADC + DS18B20)
                        data = self.decode_mode_8(payload, data)
                    elif working_mode == 8  # MOD=9 (3 DS18B20 + 2 count)
                        data = self.decode_mode_9(payload, data)
                    elif working_mode == 9  # MOD=10 (PWM)
                        data = self.decode_mode_10(payload, data)
                    elif working_mode == 10  # MOD=11 (TEMP117)
                        data = self.decode_mode_11(payload, data)
                    elif working_mode == 11  # MOD=12 (Count + SHT31)
                        data = self.decode_mode_12(payload, data)
                    else
                        print(f"SN50v3LB: Unknown working mode: {working_mode}")
                        return nil
                    end
                else
                    print(f"SN50v3LB: Payload too small for sensor data: {size(payload)}")
                    return nil
                end
                
            else
                print(f"SN50v3LB: Unknown fport: {fport}")
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
            
            # Store sensor trends for different modes
            if data.contains('ds18b20_temperature')
                if !node_data.contains('temp_history')
                    node_data['temp_history'] = []
                end
                node_data['temp_history'].push(data['ds18b20_temperature'])
                if size(node_data['temp_history']) > 10
                    node_data['temp_history'].pop(0)
                end
            end
            
            # Track count increments
            if data.contains('count_value')
                var prev_count = node_data.find('last_count', 0)
                if data['count_value'] > prev_count
                    node_data['count_increment'] = data['count_value'] - prev_count
                end
                node_data['last_count'] = data['count_value']
            end
            
            # Register downlink commands
            if !global.contains("SN50v3LB_cmdInit") || !global.SN50v3LB_cmdInit
                self.register_downlink_commands()
                global.SN50v3LB_cmdInit = true
            end

            # Save back to global storage
            global.SN50v3LB_nodes[node] = node_data
            
            # Update instance cache
            self.last_data = data
            self.last_update = node_data['last_update']
            
            return data
            
        except .. as e, m
            print(f"SN50v3LB: Decode error - {e}: {m}")
            return nil
        end
    end
    
    def decode_device_status(payload, data)
        # Sensor Model (byte 0)
        data['sensor_model'] = payload[0]
        
        # Firmware Version (bytes 1-2)
        var fw_major = (payload[1] >> 4) & 0x0F
        var fw_minor = payload[1] & 0x0F
        var fw_patch = (payload[2] >> 4) & 0x0F
        data['firmware_version'] = f"{fw_major}.{fw_minor}.{fw_patch}"
        
        # Frequency Band (byte 3)
        var band_map = {
            0x01: "EU868", 0x02: "US915", 0x03: "IN865", 0x04: "AU915",
            0x05: "KZ865", 0x06: "RU864", 0x07: "AS923", 0x08: "AS923-1",
            0x09: "AS923-2", 0x0a: "AS923-3", 0x0b: "CN470", 0x0c: "EU433",
            0x0d: "KR920", 0x0e: "MA869"
        }
        data['frequency_band'] = band_map.find(payload[3], f"Unknown({payload[3]:02X})")
        
        # Sub-band (byte 4)
        data['sub_band'] = payload[4]
        
        # Battery voltage (bytes 5-6)
        data['battery_voltage'] = (payload[6] << 8) | payload[5]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        return data
    end
    
    def decode_mode_1(payload, data)
        # MOD=1 Default Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5) 
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital input & flags (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # I2C Temperature (bytes 7-8)
        var i2c_temp_raw = (payload[8] << 8) | payload[7]
        if i2c_temp_raw != 0
            data['i2c_temperature'] = self.decode_signed_16(i2c_temp_raw) / 10.0
        end
        
        # I2C Humidity (bytes 9-10)
        var i2c_hum_raw = (payload[10] << 8) | payload[9]
        if i2c_hum_raw != 0
            data['i2c_humidity'] = i2c_hum_raw / 10.0
        end
        
        return data
    end
    
    def decode_mode_2(payload, data)
        # MOD=2 Distance Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital input & flags (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # Distance (bytes 7-8)
        data['distance_mm'] = (payload[8] << 8) | payload[7]
        data['distance_cm'] = data['distance_mm'] / 10.0
        
        return data
    end
    
    def decode_mode_3(payload, data)
        # MOD=3 3 ADC + I2C Mode (12 bytes)
        # ADC1 (bytes 0-1)
        data['adc1_pa4'] = (payload[1] << 8) | payload[0]
        
        # ADC2 (bytes 2-3)
        data['adc2_pa5'] = (payload[3] << 8) | payload[2]
        
        # ADC3 (bytes 4-5)
        data['adc3_pa8'] = (payload[5] << 8) | payload[4]
        
        # Digital interrupt (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # I2C Temperature (bytes 7-8)
        var i2c_temp_raw = (payload[8] << 8) | payload[7]
        if i2c_temp_raw != 0
            data['i2c_temperature'] = self.decode_signed_16(i2c_temp_raw) / 10.0
        end
        
        # I2C Humidity (bytes 9-10)
        var i2c_hum_raw = (payload[10] << 8) | payload[9]
        if i2c_hum_raw != 0
            data['i2c_humidity'] = i2c_hum_raw / 10.0
        end
        
        # Battery (byte 11)
        data['battery_scaled'] = payload[11]
        
        return data
    end
    
    def decode_mode_4(payload, data)
        # MOD=4 3 DS18B20 Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature 1 (bytes 2-3)
        var temp1_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_1_temperature'] = self.decode_signed_16(temp1_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital input & flags (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # DS18B20 Temperature 2 (bytes 7-8)
        var temp2_raw = (payload[8] << 8) | payload[7]
        data['ds18b20_2_temperature'] = self.decode_signed_16(temp2_raw) / 10.0
        
        # DS18B20 Temperature 3 (bytes 9-10)
        var temp3_raw = (payload[10] << 8) | payload[9]
        data['ds18b20_3_temperature'] = self.decode_signed_16(temp3_raw) / 10.0
        
        return data
    end
    
    def decode_mode_5(payload, data)
        # MOD=5 Weight Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital input & flags (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # Weight (bytes 7-10)
        data['weight_g'] = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
        data['weight_kg'] = data['weight_g'] / 1000.0
        
        return data
    end
    
    def decode_mode_6(payload, data)
        # MOD=6 Counting Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital input (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # Count value (bytes 7-10)
        data['count_value'] = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
        
        return data
    end
    
    def decode_mode_7(payload, data)
        # MOD=7 Three Interrupt Mode (9 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa5'] = (payload[5] << 8) | payload[4]
        
        # Three interrupt status (bytes 6-8)
        data['interrupt_pa8'] = payload[6]
        data['interrupt_pa4'] = payload[7]
        data['interrupt_pb15'] = payload[8]
        
        return data
    end
    
    def decode_mode_8(payload, data)
        # MOD=8 3 ADC + DS18B20 Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC1 (bytes 4-5)
        data['adc1_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital interrupt (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # ADC2 (bytes 7-8)
        data['adc2_pa5'] = (payload[8] << 8) | payload[7]
        
        # ADC3 (bytes 9-10)
        data['adc3_pa8'] = (payload[10] << 8) | payload[9]
        
        return data
    end
    
    def decode_mode_9(payload, data)
        # MOD=9 3 DS18B20 + 2 Count Mode (17 bytes)
        if size(payload) < 17
            print(f"SN50v3LB: Mode 9 requires 17 bytes, got {size(payload)}")
            return data
        end
        
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature 1 (bytes 2-3)
        var temp1_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_1_temperature'] = self.decode_signed_16(temp1_raw) / 10.0
        
        # DS18B20 Temperature 2 (bytes 4-5)
        var temp2_raw = (payload[5] << 8) | payload[4]
        data['ds18b20_2_temperature'] = self.decode_signed_16(temp2_raw) / 10.0
        
        # Digital interrupt (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # DS18B20 Temperature 3 (bytes 7-8)
        var temp3_raw = (payload[8] << 8) | payload[7]
        data['ds18b20_3_temperature'] = self.decode_signed_16(temp3_raw) / 10.0
        
        # Count 1 (bytes 9-12)
        data['count1_pa8'] = (payload[12] << 24) | (payload[11] << 16) | (payload[10] << 8) | payload[9]
        
        # Count 2 (bytes 13-16)
        data['count2_pa4'] = (payload[16] << 24) | (payload[15] << 16) | (payload[14] << 8) | payload[13]
        
        return data
    end
    
    def decode_mode_10(payload, data)
        # MOD=10 PWM Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # PWM Settings (byte 6)
        data['pwm_settings'] = payload[6]
        
        # Pulse period (bytes 7-8)
        data['pulse_period'] = (payload[8] << 8) | payload[7]
        
        # High level duration (bytes 9-10)
        data['high_duration'] = (payload[10] << 8) | payload[9]
        
        # Calculate frequency and duty cycle
        if data['pulse_period'] > 0
            var pwm_set = (data['pwm_settings'] >> 2) & 0x01
            if pwm_set == 0
                data['frequency_hz'] = 1000000 / data['pulse_period']
            else
                data['frequency_hz'] = 1000 / data['pulse_period']
            end
            data['duty_cycle_pct'] = (data['high_duration'] * 100) / data['pulse_period']
        end
        
        return data
    end
    
    def decode_mode_11(payload, data)
        # MOD=11 TEMP117 Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # DS18B20 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['ds18b20_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # ADC (bytes 4-5)
        data['adc_pa4'] = (payload[5] << 8) | payload[4]
        
        # Digital input & flags (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # TEMP117 Temperature (bytes 7-8)
        var temp117_raw = (payload[8] << 8) | payload[7]
        if temp117_raw != 0
            data['temp117_temperature'] = self.decode_signed_16(temp117_raw) / 10.0
        end
        
        return data
    end
    
    def decode_mode_12(payload, data)
        # MOD=12 Count + SHT31 Mode (11 bytes)
        # Battery (bytes 0-1)
        data['battery_voltage'] = (payload[1] << 8) | payload[0]
        data['battery_v'] = data['battery_voltage'] / 1000.0
        
        # SHT31 Temperature (bytes 2-3)
        var temp_raw = (payload[3] << 8) | payload[2]
        data['sht31_temperature'] = self.decode_signed_16(temp_raw) / 10.0
        
        # SHT31 Humidity (bytes 4-5)
        data['sht31_humidity'] = ((payload[5] << 8) | payload[4]) / 10.0
        
        # Digital input (byte 6)
        data = self.decode_digital_flags(payload[6], data)
        
        # Count value (bytes 7-10)
        data['count_value'] = (payload[10] << 24) | (payload[9] << 16) | (payload[8] << 8) | payload[7]
        
        return data
    end
    
    def decode_digital_flags(flags_byte, data)
        # Extract digital input and interrupt flags
        data['digital_pb15'] = (flags_byte & 0x02) != 0  # Bit 1
        data['interrupt_pa8'] = (flags_byte & 0x01) != 0  # Bit 0
        
        return data
    end
    
    def decode_signed_16(value)
        if (value & 0x8000) != 0
            value = value - 0x10000
        end
        return value
    end
    
    def add_web_sensor()
        import global
        
        # Try to use current instance data first
        var data_to_show = self.last_data
        var last_update = self.last_update
        
        # If no instance data, try to recover from global storage
        if size(data_to_show) == 0 && self.node != nil
            var node_data = global.SN50v3LB_nodes.find(self.node, {})
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
            name = f"SN50v3LB-{self.node}"
        end
        var name_tooltip = "Dragino SN50v3-LB Generic LoRaWAN Sensor Node"
        var battery = data_to_show.find('battery_v', 1000)
        var battery_last_seen = last_update
        var rssi = data_to_show.find('rssi', 1000)
        var simulated = data_to_show.find('simulated', false)
        
        # Build display using emoji formatter
        fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
        fmt.start_line()
        
        # Display based on working mode
        var working_mode = data_to_show.find('working_mode', -1)
        
        if working_mode == 0  # MOD=1 Default
            if data_to_show.contains('ds18b20_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_temperature'], "DS18B20", "🌡️")
            end
            if data_to_show.contains('i2c_temperature')
                fmt.add_sensor("temp", data_to_show['i2c_temperature'], "I2C Temp", "🌡️")
            end
            if data_to_show.contains('i2c_humidity')
                fmt.add_sensor("humidity", data_to_show['i2c_humidity'], "I2C Hum", "💧")
            end
            if data_to_show.contains('adc_pa4')
                fmt.add_sensor("string", f"{data_to_show['adc_pa4']}", "ADC", "📊")
            end
            
        elif working_mode == 1  # MOD=2 Distance
            if data_to_show.contains('distance_cm')
                fmt.add_sensor("distance", data_to_show['distance_cm'], "Distance", "📏")
            end
            if data_to_show.contains('ds18b20_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_temperature'], "Temp", "🌡️")
            end
            
        elif working_mode == 2  # MOD=3 3 ADC + I2C
            if data_to_show.contains('adc1_pa4')
                fmt.add_sensor("string", f"{data_to_show['adc1_pa4']}", "ADC1", "📊")
            end
            if data_to_show.contains('adc2_pa5')
                fmt.add_sensor("string", f"{data_to_show['adc2_pa5']}", "ADC2", "📊")
            end
            if data_to_show.contains('adc3_pa8')
                fmt.add_sensor("string", f"{data_to_show['adc3_pa8']}", "ADC3", "📊")
            end
            
        elif working_mode == 3  # MOD=4 3 DS18B20
            if data_to_show.contains('ds18b20_1_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_1_temperature'], "T1", "🌡️")
            end
            if data_to_show.contains('ds18b20_2_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_2_temperature'], "T2", "🌡️")
            end
            if data_to_show.contains('ds18b20_3_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_3_temperature'], "T3", "🌡️")
            end
            
        elif working_mode == 4  # MOD=5 Weight
            if data_to_show.contains('weight_kg')
                fmt.add_sensor("weight", data_to_show['weight_kg'], "Weight", "⚖️")
            end
            if data_to_show.contains('ds18b20_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_temperature'], "Temp", "🌡️")
            end
            
        elif working_mode == 5  # MOD=6 Counting
            if data_to_show.contains('count_value')
                fmt.add_sensor("string", f"{data_to_show['count_value']}", "Count", "🔢")
            end
            if data_to_show.contains('ds18b20_temperature')
                fmt.add_sensor("temp", data_to_show['ds18b20_temperature'], "Temp", "🌡️")
            end
            
        elif working_mode == 6  # MOD=7 Three Interrupt
            if data_to_show.contains('interrupt_pa8')
                var status = data_to_show['interrupt_pa8'] ? "1" : "0"
                fmt.add_sensor("string", status, "INT1", "📶")
            end
            if data_to_show.contains('interrupt_pa4')
                var status = data_to_show['interrupt_pa4'] ? "1" : "0"
                fmt.add_sensor("string", status, "INT2", "📶")
            end
            if data_to_show.contains('interrupt_pb15')
                var status = data_to_show['interrupt_pb15'] ? "1" : "0"
                fmt.add_sensor("string", status, "INT3", "📶")
            end
            
        elif working_mode == 9  # MOD=10 PWM
            if data_to_show.contains('frequency_hz')
                fmt.add_sensor("string", f"{data_to_show['frequency_hz']}Hz", "Freq", "〰️")
            end
            if data_to_show.contains('duty_cycle_pct')
                fmt.add_sensor("string", f"{data_to_show['duty_cycle_pct']}%", "Duty", "📊")
            end
            
        elif working_mode == 11  # MOD=12 Count + SHT31
            if data_to_show.contains('sht31_temperature')
                fmt.add_sensor("temp", data_to_show['sht31_temperature'], "Temp", "🌡️")
            end
            if data_to_show.contains('sht31_humidity')
                fmt.add_sensor("humidity", data_to_show['sht31_humidity'], "Hum", "💧")
            end
            if data_to_show.contains('count_value')
                fmt.add_sensor("string", f"{data_to_show['count_value']}", "Count", "🔢")
            end
        end
        
        # Add mode indicator
        if working_mode >= 0
            fmt.next_line()
            fmt.add_sensor("string", f"MOD{working_mode + 1}", "Mode", "⚙️")
        end
        
        # Device info if available
        if data_to_show.contains('firmware_version')
            fmt.add_sensor("string", data_to_show['firmware_version'], "FW", "💾")
        end
        
        if data_to_show.contains('frequency_band')
            fmt.add_sensor("string", data_to_show['frequency_band'], "Band", "📡")
        end
        
        # Add last seen info if data is old
        if last_update > 0
            var age = tasmota.rtc()['local'] - last_update
            if age > 3600  # Data older than 1 hour
                fmt.next_line()
                fmt.add_sensor("string", self.format_age(age), "Last Seen", "⏱️")
            end
        end
        
        fmt.end_line()
        msg += fmt.get_msg()

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
        var node_data = global.SN50v3LB_nodes.find(node_id, nil)
        if node_data == nil return nil end
        
        return {
            'last_update': node_data.find('last_update', 0),
            'name': node_data.find('name', 'Unknown'),
            'battery_history': node_data.find('battery_history', []),
            'temp_history': node_data.find('temp_history', []),
            'last_count': node_data.find('last_count', 0),
            'count_increment': node_data.find('count_increment', 0)
        }
    end
    
    # Clear node data (for maintenance)
    def clear_node_data(node_id)
        import global
        if global.SN50v3LB_nodes.contains(node_id)
            global.SN50v3LB_nodes.remove(node_id)
            return true
        end
        return false
    end
    
    # Register downlink commands for device control
    def register_downlink_commands()
        import string
        
        # Set transmit interval
        tasmota.remove_cmd("LwSN50v3LBInterval")
        tasmota.add_cmd("LwSN50v3LBInterval", def(cmd, idx, payload_str)
            var seconds = int(payload_str)
            if seconds < 30 || seconds > 16777215
                return tasmota.resp_cmnd_str("Invalid: range 30-16777215 seconds")
            end
            
            var hex_cmd = f"01{(seconds >> 16) & 0xFF:02X}{(seconds >> 8) & 0xFF:02X}{seconds & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Request device status
        tasmota.remove_cmd("LwSN50v3LBStatus")
        tasmota.add_cmd("LwSN50v3LBStatus", def(cmd, idx, payload_str)
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, "2601")
        end)
        
        # Set interrupt mode
        tasmota.remove_cmd("LwSN50v3LBInterrupt")
        tasmota.add_cmd("LwSN50v3LBInterrupt", def(cmd, idx, payload_str)
            # Format: LwSN50v3LBInterrupt<slot> <pin>,<mode>[,<delay>]
            var parts = string.split(payload_str, ',')
            if size(parts) < 2 || size(parts) > 3
                return tasmota.resp_cmnd_str("Usage: pin(0-2),mode(0-3)[,delay_ms]")
            end
            
            var pin = int(parts[0])
            var mode = int(parts[1])
            var delay = size(parts) == 3 ? int(parts[2]) : 0
            
            if pin < 0 || pin > 2
                return tasmota.resp_cmnd_str("Invalid pin: 0=PA8, 1=PA4, 2=PB15")
            end
            if mode < 0 || mode > 3
                return tasmota.resp_cmnd_str("Invalid mode: 0=disable, 1=both, 2=falling, 3=rising")
            end
            
            var hex_cmd = f"0600{pin:02X}{mode:02X}{(delay >> 8) & 0xFF:02X}{delay & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set 5V output duration
        tasmota.remove_cmd("LwSN50v3LBPower5V")
        tasmota.add_cmd("LwSN50v3LBPower5V", def(cmd, idx, payload_str)
            var ms = int(payload_str)
            if ms < 0 || ms > 65535
                return tasmota.resp_cmnd_str("Invalid: range 0-65535 ms")
            end
            
            var hex_cmd = f"07{(ms >> 8) & 0xFF:02X}{ms & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set weight parameters
        tasmota.remove_cmd("LwSN50v3LBWeight")
        tasmota.add_cmd("LwSN50v3LBWeight", def(cmd, idx, payload_str)
            # Format: LwSN50v3LBWeight<slot> <zero|factor,value>
            var parts = string.split(payload_str, ',')
            if size(parts) == 1 && parts[0] == "zero"
                return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, "0801")
            elif size(parts) == 2 && parts[0] == "factor"
                var factor = real(parts[1]) * 10
                var factor_int = int(factor)
                var hex_cmd = f"0802{(factor_int >> 16) & 0xFF:02X}{(factor_int >> 8) & 0xFF:02X}{factor_int & 0xFF:02X}"
                return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
            else
                return tasmota.resp_cmnd_str("Usage: zero OR factor,<value>")
            end
        end)
        
        # Set count value
        tasmota.remove_cmd("LwSN50v3LBSetCount")
        tasmota.add_cmd("LwSN50v3LBSetCount", def(cmd, idx, payload_str)
            # Format: LwSN50v3LBSetCount<slot> <count_num>,<value>
            var parts = string.split(payload_str, ',')
            if size(parts) != 2
                return tasmota.resp_cmnd_str("Usage: LwSN50v3LBSetCount<slot> <1|2>,<value>")
            end
            
            var count_num = int(parts[0])
            var value = int(parts[1])
            
            if count_num < 1 || count_num > 2
                return tasmota.resp_cmnd_str("Invalid count: 1=PA8, 2=PA4")
            end
            
            var hex_cmd = f"09{count_num:02X}{(value >> 24) & 0xFF:02X}{(value >> 16) & 0xFF:02X}{(value >> 8) & 0xFF:02X}{value & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set working mode
        tasmota.remove_cmd("LwSN50v3LBMode")
        tasmota.add_cmd("LwSN50v3LBMode", def(cmd, idx, payload_str)
            # Format: LwSN50v3LBMode<slot> <1-12>
            var mode = int(payload_str)
            if mode < 1 || mode > 12
                return tasmota.resp_cmnd_str("Invalid mode: range 1-12")
            end
            
            var hex_cmd = f"0A{mode:02X}"
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set PWM output
        tasmota.remove_cmd("LwSN50v3LBPWM")
        tasmota.add_cmd("LwSN50v3LBPWM", def(cmd, idx, payload_str)
            # Format: LwSN50v3LBPWM<slot> <freq>,<duty>,<time>
            var parts = string.split(payload_str, ',')
            if size(parts) != 3
                return tasmota.resp_cmnd_str("Usage: LwSN50v3LBPWM<slot> <freq_hz>,<duty_%>,<time_ms>")
            end
            
            var freq = int(parts[0])
            var duty = int(parts[1])
            var time = int(parts[2])
            
            if freq < 5 || freq > 100000
                return tasmota.resp_cmnd_str("Invalid frequency: range 5-100000 Hz")
            end
            if duty < 0 || duty > 100
                return tasmota.resp_cmnd_str("Invalid duty cycle: range 0-100%")
            end
            
            var hex_cmd = f"0B{(freq >> 16) & 0xFF:02X}{(freq >> 8) & 0xFF:02X}{freq & 0xFF:02X}{duty:02X}{(time >> 8) & 0xFF:02X}{time & 0xFF:02X}"
            return lwdecode.SendDownlink(global.SN50v3LB_nodes, cmd, idx, hex_cmd)
        end)
        
        # Set PWM settings
        tasmota.remove_cmd("LwSN50v3LBPWMSet")
        tasmota.add_cmd("LwSN50v3LBPWMSet", def(cmd, idx, payload_str)
            # Format: LwSN50v3LBPWMSet<slot> <0|1>
            return lwdecode.SendDownlinkMap(global.SN50v3LB_nodes, cmd, idx, payload_str, {
                '0|US|MICRO': ['0C00', 'Microseconds'],
                '1|MS|MILLI': ['0C01', 'Milliseconds']
            })
        end)
        
        print("SN50v3LB: Downlink commands registered")
    end
end

# Global instance
LwDeco = LwDecode_SN50v3LB()

# Node management commands
tasmota.remove_cmd("LwSN50v3LBNodeStats")
tasmota.add_cmd("LwSN50v3LBNodeStats", def(cmd, idx, node_id)
    var stats = LwDeco.get_node_stats(node_id)
    if stats != nil
        import json
        tasmota.resp_cmnd(json.dump(stats))
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

tasmota.remove_cmd("LwSN50v3LBClearNode")
tasmota.add_cmd("LwSN50v3LBClearNode", def(cmd, idx, node_id)
    if LwDeco.clear_node_data(node_id)
        tasmota.resp_cmnd_done()
    else
        tasmota.resp_cmnd_str("Node not found")
    end
end)

# Test UI command
tasmota.remove_cmd("LwSN50v3LBTestUI")
tasmota.add_cmd("LwSN50v3LBTestUI", def(cmd, idx, payload_str)
    var test_scenarios = {
        "default":   "AF0C6100E4030000E0016905",        # MOD=1 default mode
        "distance":  "AF0C6100E403000050C90005",        # MOD=2 distance mode
        "3adc":      "6400C8006400050E01690500",      # MOD=3 3ADC+I2C mode
        "3temp":     "AF0C6100E4030000F0006501",      # MOD=4 3DS18B20 mode
        "weight":    "AF0C6100E40300000A000000",      # MOD=5 weight mode
        "counting":  "AF0C6100E40300006400000009",     # MOD=6 counting mode
        "interrupt": "AF0C6100C800010215",            # MOD=7 three interrupt
        "pwm":       "AF0C6100E40306E8031C20",        # MOD=10 PWM mode
        "sht31":     "AF0C01016801006400000025",      # MOD=12 count+SHT31
        "status":    "1C0100010CAF0C"                # Device status (fport=5)
    }
    
    var hex_payload = test_scenarios.find(payload_str ? payload_str : 'nil', 'not_found')
    
    if hex_payload == 'not_found'
        var scenarios_list = ""
        for key: test_scenarios.keys()
            scenarios_list += key + " "
        end
        return tasmota.resp_cmnd_str(format("Available scenarios: %s", scenarios_list))
    end
    
    var rssi = -75
    var fport = hex_payload == test_scenarios['status'] ? 5 : 2

    return tasmota.cmd(f'LwSimulate{idx} {rssi},{fport},{hex_payload}')
end)

# MANDATORY: Register driver for web UI integration
tasmota.add_driver(LwDeco)
