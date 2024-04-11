/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef TASMOTA       // only when compiling stand-along

#include "berry.h"
#include "be_repl.h"
#include "be_vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "be_mapping.h"

/* using GNU/readline library */
#if defined(USE_READLINE_LIB)
    #include <readline/readline.h>
    #include <readline/history.h>
#endif

/* detect operating system name */
#if defined(__linux)
    #define OS_NAME   "Linux"
#elif defined(__unix)
    #define OS_NAME   "Unix"
#elif defined(__APPLE__)
    #define OS_NAME   "Darwin"
#elif defined(_WIN32)
    #define OS_NAME   "Windows"
#else
    #define OS_NAME   "Unknown OS"
#endif

/* detect compiler name and version */
#if defined(__clang__)
    #define COMPILER  "clang " __clang_version__
#elif defined(__GNUC__)
    #define COMPILER  "GCC " __VERSION__
#elif defined(_MSC_VER)
    #define COMPILER  "MSVC"
#elif defined(__CC_ARM)
    #define COMPILER  "ARMCC"
#elif defined(__ICCARM__)
    #define COMPILER  "IAR"
#else
    #define COMPILER  "Unknown Compiler"
#endif

#if BE_DEBUG
#define FULL_VERSION "Berry " BERRY_VERSION " (debug)"
#else
#define FULL_VERSION "Berry " BERRY_VERSION
#endif

/* prompt message when REPL is loaded */
#define repl_prelude                                                \
    FULL_VERSION " (build in " __DATE__ ", " __TIME__ ")\n"         \
    "[" COMPILER "] on " OS_NAME " (default)\n"                     \

#if defined(_WIN32)
#define PATH_SEPARATOR ";"
#else
#define PATH_SEPARATOR ":"
#endif


/* command help information */
#define help_information                                            \
    "Usage: berry [options] [script [args]]\n"                      \
    "Avilable options are:\n"                                       \
    "  -i        enter interactive mode after executing 'file'\n"   \
    "  -l        all variables in 'file' are parsed as local\n"     \
    "  -e        load 'script' source string and execute\n"         \
    "  -m <path> custom module search path(s) separated by '" PATH_SEPARATOR "'\n"\
    "  -c <file> compile script 'file' to bytecode file\n"          \
    "  -o <file> save bytecode to 'file'\n"                         \
    "  -g        force named globals in VM\n"                       \
    "  -s        force Berry compiler in strict mode\n"             \
    "  -v        show version information\n"                        \
    "  -h        show help information\n\n"                         \
    "For more information, please see:\n"                           \
    "  <https://github.com/skiars/berry>.\n"

#define array_count(a) (sizeof(a) / sizeof((a)[0]))

#define arg_i       (1 << 0)
#define arg_c       (1 << 1)
#define arg_o       (1 << 2)
#define arg_l       (1 << 3)
#define arg_h       (1 << 4)
#define arg_v       (1 << 5)
#define arg_e       (1 << 6)
#define arg_g       (1 << 7)
#define arg_s       (1 << 8)
#define arg_err     (1 << 9)
#define arg_m       (1 << 10)

struct arg_opts {
    int idx;
    const char *pattern;
    const char *optarg;
    const char *errarg;
    const char *src;
    const char *dst;
    const char *modulepath;
};

