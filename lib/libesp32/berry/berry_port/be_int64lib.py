"""
berry_port/be_int64lib.py — Port of default/be_int64_class.c

int64 class: 64-bit integer support for Berry's 32-bit architecture.
Uses Python's native arbitrary-precision integers masked to 64-bit signed range.

Original C code is included as comments.
"""

# /********************************************************************
# ** int64 - support 64 bits int on 32 bits architecture
# **
# ** Pure native Berry C API implementation - no dependency on berry_mapping.
# ** All functions use be_top/be_to* /be_push* stack operations directly.
# ********************************************************************/

# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

def _lazy_be_api():
    import berry_port.be_api as m; return m

def _lazy_be_object():
    import berry_port.be_object as m; return m

# ============================================================================
# Constants
# ============================================================================

INT64_MIN = -(2**63)
INT64_MAX = 2**63 - 1
INT32_MIN = -(2**31)
INT32_MAX = 2**31 - 1

# ============================================================================
# Helpers
# ============================================================================

def _mask64(val):
    """Mask a Python int to signed 64-bit range."""
    val = val & 0xFFFFFFFFFFFFFFFF
    if val >= 0x8000000000000000:
        val -= 0x10000000000000000
    return val

# /* Secure string to int64 conversion with validation */
# static int secure_str_to_int64(bvm *vm, const char* s, int64_t* result, void* allocated_ptr) {
#   if (!s || *s == '\0') { *result = 0; return 0; }
#   char* endptr;
#   errno = 0;
#   long long temp = strtoll(s, &endptr, 10);
#   if (errno == ERANGE) {
#     if (allocated_ptr) be_free(vm, allocated_ptr, sizeof(int64_t));
#     be_raise(vm, "value_error", "integer string out of range");
#     return -1;
#   }
#   while (*endptr == ' ' || *endptr == '\t' || *endptr == '\n' || *endptr == '\r') { endptr++; }
#   if (*endptr != '\0') {
#     if (allocated_ptr) be_free(vm, allocated_ptr, sizeof(int64_t));
#     be_raise(vm, "value_error", "invalid integer string format");
#     return -1;
#   }
#   *result = temp;
#   return 0;
# }
def _secure_str_to_int64(vm, s):
    """Secure string to int64 conversion with validation.

    Returns a Python int in [INT64_MIN, INT64_MAX].
    Raises Berry 'value_error' on invalid format or out-of-range.
    """
    be_api = _lazy_be_api()
    if s is None or s == "":
        return 0
    s = s.strip()
    try:
        result = int(s, 10)
    except ValueError:
        be_api.be_raise(vm, "value_error", "invalid integer string format")
        return 0  # unreachable after raise
    if result > INT64_MAX or result < INT64_MIN:
        be_api.be_raise(vm, "value_error", "integer string out of range")
        return 0  # unreachable after raise
    return result

# ============================================================================
# Stack access helpers (ported from C self_get_p / arg_get_p / push_int64_instance)
# ============================================================================

# static int64_t* self_get_p(bvm *vm) {
#   be_getmember(vm, 1, "_p");
#   int64_t *p = (int64_t*)be_tocomptr(vm, -1);
#   be_pop(vm, 1);
#   return p;
# }
def self_get_p(vm):
    """Get the int64 value from self (stack position 1).

    Returns a Python int in [INT64_MIN, INT64_MAX].
    """
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, "_p")
    p = be_api.be_tocomptr(vm, -1)
    be_api.be_pop(vm, 1)
    return p  # Python int or None

# /* Get the _p pointer from an int64 instance at stack position idx */
# static int64_t* arg_get_p(bvm *vm, int idx) {
#   if (be_isnil(vm, idx)) { return NULL; }
#   if (!be_isinstance(vm, idx)) { return NULL; }
#   be_getmember(vm, idx, "_p");
#   int64_t *p = (int64_t*)be_tocomptr(vm, -1);
#   be_pop(vm, 1);
#   return p;
# }
def arg_get_p(vm, idx):
    """Get the int64 value from an int64 instance at stack position idx.

    Returns Python int if idx holds a valid int64 instance, else None.
    """
    be_api = _lazy_be_api()
    if be_api.be_isnil(vm, idx):
        return None
    if not be_api.be_isinstance(vm, idx):
        return None
    be_api.be_getmember(vm, idx, "_p")
    p = be_api.be_tocomptr(vm, -1)
    be_api.be_pop(vm, 1)
    return p

