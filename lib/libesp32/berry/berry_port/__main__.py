#!/usr/bin/env python3
"""
Berry Python port — CLI entry point.

Usage:
    python3 -m berry_port                    # REPL
    python3 -m berry_port script.be          # run file
    python3 -m berry_port -e "print(1+2)"   # run string
    python3 -m berry_port -i script.be       # run file then REPL
    python3 -m berry_port -c script.be       # compile to bytecode
    python3 -m berry_port -g -s script.be    # named globals + strict

Mirrors default/berry.c and src/be_repl.c from the C implementation.
"""

import sys
import os
import platform

from berry_port.berry import (
    BERRY_VERSION, BE_OK, BE_EXIT, BE_EXCEPTION,
    BE_MALLOC_FAIL, BE_SYNTAX_ERROR, BE_IO_ERROR,
    be_vm_new, be_vm_delete, be_loadbuffer, be_loadstring, be_pcall,
)
from berry_port.be_api import (
    be_dumpexcept, be_dumpvalue, be_writestring, be_writenewline,
    be_tostring, be_toindex, be_isnil, be_pop, be_remove,
    be_absindex, be_pushfstring, be_pushstring, be_strconcat,
    be_getexcept, be_loadmode, be_newobject, be_data_push,
    be_setglobal,
)
from berry_port.be_exec import be_savecode
from berry_port.be_var import comp_set_named_gbl, comp_set_strict
from berry_port.be_module import be_module_path_set

# /*
# ** #if BE_DEBUG
# ** #define FULL_VERSION "Berry " BERRY_VERSION " (debug)"
# ** #else
# ** #define FULL_VERSION "Berry " BERRY_VERSION
# ** #endif
# */
FULL_VERSION = f"Berry {BERRY_VERSION} (Python port)"

# /*
# ** #if defined(_WIN32)
# ** #define PATH_SEPARATOR ";"
# ** #else
# ** #define PATH_SEPARATOR ":"
# ** #endif
# */
PATH_SEPARATOR = ";" if sys.platform == "win32" else ":"

# /*
# ** #define repl_prelude                                                \
# **     FULL_VERSION " (build in " __DATE__ ", " __TIME__ ")\n"         \
# **     "[" COMPILER "] on " OS_NAME " (default)\n"
# */
def _repl_prelude():
    os_name = platform.system() or "Unknown OS"
    py_ver = f"Python {sys.version.split()[0]}"
    return f"{FULL_VERSION}\n[{py_ver}] on {os_name} (default)\n"

# /*
# ** #define help_information                                            \
# **     "Usage: berry [options] [script [args]]\n"                      \
# **     "Avilable options are:\n"                                       \
# **     "  -i        enter interactive mode after executing 'file'\n"   \
# **     "  -l        all variables in 'file' are parsed as local\n"     \
# **     "  -e        load 'script' source string and execute\n"         \
# **     "  -m <path> custom module search path(s) separated by '" PATH_SEPARATOR "'\n"\
# **     "  -c <file> compile script 'file' to bytecode file\n"          \
# **     "  -o <file> save bytecode to 'file'\n"                         \
# **     "  -g        force named globals in VM\n"                       \
# **     "  -s        force Berry compiler in strict mode\n"             \
# **     "  -v        show version information\n"                        \
# **     "  -h        show help information\n\n"                         \
# **     "For more information, please see:\n"                           \
# **     "  <https://github.com/skiars/berry>.\n"
# */
HELP_INFORMATION = (
    "Usage: python3 -m berry_port [options] [script [args]]\n"
    "Available options are:\n"
    "  -i        enter interactive mode after executing 'file'\n"
    "  -l        all variables in 'file' are parsed as local\n"
    "  -e        load 'script' source string and execute\n"
    f"  -m <path> custom module search path(s) separated by '{PATH_SEPARATOR}'\n"
    "  -c <file> compile script 'file' to bytecode file\n"
    "  -o <file> save bytecode to 'file'\n"
    "  -g        force named globals in VM\n"
    "  -s        force Berry compiler in strict mode\n"
    "  -v        show version information\n"
    "  -h        show help information\n\n"
    "For more information, please see:\n"
    "  <https://github.com/skiars/berry>.\n"
)

