/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "rasterize_stroke.h"

namespace {
constexpr int kFixedPoint = 4096;

int32_t MulFP(int32_t a, int32_t b) { return (a * b) / kFixedPoint; }

int32_t DivFP(int32_t a, int32_t b) {
  if (b == 0) {
    b = 1;
  }
  return (a * kFixedPoint) / b;
}

int32_t FloatToFP(float a) { return static_cast<int32_t>(a * kFixedPoint); }

int32_t NormToCoordFP(int32_t a_fp, int32_t range_fp, int32_t half_size_fp) {
  const int32_t norm_fp = DivFP(a_fp, range_fp);
  return MulFP(norm_fp, half_size_fp) + half_size_fp;
}

int32_t RoundFPToInt(int32_t a) {
  return static_cast<int32_t>((a + (kFixedPoint / 2)) / kFixedPoint);
}

int32_t Gate(int32_t a, int32_t min, int32_t max) {
  if (a < min) {
    return min;
  } else if (a > max) {
    return max;
  } else {
    return a;
  }
}

int32_t Abs(int32_t a) {
  if (a > 0) {
    return a;
  } else {
    return -a;
  }
}

}  // namespace

void RasterizeStroke(int8_t* stroke_points, int stroke_points_count,
                     float x_range, float y_range, int width, int height,
                     int8_t* out_buffer) {
  constexpr int num_channels = 3;
  const int buffer_byte_count = height * width * num_channels;

  for (int i = 0; i < buffer_byte_count; ++i) {
    out_buffer[i] = -128;
  }

  const int32_t width_fp = width * kFixedPoint;
  const int32_t height_fp = height * kFixedPoint;
  const int32_t half_width_fp = width_fp / 2;
  const int32_t half_height_fp = height_fp / 2;
  const int32_t x_range_fp = FloatToFP(x_range);
  const int32_t y_range_fp = FloatToFP(y_range);

  const int t_inc_fp = kFixedPoint / stroke_points_count;

  const int one_half_fp = (kFixedPoint / 2);

  for (int point_index = 0; point_index < (stroke_points_count - 1);
       ++point_index) {
    const int8_t* start_point = &stroke_points[point_index * 2];
    const int32_t start_point_x_fp = (start_point[0] * kFixedPoint) / 128;
    const int32_t start_point_y_fp = (start_point[1] * kFixedPoint) / 128;

    const int8_t* end_point = &stroke_points[(point_index + 1) * 2];
    const int32_t end_point_x_fp = (end_point[0] * kFixedPoint) / 128;
    const int32_t end_point_y_fp = (end_point[1] * kFixedPoint) / 128;

    const int32_t start_x_fp =
        NormToCoordFP(start_point_x_fp, x_range_fp, half_width_fp);
    const int32_t start_y_fp =
        NormToCoordFP(-start_point_y_fp, y_range_fp, half_height_fp);
    const int32_t end_x_fp =
        NormToCoordFP(end_point_x_fp, x_range_fp, half_width_fp);
    const int32_t end_y_fp =
        NormToCoordFP(-end_point_y_fp, y_range_fp, half_height_fp);
    const int32_t delta_x_fp = end_x_fp - start_x_fp;
    const int32_t delta_y_fp = end_y_fp - start_y_fp;

    const int32_t t_fp = point_index * t_inc_fp;
    int32_t red_i32;
    int32_t green_i32;
    int32_t blue_i32;
    if (t_fp < one_half_fp) {
      const int32_t local_t_fp = DivFP(t_fp, one_half_fp);
      const int32_t one_minus_t_fp = kFixedPoint - local_t_fp;
      red_i32 = RoundFPToInt(one_minus_t_fp * 255) - 128;
      green_i32 = RoundFPToInt(local_t_fp * 255) - 128;
      blue_i32 = -128;
    } else {
      const int32_t local_t_fp = DivFP(t_fp - one_half_fp, one_half_fp);
      const int32_t one_minus_t_fp = kFixedPoint - local_t_fp;
      red_i32 = -128;
      green_i32 = RoundFPToInt(one_minus_t_fp * 255) - 128;
      blue_i32 = RoundFPToInt(local_t_fp * 255) - 128;
    }
    const int8_t red_i8 = Gate(red_i32, -128, 127);
    const int8_t green_i8 = Gate(green_i32, -128, 127);
    const int8_t blue_i8 = Gate(blue_i32, -128, 127);

    int line_length;
    int32_t x_inc_fp;
    int32_t y_inc_fp;
    if (Abs(delta_x_fp) > Abs(delta_y_fp)) {
      line_length = Abs(RoundFPToInt(delta_x_fp));
      if (delta_x_fp > 0) {
        x_inc_fp = 1 * kFixedPoint;
        y_inc_fp = DivFP(delta_y_fp, delta_x_fp);
      } else {
        x_inc_fp = -1 * kFixedPoint;
        y_inc_fp = -DivFP(delta_y_fp, delta_x_fp);
      }
    } else {
      line_length = Abs(RoundFPToInt(delta_y_fp));
      if (delta_y_fp > 0) {
        y_inc_fp = 1 * kFixedPoint;
        x_inc_fp = DivFP(delta_x_fp, delta_y_fp);
      } else {
        y_inc_fp = -1 * kFixedPoint;
        x_inc_fp = -DivFP(delta_x_fp, delta_y_fp);
      }
    }
    for (int i = 0; i < (line_length + 1); ++i) {
      const int32_t x_fp = start_x_fp + (i * x_inc_fp);
      const int32_t y_fp = start_y_fp + (i * y_inc_fp);
      const int x = RoundFPToInt(x_fp);
      const int y = RoundFPToInt(y_fp);
      if ((x < 0) or (x >= width) or (y < 0) or (y >= height)) {
        continue;
      }
      const int buffer_index = (y * width * num_channels) + (x * num_channels);
      out_buffer[buffer_index + 0] = red_i8;
      out_buffer[buffer_index + 1] = green_i8;
      out_buffer[buffer_index + 2] = blue_i8;
    }
  }
}