# /*
#  * Push a new int64 instance onto the stack with the given value.
#  * Stack on return: new int64 instance at top.
#  */
# static void push_int64_instance(bvm *vm, int64_t val) {
#   int64_t *r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
#   *r64 = val;
#   be_getglobal(vm, "int64");     /* stack: class */
#   be_call(vm, 0);                /* stack: instance */
#   be_getmember(vm, -1, "init");  /* stack: instance, init */
#   be_pushvalue(vm, -2);          /* stack: instance, init, instance */
#   be_pushcomptr(vm, r64);        /* stack: instance, init, instance, ptr */
#   be_call(vm, 2);                /* stack: instance, ret */
#   be_pop(vm, 1);                 /* stack: instance */
# }
def push_int64_instance(vm, val):
    """Push a new int64 instance with the given value onto the stack.

    The value is masked to signed 64-bit range before storing.
    No malloc needed in Python — the int is stored directly as comptr.
    """
    be_api = _lazy_be_api()
    val = _mask64(val)
    be_api.be_getglobal(vm, "int64")      # stack: class
    be_api.be_call(vm, 0)                  # stack: instance
    be_api.be_getmember(vm, -1, "init")    # stack: instance, init
    be_api.be_pushvalue(vm, -2)            # stack: instance, init, instance
    be_api.be_pushcomptr(vm, val)          # stack: instance, init, instance, val
    be_api.be_call(vm, 2)                  # stack: instance, ret
    be_api.be_pop(vm, 1)                   # stack: instance

# ============================================================================
# Constructor / Destructor
# ============================================================================

# static int int64_init(bvm *vm) {
#   int argc = be_top(vm);
#   int64_t *i64 = NULL;
#   if (argc > 1 && be_iscomptr(vm, 2)) {
#     i64 = (int64_t*)be_tocomptr(vm, 2);
#   }
#   if (i64 == NULL) {
#     i64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
#     *i64 = 0;
#   }
#   bbool invalid_arg = bfalse;
#   if (argc > 1) {
#     if (be_iscomptr(vm, 2) || be_isnil(vm, 2)) {
#       /* keep value */
#     } else if (be_isint(vm, 2)) {
#       *i64 = be_toint(vm, 2);
#     } else if (be_isreal(vm, 2)) {
#       *i64 = (int64_t)be_toreal(vm, 2);
#     } else if (be_isstring(vm, 2)) {
#       const char* s = be_tostring(vm, 2);
#       if (secure_str_to_int64(vm, s, i64, i64) != 0) { return 0; }
#     } else if (be_isbool(vm, 2)) {
#       *i64 = be_tobool(vm, 2) ? 1 : 0;
#     } else if (be_isinstance(vm, 2)) {
#       be_getglobal(vm, "int64");
#       if (be_isderived(vm, 2)) {
#         be_getmember(vm, 2, "_p");
#         int64_t *v64 = (int64_t*)be_tocomptr(vm, -1);
#         if (v64 != NULL) { *i64 = *v64; }
#       } else { invalid_arg = btrue; }
#     } else { invalid_arg = btrue; }
#   }
#   if (invalid_arg) {
#     be_free(vm, i64, sizeof(int64_t));
#     be_raise(vm, "TypeError", "unsupported argument type");
#   }
#   be_pushcomptr(vm, i64);
#   be_setmember(vm, 1, "_p");
#   be_return_nil(vm);
# }
def int64_init(vm):
    """init(self [, value]) — Initialize int64 from int, real, string,
    comptr, int64 instance, bool, or nil."""
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    i64 = 0  # default value

    # If a comptr is passed directly, use it (pre-set value from push_int64_instance)
    if argc > 1 and be_api.be_iscomptr(vm, 2):
        i64 = be_api.be_tocomptr(vm, 2)
    elif argc > 1:
        if be_api.be_isnil(vm, 2):
            pass  # keep default 0
        elif be_api.be_isint(vm, 2):
            i64 = _mask64(be_api.be_toint(vm, 2))
        elif be_api.be_isreal(vm, 2):
            i64 = _mask64(int(be_api.be_toreal(vm, 2)))
        elif be_api.be_isstring(vm, 2):
            i64 = _secure_str_to_int64(vm, be_api.be_tostring(vm, 2))
        elif be_api.be_isbool(vm, 2):
            i64 = 1 if be_api.be_tobool(vm, 2) else 0
        elif be_api.be_isinstance(vm, 2):
            be_api.be_getglobal(vm, "int64")
            if be_api.be_isderived(vm, 2):
                v64 = arg_get_p(vm, 2)
                if v64 is not None:
                    i64 = v64
            else:
                be_api.be_raise(vm, "TypeError", "unsupported argument type")
        else:
            be_api.be_raise(vm, "TypeError", "unsupported argument type")

    be_api.be_pushcomptr(vm, i64)
    be_api.be_setmember(vm, 1, "_p")
    return be_api.be_returnnilvalue(vm)

