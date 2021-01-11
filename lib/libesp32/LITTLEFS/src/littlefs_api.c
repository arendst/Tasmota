/**
 * @file littlefs_api.c
 * @brief Maps the HAL of esp_partition <-> littlefs
 * @author Brian Pugh
 */

#define ESP_LOCAL_LOG_LEVEL ESP_LOG_INFO

#include "esp_log.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "lfs.h"
#include "esp_littlefs.h"
#include "littlefs_api.h"

static const char TAG[] = "esp_littlefs_api";

int littlefs_api_read(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    esp_littlefs_t * efs = c->context;
    size_t part_off = (block * c->block_size) + off;
    esp_err_t err = esp_partition_read(efs->partition, part_off, buffer, size);
    if (err) {
        ESP_LOGE(TAG, "failed to read addr %08x, size %08x, err %d", part_off, size, err);
        return LFS_ERR_IO;
    }
    return 0;
}

int littlefs_api_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {
    esp_littlefs_t * efs = c->context;
    size_t part_off = (block * c->block_size) + off;
    esp_err_t err = esp_partition_write(efs->partition, part_off, buffer, size);
    if (err) {
        ESP_LOGE(TAG, "failed to write addr %08x, size %08x, err %d", part_off, size, err);
        return LFS_ERR_IO;
    }
    return 0;
}

int littlefs_api_erase(const struct lfs_config *c, lfs_block_t block) {
    esp_littlefs_t * efs = c->context;
    size_t part_off = block * c->block_size;
    esp_err_t err = esp_partition_erase_range(efs->partition, part_off, c->block_size);
    if (err) {
        ESP_LOGE(TAG, "failed to erase addr %08x, size %08x, err %d", part_off, c->block_size, err);
        return LFS_ERR_IO;
    }
    return 0;

}

int littlefs_api_sync(const struct lfs_config *c) {
    /* Unnecessary for esp-idf */
    return 0;
}

