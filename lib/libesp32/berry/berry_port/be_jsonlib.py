"""
berry_port/be_jsonlib.py — Port of src/be_jsonlib.c

JSON module: load (parse JSON string to Berry objects) and dump
(serialize Berry objects to JSON string).

Original C code is included as comments.
"""

# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

import math

def _lazy_be_api():
    import berry_port.be_api as m; return m

def _lazy_be_lexer():
    import berry_port.be_lexer as m; return m

def _lazy_be_mem():
    import berry_port.be_mem as m; return m

from berry_port.berry_conf import BE_STACK_FREE_MIN
from berry_port.be_strlib import be_str2real

# ============================================================================
# Constants
# ============================================================================

# #define is_space(c)     ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')
# #define is_digit(c)     ((c) >= '0' && (c) <= '9')

# #define MAX_INDENT      24
# #define INDENT_WIDTH    2
# #define INDENT_CHAR     ' '
MAX_INDENT = 24
INDENT_WIDTH = 2
INDENT_CHAR = ' '

# /* Security: Maximum JSON string length to prevent memory exhaustion attacks */
# #define MAX_JSON_STRING_LEN  (1024 * 1024)  /* 1MB limit */
MAX_JSON_STRING_LEN = 1024 * 1024  # 1MB limit

# ============================================================================
# Helper functions
# ============================================================================

def is_space(c):
    return c in (' ', '\t', '\r', '\n')

def is_digit(c):
    return '0' <= c <= '9'

# static const char* skip_space(const char *s)
# {
#     int c;
#     while (((c = *s) != '\0') && ((c == ' ')
#         || (c == '\t') || (c == '\r') || (c == '\n'))) {
#         ++s;
#     }
#     return s;
# }
def skip_space(s, pos):
    """Skip whitespace characters. Returns new position."""
    while pos < len(s) and is_space(s[pos]):
        pos += 1
    return pos

# static const char* match_char(const char *json, int ch)
# {
#     json = skip_space(json);
#     if (*json == ch) {
#         return skip_space(json + 1);
#     }
#     return NULL;
# }
def match_char(s, pos, ch):
    """Skip space, then match character ch. Returns new position or -1."""
    pos = skip_space(s, pos)
    if pos < len(s) and s[pos] == ch:
        return skip_space(s, pos + 1)
    return -1

# static int is_object(bvm *vm, const char *class, int idx)
# {
#     if (be_isinstance(vm, idx)) {
#         be_pushvalue(vm, idx);
#         while (1) {
#             be_getsuper(vm, -1);
#             if (be_isnil(vm, -1)) {
#                 be_pop(vm, 1);
#                 break;
#             }
#             be_remove(vm, -2);
#         }
#         const char *name = be_classname(vm, -1);
#         bbool ret = !strcmp(name, class);
#         be_pop(vm, 1);
#         return ret;
#     }
#     return  0;
# }
def is_object(vm, classname, idx):
    """Check if value at idx is an instance whose root class matches classname."""
    be_api = _lazy_be_api()
    if be_api.be_isinstance(vm, idx):
        be_api.be_pushvalue(vm, idx)
        while True:
            be_api.be_getsuper(vm, -1)
            if be_api.be_isnil(vm, -1):
                be_api.be_pop(vm, 1)
                break
            be_api.be_remove(vm, -2)
        name = be_api.be_classname(vm, -1)
        ret = (name == classname)
        be_api.be_pop(vm, 1)
        return ret
    return False

