#include <cstdint>

#include "tensorflow/lite/experimental/microfrontend/lib/kiss_fft_common.h"

#define FIXED_POINT 16
namespace kissfft_fixed16 {
#include "third_party/kissfft/kiss_fft.c"
#include "third_party/kissfft/tools/kiss_fftr.c"
}  // namespace kissfft_fixed16
#undef FIXED_POINT
