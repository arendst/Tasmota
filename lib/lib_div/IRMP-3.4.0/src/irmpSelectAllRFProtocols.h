/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpSelectAllRFProtocols.h
 * 2 protocols enabled
 *
 * Copyright (c) 2020 Frank Meyer - frank(at)fli4l.de
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
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Protocols Part 2: RF decoders
 * If you use an IR sensor, deactivate all RF protocols!
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#define IRMP_SUPPORT_RF_GEN24_PROTOCOL          1       // RF GEN24 (generic)   >= 15000                 ~250 bytes
#define IRMP_SUPPORT_RF_X10_PROTOCOL            1       // RF PC X10 (Medion)   >= 15000                 ~250 bytes
#define IRMP_SUPPORT_RF_MEDION_PROTOCOL         1       // RF PC Medion         >= 15000                 ~250 bytes

// Usually IR sensors are low active, RF receivers are high active.
#define IRMP_HIGH_ACTIVE                        1       // set to 1 if you use a RF receiver!

#endif // IRMP_SELECT_PROTOCOLS_H
