GPIO Test App
=============

This app tests that pin output, input, and interrupts work. To use,
uncomment one of the three function calls in `main()`.

`gpio_test` uses the first GPIO pin passed to the `gpio.rs` capsule.
On imix, this is the header labeled "D2".

- **gpio_output**: Toggle the pin high and low: connect to an LED to test.
- **gpio_input**: Read the pin every 500 ms and print if it is 1 or 0.
- **gpio_interrupt**: Print a message any time the pin changes.
