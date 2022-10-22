/*
 * Software in this file is based heavily on code written in the FreeBSD source
 * code repostiory.  While the code is written from scratch, it contains
 * many of the ideas and logic flow in the original source, this is a
 * derivative work, and the following license applies as well:
 *
 * Copyright (c) 1982, 1986, 1988, 1991, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "../include/os/os.h"
#include "../include/os/os_trace_api.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

/**
 * @addtogroup OSKernel
 * @{
 *   @defgroup OSMqueue Queue of Mbufs
 *   @{
 */

STAILQ_HEAD(, os_mbuf_pool) g_msys_pool_list =
    STAILQ_HEAD_INITIALIZER(g_msys_pool_list);


int
os_mqueue_init(struct os_mqueue *mq, ble_npl_event_fn *ev_cb, void *arg)
{
    struct ble_npl_event *ev;

    STAILQ_INIT(&mq->mq_head);

    ev = &mq->mq_ev;
    ble_npl_event_init(ev, ev_cb, arg);

    return (0);
}

struct os_mbuf *
os_mqueue_get(struct os_mqueue *mq)
{
    struct os_mbuf_pkthdr *mp;
    struct os_mbuf *m;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    mp = STAILQ_FIRST(&mq->mq_head);
    if (mp) {
        STAILQ_REMOVE_HEAD(&mq->mq_head, omp_next);
    }
    OS_EXIT_CRITICAL(sr);

    if (mp) {
        m = OS_MBUF_PKTHDR_TO_MBUF(mp);
    } else {
        m = NULL;
    }

    return (m);
}

int
os_mqueue_put(struct os_mqueue *mq, struct ble_npl_eventq *evq, struct os_mbuf *m)
{
    struct os_mbuf_pkthdr *mp;
    os_sr_t sr;
    int rc;

    /* Can only place the head of a chained mbuf on the queue. */
    if (!OS_MBUF_IS_PKTHDR(m)) {
        rc = OS_EINVAL;
        goto err;
    }

    mp = OS_MBUF_PKTHDR(m);

    OS_ENTER_CRITICAL(sr);
    STAILQ_INSERT_TAIL(&mq->mq_head, mp, omp_next);
    OS_EXIT_CRITICAL(sr);

    /* Only post an event to the queue if its specified */
    if (evq) {
        ble_npl_eventq_put(evq, &mq->mq_ev);
    }

    return (0);
err:
    return (rc);
}

int
os_msys_register(struct os_mbuf_pool *new_pool)
{
    struct os_mbuf_pool *pool;

    pool = NULL;
    STAILQ_FOREACH(pool, &g_msys_pool_list, omp_next) {
        if (new_pool->omp_databuf_len > pool->omp_databuf_len) {
            break;
        }
    }

    if (pool) {
        STAILQ_INSERT_AFTER(&g_msys_pool_list, pool, new_pool, omp_next);
    } else {
        STAILQ_INSERT_TAIL(&g_msys_pool_list, new_pool, omp_next);
    }

    return (0);
}

void
os_msys_reset(void)
{
    STAILQ_INIT(&g_msys_pool_list);
}

static struct os_mbuf_pool *
_os_msys_find_pool(uint16_t dsize)
{
    struct os_mbuf_pool *pool;

    pool = NULL;
    STAILQ_FOREACH(pool, &g_msys_pool_list, omp_next) {
        if (dsize <= pool->omp_databuf_len) {
            break;
        }
    }

    if (!pool) {
        pool = STAILQ_LAST(&g_msys_pool_list, os_mbuf_pool, omp_next);
    }

    return (pool);
}


struct os_mbuf *
os_msys_get(uint16_t dsize, uint16_t leadingspace)
{
    struct os_mbuf *m;
    struct os_mbuf_pool *pool;

    pool = _os_msys_find_pool(dsize);
    if (!pool) {
        goto err;
    }

    m = os_mbuf_get(pool, leadingspace);
    return (m);
err:
    return (NULL);
}