# static int int64_deinit(bvm *vm) {
#   /* In C: frees the allocated int64_t */
#   /* In Python: no-op (GC handles cleanup) */
#   be_return_nil(vm);
# }
def int64_deinit(vm):
    """deinit(self) — No-op in Python (GC handles cleanup)."""
    be_api = _lazy_be_api()
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# Conversion methods
# ============================================================================

# static int int64_tostring(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   static char s[24];
#   int64_toa(*i64, (uint8_t*)s);
#   be_pushstring(vm, s);
#   be_return(vm);
# }
def int64_tostring(vm):
    """tostring(self) -> string — Decimal string representation."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    be_api.be_pushstring(vm, str(val))
    return be_api.be_returnvalue(vm)

# /* static fromstring(s) -> int64 */
# static int int64_fromstring(bvm *vm) {
#   const char *s = be_tostring(vm, 1);
#   int64_t val = 0;
#   if (secure_str_to_int64(vm, s, &val, NULL) != 0) { return 0; }
#   push_int64_instance(vm, val);
#   be_return(vm);
# }
def int64_fromstring(vm):
    """static fromstring(s) -> int64 — Parse decimal string to int64.

    STATIC method: reads arg at stack[1] (no self).
    """
    be_api = _lazy_be_api()
    s = be_api.be_tostring(vm, 1)
    val = _secure_str_to_int64(vm, s)
    push_int64_instance(vm, val)
    return be_api.be_returnvalue(vm)

# /* isint(self) -> bool : is the value within int32 range? */
# static int int64_isint(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   be_pushbool(vm, *i64 >= INT32_MIN && *i64 <= INT32_MAX);
#   be_return(vm);
# }
def int64_isint(vm):
    """isint(self) -> bool — True if value fits in int32 range."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    be_api.be_pushbool(vm, INT32_MIN <= val <= INT32_MAX)
    return be_api.be_returnvalue(vm)

# /* toint(self) -> int : truncate to 32-bit int */
# static int int64_toint(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   be_pushint(vm, (int32_t)*i64);
#   be_return(vm);
# }
def int64_toint(vm):
    """toint(self) -> int — Truncate to 32-bit signed int."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    # Equivalent of C (int32_t) cast: mask to 32 bits, then sign-extend
    val32 = ((val & 0xFFFFFFFF) ^ 0x80000000) - 0x80000000
    be_api.be_pushint(vm, val32)
    return be_api.be_returnvalue(vm)

# /* tobool(self) -> bool */
# static int int64_tobool(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   be_pushbool(vm, *i64 != 0);
#   be_return(vm);
# }
def int64_tobool(vm):
    """tobool(self) -> bool — True if value != 0."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    be_api.be_pushbool(vm, val != 0)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Static factory methods
# ============================================================================