# /* Calculate the actual buffer size needed for JSON string parsing
#  * accounting for Unicode expansion and security limits */
# static size_t json_strlen_safe(const char *json, size_t *actual_len)
def json_strlen_safe(s, pos):
    """Calculate the buffer size needed for JSON string parsing,
    accounting for Unicode expansion and security limits.
    Returns (byte_count, char_count) on success, or (-1, 0) on error."""
    i = pos + 1  # skip opening '"'
    char_count = 0
    byte_count = 0

    while i < len(s) and s[i] != '"':
        char_count += 1
        if char_count > MAX_JSON_STRING_LEN:
            return (-1, 0)  # String too long

        ch = s[i]
        i += 1
        if ch == '\\':
            if i >= len(s):
                return (-1, 0)  # Malformed string
            esc = s[i]
            i += 1

            if esc in ('"', '\\', '/', 'b', 'f', 'n', 'r', 't'):
                byte_count += 1
            elif esc == 'u':
                # Unicode can expand to 1-3 UTF-8 bytes
                # We conservatively assume 3 bytes for safety
                byte_count += 3
                # Verify we have 4 hex digits following
                for j in range(4):
                    if i + j >= len(s) or s[i + j] not in '0123456789abcdefABCDEF':
                        return (-1, 0)  # Invalid unicode sequence
                i += 4  # Skip the 4 hex digits
            else:
                return (-1, 0)  # Invalid escape sequence
        elif ord(ch) <= 0x1f:
            return (-1, 0)  # Unescaped control character
        else:
            byte_count += 1

        # Check for potential overflow
        if byte_count > MAX_JSON_STRING_LEN:
            return (-1, 0)

    if i >= len(s) or s[i] != '"':
        return (-1, 0)  # Unterminated string

    return (byte_count, char_count)

# static void json2berry(bvm *vm, const char *class)
# {
#     be_getbuiltin(vm, class);
#     be_pushvalue(vm, -2);
#     be_call(vm, 1);
#     be_moveto(vm, -2, -3);
#     be_pop(vm, 2);
# }
def json2berry(vm, classname):
    """Convert raw map/list on top of stack to Berry map/list instance."""
    be_api = _lazy_be_api()
    be_api.be_getbuiltin(vm, classname)
    be_api.be_pushvalue(vm, -2)
    be_api.be_call(vm, 1)
    be_api.be_moveto(vm, -2, -3)
    be_api.be_pop(vm, 2)

# ============================================================================
# JSON Parser
# ============================================================================

# static const char* parser_true(bvm *vm, const char *json)
# {
#     if (!strncmp(json, "true", 4)) {
#         be_pushbool(vm, btrue);
#         return json + 4;
#     }
#     return NULL;
# }
def parser_true(vm, s, pos):
    """Parse 'true' literal. Returns new position or -1."""
    be_api = _lazy_be_api()
    if s[pos:pos+4] == 'true':
        be_api.be_pushbool(vm, True)
        return pos + 4
    return -1

# static const char* parser_false(bvm *vm, const char *json)
# {
#     if (!strncmp(json, "false", 5)) {
#         be_pushbool(vm, bfalse);
#         return json + 5;
#     }
#     return NULL;
# }
def parser_false(vm, s, pos):
    """Parse 'false' literal. Returns new position or -1."""
    be_api = _lazy_be_api()
    if s[pos:pos+5] == 'false':
        be_api.be_pushbool(vm, False)
        return pos + 5
    return -1

# static const char* parser_null(bvm *vm, const char *json)
# {
#     if (!strncmp(json, "null", 4)) {
#         be_pushnil(vm);
#         return json + 4;
#     }
#     return NULL;
# }
def parser_null(vm, s, pos):
    """Parse 'null' literal. Returns new position or -1."""
    be_api = _lazy_be_api()
    if s[pos:pos+4] == 'null':
        be_api.be_pushnil(vm)
        return pos + 4
    return -1


