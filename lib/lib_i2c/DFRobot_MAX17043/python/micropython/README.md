# DFRobot_MAX17043

* [中文版](./README_CN.md)

The MAX17043 is ultra-compact, low-cost,host-side fuel-gauge systems for lithium-ion (Li+) batter-ies in handheld and portable 
equpiment.It employs Gravity I2C interface,ultra-low opearting current, and real-time tracking of the relative state of charge 
(SOC) of the battery through Maxim's patented algorithm,eliminating the need for full-to-empty relearning and offset accumualtion 
errors.Plug and play to accurately measure the voltage and remaining power of the battery. The module  also features as a low 
battery power alert interrupt function.  When the battery power falls below specified threshold, the ALR pin generates a falling 
pluse to trigger the external interrupt of the controller.One thing should mention that the default value of the battery low power
interrupt alert threshold is 32%, this threshold can be set by the function set_interrupt().

![产品效果图](../../resources/images/DFR0563.jpg) 

## Product Link（[https://www.dfrobot.com/product-1734.html](https://www.dfrobot.com/product-1734.html)）
    SKU: DFR0563

## Table of Contents
* [Summary](#summary)
* [connection](connection)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary
Provides a microPython library for reading and interperting MAX17043 data over I2C.

## Connection
Wires of the same color are linked together,and We only exemplify how this board is connected to the Fuel Gauge.
When connecting , it is necessary to pay attention to the correspondence among pins, the connection diagram is as fellows.

* ESP32

<br>
<img src="../../resources/images/esp32.jpg">
<br>


## Installation

To use this library download the zip file, uncomperss it to a folder named DFRobot_MAX17043 in your upyCraft workspace.

## Methods

```python
  '''!
    @brief MAX17043 begin and test moudle
    @return initialization result:
    @retval 0     successful
    @retval -1     faild
  '''
  def begin(self):
    
  '''!
    @brief read battery voltage in mV
    @return voltage in mV
  '''    
  def read_voltage(self):
    
  '''!
    @brief read battery remaining capacity in percentage
    @return battery remaining capacity in percentage
  '''
  def read_percentage(self):
  '''!
    @brief set MAX17043 interrput threshold
    @param per       interrupt threshold as %1 - 32% (integer)
  '''
  def set_interrupt(self, per):
    
  '''!
    @brief clear MAX17043 interrupt.
  '''
  def clear_interrupt(self):
    
  '''!
    @brief set MAX17043 in sleep mode.
  '''
  def set_sleep(self):
    
  '''!
    @brief wake up MAX17043.
  ''' 

```
## Compatibility

| MCU                | Work Well | Work Wrong | Untested | Remarks |
| ------------------ | :-------: | :--------: | :------: | ------- |
|      ESP32         | √         |            |          |

## History

- 2018/04/14 - Version 1.0.0 released.

## Credits

Written by ouki.wang(ouki.wang@dfrobot.com), 2018. (Welcome to our [website](https://www.dfrobot.com/))
