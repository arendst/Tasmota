#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <aes.h>
#include <console.h>

#define KEY_LEN  16
#define IV_LEN  16
#define DATA_LEN 256
#define DEST_LEN 256
#define CHECK_LEN 240

uint8_t key_buf[KEY_LEN] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
  0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
uint8_t iv_buf[IV_LEN] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
uint8_t data_buf[DATA_LEN] = \
  "A language empowering everyone to build reliable and efficient software.";
const uint8_t expected_ctr[CHECK_LEN] = \
{0xad, 0xac, 0xb3, 0x12, 0xf6, 0x07, 0x09, 0xd1, 0x95, 0xb7, 0x36, 0x10,
  0x87, 0xee, 0xce, 0x93, 0x53, 0x59, 0x15, 0x52, 0x00, 0x53, 0x34, 0x15,
  0x7d, 0xd2, 0x0e, 0xb8, 0x92, 0x35, 0x53, 0xda, 0x05, 0x0c, 0xa1, 0x0d,
  0x11, 0xe5, 0x53, 0x6f, 0xcc, 0xd1, 0xa4, 0x72, 0x76, 0xd8, 0x00, 0x21,
  0xc8, 0xfd, 0x57, 0xfb, 0xd0, 0x94, 0xfe, 0xa0, 0xbd, 0x69, 0x58, 0xbe,
  0x7b, 0x18, 0x8b, 0x8d, 0xdf, 0x6b, 0x33, 0x8f, 0x75, 0xf8, 0xf4, 0x20,
  0xf0, 0x68, 0x30, 0x97, 0x90, 0x4b, 0xa5, 0x02, 0x58, 0x99, 0x44, 0x5a,
  0x4d, 0xe1, 0x01, 0xf5, 0x13, 0xca, 0xd1, 0x98, 0x7d, 0x89, 0xe9, 0x1b,
  0x3b, 0xd9, 0xac, 0x79, 0x49, 0xde, 0x2b, 0xf9, 0x65, 0x69, 0xac, 0x38,
  0x43, 0xf8, 0x72, 0x42, 0x7d, 0x9a, 0xce, 0x80, 0x47, 0xc3, 0x53, 0x09,
  0x15, 0x5a, 0xb8, 0xa8, 0xf0, 0x85, 0x97, 0xb1, 0xb7, 0x9c, 0xb9, 0x26,
  0x40, 0xee, 0x48, 0x97, 0x95, 0xaf, 0x36, 0x15, 0x2a, 0xb3, 0xf6, 0x3b,
  0x7a, 0x42, 0x6f, 0x76, 0x8d, 0xb9, 0xe5, 0xe8, 0x1c, 0xb5, 0xc8, 0x4e,
  0x77, 0x4d, 0xcd, 0x2d, 0xad, 0xa0, 0x4d, 0xe7, 0x28, 0x2d, 0x83, 0xde,
  0x58, 0x6e, 0xd4, 0x85, 0x0a, 0x93, 0x8f, 0x15, 0x4d, 0x22, 0xb1, 0xe1,
  0xd2, 0xb1, 0x28, 0x94, 0xfa, 0xa1, 0xff, 0xa6, 0xd4, 0x8c, 0x60, 0x33,
  0x05, 0xda, 0x9e, 0xff, 0xc9, 0xe2, 0x7e, 0xe7, 0x76, 0xf7, 0x9d, 0xd6,
  0xb6, 0x0e, 0x98, 0xf1, 0x9e, 0x21, 0xce, 0x9a, 0x6f, 0x65, 0x2b, 0x13,
  0x02, 0xcb, 0xa1, 0xf6, 0x25, 0x79, 0x17, 0xf6, 0xe4, 0x16, 0x54, 0xe6,
  0xfb, 0x40, 0x2e, 0xb7, 0x12, 0x71, 0xca, 0xf7, 0xeb, 0x19, 0x1e, 0xd3};
const uint8_t expected_ccm[96] = \
{0x4a, 0x59, 0x3d, 0x26, 0x2a, 0xe0, 0xeb, 0xbb, 0xa4, 0x53, 0x88, 0x5a,
  0x0a, 0xdc, 0x62, 0xd3, 0x37, 0xae, 0x96, 0xfa, 0xa7, 0xf4, 0x5d, 0x46,
  0xb9, 0x9e, 0x65, 0xc9, 0x5a, 0xba, 0xe6, 0xd3, 0x81, 0xc1, 0xb9, 0x7b,
  0x01, 0x1e, 0xc6, 0xb4, 0xf6, 0x3b, 0x8a, 0x4c, 0x0b, 0x0b, 0xec, 0xa6,
  0x27, 0xa3, 0x9c, 0x0c, 0xeb, 0xb4, 0xfa, 0x0a, 0xbd, 0x2a, 0x9c, 0x70,
  0x5e, 0x88, 0xaa, 0xd8, 0x7b, 0xa7, 0x16, 0x0d, 0x9b, 0x3c, 0x68, 0xbd,
  0xde, 0xa8, 0xf0, 0xd0, 0x65, 0x25, 0x29, 0xf8, 0x88, 0x07, 0x42, 0x75,
  0xee, 0x72, 0xa3, 0x86, 0xc8, 0x2d, 0x37, 0x92, 0xa7, 0x52, 0xf2, 0x46};

