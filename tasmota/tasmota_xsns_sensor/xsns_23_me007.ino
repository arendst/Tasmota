/*
  xsns_23_me007ula.ino - ME007 ultrasonic sensor support for Tasmota

  Copyright (C) 2022  Mathias Buder

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

#ifdef USE_ME007
/*********************************************************************************************\
 * ME007 - Ultrasonic distance sensor
 *
 * Code for ME007 family of ultrasonic distance sensors
 * References:
 * - https://wiki.dfrobot.com/Water-proof%20Ultrasonic%20Sensor%20(ULS)%20%20SKU:%20SEN0300
\*********************************************************************************************/

/*********************************************************************************************/
/* Includes*/
/*********************************************************************************************/
#include <TasmotaSerial.h>

/*********************************************************************************************/
/* Defines */
/*********************************************************************************************/
#define XSNS_23                            23

#define ME007_VERSION                      "1.0.0"                                              /**< Driver version X.Y.Z: X:Major, Y: Minor, Z: Patch */

#define ME007_DEBUG_MSG_TAG                "ME7: "
#define ME007_WS_MQTT_MSG_TAG              "ME7"
#define ME007_SENSOR_ERROR_CNT_CURRENT_TAG "ErrorCounterCurrent"
#define ME007_SENSOR_ERROR_CNT_TOTAL_TAG   "ErrorCounterTotal"
#define ME007_SENSOR_VERSION_TAG           "DriverVersion"

#define ME007_MIN_SENSOR_DISTANCE          30U                                                  /**< Minimum measurement distance @unit cm */
#ifndef ME007_MAX_SENSOR_DISTANCE
#define ME007_MAX_SENSOR_DISTANCE          800U                                                 /**< Maximum measurement distance @unit cm */
#endif
#define ME007_SERIAL_IF_BAUD_RATE          9600U                                                 /**< Serial interface baud rate @unit Baud */
#define ME007_SERIAL_SOF                   0xFF                                                  /**< Start of frame indicator (header) */
#define ME007_SERIAL_FRAME_SIZE            6U                                                    /**< Total frame size: Header (1Byte) + Distance (2 byte) + Temperature (2 byte) + Checksum (1 byte) = 6 byte */
#define ME007_SERIAL_MAX_WAIT_TIME         120U                                                  /**< Max. wait time for data after trigger signal @unit ms */
#define ME007_SERIAL_MAX_DATA_RECEIVE_TIME 500U                                                  /**< Max. time to receive entire data frame @unit ms */
#define ME007_TRIG_SIG_DURATION_MS         1U                                                    /**< Time duration of trigger low-pulse @unit ms */
#define ME007_MEDIAN_FILTER_SIZE           5U                                                    /**< Median filter samples, must be an odd number @unit sample */
#define ME007_MEDIAN_FILTER_MEDIAN_IDX     ( ( uint8_t )( ME007_MEDIAN_FILTER_SIZE - 1U) / 2U )  /**< Median filter samples @unit sample */
#define ME007_MEDIAN_FILTER_MEASURE_DELAY  30U                                                   /**< Small delay between consecutive measurements @unit ms */
#define ME007_SENSOR_NUM_ERROR             10U                                                   /**< Number of tries to detect sensor */

/*********************************************************************************************/
/* Enums */
/*********************************************************************************************/
/**
 * @details Sensor readings type
 */
enum ME007_SHOW_TYPE
{
    ME007_SHOW_TYPE_JS = 0U, /**< @details Domain log message tag string */
    ME007_SHOW_TYPE_WS
};

/**
 * @details Sensor serial interface mode type
 */
typedef enum ME007_SERIAL_RECEIVE_TYPE_TAG
{
    ME007_SERIAL_RECEIVE_TYPE_SOF = 0U, /**< @details Receive Start-Of-Frame character */
    ME007_SERIAL_RECEIVE_TYPE_DATA      /**< @details Receive data (distance + temperature + checksum) */
} ME007_SERIAL_RECEIVE_TYPE;

/**
 * @details Sensor serial interface byte type
 */
