#if !defined(_RADIOLIB_SX1282_H)
#define _RADIOLIB_SX1282_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX128X

#include "../../Module.h"
#include "SX128x.h"
#include "SX1280.h"

/*!
  \class SX1282
  \brief Derived class for %SX1282 modules.
*/
class SX1282: public SX1280 {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1282(Module* mod);

#if !RADIOLIB_GODMODE
  private:
#endif

};

#endif

#endif
