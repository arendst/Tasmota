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

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <TensorFlowLite.h>

#include <cmath>

#include "magic_wand_model_data.h"
#include "rasterize_stroke.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#define BLE_SENSE_UUID(val) ("4798e0f2-" val "-4d68-af64-8a8f5258404e")

#undef MAGIC_WAND_DEBUG

namespace {

const int VERSION = 0x00000000;

constexpr int stroke_transmit_stride = 2;
constexpr int stroke_transmit_max_length = 160;
constexpr int stroke_max_length =
    stroke_transmit_max_length * stroke_transmit_stride;
constexpr int stroke_points_byte_count =
    2 * sizeof(int8_t) * stroke_transmit_max_length;
constexpr int stroke_struct_byte_count =
    (2 * sizeof(int32_t)) + stroke_points_byte_count;
constexpr int moving_sample_count = 50;

constexpr int raster_width = 32;
constexpr int raster_height = 32;
constexpr int raster_channels = 3;
constexpr int raster_byte_count =
    raster_height * raster_width * raster_channels;
int8_t raster_buffer[raster_byte_count];

BLEService service(BLE_SENSE_UUID("0000"));
BLECharacteristic strokeCharacteristic(BLE_SENSE_UUID("300a"), BLERead,
                                       stroke_struct_byte_count);

// String to calculate the local and device name
String name;

// A buffer holding the last 600 sets of 3-channel values from the
// accelerometer.
constexpr int acceleration_data_length = 600 * 3;
float acceleration_data[acceleration_data_length] = {};
// The next free entry in the data array.
int acceleration_data_index = 0;
float acceleration_sample_rate = 0.0f;

// A buffer holding the last 600 sets of 3-channel values from the gyroscope.
constexpr int gyroscope_data_length = 600 * 3;
float gyroscope_data[gyroscope_data_length] = {};
float orientation_data[gyroscope_data_length] = {};
// The next free entry in the data array.
int gyroscope_data_index = 0;
float gyroscope_sample_rate = 0.0f;

float current_velocity[3] = {0.0f, 0.0f, 0.0f};
float current_position[3] = {0.0f, 0.0f, 0.0f};
float current_gravity[3] = {0.0f, 0.0f, 0.0f};
float current_gyroscope_drift[3] = {0.0f, 0.0f, 0.0f};

int32_t stroke_length = 0;
uint8_t stroke_struct_buffer[stroke_struct_byte_count] = {};
int32_t* stroke_state = reinterpret_cast<int32_t*>(stroke_struct_buffer);
int32_t* stroke_transmit_length =
    reinterpret_cast<int32_t*>(stroke_struct_buffer + sizeof(int32_t));
int8_t* stroke_points =
    reinterpret_cast<int8_t*>(stroke_struct_buffer + (sizeof(int32_t) * 2));

enum {
  eWaiting = 0,
  eDrawing = 1,
  eDone = 2,
};

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 30 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;

constexpr int label_count = 10;
const char* labels[label_count] = {"0", "1", "2", "3", "4",
                                   "5", "6", "7", "8", "9"};

void SetupIMU() {
  // Make sure we are pulling measurements into a FIFO.
  // If you see an error on this line, make sure you have at least v1.1.0 of the
  // Arduino_LSM9DS1 library installed.
  IMU.setContinuousMode();

  acceleration_sample_rate = IMU.accelerationSampleRate();
  gyroscope_sample_rate = IMU.gyroscopeSampleRate();
#ifdef MAGIC_WAND_DEBUG
  float rate_frac;
  float rate_int;
  rate_frac = modf(acceleration_sample_rate, &rate_int);
  TF_LITE_REPORT_ERROR(error_reporter, "Acceleration sample rate %d.%d Hz",
                       static_cast<int32_t>(rate_int),
                       static_cast<int32_t>(rate_frac * 100));
  rate_frac = modf(gyroscope_sample_rate, &rate_int);
  TF_LITE_REPORT_ERROR(error_reporter, "Gyroscope sample rate %d.%d Hz",
                       static_cast<int32_t>(rate_int),
                       static_cast<int32_t>(rate_frac * 100));
#endif  // MAGIC_WAND_DEBUG
}

void ReadAccelerometerAndGyroscope(int* new_accelerometer_samples,
                                   int* new_gyroscope_samples) {
  // Keep track of whether we stored any new data
  *new_accelerometer_samples = 0;
  *new_gyroscope_samples = 0;
  // Loop through new samples and add to buffer
  while (IMU.accelerationAvailable()) {
    const int gyroscope_index = (gyroscope_data_index % gyroscope_data_length);
    gyroscope_data_index += 3;
    float* current_gyroscope_data = &gyroscope_data[gyroscope_index];
    // Read each sample, removing it from the device's FIFO buffer
    if (!IMU.readGyroscope(current_gyroscope_data[0], current_gyroscope_data[1],
                           current_gyroscope_data[2])) {
      TF_LITE_REPORT_ERROR(error_reporter, "Failed to read gyroscope data");
      break;
    }
    *new_gyroscope_samples += 1;

    const int acceleration_index =
        (acceleration_data_index % acceleration_data_length);
    acceleration_data_index += 3;
    float* current_acceleration_data = &acceleration_data[acceleration_index];
    // Read each sample, removing it from the device's FIFO buffer
    if (!IMU.readAcceleration(current_acceleration_data[0],
                              current_acceleration_data[1],
                              current_acceleration_data[2])) {
      TF_LITE_REPORT_ERROR(error_reporter, "Failed to read acceleration data");
      break;
    }
    *new_accelerometer_samples += 1;
  }
}

float VectorMagnitude(const float* vec) {
  const float x = vec[0];
  const float y = vec[1];
  const float z = vec[2];
  return sqrtf((x * x) + (y * y) + (z * z));
}

void EstimateGravityDirection(float* gravity) {
  int samples_to_average = 100;
  if (samples_to_average >= acceleration_data_index) {
    samples_to_average = acceleration_data_index;
  }

  const int start_index =
      ((acceleration_data_index +
        (acceleration_data_length - (3 * (samples_to_average + 1)))) %
       acceleration_data_length);

  float x_total = 0.0f;
  float y_total = 0.0f;
  float z_total = 0.0f;
  for (int i = 0; i < samples_to_average; ++i) {
    const int index = ((start_index + (i * 3)) % acceleration_data_length);
    const float* entry = &acceleration_data[index];
    const float x = entry[0];
    const float y = entry[1];
    const float z = entry[2];
    x_total += x;
    y_total += y;
    z_total += z;
  }
  gravity[0] = x_total / samples_to_average;
  gravity[1] = y_total / samples_to_average;
  gravity[2] = z_total / samples_to_average;
}

void UpdateVelocity(int new_samples, float* gravity) {
  const float gravity_x = gravity[0];
  const float gravity_y = gravity[1];
  const float gravity_z = gravity[2];

  const int start_index =
      ((acceleration_data_index +
        (acceleration_data_length - (3 * (new_samples + 1)))) %
       acceleration_data_length);

  const float friction_fudge = 0.98f;

  for (int i = 0; i < new_samples; ++i) {
    const int index = ((start_index + (i * 3)) % acceleration_data_length);
    const float* entry = &acceleration_data[index];
    const float ax = entry[0];
    const float ay = entry[1];
    const float az = entry[2];

    // Try to remove gravity from the raw acceleration values.
    const float ax_minus_gravity = ax - gravity_x;
    const float ay_minus_gravity = ay - gravity_y;
    const float az_minus_gravity = az - gravity_z;

    // Update velocity based on the normalized acceleration.
    current_velocity[0] += ax_minus_gravity;
    current_velocity[1] += ay_minus_gravity;
    current_velocity[2] += az_minus_gravity;

    // Dampen the velocity slightly with a fudge factor to stop it exploding.
    current_velocity[0] *= friction_fudge;
    current_velocity[1] *= friction_fudge;
    current_velocity[2] *= friction_fudge;

    // Update the position estimate based on the velocity.
    current_position[0] += current_velocity[0];
    current_position[1] += current_velocity[1];
    current_position[2] += current_velocity[2];
  }
}

void EstimateGyroscopeDrift(float* drift) {
  const bool isMoving = VectorMagnitude(current_velocity) > 0.1f;
  if (isMoving) {
    return;
  }

  int samples_to_average = 20;
  if (samples_to_average >= gyroscope_data_index) {
    samples_to_average = gyroscope_data_index;
  }

  const int start_index =
      ((gyroscope_data_index +
        (gyroscope_data_length - (3 * (samples_to_average + 1)))) %
       gyroscope_data_length);

  float x_total = 0.0f;
  float y_total = 0.0f;
  float z_total = 0.0f;
  for (int i = 0; i < samples_to_average; ++i) {
    const int index = ((start_index + (i * 3)) % gyroscope_data_length);
    const float* entry = &gyroscope_data[index];
    const float x = entry[0];
    const float y = entry[1];
    const float z = entry[2];
    x_total += x;
    y_total += y;
    z_total += z;
  }
  drift[0] = x_total / samples_to_average;
  drift[1] = y_total / samples_to_average;
  drift[2] = z_total / samples_to_average;
}

void UpdateOrientation(int new_samples, float* gravity, float* drift) {
  const float drift_x = drift[0];
  const float drift_y = drift[1];
  const float drift_z = drift[2];

  const int start_index =
      ((gyroscope_data_index + (gyroscope_data_length - (3 * new_samples))) %
       gyroscope_data_length);

  // The gyroscope values are in degrees-per-second, so to approximate
  // degrees in the integrated orientation, we need to divide each value
  // by the number of samples each second.
  const float recip_sample_rate = 1.0f / gyroscope_sample_rate;

  for (int i = 0; i < new_samples; ++i) {
    const int index = ((start_index + (i * 3)) % gyroscope_data_length);
    const float* entry = &gyroscope_data[index];
    const float dx = entry[0];
    const float dy = entry[1];
    const float dz = entry[2];

    // Try to remove sensor errors from the raw gyroscope values.
    const float dx_minus_drift = dx - drift_x;
    const float dy_minus_drift = dy - drift_y;
    const float dz_minus_drift = dz - drift_z;

    // Convert from degrees-per-second to appropriate units for this
    // time interval.
    const float dx_normalized = dx_minus_drift * recip_sample_rate;
    const float dy_normalized = dy_minus_drift * recip_sample_rate;
    const float dz_normalized = dz_minus_drift * recip_sample_rate;

    // Update orientation based on the gyroscope data.
    float* current_orientation = &orientation_data[index];
    const int previous_index =
        (index + (gyroscope_data_length - 3)) % gyroscope_data_length;
    const float* previous_orientation = &orientation_data[previous_index];
    current_orientation[0] = previous_orientation[0] + dx_normalized;
    current_orientation[1] = previous_orientation[1] + dy_normalized;
    current_orientation[2] = previous_orientation[2] + dz_normalized;
  }
}

bool IsMoving(int samples_before) {
  constexpr float moving_threshold = 9.0f;

  if ((gyroscope_data_index - samples_before) < moving_sample_count) {
    return false;
  }

  const int start_index =
      ((gyroscope_data_index + (gyroscope_data_length -
                                (3 * (moving_sample_count + samples_before)))) %
       gyroscope_data_length);

  float total = 0.0f;
  for (int i = 0; i < moving_sample_count; ++i) {
    const int index = ((start_index + (i * 3)) % gyroscope_data_length);
    float* current_orientation = &orientation_data[index];
    const int previous_index =
        (index + (gyroscope_data_length - 3)) % gyroscope_data_length;
    const float* previous_orientation = &orientation_data[previous_index];
    const float dx = current_orientation[0] - previous_orientation[0];
    const float dy = current_orientation[1] - previous_orientation[1];
    const float dz = current_orientation[2] - previous_orientation[2];
    const float mag_squared = (dx * dx) + (dy * dy) + (dz * dz);
    total += mag_squared;
  }
  const bool is_moving = (total > moving_threshold);
  return is_moving;
}

void UpdateStroke(int new_samples, bool* done_just_triggered) {
  constexpr int minimum_stroke_length = moving_sample_count + 10;
  constexpr float minimum_stroke_size = 0.2f;

  *done_just_triggered = false;

  for (int i = 0; i < new_samples; ++i) {
    const int current_head = (new_samples - (i + 1));
    const bool is_moving = IsMoving(current_head);
    const int32_t old_state = *stroke_state;
    if ((old_state == eWaiting) || (old_state == eDone)) {
      if (is_moving) {
        stroke_length = moving_sample_count;
        *stroke_state = eDrawing;
      }
    } else if (old_state == eDrawing) {
      if (!is_moving) {
        if (stroke_length > minimum_stroke_length) {
          *stroke_state = eDone;
        } else {
          stroke_length = 0;
          *stroke_state = eWaiting;
#ifdef MAGIC_WAND_DEBUG
          TF_LITE_REPORT_ERROR(error_reporter, "stroke length too small");
#endif  // MAGIC_WAND_DEBUG
        }
      }
    }

    const bool is_waiting = (*stroke_state == eWaiting);
    if (is_waiting) {
      continue;
    }

    stroke_length += 1;
    if (stroke_length > stroke_max_length) {
      stroke_length = stroke_max_length;
    }

    // Only recalculate the full stroke if it's needed.
    const bool draw_last_point =
        ((i == (new_samples - 1)) && (*stroke_state == eDrawing));
    *done_just_triggered = ((old_state != eDone) && (*stroke_state == eDone));
    if (!(*done_just_triggered || draw_last_point)) {
      continue;
    }

    const int start_index =
        ((gyroscope_data_index +
          (gyroscope_data_length - (3 * (stroke_length + current_head)))) %
         gyroscope_data_length);

    float x_total = 0.0f;
    float y_total = 0.0f;
    float z_total = 0.0f;
    for (int j = 0; j < stroke_length; ++j) {
      const int index = ((start_index + (j * 3)) % gyroscope_data_length);
      const float* entry = &orientation_data[index];
      x_total += entry[0];
      y_total += entry[1];
      z_total += entry[2];
    }

    const float y_mean = y_total / stroke_length;
    const float z_mean = z_total / stroke_length;
    constexpr float range = 45.0f;

    const float gy = current_gravity[1];
    const float gz = current_gravity[2];
    float gmag = sqrtf((gy * gy) + (gz * gz));
    if (gmag < 0.0001f) {
      gmag = 0.0001f;
    }
    const float ngy = gy / gmag;
    const float ngz = gz / gmag;

    const float xaxisz = -ngz;
    const float xaxisy = -ngy;

    const float yaxisz = -ngy;
    const float yaxisy = ngz;

    *stroke_transmit_length = stroke_length / stroke_transmit_stride;

    float x_min = 0;
    float y_min = 0;
    float x_max = 0;
    float y_max = 0;
    for (int j = 0; j < *stroke_transmit_length; ++j) {
      const int orientation_index =
          ((start_index + ((j * stroke_transmit_stride) * 3)) %
           gyroscope_data_length);
      const float* orientation_entry = &orientation_data[orientation_index];

      const float orientation_y = orientation_entry[1];
      const float orientation_z = orientation_entry[2];

      const float ny = (orientation_y - y_mean) / range;
      const float nz = (orientation_z - z_mean) / range;

      const float x_axis = (xaxisz * nz) + (xaxisy * ny);
      const float y_axis = (yaxisz * nz) + (yaxisy * ny);

      const int stroke_index = j * 2;
      int8_t* stroke_entry = &stroke_points[stroke_index];

      int32_t unchecked_x = static_cast<int32_t>(roundf(x_axis * 128.0f));
      int8_t stored_x;
      if (unchecked_x > 127) {
        stored_x = 127;
      } else if (unchecked_x < -128) {
        stored_x = -128;
      } else {
        stored_x = unchecked_x;
      }
      stroke_entry[0] = stored_x;

      int32_t unchecked_y = static_cast<int32_t>(roundf(y_axis * 128.0f));
      int8_t stored_y;
      if (unchecked_y > 127) {
        stored_y = 127;
      } else if (unchecked_y < -128) {
        stored_y = -128;
      } else {
        stored_y = unchecked_y;
      }
      stroke_entry[1] = stored_y;

      const bool is_first = (j == 0);
      if (is_first || (x_axis < x_min)) {
        x_min = x_axis;
      }
      if (is_first || (y_axis < y_min)) {
        y_min = y_axis;
      }
      if (is_first || (x_axis > x_max)) {
        x_max = x_axis;
      }
      if (is_first || (y_axis > y_max)) {
        y_max = y_axis;
      }
    }

    // If the stroke is too small, cancel it.
    if (*done_just_triggered) {
      const float x_range = (x_max - x_min);
      const float y_range = (y_max - y_min);
      if ((x_range < minimum_stroke_size) && (y_range < minimum_stroke_size)) {
        *done_just_triggered = false;
        *stroke_state = eWaiting;
        *stroke_transmit_length = 0;
        stroke_length = 0;
#ifdef MAGIC_WAND_DEBUG
        TF_LITE_REPORT_ERROR(error_reporter, "stroke too small");
#endif  // MAGIC_WAND_DEBUG
      }
    }
  }
}

}  // namespace

