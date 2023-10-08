
#ifndef _HAN_PARSER_H
#define _HAN_PARSER_H
#if defined __cplusplus
#include "Arduino.h"
#include "DataParsers.h"
#include "DataParser.h"
#include "Cosem.h"
#include "ntohll.h"

#define BUF_SIZE_HAN (1280)

int16_t serial_available(void);
uint8_t serial_read(void);

class Han_Parser
{
public:
    Han_Parser(uint16_t (*)(uint8_t, uint8_t), uint8_t, uint8_t *, uint8_t *);
    ~Han_Parser(void);
    bool readHanPort(uint8_t **out, uint16_t *size);
		int16_t unwrapData(uint8_t *buf, DataParserContext &context);
		void printHanReadError(int16_t pos);
		uint8_t encryptionKey[16];
		uint8_t authenticationKey[16];
    uint8_t hanBuffer[BUF_SIZE_HAN];
    int len = 0;
private:
    uint16_t (*dispatch)(uint8_t, uint8_t);
    int serial_available(void);
    int serial_read(void);
    int16_t serial_readBytes(uint8_t *, uint16_t);
		HDLCParser *hdlcParser = NULL;
		MBUSParser *mbusParser = NULL;
		GBTParser *gbtParser = NULL;
		GCMParser *gcmParser = NULL;
		LLCParser *llcParser = NULL;
		DLMSParser *dlmsParser = NULL;
		DSMRParser *dsmrParser = NULL;
    uint8_t encryption_key[16];
    uint8_t authentication_key[16];
    uint8_t meter;
		bool serialInit = true;
		bool Debug = true;
};
#endif
#endif