# static const char* parser_string(bvm *vm, const char *json)
# — Rewritten to use json_strlen_safe for security
def parser_string(vm, s, pos):
    """Parse a JSON string starting at pos. Returns new position or -1."""
    be_api = _lazy_be_api()
    be_lexer = _lazy_be_lexer()
    if pos >= len(s) or s[pos] != '"':
        return -1

    byte_len, char_len = json_strlen_safe(s, pos)

    if byte_len < 0:
        return -1  # Invalid or too long string

    if byte_len == 0:
        # Empty string
        be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
        be_api.be_pushstring(vm, "")
        return pos + 2  # Skip opening and closing quotes

    buf = []
    src = pos + 1  # Skip opening quote

    while src < len(s) and s[src] != '"':
        ch = s[src]
        src += 1
        if ch == '\\':
            esc = s[src]
            src += 1
            if esc == '"':    buf.append('"')
            elif esc == '\\': buf.append('\\')
            elif esc == '/':  buf.append('/')
            elif esc == 'b':  buf.append('\b')
            elif esc == 'f':  buf.append('\f')
            elif esc == 'n':  buf.append('\n')
            elif esc == 'r':  buf.append('\r')
            elif esc == 't':  buf.append('\t')
            elif esc == 'u':
                hex_bytes = s[src:src+4].encode('latin-1') if src + 4 <= len(s) else b''
                utf8_bytes = be_lexer.be_load_unicode(hex_bytes, 0)
                if utf8_bytes is None:
                    return -1
                for b in utf8_bytes:
                    buf.append(chr(b))
                src += 4
            else:
                return -1  # Invalid escape
        elif ord(ch) <= 0x1f:
            return -1  # Unescaped control character
        else:
            buf.append(ch)

    if src >= len(s) or s[src] != '"':
        return -1  # Unterminated string

    # Success - create Berry string
    be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
    result = ''.join(buf)
    be_api.be_pushnstring(vm, result, len(result))
    return src + 1  # Skip closing quote

# static const char* parser_field(bvm *vm, const char *json)
# {
#     be_stack_require(vm, 2 + BE_STACK_FREE_MIN);
#     if (json && *json == '"') {
#         json = parser_string(vm, json);
#         if (json) {
#             json = match_char(json, ':');
#             if (json) {
#                 json = parser_value(vm, json);
#                 if (json) {
#                     be_data_insert(vm, -3);
#                     be_pop(vm, 2); /* pop key and value */
#                     return json;
#                 }
#             }
#             be_pop(vm, 1); /* pop key */
#         }
#     }
#     return NULL;
# }
def parser_field(vm, s, pos):
    """Parse a JSON object field (key:value). Returns new position or -1."""
    be_api = _lazy_be_api()
    be_api.be_stack_require(vm, 2 + BE_STACK_FREE_MIN)
    if pos >= 0 and pos < len(s) and s[pos] == '"':
        pos = parser_string(vm, s, pos)
        if pos >= 0:
            pos = match_char(s, pos, ':')
            if pos >= 0:
                pos = parser_value(vm, s, pos)
                if pos >= 0:
                    be_api.be_data_insert(vm, -3)
                    be_api.be_pop(vm, 2)  # pop key and value
                    return pos
            be_api.be_pop(vm, 1)  # pop key
    return -1

# static const char* parser_object(bvm *vm, const char *json)
# {
#     json = match_char(json, '{');
#     be_newmap(vm);
#     if (*json != '}') {
#         const char *s;
#         json = parser_field(vm, json);
#         if (json == NULL) {
#             be_pop(vm, 1); /* pop map */
#             return NULL;
#         }
#         while ((s = match_char(json, ',')) != NULL) {
#             json = parser_field(vm, s);
#             if (json == NULL) {
#                 be_pop(vm, 1); /* pop map */
#                 return NULL;
#             }
#         }
#     }
#     if ((json = match_char(json, '}')) == NULL) {
#         be_pop(vm, 1); /* pop map */
#         return NULL;
#     }
#     json2berry(vm, "map");
#     return json;
# }
def parser_object(vm, s, pos):
    """Parse a JSON object. Returns new position or -1."""
    be_api = _lazy_be_api()
    pos = match_char(s, pos, '{')
    if pos < 0:
        return -1
    be_api.be_newmap(vm)
    if pos < len(s) and s[pos] != '}':
        pos = parser_field(vm, s, pos)
        if pos < 0:
            be_api.be_pop(vm, 1)  # pop map
            return -1
        while True:
            comma_pos = match_char(s, pos, ',')
            if comma_pos < 0:
                break
            pos = parser_field(vm, s, comma_pos)
            if pos < 0:
                be_api.be_pop(vm, 1)  # pop map
                return -1
    pos = match_char(s, pos, '}')
    if pos < 0:
        be_api.be_pop(vm, 1)  # pop map
        return -1
    json2berry(vm, "map")
    return pos

