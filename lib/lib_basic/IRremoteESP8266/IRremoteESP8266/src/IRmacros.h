#ifndef IRMACROS_H_
#define IRMACROS_H_
/****************************************************************
 * Copyright 2022 IRremoteESP8266 project and others
 */
/// @file IRmacros.h

/**
 * VA_OPT_SUPPORTED macro to check if __VA_OPT__ is supported
 * Source: https://stackoverflow.com/a/48045656
 */
/// @cond TEST
#define PP_THIRD_ARG(a, b, c, ...) c
#define VA_OPT_SUPPORTED_I(...) \
            PP_THIRD_ARG(__VA_OPT__(, false), true, false, false)
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)
/// @endcond
/**
 * VA_OPT_SUPPORTED end
 */

/**
 * COND() Set of macros to facilitate single-line conditional compilation
 * argument checking.
 * Found here: https://www.reddit.com/r/C_Programming/comments/ud3xrv/
 * conditional_preprocessor_macro_anyone/
 * 
 * Usage:
 * COND(<define_to_test>[||/&&<more_define>...], <true_result>, <false_result>)
 * 
 * NB: If __VA_OPT__ macro not supported, the <true_result> will be expanded!
 */
/// @cond TEST
#if !VA_OPT_SUPPORTED
// #pragma message("Compiler without __VA_OPT__ support")
#define COND(cond, a, b) a
#else  // !VA_OPT_SUPPORTED
#define NOTHING
#define EXPAND(...) __VA_ARGS__
#define STUFF_P(a, ...) __VA_OPT__(a)
#define STUFF(...) STUFF_P(__VA_ARGS__)
#define VA_TEST_P(a, ...) __VA_OPT__(NO)##THING
#define VA_TEST(...) VA_TEST_P(__VA_ARGS__)
#define NEGATE(a) VA_TEST(a, a)
#define COND_P(cond, a, b) STUFF(a, cond)STUFF(b, NEGATE(cond))
#define COND(cond, a, b) EXPAND(COND_P(cond, a, b))
#endif  // !VA_OPT_SUPPORTED
/// @endcond
/**
 * end of COND() set of macros
 */

#endif  // IRMACROS_H_
