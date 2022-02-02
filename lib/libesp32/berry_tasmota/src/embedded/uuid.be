
uuid = module("uuid")
uuid.uuid4 = def ()
  import string
  import math
  return string.format("%08x-%04x-%04x-%04x-%04x%08x",
                       math.rand(),
                       math.rand() & 0xFFFF,
                       math.rand() & 0x0FFF | 0x4000,
                       math.rand() & 0x3FFF | 0x8000,
                       math.rand() & 0xFFFF,
                       math.rand() )
end

return uuid