# static const char* parser_array(bvm *vm, const char *json)
# {
#     json = match_char(json, '[');
#     be_newlist(vm);
#     if (*json != ']') {
#         const char *s;
#         json = parser_value(vm, json);
#         if (json == NULL) {
#             be_pop(vm, 1); /* pop list */
#             return NULL;
#         }
#         be_data_push(vm, -2);
#         be_pop(vm, 1); /* pop value */
#         while ((s = match_char(json, ',')) != NULL) {
#             json = parser_value(vm, s);
#             if (json == NULL) {
#                 be_pop(vm, 1); /* pop list */
#                 return NULL;
#             }
#             be_data_push(vm, -2);
#             be_pop(vm, 1); /* pop value */
#         }
#     }
#     if ((json = match_char(json, ']')) == NULL) {
#         be_pop(vm, 1); /* pop list */
#         return NULL;
#     }
#     json2berry(vm, "list");
#     return json;
# }
def parser_array(vm, s, pos):
    """Parse a JSON array. Returns new position or -1."""
    be_api = _lazy_be_api()
    pos = match_char(s, pos, '[')
    if pos < 0:
        return -1
    be_api.be_newlist(vm)
    if pos < len(s) and s[pos] != ']':
        pos = parser_value(vm, s, pos)
        if pos < 0:
            be_api.be_pop(vm, 1)  # pop list
            return -1
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, 1)  # pop value
        while True:
            comma_pos = match_char(s, pos, ',')
            if comma_pos < 0:
                break
            pos = parser_value(vm, s, comma_pos)
            if pos < 0:
                be_api.be_pop(vm, 1)  # pop list
                return -1
            be_api.be_data_push(vm, -2)
            be_api.be_pop(vm, 1)  # pop value
    pos = match_char(s, pos, ']')
    if pos < 0:
        be_api.be_pop(vm, 1)  # pop list
        return -1
    json2berry(vm, "list")
    return pos


# enum {
#     JSON_NUMBER_INVALID = 0,
#     JSON_NUMBER_INTEGER = 1,
#     JSON_NUMBER_REAL = 2
# };
JSON_NUMBER_INVALID = 0
JSON_NUMBER_INTEGER = 1
JSON_NUMBER_REAL = 2