enum ME007_SERIAL_BYTE_TYPE
{
    ME007_SERIAL_BYTE_TYPE_SOF = 0U, /**< @details Receive Start-Of-Frame character */
    ME007_SERIAL_BYTE_TYPE_DIST_H,   /**< @details Distance MSB */
    ME007_SERIAL_BYTE_TYPE_DIST_L,   /**< @details Distance LSB */
    ME007_SERIAL_BYTE_TYPE_TEMP_H,   /**< @details Temperature  MSB */
    ME007_SERIAL_BYTE_TYPE_TEMP_L,   /**< @details Temperature  LSB */
    ME007_SERIAL_BYTE_TYPE_CHECKSUM  /**< @details Frame checksum */
};

/**
 * @details Global sensor error type
 */
typedef enum ME007_ERROR_TYPE_TAG
{
    ME007_ERROR_TYPE_NONE = 0U, /**< @details No error present */
    ME007_ERROR_TYPE_TIMEOUT,   /**< @details Serial frame not receive in time  */
    ME007_ERROR_TYPE_CRC        /**< @details Checksum calculate/compare failed */
} ME007_ERROR_TYPE;

/**
 * @details Global sensor state type
 */
typedef enum ME007_STATE_TYPE_TAG
{
    ME007_STATE_NOT_DETECTED = 0U, /**< @details Sensor not detected */
    ME007_STATE_DETECTED           /**< @details Sensor detected */
} ME007_STATE_TYPE;

/*********************************************************************************************/
/* Structures */
/*********************************************************************************************/

/**
 * @details Global sensor data structure
 */
struct
{
    uint8_t          pin_rx_u8;            /**< @details Serial interface receive pin */
    uint8_t          pin_trig_u8;          /**< @details Sensor trigger pin */
    ME007_STATE_TYPE state_e;              /**< @details Global sensor state */
    float            distance_cm_f32;      /**< @details Output distance measurement @unit cm */
    float            temperature_deg_f32;  /**< @details Output temperature measurement @unit °C */
    uint8_t          error_cnt_current_u8; /**< @details Measurement error counter (current) */
    uint16_t         error_cnt_total_u16;  /**< @details Measurement error counter (total) */
} me007_data_s;

/*********************************************************************************************/
/* Global Variables */
/*********************************************************************************************/
TasmotaSerial* gp_serial_if = nullptr; /**< @details Pointer to serial interface object */

/*********************************************************************************************/
/* Function Prototypes */
/*********************************************************************************************/

/**
 * @details This function initializes the sensor driver and its underlying serial interface.
 */
void me007_init( void );

/**
 * @details This function performs a single distance/temperature measurement.
 * @param[out] float* p_distance_cm_f32 Pointer to variable supposed to store the current distance reading.
 * @param[out] float* p_temperature_f32 Pointer to variable supposed to store the current temperature reading.
 * @return ME007_ERROR_TYPE Status of current measurement.
 */
ME007_ERROR_TYPE me007_measure( float* const p_distance_cm_f32, float* const p_temperature_f32 );

/**
 * @details This function sorts a list if float values in ascending order.
 * @param[in] const void* p_list Pointer to list to be sorted
 * @param[in] const uint8_t size_u8 Size of list to be sorted.
 */
#ifdef ME007_ENABLE_MEDIAN_FILTER
void me007_sort_asc( float* const p_list, const uint8_t size_u8 );
#endif

/**
 * @details This function performs multiple sensor measurements and filters the output if enables.
 */
void me007_read_value( void );

/**
 * @details This function sends the current distance/temperature measurements to the web-interface / MQTT / Domoticz.
 * @param[in] ME007_SHOW_TYPE type_e Variable to decide where to output the sensor measurements.
 */
void me007_show( const ME007_SHOW_TYPE type_e );

