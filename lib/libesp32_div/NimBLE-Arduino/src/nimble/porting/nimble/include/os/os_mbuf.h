/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */


/**
 * @addtogroup OSKernel
 * @{
 *   @defgroup OSMbuf Chained Memory Buffers
 *   @{
 */


#ifndef _OS_MBUF_H
#define _OS_MBUF_H

#include "os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A mbuf pool from which to allocate mbufs. This contains a pointer to the os
 * mempool to allocate mbufs out of, the total number of elements in the pool,
 * and the amount of "user" data in a non-packet header mbuf. The total pool
 * size, in bytes, should be:
 *  os_mbuf_count * (omp_databuf_len + sizeof(struct os_mbuf))
 */
struct os_mbuf_pool {
    /**
     * Total length of the databuf in each mbuf.  This is the size of the
     * mempool block, minus the mbuf header
     */
    uint16_t omp_databuf_len;
    /**
     * The memory pool which to allocate mbufs out of
     */
    struct os_mempool *omp_pool;

    STAILQ_ENTRY(os_mbuf_pool) omp_next;
};


/**
 * A packet header structure that preceeds the mbuf packet headers.
 */
struct os_mbuf_pkthdr {
    /**
     * Overall length of the packet.
     */
    uint16_t omp_len;
    /**
     * Flags
     */
    uint16_t omp_flags;

    STAILQ_ENTRY(os_mbuf_pkthdr) omp_next;
};

/**
 * Chained memory buffer.
 */
struct os_mbuf {
    /**
     * Current pointer to data in the structure
     */
    uint8_t *om_data;
    /**
     * Flags associated with this buffer, see OS_MBUF_F_* defintions
     */
    uint8_t om_flags;
    /**
     * Length of packet header
     */
    uint8_t om_pkthdr_len;
    /**
     * Length of data in this buffer
     */
    uint16_t om_len;

    /**
     * The mbuf pool this mbuf was allocated out of
     */
    struct os_mbuf_pool *om_omp;

    SLIST_ENTRY(os_mbuf) om_next;

    /**
     * Pointer to the beginning of the data, after this buffer
     */
    uint8_t om_databuf[0];
};

/**
 * Structure representing a queue of mbufs.
 */
struct os_mqueue {
    STAILQ_HEAD(, os_mbuf_pkthdr) mq_head;
    /** Event to post when new buffers are available on the queue. */
    struct ble_npl_event mq_ev;
};

/*
 * Given a flag number, provide the mask for it
 *
 * @param __n The number of the flag in the mask
 */
#define OS_MBUF_F_MASK(__n) (1 << (__n))

/*
 * Checks whether a given mbuf is a packet header mbuf
 *
 * @param __om The mbuf to check
 */
#define OS_MBUF_IS_PKTHDR(__om) \
    ((__om)->om_pkthdr_len >= sizeof (struct os_mbuf_pkthdr))

/** Get a packet header pointer given an mbuf pointer */
#define OS_MBUF_PKTHDR(__om) ((struct os_mbuf_pkthdr *)     \
    ((uint8_t *)&(__om)->om_data + sizeof(struct os_mbuf)))

/** Given a mbuf packet header pointer, return a pointer to the mbuf */
#define OS_MBUF_PKTHDR_TO_MBUF(__hdr)   \
     (struct os_mbuf *)((uint8_t *)(__hdr) - sizeof(struct os_mbuf))

/**
 * Gets the length of an entire mbuf chain.  The specified mbuf must have a
 * packet header.
 */
#define OS_MBUF_PKTLEN(__om) (OS_MBUF_PKTHDR(__om)->omp_len)

/**
 * Access the data of a mbuf, and cast it to type
 *
 * @param __om The mbuf to access, and cast
 * @param __type The type to cast it to
 */
#define OS_MBUF_DATA(__om, __type) \
     (__type) ((__om)->om_data)

/**
 * Access the "user header" in the head of an mbuf chain.
 *
 * @param om                    Pointer to the head of an mbuf chain.
 */
#define OS_MBUF_USRHDR(om)                              \
    (void *)((uint8_t *)om + sizeof (struct os_mbuf) +  \
             sizeof (struct os_mbuf_pkthdr))

