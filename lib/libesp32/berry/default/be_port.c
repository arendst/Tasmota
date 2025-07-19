/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef TASMOTA       // only when compiling stand-along

#include "berry.h"
#include "be_mem.h"
#include "be_sys.h"
#include <stdio.h>
#include <string.h>

/* this file contains configuration for the file system. */

/* standard input and output */

BERRY_API void be_writebuffer(const char *buffer, size_t length)
{
    be_fwrite(stdout, buffer, length);
}

BERRY_API char* be_readstring(char *buffer, size_t size)
{
    return be_fgets(stdin, buffer, (int)size);
}

/* use the standard library implementation file API. */
#if !defined(USE_FATFS)

void* be_fopen(const char *filename, const char *modes)
{
    return fopen(filename, modes);
}

int be_fclose(void *hfile)
{
    return fclose(hfile);
}

size_t be_fwrite(void *hfile, const void *buffer, size_t length)
{
    return fwrite(buffer, 1, length, hfile);
}

size_t be_fread(void *hfile, void *buffer, size_t length)
{
    return fread(buffer, 1, length, hfile);
}

char* be_fgets(void *hfile, void *buffer, int size)
{
    return fgets(buffer, size, hfile);
}

int be_fseek(void *hfile, long offset)
{
    return fseek(hfile, offset, SEEK_SET);
}

long int be_ftell(void *hfile)
{
    return ftell(hfile);
}

long int be_fflush(void *hfile)
{
    return fflush(hfile);
}

size_t be_fsize(void *hfile)
{
    long int size, offset = be_ftell(hfile);
    fseek(hfile, 0L, SEEK_END);
    size = ftell(hfile);
    fseek(hfile, offset, SEEK_SET);
    return size;
}

#else /* use the FatFs library implementation file API. */

#include "ff.h"

#if FF_FS_RPATH != 2
  #error FatFs check (FF_FS_RPATH == 2) failed.
#endif

#if FF_USE_STRFUNC < 1
  #error FatFs check (FF_USE_STRFUNC >= 1) failed.
#endif

void* be_fopen(const char *filename, const char *modes)
{
    BYTE mode = 0;
    FIL *fp = be_os_malloc(sizeof(FIL));

    switch (modes[0]) {
    case 'r': mode |= FA_READ; break;
    case 'w': mode |= FA_CREATE_ALWAYS | FA_WRITE; break;
    case 'a': mode |= FA_OPEN_APPEND | FA_WRITE; break;
    default: return NULL;
    }
    switch (modes[1]) {
    case '+': mode |= FA_READ | FA_WRITE; break;
    case 't':
    case 'b':
    case '\0': break;
    default: return NULL;
    }
    if (modes[1] && modes[2] == '+') {
        mode |= FA_READ | FA_WRITE;
    }
    if (fp && f_open(fp, filename, mode) == FR_OK) {
        return fp;
    }
    be_os_free(fp);
    return NULL;
}

int be_fclose(void *hfile)
{
    int res = f_close(hfile) != FR_OK;
    be_os_free(hfile);
    return res;
}

size_t be_fwrite(void *hfile, const void *buffer, size_t length)
{
    UINT bw;
    if (hfile == stdout || hfile == stderr || hfile == stdin) {
        return fwrite(buffer, 1, length, hfile);
    }
    f_write(hfile, buffer, (UINT)length, &bw);
    return bw;
}

size_t be_fread(void *hfile, void *buffer, size_t length)
{
    UINT br;
    if (hfile == stdout || hfile == stderr || hfile == stdin) {
        return fread(buffer, 1, length, hfile);
    }
    f_read(hfile, buffer, (UINT)length, &br);
    return br;
}

char* be_fgets(void *hfile, void *buffer, int size)
{
    if (hfile == stdout || hfile == stderr || hfile == stdin) {
        return fgets(buffer, size, hfile);
    }
    return f_gets(buffer, size, hfile);
}

int be_fseek(void *hfile, long offset)
{
    return f_lseek(hfile, (FSIZE_t)offset);
}

long int be_ftell(void *hfile)
{
    return f_tell(hfile);
}

long int be_fflush(void *hfile)
{
    return f_sync(hfile);
}

size_t be_fsize(void *hfile)
{
    return f_size(hfile);
}

#endif /* !defined(USE_FATFS) */

#if BE_USE_FILE_SYSTEM
#if defined(USE_FATFS) /* FatFs */

int be_isdir(const char *path)
{
    FILINFO fno;
    FRESULT fr = f_stat(path, &fno);
    return fr == FR_OK && fno.fattrib & AM_DIR;
}

int be_isfile(const char *path)
{
    FILINFO fno;
    FRESULT fr = f_stat(path, &fno);
    return fr == FR_OK && !(fno.fattrib & AM_DIR);
}