struct os_mbuf *
os_msys_get_pkthdr(uint16_t dsize, uint16_t user_hdr_len)
{
    uint16_t total_pkthdr_len;
    struct os_mbuf *m;
    struct os_mbuf_pool *pool;

    total_pkthdr_len =  user_hdr_len + sizeof(struct os_mbuf_pkthdr);
    pool = _os_msys_find_pool(dsize + total_pkthdr_len);
    if (!pool) {
        goto err;
    }

    m = os_mbuf_get_pkthdr(pool, user_hdr_len);
    return (m);
err:
    return (NULL);
}

int
os_msys_count(void)
{
    struct os_mbuf_pool *omp;
    int total;

    total = 0;
    STAILQ_FOREACH(omp, &g_msys_pool_list, omp_next) {
        total += omp->omp_pool->mp_num_blocks;
    }

    return total;
}

int
os_msys_num_free(void)
{
    struct os_mbuf_pool *omp;
    int total;

    total = 0;
    STAILQ_FOREACH(omp, &g_msys_pool_list, omp_next) {
        total += omp->omp_pool->mp_num_free;
    }

    return total;
}


int
os_mbuf_pool_init(struct os_mbuf_pool *omp, struct os_mempool *mp,
                  uint16_t buf_len, uint16_t nbufs)
{
    omp->omp_databuf_len = buf_len - sizeof(struct os_mbuf);
    omp->omp_pool = mp;

    return (0);
}

struct os_mbuf *
os_mbuf_get(struct os_mbuf_pool *omp, uint16_t leadingspace)
{
    struct os_mbuf *om;

    os_trace_api_u32x2(OS_TRACE_ID_MBUF_GET, (uint32_t)omp,
                       (uint32_t)(uintptr_t)leadingspace);

    if (leadingspace > omp->omp_databuf_len) {
        om = NULL;
        goto done;
    }

    om = os_memblock_get(omp->omp_pool);
    if (!om) {
        goto done;
    }

    SLIST_NEXT(om, om_next) = NULL;
    om->om_flags = 0;
    om->om_pkthdr_len = 0;
    om->om_len = 0;
    om->om_data = (&om->om_databuf[0] + leadingspace);
    om->om_omp = omp;

done:
    os_trace_api_ret_u32(OS_TRACE_ID_MBUF_GET, (uint32_t)(uintptr_t)om);
    return om;
}

struct os_mbuf *
os_mbuf_get_pkthdr(struct os_mbuf_pool *omp, uint8_t user_pkthdr_len)
{
    uint16_t pkthdr_len;
    struct os_mbuf_pkthdr *pkthdr;
    struct os_mbuf *om;

    os_trace_api_u32x2(OS_TRACE_ID_MBUF_GET_PKTHDR, (uint32_t)(uintptr_t)omp,
                       (uint32_t)user_pkthdr_len);

    /* User packet header must fit inside mbuf */
    pkthdr_len = user_pkthdr_len + sizeof(struct os_mbuf_pkthdr);
    if ((pkthdr_len > omp->omp_databuf_len) || (pkthdr_len > 255)) {
        om = NULL;
        goto done;
    }

    om = os_mbuf_get(omp, 0);
    if (om) {
        om->om_pkthdr_len = pkthdr_len;
        om->om_data += pkthdr_len;

        pkthdr = OS_MBUF_PKTHDR(om);
        pkthdr->omp_len = 0;
        pkthdr->omp_flags = 0;
        STAILQ_NEXT(pkthdr, omp_next) = NULL;
    }

done:
    os_trace_api_ret_u32(OS_TRACE_ID_MBUF_GET_PKTHDR, (uint32_t)(uintptr_t)om);
    return om;
}

int
os_mbuf_free(struct os_mbuf *om)
{
    int rc;

    os_trace_api_u32(OS_TRACE_ID_MBUF_FREE, (uint32_t)(uintptr_t)om);

    if (om->om_omp != NULL) {
        rc = os_memblock_put(om->om_omp->omp_pool, om);
        if (rc != 0) {
            goto done;
        }
    }

    rc = 0;

done:
    os_trace_api_ret_u32(OS_TRACE_ID_MBUF_FREE, (uint32_t)rc);
    return (rc);
}

