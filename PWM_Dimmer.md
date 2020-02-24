# PWM Dimmer

The PWM Dimmer module adds support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM dimmer switches. The brightness of the load for these dimmers is controlled by a PWM GPIO pin. They typically have power, up and down buttons, a powered-on LED, five brightness LEDs and another status LED. Examples are:[ https://www.amazon.com/dp/B07FXYSVR1](https://www.amazon.com/dp/B07FXYSVR1),[ https://www.amazon.com/dp/B07V26Q3VD](https://www.amazon.com/dp/B07V26Q3VD),[ https://www.amazon.com/dp/B07K67D43J](https://www.amazon.com/dp/B07K67D43J),[ https://www.amazon.com/dp/B07TTGFWFM](https://www.amazon.com/dp/B07TTGFWFM)

To include PWM dimmer support in the build, define USE_PWM_DIMMER in your user_config_override. This adds 4.5K to the code size. The light module is not required for PWM dimmer operation so you can #undef USE_LIGHT to reduce the firmware bin size.

To enable PWM dimmer operation, select the PWM Dimmer module.


## PWM Dimmer Operation

Pressing and releasing the power button toggles the power on/off. If the toggle turns the power on, the load is returned to the last brightness it was adjusted to. If Fade is enabled, the load is faded on/off at the rate defined by the Speed setting.

When the power is on, holding the down or up button decreases/increases the brightness (PWM value). The brightness is changed faster as higher brightnesses. The BriMin command defines the lowest value the brightness can be decreased to.

The brightness can also be changed using just the power button. When the power is on, holding the power button alternately increases or decreases the brightness. Initially, holding the power button increases the brightness. Releasing and then holding the power button again decreases the brightness.

When the power is off, holding the down or up button turns the power on at a temporary brightness of the low/high levels set by the BriPreset command (default =10,255). Turning the power on at the low preset can also be accomplished by holding the power button while the power is off. The brightness presets are intended to enable quickly turning on a light to a dim or bright level without changing the normal desired brightness. Turning the power on to a preset does not change the brightness the load will be set to when the switch is turned on the next time. For example, if the light is on and you adjust the brightness to 80 and then turn the light off, when you turn it back on, the brightness will return to 80. If you turn the power off again and then press the down button, the light will be turned on with a brightness of the low preset. If you then turn the light off and on again, the brightness will return to 80.

If there are LED’s defined in the template, they are turned on to indicate the current brightness. More LEDs are turned on at higher brightnesses. The LedTimeout command enables/disables an LED timeout. If LED timeout is enabled, the LED’s turn off five seconds after the last change in brightness. Note that the lowest LED and the blue power LED are always on when the power is on.

The LEDLink LED can be used as a nightlight/powered-off indicator. The PoweredOffLed command enables/disables turning the LEDLink LED on when the power is off.

Tapping (pressing and releasing quickly) the down or up buttons a given number of times and then holding the down or up button decreases or increases settings according to the table below. For example, tapping the down button once and then holding the up button sets all RGB lights in the device group to the next fixed color. Tapping the up button three times and then holding the down button decreases the high brightness preset.


<table>
  <tr>
   <td>Taps
   </td>
   <td>Down Button
   </td>
   <td>Up Button
   </td>
  </tr>
  <tr>
   <td>1
   </td>
   <td>Set fixed color<sup>1</sup>
   </td>
   <td>Publish MQTT event<sup>2</sup>
   </td>
  </tr>
  <tr>
   <td>2
   </td>
   <td>Adjust minimum brightness
   </td>
   <td>Adjust fade speed
   </td>
  </tr>
  <tr>
   <td>3
   </td>
   <td>Adjust low brightness preset
   </td>
   <td>Adjust high brightness preset
   </td>
  </tr>
</table>


1. Setting the previous/next color only functions when remote device mode is enabled (see below) and only when the switch is in a device group with an RGB light. The color sequence as defined by the Light module is red, green, blue, orange, light green, light blue, amber, cyan, purple, yellow, pink, white using RGB channels, white using CT channels.

2. The MQTT topic has the format %group-topic%/cmnd/Event, where %group-topic% is the group topic set by the GroupTopic command. The MQTT payload is SwitchTrigger#, where # is 1 if the down button is held or 2 if the up button is held. These triggers can be used in rules on remote devices  (ON Event#SwitchTrigger1) or by automation software to trigger automations such as scene changes. For example, the Event topic SwitchTrigger1 payload could trigger the automation software to turn on the previous scene in a list and the SwitchTrigger2 payload could trigger the automation software to turn on the next scene in a list.

Holding the power button, pressing the down/up buttons a given number of times and then releasing the power button toggles options according to the table below. Note that you must press a down or up button within 0.5 seconds to prevent the power button hold action from taking place.


<table>
  <tr>
   <td>Presses
   </td>
   <td>Down Button
   </td>
   <td>Up Button
   </td>
  </tr>
  <tr>
   <td>1
   </td>
   <td>Toggle powered-off LED
   </td>
   <td>Toggle brightness LED timeout
   </td>
  </tr>
  <tr>
   <td>2
   </td>
   <td>Toggle fading
   </td>
   <td>
   </td>
  </tr>
</table>


Holding any button for over 10 seconds executes the WiFiConfig 2 command.

When Device Groups are enabled, the PWM Dimmer minimum brightness and brightness presets are kept in sync across all switches in the group. The powered-off LED and LED timeout settings are specific to each switch. Changing them does not replicate the change to the other switches in the group.


### Commands


<table>
  <tr>
   <td><strong>Command</strong>
   </td>
   <td><strong>Parameters</strong>
   </td>
  </tr>
  <tr>
   <td>BriMin
   </td>
   <td>1..255 = set minimum brightness
<p>
+ = increase minimum brightness
<p>
- = decrease minimum brightness
   </td>
  </tr>
  <tr>
   <td>BriPreset
   </td>
   <td><low>,&lt;high> = set brightness low and high presets
<p>
1..255 = set brightness preset
<p>
+ = increase brightness preset
<p>
- = decrease brightness preset
   </td>
  </tr>
  <tr>
   <td>Dimmer
   </td>
   <td>0..100 = set dimmer value from 0 to 100%
<p>
+ = increase by 10
<p>
- = decrease by 10
   </td>
  </tr>
  <tr>
   <td>Fade
   </td>
   <td>0 = do not use fade <em>(default)</em>
<p>
1 = use fade
   </td>
  </tr>
  <tr>
   <td>LedTimeout
   </td>
   <td>0 = disable LED timeout
<p>
1 = enable LED timeout
   </td>
  </tr>
  <tr>
   <td>PoweredOffLed
   </td>
   <td>0 = disable powered-off LED
<p>
1 = disable powered-off LED
   </td>
  </tr>
  <tr>
   <td>Speed
   </td>
   <td>1..20 = set fade speed from fast 1 to very slow 20
<p>
+ = increase speed
<p>
- = decrease speed
   </td>
  </tr>
</table>



### Remote Device Mode

Remote device mode allows PWM Dimmer switches to control remote devices. With remote device mode enabled, each button controls a different device. Note that dimmer switches with toggle-style down/up buttons have limited functionality as remote device mode switches because you can not push the down and up buttons simultaneously.

To include remote device mode support in the build, define USE_PWM_DIMMER_REMOTE in your user_config_override. Remote device mode support requires device group support so USE_DEVICE_GROUPS is automatically defined if USE_PWM_DIMMER_REMOTE is defined. Remote device mode support adds 0.7K to the code size in addition to the code size required for device groups support.

To enable remote device mode, set Option71 to 1. Each remote device must be running firmware with device group support and have remote device support enabled. The remote devices do not need to be built with PWM dimmer support nor do they need to be switches.

If a remote device is a PWM dimmer, the device acts like a 3-way dimmer switch would and may or may not have a load connected to it. It’s also possible to use a PWM dimmer switch without a load to act as a wall switch to control the power, brightness and color of one or more smart lights with Tasmota with device group support loaded on them.

With remote device mode enabled, button 1 is the power button for the local device while buttons 2 and 3 are the power buttons for remote devices. Group names for buttons 2 and 3 are set by the GroupTopic2 and GroupTopic3 commands respectively. Note that the button numbers are defined by the module template and can be in any physical order on the switch (button 1 can be defined as the top button, the middle button or the bottom button).

Pressing and releasing a power button toggles the power on all devices in the group assigned to the button. When the power is on, holding the button alternately increases or decreases the brightness. When the power is off, holding the button turns the power on at a temporary brightness of the low level set by the BriPreset command (default =10).

While holding a power button, the other two buttons act like the down and up buttons for the remote device. All the functions performed by the down and up buttons in non-remote device mode are available in remote device mode. While holding button 1, button 2 performs the functions of the down button and button 3 performs the functions of the up button. While holding button 2, button 1 performs the functions of the down button and button 3 performs the functions of the up button. While holding button 3, button 1 performs the functions of the down button and button 2 performs the functions of the up button.
