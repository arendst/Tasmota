/*
 * The little filesystem
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @note Modified and used by lorol for Arduino esp32 core tests
 */

//#define LFS_NO_ERROR   /* Change default error reporting level: LFS_NO_DEBUG, LFS_NO_WARN,  LFS_NO_ERROR,  LFS_YES_TRACE */

#include "lfs.h"
#include "lfs_util.h"

#define LFS_BLOCK_NULL ((lfs_block_t)-1)
#define LFS_BLOCK_INLINE ((lfs_block_t)-2)

/// Caching block device operations ///
static inline void lfs_cache_drop(lfs_t *lfs, lfs_cache_t *rcache) {
    // do not zero, cheaper if cache is readonly or only going to be
    // written with identical data (during relocates)
    (void)lfs;
    rcache->block = LFS_BLOCK_NULL;
}

static inline void lfs_cache_zero(lfs_t *lfs, lfs_cache_t *pcache) {
    // zero to avoid information leak
    memset(pcache->buffer, 0xff, lfs->cfg->cache_size);
    pcache->block = LFS_BLOCK_NULL;
}

static int lfs_bd_read(lfs_t *lfs,
        const lfs_cache_t *pcache, lfs_cache_t *rcache, lfs_size_t hint,
        lfs_block_t block, lfs_off_t off,
        void *buffer, lfs_size_t size) {
    uint8_t *data = buffer;
    if (block >= lfs->cfg->block_count ||
            off+size > lfs->cfg->block_size) {
        return LFS_ERR_CORRUPT;
    }

    while (size > 0) {
        lfs_size_t diff = size;

        if (pcache && block == pcache->block &&
                off < pcache->off + pcache->size) {
            if (off >= pcache->off) {
                // is already in pcache?
                diff = lfs_min(diff, pcache->size - (off-pcache->off));
                memcpy(data, &pcache->buffer[off-pcache->off], diff);

                data += diff;
                off += diff;
                size -= diff;
                continue;
            }

            // pcache takes priority
            diff = lfs_min(diff, pcache->off-off);
        }

        if (block == rcache->block &&
                off < rcache->off + rcache->size) {
            if (off >= rcache->off) {
                // is already in rcache?
                diff = lfs_min(diff, rcache->size - (off-rcache->off));
                memcpy(data, &rcache->buffer[off-rcache->off], diff);

                data += diff;
                off += diff;
                size -= diff;
                continue;
            }

            // rcache takes priority
            diff = lfs_min(diff, rcache->off-off);
        }

        if (size >= hint && off % lfs->cfg->read_size == 0 &&
                size >= lfs->cfg->read_size) {
            // bypass cache?
            diff = lfs_aligndown(diff, lfs->cfg->read_size);
            int err = lfs->cfg->read(lfs->cfg, block, off, data, diff);
            if (err) {
                return err;
            }

            data += diff;
            off += diff;
            size -= diff;
            continue;
        }

        // load to cache, first condition can no longer fail
        LFS_ASSERT(block < lfs->cfg->block_count);
        rcache->block = block;
        rcache->off = lfs_aligndown(off, lfs->cfg->read_size);
        rcache->size = lfs_min(
                lfs_min(
                    lfs_alignup(off+hint, lfs->cfg->read_size),
                    lfs->cfg->block_size)
                - rcache->off,
                lfs->cfg->cache_size);
        int err = lfs->cfg->read(lfs->cfg, rcache->block,
                rcache->off, rcache->buffer, rcache->size);
        LFS_ASSERT(err <= 0);
        if (err) {
            return err;
        }
    }

    return 0;
}

enum {
    LFS_CMP_EQ = 0,
    LFS_CMP_LT = 1,
    LFS_CMP_GT = 2,
};

static int lfs_bd_cmp(lfs_t *lfs,
        const lfs_cache_t *pcache, lfs_cache_t *rcache, lfs_size_t hint,
        lfs_block_t block, lfs_off_t off,
        const void *buffer, lfs_size_t size) {
    const uint8_t *data = buffer;

    for (lfs_off_t i = 0; i < size; i++) {
        uint8_t dat;
        int err = lfs_bd_read(lfs,
                pcache, rcache, hint-i,
                block, off+i, &dat, 1);
        if (err) {
            return err;
        }

        if (dat != data[i]) {
            return (dat < data[i]) ? LFS_CMP_LT : LFS_CMP_GT;
        }
    }

    return LFS_CMP_EQ;
}

static int lfs_bd_flush(lfs_t *lfs,
        lfs_cache_t *pcache, lfs_cache_t *rcache, bool validate) {
    if (pcache->block != LFS_BLOCK_NULL && pcache->block != LFS_BLOCK_INLINE) {
        LFS_ASSERT(pcache->block < lfs->cfg->block_count);
        lfs_size_t diff = lfs_alignup(pcache->size, lfs->cfg->prog_size);
        int err = lfs->cfg->prog(lfs->cfg, pcache->block,
                pcache->off, pcache->buffer, diff);
        LFS_ASSERT(err <= 0);
        if (err) {
            return err;
        }

        if (validate) {
            // check data on disk
            lfs_cache_drop(lfs, rcache);
            int res = lfs_bd_cmp(lfs,
                    NULL, rcache, diff,
                    pcache->block, pcache->off, pcache->buffer, diff);
            if (res < 0) {
                return res;
            }

            if (res != LFS_CMP_EQ) {
                return LFS_ERR_CORRUPT;
            }
        }

        lfs_cache_zero(lfs, pcache);
    }

    return 0;
}

static int lfs_bd_sync(lfs_t *lfs,
        lfs_cache_t *pcache, lfs_cache_t *rcache, bool validate) {
    lfs_cache_drop(lfs, rcache);

    int err = lfs_bd_flush(lfs, pcache, rcache, validate);
    if (err) {
        return err;
    }

    err = lfs->cfg->sync(lfs->cfg);
    LFS_ASSERT(err <= 0);
    return err;
}

static int lfs_bd_prog(lfs_t *lfs,
        lfs_cache_t *pcache, lfs_cache_t *rcache, bool validate,
        lfs_block_t block, lfs_off_t off,
        const void *buffer, lfs_size_t size) {
    const uint8_t *data = buffer;
    LFS_ASSERT(block == LFS_BLOCK_INLINE || block < lfs->cfg->block_count);
    LFS_ASSERT(off + size <= lfs->cfg->block_size);

    while (size > 0) {
        if (block == pcache->block &&
                off >= pcache->off &&
                off < pcache->off + lfs->cfg->cache_size) {
            // already fits in pcache?
            lfs_size_t diff = lfs_min(size,
                    lfs->cfg->cache_size - (off-pcache->off));
            memcpy(&pcache->buffer[off-pcache->off], data, diff);

            data += diff;
            off += diff;
            size -= diff;

            pcache->size = lfs_max(pcache->size, off - pcache->off);
            if (pcache->size == lfs->cfg->cache_size) {
                // eagerly flush out pcache if we fill up
                int err = lfs_bd_flush(lfs, pcache, rcache, validate);
                if (err) {
                    return err;
                }
            }

            continue;
        }

        // pcache must have been flushed, either by programming and
        // entire block or manually flushing the pcache
        LFS_ASSERT(pcache->block == LFS_BLOCK_NULL);

        // prepare pcache, first condition can no longer fail
        pcache->block = block;
        pcache->off = lfs_aligndown(off, lfs->cfg->prog_size);
        pcache->size = 0;
    }

    return 0;
}

static int lfs_bd_erase(lfs_t *lfs, lfs_block_t block) {
    LFS_ASSERT(block < lfs->cfg->block_count);
    int err = lfs->cfg->erase(lfs->cfg, block);
    LFS_ASSERT(err <= 0);
    return err;
}


/// Small type-level utilities ///
// operations on block pairs
static inline void lfs_pair_swap(lfs_block_t pair[2]) {
    lfs_block_t t = pair[0];
    pair[0] = pair[1];
    pair[1] = t;
}

static inline bool lfs_pair_isnull(const lfs_block_t pair[2]) {
    return pair[0] == LFS_BLOCK_NULL || pair[1] == LFS_BLOCK_NULL;
}

static inline int lfs_pair_cmp(
        const lfs_block_t paira[2],
        const lfs_block_t pairb[2]) {
    return !(paira[0] == pairb[0] || paira[1] == pairb[1] ||
             paira[0] == pairb[1] || paira[1] == pairb[0]);
}

static inline bool lfs_pair_sync(
        const lfs_block_t paira[2],
        const lfs_block_t pairb[2]) {
    return (paira[0] == pairb[0] && paira[1] == pairb[1]) ||
           (paira[0] == pairb[1] && paira[1] == pairb[0]);
}

static inline void lfs_pair_fromle32(lfs_block_t pair[2]) {
    pair[0] = lfs_fromle32(pair[0]);
    pair[1] = lfs_fromle32(pair[1]);
}

static inline void lfs_pair_tole32(lfs_block_t pair[2]) {
    pair[0] = lfs_tole32(pair[0]);
    pair[1] = lfs_tole32(pair[1]);
}

// operations on 32-bit entry tags
typedef uint32_t lfs_tag_t;
typedef int32_t lfs_stag_t;

#define LFS_MKTAG(type, id, size) \
    (((lfs_tag_t)(type) << 20) | ((lfs_tag_t)(id) << 10) | (lfs_tag_t)(size))

#define LFS_MKTAG_IF(cond, type, id, size) \
    ((cond) ? LFS_MKTAG(type, id, size) : LFS_MKTAG(LFS_FROM_NOOP, 0, 0))

#define LFS_MKTAG_IF_ELSE(cond, type1, id1, size1, type2, id2, size2) \
    ((cond) ? LFS_MKTAG(type1, id1, size1) : LFS_MKTAG(type2, id2, size2))

static inline bool lfs_tag_isvalid(lfs_tag_t tag) {
    return !(tag & 0x80000000);
}

static inline bool lfs_tag_isdelete(lfs_tag_t tag) {
    return ((int32_t)(tag << 22) >> 22) == -1;
}

static inline uint16_t lfs_tag_type1(lfs_tag_t tag) {
    return (tag & 0x70000000) >> 20;
}

static inline uint16_t lfs_tag_type3(lfs_tag_t tag) {
    return (tag & 0x7ff00000) >> 20;
}

static inline uint8_t lfs_tag_chunk(lfs_tag_t tag) {
    return (tag & 0x0ff00000) >> 20;
}

static inline int8_t lfs_tag_splice(lfs_tag_t tag) {
    return (int8_t)lfs_tag_chunk(tag);
}

static inline uint16_t lfs_tag_id(lfs_tag_t tag) {
    return (tag & 0x000ffc00) >> 10;
}

static inline lfs_size_t lfs_tag_size(lfs_tag_t tag) {
    return tag & 0x000003ff;
}

static inline lfs_size_t lfs_tag_dsize(lfs_tag_t tag) {
    return sizeof(tag) + lfs_tag_size(tag + lfs_tag_isdelete(tag));
}

// operations on attributes in attribute lists
struct lfs_mattr {
    lfs_tag_t tag;
    const void *buffer;
};

struct lfs_diskoff {
    lfs_block_t block;
    lfs_off_t off;
};

#define LFS_MKATTRS(...) \
    (struct lfs_mattr[]){__VA_ARGS__}, \
    sizeof((struct lfs_mattr[]){__VA_ARGS__}) / sizeof(struct lfs_mattr)

// operations on global state
static inline void lfs_gstate_xor(lfs_gstate_t *a, const lfs_gstate_t *b) {
    for (int i = 0; i < 3; i++) {
        ((uint32_t*)a)[i] ^= ((const uint32_t*)b)[i];
    }
}

static inline bool lfs_gstate_iszero(const lfs_gstate_t *a) {
    for (int i = 0; i < 3; i++) {
        if (((uint32_t*)a)[i] != 0) {
            return false;
        }
    }
    return true;
}

static inline bool lfs_gstate_hasorphans(const lfs_gstate_t *a) {
    return lfs_tag_size(a->tag);
}

static inline uint8_t lfs_gstate_getorphans(const lfs_gstate_t *a) {
    return lfs_tag_size(a->tag);
}

static inline bool lfs_gstate_hasmove(const lfs_gstate_t *a) {
    return lfs_tag_type1(a->tag);
}

static inline bool lfs_gstate_hasmovehere(const lfs_gstate_t *a,
        const lfs_block_t *pair) {
    return lfs_tag_type1(a->tag) && lfs_pair_cmp(a->pair, pair) == 0;
}

static inline void lfs_gstate_fromle32(lfs_gstate_t *a) {
    a->tag     = lfs_fromle32(a->tag);
    a->pair[0] = lfs_fromle32(a->pair[0]);
    a->pair[1] = lfs_fromle32(a->pair[1]);
}

static inline void lfs_gstate_tole32(lfs_gstate_t *a) {
    a->tag     = lfs_tole32(a->tag);
    a->pair[0] = lfs_tole32(a->pair[0]);
    a->pair[1] = lfs_tole32(a->pair[1]);
}

// other endianness operations
static void lfs_ctz_fromle32(struct lfs_ctz *ctz) {
    ctz->head = lfs_fromle32(ctz->head);
    ctz->size = lfs_fromle32(ctz->size);
}

static void lfs_ctz_tole32(struct lfs_ctz *ctz) {
    ctz->head = lfs_tole32(ctz->head);
    ctz->size = lfs_tole32(ctz->size);
}

static inline void lfs_superblock_fromle32(lfs_superblock_t *superblock) {
    superblock->version     = lfs_fromle32(superblock->version);
    superblock->block_size  = lfs_fromle32(superblock->block_size);
    superblock->block_count = lfs_fromle32(superblock->block_count);
    superblock->name_max    = lfs_fromle32(superblock->name_max);
    superblock->file_max    = lfs_fromle32(superblock->file_max);
    superblock->attr_max    = lfs_fromle32(superblock->attr_max);
}

static inline void lfs_superblock_tole32(lfs_superblock_t *superblock) {
    superblock->version     = lfs_tole32(superblock->version);
    superblock->block_size  = lfs_tole32(superblock->block_size);
    superblock->block_count = lfs_tole32(superblock->block_count);
    superblock->name_max    = lfs_tole32(superblock->name_max);
    superblock->file_max    = lfs_tole32(superblock->file_max);
    superblock->attr_max    = lfs_tole32(superblock->attr_max);
}


/// Internal operations predeclared here ///
static int lfs_dir_commit(lfs_t *lfs, lfs_mdir_t *dir,
        const struct lfs_mattr *attrs, int attrcount);
static int lfs_dir_compact(lfs_t *lfs,
        lfs_mdir_t *dir, const struct lfs_mattr *attrs, int attrcount,
        lfs_mdir_t *source, uint16_t begin, uint16_t end);
static int lfs_file_outline(lfs_t *lfs, lfs_file_t *file);
static int lfs_file_flush(lfs_t *lfs, lfs_file_t *file);
static void lfs_fs_preporphans(lfs_t *lfs, int8_t orphans);
static void lfs_fs_prepmove(lfs_t *lfs,
        uint16_t id, const lfs_block_t pair[2]);
static int lfs_fs_pred(lfs_t *lfs, const lfs_block_t dir[2],
        lfs_mdir_t *pdir);
static lfs_stag_t lfs_fs_parent(lfs_t *lfs, const lfs_block_t dir[2],
        lfs_mdir_t *parent);
static int lfs_fs_relocate(lfs_t *lfs,
        const lfs_block_t oldpair[2], lfs_block_t newpair[2]);
int lfs_fs_traverseraw(lfs_t *lfs,
        int (*cb)(void *data, lfs_block_t block), void *data,
        bool includeorphans);
static int lfs_fs_forceconsistency(lfs_t *lfs);
static int lfs_deinit(lfs_t *lfs);
#ifdef LFS_MIGRATE
static int lfs1_traverse(lfs_t *lfs,
        int (*cb)(void*, lfs_block_t), void *data);
#endif

/// Block allocator ///
static int lfs_alloc_lookahead(void *p, lfs_block_t block) {
    lfs_t *lfs = (lfs_t*)p;
    lfs_block_t off = ((block - lfs->free.off)
            + lfs->cfg->block_count) % lfs->cfg->block_count;

    if (off < lfs->free.size) {
        lfs->free.buffer[off / 32] |= 1U << (off % 32);
    }

    return 0;
}

static void lfs_alloc_ack(lfs_t *lfs) {
    lfs->free.ack = lfs->cfg->block_count;
}

// Invalidate the lookahead buffer. This is done during mounting and
// failed traversals
static void lfs_alloc_reset(lfs_t *lfs) {
    lfs->free.off = lfs->seed % lfs->cfg->block_count; //lfs->free.off = lfs->seed % lfs->cfg->block_size;
    lfs->free.size = 0;
    lfs->free.i = 0;
    lfs_alloc_ack(lfs);
}

static int lfs_alloc(lfs_t *lfs, lfs_block_t *block) {
    while (true) {
        while (lfs->free.i != lfs->free.size) {
            lfs_block_t off = lfs->free.i;
            lfs->free.i += 1;
            lfs->free.ack -= 1;

            if (!(lfs->free.buffer[off / 32] & (1U << (off % 32)))) {
                // found a free block
                *block = (lfs->free.off + off) % lfs->cfg->block_count;

                // eagerly find next off so an alloc ack can
                // discredit old lookahead blocks
                while (lfs->free.i != lfs->free.size &&
                        (lfs->free.buffer[lfs->free.i / 32]
                            & (1U << (lfs->free.i % 32)))) {
                    lfs->free.i += 1;
                    lfs->free.ack -= 1;
                }

                return 0;
            }
        }

        // check if we have looked at all blocks since last ack
        if (lfs->free.ack == 0) {
            LFS_ERROR("No more free space %"PRIu32,
                    lfs->free.i + lfs->free.off);
            return LFS_ERR_NOSPC;
        }

        lfs->free.off = (lfs->free.off + lfs->free.size)
                % lfs->cfg->block_count;
        lfs->free.size = lfs_min(8*lfs->cfg->lookahead_size, lfs->free.ack);
        lfs->free.i = 0;

        // find mask of free blocks from tree
        memset(lfs->free.buffer, 0, lfs->cfg->lookahead_size);
        int err = lfs_fs_traverseraw(lfs, lfs_alloc_lookahead, lfs, true);
        if (err) {
            lfs_alloc_reset(lfs);
            return err;
        }
    }
}

/// Metadata pair and directory operations ///
static lfs_stag_t lfs_dir_getslice(lfs_t *lfs, const lfs_mdir_t *dir,
        lfs_tag_t gmask, lfs_tag_t gtag,
        lfs_off_t goff, void *gbuffer, lfs_size_t gsize) {
    lfs_off_t off = dir->off;
    lfs_tag_t ntag = dir->etag;
    lfs_stag_t gdiff = 0;

    if (lfs_gstate_hasmovehere(&lfs->gdisk, dir->pair) &&
            lfs_tag_id(gmask) != 0 &&
            lfs_tag_id(lfs->gdisk.tag) <= lfs_tag_id(gtag)) {
        // synthetic moves
        gdiff -= LFS_MKTAG(0, 1, 0);
    }

    // iterate over dir block backwards (for faster lookups)
    while (off >= sizeof(lfs_tag_t) + lfs_tag_dsize(ntag)) {
        off -= lfs_tag_dsize(ntag);
        lfs_tag_t tag = ntag;
        int err = lfs_bd_read(lfs,
                NULL, &lfs->rcache, sizeof(ntag),
                dir->pair[0], off, &ntag, sizeof(ntag));
        if (err) {
            return err;
        }

        ntag = (lfs_frombe32(ntag) ^ tag) & 0x7fffffff;

        if (lfs_tag_id(gmask) != 0 &&
                lfs_tag_type1(tag) == LFS_TYPE_SPLICE &&
                lfs_tag_id(tag) <= lfs_tag_id(gtag - gdiff)) {
            if (tag == (LFS_MKTAG(LFS_TYPE_CREATE, 0, 0) |
                    (LFS_MKTAG(0, 0x3ff, 0) & (gtag - gdiff)))) {
                // found where we were created
                return LFS_ERR_NOENT;
            }

            // move around splices
            gdiff += LFS_MKTAG(0, lfs_tag_splice(tag), 0);
        }

        if ((gmask & tag) == (gmask & (gtag - gdiff))) {
            if (lfs_tag_isdelete(tag)) {
                return LFS_ERR_NOENT;
            }

            lfs_size_t diff = lfs_min(lfs_tag_size(tag), gsize);
            err = lfs_bd_read(lfs,
                    NULL, &lfs->rcache, diff,
                    dir->pair[0], off+sizeof(tag)+goff, gbuffer, diff);
            if (err) {
                return err;
            }

            memset((uint8_t*)gbuffer + diff, 0, gsize - diff);

            return tag + gdiff;
        }
    }

    return LFS_ERR_NOENT;
}

static lfs_stag_t lfs_dir_get(lfs_t *lfs, const lfs_mdir_t *dir,
        lfs_tag_t gmask, lfs_tag_t gtag, void *buffer) {
    return lfs_dir_getslice(lfs, dir,
            gmask, gtag,
            0, buffer, lfs_tag_size(gtag));
}

