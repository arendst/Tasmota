# Trimmed down version of BearSSL

Standard BearSSL lib is able to handle RSA keys up to 4096 bits and EC keys up to 521 bits. As we are limiting the use to AWS IoT, we can save hundreds of bytes of memory by limiting to 2048 bit RSA keys and 256 bits EC keys.

This is just the normal [Arduino version of BearSSL](https://github.com/earlephilhower/bearssl-esp8266)

There are only two changes in `src/inner.h`:

* Line 59

  ```#define BR_MAX_RSA_SIZE   2048```

* Line 85

  ```#define BR_MAX_EC_SIZE   256```

Then compile with

```make CONF=esp8266```

Finally copy `libbearssl.a` to this directory.
