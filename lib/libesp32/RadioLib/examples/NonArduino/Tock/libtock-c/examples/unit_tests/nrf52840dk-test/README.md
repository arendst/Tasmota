# nRF52840 system test

This test is designed to execute most functions provided by the nRF52840-DK.

To run, it requires certain connections:

## GPIO

Register 0:
```
P4.09 -> P4.10
```

Register 1:
```
P3.01 -> P3.02
```

## LED

```
P24.03 -> P3.07
P24.04 -> P3.08
P24.05 -> P4.01
P24.06 -> P4.02
```

## Button

```
P24.01 -> R2k2 -> P3.03
P24.02 -> R2k2 -> P3.04
P24.14 -> R2k2 -> P3.05
P24.15 -> R2k2 -> P3.06
```

## Expected results:

If you load this app along with the `unit_test_supervisor` in `examples/services/`,
you should see something like the following console output:

```
NRF52 HW INFO: Variant: AAC0, Part: N52840, Package: QI, Ram: K256, Flash: K1024
1.000: basic_gpio0              [✓]
1.001: basic_gpio1              [✓]
1.002: gpio0_int_raising        [✓]
1.003: gpio0_int_falling        [✓]
1.004: gpio0_int_both           [✓]
1.005: gpio1_int_raising        [✓]
1.006: gpio1_int_falling        [✓]
1.007: gpio1_int_both           [✓]
1.008: leds_start_off           [✓]
1.009: switch_led1              [✓]
1.010: switch_led2              [✓]
1.011: switch_led3              [✓]
1.012: switch_led4              [✓]
1.013: toggle_led1              [✓]
1.014: toggle_led2              [✓]
1.015: toggle_led3              [✓]
1.016: toggle_led4              [✓]
1.017: buttons_start_off        [✓]
1.018: push_button1             [✓]
1.019: push_button2             [✓]
1.020: push_button3             [✓]
1.021: push_button4             [✓]
1.022: button1_int              [✓]
1.023: two_buttons_int          [✓]
1.024: disable_button_int       [✓]
Summary 1: [25/25] Passed, [0/25] Failed, [0/25] Incomplete
```
