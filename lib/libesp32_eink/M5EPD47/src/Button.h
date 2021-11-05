/*
 * The MIT License
 * 
 * Copyright (c) IKEDA Yasuyuki
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

/**
 * Button is a state machine transitioning between PRESSED and RELEASED states.
 *
 * This provides "debounce" feature.
 * Debounce treats input changes in short time as noise and ignore them.
 * This means that debounce time is the least time where the state machine
 * must stay in the same state.
 */
class Button
{
public:
    /**
     * ctor.
     *
     * @param pin the pin number of GPIO.
     * @param invert `false` for HIGH=pressed LOW=released, `true` for LOG=pressed HIGH=released.
     * @param dbTime debounce time (milliseconds). Do not transition in this time since the last transition.
     */
    Button(uint8_t pin, uint8_t invert, uint32_t dbTime);

    /**
     * Returns the current input of the button.
     * Also updates the internal state machine.
     *
     * @return non-0 for pressed, 0 for released.
     */
    uint8_t read();

    /**
     * Return non-0 if in PRESSED state
     * @return non-0 for pressed, 0 for released.
     */
    uint8_t isPressed()
    {
         return (_state == PRESSED);
    }

    /**
     * Return non-0 if in RELEASED state
     * @return non-0 for released, 0 for pressed.
     */
    uint8_t isReleased()
    {
         return (_state == RELEASED);
    }

    /**
     * Return non-0 if transitioned to PRESSED state in the last `read()`.
     * @return non-0 for pressed, 0 for otherwise.
     */
    uint8_t wasPressed();

    /**
     * Return non-0 if transitioned to RELEASED state in the last `read()`.
     * @return non-0 for released, 0 for otherwise.
     */
    uint8_t wasReleased();

    /**
     * Return non-0 if now in PRESSED state for more than `ms` milliseconds.
     * @param ms milliseconds
     * @return non-0 for pressed for specified time, 0 for otherwise.
     */
    uint8_t pressedFor(uint32_t ms);

    /**
     * Return non-0 if now in RELEASED state for more than `ms` milliseconds.
     * @param ms milliseconds
     * @return non-0 for released for specified time.
     */
    uint8_t releasedFor(uint32_t ms);

    /**
     * Return non-0 if now in PRESSED state and was in RELEASED state for more than `ms` milliseconds.
     * @param ms milliseconds
     * @return non-0 for pressed and was released for specified time.
     */
    uint8_t wasReleasefor(uint32_t ms);

    /**
     * @return last changed time (in `millis()` value)
     */
    uint32_t lastChange();

private:
    uint8_t _pin;
    uint8_t _invert;
    uint32_t _dbTime;

    enum _State {
        PRESSED,
        RELEASED,
    };
    _State _state;
    bool _transitioned;
    uint32_t _lastRead;
    uint32_t _transitionedAt;
    uint32_t _lastTransitionedAt;

    bool transitionTo(_State newState);
};
#endif /* __BUTTON_H__ */
