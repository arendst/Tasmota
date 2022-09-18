#-
ec = crypto.EC_C25519()

# Alice
sk_A = bytes('77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a')
pk_A = ec.public_key(sk_A)
assert(pk_A == bytes('8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a'))

# Bob
sk_B = bytes('5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb')
pk_B = ec.public_key(sk_B)
assert(pk_B == bytes('de9edb7d7b7dc1b4d35b61c2ece435373f8343c85b78674dadfc7e146f882b4f'))

psk = ec.shared_key(sk_A, pk_B)
assert(psk == bytes('4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742'))
psk2 = ec.shared_key(sk_B, pk_A)
assert(psk2 == bytes('4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742'))
-#

#- test vectors from RFC77748

   Alice's private key, a:
     77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a
   Alice's public key, X25519(a, 9):
     8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a
   Bob's private key, b:
     5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb
   Bob's public key, X25519(b, 9):
     de9edb7d7b7dc1b4d35b61c2ece435373f8343c85b78674dadfc7e146f882b4f
   Their shared secret, K:
     4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742
-#
