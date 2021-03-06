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

#include "ocr/window.h"

#include <assert.h>

#include "ocr/log.h"

struct ocr_window {
    bool closed;
};

ocr_window_t *ocr_window_create(ocr_pool_t *pool, ocr_window_settings_t *settings)
{
    assert(settings);

    ocr_window_t *window = ocr_pool_alloc(pool, sizeof(*window));
    window->closed = true;

    return window;
}


void ocr_window_destroy(ocr_window_t *window)
{
    assert(window);
}


void ocr_window_do_events(ocr_window_t *window)
{
    assert(window);
}


bool ocr_window_is_closed(ocr_window_t *window)
{
    return window->closed;
}
