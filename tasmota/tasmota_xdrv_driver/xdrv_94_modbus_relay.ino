/*
  xdrv_94_modbus_relay.ino - Modbus RTU relay boards support for Tasmota

  Copyright (C) 2026  Jacek Ziółkowski

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_MODBUS_RELAY

/*********************************************************************************************\
 * Modbus RTU relay boards
 *
 * Adds support for Modbus RTU relay boards, such as those from Waveshare or generic no-name brands.
 * They typically use the same modbus chip, therefore they share the same modbus instruction set and register map.
 *
 * The module allows control of up to 8 Modbus RTU relay devices, each with a configurable number of outputs.
 * The total number of outputs is limited to 32 reduced by the number of already cofigured power outputs in Tasmota.
 * The modbus connection settinga and relay devices are defined in a JSON template that can be loaded from
 * a file, rule, or script.
 *
 * Template syntax:
 * {"Baud": <baud>, "Devices": [{"A": <address>, "O": <outputs>}, ...]}
 *
 * Where:
 * - <baud> is the configured baud rate for the module (boards have 9600 by default)
 * - <address> is the modbus address of the relay device (1-247)
 * - <outputs> is the number of outputs for the relay device
 *
 * Note: These modules have harcoded N81 serial configuration.
 *
 * Example template for 2 devices with 4 outputs each ):
 *  {"Baud":9600, "Devices":[{"A":1,"O":4}, {"A":2,"O":4}]}
 * Example template for 1 device with 8 outputs:
 *  {"Baud":9600, "Devices":[{"A":1,"O":8}]}
 *
 * Note: the addresses don't have to be sequential, neither have to start from 1.
 *
 * Store the template in a file called modbusrelay.dat, or in a rule called modbusrelay:
 *
 * rule3 on file#modbusrelay do {"Baud":9600, "Devices": [{"A": 1, "O": 8}]} endon
 *
 * or in a script section >y.
 * *********************************************************************************************/

#define XDRV_94 94

//#define MODBUS_RELAY_DEBUG
#define MODBUS_RELAY_MAX_DEVICES 8

#include <TasmotaModbus.h>
TasmotaModbus *RelayModbus;

typedef struct {
    uint8_t address;
    uint8_t outputs;

} tModbusRelayDevice;

struct MODBUS_RELAY {
    tModbusRelayDevice device[MODBUS_RELAY_MAX_DEVICES];
    uint32_t baud_rate;
    uint8_t relay_offset;
    uint8_t devices;
    uint8_t max_outputs;    
} ModbusRelay;

uint8_t ModbusRelayDevicesFromTemplate(void) {
    String mbrtmplt = "";
#ifdef USE_UFILESYS
    mbrtmplt = TfsLoadString("/modbusrelay.dat");
    if (mbrtmplt.length()) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("MBR: Modbus relay definition loaded from file"));
    }
#endif // USE_UFILESYS
#ifdef USE_RULES
    if (!mbrtmplt.length()) {
        mbrtmplt = RuleLoadFile("MODBUSRELAY");
        AddLog(LOG_LEVEL_DEBUG, PSTR("MBR: Modbus relay definition loaded from rule"));
    }
#endif // USE_RULES
#ifdef USE_SCRIPT
    if (!mbrtmplt.length()) {
        mbrtmplt = ScriptLoadSection(">y");
        AddLog(LOG_LEVEL_DEBUG, PSTR("MBR: Modbus relay definition loaded from script"));
    }
#endif // USE_SCRIPT

    uint32_t len = mbrtmplt.length() + 1;
    if (len < 7) {
        return 0;
    }

    JsonParser parser((char *)mbrtmplt.c_str());
    JsonParserObject root = parser.getRootObject();
    if (!root) {
        AddLog(LOG_LEVEL_ERROR, PSTR("MBR: JSON template invalid"));
        return 0;
    }

    JsonParserToken val = root[PSTR("Baud")];
    uint32_t baud = val ? val.getUInt() : 9600;
    ModbusRelay.baud_rate = baud;

    JsonParserArray devices = root[PSTR("Devices")].getArray();
    if (!devices) {
#ifdef MODBUS_RELAY_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBR: no devices"));
#endif
        return 0;
    }

    uint8_t count = 0;
    uint8_t total_outputs = 0;
    for (auto dev : devices) {
        if (count >= MODBUS_RELAY_MAX_DEVICES) {
#ifdef MODBUS_RELAY_DEBUG
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBR: max devices reached"));
#endif
            break;
        }

        JsonParserObject obj = dev.getObject();
        if (!obj) {
#ifdef MODBUS_RELAY_DEBUG
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBR: invalid device object"));
#endif
            continue;
        }

        uint8_t addr = obj[PSTR("A")].getUInt();
        uint8_t outs = obj[PSTR("O")].getUInt();
        if (addr < 1 || addr > 247 || outs == 0) {
        AddLog(LOG_LEVEL_ERROR, PSTR("MBR: invalid device address %d"), addr);
        break;
        }
        ModbusRelay.device[count].address = addr;
        ModbusRelay.device[count].outputs = outs;

#ifdef MODBUS_RELAY_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBR: Created device at address %d with %d outputs"), addr, outs);
#endif
        total_outputs += outs;
        count++;
    }
    ModbusRelay.devices = count;
    ModbusRelay.max_outputs = total_outputs;
    return ModbusRelay.devices;
} // ModbusRelayDevicesFromTemplate