static int lfs_dir_getread(lfs_t *lfs, const lfs_mdir_t *dir,
        const lfs_cache_t *pcache, lfs_cache_t *rcache, lfs_size_t hint,
        lfs_tag_t gmask, lfs_tag_t gtag,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    uint8_t *data = buffer;
    if (off+size > lfs->cfg->block_size) {
        return LFS_ERR_CORRUPT;
    }

    while (size > 0) {
        lfs_size_t diff = size;

        if (pcache && pcache->block == LFS_BLOCK_INLINE &&
                off < pcache->off + pcache->size) {
            if (off >= pcache->off) {
                // is already in pcache?
                diff = lfs_min(diff, pcache->size - (off-pcache->off));
                memcpy(data, &pcache->buffer[off-pcache->off], diff);

                data += diff;
                off += diff;
                size -= diff;
                continue;
            }

            // pcache takes priority
            diff = lfs_min(diff, pcache->off-off);
        }

        if (rcache->block == LFS_BLOCK_INLINE &&
                off < rcache->off + rcache->size) {
            if (off >= rcache->off) {
                // is already in rcache?
                diff = lfs_min(diff, rcache->size - (off-rcache->off));
                memcpy(data, &rcache->buffer[off-rcache->off], diff);

                data += diff;
                off += diff;
                size -= diff;
                continue;
            }

            // rcache takes priority
            diff = lfs_min(diff, rcache->off-off);
        }

        // load to cache, first condition can no longer fail
        rcache->block = LFS_BLOCK_INLINE;
        rcache->off = lfs_aligndown(off, lfs->cfg->read_size);
        rcache->size = lfs_min(lfs_alignup(off+hint, lfs->cfg->read_size),
                lfs->cfg->cache_size);
        int err = lfs_dir_getslice(lfs, dir, gmask, gtag,
                rcache->off, rcache->buffer, rcache->size);
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

static int lfs_dir_traverse_filter(void *p,
        lfs_tag_t tag, const void *buffer) {
    lfs_tag_t *filtertag = p;
    (void)buffer;

    // which mask depends on unique bit in tag structure
    uint32_t mask = (tag & LFS_MKTAG(0x100, 0, 0))
            ? LFS_MKTAG(0x7ff, 0x3ff, 0)
            : LFS_MKTAG(0x700, 0x3ff, 0);

    // check for redundancy
    if ((mask & tag) == (mask & *filtertag) ||
            lfs_tag_isdelete(*filtertag) ||
            (LFS_MKTAG(0x7ff, 0x3ff, 0) & tag) == (
                LFS_MKTAG(LFS_TYPE_DELETE, 0, 0) |
                    (LFS_MKTAG(0, 0x3ff, 0) & *filtertag))) {
        return true;
    }

    // check if we need to adjust for created/deleted tags
    if (lfs_tag_type1(tag) == LFS_TYPE_SPLICE &&
            lfs_tag_id(tag) <= lfs_tag_id(*filtertag)) {
        *filtertag += LFS_MKTAG(0, lfs_tag_splice(tag), 0);
    }

    return false;
}

static int lfs_dir_traverse(lfs_t *lfs,
        const lfs_mdir_t *dir, lfs_off_t off, lfs_tag_t ptag,
        const struct lfs_mattr *attrs, int attrcount,
        lfs_tag_t tmask, lfs_tag_t ttag,
        uint16_t begin, uint16_t end, int16_t diff,
        int (*cb)(void *data, lfs_tag_t tag, const void *buffer), void *data) {
    // iterate over directory and attrs
    while (true) {
        lfs_tag_t tag;
        const void *buffer;
        struct lfs_diskoff disk;
        if (off+lfs_tag_dsize(ptag) < dir->off) {
            off += lfs_tag_dsize(ptag);
            int err = lfs_bd_read(lfs,
                    NULL, &lfs->rcache, sizeof(tag),
                    dir->pair[0], off, &tag, sizeof(tag));
            if (err) {
                return err;
            }

            tag = (lfs_frombe32(tag) ^ ptag) | 0x80000000;
            disk.block = dir->pair[0];
            disk.off = off+sizeof(lfs_tag_t);
            buffer = &disk;
            ptag = tag;
        } else if (attrcount > 0) {
            tag = attrs[0].tag;
            buffer = attrs[0].buffer;
            attrs += 1;
            attrcount -= 1;
        } else {
            return 0;
        }

        lfs_tag_t mask = LFS_MKTAG(0x7ff, 0, 0);
        if ((mask & tmask & tag) != (mask & tmask & ttag)) {
            continue;
        }

        // do we need to filter? inlining the filtering logic here allows
        // for some minor optimizations
        if (lfs_tag_id(tmask) != 0) {
            // scan for duplicates and update tag based on creates/deletes
            int filter = lfs_dir_traverse(lfs,
                    dir, off, ptag, attrs, attrcount,
                    0, 0, 0, 0, 0,
                    lfs_dir_traverse_filter, &tag);
            if (filter < 0) {
                return filter;
            }

            if (filter) {
                continue;
            }

            // in filter range?
            if (!(lfs_tag_id(tag) >= begin && lfs_tag_id(tag) < end)) {
                continue;
            }
        }

        // handle special cases for mcu-side operations
        if (lfs_tag_type3(tag) == LFS_FROM_NOOP) {
            // do nothing
        } else if (lfs_tag_type3(tag) == LFS_FROM_MOVE) {
            uint16_t fromid = lfs_tag_size(tag);
            uint16_t toid = lfs_tag_id(tag);
            int err = lfs_dir_traverse(lfs,
                    buffer, 0, 0xffffffff, NULL, 0,
                    LFS_MKTAG(0x600, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_STRUCT, 0, 0),
                    fromid, fromid+1, toid-fromid+diff,
                    cb, data);
            if (err) {
                return err;
            }
        } else if (lfs_tag_type3(tag) == LFS_FROM_USERATTRS) {
            for (unsigned i = 0; i < lfs_tag_size(tag); i++) {
                const struct lfs_attr *a = buffer;
                int err = cb(data, LFS_MKTAG(LFS_TYPE_USERATTR + a[i].type,
                        lfs_tag_id(tag) + diff, a[i].size), a[i].buffer);
                if (err) {
                    return err;
                }
            }
        } else {
            int err = cb(data, tag + LFS_MKTAG(0, diff, 0), buffer);
            if (err) {
                return err;
            }
        }
    }
}

static lfs_stag_t lfs_dir_fetchmatch(lfs_t *lfs,
        lfs_mdir_t *dir, const lfs_block_t pair[2],
        lfs_tag_t fmask, lfs_tag_t ftag, uint16_t *id,
        int (*cb)(void *data, lfs_tag_t tag, const void *buffer), void *data) {
    // we can find tag very efficiently during a fetch, since we're already
    // scanning the entire directory
    lfs_stag_t besttag = -1;

    // if either block address is invalid we return LFS_ERR_CORRUPT here,
    // otherwise later writes to the pair could fail
    if (pair[0] >= lfs->cfg->block_count || pair[1] >= lfs->cfg->block_count) {
        return LFS_ERR_CORRUPT;
    }

    // find the block with the most recent revision
    uint32_t revs[2] = {0, 0};
    int r = 0;
    for (int i = 0; i < 2; i++) {
        int err = lfs_bd_read(lfs,
                NULL, &lfs->rcache, sizeof(revs[i]),
                pair[i], 0, &revs[i], sizeof(revs[i]));
        revs[i] = lfs_fromle32(revs[i]);
        if (err && err != LFS_ERR_CORRUPT) {
            return err;
        }

        if (err != LFS_ERR_CORRUPT &&
                lfs_scmp(revs[i], revs[(i+1)%2]) > 0) {
            r = i;
        }
    }

    dir->pair[0] = pair[(r+0)%2];
    dir->pair[1] = pair[(r+1)%2];
    dir->rev = revs[(r+0)%2];
    dir->off = 0; // nonzero = found some commits

    // now scan tags to fetch the actual dir and find possible match
    for (int i = 0; i < 2; i++) {
        lfs_off_t off = 0;
        lfs_tag_t ptag = 0xffffffff;

        uint16_t tempcount = 0;
        lfs_block_t temptail[2] = {LFS_BLOCK_NULL, LFS_BLOCK_NULL};
        bool tempsplit = false;
        lfs_stag_t tempbesttag = besttag;

        dir->rev = lfs_tole32(dir->rev);
        uint32_t crc = lfs_crc(0xffffffff, &dir->rev, sizeof(dir->rev));
        dir->rev = lfs_fromle32(dir->rev);

        while (true) {
            // extract next tag
            lfs_tag_t tag;
            off += lfs_tag_dsize(ptag);
            int err = lfs_bd_read(lfs,
                    NULL, &lfs->rcache, lfs->cfg->block_size,
                    dir->pair[0], off, &tag, sizeof(tag));
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    // can't continue?
                    dir->erased = false;
                    break;
                }
                return err;
            }

            crc = lfs_crc(crc, &tag, sizeof(tag));
            tag = lfs_frombe32(tag) ^ ptag;

            // next commit not yet programmed or we're not in valid range
            if (!lfs_tag_isvalid(tag)) {
                dir->erased = (lfs_tag_type1(ptag) == LFS_TYPE_CRC &&
                        dir->off % lfs->cfg->prog_size == 0);
                break;
            } else if (off + lfs_tag_dsize(tag) > lfs->cfg->block_size) {
                dir->erased = false;
                break;
            }

            ptag = tag;

            if (lfs_tag_type1(tag) == LFS_TYPE_CRC) {
                // check the crc attr
                uint32_t dcrc;
                err = lfs_bd_read(lfs,
                        NULL, &lfs->rcache, lfs->cfg->block_size,
                        dir->pair[0], off+sizeof(tag), &dcrc, sizeof(dcrc));
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        dir->erased = false;
                        break;
                    }
                    return err;
                }
                dcrc = lfs_fromle32(dcrc);

                if (crc != dcrc) {
                    dir->erased = false;
                    break;
                }

                // reset the next bit if we need to
                ptag ^= (lfs_tag_t)(lfs_tag_chunk(tag) & 1U) << 31;

                // toss our crc into the filesystem seed for
                // pseudorandom numbers
                lfs->seed ^= crc;

                // update with what's found so far
                besttag = tempbesttag;
                dir->off = off + lfs_tag_dsize(tag);
                dir->etag = ptag;
                dir->count = tempcount;
                dir->tail[0] = temptail[0];
                dir->tail[1] = temptail[1];
                dir->split = tempsplit;

                // reset crc
                crc = 0xffffffff;
                continue;
            }

            // crc the entry first, hopefully leaving it in the cache
            for (lfs_off_t j = sizeof(tag); j < lfs_tag_dsize(tag); j++) {
                uint8_t dat;
                err = lfs_bd_read(lfs,
                        NULL, &lfs->rcache, lfs->cfg->block_size,
                        dir->pair[0], off+j, &dat, 1);
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        dir->erased = false;
                        break;
                    }
                    return err;
                }

                crc = lfs_crc(crc, &dat, 1);
            }

            // directory modification tags?
            if (lfs_tag_type1(tag) == LFS_TYPE_NAME) {
                // increase count of files if necessary
                if (lfs_tag_id(tag) >= tempcount) {
                    tempcount = lfs_tag_id(tag) + 1;
                }
            } else if (lfs_tag_type1(tag) == LFS_TYPE_SPLICE) {
                tempcount += lfs_tag_splice(tag);

                if (tag == (LFS_MKTAG(LFS_TYPE_DELETE, 0, 0) |
                        (LFS_MKTAG(0, 0x3ff, 0) & tempbesttag))) {
                    tempbesttag |= 0x80000000;
                } else if (tempbesttag != -1 &&
                        lfs_tag_id(tag) <= lfs_tag_id(tempbesttag)) {
                    tempbesttag += LFS_MKTAG(0, lfs_tag_splice(tag), 0);
                }
            } else if (lfs_tag_type1(tag) == LFS_TYPE_TAIL) {
                tempsplit = (lfs_tag_chunk(tag) & 1);

                err = lfs_bd_read(lfs,
                        NULL, &lfs->rcache, lfs->cfg->block_size,
                        dir->pair[0], off+sizeof(tag), &temptail, 8);
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        dir->erased = false;
                        break;
                    }
                }
                lfs_pair_fromle32(temptail);
            }

            // found a match for our fetcher?
            if ((fmask & tag) == (fmask & ftag)) {
                int res = cb(data, tag, &(struct lfs_diskoff){
                        dir->pair[0], off+sizeof(tag)});
                if (res < 0) {
                    if (res == LFS_ERR_CORRUPT) {
                        dir->erased = false;
                        break;
                    }
                    return res;
                }

                if (res == LFS_CMP_EQ) {
                    // found a match
                    tempbesttag = tag;
                } else if ((LFS_MKTAG(0x7ff, 0x3ff, 0) & tag) ==
                        (LFS_MKTAG(0x7ff, 0x3ff, 0) & tempbesttag)) {
                    // found an identical tag, but contents didn't match
                    // this must mean that our besttag has been overwritten
                    tempbesttag = -1;
                } else if (res == LFS_CMP_GT &&
                        lfs_tag_id(tag) <= lfs_tag_id(tempbesttag)) {
                    // found a greater match, keep track to keep things sorted
                    tempbesttag = tag | 0x80000000;
                }
            }
        }

        // consider what we have good enough
        if (dir->off > 0) {
            // synthetic move
            if (lfs_gstate_hasmovehere(&lfs->gdisk, dir->pair)) {
                if (lfs_tag_id(lfs->gdisk.tag) == lfs_tag_id(besttag)) {
                    besttag |= 0x80000000;
                } else if (besttag != -1 &&
                        lfs_tag_id(lfs->gdisk.tag) < lfs_tag_id(besttag)) {
                    besttag -= LFS_MKTAG(0, 1, 0);
                }
            }

            // found tag? or found best id?
            if (id) {
                *id = lfs_min(lfs_tag_id(besttag), dir->count);
            }

            if (lfs_tag_isvalid(besttag)) {
                return besttag;
            } else if (lfs_tag_id(besttag) < dir->count) {
                return LFS_ERR_NOENT;
            } else {
                return 0;
            }
        }

        // failed, try the other block?
        lfs_pair_swap(dir->pair);
        dir->rev = revs[(r+1)%2];
    }

    LFS_ERROR("Corrupted dir pair at {0x%"PRIx32", 0x%"PRIx32"}",
            dir->pair[0], dir->pair[1]);
    return LFS_ERR_CORRUPT;
}

static int lfs_dir_fetch(lfs_t *lfs,
        lfs_mdir_t *dir, const lfs_block_t pair[2]) {
    // note, mask=-1, tag=-1 can never match a tag since this
    // pattern has the invalid bit set
    return (int)lfs_dir_fetchmatch(lfs, dir, pair,
            (lfs_tag_t)-1, (lfs_tag_t)-1, NULL, NULL, NULL);
}

static int lfs_dir_getgstate(lfs_t *lfs, const lfs_mdir_t *dir,
        lfs_gstate_t *gstate) {
    lfs_gstate_t temp;
    lfs_stag_t res = lfs_dir_get(lfs, dir, LFS_MKTAG(0x7ff, 0, 0),
            LFS_MKTAG(LFS_TYPE_MOVESTATE, 0, sizeof(temp)), &temp);
    if (res < 0 && res != LFS_ERR_NOENT) {
        return res;
    }

    if (res != LFS_ERR_NOENT) {
        // xor together to find resulting gstate
        lfs_gstate_fromle32(&temp);
        lfs_gstate_xor(gstate, &temp);
    }

    return 0;
}

static int lfs_dir_getinfo(lfs_t *lfs, lfs_mdir_t *dir,
        uint16_t id, struct lfs_info *info) {
    if (id == 0x3ff) {
        // special case for root
        strcpy(info->name, "/");
        info->type = LFS_TYPE_DIR;
        return 0;
    }

    lfs_stag_t tag = lfs_dir_get(lfs, dir, LFS_MKTAG(0x780, 0x3ff, 0),
            LFS_MKTAG(LFS_TYPE_NAME, id, lfs->name_max+1), info->name);
    if (tag < 0) {
        return (int)tag;
    }

    info->type = lfs_tag_type3(tag);

    struct lfs_ctz ctz;
    tag = lfs_dir_get(lfs, dir, LFS_MKTAG(0x700, 0x3ff, 0),
            LFS_MKTAG(LFS_TYPE_STRUCT, id, sizeof(ctz)), &ctz);
    if (tag < 0) {
        return (int)tag;
    }
    lfs_ctz_fromle32(&ctz);

    if (lfs_tag_type3(tag) == LFS_TYPE_CTZSTRUCT) {
        info->size = ctz.size;
    } else if (lfs_tag_type3(tag) == LFS_TYPE_INLINESTRUCT) {
        info->size = lfs_tag_size(tag);
    }

    return 0;
}

struct lfs_dir_find_match {
    lfs_t *lfs;
    const void *name;
    lfs_size_t size;
};

static int lfs_dir_find_match(void *data,
        lfs_tag_t tag, const void *buffer) {
    struct lfs_dir_find_match *name = data;
    lfs_t *lfs = name->lfs;
    const struct lfs_diskoff *disk = buffer;

    // compare with disk
    lfs_size_t diff = lfs_min(name->size, lfs_tag_size(tag));
    int res = lfs_bd_cmp(lfs,
            NULL, &lfs->rcache, diff,
            disk->block, disk->off, name->name, diff);
    if (res != LFS_CMP_EQ) {
        return res;
    }

    // only equal if our size is still the same
    if (name->size != lfs_tag_size(tag)) {
        return (name->size < lfs_tag_size(tag)) ? LFS_CMP_LT : LFS_CMP_GT;
    }

    // found a match!
    return LFS_CMP_EQ;
}

static lfs_stag_t lfs_dir_find(lfs_t *lfs, lfs_mdir_t *dir,
        const char **path, uint16_t *id) {
    // we reduce path to a single name if we can find it
    const char *name = *path;
    if (id) {
        *id = 0x3ff;
    }

    // default to root dir
    lfs_stag_t tag = LFS_MKTAG(LFS_TYPE_DIR, 0x3ff, 0);
    dir->tail[0] = lfs->root[0];
    dir->tail[1] = lfs->root[1];

    while (true) {
nextname:
        // skip slashes
        name += strspn(name, "/");
        lfs_size_t namelen = strcspn(name, "/");

        // skip '.' and root '..'
        if ((namelen == 1 && memcmp(name, ".", 1) == 0) ||
            (namelen == 2 && memcmp(name, "..", 2) == 0)) {
            name += namelen;
            goto nextname;
        }

        // skip if matched by '..' in name
        const char *suffix = name + namelen;
        lfs_size_t sufflen;
        int depth = 1;
        while (true) {
            suffix += strspn(suffix, "/");
            sufflen = strcspn(suffix, "/");
            if (sufflen == 0) {
                break;
            }

            if (sufflen == 2 && memcmp(suffix, "..", 2) == 0) {
                depth -= 1;
                if (depth == 0) {
                    name = suffix + sufflen;
                    goto nextname;
                }
            } else {
                depth += 1;
            }

            suffix += sufflen;
        }

        // found path
        if (name[0] == '\0') {
            return tag;
        }

        // update what we've found so far
        *path = name;

        // only continue if we hit a directory
        if (lfs_tag_type3(tag) != LFS_TYPE_DIR) {
            return LFS_ERR_NOTDIR;
        }

        // grab the entry data
        if (lfs_tag_id(tag) != 0x3ff) {
            lfs_stag_t res = lfs_dir_get(lfs, dir, LFS_MKTAG(0x700, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_STRUCT, lfs_tag_id(tag), 8), dir->tail);
            if (res < 0) {
                return res;
            }
            lfs_pair_fromle32(dir->tail);
        }

        // find entry matching name
        while (true) {
            tag = lfs_dir_fetchmatch(lfs, dir, dir->tail,
                    LFS_MKTAG(0x780, 0, 0),
                    LFS_MKTAG(LFS_TYPE_NAME, 0, namelen),
                     // are we last name?
                    (strchr(name, '/') == NULL) ? id : NULL,
                    lfs_dir_find_match, &(struct lfs_dir_find_match){
                        lfs, name, namelen});
            if (tag < 0) {
                return tag;
            }

            if (tag) {
                break;
            }

            if (!dir->split) {
                return LFS_ERR_NOENT;
            }
        }

        // to next name
        name += namelen;
    }
}

// commit logic
struct lfs_commit {
    lfs_block_t block;
    lfs_off_t off;
    lfs_tag_t ptag;
    uint32_t crc;

    lfs_off_t begin;
    lfs_off_t end;
};

static int lfs_dir_commitprog(lfs_t *lfs, struct lfs_commit *commit,
        const void *buffer, lfs_size_t size) {
    int err = lfs_bd_prog(lfs,
            &lfs->pcache, &lfs->rcache, false,
            commit->block, commit->off ,
            (const uint8_t*)buffer, size);
    if (err) {
        return err;
    }

    commit->crc = lfs_crc(commit->crc, buffer, size);
    commit->off += size;
    return 0;
}

