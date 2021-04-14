/* See LICENSE for license details. */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

#include "driver/uart.h"
#include "epd_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "st.h"
#include "config.h"

//#include "win.h"

#if   defined(__linux)
 #include <pty.h>
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
 #include <util.h>
#elif defined(__FreeBSD__) || defined(__DragonFly__)
 #include <libutil.h>
#endif

/* Arbitrary sizes */
#define UTF_INVALID   0xFFFD
#define UTF_SIZ       4
#define ESC_BUF_SIZ   (128*UTF_SIZ)
#define ESC_ARG_SIZ   16
#define STR_BUF_SIZ   ESC_BUF_SIZ
#define STR_ARG_SIZ   ESC_ARG_SIZ

/* macros */
#define IS_SET(flag)		((term.mode & (flag)) != 0)
#define ISCONTROLC0(c)		(BETWEEN(c, 0, 0x1f) || (c) == '\177')
#define ISCONTROLC1(c)		(BETWEEN(c, 0x80, 0x9f))
#define ISCONTROL(c)		(ISCONTROLC0(c) || ISCONTROLC1(c))
#define ISDELIM(u)		(u && wcschr(worddelimiters, u))

enum term_mode {
	MODE_WRAP        = 1 << 0,
	MODE_INSERT      = 1 << 1,
	MODE_ALTSCREEN   = 1 << 2,
	MODE_CRLF        = 1 << 3,
	MODE_ECHO        = 1 << 4,
	MODE_PRINT       = 1 << 5,
	MODE_UTF8        = 1 << 6,
	MODE_SIXEL       = 1 << 7,
};

enum cursor_movement {
	CURSOR_SAVE,
	CURSOR_LOAD
};

enum cursor_state {
	CURSOR_DEFAULT  = 0,
	CURSOR_WRAPNEXT = 1,
	CURSOR_ORIGIN   = 2,
	CURSOR_HIDE     = 4
};

enum charset {
	CS_GRAPHIC0,
	CS_GRAPHIC1,
	CS_UK,
	CS_USA,
	CS_MULTI,
	CS_GER,
	CS_FIN
};

enum escape_state {
	ESC_START      = 1,
	ESC_CSI        = 2,
	ESC_STR        = 4,  /* OSC, PM, APC */
	ESC_ALTCHARSET = 8,
	ESC_STR_END    = 16, /* a final string was encountered */
	ESC_TEST       = 32, /* Enter in test mode */
	ESC_UTF8       = 64,
	ESC_DCS        =128,
};

typedef struct {
	Glyph attr; /* current char attributes */
	int x;
	int y;
	char state;
} TCursor;

typedef struct {
	int mode;
	int type;
	int snap;
	/*
	 * Selection variables:
	 * nb – normalized coordinates of the beginning of the selection
	 * ne – normalized coordinates of the end of the selection
	 * ob – original coordinates of the beginning of the selection
	 * oe – original coordinates of the end of the selection
	 */
	struct {
		int x, y;
	} nb, ne, ob, oe;

	int alt;
} Selection;

/* Internal representation of the screen */
typedef struct {
	int row;      /* nb row */
	int col;      /* nb col */
	Line *line;   /* screen */
	Line *alt;    /* alternate screen */
	int64_t *dirty;   /* dirtyness of lines */
	TCursor c;    /* cursor */
	int ocx;      /* old cursor col */
	int ocy;      /* old cursor row */
	int top;      /* top    scroll limit */
	int bot;      /* bottom scroll limit */
	int mode;     /* terminal mode flags */
	int esc;      /* escape state flags */
	char trantbl[4]; /* charset table translation */
	int charset;  /* current charset */
	int icharset; /* selected charset for sequence */
	int *tabs;
} Term;

/* CSI Escape sequence structs */
/* ESC '[' [[ [<priv>] <arg> [;]] <mode> [<mode>]] */
typedef struct {
	char buf[ESC_BUF_SIZ]; /* raw string */
	size_t len;            /* raw string length */
	char priv;
	int arg[ESC_ARG_SIZ];
	int narg;              /* nb of args */
	char mode[2];
} CSIEscape;

/* STR Escape sequence structs */
/* ESC type [[ [<priv>] <arg> [;]] <mode>] ESC '\' */
typedef struct {
	char type;             /* ESC type ... */
	char *buf;             /* allocated raw string */
	size_t siz;            /* allocation size */
	size_t len;            /* raw string length */
	char *args[STR_ARG_SIZ];
	int narg;              /* nb of args */
} STREscape;

//static void execsh(char *, char **);
//static void stty(char **);
//static void sigchld(int);
static void ttywriteraw(const char *, size_t);

static void csidump(void);
static void csihandle(void);
static void csiparse(void);
static void csireset(void);
static int eschandle(uchar);
static void strdump(void);
static void strhandle(void);
static void strparse(void);
static void strreset(void);

static void tprinter(char *, size_t);
static void tdumpsel(void);
static void tdumpline(int);
static void tdump(void);
static void tclearregion(int, int, int, int);
static void tcursor(int);
static void tcurhide(void);
static void tcurshow(void);
static void tdeletechar(int);
static void tdeleteline(int);
static void tinsertblank(int);
static void tinsertblankline(int);
static int tlinelen(int);
static void tmoveto(int, int);
static void tmoveato(int, int);
static void tnewline(int);
static void tputtab(int);
static void tputc(Rune);
static void treset(void);
static void tscrollup(int, int);
static void tscrolldown(int, int);
static void tsetattr(int *, int);
static void tsetchar(Rune, Glyph *, int, int);
static void tsetdirt(int, int);
static void tsetscroll(int, int);
static void tswapscreen(void);
static void tsetmode(int, int, int *, int);
static int twrite(const char *, int, int);
static void tfulldirt(void);
static void tcontrolcode(uchar );
static void tdectest(char );
static void tdefutf8(char);
static int32_t tdefcolor(int *, int *, int);
static void tdeftran(char);
static void tstrsequence(uchar);

// static void drawregion(int, int, int, int);

static void selnormalize(void);
static void selscroll(int, int);
static void selsnap(int *, int *, int);

static size_t utf8decode(const char *, Rune *, size_t);
static Rune utf8decodebyte(char, size_t *);
static char utf8encodebyte(Rune, size_t);
static size_t utf8validate(Rune *, size_t);

static char *base64dec(const char *);
static char base64dec_getc(const char **);

//static ssize_t xwrite(int, const char *, size_t);
static const EpdFont* get_font(Glyph g);
static void full_refresh();
static void next_fontset(void);

static void render_line();

/* Globals */
/// Framebuffer holding the currently drawn (updated image)
static uint8_t* render_fb_back = NULL;
/// Framebuffer holding the image state before the update
static uint8_t* render_fb_front = NULL;
/// A compact mask to store which pixels have changed
static uint8_t* render_mask = NULL;
/// The mask as framebuffer to draw
static uint8_t* render_masked_buf = NULL;

/// Queue of the lines to be rendered
static QueueHandle_t line_render_queue = NULL;

/// Semaphore to signal how many lines have been processed
static SemaphoreHandle_t render_lines_done_smphr = NULL;

static uint32_t line_dirtyness[EPD_HEIGHT] = {0};

static int screen_tainted = 0;
static char* clipboard = NULL;
extern FontSet fontsets[];
static int current_fontset = 0;

static Term term;
static Selection sel;
static CSIEscape csiescseq;
static STREscape strescseq;
//static int iofd = 1;
//static int cmdfd;
//static pid_t pid;

