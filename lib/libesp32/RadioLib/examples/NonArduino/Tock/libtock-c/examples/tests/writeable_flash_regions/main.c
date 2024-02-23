#include <stdio.h>

#include <tock.h>

int main (void) {
  int num_regions = tock_app_number_writeable_flash_regions();
  if (num_regions == 0) {
    printf("No writeable flash regions defined in this app's header.\n");
  } else {
    for (int i = 0; i < num_regions; i++) {
      void* start = tock_app_writeable_flash_region_begins_at(i);
      void* end   = tock_app_writeable_flash_region_ends_at(i);
      printf("Writeable flash region %i starts at %p and ends at %p\n", i, start, end);
    }
  }
}