static int lfs_dir_commitattr(lfs_t *lfs, struct lfs_commit *commit,
        lfs_tag_t tag, const void *buffer) {
    // check if we fit
    lfs_size_t dsize = lfs_tag_dsize(tag);
    if (commit->off + dsize > commit->end) {
        return LFS_ERR_NOSPC;
    }

    // write out tag
    lfs_tag_t ntag = lfs_tobe32((tag & 0x7fffffff) ^ commit->ptag);
    int err = lfs_dir_commitprog(lfs, commit, &ntag, sizeof(ntag));
    if (err) {
        return err;
    }

    if (!(tag & 0x80000000)) {
        // from memory
        err = lfs_dir_commitprog(lfs, commit, buffer, dsize-sizeof(tag));
        if (err) {
            return err;
        }
    } else {
        // from disk
        const struct lfs_diskoff *disk = buffer;
        for (lfs_off_t i = 0; i < dsize-sizeof(tag); i++) {
            // rely on caching to make this efficient
            uint8_t dat;
            err = lfs_bd_read(lfs,
                    NULL, &lfs->rcache, dsize-sizeof(tag)-i,
                    disk->block, disk->off+i, &dat, 1);
            if (err) {
                return err;
            }

            err = lfs_dir_commitprog(lfs, commit, &dat, 1);
            if (err) {
                return err;
            }
        }
    }

    commit->ptag = tag & 0x7fffffff;
    return 0;
}

static int lfs_dir_commitcrc(lfs_t *lfs, struct lfs_commit *commit) {
    const lfs_off_t off1 = commit->off;
    const uint32_t crc1 = commit->crc;
    // align to program units
    const lfs_off_t end = lfs_alignup(off1 + 2*sizeof(uint32_t),
            lfs->cfg->prog_size);

    // create crc tags to fill up remainder of commit, note that
    // padding is not crced, which lets fetches skip padding but
    // makes committing a bit more complicated
    while (commit->off < end) {
        lfs_off_t off = commit->off + sizeof(lfs_tag_t);
        lfs_off_t noff = lfs_min(end - off, 0x3fe) + off;
        if (noff < end) {
            noff = lfs_min(noff, end - 2*sizeof(uint32_t));
        }

        // read erased state from next program unit
        lfs_tag_t tag = 0xffffffff;
        int err = lfs_bd_read(lfs,
                NULL, &lfs->rcache, sizeof(tag),
                commit->block, noff, &tag, sizeof(tag));
        if (err && err != LFS_ERR_CORRUPT) {
            return err;
        }

        // build crc tag
        bool reset = ~lfs_frombe32(tag) >> 31;
        tag = LFS_MKTAG(LFS_TYPE_CRC + reset, 0x3ff, noff - off);

        // write out crc
        uint32_t footer[2];
        footer[0] = lfs_tobe32(tag ^ commit->ptag);
        commit->crc = lfs_crc(commit->crc, &footer[0], sizeof(footer[0]));
        footer[1] = lfs_tole32(commit->crc);
        err = lfs_bd_prog(lfs,
                &lfs->pcache, &lfs->rcache, false,
                commit->block, commit->off, &footer, sizeof(footer));
        if (err) {
            return err;
        }

        commit->off += sizeof(tag)+lfs_tag_size(tag);
        commit->ptag = tag ^ ((lfs_tag_t)reset << 31);
        commit->crc = 0xffffffff; // reset crc for next "commit"
    }

    // flush buffers
    int err = lfs_bd_sync(lfs, &lfs->pcache, &lfs->rcache, false);
    if (err) {
        return err;
    }

    // successful commit, check checksums to make sure
    lfs_off_t off = commit->begin;
    lfs_off_t noff = off1 + sizeof(uint32_t);
    while (off < end) {
        uint32_t crc = 0xffffffff;
        for (lfs_off_t i = off; i < noff+sizeof(uint32_t); i++) {
            // check against written crc, may catch blocks that
            // become readonly and match our commit size exactly
            if (i == off1 && crc != crc1) {
                return LFS_ERR_CORRUPT;
            }

            // leave it up to caching to make this efficient
            uint8_t dat;
            err = lfs_bd_read(lfs,
                    NULL, &lfs->rcache, noff+sizeof(uint32_t)-i,
                    commit->block, i, &dat, 1);
            if (err) {
                return err;
            }

            crc = lfs_crc(crc, &dat, 1);
        }

        // detected write error?
        if (crc != 0) {
            return LFS_ERR_CORRUPT;
        }

        // skip padding
        off = lfs_min(end - noff, 0x3fe) + noff;
        if (off < end) {
            off = lfs_min(off, end - 2*sizeof(uint32_t));
        }
        noff = off + sizeof(uint32_t);
    }

    return 0;
}

static int lfs_dir_alloc(lfs_t *lfs, lfs_mdir_t *dir) {
    // allocate pair of dir blocks (backwards, so we write block 1 first)
    for (int i = 0; i < 2; i++) {
        int err = lfs_alloc(lfs, &dir->pair[(i+1)%2]);
        if (err) {
            return err;
        }
    }

    // zero for reproducability in case initial block is unreadable
    dir->rev = 0;

    // rather than clobbering one of the blocks we just pretend
    // the revision may be valid
    int err = lfs_bd_read(lfs,
            NULL, &lfs->rcache, sizeof(dir->rev),
            dir->pair[0], 0, &dir->rev, sizeof(dir->rev));
    dir->rev = lfs_fromle32(dir->rev);
    if (err && err != LFS_ERR_CORRUPT) {
        return err;
    }

    // make sure we don't immediately evict
    dir->rev += dir->rev & 1;

    // set defaults
    dir->off = sizeof(dir->rev);
    dir->etag = 0xffffffff;
    dir->count = 0;
    dir->tail[0] = LFS_BLOCK_NULL;
    dir->tail[1] = LFS_BLOCK_NULL;
    dir->erased = false;
    dir->split = false;

    // don't write out yet, let caller take care of that
    return 0;
}

static int lfs_dir_drop(lfs_t *lfs, lfs_mdir_t *dir, lfs_mdir_t *tail) {
    // steal state
    int err = lfs_dir_getgstate(lfs, tail, &lfs->gdelta);
    if (err) {
        return err;
    }

    // steal tail
    lfs_pair_tole32(tail->tail);
    err = lfs_dir_commit(lfs, dir, LFS_MKATTRS(
            {LFS_MKTAG(LFS_TYPE_TAIL + tail->split, 0x3ff, 8), tail->tail}));
    lfs_pair_fromle32(tail->tail);
    if (err) {
        return err;
    }

    return 0;
}

static int lfs_dir_split(lfs_t *lfs,
        lfs_mdir_t *dir, const struct lfs_mattr *attrs, int attrcount,
        lfs_mdir_t *source, uint16_t split, uint16_t end) {
    // create tail directory
    lfs_alloc_ack(lfs);
    lfs_mdir_t tail;
    int err = lfs_dir_alloc(lfs, &tail);
    if (err) {
        return err;
    }

    tail.split = dir->split;
    tail.tail[0] = dir->tail[0];
    tail.tail[1] = dir->tail[1];

    err = lfs_dir_compact(lfs, &tail, attrs, attrcount, source, split, end);
    if (err) {
        return err;
    }

    dir->tail[0] = tail.pair[0];
    dir->tail[1] = tail.pair[1];
    dir->split = true;

    // update root if needed
    if (lfs_pair_cmp(dir->pair, lfs->root) == 0 && split == 0) {
        lfs->root[0] = tail.pair[0];
        lfs->root[1] = tail.pair[1];
    }

    return 0;
}

static int lfs_dir_commit_size(void *p, lfs_tag_t tag, const void *buffer) {
    lfs_size_t *size = p;
    (void)buffer;

    *size += lfs_tag_dsize(tag);
    return 0;
}

struct lfs_dir_commit_commit {
    lfs_t *lfs;
    struct lfs_commit *commit;
};

static int lfs_dir_commit_commit(void *p, lfs_tag_t tag, const void *buffer) {
    struct lfs_dir_commit_commit *commit = p;
    return lfs_dir_commitattr(commit->lfs, commit->commit, tag, buffer);
}

static int lfs_dir_compact(lfs_t *lfs,
        lfs_mdir_t *dir, const struct lfs_mattr *attrs, int attrcount,
        lfs_mdir_t *source, uint16_t begin, uint16_t end) {
    // save some state in case block is bad
    const lfs_block_t oldpair[2] = {dir->pair[0], dir->pair[1]};
    bool relocated = false;
    bool tired = false;

    // should we split?
    while (end - begin > 1) {
        // find size
        lfs_size_t size = 0;
        int err = lfs_dir_traverse(lfs,
                source, 0, 0xffffffff, attrs, attrcount,
                LFS_MKTAG(0x400, 0x3ff, 0),
                LFS_MKTAG(LFS_TYPE_NAME, 0, 0),
                begin, end, -begin,
                lfs_dir_commit_size, &size);
        if (err) {
            return err;
        }

        // space is complicated, we need room for tail, crc, gstate,
        // cleanup delete, and we cap at half a block to give room
        // for metadata updates.
        if (end - begin < 0xff &&
                size <= lfs_min(lfs->cfg->block_size - 36,
                    lfs_alignup(lfs->cfg->block_size/2,
                        lfs->cfg->prog_size))) {
            break;
        }

        // can't fit, need to split, we should really be finding the
        // largest size that fits with a small binary search, but right now
        // it's not worth the code size
        uint16_t split = (end - begin) / 2;
        err = lfs_dir_split(lfs, dir, attrs, attrcount,
                source, begin+split, end);
        if (err) {
            // if we fail to split, we may be able to overcompact, unless
            // we're too big for even the full block, in which case our
            // only option is to error
            if (err == LFS_ERR_NOSPC && size <= lfs->cfg->block_size - 36) {
                break;
            }
            return err;
        }

        end = begin + split;
    }

    // increment revision count
    dir->rev += 1;
    // If our revision count == n * block_cycles, we should force a relocation,
    // this is how littlefs wear-levels at the metadata-pair level. Note that we
    // actually use (block_cycles+1)|1, this is to avoid two corner cases:
    // 1. block_cycles = 1, which would prevent relocations from terminating
    // 2. block_cycles = 2n, which, due to aliasing, would only ever relocate
    //    one metadata block in the pair, effectively making this useless
    if (lfs->cfg->block_cycles > 0 &&
            (dir->rev % ((lfs->cfg->block_cycles+1)|1) == 0)) {
        if (lfs_pair_cmp(dir->pair, (const lfs_block_t[2]){0, 1}) == 0) {
            // oh no! we're writing too much to the superblock,
            // should we expand?
            lfs_ssize_t res = lfs_fs_size(lfs);
            if (res < 0) {
                return res;
            }

            // do we have extra space? littlefs can't reclaim this space
            // by itself, so expand cautiously
            if ((lfs_size_t)res < lfs->cfg->block_count/2) {
                LFS_DEBUG("Expanding superblock at rev %"PRIu32, dir->rev);
                int err = lfs_dir_split(lfs, dir, attrs, attrcount,
                        source, begin, end);
                if (err && err != LFS_ERR_NOSPC) {
                    return err;
                }

                // welp, we tried, if we ran out of space there's not much
                // we can do, we'll error later if we've become frozen
                if (!err) {
                    end = begin;
                }
            }
#ifdef LFS_MIGRATE
        } else if (lfs->lfs1) {
            // do not proactively relocate blocks during migrations, this
            // can cause a number of failure states such: clobbering the
            // v1 superblock if we relocate root, and invalidating directory
            // pointers if we relocate the head of a directory. On top of
            // this, relocations increase the overall complexity of
            // lfs_migration, which is already a delicate operation.
#endif
        } else {
            // we're writing too much, time to relocate
            tired = true;
            goto relocate;
        }
    }

    // begin loop to commit compaction to blocks until a compact sticks
    while (true) {
        {
            // setup commit state
            struct lfs_commit commit = {
                .block = dir->pair[1],
                .off = 0,
                .ptag = 0xffffffff,
                .crc = 0xffffffff,

                .begin = 0,
                .end = lfs->cfg->block_size - 8,
            };

            // erase block to write to
            int err = lfs_bd_erase(lfs, dir->pair[1]);
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                return err;
            }

            // write out header
            dir->rev = lfs_tole32(dir->rev);
            err = lfs_dir_commitprog(lfs, &commit,
                    &dir->rev, sizeof(dir->rev));
            dir->rev = lfs_fromle32(dir->rev);
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                return err;
            }

            // traverse the directory, this time writing out all unique tags
            err = lfs_dir_traverse(lfs,
                    source, 0, 0xffffffff, attrs, attrcount,
                    LFS_MKTAG(0x400, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_NAME, 0, 0),
                    begin, end, -begin,
                    lfs_dir_commit_commit, &(struct lfs_dir_commit_commit){
                        lfs, &commit});
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                return err;
            }

            // commit tail, which may be new after last size check
            if (!lfs_pair_isnull(dir->tail)) {
                lfs_pair_tole32(dir->tail);
                err = lfs_dir_commitattr(lfs, &commit,
                        LFS_MKTAG(LFS_TYPE_TAIL + dir->split, 0x3ff, 8),
                        dir->tail);
                lfs_pair_fromle32(dir->tail);
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        goto relocate;
                    }
                    return err;
                }
            }

            // bring over gstate?
            lfs_gstate_t delta = {0};
            if (!relocated) {
                lfs_gstate_xor(&delta, &lfs->gdisk);
                lfs_gstate_xor(&delta, &lfs->gstate);
            }
            lfs_gstate_xor(&delta, &lfs->gdelta);
            delta.tag &= ~LFS_MKTAG(0, 0, 0x3ff);

            err = lfs_dir_getgstate(lfs, dir, &delta);
            if (err) {
                return err;
            }

            if (!lfs_gstate_iszero(&delta)) {
                lfs_gstate_tole32(&delta);
                err = lfs_dir_commitattr(lfs, &commit,
                        LFS_MKTAG(LFS_TYPE_MOVESTATE, 0x3ff,
                            sizeof(delta)), &delta);
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        goto relocate;
                    }
                    return err;
                }
            }

            // complete commit with crc
            err = lfs_dir_commitcrc(lfs, &commit);
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                return err;
            }

            // successful compaction, swap dir pair to indicate most recent
            LFS_ASSERT(commit.off % lfs->cfg->prog_size == 0);
            lfs_pair_swap(dir->pair);
            dir->count = end - begin;
            dir->off = commit.off;
            dir->etag = commit.ptag;
            // update gstate
            lfs->gdelta = (lfs_gstate_t){0};
            if (!relocated) {
                lfs->gdisk = lfs->gstate;
            }
        }
        break;

relocate:
        // commit was corrupted, drop caches and prepare to relocate block
        relocated = true;
        lfs_cache_drop(lfs, &lfs->pcache);
        if (!tired) {
            LFS_DEBUG("Bad block at 0x%"PRIx32, dir->pair[1]);
        }

        // can't relocate superblock, filesystem is now frozen
        if (lfs_pair_cmp(dir->pair, (const lfs_block_t[2]){0, 1}) == 0) {
            LFS_WARN("Superblock 0x%"PRIx32" has become unwritable",
                    dir->pair[1]);
            return LFS_ERR_NOSPC;
        }

        // relocate half of pair
        int err = lfs_alloc(lfs, &dir->pair[1]);
        if (err && (err != LFS_ERR_NOSPC || !tired)) {
            return err;
        }

        tired = false;
        continue;
    }

    if (relocated) {
        // update references if we relocated
        LFS_DEBUG("Relocating {0x%"PRIx32", 0x%"PRIx32"} "
                    "-> {0x%"PRIx32", 0x%"PRIx32"}",
                oldpair[0], oldpair[1], dir->pair[0], dir->pair[1]);
        int err = lfs_fs_relocate(lfs, oldpair, dir->pair);
        if (err) {
            return err;
        }
    }

    return 0;
}

static int lfs_dir_commit(lfs_t *lfs, lfs_mdir_t *dir,
        const struct lfs_mattr *attrs, int attrcount) {
    // check for any inline files that aren't RAM backed and
    // forcefully evict them, needed for filesystem consistency
    for (lfs_file_t *f = (lfs_file_t*)lfs->mlist; f; f = f->next) {
        if (dir != &f->m && lfs_pair_cmp(f->m.pair, dir->pair) == 0 &&
                f->type == LFS_TYPE_REG && (f->flags & LFS_F_INLINE) &&
                f->ctz.size > lfs->cfg->cache_size) {
            int err = lfs_file_outline(lfs, f);
            if (err) {
                return err;
            }

            err = lfs_file_flush(lfs, f);
            if (err) {
                return err;
            }
        }
    }

    // calculate changes to the directory
    lfs_mdir_t olddir = *dir;
    bool hasdelete = false;
    for (int i = 0; i < attrcount; i++) {
        if (lfs_tag_type3(attrs[i].tag) == LFS_TYPE_CREATE) {
            dir->count += 1;
        } else if (lfs_tag_type3(attrs[i].tag) == LFS_TYPE_DELETE) {
            LFS_ASSERT(dir->count > 0);
            dir->count -= 1;
            hasdelete = true;
        } else if (lfs_tag_type1(attrs[i].tag) == LFS_TYPE_TAIL) {
            dir->tail[0] = ((lfs_block_t*)attrs[i].buffer)[0];
            dir->tail[1] = ((lfs_block_t*)attrs[i].buffer)[1];
            dir->split = (lfs_tag_chunk(attrs[i].tag) & 1);
            lfs_pair_fromle32(dir->tail);
        }
    }

    // should we actually drop the directory block?
    if (hasdelete && dir->count == 0) {
        lfs_mdir_t pdir;
        int err = lfs_fs_pred(lfs, dir->pair, &pdir);
        if (err && err != LFS_ERR_NOENT) {
            *dir = olddir;
            return err;
        }

        if (err != LFS_ERR_NOENT && pdir.split) {
            err = lfs_dir_drop(lfs, &pdir, dir);
            if (err) {
                *dir = olddir;
                return err;
            }
        }
    }

    if (dir->erased || dir->count >= 0xff) {
        // try to commit
        struct lfs_commit commit = {
            .block = dir->pair[0],
            .off = dir->off,
            .ptag = dir->etag,
            .crc = 0xffffffff,

            .begin = dir->off,
            .end = lfs->cfg->block_size - 8,
        };

        // traverse attrs that need to be written out
        lfs_pair_tole32(dir->tail);
        int err = lfs_dir_traverse(lfs,
                dir, dir->off, dir->etag, attrs, attrcount,
                0, 0, 0, 0, 0,
                lfs_dir_commit_commit, &(struct lfs_dir_commit_commit){
                    lfs, &commit});
        lfs_pair_fromle32(dir->tail);
        if (err) {
            if (err == LFS_ERR_NOSPC || err == LFS_ERR_CORRUPT) {
                goto compact;
            }
            *dir = olddir;
            return err;
        }

        // commit any global diffs if we have any
        lfs_gstate_t delta = {0};
        lfs_gstate_xor(&delta, &lfs->gstate);
        lfs_gstate_xor(&delta, &lfs->gdisk);
        lfs_gstate_xor(&delta, &lfs->gdelta);
        delta.tag &= ~LFS_MKTAG(0, 0, 0x3ff);
        if (!lfs_gstate_iszero(&delta)) {
            err = lfs_dir_getgstate(lfs, dir, &delta);
            if (err) {
                *dir = olddir;
                return err;
            }

            lfs_gstate_tole32(&delta);
            err = lfs_dir_commitattr(lfs, &commit,
                    LFS_MKTAG(LFS_TYPE_MOVESTATE, 0x3ff,
                        sizeof(delta)), &delta);
            if (err) {
                if (err == LFS_ERR_NOSPC || err == LFS_ERR_CORRUPT) {
                    goto compact;
                }
                *dir = olddir;
                return err;
            }
        }

        // finalize commit with the crc
        err = lfs_dir_commitcrc(lfs, &commit);
        if (err) {
            if (err == LFS_ERR_NOSPC || err == LFS_ERR_CORRUPT) {
                goto compact;
            }
            *dir = olddir;
            return err;
        }

        // successful commit, update dir
        LFS_ASSERT(commit.off % lfs->cfg->prog_size == 0);
        dir->off = commit.off;
        dir->etag = commit.ptag;
        // and update gstate
        lfs->gdisk = lfs->gstate;
        lfs->gdelta = (lfs_gstate_t){0};
    } else {
compact:
        // fall back to compaction
        lfs_cache_drop(lfs, &lfs->pcache);

        int err = lfs_dir_compact(lfs, dir, attrs, attrcount,
                dir, 0, dir->count);
        if (err) {
            *dir = olddir;
            return err;
        }
    }

    // this complicated bit of logic is for fixing up any active
    // metadata-pairs that we may have affected
    //
    // note we have to make two passes since the mdir passed to
    // lfs_dir_commit could also be in this list, and even then
    // we need to copy the pair so they don't get clobbered if we refetch
    // our mdir.
    for (struct lfs_mlist *d = lfs->mlist; d; d = d->next) {
        if (&d->m != dir && lfs_pair_cmp(d->m.pair, olddir.pair) == 0) {
            d->m = *dir;
            for (int i = 0; i < attrcount; i++) {
                if (lfs_tag_type3(attrs[i].tag) == LFS_TYPE_DELETE &&
                        d->id == lfs_tag_id(attrs[i].tag)) {
                    d->m.pair[0] = LFS_BLOCK_NULL;
                    d->m.pair[1] = LFS_BLOCK_NULL;
                } else if (lfs_tag_type3(attrs[i].tag) == LFS_TYPE_DELETE &&
                        d->id > lfs_tag_id(attrs[i].tag)) {
                    d->id -= 1;
                    if (d->type == LFS_TYPE_DIR) {
                        ((lfs_dir_t*)d)->pos -= 1;
                    }
                } else if (lfs_tag_type3(attrs[i].tag) == LFS_TYPE_CREATE &&
                        d->id >= lfs_tag_id(attrs[i].tag)) {
                    d->id += 1;
                    if (d->type == LFS_TYPE_DIR) {
                        ((lfs_dir_t*)d)->pos += 1;
                    }
                }
            }
        }
    }

    for (struct lfs_mlist *d = lfs->mlist; d; d = d->next) {
        if (lfs_pair_cmp(d->m.pair, olddir.pair) == 0) {
            while (d->id >= d->m.count && d->m.split) {
                // we split and id is on tail now
                d->id -= d->m.count;
                int err = lfs_dir_fetch(lfs, &d->m, d->m.tail);
                if (err) {
                    return err;
                }
            }
        }
    }

    return 0;
}


