#include "../../../lvgl.h"
#if LV_USE_FS_LITTLEFS

#include "lfs.h"
#include "../../core/lv_global.h"

typedef struct LittleFile {
    lfs_file_t file;
} LittleFile;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);

void lv_littlefs_set_handler(lfs_t * lfs)
{
    lv_fs_drv_t * drv = lv_fs_get_drv(LV_FS_LITTLEFS_LETTER);
    drv->user_data = lfs;
}

/**
 * Register a driver for the LittleFS File System interface
 */
void lv_fs_littlefs_init(void)
{
    lv_fs_drv_t * fs_drv = &(LV_GLOBAL_DEFAULT()->littlefs_fs_drv);
    lv_fs_drv_init(fs_drv);

    fs_drv->letter = LV_FS_LITTLEFS_LETTER;
    fs_drv->open_cb = fs_open;
    fs_drv->close_cb = fs_close;
    fs_drv->read_cb = fs_read;
    fs_drv->write_cb = fs_write;
    fs_drv->seek_cb = fs_seek;
    fs_drv->tell_cb = fs_tell;

    fs_drv->dir_close_cb = NULL;
    fs_drv->dir_open_cb = NULL;
    fs_drv->dir_read_cb = NULL;

    lv_fs_drv_register(fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Open a file
 * @param drv       pointer to a driver where this function belongs
 * @param path      path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode      read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return          a file descriptor or NULL on error
 */
static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{
    LV_UNUSED(drv);

    int flags;
    if(mode == LV_FS_MODE_WR)
        flags = LFS_O_WRONLY;
    else if(mode == LV_FS_MODE_RD)
        flags = LFS_O_RDONLY;
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
        flags = LFS_O_RDWR;

    LittleFile * lf = (LittleFile *)lv_malloc(sizeof(LittleFile));
    LV_ASSERT_NULL(lf);

    lfs_t * lfs = (lfs_t *)drv->user_data;
    int err = lfs_file_open(lfs, &lf->file, path, flags);
    if(err) {
        return NULL;
    }

    return (void *)lf;
}

/**
 * Close an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable. (opened with fs_open)
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p)
{
    LV_UNUSED(drv);
    LittleFile * lf = (LittleFile *)file_p;

    lfs_t * lfs = (lfs_t *)drv->user_data;
    lfs_file_close(lfs, &lf->file);
    lv_free(lf);

    return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable.
 * @param buf       pointer to a memory block where to store the read data
 * @param btr       number of Bytes To Read
 * @param br        the real number of read bytes (Byte Read)
 * @return          LV_FS_RES_OK: no error or any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    LV_UNUSED(drv);
    LittleFile * lf = (LittleFile *)file_p;

    lfs_t * lfs = (lfs_t *)drv->user_data;
    *br = lfs_file_read(lfs, &lf->file, (uint8_t *)buf, btr);

    return (int32_t)(*br) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable
 * @param buf       pointer to a buffer with the bytes to write
 * @param btw       Bytes To Write
 * @param bw        the number of real written bytes (Bytes Written)
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    LV_UNUSED(drv);
    LittleFile * lf = (LittleFile *)file_p;

    lfs_t * lfs = (lfs_t *)drv->user_data;
    *bw = lfs_file_write(lfs, &lf->file, (uint8_t *)buf, btw);

    return (int32_t)(*bw) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable. (opened with fs_open )
 * @param pos       the new position of read write pointer
 * @param whence    tells from where to interpret the `pos`. See @lv_fs_whence_t
 * @return          LV_FS_RES_OK: no error or any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    LV_UNUSED(drv);
    int mode;
    if(whence == LV_FS_SEEK_SET)
        mode = LFS_SEEK_SET;
    else if(whence == LV_FS_SEEK_CUR)
        mode = LFS_SEEK_CUR;
    else if(whence == LV_FS_SEEK_END)
        mode = LFS_SEEK_END;

    LittleFile * lf = (LittleFile *)file_p;

    lfs_t * lfs = (lfs_t *)drv->user_data;
    int rc = lfs_file_seek(lfs, &lf->file, pos, mode);

    return rc < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * Give the position of the read write pointer
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_p variable
 * @param pos_p     pointer to store the result
 * @return          LV_FS_RES_OK: no error or any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    LV_UNUSED(drv);
    LittleFile * lf = (LittleFile *)file_p;

    lfs_t * lfs = (lfs_t *)drv->user_data;
    *pos_p = lfs_file_tell(lfs, &lf->file);

    return (int32_t)(*pos_p) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

#endif
