/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "berry.h"
#include "be_mem.h"
#include "be_sys.h"
// #include <stdio.h>
#include <string.h>
#include <Arduino.h>

// from https://github.com/eyalroz/cpp-static-block
#include "static_block.hpp"

// Local pointer for file managment
#ifdef USE_UFILESYS
    #include <FS.h>
    #include <LittleFS.h>
    #include "ZipReadFS.h"
    extern FS *ffsp;
    FS zip_ufsp(ZipReadFSImplPtr(new ZipReadFSImpl(&ffsp)));
#endif // USE_UFILESYS

/* this file contains configuration for the file system. */

/* standard input and output */
extern "C" {
    int strncmp_PP(const char * str1P, const char * str2P, size_t size)
    {
        int result = 0;

        while (size > 0)
        {
            char ch1 = pgm_read_byte(str1P++);
            char ch2 = pgm_read_byte(str2P++);
            result = ch1 - ch2;
            if (result != 0 || ch2 == '\0')
            {
                break;
            }

            size--;
        }

        return result;
    }

    //
    char * strchr_P(const char *s, int c) {
        do {
            if (pgm_read_byte(s) == c) {
                return (char*)s;
            }
        } while (pgm_read_byte(s++));
        return (0);
    }
}

// We need to create a local buffer, since we might mess up mqtt_data
#ifndef BERRY_LOGSZ
#define BERRY_LOGSZ 700
#endif

static char * log_berry_buffer = nullptr;
static_block {
    log_berry_buffer = (char*) malloc(BERRY_LOGSZ);
    if (log_berry_buffer) log_berry_buffer[0] = 0;
}
extern void berry_log(const char * berry_buf);

BERRY_API void be_writebuffer(const char *buffer, size_t length)
{
    if (!log_berry_buffer) return;
    if (buffer == nullptr || length == 0) { return; }
    uint32_t idx = 0;
    while (idx < length) {
        int32_t cr_pos = -1;
        // find next occurence of '\n' or '\r'
        for (uint32_t i = idx; i < length; i++) {
            if ((pgm_read_byte(&buffer[i]) == '\n') || (pgm_read_byte(&buffer[i]) == '\r')) {
                cr_pos = i;
                break;
            }
        }
        uint32_t chars_to_append = (cr_pos >= 0) ? cr_pos - idx : length - idx;     // note cr_pos < length
        snprintf(log_berry_buffer, BERRY_LOGSZ, "%s%.*s", log_berry_buffer, chars_to_append, &buffer[idx]);   // append at most `length` chars
        if (cr_pos >= 0) {
            // flush
            berry_log(log_berry_buffer);
            log_berry_buffer[0] = 0;        // clear string
        }
        idx += chars_to_append + 1;         // skip '\n'
    }
    // Serial.write(buffer, length);
    // be_fwrite(stdout, buffer, length);
}

