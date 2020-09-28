/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpSelectAllProtocols.h
 * 42 + 1 protocols enabled
 * 8 Protocols disabled since they conflicts with the enabled ones
 * Depending on the value of F_INTERRUPTS 2 other protocols (LEGO and RCMM or PENTAX and GREE) are disabled too - see F_INTERRUPTS below
 *
 *
 * Copyright (c) 2009-2020 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef IRMP_SELECT_PROTOCOLS_H
#define IRMP_SELECT_PROTOCOLS_H

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Change settings from 1 to 0 if you want to disable one or more decoders.
 * This saves program space.
 *
 * 1 enable  decoder
 * 0 disable decoder
 *
 * The standard decoders are enabled per default.
 * Less common protocols are disabled here, you need to enable them manually.
 *
 * If you want to use FDC or RCCAR simultaneous with RC5 protocol, additional program space is required.
 * If you don't need RC5 when using FDC/RCCAR, you should disable RC5.
 * You cannot enable both DENON and RUWIDO, only enable one of them.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Protocols Part 1: IR decoders
 * If you use a RF receiver, deactivate all IR protocols!
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
// 4 typical protocols, disable here!           Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRMP_SUPPORT_SIRCS_PROTOCOL             1       // Sony SIRCS           >= 10000                 ~150 bytes
#define IRMP_SUPPORT_NEC_PROTOCOL               1       // NEC + APPLE + ONKYO  >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SAMSUNG_PROTOCOL           1       // Samsung + Samsg32    >= 10000                 ~300 bytes
#define IRMP_SUPPORT_KASEIKYO_PROTOCOL          1       // Kaseikyo             >= 10000                 ~250 bytes

// 11 more protocols, enable here!              Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRMP_SUPPORT_JVC_PROTOCOL               1       // JVC                  >= 10000                 ~150 bytes
#define IRMP_SUPPORT_NEC16_PROTOCOL             1       // NEC16                >= 10000                 ~100 bytes
#define IRMP_SUPPORT_NEC42_PROTOCOL             1       // NEC42                >= 10000                 ~300 bytes
#define IRMP_SUPPORT_MATSUSHITA_PROTOCOL        1       // Matsushita           >= 10000                  ~50 bytes
#define IRMP_SUPPORT_DENON_PROTOCOL             1       // DENON, Sharp         >= 10000                 ~250 bytes
#define IRMP_SUPPORT_RC5_PROTOCOL               1       // RC5                  >= 10000                 ~250 bytes
#define IRMP_SUPPORT_RC6_PROTOCOL               1       // RC6 & RC6A           >= 10000                 ~250 bytes
#define IRMP_SUPPORT_IR60_PROTOCOL              1       // IR60 (SDA2008)       >= 10000                 ~300 bytes
#define IRMP_SUPPORT_GRUNDIG_PROTOCOL           1       // Grundig              >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SIEMENS_PROTOCOL           1       // Siemens Gigaset      >= 15000                 ~550 bytes
#define IRMP_SUPPORT_NOKIA_PROTOCOL             1       // Nokia                >= 10000                 ~300 bytes

