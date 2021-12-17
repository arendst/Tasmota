#include "be_constobj.h"

static be_define_const_map_slots(m_libmath_map) {
    { be_const_key(asin, 7), be_const_func(m_asin) },
    { be_const_key(isnan, -1), be_const_func(m_isnan) },
    { be_const_key(sinh, 22), be_const_func(m_sinh) },
    { be_const_key(cos, -1), be_const_func(m_cos) },
    { be_const_key(rand, -1), be_const_func(m_rand) },
    { be_const_key(deg, -1), be_const_func(m_deg) },
    { be_const_key(log10, 16), be_const_func(m_log10) },
    { be_const_key(acos, -1), be_const_func(m_acos) },
    { be_const_key(cosh, -1), be_const_func(m_cosh) },
    { be_const_key(tanh, 21), be_const_func(m_tanh) },
    { be_const_key(rad, 11), be_const_func(m_rad) },
    { be_const_key(abs, -1), be_const_func(m_abs) },
    { be_const_key(atan2, -1), be_const_func(m_atan2) },
    { be_const_key(tan, 19), be_const_func(m_tan) },
    { be_const_key(ceil, 15), be_const_func(m_ceil) },
    { be_const_key(nan, -1), be_const_real(NAN) },
    { be_const_key(imin, -1), be_const_int(M_IMIN) },
    { be_const_key(pow, -1), be_const_func(m_pow) },
    { be_const_key(atan, -1), be_const_func(m_atan) },
    { be_const_key(imax, 25), be_const_int(M_IMAX) },
    { be_const_key(exp, 17), be_const_func(m_exp) },
    { be_const_key(log, 1), be_const_func(m_log) },
    { be_const_key(sqrt, -1), be_const_func(m_sqrt) },
    { be_const_key(srand, -1), be_const_func(m_srand) },
    { be_const_key(floor, -1), be_const_func(m_floor) },
    { be_const_key(sin, -1), be_const_func(m_sin) },
    { be_const_key(pi, 8), be_const_real(M_PI) },
};

static be_define_const_map(
    m_libmath_map,
    27
);

static be_define_const_module(
    m_libmath,
    "math"
);

BE_EXPORT_VARIABLE be_define_const_native_module(math);
