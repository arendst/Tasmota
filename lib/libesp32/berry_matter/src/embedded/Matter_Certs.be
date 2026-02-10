#
# Matter_Certs.be - certificates and crypto values for Matter provisioning
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

#@ solidify:Matter_Certs,weak

class Matter_Certs

  static var PAI_Cert_FFF1 = bytes(
    "308201CB30820171A003020102020856AD8222AD945B64300A06082A8648CE3D"
    "04030230303118301606035504030C0F4D617474657220546573742050414131"
    "143012060A2B0601040182A27C02010C04464646313020170D32323032303530"
    "30303030305A180F39393939313233313233353935395A303D31253023060355"
    "04030C1C4D6174746572204465762050414920307846464631206E6F20504944"
    "31143012060A2B0601040182A27C02010C04464646313059301306072A8648CE"
    "3D020106082A8648CE3D03010703420004419A9315C2173E0C8C876D03CCFC94"
    "4852647F7FEC5E5082F4059928ECA894C594151309AC631E4CB03392AF684B0B"
    "AFB7E65B3B8162C2F52BF931B8E77AAA82A366306430120603551D130101FF04"
    "0830060101FF020100300E0603551D0F0101FF040403020106301D0603551D0E"
    "0416041463540E47F64B1C38D13884A462D16C195D8FFB3C301F0603551D2304"
    "18301680146AFD22771F511FECBF1641976710DCDC31A1717E300A06082A8648"
    "CE3D0403020348003045022100B2EF27F49AE9B50FB91EEAC94C4D0BDBB8D792"
    "9C6CB88FACE529368D12054C0C0220655DC92B86BD909882A6C62177B825D7D0"
    "5EDBE7C22F9FEA71220E7EA703F891"
  )

  # static var PAI_Pub_FFF1 = bytes(
  #   "04419A9315C2173E0C8C876D03CCFC944852647F7FEC5E5082F4059928ECA894"
  #   "C594151309AC631E4CB03392AF684B0BAFB7E65B3B8162C2F52BF931B8E77AAA"
  #   "82"
  # )

  # static var PAI_Priv_FFF1 = bytes(
  #   "C1AB5FE284B4C18940A53161F806944050A1697B71761D385337A8A3CD095C34"
  # )

  #-
  Certificate:
      Data:
          Version: 3 (0x2)
          Serial Number:
              56:ad:82:22:ad:94:5b:64
          Signature Algorithm: ecdsa-with-SHA256
          Issuer: CN = Matter Test PAA, 1.3.6.1.4.1.37244.2.1 = FFF1
          Validity
              Not Before: Feb  5 00:00:00 2022 GMT
              Not After : Dec 31 23:59:59 9999 GMT
          Subject: CN = Matter Dev PAI 0xFFF1 no PID, 1.3.6.1.4.1.37244.2.1 = FFF1
          Subject Public Key Info:
              Public Key Algorithm: id-ecPublicKey
                  Public-Key: (256 bit)
                  pub:
                      04:41:9a:93:15:c2:17:3e:0c:8c:87:6d:03:cc:fc:
                      94:48:52:64:7f:7f:ec:5e:50:82:f4:05:99:28:ec:
                      a8:94:c5:94:15:13:09:ac:63:1e:4c:b0:33:92:af:
                      68:4b:0b:af:b7:e6:5b:3b:81:62:c2:f5:2b:f9:31:
                      b8:e7:7a:aa:82
                  ASN1 OID: prime256v1
                  NIST CURVE: P-256
          X509v3 extensions:
              X509v3 Basic Constraints: critical
                  CA:TRUE, pathlen:0
              X509v3 Key Usage: critical
                  Certificate Sign, CRL Sign
              X509v3 Subject Key Identifier:
                  63:54:0E:47:F6:4B:1C:38:D1:38:84:A4:62:D1:6C:19:5D:8F:FB:3C
              X509v3 Authority Key Identifier:
                  keyid:6A:FD:22:77:1F:51:1F:EC:BF:16:41:97:67:10:DC:DC:31:A1:71:7E

      Signature Algorithm: ecdsa-with-SHA256
          30:45:02:21:00:b2:ef:27:f4:9a:e9:b5:0f:b9:1e:ea:c9:4c:
          4d:0b:db:b8:d7:92:9c:6c:b8:8f:ac:e5:29:36:8d:12:05:4c:
          0c:02:20:65:5d:c9:2b:86:bd:90:98:82:a6:c6:21:77:b8:25:
          d7:d0:5e:db:e7:c2:2f:9f:ea:71:22:0e:7e:a7:03:f8:91
  -#

  #-
  ASN.1

  SEQUENCE (3 elem)
    SEQUENCE (8 elem)
      [0] (1 elem)
        INTEGER 2
      INTEGER (63 bit) 6245791343685426020
      SEQUENCE (1 elem)
        OBJECT IDENTIFIER 1.2.840.10045.4.3.2 ecdsaWithSHA256 (ANSI X9.62 ECDSA algorithm with SHA256)
      SEQUENCE (2 elem)
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.4.3 commonName (X.520 DN component)
            UTF8String Matter Test PAA
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 1.3.6.1.4.1.37244.2.1
            UTF8String FFF1
      SEQUENCE (2 elem)
        UTCTime 2022-02-05 00:00:00 UTC
        GeneralizedTime 9999-12-31 23:59:59 UTC
      SEQUENCE (2 elem)
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.4.3 commonName (X.520 DN component)
            UTF8String Matter Dev PAI 0xFFF1 no PID
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 1.3.6.1.4.1.37244.2.1
            UTF8String FFF1
      SEQUENCE (2 elem)
        SEQUENCE (2 elem)
          OBJECT IDENTIFIER 1.2.840.10045.2.1 ecPublicKey (ANSI X9.62 public key type)
          OBJECT IDENTIFIER 1.2.840.10045.3.1.7 prime256v1 (ANSI X9.62 named elliptic curve)
        BIT STRING (520 bit) 0000010001000001100110101001001100010101110000100001011100111110000011…
      [3] (1 elem)
        SEQUENCE (4 elem)
          SEQUENCE (3 elem)
            OBJECT IDENTIFIER 2.5.29.19 basicConstraints (X.509 extension)
            BOOLEAN true
            OCTET STRING (8 byte) 30060101FF020100
              SEQUENCE (2 elem)
                BOOLEAN true
                INTEGER 0
          SEQUENCE (3 elem)
            OBJECT IDENTIFIER 2.5.29.15 keyUsage (X.509 extension)
            BOOLEAN true
            OCTET STRING (4 byte) 03020106
              BIT STRING (7 bit) 0000011
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.29.14 subjectKeyIdentifier (X.509 extension)
            OCTET STRING (22 byte) 041463540E47F64B1C38D13884A462D16C195D8FFB3C
              OCTET STRING (20 byte) 63540E47F64B1C38D13884A462D16C195D8FFB3C
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.29.35 authorityKeyIdentifier (X.509 extension)
            OCTET STRING (24 byte) 301680146AFD22771F511FECBF1641976710DCDC31A1717E
              SEQUENCE (1 elem)
                [0] (20 byte) 6AFD22771F511FECBF1641976710DCDC31A1717E
    SEQUENCE (1 elem)
  Offset: 377
  Length: 2+10
  (constructed)
  Value:
  (1 elem)
      OBJECT IDENTIFIER 1.2.840.10045.4.3.2 ecdsaWithSHA256 (ANSI X9.62 ECDSA algorithm with SHA256)
    BIT STRING (568 bit) 0011000001000101000000100010000100000000101100101110111100100111111101…
      SEQUENCE (2 elem)
        INTEGER (256 bit) 8093423925895219711386938045632983408737245943652937189339272815747009…
        INTEGER (255 bit) 4584930290841753608947726397552150715869097421278636946745345682393985…
  -#
  static var DAC_Cert_FFF1_8000 = bytes(
    "308201E93082018EA0030201020208238A647BBC4C30DD300A06082A8648CE3D"
    "040302303D3125302306035504030C1C4D617474657220446576205041492030"
    "7846464631206E6F2050494431143012060A2B0601040182A27C02010C044646"
    "46313020170D3232303230353030303030305A180F3939393931323331323335"
    "3935395A30533125302306035504030C1C4D6174746572204465762044414320"
    "3078464646312F30783830303031143012060A2B0601040182A27C02010C0446"
    "46463131143012060A2B0601040182A27C02020C04383030303059301306072A"
    "8648CE3D020106082A8648CE3D0301070342000462DB16BADEA326A6DB84814A"
    "063FC6C7E9E2B101B721648EBA4E5AC840F5DA301EE618124EB4180E2FC3A204"
    "7A564BA9BCFA0BF71F60CE8930F1E7F66EC8D728A360305E300C0603551D1301"
    "01FF04023000300E0603551D0F0101FF040403020780301D0603551D0E041604"
    "14BCF7B007497063606A26BE4E087C595687745A5A301F0603551D2304183016"
    "801463540E47F64B1C38D13884A462D16C195D8FFB3C300A06082A8648CE3D04"
    "03020349003046022100979711EC9E7618CE41801132C250DB707674630CD58C"
    "12C6E2315F08D01EE178022100ECFC1306BD2A133D122A278610EA3DCA47F05C"
    "7A8B805FA71C6FF41538A864C8"
  )

  #-
  Decoded cert: openssl x509 -inform der -in DAC_Cert_FFF1_8000.der -text -noout

  Certificate:
      Data:
          Version: 3 (0x2)
          Serial Number:
              23:8a:64:7b:bc:4c:30:dd
          Signature Algorithm: ecdsa-with-SHA256
          Issuer: CN = Matter Dev PAI 0xFFF1 no PID, 1.3.6.1.4.1.37244.2.1 = FFF1
          Validity
              Not Before: Feb  5 00:00:00 2022 GMT
              Not After : Dec 31 23:59:59 9999 GMT
          Subject: CN = Matter Dev DAC 0xFFF1/0x8000, 1.3.6.1.4.1.37244.2.1 = FFF1, 1.3.6.1.4.1.37244.2.2 = 8000
          Subject Public Key Info:
              Public Key Algorithm: id-ecPublicKey
                  Public-Key: (256 bit)
                  pub:
                      04:62:db:16:ba:de:a3:26:a6:db:84:81:4a:06:3f:
                      c6:c7:e9:e2:b1:01:b7:21:64:8e:ba:4e:5a:c8:40:
                      f5:da:30:1e:e6:18:12:4e:b4:18:0e:2f:c3:a2:04:
                      7a:56:4b:a9:bc:fa:0b:f7:1f:60:ce:89:30:f1:e7:
                      f6:6e:c8:d7:28
                  ASN1 OID: prime256v1
                  NIST CURVE: P-256
          X509v3 extensions:
              X509v3 Basic Constraints: critical
                  CA:FALSE
              X509v3 Key Usage: critical
                  Digital Signature
              X509v3 Subject Key Identifier:
                  BC:F7:B0:07:49:70:63:60:6A:26:BE:4E:08:7C:59:56:87:74:5A:5A
              X509v3 Authority Key Identifier:
                  keyid:63:54:0E:47:F6:4B:1C:38:D1:38:84:A4:62:D1:6C:19:5D:8F:FB:3C

      Signature Algorithm: ecdsa-with-SHA256
          30:46:02:21:00:97:97:11:ec:9e:76:18:ce:41:80:11:32:c2:
          50:db:70:76:74:63:0c:d5:8c:12:c6:e2:31:5f:08:d0:1e:e1:
          78:02:21:00:ec:fc:13:06:bd:2a:13:3d:12:2a:27:86:10:ea:
          3d:ca:47:f0:5c:7a:8b:80:5f:a7:1c:6f:f4:15:38:a8:64:c8

  -#

  #- Decoded ASN.1

  SEQUENCE (3 elem)
    SEQUENCE (8 elem)
      [0] (1 elem)
        INTEGER 2
      INTEGER (62 bit) 2560969820716413149
      SEQUENCE (1 elem)
        OBJECT IDENTIFIER 1.2.840.10045.4.3.2 ecdsaWithSHA256 (ANSI X9.62 ECDSA algorithm with SHA256)
      SEQUENCE (2 elem)
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.4.3 commonName (X.520 DN component)
            UTF8String Matter Dev PAI 0xFFF1 no PID
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 1.3.6.1.4.1.37244.2.1
            UTF8String FFF1
      SEQUENCE (2 elem)
        UTCTime 2022-02-05 00:00:00 UTC
        GeneralizedTime 9999-12-31 23:59:59 UTC
      SEQUENCE (3 elem)
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.4.3 commonName (X.520 DN component)
            UTF8String Matter Dev DAC 0xFFF1/0x8000
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 1.3.6.1.4.1.37244.2.1
            UTF8String FFF1
        SET (1 elem)
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 1.3.6.1.4.1.37244.2.2
            UTF8String 8000
      SEQUENCE (2 elem)
        SEQUENCE (2 elem)
          OBJECT IDENTIFIER 1.2.840.10045.2.1 ecPublicKey (ANSI X9.62 public key type)
          OBJECT IDENTIFIER 1.2.840.10045.3.1.7 prime256v1 (ANSI X9.62 named elliptic curve)
        BIT STRING (520 bit) 0000010001100010110110110001011010111010110111101010001100100110101001…
      [3] (1 elem)
        SEQUENCE (4 elem)
          SEQUENCE (3 elem)
            OBJECT IDENTIFIER 2.5.29.19 basicConstraints (X.509 extension)
            BOOLEAN true
            OCTET STRING (2 byte) 3000
              SEQUENCE (0 elem)
          SEQUENCE (3 elem)
            OBJECT IDENTIFIER 2.5.29.15 keyUsage (X.509 extension)
            BOOLEAN true
            OCTET STRING (4 byte) 03020780
              BIT STRING (1 bit) 1
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.29.14 subjectKeyIdentifier (X.509 extension)
            OCTET STRING (22 byte) 0414BCF7B007497063606A26BE4E087C595687745A5A
              OCTET STRING (20 byte) BCF7B007497063606A26BE4E087C595687745A5A
          SEQUENCE (2 elem)
            OBJECT IDENTIFIER 2.5.29.35 authorityKeyIdentifier (X.509 extension)
            OCTET STRING (24 byte) 3016801463540E47F64B1C38D13884A462D16C195D8FFB3C
              SEQUENCE (1 elem)
                [0] (20 byte) 63540E47F64B1C38D13884A462D16C195D8FFB3C
    SEQUENCE (1 elem)
      OBJECT IDENTIFIER 1.2.840.10045.4.3.2 ecdsaWithSHA256 (ANSI X9.62 ECDSA algorithm with SHA256)
    BIT STRING (576 bit) 0011000001000110000000100010000100000000100101111001011100010001111011…
      SEQUENCE (2 elem)
        INTEGER (256 bit) 6856615775177799389454949658700013485116050405241433595512074226650010…
        INTEGER (256 bit) 1071912090408366213691559168363210336766445086127676108978024779974417…
  -#
  # static var DAC_Pub_FFF1_8000 = bytes(
  #   "0462DB16BADEA326A6DB84814A063FC6C7E9E2B101B721648EBA4E5AC840F5DA"
  #   "301EE618124EB4180E2FC3A2047A564BA9BCFA0BF71F60CE8930F1E7F66EC8D7"
  #   "28"
  # )
  static var DAC_Priv_FFF1_8000 = bytes(
    "CCCF9DC7050EF5D90BE45707B90E1F875D59BE1FA942E8ED2E427203F6C2EE3D"
  )

  # CertificationDeclaration */
  # -> format_version = 1
  # -> vendor_id = 0xFFF1
  # -> product_id_array = [ 0x8000, 0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8007, 0x8008, 0x8009, 0x800A, 0x800B,
  #  0x800C, 0x800D, 0x800E, 0x800F, 0x8010, 0x8011, 0x8012, 0x8013, 0x8014, 0x8015, 0x8016, 0x8017, 0x8018, 0x8019, 0x801A,
  #  0x801B, 0x801C, 0x801D, 0x801E, 0x801F, 0x8020, 0x8021, 0x8022, 0x8023, 0x8024, 0x8025, 0x8026, 0x8027, 0x8028, 0x8029,
  #  0x802A, 0x802B, 0x802C, 0x802D, 0x802E, 0x802F, 0x8030, 0x8031, 0x8032, 0x8033, 0x8034, 0x8035, 0x8036, 0x8037, 0x8038,
  #  0x8039, 0x803A, 0x803B, 0x803C, 0x803D, 0x803E, 0x803F, 0x8040, 0x8041, 0x8042, 0x8043, 0x8044, 0x8045, 0x8046, 0x8047,
  #  0x8048, 0x8049, 0x804A, 0x804B, 0x804C, 0x804D, 0x804E, 0x804F, 0x8050, 0x8051, 0x8052, 0x8053, 0x8054, 0x8055, 0x8056,
  #  0x8057, 0x8058, 0x8059, 0x805A, 0x805B, 0x805C, 0x805D, 0x805E, 0x805F, 0x8060, 0x8061, 0x8062, 0x8063 ]
  # -> device_type_id = 0x0016
  # -> certificate_id = "CSA00000SWC00000-00"
  # -> security_level = 0
  # -> security_information = 0
  # -> version_number = 1
  # -> certification_type = 0
  # -> dac_origin_vendor_id is not present
  # -> dac_origin_product_id is not present
  static var CD_FFF1_8000 = bytes(
    "3082021706092A864886F70D010702A082020830820204020103310D300B0609"
    "6086480165030402013082017006092A864886F70D010701A08201610482015D"
    "152400012501F1FF360205008005018005028005038005048005058005068005"
    "0780050880050980050A80050B80050C80050D80050E80050F80051080051180"
    "051280051380051480051580051680051780051880051980051A80051B80051C"
    "80051D80051E80051F8005208005218005228005238005248005258005268005"
    "2780052880052980052A80052B80052C80052D80052E80052F80053080053180"
    "053280053380053480053580053680053780053880053980053A80053B80053C"
    "80053D80053E80053F8005408005418005428005438005448005458005468005"
    "4780054880054980054A80054B80054C80054D80054E80054F80055080055180"
    "055280055380055480055580055680055780055880055980055A80055B80055C"
    "80055D80055E80055F80056080056180056280056380182403162C0413435341"
    "303030303053574330303030302D303024050024060024070124080018317C30"
    "7A0201038014FE343F959947763B61EE4539131338494FE67D8E300B06096086"
    "48016503040201300A06082A8648CE3D0403020446304402204A12F8D42F9023"
    "5C05A77121CBEBAE15D590146558E9C9B47A1A38F7A36A7DC5022020A4742897"
    "C30AEDA0A56B36E14EBBC85BBDB74493F993581EB0444ED6CA940B"
  )

end
matter.Certs = Matter_Certs

#-

assert(matter.Certs.PAI_Cert_FFF1 == matter.PAI_Cert_FFF1())
assert(matter.Certs.PAI_Pub_FFF1 == matter.PAI_Pub_FFF1())
assert(matter.Certs.PAI_Priv_FFF1 == matter.PAI_Priv_FFF1())
assert(matter.Certs.DAC_Priv_FFF1_8000 == matter.DAC_Priv_FFF1_8000())
assert(matter.Certs.DAC_Cert_FFF1_8000 == matter.DAC_Cert_FFF1_8000())
assert(matter.Certs.DAC_Pub_FFF1_8000 == matter.DAC_Pub_FFF1_8000())
assert(matter.Certs.CD_FFF1_8000 == matter.CD_FFF1_8000())

-#