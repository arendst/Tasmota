# ESP-NN

The library contains optimised NN (Neural Network) functions for various Espressif chipsets.

* Supported platforms:
   * TensorFlow Lite Micro (TFLite Micro). Repo can be found [here](https://github.com/espressif/tflite-micro-esp-examples)

* Supported ESP chipsets include:
   * ESP32-S3 (Assembly versions optimised to benefit from vector instructions of ESP32-S3)
   * ESP32 (Generic optimisations)
   * ESP32-C3 (Generic optimisations)

## Performance

### Kernelwise performance for s8 versions:

  * Kernelwise performance on ESP32-S3 chip
    * Numbers are ticks taken for kernel to execute
    * Chip config: 240MHz, SPI: QPI 80MHz, Data cache: 64KB

    | Function        | ANSI C  | ESP32-S3 Opt  | Opt Ratio | Data info   | Memory    |
    | ----------------| --------|---------|---------|-------------|-----------|
    | elementwise_add | 320397  | 87119   | 3.68    | size = 1615 | External  |
    | elementwise_mul | 125958  | 44239   | 2.85    | size = 1615 | External  |
    | convolution     | 4663012 | 428675  | 10.88   | input(10,10), filter(64x1x1x64) | External |
    | convolution     | 301014  | 32433   | 9.28    | input(8,8), filter(16x1x1x16) | External |
    | convolution     | 2115418 | 1020923 | 2.07    | input(10,10), filter(64x3x3x3) | External |
    | depthwise conv  | 1190062 | 203278  | 5.85    | input (18, 18), pad(0,0), stride(1,1) filter: 1x3x3x16 | External |
    | depthwise conv  | 837072  | 182335  | 4.59    | input (12, 12), pad(1,1), stride(1,1)  filter: 8x5x5x4 | External |
    | max pool        | 485714  | 76747   | 6.33    | input(16,16), filter (1x3x3x16) | Internal |
    | avg pool        | 541462  | 160580  | 3.37    | input(16,16), filter (1x3x3x16) | Internal |
    | fully connected | 15853   | 9547    | 1.66    | len: 265, ch = 3 | Internal |
    | prelu (relu6)   | 19472   | 2734    | 7.12    | size, 1615  | Internal  |


## Configuration

  * To configure, please use `idf.py menuconfig` and under `ESP-NN` select `NN_OPTIMIZATIONS`
  * There are two options presented:
     * Optimized versions
     * ANSI C

  * Default selection is for `Optimized versions`. For ESP32-S3, assembly versions are automatically selected, whereas for other chipsets (viz., ESP32, ESP32-C3), generic optimisations are selected.
  * For debugging purposes, you may want to select `ANSI C` reference versions.


## Contributing

If you encounter an issue with ESP-NN, or wish to submit a feature request, please use the Issues section on the Github.

For general questions related to this library, please use the esp32.com forum.

## Copyrights and License

All original source code in this repository is Copyright (C) 2020-2021 Espressif Systems. This source code is licensed under the Apache License 2.0 as described in the file LICENSE.
