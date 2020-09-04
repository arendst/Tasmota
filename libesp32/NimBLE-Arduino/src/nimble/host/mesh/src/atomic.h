/* atomic operations */

/*
 * Copyright (c) 1997-2015, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef int atomic_t;
typedef atomic_t atomic_val_t;

/**
 * @defgroup atomic_apis Atomic Services APIs
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Atomic compare-and-set.
 *
 * This routine performs an atomic compare-and-set on @a target. If the current
 * value of @a target equals @a old_value, @a target is set to @a new_value.
 * If the current value of @a target does not equal @a old_value, @a target
 * is left unchanged.
 *
 * @param target Address of atomic variable.
 * @param old_value Original value to compare against.
 * @param new_value New value to store.
 * @return 1 if @a new_value is written, 0 otherwise.
 */
static inline int atomic_cas(atomic_t *target, atomic_val_t old_value,
        atomic_val_t new_value)
{
    return __atomic_compare_exchange_n(target, &old_value, new_value,
            0, __ATOMIC_SEQ_CST,
            __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic addition.
 *
 * This routine performs an atomic addition on @a target.
 *
 * @param target Address of atomic variable.
 * @param value Value to add.
 *
 * @return Previous value of @a target.
 */
static inline atomic_val_t atomic_add(atomic_t *target, atomic_val_t value)
{
    return __atomic_fetch_add(target, value, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic subtraction.
 *
 * This routine performs an atomic subtraction on @a target.
 *
 * @param target Address of atomic variable.
 * @param value Value to subtract.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_sub(atomic_t *target, atomic_val_t value)
{
    return __atomic_fetch_sub(target, value, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic increment.
 *
 * This routine performs an atomic increment by 1 on @a target.
 *
 * @param target Address of atomic variable.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_inc(atomic_t *target)
{
    return atomic_add(target, 1);
}

/**
 *
 * @brief Atomic decrement.
 *
 * This routine performs an atomic decrement by 1 on @a target.
 *
 * @param target Address of atomic variable.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_dec(atomic_t *target)
{
    return atomic_sub(target, 1);
}

/**
 *
 * @brief Atomic get.
 *
 * This routine performs an atomic read on @a target.
 *
 * @param target Address of atomic variable.
 *
 * @return Value of @a target.
 */

static inline atomic_val_t atomic_get(const atomic_t *target)
{
    return __atomic_load_n(target, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic get-and-set.
 *
 * This routine atomically sets @a target to @a value and returns
 * the previous value of @a target.
 *
 * @param target Address of atomic variable.
 * @param value Value to write to @a target.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_set(atomic_t *target, atomic_val_t value)
{
    /* This builtin, as described by Intel, is not a traditional
     * test-and-set operation, but rather an atomic exchange operation. It
     * writes value into *ptr, and returns the previous contents of *ptr.
     */
    return __atomic_exchange_n(target, value, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic clear.
 *
 * This routine atomically sets @a target to zero and returns its previous
 * value. (Hence, it is equivalent to atomic_set(target, 0).)
 *
 * @param target Address of atomic variable.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_clear(atomic_t *target)
{
    return atomic_set(target, 0);
}

/**
 *
 * @brief Atomic bitwise inclusive OR.
 *
 * This routine atomically sets @a target to the bitwise inclusive OR of
 * @a target and @a value.
 *
 * @param target Address of atomic variable.
 * @param value Value to OR.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_or(atomic_t *target, atomic_val_t value)
{
    return __atomic_fetch_or(target, value, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic bitwise exclusive OR (XOR).
 *
 * This routine atomically sets @a target to the bitwise exclusive OR (XOR) of
 * @a target and @a value.
 *
 * @param target Address of atomic variable.
 * @param value Value to XOR
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_xor(atomic_t *target, atomic_val_t value)
{
    return __atomic_fetch_xor(target, value, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic bitwise AND.
 *
 * This routine atomically sets @a target to the bitwise AND of @a target
 * and @a value.
 *
 * @param target Address of atomic variable.
 * @param value Value to AND.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_and(atomic_t *target, atomic_val_t value)
{
    return __atomic_fetch_and(target, value, __ATOMIC_SEQ_CST);
}

/**
 *
 * @brief Atomic bitwise NAND.
 *
 * This routine atomically sets @a target to the bitwise NAND of @a target
 * and @a value. (This operation is equivalent to target = ~(target & value).)
 *
 * @param target Address of atomic variable.
 * @param value Value to NAND.
 *
 * @return Previous value of @a target.
 */

static inline atomic_val_t atomic_nand(atomic_t *target, atomic_val_t value)
{
    return __atomic_fetch_nand(target, value, __ATOMIC_SEQ_CST);
}

    /**
     * @brief Initialize an atomic variable.
     *
     * This macro can be used to initialize an atomic variable. For example,
     * @code atomic_t my_var = ATOMIC_INIT(75); @endcode
     *
     * @param i Value to assign to atomic variable.
     */
#define ATOMIC_INIT(i) (i)

    /**
     * @cond INTERNAL_HIDDEN
     */

#define ATOMIC_BITS (sizeof(atomic_val_t) * 8)
#define ATOMIC_MASK(bit) (1 << ((bit) & (ATOMIC_BITS - 1)))
#define ATOMIC_ELEM(addr, bit) ((addr) + ((bit) / ATOMIC_BITS))

    /**
     * INTERNAL_HIDDEN @endcond
     */

    /**
     * @brief Define an array of atomic variables.
     *
     * This macro defines an array of atomic variables containing at least
     * @a num_bits bits.
     *
     * @note
     * If used from file scope, the bits of the array are initialized to zero;
     * if used from within a function, the bits are left uninitialized.
     *
     * @param name Name of array of atomic variables.
     * @param num_bits Number of bits needed.
     */
#define ATOMIC_DEFINE(name, num_bits) \
	atomic_t name[1 + ((num_bits) - 1) / ATOMIC_BITS]

    /**
     * @brief Atomically test a bit.
     *
     * This routine tests whether bit number @a bit of @a target is set or not.
     * The target may be a single atomic variable or an array of them.
     *
     * @param target Address of atomic variable or array.
     * @param bit Bit number (starting from 0).
     *
     * @return 1 if the bit was set, 0 if it wasn't.
     */
    static inline int
    atomic_test_bit(const atomic_t *target, int bit)
    {
        atomic_val_t val = atomic_get(ATOMIC_ELEM(target, bit));

        return (1 & (val >> (bit & (ATOMIC_BITS - 1))));
    }

    /**
     * @brief Atomically test and clear a bit.
     *
     * Atomically clear bit number @a bit of @a target and return its old value.
     * The target may be a single atomic variable or an array of them.
     *
     * @param target Address of atomic variable or array.
     * @param bit Bit number (starting from 0).
     *
     * @return 1 if the bit was set, 0 if it wasn't.
     */
    static inline int
    atomic_test_and_clear_bit(atomic_t *target, int bit)
    {
        atomic_val_t mask = ATOMIC_MASK(bit);
        atomic_val_t old;

        old = atomic_and(ATOMIC_ELEM(target, bit), ~mask);

        return (old & mask) != 0;
    }

    /**
     * @brief Atomically set a bit.
     *
     * Atomically set bit number @a bit of @a target and return its old value.
     * The target may be a single atomic variable or an array of them.
     *
     * @param target Address of atomic variable or array.
     * @param bit Bit number (starting from 0).
     *
     * @return 1 if the bit was set, 0 if it wasn't.
     */
    static inline int
    atomic_test_and_set_bit(atomic_t *target, int bit)
    {
        atomic_val_t mask = ATOMIC_MASK(bit);
        atomic_val_t old;

        old = atomic_or(ATOMIC_ELEM(target, bit), mask);

        return (old & mask) != 0;
    }

    /**
     * @brief Atomically clear a bit.
     *
     * Atomically clear bit number @a bit of @a target.
     * The target may be a single atomic variable or an array of them.
     *
     * @param target Address of atomic variable or array.
     * @param bit Bit number (starting from 0).
     *
     * @return N/A
     */
    static inline void
    atomic_clear_bit(atomic_t *target, int bit)
    {
        atomic_val_t mask = ATOMIC_MASK(bit);

        atomic_and(ATOMIC_ELEM(target, bit), ~mask);
    }

    /**
     * @brief Atomically set a bit.
     *
     * Atomically set bit number @a bit of @a target.
     * The target may be a single atomic variable or an array of them.
     *
     * @param target Address of atomic variable or array.
     * @param bit Bit number (starting from 0).
     *
     * @return N/A
     */
    static inline void
    atomic_set_bit(atomic_t *target, int bit)
    {
        atomic_val_t mask = ATOMIC_MASK(bit);

        atomic_or(ATOMIC_ELEM(target, bit), mask);
    }

/**
* @brief Atomically set a bit to a given value.
*
* Atomically set bit number @a bit of @a target to value @a val.
* The target may be a single atomic variable or an array of them.
*
* @param target Address of atomic variable or array.
* @param bit Bit number (starting from 0).
* @param val true for 1, false for 0.
*
* @return N/A
*/
static inline void atomic_set_bit_to(atomic_t *target, int bit, bool val)
{
    atomic_val_t mask = ATOMIC_MASK(bit);

    if (val) {
        (void)atomic_or(ATOMIC_ELEM(target, bit), mask);
    } else {
        (void)atomic_and(ATOMIC_ELEM(target, bit), ~mask);
    }
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ATOMIC_H__ */
