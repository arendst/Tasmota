# Stepper library

## Methods

### `stepper()`

This function creates a new instance of the Stepper class that represents a particular stepper motor attached to your Arduino board. Use it at the top of your sketch, above `setup()` and `loop()`. The number of parameters depends on how you've wired your motor, either using two or four pins of the Arduino board.

#### Syntax

```
Stepper(steps, pin1, pin2)
Stepper(steps, pin1, pin2, pin3, pin4)
```

#### Parameters

* `steps`: the number of steps in one revolution of your motor. If your motor gives the number of degrees per step, divide that number into 360 to get the number of steps (e.g. 360 / 3.6 gives 100 steps).
* `pin1, pin2`: two pins that are attached to the motor.
* `pin3, pin4`: the last two pins attached to the motor, if it's connected to four pins.

#### Returns

A new instance of the Stepper motor class.

#### Example

```
Stepper myStepper = Stepper(100, 5, 6);
```

#### See also

* [setSpeed()](#setspeed)
* [step()](#step)

### `setSpeed()`

This function sets the motor speed in rotations per minute (RPMs). This function doesn't make the motor turn, just sets the speed at which it will when you call step().

#### Syntax

```
setSpeed(rpms)
```

#### Parameters

* `rpms`: the speed at which the motor should turn in rotations per minute (positive long).

#### Returns

None.

#### See also

* [Stepper()](#stepper)
* [step()](#step)

### `step()`

This function turns the motor a specific number of steps, at a speed determined by the most recent call to `setSpeed()`. This function is blocking; that is, it will wait until the motor has finished moving to pass control to the next line in your sketch. For example, if you set the speed to, say, 1 RPM and called step(100) on a 100-step motor, this function would take a full minute to run. For better control, keep the speed high and only go a few steps with each call to `step()`.

#### Syntax

```
step(steps)
```

#### Parameters

* `steps`: the number of steps to turn the motor. Positive integer to turn one direction, negative integer to turn the other.

#### Returns

None.

#### See also

* [Stepper()](#stepper)
* [setSpeed()](#setspeed)