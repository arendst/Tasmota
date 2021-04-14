/* See LICENSE file for copyright and license details. */
#pragma once

#include "firacode.h"
#include "firacode_bold.h"
#include "firasans.h"
#include "firasans_bold.h"

typedef struct {
    const EpdFont* regular;
    const EpdFont* bold;
    const char* name;
} FontSet;

static int clear_cycles = 1;
static int clear_cycle_length = 12;

static unsigned int fallback_glyph = '?';
static FontSet fontsets[] = {
    {
        .regular = &FiraCode,
        .bold = &FiraCode_Bold,
        .name = "FiraCode"
    },
    {
        .regular = &FiraSans,
        .bold = &FiraSans_Bold,
        .name = "FiraSans"
    }
};
bool allow_clipboard_paste = false;

/*
 * appearance
 *
 * font: see http://freedesktop.org/software/fontconfig/fontconfig-user.html
 */
//static char *font = "Fira Mono:pixelsize=14:antialias=true:autohint=true";
//static int borderpx = 0;

/*
 * What program is execed by st depends of these precedence rules:
 * 1: program passed with -e
 * 2: utmp option
 * 3: SHELL environment variable
 * 4: value of shell in /etc/passwd
 * 5: value of shell in config.h
 */
//static char *shell = "/bin/sh";
char *utmp = NULL;
char *stty_args = "stty raw pass8 nl -echo -iexten -cstopb 38400";

/* identification sequence returned in DA and DECID */
char *vtiden = "\033[?6c";

/* Kerning / character bounding-box multipliers */
//static float cwscale = 1.0;
//static float chscale = 1.0;

/*
 * word delimiter string
 *
 * More advanced example: L" `'\"()[]{}"
 */
wchar_t *worddelimiters = L" ";

/* selection timeouts (in milliseconds) */
//static unsigned int doubleclicktimeout = 300;
//static unsigned int tripleclicktimeout = 600;

/* alt screens */
int allowaltscreen = 1;

/* frames per second st should at maximum draw to the screen */
//static unsigned int xfps = 120;
//static unsigned int actionfps = 30;

/*
 * blinking timeout (set to 0 to disable blinking) for the terminal blinking
 * attribute.
 */
//static unsigned int blinktimeout = 800;

/*
 * thickness of underline and bar cursors
 */
//static unsigned int cursorthickness = 2;

/*
 * bell volume. It must be a value between -100 and 100. Use 0 for disabling
 * it
 */
//static int bellvolume = 0;

/* default TERM value */
char *termname = "st-256color";

/*
 * spaces per tab
 *
 * When you are changing this value, don't forget to adapt the »it« value in
 * the st.info and appropriately install the st.info in the environment where
 * you use this st version.
 *
 *	it#$tabspaces,
 *
 * Secondly make sure your kernel is not expanding tabs. When running `stty
 * -a` »tab0« should appear. You can tell the terminal to not expand tabs by
 *  running following command:
 *
 *	stty tabs
 */
unsigned int tabspaces = 8;