uint8_t dest_buf[DEST_LEN];



static void aes_cb(int                          result,
                   __attribute__ ((unused)) int length,
                   __attribute__ ((unused)) int verified,
                   void*                        con) {
  if (result != 0) {
    printf("Crypto failure %d\n", result);
    exit(-1);
  }

  *(bool*)con = true;
}

static void aes_ctr_test(void) {
  int i;
  bool con = false;

  printf("[TEST] AES CTR Example Test\r\n");

  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_check_status());

  /*** Encryption ***/
  // Set AES128Ctr as the encryption algorithm
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_algorithm(0, true));

  printf("Loading in the key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_key_buffer(key_buf, KEY_LEN));
  printf("   done\r\n");

  printf("Loading in the IV buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_iv_buffer(iv_buf, IV_LEN));
  printf("   done\r\n");

  printf("Loading in the first half of the source buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_source_buffer(&data_buf[0], DATA_LEN / 2));
  printf("   done\r\n");

  printf("Setting up the first half of the dest buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_dest_buffer(&dest_buf[0], 128));
  printf("   done\r\n");

  printf("Setting up the callback...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_callback(aes_cb, &con));
  printf("   done\r\n");

  printf("Starting the initial run...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_setup());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Loading in second half of the source buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_source_buffer(&data_buf[DATA_LEN / 2], 128));
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_dest_buffer(&dest_buf[128], DEST_LEN - 128));
  printf("   done\r\n");

  printf("Running AES twice...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_crypt());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Finish AES operation...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_finish());
  printf("   done\r\n");

  for (i = 0; i < CHECK_LEN; i++) {
    if (dest_buf[i] != expected_ctr[i]) {
      printf("Comparison failure at %d\n", i);
      printf("Expected value: 0x%x\n", expected_ctr[i]);
      printf(" But got value: 0x%x\n", dest_buf[i]);
      exit(-1);
    }
  }

  printf("Encrypted text: '%s'\r\n", dest_buf);

  /*** Decryption ***/

  // Set AES128Ctr as the decryption algorithm
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_algorithm(0, true));

  printf("Loading in the decryption source buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_source_buffer(dest_buf, DEST_LEN));
  printf("   done\r\n");

  printf("Setting up the decryption dest buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_dest_buffer(dest_buf, DEST_LEN));
  printf("   done\r\n");

  printf("Running decryption...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_setup());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Finish AES operation...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_finish());
  printf("   done\r\n");

  printf("Original text: '%s'\r\n", dest_buf);
}

static void aes_ccm_test(void) {
  int i;
  bool con = false;

  printf("[TEST] AES CCM Example Test\r\n");

  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_check_status());

  /*** Encryption ***/
  // Set AES128CCM as the encryption algorithm
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_algorithm(3, true));
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_ccm_set_mic_len(8));

  printf("Loading in the key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_key_buffer(key_buf, KEY_LEN));
  printf("   done\r\n");

  printf("Loading in the nonce buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_nonce_buffer(iv_buf, IV_LEN));
  printf("   done\r\n");

  printf("Loading in the source buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_source_buffer(data_buf, 96));
  printf("   done\r\n");

  printf("Setting up the dest buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_dest_buffer(dest_buf, 96));
  printf("   done\r\n");

  printf("Setting up the callback...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_callback(aes_cb, &con));
  printf("   done\r\n");

  printf("Starting the initial run...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_setup());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Finish AES operation...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_finish());
  printf("   done\r\n");

  for (i = 0; i < 96; i++) {
    if (dest_buf[i] != expected_ccm[i]) {
      printf("Comparison failure at %d\n", i);
      printf("Expected value: 0x%x\n", expected_ccm[i]);
      printf(" But got value: 0x%x\n", dest_buf[i]);
      exit(-1);
    }
  }

  printf("Encrypted text: '%s'\r\n", dest_buf);

  /*** Decryption ***/

  // Set AES128CCM as the decryption algorithm
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_algorithm(3, false));

  printf("Loading in the decryption source buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_source_buffer(dest_buf, 96));
  printf("   done\r\n");

  printf("Setting up the decryption dest buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_set_dest_buffer(dest_buf, 96));
  printf("   done\r\n");

  printf("Running decryption...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_setup());
  printf("   done\r\n");

  yield_for(&con);
  con = false;

  printf("Finish AES operation...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, aes_finish());
  printf("   done\r\n");

  printf("Original text: '%s'\r\n", dest_buf);
}

int main(void) {
  memset(dest_buf, 0, DEST_LEN);
  aes_ctr_test();

  memset(dest_buf, 0, DEST_LEN);
  aes_ccm_test();

  return 0;
}
