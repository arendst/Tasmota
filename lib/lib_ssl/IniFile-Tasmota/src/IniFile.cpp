#include "IniFile.h"

#include <Arduino.h>
#include "base64.hpp"

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************

extern FS *ffsp;

IniFile::IniFile(File &file)
{
	_file = file;
}

IniFile::~IniFile()
{
	//if (_file)
	//  _file.close();
}

IniFile::error_t IniFile::getError(void)
{
	return _error;
}

void IniFile::clearError(void)
{
	_error = errorNoError;
}

bool IniFile::getValue(const char* section, const char* key, IniFileState &state)
{
	char *cp = nullptr;
	char *bufptr = buffer;
	bool done = false;
	if (!_file) {
		_error = errorFileNotOpen;
		return true;
	}

	switch (state.getValueState) {
	case IniFileState::funcUnset:
		state.getValueState = (section == NULL ? IniFileState::funcFindKey
							   : IniFileState::funcFindSection);
		state.readLinePosition = 0;
		break;

	case IniFileState::funcFindSection:
		if (findSection(section, state)) {
			if (_error != errorNoError)
				return true;
			state.getValueState = IniFileState::funcFindKey;
		}
		break;

	case IniFileState::funcFindKey:
		if (findKey(section, key, &cp, state)) {
			if (_error != errorNoError)
				return true;
			// Found key line in correct section
			cp = skipWhiteSpace(cp);
			removeTrailingWhiteSpace(cp);

			// Copy from cp to buffer, but the strings overlap so strcpy is out
			while (*cp != '\0')
				*bufptr++ = *cp++;
			*bufptr = '\0';
			return true;
		}
		break;

	default:
		// How did this happen?
		_error = errorUnknownError;
		done = true;
		break;
	}

	return done;
}

bool IniFile::getValue(const char* section, const char* key)
{
	IniFileState state;
	while (!getValue(section, key, state))
		;
	return _error == errorNoError;
}


bool IniFile::getValueStr(const char* section, const char* key, char *value, size_t vlen)
{
	if (!getValue(section, key)) return false; // error
	if (strlen(buffer) >= vlen) return false;
	strcpy(value, buffer);
	return true;
}

bool IniFile::getValueString(const char* section, const char* key, String &value)
{
	if (!getValue(section, key)) return false; // error
	value = buffer;
	return true;
}


// For true accept: true, yes, 1
// For false accept: false, no, 0
bool IniFile::getValueBool(const char* section, const char* key, bool& val)
{
	if (!getValue(section, key))
		return false; // error

	if (strcasecmp_P(buffer, PSTR("true")) == 0 ||
		strcasecmp_P(buffer, PSTR("yes")) == 0 ||
		strcasecmp_P(buffer, PSTR("1")) == 0) {
		val = true;
		return true;
	}
	if (strcasecmp_P(buffer, PSTR("false")) == 0 ||
		strcasecmp_P(buffer, PSTR("no")) == 0 ||
		strcasecmp_P(buffer, PSTR("0")) == 0) {
		val = false;
		return true;
	}
	return false; // does not match any known strings
}

bool IniFile::getValueInt(const char* section, const char* key, int32_t& val)
{
	if (!getValue(section, key)) return false; // error
	val = atoi(buffer);
	return true;
}

bool IniFile::getValueUInt16(const char* section, const char* key, uint16_t& val)
{
	int32_t val32;
	if (!getValue(section, key)) return false; // error
	val32 = atoi(buffer);
	if (val32 < 0 || val32 > 65535) return false;
	val = (uint16_t) val32;
	return true;

}

bool IniFile::getValueFloat(const char* section, const char* key, float & val)
{
	if (!getValue(section, key))
		return false; // error

	char *endptr;
	float tmp = strtod(buffer, &endptr);
	if (endptr == buffer)
		return false; // no conversion
	if (*endptr == '\0') {
		val = tmp;
		return true; // valid conversion
	}
	// buffer has trailing non-numeric characters, and since the buffer
	// already had whitespace removed discard the entire results
	return false;
}

bool IniFile::getIPAddress(const char* section, const char* key, ip_addr_t *ip)
{
	if (!getValue(section, key)) return false; // error
	IPAddress ipaddr;
	if (!ipaddr.fromString(buffer)) { return false; }
#ifdef ESP32
	ipaddr.to_ip_addr_t(ip);
#else
	*ip = ipaddr;
#endif
	return true;
}

