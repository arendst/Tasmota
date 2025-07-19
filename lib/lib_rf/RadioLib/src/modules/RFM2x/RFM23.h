#if !defined(_RADIOLIB_RFM23_H)
#define _RADIOLIB_RFM23_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_RFM2X

#include "../../Module.h"
#include "../Si443x/Si443x.h"
#include "../Si443x/Si4431.h"

/*!
  \class RFM23
  \brief Only exists as alias for Si4431, since there seems to be no difference between %RFM23 and %Si4431 modules.
*/
RADIOLIB_TYPE_ALIAS(Si4431, RFM23)

#endif

#endif
