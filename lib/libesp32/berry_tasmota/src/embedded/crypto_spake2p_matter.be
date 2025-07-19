# SPAKE2+ with Matter 1.0 specification
#
# Real name is SPAKE2+-P256-SHA256-HKDF-SHA256-HMAC-SHA256
# but we'll use SPAKE2P_Matter as a shorter name
#
# To start SPAKE2+, we need either w0/w1 for the prover (Matter Gateway)
# or w0/L for the verifier (end-device).
# The end-device can store in flash pre-computed values for w0/L.
#
# In this class we consider w0/w1/L to be computerd before using this class

#@ solidify:SPAKE2P_Matter,weak
class SPAKE2P_Matter
  # computed values from above
  var w0                                  # w0
  var w1
  var L
  var pA, pB, Z, V
  var x, y
  # var TT
  var Kmain
  var KcA, KcB, K_shared
  var Ke                                  # specific to Matter
  var cA, cB
  # Matter values for key generation
  var A, B, Context
  # values from the Matter specification
  static var CRYPTO_GROUP_SIZE_BYTES = 32
  static var CRYPTO_W_SIZE_BYTES = 40         # CRYPTO_GROUP_SIZE_BYTES + 8
  static var spake_M_hex = "04886e2f97ace46e55ba9dd7242579f2993b64e16ef3dcab95afd497333d8fa12f5ff355163e43ce224e0b0e65ff02ac8e5c7be09419c785e0ca547d55a12e2d20"
  static var spake_N_hex = "04d8bbd6c639c62937b04d997f38c3770719c629d7014d49a24b4f98baa1292b4907d60aa6bfade45008a636337f5168c64d9bd36034808cd564490b1e656edbe7"
  var M, N    # due to the nature of solification, we can't solidify the bytes versions nor store as static variable

  # Initialize the SPAKE2+ class with required value.
  # We need either w0/w1 for the gateway or w0/L for the end-device
  def init(w0, w1, L)
    self.M = bytes().fromhex(self.spake_M_hex)
    self.N = bytes().fromhex(self.spake_N_hex)
    # set w0/w1/L
    self.w0 = w0
    self.w1 = w1
    self.L = L
