This is a fork from **http://tmrh20.github.io/RF24** which can be build as a static library for Atmel Studio 7.

Not all files are needed.

Just copy the following structure into a GCC Static Library project in AS7:
```
utility\
  ATXMega256D3\
    compatibility.c
    compatibility.h
    gpio.cpp
    gpio.h
    gpio_helper.c
    gpio_helper.h
    includes.h
    RF24_arch_config.h
    spi.cpp
    spi.h
nRF24L01.h
printf.h
RF24.cpp
RF24.h
RF24_config.h
```

Only ATXMega256D3 is supported right now!

## Notes
The millisecond functionality is based on the TCE0 so don't use these pins as IO.

The operating frequency of the uC is 32MHz. If else change the TCE0 registers appropriatly in function **__start_timer()** in **compatibility.c** file for your frequency. 


## Usage
Add the library to your project!
In the file where the **main()** is put the following in order to update the millisecond functionality:
```
ISR(TCE0_OVF_vect)
{
	update_milisec();
}
```

Declare the rf24 radio with **RF24 radio(XMEGA_PORTC_PIN3, XMEGA_SPI_PORT_C);**

First parameter is the CE pin which can be any available pin on the uC.

Second parameter is the CS which can be on port C (**XMEGA_SPI_PORT_C**) or on port D (**XMEGA_SPI_PORT_D**). 

Call the **__start_timer()** to start the millisecond timer.



** For further information please see http://tmrh20.github.io/RF24 for all documentation**