# /*
# ** #define arg_i       (1 << 0)
# ** #define arg_c       (1 << 1)
# ** #define arg_o       (1 << 2)
# ** #define arg_l       (1 << 3)
# ** #define arg_h       (1 << 4)
# ** #define arg_v       (1 << 5)
# ** #define arg_e       (1 << 6)
# ** #define arg_g       (1 << 7)
# ** #define arg_s       (1 << 8)
# ** #define arg_err     (1 << 9)
# ** #define arg_m       (1 << 10)
# */
arg_i   = (1 << 0)
arg_c   = (1 << 1)
arg_o   = (1 << 2)
arg_l   = (1 << 3)
arg_h   = (1 << 4)
arg_v   = (1 << 5)
arg_e   = (1 << 6)
arg_g   = (1 << 7)
arg_s   = (1 << 8)
arg_err = (1 << 9)
arg_m   = (1 << 10)


# ── Option parsing (mirrors C arg_getopt / parse_arg) ──────────────────

# /*
# ** struct arg_opts {
# **     int idx;
# **     const char *pattern;
# **     const char *optarg;
# **     const char *errarg;
# **     const char *src;
# **     const char *dst;
# **     const char *modulepath;
# **     const char *execute;
# ** };
# */
class arg_opts:
    def __init__(self):
        self.idx = 0
        self.pattern = ""
        self.optarg = None
        self.errarg = None
        self.src = None
        self.dst = None
        self.modulepath = None
        self.execute = None


# /*
# ** static int is_letter(int ch)
# ** {
# **     return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
# ** }
# */
def is_letter(ch):
    return ch.isalpha() if ch else False


# /*
# ** static const char* match_opt(const char *pattern, int ch)
# ** {
# **     int c = '\0';
# **     if (pattern) {
# **         while ((c = *pattern) != '\0' && c != ch) {
# **             c = *(++pattern);
# **             while (c != '\0' && !is_letter(c)) {
# **                 c = *(++pattern);
# **             }
# **         }
# **     }
# **     return c == ch ? pattern : NULL;
# ** }
# */
def match_opt(pattern, ch):
    """Match option character against pattern string.
    Pattern format: letters optionally followed by '?' meaning the option takes an argument.
    Returns (index_into_pattern, has_arg) or None.
    """
    if not pattern:
        return None
    i = 0
    while i < len(pattern):
        c = pattern[i]
        if c == ch:
            has_arg = (i + 1 < len(pattern) and pattern[i + 1] == '?')
            return (i, has_arg)
        # skip to next letter
        i += 1
        while i < len(pattern) and not is_letter(pattern[i]):
            i += 1
    return None


# /*
# ** static int arg_getopt(struct arg_opts *opt, int argc, char *argv[])
# ** {
# **     if (opt->idx < argc) {
# **         char *arg = argv[opt->idx];
# **         if (arg[0] == '-' && strlen(arg) == 2) {
# **             const char *res = match_opt(opt->pattern, arg[1]);
# **             if (++opt->idx < argc && res != NULL
# **                 && res[1] == '?' && *argv[opt->idx] != '-') {
# **                 opt->optarg = argv[opt->idx++];
# **                 return *res;
# **             }
# **             opt->optarg = NULL;
# **             opt->errarg = arg;
# **             return res != NULL ? *res : '?';
# **         }
# **     }
# **     return 0;
# ** }
# */
def arg_getopt(opt, argv):
    """Read one option from argv starting at opt.idx. Returns option char or 0."""
    argc = len(argv)
    if opt.idx < argc:
        arg = argv[opt.idx]
        if len(arg) == 2 and arg[0] == '-' and is_letter(arg[1]):
            ch = arg[1]
            res = match_opt(opt.pattern, ch)
            opt.idx += 1
            if (opt.idx < argc and res is not None
                    and res[1]  # has_arg
                    and not argv[opt.idx].startswith('-')):
                opt.optarg = argv[opt.idx]
                opt.idx += 1
                return ch
            opt.optarg = None
            opt.errarg = arg
            return ch if res is not None else '?'
    return '\0'