/**
 * Retrieves the length of the user header in an mbuf.
 *
 * @param om                    Pointer to the mbuf to query.
 */
#define OS_MBUF_USRHDR_LEN(om) \
    ((om)->om_pkthdr_len - sizeof (struct os_mbuf_pkthdr))


/** @cond INTERNAL_HIDDEN */

/*
 * Called by OS_MBUF_LEADINGSPACE() macro
 */
static inline uint16_t
_os_mbuf_leadingspace(struct os_mbuf *om)
{
    uint16_t startoff;
    uint16_t leadingspace;

    startoff = 0;
    if (OS_MBUF_IS_PKTHDR(om)) {
        startoff = om->om_pkthdr_len;
    }

    leadingspace = (uint16_t) (OS_MBUF_DATA(om, uint8_t *) -
        ((uint8_t *) &om->om_databuf[0] + startoff));

    return (leadingspace);
}

/** @endcond */

/**
 * Returns the leading space (space at the beginning) of the mbuf.
 * Works on both packet header, and regular mbufs, as it accounts
 * for the additional space allocated to the packet header.
 *
 * @param __omp Is the mbuf pool (which contains packet header length.)
 * @param __om  Is the mbuf in that pool to get the leadingspace for
 *
 * @return Amount of leading space available in the mbuf
 */
#define OS_MBUF_LEADINGSPACE(__om) _os_mbuf_leadingspace(__om)


/** @cond INTERNAL_HIDDEN */

/* Called by OS_MBUF_TRAILINGSPACE() macro. */
static inline uint16_t
_os_mbuf_trailingspace(struct os_mbuf *om)
{
    struct os_mbuf_pool *omp;

    omp = om->om_omp;

    return (&om->om_databuf[0] + omp->omp_databuf_len) -
      (om->om_data + om->om_len);
}

/** @endcond */

/**
 * Returns the trailing space (space at the end) of the mbuf.
 * Works on both packet header and regular mbufs.
 *
 * @param __omp The mbuf pool for this mbuf
 * @param __om  Is the mbuf in that pool to get trailing space for
 *
 * @return The amount of trailing space available in the mbuf
 */
#define OS_MBUF_TRAILINGSPACE(__om) _os_mbuf_trailingspace(__om)


/**
 * Initializes an mqueue.  An mqueue is a queue of mbufs that ties to a
 * particular task's event queue.  Mqueues form a helper API around a common
 * paradigm: wait on an event queue until at least one packet is available,
 * then process a queue of packets.
 *
 * When mbufs are available on the queue, an event OS_EVENT_T_MQUEUE_DATA
 * will be posted to the task's mbuf queue.
 *
 * @param mq                    The mqueue to initialize
 * @param ev_cb                 The callback to associate with the mqeueue
 *                                  event.  Typically, this callback pulls each
 *                                  packet off the mqueue and processes them.
 * @param arg                   The argument to associate with the mqueue event.
 *
 * @return                      0 on success, non-zero on failure.
 */
int os_mqueue_init(struct os_mqueue *mq, ble_npl_event_fn *ev_cb, void *arg);

/**
 * Remove and return a single mbuf from the mbuf queue.  Does not block.
 *
 * @param mq The mbuf queue to pull an element off of.
 *
 * @return The next mbuf in the queue, or NULL if queue has no mbufs.
 */
struct os_mbuf *os_mqueue_get(struct os_mqueue *);

/**
 * Adds a packet (i.e. packet header mbuf) to an mqueue. The event associated
 * with the mqueue gets posted to the specified eventq.
 *
 * @param mq                    The mbuf queue to append the mbuf to.
 * @param evq                   The event queue to post an event to.
 * @param m                     The mbuf to append to the mbuf queue.
 *
 * @return 0 on success, non-zero on failure.
 */
int os_mqueue_put(struct os_mqueue *, struct ble_npl_eventq *, struct os_mbuf *);

/**
 * MSYS is a system level mbuf registry.  Allows the system to share
 * packet buffers amongst the various networking stacks that can be running
 * simultaeneously.
 *
 * Mbuf pools are created in the system initialization code, and then when
 * a mbuf is allocated out of msys, it will try and find the best fit based
 * upon estimated mbuf size.
 *
 * os_msys_register() registers a mbuf pool with MSYS, and allows MSYS to
 * allocate mbufs out of it.
 *
 * @param new_pool The pool to register with MSYS
 *
 * @return 0 on success, non-zero on failure
 */