/*********************************************************************************************/
/* Function Definitions */
/*********************************************************************************************/
void me007_init( void )
{
    AddLog( LOG_LEVEL_INFO, PSTR( ME007_DEBUG_MSG_TAG "Initializing ..." ) );

    /* Check if sensor pins are selected/used in web-interface */
    if (    ( false == PinUsed( GPIO_ME007_TRIG ) )
         || ( false == PinUsed( GPIO_ME007_RX ) ) )
    {
        AddLog( LOG_LEVEL_ERROR, PSTR( ME007_DEBUG_MSG_TAG "Serial/Trigger interface not configured" ) );
        return;
    }

    /* Init some global sensor data structure elements */
    me007_data_s.state_e              = ME007_STATE_NOT_DETECTED;
    me007_data_s.error_cnt_current_u8 = 0U;
    me007_data_s.error_cnt_total_u16  = 0U;

    /* Store real pin number */
    me007_data_s.pin_rx_u8   = Pin( GPIO_ME007_RX );
    me007_data_s.pin_trig_u8 = Pin( GPIO_ME007_TRIG );

    DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Using GPIOs: Trigger: %i / Rx: %i)" ),
                      me007_data_s.pin_trig_u8,
                      me007_data_s.pin_rx_u8 );

    /* Configure serial interface */
    /* Only Rx pin is required as ME007 is controlled using its trigger pin. Therefore, passing value "-1" as transmit_pin argument */
    gp_serial_if = new TasmotaSerial( me007_data_s.pin_rx_u8, -1, 2U );

    if (    ( nullptr != gp_serial_if )
         && ( true    == gp_serial_if->begin( ME007_SERIAL_IF_BAUD_RATE ) ) )
    {
        if ( true == gp_serial_if->hardwareSerial() )
        {
            ClaimSerial();
        }
#ifdef ESP32
        AddLog(LOG_LEVEL_DEBUG, PSTR(ME007_DEBUG_MSG_TAG "Serial UART%d"), gp_serial_if->getUart());
#endif

        pinMode( me007_data_s.pin_trig_u8, OUTPUT );    /**< @details Configure trigger pin as output */
        digitalWrite( me007_data_s.pin_trig_u8, HIGH ); /**< @details Set trigger pin to high-level as it ME007 requires a falling edge to initiate measurement */

        /* Detect sensor */
        AddLog( LOG_LEVEL_INFO, PSTR( ME007_DEBUG_MSG_TAG "Detecting ..." ) );
        for ( uint8_t idx_u8 = 0U; idx_u8 < ME007_SENSOR_NUM_ERROR; ++idx_u8 )
        {
            ME007_ERROR_TYPE detected_e = me007_measure( &me007_data_s.distance_cm_f32,
                                                         &me007_data_s.temperature_deg_f32 );
            if ( ME007_ERROR_TYPE_NONE == detected_e )
            {
                me007_data_s.state_e = ME007_STATE_DETECTED;
                break;
            }
            else
            {
                DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Measurement error: %i" ), idx_u8 );
            }
        }
        AddLog( LOG_LEVEL_INFO, PSTR( ME007_DEBUG_MSG_TAG "%s" ), me007_data_s.state_e == ME007_STATE_DETECTED ? "Detected" : "Not detected, Sensor deactivated" );
    }
    else
    {
        AddLog( LOG_LEVEL_ERROR, PSTR( ME007_DEBUG_MSG_TAG "Serial interface unavailable" ) );
    }
}