# /*
# ** static int parse_arg(struct arg_opts *opt, int argc, char *argv[])
# ** {
# **     int ch, args = 0;
# **     opt->idx = 1;
# **     while ((ch = arg_getopt(opt, argc, argv)) != '\0') {
# **         switch (ch) {
# **         case 'h': args |= arg_h; break;
# **         case 'v': args |= arg_v; break;
# **         case 'i': args |= arg_i; break;
# **         case 'l': args |= arg_l; break;
# **         case 'g': args |= arg_g; break;
# **         case 's': args |= arg_s; break;
# **         case 'e':
# **             args |= arg_e;
# **             opt->execute = opt->optarg;
# **             break;
# **         case 'm':
# **             args |= arg_m;
# **             opt->modulepath = opt->optarg;
# **             break;
# **         case '?': return args | arg_err;
# **         case 'c':
# **             args |= arg_c;
# **             opt->src = opt->optarg;
# **             break;
# **         case 'o':
# **             args |= arg_o;
# **             opt->dst = opt->optarg;
# **             break;
# **         default:
# **             break;
# **         }
# **     }
# **     return args;
# ** }
# */
def parse_arg(opt, argv):
    """Parse all options from argv. Returns bitmask of enabled options."""
    args = 0
    opt.idx = 1
    while True:
        ch = arg_getopt(opt, argv)
        if ch == '\0':
            break
        if ch == 'h':
            args |= arg_h
        elif ch == 'v':
            args |= arg_v
        elif ch == 'i':
            args |= arg_i
        elif ch == 'l':
            args |= arg_l
        elif ch == 'g':
            args |= arg_g
        elif ch == 's':
            args |= arg_s
        elif ch == 'e':
            args |= arg_e
            opt.execute = opt.optarg
        elif ch == 'm':
            args |= arg_m
            opt.modulepath = opt.optarg
        elif ch == '?':
            return args | arg_err
        elif ch == 'c':
            args |= arg_c
            opt.src = opt.optarg
        elif ch == 'o':
            args |= arg_o
            opt.dst = opt.optarg
    return args


# ── push_args ───────────────────────────────────────────────────────────

# /*
# ** static void push_args(bvm *vm, int argc, char *argv[])
# ** {
# **     be_newobject(vm, "list");
# **     while (argc--) {
# **         be_pushstring(vm, *argv++);
# **         be_data_push(vm, -2);
# **         be_pop(vm, 1);
# **     }
# **     be_pop(vm, 1);
# **     be_setglobal(vm, "_argv");
# **     be_pop(vm, 1);
# ** }
# */
def push_args(vm, argv):
    """Create Berry list from argv and set as _argv global."""
    be_newobject(vm, "list")
    for a in argv:
        be_pushstring(vm, a)
        be_data_push(vm, -2)
        be_pop(vm, 1)
    be_pop(vm, 1)
    be_setglobal(vm, "_argv")
    be_pop(vm, 1)


# ── handle_result ───────────────────────────────────────────────────────

# /*
# ** static int handle_result(bvm *vm, int res)
# ** {
# **     switch (res) {
# **     case BE_OK:
# **         return 0;
# **     case BE_EXCEPTION:
# **         be_dumpexcept(vm);
# **         return 1;
# **     case BE_EXIT:
# **         return be_toindex(vm, -1);
# **     case BE_IO_ERROR:
# **         be_writestring("error: ");
# **         be_writestring(be_tostring(vm, -1));
# **         be_writenewline();
# **         return -2;
# **     case BE_MALLOC_FAIL:
# **         be_writestring("error: memory allocation failed.\n");
# **         return -1;
# **     default:
# **         return 2;
# **     }
# ** }
# */
def handle_result(vm, res):
    if res == BE_OK:
        return 0
    elif res == BE_EXCEPTION:
        be_dumpexcept(vm)
        return 1
    elif res == BE_EXIT:
        return be_toindex(vm, -1)
    elif res == BE_IO_ERROR:
        be_writestring("error: ")
        be_writestring(be_tostring(vm, -1))
        be_writenewline()
        return -2
    elif res == BE_MALLOC_FAIL:
        be_writestring("error: memory allocation failed.\n")
        return -1
    else:
        return 2


# ── build_file ──────────────────────────────────────────────────────────

# /*
# ** static int build_file(bvm *vm, const char *dst, const char *src, int args)
# ** {
# **     int res = be_loadmode(vm, src, args & arg_l);
# **     if (res == BE_OK) {
# **         if (!dst) dst = "a.out";
# **         res = be_savecode(vm, dst);
# **     }
# **     return handle_result(vm, res);
# ** }
# */
def build_file(vm, dst, src, args):
    """Compile source file to bytecode file."""
    res = be_loadmode(vm, src, args & arg_l)
    if res == BE_OK:
        if not dst:
            dst = "a.out"
        res = be_savecode(vm, dst)
    return handle_result(vm, res)


