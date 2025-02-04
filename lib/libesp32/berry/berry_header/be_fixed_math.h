#include "be_constobj.h"

static be_define_const_map_slots(m_libmath_map) {
    { be_const_key(deg, 20), be_const_func(m_deg) },
    { be_const_key(imin, 11), be_const_int(M_IMIN) },
    { be_const_key(rad, -1), be_const_func(m_rad) },
    { be_const_key(cosh, -1), be_const_func(m_cosh) },
    { be_const_key(abs, -1), be_const_func(m_abs) },
    { be_const_key(exp, 2), be_const_func(m_exp) },
    { be_const_key(asin, -1), be_const_func(m_asin) },
    { be_const_key(log10, -1), be_const_func(m_log10) },
    { be_const_key(inf, 19), be_const_real(INFINITY) },
    { be_const_key(srand, -1), be_const_func(m_srand) },
    { be_const_key(isnan, -1), be_const_func(m_isnan) },
    { be_const_key(isinf, -1), be_const_func(m_isinf) },
    { be_const_key(atan2, -1), be_const_func(m_atan2) },
    { be_const_key(sin, -1), be_const_func(m_sin) },
    { be_const_key(imax, -1), be_const_int(M_IMAX) },
    { be_const_key(sqrt, -1), be_const_func(m_sqrt) },
    { be_const_key(tanh, 7), be_const_func(m_tanh) },
    { be_const_key(log, -1), be_const_func(m_log) },
    { be_const_key(pi, 14), be_const_real(M_PI) },
    { be_const_key(ceil, 5), be_const_func(m_ceil) },
    { be_const_key(acos, 30), be_const_func(m_acos) },
    { be_const_key(pow, -1), be_const_func(m_pow) },
    { be_const_key(rand, -1), be_const_func(m_rand) },
    { be_const_key(min, 12), be_const_func(m_min) },
    { be_const_key(tan, 1), be_const_func(m_tan) },
    { be_const_key(max, -1), be_const_func(m_max) },
    { be_const_key(nan, -1), be_const_real(NAN) },
    { be_const_key(round, 4), be_const_func(m_round) },
    { be_const_key(cos, 3), be_const_func(m_cos) },
    { be_const_key(floor, -1), be_const_func(m_floor) },
    { be_const_key(sinh, -1), be_const_func(m_sinh) },
    { be_const_key(atan, 0), be_const_func(m_atan) },
};

static be_define_const_map(
    m_libmath_map,
    32
);

static be_define_const_module(
    m_libmath,
    "math"
);

BE_EXPORT_VARIABLE be_define_const_native_module(math);