// provides MPATH_ constants
#include "be_port.h"
extern "C" {
    // this combined action is called from be_path_tasmota_lib.c
    // by using a single function, we save >200 bytes of flash
    // by reducing code repetition.
    int _m_path_action(bvm *vm, int8_t action){
#ifdef USE_UFILESYS
        // if this changes to not -1, we push it as a bool
        int res = -1;
        // this tells us to return the vm, not nil
        int returnit = 0;

        // comment out if nil return is OK to save some flash
        switch (action){
            case MPATH_EXISTS:
            case MPATH_REMOVE:
                res = 0;
                break;
        }

        if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
            const char *path = be_tostring(vm, 1);
            if (path != nullptr) {
                switch (action){
                    case MPATH_EXISTS:
                        res = be_isexist(path);
                        break;
                    case MPATH_REMOVE:
                        res = be_unlink(path);
                        break;
                    case MPATH_RMDIR:
                        res = zip_ufsp.rmdir(path);
                        break;
                    case MPATH_MKDIR:
                        res = zip_ufsp.mkdir(path);
                        break;
                    case MPATH_LISTDIR:
                        be_newobject(vm, "list"); // add our list object and fall through
                        returnit = 1;
                    case MPATH_ISDIR:
                    case MPATH_MODIFIED: {
                        //isdir needs to open the file, listdir does not

                        // we use be_fopen because it pre-pends with '/'.
                        // without this TAS fails to find stuff at boot...
                        File *dir = (File *)be_fopen(path, "r");
                        if (dir) {
                            String fpath;
                            String fname;
                            switch (action){
                                case MPATH_LISTDIR:
                                    dir->seekDir(0);
                                    fpath = dir->getNextFileName();
                                    while (fpath.length() != 0) {
                                        fname = fpath.substring(fpath.lastIndexOf("/") + 1);
                                        const char * fn = fname.c_str();
                                        be_pushstring(vm, fn);
                                        be_data_push(vm, -2);
                                        be_pop(vm, 1);
                                        fpath = dir->getNextFileName();
                                    }
                                    break;
                                case MPATH_ISDIR:
                                    // push bool belowthe only one to push an int, so do it here.
                                    res = dir->isDirectory();
                                    break;
                                case MPATH_MODIFIED:
                                    // the only one to push an int, so do it here.
                                    be_pushint(vm, dir->getLastWrite());
                                    returnit = 1;
                                    break;
                            }
                            be_fclose(dir);
                        }
                    } break;
                }

                // these 
                switch (action){
                    case MPATH_LISTDIR:
                        // if it was list, pop always
                        be_pop(vm, 1);
                        break;
                }
            } // invalid filename -> nil return
        } // not a string, or no arg -> nil return unless see below

        // if we get here, and it was exists or remove, return false always
        // i.e. it's false for no filename or null filename.

        // if it was a boolean result
        if (res != -1){
            be_pushbool(vm, res);
            returnit = 1;
        }
        if (returnit){
            be_return(vm);
        }

#endif // USE_UFILESYS
        be_return_nil(vm);
    }
}

BERRY_API char* be_readstring(char *buffer, size_t size)
{
    return be_fgets(stdin, buffer, (int)size);
}

/* use the standard library implementation file API. */
#ifdef USE_UFILESYS
void* be_fopen(const char *filename, const char *modes)
{
    if (filename != nullptr && modes != nullptr) {
        char fname2[strlen(filename) + 2];
        if (filename[0] == '/') {
            strcpy(fname2, filename);   // copy unchanged
        } else {
            fname2[0] = '/';
            strcpy(fname2 + 1, filename);   // prepend with '/'
        }
        // Serial.printf("be_fopen filename=%s, modes=%s\n", filename, modes);
        File f = zip_ufsp.open(fname2, modes);       // returns an object, not a pointer
        if (f) {
            File * f_ptr = new File(f);                 // copy to dynamic object
            *f_ptr = f;                                 // TODO is this necessary?
            return f_ptr;
        }
    }
    return nullptr;
    // return fopen(filename, modes);
}

#endif // USE_UFILESYS


#ifdef USE_UFILESYS
// Tasmota specific, get the underlying Arduino File
File * be_get_arduino_file(void *hfile)
{
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        return f_ptr;
    }
    return nullptr;
    // return fopen(filename, modes);
}
#endif

int be_fclose(void *hfile)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_fclose\n");
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        f_ptr->close();
        delete f_ptr;
        return 0;
    }
#endif // USE_UFILESYS
    return -1;
    // return fclose(hfile);
}

size_t be_fwrite(void *hfile, const void *buffer, size_t length)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_fwrite %d\n", length);
    if (hfile != nullptr && buffer != nullptr) {
        File * f_ptr = (File*) hfile;
        return f_ptr->write((const uint8_t*) buffer, length);
    }
#endif // USE_UFILESYS
    return 0;
    // return fwrite(buffer, 1, length, hfile);
}

