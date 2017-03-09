/*
 * Copyright 2016 Jeff Upton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ocr/timer.h"

#include "ocr/log.h"
#include "ocr/windows.h"

namespace ocr {

class Timer::Impl {
public:

    Impl()
    {
        QueryPerformanceFrequency(&_frequency);
        QueryPerformanceCounter(&_start);
    }

    void reset()
    {
        QueryPerformanceCounter(&_start);
    }

    r32 time() const
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);

        return (r32)(time.QuadPart - _start.QuadPart) / _frequency.QuadPart;
    }

private:

    LARGE_INTEGER _start;
    LARGE_INTEGER _frequency;
};

Timer::Timer()
    : _impl{std::make_unique<Impl>()}
{
}


void Timer::reset()
{
    _impl->reset();
}


r32 Timer::time() const
{
    return _impl->time();
}

}
