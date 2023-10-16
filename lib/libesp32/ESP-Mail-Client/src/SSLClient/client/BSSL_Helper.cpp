/*
  WiFiClientBearSSL- SSL client/server for esp8266 using BearSSL libraries
  - Mostly compatible with Arduino WiFi shield library and standard
    WiFiClient/ServerSecure (except for certificate handling).

  Copyright (c) 2018 Earle F. Philhower, III

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef BSSL_HELPER_CPP
#define BSSL_HELPER_CPP

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wvla"

#include <Arduino.h>
#include "../ESP_SSLClient_FS.h"
#include "../ESP_SSLClient_Const.h"
#if defined(USE_LIB_SSL_ENGINE)

#include "BSSL_Helper.h"
#include <memory>
#include <vector>
#include "../bssl/bearssl.h"
#include <stdlib.h>
#include <string.h>
#if defined __has_include
#if __has_include(<pgmspace.h>)
#include <pgmspace.h>
#endif
#endif

namespace key_bssl
{
  // Code here is pulled from brssl sources, with the copyright and license
  // shown below.  I've rewritten things using C++ semantics and removed
  // custom VEC_* calls (std::vector to the rescue) and adjusted things to
  // allow for long-running operation (i.e. some memory issues when DERs
  // passed into the decoders).  Bugs are most likely my fault.

  // Original (c) message follows:
  /*
     Copyright (c) 2016 Thomas Pornin <pornin@bolet.org>

     Permission is hereby granted, free of charge, to any person obtaining
     a copy of this software and associated documentation files (the
     "Software"), to deal in the Software without restriction, including
     without limitation the rights to use, copy, modify, merge, publish,
     distribute, sublicense, and/or sell copies of the Software, and to
     permit persons to whom the Software is furnished to do so, subject to
     the following conditions:

     The above copyright notice and this permission notice shall be
     included in all copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
     BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
     ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
     CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
     SOFTWARE.
  */

  class private_key
  {
  public:
    int key_type; /* BR_KEYTYPE_RSA or BR_KEYTYPE_EC */
    union
    {
      br_rsa_private_key rsa;
      br_ec_private_key ec;
    } key;
  };

  class public_key
  {
  public:
    int key_type; /* BR_KEYTYPE_RSA or BR_KEYTYPE_EC */
    union
    {
      br_rsa_public_key rsa;
      br_ec_public_key ec;
    } key;
  };

  class pem_object
  {
  public:
    char *name;
    unsigned char *data;
    size_t data_len;
  };

  // Forward definitions
  void free_ta_contents(br_x509_trust_anchor *ta);
  void free_public_key(public_key *pk);
  void free_private_key(private_key *sk);
  bool looks_like_DER(const unsigned char *buf, size_t len);
  pem_object *decode_pem(const void *src, size_t len, size_t *num);
  void free_pem_object_contents(pem_object *po);
  char *strdupImpl(const char *s);

  // Used as callback multiple places to append a string to a vector
  static void byte_vector_append(void *ctx, const void *buff, size_t len)
  {
    std::vector<uint8_t> *vec = static_cast<std::vector<uint8_t> *>(ctx);
    vec->reserve(vec->size() + len); // Allocate extra space all at once
    for (size_t i = 0; i < len; i++)
    {
      vec->push_back(((uint8_t *)buff)[i]);
    }
  }

  static bool certificate_to_trust_anchor_inner(br_x509_trust_anchor *ta, const br_x509_certificate *xc)
  {
    std::unique_ptr<br_x509_decoder_context_libmail> dc(new br_x509_decoder_context_libmail); // auto-delete on exit
    std::vector<uint8_t> vdn;
    br_x509_pkey *pk;

    // Clear everything in the Trust Anchor
    memset(ta, 0, sizeof(*ta));

    br_x509_decoder_init_libmail(dc.get(), byte_vector_append, (void *)&vdn);
    br_x509_decoder_push_libmail(dc.get(), xc->data, xc->data_len);
    pk = br_x509_decoder_get_pkey(dc.get());
    if (pk == nullptr)
    {
      return false; // No key present, something broken in the cert!
    }

    // Copy the raw certificate data
    ta->dn.data = (uint8_t *)malloc(vdn.size());
    if (!ta->dn.data)
    {
      return false; // OOM, but nothing yet allocated
    }
    memcpy(ta->dn.data, &vdn[0], vdn.size());
    ta->dn.len = vdn.size();
    ta->flags = 0;
    if (br_x509_decoder_isCA(dc.get()))
    {
      ta->flags |= BR_X509_TA_CA;
    }

    // Extract the public key
    switch (pk->key_type)
    {
    case BR_KEYTYPE_RSA:
      ta->pkey.key_type = BR_KEYTYPE_RSA;
      ta->pkey.key.rsa.n = (uint8_t *)malloc(pk->key.rsa.nlen);
      ta->pkey.key.rsa.e = (uint8_t *)malloc(pk->key.rsa.elen);
      if ((ta->pkey.key.rsa.n == nullptr) || (ta->pkey.key.rsa.e == nullptr))
      {
        free_ta_contents(ta); // OOM, so clean up
        return false;
      }
      memcpy(ta->pkey.key.rsa.n, pk->key.rsa.n, pk->key.rsa.nlen);
      ta->pkey.key.rsa.nlen = pk->key.rsa.nlen;
      memcpy(ta->pkey.key.rsa.e, pk->key.rsa.e, pk->key.rsa.elen);
      ta->pkey.key.rsa.elen = pk->key.rsa.elen;
      return true;
    case BR_KEYTYPE_EC:
      ta->pkey.key_type = BR_KEYTYPE_EC;
      ta->pkey.key.ec.curve = pk->key.ec.curve;
      ta->pkey.key.ec.q = (uint8_t *)malloc(pk->key.ec.qlen);
      if (ta->pkey.key.ec.q == nullptr)
      {
        free_ta_contents(ta); // OOM, so clean up
        return false;
      }
      memcpy(ta->pkey.key.ec.q, pk->key.ec.q, pk->key.ec.qlen);
      ta->pkey.key.ec.qlen = pk->key.ec.qlen;
      return true;
    default:
      free_ta_contents(ta); // Unknown key type
      return false;
    }

    // Should never get here, if so there was an unknown error
    return false;
  }

  br_x509_trust_anchor *certificate_to_trust_anchor(const br_x509_certificate *xc)
  {
    br_x509_trust_anchor *ta = (br_x509_trust_anchor *)malloc(sizeof(br_x509_trust_anchor));
    if (!ta)
    {
      return nullptr;
    }

    if (!certificate_to_trust_anchor_inner(ta, xc))
    {
      free(ta);
      return nullptr;
    }
    return ta;
  }

  void free_ta_contents(br_x509_trust_anchor *ta)
  {
    if (ta)
    {
      free(ta->dn.data);
      if (ta->pkey.key_type == BR_KEYTYPE_RSA)
      {
        free(ta->pkey.key.rsa.n);
        free(ta->pkey.key.rsa.e);
      }
      else if (ta->pkey.key_type == BR_KEYTYPE_EC)
      {
        free(ta->pkey.key.ec.q);
      }
      memset(ta, 0, sizeof(*ta));
    }
  }

  // Checks if a bitstream looks like a valid DER(binary) encoding.
  // Basically tries to verify the length of all included segments
  // matches the length of the input buffer.  Does not actually
  // validate any contents.
  bool looks_like_DER(const unsigned char *buff, size_t len)
  {
    if (len < 2)
    {
      return false;
    }
    if (pgm_read_byte(buff++) != 0x30)
    {
      return false;
    }
    int fb = pgm_read_byte(buff++);
    len -= 2;
    if (fb < 0x80)
    {
      return (size_t)fb == len;
    }
    else if (fb == 0x80)
    {
      return false;
    }
    else
    {
      fb -= 0x80;
      if (len < (size_t)fb + 2)
      {
        return false;
      }
      len -= (size_t)fb;
      size_t dlen = 0;
      while (fb-- > 0)
      {
        if (dlen > (len >> 8))
        {
          return false;
        }
        dlen = (dlen << 8) + (size_t)pgm_read_byte(buff++);
      }
      return dlen == len;
    }
  }

  void free_pem_object_contents(pem_object *po)
  {
    if (po)
    {
      free(po->name);
      free(po->data);
      po->name = nullptr;
      po->data = nullptr;
    }
  }

  char *strdupImpl(const char *s)
  {
    size_t slen = strlen(s);
    char *result = (char *)malloc(slen + 1);
    if (!result)
      return NULL;
    memcpy(result, s, slen + 1);
    return result;
  }

  // Converts a PEM (~=base64) source into a set of DER-encoded binary blobs.
  // Each blob is named by the ---- BEGIN xxx ---- field, and multiple
  // blobs may be returned.
  pem_object *decode_pem(const void *src, size_t len, size_t *num)
  {
    std::vector<pem_object> pem_list;
    std::unique_ptr<br_pem_decoder_context> pc(new br_pem_decoder_context); // auto-delete on exit
    if (!pc.get())
    {
      return nullptr;
    }
    pem_object po, *pos;
    const unsigned char *buff;
    std::vector<uint8_t> bv;

    *num = 0;
    br_pem_decoder_init(pc.get());
    buff = (const unsigned char *)src;
    po.name = nullptr;
    po.data = nullptr;
    po.data_len = 0;
    bool inobj = false;
    bool extra_nl = true;

    while (len > 0)
    {
      size_t tlen;

      tlen = br_pem_decoder_push(pc.get(), buff, len);
      buff += tlen;
      len -= tlen;
      switch (br_pem_decoder_event(pc.get()))
      {
      case BR_PEM_BEGIN_OBJ:
        po.name = strdupImpl(br_pem_decoder_name(pc.get()));
        br_pem_decoder_setdest(pc.get(), byte_vector_append, &bv);
        inobj = true;
        break;

      case BR_PEM_END_OBJ:
        if (inobj)
        {
          // Stick data into the vector
          po.data = (uint8_t *)malloc(bv.size());
          if (po.data)
          {
            memcpy(po.data, &bv[0], bv.size());
            po.data_len = bv.size();
            pem_list.push_back(po);
          }
          // Clean up state for next blob processing
          bv.clear();
          po.name = nullptr;
          po.data = nullptr;
          po.data_len = 0;
          inobj = false;
        }
        break;

      case BR_PEM_ERROR:
        free(po.name);
        for (size_t i = 0; i < pem_list.size(); i++)
        {
          free_pem_object_contents(&pem_list[i]);
        }
        return nullptr;

      default:
        // Do nothing here, the parser is still working on things
        break;
      }

      if (len == 0 && extra_nl)
      {
        extra_nl = false;
        buff = (const unsigned char *)"\n";
        len = 1;
      }
    }

    if (inobj)
    {
      free(po.name);
      for (size_t i = 0; i < pem_list.size(); i++)
      {
        free_pem_object_contents(&pem_list[i]);
      }
      return nullptr;
    }

    pos = (pem_object *)malloc((1 + pem_list.size()) * sizeof(*pos));
    if (pos)
    {
      *num = pem_list.size();
      pem_list.push_back(po); // Null-terminate list
      memcpy(pos, &pem_list[0], pem_list.size() * sizeof(*pos));
    }
    return pos;
  }

  // Parse out DER or PEM encoded certificates from a binary buffer,
  // potentially stored in PROGMEM.
  br_x509_certificate *read_certificates(const char *buff, size_t len, size_t *num)
  {
    std::vector<br_x509_certificate> cert_list;
    pem_object *pos;
    size_t u, num_pos;
    br_x509_certificate *xcs;
    br_x509_certificate dummy;

    *num = 0;

    if (looks_like_DER((const unsigned char *)buff, len))
    {
      xcs = (br_x509_certificate *)malloc(2 * sizeof(*xcs));
      if (!xcs)
      {
        return nullptr;
      }
      xcs[0].data = (uint8_t *)malloc(len);
      if (!xcs[0].data)
      {
        free(xcs);
        return nullptr;
      }
      memcpy_P(xcs[0].data, buff, len);
      xcs[0].data_len = len;
      xcs[1].data = nullptr;
      xcs[1].data_len = 0;
      *num = 1;
      return xcs;
    }

    pos = decode_pem(buff, len, &num_pos);
    if (!pos)
    {
      return nullptr;
    }
    for (u = 0; u < num_pos; u++)
    {
      if (!strcmp_P(pos[u].name, PSTR("CERTIFICATE")) || !strcmp_P(pos[u].name, PSTR("X509 CERTIFICATE")))
      {
        br_x509_certificate xc;
        xc.data = pos[u].data;
        xc.data_len = pos[u].data_len;
        pos[u].data = nullptr; // Don't free the data we moved to the xc vector!
        cert_list.push_back(xc);
      }
    }
    for (u = 0; u < num_pos; u++)
    {
      free_pem_object_contents(&pos[u]);
    }
    free(pos);

    if (cert_list.size() == 0)
    {
      return nullptr;
    }
    *num = cert_list.size();
    dummy.data = nullptr;
    dummy.data_len = 0;
    cert_list.push_back(dummy);
    xcs = (br_x509_certificate *)malloc(cert_list.size() * sizeof(*xcs));
    if (!xcs)
    {
      for (size_t i = 0; i < cert_list.size(); i++)
      {
        free(cert_list[i].data); // Clean up any captured data blobs
      }
      return nullptr;
    }
    memcpy(xcs, &cert_list[0], cert_list.size() * sizeof(br_x509_certificate));
    // XCS now has [].data pointing to the previously allocated blobs, so don't
    // want to free anything in cert_list[].
    return xcs;
  }

  void free_certificates(br_x509_certificate *certs, size_t num)
  {
    if (certs)
    {
      for (size_t u = 0; u < num; u++)
      {
        free(certs[u].data);
      }
      free(certs);
    }
  }

  static public_key *decode_public_key(const unsigned char *buff, size_t len)
  {
    std::unique_ptr<br_pkey_decoder_context> dc(new br_pkey_decoder_context); // auto-delete on exit
    if (!dc.get())
    {
      return nullptr;
    }

    public_key *pk = nullptr;
    // https://github.com/yglukhov/bearssl_pkey_decoder
    br_pkey_decoder_init(dc.get());
    br_pkey_decoder_push(dc.get(), buff, len);
    int err = br_pkey_decoder_last_error(dc.get());
    if (err != 0)
    {
      return nullptr;
    }

    const br_rsa_public_key *rk = nullptr;
    const br_ec_public_key *ek = nullptr;
    switch (br_pkey_decoder_key_type(dc.get()))
    {
    case BR_KEYTYPE_RSA:
      rk = br_pkey_decoder_get_rsa(dc.get());
      pk = (public_key *)malloc(sizeof *pk);
      if (!pk)
      {
        return nullptr;
      }
      pk->key_type = BR_KEYTYPE_RSA;
      pk->key.rsa.n = (uint8_t *)malloc(rk->nlen);
      pk->key.rsa.e = (uint8_t *)malloc(rk->elen);
      if (!pk->key.rsa.n || !pk->key.rsa.e)
      {
        free(pk->key.rsa.n);
        free(pk->key.rsa.e);
        free(pk);
        return nullptr;
      }
      memcpy(pk->key.rsa.n, rk->n, rk->nlen);
      pk->key.rsa.nlen = rk->nlen;
      memcpy(pk->key.rsa.e, rk->e, rk->elen);
      pk->key.rsa.elen = rk->elen;
      return pk;

    case BR_KEYTYPE_EC:
      ek = br_pkey_decoder_get_ec(dc.get());
      pk = (public_key *)malloc(sizeof *pk);
      if (!pk)
      {
        return nullptr;
      }
      pk->key_type = BR_KEYTYPE_EC;
      pk->key.ec.q = (uint8_t *)malloc(ek->qlen);
      if (!pk->key.ec.q)
      {
        free(pk);
        return nullptr;
      }
      memcpy(pk->key.ec.q, ek->q, ek->qlen);
      pk->key.ec.qlen = ek->qlen;
      pk->key.ec.curve = ek->curve;
      return pk;

    default:
      return nullptr;
    }
  }

  void free_public_key(public_key *pk)
  {
    if (pk)
    {
      if (pk->key_type == BR_KEYTYPE_RSA)
      {
        free(pk->key.rsa.n);
        free(pk->key.rsa.e);
      }
      else if (pk->key_type == BR_KEYTYPE_EC)
      {
        free(pk->key.ec.q);
      }
      free(pk);
    }
  }

  static private_key *decode_private_key(const unsigned char *buff, size_t len)
  {
    std::unique_ptr<br_skey_decoder_context> dc(new br_skey_decoder_context); // auto-delete on exit
    if (!dc.get())
    {
      return nullptr;
    }

    private_key *sk = nullptr;

    br_skey_decoder_init(dc.get());
    br_skey_decoder_push(dc.get(), buff, len);
    int err = br_skey_decoder_last_error(dc.get());
    if (err != 0)
    {
      return nullptr;
    }

    const br_rsa_private_key *rk = nullptr;
    const br_ec_private_key *ek = nullptr;
    switch (br_skey_decoder_key_type(dc.get()))
    {
    case BR_KEYTYPE_RSA:
      rk = br_skey_decoder_get_rsa(dc.get());
      sk = (private_key *)malloc(sizeof *sk);
      if (!sk)
      {
        return nullptr;
      }
      sk->key_type = BR_KEYTYPE_RSA;
      sk->key.rsa.p = (uint8_t *)malloc(rk->plen);
      sk->key.rsa.q = (uint8_t *)malloc(rk->qlen);
      sk->key.rsa.dp = (uint8_t *)malloc(rk->dplen);
      sk->key.rsa.dq = (uint8_t *)malloc(rk->dqlen);
      sk->key.rsa.iq = (uint8_t *)malloc(rk->iqlen);
      if (!sk->key.rsa.p || !sk->key.rsa.q || !sk->key.rsa.dp || !sk->key.rsa.dq || !sk->key.rsa.iq)
      {
        free_private_key(sk);
        return nullptr;
      }
      sk->key.rsa.n_bitlen = rk->n_bitlen;
      memcpy(sk->key.rsa.p, rk->p, rk->plen);
      sk->key.rsa.plen = rk->plen;
      memcpy(sk->key.rsa.q, rk->q, rk->qlen);
      sk->key.rsa.qlen = rk->qlen;
      memcpy(sk->key.rsa.dp, rk->dp, rk->dplen);
      sk->key.rsa.dplen = rk->dplen;
      memcpy(sk->key.rsa.dq, rk->dq, rk->dqlen);
      sk->key.rsa.dqlen = rk->dqlen;
      memcpy(sk->key.rsa.iq, rk->iq, rk->iqlen);
      sk->key.rsa.iqlen = rk->iqlen;
      return sk;

    case BR_KEYTYPE_EC:
      ek = br_skey_decoder_get_ec(dc.get());
      sk = (private_key *)malloc(sizeof *sk);
      if (!sk)
      {
        return nullptr;
      }
      sk->key_type = BR_KEYTYPE_EC;
      sk->key.ec.curve = ek->curve;
      sk->key.ec.x = (uint8_t *)malloc(ek->xlen);
      if (!sk->key.ec.x)
      {
        free_private_key(sk);
        return nullptr;
      }
      memcpy(sk->key.ec.x, ek->x, ek->xlen);
      sk->key.ec.xlen = ek->xlen;
      return sk;

    default:
      return nullptr;
    }
  }

  void free_private_key(private_key *sk)
  {
    if (sk)
    {
      switch (sk->key_type)
      {
      case BR_KEYTYPE_RSA:
        free(sk->key.rsa.p);
        free(sk->key.rsa.q);
        free(sk->key.rsa.dp);
        free(sk->key.rsa.dq);
        free(sk->key.rsa.iq);
        break;
      case BR_KEYTYPE_EC:
        free(sk->key.ec.x);
        break;
      default:
        // Could be an uninitted key, no sub elements to free
        break;
      }
      free(sk);
    }
  }

  void free_pem_object(pem_object *pos)
  {
    if (pos != nullptr)
    {
      for (size_t u = 0; pos[u].name; u++)
      {
        free_pem_object_contents(&pos[u]);
      }
      free(pos);
    }
  }

  private_key *read_private_key(const char *buff, size_t len)
  {
    private_key *sk = nullptr;
    pem_object *pos = nullptr;

    if (looks_like_DER((const unsigned char *)buff, len))
    {
      sk = decode_private_key((const unsigned char *)buff, len);
      return sk;
    }

    size_t num;
    pos = decode_pem(buff, len, &num);
    if (pos == nullptr)
    {
      return nullptr; // PEM decode error
    }
    for (size_t u = 0; pos[u].name; u++)
    {
      const char *name = pos[u].name;
      if (!strcmp_P(name, PSTR("RSA PRIVATE KEY")) || !strcmp_P(name, PSTR("EC PRIVATE KEY")) || !strcmp_P(name, PSTR("PRIVATE KEY")))
      {
        sk = decode_private_key(pos[u].data, pos[u].data_len);
        free_pem_object(pos);
        return sk;
      }
    }
    // If we hit here, no match
    free_pem_object(pos);
    return nullptr;
  }

  public_key *read_public_key(const char *buff, size_t len)
  {
    public_key *pk = nullptr;
    pem_object *pos = nullptr;

    if (looks_like_DER((const unsigned char *)buff, len))
    {
      pk = decode_public_key((const unsigned char *)buff, len);
      return pk;
    }
    size_t num;
    pos = decode_pem(buff, len, &num);
    if (pos == nullptr)
    {
      return nullptr; // PEM decode error
    }
    for (size_t u = 0; pos[u].name; u++)
    {
      const char *name = pos[u].name;
      if (!strcmp_P(name, PSTR("RSA PUBLIC KEY")) || !strcmp_P(name, PSTR("EC PUBLIC KEY")) || !strcmp_P(name, PSTR("PUBLIC KEY")))
      {
        pk = decode_public_key(pos[u].data, pos[u].data_len);
        free_pem_object(pos);
        return pk;
      }
    }

    // We hit here == no key found
    free_pem_object(pos);
    return pk;
  }

  static uint8_t *loadStream(Stream &stream, size_t size)
  {
    uint8_t *dest = (uint8_t *)malloc(size);
    if (!dest)
    {
      return nullptr; // OOM error
    }
    if (size != stream.readBytes(dest, size))
    {
      free(dest); // Error during read
      return nullptr;
    }
    return dest;
  }
};

