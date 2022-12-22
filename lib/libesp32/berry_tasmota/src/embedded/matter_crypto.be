# Matter crypto
#
# Implement all the crypto required by the Matter protocol

#@ solidify:Matter_Crypto,weak
class Matter_Crypto
  # inputs from device (all are bytes or string - if string, they are converted to raw bytes)
  var passcode
  # provided by device
  var salt
  var iterations
  # computed values from above
  var w0                                  # w0
  var w1
  var L
  # value for prover
  var pA, pB, Z, V
  var x, y
  # values from the Matter specification
  static var CRYPTO_GROUP_SIZE_BYTES = 32
  static var CRYPTO_W_SIZE_BYTES = 40         # CRYPTO_GROUP_SIZE_BYTES + 8
  static var spake_M_hex = "04886e2f97ace46e55ba9dd7242579f2993b64e16ef3dcab95afd497333d8fa12f5ff355163e43ce224e0b0e65ff02ac8e5c7be09419c785e0ca547d55a12e2d20"
  static var spake_N_hex = "04d8bbd6c639c62937b04d997f38c3770719c629d7014d49a24b4f98baa1292b4907d60aa6bfade45008a636337f5168c64d9bd36034808cd564490b1e656edbe7"
  var M, N    # due to the nature of solification, we can't solidify the bytes versions nor store as static variable

  # init does not do much
  def init()
    self.M = bytes().fromhex(self.spake_M_hex)
    self.N = bytes().fromhex(self.spake_N_hex)
  end

  # set device information
  def set_device_info(passcode, salt, iterations)
    passcode = int(passcode)
    iterations = int(iterations)
    if type(salt) == 'string'   salt = bytes().fromstring(salt) end

    self.passcode = passcode
    self.salt = salt
    self.iterations = iterations
  end

  # compute w0, w1 and L
  def compute_w0_w1()
    import crypto
    var passcode_bin = bytes().add(self.passcode, 4)      # passcode is encoded as 4 bytes int, little endian
    var tv = crypto.PBKDF2_HMAC_SHA256().derive(passcode_bin, self.salt, self.iterations, 2 * self.CRYPTO_W_SIZE_BYTES)   # generate 80 bytes
    var w0s = tv[0 .. self.CRYPTO_W_SIZE_BYTES - 1]       # w0 is the first 40 bytes
    var w1s = tv[self.CRYPTO_W_SIZE_BYTES .. 2*self.CRYPTO_W_SIZE_BYTES - 1]            # w1 is the last 40 bytes
    # take the modulus from the EC curve
    var ec = crypto.EC_P256()
    self.w0 = ec.mod(w0s)
    self.w1 = ec.mod(w1s)
    # L is w1*G which is also the commong way to generate the public key
    self.L = ec.public_key(self.w1)
  end

  # compute the shared values pA
  # w0 must be known
  # x is a random 32 bytes vector. It does not need to be lower than the order (we take the modulus anyways)
  def compute_pA(x)
    import crypto
    var ec = crypto.EC_P256()
    self.x = ec.mod(x)
    self.pA = ec.muladd(self.w0, self.M, self.x, bytes() #- empty means generator -#)    # compute x*P+w0*M
  end

  # compute the shared values pB
  # w0 must be known
  # x is a random 32 bytes vector. It does not need to be lower than the order (we take the modulus anyways)
  def compute_pB(y)
    import crypto
    var ec = crypto.EC_P256()
    self.y = ec.mod(y)
    self.pB = ec.muladd(self.w0, self.N, self.y, bytes() #- empty means generator -#)    # compute y*P+w0*M
  end

  # compute Z and V for the Prover, receiving pB from the verifier
  # pB must be bytes(65)
  # x must be known
  def compute_ZV_prover(pB)
    import crypto
    var ec = crypto.EC_P256()
    self.pB = pB
    # we are supposed to check that the point is on the curve -- TODO
    # we need a substraction so we negate w0
    var w0_neg = ec.neg(self.w0)
    var t_inner = ec.muladd(bytes("01"), self.pB, w0_neg, self.N)    # compute 1*pB-w0*N
    self.Z = ec.mul(self.x, t_inner)
    self.V = ec.mul(self.w1, t_inner)
  end

  # compute Z and V for the Verifier, receiving pA from the prover
  # pA must be bytes(65)
  # y must be known
  def compute_ZV_verifier(pA)
    import crypto
    var ec = crypto.EC_P256()
    self.pA = pA
    # we are supposed to check that the point is on the curve -- TODO
    # we need a substraction so we negate w0
    var w0_neg = ec.neg(self.w0)
    var t_inner = ec.muladd(bytes("01"), self.pA, w0_neg, self.M)    # compute 1*pA-w0*M
    self.Z = ec.mul(self.y, t_inner)
    self.V = ec.mul(self.y, self.L)
  end
end


#-
# Example:

# ======================================================================
# example from esp-matter
# passcode: 20202021
# salt: U1BBS0UyUCBLZXkgU2FsdA==
# iterations: 1000

var mc = Matter_Crypto()
mc.set_device_info(20202021, bytes().fromb64("U1BBS0UyUCBLZXkgU2FsdA=="), 1000)
mc.compute_w0_w1()

assert(mc.w0 == bytes('B96170AAE803346884724FE9A3B287C30330C2A660375D17BB205A8CF1AECB35'))
assert(mc.w1 == bytes('823D264225E36F4923B43AD64F8C862A30F4A129BBF9EE8074A32D6D67586A90'))
assert(mc.L  == bytes('0457F8AB79EE253AB6A8E46BB09E543AE422736DE501E3DB37D441FE344920D09548E4C18240630C4FF4913C53513839B7C07FCC0627A1B8573A149FCD1FA466CF'))
var test_verifier = (bytes() + mc.w0 + mc.L).tob64()
assert(test_verifier == 'uWFwqugDNGiEck/po7KHwwMwwqZgN10XuyBajPGuyzUEV/iree4lOrao5GuwnlQ65CJzbeUB49s31EH+NEkg0JVI5MGCQGMMT/SRPFNRODm3wH/MBiehuFc6FJ/NH6Rmzw==')

# ======================================================================
# Example from SPAKE2P test vectors

var mc = Matter_Crypto()
mc.w0 = bytes("bb8e1bbcf3c48f62c08db243652ae55d3e5586053fca77102994f23ad95491b3")
mc.w1 = bytes("7e945f34d78785b8a3ef44d0df5a1a97d6b3b460409a345ca7830387a74b1dba")
mc.L  = bytes("04eb7c9db3d9a9eb1f8adab81b5794c1f13ae3e225efbe91ea487425854c7fc00f00bfedcbd09b2400142d40a14f2064ef31dfaa903b91d1faea7093d835966efd")

# x is supposed to be random 32 bytes, test vector sets it
var x = bytes("d1232c8e8693d02368976c174e2088851b8365d0d79a9eee709c6a05a2fad539")
mc.compute_pA(x)
assert(mc.pA == bytes('04EF3BD051BF78A2234EC0DF197F7828060FE9856503579BB1733009042C15C0C1DE127727F418B5966AFADFDD95A6E4591D171056B333DAB97A79C7193E341727'))

# y is supposed to be random 32 bytes, test vector sets it
var y = bytes("717a72348a182085109c8d3917d6c43d59b224dc6a7fc4f0483232fa6516d8b3")
mc.compute_pB(y)
assert(mc.pB == bytes('04C0F65DA0D11927BDF5D560C69E1D7D939A05B0E88291887D679FCADEA75810FB5CC1CA7494DB39E82FF2F50665255D76173E09986AB46742C798A9A68437B048'))

mc.compute_ZV_prover(mc.pB)
assert(mc.Z == bytes('04BBFCE7DD7F277819C8DA21544AFB7964705569BDF12FB92AA388059408D50091A0C5F1D3127F56813B5337F9E4E67E2CA633117A4FBD559946AB474356C41839'))
assert(mc.V == bytes('0458BF27C6BCA011C9CE1930E8984A797A3419797B936629A5A937CF2F11C8B9514B82B993DA8A46E664F23DB7C01EDC87FAA530DB01C2EE405230B18997F16B68'))

mc.compute_ZV_verifier(mc.pA)
assert(mc.Z == bytes('04BBFCE7DD7F277819C8DA21544AFB7964705569BDF12FB92AA388059408D50091A0C5F1D3127F56813B5337F9E4E67E2CA633117A4FBD559946AB474356C41839'))
assert(mc.V == bytes('0458BF27C6BCA011C9CE1930E8984A797A3419797B936629A5A937CF2F11C8B9514B82B993DA8A46E664F23DB7C01EDC87FAA530DB01C2EE405230B18997F16B68'))

-#