ME007_ERROR_TYPE me007_measure( float* const p_distance_cm_f32, float* const p_temperature_f32 )
{
    ME007_SERIAL_RECEIVE_TYPE state_e                             = ME007_SERIAL_RECEIVE_TYPE_SOF;  /**< @details Always start trying to receive SOF */
    uint8_t                   buffer_vu8[ME007_SERIAL_FRAME_SIZE] = {0U};
    uint8_t                   buffer_idx_u8                       = 0U;
    uint8_t                   data_byte_u8                        = 0U;
    uint32_t                  timestamp_ms_u32                    = 0U;

    if (    ( nullptr != p_distance_cm_f32 )
         && ( nullptr != p_temperature_f32 )
         && ( nullptr != gp_serial_if ) )
    {
        /* Trigger new sensor measurement */
        digitalWrite( me007_data_s.pin_trig_u8, LOW );
        timestamp_ms_u32 = millis(); /**< @details Store trigger time */
        DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Sensor reading triggered" ) );
        delay( ME007_TRIG_SIG_DURATION_MS ); /**< @details Wait 1ms to give the oin time to go low */
        digitalWrite( me007_data_s.pin_trig_u8, HIGH );

        /* Give sensor some time to take a measurement and send the result */
        /* Max. wait time should be T2max + T3max = 61 ms (see https://wiki.dfrobot.com/Water-proof%20Ultrasonic%20Sensor%20(ULS)%20%20SKU:%20SEN0300, section "Serial Output" for details) */
        while ( ( timestamp_ms_u32 + ME007_SERIAL_MAX_WAIT_TIME ) >= millis() )
        {
            if ( 0U < gp_serial_if->available() )
            {
                /* Read 1 byte of data */
                data_byte_u8 = gp_serial_if->read();

                DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Serial: Byte received: 0x%x" ), data_byte_u8 );

                /* Serial Data Frame Layout
                 +──────────────────+─────────────────────+────────────────────+────────────────────────+───────────────────────+────────────+
                 | Frame Header ID  | Distance Data High  | Distance Data Low  | Temperature Data High  | Temperature Data Low  | Checksum   |
                 +──────────────────+─────────────────────+────────────────────+────────────────────────+───────────────────────+────────────+
                 |       0xFF       |        Data_H       |       Data_L       |         Temp_H         |         Temp_L        |    SUM     |
                 +──────────────────+─────────────────────+────────────────────+────────────────────────+───────────────────────+────────────+ */

                switch ( state_e )
                {
                case ME007_SERIAL_RECEIVE_TYPE_SOF:
                    if ( ME007_SERIAL_SOF == data_byte_u8 )
                    {
                        buffer_vu8[buffer_idx_u8++] = data_byte_u8;
                        state_e                     = ME007_SERIAL_RECEIVE_TYPE_DATA;
                        DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "(Idx: %i) Serial: SOF detected" ), buffer_idx_u8 );
                    }
                    break;

                case ME007_SERIAL_RECEIVE_TYPE_DATA:
                    buffer_vu8[buffer_idx_u8++] = data_byte_u8;
                    DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "(Idx: %i) Receiving data: 0x%x" ), buffer_idx_u8, data_byte_u8 );

                    /* If all bytes have been received: calculate checksum and assembly date */
                    if ( ME007_SERIAL_FRAME_SIZE <= buffer_idx_u8 )
                    {
                        /* Calculate expected checksum (only lower 8 bit shall be used) */
                        uint8_t checksum_u8 = (   buffer_vu8[ME007_SERIAL_BYTE_TYPE_SOF]
                                                + buffer_vu8[ME007_SERIAL_BYTE_TYPE_DIST_H]
                                                + buffer_vu8[ME007_SERIAL_BYTE_TYPE_DIST_L]
                                                + buffer_vu8[ME007_SERIAL_BYTE_TYPE_TEMP_H]
                                                + buffer_vu8[ME007_SERIAL_BYTE_TYPE_TEMP_L] ) & 0x00FF;

                        DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Comparing expected sum %i to checksum %i" ), checksum_u8, buffer_vu8[ME007_SERIAL_BYTE_TYPE_CHECKSUM] );

                        /* Compare expected and receive checksum */
                        if ( checksum_u8 == buffer_vu8[ME007_SERIAL_BYTE_TYPE_CHECKSUM] )
                        {
                            /* Assemble and scale distance to cm */
                            *p_distance_cm_f32 = ( ( buffer_vu8[ME007_SERIAL_BYTE_TYPE_DIST_H] << 8U ) + buffer_vu8[ME007_SERIAL_BYTE_TYPE_DIST_L] ) / 10.0F;

                            /* Apply physical sensor measurement limits */
                            if ( ME007_MIN_SENSOR_DISTANCE > *p_distance_cm_f32 )
                            {
                                *p_distance_cm_f32 = 0.0F;
                            }
                            else if ( ME007_MAX_SENSOR_DISTANCE < *p_distance_cm_f32 )
                            {
                                *p_distance_cm_f32 = ME007_MAX_SENSOR_DISTANCE;
                            }
                            else
                            {
                                /* Ok: Measurement is within the physical sensor measurement limits */
                            }

                            /* Assemble and scale temperature °C */
                            /* Check sign-bit (MSB) */
                            if ( 0x80 == ( buffer_vu8[ME007_SERIAL_BYTE_TYPE_TEMP_H] & 0x80 ) )
                            {
                                buffer_vu8[2U] ^= 0x80;
                            }

                            *p_temperature_f32 = ( ( buffer_vu8[ME007_SERIAL_BYTE_TYPE_TEMP_H] << 8U ) + buffer_vu8[ME007_SERIAL_BYTE_TYPE_TEMP_L] ) / 10.0F;

                            DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Distance: %s cm, Temperature: %s °C" ),
                                                    String( *p_distance_cm_f32, 1U ).c_str(),
                                                    String( *p_temperature_f32, 1U ).c_str() );

                            /* All ok: Measurement valid */
                            DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Measurement valid" ) );
                            return ME007_ERROR_TYPE_NONE;
                        }
                        else
                        {
                            /* Checksum nok: Measurement invalid */
                            DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Checksum nok: Measurement invalid" ) );
                            return ME007_ERROR_TYPE_CRC;
                        }
                    }
                    break;

                    default: /* Should never happen */
                        break;
                }
            }
        }
    }

    /* Timeout: Measurement invalid */
    DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Timeout: Measurement invalid" ) );
    return ME007_ERROR_TYPE_TIMEOUT;
}

