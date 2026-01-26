#if !defined(_RADIOLIB_SX1281_H)
#define _RADIOLIB_SX1281_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX128X

#include "../../Module.h"
#include "SX128x.h"

/*!
  \class SX1281
  \brief Derived class for %SX1281 modules.
*/
class SX1281: public SX128x {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1281(Module* mod);

#if !RADIOLIB_GODMODE
  private:
#endif

};

#endif

#endif
