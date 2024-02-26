#if !defined(_RADIOLIB_SX1261_H)
#define _RADIOLIB_SX1261_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX126X

#include "../../Module.h"
#include "SX126x.h"
#include "SX1262.h"

//RADIOLIB_SX126X_CMD_SET_PA_CONFIG
#define RADIOLIB_SX126X_PA_CONFIG_SX1261                        0x01

//RADIOLIB_SX126X_REG_VERSION_STRING
#define RADIOLIB_SX1261_CHIP_TYPE                               "SX1261"

/*!
  \class SX1261
  \brief Derived class for %SX1261 modules.
*/
class SX1261 : public SX1262 {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1261(Module* mod);

    /*!
      \brief Sets output power. Allowed values are in range from -17 to 14 dBm.
      \param power Output power to be set in dBm.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t power);

#if !RADIOLIB_GODMODE
  private:
#endif

};

#endif

#endif
