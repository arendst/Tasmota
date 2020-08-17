/**
 *
 * @file HPMA115S0.h
 * @author Felix A. Galindo
 * @date June 2017
 * @brief Arduino Library for Honeywell's Particle Sensor (HPMA115S0-XXX)
 *
 */

#ifndef HPMA115S0_H
#define HPMA115S0_H

#include "Arduino.h"

#define HPM_CMD_RESP_HEAD 0x40
#define HPM_MAX_RESP_SIZE 16 // max command response size is 16 bytes
#define HPM_READ_PARTICLE_MEASURMENT_LEN 5
#define HPM_READ_PARTICLE_MEASURMENT_LEN_C 13

enum CMD_TYPE_T {
    READ_PARTICLE_MEASURMENT = 0x04,
    START_PARTICLE_MEASURMENT = 0x01,
    STOP_PARTICLE_MEASURMENT = 0x02,
    SET_ADJUSTMENT_COEFFICIENT = 0x08,
    READ_ADJUSTMENT_COEFFICIENT = 0x08,
    STOP_AUTO_SEND = 0x20,
    ENABLE_AUTO_SEND = 0x40,
};

enum HPM_PACKET_T {
    HPM_HEAD_IDX,
    HPM_LEN_IDX,
    HPM_CMD_IDX,
    HPM_DATA_START_IDX
};

class HPMA115S0
{
public:
    /**
     * @brief Constructor for HPMA115S0 class
     * @param  a Stream ({Software/Hardware}Serial) object.
     * @note The serial stream should be already initialized
     * @return  void
     */
    HPMA115S0(Stream& serial);

    /**
     * @brief Function that initializes sensor
     * @return  a String containing sensor response
     */
    void Init();

    /**
     * @brief Function that sends a read command to sensor
     * @return  returns true if valid measurements were read from sensor
     */
    boolean ReadParticleMeasurement(unsigned int * pm2_5, unsigned int * pm10);

    /**
     * @brief Function that starts sensor measurement
     * @return  void
     */
    void StartParticleMeasurement();
    /**
     * @brief Function that stops sensor measurement
     * @return  void
     */
    void StopParticleMeasurement();

    /**
     * @brief Function that enables auto send
     * @return  void
     */
    void EnableAutoSend();

    /**
     * @brief Function that stops auto send
     * @return  void
     */
    void DisableAutoSend();

    /**
    * @brief Function that returns the latest PM 2.5 reading
    * @note Sensor reports new reading ~ every 1 sec.
    * @return  PM 2.5 reading (unsigned int)
    */
    unsigned int GetPM2_5();

    /**
    * @brief Function that returns the latest PM 10 reading
    * @note Sensor reports new reading ~ every 1 sec.
    * @return  PM 10 reading (unsigned int)
    */
    unsigned int GetPM10();

private:
    Stream& _serial;

    //Latest PM 2.5 reading
    unsigned int _pm2_5 = 0;

    //Latest PM 10 reading
    unsigned int _pm10 = 0;

    /**
     * @brief Function that sends serial command to sensor
     * @param  a char * containing the command
     * @param size of buffer
     * @return  void
     */
    void SendCmd(const char * command, unsigned int size);

    /**
    * @brief Function that reads command response from sensor
    * @param Buffer to store data in
    * @param Buffer size
    * @param Expected command type
    * @return  returns number of bytes read from sensor
    */
    int ReadCmdResp(unsigned char * dataBuf, unsigned int dataBufSize, unsigned int cmdType);
};

#endif
