"""re1.5 opcode constants, error codes, and ByteProg data class."""

# Consumer opcodes
CONSUMERS = 0x01
Char = 0x01
Any = 0x02
Class = 0x03
ClassNot = 0x04
NamedClass = 0x05

# Assert opcodes
ASSERTS = 0x50
Bol = 0x50
Eol = 0x51

# Jump opcodes
JUMPS = 0x60
Jmp = 0x60
Split = 0x61
RSplit = 0x62

# Special opcodes
Save = 0x7e
Match = 0x7f

NON_ANCHORED_PREFIX = 5
MAXSUB = 20

# Error codes
RE1_5_SUCCESS = 0
RE1_5_SYNTAX_ERROR = -2
RE1_5_UNSUPPORTED_ESCAPE = -3
RE1_5_UNSUPPORTED_SYNTAX = -4


class ByteProg:
    """Compiled regex bytecode program, mirrors the C ByteProg struct."""
    __slots__ = ('bytelen', 'len', 'sub', 'insts')

    def __init__(self, bytelen=0, length=0, sub=0, insts=None):
        self.bytelen = bytelen
        self.len = length
        self.sub = sub
        self.insts = insts if insts is not None else bytearray()


def inst_is_consumer(opcode):
    """Return True if the opcode is a consumer (advances the string pointer)."""
    return opcode < ASSERTS