int
os_mbuf_free_chain(struct os_mbuf *om)
{
    struct os_mbuf *next;
    int rc;

    os_trace_api_u32(OS_TRACE_ID_MBUF_FREE_CHAIN, (uint32_t)(uintptr_t)om);

    while (om != NULL) {
        next = SLIST_NEXT(om, om_next);

        rc = os_mbuf_free(om);
        if (rc != 0) {
            goto done;
        }

        om = next;
    }

    rc = 0;

done:
    os_trace_api_ret_u32(OS_TRACE_ID_MBUF_FREE_CHAIN, (uint32_t)rc);
    return (rc);
}

/**
 * Copy a packet header from one mbuf to another.
 *
 * @param omp The mbuf pool associated with these buffers
 * @param new_buf The new buffer to copy the packet header into
 * @param old_buf The old buffer to copy the packet header from
 */
static inline void
_os_mbuf_copypkthdr(struct os_mbuf *new_buf, struct os_mbuf *old_buf)
{
    assert(new_buf->om_len == 0);

    memcpy(&new_buf->om_databuf[0], &old_buf->om_databuf[0],
           old_buf->om_pkthdr_len);
    new_buf->om_pkthdr_len = old_buf->om_pkthdr_len;
    new_buf->om_data = new_buf->om_databuf + old_buf->om_pkthdr_len;
}