# /* static fromu32(low [, high]) -> int64 */
# static int int64_fromu32(bvm *vm) {
#   int argc = be_top(vm);
#   uint32_t low = (uint32_t)be_toint(vm, 1);
#   uint32_t high = (argc > 1) ? (uint32_t)be_toint(vm, 2) : 0;
#   int64_t val = (int64_t)low | (((int64_t)high) << 32);
#   push_int64_instance(vm, val);
#   be_return(vm);
# }
def int64_fromu32(vm):
    """static fromu32(low [, high]) -> int64 — Construct from unsigned 32-bit halves.

    STATIC method: reads args at stack[1], stack[2] (no self).
    low and high are treated as unsigned 32-bit values.
    """
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    low = be_api.be_toint(vm, 1) & 0xFFFFFFFF
    high = (be_api.be_toint(vm, 2) & 0xFFFFFFFF) if argc > 1 else 0
    val = low | (high << 32)
    push_int64_instance(vm, val)
    return be_api.be_returnvalue(vm)

# /* static fromfloat(f) -> int64 */
# static int int64_fromfloat(bvm *vm) {
#   breal f = be_toreal(vm, 1);
#   push_int64_instance(vm, (int64_t)f);
#   be_return(vm);
# }
def int64_fromfloat(vm):
    """static fromfloat(f) -> int64 — Construct from float.

    STATIC method: reads arg at stack[1] (no self).
    """
    be_api = _lazy_be_api()
    f = be_api.be_toreal(vm, 1)
    push_int64_instance(vm, _mask64(int(f)))
    return be_api.be_returnvalue(vm)

# /* static toint64(v) -> int64 | nil */
# static int int64_toint64(bvm *vm) {
#   if (be_top(vm) == 0 || be_isnil(vm, 1)) {
#     be_return_nil(vm);
#   }
#   be_getglobal(vm, "int64");
#   be_pushvalue(vm, 1);
#   be_call(vm, 1);
#   be_pop(vm, 1);
#   be_return(vm);
# }
def int64_toint64(vm):
    """static toint64(v) -> int64 | nil — Convert value to int64, nil passthrough.

    STATIC method: reads arg at stack[1] (no self).
    If no args or nil at stack[1], returns nil.
    Otherwise creates a new int64 instance via int64(v).
    """
    be_api = _lazy_be_api()
    if be_api.be_top(vm) == 0 or be_api.be_isnil(vm, 1):
        return be_api.be_returnnilvalue(vm)
    # Push int64 class, push value, call int64(v)
    be_api.be_getglobal(vm, "int64")
    be_api.be_pushvalue(vm, 1)
    be_api.be_call(vm, 1)
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Arithmetic operators
# ============================================================================

# static int int64_add(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t b = j64 ? *j64 : 0;
#   if ((b > 0 && *i64 > INT64_MAX - b) || (b < 0 && *i64 < INT64_MIN - b)) {
#     be_raise(vm, "overflow_error", "integer overflow in addition");
#   }
#   push_int64_instance(vm, *i64 + b);
#   be_return(vm);
# }
def int64_add(vm):
    """+(self, other_int64) -> int64 — Add two int64 values."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    # Overflow check (matching C implementation)
    if (b > 0 and a > INT64_MAX - b) or (b < 0 and a < INT64_MIN - b):
        be_api.be_raise(vm, "overflow_error", "integer overflow in addition")
    push_int64_instance(vm, a + b)
    return be_api.be_returnvalue(vm)

# /* add(self, int32) -> int64 : add a Berry int directly */
# static int int64_add32(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t b = (int64_t)be_toint(vm, 2);
#   if ((b > 0 && *i64 > INT64_MAX - b) || (b < 0 && *i64 < INT64_MIN - b)) {
#     be_raise(vm, "overflow_error", "integer overflow in addition");
#   }
#   push_int64_instance(vm, *i64 + b);
#   be_return(vm);
# }
def int64_add32(vm):
    """add(self, int32) -> int64 — Add a Berry int32 directly."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b = be_api.be_toint(vm, 2)
    # Overflow check (matching C implementation)
    if (b > 0 and a > INT64_MAX - b) or (b < 0 and a < INT64_MIN - b):
        be_api.be_raise(vm, "overflow_error", "integer overflow in addition")
    push_int64_instance(vm, a + b)
    return be_api.be_returnvalue(vm)

