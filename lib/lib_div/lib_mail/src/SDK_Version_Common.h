#ifndef _SDK_VERSION_COMMON_H
#define _SDK_VERSION_COMMON_H

#if defined(ESP8266)

#include <string>

//__GNUC__
//__GNUC_MINOR__
//__GNUC_PATCHLEVEL__

#ifdef __GNUC__
#if __GNUC__ > 4 || __GNUC__ == 10
#define ESP8266_CORE_SDK_V3_X_X
#endif
#endif

#endif

#endif