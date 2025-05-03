#ifndef _INIFILE_H
#define _INIFILE_H

#include <stdint.h>

#include <FS.h>
#include "IPAddress.h"

#define INIFILE_VERSION "1.3.0"

class IniFileState;

class IniFile {
public:

	enum error_t {
		errorNoError = 0,
		errorFileNotFound,
		errorFileNotOpen,
		errorBufferTooSmall,
		errorSeekError,
		errorSectionNotFound,
		errorKeyNotFound,
		errorEndOfFile,
		errorUnknownError,
	};

	// Create an IniFile object. It isn't opened until open() is called on it.
	IniFile(File &file);
	~IniFile();

	error_t getError(void);
	void clearError(void);

	// Get value from the file, but split into many short tasks. Return
	// value: false means continue, true means stop. Call getError() to
	// find out if any error
	bool getValue(const char* section, const char* key, IniFileState &state);

	// Get value, as one big task. Return = true means value is present
	// in buffer
	bool getValue(const char* section, const char* key);

	// Get the value as a string, storing the result in a new buffer
	// (not the working buffer)
	bool getValueStr(const char* section, const char* key, char *value, size_t vlen);
	bool getValueString(const char* section, const char* key, String &value);


	// Get a boolean value
	bool getValueBool(const char* section, const char* key, bool& b);

	// Get an integer value
	bool getValueInt(const char* section, const char* key, int32_t& val);

	// Get an uint16_t value
	bool getValueUInt16(const char* section, const char* key, uint16_t& val);

	// Get a float value
	bool getValueFloat(const char* section, const char* key, float& val);

	bool getIPAddress(const char* section, const char* key, ip_addr_t *ip);

	bool getMACAddress(const char* section, const char* key,uint8_t mac[6]);
					   
	bool getValueBase64(const char* section, const char* key, uint8_t *value, size_t vlen);

	static bool parseCIDR(String& str, ip_addr_t *ip, ip_addr_t *mask);
	bool getCIDR(const char* section, const char* key, ip_addr_t *ip, ip_addr_t *mask);

	bool getDomainPort(const char* section, const char* key, String &domain, uint16_t &port, uint16_t default_port);

	// From the file location saved in 'state' look for the next section and read its name.
	// The name will be in the buffer. Returns false if no section found. 
	bool browseSections(IniFileState &state);

	// Utility function to read a line from a file, make available to all
	//static int8_t readLine(File &file, char *buffer, size_t len, uint32_t &pos);
	error_t readLine(File &file, uint32_t &pos);
	static bool isCommentChar(char c);
	static char* skipWhiteSpace(char* str);
	static void removeTrailingWhiteSpace(char* str);

protected:
	// True means stop looking, false means not yet found
	bool findSection(const char* section, IniFileState &state);
	bool findKey(const char* section, const char* key, char** keyptr, IniFileState &state);


private:
	String _filename;
	error_t _error;
	File _file;
	static constexpr size_t bufferLen = 80;
	char buffer[bufferLen];
};


class IniFileState {
public:
	IniFileState();

private:
	enum {funcUnset = 0,
		  funcFindSection,
		  funcFindKey,
	};

	uint32_t readLinePosition;
	uint8_t getValueState;

	friend class IniFile;
};


#endif