uint16_t
os_mbuf_len(const struct os_mbuf *om)
{
    uint16_t len;

    len = 0;
    while (om != NULL) {
        len += om->om_len;
        om = SLIST_NEXT(om, om_next);
    }

    return len;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
int
os_mbuf_append(struct os_mbuf *om, const void *data,  uint16_t len)
{
    struct os_mbuf_pool *omp;
    struct os_mbuf *last;
    struct os_mbuf *new;
    int remainder;
    int space;
    int rc;

    if (om == NULL) {
        rc = OS_EINVAL;
        goto err;
    }

    omp = om->om_omp;

    /* Scroll to last mbuf in the chain */
    last = om;
    while (SLIST_NEXT(last, om_next) != NULL) {
        last = SLIST_NEXT(last, om_next);
    }

    remainder = len;
    space = OS_MBUF_TRAILINGSPACE(last);

    /* If room in current mbuf, copy the first part of the data into the
     * remaining space in that mbuf.
     */
    if (space > 0) {
        if (space > remainder) {
            space = remainder;
        }

        memcpy(OS_MBUF_DATA(last, uint8_t *) + last->om_len , data, space);

        last->om_len += space;
        data += space;
        remainder -= space;
    }

    /* Take the remaining data, and keep allocating new mbufs and copying
     * data into it, until data is exhausted.
     */
    while (remainder > 0) {
        new = os_mbuf_get(omp, 0);
        if (!new) {
            break;
        }

        new->om_len = min(omp->omp_databuf_len, remainder);
        memcpy(OS_MBUF_DATA(new, void *), data, new->om_len);
        data += new->om_len;
        remainder -= new->om_len;
        SLIST_NEXT(last, om_next) = new;
        last = new;
    }

    /* Adjust the packet header length in the buffer */
    if (OS_MBUF_IS_PKTHDR(om)) {
        OS_MBUF_PKTHDR(om)->omp_len += len - remainder;
    }

    if (remainder != 0) {
        rc = OS_ENOMEM;
        goto err;
    }


    return (0);
err:
    return (rc);
}
#pragma GCC diagnostic pop

int
os_mbuf_appendfrom(struct os_mbuf *dst, const struct os_mbuf *src,
                   uint16_t src_off, uint16_t len)
{
    const struct os_mbuf *src_cur_om;
    uint16_t src_cur_off;
    uint16_t chunk_sz;
    int rc;

    src_cur_om = os_mbuf_off(src, src_off, &src_cur_off);
    while (len > 0) {
        if (src_cur_om == NULL) {
            return OS_EINVAL;
        }

        chunk_sz = min(len, src_cur_om->om_len - src_cur_off);
        rc = os_mbuf_append(dst, src_cur_om->om_data + src_cur_off, chunk_sz);
        if (rc != 0) {
            return rc;
        }

        len -= chunk_sz;
        src_cur_om = SLIST_NEXT(src_cur_om, om_next);
        src_cur_off = 0;
    }

    return 0;
}

struct os_mbuf *
os_mbuf_dup(struct os_mbuf *om)
{
    struct os_mbuf_pool *omp;
    struct os_mbuf *head;
    struct os_mbuf *copy;

    omp = om->om_omp;

    head = NULL;
    copy = NULL;

    for (; om != NULL; om = SLIST_NEXT(om, om_next)) {
        if (head) {
            SLIST_NEXT(copy, om_next) = os_mbuf_get(omp,
                    OS_MBUF_LEADINGSPACE(om));
            if (!SLIST_NEXT(copy, om_next)) {
                os_mbuf_free_chain(head);
                goto err;
            }

            copy = SLIST_NEXT(copy, om_next);
        } else {
            head = os_mbuf_get(omp, OS_MBUF_LEADINGSPACE(om));
            if (!head) {
                goto err;
            }

            if (OS_MBUF_IS_PKTHDR(om)) {
                _os_mbuf_copypkthdr(head, om);
            }
            copy = head;
        }
        copy->om_flags = om->om_flags;
        copy->om_len = om->om_len;
        memcpy(OS_MBUF_DATA(copy, uint8_t *), OS_MBUF_DATA(om, uint8_t *),
                om->om_len);
    }

    return (head);
err:
    return (NULL);
}

struct os_mbuf *
os_mbuf_off(const struct os_mbuf *om, int off, uint16_t *out_off)
{
    struct os_mbuf *next;
    struct os_mbuf *cur;

    /* Cast away const. */
    cur = (struct os_mbuf *)om;

    while (1) {
        if (cur == NULL) {
            return NULL;
        }

        next = SLIST_NEXT(cur, om_next);

        if (cur->om_len > off ||
            (cur->om_len == off && next == NULL)) {

            *out_off = off;
            return cur;
        }

        off -= cur->om_len;
        cur = next;
    }
}

int
os_mbuf_copydata(const struct os_mbuf *m, int off, int len, void *dst)
{
    unsigned int count;
    uint8_t *udst;

    if (!len) {
        return 0;
    }

    udst = dst;

    while (off > 0) {
        if (!m) {
            return (-1);
        }

        if (off < m->om_len)
            break;
        off -= m->om_len;
        m = SLIST_NEXT(m, om_next);
    }
    while (len > 0 && m != NULL) {
        count = min(m->om_len - off, len);
        memcpy(udst, m->om_data + off, count);
        len -= count;
        udst += count;
        off = 0;
        m = SLIST_NEXT(m, om_next);
    }

    return (len > 0 ? -1 : 0);
}

void
os_mbuf_adj(struct os_mbuf *mp, int req_len)
{
    int len = req_len;
    struct os_mbuf *m;
    int count;

    if ((m = mp) == NULL)
        return;
    if (len >= 0) {
        /*
         * Trim from head.
         */
        while (m != NULL && len > 0) {
            if (m->om_len <= len) {
                len -= m->om_len;
                m->om_len = 0;
                m = SLIST_NEXT(m, om_next);
            } else {
                m->om_len -= len;
                m->om_data += len;
                len = 0;
            }
        }
        if (OS_MBUF_IS_PKTHDR(mp))
            OS_MBUF_PKTHDR(mp)->omp_len -= (req_len - len);
    } else {
        /*
         * Trim from tail.  Scan the mbuf chain,
         * calculating its length and finding the last mbuf.
         * If the adjustment only affects this mbuf, then just
         * adjust and return.  Otherwise, rescan and truncate
         * after the remaining size.
         */
        len = -len;
        count = 0;
        for (;;) {
            count += m->om_len;
            if (SLIST_NEXT(m, om_next) == (struct os_mbuf *)0)
                break;
            m = SLIST_NEXT(m, om_next);
        }
        if (m->om_len >= len) {
            m->om_len -= len;
            if (OS_MBUF_IS_PKTHDR(mp))
                OS_MBUF_PKTHDR(mp)->omp_len -= len;
            return;
        }
        count -= len;
        if (count < 0)
            count = 0;
        /*
         * Correct length for chain is "count".
         * Find the mbuf with last data, adjust its length,
         * and toss data from remaining mbufs on chain.
         */
        m = mp;
        if (OS_MBUF_IS_PKTHDR(m))
            OS_MBUF_PKTHDR(m)->omp_len = count;
        for (; m; m = SLIST_NEXT(m, om_next)) {
            if (m->om_len >= count) {
                m->om_len = count;
                if (SLIST_NEXT(m, om_next) != NULL) {
                    os_mbuf_free_chain(SLIST_NEXT(m, om_next));
                    SLIST_NEXT(m, om_next) = NULL;
                }
                break;
            }
            count -= m->om_len;
        }
    }
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
int
os_mbuf_cmpf(const struct os_mbuf *om, int off, const void *data, int len)
{
    uint16_t chunk_sz;
    uint16_t data_off;
    uint16_t om_off;
    int rc;

    if (len <= 0) {
        return 0;
    }

    data_off = 0;
    om = os_mbuf_off(om, off, &om_off);
    while (1) {
        if (om == NULL) {
            return INT_MAX;
        }

        chunk_sz = min(om->om_len - om_off, len - data_off);
        if (chunk_sz > 0) {
            rc = memcmp(om->om_data + om_off, data + data_off, chunk_sz);
            if (rc != 0) {
                return rc;
            }
        }

        data_off += chunk_sz;
        if (data_off == len) {
            return 0;
        }

        om = SLIST_NEXT(om, om_next);
        om_off = 0;

        if (om == NULL) {
            return INT_MAX;
        }
    }
}
#pragma GCC diagnostic pop

int
os_mbuf_cmpm(const struct os_mbuf *om1, uint16_t offset1,
             const struct os_mbuf *om2, uint16_t offset2,
             uint16_t len)
{
    const struct os_mbuf *cur1;
    const struct os_mbuf *cur2;
    uint16_t bytes_remaining;
    uint16_t chunk_sz;
    uint16_t om1_left;
    uint16_t om2_left;
    uint16_t om1_off;
    uint16_t om2_off;
    int rc;

    om1_off = 0;
    om2_off = 0;

    cur1 = os_mbuf_off(om1, offset1, &om1_off);
    cur2 = os_mbuf_off(om2, offset2, &om2_off);

    bytes_remaining = len;
    while (1) {
        if (bytes_remaining == 0) {
            return 0;
        }

        while (cur1 != NULL && om1_off >= cur1->om_len) {
            cur1 = SLIST_NEXT(cur1, om_next);
            om1_off = 0;
        }
        while (cur2 != NULL && om2_off >= cur2->om_len) {
            cur2 = SLIST_NEXT(cur2, om_next);
            om2_off = 0;
        }

        if (cur1 == NULL || cur2 == NULL) {
            return INT_MAX;
        }

        om1_left = cur1->om_len - om1_off;
        om2_left = cur2->om_len - om2_off;
        chunk_sz = min(min(om1_left, om2_left), bytes_remaining);

        rc = memcmp(cur1->om_data + om1_off, cur2->om_data + om2_off,
                    chunk_sz);
        if (rc != 0) {
            return rc;
        }

        om1_off += chunk_sz;
        om2_off += chunk_sz;
        bytes_remaining -= chunk_sz;
    }
}

struct os_mbuf *
os_mbuf_prepend(struct os_mbuf *om, int len)
{
    struct os_mbuf *p;
    int leading;

    while (1) {
        /* Fill the available space at the front of the head of the chain, as
         * needed.
         */
        leading = min(len, OS_MBUF_LEADINGSPACE(om));

        om->om_data -= leading;
        om->om_len += leading;
        if (OS_MBUF_IS_PKTHDR(om)) {
            OS_MBUF_PKTHDR(om)->omp_len += leading;
        }

        len -= leading;
        if (len == 0) {
            break;
        }

        /* The current head didn't have enough space; allocate a new head. */
        if (OS_MBUF_IS_PKTHDR(om)) {
            p = os_mbuf_get_pkthdr(om->om_omp,
                om->om_pkthdr_len - sizeof (struct os_mbuf_pkthdr));
        } else {
            p = os_mbuf_get(om->om_omp, 0);
        }
        if (p == NULL) {
            os_mbuf_free_chain(om);
            om = NULL;
            break;
        }

        if (OS_MBUF_IS_PKTHDR(om)) {
            _os_mbuf_copypkthdr(p, om);
            om->om_pkthdr_len = 0;
        }

        /* Move the new head's data pointer to the end so that data can be
         * prepended.
         */
        p->om_data += OS_MBUF_TRAILINGSPACE(p);

        SLIST_NEXT(p, om_next) = om;
        om = p;
    }

    return om;
}

struct os_mbuf *
os_mbuf_prepend_pullup(struct os_mbuf *om, uint16_t len)
{
    om = os_mbuf_prepend(om, len);
    if (om == NULL) {
        return NULL;
    }

    om = os_mbuf_pullup(om, len);
    if (om == NULL) {
        return NULL;
    }

    return om;
}

int
os_mbuf_copyinto(struct os_mbuf *om, int off, const void *src, int len)
{
    struct os_mbuf *next;
    struct os_mbuf *cur;
    const uint8_t *sptr;
    uint16_t cur_off;
    int copylen;
    int rc;

    /* Find the mbuf,offset pair for the start of the destination. */
    cur = os_mbuf_off(om, off, &cur_off);
    if (cur == NULL) {
        return -1;
    }

    /* Overwrite existing data until we reach the end of the chain. */
    sptr = src;
    while (1) {
        copylen = min(cur->om_len - cur_off, len);
        if (copylen > 0) {
            memcpy(cur->om_data + cur_off, sptr, copylen);
            sptr += copylen;
            len -= copylen;

            copylen = 0;
        }

        if (len == 0) {
            /* All the source data fit in the existing mbuf chain. */
            return 0;
        }

        next = SLIST_NEXT(cur, om_next);
        if (next == NULL) {
            break;
        }

        cur = next;
        cur_off = 0;
    }

    /* Append the remaining data to the end of the chain. */
    rc = os_mbuf_append(cur, sptr, len);
    if (rc != 0) {
        return rc;
    }

    /* Fix up the packet header, if one is present. */
    if (OS_MBUF_IS_PKTHDR(om)) {
        OS_MBUF_PKTHDR(om)->omp_len =
            max(OS_MBUF_PKTHDR(om)->omp_len, off + len);
    }

    return 0;
}

void
os_mbuf_concat(struct os_mbuf *first, struct os_mbuf *second)
{
    struct os_mbuf *next;
    struct os_mbuf *cur;

    /* Point 'cur' to the last buffer in the first chain. */
    cur = first;
    while (1) {
        next = SLIST_NEXT(cur, om_next);
        if (next == NULL) {
            break;
        }

        cur = next;
    }

    /* Attach the second chain to the end of the first. */
    SLIST_NEXT(cur, om_next) = second;

    /* If the first chain has a packet header, calculate the length of the
     * second chain and add it to the header length.
     */
    if (OS_MBUF_IS_PKTHDR(first)) {
        if (OS_MBUF_IS_PKTHDR(second)) {
            OS_MBUF_PKTHDR(first)->omp_len += OS_MBUF_PKTHDR(second)->omp_len;
        } else {
            for (cur = second; cur != NULL; cur = SLIST_NEXT(cur, om_next)) {
                OS_MBUF_PKTHDR(first)->omp_len += cur->om_len;
            }
        }
    }

    second->om_pkthdr_len = 0;
}

void *
os_mbuf_extend(struct os_mbuf *om, uint16_t len)
{
    struct os_mbuf *newm;
    struct os_mbuf *last;
    void *data;

    if (len > om->om_omp->omp_databuf_len) {
        return NULL;
    }

    /* Scroll to last mbuf in the chain */
    last = om;
    while (SLIST_NEXT(last, om_next) != NULL) {
        last = SLIST_NEXT(last, om_next);
    }

    if (OS_MBUF_TRAILINGSPACE(last) < len) {
        newm = os_mbuf_get(om->om_omp, 0);
        if (newm == NULL) {
            return NULL;
        }

        SLIST_NEXT(last, om_next) = newm;
        last = newm;
    }

    data = last->om_data + last->om_len;
    last->om_len += len;

    if (OS_MBUF_IS_PKTHDR(om)) {
        OS_MBUF_PKTHDR(om)->omp_len += len;
    }

    return data;
}


struct os_mbuf *
os_mbuf_pullup(struct os_mbuf *om, uint16_t len)
{
    struct os_mbuf_pool *omp;
    struct os_mbuf *next;
    struct os_mbuf *om2;
    int count;
    int space;

    omp = om->om_omp;

    /*
     * If first mbuf has no cluster, and has room for len bytes
     * without shifting current data, pullup into it,
     * otherwise allocate a new mbuf to prepend to the chain.
     */
    if (om->om_len >= len) {
        return (om);
    }
    if (om->om_len + OS_MBUF_TRAILINGSPACE(om) >= len &&
        SLIST_NEXT(om, om_next)) {
        om2 = om;
        om = SLIST_NEXT(om, om_next);
        len -= om2->om_len;
    } else {
        if (len > omp->omp_databuf_len - om->om_pkthdr_len) {
            goto bad;
        }

        om2 = os_mbuf_get(omp, 0);
        if (om2 == NULL) {
            goto bad;
        }

        if (OS_MBUF_IS_PKTHDR(om)) {
            _os_mbuf_copypkthdr(om2, om);
        }
    }
    space = OS_MBUF_TRAILINGSPACE(om2);
    do {
        count = min(min(len, space), om->om_len);
        memcpy(om2->om_data + om2->om_len, om->om_data, count);
        len -= count;
        om2->om_len += count;
        om->om_len -= count;
        space -= count;
        if (om->om_len) {
            om->om_data += count;
        } else {
            next = SLIST_NEXT(om, om_next);
            os_mbuf_free(om);
            om = next;
        }
    } while (len > 0 && om);
    if (len > 0) {
        os_mbuf_free(om2);
        goto bad;
    }
    SLIST_NEXT(om2, om_next) = om;
    return (om2);
bad:
    os_mbuf_free_chain(om);
    return (NULL);
}

struct os_mbuf *
os_mbuf_trim_front(struct os_mbuf *om)
{
    struct os_mbuf *next;
    struct os_mbuf *cur;

    /* Abort early if there is nothing to trim. */
    if (om->om_len != 0) {
        return om;
    }

    /* Starting with the second mbuf in the chain, continue removing and
     * freeing mbufs until an non-empty one is encountered.
     */
    cur = SLIST_NEXT(om, om_next);
    while (cur != NULL && cur->om_len == 0) {
        next = SLIST_NEXT(cur, om_next);

        SLIST_NEXT(om, om_next) = next;
        os_mbuf_free(cur);

        cur = next;
    }

    if (cur == NULL) {
        /* All buffers after the first have been freed. */
        return om;
    }

    /* Try to remove the first mbuf in the chain.  If this buffer contains a
     * packet header, make sure the second buffer can accommodate it.
     */
    if (OS_MBUF_LEADINGSPACE(cur) >= om->om_pkthdr_len) {
        /* Second buffer has room; copy packet header. */
        cur->om_pkthdr_len = om->om_pkthdr_len;
        memcpy(OS_MBUF_PKTHDR(cur), OS_MBUF_PKTHDR(om), om->om_pkthdr_len);

        /* Free first buffer. */
        os_mbuf_free(om);
        om = cur;
    }

    return om;
}

int
os_mbuf_widen(struct os_mbuf *om, uint16_t off, uint16_t len)
{
    struct os_mbuf *first_new;
    struct os_mbuf *edge_om;
    struct os_mbuf *prev;
    struct os_mbuf *cur;
    uint16_t rem_len;
    uint16_t sub_off;
    int rc;

    /* Locate the mbuf and offset within the chain where the gap will be
     * inserted.
     */
    edge_om = os_mbuf_off(om, off, &sub_off);
    if (edge_om == NULL) {
        return OS_EINVAL;
    }

    /* If the mbuf has sufficient capacity for the gap, just make room within
     * the mbuf.
     */
    if (OS_MBUF_TRAILINGSPACE(edge_om) >= len) {
        memmove(edge_om->om_data + sub_off + len,
                edge_om->om_data + sub_off,
                edge_om->om_len - sub_off);
        edge_om->om_len += len;
        if (OS_MBUF_IS_PKTHDR(om)) {
            OS_MBUF_PKTHDR(om)->omp_len += len;
        }
        return 0;
    }

    /* Otherwise, allocate new mbufs until the chain has sufficient capacity
     * for the gap.
     */
    rem_len = len;
    first_new = NULL;
    prev = NULL;
    while (rem_len > 0) {
        cur = os_mbuf_get(om->om_omp, 0);
        if (cur == NULL) {
            /* Free only the mbufs that this function allocated. */
            os_mbuf_free_chain(first_new);
            return OS_ENOMEM;
        }

        /* Remember the start of the chain of new mbufs. */
        if (first_new == NULL) {
            first_new = cur;
        }

        if (rem_len > OS_MBUF_TRAILINGSPACE(cur)) {
            cur->om_len = OS_MBUF_TRAILINGSPACE(cur);
        } else {
            cur->om_len = rem_len;
        }
        rem_len -= cur->om_len;

        if (prev != NULL) {
            SLIST_NEXT(prev, om_next) = cur;
        }
        prev = cur;
    }

    /* Move the misplaced data from the edge mbuf over to the right side of the
     * gap.
     */
    rc = os_mbuf_append(prev, edge_om->om_data + sub_off,
                        edge_om->om_len - sub_off);
    if (rc != 0) {
        os_mbuf_free_chain(first_new);
        return OS_ENOMEM;
    }
    edge_om->om_len = sub_off;

    /* Insert the gap into the chain. */
    SLIST_NEXT(prev, om_next) = SLIST_NEXT(edge_om, om_next);
    SLIST_NEXT(edge_om, om_next) = first_new;

    if (OS_MBUF_IS_PKTHDR(om)) {
        OS_MBUF_PKTHDR(om)->omp_len += len;
    }

    return 0;
}

struct os_mbuf *
os_mbuf_pack_chains(struct os_mbuf *m1, struct os_mbuf *m2)
{
    uint16_t rem_len;
    uint16_t copylen;
    uint8_t *dptr;
    struct os_mbuf *cur;
    struct os_mbuf *next;

    /* If m1 is NULL, return NULL */
    if (m1 == NULL) {
        return NULL;
    }

    /*
     * Concatenate the two chains to start. This will discard packet header in
     * m2 and adjust packet length in m1 if m1 has a packet header.
     */
    if (m2 != NULL) {
        os_mbuf_concat(m1, m2);
    }

    cur = m1;
    while (1) {
        /* If there is leading space in the mbuf, move data up */
        if (OS_MBUF_LEADINGSPACE(cur)) {
            dptr = &cur->om_databuf[0];
            if (OS_MBUF_IS_PKTHDR(cur)) {
                dptr += cur->om_pkthdr_len;
            }
            memmove(dptr, cur->om_data, cur->om_len);
            cur->om_data = dptr;
        }

        /* Set pointer to where we will begin copying data in current mbuf */
        dptr = cur->om_data + cur->om_len;

        /* Get a pointer to the next buf we want to absorb */
        next = SLIST_NEXT(cur, om_next);

        /*
         * Is there trailing space in the mbuf? If so, copy data from
         * following mbufs into the current mbuf
         */
        rem_len = OS_MBUF_TRAILINGSPACE(cur);
        while (rem_len && next) {
            copylen = min(rem_len, next->om_len);
            memcpy(dptr, next->om_data, copylen);
            cur->om_len += copylen;
            dptr += copylen;
            rem_len -= copylen;

            /*
             * We copied bytes from the next mbuf. Move the data pointer
             * and subtract from its length
             */
            next->om_data += copylen;
            next->om_len -= copylen;

            /*
             * Keep removing and freeing consecutive zero length mbufs,
             * stopping when we find one with data in it or we have
             * reached the end. This will prevent any zero length mbufs
             * from remaining in the chain.
             */
            while (next->om_len == 0) {
                SLIST_NEXT(cur, om_next) = SLIST_NEXT(next, om_next);
                os_mbuf_free(next);
                next = SLIST_NEXT(cur, om_next);
                if (next == NULL) {
                    break;
                }
            }
        }

        /* If no mbufs are left, we are done */
        if (next == NULL) {
            break;
        }

        /* Move cur to next as we filled up current */
        cur = next;
    }

    return m1;
}
