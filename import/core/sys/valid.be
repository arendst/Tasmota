#############################################
## SYSTEM EEPROM - INTEGRITY PROCEDURE   ####
#############################################
import gc

syscfg.system_fatal_error       = false 
syscfg.system_fatal_error_codes = []
tasmota.delay(100)
syscfg.eeprom_signature_status  = func.check_eeprom_signature(syscfg.devicemac, EEPROM_DATA[22], EEPROM_DATA[26], EEPROM_DATA[23])
tasmota.yield()
gc.collect()
syscfg.eeprom_crc_status        = func.check_eeprom_crc(AT24C512.concat_sectors_crc(), EEPROM_DATA[22], EEPROM_DATA[23], EEPROM_DATA[26], syscfg.devicemac, EEPROM_DATA[41])
tasmota.yield()
gc.collect()
syscfg.eeprom_corrupt_status    = func.check_eeprom_corrupt(EEPROM_DATA)
tasmota.yield()
gc.collect()

tasmota.delay(100)

if !syscfg.eeprom_signature_status 
    syscfg.system_fatal_error = true
    syscfg.system_fatal_error_codes.push("C00")
end

if !syscfg.eeprom_crc_status 
    syscfg.system_fatal_error = true
    syscfg.system_fatal_error_codes.push("C01")
end
    
if !syscfg.eeprom_corrupt_status
    syscfg.system_fatal_error = true
    syscfg.system_fatal_error_codes.push("C02")
end