# ── REPL ────────────────────────────────────────────────────────────────

# /*
# ** /* portable readline function package */
# ** static char* get_line(const char *prompt)
# ** {
# **     static char buffer[1000];
# **     fputs(prompt, stdout);
# **     fflush(stdout);
# **     if (be_readstring(buffer, sizeof(buffer))) {
# **         buffer[strlen(buffer) - 1] = '\0';
# **         return buffer;
# **     }
# **     return NULL;
# ** }
# */
def _get_line(prompt):
    """Read a line from stdin with prompt. Returns None on EOF."""
    try:
        return input(prompt)
    except EOFError:
        return None
    except KeyboardInterrupt:
        print()  # newline after ^C
        return ""


# /*
# ** static int try_return(bvm *vm, const char *line)
# ** {
# **     int res, idx;
# **     line = be_pushfstring(vm, "return (%s)", line);
# **     idx = be_absindex(vm, -1);
# **     res = be_loadbuffer(vm, "stdin", line, strlen(line));
# **     be_remove(vm, idx);
# **     return res;
# ** }
# */
def _try_return(vm, line):
    """Try compiling 'return (line)' — allows expressions to print values."""
    src = be_pushfstring(vm, "return (%s)", line)
    idx = be_absindex(vm, -1)
    res = be_loadbuffer(vm, "stdin", src, len(src))
    be_remove(vm, idx)
    return res


# /*
# ** static bbool is_multline(bvm *vm)
# ** {
# **     const char *msg = be_tostring(vm, -1);
# **     size_t len = strlen(msg);
# **     if (len > 5) {
# **         return !strcmp(msg + len - 5, "'EOS'");
# **     }
# **     return bfalse;
# ** }
# */
def _is_multiline(vm):
    """Check if the error message ends with 'EOS' (incomplete input)."""
    msg = be_tostring(vm, -1)
    if msg is not None and len(msg) > 5:
        return msg.endswith("'EOS'")
    return False


# /*
# ** static int compile(bvm *vm, char *line, breadline getl, bfreeline freel)
# ** {
# **     int res = try_return(vm, line);
# **     if (be_getexcept(vm, res) == BE_SYNTAX_ERROR) {
# **         be_pop(vm, 2);
# **         be_pushstring(vm, line);
# **         safecall(freel, line);
# **         for (;;) {
# **             const char *src = be_tostring(vm, -1);
# **             int idx = be_absindex(vm, -1);
# **             res = be_loadbuffer(vm, "stdin", src, strlen(src));
# **             if (!res || !is_multline(vm)) {
# **                 be_remove(vm, idx);
# **                 return res;
# **             }
# **             be_pop(vm, 2);
# **             line = getl(">> ");
# **             be_pushfstring(vm, "\n%s", line);
# **             safecall(freel, line);
# **             be_strconcat(vm, -2);
# **             be_pop(vm, 1);
# **         }
# **     } else {
# **         safecall(freel, line);
# **     }
# **     return res;
# ** }
# */
def _compile(vm, line):
    """Compile a REPL line, handling multi-line input."""
    # Normalize Python Unicode to UTF-8 byte representation (as Latin-1)
    # to match C Berry where REPL input arrives as raw UTF-8 bytes.
    line = line.encode('utf-8').decode('latin-1')
    res = _try_return(vm, line)
    if be_getexcept(vm, res) == BE_SYNTAX_ERROR:
        be_pop(vm, 2)  # pop exception values
        be_pushstring(vm, line)
        while True:
            src = be_tostring(vm, -1)
            idx = be_absindex(vm, -1)
            res = be_loadbuffer(vm, "stdin", src, len(src))
            if not res or not _is_multiline(vm):
                be_remove(vm, idx)
                return res
            be_pop(vm, 2)  # pop exception values
            next_line = _get_line(">> ")
            if next_line is None:
                be_remove(vm, idx)
                return res
            next_line = next_line.encode('utf-8').decode('latin-1')
            be_pushfstring(vm, "\n%s", next_line)
            be_strconcat(vm, -2)
            be_pop(vm, 1)  # pop new line
    return res