int os_msys_register(struct os_mbuf_pool *);

/**
 * Allocate a mbuf from msys.  Based upon the data size requested,
 * os_msys_get() will choose the mbuf pool that has the best fit.
 *
 * @param dsize The estimated size of the data being stored in the mbuf
 * @param leadingspace The amount of leadingspace to allocate in the mbuf
 *
 * @return A freshly allocated mbuf on success, NULL on failure.
 */
struct os_mbuf *os_msys_get(uint16_t dsize, uint16_t leadingspace);

/**
 * De-registers all mbuf pools from msys.
 */
void os_msys_reset(void);

/**
 * Allocate a packet header structure from the MSYS pool.  See
 * os_msys_register() for a description of MSYS.
 *
 * @param dsize The estimated size of the data being stored in the mbuf
 * @param user_hdr_len The length to allocate for the packet header structure
 *
 * @return A freshly allocated mbuf on success, NULL on failure.
 */
struct os_mbuf *os_msys_get_pkthdr(uint16_t dsize, uint16_t user_hdr_len);

/**
 * Count the number of blocks in all the mbuf pools that are allocated.
 *
 * @return total number of blocks allocated in Msys
 */
int os_msys_count(void);

/**
 * Return the number of free blocks in Msys
 *
 * @return Number of free blocks available in Msys
 */
int os_msys_num_free(void);

/**
 * Initialize a pool of mbufs.
 *
 * @param omp     The mbuf pool to initialize
 * @param mp      The memory pool that will hold this mbuf pool
 * @param buf_len The length of the buffer itself.
 * @param nbufs   The number of buffers in the pool
 *
 * @return 0 on success, error code on failure.
 */
int os_mbuf_pool_init(struct os_mbuf_pool *, struct os_mempool *mp,
        uint16_t, uint16_t);

/**
 * Get an mbuf from the mbuf pool.  The mbuf is allocated, and initialized
 * prior to being returned.
 *
 * @param omp The mbuf pool to return the packet from
 * @param leadingspace The amount of leadingspace to put before the data
 *     section by default.
 *
 * @return An initialized mbuf on success, and NULL on failure.
 */
struct os_mbuf *os_mbuf_get(struct os_mbuf_pool *omp, uint16_t);

/**
 * Allocate a new packet header mbuf out of the os_mbuf_pool.
 *
 * @param omp The mbuf pool to allocate out of
 * @param user_pkthdr_len The packet header length to reserve for the caller.
 *
 * @return A freshly allocated mbuf on success, NULL on failure.
 */
struct os_mbuf *os_mbuf_get_pkthdr(struct os_mbuf_pool *omp,
        uint8_t pkthdr_len);

/**
 * Duplicate a chain of mbufs.  Return the start of the duplicated chain.
 *
 * @param omp The mbuf pool to duplicate out of
 * @param om  The mbuf chain to duplicate
 *
 * @return A pointer to the new chain of mbufs
 */
struct os_mbuf *os_mbuf_dup(struct os_mbuf *m);

/**
 * Locates the specified absolute offset within an mbuf chain.  The offset
 * can be one past than the total length of the chain, but no greater.
 *
 * @param om                    The start of the mbuf chain to seek within.
 * @param off                   The absolute address to find.
 * @param out_off               On success, this points to the relative offset
 *                                  within the returned mbuf.
 *
 * @return                      The mbuf containing the specified offset on
 *                                  success.
 *                              NULL if the specified offset is out of bounds.
 */
struct os_mbuf *os_mbuf_off(const struct os_mbuf *om, int off,
                            uint16_t *out_off);


/*
 * Copy data from an mbuf chain starting "off" bytes from the beginning,
 * continuing for "len" bytes, into the indicated buffer.
 *
 * @param m The mbuf chain to copy from
 * @param off The offset into the mbuf chain to begin copying from
 * @param len The length of the data to copy
 * @param dst The destination buffer to copy into
 *
 * @return                      0 on success;
 *                              -1 if the mbuf does not contain enough data.
 */
