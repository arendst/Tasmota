class IP5306
    var wire
    var i2c_addr

#-
SYS_CTL0 (adresa 0x00)  (DEFAULT MSB: 53 - 0 0 1 1 0 1 0 1)
Bit	Názov	Popis	R/W	Reset
7:6	Reserved	Rezervované, nemeniť!		        10
5	Boost enable	0: vypnuté, 1: zapnuté 	RW	    1
4	Charger enable	0: vypnuté, 1: zapnuté  RW	    1
3	Reserved	Rezervované, nemeniť!		        1
2	Automatické zapnutie po pripojení záťaže	0: vypnuté, 1: zapnuté	RW	1
1	BOOST výstup stále zapnutý	0: vypnuté, 1: zapnuté	RW	1
0	Vypnutie tlačidlom	0: vypnuté, 1: zapnuté	RW	0

SYS_CTL1 (adresa 0x01) (DEFAULT MSB: 221 - 1 1 0 1 1 1 0 1)
Bit	Názov	Popis	R/W	Reset
7	Výber signálu vypnutia boost	1: Dlhé stlačenie, 0: dvojklik	R/W	        0
6	Výber signálu pre LED baterku	1: Dvojklik, 0: dlhé stlačenie	R/W	        0
5	Krátke stlačenie pre zapnutie boost	0: vypnuté, 1: zapnuté	R/W	            0
4:3	Reserved	Rezervované		                                                11
2	Boost po odpojení VIN	0: vypnuté, 1: zapnuté	R/W	                        1
1	Reserved	Rezervované	R/W	                                                0
0	Nízkonapäťové vypnutie (Batlow 3.0V)	0: vypnuté, 1: zapnuté	R/W	        1

SYS_CTL2 (adresa 0x02) (DEFAULT 100) 0 1 1 0 0 1 0 0 - NO CHANGE, 32 second default
Bit	Názov	Popis (čas vypnutia pri nízkej záťaži)	R/W	Reset
7:4	Reserved	Rezervované		
3:2	Čas vypnutia	11: 64s, 10: 16s, 01: 32s, 00: 8s	R/W	
1:0	Reserved	Rezervované	R/W	0

Charger_CTL0 (adresa 0x20) default 1, no change
Bit	Názov	Popis (napätie ukončenia nabíjania)	R/W	Reset
7:2	Reserved	Rezervované		
1:0	Napätie ukončenia nabíjania	11:4.2/4.305/4.35/4.395V, 10:4.185/4.29/4.335/4.38V, 01:4.17/4.275/4.32/4.365V, 00:4.14/4.26/4.305/4.35V. Odporúča sa použiť nastavenie 01 alebo 00.

Charger_CTL1 (adresa 0x21) default 137 , 1 0 0 0 1 0 0 1 , MSB modified 0 0 0 1 1 1 0 1 = 29 (200mA, 4.8V)
Bit	Názov	Popis	R/W	Reset
7:6	Prúd zastavenia nabíjania (detekcia prúdu)	11: 600mA, 10: 500mA, 01: 400mA, 00: 200mA. IP5306 najskôr kontroluje prúd, potom napätie	RW	01
5	Reserved	Rezervované		0
4:2	Undervoltage threshold	Napätie výstupu VOUT počas nabíjania: 111:4.8V, 110:4.75V, 101:4.7V, 100:4.65V, 011:4.6V, 010:4.55V, 001:4.5V, 000:4.45V. IC automaticky nastavuje prúd nabíjania podľa tohto napätia.	RW	101

Charger_CTL2 (adresa 0x22) default 2, 0 0 0 0 0 0 1 0  // No change!
Bit	Názov	Popis	R/W	Reset
7:4	Reserved	Rezervované		0000
3:2	Napätie batérie	11:4.4V, 10:4.35V, 01:4.3V, 00:4.2V	RW	00
1:0	Zvýšenie napätia pri CV režime	11:+42mV, 10:+28mV, 01:+14mV, 00:žiadne. (Odporúča sa +14mV pre 4.3/4.35/4.4V, +28mV pre 4.2V)	RW	01

Register 0x23 // No change!
Bit	Názov	Popis	R/W	Reset
7:6	Reserved	Rezervované		
5	Výber CC smyčky	1: VIN CC prúd, 0: BAT CC prúd	RW	1
4:0	Reserved	Rezervované	RW	