/// Top level directory operations ///
int lfs_mkdir(lfs_t *lfs, const char *path) {
    LFS_TRACE("lfs_mkdir(%p, \"%s\")", (void*)lfs, path);
    // deorphan if we haven't yet, needed at most once after poweron
    int err = lfs_fs_forceconsistency(lfs);
    if (err) {
        LFS_TRACE("lfs_mkdir -> %d", err);
        return err;
    }

    struct lfs_mlist cwd;
    cwd.next = lfs->mlist;
    uint16_t id;
    err = lfs_dir_find(lfs, &cwd.m, &path, &id);
    if (!(err == LFS_ERR_NOENT && id != 0x3ff)) {
        LFS_TRACE("lfs_mkdir -> %d", (err < 0) ? err : LFS_ERR_EXIST);
        return (err < 0) ? err : LFS_ERR_EXIST;
    }

    // check that name fits
    lfs_size_t nlen = strlen(path);
    if (nlen > lfs->name_max) {
        LFS_TRACE("lfs_mkdir -> %d", LFS_ERR_NAMETOOLONG);
        return LFS_ERR_NAMETOOLONG;
    }

    // build up new directory
    lfs_alloc_ack(lfs);
    lfs_mdir_t dir;
    err = lfs_dir_alloc(lfs, &dir);
    if (err) {
        LFS_TRACE("lfs_mkdir -> %d", err);
        return err;
    }

    // find end of list
    lfs_mdir_t pred = cwd.m;
    while (pred.split) {
        err = lfs_dir_fetch(lfs, &pred, pred.tail);
        if (err) {
            LFS_TRACE("lfs_mkdir -> %d", err);
            return err;
        }
    }

    // setup dir
    lfs_pair_tole32(pred.tail);
    err = lfs_dir_commit(lfs, &dir, LFS_MKATTRS(
            {LFS_MKTAG(LFS_TYPE_SOFTTAIL, 0x3ff, 8), pred.tail}));
    lfs_pair_fromle32(pred.tail);
    if (err) {
        LFS_TRACE("lfs_mkdir -> %d", err);
        return err;
    }

    // current block end of list?
    if (cwd.m.split) {
        // update tails, this creates a desync
        lfs_fs_preporphans(lfs, +1);

        // it's possible our predecessor has to be relocated, and if
        // our parent is our predecessor's predecessor, this could have
        // caused our parent to go out of date, fortunately we can hook
        // ourselves into littlefs to catch this
        cwd.type = 0;
        cwd.id = 0;
        lfs->mlist = &cwd;

        lfs_pair_tole32(dir.pair);
        err = lfs_dir_commit(lfs, &pred, LFS_MKATTRS(
                {LFS_MKTAG(LFS_TYPE_SOFTTAIL, 0x3ff, 8), dir.pair}));
        lfs_pair_fromle32(dir.pair);
        if (err) {
            lfs->mlist = cwd.next;
            LFS_TRACE("lfs_mkdir -> %d", err);
            return err;
        }

        lfs->mlist = cwd.next;
        lfs_fs_preporphans(lfs, -1);
    }

    // now insert into our parent block
    lfs_pair_tole32(dir.pair);
    err = lfs_dir_commit(lfs, &cwd.m, LFS_MKATTRS(
            {LFS_MKTAG(LFS_TYPE_CREATE, id, 0), NULL},
            {LFS_MKTAG(LFS_TYPE_DIR, id, nlen), path},
            {LFS_MKTAG(LFS_TYPE_DIRSTRUCT, id, 8), dir.pair},
            {LFS_MKTAG_IF(!cwd.m.split,
                LFS_TYPE_SOFTTAIL, 0x3ff, 8), dir.pair}));
    lfs_pair_fromle32(dir.pair);
    if (err) {
        LFS_TRACE("lfs_mkdir -> %d", err);
        return err;
    }

    LFS_TRACE("lfs_mkdir -> %d", 0);
    return 0;
}

int lfs_dir_open(lfs_t *lfs, lfs_dir_t *dir, const char *path) {
    LFS_TRACE("lfs_dir_open(%p, %p, \"%s\")", (void*)lfs, (void*)dir, path);
    lfs_stag_t tag = lfs_dir_find(lfs, &dir->m, &path, NULL);
    if (tag < 0) {
        LFS_TRACE("lfs_dir_open -> %"PRId32, tag);
        return tag;
    }

    if (lfs_tag_type3(tag) != LFS_TYPE_DIR) {
        LFS_TRACE("lfs_dir_open -> %d", LFS_ERR_NOTDIR);
        return LFS_ERR_NOTDIR;
    }

    lfs_block_t pair[2];
    if (lfs_tag_id(tag) == 0x3ff) {
        // handle root dir separately
        pair[0] = lfs->root[0];
        pair[1] = lfs->root[1];
    } else {
        // get dir pair from parent
        lfs_stag_t res = lfs_dir_get(lfs, &dir->m, LFS_MKTAG(0x700, 0x3ff, 0),
                LFS_MKTAG(LFS_TYPE_STRUCT, lfs_tag_id(tag), 8), pair);
        if (res < 0) {
            LFS_TRACE("lfs_dir_open -> %"PRId32, res);
            return res;
        }
        lfs_pair_fromle32(pair);
    }

    // fetch first pair
    int err = lfs_dir_fetch(lfs, &dir->m, pair);
    if (err) {
        LFS_TRACE("lfs_dir_open -> %d", err);
        return err;
    }

    // setup entry
    dir->head[0] = dir->m.pair[0];
    dir->head[1] = dir->m.pair[1];
    dir->id = 0;
    dir->pos = 0;

    // add to list of mdirs
    dir->type = LFS_TYPE_DIR;
    dir->next = (lfs_dir_t*)lfs->mlist;
    lfs->mlist = (struct lfs_mlist*)dir;

    LFS_TRACE("lfs_dir_open -> %d", 0);
    return 0;
}

int lfs_dir_close(lfs_t *lfs, lfs_dir_t *dir) {
    LFS_TRACE("lfs_dir_close(%p, %p)", (void*)lfs, (void*)dir);
    // remove from list of mdirs
    for (struct lfs_mlist **p = &lfs->mlist; *p; p = &(*p)->next) {
        if (*p == (struct lfs_mlist*)dir) {
            *p = (*p)->next;
            break;
        }
    }

    LFS_TRACE("lfs_dir_close -> %d", 0);
    return 0;
}

int lfs_dir_read(lfs_t *lfs, lfs_dir_t *dir, struct lfs_info *info) {
    LFS_TRACE("lfs_dir_read(%p, %p, %p)",
            (void*)lfs, (void*)dir, (void*)info);
    memset(info, 0, sizeof(*info));

    // special offset for '.' and '..'
    if (dir->pos == 0) {
        info->type = LFS_TYPE_DIR;
        strcpy(info->name, ".");
        dir->pos += 1;
        LFS_TRACE("lfs_dir_read -> %d", true);
        return true;
    } else if (dir->pos == 1) {
        info->type = LFS_TYPE_DIR;
        strcpy(info->name, "..");
        dir->pos += 1;
        LFS_TRACE("lfs_dir_read -> %d", true);
        return true;
    }

    while (true) {
        if (dir->id == dir->m.count) {
            if (!dir->m.split) {
                LFS_TRACE("lfs_dir_read -> %d", false);
                return false;
            }

            int err = lfs_dir_fetch(lfs, &dir->m, dir->m.tail);
            if (err) {
                LFS_TRACE("lfs_dir_read -> %d", err);
                return err;
            }

            dir->id = 0;
        }

        int err = lfs_dir_getinfo(lfs, &dir->m, dir->id, info);
        if (err && err != LFS_ERR_NOENT) {
            LFS_TRACE("lfs_dir_read -> %d", err);
            return err;
        }

        dir->id += 1;
        if (err != LFS_ERR_NOENT) {
            break;
        }
    }

    dir->pos += 1;
    LFS_TRACE("lfs_dir_read -> %d", true);
    return true;
}

int lfs_dir_seek(lfs_t *lfs, lfs_dir_t *dir, lfs_off_t off) {
    LFS_TRACE("lfs_dir_seek(%p, %p, %"PRIu32")",
            (void*)lfs, (void*)dir, off);
    // simply walk from head dir
    int err = lfs_dir_rewind(lfs, dir);
    if (err) {
        LFS_TRACE("lfs_dir_seek -> %d", err);
        return err;
    }

    // first two for ./..
    dir->pos = lfs_min(2, off);
    off -= dir->pos;

    // skip superblock entry
    dir->id = (off > 0 && lfs_pair_cmp(dir->head, lfs->root) == 0);

    while (off > 0) {
        int diff = lfs_min(dir->m.count - dir->id, off);
        dir->id += diff;
        dir->pos += diff;
        off -= diff;

        if (dir->id == dir->m.count) {
            if (!dir->m.split) {
                LFS_TRACE("lfs_dir_seek -> %d", LFS_ERR_INVAL);
                return LFS_ERR_INVAL;
            }

            err = lfs_dir_fetch(lfs, &dir->m, dir->m.tail);
            if (err) {
                LFS_TRACE("lfs_dir_seek -> %d", err);
                return err;
            }

            dir->id = 0;
        }
    }

    LFS_TRACE("lfs_dir_seek -> %d", 0);
    return 0;
}

lfs_soff_t lfs_dir_tell(lfs_t *lfs, lfs_dir_t *dir) {
    LFS_TRACE("lfs_dir_tell(%p, %p)", (void*)lfs, (void*)dir);
    (void)lfs;
    LFS_TRACE("lfs_dir_tell -> %"PRId32, dir->pos);
    return dir->pos;
}

int lfs_dir_rewind(lfs_t *lfs, lfs_dir_t *dir) {
    LFS_TRACE("lfs_dir_rewind(%p, %p)", (void*)lfs, (void*)dir);
    // reload the head dir
    int err = lfs_dir_fetch(lfs, &dir->m, dir->head);
    if (err) {
        LFS_TRACE("lfs_dir_rewind -> %d", err);
        return err;
    }

    dir->id = 0;
    dir->pos = 0;
    LFS_TRACE("lfs_dir_rewind -> %d", 0);
    return 0;
}


/// File index list operations ///
static int lfs_ctz_index(lfs_t *lfs, lfs_off_t *off) {
    lfs_off_t size = *off;
    lfs_off_t b = lfs->cfg->block_size - 2*4;
    lfs_off_t i = size / b;
    if (i == 0) {
        return 0;
    }

    i = (size - 4*(lfs_popc(i-1)+2)) / b;
    *off = size - b*i - 4*lfs_popc(i);
    return i;
}

static int lfs_ctz_find(lfs_t *lfs,
        const lfs_cache_t *pcache, lfs_cache_t *rcache,
        lfs_block_t head, lfs_size_t size,
        lfs_size_t pos, lfs_block_t *block, lfs_off_t *off) {
    if (size == 0) {
        *block = LFS_BLOCK_NULL;
        *off = 0;
        return 0;
    }

    lfs_off_t current = lfs_ctz_index(lfs, &(lfs_off_t){size-1});
    lfs_off_t target = lfs_ctz_index(lfs, &pos);

    while (current > target) {
        lfs_size_t skip = lfs_min(
                lfs_npw2(current-target+1) - 1,
                lfs_ctz(current));

        int err = lfs_bd_read(lfs,
                pcache, rcache, sizeof(head),
                head, 4*skip, &head, sizeof(head));
        head = lfs_fromle32(head);
        if (err) {
            return err;
        }

        current -= 1 << skip;
    }

    *block = head;
    *off = pos;
    return 0;
}

static int lfs_ctz_extend(lfs_t *lfs,
        lfs_cache_t *pcache, lfs_cache_t *rcache,
        lfs_block_t head, lfs_size_t size,
        lfs_block_t *block, lfs_off_t *off) {
    while (true) {
        // go ahead and grab a block
        lfs_block_t nblock;
        int err = lfs_alloc(lfs, &nblock);
        if (err) {
            return err;
        }

        {
            err = lfs_bd_erase(lfs, nblock);
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                return err;
            }

            if (size == 0) {
                *block = nblock;
                *off = 0;
                return 0;
            }

            lfs_size_t noff = size - 1;
            lfs_off_t index = lfs_ctz_index(lfs, &noff);
            noff = noff + 1;

            // just copy out the last block if it is incomplete
            if (noff != lfs->cfg->block_size) {
                for (lfs_off_t i = 0; i < noff; i++) {
                    uint8_t data;
                    err = lfs_bd_read(lfs,
                            NULL, rcache, noff-i,
                            head, i, &data, 1);
                    if (err) {
                        return err;
                    }

                    err = lfs_bd_prog(lfs,
                            pcache, rcache, true,
                            nblock, i, &data, 1);
                    if (err) {
                        if (err == LFS_ERR_CORRUPT) {
                            goto relocate;
                        }
                        return err;
                    }
                }

                *block = nblock;
                *off = noff;
                return 0;
            }

            // append block
            index += 1;
            lfs_size_t skips = lfs_ctz(index) + 1;
            lfs_block_t nhead = head;
            for (lfs_off_t i = 0; i < skips; i++) {
                nhead = lfs_tole32(nhead);
                err = lfs_bd_prog(lfs, pcache, rcache, true,
                        nblock, 4*i, &nhead, 4);
                nhead = lfs_fromle32(nhead);
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        goto relocate;
                    }
                    return err;
                }

                if (i != skips-1) {
                    err = lfs_bd_read(lfs,
                            NULL, rcache, sizeof(nhead),
                            nhead, 4*i, &nhead, sizeof(nhead));
                    nhead = lfs_fromle32(nhead);
                    if (err) {
                        return err;
                    }
                }
            }

            *block = nblock;
            *off = 4*skips;
            return 0;
        }

relocate:
        LFS_DEBUG("Bad block at 0x%"PRIx32, nblock);

        // just clear cache and try a new block
        lfs_cache_drop(lfs, pcache);
    }
}

static int lfs_ctz_traverse(lfs_t *lfs,
        const lfs_cache_t *pcache, lfs_cache_t *rcache,
        lfs_block_t head, lfs_size_t size,
        int (*cb)(void*, lfs_block_t), void *data) {
    if (size == 0) {
        return 0;
    }

    lfs_off_t index = lfs_ctz_index(lfs, &(lfs_off_t){size-1});

    while (true) {
        int err = cb(data, head);
        if (err) {
            return err;
        }

        if (index == 0) {
            return 0;
        }

        lfs_block_t heads[2];
        int count = 2 - (index & 1);
        err = lfs_bd_read(lfs,
                pcache, rcache, count*sizeof(head),
                head, 0, &heads, count*sizeof(head));
        heads[0] = lfs_fromle32(heads[0]);
        heads[1] = lfs_fromle32(heads[1]);
        if (err) {
            return err;
        }

        for (int i = 0; i < count-1; i++) {
            err = cb(data, heads[i]);
            if (err) {
                return err;
            }
        }

        head = heads[count-1];
        index -= count;
    }
}


/// Top level file operations ///
int lfs_file_opencfg(lfs_t *lfs, lfs_file_t *file,
        const char *path, int flags,
        const struct lfs_file_config *cfg) {
    LFS_TRACE("lfs_file_opencfg(%p, %p, \"%s\", %x, %p {"
                 ".buffer=%p, .attrs=%p, .attr_count=%"PRIu32"})",
            (void*)lfs, (void*)file, path, flags,
            (void*)cfg, cfg->buffer, (void*)cfg->attrs, cfg->attr_count);

    // deorphan if we haven't yet, needed at most once after poweron
    if ((flags & 3) != LFS_O_RDONLY) {
        int err = lfs_fs_forceconsistency(lfs);
        if (err) {
            LFS_TRACE("lfs_file_opencfg -> %d", err);
            return err;
        }
    }

    // setup simple file details
    int err;
    file->cfg = cfg;
    file->flags = flags | LFS_F_OPENED;
    file->pos = 0;
    file->off = 0;
    file->cache.buffer = NULL;

    // allocate entry for file if it doesn't exist
    lfs_stag_t tag = lfs_dir_find(lfs, &file->m, &path, &file->id);
    if (tag < 0 && !(tag == LFS_ERR_NOENT && file->id != 0x3ff)) {
        err = tag;
        goto cleanup;
    }

    // get id, add to list of mdirs to catch update changes
    file->type = LFS_TYPE_REG;
    file->next = (lfs_file_t*)lfs->mlist;
    lfs->mlist = (struct lfs_mlist*)file;

    if (tag == LFS_ERR_NOENT) {
        if (!(flags & LFS_O_CREAT)) {
            err = LFS_ERR_NOENT;
            goto cleanup;
        }

        // check that name fits
        lfs_size_t nlen = strlen(path);
        if (nlen > lfs->name_max) {
            err = LFS_ERR_NAMETOOLONG;
            goto cleanup;
        }

        // get next slot and create entry to remember name
        err = lfs_dir_commit(lfs, &file->m, LFS_MKATTRS(
                {LFS_MKTAG(LFS_TYPE_CREATE, file->id, 0), NULL},
                {LFS_MKTAG(LFS_TYPE_REG, file->id, nlen), path},
                {LFS_MKTAG(LFS_TYPE_INLINESTRUCT, file->id, 0), NULL}));
        if (err) {
            err = LFS_ERR_NAMETOOLONG;
            goto cleanup;
        }

        tag = LFS_MKTAG(LFS_TYPE_INLINESTRUCT, 0, 0);
    } else if (flags & LFS_O_EXCL) {
        err = LFS_ERR_EXIST;
        goto cleanup;
    } else if (lfs_tag_type3(tag) != LFS_TYPE_REG) {
        err = LFS_ERR_ISDIR;
        goto cleanup;
    } else if (flags & LFS_O_TRUNC) {
        // truncate if requested
        tag = LFS_MKTAG(LFS_TYPE_INLINESTRUCT, file->id, 0);
        file->flags |= LFS_F_DIRTY;
    } else {
        // try to load what's on disk, if it's inlined we'll fix it later
        tag = lfs_dir_get(lfs, &file->m, LFS_MKTAG(0x700, 0x3ff, 0),
                LFS_MKTAG(LFS_TYPE_STRUCT, file->id, 8), &file->ctz);
        if (tag < 0) {
            err = tag;
            goto cleanup;
        }
        lfs_ctz_fromle32(&file->ctz);
    }

    // fetch attrs
    for (unsigned i = 0; i < file->cfg->attr_count; i++) {
        if ((file->flags & 3) != LFS_O_WRONLY) {
            lfs_stag_t res = lfs_dir_get(lfs, &file->m,
                    LFS_MKTAG(0x7ff, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_USERATTR + file->cfg->attrs[i].type,
                        file->id, file->cfg->attrs[i].size),
                        file->cfg->attrs[i].buffer);
            if (res < 0 && res != LFS_ERR_NOENT) {
                err = res;
                goto cleanup;
            }
        }

        if ((file->flags & 3) != LFS_O_RDONLY) {
            if (file->cfg->attrs[i].size > lfs->attr_max) {
                err = LFS_ERR_NOSPC;
                goto cleanup;
            }

            file->flags |= LFS_F_DIRTY;
        }
    }

    // allocate buffer if needed
    if (file->cfg->buffer) {
        file->cache.buffer = file->cfg->buffer;
    } else {
        file->cache.buffer = lfs_malloc(lfs->cfg->cache_size);
        if (!file->cache.buffer) {
            err = LFS_ERR_NOMEM;
            goto cleanup;
        }
    }

    // zero to avoid information leak
    lfs_cache_zero(lfs, &file->cache);

    if (lfs_tag_type3(tag) == LFS_TYPE_INLINESTRUCT) {
        // load inline files
        file->ctz.head = LFS_BLOCK_INLINE;
        file->ctz.size = lfs_tag_size(tag);
        file->flags |= LFS_F_INLINE;
        file->cache.block = file->ctz.head;
        file->cache.off = 0;
        file->cache.size = lfs->cfg->cache_size;

        // don't always read (may be new/trunc file)
        if (file->ctz.size > 0) {
            lfs_stag_t res = lfs_dir_get(lfs, &file->m,
                    LFS_MKTAG(0x700, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_STRUCT, file->id,
                        lfs_min(file->cache.size, 0x3fe)),
                    file->cache.buffer);
            if (res < 0) {
                err = res;
                goto cleanup;
            }
        }
    }

    LFS_TRACE("lfs_file_opencfg -> %d", 0);
    return 0;

cleanup:
    // clean up lingering resources
    file->flags |= LFS_F_ERRED;
    lfs_file_close(lfs, file);
    LFS_TRACE("lfs_file_opencfg -> %d", err);
    return err;
}

