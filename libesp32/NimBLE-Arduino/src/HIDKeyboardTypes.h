/* Copyright (c) 2015 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Note: this file was pulled from different parts of the USBHID library, in mbed SDK
 */

#ifndef KEYBOARD_DEFS_H
#define KEYBOARD_DEFS_H

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_VOLUME   3

/* Modifiers */
enum MODIFIER_KEY {
	KEY_CTRL = 1,
	KEY_SHIFT = 2,
	KEY_ALT = 4,
};


enum MEDIA_KEY {
    KEY_NEXT_TRACK,     /*!< next Track Button */
    KEY_PREVIOUS_TRACK, /*!< Previous track Button */
    KEY_STOP,           /*!< Stop Button */
    KEY_PLAY_PAUSE,     /*!< Play/Pause Button */
    KEY_MUTE,           /*!< Mute Button */
    KEY_VOLUME_UP,      /*!< Volume Up Button */
    KEY_VOLUME_DOWN,    /*!< Volume Down Button */
};

enum FUNCTION_KEY {
	KEY_F1 = 128,   /* F1 key */
	KEY_F2,		 /* F2 key */
	KEY_F3,		 /* F3 key */
	KEY_F4,		 /* F4 key */
	KEY_F5,		 /* F5 key */
	KEY_F6,		 /* F6 key */
	KEY_F7,		 /* F7 key */
	KEY_F8,		 /* F8 key */
	KEY_F9,		 /* F9 key */
	KEY_F10,		/* F10 key */
	KEY_F11,		/* F11 key */
	KEY_F12,		/* F12 key */

	KEY_PRINT_SCREEN,   /* Print Screen key */
	KEY_SCROLL_LOCK,	/* Scroll lock */
	KEY_CAPS_LOCK,	  /* caps lock */
	KEY_NUM_LOCK,	   /* num lock */
	KEY_INSERT,		 /* Insert key */
	KEY_HOME,		   /* Home key */
	KEY_PAGE_UP,		/* Page Up key */
	KEY_PAGE_DOWN,	  /* Page Down key */

	RIGHT_ARROW,		/* Right arrow */
	LEFT_ARROW,		 /* Left arrow */
	DOWN_ARROW,		 /* Down arrow */
	UP_ARROW,		   /* Up arrow */
};

typedef struct {
	unsigned char usage;
	unsigned char modifier;
} KEYMAP;