bool IniFile::getMACAddress(const char* section, const char* key,
							uint8_t mac[6])
{
	// Need 18 chars: 6 * 2 hex digits, 5 : or - and a null char
	if (bufferLen < 18)
		return false;

	if (!getValue(section, key))
		return false; // error

	int i = 0;
	char* cp = buffer;
	memset(mac, 0, 6);

	while (*cp != '\0' && i < 6) {
		if (*cp == ':' || *cp == '-') {
			++i;
			++cp;
			continue;
		}
		if (isdigit(*cp)) {
			mac[i] *= 16; // working in hex!
			mac[i] += (*cp - '0');
		}
		else {
			if (isxdigit(*cp)) {
				mac[i] *= 16; // working in hex!
				mac[i] += (toupper(*cp) - 55); // convert A to 0xA, F to 0xF
			}
			else {
				memset(mac, 0, 6);
				return false;
			}
		}
		++cp;
	}
	return true;
}

bool IniFile::getValueBase64(const char* section, const char* key, uint8_t *value, size_t vlen)
{
	if (!getValue(section, key)) return false; // error
	size_t len = decode_base64_length((unsigned char*)buffer);
	if (len != vlen) return false;
	len = decode_base64((unsigned char*)buffer, value);
	return true;
}

bool IniFile::parseCIDR(String& cidr, ip_addr_t *ip, ip_addr_t *mask)
{
	int32_t slash = cidr.indexOf('/');
	if (slash < 0) { return false; }
	IPAddress ipaddr;
	if (!ipaddr.fromString(cidr.substring(0, slash))) { return false; }
#ifdef ESP32
	ipaddr.to_ip_addr_t(ip);
#else
	*ip = ipaddr;
#endif

	int32_t prefixLen = cidr.substring(slash + 1).toInt();
	if (prefixLen < 0 || prefixLen > 32) { return false; }
	IPAddress maskaddr((prefixLen <= 0) ? 0 : (0xFFFFFFFF >> (32 - prefixLen)));
#ifdef ESP32
	maskaddr.to_ip_addr_t(mask);
#else
	*mask = maskaddr;
#endif
	return true;
}

bool IniFile::getCIDR(const char* section, const char* key, ip_addr_t *ip, ip_addr_t *mask)
{
	String cidr;
	if (!getValueString(section, key, cidr)) return false; // error
	return parseCIDR(cidr, ip, mask);
}

bool IniFile::getDomainPort(const char* section, const char* key, String &domain, uint16_t &port, uint16_t default_port)
{
	if (!getValueString(section, key, domain)) return false; // error
	int32_t colon = domain.indexOf(':');
	if (colon == 0) { return false; }			// having an empty domain is wrong
	if (colon > 0) {
		port = domain.substring(colon + 1).toInt();
		domain = domain.substring(0, colon);
	} else {
		port = default_port;
	}
	return true;
}

// From the file location saved in 'state' look for the next section and read its name.
// The name will be in the buffer. Returns false if no section found. 
bool IniFile::browseSections(IniFileState &state)
{
	error_t err = errorNoError;
	char *bufptr = &buffer[0];
	
	do {
		err = IniFile::readLine(_file, state.readLinePosition);
		
		if (err != errorNoError) {
			// end of file or other error
			_error = err;
			return false;
		} else { 
			char *cp = skipWhiteSpace(buffer);

			if (*cp == '[') {
				// Found a section, read the name
				++cp;
				cp = skipWhiteSpace(cp);
				char *ep = strchr(cp, ']');
				if (ep != NULL) {
					*ep = '\0'; // make ] be end of string
					removeTrailingWhiteSpace(cp);
					// Copy from cp to buffer, but the strings overlap so strcpy is out
					while (*cp != '\0')
						*bufptr++ = *cp++;
					*bufptr = '\0';
					_error = errorNoError;
					return true;
				}
			}
		}
		// continue searching
	} while (err == errorNoError);
	
	// we should never get here...
	_error = err;
	return false;
}