void setup() {
  tflite::InitializeTarget();  // setup serial port

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  static tflite::MicroErrorReporter micro_error_reporter;  // NOLINT
  error_reporter = &micro_error_reporter;

  TF_LITE_REPORT_ERROR(error_reporter, "Started");

  if (!IMU.begin()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Failed to initialized IMU!");
    while (true) {
      // NORETURN
    }
  }

  SetupIMU();

  if (!BLE.begin()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Failed to initialized BLE!");
    while (true) {
      // NORETURN
    }
  }

  String address = BLE.address();

  TF_LITE_REPORT_ERROR(error_reporter, "address = %s", address.c_str());

  address.toUpperCase();

  name = "BLESense-";
  name += address[address.length() - 5];
  name += address[address.length() - 4];
  name += address[address.length() - 2];
  name += address[address.length() - 1];

  TF_LITE_REPORT_ERROR(error_reporter, "name = %s", name.c_str());

  BLE.setLocalName(name.c_str());
  BLE.setDeviceName(name.c_str());
  BLE.setAdvertisedService(service);

  service.addCharacteristic(strokeCharacteristic);

  BLE.addService(service);

  BLE.advertise();

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_magic_wand_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  static tflite::MicroMutableOpResolver<4> micro_op_resolver;  // NOLINT
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddMean();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddSoftmax();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  interpreter->AllocateTensors();

  TfLiteTensor* model_input = interpreter->input(0);
  if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] != raster_height) ||
      (model_input->dims->data[2] != raster_width) ||
      (model_input->dims->data[3] != raster_channels) ||
      (model_input->type != kTfLiteInt8) ||
      (model_input->params.zero_point != -128) ||
      (model_input->params.scale != 1.0)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");
    return;
  }

  TfLiteTensor* model_output = interpreter->output(0);
  if ((model_output->dims->size != 2) || (model_output->dims->data[0] != 1) ||
      (model_output->dims->data[1] != label_count) ||
      (model_output->type != kTfLiteInt8)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad output tensor parameters in model");
    return;
  }
}

