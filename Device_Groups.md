# Device Groups

The device groups module provides a framework to allow multiple devices to be in a group with values such as power, light color/temperature/brightness, PWM values, sensor values, etc. shared with other devices in the group. For example, with multiple light modules in a device group, the light settings can be changed on one module and the settings will automatically be changed on the other light modules. Dimmer switch modules could be in a device group with light modules and the dimmer switch could control the power, brightness and colors of all the lights in the group. Multiple dimmer switches could be in a device group to form a 3-way/4-way dimmer switch.

UDP multicasts, followed by UDP unicasts if necessary, are used to send updates to all devices so updates are fast. There is no need for an MQTT server but all the devices in a group must be on the same IP network.

To include device groups support in the build, define USE_DEVICE_GROUPS in your user_config_override. This adds 3.5K to the code size. All devices in a group must be running firmware with device group support and have device groups enabled.

To enable device groups, execute the command SetOption85 1.


## Device Groups Operation

The device group name is set with the DevGroupName command. If the device group name is not set for a group, the MQTT group topic is used (with the device group number appended for device group numbers > 1). All devices in the same IP network with the same device group name are in the same group. Some modules may define additional device groups. For example, if Remote Device Mode is enabled, the PWM Dimmer module defines three devices groups.

The items that are sent to the group and the items that are received from the group are selected with the DevGroupShare command. By default all items are sent and received from the group. An example of when the DevGroupShare command would be used is when you have a group of lights that you control with a dimmer switch and home automation software. You want the dimmer switch to be able to control all items. The home automation software controls each light individually. When it controls the whole group, it actually sends command to each light in the group. If you use the home automation software to turn an individual light on or off or change itâ€™s brightness, color or scheme, you do not want the change to be replicated to the other lights. In this case, you would set the incoming and outgoing item masks to 0xffffffff (all items) on the dimmer switch (DevGroupShare 0xffffffff,0xffffffff) and set the incoming item mask to 0xffffffff and outgoing item mask to 0 on all the lights (DevGroupShare 0xffffffff,0).


### Commands


<table>
  <tr>
   <td><strong>Command</strong>
   </td>
   <td><strong>Parameters</strong>
   </td>
  </tr>
  <tr>
   <td>DevGroupName&lt;x>
   </td>
   <td>0 = clear device group &lt;x> name and restart<br>
&lt;value> = set device group &lt;x> name and restart
   </td>
  </tr>
  </tr>
  <tr>
   <td>DevGroupSend&lt;x>
   </td>
   <td>&lt;item>=&lt;value>[ ...] = send an update to device grouup &lt;x>. The device group name must have been previously set with DevGroupName&lt;x>. Multiple items/value pairs can be specified separated by a space. For example, DevGroupSend 4=90 128=1 will send an update to set the light brightness to 90 and turn relay 1 on.<br>
   <br>
   For items with numeric values, &lt;value> can be specified as @&lt;operator>&lt;value> to send a value after performing an operation on the current value. &lt;operator> can be + (add), - (subtract) or ^ (invert). If a value is not specified for the invert operator, 0xffffffff is used. For example, if the group's current light brightness is 40, DevGroupSend 4=@10 will send an update to increase the light brightness to 50. If all the groups relays are currently on, DevGroupSend 128=^ will turn all the relays off.<br><br>
    <table>
     <tr>
      <td><strong>Item</strong>
      </td>
      <td><strong>Description</strong>
      </td>
      <td><strong>Value</strong>
      </td>
     </tr>
     <tr>
      <td>2
      </td>
      <td>Light fade
      </td>
      <td>0 = Off<br>1 = On
     </tr>
     <tr>
      <td>3
      </td>
      <td>Light speed
      </td>
      <td>1..40
     </tr>
     <tr>
      <td>4
      </td>
      <td>Light brightness
      </td>
      <td>0..255
     </tr>
     <tr>
      <td>5
      </td>
      <td>Light scheme
      </td>
      <td>See Scheme command in <a href="https://tasmota.github.io/docs/Commands/#light">Light Commands</a>
     </tr>
     <tr>
      <td>6
      </td>
      <td>Light fixed color
      </td>
      <td>0 = white (using CT channels), 1 = red, 2 = green, 3 = blue, 4 = orange, 5 = light green, 6 = light blue, 7 = amber, 8 = cyan, 9 = purple, 10 = yellow, 11 = pink, 12 = white (using RGB channels)
     </tr>
     <tr>
      <td>7
      </td>
      <td>PWM dimmer low preset
      </td>
      <td>0..255
     </tr>
     <tr>
      <td>8
      </td>
      <td>PWM dimmer high preset
      </td>
      <td>0..255
     </tr>
     <tr>
      <td>9
      </td>
      <td>PWM dimmer power-on brightness
      </td>
      <td>0..255
     </tr>
     <tr>
      <td>128
      </td>
      <td>Relay Power
      </td>
      <td>Bitmask with bits set for relays to be powered on. The number of relays can be specified in bits 24 - 31. If the number of relays is not specified, only relay 1 is set.
     </tr>
     <tr>
      <td>224
      </td>
      <td>Light channels
      </td>
      <td>Comma separated list of brightness levels (0..255) for channels 1 - 5 (e.g. 255,128,0,0,0 will turn the red channel on at 100% and the green channel on at 50% on an RBG light).
     </tr>
    </table>
   </td>
  </tr>
  </tr>
  <tr>
   <td>DevGroupShare
   </td>
   <td>&lt;in>,&lt;out> = set incoming and outgoing shared item mask (default = 0xffffffff,0xffffffff)<br>
1 = Power, 2 = Light brightness, 4 = Light fade/speed, 8 = Light scheme, 16 = Light color, 32 = Dimmer settings (presets)
   </td>
</table>