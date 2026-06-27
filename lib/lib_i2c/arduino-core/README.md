# Sensirion Arduino Core Library

This library provides SHDLC and I2C protocol implementations for Sensirion
sensors. There shouldn't be a reason to use it directly, but is required by the
sensor driver libraries provided here:

- [SCD4x](https://github.com/Sensirion/arduino-i2c-scd4x)
- [SVM40-I2C](https://github.com/Sensirion/arduino-i2c-svm40)
- [SVM40-UART](https://github.com/Sensirion/arduino-uart-svm40)
- [SFA3x-I2C](https://github.com/Sensirion/arduino-i2c-sfa3x)
- [SFA3x-UART](https://github.com/Sensirion/arduino-uart-sfa3x)

# More Drivers

Not looking for Arduino drivers? Check out our other drivers here:

- [Embedded](https://github.com/Sensirion/info#repositories)
- [Python](https://github.com/Sensirion/info#python-drivers)

# Usage

## SHDLC

SHDLC (Sensirion High-Level Data Link Control) is a byte-oriented master-slave
communication protocol based on [ISO
HDLC](https://en.wikipedia.org/wiki/High-Level_Data_Link_Control). It is used
to control some of Sensirion’s devices (for example mass flow controllers). The
detailed protocol documentation is not publicly available (yet). If you need
it, please contact our [customer
support](https://www.sensirion.com/en/about-us/contact/).

This library provides the following classes for communication with Sensirion
Sensors using the SHDLC protocol.
- `SensirionShdlcTxFrame`
- `SensirionShdlcRxFrame`
- `SensirionShdlcCommunication`

### Example Usage
First initialize an instance of `SensirionShdlcTxFrame` and
`SensirionShdlcRxFrame` with a properly sized buffer. A good worst case
estimation for the buffer size is `2 * (n+6)` where `n` is the number of bytes
you want to send. After that you can build your frame by first calling
`begin()`. Information about the correct COMMAND and ADDRESS can be found on
the data sheet of your sensor. Then you can add data to the frame by using
different add member functions. See the code below for examples. After adding
your data finish the frame by calling `finish()`.

To send this frame to the sensor you first need to initialize the correct
Stream object (Serial,UART,...) to talk to your sensor. Don't forget to also
call the `.begin()` function with the right configuration. Then call the static
function `sendAndReceiveFrame()` from `SensirionShdlcCommunication` as shown
below. You need to replace `STREAMOBJECT` with the initialized Stream object of
your choice. Additionally you need to provide a timeout for to receive data
back, consult the data sheet of your sensor for information on the best timeout
value.

You can decode the frame by using the different get members to convert the
received data to desired data types.

All functions return a error code if an error occurs during execution and zero
otherwise.

```cpp
uint8_t txBuffer[256];
uint8_t rxBuffer[256];

SensirionShdlcTxFrame txFrame(txBuffer, 256);
SensirionShdlcRxFrame rxFrame(rxBuffer, 256);

txFrame.begin(COMMAND, ADDRESS, DATALENGTH);

txFrame.addUInt8(UINT8);
txFrame.addUInt32(UINT32);

txFrame.finish();

SensirionShdlcCommunication::sendAndReceiveFrame(STREAMOBJECT, txFrame, rxFrame, TIMEOUT);

rxFrame.getUInt16(UINT16);
rxFrame.getFloat(FLOAT);

```

## I2C

This library provides the following classes for communication with Sensirion
Sensors using the I2C protocol.
- `SensirionI2cTxFrame`
- `SensirionI2cRxFrame`
- `SensirionI2cCommunication`

### Example Usage

First initialize an instance of `SensirionI2CTxFrame` and `SensirionI2CRxFrame`
with a buffer sized the amount of data to read times 1.5. This is needed to
account for the CRC which is added after every second byte. After that you can
build your frame by first calling `addCommand()` to add the command at the
beginning of the frame. Information about the different COMMANDs can be found
on the data sheet of your sensor. Then you can add data to the frame by using
different add member functions. See the code below for examples.

To send this frame to the sensor you first need to initialize a Wire object.
Don't forget to also call the `.begin()` function with the right configuration.
Then call the static function `sendFrame()` form `SensirionI2CCommunication` as
shown below. You can find the ADDRESS on the data sheet of the sensor. You also
need to replace `WIREOBJECT` with the initialized Wire object. Then wait the in
the data sheet documented `READ_DELAY` before receiving the reply from the
sensor by calling `receiveFrame()` with the same Wire object.

You then can decode the frame by using the different get members to convert the
received data to desired data types.

All functions return a error code if an error occurs during execution and zero
otherwise.

```cpp
uint8_t txBuffer[256];
uint8_t rxBuffer[256];

SensirionShdlcTxFrame txFrame(txBuffer, 256);
SensirionShdlcRxFrame rxFrame(rxBuffer, 256);

txFrame.addCommand(COMMAND);

txFrame.addUInt8(UINT8);
txFrame.addUInt32(UINT32);

SensirionShdlcCommunication::sendFrame(ADDRESS, txFrame, WIREOBJECT);

delay(READ_DELAY);

SensirionShdlcCommunication::receiveFrame(ADDRESS, rxFrame, WIREOBJECT);

rxFrame.getUInt16(UINT16);
rxFrame.getFloat(FLOAT);

```