size_t be_fread(void *hfile, void *buffer, size_t length)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_fread %d\n", length);
    if (hfile != nullptr && buffer != nullptr) {
        File * f_ptr = (File*) hfile;
        int32_t ret = f_ptr->read((uint8_t*) buffer, length);
        if (ret >= 0) {
            // Serial.printf("be_fread ret = %d\n", ret);
            return ret;
        }
    }
#endif // USE_UFILESYS
    return 0;
    // return fread(buffer, 1, length, hfile);
}

char* be_fgets(void *hfile, void *buffer, int size)
{
#ifdef USE_UFILESYS
    if (size <= 2) { return nullptr; }  // can't work if size is 2 or less
    // Serial.printf("be_fgets size=%d hfile=%p buf=%p\n", size, hfile, buffer);
    uint8_t * buf = (uint8_t*) buffer;
    if (hfile != nullptr && buffer != nullptr && size > 0) {
        File * f_ptr = (File*) hfile;
        int ret = f_ptr->readBytesUntil('\n', buf, size - 2);
        // Serial.printf("be_fgets ret=%d\n", ret);
        if (ret >= 0) {
            buf[ret] = 0;           // add string terminator
            if (ret > 0 && ret < size - 2) {
                buf[ret] = '\n';
                buf[ret+1] = 0;
            }
            return (char*) buffer;
        }
    }
#endif // USE_UFILESYS
    return nullptr;
    // return fgets(buffer, size, hfile);
}

int be_fseek(void *hfile, long offset)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_fseek %d\n", offset);
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        if (f_ptr->seek(offset)) {
            return 0;       // success
        }
    }
#endif // USE_UFILESYS
    return -1;
    // return fseek(hfile, offset, SEEK_SET);
}

long int be_ftell(void *hfile)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_ftell\n");
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        return f_ptr->position();
    }
#endif // USE_UFILESYS
    return 0;
    // return ftell(hfile);
}

long int be_fflush(void *hfile)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_fflush\n");
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        f_ptr->flush();
    }
#endif // USE_UFILESYS
    return 0;
    // return fflush(hfile);
}

size_t be_fsize(void *hfile)
{
#ifdef USE_UFILESYS
    // Serial.printf("be_fsize\n");
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        return f_ptr->size();
    }
    // long int size, offset = be_ftell(hfile);
    // fseek(hfile, 0L, SEEK_END);
    // size = ftell(hfile);
    // fseek(hfile, offset, SEEK_SET);
    // return size;
#endif // USE_UFILESYS
    return 0;
}

extern "C" time_t be_last_modified(void *hfile)
{
#ifdef USE_UFILESYS
    if (hfile != nullptr) {
        File * f_ptr = (File*) hfile;
        return f_ptr->getLastWrite();
    }
#endif // USE_UFILESYS
    return 0;
}

int be_isexist(const char *filename)
{
#ifdef USE_UFILESYS
    char fname2[strlen(filename) + 2];
    if (filename[0] == '/') {
        strcpy(fname2, filename);   // copy unchanged
    } else {
        fname2[0] = '/';
        strcpy(fname2 + 1, filename);   // prepend with '/'
    }
    return zip_ufsp.exists(fname2);
#endif // USE_UFILESYS
    return 0;
}

int be_unlink(const char *filename)
{
#ifdef USE_UFILESYS
    char fname2[strlen(filename) + 2];
    if (filename[0] == '/') {
        strcpy(fname2, filename);   // copy unchanged
    } else {
        fname2[0] = '/';
        strcpy(fname2 + 1, filename);   // prepend with '/'
    }
    return zip_ufsp.remove(fname2);
#endif // USE_UFILESYS
    return 0;
}

/* format file system - erase everything */
extern "C" int be_format_fs(void)
{
#ifdef USE_UFILESYS
    return LittleFS.format();
#endif // USE_UFILESYS
    return 0;
}

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
