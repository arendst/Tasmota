#include <stdio.h>

#include <console.h>
#include <hmac.h>

#define KEY_LEN  32
#define DATA_LEN 256
#define DEST_LEN 32

uint8_t key_buf[KEY_LEN]   = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xA1};
uint8_t data_buf[DATA_LEN] = "A language empowering everyone to build reliable and efficient software.";
uint8_t dest_buf[DEST_LEN];

static void hmac_cb(__attribute__((unused)) int    result,
                    __attribute__ ((unused)) int   digest,
                    __attribute__ ((unused)) int   unused,
                    __attribute__ ((unused)) void* userdata) {
  int i;

  printf("Operation complete\r\n");

  for (i = 0; i < DEST_LEN; i++) {
    printf("%d: 0x%x\r\n", i, dest_buf[i]);
  }

}

int main(void) {
  printf("[TEST] HMAC Example Test\r\n");

  // Set SHA256 as the algorithm
  hmac_set_algorithm(0);

  printf("Loading in the 0 key...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, hmac_set_key_buffer(key_buf, KEY_LEN));
  printf("   done\r\n");

  printf("Loading in the data buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, hmac_set_data_buffer(data_buf, DATA_LEN));
  printf("   done\r\n");

  printf("Setting up the dest buf...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, hmac_set_dest_buffer(dest_buf, DEST_LEN));
  printf("   done\r\n");

  printf("Setting up the callback...\r\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, hmac_set_callback(hmac_cb, NULL));
  printf("   done\r\n");

  printf("Running HMAC...\r\n");
  hmac_run();
  printf("   done\r\n");

  yield();

  return 0;
}
