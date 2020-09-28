/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irsndSelectMain15Protocols.h
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
 * 1 enable  decoder
 * 0 disable decoder
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

#endif // IRSND_SELECT_PROTOCOLS_H
