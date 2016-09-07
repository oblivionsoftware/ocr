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

#include "ocr/event.h"

#if OCR_PLATFORM == OCR_PLATFORM_MAC

#include <sys/event.h>
#include <unistd.h>

#include "ocr/log.h"

struct ocr_event_loop {
    int kq;
    bool running;
};


ocr_status_t ocr_event_loop_create(ocr_pool_t *pool, ocr_event_loop_t **loop_out)
{
    ocr_event_loop_t *loop = ocr_alloc(pool, sizeof(ocr_event_loop_t));
    if ((loop->kq = kqueue()) == -1) {
        OCR_ERROR("kqueue failed");
        return OCR_GENERAL_ERROR;
    }

    *loop_out = loop;

    return OCR_OK;
}


void ocr_event_loop_destroy(ocr_event_loop_t *loop)
{
    if (loop) {
        close(loop->kq);
    }
}


void ocr_event_loop_run(ocr_event_loop_t *loop)
{
    loop->running = true;

    struct kevent change;
    EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 5000, 0);

    for (int i = 0; i < 10; ++i) {
        struct kevent event;

        struct timespec timeout;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 100000000;

        int event_count = kevent(loop->kq, &change, 1, &event, 1, &timeout);
        if (event_count > 0) {
            OCR_INFO("got %d events", event_count);
        } else {
            OCR_INFO("no events from kqueue");
        }
    }
}


#endif