# int check_json_number(const char *json)
def check_json_number(s, pos):
    """Validate a JSON number starting at pos.
    Returns (number_type, end_pos) where number_type is one of
    JSON_NUMBER_INVALID, JSON_NUMBER_INTEGER, JSON_NUMBER_REAL.
    end_pos is the position after the last character of the number."""
    if pos >= len(s):
        return (JSON_NUMBER_INVALID, pos)

    p = pos
    has_fraction = False
    has_exponent = False

    # Skip leading whitespace
    while p < len(s) and is_space(s[p]):
        p += 1

    if p >= len(s):
        return (JSON_NUMBER_INVALID, p)

    # Handle optional minus sign
    if s[p] == '-':
        p += 1
        if p >= len(s):
            return (JSON_NUMBER_INVALID, p)

    # Integer part
    if p < len(s) and s[p] == '0':
        # If starts with 0, next char must not be a digit
        p += 1
        if p < len(s) and is_digit(s[p]):
            return (JSON_NUMBER_INVALID, p)  # Leading zeros not allowed
    elif p < len(s) and is_digit(s[p]):
        # First digit must be 1-9, then any digits
        p += 1
        while p < len(s) and is_digit(s[p]):
            p += 1
    else:
        return (JSON_NUMBER_INVALID, p)  # Must start with digit

    # Optional fractional part
    if p < len(s) and s[p] == '.':
        has_fraction = True
        p += 1
        if p >= len(s) or not is_digit(s[p]):
            return (JSON_NUMBER_INVALID, p)  # Must have at least one digit after decimal point
        while p < len(s) and is_digit(s[p]):
            p += 1

    # Optional exponent part
    if p < len(s) and s[p] in ('e', 'E'):
        has_exponent = True
        p += 1
        # Optional sign in exponent
        if p < len(s) and s[p] in ('+', '-'):
            p += 1
        if p >= len(s) or not is_digit(s[p]):
            return (JSON_NUMBER_INVALID, p)  # Must have at least one digit in exponent
        while p < len(s) and is_digit(s[p]):
            p += 1

    # Number ends here - check that next char is not a continuation
    if p < len(s) and not is_space(s[p]) and s[p] not in (',', ']', '}', ':'):
        return (JSON_NUMBER_INVALID, p)

    # Determine return value based on what was found
    if has_exponent or has_fraction:
        return (JSON_NUMBER_REAL, p)
    else:
        return (JSON_NUMBER_INTEGER, p)

# static const char* parser_number(bvm *vm, const char *json)
# — Rewritten to use check_json_number + Python int()/float()
def parser_number(vm, s, pos):
    """Parse a JSON number (integer or real). Returns new position or -1."""
    be_api = _lazy_be_api()
    number_type, end_pos = check_json_number(s, pos)

    if number_type == JSON_NUMBER_INTEGER:
        be_api.be_pushint(vm, int(s[pos:end_pos]))
        return end_pos
    elif number_type == JSON_NUMBER_REAL:
        be_api.be_pushreal(vm, be_str2real(s[pos:end_pos]))
        return end_pos
    else:
        return -1

# /* parser json value */
# static const char* parser_value(bvm *vm, const char *json)
# {
#     json = skip_space(json);
#     be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#     switch (*json) {
#     case '{': return parser_object(vm, json);
#     case '[': return parser_array(vm, json);
#     case '"': return parser_string(vm, json);
#     case 't': return parser_true(vm, json);
#     case 'f': return parser_false(vm, json);
#     case 'n': return parser_null(vm, json);
#     default:
#         if (*json == '-' || is_digit(*json)) {
#            return parser_number(vm, json);
#         }
#     }
#     return NULL;
# }
def parser_value(vm, s, pos):
    """Parse any JSON value. Returns new position or -1."""
    be_api = _lazy_be_api()
    pos = skip_space(s, pos)
    if pos >= len(s):
        return -1
    be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
    ch = s[pos]
    if ch == '{':
        return parser_object(vm, s, pos)
    elif ch == '[':
        return parser_array(vm, s, pos)
    elif ch == '"':
        return parser_string(vm, s, pos)
    elif ch == 't':
        return parser_true(vm, s, pos)
    elif ch == 'f':
        return parser_false(vm, s, pos)
    elif ch == 'n':
        return parser_null(vm, s, pos)
    elif ch == '-' or is_digit(ch):
        return parser_number(vm, s, pos)
    return -1

# ============================================================================
# m_json_load — entry point for json.load()
# ============================================================================

# static int m_json_load(bvm *vm)
# {
#     if (be_isstring(vm, 1)) {
#         const char *json = be_tostring(vm, 1);
#         json = parser_value(vm, json);
#         if (json != NULL && *json == '\0') {
#             be_return(vm);
#         }
#     }
#     be_return_nil(vm);
# }
def m_json_load(vm):
    be_api = _lazy_be_api()
    if be_api.be_isstring(vm, 1):
        json_str = be_api.be_tostring(vm, 1)
        pos = parser_value(vm, json_str, 0)
        if pos >= 0:
            # check that remaining is only whitespace
            end = skip_space(json_str, pos)
            if end >= len(json_str):
                return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# JSON Serializer (dump)