# /*
# ** static int call_script(bvm *vm)
# ** {
# **     int res = be_pcall(vm, 0);
# **     switch (res) {
# **     case BE_OK:
# **         if (!be_isnil(vm, -1)) {
# **             be_dumpvalue(vm, -1);
# **         }
# **         be_pop(vm, 1);
# **         break;
# **     case BE_EXCEPTION:
# **         be_dumpexcept(vm);
# **         be_pop(vm, 1);
# **         break;
# **     default:
# **         return res;
# **     }
# **     return 0;
# ** }
# */
def _call_script(vm):
    """Execute compiled script and handle result."""
    res = be_pcall(vm, 0)
    if res == BE_OK:
        if not be_isnil(vm, -1):
            be_dumpvalue(vm, -1)
        be_pop(vm, 1)
        return 0
    elif res == BE_EXCEPTION:
        be_dumpexcept(vm)
        be_pop(vm, 1)
        return 0
    else:
        return res


# /*
# ** BERRY_API int be_repl(bvm *vm, breadline getline, bfreeline freeline)
# ** {
# **     char *line;
# **     be_assert(getline != NULL);
# **     while ((line = getline("> ")) != NULL) {
# **         int res = compile(vm, line, getline, freeline);
# **         if (res == BE_MALLOC_FAIL)
# **             return BE_MALLOC_FAIL;
# **         if (res) {
# **             be_dumpexcept(vm);
# **         } else {
# **             res = call_script(vm);
# **             if (res) {
# **                 return res == BE_EXIT ? be_toindex(vm, -1) : res;
# **             }
# **         }
# **     }
# **     be_writenewline();
# **     return 0;
# ** }
# */
def repl(vm):
    """Interactive REPL loop. Mirrors be_repl() from src/be_repl.c."""
    while True:
        line = _get_line("> ")
        if line is None:
            break
        if not line.strip():
            continue
        res = _compile(vm, line)
        if res == BE_MALLOC_FAIL:
            return BE_MALLOC_FAIL
        if res:
            be_dumpexcept(vm)
        else:
            r = _call_script(vm)
            if r:
                return be_toindex(vm, -1) if r == BE_EXIT else r
    be_writenewline()
    return 0


# ── doscript / load_script ──────────────────────────────────────────────

# /*
# ** static int doscript(bvm *vm, const char *name, int args)
# ** {
# **     int res = be_loadmode(vm, name, args & arg_l);
# **     if (res == BE_OK) {
# **         res = be_pcall(vm, 0);
# **     }
# **     return handle_result(vm, res);
# ** }
# */
def doscript(vm, name, args):
    """Execute a script source or file and output a result or error."""
    res = be_loadmode(vm, name, args & arg_l)
    if res == BE_OK:
        res = be_pcall(vm, 0)
    return handle_result(vm, res)


# /*
# ** static int load_script(bvm *vm, int argc, char *argv[], int args, const char * script)
# ** {
# **     int res = 0;
# **     int repl_mode = args & arg_i || (args == 0 && argc == 0);
# **     if (repl_mode) {
# **         be_writestring(repl_prelude);
# **     }
# **     if (script) {
# **         res = be_loadstring(vm, script);
# **         if (res == BE_OK) {
# **             res = be_pcall(vm, 0);
# **         }
# **         res = handle_result(vm, res);
# **     }
# **     if (res == BE_OK && argc > 0) {
# **         res = doscript(vm, argv[0], args);
# **     }
# **     if (res == BE_OK && repl_mode) {
# **         res = be_repl(vm, get_line, free_line);
# **         if (res == -BE_MALLOC_FAIL) {
# **             be_writestring("error: memory allocation failed.\n");
# **         }
# **     }
# **     return res;
# ** }
# */
def load_script(vm, remaining_argv, args, script):
    """Load a Berry script string or file and execute."""
    res = 0
    repl_mode = bool(args & arg_i) or (args == 0 and len(remaining_argv) == 0)
    if repl_mode:
        be_writestring(_repl_prelude())
    # compile -e script string
    if script:
        res = be_loadstring(vm, script)
        if res == BE_OK:
            res = be_pcall(vm, 0)
        res = handle_result(vm, res)
    # run script file
    if res == 0 and len(remaining_argv) > 0:
        res = doscript(vm, remaining_argv[0], args)
    # enter REPL
    if res == 0 and repl_mode:
        res = repl(vm)
        if res == -BE_MALLOC_FAIL:
            be_writestring("error: memory allocation failed.\n")
    return res