#ifdef ME007_ENABLE_MEDIAN_FILTER
void me007_sort_asc( float* const p_list, const uint8_t size_u8 )
{
    if( NULL != p_list )
    {
        for( uint8_t idx_u8 = 0U; idx_u8 < size_u8; ++idx_u8 )
        {
            for( uint8_t idy_u8 = ( idx_u8 + 1U ); idy_u8 < size_u8; ++idy_u8 )
            {
                if( p_list[idx_u8] > p_list[idy_u8] )
                {
                    float tmp_f32  = p_list[idx_u8];
                    p_list[idx_u8] = p_list[idy_u8];
                    p_list[idy_u8] = tmp_f32;
                }
            }
        }
    }
}
#endif

void me007_read_value( void )
{
    float distance_cm_f32 = 0.0F;
#ifdef ME007_ENABLE_MEDIAN_FILTER
    float distance_buffer_vf32[ME007_MEDIAN_FILTER_SIZE] = {0.0F};
#endif

    /* Record some sensor measurements */
#ifdef ME007_ENABLE_MEDIAN_FILTER
    for ( uint8_t idx_u8 = 0U; idx_u8 < ME007_MEDIAN_FILTER_SIZE; ++idx_u8 )
    {
#endif
        ME007_ERROR_TYPE status_e = me007_measure( &distance_cm_f32,
                                                   &me007_data_s.temperature_deg_f32 );

        switch ( status_e )
        {
        case ME007_ERROR_TYPE_NONE:

#ifdef ME007_ENABLE_MEDIAN_FILTER
            /* Store valid distance measurement into histogram buffer */
            distance_buffer_vf32[idx_u8] = distance_cm_f32;
#else
            me007_data_s.distance_cm_f32 = distance_cm_f32;
#endif

            if ( 0U < me007_data_s.error_cnt_current_u8 )
            {
                me007_data_s.error_cnt_current_u8--; /* Decrease current measurement errors by one */
            }
            break;

        case ME007_ERROR_TYPE_CRC:
        case ME007_ERROR_TYPE_TIMEOUT:
            me007_data_s.error_cnt_current_u8++; /* Increase current measurement errors by one */
            me007_data_s.error_cnt_total_u16++;  /* Store total number of measurement errors */
            break;

        default: /* Should never happen */
            break;
        }

        /* Check error counter and only print message to error log (for now)
           in case error was present for at least ME007_SENSOR_NUM_ERROR cycles.
           For some reason, the checksum check fails quite ofter and because
           of that it wouldn't make sense to deactivate ME007 in case
           ME007_SENSOR_NUM_ERROR is exceeded. */
        if ( ME007_SENSOR_NUM_ERROR <= me007_data_s.error_cnt_current_u8 )
        {
            AddLog( LOG_LEVEL_ERROR, PSTR( ME007_DEBUG_MSG_TAG "Error @ counter: %i" ), me007_data_s.error_cnt_current_u8 );
        }

        DEBUG_SENSOR_LOG( PSTR( ME007_DEBUG_MSG_TAG "Error counter: Current: %i, Total: %i" ),
                                me007_data_s.error_cnt_current_u8,
                                me007_data_s.error_cnt_total_u16 );

#ifdef ME007_ENABLE_MEDIAN_FILTER
        /* Add small delay between measurement */
        if ( ( ME007_MEDIAN_FILTER_SIZE - 1U ) > idx_u8 )
        {
            delay( ME007_MEDIAN_FILTER_MEASURE_DELAY );
        }
    }

    /* Sort median filter buffer and assign median value to current distance measurement */
    me007_sort_asc( distance_buffer_vf32, ME007_MEDIAN_FILTER_SIZE );

    /* Update distance measurement */
    me007_data_s.distance_cm_f32 = distance_buffer_vf32[ME007_MEDIAN_FILTER_MEDIAN_IDX];
#endif

}

