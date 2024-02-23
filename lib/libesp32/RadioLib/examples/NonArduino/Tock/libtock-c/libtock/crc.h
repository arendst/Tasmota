#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CRC 0x40002

// CRC algorithms
//
// In all cases, input bytes are bit-reversed (i.e., consumed from LSB to MSB.)
//
// Algorithms prefixed with `SAM4L_` are native to that chip and thus require
// no software post-processing on platforms using it.
//
enum crc_alg {
    // Polynomial 0x04C11DB7, output reversed then inverted ("CRC-32")
    CRC_32,
    // Polynomial 0x1EDC6F41, output reversed then inverted ("CRC-32C" / "Castagnoli")
    CRC_32C,

    /// Polynomial 0x1021, no output post-processing
    CRC_16CCITT,
};

// Does the driver exist?
int crc_exists(void);

// Compute a CRC value over the given buffer using the given algorithm
//
// Returns SUCCESS and sets `result` on success.
// Returns EBUSY if a computation is already in progress.
// Returns ESIZE if the buffer is too big for the unit.
int crc_compute(const void *buf, size_t buflen, enum crc_alg, uint32_t *result);

// Register a callback to receive CRC results
//
// The callback will receive these parameters, in order:
//    status: SUCCESS if all inputs are valid, else EINVAL
//    result: When status == SUCCESS, the CRC result
int crc_subscribe(subscribe_upcall, void *);

// Provide the buffer over which to compute a CRC
int crc_set_buffer(const void*, size_t);

// Request a CRC computation asynchronously
//
// The callback and buffer must be provided first.
//
// If SUCCESS is returned, the result will be provided to
// the registered callback.
//
// Returns EBUSY if a computation is already in progress.
// Returns ESIZE if the buffer is too big for the unit.
int crc_request(enum crc_alg, size_t len);

#ifdef __cplusplus
}
#endif