const uint8_t colorscheme[256] = {
  // 3 bit colors
  0x0, // black
  0x2, // read
  0x3, // green
  0x4, // yellow
  0x5, // blue
  0x6, // magenta
  0x7, // cyan
  0xF, // white

  // 4 bit colors
  0x6, // bright black
  0x5, // bright red
  0x7, // bright green
  0x8, // bright yellow
  0x9, // bright blue
  0xA, // bright magenta
  0xB, // bright cyan
  0xF, // bright white

  // 8 bit colors
  0,  //16
  0,  //17
  0,  //18
  1,  //19
  1,  //20
  1,  //21
  3,  //22
  4,  //23
  4,  //24
  4,  //25
  4,  //26
  5,  //27
  4,  //28
  5,  //29
  5,  //30
  6,  //31
  6,  //32
  6,  //33
  6,  //34
  7,  //35
  7,  //36
  7,  //37
  7,  //38
  8,  //39
  7,  //40
  8,  //41
  8,  //42
  9,  //43
  9,  //44
  9,  //45
  9,  //46
  10,  //47
  10,  //48
  10,  //49
  10,  //50
  11,  //51
  1,  //52
  2,  //53
  2,  //54
  2,  //55
  3,  //56
  3,  //57
  5,  //58
  5,  //59
  6,  //60
  6,  //61
  6,  //62
  7,  //63
  6,  //64
  7,  //65
  7,  //66
  7,  //67
  8,  //68
  8,  //69
  8,  //70
  8,  //71
  9,  //72
  9,  //73
  9,  //74
  9,  //75
  9,  //76
  10,  //77
  10,  //78
  10,  //79
  11,  //80
  11,  //81
  11,  //82
  11,  //83
  12,  //84
  12,  //85
  12,  //86
  12,  //87
  2,  //88
  3,  //89
  3,  //90
  3,  //91
  4,  //92
  4,  //93
  6,  //94
  6,  //95
  6,  //96
  7,  //97
  7,  //98
  7,  //99
  7,  //100
  8,  //101
  8,  //102
  8,  //103
  8,  //104
  9,  //105
  8,  //106
  9,  //107
  9,  //108
  10,  //109
  10,  //110
  10,  //111
  10,  //112
  11,  //113
  11,  //114
  11,  //115
  11,  //116
  12,  //117
  11,  //118
  12,  //119
  12,  //120
  13,  //121
  13,  //122
  13,  //123
  3,  //124
  3,  //125
  4,  //126
  4,  //127
  4,  //128
  5,  //129
  6,  //130
  7,  //131
  7,  //132
  7,  //133
  8,  //134
  8,  //135
  8,  //136
  8,  //137
  9,  //138
  9,  //139
  9,  //140
  10,  //141
  9,  //142
  10,  //143
  10,  //144
  10,  //145
  11,  //146
  11,  //147
  11,  //148
  11,  //149
  12,  //150
  12,  //151
  12,  //152
  12,  //153
  12,  //154
  13,  //155
  13,  //156
  13,  //157
  14,  //158
  14,  //159
  4,  //160
  4,  //161
  4,  //162
  5,  //163
  5,  //164
  5,  //165
  7,  //166
  8,  //167
  8,  //168
  8,  //169
  9,  //170
  9,  //171
  9,  //172
  9,  //173
  9,  //174
  10,  //175
  10,  //176
  10,  //177
  10,  //178
  11,  //179
  11,  //180
  11,  //181
  11,  //182
  12,  //183
  11,  //184
  12,  //185
  12,  //186
  13,  //187
  13,  //188
  13,  //189
  13,  //190
  14,  //191
  14,  //192
  14,  //193
  14,  //194
  15,  //195
  4,  //196
  5,  //197
  5,  //198
  5,  //199
  6,  //200
  6,  //201
  8,  //202
  8,  //203
  9,  //204
  9,  //205
  9,  //206
  10,  //207
  9,  //208
  10,  //209
  10,  //210
  10,  //211
  11,  //212
  11,  //213
  11,  //214
  11,  //215
  12,  //216
  12,  //217
  12,  //218
  12,  //219
  12,  //220
  13,  //221
  13,  //222
  13,  //223
  14,  //224
  14,  //225
  14,  //226
  14,  //227
  15,  //228
  15,  //229
  15,  //230
  15,  //231
  0,  //232
  1,  //233
  1,  //234
  2,  //235
  3,  //236
  3,  //237
  4,  //238
  4,  //239
  5,  //240
  6,  //241
  6,  //242
  7,  //243
  8,  //244
  8,  //245
  9,  //246
  9,  //247
  10,  //248
  11,  //249
  11,  //250
  12,  //251
  13,  //252
  13,  //253
  14,  //254
  14,  //255
};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 0;
unsigned int defaultbg = 7;
//static unsigned int defaultcs = 256;
//static unsigned int defaultrcs = 257;

/*
 * Default shape of cursor
 * 2: Block ("█")
 * 4: Underline ("_")
 * 6: Bar ("|")
 * 7: Snowman ("☃")
 */
//static unsigned int cursorshape = 2;

/*
 * Default columns and rows numbers
 */

unsigned int cols = 120;
unsigned int rows = 38;

/*
 * Default colour and shape of the mouse cursor
 */
//static unsigned int mouseshape = XC_xterm;
//static unsigned int mousefg = 7;
//static unsigned int mousebg = 0;

