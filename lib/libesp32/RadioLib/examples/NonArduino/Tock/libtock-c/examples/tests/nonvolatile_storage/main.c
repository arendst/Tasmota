#include <stdint.h>
#include <stdio.h>

#include <internal/nonvolatile_storage.h>

static int test_all(void);
static int test(uint8_t *readbuf, uint8_t *writebuf, size_t size, size_t offset, size_t len);

static bool done = false;

static void read_done(int                            length,
                      __attribute__ ((unused)) int   arg1,
                      __attribute__ ((unused)) int   arg2,
                      __attribute__ ((unused)) void* ud) {
  printf("\tFinished read! %i\n", length);
  done = true;
}

static void write_done(int                            length,
                       __attribute__ ((unused)) int   arg1,
                       __attribute__ ((unused)) int   arg2,
                       __attribute__ ((unused)) void* ud) {
  printf("\tFinished write! %i\n", length);
  done = true;
}

int main (void) {
  printf("[Nonvolatile Storage] Test App\n");

  int r = test_all();
  if (r == 0) {
    printf("All tests succeeded\n");
  } else {
    printf("Failed with code %d\n", r);
  }

  return r;
}

static int test_all(void) {
  int num_bytes;
  nonvolatile_storage_internal_get_number_bytes(&num_bytes);
  printf("Have %i bytes of nonvolatile storage\n", num_bytes);

  int r;
  uint8_t readbuf[512];
  uint8_t writebuf[512];

  if ((r = test(readbuf, writebuf, 256, 0,  14)) != 0) return r;
  if ((r = test(readbuf, writebuf, 256, 20, 14)) != 0) return r;
  if ((r = test(readbuf, writebuf, 512, 0, 512)) != 0) return r;

  printf("Write to end of region (offset %d)\n", num_bytes - 512);
  if ((r = test(readbuf, writebuf, 512, num_bytes - 512, 500)) != 0) return r;

  printf("Write beyond end region, should fail (offset %d)\n", num_bytes);
  if ((r = test(readbuf, writebuf, 512, num_bytes, 501)) == 0) return -1;

  return 0;
}

static int test(uint8_t *readbuf, uint8_t *writebuf, size_t size, size_t offset, size_t len) {
  int ret;

  printf("Test with size %d ...\n", size);

  ret = nonvolatile_storage_internal_read_buffer(readbuf, size);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR setting read buffer\n");
    return ret;
  }

  ret = nonvolatile_storage_internal_write_buffer(writebuf, size);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR setting write buffer\n");
    return ret;
  }

  // Setup callbacks
  ret = nonvolatile_storage_internal_read_done_subscribe(read_done, NULL);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR setting read done callback\n");
    return ret;
  }

  ret = nonvolatile_storage_internal_write_done_subscribe(write_done, NULL);
  if (ret != RETURNCODE_SUCCESS) {
    printf("\tERROR setting write done callback\n");
    return ret;
  }

  for (size_t i = 0; i < len; i++) {
    writebuf[i] = i;
  }

  done = false;
  ret  = nonvolatile_storage_internal_write(offset, len);
  if (ret != 0) {
    printf("\tERROR calling write\n");
    return ret;
  }
  yield_for(&done);

  done = false;
  ret  = nonvolatile_storage_internal_read(offset, len);
  if (ret != 0) {
    printf("\tERROR calling read\n");
    return ret;
  }
  yield_for(&done);

  for (size_t i = 0; i < len; i++) {
    if (readbuf[i] != writebuf[i]) {
      printf("\tInconsistency between data written and read at index %u\n", i);
      return -1;
    }
  }

  return 0;
}
