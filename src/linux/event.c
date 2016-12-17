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

#include <sys/epoll.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

#include "ocr/log.h"

struct ocr_event_loop {
    int efd;
    bool running;
};


ocr_event_loop_t *ocr_event_loop_create(ocr_pool_t *pool)
{
    ocr_event_loop_t *loop = ocr_pool_alloc(pool, sizeof(ocr_event_loop_t));
    if ((loop->efd = epoll_create1(0)) == -1) {
        OCR_ERROR("epoll_create1 failed: %s", strerror(errno));
        return NULL;
    }

    return loop;
}


void ocr_event_loop_destroy(ocr_event_loop_t *loop)
{
    if (loop) {
        close(loop->efd);
    }
}


void ocr_event_loop_run(ocr_event_loop_t *loop)
{
    loop->running = true;

    struct epoll_event events[64];

    for (int i = 0; i < 10; ++i) {
        int event_count = epoll_wait(loop->efd, events, 64, 100);
        OCR_INFO("found %d events", event_count);
    }
}