#ifdef US_KEYBOARD
/* US keyboard (as HID standard) */
#define KEYMAP_SIZE (152)
const KEYMAP keymap[KEYMAP_SIZE] = {
    {0, 0},             /* NUL */
    {0, 0},             /* SOH */
    {0, 0},             /* STX */
    {0, 0},             /* ETX */
    {0, 0},             /* EOT */
    {0, 0},             /* ENQ */
    {0, 0},             /* ACK */
    {0, 0},             /* BEL */
    {0x2a, 0},          /* BS  */  /* Keyboard Delete (Backspace) */
    {0x2b, 0},          /* TAB */  /* Keyboard Tab */
    {0x28, 0},          /* LF  */  /* Keyboard Return (Enter) */
    {0, 0},             /* VT  */
    {0, 0},             /* FF  */
    {0, 0},             /* CR  */
    {0, 0},             /* SO  */
    {0, 0},             /* SI  */
    {0, 0},             /* DEL */
    {0, 0},             /* DC1 */
    {0, 0},             /* DC2 */
    {0, 0},             /* DC3 */
    {0, 0},             /* DC4 */
    {0, 0},             /* NAK */
    {0, 0},             /* SYN */
    {0, 0},             /* ETB */
    {0, 0},             /* CAN */
    {0, 0},             /* EM  */
    {0, 0},             /* SUB */
    {0, 0},             /* ESC */
    {0, 0},             /* FS  */
    {0, 0},             /* GS  */
    {0, 0},             /* RS  */
    {0, 0},             /* US  */
    {0x2c, 0},          /*   */
    {0x1e, KEY_SHIFT},      /* ! */
    {0x34, KEY_SHIFT},      /* " */
    {0x20, KEY_SHIFT},      /* # */
    {0x21, KEY_SHIFT},      /* $ */
    {0x22, KEY_SHIFT},      /* % */
    {0x24, KEY_SHIFT},      /* & */
    {0x34, 0},          /* ' */
    {0x26, KEY_SHIFT},      /* ( */
    {0x27, KEY_SHIFT},      /* ) */
    {0x25, KEY_SHIFT},      /* * */
    {0x2e, KEY_SHIFT},      /* + */
    {0x36, 0},          /* , */
    {0x2d, 0},          /* - */
    {0x37, 0},          /* . */
    {0x38, 0},          /* / */
    {0x27, 0},          /* 0 */
    {0x1e, 0},          /* 1 */
    {0x1f, 0},          /* 2 */
    {0x20, 0},          /* 3 */
    {0x21, 0},          /* 4 */
    {0x22, 0},          /* 5 */
    {0x23, 0},          /* 6 */
    {0x24, 0},          /* 7 */
    {0x25, 0},          /* 8 */
    {0x26, 0},          /* 9 */
    {0x33, KEY_SHIFT},      /* : */
    {0x33, 0},          /* ; */
    {0x36, KEY_SHIFT},      /* < */
    {0x2e, 0},          /* = */
    {0x37, KEY_SHIFT},      /* > */
    {0x38, KEY_SHIFT},      /* ? */
    {0x1f, KEY_SHIFT},      /* @ */
    {0x04, KEY_SHIFT},      /* A */
    {0x05, KEY_SHIFT},      /* B */
    {0x06, KEY_SHIFT},      /* C */
    {0x07, KEY_SHIFT},      /* D */
    {0x08, KEY_SHIFT},      /* E */
    {0x09, KEY_SHIFT},      /* F */
    {0x0a, KEY_SHIFT},      /* G */
    {0x0b, KEY_SHIFT},      /* H */
    {0x0c, KEY_SHIFT},      /* I */
    {0x0d, KEY_SHIFT},      /* J */
    {0x0e, KEY_SHIFT},      /* K */
    {0x0f, KEY_SHIFT},      /* L */
    {0x10, KEY_SHIFT},      /* M */
    {0x11, KEY_SHIFT},      /* N */
    {0x12, KEY_SHIFT},      /* O */
    {0x13, KEY_SHIFT},      /* P */
    {0x14, KEY_SHIFT},      /* Q */
    {0x15, KEY_SHIFT},      /* R */
    {0x16, KEY_SHIFT},      /* S */
    {0x17, KEY_SHIFT},      /* T */
    {0x18, KEY_SHIFT},      /* U */
    {0x19, KEY_SHIFT},      /* V */
    {0x1a, KEY_SHIFT},      /* W */
    {0x1b, KEY_SHIFT},      /* X */
    {0x1c, KEY_SHIFT},      /* Y */
    {0x1d, KEY_SHIFT},      /* Z */
    {0x2f, 0},          /* [ */
    {0x31, 0},          /* \ */
    {0x30, 0},          /* ] */
    {0x23, KEY_SHIFT},      /* ^ */
    {0x2d, KEY_SHIFT},      /* _ */
    {0x35, 0},          /* ` */
    {0x04, 0},          /* a */
    {0x05, 0},          /* b */
    {0x06, 0},          /* c */
    {0x07, 0},          /* d */
    {0x08, 0},          /* e */
    {0x09, 0},          /* f */
    {0x0a, 0},          /* g */
    {0x0b, 0},          /* h */
    {0x0c, 0},          /* i */
    {0x0d, 0},          /* j */
    {0x0e, 0},          /* k */
    {0x0f, 0},          /* l */
    {0x10, 0},          /* m */
    {0x11, 0},          /* n */
    {0x12, 0},          /* o */
    {0x13, 0},          /* p */
    {0x14, 0},          /* q */
    {0x15, 0},          /* r */
    {0x16, 0},          /* s */
    {0x17, 0},          /* t */
    {0x18, 0},          /* u */
    {0x19, 0},          /* v */
    {0x1a, 0},          /* w */
    {0x1b, 0},          /* x */
    {0x1c, 0},          /* y */
    {0x1d, 0},          /* z */
    {0x2f, KEY_SHIFT},      /* { */
    {0x31, KEY_SHIFT},      /* | */
    {0x30, KEY_SHIFT},      /* } */
    {0x35, KEY_SHIFT},      /* ~ */
    {0,0},              /* DEL */

    {0x3a, 0},          /* F1 */
    {0x3b, 0},          /* F2 */
    {0x3c, 0},          /* F3 */
    {0x3d, 0},          /* F4 */
    {0x3e, 0},          /* F5 */
    {0x3f, 0},          /* F6 */
    {0x40, 0},          /* F7 */
    {0x41, 0},          /* F8 */
    {0x42, 0},          /* F9 */
    {0x43, 0},          /* F10 */
    {0x44, 0},          /* F11 */
    {0x45, 0},          /* F12 */

    {0x46, 0},          /* PRINT_SCREEN */
    {0x47, 0},          /* SCROLL_LOCK */
    {0x39, 0},          /* CAPS_LOCK */
    {0x53, 0},          /* NUM_LOCK */
    {0x49, 0},          /* INSERT */
    {0x4a, 0},          /* HOME */
    {0x4b, 0},          /* PAGE_UP */
    {0x4e, 0},          /* PAGE_DOWN */

    {0x4f, 0},          /* RIGHT_ARROW */
    {0x50, 0},          /* LEFT_ARROW */
    {0x51, 0},          /* DOWN_ARROW */
    {0x52, 0},          /* UP_ARROW */
};