void me007_show( const ME007_SHOW_TYPE type_e )
{
    switch ( type_e )
    {
    case ME007_SHOW_TYPE_JS:
        ResponseAppend_P( PSTR( ",\"" ME007_WS_MQTT_MSG_TAG "\":{\"" D_JSON_DISTANCE "\":%1_f,\"" D_JSON_TEMPERATURE "\":%1_f,\"" ME007_SENSOR_ERROR_CNT_CURRENT_TAG "\":%i,\"" ME007_SENSOR_ERROR_CNT_TOTAL_TAG "\":%i,\"" ME007_SENSOR_VERSION_TAG "\":\"" ME007_VERSION "\"}" ),
                          &me007_data_s.distance_cm_f32,
                          &me007_data_s.temperature_deg_f32,
                          me007_data_s.error_cnt_current_u8,
                          me007_data_s.error_cnt_total_u16 );
#ifdef USE_DOMOTICZ
        if ( 0U == TasmotaGlobal.tele_period )
        {
            DomoticzFloatSensor( DZ_COUNT, me007_data_s.distance_cm_f32 );    /**< @details Send distance as Domoticz counter value */
            DomoticzFloatSensor( DZ_TEMP, me007_data_s.temperature_deg_f32 ); /**< @details Send distance as Domoticz temperature value */
        }
#endif /* USE_DOMOTICZ */
        break;
#ifdef USE_WEBSERVER
    case ME007_SHOW_TYPE_WS:
        WSContentSend_PD( HTTP_SNS_F_DISTANCE_CM,
                            ME007_WS_MQTT_MSG_TAG,
                            &me007_data_s.distance_cm_f32 );

        WSContentSend_PD( HTTP_SNS_F_TEMP,
                          ME007_WS_MQTT_MSG_TAG,
                          Settings->flag2.temperature_resolution,
                          &me007_data_s.temperature_deg_f32 );
        break;
#endif /* USE_WEBSERVER */

    default: /* Should never happen */
        break;
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns23( uint32_t function )
{
    bool result_b = false;

    switch ( function )
    {
    case FUNC_INIT:
        me007_init();
        break;

    case FUNC_EVERY_SECOND:
        if ( ME007_STATE_DETECTED == me007_data_s.state_e )
        {
            me007_read_value();
            result_b = true;
        }
        break;

    case FUNC_JSON_APPEND:
        if ( ME007_STATE_DETECTED == me007_data_s.state_e )
        {
            me007_show( ME007_SHOW_TYPE_JS );
        }
        break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
        if ( ME007_STATE_DETECTED == me007_data_s.state_e )
        {
            me007_show( ME007_SHOW_TYPE_WS );
        }
        break;
#endif   // USE_WEBSERVER
    }
    return result_b;
}

#endif   // USE_ME007
