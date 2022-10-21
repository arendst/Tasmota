//
// Compat with ESP32
//
#include <HTTPUpdate.h>
#define ESPhttpUpdate httpUpdate

inline HTTPUpdateResult ESPhttpUpdate_update(const String& url, const String& currentVersion = "")
{
	return HTTP_UPDATE_OK;
}
