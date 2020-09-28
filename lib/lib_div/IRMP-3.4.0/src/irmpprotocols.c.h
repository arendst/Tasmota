/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpprotocols.c.h - irmp protocols
 *
 * DO NOT INCLUDE THIS FILE, WILL BE INCLUDED BY IRMP.H or IRSND.H!
 *
 * Copyright (c) 2020 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _IRMP_PROTOCOLS_C_H_
#define _IRMP_PROTOCOLS_C_H_

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * IR protocol strings for IRMP and IRSND:
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
const char proto_unknown[]       PROGMEM = "UNKNOWN";
const char proto_sircs[]         PROGMEM = "SIRCS";
const char proto_nec[]           PROGMEM = "NEC";
const char proto_samsung[]       PROGMEM = "SAMSUNG";
const char proto_matsushita[]    PROGMEM = "MATSUSH";
const char proto_kaseikyo[]      PROGMEM = "KASEIKYO";
const char proto_recs80[]        PROGMEM = "RECS80";
const char proto_rc5[]           PROGMEM = "RC5";
const char proto_denon[]         PROGMEM = "DENON";
const char proto_rc6[]           PROGMEM = "RC6";
const char proto_samsung32[]     PROGMEM = "SAMSG32";
const char proto_apple[]         PROGMEM = "APPLE";
const char proto_recs80ext[]     PROGMEM = "RECS80EX";
const char proto_nubert[]        PROGMEM = "NUBERT";
const char proto_bang_olufsen[]  PROGMEM = "BANG OLU";
const char proto_grundig[]       PROGMEM = "GRUNDIG";
const char proto_nokia[]         PROGMEM = "NOKIA";
const char proto_siemens[]       PROGMEM = "SIEMENS";
const char proto_fdc[]           PROGMEM = "FDC";
const char proto_rccar[]         PROGMEM = "RCCAR";
const char proto_jvc[]           PROGMEM = "JVC";
const char proto_rc6a[]          PROGMEM = "RC6A";
const char proto_nikon[]         PROGMEM = "NIKON";
const char proto_ruwido[]        PROGMEM = "RUWIDO";
const char proto_ir60[]          PROGMEM = "IR60";
const char proto_kathrein[]      PROGMEM = "KATHREIN";
const char proto_netbox[]        PROGMEM = "NETBOX";
const char proto_nec16[]         PROGMEM = "NEC16";
const char proto_nec42[]         PROGMEM = "NEC42";
const char proto_lego[]          PROGMEM = "LEGO";
const char proto_thomson[]       PROGMEM = "THOMSON";
const char proto_bose[]          PROGMEM = "BOSE";
const char proto_a1tvbox[]       PROGMEM = "A1TVBOX";
const char proto_ortek[]         PROGMEM = "ORTEK";
const char proto_telefunken[]    PROGMEM = "TELEFUNKEN";
const char proto_roomba[]        PROGMEM = "ROOMBA";
const char proto_rcmm32[]        PROGMEM = "RCMM32";
const char proto_rcmm24[]        PROGMEM = "RCMM24";
const char proto_rcmm12[]        PROGMEM = "RCMM12";
const char proto_speaker[]       PROGMEM = "SPEAKER";
const char proto_lgair[]         PROGMEM = "LGAIR";
const char proto_samsung48[]     PROGMEM = "SAMSG48";
const char proto_merlin[]        PROGMEM = "MERLIN";
const char proto_pentax[]        PROGMEM = "PENTAX";
const char proto_fan[]           PROGMEM = "FAN";
const char proto_s100[]          PROGMEM = "S100";
const char proto_acp24[]         PROGMEM = "ACP24";
const char proto_technics[]      PROGMEM = "TECHNICS";
const char proto_panasonic[]     PROGMEM = "PANASONIC";
const char proto_mitsu_heavy[]   PROGMEM = "MITSU_HEAVY";
const char proto_vincent[]       PROGMEM = "VINCENT";
const char proto_samsungah[]     PROGMEM = "SAMSUNGAH";
const char proto_irmp16[]        PROGMEM = "IRMP16";
const char proto_gree[]          PROGMEM = "GREE";
const char proto_rcii[]          PROGMEM = "RCII";
const char proto_metz[]          PROGMEM = "METZ";
const char proto_onkyo[]         PROGMEM = "ONKYO";

const char proto_rf_gen24[]      PROGMEM = "RF_GEN24";
const char proto_rf_x10[]        PROGMEM = "RF_X10";
const char proto_rf_medion[]     PROGMEM = "RF_MEDION";

const char proto_melinera[]      PROGMEM = "MELINERA";

/*
 * Must be in the same order as the Protocol numbers in irmpprotocols.h starting with IRMP_UNKNOWN_PROTOCOL = 0
 */
const char * const
irmp_protocol_names[IRMP_N_PROTOCOLS + 1] PROGMEM =
{
    proto_unknown,
    proto_sircs,
    proto_nec,
    proto_samsung,
    proto_matsushita,
    proto_kaseikyo,
    proto_recs80,
    proto_rc5,
    proto_denon,
    proto_rc6,
    proto_samsung32,
    proto_apple,
    proto_recs80ext,
    proto_nubert,
    proto_bang_olufsen,
    proto_grundig,
    proto_nokia,
    proto_siemens,
    proto_fdc,
    proto_rccar,
    proto_jvc,
    proto_rc6a,
    proto_nikon,
    proto_ruwido,
    proto_ir60,
    proto_kathrein,
    proto_netbox,
    proto_nec16,
    proto_nec42,
    proto_lego,
    proto_thomson,
    proto_bose,
    proto_a1tvbox,
    proto_ortek,
    proto_telefunken,
    proto_roomba,
    proto_rcmm32,
    proto_rcmm24,
    proto_rcmm12,
    proto_speaker,
    proto_lgair,
    proto_samsung48,
    proto_merlin,
    proto_pentax,
    proto_fan,
    proto_s100,
    proto_acp24,
    proto_technics,
    proto_panasonic,
    proto_mitsu_heavy,
    proto_vincent,
    proto_samsungah,
    proto_irmp16,
    proto_gree,
    proto_rcii,
    proto_metz,
    proto_onkyo,

    proto_rf_gen24,
    proto_rf_x10,
    proto_rf_medion,

    proto_melinera
};

#endif // _IRMP_PROTOCOLS_C_H_