static uchar utfbyte[UTF_SIZ + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0};
static uchar utfmask[UTF_SIZ + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
static Rune utfmin[UTF_SIZ + 1] = {       0,    0,  0x80,  0x800,  0x10000};
static Rune utfmax[UTF_SIZ + 1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

// ssize_t
// xwrite(int fd, const char *s, size_t len)
// {
// 	size_t aux = len;
// 	ssize_t r;
//
// 	while (len > 0) {
// 		r = write(fd, s, len);
// 		if (r < 0)
// 			return r;
// 		len -= r;
// 		s += r;
// 	}
//
// 	return aux;
// }

void *
xmalloc(size_t len)
{
	void *p;

	if (!(p = malloc(len)))
		die("malloc: %s\n", strerror(errno));

	return p;
}

void *
xmalloc_32(size_t len)
{
	void *p;

	if (!(p = heap_caps_malloc(len, MALLOC_CAP_32BIT | MALLOC_CAP_INTERNAL)))
		die("malloc: %s\n", strerror(errno));

	return p;
}

void *
xrealloc(void *p, size_t len)
{
	if ((p = realloc(p, len)) == NULL)
		die("realloc: %s\n", strerror(errno));

	return p;
}

void *
xrealloc_32(void *p, size_t len)
{
	if ((p = heap_caps_realloc(p, len, MALLOC_CAP_32BIT | MALLOC_CAP_INTERNAL)) == NULL)
		die("realloc: %s\n", strerror(errno));

	return p;
}

char *
xstrdup(char *s)
{
	if ((s = strdup(s)) == NULL)
		die("strdup: %s\n", strerror(errno));

	return s;
}

size_t
utf8decode(const char *c, Rune *u, size_t clen)
{
	size_t i, j, len, type;
	Rune udecoded;

	*u = UTF_INVALID;
	if (!clen)
		return 0;
	udecoded = utf8decodebyte(c[0], &len);
	if (!BETWEEN(len, 1, UTF_SIZ))
		return 1;
	for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
		udecoded = (udecoded << 6) | utf8decodebyte(c[i], &type);
		if (type != 0)
			return j;
	}
	if (j < len)
		return 0;
	*u = udecoded;
	utf8validate(u, len);

	return len;
}

Rune
utf8decodebyte(char c, size_t *i)
{
	for (*i = 0; *i < LEN(utfmask); ++(*i))
		if (((uchar)c & utfmask[*i]) == utfbyte[*i])
			return (uchar)c & ~utfmask[*i];

	return 0;
}

size_t
utf8encode(Rune u, char *c)
{
	size_t len, i;

	len = utf8validate(&u, 0);
	if (len > UTF_SIZ)
		return 0;

	for (i = len - 1; i != 0; --i) {
		c[i] = utf8encodebyte(u, 0);
		u >>= 6;
	}
	c[0] = utf8encodebyte(u, len);

	return len;
}

char
utf8encodebyte(Rune u, size_t i)
{
	return utfbyte[i] | (u & ~utfmask[i]);
}

size_t
utf8validate(Rune *u, size_t i)
{
	if (!BETWEEN(*u, utfmin[i], utfmax[i]) || BETWEEN(*u, 0xD800, 0xDFFF))
		*u = UTF_INVALID;
	for (i = 1; *u > utfmax[i]; ++i)
		;

	return i;
}

static const char base64_digits[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0,
	63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, -1, 0, 0, 0, 0, 1,
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char
base64dec_getc(const char **src)
{
	while (**src && !isprint(**src))
		(*src)++;
	return **src ? *((*src)++) : '=';  /* emulate padding if string ends */
}

char *
base64dec(const char *src)
{
	size_t in_len = strlen(src);
	char *result, *dst;

	if (in_len % 4)
		in_len += 4 - (in_len % 4);
	result = dst = xmalloc(in_len / 4 * 3 + 1);
	while (*src) {
		int a = base64_digits[(unsigned char) base64dec_getc(&src)];
		int b = base64_digits[(unsigned char) base64dec_getc(&src)];
		int c = base64_digits[(unsigned char) base64dec_getc(&src)];
		int d = base64_digits[(unsigned char) base64dec_getc(&src)];

		/* invalid input. 'a' can be -1, e.g. if src is "\n" (c-str) */
		if (a == -1 || b == -1)
			break;

		*dst++ = (a << 2) | ((b & 0x30) >> 4);
		if (c == -1)
			break;
		*dst++ = ((b & 0x0f) << 4) | ((c & 0x3c) >> 2);
		if (d == -1)
			break;
		*dst++ = ((c & 0x03) << 6) | d;
	}
	*dst = '\0';
	return result;
}

void
selinit(void)
{
	sel.mode = SEL_IDLE;
	sel.snap = 0;
	sel.ob.x = -1;
}

int
tlinelen(int y)
{
	int i = term.col;

	if (term.line[y][i - 1].mode & ATTR_WRAP)
		return i;

	while (i > 0 && term.line[y][i - 1].u == ' ')
		--i;

	return i;
}

void
selstart(int col, int row, int snap)
{
	selclear();
	sel.mode = SEL_EMPTY;
	sel.type = SEL_REGULAR;
	sel.alt = IS_SET(MODE_ALTSCREEN);
	sel.snap = snap;
	sel.oe.x = sel.ob.x = col;
	sel.oe.y = sel.ob.y = row;
	selnormalize();

	if (sel.snap != 0)
		sel.mode = SEL_READY;
	tsetdirt(sel.nb.y, sel.ne.y);
}

void
selextend(int col, int row, int type, int done)
{
	int oldey, oldex, oldsby, oldsey, oldtype;

	if (sel.mode == SEL_IDLE)
		return;
	if (done && sel.mode == SEL_EMPTY) {
		selclear();
		return;
	}

	oldey = sel.oe.y;
	oldex = sel.oe.x;
	oldsby = sel.nb.y;
	oldsey = sel.ne.y;
	oldtype = sel.type;

	sel.oe.x = col;
	sel.oe.y = row;
	selnormalize();
	sel.type = type;

	if (oldey != sel.oe.y || oldex != sel.oe.x || oldtype != sel.type || sel.mode == SEL_EMPTY)
		tsetdirt(MIN(sel.nb.y, oldsby), MAX(sel.ne.y, oldsey));

	sel.mode = done ? SEL_IDLE : SEL_READY;
}

void
selnormalize(void)
{
	int i;

	if (sel.type == SEL_REGULAR && sel.ob.y != sel.oe.y) {
		sel.nb.x = sel.ob.y < sel.oe.y ? sel.ob.x : sel.oe.x;
		sel.ne.x = sel.ob.y < sel.oe.y ? sel.oe.x : sel.ob.x;
	} else {
		sel.nb.x = MIN(sel.ob.x, sel.oe.x);
		sel.ne.x = MAX(sel.ob.x, sel.oe.x);
	}
	sel.nb.y = MIN(sel.ob.y, sel.oe.y);
	sel.ne.y = MAX(sel.ob.y, sel.oe.y);

	selsnap(&sel.nb.x, &sel.nb.y, -1);
	selsnap(&sel.ne.x, &sel.ne.y, +1);

	/* expand selection over line breaks */
	if (sel.type == SEL_RECTANGULAR)
		return;
	i = tlinelen(sel.nb.y);
	if (i < sel.nb.x)
		sel.nb.x = i;
	if (tlinelen(sel.ne.y) <= sel.ne.x)
		sel.ne.x = term.col - 1;
}

int
selected(int x, int y)
{
	if (sel.mode == SEL_EMPTY || sel.ob.x == -1 ||
			sel.alt != IS_SET(MODE_ALTSCREEN))
		return 0;

	if (sel.type == SEL_RECTANGULAR)
		return BETWEEN(y, sel.nb.y, sel.ne.y)
		    && BETWEEN(x, sel.nb.x, sel.ne.x);

	return BETWEEN(y, sel.nb.y, sel.ne.y)
	    && (y != sel.nb.y || x >= sel.nb.x)
	    && (y != sel.ne.y || x <= sel.ne.x);
}

void
selsnap(int *x, int *y, int direction)
{
	int newx, newy, xt, yt;
	int delim, prevdelim;
	Glyph *gp, *prevgp;

	switch (sel.snap) {
	case SNAP_WORD:
		/*
		 * Snap around if the word wraps around at the end or
		 * beginning of a line.
		 */
		prevgp = &term.line[*y][*x];
		prevdelim = ISDELIM(prevgp->u);
		for (;;) {
			newx = *x + direction;
			newy = *y;
			if (!BETWEEN(newx, 0, term.col - 1)) {
				newy += direction;
				newx = (newx + term.col) % term.col;
				if (!BETWEEN(newy, 0, term.row - 1))
					break;

				if (direction > 0)
					yt = *y, xt = *x;
				else
					yt = newy, xt = newx;
				if (!(term.line[yt][xt].mode & ATTR_WRAP))
					break;
			}

			if (newx >= tlinelen(newy))
				break;

			gp = &term.line[newy][newx];
			delim = ISDELIM(gp->u);
			if (!(gp->mode & ATTR_WDUMMY) && (delim != prevdelim
					|| (delim && gp->u != prevgp->u)))
				break;

			*x = newx;
			*y = newy;
			prevgp = gp;
			prevdelim = delim;
		}
		break;
	case SNAP_LINE:
		/*
		 * Snap around if the the previous line or the current one
		 * has set ATTR_WRAP at its end. Then the whole next or
		 * previous line will be selected.
		 */
		*x = (direction < 0) ? 0 : term.col - 1;
		if (direction < 0) {
			for (; *y > 0; *y += direction) {
				if (!(term.line[*y-1][term.col-1].mode
						& ATTR_WRAP)) {
					break;
				}
			}
		} else if (direction > 0) {
			for (; *y < term.row-1; *y += direction) {
				if (!(term.line[*y][term.col-1].mode
						& ATTR_WRAP)) {
					break;
				}
			}
		}
		break;
	}
}

char *
getsel(void)
{
	char *str, *ptr;
	int y, bufsize, lastx, linelen;
	Glyph *gp, *last;

	if (sel.ob.x == -1)
		return NULL;

	bufsize = (term.col+1) * (sel.ne.y-sel.nb.y+1) * UTF_SIZ;
	ptr = str = xmalloc(bufsize);

	/* append every set & selected glyph to the selection */
	for (y = sel.nb.y; y <= sel.ne.y; y++) {
		if ((linelen = tlinelen(y)) == 0) {
			*ptr++ = '\n';
			continue;
		}

		if (sel.type == SEL_RECTANGULAR) {
			gp = &term.line[y][sel.nb.x];
			lastx = sel.ne.x;
		} else {
			gp = &term.line[y][sel.nb.y == y ? sel.nb.x : 0];
			lastx = (sel.ne.y == y) ? sel.ne.x : term.col-1;
		}
		last = &term.line[y][MIN(lastx, linelen-1)];
		while (last >= gp && last->u == ' ')
			--last;

		for ( ; gp <= last; ++gp) {
			if (gp->mode & ATTR_WDUMMY)
				continue;

			ptr += utf8encode(gp->u, ptr);
		}

		/*
		 * Copy and pasting of line endings is inconsistent
		 * in the inconsistent terminal and GUI world.
		 * The best solution seems like to produce '\n' when
		 * something is copied from st and convert '\n' to
		 * '\r', when something to be pasted is received by
		 * st.
		 * FIXME: Fix the computer world.
		 */
		if ((y < sel.ne.y || lastx >= linelen) && !(last->mode & ATTR_WRAP))
			*ptr++ = '\n';
	}
	*ptr = 0;
	return str;
}

void
selclear(void)
{
	if (sel.ob.x == -1)
		return;
	sel.mode = SEL_IDLE;
	sel.ob.x = -1;
	tsetdirt(sel.nb.y, sel.ne.y);
}

void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}

/*
void
execsh(char *cmd, char **args)
{
	char *sh, *prog, *arg;
	const struct passwd *pw;

	errno = 0;
	if ((pw = getpwuid(getuid())) == NULL) {
		if (errno)
			die("getpwuid: %s\n", strerror(errno));
		else
			die("who are you?\n");
	}

	if ((sh = getenv("SHELL")) == NULL)
		sh = (pw->pw_shell[0]) ? pw->pw_shell : cmd;

	if (args) {
		prog = args[0];
		arg = NULL;
	} else if (scroll) {
		prog = scroll;
		arg = utmp ? utmp : sh;
	} else if (utmp) {
		prog = utmp;
		arg = NULL;
	} else {
		prog = sh;
		arg = NULL;
	}
	DEFAULT(args, ((char *[]) {prog, arg, NULL}));

	unsetenv("COLUMNS");
	unsetenv("LINES");
	unsetenv("TERMCAP");
	setenv("LOGNAME", pw->pw_name, 1);
	setenv("USER", pw->pw_name, 1);
	setenv("SHELL", sh, 1);
	setenv("HOME", pw->pw_dir, 1);
	setenv("TERM", termname, 1);

	signal(SIGCHLD, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGALRM, SIG_DFL);

	execvp(prog, args);
	_exit(1);
}
*/

/*
void
sigchld(int a)
{
	int stat;
	pid_t p;

	if ((p = waitpid(pid, &stat, WNOHANG)) < 0)
		die("waiting for pid %hd failed: %s\n", pid, strerror(errno));

	if (pid != p)
		return;

	if (WIFEXITED(stat) && WEXITSTATUS(stat))
		die("child exited with status %d\n", WEXITSTATUS(stat));
	else if (WIFSIGNALED(stat))
		die("child terminated due to signal %d\n", WTERMSIG(stat));
	_exit(0);
}
*/

/*
void
stty(char **args)
{
	char cmd[_POSIX_ARG_MAX], **p, *q, *s;
	size_t n, siz;

	if ((n = strlen(stty_args)) > sizeof(cmd)-1)
		die("incorrect stty parameters\n");
	memcpy(cmd, stty_args, n);
	q = cmd + n;
	siz = sizeof(cmd) - n;
	for (p = args; p && (s = *p); ++p) {
		if ((n = strlen(s)) > siz-1)
			die("stty parameter length too long\n");
		*q++ = ' ';
		memcpy(q, s, n);
		q += n;
		siz -= n + 1;
	}
	*q = '\0';
	if (system(cmd) != 0)
		perror("Couldn't call stty");
}
*/

/*
int
ttynew(char *line, char *cmd, char *out, char **args)
{
	int m, s;

	if (out) {
		term.mode |= MODE_PRINT;
		iofd = (!strcmp(out, "-")) ?
			  1 : open(out, O_WRONLY | O_CREAT, 0666);
		if (iofd < 0) {
			fprintf(stderr, "Error opening %s:%s\n",
				out, strerror(errno));
		}
	}

	if (line) {
		if ((cmdfd = open(line, O_RDWR)) < 0)
			die("open line '%s' failed: %s\n",
			    line, strerror(errno));
		dup2(cmdfd, 0);
		stty(args);
		return cmdfd;
	}

	// seems to work fine on linux, openbsd and freebsd
	if (openpty(&m, &s, NULL, NULL, NULL) < 0)
		die("openpty failed: %s\n", strerror(errno));

	switch (pid = fork()) {
	case -1:
		die("fork failed: %s\n", strerror(errno));
		break;
	case 0:
		close(iofd);
		setsid(); // create a new process group
		dup2(s, 0);
		dup2(s, 1);
		dup2(s, 2);
		if (ioctl(s, TIOCSCTTY, NULL) < 0)
			die("ioctl TIOCSCTTY failed: %s\n", strerror(errno));
		close(s);
		close(m);
#ifdef __OpenBSD__
		if (pledge("stdio getpw proc exec", NULL) == -1)
			die("pledge\n");
#endif
		execsh(cmd, args);
		break;
	default:
#ifdef __OpenBSD__
		if (pledge("stdio rpath tty proc", NULL) == -1)
			die("pledge\n");
#endif
		close(s);
		cmdfd = m;
		signal(SIGCHLD, sigchld);
		break;
	}
	return cmdfd;
}
*/

size_t
ttyread(void)
{
	static char buf[256];
    static int buflen = 0;
	int ret, written;

    size_t length = 0;
    //uart_get_buffered_data_len(UART_NUM_1, &length);
    ret = uart_read_bytes(UART_NUM_1, (uint8_t *)buf+buflen, 200, 5);
	switch (ret) {
	case 0:
        draw();
		return 0;
	case -1:
		die("couldn't read from shell: %s\n", esp_err_to_name(ret));
        return 0;
	default: {
		buflen += ret;
		written = twrite(buf, buflen, 0);
		buflen -= written;
		/* keep any incomplete UTF-8 byte sequence for the next call */
		if (buflen > 0)
			memmove(buf, buf + written, buflen);
		return ret;
	 }
	}
}

void
ttywrite(const char *s, size_t n, int may_echo)
{
	const char *next;

	if (may_echo && IS_SET(MODE_ECHO))
		twrite(s, n, 1);

	if (!IS_SET(MODE_CRLF)) {
		ttywriteraw(s, n);
		return;
	}

	/* This is similar to how the kernel handles ONLCR for ttys */
	while (n > 0) {
		if (*s == '\r') {
			next = s + 1;
			ttywriteraw("\r\n", 2);
		} else {
			next = memchr(s, '\r', n);
			DEFAULT(next, s + n);
			ttywriteraw(s, next - s);
		}
		n -= next - s;
		s = next;
	}
}

void
ttywriteraw(const char *s, size_t n)
{
	//fd_set wfd, rfd;
	ssize_t r;
	size_t lim = 256;

	/*
	 * Remember that we are using a pty, which might be a modem line.
	 * Writing too much will clog the line. That's why we are doing this
	 * dance.
	 * FIXME: Migrate the world to Plan 9.
	 */
    ESP_LOGI("term", "ttywriteraw!");
	while (n > 0) {
		// FD_ZERO(&wfd);
		// FD_ZERO(&rfd);
		// FD_SET(cmdfd, &wfd);
		// FD_SET(cmdfd, &rfd);

		/* Check if we can write. */
		// if (pselect(cmdfd+1, &rfd, &wfd, NULL, NULL, NULL) < 0) {
		// 	if (errno == EINTR)
		// 		continue;
		// 	die("select failed: %s\n", strerror(errno));
		// }
		// if (FD_ISSET(cmdfd, &wfd)) {
			/*
			 * Only write the bytes written by ttywrite() or the
			 * default of 256. This seems to be a reasonable value
			 * for a serial line. Bigger values might clog the I/O.
			 */
			if ((r = uart_write_bytes(UART_NUM_1, s, (n < lim)? n : lim)) < 0)
	            die("write error on tty: %s\n", esp_err_to_name(r));
			if (r < n) {
				/*
				 * We weren't able to write out everything.
				 * This means the buffer is getting full
				 * again. Empty it.
				 */
				if (n < lim)
					lim = ttyread();
				n -= r;
				s += r;
			} else {
				/* All bytes have been written. */
				break;
			}
		//}
		//if (FD_ISSET(cmdfd, &rfd))
        lim = ttyread();
	}
	return;
}

void
ttyresize(int tw, int th)
{
    // TODO: dynamic window resize is unsupported.

	// struct winsize w;

	// w.ws_row = term.row;
	// w.ws_col = term.col;
	// w.ws_xpixel = tw;
	// w.ws_ypixel = th;
	// if (ioctl(cmdfd, TIOCSWINSZ, &w) < 0)
	// 	fprintf(stderr, "Couldn't set window size: %s\n", strerror(errno));
}

void
ttyhangup()
{
	/* Send SIGHUP to shell */
    die("tty died");
	//kill(pid, SIGHUP);
}

int
tattrset(int attr)
{
	int i, j;

	for (i = 0; i < term.row-1; i++) {
		for (j = 0; j < term.col-1; j++) {
			if (term.line[i][j].mode & attr)
				return 1;
		}
	}

	return 0;
}

void
tsetdirt(int top, int bot)
{
	int i;

	LIMIT(top, 0, term.row-1);
	LIMIT(bot, 0, term.row-1);

	for (i = top; i <= bot; i++)
		term.dirty[i] = 1;
}

void
tsetdirtattr(int attr)
{
	int i, j;

	for (i = 0; i < term.row-1; i++) {
		for (j = 0; j < term.col-1; j++) {
			if (term.line[i][j].mode & attr) {
				tsetdirt(i, i);
				break;
			}
		}
	}
}

void
tfulldirt(void)
{
	tsetdirt(0, term.row-1);
}

void tcurhide(void)
{
    term.c.state |= CURSOR_HIDE;
    term.dirty[term.c.y] = 1;
}

void tcurshow(void)
{
    term.c.state &= ~CURSOR_HIDE;
    term.dirty[term.c.y] = 1;
}

void
tcursor(int mode)
{
	static TCursor c[2];
	int alt = IS_SET(MODE_ALTSCREEN);

	if (mode == CURSOR_SAVE) {
		c[alt] = term.c;
	} else if (mode == CURSOR_LOAD) {
		term.c = c[alt];
		tmoveto(c[alt].x, c[alt].y);
	}
}

void
treset(void)
{
	uint i;

	term.c = (TCursor){{
		.mode = ATTR_NULL,
		.fg = defaultfg,
		.bg = defaultbg
	}, .x = 0, .y = 0, .state = CURSOR_DEFAULT};

	memset(term.tabs, 0, term.col * sizeof(*term.tabs));
	for (i = tabspaces; i < term.col; i += tabspaces)
		term.tabs[i] = 1;
	term.top = 0;
	term.bot = term.row - 1;
	term.mode = MODE_WRAP|MODE_UTF8;
	memset(term.trantbl, CS_USA, sizeof(term.trantbl));
	term.charset = 0;
    memset(render_fb_front, 255, EPD_WIDTH / 2 * EPD_HEIGHT);
    memset(render_fb_back, 255, EPD_WIDTH / 2 * EPD_HEIGHT);
    memset(render_mask, 255, EPD_WIDTH / 8 * EPD_HEIGHT);


	for (i = 0; i < 2; i++) {
		tmoveto(0, 0);
		tcursor(CURSOR_SAVE);
		tclearregion(0, 0, term.col-1, term.row-1);
		tswapscreen();
	}
}

void
tnew(int col, int row)
{
	term = (Term){ .c = { .attr = { .fg = defaultfg, .bg = defaultbg } } };
	tresize(col, row);
	treset();
}

void
tswapscreen(void)
{
	Line *tmp = term.line;

	term.line = term.alt;
	term.alt = tmp;
	term.mode ^= MODE_ALTSCREEN;
	tfulldirt();
}

void
tscrolldown(int orig, int n)
{
	int i;
	Line temp;

	LIMIT(n, 0, term.bot-orig+1);

	tsetdirt(orig, term.bot-n);
	tclearregion(0, term.bot-n+1, term.col-1, term.bot);

	for (i = term.bot; i >= orig+n; i--) {
		temp = term.line[i];
		term.line[i] = term.line[i-n];
		term.line[i-n] = temp;
	}

	selscroll(orig, n);
}

void
tscrollup(int orig, int n)
{
	int i;
	Line temp;

	LIMIT(n, 0, term.bot-orig+1);

	tclearregion(0, orig, term.col-1, orig+n-1);
	tsetdirt(orig+n, term.bot);

	for (i = orig; i <= term.bot-n; i++) {
		temp = term.line[i];
		term.line[i] = term.line[i+n];
		term.line[i+n] = temp;
	}

	selscroll(orig, -n);
}

void
selscroll(int orig, int n)
{
	if (sel.ob.x == -1)
		return;

	if (BETWEEN(sel.ob.y, orig, term.bot) || BETWEEN(sel.oe.y, orig, term.bot)) {
		if ((sel.ob.y += n) > term.bot || (sel.oe.y += n) < term.top) {
			selclear();
			return;
		}
		if (sel.type == SEL_RECTANGULAR) {
			if (sel.ob.y < term.top)
				sel.ob.y = term.top;
			if (sel.oe.y > term.bot)
				sel.oe.y = term.bot;
		} else {
			if (sel.ob.y < term.top) {
				sel.ob.y = term.top;
				sel.ob.x = 0;
			}
			if (sel.oe.y > term.bot) {
				sel.oe.y = term.bot;
				sel.oe.x = term.col;
			}
		}
		selnormalize();
	}
}

void
tnewline(int first_col)
{
	int y = term.c.y;

	if (y == term.bot) {
        // scroll further on bottom
		tscrollup(term.top, 5);
        y -= 5 - 1;
	} else {
		y++;
	}
	tmoveto(first_col ? 0 : term.c.x, y);
}

void
csiparse(void)
{
	char *p = csiescseq.buf, *np;
	long int v;

	csiescseq.narg = 0;
	if (*p == '?') {
		csiescseq.priv = 1;
		p++;
	}

	csiescseq.buf[csiescseq.len] = '\0';
	while (p < csiescseq.buf+csiescseq.len) {
		np = NULL;
		v = strtol(p, &np, 10);
		if (np == p)
			v = 0;
		if (v == LONG_MAX || v == LONG_MIN)
			v = -1;
		csiescseq.arg[csiescseq.narg++] = v;
		p = np;
		if (*p != ';' || csiescseq.narg == ESC_ARG_SIZ)
			break;
		p++;
	}
	csiescseq.mode[0] = *p++;
	csiescseq.mode[1] = (p < csiescseq.buf+csiescseq.len) ? *p : '\0';
}

/* for absolute user moves, when decom is set */
void
tmoveato(int x, int y)
{
	tmoveto(x, y + ((term.c.state & CURSOR_ORIGIN) ? term.top: 0));
}

void
tmoveto(int x, int y)
{
	int miny, maxy;

	if (term.c.state & CURSOR_ORIGIN) {
		miny = term.top;
		maxy = term.bot;
	} else {
		miny = 0;
		maxy = term.row - 1;
	}
	term.c.state &= ~CURSOR_WRAPNEXT;
    term.dirty[term.c.y] = 1;
	term.c.x = LIMIT(x, 0, term.col-1);
	term.c.y = LIMIT(y, miny, maxy);
    term.dirty[term.c.y] = 1;
}

void
tsetchar(Rune u, Glyph *attr, int x, int y)
{
	static char *vt100_0[62] = { /* 0x41 - 0x7e */
		"↑", "↓", "→", "←", "█", "▚", "☃", /* A - G */
		0, 0, 0, 0, 0, 0, 0, 0, /* H - O */
		0, 0, 0, 0, 0, 0, 0, 0, /* P - W */
		0, 0, 0, 0, 0, 0, 0, " ", /* X - _ */
		"◆", "▒", "␉", "␌", "␍", "␊", "°", "±", /* ` - g */
		"␤", "␋", "┘", "┐", "┌", "└", "┼", "⎺", /* h - o */
		"⎻", "─", "⎼", "⎽", "├", "┤", "┴", "┬", /* p - w */
		"│", "≤", "≥", "π", "≠", "£", "·", /* x - ~ */
	};

	/*
	 * The table is proudly stolen from rxvt.
	 */
	if (term.trantbl[term.charset] == CS_GRAPHIC0 &&
	   BETWEEN(u, 0x41, 0x7e) && vt100_0[u - 0x41])
		utf8decode(vt100_0[u - 0x41], &u, UTF_SIZ);

	if (term.line[y][x].mode & ATTR_WIDE) {
		if (x+1 < term.col) {
			term.line[y][x+1].u = ' ';
			term.line[y][x+1].mode &= ~ATTR_WDUMMY;
		}
	} else if (term.line[y][x].mode & ATTR_WDUMMY) {
		term.line[y][x-1].u = ' ';
		term.line[y][x-1].mode &= ~ATTR_WIDE;
	}

	term.dirty[y] = 1;
	term.line[y][x] = *attr;
	term.line[y][x].u = u;
}

void
tclearregion(int x1, int y1, int x2, int y2)
{
	int x, y, temp;
	Glyph *gp;

	if (x1 > x2)
		temp = x1, x1 = x2, x2 = temp;
	if (y1 > y2)
		temp = y1, y1 = y2, y2 = temp;

	LIMIT(x1, 0, term.col-1);
	LIMIT(x2, 0, term.col-1);
	LIMIT(y1, 0, term.row-1);
	LIMIT(y2, 0, term.row-1);

	for (y = y1; y <= y2; y++) {
		term.dirty[y] = 1;
		for (x = x1; x <= x2; x++) {
			gp = &term.line[y][x];
			if (selected(x, y))
				selclear();
			gp->fg = term.c.attr.fg;
			gp->bg = term.c.attr.bg;
			gp->mode = 0;
			gp->u = ' ';
		}
	}
}

void
tdeletechar(int n)
{
	int dst, src, size;
	Glyph *line;

	LIMIT(n, 0, term.col - term.c.x);

	dst = term.c.x;
	src = term.c.x + n;
	size = term.col - src;
	line = term.line[term.c.y];

	memmove(&line[dst], &line[src], size * sizeof(Glyph));
	tclearregion(term.col-n, term.c.y, term.col-1, term.c.y);
}

void
tinsertblank(int n)
{
	int dst, src, size;
	Glyph *line;

	LIMIT(n, 0, term.col - term.c.x);

	dst = term.c.x + n;
	src = term.c.x;
	size = term.col - dst;
	line = term.line[term.c.y];

	memmove(&line[dst], &line[src], size * sizeof(Glyph));
	tclearregion(src, term.c.y, dst - 1, term.c.y);
}

void
tinsertblankline(int n)
{
	if (BETWEEN(term.c.y, term.top, term.bot))
		tscrolldown(term.c.y, n);
}

void
tdeleteline(int n)
{
	if (BETWEEN(term.c.y, term.top, term.bot))
		tscrollup(term.c.y, n);
}

int32_t
tdefcolor(int *attr, int *npar, int l)
{
	int32_t idx = -1;
	uint r, g, b;

	switch (attr[*npar + 1]) {
	case 2: /* direct color in RGB space */
		if (*npar + 4 >= l) {
			fprintf(stderr,
				"erresc(38): Incorrect number of parameters (%d)\n",
				*npar);
			break;
		}
		r = attr[*npar + 2];
		g = attr[*npar + 3];
		b = attr[*npar + 4];
		*npar += 4;
		if (!BETWEEN(r, 0, 255) || !BETWEEN(g, 0, 255) || !BETWEEN(b, 0, 255))
			fprintf(stderr, "erresc: bad rgb color (%u,%u,%u)\n",
				r, g, b);
		else
			idx = TRUECOLOR(r, g, b);
		break;
	case 5: /* indexed color */
		if (*npar + 2 >= l) {
			fprintf(stderr,
				"erresc(38): Incorrect number of parameters (%d)\n",
				*npar);
			break;
		}
		*npar += 2;
		if (!BETWEEN(attr[*npar], 0, 255))
			fprintf(stderr, "erresc: bad fgcolor %d\n", attr[*npar]);
		else
			idx = attr[*npar];
		break;
	case 0: /* implemented defined (only foreground) */
	case 1: /* transparent */
	case 3: /* direct color in CMY space */
	case 4: /* direct color in CMYK space */
	default:
		fprintf(stderr,
		        "erresc(38): gfx attr %d unknown\n", attr[*npar]);
		break;
	}

	return idx;
}

void
tsetattr(int *attr, int l)
{
	int i;
	int32_t idx;

	for (i = 0; i < l; i++) {
		switch (attr[i]) {
		case 0:
			term.c.attr.mode &= ~(
				ATTR_BOLD       |
				ATTR_FAINT      |
				ATTR_ITALIC     |
				ATTR_UNDERLINE  |
				ATTR_BLINK      |
				ATTR_REVERSE    |
				ATTR_INVISIBLE  |
				ATTR_STRUCK     );
			term.c.attr.fg = defaultfg;
			term.c.attr.bg = defaultbg;
			break;
		case 1:
			term.c.attr.mode |= ATTR_BOLD;
			break;
		case 2:
			term.c.attr.mode |= ATTR_FAINT;
			break;
		case 3:
			term.c.attr.mode |= ATTR_ITALIC;
			break;
		case 4:
			term.c.attr.mode |= ATTR_UNDERLINE;
			break;
		case 5: /* slow blink */
			/* FALLTHROUGH */
		case 6: /* rapid blink */
			term.c.attr.mode |= ATTR_BLINK;
			break;
		case 7:
			term.c.attr.mode |= ATTR_REVERSE;
			break;
		case 8:
			term.c.attr.mode |= ATTR_INVISIBLE;
			break;
		case 9:
			term.c.attr.mode |= ATTR_STRUCK;
			break;
		case 22:
			term.c.attr.mode &= ~(ATTR_BOLD | ATTR_FAINT);
			break;
		case 23:
			term.c.attr.mode &= ~ATTR_ITALIC;
			break;
		case 24:
			term.c.attr.mode &= ~ATTR_UNDERLINE;
			break;
		case 25:
			term.c.attr.mode &= ~ATTR_BLINK;
			break;
		case 27:
			term.c.attr.mode &= ~ATTR_REVERSE;
			break;
		case 28:
			term.c.attr.mode &= ~ATTR_INVISIBLE;
			break;
		case 29:
			term.c.attr.mode &= ~ATTR_STRUCK;
			break;
		case 38:
			if ((idx = tdefcolor(attr, &i, l)) >= 0)
				term.c.attr.fg = idx;
			break;
		case 39:
			term.c.attr.fg = defaultfg;
			break;
		case 48:
			if ((idx = tdefcolor(attr, &i, l)) >= 0)
				term.c.attr.bg = idx;
			break;
		case 49:
			term.c.attr.bg = defaultbg;
			break;
		default:
			if (BETWEEN(attr[i], 30, 37)) {
				term.c.attr.fg = attr[i] - 30;
			} else if (BETWEEN(attr[i], 40, 47)) {
				term.c.attr.bg = attr[i] - 40;
			} else if (BETWEEN(attr[i], 90, 97)) {
				term.c.attr.fg = attr[i] - 90 + 8;
			} else if (BETWEEN(attr[i], 100, 107)) {
				term.c.attr.bg = attr[i] - 100 + 8;
			} else {
				fprintf(stderr,
					"erresc(default): gfx attr %d unknown\n",
					attr[i]);
				csidump();
			}
			break;
		}
	}
}

void
tsetscroll(int t, int b)
{
	int temp;

	LIMIT(t, 0, term.row-1);
	LIMIT(b, 0, term.row-1);
	if (t > b) {
		temp = t;
		t = b;
		b = temp;
	}
	term.top = t;
	term.bot = b;
}

void
tsetmode(int priv, int set, int *args, int narg)
{
	int alt, *lim;

	for (lim = args + narg; args < lim; ++args) {
		if (priv) {
			switch (*args) {
			case 1: /* DECCKM -- Cursor key */
                // TODO: Cursor rendering
				//xsetmode(set, MODE_APPCURSOR);
				break;
			case 5: /* DECSCNM -- Reverse video */
				//xsetmode(set, MODE_REVERSE);
				break;
			case 6: /* DECOM -- Origin */
				MODBIT(term.c.state, set, CURSOR_ORIGIN);
				tmoveato(0, 0);
				break;
			case 7: /* DECAWM -- Auto wrap */
				MODBIT(term.mode, set, MODE_WRAP);
				break;
			case 0:  /* Error (IGNORED) */
			case 2:  /* DECANM -- ANSI/VT52 (IGNORED) */
			case 3:  /* DECCOLM -- Column  (IGNORED) */
			case 4:  /* DECSCLM -- Scroll (IGNORED) */
			case 8:  /* DECARM -- Auto repeat (IGNORED) */
			case 18: /* DECPFF -- Printer feed (IGNORED) */
			case 19: /* DECPEX -- Printer extent (IGNORED) */
			case 42: /* DECNRCM -- National characters (IGNORED) */
			case 12: /* att610 -- Start blinking cursor (IGNORED) */
				break;
			case 25: /* DECTCEM -- Text Cursor Enable Mode */
				if (!set) {
					tcurhide();
				} else {
					tcurshow();
				}
				//xsetmode(!set, MODE_HIDE);
				break;
			case 9:    /* X10 mouse compatibility mode */
                // TODO: Some kind of mouse input
				// xsetpointermotion(0);
				// xsetmode(0, MODE_MOUSE);
				// xsetmode(set, MODE_MOUSEX10);
				break;
			case 1000: /* 1000: report button press */
                // TODO: Some kind of mouse input
				// xsetpointermotion(0);
				// xsetmode(0, MODE_MOUSE);
				// xsetmode(set, MODE_MOUSEBTN);
				break;
			case 1002: /* 1002: report motion on button press */
                // TODO: Some kind of mouse input
				// xsetpointermotion(0);
				// xsetmode(0, MODE_MOUSE);
				// xsetmode(set, MODE_MOUSEMOTION);
				break;
			case 1003: /* 1003: enable all mouse motions */
                // TODO: Some kind of mouse input
				// xsetpointermotion(set);
				// xsetmode(0, MODE_MOUSE);
				// xsetmode(set, MODE_MOUSEMANY);
				break;
			case 1004: /* 1004: send focus events to tty */
				//xsetmode(set, MODE_FOCUS);
				break;
			case 1006: /* 1006: extended reporting mode */
                // TODO: Some kind of mouse input
				//xsetmode(set, MODE_MOUSESGR);
				break;
			case 1034:
				//xsetmode(set, MODE_8BIT);
				break;
			case 1049: /* swap screen & set/restore cursor as xterm */
				if (!allowaltscreen)
					break;
				tcursor((set) ? CURSOR_SAVE : CURSOR_LOAD);
				/* FALLTHROUGH */
			case 47: /* swap screen */
			case 1047:
				if (!allowaltscreen)
					break;
				alt = IS_SET(MODE_ALTSCREEN);
				if (alt) {
					tclearregion(0, 0, term.col-1,
							term.row-1);
				}
                full_refresh();
				if (set ^ alt) /* set is always 1 or 0 */
					tswapscreen();
				if (*args != 1049)
					break;
				/* FALLTHROUGH */
			case 1048:
				tcursor((set) ? CURSOR_SAVE : CURSOR_LOAD);
				break;
			case 2004: /* 2004: bracketed paste mode */
                // TODO: Bracketed paste
				// xsetmode(set, MODE_BRCKTPASTE);
				break;
			/* Not implemented mouse modes. See comments there. */
			case 1001: /* mouse highlight mode; can hang the
				      terminal by design when implemented. */
			case 1005: /* UTF-8 mouse mode; will confuse
				      applications not supporting UTF-8
				      and luit. */
			case 1015: /* urxvt mangled mouse mode; incompatible
				      and can be mistaken for other control
				      codes. */
				break;
			default:
				fprintf(stderr,
					"erresc: unknown private set/reset mode %d\n",
					*args);
				break;
			}
		} else {
			switch (*args) {
			case 0:  /* Error (IGNORED) */
				break;
			case 2:
                // TODO: MODE_KBDLOCK
				// xsetmode(set, MODE_KBDLOCK);
				break;
			case 4:  /* IRM -- Insertion-replacement */
				MODBIT(term.mode, set, MODE_INSERT);
				break;
			case 12: /* SRM -- Send/Receive */
				MODBIT(term.mode, !set, MODE_ECHO);
				break;
			case 20: /* LNM -- Linefeed/new line */
				MODBIT(term.mode, set, MODE_CRLF);
				break;
			default:
				fprintf(stderr,
					"erresc: unknown set/reset mode %d\n",
					*args);
				break;
			}
		}
	}
}

void
csihandle(void)
{
	char buf[40];
	int len;

	switch (csiescseq.mode[0]) {
	default:
	unknown:
		fprintf(stderr, "erresc: unknown csi ");
		csidump();
		/* die(""); */
		break;
	case '@': /* ICH -- Insert <n> blank char */
		DEFAULT(csiescseq.arg[0], 1);
		tinsertblank(csiescseq.arg[0]);
		break;
	case 'A': /* CUU -- Cursor <n> Up */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(term.c.x, term.c.y-csiescseq.arg[0]);
		break;
	case 'B': /* CUD -- Cursor <n> Down */
	case 'e': /* VPR --Cursor <n> Down */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(term.c.x, term.c.y+csiescseq.arg[0]);
		break;
	case 'i': /* MC -- Media Copy */
		switch (csiescseq.arg[0]) {
		case 0:
			tdump();
			break;
		case 1:
			tdumpline(term.c.y);
			break;
		case 2:
			tdumpsel();
			break;
		case 4:
			term.mode &= ~MODE_PRINT;
			break;
		case 5:
			term.mode |= MODE_PRINT;
			break;
		}
		break;
	case 'c': /* DA -- Device Attributes */
		if (csiescseq.arg[0] == 0)
			ttywrite(vtiden, strlen(vtiden), 0);
		break;
	case 'C': /* CUF -- Cursor <n> Forward */
	case 'a': /* HPR -- Cursor <n> Forward */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(term.c.x+csiescseq.arg[0], term.c.y);
		break;
	case 'D': /* CUB -- Cursor <n> Backward */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(term.c.x-csiescseq.arg[0], term.c.y);
		break;
	case 'E': /* CNL -- Cursor <n> Down and first col */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(0, term.c.y+csiescseq.arg[0]);
		break;
	case 'F': /* CPL -- Cursor <n> Up and first col */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(0, term.c.y-csiescseq.arg[0]);
		break;
	case 'g': /* TBC -- Tabulation clear */
		switch (csiescseq.arg[0]) {
		case 0: /* clear current tab stop */
			term.tabs[term.c.x] = 0;
			break;
		case 3: /* clear all the tabs */
			memset(term.tabs, 0, term.col * sizeof(*term.tabs));
			break;
		default:
			goto unknown;
		}
		break;
	case 'G': /* CHA -- Move to <col> */
	case '`': /* HPA */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveto(csiescseq.arg[0]-1, term.c.y);
		break;
	case 'H': /* CUP -- Move to <row> <col> */
	case 'f': /* HVP */
		DEFAULT(csiescseq.arg[0], 1);
		DEFAULT(csiescseq.arg[1], 1);
		tmoveato(csiescseq.arg[1]-1, csiescseq.arg[0]-1);
		break;
	case 'I': /* CHT -- Cursor Forward Tabulation <n> tab stops */
		DEFAULT(csiescseq.arg[0], 1);
		tputtab(csiescseq.arg[0]);
		break;
	case 'J': /* ED -- Clear screen */
        ESP_LOGI("term", "clear mode: %d", csiescseq.arg[0]);
		switch (csiescseq.arg[0]) {
		case 0: /* below */
			tclearregion(term.c.x, term.c.y, term.col-1, term.c.y);
			if (term.c.y < term.row-1) {
				tclearregion(0, term.c.y+1, term.col-1,
						term.row-1);
			}
			break;
		case 1: /* above */
			if (term.c.y > 1)
				tclearregion(0, 0, term.col-1, term.c.y-1);
			tclearregion(0, term.c.y, term.c.x, term.c.y);
			break;
		case 2: /* all */
			tclearregion(0, 0, term.col-1, term.row-1);
            full_refresh();
			break;
		default:
			goto unknown;
		}
		break;
	case 'K': /* EL -- Clear line */
		switch (csiescseq.arg[0]) {
		case 0: /* right */
			tclearregion(term.c.x, term.c.y, term.col-1,
					term.c.y);
			break;
		case 1: /* left */
			tclearregion(0, term.c.y, term.c.x, term.c.y);
			break;
		case 2: /* all */
			tclearregion(0, term.c.y, term.col-1, term.c.y);
			break;
		}
		break;
	case 'S': /* SU -- Scroll <n> line up */
		DEFAULT(csiescseq.arg[0], 1);
		tscrollup(term.top, csiescseq.arg[0]);
		break;
	case 'T': /* SD -- Scroll <n> line down */
		DEFAULT(csiescseq.arg[0], 1);
		tscrolldown(term.top, csiescseq.arg[0]);
		break;
	case 'L': /* IL -- Insert <n> blank lines */
		DEFAULT(csiescseq.arg[0], 1);
		tinsertblankline(csiescseq.arg[0]);
		break;
	case 'l': /* RM -- Reset Mode */
		tsetmode(csiescseq.priv, 0, csiescseq.arg, csiescseq.narg);
		break;
	case 'M': /* DL -- Delete <n> lines */
		DEFAULT(csiescseq.arg[0], 1);
		tdeleteline(csiescseq.arg[0]);
		break;
	case 'X': /* ECH -- Erase <n> char */
		DEFAULT(csiescseq.arg[0], 1);
		tclearregion(term.c.x, term.c.y,
				term.c.x + csiescseq.arg[0] - 1, term.c.y);
		break;
	case 'P': /* DCH -- Delete <n> char */
		DEFAULT(csiescseq.arg[0], 1);
		tdeletechar(csiescseq.arg[0]);
		break;
	case 'Z': /* CBT -- Cursor Backward Tabulation <n> tab stops */
		DEFAULT(csiescseq.arg[0], 1);
		tputtab(-csiescseq.arg[0]);
		break;
	case 'd': /* VPA -- Move to <row> */
		DEFAULT(csiescseq.arg[0], 1);
		tmoveato(term.c.x, csiescseq.arg[0]-1);
		break;
	case 'h': /* SM -- Set terminal mode */
		tsetmode(csiescseq.priv, 1, csiescseq.arg, csiescseq.narg);
		break;
	case 'm': /* SGR -- Terminal attribute (color) */
		tsetattr(csiescseq.arg, csiescseq.narg);
		break;
	case 'n': /* DSR – Device Status Report (cursor position) */
		if (csiescseq.arg[0] == 6) {
			len = snprintf(buf, sizeof(buf),"\033[%i;%iR",
					term.c.y+1, term.c.x+1);
			ttywrite(buf, len, 0);
		}
		break;
	case 'r': /* DECSTBM -- Set Scrolling Region */
		if (csiescseq.priv) {
			goto unknown;
		} else {
			DEFAULT(csiescseq.arg[0], 1);
			DEFAULT(csiescseq.arg[1], term.row);
			tsetscroll(csiescseq.arg[0]-1, csiescseq.arg[1]-1);
			tmoveato(0, 0);
		}
		break;
	case 's': /* DECSC -- Save cursor position (ANSI.SYS) */
		tcursor(CURSOR_SAVE);
		break;
	case 'u': /* DECRC -- Restore cursor position (ANSI.SYS) */
		tcursor(CURSOR_LOAD);
		break;
	case ' ':
		switch (csiescseq.mode[1]) {
		case 'q': /* DECSCUSR -- Set Cursor Style */
            // TODO: Cusor style
			// if (xsetcursor(csiescseq.arg[0]))
			// 	goto unknown;
			break;
		default:
			goto unknown;
		}
		break;
	}
}

void
csidump(void)
{
	size_t i;
	uint c;

	fprintf(stderr, "ESC[");
	for (i = 0; i < csiescseq.len; i++) {
		c = csiescseq.buf[i] & 0xff;
		if (isprint(c)) {
			putc(c, stderr);
		} else if (c == '\n') {
			fprintf(stderr, "(\\n)");
		} else if (c == '\r') {
			fprintf(stderr, "(\\r)");
		} else if (c == 0x1b) {
			fprintf(stderr, "(\\e)");
		} else {
			fprintf(stderr, "(%02x)", c);
		}
	}
	putc('\n', stderr);
}

void
csireset(void)
{
	memset(&csiescseq, 0, sizeof(csiescseq));
}

void
strhandle(void)
{
	char *dec;
	int narg, par;

	term.esc &= ~(ESC_STR_END|ESC_STR);
	strparse();
	par = (narg = strescseq.narg) ? atoi(strescseq.args[0]) : 0;

	switch (strescseq.type) {
    // useful resources: https://github.com/terminalguide/terminalguide
	case ']': /* OSC -- Operating System Command */
        // TODO: 9 / 777 for system notification?
		switch (par) {
		case 0:
		case 1:
		case 2:
            // TODO: Settitle?
			// if (narg > 1)
			// 	xsettitle(strescseq.args[1]);
			return;
        // clipboard handling
		case 52:
            if (narg <= 1 || !(strnlen(strescseq.args[1], 2) == 1)) {
				fprintf(stderr, "OSC 52: invalid operation\n");
                return;
            }

            char op = strescseq.args[1][0];
			if (narg > 2 && op == 'c') {
				dec = base64dec(strescseq.args[2]);
				if (dec) {
            	    fprintf(stderr, "copied.\n");
                    free(clipboard);
                    clipboard = dec;
				} else {
					fprintf(stderr, "erresc: invalid base64\n");
				}
			} else if (op == 'w') {
                if (allow_clipboard_paste) {
                    clipboard_paste();
                } else {
                    fprintf(stderr, "clipboard paste disabled.\n");
                }
            }
            return;
		case 4: /* color set */
			// if (narg < 3)
			// 	break;
			// p = strescseq.args[2];
			/* FALLTHROUGH */
		case 104: /* color reset, here p = NULL */
			//j = (narg > 1) ? atoi(strescseq.args[1]) : -1;
            // TODO: OS color set / reset
			// if (xsetcolorname(j, p)) {
			// 	if (par == 104 && narg <= 1)
			// 		return; /* color reset without parameter */
			// 	fprintf(stderr, "erresc: invalid color j=%d, p=%s\n",
			// 	        j, p ? p : "(null)");
			// } else {
			// 	/*
			// 	 * TODO if defaultbg color is changed, borders
			// 	 * are dirty
			// 	 */
			// 	redraw();
			// }
			return;
        case 800: /* EDP-specific: refresh screen. */
            full_refresh();
            fprintf(stderr, "refresh.\n");
            return;
        case 801: /* EPD-specific: cycle fontset */
            next_fontset();
            full_refresh();
            fprintf(stderr, "swiched to fontset %s.\n", fontsets[current_fontset].name);
		}
		break;
	case 'k': /* old title set compatibility */
        // TODO: set title, again...
		// xsettitle(strescseq.args[0]);
		return;
	case 'P': /* DCS -- Device Control String */
		term.mode |= ESC_DCS;
	case '_': /* APC -- Application Program Command */
	case '^': /* PM -- Privacy Message */
		return;
	}

	fprintf(stderr, "erresc: unknown str ");
	strdump();
}

void
strparse(void)
{
	int c;
	char *p = strescseq.buf;

	strescseq.narg = 0;
	strescseq.buf[strescseq.len] = '\0';

	if (*p == '\0')
		return;

	while (strescseq.narg < STR_ARG_SIZ) {
		strescseq.args[strescseq.narg++] = p;
		while ((c = *p) != ';' && c != '\0')
			++p;
		if (c == '\0')
			return;
		*p++ = '\0';
	}
}

void
strdump(void)
{
	size_t i;
	uint c;

	fprintf(stderr, "ESC%c", strescseq.type);
	for (i = 0; i < strescseq.len; i++) {
		c = strescseq.buf[i] & 0xff;
		if (c == '\0') {
			putc('\n', stderr);
			return;
		} else if (isprint(c)) {
			putc(c, stderr);
		} else if (c == '\n') {
			fprintf(stderr, "(\\n)");
		} else if (c == '\r') {
			fprintf(stderr, "(\\r)");
		} else if (c == 0x1b) {
			fprintf(stderr, "(\\e)");
		} else {
			fprintf(stderr, "(%02x)", c);
		}
	}
	fprintf(stderr, "ESC\\\n");
}

void
strreset(void)
{
	strescseq = (STREscape){
		.buf = xrealloc(strescseq.buf, STR_BUF_SIZ),
		.siz = STR_BUF_SIZ,
	};
}

// void
// sendbreak(const Arg *arg)
// {
// 	if (tcsendbreak(cmdfd, 0))
// 		perror("Error sending break");
// }

void
tprinter(char *s, size_t len)
{
    // TODO: trpinter Might be useful if selections are used?
	// if (iofd != -1 && xwrite(iofd, s, len) < 0) {
	// 	perror("Error writing to output file");
	// 	close(iofd);
	// 	iofd = -1;
	// }
}

void
toggleprinter(const Arg *arg)
{
	term.mode ^= MODE_PRINT;
}

void
printscreen(const Arg *arg)
{
	tdump();
}

void
printsel(const Arg *arg)
{
	tdumpsel();
}

void
tdumpsel(void)
{
	char *ptr;

	if ((ptr = getsel())) {
		tprinter(ptr, strlen(ptr));
		free(ptr);
	}
}

void
tdumpline(int n)
{
	char buf[UTF_SIZ];
	Glyph *bp, *end;

	bp = &term.line[n][0];
	end = &bp[MIN(tlinelen(n), term.col) - 1];
	if (bp != end || bp->u != ' ') {
		for ( ;bp <= end; ++bp)
			tprinter(buf, utf8encode(bp->u, buf));
	}
	tprinter("\n", 1);
}

void
tdump(void)
{
	int i;

	for (i = 0; i < term.row; ++i)
		tdumpline(i);
}

void
tputtab(int n)
{
	uint x = term.c.x;

	if (n > 0) {
		while (x < term.col && n--)
			for (++x; x < term.col && !term.tabs[x]; ++x)
				/* nothing */ ;
	} else if (n < 0) {
		while (x > 0 && n++)
			for (--x; x > 0 && !term.tabs[x]; --x)
				/* nothing */ ;
	}
	term.c.x = LIMIT(x, 0, term.col-1);
}

void
tdefutf8(char ascii)
{
	if (ascii == 'G')
		term.mode |= MODE_UTF8;
	else if (ascii == '@')
		term.mode &= ~MODE_UTF8;
}

void
tdeftran(char ascii)
{
	static char cs[] = "0B";
	static int vcs[] = {CS_GRAPHIC0, CS_USA};
	char *p;

	if ((p = strchr(cs, ascii)) == NULL) {
		fprintf(stderr, "esc unhandled charset: ESC ( %c\n", ascii);
	} else {
		term.trantbl[term.icharset] = vcs[p - cs];
	}
}

void
tdectest(char c)
{
	int x, y;

	if (c == '8') { /* DEC screen alignment test. */
		for (x = 0; x < term.col; ++x) {
			for (y = 0; y < term.row; ++y)
				tsetchar('E', &term.c.attr, x, y);
		}
	}
}

void
tstrsequence(uchar c)
{
	strreset();

	switch (c) {
	case 0x90:   /* DCS -- Device Control String */
		c = 'P';
		term.esc |= ESC_DCS;
		break;
	case 0x9f:   /* APC -- Application Program Command */
		c = '_';
		break;
	case 0x9e:   /* PM -- Privacy Message */
		c = '^';
		break;
	case 0x9d:   /* OSC -- Operating System Command */
		c = ']';
		break;
	}
	strescseq.type = c;
	term.esc |= ESC_STR;
}

void
tcontrolcode(uchar ascii)
{
	switch (ascii) {
	case '\t':   /* HT */
		tputtab(1);
		return;
	case '\b':   /* BS */
		tmoveto(term.c.x-1, term.c.y);
		return;
	case '\r':   /* CR */
		tmoveto(0, term.c.y);
		return;
	case '\f':   /* LF */
	case '\v':   /* VT */
	case '\n':   /* LF */
		/* go to first col if the mode is set */
		tnewline(IS_SET(MODE_CRLF));
		return;
	case '\a':   /* BEL */
		if (term.esc & ESC_STR_END) {
			/* backwards compatibility to xterm */
			strhandle();
		} else {
            // TODO: Some bell mechanism
			// xbell();
		}
		break;
	case '\033': /* ESC */
		csireset();
		term.esc &= ~(ESC_CSI|ESC_ALTCHARSET|ESC_TEST);
		term.esc |= ESC_START;
		return;
	case '\016': /* SO (LS1 -- Locking shift 1) */
	case '\017': /* SI (LS0 -- Locking shift 0) */
		term.charset = 1 - (ascii - '\016');
		return;
	case '\032': /* SUB */
		tsetchar('?', &term.c.attr, term.c.x, term.c.y);
	case '\030': /* CAN */
		csireset();
		break;
	case '\005': /* ENQ (IGNORED) */
	case '\000': /* NUL (IGNORED) */
	case '\021': /* XON (IGNORED) */
	case '\023': /* XOFF (IGNORED) */
	case 0177:   /* DEL (IGNORED) */
		return;
	case 0x80:   /* TODO: PAD */
	case 0x81:   /* TODO: HOP */
	case 0x82:   /* TODO: BPH */
	case 0x83:   /* TODO: NBH */
	case 0x84:   /* TODO: IND */
		break;
	case 0x85:   /* NEL -- Next line */
		tnewline(1); /* always go to first col */
		break;
	case 0x86:   /* TODO: SSA */
	case 0x87:   /* TODO: ESA */
		break;
	case 0x88:   /* HTS -- Horizontal tab stop */
		term.tabs[term.c.x] = 1;
		break;
	case 0x89:   /* TODO: HTJ */
	case 0x8a:   /* TODO: VTS */
	case 0x8b:   /* TODO: PLD */
	case 0x8c:   /* TODO: PLU */
	case 0x8d:   /* TODO: RI */
	case 0x8e:   /* TODO: SS2 */
	case 0x8f:   /* TODO: SS3 */
	case 0x91:   /* TODO: PU1 */
	case 0x92:   /* TODO: PU2 */
	case 0x93:   /* TODO: STS */
	case 0x94:   /* TODO: CCH */
	case 0x95:   /* TODO: MW */
	case 0x96:   /* TODO: SPA */
	case 0x97:   /* TODO: EPA */
	case 0x98:   /* TODO: SOS */
	case 0x99:   /* TODO: SGCI */
		break;
	case 0x9a:   /* DECID -- Identify Terminal */
		ttywrite(vtiden, strlen(vtiden), 0);
		break;
	case 0x9b:   /* TODO: CSI */
	case 0x9c:   /* TODO: ST */
		break;
	case 0x90:   /* DCS -- Device Control String */
	case 0x9d:   /* OSC -- Operating System Command */
	case 0x9e:   /* PM -- Privacy Message */
	case 0x9f:   /* APC -- Application Program Command */
		tstrsequence(ascii);
		return;
	}
	/* only CAN, SUB, \a and C1 chars interrupt a sequence */
	term.esc &= ~(ESC_STR_END|ESC_STR);
}

/*
 * returns 1 when the sequence is finished and it hasn't to read
 * more characters for this sequence, otherwise 0
 */
int
eschandle(uchar ascii)
{
	switch (ascii) {
	case '[':
		term.esc |= ESC_CSI;
		return 0;
	case '#':
		term.esc |= ESC_TEST;
		return 0;
	case '%':
		term.esc |= ESC_UTF8;
		return 0;
	case 'P': /* DCS -- Device Control String */
	case '_': /* APC -- Application Program Command */
	case '^': /* PM -- Privacy Message */
	case ']': /* OSC -- Operating System Command */
	case 'k': /* old title set compatibility */
		tstrsequence(ascii);
		return 0;
	case 'n': /* LS2 -- Locking shift 2 */
	case 'o': /* LS3 -- Locking shift 3 */
		term.charset = 2 + (ascii - 'n');
		break;
	case '(': /* GZD4 -- set primary charset G0 */
	case ')': /* G1D4 -- set secondary charset G1 */
	case '*': /* G2D4 -- set tertiary charset G2 */
	case '+': /* G3D4 -- set quaternary charset G3 */
		term.icharset = ascii - '(';
		term.esc |= ESC_ALTCHARSET;
		return 0;
	case 'D': /* IND -- Linefeed */
		if (term.c.y == term.bot) {
			tscrollup(term.top, 1);
		} else {
			tmoveto(term.c.x, term.c.y+1);
		}
		break;
	case 'E': /* NEL -- Next line */
		tnewline(1); /* always go to first col */
		break;
	case 'H': /* HTS -- Horizontal tab stop */
		term.tabs[term.c.x] = 1;
		break;
	case 'M': /* RI -- Reverse index */
		if (term.c.y == term.top) {
			tscrolldown(term.top, 1);
		} else {
			tmoveto(term.c.x, term.c.y-1);
		}
		break;
	case 'Z': /* DECID -- Identify Terminal */
		ttywrite(vtiden, strlen(vtiden), 0);
		break;
	case 'c': /* RIS -- Reset to initial state */
		treset();
		resettitle();
		//xloadcols();
		break;
	case '=': /* DECPAM -- Application keypad */
        // TODO: Appkeypad
		// xsetmode(1, MODE_APPKEYPAD);
		break;
	case '>': /* DECPNM -- Normal keypad */
        // TODO: normal keypad
		// xsetmode(0, MODE_APPKEYPAD);
		break;
	case '7': /* DECSC -- Save Cursor */
		tcursor(CURSOR_SAVE);
		break;
	case '8': /* DECRC -- Restore Cursor */
		tcursor(CURSOR_LOAD);
		break;
	case '\\': /* ST -- String Terminator */
		if (term.esc & ESC_STR_END)
			strhandle();
		break;
	default:
		fprintf(stderr, "erresc: unknown sequence ESC 0x%02X '%c'\n",
			(uchar) ascii, isprint(ascii)? ascii:'.');
		break;
	}
	return 1;
}

void
tputc(Rune u)
{
	char c[UTF_SIZ];
	int control;
	int width, len;
	Glyph *gp;

	control = ISCONTROL(u);
	if (!IS_SET(MODE_UTF8) && !IS_SET(MODE_SIXEL)) {
		c[0] = u;
		width = len = 1;
	} else {
		len = utf8encode(u, c);
		if (!control && (width = wcwidth(u)) == -1) {
			memcpy(c, "\357\277\275", 4); /* UTF_INVALID */
			width = 1;
		}
	}

	if (IS_SET(MODE_PRINT))
		tprinter(c, len);

	/*
	 * STR sequence must be checked before anything else
	 * because it uses all following characters until it
	 * receives a ESC, a SUB, a ST or any other C1 control
	 * character.
	 */
	if (term.esc & ESC_STR) {
		if (u == '\a' || u == 030 || u == 032 || u == 033 ||
		   ISCONTROLC1(u)) {
			term.esc &= ~(ESC_START|ESC_STR|ESC_DCS);
			if (IS_SET(MODE_SIXEL)) {
				/* TODO: render sixel */;
				term.mode &= ~MODE_SIXEL;
				return;
			}
			term.esc |= ESC_STR_END;
			goto check_control_code;
		}

		if (IS_SET(MODE_SIXEL)) {
			/* TODO: implement sixel mode */
			return;
		}
		if (term.esc&ESC_DCS && strescseq.len == 0 && u == 'q')
			term.mode |= MODE_SIXEL;

		if (strescseq.len+len >= strescseq.siz) {
			/*
			 * Here is a bug in terminals. If the user never sends
			 * some code to stop the str or esc command, then st
			 * will stop responding. But this is better than
			 * silently failing with unknown characters. At least
			 * then users will report back.
			 *
			 * In the case users ever get fixed, here is the code:
			 */
			/*
			 * term.esc = 0;
			 * strhandle();
			 */
			if (strescseq.siz > (SIZE_MAX - UTF_SIZ) / 2)
				return;
			strescseq.siz *= 2;
			strescseq.buf = xrealloc(strescseq.buf, strescseq.siz);
		}

		memmove(&strescseq.buf[strescseq.len], c, len);
		strescseq.len += len;
		return;
	}

check_control_code:
	/*
	 * Actions of control codes must be performed as soon they arrive
	 * because they can be embedded inside a control sequence, and
	 * they must not cause conflicts with sequences.
	 */
	if (control) {
		tcontrolcode(u);
		/*
		 * control codes are not shown ever
		 */
		return;
	} else if (term.esc & ESC_START) {
		if (term.esc & ESC_CSI) {
			csiescseq.buf[csiescseq.len++] = u;
			if (BETWEEN(u, 0x40, 0x7E)
					|| csiescseq.len >= \
					sizeof(csiescseq.buf)-1) {
				term.esc = 0;
				csiparse();
				csihandle();
			}
			return;
		} else if (term.esc & ESC_UTF8) {
			tdefutf8(u);
		} else if (term.esc & ESC_ALTCHARSET) {
			tdeftran(u);
		} else if (term.esc & ESC_TEST) {
			tdectest(u);
		} else {
			if (!eschandle(u))
				return;
			/* sequence already finished */
		}
		term.esc = 0;
		/*
		 * All characters which form part of a sequence are not
		 * printed
		 */
		return;
	}
	if (sel.ob.x != -1 && BETWEEN(term.c.y, sel.ob.y, sel.oe.y))
		selclear();

	gp = &term.line[term.c.y][term.c.x];
	if (IS_SET(MODE_WRAP) && (term.c.state & CURSOR_WRAPNEXT)) {
		gp->mode |= ATTR_WRAP;
		tnewline(1);
		gp = &term.line[term.c.y][term.c.x];
	}

	if (IS_SET(MODE_INSERT) && term.c.x+width < term.col)
		memmove(gp+width, gp, (term.col - term.c.x - width) * sizeof(Glyph));

	if (term.c.x+width > term.col) {
		tnewline(1);
		gp = &term.line[term.c.y][term.c.x];
	}

	tsetchar(u, &term.c.attr, term.c.x, term.c.y);

	if (width == 2) {
		gp->mode |= ATTR_WIDE;
		if (term.c.x+1 < term.col) {
			gp[1].u = '\0';
			gp[1].mode = ATTR_WDUMMY;
		}
	}
	if (term.c.x+width < term.col) {
		tmoveto(term.c.x+width, term.c.y);
	} else {
		term.c.state |= CURSOR_WRAPNEXT;
	}
}

int
twrite(const char *buf, int buflen, int show_ctrl)
{
	int charsize;
	Rune u;
	int n;

	for (n = 0; n < buflen; n += charsize) {
		if (IS_SET(MODE_UTF8) && !IS_SET(MODE_SIXEL)) {
			/* process a complete utf8 char */
			charsize = utf8decode(buf + n, &u, buflen - n);
			if (charsize == 0)
				break;
		} else {
			u = buf[n] & 0xFF;
			charsize = 1;
		}
		if (show_ctrl && ISCONTROL(u)) {
			if (u & 0x80) {
				u &= 0x7f;
				tputc('^');
				tputc('[');
			} else if (u != '\n' && u != '\r' && u != '\t') {
				u ^= 0x40;
				tputc('^');
			}
		}
		tputc(u);
	}
	return n;
}

void
tresize(int col, int row)
{
	int i;
	int minrow = MIN(row, term.row);
	int mincol = MIN(col, term.col);
	int *bp;
	TCursor c;

	if (col < 1 || row < 1) {
		fprintf(stderr,
		        "tresize: error resizing to %dx%d\n", col, row);
		return;
	}

    if (line_render_queue == NULL) {
        line_render_queue = xQueueCreate(row, sizeof(int));
        if (line_render_queue == NULL) {
            die("could not create line queue");
        }

        render_lines_done_smphr = xSemaphoreCreateCounting(row, 0);
        RTOS_ERROR_CHECK(xTaskCreatePinnedToCore(
            &render_line, "lr1", 1 << 12, NULL, 5, NULL, 1));
        RTOS_ERROR_CHECK(xTaskCreatePinnedToCore(
            &render_line, "lr2", 1 << 12, NULL, 5, NULL, 1));
    }

    if (render_fb_front == NULL) {

      render_fb_front = heap_caps_malloc(EPD_WIDTH / 2 * EPD_HEIGHT, MALLOC_CAP_SPIRAM);
    }
    if (render_fb_back == NULL) {
      render_fb_back = heap_caps_malloc(EPD_WIDTH / 2 * EPD_HEIGHT, MALLOC_CAP_SPIRAM);
    }
    if (render_mask == NULL) {
      render_mask = heap_caps_malloc(EPD_WIDTH / 8 * EPD_HEIGHT, MALLOC_CAP_SPIRAM);
    }
    if (render_masked_buf == NULL) {
      render_masked_buf = heap_caps_malloc(EPD_WIDTH / 2 * EPD_HEIGHT, MALLOC_CAP_SPIRAM);
    }
    memset(render_fb_front, 255, EPD_WIDTH / 2 * EPD_HEIGHT);
    memset(render_fb_back, 255, EPD_WIDTH / 2 * EPD_HEIGHT);
    memset(render_mask, 255, EPD_WIDTH / 8 * EPD_HEIGHT);

	/*
	 * slide screen to keep cursor where we expect it -
	 * tscrollup would work here, but we can optimize to
	 * memmove because we're freeing the earlier lines
	 */
	for (i = 0; i <= term.c.y - row; i++) {
		free(term.line[i]);
		free(term.alt[i]);
	}
	/* ensure that both src and dst are not NULL */
	if (i > 0) {
		memmove(term.line, term.line + i, row * sizeof(Line));
		memmove(term.alt, term.alt + i, row * sizeof(Line));
	}
	for (i += row; i < term.row; i++) {
		free(term.line[i]);
		free(term.alt[i]);
	}

	/* resize to new height */
	term.line = xrealloc(term.line, row * sizeof(Line));
	term.alt  = xrealloc(term.alt,  row * sizeof(Line));
	term.dirty = xrealloc(term.dirty, row * sizeof(*term.dirty));
	term.tabs = xrealloc(term.tabs, col * sizeof(*term.tabs));

	/* resize each row to new width, zero-pad if needed */
	for (i = 0; i < minrow; i++) {
		term.line[i] = xrealloc_32(term.line[i], col * sizeof(Glyph));
		term.alt[i]  = xrealloc_32(term.alt[i],  col * sizeof(Glyph));
	}

	/* allocate any new rows */
	for (/* i = minrow */; i < row; i++) {
		term.line[i] = xmalloc_32(col * sizeof(Glyph));
		term.alt[i] = xmalloc_32(col * sizeof(Glyph));
	}
	if (col > term.col) {
		bp = term.tabs + term.col;

		memset(bp, 0, sizeof(*term.tabs) * (col - term.col));
		while (--bp > term.tabs && !*bp)
			/* nothing */ ;
		for (bp += tabspaces; bp < term.tabs + col; bp += tabspaces)
			*bp = 1;
	}
	/* update terminal size */
	term.col = col;
	term.row = row;
	/* reset scrolling region */
	tsetscroll(0, row-1);
	/* make use of the LIMIT in tmoveto */
	tmoveto(term.c.x, term.c.y);
	/* Clearing both screens (it makes dirty all lines) */
	c = term.c;
	for (i = 0; i < 2; i++) {
		if (mincol < col && 0 < minrow) {
			tclearregion(mincol, 0, col - 1, minrow - 1);
		}
		if (0 < col && minrow < row) {
			tclearregion(0, minrow, col - 1, row - 1);
		}
		tswapscreen();
		tcursor(CURSOR_LOAD);
	}
	term.c = c;
}

void
resettitle(void)
{
    // TODO: Title handling
	//xsettitle(NULL);
}

// void
// drawregion(int x1, int y1, int x2, int y2)
// {
// 	int y;
//
// 	for (y = y1; y < y2; y++) {
// 		if (!term.dirty[y])
// 			continue;
//
// 		term.dirty[y] = 0;
// 		xdrawline(term.line[y], x1, y, x2);
// 	}
// }

int calculate_horizontal_advance(const EpdFont* font, Line line, int col) {
  /*
  int total = 0;
  for (int i = 0; i < col; i++) {
    Rune cp = line[i].u;
    const EpdGlyp* glyph;
    // use space width for unfilled columns
    if (cp == 0) {
        cp = 32;
    }
    epd_get_glyph(font, cp, &glyph);

    if (!glyph) {
        epd_get_glyph(font, fallback_glyph, &glyph);
    }

    if (!glyph) {
      ESP_LOGW("terminal", "no glyph for %d", cp);
      continue;
    }
    total += glyph->advance_x;
  }
  return total;
  */
  const EpdGlyph* glyph = epd_get_glyph(font, 32);
  return col * glyph->advance_x;
}

enum RenderOperation {
  NOP = 0,
  WRITE = 1,
  DELETE = 2,
  REPLACE = 3,
};

static int pixel_start_x = 3;
static int pixel_start_y = 20;

static const EpdFont* get_font(Glyph g) {
    const FontSet* fs = &fontsets[current_fontset];
    if (g.mode & ATTR_BOLD) {
        return fs->bold;
    }
    return fs->regular;
};

static void next_fontset(void) {
    current_fontset ++;
    if (current_fontset >= sizeof(fontsets) / sizeof(FontSet))
        current_fontset = 0;
}


static uint8_t displaycolor(uint32_t col) {
    if (IS_TRUECOL(col)) {
        return (30 * ((col & 0xFF0000) >> 16) +
                59 * ((col & 0x00FF00) >> 8) +
                11 * (col & 0x0000FF)) / 100 / 16;
    }
    return colorscheme[col];
}

// input buffers must be 32-bit line aligned.
void IRAM_ATTR calculate_dirty_buffer(
        uint8_t* dst, uint8_t* delete_buffer,
        uint8_t* write_buffer, int lines, uint32_t* dirty_lines)
{
  uint32_t* p1 = (uint32_t*)delete_buffer;
  uint32_t* p2 = (uint32_t*)write_buffer;
  for (int l = 0; l < lines; l++) {
    for (int i = 0; i < EPD_WIDTH / 8; i++) {
      uint32_t c = ((*p1++) ^ (*p2++));
      dirty_lines[l] |= c;
      uint8_t d = 0;
      d = ((c & 0xF) > 0) << 0;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 1;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 2;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 3;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 4;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 5;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 6;
      c = c >> 4;
      d |= ((c & 0xF) > 0) << 7;
      (*dst++) = ~d;
    }
  }
}

static const DRAM_ATTR uint32_t mask_lut[256] = {
    0x00000000, 0x0000000f, 0x000000f0, 0x000000ff, 0x00000f00, 0x00000f0f,
    0x00000ff0, 0x00000fff, 0x0000f000, 0x0000f00f, 0x0000f0f0, 0x0000f0ff,
    0x0000ff00, 0x0000ff0f, 0x0000fff0, 0x0000ffff, 0x000f0000, 0x000f000f,
    0x000f00f0, 0x000f00ff, 0x000f0f00, 0x000f0f0f, 0x000f0ff0, 0x000f0fff,
    0x000ff000, 0x000ff00f, 0x000ff0f0, 0x000ff0ff, 0x000fff00, 0x000fff0f,
    0x000ffff0, 0x000fffff, 0x00f00000, 0x00f0000f, 0x00f000f0, 0x00f000ff,
    0x00f00f00, 0x00f00f0f, 0x00f00ff0, 0x00f00fff, 0x00f0f000, 0x00f0f00f,
    0x00f0f0f0, 0x00f0f0ff, 0x00f0ff00, 0x00f0ff0f, 0x00f0fff0, 0x00f0ffff,
    0x00ff0000, 0x00ff000f, 0x00ff00f0, 0x00ff00ff, 0x00ff0f00, 0x00ff0f0f,
    0x00ff0ff0, 0x00ff0fff, 0x00fff000, 0x00fff00f, 0x00fff0f0, 0x00fff0ff,
    0x00ffff00, 0x00ffff0f, 0x00fffff0, 0x00ffffff, 0x0f000000, 0x0f00000f,
    0x0f0000f0, 0x0f0000ff, 0x0f000f00, 0x0f000f0f, 0x0f000ff0, 0x0f000fff,
    0x0f00f000, 0x0f00f00f, 0x0f00f0f0, 0x0f00f0ff, 0x0f00ff00, 0x0f00ff0f,
    0x0f00fff0, 0x0f00ffff, 0x0f0f0000, 0x0f0f000f, 0x0f0f00f0, 0x0f0f00ff,
    0x0f0f0f00, 0x0f0f0f0f, 0x0f0f0ff0, 0x0f0f0fff, 0x0f0ff000, 0x0f0ff00f,
    0x0f0ff0f0, 0x0f0ff0ff, 0x0f0fff00, 0x0f0fff0f, 0x0f0ffff0, 0x0f0fffff,
    0x0ff00000, 0x0ff0000f, 0x0ff000f0, 0x0ff000ff, 0x0ff00f00, 0x0ff00f0f,
    0x0ff00ff0, 0x0ff00fff, 0x0ff0f000, 0x0ff0f00f, 0x0ff0f0f0, 0x0ff0f0ff,
    0x0ff0ff00, 0x0ff0ff0f, 0x0ff0fff0, 0x0ff0ffff, 0x0fff0000, 0x0fff000f,
    0x0fff00f0, 0x0fff00ff, 0x0fff0f00, 0x0fff0f0f, 0x0fff0ff0, 0x0fff0fff,
    0x0ffff000, 0x0ffff00f, 0x0ffff0f0, 0x0ffff0ff, 0x0fffff00, 0x0fffff0f,
    0x0ffffff0, 0x0fffffff, 0xf0000000, 0xf000000f, 0xf00000f0, 0xf00000ff,
    0xf0000f00, 0xf0000f0f, 0xf0000ff0, 0xf0000fff, 0xf000f000, 0xf000f00f,
    0xf000f0f0, 0xf000f0ff, 0xf000ff00, 0xf000ff0f, 0xf000fff0, 0xf000ffff,
    0xf00f0000, 0xf00f000f, 0xf00f00f0, 0xf00f00ff, 0xf00f0f00, 0xf00f0f0f,
    0xf00f0ff0, 0xf00f0fff, 0xf00ff000, 0xf00ff00f, 0xf00ff0f0, 0xf00ff0ff,
    0xf00fff00, 0xf00fff0f, 0xf00ffff0, 0xf00fffff, 0xf0f00000, 0xf0f0000f,
    0xf0f000f0, 0xf0f000ff, 0xf0f00f00, 0xf0f00f0f, 0xf0f00ff0, 0xf0f00fff,
    0xf0f0f000, 0xf0f0f00f, 0xf0f0f0f0, 0xf0f0f0ff, 0xf0f0ff00, 0xf0f0ff0f,
    0xf0f0fff0, 0xf0f0ffff, 0xf0ff0000, 0xf0ff000f, 0xf0ff00f0, 0xf0ff00ff,
    0xf0ff0f00, 0xf0ff0f0f, 0xf0ff0ff0, 0xf0ff0fff, 0xf0fff000, 0xf0fff00f,
    0xf0fff0f0, 0xf0fff0ff, 0xf0ffff00, 0xf0ffff0f, 0xf0fffff0, 0xf0ffffff,
    0xff000000, 0xff00000f, 0xff0000f0, 0xff0000ff, 0xff000f00, 0xff000f0f,
    0xff000ff0, 0xff000fff, 0xff00f000, 0xff00f00f, 0xff00f0f0, 0xff00f0ff,
    0xff00ff00, 0xff00ff0f, 0xff00fff0, 0xff00ffff, 0xff0f0000, 0xff0f000f,
    0xff0f00f0, 0xff0f00ff, 0xff0f0f00, 0xff0f0f0f, 0xff0f0ff0, 0xff0f0fff,
    0xff0ff000, 0xff0ff00f, 0xff0ff0f0, 0xff0ff0ff, 0xff0fff00, 0xff0fff0f,
    0xff0ffff0, 0xff0fffff, 0xfff00000, 0xfff0000f, 0xfff000f0, 0xfff000ff,
    0xfff00f00, 0xfff00f0f, 0xfff00ff0, 0xfff00fff, 0xfff0f000, 0xfff0f00f,
    0xfff0f0f0, 0xfff0f0ff, 0xfff0ff00, 0xfff0ff0f, 0xfff0fff0, 0xfff0ffff,
    0xffff0000, 0xffff000f, 0xffff00f0, 0xffff00ff, 0xffff0f00, 0xffff0f0f,
    0xffff0ff0, 0xffff0fff, 0xfffff000, 0xfffff00f, 0xfffff0f0, 0xfffff0ff,
    0xffffff00, 0xffffff0f, 0xfffffff0, 0xffffffff,
};

void IRAM_ATTR mask_buffer(uint8_t* mask, uint32_t* input, uint32_t* output, int lines) {
  for (int i=0; i < EPD_WIDTH / 8 * lines; i++) {
    output[i] = input[i] & mask_lut[0xFF ^ mask[i]];
  }
}

static void render_line() {
    while (true) {
        int line = -1;
        if (xQueueReceive(line_render_queue, &line, portMAX_DELAY) != pdPASS) {
            die("term: failed to receive line to render");
        }
        Glyph* glyphs = term.line[line];
        int min_y_px = EPD_HEIGHT + 1;
        int max_y_px = -1;

        const EpdFont* font = get_font(glyphs[0]);
        int line_y = pixel_start_y + font->advance_y * line - font->ascender;
        int line_height = font->ascender - font->descender;

        memset(render_fb_front + line_y * EPD_WIDTH / 2, 255, EPD_WIDTH / 2 * line_height);

        char data[4 * term.col + 1];
        int idx = 0;
        int px_x = pixel_start_x;

        Glyph cursor_char;

        if (term.c.y == line) {
            Glyph* cchar = &glyphs[term.c.x];
            cursor_char = *cchar;
            if (cchar->u == ' ') {
                cchar->u = '_';
            } else {
                cchar->bg = 3;
            }
        }

        while (idx < term.col) {
          int data_pos = 0;
          Glyph c = glyphs[idx];
          while (idx < term.col && (
                  glyphs[idx].fg == c.fg
                  && glyphs[idx].bg == c.bg
                  && glyphs[idx].mode == c.mode)) {
            c = glyphs[idx];
            data_pos += utf8encode(c.u, data + data_pos);
            idx++;
          }
          // null-terminate
          data[data_pos] = 0;

          EpdFontProperties fprops = epd_font_properties_default();
          fprops.fg_color = displaycolor(c.fg);
          fprops.bg_color = displaycolor(c.bg);
          fprops.fallback_glyph = fallback_glyph;
          fprops.flags |= (c.bg != defaultbg ? EPD_DRAW_BACKGROUND : 0);
          const EpdFont* f = get_font(c);
          int px_y = pixel_start_y + f->advance_y * line;
          min_y_px = MIN(min_y_px, px_y - f->ascender);
          max_y_px = MAX(min_y_px, px_y - f->descender);

          epd_write_string(f, data, &px_x, &px_y, render_fb_front, &fprops);
        }

        if (term.c.y == line) {
            glyphs[term.c.x] = cursor_char;
        }

        int height = max_y_px - min_y_px;
        if (height > 0) {
          height = MIN(height, EPD_HEIGHT - min_y_px);
          int y_offset = EPD_WIDTH / 2 * min_y_px;
          uint8_t* start_ptr = render_fb_front + y_offset;

          uint8_t* mask_start = render_mask + (min_y_px * EPD_WIDTH / 8);

          calculate_dirty_buffer(
                  mask_start,
                  render_fb_back + y_offset,
                  start_ptr,
                  height,
                  &line_dirtyness[min_y_px]
          );
          mask_buffer(
                  mask_start,
                  (uint32_t*)start_ptr,
                  (uint32_t*)(render_masked_buf + y_offset),
                  height
          );
        }
        xSemaphoreGive(render_lines_done_smphr);
    }
}

static void draw_mask(EpdRect area, uint8_t* mask, bool* dirtyness) {
    uint64_t start = esp_timer_get_time();
    int temperature = 25;

    enum EpdDrawError err = epd_draw_base(epd_full_screen(), mask, area, MODE_PACKING_8PPB | MODE_EPDIY_MONOCHROME | PREVIOUSLY_WHITE, temperature, dirtyness, EPD_BUILTIN_WAVEFORM);
    uint64_t time_ms = (esp_timer_get_time() - start) / 1000;
    // The particles must be given ~20ms to follow the applied charge.
    if (time_ms < MINIMUM_FRAME_TIME) {
      vTaskDelay(MINIMUM_FRAME_TIME - time_ms);
    }
}

const uint8_t nibble_inversion_lut[256] = {
0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8,
0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0,
0xef, 0xee, 0xed, 0xec, 0xeb, 0xea, 0xe9, 0xe8,
0xe7, 0xe6, 0xe5, 0xe4, 0xe3, 0xe2, 0xe1, 0xe0,
0xdf, 0xde, 0xdd, 0xdc, 0xdb, 0xda, 0xd9, 0xd8,
0xd7, 0xd6, 0xd5, 0xd4, 0xd3, 0xd2, 0xd1, 0xd0,
0xcf, 0xce, 0xcd, 0xcc, 0xcb, 0xca, 0xc9, 0xc8,
0xc7, 0xc6, 0xc5, 0xc4, 0xc3, 0xc2, 0xc1, 0xc0,
0xbf, 0xbe, 0xbd, 0xbc, 0xbb, 0xba, 0xb9, 0xb8,
0xb7, 0xb6, 0xb5, 0xb4, 0xb3, 0xb2, 0xb1, 0xb0,
0xaf, 0xae, 0xad, 0xac, 0xab, 0xaa, 0xa9, 0xa8,
0xa7, 0xa6, 0xa5, 0xa4, 0xa3, 0xa2, 0xa1, 0xa0,
0x9f, 0x9e, 0x9d, 0x9c, 0x9b, 0x9a, 0x99, 0x98,
0x97, 0x96, 0x95, 0x94, 0x93, 0x92, 0x91, 0x90,
0x8f, 0x8e, 0x8d, 0x8c, 0x8b, 0x8a, 0x89, 0x88,
0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x80,
0x7f, 0x7e, 0x7d, 0x7c, 0x7b, 0x7a, 0x79, 0x78,
0x77, 0x76, 0x75, 0x74, 0x73, 0x72, 0x71, 0x70,
0x6f, 0x6e, 0x6d, 0x6c, 0x6b, 0x6a, 0x69, 0x68,
0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61, 0x60,
0x5f, 0x5e, 0x5d, 0x5c, 0x5b, 0x5a, 0x59, 0x58,
0x57, 0x56, 0x55, 0x54, 0x53, 0x52, 0x51, 0x50,
0x4f, 0x4e, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48,
0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x40,
0x3f, 0x3e, 0x3d, 0x3c, 0x3b, 0x3a, 0x39, 0x38,
0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
0x2f, 0x2e, 0x2d, 0x2c, 0x2b, 0x2a, 0x29, 0x28,
0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18,
0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10,
0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};

static void invert_framebuffer(uint8_t* fb) {
    for (int i=0; i < EPD_WIDTH / 2 * EPD_HEIGHT; i++) {
        fb[i] = nibble_inversion_lut[fb[i]];
    }
}

static void full_refresh() {
  if (!screen_tainted) {
	return;
  }

  int temperature = 25;
  ESP_LOGI("term", "epd clear.");
  invert_framebuffer(render_fb_front);
  epd_poweron();
  epd_draw_base(epd_full_screen(), render_fb_front, epd_full_screen(), MODE_PACKING_2PPB | MODE_EPDIY_BLACK_TO_GL16 | PREVIOUSLY_BLACK, temperature, NULL, EPD_BUILTIN_WAVEFORM);
  epd_clear_area_cycles(epd_full_screen(), clear_cycles, clear_cycle_length);
  epd_poweroff();
  tfulldirt();
  screen_tainted = 0;
  memset(render_fb_front, 255, EPD_WIDTH / 2 * EPD_HEIGHT);
  memset(render_fb_back, 255, EPD_WIDTH / 2 * EPD_HEIGHT);
  memset(render_mask, 255, EPD_WIDTH / 8 * EPD_HEIGHT);
}


void epd_render(void) {

  /// use a fixed temperature of 25°C.
  int temperature = 25;

  memset(line_dirtyness, 0, sizeof(line_dirtyness));

  bool is_full_clear = false;

  int drawn_lines = 0;

  // run two render tasks in parallel
  for (int y = 0; y < term.row; y++) {
    if (!term.dirty[y] && !is_full_clear) {
      continue;
    }

    if (xQueueSend(line_render_queue, (void*)&y, 10) != pdPASS) {
        die("term: failed to post to line queue!");
        return;
    }
    drawn_lines++;
    term.dirty[y] = 0;
  }

  // wait for rendering completion
  for (int li = 0; li < drawn_lines; li++) {
    xSemaphoreTake(render_lines_done_smphr, portMAX_DELAY);
  }

  if (drawn_lines) {
    screen_tainted = 1;

    epd_poweron();
    uint64_t t_poweron = esp_timer_get_time();

    bool boolean_line_dirtyness[EPD_HEIGHT];
    for (int i=0; i < EPD_HEIGHT; i++) {
      boolean_line_dirtyness[i] = line_dirtyness[i] > 0;
    }

    int min_y = 0;
    int max_y = EPD_HEIGHT - 1;
    int height = max_y - min_y;
    if (height > 0) {
      EpdRect area = {
        .x = 0,
        .y = min_y,
        .width = EPD_WIDTH,
        .height = height,
      };

      uint64_t time_since_poweron_ms = (esp_timer_get_time() - t_poweron) / 1000;
      // poweron takes ~10ms until all capacitors are charged
      if (time_since_poweron_ms < 10) {
        vTaskDelay(10 - time_since_poweron_ms);
      }

      draw_mask(area, render_mask, boolean_line_dirtyness);
      draw_mask(area, render_mask, boolean_line_dirtyness);
      draw_mask(area, render_mask, boolean_line_dirtyness);
      draw_mask(area, render_mask, boolean_line_dirtyness);
      draw_mask(area, render_mask, boolean_line_dirtyness);

      enum EpdDrawError err = epd_draw_base(epd_full_screen(), render_masked_buf, epd_full_screen(), MODE_PACKING_2PPB | MODE_EPDIY_BLACK_TO_GL16 | PREVIOUSLY_BLACK, temperature, boolean_line_dirtyness, EPD_BUILTIN_WAVEFORM);
      epd_poweroff();

      if (err)
          ESP_LOGI("term", "draw err: %d", err);

      for (int i=0; i < EPD_HEIGHT; i++) {
        if (boolean_line_dirtyness[i]) {
          memcpy(
              render_fb_back + EPD_WIDTH / 2 * i,
              render_fb_front + EPD_WIDTH / 2 * i,
              EPD_WIDTH / 2
          );
        }
      }
    }
  }
}

void
draw(void)
{
    epd_render();
}

void
redraw(void)
{
	tfulldirt();
	draw();
}

void
clipboard_paste(void)
{
    if (clipboard) {
        ttywrite(clipboard, strlen(clipboard), 0);
    }
}
