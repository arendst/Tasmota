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
#include "Button.h"
#include <Arduino.h>

Button::Button(uint8_t pin, uint8_t invert, uint32_t dbTime)
    : _pin(pin)
    , _invert(invert)
    , _dbTime(dbTime)
    , _state(RELEASED)
    , _transitioned(false)
{
    pinMode(_pin, INPUT_PULLUP);
    _transitionedAt = _lastTransitionedAt = millis();
}

uint8_t Button::read()
{
    int pressed = digitalRead(_pin);
    if (_invert)
    {
        pressed = !pressed;
    }
    _lastRead = millis();
    _transitioned = transitionTo(pressed ? PRESSED : RELEASED);
    return pressed;
}

bool Button::transitionTo(_State newState)
{
    if (newState == _state)
    {
        return false;
    }
    if (_lastRead - _transitionedAt < _dbTime)
    {
        // transition is forbidden for debounce.
        return false;
    }
    _lastTransitionedAt = _transitionedAt;
    _transitionedAt = _lastRead;
    _state = newState;
    return true;
}

uint8_t Button::wasPressed()
{
    return isPressed() && _transitioned;
}

uint8_t Button::wasReleased()
{
    return isReleased() && _transitioned;
}

uint8_t Button::pressedFor(uint32_t ms)
{
    return isPressed() && (_lastRead - _transitionedAt) >= ms;
}

uint8_t Button::releasedFor(uint32_t ms)
{
    return isReleased() && (_lastRead - _transitionedAt) >= ms;
}

uint8_t Button::wasReleasefor(uint32_t ms)
{
    return isPressed() && (_transitionedAt - _lastTransitionedAt) >= ms;
}

uint32_t Button::lastChange()
{
    return _transitionedAt;
}
