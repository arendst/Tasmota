# Device Groups

The device groups module provides a framework to allow multiple devices to be in a group with values such as power, light color/temperature/brightness, PWM values, sensor values, etc. shared with other devices in the group. For example, with multiple light modules in a device group, the light settings can be changed on one module and the settings will automatically be changed on the other light modules. Dimmer switch modules could be in a device group with light modules and the dimmer switch could control the power, brightness and colors of all the lights in the group. Multiple dimmer switches could be in a device group to form a 3-way/4-way dimmer switch.

UDP broadcasts, followed by UDP unicasts if necessary, are used to send updates to all devices so updates are fast. There is no need for an MQTT server but all the devices in a group must be on the same IP network.

To include device groups support in the build, define USE_DEVICE_GROUPS in your user_config_override. This adds 3.5K to the code size. All devices in a group must be running firmware with device group support and have device groups enabled.

To enable device groups, set Option16 to 1 and **restart the device**.


## Device Groups Operation

The device group name is the MQTT group topic set with the GroupTopic command. All devices in the same IP network with the same group topic are in the same group. Some modules may define additional device groups. For example, if Remote Device Mode is enabled, the PWM Dimmer module defines three devices groups.

The items that are sent to the group and the items that are received from the group are selected with the DevGroupShare command. By default all items are sent and received from the group. An example of when the DevGroupShare command would be used is when you have a group of lights that you control with a dimmer switch and home automation software. You want the dimmer switch to be able to control all items. The home automation software controls each light individually. When it controls the whole group, it actually sends command to each light in the group. If you use the home automation software to turn an individual light on or off or change it’s brightness, color or scheme, you do not want the change to be replicated to the other lights. In this case, you would set the incoming and outgoing item masks to 255 on the dimmer switch (DevGroupShare 255,255) and set the incoming item mask to 255 and outgoing item mask to 0 on all the lights (DevGroupShare 255,0).


### Commands


<table>
  <tr>
   <td><strong>Command</strong>
   </td>
   <td><strong>Parameters</strong>
   </td>
  </tr>
  <tr>
   <td>DevGroupShare
   </td>
   <td><in>,&lt;out> = set incoming and outgoing shared item mask (default = 255,255)
<p>
1 = Power, 2 = Light brightness, 4 = Light fade/speed, 8 = Light scheme, 16 = Light color, 32 = Minimum brightness
   </td>
  </tr>
  <tr>
   <td>GroupTopic&lt;x>
   </td>
   <td>1 = reset device group &lt;x> MQTT group topic to firmware default (MQTT_GRPTOPIC) and restart
<p>
<value> = set device group &lt;x> MQTT group topic (32 chars max) and restart
   </td>
  </tr>
</table>