/*
 * Color used to display font attributes when fontconfig selected a font which
 * doesn't match the ones requested.
 */
//static unsigned int defaultattr = 11;

/*
 * Force mouse select/shortcuts while mask is active (when MODE_MOUSE is set).
 * Note that if you want to use ShiftMask with selmasks, set this to an other
 * modifier, set to 0 to not use it.
 */
// static uint forcemousemod = ShiftMask;
//
//
// /*
//  * Internal mouse shortcuts.
//  * Beware that overloading Button1 will disable the selection.
//  */
// static MouseShortcut mshortcuts[] = {
// 	/* mask                 button   function        argument       release */
// 	{ XK_ANY_MOD,           Button2, selpaste,       {.i = 0},      1 },
// 	{ XK_ANY_MOD,           Button4, ttysend,        {.s = "\031"} },
// 	{ XK_ANY_MOD,           Button5, ttysend,        {.s = "\005"} },
// };
//
// /* Internal keyboard shortcuts. */
// #define MODKEY Mod1Mask
// #define TERMMOD (ControlMask|ShiftMask)
//
// static Shortcut shortcuts[] = {
// 	/* mask                 keysym          function        argument */
// 	{ XK_ANY_MOD,           XK_Break,       sendbreak,      {.i =  0} },
// 	{ ControlMask,          XK_Print,       toggleprinter,  {.i =  0} },
// 	{ ShiftMask,            XK_Print,       printscreen,    {.i =  0} },
// 	{ XK_ANY_MOD,           XK_Print,       printsel,       {.i =  0} },
// 	{ TERMMOD,              XK_Prior,       zoom,           {.f = +1} },
// 	{ TERMMOD,              XK_Next,        zoom,           {.f = -1} },
// 	{ TERMMOD,              XK_Home,        zoomreset,      {.f =  0} },
// 	{ TERMMOD,              XK_C,           clipcopy,       {.i =  0} },
// 	{ TERMMOD,              XK_V,           clippaste,      {.i =  0} },
// 	{ TERMMOD,              XK_Y,           selpaste,       {.i =  0} },
// 	{ ShiftMask,            XK_Insert,      selpaste,       {.i =  0} },
// 	{ TERMMOD,              XK_Num_Lock,    numlock,        {.i =  0} },
// };
//
// /*
//  * Special keys (change & recompile st.info accordingly)
//  *
//  * Mask value:
//  * * Use XK_ANY_MOD to match the key no matter modifiers state
//  * * Use XK_NO_MOD to match the key alone (no modifiers)
//  * appkey value:
//  * * 0: no value
//  * * > 0: keypad application mode enabled
//  * *   = 2: term.numlock = 1
//  * * < 0: keypad application mode disabled
//  * appcursor value:
//  * * 0: no value
//  * * > 0: cursor application mode enabled
//  * * < 0: cursor application mode disabled
//  *
//  * Be careful with the order of the definitions because st searches in
//  * this table sequentially, so any XK_ANY_MOD must be in the last
//  * position for a key.
//  */
//
// /*
//  * If you want keys other than the X11 function keys (0xFD00 - 0xFFFF)
//  * to be mapped below, add them to this array.
//  */
// static KeySym mappedkeys[] = { -1 };
//
// /*
//  * State bits to ignore when matching key or button events.  By default,
//  * numlock (Mod2Mask) and keyboard layout (XK_SWITCH_MOD) are ignored.
//  */
// static uint ignoremod = Mod2Mask|XK_SWITCH_MOD;
//
// /*
//  * This is the huge key array which defines all compatibility to the Linux
//  * world. Please decide about changes wisely.
//  */
// static Key key[] = {
// 	/* keysym           mask            string      appkey appcursor */
// 	{ XK_KP_Home,       ShiftMask,      "\033[2J",       0,   -1},
// 	{ XK_KP_Home,       ShiftMask,      "\033[1;2H",     0,   +1},
// 	{ XK_KP_Home,       XK_ANY_MOD,     "\033[H",        0,   -1},
// 	{ XK_KP_Home,       XK_ANY_MOD,     "\033[1~",       0,   +1},
// 	{ XK_KP_Up,         XK_ANY_MOD,     "\033Ox",       +1,    0},
// 	{ XK_KP_Up,         XK_ANY_MOD,     "\033[A",        0,   -1},
// 	{ XK_KP_Up,         XK_ANY_MOD,     "\033OA",        0,   +1},
// 	{ XK_KP_Down,       XK_ANY_MOD,     "\033Or",       +1,    0},
// 	{ XK_KP_Down,       XK_ANY_MOD,     "\033[B",        0,   -1},
// 	{ XK_KP_Down,       XK_ANY_MOD,     "\033OB",        0,   +1},
// 	{ XK_KP_Left,       XK_ANY_MOD,     "\033Ot",       +1,    0},
// 	{ XK_KP_Left,       XK_ANY_MOD,     "\033[D",        0,   -1},
// 	{ XK_KP_Left,       XK_ANY_MOD,     "\033OD",        0,   +1},
// 	{ XK_KP_Right,      XK_ANY_MOD,     "\033Ov",       +1,    0},
// 	{ XK_KP_Right,      XK_ANY_MOD,     "\033[C",        0,   -1},
// 	{ XK_KP_Right,      XK_ANY_MOD,     "\033OC",        0,   +1},
// 	{ XK_KP_Prior,      ShiftMask,      "\033[5;2~",     0,    0},
// 	{ XK_KP_Prior,      XK_ANY_MOD,     "\033[5~",       0,    0},
// 	{ XK_KP_Begin,      XK_ANY_MOD,     "\033[E",        0,    0},
// 	{ XK_KP_End,        ControlMask,    "\033[J",       -1,    0},
// 	{ XK_KP_End,        ControlMask,    "\033[1;5F",    +1,    0},
// 	{ XK_KP_End,        ShiftMask,      "\033[K",       -1,    0},
// 	{ XK_KP_End,        ShiftMask,      "\033[1;2F",    +1,    0},
// 	{ XK_KP_End,        XK_ANY_MOD,     "\033[4~",       0,    0},
// 	{ XK_KP_Next,       ShiftMask,      "\033[6;2~",     0,    0},
// 	{ XK_KP_Next,       XK_ANY_MOD,     "\033[6~",       0,    0},
// 	{ XK_KP_Insert,     ShiftMask,      "\033[2;2~",    +1,    0},
// 	{ XK_KP_Insert,     ShiftMask,      "\033[4l",      -1,    0},
// 	{ XK_KP_Insert,     ControlMask,    "\033[L",       -1,    0},
// 	{ XK_KP_Insert,     ControlMask,    "\033[2;5~",    +1,    0},
// 	{ XK_KP_Insert,     XK_ANY_MOD,     "\033[4h",      -1,    0},
// 	{ XK_KP_Insert,     XK_ANY_MOD,     "\033[2~",      +1,    0},
// 	{ XK_KP_Delete,     ControlMask,    "\033[M",       -1,    0},
// 	{ XK_KP_Delete,     ControlMask,    "\033[3;5~",    +1,    0},
// 	{ XK_KP_Delete,     ShiftMask,      "\033[2K",      -1,    0},
// 	{ XK_KP_Delete,     ShiftMask,      "\033[3;2~",    +1,    0},
// 	{ XK_KP_Delete,     XK_ANY_MOD,     "\033[P",       -1,    0},
// 	{ XK_KP_Delete,     XK_ANY_MOD,     "\033[3~",      +1,    0},
// 	{ XK_KP_Multiply,   XK_ANY_MOD,     "\033Oj",       +2,    0},
// 	{ XK_KP_Add,        XK_ANY_MOD,     "\033Ok",       +2,    0},
// 	{ XK_KP_Enter,      XK_ANY_MOD,     "\033OM",       +2,    0},
// 	{ XK_KP_Enter,      XK_ANY_MOD,     "\r",           -1,    0},
// 	{ XK_KP_Subtract,   XK_ANY_MOD,     "\033Om",       +2,    0},
// 	{ XK_KP_Decimal,    XK_ANY_MOD,     "\033On",       +2,    0},
// 	{ XK_KP_Divide,     XK_ANY_MOD,     "\033Oo",       +2,    0},
// 	{ XK_KP_0,          XK_ANY_MOD,     "\033Op",       +2,    0},
// 	{ XK_KP_1,          XK_ANY_MOD,     "\033Oq",       +2,    0},
// 	{ XK_KP_2,          XK_ANY_MOD,     "\033Or",       +2,    0},
// 	{ XK_KP_3,          XK_ANY_MOD,     "\033Os",       +2,    0},
// 	{ XK_KP_4,          XK_ANY_MOD,     "\033Ot",       +2,    0},
// 	{ XK_KP_5,          XK_ANY_MOD,     "\033Ou",       +2,    0},
// 	{ XK_KP_6,          XK_ANY_MOD,     "\033Ov",       +2,    0},
// 	{ XK_KP_7,          XK_ANY_MOD,     "\033Ow",       +2,    0},
// 	{ XK_KP_8,          XK_ANY_MOD,     "\033Ox",       +2,    0},
// 	{ XK_KP_9,          XK_ANY_MOD,     "\033Oy",       +2,    0},
// 	{ XK_Up,            ShiftMask,      "\033[1;2A",     0,    0},
// 	{ XK_Up,            Mod1Mask,       "\033[1;3A",     0,    0},
// 	{ XK_Up,         ShiftMask|Mod1Mask,"\033[1;4A",     0,    0},
// 	{ XK_Up,            ControlMask,    "\033[1;5A",     0,    0},
// 	{ XK_Up,      ShiftMask|ControlMask,"\033[1;6A",     0,    0},
// 	{ XK_Up,       ControlMask|Mod1Mask,"\033[1;7A",     0,    0},
// 	{ XK_Up,ShiftMask|ControlMask|Mod1Mask,"\033[1;8A",  0,    0},
// 	{ XK_Up,            XK_ANY_MOD,     "\033[A",        0,   -1},
// 	{ XK_Up,            XK_ANY_MOD,     "\033OA",        0,   +1},
// 	{ XK_Down,          ShiftMask,      "\033[1;2B",     0,    0},
// 	{ XK_Down,          Mod1Mask,       "\033[1;3B",     0,    0},
// 	{ XK_Down,       ShiftMask|Mod1Mask,"\033[1;4B",     0,    0},
// 	{ XK_Down,          ControlMask,    "\033[1;5B",     0,    0},
// 	{ XK_Down,    ShiftMask|ControlMask,"\033[1;6B",     0,    0},
// 	{ XK_Down,     ControlMask|Mod1Mask,"\033[1;7B",     0,    0},
// 	{ XK_Down,ShiftMask|ControlMask|Mod1Mask,"\033[1;8B",0,    0},
// 	{ XK_Down,          XK_ANY_MOD,     "\033[B",        0,   -1},
// 	{ XK_Down,          XK_ANY_MOD,     "\033OB",        0,   +1},
// 	{ XK_Left,          ShiftMask,      "\033[1;2D",     0,    0},
// 	{ XK_Left,          Mod1Mask,       "\033[1;3D",     0,    0},
// 	{ XK_Left,       ShiftMask|Mod1Mask,"\033[1;4D",     0,    0},
// 	{ XK_Left,          ControlMask,    "\033[1;5D",     0,    0},
// 	{ XK_Left,    ShiftMask|ControlMask,"\033[1;6D",     0,    0},
// 	{ XK_Left,     ControlMask|Mod1Mask,"\033[1;7D",     0,    0},
// 	{ XK_Left,ShiftMask|ControlMask|Mod1Mask,"\033[1;8D",0,    0},
// 	{ XK_Left,          XK_ANY_MOD,     "\033[D",        0,   -1},
// 	{ XK_Left,          XK_ANY_MOD,     "\033OD",        0,   +1},
// 	{ XK_Right,         ShiftMask,      "\033[1;2C",     0,    0},
// 	{ XK_Right,         Mod1Mask,       "\033[1;3C",     0,    0},
// 	{ XK_Right,      ShiftMask|Mod1Mask,"\033[1;4C",     0,    0},
// 	{ XK_Right,         ControlMask,    "\033[1;5C",     0,    0},
// 	{ XK_Right,   ShiftMask|ControlMask,"\033[1;6C",     0,    0},
// 	{ XK_Right,    ControlMask|Mod1Mask,"\033[1;7C",     0,    0},
// 	{ XK_Right,ShiftMask|ControlMask|Mod1Mask,"\033[1;8C",0,   0},
// 	{ XK_Right,         XK_ANY_MOD,     "\033[C",        0,   -1},
// 	{ XK_Right,         XK_ANY_MOD,     "\033OC",        0,   +1},
// 	{ XK_ISO_Left_Tab,  ShiftMask,      "\033[Z",        0,    0},
// 	{ XK_Return,        Mod1Mask,       "\033\r",        0,    0},
// 	{ XK_Return,        XK_ANY_MOD,     "\r",            0,    0},
// 	{ XK_Insert,        ShiftMask,      "\033[4l",      -1,    0},
// 	{ XK_Insert,        ShiftMask,      "\033[2;2~",    +1,    0},
// 	{ XK_Insert,        ControlMask,    "\033[L",       -1,    0},
// 	{ XK_Insert,        ControlMask,    "\033[2;5~",    +1,    0},
// 	{ XK_Insert,        XK_ANY_MOD,     "\033[4h",      -1,    0},
// 	{ XK_Insert,        XK_ANY_MOD,     "\033[2~",      +1,    0},
// 	{ XK_Delete,        ControlMask,    "\033[M",       -1,    0},
// 	{ XK_Delete,        ControlMask,    "\033[3;5~",    +1,    0},
// 	{ XK_Delete,        ShiftMask,      "\033[2K",      -1,    0},
// 	{ XK_Delete,        ShiftMask,      "\033[3;2~",    +1,    0},
// 	{ XK_Delete,        XK_ANY_MOD,     "\033[P",       -1,    0},
// 	{ XK_Delete,        XK_ANY_MOD,     "\033[3~",      +1,    0},
// 	{ XK_BackSpace,     XK_NO_MOD,      "\177",          0,    0},
// 	{ XK_BackSpace,     Mod1Mask,       "\033\177",      0,    0},
// 	{ XK_Home,          ShiftMask,      "\033[2J",       0,   -1},
// 	{ XK_Home,          ShiftMask,      "\033[1;2H",     0,   +1},
// 	{ XK_Home,          XK_ANY_MOD,     "\033[H",        0,   -1},
// 	{ XK_Home,          XK_ANY_MOD,     "\033[1~",       0,   +1},
// 	{ XK_End,           ControlMask,    "\033[J",       -1,    0},
// 	{ XK_End,           ControlMask,    "\033[1;5F",    +1,    0},
// 	{ XK_End,           ShiftMask,      "\033[K",       -1,    0},
// 	{ XK_End,           ShiftMask,      "\033[1;2F",    +1,    0},
// 	{ XK_End,           XK_ANY_MOD,     "\033[4~",       0,    0},
// 	{ XK_Prior,         ControlMask,    "\033[5;5~",     0,    0},
// 	{ XK_Prior,         ShiftMask,      "\033[5;2~",     0,    0},
// 	{ XK_Prior,         XK_ANY_MOD,     "\033[5~",       0,    0},
// 	{ XK_Next,          ControlMask,    "\033[6;5~",     0,    0},
// 	{ XK_Next,          ShiftMask,      "\033[6;2~",     0,    0},
// 	{ XK_Next,          XK_ANY_MOD,     "\033[6~",       0,    0},
// 	{ XK_F1,            XK_NO_MOD,      "\033OP" ,       0,    0},
// 	{ XK_F1, /* F13 */  ShiftMask,      "\033[1;2P",     0,    0},
// 	{ XK_F1, /* F25 */  ControlMask,    "\033[1;5P",     0,    0},
// 	{ XK_F1, /* F37 */  Mod4Mask,       "\033[1;6P",     0,    0},
// 	{ XK_F1, /* F49 */  Mod1Mask,       "\033[1;3P",     0,    0},
// 	{ XK_F1, /* F61 */  Mod3Mask,       "\033[1;4P",     0,    0},
// 	{ XK_F2,            XK_NO_MOD,      "\033OQ" ,       0,    0},
// 	{ XK_F2, /* F14 */  ShiftMask,      "\033[1;2Q",     0,    0},
// 	{ XK_F2, /* F26 */  ControlMask,    "\033[1;5Q",     0,    0},
// 	{ XK_F2, /* F38 */  Mod4Mask,       "\033[1;6Q",     0,    0},
// 	{ XK_F2, /* F50 */  Mod1Mask,       "\033[1;3Q",     0,    0},
// 	{ XK_F2, /* F62 */  Mod3Mask,       "\033[1;4Q",     0,    0},
// 	{ XK_F3,            XK_NO_MOD,      "\033OR" ,       0,    0},
// 	{ XK_F3, /* F15 */  ShiftMask,      "\033[1;2R",     0,    0},
// 	{ XK_F3, /* F27 */  ControlMask,    "\033[1;5R",     0,    0},
// 	{ XK_F3, /* F39 */  Mod4Mask,       "\033[1;6R",     0,    0},
// 	{ XK_F3, /* F51 */  Mod1Mask,       "\033[1;3R",     0,    0},
// 	{ XK_F3, /* F63 */  Mod3Mask,       "\033[1;4R",     0,    0},
// 	{ XK_F4,            XK_NO_MOD,      "\033OS" ,       0,    0},
// 	{ XK_F4, /* F16 */  ShiftMask,      "\033[1;2S",     0,    0},
// 	{ XK_F4, /* F28 */  ControlMask,    "\033[1;5S",     0,    0},
// 	{ XK_F4, /* F40 */  Mod4Mask,       "\033[1;6S",     0,    0},
// 	{ XK_F4, /* F52 */  Mod1Mask,       "\033[1;3S",     0,    0},
// 	{ XK_F5,            XK_NO_MOD,      "\033[15~",      0,    0},
// 	{ XK_F5, /* F17 */  ShiftMask,      "\033[15;2~",    0,    0},
// 	{ XK_F5, /* F29 */  ControlMask,    "\033[15;5~",    0,    0},
// 	{ XK_F5, /* F41 */  Mod4Mask,       "\033[15;6~",    0,    0},
// 	{ XK_F5, /* F53 */  Mod1Mask,       "\033[15;3~",    0,    0},
// 	{ XK_F6,            XK_NO_MOD,      "\033[17~",      0,    0},
// 	{ XK_F6, /* F18 */  ShiftMask,      "\033[17;2~",    0,    0},
// 	{ XK_F6, /* F30 */  ControlMask,    "\033[17;5~",    0,    0},
// 	{ XK_F6, /* F42 */  Mod4Mask,       "\033[17;6~",    0,    0},
// 	{ XK_F6, /* F54 */  Mod1Mask,       "\033[17;3~",    0,    0},
// 	{ XK_F7,            XK_NO_MOD,      "\033[18~",      0,    0},
// 	{ XK_F7, /* F19 */  ShiftMask,      "\033[18;2~",    0,    0},
// 	{ XK_F7, /* F31 */  ControlMask,    "\033[18;5~",    0,    0},
// 	{ XK_F7, /* F43 */  Mod4Mask,       "\033[18;6~",    0,    0},
// 	{ XK_F7, /* F55 */  Mod1Mask,       "\033[18;3~",    0,    0},
// 	{ XK_F8,            XK_NO_MOD,      "\033[19~",      0,    0},
// 	{ XK_F8, /* F20 */  ShiftMask,      "\033[19;2~",    0,    0},
// 	{ XK_F8, /* F32 */  ControlMask,    "\033[19;5~",    0,    0},
// 	{ XK_F8, /* F44 */  Mod4Mask,       "\033[19;6~",    0,    0},
// 	{ XK_F8, /* F56 */  Mod1Mask,       "\033[19;3~",    0,    0},
// 	{ XK_F9,            XK_NO_MOD,      "\033[20~",      0,    0},
// 	{ XK_F9, /* F21 */  ShiftMask,      "\033[20;2~",    0,    0},
// 	{ XK_F9, /* F33 */  ControlMask,    "\033[20;5~",    0,    0},
// 	{ XK_F9, /* F45 */  Mod4Mask,       "\033[20;6~",    0,    0},
// 	{ XK_F9, /* F57 */  Mod1Mask,       "\033[20;3~",    0,    0},
// 	{ XK_F10,           XK_NO_MOD,      "\033[21~",      0,    0},
// 	{ XK_F10, /* F22 */ ShiftMask,      "\033[21;2~",    0,    0},
// 	{ XK_F10, /* F34 */ ControlMask,    "\033[21;5~",    0,    0},
// 	{ XK_F10, /* F46 */ Mod4Mask,       "\033[21;6~",    0,    0},
// 	{ XK_F10, /* F58 */ Mod1Mask,       "\033[21;3~",    0,    0},
// 	{ XK_F11,           XK_NO_MOD,      "\033[23~",      0,    0},
// 	{ XK_F11, /* F23 */ ShiftMask,      "\033[23;2~",    0,    0},
// 	{ XK_F11, /* F35 */ ControlMask,    "\033[23;5~",    0,    0},
// 	{ XK_F11, /* F47 */ Mod4Mask,       "\033[23;6~",    0,    0},
// 	{ XK_F11, /* F59 */ Mod1Mask,       "\033[23;3~",    0,    0},
// 	{ XK_F12,           XK_NO_MOD,      "\033[24~",      0,    0},
// 	{ XK_F12, /* F24 */ ShiftMask,      "\033[24;2~",    0,    0},
// 	{ XK_F12, /* F36 */ ControlMask,    "\033[24;5~",    0,    0},
// 	{ XK_F12, /* F48 */ Mod4Mask,       "\033[24;6~",    0,    0},
// 	{ XK_F12, /* F60 */ Mod1Mask,       "\033[24;3~",    0,    0},
// 	{ XK_F13,           XK_NO_MOD,      "\033[1;2P",     0,    0},
// 	{ XK_F14,           XK_NO_MOD,      "\033[1;2Q",     0,    0},
// 	{ XK_F15,           XK_NO_MOD,      "\033[1;2R",     0,    0},
// 	{ XK_F16,           XK_NO_MOD,      "\033[1;2S",     0,    0},
// 	{ XK_F17,           XK_NO_MOD,      "\033[15;2~",    0,    0},
// 	{ XK_F18,           XK_NO_MOD,      "\033[17;2~",    0,    0},
// 	{ XK_F19,           XK_NO_MOD,      "\033[18;2~",    0,    0},
// 	{ XK_F20,           XK_NO_MOD,      "\033[19;2~",    0,    0},
// 	{ XK_F21,           XK_NO_MOD,      "\033[20;2~",    0,    0},
// 	{ XK_F22,           XK_NO_MOD,      "\033[21;2~",    0,    0},
// 	{ XK_F23,           XK_NO_MOD,      "\033[23;2~",    0,    0},
// 	{ XK_F24,           XK_NO_MOD,      "\033[24;2~",    0,    0},
// 	{ XK_F25,           XK_NO_MOD,      "\033[1;5P",     0,    0},
// 	{ XK_F26,           XK_NO_MOD,      "\033[1;5Q",     0,    0},
// 	{ XK_F27,           XK_NO_MOD,      "\033[1;5R",     0,    0},
// 	{ XK_F28,           XK_NO_MOD,      "\033[1;5S",     0,    0},
// 	{ XK_F29,           XK_NO_MOD,      "\033[15;5~",    0,    0},
// 	{ XK_F30,           XK_NO_MOD,      "\033[17;5~",    0,    0},
// 	{ XK_F31,           XK_NO_MOD,      "\033[18;5~",    0,    0},
// 	{ XK_F32,           XK_NO_MOD,      "\033[19;5~",    0,    0},
// 	{ XK_F33,           XK_NO_MOD,      "\033[20;5~",    0,    0},
// 	{ XK_F34,           XK_NO_MOD,      "\033[21;5~",    0,    0},
// 	{ XK_F35,           XK_NO_MOD,      "\033[23;5~",    0,    0},
// };
//
// /*
//  * Selection types' masks.
//  * Use the same masks as usual.
//  * Button1Mask is always unset, to make masks match between ButtonPress.
//  * ButtonRelease and MotionNotify.
//  * If no match is found, regular selection is used.
//  */
// static uint selmasks[] = {
// 	[SEL_RECTANGULAR] = Mod1Mask,
// };

/*
 * Printable characters in ASCII, used to estimate the advance width
 * of single wide characters.
 */
// static char ascii_printable[] =
// 	" !\"#$%&'()*+,-./0123456789:;<=>?"
// 	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
// 	"`abcdefghijklmnopqrstuvwxyz{|}~";
