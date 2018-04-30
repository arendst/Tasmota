/*
 * debug.h
 *
 *  Created on: Dec 4, 2014
 *      Author: Minh
 */

#ifndef USER_DEBUG_H_
#define USER_DEBUG_H_


#if defined(MQTT_DEBUG_ON)
#define MQTT_INFO( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define MQTT_INFO( format, ... )
#endif


#endif /* USER_DEBUG_H_ */