int os_mbuf_copydata(const struct os_mbuf *m, int off, int len, void *dst);

/**
 * Append data onto a mbuf
 *
 * @param om   The mbuf to append the data onto
 * @param data The data to append onto the mbuf
 * @param len  The length of the data to append
 *
 * @return 0 on success, and an error code on failure
 */
int os_mbuf_append(struct os_mbuf *m, const void *, uint16_t);

/**
 * Reads data from one mbuf and appends it to another.  On error, the specified
 * data range may be partially appended.  Neither mbuf is required to contain
 * an mbuf packet header.
 *
 * @param dst                   The mbuf to append to.
 * @param src                   The mbuf to copy data from.
 * @param src_off               The absolute offset within the source mbuf
 *                                  chain to read from.
 * @param len                   The number of bytes to append.
 *
 * @return                      0 on success;
 *                              OS_EINVAL if the specified range extends beyond
 *                                  the end of the source mbuf chain.
 */
int os_mbuf_appendfrom(struct os_mbuf *dst, const struct os_mbuf *src,
                       uint16_t src_off, uint16_t len);

/**
 * Release a mbuf back to the pool
 *
 * @param omp The Mbuf pool to release back to
 * @param om  The Mbuf to release back to the pool
 *
 * @return 0 on success, -1 on failure
 */
int os_mbuf_free(struct os_mbuf *mb);

/**
 * Free a chain of mbufs
 *
 * @param omp The mbuf pool to free the chain of mbufs into
 * @param om  The starting mbuf of the chain to free back into the pool
 *
 * @return 0 on success, -1 on failure
 */
int os_mbuf_free_chain(struct os_mbuf *om);

/**
 * Adjust the length of a mbuf, trimming either from the head or the tail
 * of the mbuf.
 *
 * @param mp The mbuf chain to adjust
 * @param req_len The length to trim from the mbuf.  If positive, trims
 *                from the head of the mbuf, if negative, trims from the
 *                tail of the mbuf.
 */
void os_mbuf_adj(struct os_mbuf *mp, int req_len);


/**
 * Performs a memory compare of the specified region of an mbuf chain against a
 * flat buffer.
 *
 * @param om                    The start of the mbuf chain to compare.
 * @param off                   The offset within the mbuf chain to start the
 *                                  comparison.
 * @param data                  The flat buffer to compare.
 * @param len                   The length of the flat buffer.
 *
 * @return                      0 if both memory regions are identical;
 *                              A memcmp return code if there is a mismatch;
 *                              INT_MAX if the mbuf is too short.
 */
int os_mbuf_cmpf(const struct os_mbuf *om, int off, const void *data, int len);

/**
 * Compares the contents of two mbuf chains.  The ranges of the two chains to
 * be compared are specified via the two offset parameters and the len
 * parameter.  Neither mbuf chain is required to contain a packet header.
 *
 * @param om1                   The first mbuf chain to compare.
 * @param offset1               The absolute offset within om1 at which to
 *                                  start the comparison.
 * @param om2                   The second mbuf chain to compare.
 * @param offset2               The absolute offset within om2 at which to
 *                                  start the comparison.
 * @param len                   The number of bytes to compare.
 *
 * @return                      0 if both mbuf segments are identical;
 *                              A memcmp() return code if the segment contents
 *                                  differ;
 *                              INT_MAX if a specified range extends beyond the
 *                                  end of its corresponding mbuf chain.
 */
int os_mbuf_cmpm(const struct os_mbuf *om1, uint16_t offset1,
                 const struct os_mbuf *om2, uint16_t offset2,
                 uint16_t len);

/**
 * Increases the length of an mbuf chain by adding data to the front.  If there
 * is insufficient room in the leading mbuf, additional mbufs are allocated and
 * prepended as necessary.  If this function fails to allocate an mbuf, the
 * entire chain is freed.
 *
 * The specified mbuf chain does not need to contain a packet header.
 *
 * @param omp                   The mbuf pool to allocate from.
 * @param om                    The head of the mbuf chain.
 * @param len                   The number of bytes to prepend.
 *
 * @return                      The new head of the chain on success;
 *                              NULL on failure.
 */
struct os_mbuf *os_mbuf_prepend(struct os_mbuf *om, int len);

