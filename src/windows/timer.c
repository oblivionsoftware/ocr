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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ocr/log.h"

struct ocr_timer {
    LARGE_INTEGER start;
    LARGE_INTEGER frequency;
};


ocr_timer_t *ocr_timer_create(ocr_pool_t *pool)
{
    ocr_timer_t *timer = ocr_pool_alloc(pool, sizeof(*timer));
    QueryPerformanceFrequency(&timer->frequency);
    QueryPerformanceCounter(&timer->start);

    return timer;
}


void ocr_timer_reset(ocr_timer_t *timer)
{
    QueryPerformanceCounter(&timer->start);
}


r32 ocr_timer_time(ocr_timer_t *timer)
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    return (r32)(time.QuadPart - timer->start.QuadPart) / timer->frequency.QuadPart;
}