#else
/* UK keyboard */
#define KEYMAP_SIZE (152)
const KEYMAP keymap[KEYMAP_SIZE] = {
    {0, 0},             /* NUL */
    {0, 0},             /* SOH */
    {0, 0},             /* STX */
    {0, 0},             /* ETX */
    {0, 0},             /* EOT */
    {0, 0},             /* ENQ */
    {0, 0},             /* ACK */
    {0, 0},             /* BEL */
    {0x2a, 0},          /* BS  */  /* Keyboard Delete (Backspace) */
    {0x2b, 0},          /* TAB */  /* Keyboard Tab */
    {0x28, 0},          /* LF  */  /* Keyboard Return (Enter) */
    {0, 0},             /* VT  */
    {0, 0},             /* FF  */
    {0, 0},             /* CR  */
    {0, 0},             /* SO  */
    {0, 0},             /* SI  */
    {0, 0},             /* DEL */
    {0, 0},             /* DC1 */
    {0, 0},             /* DC2 */
    {0, 0},             /* DC3 */
    {0, 0},             /* DC4 */
    {0, 0},             /* NAK */
    {0, 0},             /* SYN */
    {0, 0},             /* ETB */
    {0, 0},             /* CAN */
    {0, 0},             /* EM  */
    {0, 0},             /* SUB */
    {0, 0},             /* ESC */
    {0, 0},             /* FS  */
    {0, 0},             /* GS  */
    {0, 0},             /* RS  */
    {0, 0},             /* US  */
    {0x2c, 0},          /*   */
    {0x1e, KEY_SHIFT},      /* ! */
    {0x1f, KEY_SHIFT},      /* " */
    {0x32, 0},          /* # */
    {0x21, KEY_SHIFT},      /* $ */
    {0x22, KEY_SHIFT},      /* % */
    {0x24, KEY_SHIFT},      /* & */
    {0x34, 0},          /* ' */
    {0x26, KEY_SHIFT},      /* ( */
    {0x27, KEY_SHIFT},      /* ) */
    {0x25, KEY_SHIFT},      /* * */
    {0x2e, KEY_SHIFT},      /* + */
    {0x36, 0},          /* , */
    {0x2d, 0},          /* - */
    {0x37, 0},          /* . */
    {0x38, 0},          /* / */
    {0x27, 0},          /* 0 */
    {0x1e, 0},          /* 1 */
    {0x1f, 0},          /* 2 */
    {0x20, 0},          /* 3 */
    {0x21, 0},          /* 4 */
    {0x22, 0},          /* 5 */
    {0x23, 0},          /* 6 */
    {0x24, 0},          /* 7 */
    {0x25, 0},          /* 8 */
    {0x26, 0},          /* 9 */
    {0x33, KEY_SHIFT},      /* : */
    {0x33, 0},          /* ; */
    {0x36, KEY_SHIFT},      /* < */
    {0x2e, 0},          /* = */
    {0x37, KEY_SHIFT},      /* > */
    {0x38, KEY_SHIFT},      /* ? */
    {0x34, KEY_SHIFT},      /* @ */
    {0x04, KEY_SHIFT},      /* A */
    {0x05, KEY_SHIFT},      /* B */
    {0x06, KEY_SHIFT},      /* C */
    {0x07, KEY_SHIFT},      /* D */
    {0x08, KEY_SHIFT},      /* E */
    {0x09, KEY_SHIFT},      /* F */
    {0x0a, KEY_SHIFT},      /* G */
    {0x0b, KEY_SHIFT},      /* H */
    {0x0c, KEY_SHIFT},      /* I */
    {0x0d, KEY_SHIFT},      /* J */
    {0x0e, KEY_SHIFT},      /* K */
    {0x0f, KEY_SHIFT},      /* L */
    {0x10, KEY_SHIFT},      /* M */
    {0x11, KEY_SHIFT},      /* N */
    {0x12, KEY_SHIFT},      /* O */
    {0x13, KEY_SHIFT},      /* P */
    {0x14, KEY_SHIFT},      /* Q */
    {0x15, KEY_SHIFT},      /* R */
    {0x16, KEY_SHIFT},      /* S */
    {0x17, KEY_SHIFT},      /* T */
    {0x18, KEY_SHIFT},      /* U */
    {0x19, KEY_SHIFT},      /* V */
    {0x1a, KEY_SHIFT},      /* W */
    {0x1b, KEY_SHIFT},      /* X */
    {0x1c, KEY_SHIFT},      /* Y */
    {0x1d, KEY_SHIFT},      /* Z */
    {0x2f, 0},          /* [ */
    {0x64, 0},          /* \ */
    {0x30, 0},          /* ] */
    {0x23, KEY_SHIFT},      /* ^ */
    {0x2d, KEY_SHIFT},      /* _ */
    {0x35, 0},          /* ` */
    {0x04, 0},          /* a */
    {0x05, 0},          /* b */
    {0x06, 0},          /* c */
    {0x07, 0},          /* d */
    {0x08, 0},          /* e */
    {0x09, 0},          /* f */
    {0x0a, 0},          /* g */
    {0x0b, 0},          /* h */
    {0x0c, 0},          /* i */
    {0x0d, 0},          /* j */
    {0x0e, 0},          /* k */
    {0x0f, 0},          /* l */
    {0x10, 0},          /* m */
    {0x11, 0},          /* n */
    {0x12, 0},          /* o */
    {0x13, 0},          /* p */
    {0x14, 0},          /* q */
    {0x15, 0},          /* r */
    {0x16, 0},          /* s */
    {0x17, 0},          /* t */
    {0x18, 0},          /* u */
    {0x19, 0},          /* v */
    {0x1a, 0},          /* w */
    {0x1b, 0},          /* x */
    {0x1c, 0},          /* y */
    {0x1d, 0},          /* z */
    {0x2f, KEY_SHIFT},      /* { */
    {0x64, KEY_SHIFT},      /* | */
    {0x30, KEY_SHIFT},      /* } */
    {0x32, KEY_SHIFT},      /* ~ */
    {0,0},             /* DEL */

    {0x3a, 0},          /* F1 */
    {0x3b, 0},          /* F2 */
    {0x3c, 0},          /* F3 */
    {0x3d, 0},          /* F4 */
    {0x3e, 0},          /* F5 */
    {0x3f, 0},          /* F6 */
    {0x40, 0},          /* F7 */
    {0x41, 0},          /* F8 */
    {0x42, 0},          /* F9 */
    {0x43, 0},          /* F10 */
    {0x44, 0},          /* F11 */
    {0x45, 0},          /* F12 */

    {0x46, 0},          /* PRINT_SCREEN */
    {0x47, 0},          /* SCROLL_LOCK */
    {0x39, 0},          /* CAPS_LOCK */
    {0x53, 0},          /* NUM_LOCK */
    {0x49, 0},          /* INSERT */
    {0x4a, 0},          /* HOME */
    {0x4b, 0},          /* PAGE_UP */
    {0x4e, 0},          /* PAGE_DOWN */

    {0x4f, 0},          /* RIGHT_ARROW */
    {0x50, 0},          /* LEFT_ARROW */
    {0x51, 0},          /* DOWN_ARROW */
    {0x52, 0},          /* UP_ARROW */
};
#endif

#endif
