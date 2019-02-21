# Adafruit Unified Sensor Driver #

Many small embedded systems exist to collect data from sensors, analyse the data, and either take an appropriate action or send that sensor data to another system for processing.

One of the many challenges of embedded systems design is the fact that parts you used today may be out of production tomorrow, or system requirements may change and you may need to choose a different sensor down the road.

Creating new drivers is a relatively easy task, but integrating them into existing systems is both error prone and time consuming since sensors rarely use the exact same units of measurement.

By reducing all data to a single **sensors\_event\_t** 'type' and settling on specific, **standardised SI units** for each sensor family the same sensor types return values that are comparable with any other similar sensor.  This enables you to switch sensor models with very little impact on the rest of the system, which can help mitigate some of the risks and problems of sensor availability and code reuse.

The unified sensor abstraction layer is also useful for data-logging and data-transmission since you only have one well-known type to log or transmit over the air or wire.

## Unified Sensor Drivers ##

The following drivers are based on the Adafruit Unified Sensor Driver:

**Accelerometers**
  - [Adafruit\_ADXL345](https://github.com/adafruit/Adafruit_ADXL345)
  - [Adafruit\_LSM303DLHC](https://github.com/adafruit/Adafruit_LSM303DLHC)
  - [Adafruit\_MMA8451\_Library](https://github.com/adafruit/Adafruit_MMA8451_Library)

**Gyroscope**
  - [Adafruit\_L3GD20\_U](https://github.com/adafruit/Adafruit_L3GD20_U)

**Light**
  - [Adafruit\_TSL2561](https://github.com/adafruit/Adafruit_TSL2561)
  - [Adafruit\_TSL2591\_Library](https://github.com/adafruit/Adafruit_TSL2591_Library)

**Magnetometers**
  - [Adafruit\_LSM303DLHC](https://github.com/adafruit/Adafruit_LSM303DLHC)
  - [Adafruit\_HMC5883\_Unified](https://github.com/adafruit/Adafruit_HMC5883_Unified)
  
**Barometric Pressure**
  - [Adafruit\_BMP085\_Unified](https://github.com/adafruit/Adafruit_BMP085_Unified)
  - [Adafruit\_BMP183\_Unified\_Library](https://github.com/adafruit/Adafruit_BMP183_Unified_Library)

**Humidity & Temperature**
  - [DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library)

**Orientation**
 - [Adafruit_BNO055](https://github.com/adafruit/Adafruit_BNO055)

## How Does it Work? ##

Any driver that supports the Adafruit unified sensor abstraction layer will implement the Adafruit\_Sensor base class.  There are two main typedefs and one enum defined in Adafruit_Sensor.h that are used to 'abstract' away the sensor details and values:

**Sensor Types (sensors\_type\_t)**

These pre-defined sensor types are used to properly handle the two related typedefs below, and allows us determine what types of units the sensor uses, etc.

```
/** Sensor types */
typedef enum
{
  SENSOR_TYPE_ACCELEROMETER         = (1),
  SENSOR_TYPE_MAGNETIC_FIELD        = (2),
  SENSOR_TYPE_ORIENTATION           = (3),
  SENSOR_TYPE_GYROSCOPE             = (4),
  SENSOR_TYPE_LIGHT                 = (5),
  SENSOR_TYPE_PRESSURE              = (6),
  SENSOR_TYPE_PROXIMITY             = (8),
  SENSOR_TYPE_GRAVITY               = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION   = (10),
  SENSOR_TYPE_ROTATION_VECTOR       = (11),
  SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
  SENSOR_TYPE_AMBIENT_TEMPERATURE   = (13),
  SENSOR_TYPE_VOLTAGE               = (15),
  SENSOR_TYPE_CURRENT               = (16),
  SENSOR_TYPE_COLOR                 = (17)
} sensors_type_t;
```

**Sensor Details (sensor\_t)**

This typedef describes the specific capabilities of this sensor, and allows us to know what sensor we are using beneath the abstraction layer.

```
/* Sensor details (40 bytes) */
/** struct sensor_s is used to describe basic information about a specific sensor. */
typedef struct
{
    char     name[12];
    int32_t  version;
    int32_t  sensor_id;
    int32_t  type;
    float    max_value;
    float    min_value;
    float    resolution;
    int32_t  min_delay;
} sensor_t;
```

The individual fields are intended to be used as follows:

- **name**: The sensor name or ID, up to a maximum of twelve characters (ex. "MPL115A2")
- **version**: The version of the sensor HW and the driver to allow us to differentiate versions of the board or driver
- **sensor\_id**: A unique sensor identifier that is used to differentiate this specific sensor instance from any others that are present on the system or in the sensor network
- **type**: The sensor type, based on **sensors\_type\_t** in sensors.h
- **max\_value**: The maximum value that this sensor can return (in the appropriate SI unit)
- **min\_value**: The minimum value that this sensor can return (in the appropriate SI unit)
- **resolution**: The smallest difference between two values that this sensor can report (in the appropriate SI unit)
- **min\_delay**: The minimum delay in microseconds between two sensor events, or '0' if there is no constant sensor rate

**Sensor Data/Events (sensors\_event\_t)**

This typedef is used to return sensor data from any sensor supported by the abstraction layer, using standard SI units and scales.

```
/* Sensor event (36 bytes) */
/** struct sensor_event_s is used to provide a single sensor event in a common format. */
typedef struct
{
    int32_t version;
    int32_t sensor_id;
    int32_t type;
    int32_t reserved0;
    int32_t timestamp;
    union
    {
        float           data[4];
        sensors_vec_t   acceleration;
        sensors_vec_t   magnetic;
        sensors_vec_t   orientation;
        sensors_vec_t   gyro;
        float           temperature;
        float           distance;
        float           light;
        float           pressure;
        float           relative_humidity;
        float           current;
        float           voltage;
        sensors_color_t color;
    };
} sensors_event_t;
```
It includes the following fields:

- **version**: Contain 'sizeof(sensors\_event\_t)' to identify which version of the API we're using in case this changes in the future
- **sensor\_id**: A unique sensor identifier that is used to differentiate this specific sensor instance from any others that are present on the system or in the sensor network (must match the sensor\_id value in the corresponding sensor\_t enum above!)
- **type**: the sensor type, based on **sensors\_type\_t** in sensors.h
- **timestamp**: time in milliseconds when the sensor value was read
- **data[4]**: An array of four 32-bit values that allows us to encapsulate any type of sensor data via a simple union (further described below)

**Required Functions**

In addition to the two standard types and the sensor type enum, all drivers based on Adafruit_Sensor must also implement the following two functions:

```
bool getEvent(sensors_event_t*);
```
Calling this function will populate the supplied sensors\_event\_t reference with the latest available sensor data.  You should call this function as often as you want to update your data.

```
void getSensor(sensor_t*);
```
Calling this function will provide some basic information about the sensor (the sensor name, driver version, min and max values, etc.

**Standardised SI values for sensors\_event\_t**

A key part of the abstraction layer is the standardisation of values on SI units of a particular scale, which is accomplished via the data[4] union in sensors\_event\_t above.  This 16 byte union includes fields for each main sensor type, and uses the following SI units and scales:

- **acceleration**: values are in **meter per second per second** (m/s^2)
- **magnetic**: values are in **micro-Tesla** (uT)
- **orientation**: values are in **degrees**
- **gyro**: values are in **rad/s**
- **temperature**: values in **degrees centigrade** (Celsius) 
- **distance**: values are in **centimeters**
- **light**: values are in **SI lux** units
- **pressure**: values are in **hectopascal** (hPa)
- **relative\_humidity**: values are in **percent**
- **current**: values are in **milliamps** (mA)
- **voltage**: values are in **volts** (V)
- **color**: values are in 0..1.0 RGB channel luminosity and 32-bit RGBA format

## The Unified Driver Abstraction Layer in Practice ##

Using the unified sensor abstraction layer is relatively easy once a compliant driver has been created.

Every compliant sensor can now be read using a single, well-known 'type' (sensors\_event\_t), and there is a standardised way of interrogating a sensor about its specific capabilities (via sensor\_t).

An example of reading the [TSL2561](https://github.com/adafruit/Adafruit_TSL2561) light sensor can be seen below:

```
 Adafruit_TSL2561 tsl = Adafruit_TSL2561(TSL2561_ADDR_FLOAT, 12345);
 ...
 /* Get a new sensor event */ 
 sensors_event_t event;
 tsl.getEvent(&event);
 
 /* Display the results (light is measured in lux) */
 if (event.light)
 {
   Serial.print(event.light); Serial.println(" lux");
 }
 else
 {
   /* If event.light = 0 lux the sensor is probably saturated
      and no reliable data could be generated! */
   Serial.println("Sensor overload");
 }
```

Similarly, we can get the basic technical capabilities of this sensor with the following code:

```
 sensor_t sensor;
 
 sensor_t sensor;
 tsl.getSensor(&sensor);

 /* Display the sensor details */
 Serial.println("------------------------------------");
 Serial.print  ("Sensor:       "); Serial.println(sensor.name);
 Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
 Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
 Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
 Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
 Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
 Serial.println("------------------------------------");
 Serial.println("");
```
