#ifndef CHECK_MEM_H
#define CHECK_MEM_H

#ifdef __cplusplus
extern "C"
{
#endif

#if CHECK_MAJOR_VERSION == 0 && CHECK_MINOR_VERSION < 11

#define _ck_assert_mem(X, Y, L, OP) do { \
  const char* _ck_x = (const char*)(void*)(X); \
  const char* _ck_y = (const char*)(void*)(Y); \
  size_t _ck_l = (L); \
  char _ck_x_str[129]; \
  char _ck_y_str[129]; \
  static char _ck_hexdigits[] = "0123456789abcdef"; \
  size_t _ck_i; \
  for (_ck_i = 0; _ck_i < ((_ck_l > 64) ? 64 : _ck_l); _ck_i++) { \
    _ck_x_str[_ck_i * 2  ]   = _ck_hexdigits[(_ck_x[_ck_i] >> 4) & 0xF]; \
    _ck_y_str[_ck_i * 2  ]   = _ck_hexdigits[(_ck_y[_ck_i] >> 4) & 0xF]; \
    _ck_x_str[_ck_i * 2 + 1] = _ck_hexdigits[_ck_x[_ck_i] & 0xF]; \
    _ck_y_str[_ck_i * 2 + 1] = _ck_hexdigits[_ck_y[_ck_i] & 0xF]; \
  } \
  _ck_x_str[_ck_i * 2] = 0; \
  _ck_y_str[_ck_i * 2] = 0; \
  ck_assert_msg(0 OP memcmp(_ck_y, _ck_x, _ck_l), \
    "Assertion '"#X#OP#Y"' failed: "#X"==\"%s\", "#Y"==\"%s\"", _ck_x_str, _ck_y_str); \
} while (0)
#define ck_assert_mem_eq(X, Y, L) _ck_assert_mem(X, Y, L, ==)
#define ck_assert_mem_ne(X, Y, L) _ck_assert_mem(X, Y, L, !=)

#endif

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
