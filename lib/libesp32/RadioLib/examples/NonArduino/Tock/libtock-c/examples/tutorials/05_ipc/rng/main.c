#include <ipc.h>
#include <rng.h>
#include <tock.h>

// Random Number Generator Service
//
// This application waits for IPC notifications and returns random bytes
// in the buffer shared with it from the requesting application.
//
// Interface
// =========
//
// The first byte of the shared buffer is the number of random bytes the
// calling application wants. This service will fill the buffer with that
// many random bytes (starting with index 0 and overwriting the number
// requested). This service will then notify the client.
//
// If the buffer is not long enough to hold the number of requested bytes
// the service will stop processing the request and not notify the client.

static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
  uint8_t* buffer = (uint8_t*) buf;
  uint8_t* rng;

  if (len < 1) {
    // Need at least one byte for the number of bytes
    return;
  }

  uint8_t number_of_bytes = buffer[0];

  if (len < number_of_bytes) {
    // Buffer must be able to hold the random bytes requested.
    return;
  }

  rng = malloc(len);
  if (rng == NULL) {
    return;
  }

  // Fill the buffer with random bytes.
  int number_of_bytes_received;
  rng_sync(rng, len, number_of_bytes, &number_of_bytes_received);
  memcpy(buffer, rng, number_of_bytes_received);
  free(rng);

  // Signal done.
  ipc_notify_client(pid);
}

int main(void) {
  // Register the IPC service for this app. It is identified by the PACKAGE_NAME
  // of this app.
  ipc_register_service_callback("org.tockos.tutorials.ipc.rng", ipc_callback,
                                NULL);
  return 0;
}