IniFile::error_t IniFile::readLine(File &file, uint32_t &pos)
{
	if (!file)
		return errorFileNotOpen;

	if (bufferLen < 3)
		return errorBufferTooSmall;

	if (!file.seek(pos))
		return errorSeekError;

	size_t bytesRead = file.readBytes(buffer, bufferLen);
	if (!bytesRead) {
		buffer[0] = '\0';
		//return 1; // done
		return errorEndOfFile;
	}

	for (size_t i = 0; i < bytesRead && i < bufferLen-1; ++i) {
		// Test for '\n' with optional '\r' too
		// if (endOfLineTest(i, '\n', '\r')

		if (buffer[i] == '\n' || buffer[i] == '\r') {
			char match = buffer[i];
			char otherNewline = (match == '\n' ? '\r' : '\n');
			// end of line, discard any trailing character of the other sort
			// of newline
			buffer[i] = '\0';

			if (buffer[i+1] == otherNewline)
				++i;
			pos += (i + 1); // skip past newline(s)
			//return (i+1 == bytesRead && !file.available());
			return errorNoError;
		}
	}
	if (!file.available()) {
		// end of file without a newline
		buffer[bytesRead] = '\0';
		// return 1; //done
		return errorEndOfFile;
	}

	buffer[bufferLen-1] = '\0'; // terminate the string
	return errorBufferTooSmall;
}

bool IniFile::isCommentChar(char c)
{
	return (c == ';' || c == '#');
}

char* IniFile::skipWhiteSpace(char* str)
{
	char *cp = str;
	if (cp)
		while (isspace(*cp))
			++cp;
	return cp;
}

void IniFile::removeTrailingWhiteSpace(char* str)
{
	if (str == nullptr)
		return;
	char *cp = str + strlen(str) - 1;
	while (cp >= str && isspace(*cp))
		*cp-- = '\0';
}

bool IniFile::findSection(const char* section, IniFileState &state)
{
	if (section == NULL) {
		_error = errorSectionNotFound;
		return true;
	}

	error_t err = IniFile::readLine(_file, state.readLinePosition);

	if (err != errorNoError && err != errorEndOfFile) {
		// Signal to caller to stop looking and any error value
		_error = err;
		return true;
	}

	char *cp = skipWhiteSpace(buffer);
	//if (isCommentChar(*cp))
	//return (done ? errorSectionNotFound : 0);
	if (isCommentChar(*cp)) {
		// return (err == errorEndOfFile ? errorSectionNotFound : errorNoError);
		if (err == errorEndOfFile) {
			_error = errorSectionNotFound;
			return true;
		}
		else
			return false; // Continue searching
	}

	if (*cp == '[') {
		// Start of section
		++cp;
		cp = skipWhiteSpace(cp);
		char *ep = strchr(cp, ']');
		if (ep != NULL) {
			*ep = '\0'; // make ] be end of string
			removeTrailingWhiteSpace(cp);
			if (strcmp(cp, section) == 0) {
				_error = errorNoError;
				return true;
			}
		}
	}

	// Not a valid section line
	//return (done ? errorSectionNotFound : 0);
	if (err == errorEndOfFile) {
		_error = errorSectionNotFound;
		return true;
	}

	return false;
}

// From the current file location look for the matching key. If
// section is non-NULL don't look in the next section
bool IniFile::findKey(const char* section, const char* key,
					  char** keyptr,
					  IniFileState &state)
{
	if (key == NULL || *key == '\0') {
		_error = errorKeyNotFound;
		return true;
	}

	error_t err = IniFile::readLine(_file, state.readLinePosition);
	if (err != errorNoError && err != errorEndOfFile) {
		_error = err;
		return true;
	}

	char *cp = skipWhiteSpace(buffer);
	// if (isCommentChar(*cp))
	//   return (done ? errorKeyNotFound : 0);
	if (isCommentChar(*cp)) {
		if (err == errorEndOfFile) {
			_error = errorKeyNotFound;
			return true;
		}
		else
			return false; // Continue searching
	}

	if (section && *cp == '[') {
		// Start of a new section
		_error = errorKeyNotFound;
		return true;
	}

	// Find '='
	char *ep = strchr(cp, '=');
	if (ep != NULL) {
		*ep = '\0'; // make = be the end of string
		removeTrailingWhiteSpace(cp);
		if (strcmp(cp, key) == 0) {
			*keyptr = ep + 1;
			_error = errorNoError;
			return true;
		}
	}

	// Not the valid key line
	if (err == errorEndOfFile) {
		_error = errorKeyNotFound;
		return true;
	}
	return false;
}

IniFileState::IniFileState()
{
	readLinePosition = 0;
	getValueState = funcUnset;
}