int lfs_file_open(lfs_t *lfs, lfs_file_t *file,
        const char *path, int flags) {
    LFS_TRACE("lfs_file_open(%p, %p, \"%s\", %x)",
            (void*)lfs, (void*)file, path, flags);
    static const struct lfs_file_config defaults = {0};
    int err = lfs_file_opencfg(lfs, file, path, flags, &defaults);
    LFS_TRACE("lfs_file_open -> %d", err);
    return err;
}

int lfs_file_close(lfs_t *lfs, lfs_file_t *file) {
    LFS_TRACE("lfs_file_close(%p, %p)", (void*)lfs, (void*)file);
    LFS_ASSERT(file->flags & LFS_F_OPENED);

    int err = lfs_file_sync(lfs, file);

    // remove from list of mdirs
    for (struct lfs_mlist **p = &lfs->mlist; *p; p = &(*p)->next) {
        if (*p == (struct lfs_mlist*)file) {
            *p = (*p)->next;
            break;
        }
    }

    // clean up memory
    if (!file->cfg->buffer) {
        lfs_free(file->cache.buffer);
    }

    file->flags &= ~LFS_F_OPENED;
    LFS_TRACE("lfs_file_close -> %d", err);
    return err;
}

static int lfs_file_relocate(lfs_t *lfs, lfs_file_t *file) {
    LFS_ASSERT(file->flags & LFS_F_OPENED);

    while (true) {
        // just relocate what exists into new block
        lfs_block_t nblock;
        int err = lfs_alloc(lfs, &nblock);
        if (err) {
            return err;
        }

        err = lfs_bd_erase(lfs, nblock);
        if (err) {
            if (err == LFS_ERR_CORRUPT) {
                goto relocate;
            }
            return err;
        }

        // either read from dirty cache or disk
        for (lfs_off_t i = 0; i < file->off; i++) {
            uint8_t data;
            if (file->flags & LFS_F_INLINE) {
                err = lfs_dir_getread(lfs, &file->m,
                        // note we evict inline files before they can be dirty
                        NULL, &file->cache, file->off-i,
                        LFS_MKTAG(0xfff, 0x1ff, 0),
                        LFS_MKTAG(LFS_TYPE_INLINESTRUCT, file->id, 0),
                        i, &data, 1);
                if (err) {
                    return err;
                }
            } else {
                err = lfs_bd_read(lfs,
                        &file->cache, &lfs->rcache, file->off-i,
                        file->block, i, &data, 1);
                if (err) {
                    return err;
                }
            }

            err = lfs_bd_prog(lfs,
                    &lfs->pcache, &lfs->rcache, true,
                    nblock, i, &data, 1);
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                return err;
            }
        }

        // copy over new state of file
        memcpy(file->cache.buffer, lfs->pcache.buffer, lfs->cfg->cache_size);
        file->cache.block = lfs->pcache.block;
        file->cache.off = lfs->pcache.off;
        file->cache.size = lfs->pcache.size;
        lfs_cache_zero(lfs, &lfs->pcache);

        file->block = nblock;
        file->flags |= LFS_F_WRITING;
        return 0;

relocate:
        LFS_DEBUG("Bad block at 0x%"PRIx32, nblock);

        // just clear cache and try a new block
        lfs_cache_drop(lfs, &lfs->pcache);
    }
}

static int lfs_file_outline(lfs_t *lfs, lfs_file_t *file) {
    file->off = file->pos;
    lfs_alloc_ack(lfs);
    int err = lfs_file_relocate(lfs, file);
    if (err) {
        return err;
    }

    file->flags &= ~LFS_F_INLINE;
    return 0;
}

static int lfs_file_flush(lfs_t *lfs, lfs_file_t *file) {
    LFS_ASSERT(file->flags & LFS_F_OPENED);

    if (file->flags & LFS_F_READING) {
        if (!(file->flags & LFS_F_INLINE)) {
            lfs_cache_drop(lfs, &file->cache);
        }
        file->flags &= ~LFS_F_READING;
    }

    if (file->flags & LFS_F_WRITING) {
        lfs_off_t pos = file->pos;

        if (!(file->flags & LFS_F_INLINE)) {
            // copy over anything after current branch
            lfs_file_t orig = {
                .ctz.head = file->ctz.head,
                .ctz.size = file->ctz.size,
                .flags = LFS_O_RDONLY | LFS_F_OPENED,
                .pos = file->pos,
                .cache = lfs->rcache,
            };
            lfs_cache_drop(lfs, &lfs->rcache);

            while (file->pos < file->ctz.size) {
                // copy over a byte at a time, leave it up to caching
                // to make this efficient
                uint8_t data;
                lfs_ssize_t res = lfs_file_read(lfs, &orig, &data, 1);
                if (res < 0) {
                    return res;
                }

                res = lfs_file_write(lfs, file, &data, 1);
                if (res < 0) {
                    return res;
                }

                // keep our reference to the rcache in sync
                if (lfs->rcache.block != LFS_BLOCK_NULL) {
                    lfs_cache_drop(lfs, &orig.cache);
                    lfs_cache_drop(lfs, &lfs->rcache);
                }
            }

            // write out what we have
            while (true) {
                int err = lfs_bd_flush(lfs, &file->cache, &lfs->rcache, true);
                if (err) {
                    if (err == LFS_ERR_CORRUPT) {
                        goto relocate;
                    }
                    return err;
                }

                break;

relocate:
                LFS_DEBUG("Bad block at 0x%"PRIx32, file->block);
                err = lfs_file_relocate(lfs, file);
                if (err) {
                    return err;
                }
            }
        } else {
            file->pos = lfs_max(file->pos, file->ctz.size);
        }

        // actual file updates
        file->ctz.head = file->block;
        file->ctz.size = file->pos;
        file->flags &= ~LFS_F_WRITING;
        file->flags |= LFS_F_DIRTY;

        file->pos = pos;
    }

    return 0;
}

int lfs_file_sync(lfs_t *lfs, lfs_file_t *file) {
    LFS_TRACE("lfs_file_sync(%p, %p)", (void*)lfs, (void*)file);
    LFS_ASSERT(file->flags & LFS_F_OPENED);

    if (file->flags & LFS_F_ERRED) {
        // it's not safe to do anything if our file errored
        LFS_TRACE("lfs_file_sync -> %d", 0);
        return 0;
    }

    int err = lfs_file_flush(lfs, file);
    if (err) {
        file->flags |= LFS_F_ERRED;
        LFS_TRACE("lfs_file_sync -> %d", err);
        return err;
    }

    if ((file->flags & LFS_F_DIRTY) &&
            !lfs_pair_isnull(file->m.pair)) {
        // update dir entry
        uint16_t type;
        const void *buffer;
        lfs_size_t size;
        struct lfs_ctz ctz;
        if (file->flags & LFS_F_INLINE) {
            // inline the whole file
            type = LFS_TYPE_INLINESTRUCT;
            buffer = file->cache.buffer;
            size = file->ctz.size;
        } else {
            // update the ctz reference
            type = LFS_TYPE_CTZSTRUCT;
            // copy ctz so alloc will work during a relocate
            ctz = file->ctz;
            lfs_ctz_tole32(&ctz);
            buffer = &ctz;
            size = sizeof(ctz);
        }

        // commit file data and attributes
        err = lfs_dir_commit(lfs, &file->m, LFS_MKATTRS(
                {LFS_MKTAG(type, file->id, size), buffer},
                {LFS_MKTAG(LFS_FROM_USERATTRS, file->id,
                    file->cfg->attr_count), file->cfg->attrs}));
        if (err) {
            file->flags |= LFS_F_ERRED;
            LFS_TRACE("lfs_file_sync -> %d", err);
            return err;
        }

        file->flags &= ~LFS_F_DIRTY;
    }

    LFS_TRACE("lfs_file_sync -> %d", 0);
    return 0;
}

lfs_ssize_t lfs_file_read(lfs_t *lfs, lfs_file_t *file,
        void *buffer, lfs_size_t size) {
    LFS_TRACE("lfs_file_read(%p, %p, %p, %"PRIu32")",
            (void*)lfs, (void*)file, buffer, size);
    LFS_ASSERT(file->flags & LFS_F_OPENED);
    LFS_ASSERT((file->flags & 3) != LFS_O_WRONLY);

    uint8_t *data = buffer;
    lfs_size_t nsize = size;

    if (file->flags & LFS_F_WRITING) {
        // flush out any writes
        int err = lfs_file_flush(lfs, file);
        if (err) {
            LFS_TRACE("lfs_file_read -> %d", err);
            return err;
        }
    }

    if (file->pos >= file->ctz.size) {
        // eof if past end
        LFS_TRACE("lfs_file_read -> %d", 0);
        return 0;
    }

    size = lfs_min(size, file->ctz.size - file->pos);
    nsize = size;

    while (nsize > 0) {
        // check if we need a new block
        if (!(file->flags & LFS_F_READING) ||
                file->off == lfs->cfg->block_size) {
            if (!(file->flags & LFS_F_INLINE)) {
                int err = lfs_ctz_find(lfs, NULL, &file->cache,
                        file->ctz.head, file->ctz.size,
                        file->pos, &file->block, &file->off);
                if (err) {
                    LFS_TRACE("lfs_file_read -> %d", err);
                    return err;
                }
            } else {
                file->block = LFS_BLOCK_INLINE;
                file->off = file->pos;
            }

            file->flags |= LFS_F_READING;
        }

        // read as much as we can in current block
        lfs_size_t diff = lfs_min(nsize, lfs->cfg->block_size - file->off);
        if (file->flags & LFS_F_INLINE) {
            int err = lfs_dir_getread(lfs, &file->m,
                    NULL, &file->cache, lfs->cfg->block_size,
                    LFS_MKTAG(0xfff, 0x1ff, 0),
                    LFS_MKTAG(LFS_TYPE_INLINESTRUCT, file->id, 0),
                    file->off, data, diff);
            if (err) {
                LFS_TRACE("lfs_file_read -> %d", err);
                return err;
            }
        } else {
            int err = lfs_bd_read(lfs,
                    NULL, &file->cache, lfs->cfg->block_size,
                    file->block, file->off, data, diff);
            if (err) {
                LFS_TRACE("lfs_file_read -> %d", err);
                return err;
            }
        }

        file->pos += diff;
        file->off += diff;
        data += diff;
        nsize -= diff;
    }

    LFS_TRACE("lfs_file_read -> %"PRId32, size);
    return size;
}

lfs_ssize_t lfs_file_write(lfs_t *lfs, lfs_file_t *file,
        const void *buffer, lfs_size_t size) {
    LFS_TRACE("lfs_file_write(%p, %p, %p, %"PRIu32")",
            (void*)lfs, (void*)file, buffer, size);
    LFS_ASSERT(file->flags & LFS_F_OPENED);
    LFS_ASSERT((file->flags & 3) != LFS_O_RDONLY);

    const uint8_t *data = buffer;
    lfs_size_t nsize = size;

    if (file->flags & LFS_F_READING) {
        // drop any reads
        int err = lfs_file_flush(lfs, file);
        if (err) {
            LFS_TRACE("lfs_file_write -> %d", err);
            return err;
        }
    }

    if ((file->flags & LFS_O_APPEND) && file->pos < file->ctz.size) {
        file->pos = file->ctz.size;
    }

    if (file->pos + size > lfs->file_max) {
        // Larger than file limit?
        LFS_TRACE("lfs_file_write -> %d", LFS_ERR_FBIG);
        return LFS_ERR_FBIG;
    }

    if (!(file->flags & LFS_F_WRITING) && file->pos > file->ctz.size) {
        // fill with zeros
        lfs_off_t pos = file->pos;
        file->pos = file->ctz.size;

        while (file->pos < pos) {
            lfs_ssize_t res = lfs_file_write(lfs, file, &(uint8_t){0}, 1);
            if (res < 0) {
                LFS_TRACE("lfs_file_write -> %"PRId32, res);
                return res;
            }
        }
    }

    if ((file->flags & LFS_F_INLINE) &&
            lfs_max(file->pos+nsize, file->ctz.size) >
            lfs_min(0x3fe, lfs_min(
                lfs->cfg->cache_size, lfs->cfg->block_size/8))) {
        // inline file doesn't fit anymore
        int err = lfs_file_outline(lfs, file);
        if (err) {
            file->flags |= LFS_F_ERRED;
            LFS_TRACE("lfs_file_write -> %d", err);
            return err;
        }
    }

    while (nsize > 0) {
        // check if we need a new block
        if (!(file->flags & LFS_F_WRITING) ||
                file->off == lfs->cfg->block_size) {
            if (!(file->flags & LFS_F_INLINE)) {
                if (!(file->flags & LFS_F_WRITING) && file->pos > 0) {
                    // find out which block we're extending from
                    int err = lfs_ctz_find(lfs, NULL, &file->cache,
                            file->ctz.head, file->ctz.size,
                            file->pos-1, &file->block, &file->off);
                    if (err) {
                        file->flags |= LFS_F_ERRED;
                        LFS_TRACE("lfs_file_write -> %d", err);
                        return err;
                    }

                    // mark cache as dirty since we may have read data into it
                    lfs_cache_zero(lfs, &file->cache);
                }

                // extend file with new blocks
                lfs_alloc_ack(lfs);
                int err = lfs_ctz_extend(lfs, &file->cache, &lfs->rcache,
                        file->block, file->pos,
                        &file->block, &file->off);
                if (err) {
                    file->flags |= LFS_F_ERRED;
                    LFS_TRACE("lfs_file_write -> %d", err);
                    return err;
                }
            } else {
                file->block = LFS_BLOCK_INLINE;
                file->off = file->pos;
            }

            file->flags |= LFS_F_WRITING;
        }

        // program as much as we can in current block
        lfs_size_t diff = lfs_min(nsize, lfs->cfg->block_size - file->off);
        while (true) {
            int err = lfs_bd_prog(lfs, &file->cache, &lfs->rcache, true,
                    file->block, file->off, data, diff);
            if (err) {
                if (err == LFS_ERR_CORRUPT) {
                    goto relocate;
                }
                file->flags |= LFS_F_ERRED;
                LFS_TRACE("lfs_file_write -> %d", err);
                return err;
            }

            break;
relocate:
            err = lfs_file_relocate(lfs, file);
            if (err) {
                file->flags |= LFS_F_ERRED;
                LFS_TRACE("lfs_file_write -> %d", err);
                return err;
            }
        }

        file->pos += diff;
        file->off += diff;
        data += diff;
        nsize -= diff;

        lfs_alloc_ack(lfs);
    }

    file->flags &= ~LFS_F_ERRED;
    LFS_TRACE("lfs_file_write -> %"PRId32, size);
    return size;
}

lfs_soff_t lfs_file_seek(lfs_t *lfs, lfs_file_t *file,
        lfs_soff_t off, int whence) {
    LFS_TRACE("lfs_file_seek(%p, %p, %"PRId32", %d)",
            (void*)lfs, (void*)file, off, whence);
    LFS_ASSERT(file->flags & LFS_F_OPENED);

    // write out everything beforehand, may be noop if rdonly
    int err = lfs_file_flush(lfs, file);
    if (err) {
        LFS_TRACE("lfs_file_seek -> %d", err);
        return err;
    }

    // find new pos
    lfs_off_t npos = file->pos;
    if (whence == LFS_SEEK_SET) {
        npos = off;
    } else if (whence == LFS_SEEK_CUR) {
        npos = file->pos + off;
    } else if (whence == LFS_SEEK_END) {
        npos = file->ctz.size + off;
    }

    if (npos > lfs->file_max) {
        // file position out of range
        LFS_TRACE("lfs_file_seek -> %d", LFS_ERR_INVAL);
        return LFS_ERR_INVAL;
    }

    // update pos
    file->pos = npos;
    LFS_TRACE("lfs_file_seek -> %"PRId32, npos);
    return npos;
}

int lfs_file_truncate(lfs_t *lfs, lfs_file_t *file, lfs_off_t size) {
    LFS_TRACE("lfs_file_truncate(%p, %p, %"PRIu32")",
            (void*)lfs, (void*)file, size);
    LFS_ASSERT(file->flags & LFS_F_OPENED);
    LFS_ASSERT((file->flags & 3) != LFS_O_RDONLY);

    if (size > LFS_FILE_MAX) {
        LFS_TRACE("lfs_file_truncate -> %d", LFS_ERR_INVAL);
        return LFS_ERR_INVAL;
    }

    lfs_off_t pos = file->pos;
    lfs_off_t oldsize = lfs_file_size(lfs, file);
    if (size < oldsize) {
        // need to flush since directly changing metadata
        int err = lfs_file_flush(lfs, file);
        if (err) {
            LFS_TRACE("lfs_file_truncate -> %d", err);
            return err;
        }

        // lookup new head in ctz skip list
        err = lfs_ctz_find(lfs, NULL, &file->cache,
                file->ctz.head, file->ctz.size,
                size, &file->block, &file->off);
        if (err) {
            LFS_TRACE("lfs_file_truncate -> %d", err);
            return err;
        }

        file->ctz.head = file->block;
        file->ctz.size = size;
        file->flags |= LFS_F_DIRTY | LFS_F_READING;
    } else if (size > oldsize) {
        // flush+seek if not already at end
        if (file->pos != oldsize) {
            lfs_soff_t res = lfs_file_seek(lfs, file, 0, LFS_SEEK_END);
            if (res < 0) {
                LFS_TRACE("lfs_file_truncate -> %"PRId32, res);
                return (int)res;
            }
        }

        // fill with zeros
        while (file->pos < size) {
            lfs_ssize_t res = lfs_file_write(lfs, file, &(uint8_t){0}, 1);
            if (res < 0) {
                LFS_TRACE("lfs_file_truncate -> %"PRId32, res);
                return (int)res;
            }
        }
    }

    // restore pos
    lfs_soff_t res = lfs_file_seek(lfs, file, pos, LFS_SEEK_SET);
    if (res < 0) {
      LFS_TRACE("lfs_file_truncate -> %"PRId32, res);
      return (int)res;
    }

    LFS_TRACE("lfs_file_truncate -> %d", 0);
    return 0;
}

lfs_soff_t lfs_file_tell(lfs_t *lfs, lfs_file_t *file) {
    LFS_TRACE("lfs_file_tell(%p, %p)", (void*)lfs, (void*)file);
    LFS_ASSERT(file->flags & LFS_F_OPENED);
    (void)lfs;
    LFS_TRACE("lfs_file_tell -> %"PRId32, file->pos);
    return file->pos;
}

int lfs_file_rewind(lfs_t *lfs, lfs_file_t *file) {
    LFS_TRACE("lfs_file_rewind(%p, %p)", (void*)lfs, (void*)file);
    lfs_soff_t res = lfs_file_seek(lfs, file, 0, LFS_SEEK_SET);
    if (res < 0) {
        LFS_TRACE("lfs_file_rewind -> %"PRId32, res);
        return (int)res;
    }

    LFS_TRACE("lfs_file_rewind -> %d", 0);
    return 0;
}

lfs_soff_t lfs_file_size(lfs_t *lfs, lfs_file_t *file) {
    LFS_TRACE("lfs_file_size(%p, %p)", (void*)lfs, (void*)file);
    LFS_ASSERT(file->flags & LFS_F_OPENED);
    (void)lfs;
    if (file->flags & LFS_F_WRITING) {
        LFS_TRACE("lfs_file_size -> %"PRId32,
                lfs_max(file->pos, file->ctz.size));
        return lfs_max(file->pos, file->ctz.size);
    } else {
        LFS_TRACE("lfs_file_size -> %"PRId32, file->ctz.size);
        return file->ctz.size;
    }
}