# ── berry_custom_paths ──────────────────────────────────────────────────

# /*
# ** static void berry_custom_paths(bvm *vm, const char *modulepath)
# ** {
# **     const char delim[] = PATH_SEPARATOR;
# **     char *copy = malloc(strlen(modulepath) + 1);
# **     strcpy(copy, modulepath);
# **     char *ptr = strtok(copy, delim);
# **     while (ptr != NULL) {
# **         be_module_path_set(vm, ptr);
# **         ptr = strtok(NULL, delim);
# **     }
# **     free(copy);
# ** }
# */
def berry_custom_paths(vm, modulepath):
    """Add custom module search paths (separated by PATH_SEPARATOR)."""
    for p in modulepath.split(PATH_SEPARATOR):
        if p:
            be_module_path_set(vm, p)


# ── analysis_args (main entry) ──────────────────────────────────────────

# /*
# ** static int analysis_args(bvm *vm, int argc, char *argv[])
# ** {
# **     int args = 0;
# **     struct arg_opts opt = { 0 };
# **     opt.pattern = "m?vhile?gsc?o?";
# **     args = parse_arg(&opt, argc, argv);
# **     argc -= opt.idx;
# **     argv += opt.idx;
# **     if (args & arg_err) {
# **         be_writestring(be_pushfstring(vm,
# **             "error: missing argument to '%s'\n", opt.errarg));
# **         be_pop(vm, 1);
# **         return -1;
# **     }
# **     if (args & arg_m) {
# **         berry_custom_paths(vm, opt.modulepath);
# **         args &= ~arg_m;
# **     }
# **     if (args & arg_g) {
# **         comp_set_named_gbl(vm);
# **         args &= ~arg_g;
# **     }
# **     if (args & arg_s) {
# **         comp_set_strict(vm);
# **         args &= ~arg_s;
# **     }
# **     if (args & arg_v) {
# **         be_writestring(FULL_VERSION "\n");
# **     }
# **     if (args & arg_h) {
# **         be_writestring(help_information);
# **     }
# **     push_args(vm, argc, argv);
# **     if (args & (arg_c | arg_o)) {
# **         if (!opt.src && argc > 0) {
# **             opt.src = *argv;
# **         }
# **         return build_file(vm, opt.dst, opt.src, args);
# **     }
# **     return load_script(vm, argc, argv, args, opt.execute);
# ** }
# */
def analysis_args(vm, argv):
    """Parse arguments and dispatch to the appropriate action."""
    opt = arg_opts()
    opt.pattern = "m?vhile?gsc?o?"
    args = parse_arg(opt, argv)
    remaining = argv[opt.idx:]

    if args & arg_err:
        be_writestring(be_pushfstring(vm,
            "error: missing argument to '%s'\n", opt.errarg))
        be_pop(vm, 1)
        return -1

    if args & arg_m:
        berry_custom_paths(vm, opt.modulepath)
        args &= ~arg_m

    if args & arg_g:
        comp_set_named_gbl(vm)
        args &= ~arg_g

    if args & arg_s:
        comp_set_strict(vm)
        args &= ~arg_s

    if args & arg_v:
        be_writestring(FULL_VERSION + "\n")

    if args & arg_h:
        be_writestring(HELP_INFORMATION)

    push_args(vm, remaining)

    if args & (arg_c | arg_o):
        if not opt.src and len(remaining) > 0:
            opt.src = remaining[0]
        return build_file(vm, opt.dst, opt.src, args)

    return load_script(vm, remaining, args, opt.execute)


# ── main ────────────────────────────────────────────────────────────────

# /*
# ** int main(int argc, char *argv[])
# ** {
# **     int res;
# **     bvm *vm = be_vm_new();
# **     res = analysis_args(vm, argc, argv);
# **     be_vm_delete(vm);
# **     return res;
# ** }
# */
def main():
    vm = be_vm_new()
    try:
        res = analysis_args(vm, sys.argv)
    except KeyboardInterrupt:
        print()
        res = 0
    finally:
        be_vm_delete(vm)
    return res


if __name__ == "__main__":
    sys.exit(main())