# ============================================================================

# static void make_indent(bvm *vm, int stridx, int indent)
# {
#     if (indent) {
#         be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#         char buf[MAX_INDENT * INDENT_WIDTH + 1];
#         indent = (indent < MAX_INDENT ? indent : MAX_INDENT) * INDENT_WIDTH;
#         memset(buf, INDENT_CHAR, indent);
#         buf[indent] = '\0';
#         stridx = be_absindex(vm, stridx);
#         be_pushstring(vm, buf);
#         be_strconcat(vm, stridx);
#         be_pop(vm, 1);
#     }
# }
def make_indent(vm, stridx, indent):
    """Append indentation to the string at stridx."""
    be_api = _lazy_be_api()
    if indent:
        be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
        clamped = min(indent, MAX_INDENT) * INDENT_WIDTH
        buf = INDENT_CHAR * clamped
        stridx = be_api.be_absindex(vm, stridx)
        be_api.be_pushstring(vm, buf)
        be_api.be_strconcat(vm, stridx)
        be_api.be_pop(vm, 1)

# void string_dump(bvm *vm, int index)
# {
#     be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#     be_tostring(vm, index); /* convert value to string */
#     be_toescape(vm, index, 'u');
#     be_pushvalue(vm, index);
# }
def string_dump(vm, index):
    """Convert value at index to escaped JSON string and push to top."""
    be_api = _lazy_be_api()
    be_strlib = None
    try:
        import berry_port.be_strlib as be_strlib
    except ImportError:
        pass
    be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
    be_api.be_tostring(vm, index)
    if be_strlib:
        be_strlib.be_toescape(vm, index, 'u')
    be_api.be_pushvalue(vm, index)

# static void object_dump(bvm *vm, int *indent, int idx, int fmt)
# {
#     be_stack_require(vm, 3 + BE_STACK_FREE_MIN);
#     be_getmember(vm, idx, ".p");
#     be_pushstring(vm, fmt ? "{\n" : "{");
#     be_pushiter(vm, -2); /* map iterator use 1 register */
#     *indent += fmt;
#     while (be_iter_hasnext(vm, -3)) {
#         be_stack_require(vm, 3 + BE_STACK_FREE_MIN);
#         make_indent(vm, -2, fmt ? *indent : 0);
#         be_iter_next(vm, -3);
#         /* key.tostring() */
#         string_dump(vm, -2);
#         be_strconcat(vm, -5);
#         be_pop(vm, 1);
#         be_pushstring(vm, fmt ? ": " : ":"); /* add ': ' */
#         be_strconcat(vm, -5);
#         be_pop(vm, 1);
#         /* value.tostring() */
#         value_dump(vm, indent, -1, fmt);
#         be_strconcat(vm, -5);
#         be_pop(vm, 3);
#         if (be_iter_hasnext(vm, -3)) {
#             be_pushstring(vm, fmt ? ",\n" : ",");
#             be_strconcat(vm, -3);
#             be_pop(vm, 1);
#         } else if (fmt) {
#             be_pushstring(vm, "\n");
#             be_strconcat(vm, -3);
#             be_pop(vm, 1);
#         }
#     }
#     *indent -= fmt;
#     be_pop(vm, 1); /* pop iterator */
#     make_indent(vm, -1,  fmt ? *indent : 0);
#     be_pushstring(vm, "}");
#     be_strconcat(vm, -2);
#     be_moveto(vm, -2, -3);
#     be_pop(vm, 2);
# }
def object_dump(vm, indent, idx, fmt):
    """Serialize a map instance to JSON object string. Returns updated indent."""
    be_api = _lazy_be_api()
    be_api.be_stack_require(vm, 3 + BE_STACK_FREE_MIN)
    be_api.be_getmember(vm, idx, ".p")
    be_api.be_pushstring(vm, "{\n" if fmt else "{")
    be_api.be_pushiter(vm, -2)  # map iterator
    indent[0] += fmt
    while be_api.be_iter_hasnext(vm, -3):
        be_api.be_stack_require(vm, 3 + BE_STACK_FREE_MIN)
        make_indent(vm, -2, indent[0] if fmt else 0)
        be_api.be_iter_next(vm, -3)
        # key.tostring()
        string_dump(vm, -2)
        be_api.be_strconcat(vm, -5)
        be_api.be_pop(vm, 1)
        be_api.be_pushstring(vm, ": " if fmt else ":")
        be_api.be_strconcat(vm, -5)
        be_api.be_pop(vm, 1)
        # value.tostring()
        value_dump(vm, indent, -1, fmt)
        be_api.be_strconcat(vm, -5)
        be_api.be_pop(vm, 3)
        if be_api.be_iter_hasnext(vm, -3):
            be_api.be_pushstring(vm, ",\n" if fmt else ",")
            be_api.be_strconcat(vm, -3)
            be_api.be_pop(vm, 1)
        elif fmt:
            be_api.be_pushstring(vm, "\n")
            be_api.be_strconcat(vm, -3)
            be_api.be_pop(vm, 1)
    indent[0] -= fmt
    be_api.be_pop(vm, 1)  # pop iterator
    make_indent(vm, -1, indent[0] if fmt else 0)
    be_api.be_pushstring(vm, "}")
    be_api.be_strconcat(vm, -2)
    be_api.be_moveto(vm, -2, -3)
    be_api.be_pop(vm, 2)

