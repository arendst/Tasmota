#ifndef ESP_LITTLEFS_API_H__
#define ESP_LITTLEFS_API_H__

#include <stdint.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_vfs.h"
#include "esp_partition.h"
#include "lfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief a file descriptor
 * That's also a singly linked list used for keeping tracks of all opened file descriptor 
 *
 * Shortcomings/potential issues of 32-bit hash (when CONFIG_LITTLEFS_USE_ONLY_HASH) listed here:
 *     * unlink - If a different file is open that generates a hash collision, it will report an
 *                error that it cannot unlink an open file.
 *     * rename - If a different file is open that generates a hash collision with
 *                src or dst, it will report an error that it cannot rename an open file.
 * Potential consequences:
 *    1. A file cannot be deleted while a collision-geneating file is open.
 *       Worst-case, if the other file is always open during the lifecycle
 *       of your app, it's collision file cannot be deleted, which in the 
 *       worst-case could cause storage-capacity issues.
 *    2. Same as (1), but for renames
 */
typedef struct _vfs_littlefs_file_t {
    lfs_file_t file;
    uint32_t   hash;
    struct _vfs_littlefs_file_t * next;       /*!< Pointer to next file in Singly Linked List */
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
    char     * path;
#endif
} vfs_littlefs_file_t;

/**
 * @brief littlefs definition structure
 */
typedef struct {
    lfs_t *fs;                                /*!< Handle to the underlying littlefs */
    SemaphoreHandle_t lock;                   /*!< FS lock */
    const esp_partition_t* partition;         /*!< The partition on which littlefs is located */
    char base_path[ESP_VFS_PATH_MAX+1];       /*!< Mount point */

    struct lfs_config cfg;                    /*!< littlefs Mount configuration */

    vfs_littlefs_file_t *file;                /*!< Singly Linked List of files */

    vfs_littlefs_file_t **cache;              /*!< A cache of pointers to the opened files */
    uint16_t             cache_size;          /*!< The cache allocated size (in pointers) */
    uint16_t             fd_count;            /*!< The count of opened file descriptor used to speed up computation */
} esp_littlefs_t;

/**
 * @brief Read a region in a block.
 *
 * Negative error codes are propogated to the user.
 *
 * @return errorcode. 0 on success.
 */
int littlefs_api_read(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size);

/**
 * @brief Program a region in a block.
 *
 * The block must have previously been erased. 
 * Negative error codes are propogated to the user.
 * May return LFS_ERR_CORRUPT if the block should be considered bad.
 *
 * @return errorcode. 0 on success.
 */
int littlefs_api_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size);

/**
 * @brief Erase a block.
 *
 * A block must be erased before being programmed.
 * The state of an erased block is undefined.
 * Negative error codes are propogated to the user.
 * May return LFS_ERR_CORRUPT if the block should be considered bad.
 * @return errorcode. 0 on success.
 */
int littlefs_api_erase(const struct lfs_config *c, lfs_block_t block);

/**
 * @brief Sync the state of the underlying block device.
 *
 * Negative error codes are propogated to the user.
 *
 * @return errorcode. 0 on success.
 */
int littlefs_api_sync(const struct lfs_config *c);

#ifdef __cplusplus
}
#endif

#endif
