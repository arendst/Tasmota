#if !defined(_RADIOLIB_SX1280_H)
#define _RADIOLIB_SX1280_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX128X

#include "../../Module.h"
#include "SX128x.h"
#include "SX1281.h"

/*!
  \class SX1280
  \brief Derived class for %SX1280 modules.
*/
class SX1280: public SX1281 {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1280(Module* mod);

    /*!
      \brief Blocking ranging method.
      \param master Whether to execute ranging in master mode (true) or slave mode (false).
      \param addr Ranging address to be used.
      \param calTable Ranging calibration table - set to NULL to use the default.
      \returns \ref status_codes
    */
    int16_t range(bool master, uint32_t addr, uint16_t calTable[3][6] = NULL);

    /*!
      \brief Interrupt-driven ranging method.
      \param master Whether to execute ranging in master mode (true) or slave mode (false).
      \param addr Ranging address to be used.
      \param calTable Ranging calibration table - set to NULL to use the default.
      \returns \ref status_codes
    */
    int16_t startRanging(bool master, uint32_t addr, uint16_t calTable[3][6] = NULL);

    /*!
      \brief Gets ranging result of the last ranging exchange.
      \returns Ranging result in meters.
    */
    float getRangingResult();

#if !RADIOLIB_GODMODE
  private:
#endif

};

#endif

#endif
