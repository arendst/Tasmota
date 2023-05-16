cmake_minimum_required(VERSION 2.8)

set(SOURCES
    bignum.c ecdsa.c curves.c secp256k1.c nist256p1.c rand.c hmac.c bip32.c bip39.c pbkdf2.c base58.c base32.c
    address.c
    script.c
    ripemd160.c
    sha2.c
    sha3.c
    hasher.c
    aes/aescrypt.c aes/aeskey.c aes/aestab.c aes/aes_modes.c
    ed25519-donna/curve25519-donna-32bit.c ed25519-donna/curve25519-donna-helpers.c ed25519-donna/modm-donna-32bit.c
    ed25519-donna/ed25519-donna-basepoint-table.c ed25519-donna/ed25519-donna-32bit-tables.c ed25519-donna/ed25519-donna-impl-base.c
    ed25519-donna/ed25519.c ed25519-donna/curve25519-donna-scalarmult-base.c ed25519-donna/ed25519-sha3.c ed25519-donna/ed25519-keccak.c
    blake256.c
    blake2b.c blake2s.c
    chacha20poly1305/chacha20poly1305.c chacha20poly1305/chacha_merged.c chacha20poly1305/poly1305-donna.c chacha20poly1305/rfc7539.c
    rc4.c
    nem.c
)

add_library(TrezorCrypto STATIC ${SOURCES})

target_include_directories(TrezorCrypto PUBLIC .)
target_include_directories(TrezorCrypto PUBLIC aes)
target_include_directories(TrezorCrypto PUBLIC chacha20poly1305)
target_include_directories(TrezorCrypto PUBLIC ed25519-donna)

target_compile_options(TrezorCrypto PRIVATE "-std=c99")

if(MSVC)
  set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
endif(MSVC)

# Build trezor-crypto tests (requires OpenSSL)
if (TREZOR_CRYPTO_TESTS)
    add_executable(test_check test_check.c)
    target_link_libraries(test_check TrezorCrypto check rt pthread m crypto)
    add_test(NAME trezor-crypto COMMAND test_check)

    add_executable(test_openssl test_openssl.c)
    target_link_libraries(test_openssl TrezorCrypto check rt pthread m crypto)
    add_test(NAME trezor-crypto-openssl COMMAND test_openssl 100)
endif()