void ModbusRelayModuleInit(void) {
    if (PinUsed(GPIO_MODBUSRELAY_TX) && PinUsed(GPIO_MODBUSRELAY_RX)) {
        uint8_t num_devices = ModbusRelayDevicesFromTemplate();
        if (num_devices > 0) {
            RelayModbus = new TasmotaModbus(Pin(GPIO_MODBUSRELAY_RX), Pin(GPIO_MODBUSRELAY_TX), Pin(GPIO_MODBUSRELAY_TX_ENA), Pin(GPIO_MODBUSRELAY_RX_ENA));
            RelayModbus->Begin(ModbusRelay.baud_rate, SERIAL_8N1);            
            ModbusRelay.relay_offset = TasmotaGlobal.devices_present;
            UpdateDevicesPresent(ModbusRelay.max_outputs);
        }
    }
} // ModbusRelayModuleInit

void ModbusRelaySwitchRelay(void) {
    power_t rpower = XdrvMailbox.index;    
    uint32_t max_outputs = ModbusRelay.max_outputs;    
    DevicesPresentNonDisplayOrLight(max_outputs);          // Skip display and/or light(s)    
    uint32_t relay_offset = ModbusRelay.max_outputs - max_outputs + ModbusRelay.relay_offset;    
    rpower >>= relay_offset;

    for (uint8_t i = 0; i < ModbusRelay.devices; i++) {
        uint8_t outs = ModbusRelay.device[i].outputs;
        uint8_t addr = ModbusRelay.device[i].address;
        
        uint32_t mask = (outs == 32) ? 0xFFFFFFFF : ((1UL << outs) - 1);
        uint32_t state = rpower & mask;

        uint16_t writeData[2] = {0};

        uint16_t raw_wd0 = (uint16_t)(state & 0xFFFF);
        writeData[0] = ((raw_wd0 & 0x00FF) << 8) | ((raw_wd0 & 0xFF00) >> 8);

        if (outs > 16) {
            uint16_t raw_wd1 = (uint16_t)((state >> 16) & 0xFFFF);
            writeData[1] = ((raw_wd1 & 0x00FF) << 8) | ((raw_wd1 & 0xFF00) >> 8);
        }

        uint8_t err = RelayModbus->Send(addr, 0x0F, 0x0000, outs, writeData);

#ifdef MODBUS_RELAY_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBR: Send device %d, outputs %d, state %08X, err %u"), addr, outs, state, err);
#endif

        if (err) {
            AddLog(LOG_LEVEL_ERROR, PSTR("MBR: Send err %u to device %u"), err, addr);
        }
        else {
            uint32_t timeout = millis() + 35;
            bool data_ready = false;

            while (millis() < timeout) {
                if (RelayModbus->ReceiveReady()) {
                    data_ready = true;
                    break;
                }
                yield();
            }

            if (data_ready) {
                uint8_t buf[16] = {0};
                uint8_t recv_err = RelayModbus->ReceiveBuffer(buf, 2);

                if (recv_err) {
                    AddLog(LOG_LEVEL_DEBUG, PSTR("MBR: Recv parsing err %u from device %u"), recv_err, addr);
                }
                else {
                    uint16_t start = (buf[2] << 8) | buf[3];
                    uint16_t qty = (buf[4] << 8) | buf[5];

                    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBR: Reply deviceAddress: %02X, function: %02X, start:%u count:%u"),
                           buf[0], buf[1], start, qty);
                }
            }
            else {
                AddLog(LOG_LEVEL_DEBUG, PSTR("MBR: Device %u timed out or CRC failed"), addr);
            }
        }

        if (outs == 32) {
            rpower = 0;
        }
        else {
            rpower >>= outs;
        }
        delay(10);
    }
} // ModbusRelaySwitchRelay

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv94(uint32_t function) {
    bool result = false;

    if (FUNC_SETUP_RING2 == function) {
        ModbusRelayModuleInit();
    }
    else if (ModbusRelay.devices > 0) {
        switch (function) {
        case FUNC_SET_POWER:
            ModbusRelaySwitchRelay();
            break;
        case FUNC_ACTIVE:
            result = true;
            break;
        }
    }
    return result;
}

#endif // USE_MODBUS_RELAY