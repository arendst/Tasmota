# RadioLib as Tock application

[Tock](https://github.com/tock/tock) is an embedded operating system designed
for running multiple concurrent, mutually distrustful applications on Cortex-M
and RISC-V based embedded platforms.

RadioLib can be built as a Tock application using
[libtock-c](https://github.com/tock/libtock-c). This is an example of running
RadioLib as a Tock application.

This has been tested on the
[SparkFun LoRa Thing Plus - expLoRaBLE board] (https://github.com/tock/tock/tree/master/boards/apollo3/lora_things_plus)
but will work on any LoRa compatible Tock board (currently only the
expLoRaBLE board).

The RadioLib example can be built with:

```shell
$ git clone https://github.com/jgromes/RadioLib.git
$ cd RadioLib/examples/NonArduino/Tock/
$ ./build.sh
```

Then in the Tock repo you can flash the kernel and app with:

```shell
$ make flash; APP=RadioLib/examples/NonArduino/Tock/build/tock-sx1261.tbf make flash-app
```
