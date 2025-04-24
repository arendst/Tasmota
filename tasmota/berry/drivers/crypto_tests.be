import crypto
e = crypto.ED25519()
seed = bytes("9d61b19deffd5a60ba844af492ec2cc44449c5697b326919703bac031cae7f60")
secret_key=e.secret_key(seed)
message = bytes()
public_key = secret_key[-32..]
_public_key = bytes("d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a")
assert(public_key == _public_key)
signature = e.sign(message, secret_key)
assert(signature == bytes("e5564300c360ac729086e2cc806e828a84877f1eb8e5d974d873e065224901555fb8821590a33bacc61e39701cf9b46bd25bf5f0595bbe24655141438e7a100b"))
# now verify
assert(e.verify(message, signature,public_key)==true)


def pad16(data)
    # pad to 16 bytes
    if (size(data) % 16) == 0
        return data
    else
        return data + bytes(-(16 - (size(data) % 16)))
    end
end

# https://boringssl.googlesource.com/boringssl/+/2e2a226ac9201ac411a84b5e79ac3a7333d8e1c9/crypto/cipher_extra/test/chacha20_poly1305_tests.txt
import crypto
c = crypto.CHACHA20_POLY1305()
key = bytes("808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f")
iv =bytes("070000004041424344454647")
aad= bytes("50515253c0c1c2c3c4c5c6c7")
_msg = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it."
msg = bytes().fromstring(_msg)
ct = bytes("d31a8d34648e60db7b86afbc53ef7ec2a4aded51296e08fea9e2b5a736ee62d63dbea45e8ca9671282fafb69da92728b1a71de0a9e060b2905d6a5b67ecd3b3692ddbd7f2d778b8c9803aee328091b58fab324e4fad675945585808b4831d7bc3ff4def08e4b7a9de576d26586cec64b6116")
tag = bytes("1ae10b594f09e26a7e902ecbd0600691")
#create polykey
poly_key = bytes(-32)
c.chacha_run(key,iv,0,poly_key)
sizes = bytes(-16)
sizes.seti(0,size(aad),4)
sizes.seti(8,size(ct),4)
_tag = c.poly_run(pad16(aad)+pad16(ct)+sizes,poly_key)
# encrypt
c.chacha_run(key,iv,1,msg)
assert(_tag == tag)
assert(ct == msg)

# now decrypt - reuse aad and sizes
 _newtag = c.poly_run(pad16(aad)+pad16(msg)+sizes,poly_key)
c.chacha_run(key,iv,1,msg)
assert(_newtag==_tag)
assert(msg.asstring()==_msg)

# https://datatracker.ietf.org/doc/draft-ietf-sshm-chacha20-poly1305/01/
# special needs for SSH:
# chacha run over length and data with different keys
# poly run over whole encrypted material with key used for data

import crypto
c = crypto.CHACHA20_POLY1305()
keys = bytes("8bbff6855fc102338c373e73aac0c914f076a905b2444a32eecaffeae22becc5e9b7a7a5825a8249346ec1c28301cf394543fc7569887d76e168f37562ac0740")
packet = bytes("2c3ecce4a5bc05895bf07a7ba956b6c68829ac7c83b780b7000ecde745afc705bbc378ce03a280236b87b53bed5839662302b164b6286a48cd1e097138e3cb909b8b2b829dd18d2a35ff82d995349e855bf02c298ef775f2d1a7e8b8")
iv = bytes("000000000000000000000007") # seq number
k_header = keys[-32..]
k_main = keys[0..31]

poly_key = bytes(-32)
c.chacha_run(k_main,iv,0,poly_key)
given_mac = packet[-16..]
calculated_mac = c.poly_run(packet[0..-17],poly_key)
assert(calculated_mac == given_mac)

data = packet[4..-17]
length = packet[0..3]
c.chacha_run(k_header,iv,0,length) # use upper 32 bytes of key material
assert(length == bytes("00000048"))
counter = c.chacha_run(k_main, iv, 1, data) # lower bytes of key for packet
# assert(valid == true)
assert(length + data == bytes("00000048065e00000000000000384c6f72656d20697073756d20646f6c6f722073697420616d65742c20636f6e7365637465747572206164697069736963696e6720656c69744e43e804dc6c"))

# reverse it - now encrypt a packet

raw_packet = bytes("00000048065e00000000000000384c6f72656d20697073756d20646f6c6f722073697420616d65742c20636f6e7365637465747572206164697069736963696e6720656c69744e43e804dc6c")
length = raw_packet[0..3]
c.chacha_run(k_header,iv,0,length) # use upper 32 bytes of key material

data = raw_packet[4..]
counter = c.chacha_run(k_main, iv, 1, data) # lower bytes of key for packet
enc_packet = length + data

poly_key = bytes(-32)
c.chacha_run(k_main,iv,0,poly_key)
mac = c.poly_run(enc_packet,poly_key)
enc_packet .. mac
assert(enc_packet == packet)