/// General fs operations ///
int lfs_stat(lfs_t *lfs, const char *path, struct lfs_info *info) {
    LFS_TRACE("lfs_stat(%p, \"%s\", %p)", (void*)lfs, path, (void*)info);
    lfs_mdir_t cwd;
    lfs_stag_t tag = lfs_dir_find(lfs, &cwd, &path, NULL);
    if (tag < 0) {
        LFS_TRACE("lfs_stat -> %"PRId32, tag);
        return (int)tag;
    }

    int err = lfs_dir_getinfo(lfs, &cwd, lfs_tag_id(tag), info);
    LFS_TRACE("lfs_stat -> %d", err);
    return err;
}

int lfs_remove(lfs_t *lfs, const char *path) {
    LFS_TRACE("lfs_remove(%p, \"%s\")", (void*)lfs, path);
    // deorphan if we haven't yet, needed at most once after poweron
    int err = lfs_fs_forceconsistency(lfs);
    if (err) {
        LFS_TRACE("lfs_remove -> %d", err);
        return err;
    }

    lfs_mdir_t cwd;
    lfs_stag_t tag = lfs_dir_find(lfs, &cwd, &path, NULL);
    if (tag < 0 || lfs_tag_id(tag) == 0x3ff) {
        LFS_TRACE("lfs_remove -> %"PRId32, (tag < 0) ? tag : LFS_ERR_INVAL);
        return (tag < 0) ? (int)tag : LFS_ERR_INVAL;
    }

    struct lfs_mlist dir;
    dir.next = lfs->mlist;
    if (lfs_tag_type3(tag) == LFS_TYPE_DIR) {
        // must be empty before removal
        lfs_block_t pair[2];
        lfs_stag_t res = lfs_dir_get(lfs, &cwd, LFS_MKTAG(0x700, 0x3ff, 0),
                LFS_MKTAG(LFS_TYPE_STRUCT, lfs_tag_id(tag), 8), pair);
        if (res < 0) {
            LFS_TRACE("lfs_remove -> %"PRId32, res);
            return (int)res;
        }
        lfs_pair_fromle32(pair);

        err = lfs_dir_fetch(lfs, &dir.m, pair);
        if (err) {
            LFS_TRACE("lfs_remove -> %d", err);
            return err;
        }

        if (dir.m.count > 0 || dir.m.split) {
            LFS_TRACE("lfs_remove -> %d", LFS_ERR_NOTEMPTY);
            return LFS_ERR_NOTEMPTY;
        }

        // mark fs as orphaned
        lfs_fs_preporphans(lfs, +1);

        // I know it's crazy but yes, dir can be changed by our parent's
        // commit (if predecessor is child)
        dir.type = 0;
        dir.id = 0;
        lfs->mlist = &dir;
    }

    // delete the entry
    err = lfs_dir_commit(lfs, &cwd, LFS_MKATTRS(
            {LFS_MKTAG(LFS_TYPE_DELETE, lfs_tag_id(tag), 0), NULL}));
    if (err) {
        lfs->mlist = dir.next;
        LFS_TRACE("lfs_remove -> %d", err);
        return err;
    }

    lfs->mlist = dir.next;
    if (lfs_tag_type3(tag) == LFS_TYPE_DIR) {
        // fix orphan
        lfs_fs_preporphans(lfs, -1);

        err = lfs_fs_pred(lfs, dir.m.pair, &cwd);
        if (err) {
            LFS_TRACE("lfs_remove -> %d", err);
            return err;
        }

        err = lfs_dir_drop(lfs, &cwd, &dir.m);
        if (err) {
            LFS_TRACE("lfs_remove -> %d", err);
            return err;
        }
    }

    LFS_TRACE("lfs_remove -> %d", 0);
    return 0;
}

int lfs_rename(lfs_t *lfs, const char *oldpath, const char *newpath) {
    LFS_TRACE("lfs_rename(%p, \"%s\", \"%s\")", (void*)lfs, oldpath, newpath);

    // deorphan if we haven't yet, needed at most once after poweron
    int err = lfs_fs_forceconsistency(lfs);
    if (err) {
        LFS_TRACE("lfs_rename -> %d", err);
        return err;
    }

    // find old entry
    lfs_mdir_t oldcwd;
    lfs_stag_t oldtag = lfs_dir_find(lfs, &oldcwd, &oldpath, NULL);
    if (oldtag < 0 || lfs_tag_id(oldtag) == 0x3ff) {
        LFS_TRACE("lfs_rename -> %"PRId32,
                (oldtag < 0) ? oldtag : LFS_ERR_INVAL);
        return (oldtag < 0) ? (int)oldtag : LFS_ERR_INVAL;
    }

    // find new entry
    lfs_mdir_t newcwd;
    uint16_t newid;
    lfs_stag_t prevtag = lfs_dir_find(lfs, &newcwd, &newpath, &newid);
    if ((prevtag < 0 || lfs_tag_id(prevtag) == 0x3ff) &&
            !(prevtag == LFS_ERR_NOENT && newid != 0x3ff)) {
        LFS_TRACE("lfs_rename -> %"PRId32,
            (prevtag < 0) ? prevtag : LFS_ERR_INVAL);
        return (prevtag < 0) ? (int)prevtag : LFS_ERR_INVAL;
    }

    // if we're in the same pair there's a few special cases...
    bool samepair = (lfs_pair_cmp(oldcwd.pair, newcwd.pair) == 0);
    uint16_t newoldid = lfs_tag_id(oldtag);

    struct lfs_mlist prevdir;
    prevdir.next = lfs->mlist;
    if (prevtag == LFS_ERR_NOENT) {
        // check that name fits
        lfs_size_t nlen = strlen(newpath);
        if (nlen > lfs->name_max) {
            LFS_TRACE("lfs_rename -> %d", LFS_ERR_NAMETOOLONG);
            return LFS_ERR_NAMETOOLONG;
        }

        // there is a small chance we are being renamed in the same
        // directory/ to an id less than our old id, the global update
        // to handle this is a bit messy
        if (samepair && newid <= newoldid) {
            newoldid += 1;
        }
    } else if (lfs_tag_type3(prevtag) != lfs_tag_type3(oldtag)) {
        LFS_TRACE("lfs_rename -> %d", LFS_ERR_ISDIR);
        return LFS_ERR_ISDIR;
    } else if (samepair && newid == newoldid) {
        // we're renaming to ourselves??
        LFS_TRACE("lfs_rename -> %d", 0);
        return 0;
    } else if (lfs_tag_type3(prevtag) == LFS_TYPE_DIR) {
        // must be empty before removal
        lfs_block_t prevpair[2];
        lfs_stag_t res = lfs_dir_get(lfs, &newcwd, LFS_MKTAG(0x700, 0x3ff, 0),
                LFS_MKTAG(LFS_TYPE_STRUCT, newid, 8), prevpair);
        if (res < 0) {
            LFS_TRACE("lfs_rename -> %"PRId32, res);
            return (int)res;
        }
        lfs_pair_fromle32(prevpair);

        // must be empty before removal
        err = lfs_dir_fetch(lfs, &prevdir.m, prevpair);
        if (err) {
            LFS_TRACE("lfs_rename -> %d", err);
            return err;
        }

        if (prevdir.m.count > 0 || prevdir.m.split) {
            LFS_TRACE("lfs_rename -> %d", LFS_ERR_NOTEMPTY);
            return LFS_ERR_NOTEMPTY;
        }

        // mark fs as orphaned
        lfs_fs_preporphans(lfs, +1);

        // I know it's crazy but yes, dir can be changed by our parent's
        // commit (if predecessor is child)
        prevdir.type = 0;
        prevdir.id = 0;
        lfs->mlist = &prevdir;
    }

    if (!samepair) {
        lfs_fs_prepmove(lfs, newoldid, oldcwd.pair);
    }

    // move over all attributes
    err = lfs_dir_commit(lfs, &newcwd, LFS_MKATTRS(
            {LFS_MKTAG_IF(prevtag != LFS_ERR_NOENT,
                LFS_TYPE_DELETE, newid, 0), NULL},
            {LFS_MKTAG(LFS_TYPE_CREATE, newid, 0), NULL},
            {LFS_MKTAG(lfs_tag_type3(oldtag), newid, strlen(newpath)), newpath},
            {LFS_MKTAG(LFS_FROM_MOVE, newid, lfs_tag_id(oldtag)), &oldcwd},
            {LFS_MKTAG_IF(samepair,
                LFS_TYPE_DELETE, newoldid, 0), NULL}));
    if (err) {
        lfs->mlist = prevdir.next;
        LFS_TRACE("lfs_rename -> %d", err);
        return err;
    }

    // let commit clean up after move (if we're different! otherwise move
    // logic already fixed it for us)
    if (!samepair && lfs_gstate_hasmove(&lfs->gstate)) {
        // prep gstate and delete move id
        lfs_fs_prepmove(lfs, 0x3ff, NULL);
        err = lfs_dir_commit(lfs, &oldcwd, LFS_MKATTRS(
                {LFS_MKTAG(LFS_TYPE_DELETE, lfs_tag_id(oldtag), 0), NULL}));
        if (err) {
            lfs->mlist = prevdir.next;
            LFS_TRACE("lfs_rename -> %d", err);
            return err;
        }
    }

    lfs->mlist = prevdir.next;
    if (prevtag != LFS_ERR_NOENT && lfs_tag_type3(prevtag) == LFS_TYPE_DIR) {
        // fix orphan
        lfs_fs_preporphans(lfs, -1);

        err = lfs_fs_pred(lfs, prevdir.m.pair, &newcwd);
        if (err) {
            LFS_TRACE("lfs_rename -> %d", err);
            return err;
        }

        err = lfs_dir_drop(lfs, &newcwd, &prevdir.m);
        if (err) {
            LFS_TRACE("lfs_rename -> %d", err);
            return err;
        }
    }

    LFS_TRACE("lfs_rename -> %d", 0);
    return 0;
}

lfs_ssize_t lfs_getattr(lfs_t *lfs, const char *path,
        uint8_t type, void *buffer, lfs_size_t size) {
    LFS_TRACE("lfs_getattr(%p, \"%s\", %"PRIu8", %p, %"PRIu32")",
            (void*)lfs, path, type, buffer, size);
    lfs_mdir_t cwd;
    lfs_stag_t tag = lfs_dir_find(lfs, &cwd, &path, NULL);
    if (tag < 0) {
        LFS_TRACE("lfs_getattr -> %"PRId32, tag);
        return tag;
    }

    uint16_t id = lfs_tag_id(tag);
    if (id == 0x3ff) {
        // special case for root
        id = 0;
        int err = lfs_dir_fetch(lfs, &cwd, lfs->root);
        if (err) {
            LFS_TRACE("lfs_getattr -> %d", err);
            return err;
        }
    }

    tag = lfs_dir_get(lfs, &cwd, LFS_MKTAG(0x7ff, 0x3ff, 0),
            LFS_MKTAG(LFS_TYPE_USERATTR + type,
                id, lfs_min(size, lfs->attr_max)),
            buffer);
    if (tag < 0) {
        if (tag == LFS_ERR_NOENT) {
            LFS_TRACE("lfs_getattr -> %d", LFS_ERR_NOATTR);
            return LFS_ERR_NOATTR;
        }

        LFS_TRACE("lfs_getattr -> %"PRId32, tag);
        return tag;
    }

    size = lfs_tag_size(tag);
    LFS_TRACE("lfs_getattr -> %"PRId32, size);
    return size;
}

static int lfs_commitattr(lfs_t *lfs, const char *path,
        uint8_t type, const void *buffer, lfs_size_t size) {
    lfs_mdir_t cwd;
    lfs_stag_t tag = lfs_dir_find(lfs, &cwd, &path, NULL);
    if (tag < 0) {
        return tag;
    }

    uint16_t id = lfs_tag_id(tag);
    if (id == 0x3ff) {
        // special case for root
        id = 0;
        int err = lfs_dir_fetch(lfs, &cwd, lfs->root);
        if (err) {
            return err;
        }
    }

    return lfs_dir_commit(lfs, &cwd, LFS_MKATTRS(
            {LFS_MKTAG(LFS_TYPE_USERATTR + type, id, size), buffer}));
}

int lfs_setattr(lfs_t *lfs, const char *path,
        uint8_t type, const void *buffer, lfs_size_t size) {
    LFS_TRACE("lfs_setattr(%p, \"%s\", %"PRIu8", %p, %"PRIu32")",
            (void*)lfs, path, type, buffer, size);
    if (size > lfs->attr_max) {
        LFS_TRACE("lfs_setattr -> %d", LFS_ERR_NOSPC);
        return LFS_ERR_NOSPC;
    }

    int err = lfs_commitattr(lfs, path, type, buffer, size);
    LFS_TRACE("lfs_setattr -> %d", err);
    return err;
}

int lfs_removeattr(lfs_t *lfs, const char *path, uint8_t type) {
    LFS_TRACE("lfs_removeattr(%p, \"%s\", %"PRIu8")", (void*)lfs, path, type);
    int err = lfs_commitattr(lfs, path, type, NULL, 0x3ff);
    LFS_TRACE("lfs_removeattr -> %d", err);
    return err;
}


/// Filesystem operations ///
static int lfs_init(lfs_t *lfs, const struct lfs_config *cfg) {
    lfs->cfg = cfg;
    int err = 0;

    // validate that the lfs-cfg sizes were initiated properly before
    // performing any arithmetic logics with them
    LFS_ASSERT(lfs->cfg->read_size != 0);
    LFS_ASSERT(lfs->cfg->prog_size != 0);
    LFS_ASSERT(lfs->cfg->cache_size != 0);

    // check that block size is a multiple of cache size is a multiple
    // of prog and read sizes
    LFS_ASSERT(lfs->cfg->cache_size % lfs->cfg->read_size == 0);
    LFS_ASSERT(lfs->cfg->cache_size % lfs->cfg->prog_size == 0);
    LFS_ASSERT(lfs->cfg->block_size % lfs->cfg->cache_size == 0);

    // check that the block size is large enough to fit ctz pointers
    LFS_ASSERT(4*lfs_npw2(0xffffffff / (lfs->cfg->block_size-2*4))
            <= lfs->cfg->block_size);

    // block_cycles = 0 is no longer supported.
    //
    // block_cycles is the number of erase cycles before littlefs evicts
    // metadata logs as a part of wear leveling. Suggested values are in the
    // range of 100-1000, or set block_cycles to -1 to disable block-level
    // wear-leveling.
    LFS_ASSERT(lfs->cfg->block_cycles != 0);


    // setup read cache
    if (lfs->cfg->read_buffer) {
        lfs->rcache.buffer = lfs->cfg->read_buffer;
    } else {
        lfs->rcache.buffer = lfs_malloc(lfs->cfg->cache_size);
        if (!lfs->rcache.buffer) {
            err = LFS_ERR_NOMEM;
            goto cleanup;
        }
    }

    // setup program cache
    if (lfs->cfg->prog_buffer) {
        lfs->pcache.buffer = lfs->cfg->prog_buffer;
    } else {
        lfs->pcache.buffer = lfs_malloc(lfs->cfg->cache_size);
        if (!lfs->pcache.buffer) {
            err = LFS_ERR_NOMEM;
            goto cleanup;
        }
    }

    // zero to avoid information leaks
    lfs_cache_zero(lfs, &lfs->rcache);
    lfs_cache_zero(lfs, &lfs->pcache);

    // setup lookahead, must be multiple of 64-bits, 32-bit aligned
    LFS_ASSERT(lfs->cfg->lookahead_size > 0);
    LFS_ASSERT(lfs->cfg->lookahead_size % 8 == 0 &&
            (uintptr_t)lfs->cfg->lookahead_buffer % 4 == 0);
    if (lfs->cfg->lookahead_buffer) {
        lfs->free.buffer = lfs->cfg->lookahead_buffer;
    } else {
        lfs->free.buffer = lfs_malloc(lfs->cfg->lookahead_size);
        if (!lfs->free.buffer) {
            err = LFS_ERR_NOMEM;
            goto cleanup;
        }
    }

    // check that the size limits are sane
    LFS_ASSERT(lfs->cfg->name_max <= LFS_NAME_MAX);
    lfs->name_max = lfs->cfg->name_max;
    if (!lfs->name_max) {
        lfs->name_max = LFS_NAME_MAX;
    }

    LFS_ASSERT(lfs->cfg->file_max <= LFS_FILE_MAX);
    lfs->file_max = lfs->cfg->file_max;
    if (!lfs->file_max) {
        lfs->file_max = LFS_FILE_MAX;
    }

    LFS_ASSERT(lfs->cfg->attr_max <= LFS_ATTR_MAX);
    lfs->attr_max = lfs->cfg->attr_max;
    if (!lfs->attr_max) {
        lfs->attr_max = LFS_ATTR_MAX;
    }

    // setup default state
    lfs->root[0] = LFS_BLOCK_NULL;
    lfs->root[1] = LFS_BLOCK_NULL;
    lfs->mlist = NULL;
    lfs->seed = 0;
    lfs->gdisk = (lfs_gstate_t){0};
    lfs->gstate = (lfs_gstate_t){0};
    lfs->gdelta = (lfs_gstate_t){0};
#ifdef LFS_MIGRATE
    lfs->lfs1 = NULL;
#endif

    return 0;

cleanup:
    lfs_deinit(lfs);
    return err;
}

static int lfs_deinit(lfs_t *lfs) {
    // free allocated memory
    if (!lfs->cfg->read_buffer) {
        lfs_free(lfs->rcache.buffer);
    }

    if (!lfs->cfg->prog_buffer) {
        lfs_free(lfs->pcache.buffer);
    }

    if (!lfs->cfg->lookahead_buffer) {
        lfs_free(lfs->free.buffer);
    }

    return 0;
}

int lfs_format(lfs_t *lfs, const struct lfs_config *cfg) {
    LFS_TRACE("lfs_format(%p, %p {.context=%p, "
                ".read=%p, .prog=%p, .erase=%p, .sync=%p, "
                ".read_size=%"PRIu32", .prog_size=%"PRIu32", "
                ".block_size=%"PRIu32", .block_count=%"PRIu32", "
                ".block_cycles=%"PRIu32", .cache_size=%"PRIu32", "
                ".lookahead_size=%"PRIu32", .read_buffer=%p, "
                ".prog_buffer=%p, .lookahead_buffer=%p, "
                ".name_max=%"PRIu32", .file_max=%"PRIu32", "
                ".attr_max=%"PRIu32"})",
            (void*)lfs, (void*)cfg, cfg->context,
            (void*)(uintptr_t)cfg->read, (void*)(uintptr_t)cfg->prog,
            (void*)(uintptr_t)cfg->erase, (void*)(uintptr_t)cfg->sync,
            cfg->read_size, cfg->prog_size, cfg->block_size, cfg->block_count,
            cfg->block_cycles, cfg->cache_size, cfg->lookahead_size,
            cfg->read_buffer, cfg->prog_buffer, cfg->lookahead_buffer,
            cfg->name_max, cfg->file_max, cfg->attr_max);
    int err = 0;
    {
        err = lfs_init(lfs, cfg);
        if (err) {
            LFS_TRACE("lfs_format -> %d", err);
            return err;
        }

        // create free lookahead
        memset(lfs->free.buffer, 0, lfs->cfg->lookahead_size);
        lfs->free.off = 0;
        lfs->free.size = lfs_min(8*lfs->cfg->lookahead_size,
                lfs->cfg->block_count);
        lfs->free.i = 0;
        lfs_alloc_ack(lfs);

        // create root dir
        lfs_mdir_t root;
        err = lfs_dir_alloc(lfs, &root);
        if (err) {
            goto cleanup;
        }

        // write one superblock
        lfs_superblock_t superblock = {
            .version     = LFS_DISK_VERSION,
            .block_size  = lfs->cfg->block_size,
            .block_count = lfs->cfg->block_count,
            .name_max    = lfs->name_max,
            .file_max    = lfs->file_max,
            .attr_max    = lfs->attr_max,
        };

        lfs_superblock_tole32(&superblock);
        err = lfs_dir_commit(lfs, &root, LFS_MKATTRS(
                {LFS_MKTAG(LFS_TYPE_CREATE, 0, 0), NULL},
                {LFS_MKTAG(LFS_TYPE_SUPERBLOCK, 0, 8), "littlefs"},
                {LFS_MKTAG(LFS_TYPE_INLINESTRUCT, 0, sizeof(superblock)),
                    &superblock}));
        if (err) {
            goto cleanup;
        }

        // sanity check that fetch works
        err = lfs_dir_fetch(lfs, &root, (const lfs_block_t[2]){0, 1});
        if (err) {
            goto cleanup;
        }

        // force compaction to prevent accidentally mounting any
        // older version of littlefs that may live on disk
        root.erased = false;
        err = lfs_dir_commit(lfs, &root, NULL, 0);
        if (err) {
            goto cleanup;
        }
    }

cleanup:
    lfs_deinit(lfs);
    LFS_TRACE("lfs_format -> %d", err);
    return err;
}

