#if !defined(_RADIOLIB_RFM22_H)
#define _RADIOLIB_RFM22_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_RFM2X

#include "../../Module.h"
#include "../Si443x/Si443x.h"
#include "../Si443x/Si4432.h"

/*!
  \class RFM22
  \brief Only exists as alias for Si4432, since there seems to be no difference between %RFM22 and %Si4432 modules.
*/
RADIOLIB_TYPE_ALIAS(Si4432, RFM22)

#endif

#endif