CHG_DIG_CTL0 (adresa 0x24) // No change!
Bit	Názov (nastavenie nabíjacieho prúdu VIN)	Popis	R/W
7:5	Reserved	Rezervované	
4:0	Nabíjací prúd VIN	I = 0.05 + b00.1 + b10.2 + b20.4 + b30.8 + b4*1.6A	RW


-#


    def init(bus, add)

        self.i2c_addr = add

        if bus == 1
            self.wire = wire1
        elif bus == 2
            self.wire = wire2
        else
            return print("[IP5306] ERROR: bad I²C bus", bus)
        end

        print("[IP5306] Initialized... ")

        ## MSB Master G1 Default Inits:

        self.write_reg(0x21, 29) 
        self.write_reg(0x01, 221)            # Long push - OFF, short - ON, boost after DC in loss - ON (UPS)
        self.write_reg(0x00, 53)             # Boost ON, Charger ON, Load ON, Button ON, BOOST alway on - OFF
        self.write_reg(0x24, 13)             # Battery charging current 500mA

    end

    # Zápis registra
    def write_reg(reg, value)
        self.wire._begin_transmission(self.i2c_addr)
        self.wire._write(reg)
        self.wire._write(value)
        self.wire._end_transmission()
    end

    # Čítanie registra
    def read_reg(reg)
        self.wire._begin_transmission(self.i2c_addr)
        self.wire._write(reg)
        self.wire._end_transmission()
        self.wire._request_from(self.i2c_addr, 1, true)
        return self.wire._read()
    end

    # Kontrola stavu nabíjania
    def charging_state()
        var state = self.read_reg(0x70)
        return (state & 0x04) == 0
    end

    # Nastavenie boost režimu (zapnutie/vypnutie) (DEFAULT: 1)
    def set_boost(enable)
        var reg = self.read_reg(0x00)
        if enable
            reg |= 0x20
        else
            reg &= ~0x20
        end
        self.write_reg(0x00, reg)
    end

    # Nastavenie režimu nabíjania (zapnutie/vypnutie)  (DEFAULT: 1)
    def set_charger(enable)
        var reg = self.read_reg(0x00)
        if enable
            reg |= 0x10
        else
            reg &= ~0x10
        end
        self.write_reg(0x00, reg)
    end

    # Nastavenie automatického zapnutia pri pripojení adaptéra (DEFAULT: 1)
    def set_auto_power_on(enable) 
        var reg = self.read_reg(0x00)
        if enable
            reg |= 0x04
        else
            reg &= ~0x04
        end
        self.write_reg(0x00, reg)
    end

    # Nastavenie režimu Boost output always on (DEFAULT: 0)
    def set_boost_always_on(enable)
        var reg = self.read_reg(0x00)
        if enable
            reg |= 0x02
        else
            reg &= ~0x02
        end
        self.write_reg(0x00, reg)
    end

    # Nastavenie vypnutia tlačidlom (DEFAULT: 1)
    def set_button_shutdown(enable)
        var reg = self.read_reg(0x00)
        if enable
            reg |= 0x01
        else
            reg &= ~0x01
        end
        self.write_reg(0x00, reg)
    end

    # Kontrola plného nabitia
    def fully_charged()
        var state = self.read_reg(0x71)
        return (state & 0x08) != 0
    end

    # Kontrola nízkej záťaže
    def is_light_load()
        var state = self.read_reg(0x72)
        return (state & 0x04) != 0
    end

    # Nastavenie nabíjacieho prúdu. 12 = 500mA - optimum MSB Master Value, use only between 7 - 15
    def set_charging_current(reg)

        # Kontrola rozsahu
        if reg < 0 || reg > 255
            return print("[IP5306] ERROR: Invalid charging current (0-255):")
        end
        # Zápis správneho registra
        self.write_reg(0x24, reg)
        print(reg)
    end


    # Nastavenie constant-voltage boost (register 0x22)
    def set_constant_voltage_boost(boost_mV)
        var reg = self.read_reg(0x22) & 0xFC
        if boost_mV == 0
            reg |= 0x00
        elif boost_mV == 14
            reg |= 0x01
        elif boost_mV == 28
            reg |= 0x02
        elif boost_mV == 42
            reg |= 0x03
        else
            return print("[IP5306] ERROR: Invalid CV boost", boost_mV)
        end
        self.write_reg(0x22, reg)
    end

end
return IP5306

var ip5306 = IP5306(2)