int lfs_mount(lfs_t *lfs, const struct lfs_config *cfg) {
    LFS_TRACE("lfs_mount(%p, %p {.context=%p, "
                ".read=%p, .prog=%p, .erase=%p, .sync=%p, "
                ".read_size=%"PRIu32", .prog_size=%"PRIu32", "
                ".block_size=%"PRIu32", .block_count=%"PRIu32", "
                ".block_cycles=%"PRIu32", .cache_size=%"PRIu32", "
                ".lookahead_size=%"PRIu32", .read_buffer=%p, "
                ".prog_buffer=%p, .lookahead_buffer=%p, "
                ".name_max=%"PRIu32", .file_max=%"PRIu32", "
                ".attr_max=%"PRIu32"})",
            (void*)lfs, (void*)cfg, cfg->context,
            (void*)(uintptr_t)cfg->read, (void*)(uintptr_t)cfg->prog,
            (void*)(uintptr_t)cfg->erase, (void*)(uintptr_t)cfg->sync,
            cfg->read_size, cfg->prog_size, cfg->block_size, cfg->block_count,
            cfg->block_cycles, cfg->cache_size, cfg->lookahead_size,
            cfg->read_buffer, cfg->prog_buffer, cfg->lookahead_buffer,
            cfg->name_max, cfg->file_max, cfg->attr_max);
    int err = lfs_init(lfs, cfg);
    if (err) {
        LFS_TRACE("lfs_mount -> %d", err);
        return err;
    }

    // scan directory blocks for superblock and any global updates
    lfs_mdir_t dir = {.tail = {0, 1}};
    lfs_block_t cycle = 0;
    while (!lfs_pair_isnull(dir.tail)) {
        if (cycle >= lfs->cfg->block_count/2) {
            // loop detected
            err = LFS_ERR_CORRUPT;
            goto cleanup;
        }
        cycle += 1;

        // fetch next block in tail list
        lfs_stag_t tag = lfs_dir_fetchmatch(lfs, &dir, dir.tail,
                LFS_MKTAG(0x7ff, 0x3ff, 0),
                LFS_MKTAG(LFS_TYPE_SUPERBLOCK, 0, 8),
                NULL,
                lfs_dir_find_match, &(struct lfs_dir_find_match){
                    lfs, "littlefs", 8});
        if (tag < 0) {
            err = tag;
            goto cleanup;
        }

        // has superblock?
        if (tag && !lfs_tag_isdelete(tag)) {
            // update root
            lfs->root[0] = dir.pair[0];
            lfs->root[1] = dir.pair[1];

            // grab superblock
            lfs_superblock_t superblock;
            tag = lfs_dir_get(lfs, &dir, LFS_MKTAG(0x7ff, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_INLINESTRUCT, 0, sizeof(superblock)),
                    &superblock);
            if (tag < 0) {
                err = tag;
                goto cleanup;
            }
            lfs_superblock_fromle32(&superblock);

            // check version
            uint16_t major_version = (0xffff & (superblock.version >> 16));
            uint16_t minor_version = (0xffff & (superblock.version >>  0));
            if ((major_version != LFS_DISK_VERSION_MAJOR ||
                 minor_version > LFS_DISK_VERSION_MINOR)) {
                LFS_ERROR("Invalid version v%"PRIu16".%"PRIu16,
                        major_version, minor_version);
                err = LFS_ERR_INVAL;
                goto cleanup;
            }

            // check superblock configuration
            if (superblock.name_max) {
                if (superblock.name_max > lfs->name_max) {
                    LFS_ERROR("Unsupported name_max (%"PRIu32" > %"PRIu32")",
                            superblock.name_max, lfs->name_max);
                    err = LFS_ERR_INVAL;
                    goto cleanup;
                }

                lfs->name_max = superblock.name_max;
            }

            if (superblock.file_max) {
                if (superblock.file_max > lfs->file_max) {
                    LFS_ERROR("Unsupported file_max (%"PRIu32" > %"PRIu32")",
                            superblock.file_max, lfs->file_max);
                    err = LFS_ERR_INVAL;
                    goto cleanup;
                }

                lfs->file_max = superblock.file_max;
            }

            if (superblock.attr_max) {
                if (superblock.attr_max > lfs->attr_max) {
                    LFS_ERROR("Unsupported attr_max (%"PRIu32" > %"PRIu32")",
                            superblock.attr_max, lfs->attr_max);
                    err = LFS_ERR_INVAL;
                    goto cleanup;
                }

                lfs->attr_max = superblock.attr_max;
            }
        }

        // has gstate?
        err = lfs_dir_getgstate(lfs, &dir, &lfs->gstate);
        if (err) {
            goto cleanup;
        }
    }

    // found superblock?
    if (lfs_pair_isnull(lfs->root)) {
        err = LFS_ERR_INVAL;
        goto cleanup;
    }

    // update littlefs with gstate
    if (!lfs_gstate_iszero(&lfs->gstate)) {
        LFS_DEBUG("Found pending gstate 0x%08"PRIx32"%08"PRIx32"%08"PRIx32,
                lfs->gstate.tag,
                lfs->gstate.pair[0],
                lfs->gstate.pair[1]);
    }
    lfs->gstate.tag += !lfs_tag_isvalid(lfs->gstate.tag);
    lfs->gdisk = lfs->gstate;

    // setup free lookahead
    lfs_alloc_reset(lfs);

    LFS_TRACE("lfs_mount -> %d", 0);
    return 0;

cleanup:
    lfs_unmount(lfs);
    LFS_TRACE("lfs_mount -> %d", err);
    return err;
}

int lfs_unmount(lfs_t *lfs) {
    LFS_TRACE("lfs_unmount(%p)", (void*)lfs);
    int err = lfs_deinit(lfs);
    LFS_TRACE("lfs_unmount -> %d", err);
    return err;
}


/// Filesystem filesystem operations ///
int lfs_fs_traverseraw(lfs_t *lfs,
        int (*cb)(void *data, lfs_block_t block), void *data,
        bool includeorphans) {
    // iterate over metadata pairs
    lfs_mdir_t dir = {.tail = {0, 1}};

#ifdef LFS_MIGRATE
    // also consider v1 blocks during migration
    if (lfs->lfs1) {
        int err = lfs1_traverse(lfs, cb, data);
        if (err) {
            return err;
        }

        dir.tail[0] = lfs->root[0];
        dir.tail[1] = lfs->root[1];
    }
#endif

    lfs_block_t cycle = 0;
    while (!lfs_pair_isnull(dir.tail)) {
        if (cycle >= lfs->cfg->block_count/2) {
            // loop detected
            return LFS_ERR_CORRUPT;
        }
        cycle += 1;

        for (int i = 0; i < 2; i++) {
            int err = cb(data, dir.tail[i]);
            if (err) {
                return err;
            }
        }

        // iterate through ids in directory
        int err = lfs_dir_fetch(lfs, &dir, dir.tail);
        if (err) {
            return err;
        }

        for (uint16_t id = 0; id < dir.count; id++) {
            struct lfs_ctz ctz;
            lfs_stag_t tag = lfs_dir_get(lfs, &dir, LFS_MKTAG(0x700, 0x3ff, 0),
                    LFS_MKTAG(LFS_TYPE_STRUCT, id, sizeof(ctz)), &ctz);
            if (tag < 0) {
                if (tag == LFS_ERR_NOENT) {
                    continue;
                }
                return tag;
            }
            lfs_ctz_fromle32(&ctz);

            if (lfs_tag_type3(tag) == LFS_TYPE_CTZSTRUCT) {
                err = lfs_ctz_traverse(lfs, NULL, &lfs->rcache,
                        ctz.head, ctz.size, cb, data);
                if (err) {
                    return err;
                }
            } else if (includeorphans && 
                    lfs_tag_type3(tag) == LFS_TYPE_DIRSTRUCT) {
                for (int i = 0; i < 2; i++) {
                    err = cb(data, (&ctz.head)[i]);
                    if (err) {
                        return err;
                    }
                }
            }
        }
    }

    // iterate over any open files
    for (lfs_file_t *f = (lfs_file_t*)lfs->mlist; f; f = f->next) {
        if (f->type != LFS_TYPE_REG) {
            continue;
        }

        if ((f->flags & LFS_F_DIRTY) && !(f->flags & LFS_F_INLINE)) {
            int err = lfs_ctz_traverse(lfs, &f->cache, &lfs->rcache,
                    f->ctz.head, f->ctz.size, cb, data);
            if (err) {
                return err;
            }
        }

        if ((f->flags & LFS_F_WRITING) && !(f->flags & LFS_F_INLINE)) {
            int err = lfs_ctz_traverse(lfs, &f->cache, &lfs->rcache,
                    f->block, f->pos, cb, data);
            if (err) {
                return err;
            }
        }
    }

    return 0;
}

int lfs_fs_traverse(lfs_t *lfs,
        int (*cb)(void *data, lfs_block_t block), void *data) {
    LFS_TRACE("lfs_fs_traverse(%p, %p, %p)",
            (void*)lfs, (void*)(uintptr_t)cb, data);
    int err = lfs_fs_traverseraw(lfs, cb, data, true);
    LFS_TRACE("lfs_fs_traverse -> %d", 0);
    return err;
}

static int lfs_fs_pred(lfs_t *lfs,
        const lfs_block_t pair[2], lfs_mdir_t *pdir) {
    // iterate over all directory directory entries
    pdir->tail[0] = 0;
    pdir->tail[1] = 1;
    lfs_block_t cycle = 0;
    while (!lfs_pair_isnull(pdir->tail)) {
        if (cycle >= lfs->cfg->block_count/2) {
            // loop detected
            return LFS_ERR_CORRUPT;
        }
        cycle += 1;

        if (lfs_pair_cmp(pdir->tail, pair) == 0) {
            return 0;
        }

        int err = lfs_dir_fetch(lfs, pdir, pdir->tail);
        if (err) {
            return err;
        }
    }

    return LFS_ERR_NOENT;
}

struct lfs_fs_parent_match {
    lfs_t *lfs;
    const lfs_block_t pair[2];
};

static int lfs_fs_parent_match(void *data,
        lfs_tag_t tag, const void *buffer) {
    struct lfs_fs_parent_match *find = data;
    lfs_t *lfs = find->lfs;
    const struct lfs_diskoff *disk = buffer;
    (void)tag;

    lfs_block_t child[2];
    int err = lfs_bd_read(lfs,
            &lfs->pcache, &lfs->rcache, lfs->cfg->block_size,
            disk->block, disk->off, &child, sizeof(child));
    if (err) {
        return err;
    }

    lfs_pair_fromle32(child);
    return (lfs_pair_cmp(child, find->pair) == 0) ? LFS_CMP_EQ : LFS_CMP_LT;
}

static lfs_stag_t lfs_fs_parent(lfs_t *lfs, const lfs_block_t pair[2],
        lfs_mdir_t *parent) {
    // use fetchmatch with callback to find pairs
    parent->tail[0] = 0;
    parent->tail[1] = 1;
    lfs_block_t cycle = 0;
    while (!lfs_pair_isnull(parent->tail)) {
        if (cycle >= lfs->cfg->block_count/2) {
            // loop detected
            return LFS_ERR_CORRUPT;
        }
        cycle += 1;

        lfs_stag_t tag = lfs_dir_fetchmatch(lfs, parent, parent->tail,
                LFS_MKTAG(0x7ff, 0, 0x3ff),
                LFS_MKTAG(LFS_TYPE_DIRSTRUCT, 0, 8),
                NULL,
                lfs_fs_parent_match, &(struct lfs_fs_parent_match){
                    lfs, {pair[0], pair[1]}});
        if (tag && tag != LFS_ERR_NOENT) {
            return tag;
        }
    }

    return LFS_ERR_NOENT;
}

static int lfs_fs_relocate(lfs_t *lfs,
        const lfs_block_t oldpair[2], lfs_block_t newpair[2]) {
    // update internal root
    if (lfs_pair_cmp(oldpair, lfs->root) == 0) {
        lfs->root[0] = newpair[0];
        lfs->root[1] = newpair[1];
    }

    // update internally tracked dirs
    for (struct lfs_mlist *d = lfs->mlist; d; d = d->next) {
        if (lfs_pair_cmp(oldpair, d->m.pair) == 0) {
            d->m.pair[0] = newpair[0];
            d->m.pair[1] = newpair[1];
        }

        if (d->type == LFS_TYPE_DIR &&
                lfs_pair_cmp(oldpair, ((lfs_dir_t*)d)->head) == 0) {
            ((lfs_dir_t*)d)->head[0] = newpair[0];
            ((lfs_dir_t*)d)->head[1] = newpair[1];
        }
    }

    // find parent
    lfs_mdir_t parent;
    lfs_stag_t tag = lfs_fs_parent(lfs, oldpair, &parent);
    if (tag < 0 && tag != LFS_ERR_NOENT) {
        return tag;
    }

    if (tag != LFS_ERR_NOENT) {
        // update disk, this creates a desync
        lfs_fs_preporphans(lfs, +1);

        // fix pending move in this pair? this looks like an optimization but
        // is in fact _required_ since relocating may outdate the move.
        uint16_t moveid = 0x3ff;
        if (lfs_gstate_hasmovehere(&lfs->gstate, parent.pair)) {
            moveid = lfs_tag_id(lfs->gstate.tag);
            LFS_DEBUG("Fixing move while relocating "
                    "{0x%"PRIx32", 0x%"PRIx32"} 0x%"PRIx16"\n",
                    parent.pair[0], parent.pair[1], moveid);
            lfs_fs_prepmove(lfs, 0x3ff, NULL);
            if (moveid < lfs_tag_id(tag)) {
                tag -= LFS_MKTAG(0, 1, 0);
            }
        }

        lfs_pair_tole32(newpair);
        int err = lfs_dir_commit(lfs, &parent, LFS_MKATTRS(
                {LFS_MKTAG_IF(moveid != 0x3ff,
                    LFS_TYPE_DELETE, moveid, 0), NULL},
                {tag, newpair}));
        lfs_pair_fromle32(newpair);
        if (err) {
            return err;
        }

        // next step, clean up orphans
        lfs_fs_preporphans(lfs, -1);
    }

    // find pred
    int err = lfs_fs_pred(lfs, oldpair, &parent);
    if (err && err != LFS_ERR_NOENT) {
        return err;
    }

    // if we can't find dir, it must be new
    if (err != LFS_ERR_NOENT) {
        // fix pending move in this pair? this looks like an optimization but
        // is in fact _required_ since relocating may outdate the move.
        uint16_t moveid = 0x3ff;
        if (lfs_gstate_hasmovehere(&lfs->gstate, parent.pair)) {
            moveid = lfs_tag_id(lfs->gstate.tag);
            LFS_DEBUG("Fixing move while relocating "
                    "{0x%"PRIx32", 0x%"PRIx32"} 0x%"PRIx16"\n",
                    parent.pair[0], parent.pair[1], moveid);
            lfs_fs_prepmove(lfs, 0x3ff, NULL);
        }

        // replace bad pair, either we clean up desync, or no desync occured
        lfs_pair_tole32(newpair);
        err = lfs_dir_commit(lfs, &parent, LFS_MKATTRS(
                {LFS_MKTAG_IF(moveid != 0x3ff,
                    LFS_TYPE_DELETE, moveid, 0), NULL},
                {LFS_MKTAG(LFS_TYPE_TAIL + parent.split, 0x3ff, 8), newpair}));
        lfs_pair_fromle32(newpair);
        if (err) {
            return err;
        }
    }

    return 0;
}

static void lfs_fs_preporphans(lfs_t *lfs, int8_t orphans) {
    LFS_ASSERT(lfs_tag_size(lfs->gstate.tag) > 0 || orphans >= 0);
    lfs->gstate.tag += orphans;
    lfs->gstate.tag = ((lfs->gstate.tag & ~LFS_MKTAG(0x800, 0, 0)) |
            ((uint32_t)lfs_gstate_hasorphans(&lfs->gstate) << 31));
}

static void lfs_fs_prepmove(lfs_t *lfs,
        uint16_t id, const lfs_block_t pair[2]) {
    lfs->gstate.tag = ((lfs->gstate.tag & ~LFS_MKTAG(0x7ff, 0x3ff, 0)) |
            ((id != 0x3ff) ? LFS_MKTAG(LFS_TYPE_DELETE, id, 0) : 0));
    lfs->gstate.pair[0] = (id != 0x3ff) ? pair[0] : 0;
    lfs->gstate.pair[1] = (id != 0x3ff) ? pair[1] : 0;
}

static int lfs_fs_demove(lfs_t *lfs) {
    if (!lfs_gstate_hasmove(&lfs->gdisk)) {
        return 0;
    }

    // Fix bad moves
    LFS_DEBUG("Fixing move {0x%"PRIx32", 0x%"PRIx32"} 0x%"PRIx16,
            lfs->gdisk.pair[0],
            lfs->gdisk.pair[1],
            lfs_tag_id(lfs->gdisk.tag));

    // fetch and delete the moved entry
    lfs_mdir_t movedir;
    int err = lfs_dir_fetch(lfs, &movedir, lfs->gdisk.pair);
    if (err) {
        return err;
    }

    // prep gstate and delete move id
    uint16_t moveid = lfs_tag_id(lfs->gdisk.tag);
    lfs_fs_prepmove(lfs, 0x3ff, NULL);
    err = lfs_dir_commit(lfs, &movedir, LFS_MKATTRS(
            {LFS_MKTAG(LFS_TYPE_DELETE, moveid, 0), NULL}));
    if (err) {
        return err;
    }

    return 0;
}

static int lfs_fs_deorphan(lfs_t *lfs) {
    if (!lfs_gstate_hasorphans(&lfs->gstate)) {
        return 0;
    }

    // Fix any orphans
    lfs_mdir_t pdir = {.split = true, .tail = {0, 1}};
    lfs_mdir_t dir;

    // iterate over all directory directory entries
    while (!lfs_pair_isnull(pdir.tail)) {
        int err = lfs_dir_fetch(lfs, &dir, pdir.tail);
        if (err) {
            return err;
        }

        // check head blocks for orphans
        if (!pdir.split) {
            // check if we have a parent
            lfs_mdir_t parent;
            lfs_stag_t tag = lfs_fs_parent(lfs, pdir.tail, &parent);
            if (tag < 0 && tag != LFS_ERR_NOENT) {
                return tag;
            }

            if (tag == LFS_ERR_NOENT) {
                // we are an orphan
                LFS_DEBUG("Fixing orphan {0x%"PRIx32", 0x%"PRIx32"}",
                        pdir.tail[0], pdir.tail[1]);

                err = lfs_dir_drop(lfs, &pdir, &dir);
                if (err) {
                    return err;
                }

                // refetch tail
                continue;
            }

            lfs_block_t pair[2];
            lfs_stag_t res = lfs_dir_get(lfs, &parent,
                    LFS_MKTAG(0x7ff, 0x3ff, 0), tag, pair);
            if (res < 0) {
                return res;
            }
            lfs_pair_fromle32(pair);

            if (!lfs_pair_sync(pair, pdir.tail)) {
                // we have desynced
                LFS_DEBUG("Fixing half-orphan {0x%"PRIx32", 0x%"PRIx32"} "
                            "-> {0x%"PRIx32", 0x%"PRIx32"}",
                        pdir.tail[0], pdir.tail[1], pair[0], pair[1]);

                lfs_pair_tole32(pair);
                err = lfs_dir_commit(lfs, &pdir, LFS_MKATTRS(
                        {LFS_MKTAG(LFS_TYPE_SOFTTAIL, 0x3ff, 8), pair}));
                lfs_pair_fromle32(pair);
                if (err) {
                    return err;
                }

                // refetch tail
                continue;
            }
        }

        pdir = dir;
    }

    // mark orphans as fixed
    lfs_fs_preporphans(lfs, -lfs_gstate_getorphans(&lfs->gstate));
    return 0;
}

static int lfs_fs_forceconsistency(lfs_t *lfs) {
    int err = lfs_fs_demove(lfs);
    if (err) {
        return err;
    }

    err = lfs_fs_deorphan(lfs);
    if (err) {
        return err;
    }

    return 0;
}

static int lfs_fs_size_count(void *p, lfs_block_t block) {
    (void)block;
    lfs_size_t *size = p;
    *size += 1;
    return 0;
}

lfs_ssize_t lfs_fs_size(lfs_t *lfs) {
    LFS_TRACE("lfs_fs_size(%p)", (void*)lfs);
    lfs_size_t size = 0;
    int err = lfs_fs_traverseraw(lfs, lfs_fs_size_count, &size, false);
    if (err) {
        LFS_TRACE("lfs_fs_size -> %d", err);
        return err;
    }

    LFS_TRACE("lfs_fs_size -> %d", err);
    return size;
}

#ifdef LFS_MIGRATE
////// Migration from littelfs v1 below this //////

/// Version info ///

// Software library version
// Major (top-nibble), incremented on backwards incompatible changes
// Minor (bottom-nibble), incremented on feature additions
#define LFS1_VERSION 0x00010007
#define LFS1_VERSION_MAJOR (0xffff & (LFS1_VERSION >> 16))
#define LFS1_VERSION_MINOR (0xffff & (LFS1_VERSION >>  0))

