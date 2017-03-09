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

namespace ocr {

struct Timer::Impl {
    u64 start;
};

static u64 ns_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    return ts.tv_nsec + ts.tv_sec * 1000000;
}

Timer::Timer()
        : _impl {new Impl()} {

    _impl->start = ns_time();
}

void Timer::reset() {
    _impl->start = ns_time();
}

r32 Timer::time() {
    u64 elapsed = ns_time() - _impl->start;
    return elapsed / 1000000.0f;
}

}