namespace bssl
{

  // ----- Public Key -----

  PublicKey::PublicKey()
  {
    _key = nullptr;
  }

  PublicKey::PublicKey(const char *pemKey)
  {
    _key = nullptr;
    parse(pemKey);
  }

  PublicKey::PublicKey(const uint8_t *derKey, size_t derLen)
  {
    _key = nullptr;
    parse(derKey, derLen);
  }

  PublicKey::PublicKey(Stream &stream, size_t size)
  {
    _key = nullptr;
    auto buff = key_bssl::loadStream(stream, size);
    if (buff)
    {
      parse(buff, size);
      free(buff);
    }
  }

  PublicKey::~PublicKey()
  {
    if (_key)
    {
      key_bssl::free_public_key(_key);
    }
  }

  bool PublicKey::parse(const char *pemKey)
  {
    return parse((const uint8_t *)pemKey, strlen_P(pemKey));
  }

  bool PublicKey::parse(const uint8_t *derKey, size_t derLen)
  {
    if (_key)
    {
      key_bssl::free_public_key(_key);
      _key = nullptr;
    }
    _key = key_bssl::read_public_key((const char *)derKey, derLen);
    return _key ? true : false;
  }

  bool PublicKey::isRSA() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_RSA)
    {
      return false;
    }
    return true;
  }

  bool PublicKey::isEC() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_EC)
    {
      return false;
    }
    return true;
  }

  const br_rsa_public_key *PublicKey::getRSA() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_RSA)
    {
      return nullptr;
    }
    return &_key->key.rsa;
  }

  const br_ec_public_key *PublicKey::getEC() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_EC)
    {
      return nullptr;
    }
    return &_key->key.ec;
  }

  // ----- Private Key -----

  PrivateKey::PrivateKey()
  {
    _key = nullptr;
  }

  PrivateKey::PrivateKey(const char *pemKey)
  {
    _key = nullptr;
    parse(pemKey);
  }

  PrivateKey::PrivateKey(const uint8_t *derKey, size_t derLen)
  {
    _key = nullptr;
    parse(derKey, derLen);
  }

  PrivateKey::PrivateKey(Stream &stream, size_t size)
  {
    _key = nullptr;
    auto buff = key_bssl::loadStream(stream, size);
    if (buff)
    {
      parse(buff, size);
      free(buff);
    }
  }

  PrivateKey::~PrivateKey()
  {
    if (_key)
    {
      key_bssl::free_private_key(_key);
    }
  }

  bool PrivateKey::parse(const char *pemKey)
  {
    return parse((const uint8_t *)pemKey, strlen_P(pemKey));
  }

  bool PrivateKey::parse(const uint8_t *derKey, size_t derLen)
  {
    if (_key)
    {
      key_bssl::free_private_key(_key);
      _key = nullptr;
    }
    _key = key_bssl::read_private_key((const char *)derKey, derLen);
    return _key ? true : false;
  }

  bool PrivateKey::isRSA() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_RSA)
    {
      return false;
    }
    return true;
  }

  bool PrivateKey::isEC() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_EC)
    {
      return false;
    }
    return true;
  }

  const br_rsa_private_key *PrivateKey::getRSA() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_RSA)
    {
      return nullptr;
    }
    return &_key->key.rsa;
  }

  const br_ec_private_key *PrivateKey::getEC() const
  {
    if (!_key || _key->key_type != BR_KEYTYPE_EC)
    {
      return nullptr;
    }
    return &_key->key.ec;
  }

  // ----- Certificate Lists -----

  X509List::X509List()
  {
    _count = 0;
    _cert = nullptr;
    _ta = nullptr;
  }

  X509List::X509List(const char *pemCert)
  {
    _count = 0;
    _cert = nullptr;
    _ta = nullptr;
    append(pemCert);
  }

  X509List::X509List(const uint8_t *derCert, size_t derLen)
  {
    _count = 0;
    _cert = nullptr;
    _ta = nullptr;
    append(derCert, derLen);
  }

  X509List::X509List(Stream &stream, size_t size)
  {
    _count = 0;
    _cert = nullptr;
    _ta = nullptr;
    auto buff = key_bssl::loadStream(stream, size);
    if (buff)
    {
      append(buff, size);
      free(buff);
    }
  }

  X509List::~X509List()
  {
    key_bssl::free_certificates(_cert, _count); // also frees cert
    for (size_t i = 0; i < _count; i++)
    {
      key_bssl::free_ta_contents(&_ta[i]);
    }
    free(_ta);
  }

  bool X509List::append(const char *pemCert)
  {
    return append((const uint8_t *)pemCert, strlen_P(pemCert));
  }

  bool X509List::append(const uint8_t *derCert, size_t derLen)
  {
    size_t numCerts;
    br_x509_certificate *newCerts = key_bssl::read_certificates((const char *)derCert, derLen, &numCerts);
    if (!newCerts)
    {
      return false;
    }

    // Add in the certificates
    br_x509_certificate *saveCert = _cert;
    _cert = (br_x509_certificate *)realloc(_cert, (numCerts + _count) * sizeof(br_x509_certificate));
    if (!_cert)
    {
      free(newCerts);
      _cert = saveCert;
      return false;
    }
    memcpy(&_cert[_count], newCerts, numCerts * sizeof(br_x509_certificate));
    free(newCerts);

    // Build TAs for each certificate
    br_x509_trust_anchor *saveTa = _ta;
    _ta = (br_x509_trust_anchor *)realloc(_ta, (numCerts + _count) * sizeof(br_x509_trust_anchor));
    if (!_ta)
    {
      _ta = saveTa;
      return false;
    }
    for (size_t i = 0; i < numCerts; i++)
    {
      br_x509_trust_anchor *newTa = key_bssl::certificate_to_trust_anchor(&_cert[_count + i]);
      if (newTa)
      {
        _ta[_count + i] = *newTa;
        free(newTa);
      }
      else
      {
        return false; // OOM
      }
    }
    _count += numCerts;

    return true;
  }

};

#endif

#endif