// Version of On-disk data structures
// Major (top-nibble), incremented on backwards incompatible changes
// Minor (bottom-nibble), incremented on feature additions
#define LFS1_DISK_VERSION 0x00010001
#define LFS1_DISK_VERSION_MAJOR (0xffff & (LFS1_DISK_VERSION >> 16))
#define LFS1_DISK_VERSION_MINOR (0xffff & (LFS1_DISK_VERSION >>  0))


/// v1 Definitions ///

// File types
enum lfs1_type {
    LFS1_TYPE_REG        = 0x11,
    LFS1_TYPE_DIR        = 0x22,
    LFS1_TYPE_SUPERBLOCK = 0x2e,
};

typedef struct lfs1 {
    lfs_block_t root[2];
} lfs1_t;

typedef struct lfs1_entry {
    lfs_off_t off;

    struct lfs1_disk_entry {
        uint8_t type;
        uint8_t elen;
        uint8_t alen;
        uint8_t nlen;
        union {
            struct {
                lfs_block_t head;
                lfs_size_t size;
            } file;
            lfs_block_t dir[2];
        } u;
    } d;
} lfs1_entry_t;

typedef struct lfs1_dir {
    struct lfs1_dir *next;
    lfs_block_t pair[2];
    lfs_off_t off;

    lfs_block_t head[2];
    lfs_off_t pos;

    struct lfs1_disk_dir {
        uint32_t rev;
        lfs_size_t size;
        lfs_block_t tail[2];
    } d;
} lfs1_dir_t;

typedef struct lfs1_superblock {
    lfs_off_t off;

    struct lfs1_disk_superblock {
        uint8_t type;
        uint8_t elen;
        uint8_t alen;
        uint8_t nlen;
        lfs_block_t root[2];
        uint32_t block_size;
        uint32_t block_count;
        uint32_t version;
        char magic[8];
    } d;
} lfs1_superblock_t;


/// Low-level wrappers v1->v2 ///
static void lfs1_crc(uint32_t *crc, const void *buffer, size_t size) {
    *crc = lfs_crc(*crc, buffer, size);
}

static int lfs1_bd_read(lfs_t *lfs, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    // if we ever do more than writes to alternating pairs,
    // this may need to consider pcache
    return lfs_bd_read(lfs, &lfs->pcache, &lfs->rcache, size,
            block, off, buffer, size);
}

static int lfs1_bd_crc(lfs_t *lfs, lfs_block_t block,
        lfs_off_t off, lfs_size_t size, uint32_t *crc) {
    for (lfs_off_t i = 0; i < size; i++) {
        uint8_t c;
        int err = lfs1_bd_read(lfs, block, off+i, &c, 1);
        if (err) {
            return err;
        }

        lfs1_crc(crc, &c, 1);
    }

    return 0;
}


/// Endian swapping functions ///
static void lfs1_dir_fromle32(struct lfs1_disk_dir *d) {
    d->rev     = lfs_fromle32(d->rev);
    d->size    = lfs_fromle32(d->size);
    d->tail[0] = lfs_fromle32(d->tail[0]);
    d->tail[1] = lfs_fromle32(d->tail[1]);
}

static void lfs1_dir_tole32(struct lfs1_disk_dir *d) {
    d->rev     = lfs_tole32(d->rev);
    d->size    = lfs_tole32(d->size);
    d->tail[0] = lfs_tole32(d->tail[0]);
    d->tail[1] = lfs_tole32(d->tail[1]);
}

static void lfs1_entry_fromle32(struct lfs1_disk_entry *d) {
    d->u.dir[0] = lfs_fromle32(d->u.dir[0]);
    d->u.dir[1] = lfs_fromle32(d->u.dir[1]);
}

static void lfs1_entry_tole32(struct lfs1_disk_entry *d) {
    d->u.dir[0] = lfs_tole32(d->u.dir[0]);
    d->u.dir[1] = lfs_tole32(d->u.dir[1]);
}

static void lfs1_superblock_fromle32(struct lfs1_disk_superblock *d) {
    d->root[0]     = lfs_fromle32(d->root[0]);
    d->root[1]     = lfs_fromle32(d->root[1]);
    d->block_size  = lfs_fromle32(d->block_size);
    d->block_count = lfs_fromle32(d->block_count);
    d->version     = lfs_fromle32(d->version);
}


///// Metadata pair and directory operations ///
static inline lfs_size_t lfs1_entry_size(const lfs1_entry_t *entry) {
    return 4 + entry->d.elen + entry->d.alen + entry->d.nlen;
}

static int lfs1_dir_fetch(lfs_t *lfs,
        lfs1_dir_t *dir, const lfs_block_t pair[2]) {
    // copy out pair, otherwise may be aliasing dir
    const lfs_block_t tpair[2] = {pair[0], pair[1]};
    bool valid = false;

    // check both blocks for the most recent revision
    for (int i = 0; i < 2; i++) {
        struct lfs1_disk_dir test;
        int err = lfs1_bd_read(lfs, tpair[i], 0, &test, sizeof(test));
        lfs1_dir_fromle32(&test);
        if (err) {
            if (err == LFS_ERR_CORRUPT) {
                continue;
            }
            return err;
        }

        if (valid && lfs_scmp(test.rev, dir->d.rev) < 0) {
            continue;
        }

        if ((0x7fffffff & test.size) < sizeof(test)+4 ||
            (0x7fffffff & test.size) > lfs->cfg->block_size) {
            continue;
        }

        uint32_t crc = 0xffffffff;
        lfs1_dir_tole32(&test);
        lfs1_crc(&crc, &test, sizeof(test));
        lfs1_dir_fromle32(&test);
        err = lfs1_bd_crc(lfs, tpair[i], sizeof(test),
                (0x7fffffff & test.size) - sizeof(test), &crc);
        if (err) {
            if (err == LFS_ERR_CORRUPT) {
                continue;
            }
            return err;
        }

        if (crc != 0) {
            continue;
        }

        valid = true;

        // setup dir in case it's valid
        dir->pair[0] = tpair[(i+0) % 2];
        dir->pair[1] = tpair[(i+1) % 2];
        dir->off = sizeof(dir->d);
        dir->d = test;
    }

    if (!valid) {
        LFS_ERROR("Corrupted dir pair at {0x%"PRIx32", 0x%"PRIx32"}",
                tpair[0], tpair[1]);
        return LFS_ERR_CORRUPT;
    }

    return 0;
}

static int lfs1_dir_next(lfs_t *lfs, lfs1_dir_t *dir, lfs1_entry_t *entry) {
    while (dir->off + sizeof(entry->d) > (0x7fffffff & dir->d.size)-4) {
        if (!(0x80000000 & dir->d.size)) {
            entry->off = dir->off;
            return LFS_ERR_NOENT;
        }

        int err = lfs1_dir_fetch(lfs, dir, dir->d.tail);
        if (err) {
            return err;
        }

        dir->off = sizeof(dir->d);
        dir->pos += sizeof(dir->d) + 4;
    }

    int err = lfs1_bd_read(lfs, dir->pair[0], dir->off,
            &entry->d, sizeof(entry->d));
    lfs1_entry_fromle32(&entry->d);
    if (err) {
        return err;
    }

    entry->off = dir->off;
    dir->off += lfs1_entry_size(entry);
    dir->pos += lfs1_entry_size(entry);
    return 0;
}

/// littlefs v1 specific operations ///
int lfs1_traverse(lfs_t *lfs, int (*cb)(void*, lfs_block_t), void *data) {
    if (lfs_pair_isnull(lfs->lfs1->root)) {
        return 0;
    }

    // iterate over metadata pairs
    lfs1_dir_t dir;
    lfs1_entry_t entry;
    lfs_block_t cwd[2] = {0, 1};

    while (true) {
        for (int i = 0; i < 2; i++) {
            int err = cb(data, cwd[i]);
            if (err) {
                return err;
            }
        }

        int err = lfs1_dir_fetch(lfs, &dir, cwd);
        if (err) {
            return err;
        }

        // iterate over contents
        while (dir.off + sizeof(entry.d) <= (0x7fffffff & dir.d.size)-4) {
            err = lfs1_bd_read(lfs, dir.pair[0], dir.off,
                    &entry.d, sizeof(entry.d));
            lfs1_entry_fromle32(&entry.d);
            if (err) {
                return err;
            }

            dir.off += lfs1_entry_size(&entry);
            if ((0x70 & entry.d.type) == (0x70 & LFS1_TYPE_REG)) {
                err = lfs_ctz_traverse(lfs, NULL, &lfs->rcache,
                        entry.d.u.file.head, entry.d.u.file.size, cb, data);
                if (err) {
                    return err;
                }
            }
        }

        // we also need to check if we contain a threaded v2 directory
        lfs_mdir_t dir2 = {.split=true, .tail={cwd[0], cwd[1]}};
        while (dir2.split) {
            err = lfs_dir_fetch(lfs, &dir2, dir2.tail);
            if (err) {
                break;
            }

            for (int i = 0; i < 2; i++) {
                err = cb(data, dir2.pair[i]);
                if (err) {
                    return err;
                }
            }
        }

        cwd[0] = dir.d.tail[0];
        cwd[1] = dir.d.tail[1];

        if (lfs_pair_isnull(cwd)) {
            break;
        }
    }

    return 0;
}

static int lfs1_moved(lfs_t *lfs, const void *e) {
    if (lfs_pair_isnull(lfs->lfs1->root)) {
        return 0;
    }

    // skip superblock
    lfs1_dir_t cwd;
    int err = lfs1_dir_fetch(lfs, &cwd, (const lfs_block_t[2]){0, 1});
    if (err) {
        return err;
    }

    // iterate over all directory directory entries
    lfs1_entry_t entry;
    while (!lfs_pair_isnull(cwd.d.tail)) {
        err = lfs1_dir_fetch(lfs, &cwd, cwd.d.tail);
        if (err) {
            return err;
        }

        while (true) {
            err = lfs1_dir_next(lfs, &cwd, &entry);
            if (err && err != LFS_ERR_NOENT) {
                return err;
            }

            if (err == LFS_ERR_NOENT) {
                break;
            }

            if (!(0x80 & entry.d.type) &&
                 memcmp(&entry.d.u, e, sizeof(entry.d.u)) == 0) {
                return true;
            }
        }
    }

    return false;
}

/// Filesystem operations ///
static int lfs1_mount(lfs_t *lfs, struct lfs1 *lfs1,
        const struct lfs_config *cfg) {
    int err = 0;
    {
        err = lfs_init(lfs, cfg);
        if (err) {
            return err;
        }

        lfs->lfs1 = lfs1;
        lfs->lfs1->root[0] = LFS_BLOCK_NULL;
        lfs->lfs1->root[1] = LFS_BLOCK_NULL;

        // setup free lookahead
        lfs->free.off = 0;
        lfs->free.size = 0;
        lfs->free.i = 0;
        lfs_alloc_ack(lfs);

        // load superblock
        lfs1_dir_t dir;
        lfs1_superblock_t superblock;
        err = lfs1_dir_fetch(lfs, &dir, (const lfs_block_t[2]){0, 1});
        if (err && err != LFS_ERR_CORRUPT) {
            goto cleanup;
        }

        if (!err) {
            err = lfs1_bd_read(lfs, dir.pair[0], sizeof(dir.d),
                    &superblock.d, sizeof(superblock.d));
            lfs1_superblock_fromle32(&superblock.d);
            if (err) {
                goto cleanup;
            }

            lfs->lfs1->root[0] = superblock.d.root[0];
            lfs->lfs1->root[1] = superblock.d.root[1];
        }

        if (err || memcmp(superblock.d.magic, "littlefs", 8) != 0) {
            LFS_ERROR("Invalid superblock at {0x%"PRIx32", 0x%"PRIx32"}",
                    0, 1);
            err = LFS_ERR_CORRUPT;
            goto cleanup;
        }

        uint16_t major_version = (0xffff & (superblock.d.version >> 16));
        uint16_t minor_version = (0xffff & (superblock.d.version >>  0));
        if ((major_version != LFS1_DISK_VERSION_MAJOR ||
             minor_version > LFS1_DISK_VERSION_MINOR)) {
            LFS_ERROR("Invalid version v%d.%d", major_version, minor_version);
            err = LFS_ERR_INVAL;
            goto cleanup;
        }

        return 0;
    }

cleanup:
    lfs_deinit(lfs);
    return err;
}

static int lfs1_unmount(lfs_t *lfs) {
    return lfs_deinit(lfs);
}

/// v1 migration ///
int lfs_migrate(lfs_t *lfs, const struct lfs_config *cfg) {
    LFS_TRACE("lfs_migrate(%p, %p {.context=%p, "
                ".read=%p, .prog=%p, .erase=%p, .sync=%p, "
                ".read_size=%"PRIu32", .prog_size=%"PRIu32", "
                ".block_size=%"PRIu32", .block_count=%"PRIu32", "
                ".block_cycles=%"PRIu32", .cache_size=%"PRIu32", "
                ".lookahead_size=%"PRIu32", .read_buffer=%p, "
                ".prog_buffer=%p, .lookahead_buffer=%p, "
                ".name_max=%"PRIu32", .file_max=%"PRIu32", "
                ".attr_max=%"PRIu32"})",
            (void*)lfs, (void*)cfg, cfg->context,
            (void*)(uintptr_t)cfg->read, (void*)(uintptr_t)cfg->prog,
            (void*)(uintptr_t)cfg->erase, (void*)(uintptr_t)cfg->sync,
            cfg->read_size, cfg->prog_size, cfg->block_size, cfg->block_count,
            cfg->block_cycles, cfg->cache_size, cfg->lookahead_size,
            cfg->read_buffer, cfg->prog_buffer, cfg->lookahead_buffer,
            cfg->name_max, cfg->file_max, cfg->attr_max);
    struct lfs1 lfs1;
    int err = lfs1_mount(lfs, &lfs1, cfg);
    if (err) {
        LFS_TRACE("lfs_migrate -> %d", err);
        return err;
    }

    {
        // iterate through each directory, copying over entries
        // into new directory
        lfs1_dir_t dir1;
        lfs_mdir_t dir2;
        dir1.d.tail[0] = lfs->lfs1->root[0];
        dir1.d.tail[1] = lfs->lfs1->root[1];
        while (!lfs_pair_isnull(dir1.d.tail)) {
            // iterate old dir
            err = lfs1_dir_fetch(lfs, &dir1, dir1.d.tail);
            if (err) {
                goto cleanup;
            }

            // create new dir and bind as temporary pretend root
            err = lfs_dir_alloc(lfs, &dir2);
            if (err) {
                goto cleanup;
            }

            dir2.rev = dir1.d.rev;
            dir1.head[0] = dir1.pair[0];
            dir1.head[1] = dir1.pair[1];
            lfs->root[0] = dir2.pair[0];
            lfs->root[1] = dir2.pair[1];

            err = lfs_dir_commit(lfs, &dir2, NULL, 0);
            if (err) {
                goto cleanup;
            }

            while (true) {
                lfs1_entry_t entry1;
                err = lfs1_dir_next(lfs, &dir1, &entry1);
                if (err && err != LFS_ERR_NOENT) {
                    goto cleanup;
                }

                if (err == LFS_ERR_NOENT) {
                    break;
                }

                // check that entry has not been moved
                if (entry1.d.type & 0x80) {
                    int moved = lfs1_moved(lfs, &entry1.d.u);
                    if (moved < 0) {
                        err = moved;
                        goto cleanup;
                    }

                    if (moved) {
                        continue;
                    }

                    entry1.d.type &= ~0x80;
                }

                // also fetch name
                char name[LFS_NAME_MAX+1];
                memset(name, 0, sizeof(name));
                err = lfs1_bd_read(lfs, dir1.pair[0],
                        entry1.off + 4+entry1.d.elen+entry1.d.alen,
                        name, entry1.d.nlen);
                if (err) {
                    goto cleanup;
                }

                bool isdir = (entry1.d.type == LFS1_TYPE_DIR);

                // create entry in new dir
                err = lfs_dir_fetch(lfs, &dir2, lfs->root);
                if (err) {
                    goto cleanup;
                }

                uint16_t id;
                err = lfs_dir_find(lfs, &dir2, &(const char*){name}, &id);
                if (!(err == LFS_ERR_NOENT && id != 0x3ff)) {
                    err = (err < 0) ? err : LFS_ERR_EXIST;
                    goto cleanup;
                }

                lfs1_entry_tole32(&entry1.d);
                err = lfs_dir_commit(lfs, &dir2, LFS_MKATTRS(
                        {LFS_MKTAG(LFS_TYPE_CREATE, id, 0)},
                        {LFS_MKTAG_IF_ELSE(isdir,
                            LFS_TYPE_DIR, id, entry1.d.nlen,
                            LFS_TYPE_REG, id, entry1.d.nlen),
                                name},
                        {LFS_MKTAG_IF_ELSE(isdir,
                            LFS_TYPE_DIRSTRUCT, id, sizeof(entry1.d.u),
                            LFS_TYPE_CTZSTRUCT, id, sizeof(entry1.d.u)),
                                &entry1.d.u}));
                lfs1_entry_fromle32(&entry1.d);
                if (err) {
                    goto cleanup;
                }
            }

            if (!lfs_pair_isnull(dir1.d.tail)) {
                // find last block and update tail to thread into fs
                err = lfs_dir_fetch(lfs, &dir2, lfs->root);
                if (err) {
                    goto cleanup;
                }

                while (dir2.split) {
                    err = lfs_dir_fetch(lfs, &dir2, dir2.tail);
                    if (err) {
                        goto cleanup;
                    }
                }

                lfs_pair_tole32(dir2.pair);
                err = lfs_dir_commit(lfs, &dir2, LFS_MKATTRS(
                        {LFS_MKTAG(LFS_TYPE_SOFTTAIL, 0x3ff, 8), dir1.d.tail}));
                lfs_pair_fromle32(dir2.pair);
                if (err) {
                    goto cleanup;
                }
            }

            // Copy over first block to thread into fs. Unfortunately
            // if this fails there is not much we can do.
            LFS_DEBUG("Migrating {0x%"PRIx32", 0x%"PRIx32"} "
                        "-> {0x%"PRIx32", 0x%"PRIx32"}",
                    lfs->root[0], lfs->root[1], dir1.head[0], dir1.head[1]);

            err = lfs_bd_erase(lfs, dir1.head[1]);
            if (err) {
                goto cleanup;
            }

            err = lfs_dir_fetch(lfs, &dir2, lfs->root);
            if (err) {
                goto cleanup;
            }

            for (lfs_off_t i = 0; i < dir2.off; i++) {
                uint8_t dat;
                err = lfs_bd_read(lfs,
                        NULL, &lfs->rcache, dir2.off,
                        dir2.pair[0], i, &dat, 1);
                if (err) {
                    goto cleanup;
                }

                err = lfs_bd_prog(lfs,
                        &lfs->pcache, &lfs->rcache, true,
                        dir1.head[1], i, &dat, 1);
                if (err) {
                    goto cleanup;
                }
            }

            err = lfs_bd_flush(lfs, &lfs->pcache, &lfs->rcache, true);
            if (err) {
                goto cleanup;
            }
        }

        // Create new superblock. This marks a successful migration!
        err = lfs1_dir_fetch(lfs, &dir1, (const lfs_block_t[2]){0, 1});
        if (err) {
            goto cleanup;
        }

        dir2.pair[0] = dir1.pair[0];
        dir2.pair[1] = dir1.pair[1];
        dir2.rev = dir1.d.rev;
        dir2.off = sizeof(dir2.rev);
        dir2.etag = 0xffffffff;
        dir2.count = 0;
        dir2.tail[0] = lfs->lfs1->root[0];
        dir2.tail[1] = lfs->lfs1->root[1];
        dir2.erased = false;
        dir2.split = true;

        lfs_superblock_t superblock = {
            .version     = LFS_DISK_VERSION,
            .block_size  = lfs->cfg->block_size,
            .block_count = lfs->cfg->block_count,
            .name_max    = lfs->name_max,
            .file_max    = lfs->file_max,
            .attr_max    = lfs->attr_max,
        };

        lfs_superblock_tole32(&superblock);
        err = lfs_dir_commit(lfs, &dir2, LFS_MKATTRS(
                {LFS_MKTAG(LFS_TYPE_CREATE, 0, 0)},
                {LFS_MKTAG(LFS_TYPE_SUPERBLOCK, 0, 8), "littlefs"},
                {LFS_MKTAG(LFS_TYPE_INLINESTRUCT, 0, sizeof(superblock)),
                    &superblock}));
        if (err) {
            goto cleanup;
        }

        // sanity check that fetch works
        err = lfs_dir_fetch(lfs, &dir2, (const lfs_block_t[2]){0, 1});
        if (err) {
            goto cleanup;
        }

        // force compaction to prevent accidentally mounting v1
        dir2.erased = false;
        err = lfs_dir_commit(lfs, &dir2, NULL, 0);
        if (err) {
            goto cleanup;
        }
    }

cleanup:
    lfs1_unmount(lfs);
    LFS_TRACE("lfs_migrate -> %d", err);
    return err;
}

#endif