end

  # compute the shared values pA (done by Matter gateway)
  # w0 must be known
  # x is a random 32 bytes vector. It does not need to be lower than the order (we take the modulus anyways)
  #
  # if `x` is nil, we take a random vector, otherwise it must be byytes(32)
  def compute_pA(x)
    import crypto
    if x == nil   x = crypto.random(32) end
    var ec = crypto.EC_P256()
    self.x = ec.mod(x)
    self.pA = ec.muladd(self.w0, self.M, self.x, bytes() #- empty means generator -#)    # compute x*P+w0*M
    return self.pA
  end

  # compute the shared values pB (done by Matter end-device)
  # w0 must be known
  # y is a random 32 bytes vector. It does not need to be lower than the order (we take the modulus anyways)
  def compute_pB(y)
    import crypto
    if y == nil   y = crypto.random(32) end
    var ec = crypto.EC_P256()
    self.y = ec.mod(y)
    self.pB = ec.muladd(self.w0, self.N, self.y, bytes() #- empty means generator -#)    # compute y*P+w0*M
    return self.pB
  end

  # compute Z and V for the Prover (Gateway), receiving pB from the verifier (end-device)
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

  # compute Z and V for the Verifier (end-device), receiving pA from the prover (Gateway)
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

  # Set context, prover and verifier identitites (last 2 can be empty)
  def set_context(context, prover, verifier)
    if prover == nil    prover   = bytes() end
    if verifier == nil  verifier = bytes() end
    self.Context = context
    self.A = prover
    self.B = verifier
  end

  # Need to know "Context, pA, pB, Z, V, w0"
  #
  # Arg:
  #  matter_specific: if set to `true`, uses only half of the hash as implemented
  #                   in reference code, but not compliant with SPAKE2+
  def compute_TT_hash(matter_specific)
    class SPAKE_Hasher
      var hash
      # var complete        # complete value for the bytes -- will be removed in production code
      def init()
        import crypto
        self.hash = crypto.SHA256()
        # self.complete = bytes()
      end
      # add v (bytes) to the hash
      def add_item(v)
        var len = v.size()
        var len_bin = bytes().add(len, 4).add(0, 4)
        # self.complete += len_bin
        # self.complete += v
        self.hash.update(len_bin)
        self.hash.update(v)
      end
      def out()
        return self.hash.out()
      end
    end
    import crypto
    var hasher = SPAKE_Hasher()
    
    # Context
    hasher.add_item(self.Context)
    hasher.add_item(self.A)
    hasher.add_item(self.B)
    hasher.add_item(self.M)
    hasher.add_item(self.N)
    hasher.add_item(self.pA)
    hasher.add_item(self.pB)
    hasher.add_item(self.Z)
    hasher.add_item(self.V)
    hasher.add_item(self.w0)

    # self.TT = hasher.complete
    self.Kmain = hasher.out()
    if matter_specific
      self.Ke = self.Kmain[16..31]
      self.Kmain = self.Kmain[0..15]
    end

    # compute KcA and KcB
    var kdf = crypto.HKDF_SHA256()
    var KPV = kdf.derive(self.Kmain, bytes(), bytes().fromstring("ConfirmationKeys"), 64)
    self.KcA = matter_specific ? KPV[0..15] : KPV[0..31]
    self.KcB = matter_specific ? KPV[16..31] : KPV[32..63]
    self.K_shared = kdf.derive(self.Kmain, bytes(), bytes().fromstring("SharedKey"), 32)
    # if matter_specific    self.K_shared = self.K_shared[0..15] end

    self.cA = crypto.HMAC_SHA256(self.KcA).update(self.pB).out()
    self.cB = crypto.HMAC_SHA256(self.KcB).update(self.pA).out()
  end
end


#-
# ======================================================================
# Example from SPAKE2P test vectors
# https://chris-wood.github.io/draft-bar-cfrg-spake2plus/draft-bar-cfrg-spake2plus.html

import crypto
var w0 = bytes("bb8e1bbcf3c48f62c08db243652ae55d3e5586053fca77102994f23ad95491b3")
var w1 = bytes("7e945f34d78785b8a3ef44d0df5a1a97d6b3b460409a345ca7830387a74b1dba")
var L  = bytes("04eb7c9db3d9a9eb1f8adab81b5794c1f13ae3e225efbe91ea487425854c7fc00f00bfedcbd09b2400142d40a14f2064ef31dfaa903b91d1faea7093d835966efd")
var spake_matter = crypto.SPAKE2P_Matter(w0, w1, L)

# context values
var Context = bytes().fromstring('SPAKE2+-P256-SHA256-HKDF-SHA256-HMAC-SHA256 Test Vectors')
var Prover = bytes().fromstring('client')
var Verifier = bytes().fromstring('server')
spake_matter.set_context(Context, Prover, Verifier)

# x is supposed to be random 32 bytes, test vector sets it
var x = bytes("d1232c8e8693d02368976c174e2088851b8365d0d79a9eee709c6a05a2fad539")
spake_matter.compute_pA(x)
assert(spake_matter.pA == bytes('04EF3BD051BF78A2234EC0DF197F7828060FE9856503579BB1733009042C15C0C1DE127727F418B5966AFADFDD95A6E4591D171056B333DAB97A79C7193E341727'))

# y is supposed to be random 32 bytes, test vector sets it
var y = bytes("717a72348a182085109c8d3917d6c43d59b224dc6a7fc4f0483232fa6516d8b3")
spake_matter.compute_pB(y)
assert(spake_matter.pB == bytes('04C0F65DA0D11927BDF5D560C69E1D7D939A05B0E88291887D679FCADEA75810FB5CC1CA7494DB39E82FF2F50665255D76173E09986AB46742C798A9A68437B048'))

spake_matter.compute_ZV_prover(spake_matter.pB)
assert(spake_matter.Z == bytes('04BBFCE7DD7F277819C8DA21544AFB7964705569BDF12FB92AA388059408D50091A0C5F1D3127F56813B5337F9E4E67E2CA633117A4FBD559946AB474356C41839'))
assert(spake_matter.V == bytes('0458BF27C6BCA011C9CE1930E8984A797A3419797B936629A5A937CF2F11C8B9514B82B993DA8A46E664F23DB7C01EDC87FAA530DB01C2EE405230B18997F16B68'))

spake_matter.compute_ZV_verifier(spake_matter.pA)
assert(spake_matter.Z == bytes('04BBFCE7DD7F277819C8DA21544AFB7964705569BDF12FB92AA388059408D50091A0C5F1D3127F56813B5337F9E4E67E2CA633117A4FBD559946AB474356C41839'))
assert(spake_matter.V == bytes('0458BF27C6BCA011C9CE1930E8984A797A3419797B936629A5A937CF2F11C8B9514B82B993DA8A46E664F23DB7C01EDC87FAA530DB01C2EE405230B18997F16B68'))

spake_matter.compute_TT_hash()

assert(spake_matter.Kmain == bytes('4c59e1ccf2cfb961aa31bd9434478a1089b56cd11542f53d3576fb6c2a438a29'))

assert(spake_matter.KcA == bytes('871ae3f7b78445e34438fb284504240239031c39d80ac23eb5ab9be5ad6db58a'))
assert(spake_matter.KcB == bytes('ccd53c7c1fa37b64a462b40db8be101cedcf838950162902054e644b400f1680'))

assert(spake_matter.cA == bytes('926cc713504b9b4d76c9162ded04b5493e89109f6d89462cd33adc46fda27527'))
assert(spake_matter.cB == bytes('9747bcc4f8fe9f63defee53ac9b07876d907d55047e6ff2def2e7529089d3e68'))
assert(spake_matter.K_shared   == bytes('0c5f8ccd1413423a54f6c1fb26ff01534a87f893779c6e68666d772bfd91f3e7'))

print("SPAKE2P Matter all tests passed")

-#