#ifndef UDISPLAY_CONFIG_H
#define UDISPLAY_CONFIG_H

// Logging system interface - only declare if not building within Tasmota
#ifndef _TASMOTA_H_
enum LoggingLevels {
    LOG_LEVEL_NONE, 
    LOG_LEVEL_ERROR, 
    LOG_LEVEL_INFO, 
    LOG_LEVEL_DEBUG, 
    LOG_LEVEL_DEBUG_MORE
};

// Function declarations - only if not building within Tasmota
extern void AddLog(uint32_t loglevel, const char* formatP, ...);
extern uint32_t ESP_ResetInfoReason(void);
extern bool UsePSRAM(void);
extern float CharToFloat(const char *str);
extern SPIClass *SpiBegin(uint32_t bus);
#endif  // _TASMOTA_H_

// Enable universal touch support
#ifndef USE_UNIVERSAL_TOUCH
#define USE_UNIVERSAL_TOUCH
#endif

enum uColorType { uCOLOR_BW, uCOLOR_COLOR };

// Color definitions
constexpr uint16_t UDISP_BLACK       = 0x0000;  /*   0,   0,   0 */
constexpr uint16_t UDISP_NAVY        = 0x000F;  /*   0,   0, 128 */
constexpr uint16_t UDISP_DARKGREEN   = 0x03E0;  /*   0, 128,   0 */
constexpr uint16_t UDISP_DARKCYAN    = 0x03EF;  /*   0, 128, 128 */
constexpr uint16_t UDISP_MAROON      = 0x7800;  /* 128,   0,   0 */
constexpr uint16_t UDISP_PURPLE      = 0x780F;  /* 128,   0, 128 */
constexpr uint16_t UDISP_OLIVE       = 0x7BE0;  /* 128, 128,   0 */
constexpr uint16_t UDISP_LIGHTGREY   = 0xC618;  /* 192, 192, 192 */
constexpr uint16_t UDISP_DARKGREY    = 0x7BEF;  /* 128, 128, 128 */
constexpr uint16_t UDISP_BLUE        = 0x001F;  /*   0,   0, 255 */
constexpr uint16_t UDISP_GREEN       = 0x07E0;  /*   0, 255,   0 */
constexpr uint16_t UDISP_CYAN        = 0x07FF;  /*   0, 255, 255 */
constexpr uint16_t UDISP_RED         = 0xF800;  /* 255,   0,   0 */
constexpr uint16_t UDISP_MAGENTA     = 0xF81F;  /* 255,   0, 255 */
constexpr uint16_t UDISP_YELLOW      = 0xFFE0;  /* 255, 255,   0 */
constexpr uint16_t UDISP_WHITE       = 0xFFFF;  /* 255, 255, 255 */
constexpr uint16_t UDISP_ORANGE      = 0xFD20;  /* 255, 165,   0 */
constexpr uint16_t UDISP_GREENYELLOW = 0xAFE5;  /* 173, 255,  47 */
constexpr uint16_t UDISP_PINK        = 0xFC18;  /* 255, 128, 192 */

// epaper pseudo opcodes
constexpr uint8_t EP_RESET = 0x60;
constexpr uint8_t EP_LUT_FULL = 0x61;
constexpr uint8_t EP_LUT_PARTIAL = 0x62;
constexpr uint8_t EP_WAITIDLE = 0x63;
constexpr uint8_t EP_SET_MEM_AREA = 0x64;
constexpr uint8_t EP_SET_MEM_PTR = 0x65;
constexpr uint8_t EP_SEND_DATA = 0x66;
constexpr uint8_t EP_CLR_FRAME = 0x67;
constexpr uint8_t EP_SEND_FRAME = 0x68;
constexpr uint8_t EP_BREAK_RR_EQU = 0x69;
constexpr uint8_t EP_BREAK_RR_NEQ = 0x6a;

#endif
