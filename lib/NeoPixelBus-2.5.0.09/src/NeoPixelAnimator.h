/*-------------------------------------------------------------------------
NeoPixelAnimator provides animation timing support.

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#pragma once

#include <Arduino.h>
#include "internal/NeoEase.h"

enum AnimationState
{
    AnimationState_Started,
    AnimationState_Progress,
    AnimationState_Completed
};

struct AnimationParam
{
    float progress;
    uint16_t index;
    AnimationState state;
};

#ifdef ARDUINO_ARCH_AVR

typedef void(*AnimUpdateCallback)(const AnimationParam& param);

#else

#undef max
#undef min
#include <functional>
typedef std::function<void(const AnimationParam& param)> AnimUpdateCallback;

#endif


#define NEO_MILLISECONDS        1    // ~65 seconds max duration, ms updates
#define NEO_CENTISECONDS       10    // ~10.9 minutes max duration, centisecond updates
#define NEO_DECISECONDS       100    // ~1.8 hours max duration, decisecond updates
#define NEO_SECONDS          1000    // ~18.2 hours max duration, second updates
#define NEO_DECASECONDS     10000    // ~7.5 days, 10 second updates

class NeoPixelAnimator
{
public:
    NeoPixelAnimator(uint16_t countAnimations, uint16_t timeScale = NEO_MILLISECONDS);
    ~NeoPixelAnimator();

    bool IsAnimating() const
    {
        return _activeAnimations > 0;
    }


    bool NextAvailableAnimation(uint16_t* indexAvailable, uint16_t indexStart = 0);

    void StartAnimation(uint16_t indexAnimation, uint16_t duration, AnimUpdateCallback animUpdate);
    void StopAnimation(uint16_t indexAnimation);
    void StopAll();

    void RestartAnimation(uint16_t indexAnimation)
    {
        if (indexAnimation >= _countAnimations || _animations[indexAnimation]._duration == 0)
        {
            return;
        }

        StartAnimation(indexAnimation, _animations[indexAnimation]._duration, (_animations[indexAnimation]._fnCallback));
    }

    bool IsAnimationActive(uint16_t indexAnimation) const
    {
        if (indexAnimation >= _countAnimations)
        {
            return false;
        }
        return (IsAnimating() && _animations[indexAnimation]._remaining != 0);
    }

    uint16_t AnimationDuration(uint16_t indexAnimation)
    {
        if (indexAnimation >= _countAnimations)
        {
            return 0;
        }
        return _animations[indexAnimation]._duration;
    }

    void ChangeAnimationDuration(uint16_t indexAnimation, uint16_t newDuration);

    void UpdateAnimations();

    bool IsPaused()
    {
        return (!_isRunning);
    }

    void Pause()
    {
        _isRunning = false;
    }

    void Resume()
    {
        _isRunning = true;
        _animationLastTick = millis();
    }

    uint16_t getTimeScale()
    {
        return _timeScale;
    }

    void setTimeScale(uint16_t timeScale)
    {
        _timeScale = (timeScale < 1) ? (1) : (timeScale > 32768) ? 32768 : timeScale;
    }

private:
    struct AnimationContext
    {
        AnimationContext() :
            _duration(0),
            _remaining(0),
            _fnCallback(NULL)
        {}

        void StartAnimation(uint16_t duration, AnimUpdateCallback animUpdate)
        {
            _duration = duration;
            _remaining = duration;
            _fnCallback = animUpdate;
            
        }

        void StopAnimation()
        {
            _remaining = 0;
        }

        float CurrentProgress()
        {
            return (float)(_duration - _remaining) / (float)_duration;
        }

        uint16_t _duration;
        uint16_t _remaining;
       
        AnimUpdateCallback _fnCallback;
    };

    uint16_t _countAnimations;
    AnimationContext* _animations;
    uint32_t _animationLastTick;
    uint16_t _activeAnimations;
    uint16_t _timeScale;
    bool _isRunning;
};
