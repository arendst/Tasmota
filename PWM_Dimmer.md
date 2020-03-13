# PWM Dimmer

The PWM Dimmer module adds support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM dimmer switches. The brightness of the load for these dimmers is controlled by a PWM GPIO pin. They typically have power, up and down buttons, a powered-on LED, five brightness LEDs and another status LED. Examples are:[ https://www.amazon.com/dp/B07FXYSVR1](https://www.amazon.com/dp/B07FXYSVR1),[ https://www.amazon.com/dp/B07V26Q3VD](https://www.amazon.com/dp/B07V26Q3VD),[ https://www.amazon.com/dp/B07K67D43J](https://www.amazon.com/dp/B07K67D43J),[ https://www.amazon.com/dp/B07TTGFWFM](https://www.amazon.com/dp/B07TTGFWFM)

To include PWM dimmer support in the build, define USE_PWM_DIMMER in your user_config_override. This adds 4.5K to the code size. The light module is not required for PWM dimmer operation so you can #undef USE_LIGHT to reduce the firmware bin size.

To enable PWM dimmer operation, select the PWM Dimmer module.


## PWM Dimmer Operation

Pressing and releasing the power button toggles the power on/off. If the toggle turns the power on, the load is returned to the last brightness it was adjusted to. If Fade is enabled, the load is faded on/off at the rate defined by the Speed setting.

When the power is on, holding the down or up button decreases/increases the brightness. The brightness can also be changed using just the power button. When the power is on, holding the power button alternately increases or decreases the brightness. Initially, holding the power button increases the brightness. Releasing and then holding the power button again decreases the brightness.

When the power is off, pressing and releasing the down or up button turns the power on at a temporary brightness of the low/high levels set by the DimmerPreset command. Turning the power on at the low preset can also be accomplished by holding the power button while the power is off. The dimmer presets are intended to enable quickly turning on a light to a dim or bright level without changing the normal desired brightness. Turning the power on to a preset does not change the brightness the load will be set to when the switch is turned on the next time. For example, if the light is on and you adjust the brightness to 80 and then turn the light off, when you turn it back on, the brightness will return to 80. If you turn the power off again and then press the down button, the light will be turned on with a brightness of the low preset. If you then turn the light off and on again, the brightness will return to 80.

When the power is off, holding the down or up button publishes an MQTT EVENT command. The topic follows the format of the Full Topic with a prefix of Event (ex. cmnd/LightSwitch1/EVENT). The MQTT payload is Trigger#, where # is 1 if the down button is held or 2 if the up button is held. These triggers can be used in rules on remote devices  (ON Event#Trigger1) or by automation software to trigger automations such as scene changes. For example, the Event topic Trigger1 payload could trigger the automation software to turn on the previous scene in a list and the Trigger2 payload could trigger the automation software to turn on the next scene in a list.

If there are LED’s defined in the template, they are turned on to indicate the current brightness. More LEDs are turned on at higher brightnesses. The LedTimeout command enables/disables an LED timeout. If LED timeout is enabled, the LED’s turn off five seconds after the last change in brightness. Note that the lowest LED and the blue power LED are always on when the power is on. The LED timeout can also be enabled/disabled by holding the power button while tapping (pressing and releasing quickly) the down button.

The LEDLink LED can be used as a nightlight/powered-off indicator. The PoweredOffLed command enables/disables turning the LEDLink LED on when the power is off. The powered-off indicator can also be enabled/disabled by holding the power button and tapping the up button.

Holding the power button, tapping the down button and then tapping or holding the down or up button sends a device group message to set RGB lights in the device group to the previous/next fixed color. The command is sent/value is adjusted once every .5 seconds for as long as the button is held. The color sequence as defined by the Light module is red, green, blue, orange, light green, light blue, amber, cyan, purple, yellow, pink, white using RGB channels, white using CT channels.

Holding the power button, tapping the up button and then tapping or holding the down or up button publishes an MQTT Event command. The command is sent/value is adjusted once every .5 seconds for as long as the button is held. The MQTT topic is as described above. The MQTT payload is Trigger#, where # is 3 if the down button is held or 4 if the up button is held.

Holding the down or up button alone for over 10 seconds executes the WiFiConfig 2 command.

Pressing and releasing any button publishes an MQTT TOGGLE command for the button. Holding a button publishes an MQTT HOLD command followed by an MQTT OFF command when the button is released.

When Device Groups are enabled, the PWM Dimmer brightness presets are kept in sync across all switches in the group. The powered-off LED and LED timeout settings are specific to each switch. Changing them does not replicate the change to the other switches in the group.

### Commands

<table>
  <tr>
   <td><strong>Command</strong>
   </td>
   <td><strong>Parameters</strong>
   </td>
  </tr>
  <tr>
   <td>BriPreset
   </td>
   <td> &lt;low>,&lt;high> = set brightness low and high presets<br>
1..255 = set brightness preset<br>
+ = increase brightness preset<br>
- = decrease brightness preset
   </td>
  </tr>
  <tr>
   <td>SetOption86
   </td>
   <td>Set brightness LED timeout<br>
0 = disable timeout (default)<br>
1 = enable timeout
   </td>
  </tr>
  <tr>
   <td>SetOption87
   </td>
   <td>Set powered-off LED (nightlight)<br>
0 = disable powered-off LED (default)<br>
1 = enable powered-off LED
   </td>
  </tr>
  <tr>
   <td>SetOption88
   </td>
   <td>Set remote device mode<br>
0 = disable remote device mode(default)<br>
1 = enable remote device mode
   </td>
  </tr>
</table>



### Remote Device Mode

Remote device mode allows PWM Dimmer switches to control remote devices. With remote device mode enabled, each button controls a different device. Note that dimmer switches with toggle-style down/up buttons have limited functionality as remote device mode switches because you can not push the down and up buttons simultaneously.

To include remote device mode support in the build, define USE_PWM_DIMMER_REMOTE in your user_config_override. Remote device mode support requires device group support so USE_DEVICE_GROUPS is automatically defined if USE_PWM_DIMMER_REMOTE is defined. Remote device mode support adds 0.7K to the code size in addition to the code size required for device groups support.

To enable remote device mode, execute the command SetOption88 1. Each remote device must be running firmware with device group support and have remote device support enabled. The remote devices do not need to be built with PWM dimmer support nor do they need to be switches.

If a remote device is a PWM dimmer, the device acts like a 3-way dimmer switch would and may or may not have a load connected to it. It’s also possible to use a PWM dimmer switch without a load to act as a wall switch to control the power, brightness and color of one or more smart lights with Tasmota with device group support loaded on them.

With remote device mode is enabled, button 1 is the power button for the local device while buttons 2 and 3 are the power buttons for remote devices. Group names for buttons 2 and 3 are set by the GroupTopic2 and GroupTopic3 commands respectively. Note that the button numbers are defined by the module template and can be in any physical order on the switch (button 1 can be defined as the top button, the middle button or the bottom button). Button combinations that publish MQTT Event commands use a topic in the format cmnd/%group-topic%/EVENT.

Pressing and releasing a power button toggles the power on all devices in the group assigned to the button. When the power is on, holding the button alternately increases or decreases the brightness. When the power is off, holding the button turns the power on at a temporary brightness of the low level set by the BriPreset command.

While holding a power button, the other two buttons act like the down and up buttons for the remote device. All the functions performed by the down and up buttons in non-remote device mode are available in remote device mode. While holding button 1, button 2 performs the functions of the down button and button 3 performs the functions of the up button. While holding button 2, button 1 performs the functions of the down button and button 3 performs the functions of the up button. While holding button 3, button 1 performs the functions of the down button and button 2 performs the functions of the up button.