/* check if the character is a letter */
static int is_letter(int ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

/* matching options
 * pattern: pattern string, the set of vaild options
 * ch: option character to be matched
 * */
static const char* match_opt(const char *pattern, int ch)
{
    int c = '\0';
    if (pattern) {
        while ((c = *pattern) != '\0' && c != ch) {
            c = *(++pattern);
            while (c != '\0' && !is_letter(c)) {
                c = *(++pattern); /* skip characters that are not letters */
            }
        }
    }
    return c == ch ? pattern : NULL;
}

/* read an option from the arguments
 * opt: option match state
 * argc: the number of arguments
 * argv: the arguments list
 * */
static int arg_getopt(struct arg_opts *opt, int argc, char *argv[])
{
    if (opt->idx < argc) {
        char *arg = argv[opt->idx];
        if (arg[0] == '-' && strlen(arg) == 2) {
            const char *res = match_opt(opt->pattern, arg[1]);
            /* the '?' indicates an optional argument after the option */
            if (++opt->idx < argc && res != NULL
                && res[1] == '?' && *argv[opt->idx] != '-') {
                opt->optarg = argv[opt->idx++]; /* save the argument */
                return *res;
            }
            opt->optarg = NULL;
            opt->errarg = arg;
            return res != NULL ? *res : '?';
        }
    }
    return 0;
}

/* portable readline function package */
static char* get_line(const char *prompt)
{
#if defined(USE_READLINE_LIB)
    char *line = readline(prompt);
    if (line && strlen(line)) {
        add_history(line);
    }
    return line;
#else
    static char buffer[1000];
    fputs(prompt, stdout);
    fflush(stdout);
    if (be_readstring(buffer, sizeof(buffer))) {
        buffer[strlen(buffer) - 1] = '\0';
        return buffer;
    }
    return NULL;
#endif
}

static void free_line(char *ptr)
{
#if defined(USE_READLINE_LIB)
    free(ptr);
#else
    (void)ptr;
#endif
}

static int handle_result(bvm *vm, int res)
{
    switch (res) {
    case BE_OK: /* everything is OK */
        return 0;
    case BE_EXCEPTION: /* uncatched exception */
        be_dumpexcept(vm);
        return 1;
    case BE_EXIT: /* return exit code */
        return be_toindex(vm, -1);
    case BE_IO_ERROR:
        be_writestring("error: "); 
        be_writestring(be_tostring(vm, -1));
        be_writenewline();
        return -2;
    case BE_MALLOC_FAIL:
        be_writestring("error: memory allocation failed.\n");
        return -1;
    default: /* unkonw result */
        return 2;
    }
}

/* execute a script source or file and output a result or error */
static int doscript(bvm *vm, const char *name, int args)
{
    /* load string, bytecode file or compile script file */
    int res = args & arg_e ? /* check script source string */
        be_loadstring(vm, name) : be_loadmode(vm, name, args & arg_l);
    if (res == BE_OK) { /* parsing succeeded */
        res = be_pcall(vm, 0); /* execute */
    }
    return handle_result(vm, res);
}

/* load a Berry script string or file and execute
 * args: the enabled options mask
 * */
static int load_script(bvm *vm, int argc, char *argv[], int args)
{
    int res = 0;
    int repl_mode = args & arg_i || (args == 0 && argc == 0);
    if (repl_mode) { /* enter the REPL mode after executing the script file */
        be_writestring(repl_prelude);
    }
    if (argc > 0) { /* check file path or source string argument */
        res = doscript(vm, argv[0], args);
    }
    if (repl_mode) { /* enter the REPL mode */
        res = be_repl(vm, get_line, free_line);
        if (res == -BE_MALLOC_FAIL) {
            be_writestring("error: memory allocation failed.\n");
        }
    }
    return res;
}

/* compile the source code to a bytecode file */
static int build_file(bvm *vm, const char *dst, const char *src, int args)
{
    int res = be_loadmode(vm, src, args & arg_l); /* compile script file */
    if (res == BE_OK) {
        if (!dst) dst = "a.out"; /* the default output file name */
        res = be_savecode(vm, dst); /* save bytecode file */
    }
    return handle_result(vm, res);
}

static int parse_arg(struct arg_opts *opt, int argc, char *argv[])
{
    int ch, args = 0;
    opt->idx = 1;
    while ((ch = arg_getopt(opt, argc, argv)) != '\0') {
        switch (ch) {
        case 'h': args |= arg_h; break;
        case 'v': args |= arg_v; break;
        case 'i': args |= arg_i; break;
        case 'l': args |= arg_l; break;
        case 'e': args |= arg_e; break;
        case 'g': args |= arg_g; break;
        case 's': args |= arg_s; break;
        case 'm':
            args |= arg_m;
            opt->modulepath = opt->optarg;
            break;
        case '?': return args | arg_err;
        case 'c':
            args |= arg_c;
            opt->src = opt->optarg;
            break;
        case 'o':
            args |= arg_o;
            opt->dst = opt->optarg;
            break;
        default:
            break;
        }
    }
    return args;
}

static void push_args(bvm *vm, int argc, char *argv[])
{
    be_newobject(vm, "list");
    while (argc--) {
        be_pushstring(vm, *argv++);
        be_data_push(vm, -2);
        be_pop(vm, 1);
    }
    be_pop(vm, 1);
    be_setglobal(vm, "_argv");
    be_pop(vm, 1);
}

#if defined(_WIN32)
#define BERRY_ROOT "\\Windows\\system32"
static const char *module_paths[] = {
    BERRY_ROOT "\\berry\\packages",
};
#else
#define BERRY_ROOT "/usr/local"
static const char *module_paths[] = {
    BERRY_ROOT "/lib/berry/packages",
};
#endif

static void berry_paths(bvm * vm)
{
    size_t i;
    for (i = 0; i < array_count(module_paths); ++i) {
        be_module_path_set(vm, module_paths[i]);
    }
}

static void berry_custom_paths(bvm *vm, const char *modulepath)
{
    const char delim[] = PATH_SEPARATOR;
    char *copy = malloc(strlen(modulepath) + 1);
    strcpy(copy, modulepath);
    char *ptr = strtok(copy, delim);

    while (ptr != NULL) {
        be_module_path_set(vm, ptr);
        ptr = strtok(NULL, delim);
    }
    free(copy);
}

/* 
 * command format: berry [options] [script [args]]
 *  command options:
 *   -i: enter interactive mode after executing 'script'
 *   -b: load code from bytecode file
 *   -e: load 'script' source and execute
 *   -m: specify custom module search path(s)
 * command format: berry options
 *  command options:
 *   -v: show version information
 *   -h: show help information
 * command format: berry option file [option file]
 *  command options:
 *   -c: compile script file to bytecode file
 *   -o: set the output file name
 * */
static int analysis_args(bvm *vm, int argc, char *argv[])
{
    int args = 0;
    struct arg_opts opt = { 0 };
    opt.pattern = "m?vhilegsc?o?";
    args = parse_arg(&opt, argc, argv);
    argc -= opt.idx;
    argv += opt.idx;
    if (args & arg_err) {
        be_writestring(be_pushfstring(vm,
            "error: missing argument to '%s'\n", opt.errarg));
        be_pop(vm, 1);
        return -1;
    }
    
    if (args & arg_m) {
        berry_custom_paths(vm, opt.modulepath);        
        args &= ~arg_m;
    }
    else {
        // use default module paths
        berry_paths(vm);
    }
    
    if (args & arg_g) {
        comp_set_named_gbl(vm); /* forced named global in VM code */
        args &= ~arg_g;         /* clear the flag for this option not to interfere with other options */
    }
    if (args & arg_s) {
        comp_set_strict(vm);    /* compiler in strict mode */
        args &= ~arg_s;
    }
    if (args & arg_v) {
        be_writestring(FULL_VERSION "\n");
    }
    if (args & arg_h) {
        be_writestring(help_information);
    }
    push_args(vm, argc, argv);
    if (args & (arg_c | arg_o)) {
        if (!opt.src && argc > 0) {
            opt.src = *argv;
        }
        return build_file(vm, opt.dst, opt.src, args);
    }
    return load_script(vm, argc, argv, args);
}


int main(int argc, char *argv[])
{
    int res;
    bvm *vm = be_vm_new(); /* create a virtual machine instance */
    be_set_ctype_func_hanlder(vm, be_call_ctype_func);
    res = analysis_args(vm, argc, argv);
    be_vm_delete(vm); /* free all objects and vm */
    return res;
}

#endif // COMPILE_BERRY_LIB