# /* -(self, other_int64) -> int64 */
# static int int64_sub(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t b = j64 ? *j64 : 0;
#   if ((*i64 > 0 && b < *i64 - INT64_MAX) || (*i64 < 0 && b > *i64 - INT64_MIN)) {
#     be_raise(vm, "overflow_error", "integer overflow in subtraction");
#   }
#   push_int64_instance(vm, *i64 - b);
#   be_return(vm);
# }
def int64_sub(vm):
    """-(self, other_int64) -> int64 — Subtract two int64 values."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    # Overflow check for subtraction (matching C implementation)
    if (a > 0 and b < a - INT64_MAX) or (a < 0 and b > a - INT64_MIN):
        be_api.be_raise(vm, "overflow_error", "integer overflow in subtraction")
    push_int64_instance(vm, a - b)
    return be_api.be_returnvalue(vm)

# /* -*(self) -> int64 : unary negation */
# static int int64_neg(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   if (*i64 == INT64_MIN) {
#     be_raise(vm, "overflow_error", "cannot negate INT64_MIN");
#   }
#   push_int64_instance(vm, -*i64);
#   be_return(vm);
# }
def int64_neg(vm):
    """-*(self) -> int64 — Unary negation."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    if a == INT64_MIN:
        be_api.be_raise(vm, "overflow_error", "cannot negate INT64_MIN")
    push_int64_instance(vm, -a)
    return be_api.be_returnvalue(vm)

