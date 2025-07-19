#
# Matter_Base38.be - suppport for Base38 encoding which is used in QR Codes
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import matter

#@ solidify:Matter_Base38,weak

class Matter_Base38

  static def encode(raw)
    # encodes b38 (mutates `b`)
    def b38_enc(v, l)
      var ENCODE = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-."
      var i = 0
      var ret = ""
      while i < l
        ret += ENCODE[v % 38]
        v = v / 38
        i += 1
      end
      return ret
    end

    var idx = 0
    var sz = size(raw)
    var out = ""

    while idx < sz
      var val
      if   idx + 2 < sz
        # encode 3 bytes
        val = raw[idx] | (raw[idx+1] << 8) | (raw[idx+2] << 16)
        out += b38_enc(val, 5)
        idx += 3
      elif idx + 1 < sz
        # encode 2 bytes
        val = raw[idx] | (raw[idx+1] << 8)
        out += b38_enc(val, 4)
        idx += 2
      else
        # encode 1 byte
        val = raw[idx]
        out += b38_enc(val, 2)
        idx += 1
      end
    end
    return out
  end
end
matter.Base38 = Matter_Base38

#-

assert(matter.Base38.encode(bytes("DEADBEEF")) == "C.R.5B6")
assert(matter.Base38.encode(bytes("")) == "")
assert(matter.Base38.encode(bytes("00")) == "00")
assert(matter.Base38.encode(bytes("FFFFFFFF")) == "PLS18R6")

-#
