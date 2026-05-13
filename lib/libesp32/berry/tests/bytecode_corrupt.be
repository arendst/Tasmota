# Tests that the bytecode loader fails cleanly on corrupt .bec input,
# raising `bytecode_error` rather than crashing or silently producing
# zero-filled bytecode.

import os
import global

var tmpdir = '/tmp'
var src_path = tmpdir + '/be_bc_src.be'
var bec_path = tmpdir + '/be_bc.bec'

# 1. Save a real .bec file and confirm it still round-trips through the loader.
var f = open(src_path, 'w')
f.write('def hello(x) return x + 1 end\n')
f.close()

var fn = compile(src_path, 'file')
var fout = open(bec_path, 'w')
fout.savecode(fn)
fout.close()

# Load it back: the new bytecode_error path must NOT fire on valid input.
var loaded = compile(bec_path, 'file')
assert(type(loaded) == 'function')
loaded()  # executes the top-level, defining `hello` as a global
assert(global.hello(41) == 42)

# Read the saved .bec into a buffer for crafting variants.
var fin = open(bec_path, 'r')
var good = fin.readbytes()
fin.close()
assert(size(good) >= 16)

def expect_bytecode_error(path, label)
    var caught = false
    try
        compile(path, 'file')
    except 'bytecode_error' as e, m
        caught = true
    end
    assert(caught, 'expected bytecode_error for ' + label)
end

def write_bytes(path, b)
    var w = open(path, 'w')
    w.write(b)
    w.close()
end

# 2. Truncate after the magic header (8 bytes). Any subsequent count read
#    must hit EOF and raise bytecode_error.
var truncated = good[0..7]
write_bytes(bec_path, truncated)
expect_bytecode_error(bec_path, 'header-only truncation')

# 3. Truncate inside the body (keep some payload, drop the tail).
write_bytes(bec_path, good[0..(size(good) / 2)])
expect_bytecode_error(bec_path, 'mid-file truncation')

# 4. Header is valid, but the global-count field is set to 0xFFFFFFFF.
#    Layout: 8-byte header, then 4-byte builtin-count, then 4-byte gcnt.
var corrupt_gcnt = good[0..7] + good[8..11] + bytes('FFFFFFFF')
write_bytes(bec_path, corrupt_gcnt)
expect_bytecode_error(bec_path, 'oversized global count')

# Cleanup.
os.remove(src_path)
os.remove(bec_path)