void loop() {
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  static bool was_connected_last = false;
  if (central && !was_connected_last) {
    // print the central's BT address:
    TF_LITE_REPORT_ERROR(error_reporter, "Connected to central: %s",
                         central.address().c_str());
  }
  was_connected_last = central;

  const bool data_available =
      IMU.accelerationAvailable() || IMU.gyroscopeAvailable();
  if (!data_available) {
    return;
  }

  int accelerometer_samples_read;
  int gyroscope_samples_read;
  ReadAccelerometerAndGyroscope(&accelerometer_samples_read,
                                &gyroscope_samples_read);

  bool done_just_triggered = false;
  if (gyroscope_samples_read > 0) {
    EstimateGyroscopeDrift(current_gyroscope_drift);
    UpdateOrientation(gyroscope_samples_read, current_gravity,
                      current_gyroscope_drift);
    UpdateStroke(gyroscope_samples_read, &done_just_triggered);
    if (central && central.connected()) {
      strokeCharacteristic.writeValue(stroke_struct_buffer,
                                      stroke_struct_byte_count);
    }
  }

  if (accelerometer_samples_read > 0) {
    EstimateGravityDirection(current_gravity);
    UpdateVelocity(accelerometer_samples_read, current_gravity);
  }

  if (done_just_triggered) {
    RasterizeStroke(stroke_points, *stroke_transmit_length, 0.6f, 0.6f,
                    raster_width, raster_height, raster_buffer);
    for (int y = 0; y < raster_height; ++y) {
      char line[raster_width + 1];
      for (int x = 0; x < raster_width; ++x) {
        const int8_t* pixel =
            &raster_buffer[(y * raster_width * raster_channels) +
                           (x * raster_channels)];
        const int8_t red = pixel[0];
        const int8_t green = pixel[1];
        const int8_t blue = pixel[2];
        char output;
        if ((red > -128) || (green > -128) || (blue > -128)) {
          output = '#';
        } else {
          output = '.';
        }
        line[x] = output;
      }
      line[raster_width] = 0;
      TF_LITE_REPORT_ERROR(error_reporter, line);
    }
#ifdef MAGIC_WAND_DEBUG
    TF_LITE_REPORT_ERROR(error_reporter, "tx len: %d", *stroke_transmit_length);
#endif  // MAGIC_WAND_DEBUG

    TfLiteTensor* model_input = interpreter->input(0);
    for (int i = 0; i < raster_byte_count; ++i) {
      model_input->data.int8[i] = raster_buffer[i];
    }

    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed");
      return;
    }

    TfLiteTensor* output = interpreter->output(0);

    int8_t max_score;
    int max_index;
    for (int i = 0; i < label_count; ++i) {
      const int8_t score = output->data.int8[i];
      if ((i == 0) || (score > max_score)) {
        max_score = score;
        max_index = i;
      }
    }
    float max_score_f =
        (max_score - output->params.zero_point) * output->params.scale;
    float max_score_int;
    float max_score_frac = modf(max_score_f * 100, &max_score_int);
    TF_LITE_REPORT_ERROR(error_reporter, "Found %s (%d.%d%%)",
                         labels[max_index], static_cast<int>(max_score_int),
                         static_cast<int>(max_score_frac * 100));
  }
}