# static int int64_mul(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t a = *i64;
#   int64_t b = j64 ? *j64 : 0;
#   if (a == 0 || b == 0) {
#     push_int64_instance(vm, 0);
#     be_return(vm);
#   }
#   if ((a == INT64_MIN && b == -1) || (a == -1 && b == INT64_MIN)) {
#     be_raise(vm, "overflow_error", "integer overflow in multiplication");
#   }
#   if ((a > 0 && b > 0 && a > INT64_MAX / b) ||
#       (a < 0 && b < 0 && a < INT64_MAX / b) ||
#       (a > 0 && b < 0 && b < INT64_MIN / a) ||
#       (a < 0 && b > 0 && a < INT64_MIN / b)) {
#     be_raise(vm, "overflow_error", "integer overflow in multiplication");
#   }
#   push_int64_instance(vm, a * b);
#   be_return(vm);
# }
def int64_mul(vm):
    """*(self, other_int64) -> int64 — Multiply two int64 values."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    # Short-circuit: if either is 0, result is 0
    if a == 0 or b == 0:
        push_int64_instance(vm, 0)
        return be_api.be_returnvalue(vm)
    # Overflow check: INT64_MIN * -1 (or vice versa)
    if (a == INT64_MIN and b == -1) or (a == -1 and b == INT64_MIN):
        be_api.be_raise(vm, "overflow_error", "integer overflow in multiplication")
    # General overflow check: Python has arbitrary precision, so we can
    # simply compute the product and check if it fits in int64 range.
    result = a * b
    if result > INT64_MAX or result < INT64_MIN:
        be_api.be_raise(vm, "overflow_error", "integer overflow in multiplication")
    push_int64_instance(vm, result)
    return be_api.be_returnvalue(vm)

# /* /(self, other_int64) -> int64 */
# static int int64_div(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   if (j64 == NULL || *j64 == 0) {
#     be_raise(vm, "divzero_error", "division by zero");
#   }
#   if (*i64 == INT64_MIN && *j64 == -1) {
#     be_raise(vm, "overflow_error", "division overflow: INT64_MIN / -1");
#   }
#   push_int64_instance(vm, *i64 / *j64);
#   be_return(vm);
# }
def int64_div(vm):
    """/(self, other_int64) -> int64 — Divide two int64 values (truncation toward zero)."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    # Division by zero check (null or zero)
    if b_ptr is None or b_ptr == 0:
        be_api.be_raise(vm, "divzero_error", "division by zero")
    b = b_ptr
    # Overflow check: INT64_MIN / -1
    if a == INT64_MIN and b == -1:
        be_api.be_raise(vm, "overflow_error", "division overflow: INT64_MIN / -1")
    # C-style truncation toward zero (Python // floors toward negative infinity)
    # Use: sign * (abs(a) // abs(b))
    if (a < 0) != (b < 0):
        result = -(abs(a) // abs(b))
    else:
        result = abs(a) // abs(b)
    push_int64_instance(vm, result)
    return be_api.be_returnvalue(vm)

# /* %(self, other_int64) -> int64 */
# static int int64_mod(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   if (j64 == NULL || *j64 == 0) {
#     be_raise(vm, "divzero_error", "modulo by zero");
#   }
#   push_int64_instance(vm, *i64 % *j64);
#   be_return(vm);
# }
def int64_mod(vm):
    """%(self, other_int64) -> int64 — Modulo (C-style truncation toward zero)."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    # Modulo by zero check
    if b_ptr is None or b_ptr == 0:
        be_api.be_raise(vm, "divzero_error", "modulo by zero")
    b = b_ptr
    # C-style modulo: a - trunc(a/b) * b
    # trunc(a/b) = sign * (abs(a) // abs(b))
    if (a < 0) != (b < 0):
        trunc_div = -(abs(a) // abs(b))
    else:
        trunc_div = abs(a) // abs(b)
    result = a - trunc_div * b
    push_int64_instance(vm, result)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Shift operators
# ============================================================================

# /* <<(self, int32) -> int64 */
# static int int64_shiftleft(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int32_t j32 = be_toint(vm, 2) & 63;
#   push_int64_instance(vm, *i64 << j32);
#   be_return(vm);
# }
def int64_shiftleft(vm):
    """<<(self, int32) -> int64 — Left shift, amount masked to 0-63."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    shift = be_api.be_toint(vm, 2) & 63
    push_int64_instance(vm, a << shift)
    return be_api.be_returnvalue(vm)

# /* >>(self, int32) -> int64 */
# static int int64_shiftright(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int32_t j32 = be_toint(vm, 2) & 63;
#   push_int64_instance(vm, *i64 >> j32);
#   be_return(vm);
# }
def int64_shiftright(vm):
    """>>(self, int32) -> int64 — Arithmetic right shift, amount masked to 0-63."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    shift = be_api.be_toint(vm, 2) & 63
    push_int64_instance(vm, a >> shift)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Comparison operators
# ============================================================================

import struct

# static int int64_compare(bvm *vm, int64_t *i64, int idx, bbool *valid) {
#   *valid = btrue;
#   int64_t *j64 = arg_get_p(vm, idx);
#   if (j64) {
#     if (*i64 < *j64) return -1;
#     if (*i64 > *j64) return 1;
#     return 0;
#   }
#   if (be_isint(vm, idx) || be_isnil(vm, idx)) {
#     int64_t b = be_isint(vm, idx) ? (int64_t)be_toint(vm, idx) : 0;
#     if (*i64 < b) return -1;
#     if (*i64 > b) return 1;
#     return 0;
#   }
#   if (be_isbytes(vm, idx)) {
#     size_t len = 0;
#     const void *ptr = be_tobytes(vm, idx, &len);
#     if (len == sizeof(int64_t) && memcmp(i64, ptr, sizeof(int64_t)) == 0) {
#       return 0;
#     }
#     *valid = btrue;
#     return 1;
#   }
#   *valid = bfalse;
#   return 1;
# }
def _int64_compare(vm, val, idx):
    """Compare int64 value against the argument at stack position idx.

    Returns (cmp, valid) where cmp is -1/0/1 and valid is bool.
    """
    be_api = _lazy_be_api()

    # Check if other is an int64 instance
    j64 = arg_get_p(vm, idx)
    if j64 is not None:
        if val < j64:
            return (-1, True)
        if val > j64:
            return (1, True)
        return (0, True)

    # Check if other is a plain int or nil (nil treated as 0)
    if be_api.be_isint(vm, idx) or be_api.be_isnil(vm, idx):
        b = be_api.be_toint(vm, idx) if be_api.be_isint(vm, idx) else 0
        if val < b:
            return (-1, True)
        if val > b:
            return (1, True)
        return (0, True)

    # Check if other is bytes — compare raw 8-byte LE representation
    from berry_port.be_byteslib import be_isbytes, be_tobytes
    if be_isbytes(vm, idx):
        bufptr, length = be_tobytes(vm, idx)
        if length == 8 and bufptr is not None:
            self_bytes = struct.pack('<q', val)
            other_bytes = bytes(bufptr[:8])
            if self_bytes == other_bytes:
                return (0, True)
        return (1, True)

    # Unknown type — not valid for comparison
    return (1, False)

# static int int64_equals(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   bbool valid;
#   int cmp = int64_compare(vm, i64, 2, &valid);
#   be_pushbool(vm, valid && cmp == 0);
#   be_return(vm);
# }
def int64_equals(vm):
    """==(self, other) -> bool — Equality comparison."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    cmp, valid = _int64_compare(vm, val, 2)
    be_api.be_pushbool(vm, valid and cmp == 0)
    return be_api.be_returnvalue(vm)

# static int int64_nequals(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   bbool valid;
#   int cmp = int64_compare(vm, i64, 2, &valid);
#   be_pushbool(vm, !valid || cmp != 0);
#   be_return(vm);
# }
def int64_nequals(vm):
    """!=(self, other) -> bool — Inequality comparison."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    cmp, valid = _int64_compare(vm, val, 2)
    be_api.be_pushbool(vm, not valid or cmp != 0)
    return be_api.be_returnvalue(vm)

# static int int64_gt(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t b = j64 ? *j64 : 0;
#   be_pushbool(vm, *i64 > b);
#   be_return(vm);
# }
def int64_gt(vm):
    """>(self, other) -> bool — Greater than."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    be_api.be_pushbool(vm, a > b)
    return be_api.be_returnvalue(vm)

# static int int64_gte(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t b = j64 ? *j64 : 0;
#   be_pushbool(vm, *i64 >= b);
#   be_return(vm);
# }
def int64_gte(vm):
    """>=(self, other) -> bool — Greater than or equal."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    be_api.be_pushbool(vm, a >= b)
    return be_api.be_returnvalue(vm)

# static int int64_lt(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t b = j64 ? *j64 : 0;
#   be_pushbool(vm, *i64 < b);
#   be_return(vm);
# }
def int64_lt(vm):
    """<(self, other) -> bool — Less than."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    be_api.be_pushbool(vm, a < b)
    return be_api.be_returnvalue(vm)

# static int int64_lte(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   int64_t *j64 = arg_get_p(vm, 2);
#   int64_t b = j64 ? *j64 : 0;
#   be_pushbool(vm, *i64 <= b);
#   be_return(vm);
# }
def int64_lte(vm):
    """<=(self, other) -> bool — Less than or equal."""
    be_api = _lazy_be_api()
    a = self_get_p(vm)
    b_ptr = arg_get_p(vm, 2)
    b = b_ptr if b_ptr is not None else 0
    be_api.be_pushbool(vm, a <= b)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Bytes conversion
# ============================================================================

# /* tobytes(self) -> bytes */
# static int int64_tobytes(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   be_pushbytes(vm, i64, sizeof(int64_t));
#   be_return(vm);
# }
def int64_tobytes(vm):
    """tobytes(self) -> bytes — 8-byte little-endian representation."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    data = struct.pack('<q', val)
    from berry_port.be_byteslib import be_pushbytes
    be_pushbytes(vm, data, len(data))
    return be_api.be_returnvalue(vm)

# /* static frombytes(b [, idx]) -> int64 */
# static int int64_frombytes(bvm *vm) {
#   int argc = be_top(vm);
#   size_t len = 0;
#   const uint8_t *ptr = (const uint8_t*)be_tobytes(vm, 1, &len);
#   int32_t idx = (argc > 1) ? be_toint(vm, 2) : 0;
#
#   if (idx < 0) { idx = (int32_t)len + idx; }
#   if (idx < 0) { idx = 0; }
#   if (idx >= (int32_t)len) { idx = (int32_t)len; }
#
#   uint32_t usable_len = len - idx;
#   if (usable_len > sizeof(int64_t)) { usable_len = sizeof(int64_t); }
#   int64_t val = 0;
#   memmove(&val, ptr + idx, usable_len);
#   push_int64_instance(vm, val);
#   be_return(vm);
# }
def int64_frombytes(vm):
    """static frombytes(b [, idx]) -> int64 — Read int64 from bytes at offset.

    STATIC method: reads args at stack[1] (bytes), stack[2] (optional offset).
    Supports negative indexing. Zero-pads if fewer than 8 bytes available.
    """
    be_api = _lazy_be_api()
    from berry_port.be_byteslib import be_tobytes
    argc = be_api.be_top(vm)
    bufptr, length = be_tobytes(vm, 1)
    idx = be_api.be_toint(vm, 2) if argc > 1 else 0

    # Negative indexing
    if idx < 0:
        idx = length + idx
    if idx < 0:
        idx = 0
    if idx >= length:
        idx = length

    usable_len = length - idx
    if usable_len > 8:
        usable_len = 8

    # Extract usable bytes and zero-pad to 8 bytes
    if bufptr is not None and usable_len > 0:
        raw = bytes(bufptr[idx:idx + usable_len])
    else:
        raw = b''
    padded = raw + b'\x00' * (8 - len(raw))

    val = struct.unpack('<q', padded)[0]
    push_int64_instance(vm, val)
    return be_api.be_returnvalue(vm)

# ============================================================================
# 32-bit accessors
# ============================================================================

# /* low32(self) -> int */
# static int int64_low32(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   be_pushint(vm, (int32_t)*i64);
#   be_return(vm);
# }
def int64_low32(vm):
    """low32(self) -> int — Lower 32 bits as signed int32."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    # Equivalent of C (int32_t) cast: mask to 32 bits, then sign-extend
    val32 = ((val & 0xFFFFFFFF) ^ 0x80000000) - 0x80000000
    be_api.be_pushint(vm, val32)
    return be_api.be_returnvalue(vm)

# /* high32(self) -> int */
# static int int64_high32(bvm *vm) {
#   int64_t *i64 = self_get_p(vm);
#   be_pushint(vm, (int32_t)(*i64 >> 32));
#   be_return(vm);
# }
def int64_high32(vm):
    """high32(self) -> int — Upper 32 bits as signed int32 (arithmetic shift right by 32)."""
    be_api = _lazy_be_api()
    val = self_get_p(vm)
    # Arithmetic right shift by 32, then cast to signed int32
    shifted = val >> 32
    val32 = ((shifted & 0xFFFFFFFF) ^ 0x80000000) - 0x80000000
    be_api.be_pushint(vm, val32)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Registration
# ============================================================================

def be_load_int64lib(vm):
    be_api = _lazy_be_api()
    members = [
        ("_p", None),
        ("init", int64_init),
        ("deinit", int64_deinit),
        ("fromu32", int64_fromu32, True),       # static
        ("fromfloat", int64_fromfloat, True),   # static
        ("toint64", int64_toint64, True),       # static
        ("tostring", int64_tostring),
        ("fromstring", int64_fromstring, True), # static
        ("isint", int64_isint),
        ("toint", int64_toint),
        ("tobool", int64_tobool),
        ("add", int64_add32),
        ("+", int64_add),
        ("-", int64_sub),
        ("*", int64_mul),
        ("/", int64_div),
        ("%", int64_mod),
        ("-*", int64_neg),
        ("==", int64_equals),
        ("!=", int64_nequals),
        (">", int64_gt),
        (">=", int64_gte),
        ("<", int64_lt),
        ("<=", int64_lte),
        ("<<", int64_shiftleft),
        (">>", int64_shiftright),
        ("tobytes", int64_tobytes),
        ("frombytes", int64_frombytes, True),   # static
        ("low32", int64_low32),
        ("high32", int64_high32),
    ]
    be_api.be_regclass(vm, "int64", members)