# static void array_dump(bvm *vm, int *indent, int idx, int fmt)
# {
#     be_stack_require(vm, 3 + BE_STACK_FREE_MIN);
#     be_getmember(vm, idx, ".p");
#     be_pushstring(vm, fmt ? "[\n" : "[");
#     be_pushiter(vm, -2);
#     *indent += fmt;
#     while (be_iter_hasnext(vm, -3)) {
#         make_indent(vm, -2,  fmt ? *indent : 0);
#         be_iter_next(vm, -3);
#         value_dump(vm, indent, -1, fmt);
#         be_strconcat(vm, -4);
#         be_pop(vm, 2);
#         be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#         if (be_iter_hasnext(vm, -3)) {
#             be_pushstring(vm, fmt ? ",\n" : ",");
#             be_strconcat(vm, -3);
#             be_pop(vm, 1);
#         } else if (fmt) {
#             be_pushstring(vm, "\n");
#             be_strconcat(vm, -3);
#             be_pop(vm, 1);
#         }
#     }
#     *indent -= fmt;
#     be_pop(vm, 1); /* pop iterator */
#     make_indent(vm, -1,  fmt ? *indent : 0);
#     be_pushstring(vm, "]");
#     be_strconcat(vm, -2);
#     be_moveto(vm, -2, -3);
#     be_pop(vm, 2);
# }
def array_dump(vm, indent, idx, fmt):
    """Serialize a list instance to JSON array string. Returns updated indent."""
    be_api = _lazy_be_api()
    be_api.be_stack_require(vm, 3 + BE_STACK_FREE_MIN)
    be_api.be_getmember(vm, idx, ".p")
    be_api.be_pushstring(vm, "[\n" if fmt else "[")
    be_api.be_pushiter(vm, -2)
    indent[0] += fmt
    while be_api.be_iter_hasnext(vm, -3):
        make_indent(vm, -2, indent[0] if fmt else 0)
        be_api.be_iter_next(vm, -3)
        value_dump(vm, indent, -1, fmt)
        be_api.be_strconcat(vm, -4)
        be_api.be_pop(vm, 2)
        be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
        if be_api.be_iter_hasnext(vm, -3):
            be_api.be_pushstring(vm, ",\n" if fmt else ",")
            be_api.be_strconcat(vm, -3)
            be_api.be_pop(vm, 1)
        elif fmt:
            be_api.be_pushstring(vm, "\n")
            be_api.be_strconcat(vm, -3)
            be_api.be_pop(vm, 1)
    indent[0] -= fmt
    be_api.be_pop(vm, 1)  # pop iterator
    make_indent(vm, -1, indent[0] if fmt else 0)
    be_api.be_pushstring(vm, "]")
    be_api.be_strconcat(vm, -2)
    be_api.be_moveto(vm, -2, -3)
    be_api.be_pop(vm, 2)

