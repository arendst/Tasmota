/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpSelectMain15Protocols.h
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
 * Change settings from 1 to 0 (or deactivate it) if you want to disable one or more decoders.
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

// typical protocols, disable here!             Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRMP_SUPPORT_SIRCS_PROTOCOL             1       // Sony SIRCS           >= 10000                 ~150 bytes
#define IRMP_SUPPORT_NEC_PROTOCOL               1       // NEC + APPLE + ONKYO  >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SAMSUNG_PROTOCOL           1       // Samsung + Samsg32    >= 10000                 ~300 bytes
#define IRMP_SUPPORT_KASEIKYO_PROTOCOL          1       // Kaseikyo             >= 10000                 ~250 bytes

// more protocols, enable here!                 Enable  Remarks                 F_INTERRUPTS            Program Space
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

#endif // IRMP_SELECT_PROTOCOLS_H