// 27 + 8 exotic protocols, enable here!        Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRMP_SUPPORT_BOSE_PROTOCOL              1       // BOSE                 >= 10000                 ~150 bytes
#define IRMP_SUPPORT_KATHREIN_PROTOCOL          1       // Kathrein             >= 10000                 ~200 bytes
#define IRMP_SUPPORT_NUBERT_PROTOCOL            1       // NUBERT               >= 10000                  ~50 bytes
#define IRMP_SUPPORT_FAN_PROTOCOL               0       // FAN (ventilator)     >= 10000                  ~50 bytes     conflicts with NUBERT
#define IRMP_SUPPORT_SPEAKER_PROTOCOL           1       // SPEAKER (~NUBERT)    >= 10000                  ~50 bytes
#define IRMP_SUPPORT_BANG_OLUFSEN_PROTOCOL      1       // Bang & Olufsen       >= 10000                 ~200 bytes
#define IRMP_SUPPORT_RECS80_PROTOCOL            1       // RECS80 (SAA3004)     >= 15000                  ~50 bytes
#define IRMP_SUPPORT_RECS80EXT_PROTOCOL         1       // RECS80EXT (SAA3008)  >= 15000                  ~50 bytes
#define IRMP_SUPPORT_THOMSON_PROTOCOL           1       // Thomson              >= 10000                 ~250 bytes
#define IRMP_SUPPORT_NIKON_PROTOCOL             1       // NIKON camera         >= 10000                 ~250 bytes
#define IRMP_SUPPORT_NETBOX_PROTOCOL            1       // Netbox keyboard      >= 10000                 ~400 bytes (PROTOTYPE!)
#define IRMP_SUPPORT_ORTEK_PROTOCOL             0       // ORTEK (Hama)         >= 10000                 ~150 bytes     conflicts with FDC and NETBOX
#define IRMP_SUPPORT_TELEFUNKEN_PROTOCOL        1       // Telefunken 1560      >= 10000                 ~150 bytes
#define IRMP_SUPPORT_FDC_PROTOCOL               1       // FDC3402 keyboard     >= 10000 (better 15000)  ~150 bytes (~400 in combination with RC5)
#define IRMP_SUPPORT_RCCAR_PROTOCOL             1       // RC Car               >= 10000 (better 15000)  ~150 bytes (~500 in combination with RC5)
#define IRMP_SUPPORT_ROOMBA_PROTOCOL            0       // iRobot Roomba        >= 10000                 ~150 bytes     conflicts with RC6
#define IRMP_SUPPORT_RUWIDO_PROTOCOL            0       // RUWIDO, T-Home       >= 15000                 ~550 bytes     conflicts with DENON
#define IRMP_SUPPORT_A1TVBOX_PROTOCOL           1       // A1 TV BOX            >= 15000 (better 20000)  ~300 bytes
#define IRMP_SUPPORT_LEGO_PROTOCOL              1       // LEGO Power RC        >= 20000                 ~150 bytes
#define IRMP_SUPPORT_RCMM_PROTOCOL              1       // RCMM 12,24, or 32    >= 20000                 ~150 bytes
#define IRMP_SUPPORT_LGAIR_PROTOCOL             1       // LG Air Condition     >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SAMSUNG48_PROTOCOL         1       // Samsung48            >= 10000                 ~100 bytes (SAMSUNG must be enabled!)
#define IRMP_SUPPORT_MERLIN_PROTOCOL            1       // Merlin               >= 15000 (better 20000)  ~300 bytes
#define IRMP_SUPPORT_PENTAX_PROTOCOL            1       // Pentax               >= 10000 <=17000         ~150 bytes (<= 17000 due to 8 bit timing overflow issue)
#define IRMP_SUPPORT_S100_PROTOCOL              0       // S100                 >= 10000                 ~250 bytes     conflicts with RC5
#define IRMP_SUPPORT_ACP24_PROTOCOL             0       // ACP24                >= 10000                 ~250 bytes     conflicts with DENON
#define IRMP_SUPPORT_TECHNICS_PROTOCOL          1       // TECHNICS             >= 10000                 ~250 bytes
#define IRMP_SUPPORT_PANASONIC_PROTOCOL         0       // PANASONIC Beamer     >= 10000                 ~250 bytes     conflicts with KASEIKYO
#define IRMP_SUPPORT_MITSU_HEAVY_PROTOCOL       1       // Mitsubishi Aircond   >= 10000                 ~250 bytes
#define IRMP_SUPPORT_VINCENT_PROTOCOL           1       // VINCENT              >= 10000                 ~250 bytes
#define IRMP_SUPPORT_SAMSUNGAH_PROTOCOL         1       // SAMSUNG AH           >= 10000                 ~250 bytes
#define IRMP_SUPPORT_IRMP16_PROTOCOL            0       // IRMP specific        >= 15000                 ~250 bytes
#define IRMP_SUPPORT_GREE_PROTOCOL              1       // GREE CLIMATE         >= 10000 <=17000         ~250 bytes
#define IRMP_SUPPORT_RCII_PROTOCOL              0       // RCII T+A             >= 15000                 ~250 bytes     conflicts with GRUNDIG and NOKIA
#define IRMP_SUPPORT_METZ_PROTOCOL              1
#define IRMP_SUPPORT_MELINERA_PROTOCOL          1       // MELINERA (Lidl)      >= 10000

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Protocols Part 2: RF decoders
 * If you use an IR sensor, deactivate all RF protocols!
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#define IRMP_SUPPORT_RF_GEN24_PROTOCOL          0       // RF GEN24 (generic)   >= 15000                 ~250 bytes
#define IRMP_SUPPORT_RF_X10_PROTOCOL            0       // RF PC X10 (Medion)   >= 15000                 ~250 bytes
#define IRMP_SUPPORT_RF_MEDION_PROTOCOL         0       // RF PC Medion         >= 15000                 ~250 bytes


#endif // IRMP_SELECT_PROTOCOLS_H