# static void value_dump(bvm *vm, int *indent, int idx, int fmt)
# {
#     if (is_object(vm, "map", idx)) {
#         object_dump(vm, indent, idx, fmt);
#     } else if (is_object(vm, "list", idx)) {
#         array_dump(vm, indent, idx, fmt);
#     } else if (be_isnil(vm, idx)) {
#         be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#         be_pushstring(vm, "null");
#     } else if (be_isreal(vm, idx)) {
#         be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#         breal v = be_toreal(vm, idx);
#         if (isnan(v) || isinf(v)) {
#             be_pushstring(vm, "null");
#         } else {
#             be_tostring(vm, idx);
#             be_pushvalue(vm, idx);
#         };
#     } else if (be_isnumber(vm, idx) || be_isbool(vm, idx)) {
#         be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#         be_tostring(vm, idx);
#         be_pushvalue(vm, idx);
#     } else {
#         string_dump(vm, idx);
#     }
# }
def value_dump(vm, indent, idx, fmt):
    """Serialize any Berry value to JSON string and push result to top."""
    be_api = _lazy_be_api()
    if is_object(vm, "map", idx):
        object_dump(vm, indent, idx, fmt)
    elif is_object(vm, "list", idx):
        array_dump(vm, indent, idx, fmt)
    elif be_api.be_isnil(vm, idx):
        be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
        be_api.be_pushstring(vm, "null")
    elif be_api.be_isreal(vm, idx):
        be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
        v = be_api.be_toreal(vm, idx)
        if math.isnan(v) or math.isinf(v):
            be_api.be_pushstring(vm, "null")
        else:
            be_api.be_tostring(vm, idx)
            be_api.be_pushvalue(vm, idx)
    elif be_api.be_isnumber(vm, idx) or be_api.be_isbool(vm, idx):
        be_api.be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
        be_api.be_tostring(vm, idx)
        be_api.be_pushvalue(vm, idx)
    else:
        string_dump(vm, idx)

# ============================================================================
# m_json_dump — entry point for json.dump()
# ============================================================================

# static int m_json_dump(bvm *vm)
# {
#     int indent = 0, argc = be_top(vm);
#     int fmt = 0;
#     if (argc > 1) {
#         fmt = !strcmp(be_tostring(vm, 2), "format");
#     }
#     value_dump(vm, &indent, 1, fmt);
#     be_return(vm);
# }
def m_json_dump(vm):
    be_api = _lazy_be_api()
    indent = [0]  # mutable list to allow pass-by-reference
    argc = be_api.be_top(vm)
    fmt = 0
    if argc > 1:
        fmt = 1 if be_api.be_tostring(vm, 2) == "format" else 0
    value_dump(vm, indent, 1, fmt)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Module registration table
# ============================================================================

# /* @const_object_info_begin
# module json (scope: global, depend: BE_USE_JSON_MODULE) {
#     load, func(m_json_load)
#     dump, func(m_json_dump)
# }
# @const_object_info_end */

def be_json_module_table():
    """Return the native module attribute table for the json module."""
    return [
        ("load", m_json_load),
        ("dump", m_json_dump),
    ]
