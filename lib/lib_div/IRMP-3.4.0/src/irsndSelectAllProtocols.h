/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irsndSelectAllProtocols.h
 * 39 protocols supported
 *
 *
 * Copyright (c) 2010-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef IRSND_SELECT_PROTOCOLS_H
#define IRSND_SELECT_PROTOCOLS_H

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Change settings from 1 to 0 if you want to disable one or more encoders.
 * This saves program space.
 * 1 enable  encoder
 * 0 disable encoder
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
// typical protocols, disable here!             Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_SIRCS_PROTOCOL            1       // Sony SIRCS           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_NEC_PROTOCOL              1       // NEC + APPLE          >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SAMSUNG_PROTOCOL          1       // Samsung + Samsung32  >= 10000                 ~300 bytes
#define IRSND_SUPPORT_KASEIKYO_PROTOCOL         1       // Kaseikyo             >= 10000                 ~300 bytes

// more protocols, enable here!                 Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_JVC_PROTOCOL              1       // JVC                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC16_PROTOCOL            1       // NEC16                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC42_PROTOCOL            1       // NEC42                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_MATSUSHITA_PROTOCOL       1       // Matsushita           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_DENON_PROTOCOL            1       // DENON, Sharp         >= 10000                 ~200 bytes
#define IRSND_SUPPORT_RC5_PROTOCOL              1       // RC5                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RC6_PROTOCOL              1       // RC6                  >= 10000                 ~250 bytes
#define IRSND_SUPPORT_RC6A_PROTOCOL             1       // RC6A                 >= 10000                 ~250 bytes
#define IRSND_SUPPORT_GRUNDIG_PROTOCOL          1       // Grundig              >= 10000                 ~300 bytes
#define IRSND_SUPPORT_SIEMENS_PROTOCOL          1       // Siemens, Gigaset     >= 15000                 ~150 bytes
#define IRSND_SUPPORT_NOKIA_PROTOCOL            1       // Nokia                >= 10000                 ~400 bytes

// exotic protocols, enable here!               Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_IR60_PROTOCOL             0       // IR60 (SDA2008)       >= 10000                 ~250 bytes // IR frequency 30 kHz
#define IRSND_SUPPORT_BOSE_PROTOCOL             1       // BOSE                 >= 10000                 ~100 bytes
#define IRSND_SUPPORT_KATHREIN_PROTOCOL         0       // Kathrein             >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_NUBERT_PROTOCOL           1       // NUBERT               >= 10000                 ~100 bytes
#define IRSND_SUPPORT_FAN_PROTOCOL              1       // FAN (ventilator)     >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SPEAKER_PROTOCOL          1       // SPEAKER              >= 10000                 ~100 bytes
#define IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL     0       // Bang&Olufsen         >= 10000                 ~250 bytes // IR frequency 455 kHz
#define IRSND_SUPPORT_RECS80_PROTOCOL           1       // RECS80               >= 15000                 ~100 bytes
#define IRSND_SUPPORT_RECS80EXT_PROTOCOL        1       // RECS80EXT            >= 15000                 ~100 bytes
#define IRSND_SUPPORT_THOMSON_PROTOCOL          1       // Thomson              >= 10000                 ~250 bytes
#define IRSND_SUPPORT_NIKON_PROTOCOL            1       // NIKON                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NETBOX_PROTOCOL           0       // Netbox keyboard      >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_ORTEK_PROTOCOL            0       // ORTEK (Hama)         >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_TELEFUNKEN_PROTOCOL       1       // Telefunken 1560      >= 10000                 ~150 bytes
#define IRSND_SUPPORT_FDC_PROTOCOL              1       // FDC IR keyboard      >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_RCCAR_PROTOCOL            1       // RC CAR               >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_ROOMBA_PROTOCOL           1       // iRobot Roomba        >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RUWIDO_PROTOCOL           1       // RUWIDO, T-Home       >= 15000                 ~250 bytes
#define IRSND_SUPPORT_A1TVBOX_PROTOCOL          1       // A1 TV BOX            >= 15000 (better 20000)  ~200 bytes
#define IRSND_SUPPORT_LEGO_PROTOCOL             1       // LEGO Power RC        >= 20000                 ~150 bytes
#define IRSND_SUPPORT_RCMM_PROTOCOL             0       // RCMM 12,24, or 32    >= 20000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_LGAIR_PROTOCOL            1       // LG Air Condition     >= 10000                 ~150 bytes.
#define IRSND_SUPPORT_SAMSUNG48_PROTOCOL        1       // Samsung48            >= 10000                 ~100 bytes
#define IRSND_SUPPORT_PENTAX_PROTOCOL           1       // Pentax               >= 10000                 ~150 bytes
#define IRSND_SUPPORT_S100_PROTOCOL             1       // S100                 >= 10000                 ~150 bytes
#define IRSND_SUPPORT_ACP24_PROTOCOL            1       // ACP24                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_TECHNICS_PROTOCOL         0       // TECHNICS             >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_PANASONIC_PROTOCOL        1       // PANASONIC Beamer     >= 10000                 ~150 bytes
#define IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL      1       // Mitsubishi-Heavy Aircondition, similar Timing to Panasonic beamer
#define IRSND_SUPPORT_IRMP16_PROTOCOL           0       // IRMP specific        >= 15000                 ~250 bytes

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Protocols Part 2: RF decoders
 * If you use an IR sensor, deactivate all RF protocols!
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#define IRMP_SUPPORT_RF_GEN24_PROTOCOL          0       // RF GEN24 (generic)   >= 15000                 ~250 bytes
#define IRMP_SUPPORT_RF_X10_PROTOCOL            0       // RF PC X10 (Medion)   >= 15000                 ~250 bytes
#define IRMP_SUPPORT_RF_MEDION_PROTOCOL         0       // RF PC Medion         >= 15000                 ~250 bytes

#endif // IRSND_SELECT_PROTOCOLS_H
