# crypto.PBKDF2_HMAC_SHA256
#
# This crypto algorigthm is requires for device pre-provisioning
#
# The code below is slow and should only be used occasionally
# to pre-compute Matter Verifier


# unit step of PBKDF2
# res is bytes(32) -- unchecked
#@ solidify:_f,weak
def _f(password, salt_i, c, res)
  # xor in place for bytes a with b
  def xor(a, b)
    assert(a.size() == b.size())
    var len = a.size()
    var i = 0
    while i < len
      a[i] = a[i] ^ b[i]
      i += 1
    end
  end

  import crypto
  # iteration 1
  var h = crypto.HMAC_SHA256(password)
  h.update(salt_i)
  var u = h.out()
  res.setbytes(0, u)

  # iterate
  var n = 2
  while n <= c
    h = crypto.HMAC_SHA256(password)
    h.update(u)
    u = h.out()
    xor(res, u)
    n += 1
    tasmota.yield()
  end
end

#@ solidify:PBKDF2_HMAC_SHA256,weak
def PBKDF2_HMAC_SHA256(self, p, s, c, dklen)
  if type(p) == 'string'  p = bytes().fromstring(p) end
  if type(s) == 'string'  s = bytes().fromstring(s) end
  var r = bytes()
  var i = 1
  while r.size() < dklen
    # compute salt_i as the ready to use salt+i
    var salt_i = s.copy()
    salt_i.add(i, -4)      # 4 bytes Big Endian
    var ri = bytes().resize(32)
    self._f(p, salt_i, c, ri)
    r += ri
    i += 1
  end
  return r[0..dklen-1]
end
  
#- TEST VECTORS

# https://github.com/brycx/Test-Vector-Generation/blob/master/PBKDF2/pbkdf2-hmac-sha2-test-vectors.md
import crypto
kd = crypto.PBKDF2_HMAC_SHA256()
print(kd.derive("password", "salt", 1, 20))
# bytes('120FB6CFFCF8B32C43E7225256C4F837A86548C9')
#        120fb6cffcf8b32c43e7225256c4f837a86548c9

print(kd.derive("password", "salt", 2, 20))
# bytes('AE4D0C95AF6B46D32D0ADFF928F06DD02A303F8E')
#        ae4d0c95af6b46d32d0adff928f06dd02a303f8e

print(kd.derive("password", "salt", 3, 20))
# bytes('AD35240AC683FEBFAF3CD49D845473FBBBAA2437')
#        ad35240ac683febfaf3cd49d845473fbbbaa2437

var now=tasmota.millis()
print(kd.derive("password", "salt", 4096, 20))
print("Done in",tasmota.millis()-now,"ms")
# bytes('C5E478D59288C841AA530DB6845C4C8D962893A0')
#        c5e478d59288c841aa530db6845c4c8d962893a0
# Done in 205 ms

-#