/**
 * Prepends a chunk of empty data to the specified mbuf chain and ensures the
 * chunk is contiguous.  If either operation fails, the specified mbuf chain is
 * freed and NULL is returned.
 *
 * @param om                    The mbuf chain to prepend to.
 * @param len                   The number of bytes to prepend and pullup.
 *
 * @return                      The modified mbuf on success;
 *                              NULL on failure (and the mbuf chain is freed).
 */
struct os_mbuf *os_mbuf_prepend_pullup(struct os_mbuf *om, uint16_t len);

/**
 * Copies the contents of a flat buffer into an mbuf chain, starting at the
 * specified destination offset.  If the mbuf is too small for the source data,
 * it is extended as necessary.  If the destination mbuf contains a packet
 * header, the header length is updated.
 *
 * @param omp                   The mbuf pool to allocate from.
 * @param om                    The mbuf chain to copy into.
 * @param off                   The offset within the chain to copy to.
 * @param src                   The source buffer to copy from.
 * @param len                   The number of bytes to copy.
 *
 * @return                      0 on success; nonzero on failure.
 */
int os_mbuf_copyinto(struct os_mbuf *om, int off, const void *src, int len);

/**
 * Attaches a second mbuf chain onto the end of the first.  If the first chain
 * contains a packet header, the header's length is updated.  If the second
 * chain has a packet header, its header is cleared.
 *
 * @param first                 The mbuf chain being attached to.
 * @param second                The mbuf chain that gets attached.
 */
void os_mbuf_concat(struct os_mbuf *first, struct os_mbuf *second);


/**
 * Increases the length of an mbuf chain by the specified amount.  If there is
 * not sufficient room in the last buffer, a new buffer is allocated and
 * appended to the chain.  It is an error to request more data than can fit in
 * a single buffer.
 *
 * @param omp
 * @param om                    The head of the chain to extend.
 * @param len                   The number of bytes to extend by.
 *
 * @return                      A pointer to the new data on success;
 *                              NULL on failure.
 */
void *os_mbuf_extend(struct os_mbuf *om, uint16_t len);

/**
 * Rearrange a mbuf chain so that len bytes are contiguous,
 * and in the data area of an mbuf (so that OS_MBUF_DATA() will
 * work on a structure of size len.)  Returns the resulting
 * mbuf chain on success, free's it and returns NULL on failure.
 *
 * If there is room, it will add up to "max_protohdr - len"
 * extra bytes to the contiguous region, in an attempt to avoid being
 * called next time.
 *
 * @param omp The mbuf pool to take the mbufs out of
 * @param om The mbuf chain to make contiguous
 * @param len The number of bytes in the chain to make contiguous
 *
 * @return The contiguous mbuf chain on success, NULL on failure.
 */
struct os_mbuf *os_mbuf_pullup(struct os_mbuf *om, uint16_t len);


/**
 * Removes and frees empty mbufs from the front of a chain.  If the chain
 * contains a packet header, it is preserved.
 *
 * @param om                    The mbuf chain to trim.
 *
 * @return                      The head of the trimmed mbuf chain.
 */
struct os_mbuf *os_mbuf_trim_front(struct os_mbuf *om);

/**
 * Creates a single chained mbuf from m1 and m2 utilizing all
 * the available buffer space in all mbufs in the resulting
 * chain. In other words, ensures there is no leading space in
 * any mbuf in the resulting chain and trailing space only in
 * the last mbuf in the chain. Mbufs from either chain may be
 * freed if not needed. No mbufs are allocated. Note that mbufs
 * from m2 are added to the end of m1. If m1 has a packet
 * header, it is retained and length updated. If m2 has a packet
 * header it is discarded. If m1 is NULL, NULL is returned and
 * m2 is left untouched.
 *
 * @param m1 Pointer to first mbuf chain to pack
 * @param m2 Pointer to second mbuf chain to pack
 *
 * @return struct os_mbuf* Pointer to resulting mbuf chain
 */
struct os_mbuf *os_mbuf_pack_chains(struct os_mbuf *m1, struct os_mbuf *m2);

#ifdef __cplusplus
}
#endif

#endif /* _OS_MBUF_H */


/**
 *   @} OSMbuf
 * @} OSKernel
 */