int be_isexist(const char *path)
{
    FILINFO fno;
    return f_stat(path, &fno) == FR_OK;
}

char* be_getcwd(char *buf, size_t size)
{
    FRESULT fr = f_getcwd(buf, (UINT)size);
    return fr == FR_OK ? buf : NULL;
}

int be_chdir(const char *path)
{
    return f_chdir(path);
}

int be_mkdir(const char *path)
{
    return f_mkdir(path);
}

int be_unlink(const char *filename)
{
    return f_unlink(filename);
}

int be_dirfirst(bdirinfo *info, const char *path)
{
    info->dir = be_os_malloc(sizeof(DIR));
    info->file = be_os_malloc(sizeof(FILINFO));
    if (info->dir && info->file) {
        FRESULT fr = f_opendir(info->dir, path);
        return fr == FR_OK ? be_dirnext(info) : 1;
    }
    be_os_free(info->dir);
    be_os_free(info->file);
    info->dir = NULL;
    info->file = NULL;
    return 1;
}

int be_dirnext(bdirinfo *info)
{
    FRESULT fr = f_readdir(info->dir, info->file);
    info->name = ((FILINFO *)info->file)->fname;
    return fr != FR_OK || *info->name == '\0';
}

int be_dirclose(bdirinfo *info)
{
    if (info->dir) {
        int res = f_closedir(info->dir) != FR_OK;
        be_os_free(info->dir);
        be_os_free(info->file);
        return res;
    }
    return 1;
}

#elif defined(_MSC_VER) /* MSVC*/

#include <windows.h>
#include <direct.h>
#include <io.h>

int be_isdir(const char *path)
{
    DWORD type = GetFileAttributes(path);
    return type != INVALID_FILE_ATTRIBUTES
        && (type & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int be_isfile(const char *path)
{
    DWORD type = GetFileAttributes(path);
    return type != INVALID_FILE_ATTRIBUTES
        && (type & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

int be_isexist(const char *path)
{
    return GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES;
}

char* be_getcwd(char *buf, size_t size)
{
    return _getcwd(buf, (int)size);
}

int be_chdir(const char *path)
{
    return _chdir(path);
}

int be_mkdir(const char *path)
{
    return _mkdir(path);
}

int be_unlink(const char *filename)
{
    return remove(filename);
}

int be_dirfirst(bdirinfo *info, const char *path)
{
    char *buf = be_os_malloc(strlen(path) + 3);
    info->file = be_os_malloc(sizeof(struct _finddata_t));
    info->dir = NULL;
    if (buf && info->file) {
        struct _finddata_t *cfile = info->file;
        strcat(strcpy(buf, path), "/*");
        info->dir = (void *)_findfirst(buf, cfile);
        info->name = cfile->name;
        be_os_free(buf);
        return (intptr_t)info->dir == -1;
    }
    be_os_free(buf);
    return 1;
}

int be_dirnext(bdirinfo *info)
{
    struct _finddata_t *cfile = info->file;
    int res = _findnext((intptr_t)info->dir, cfile) != 0;
    info->name = cfile->name;
    return res;
}

int be_dirclose(bdirinfo *info)
{
    be_os_free(info->file);
    return _findclose((intptr_t)info->dir) != 0;
}

#else /* must be POSIX */

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

int be_isdir(const char *path)
{
    struct stat path_stat;
    int res = stat(path, &path_stat);
    return res == 0 && S_ISDIR(path_stat.st_mode);
}

int be_isfile(const char *path)
{
    struct stat path_stat;
    int res = stat(path, &path_stat);
    return res == 0 && !S_ISDIR(path_stat.st_mode);
}

int be_isexist(const char *path)
{
    struct stat path_stat;
    return stat(path, &path_stat) == 0;
}

char* be_getcwd(char *buf, size_t size)
{
    return getcwd(buf, size);
}

int be_chdir(const char *path)
{
    return chdir(path);
}

int be_mkdir(const char *path)
{
#ifdef _WIN32
    return mkdir(path);
#else
    return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

int be_unlink(const char *filename)
{
    return remove(filename);
}

int be_dirfirst(bdirinfo *info, const char *path)
{
    info->dir = opendir(path);
    if (info->dir) {
        return be_dirnext(info);
    }
    return 1;
}

int be_dirnext(bdirinfo *info)
{
    struct dirent *file;
    info->file = file = readdir(info->dir);
    if (file) {
        info->name = file->d_name;
        return 0;
    }
    return 1;
}

int be_dirclose(bdirinfo *info)
{
    return closedir(info->dir) != 0;
}

#endif /* POSIX */
#endif /* BE_USE_OS_MODULE || BE_USE_FILE_SYSTEM */

#endif // COMPILE_BERRY_LIB
