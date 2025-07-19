import re

def hashcode(s):
    """Compute hash for a string, in uint32_t"""
    hash = 2166136261
    for c in s.encode('utf8'):
        hash = ((hash ^ c) * 16777619) & 0xFFFFFFFF
    return hash

def escape_operator_v1(s):
    tab = {
        # "void": "",
        "..": "opt_connect",
        "+": "opt_add",        "-": "opt_sub",
        "*": "opt_mul",        "/": "opt_div",
        "%": "opt_mod",        "&": "opt_and",
        "^": "opt_xor",        "|": "opt_or",
        "<": "opt_lt",         ">": "opt_gt",
        "<=": "opt_le",        ">=": "opt_ge",
        "==": "opt_eq",        "!=": "opt_neq",
        "<<": "opt_shl",       ">>": "opt_shr",
        "-*": "opt_neg",       "~": "opt_flip",
        "()": "opt_call",
    }
    if s in tab: return tab[s]
    s2 = ""
    for c in s:
        if c == '.':     s2 += 'dot_'
        else:            s2 += c
    return s2

def escape_operator(s):
    s = re.sub('_X', '_X_', s)                  # replace any esape sequence '_X' with '_XX'
    s = re.sub('[^a-zA-Z0-9_]', lambda m: "_X{0:02X}".format(ord(m.group())), s)
    return s

def unescape_operator(s):
    s = re.sub('_X[0-9A-F][0-9A-F]', lambda m: chr(int(m.group()[2:], 16)), s)
    s = re.sub('_X_', '_X', s)
    return s
