#include "uDisplay.h"
#include "uDisplay_config.h"

// Color palette definition
static constexpr uint16_t udisp_colors[] = {
    UDISP_BLACK, UDISP_WHITE, UDISP_RED, UDISP_GREEN, UDISP_BLUE, UDISP_CYAN, 
    UDISP_MAGENTA, UDISP_YELLOW, UDISP_NAVY, UDISP_DARKGREEN, UDISP_DARKCYAN, 
    UDISP_MAROON, UDISP_PURPLE, UDISP_OLIVE, UDISP_LIGHTGREY, UDISP_DARKGREY, 
    UDISP_ORANGE, UDISP_GREENYELLOW, UDISP_PINK
};

uint16_t uDisplay::GetColorFromIndex(uint8_t index) {
    const size_t color_count = sizeof(udisp_colors) / sizeof(udisp_colors[0]);
    
    if (index >= color_count) {
        index = 0;
    }
    return udisp_colors[index];
}

uint16_t uDisplay::fgcol(void) {
    return fg_col;
}

uint16_t uDisplay::bgcol(void) {
    return bg_col;
}

int8_t uDisplay::color_type(void) {
    return col_type;
}