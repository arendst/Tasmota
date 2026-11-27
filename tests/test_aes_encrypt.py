import ctypes
import random
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path


SOURCE = Path(__file__).resolve().parents[1] / "pio-tools" / "aes_encrypt.cpp"


def gf_multiply(left, right):
    result = 0
    for _ in range(8):
        if right & 1:
            result ^= left
        left = ((left << 1) ^ (0x11b if left & 0x80 else 0)) & 0xff
        right >>= 1
    return result


def make_sbox():
    values = []
    for value in range(256):
        inverse = 1
        for _ in range(254):
            inverse = gf_multiply(inverse, value)
        rotated = inverse
        result = inverse
        for _ in range(4):
            rotated = ((rotated << 1) | (rotated >> 7)) & 0xff
            result ^= rotated
        values.append(result ^ 0x63)
    return values


def expand_key(key, sbox):
    words = [int.from_bytes(key[offset:offset + 4], "big") for offset in range(0, 16, 4)]
    rcon = 1
    for index in range(4, 44):
        word = words[index - 1]
        if index % 4 == 0:
            word = ((word << 8) | (word >> 24)) & 0xffffffff
            word = sum(sbox[(word >> shift) & 0xff] << shift for shift in (0, 8, 16, 24))
            word ^= rcon << 24
            rcon = gf_multiply(rcon, 2)
        words.append(words[index - 4] ^ word)
    return (ctypes.c_uint32 * 61)(*words, *([0] * 16), 10)


class AesEncryptTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which("c++")
        if not compiler:
            raise unittest.SkipTest("C++ compiler not available")
        cls.temp = tempfile.TemporaryDirectory()
        directory = Path(cls.temp.name)
        (directory / "c_types.h").write_text("#define ICACHE_FLASH_ATTR\n")
        (directory / "table.cpp").write_text('extern "C" { unsigned int Te0[256]; }\n')
        library = directory / "aes.so"
        subprocess.run([compiler, "-shared", "-fPIC", "-DESP8266", "-I", str(directory),
                        str(SOURCE), str(directory / "table.cpp"), "-o", str(library)], check=True)
        cls.library = ctypes.CDLL(str(library))
        cls.encrypt = getattr(cls.library, "__wrap_aes_encrypt")
        cls.encrypt.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
        cls.sbox = make_sbox()
        table = (ctypes.c_uint32 * 256).in_dll(cls.library, "Te0")
        for index, value in enumerate(cls.sbox):
            doubled = gf_multiply(value, 2)
            table[index] = (doubled << 24) | (value << 16) | (value << 8) | (doubled ^ value)

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def encrypt_block(self, key, block, in_place=False):
        context = expand_key(key, self.sbox)
        source = ctypes.create_string_buffer(block, 16)
        output = source if in_place else ctypes.create_string_buffer(16)
        self.encrypt(context, source, output)
        return output.raw

    def test_fips_197(self):
        key = bytes.fromhex("000102030405060708090a0b0c0d0e0f")
        plain = bytes.fromhex("00112233445566778899aabbccddeeff")
        self.assertEqual(list(expand_key(key, self.sbox))[40:44],
                         [0x13111d7f, 0xe3944a17, 0xf307a78b, 0x4d2b30c5])
        expected = bytes.fromhex("69c4e0d86a7b0430d8cdb78070b4c55a")
        self.assertEqual(self.encrypt_block(key, plain), expected)
        self.assertEqual(self.encrypt_block(key, plain, in_place=True), expected)

    def test_against_openssl(self):
        openssl = shutil.which("openssl")
        if not openssl:
            self.skipTest("OpenSSL not available")
        generator = random.Random(20261127)
        for _ in range(16):
            key = generator.randbytes(16)
            plain = generator.randbytes(16 * 16)
            expected = subprocess.run([openssl, "enc", "-aes-128-ecb", "-nopad", "-nosalt", "-K", key.hex()],
                                      input=plain, stdout=subprocess.PIPE, check=True).stdout
            actual = b"".join(self.encrypt_block(key, plain[offset:offset + 16], in_place=bool(offset % 32))
                              for offset in range(0, len(plain), 16))
            self.assertEqual(actual, expected)


if __name__ == "__main__":
    unittest.main()
