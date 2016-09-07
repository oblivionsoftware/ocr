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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ocr/log.h"

struct ocr_event_loop {
    bool running;
    HANDLE completion_port;
};


ocr_status_t ocr_event_loop_create(ocr_pool_t *pool, ocr_event_loop_t **loop_out)
{
    ocr_event_loop_t *loop = ocr_alloc(pool, sizeof(ocr_event_loop_t));
    loop->completion_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)loop, 1);

    if (loop->completion_port == NULL) {
        return OCR_GENERAL_ERROR;
    }

    *loop_out = loop;

    return OCR_OK;
}


void ocr_event_loop_destroy(ocr_event_loop_t *loop)
{
    if (loop) {
        CloseHandle(loop->completion_port);
        loop->completion_port = NULL;
    }
}


void ocr_event_loop_run(ocr_event_loop_t *loop)
{
    loop->running = true;

    for (int i = 0; i < 10; ++i) {
        //while (loop->running) {
        DWORD byte_count;
        ULONG_PTR key;
        LPOVERLAPPED overlapped;
        if (GetQueuedCompletionStatus(loop->completion_port, &byte_count, &key, &overlapped, 100)) {
            OCR_INFO("got completion status: %u", byte_count);
        } else {
            OCR_INFO("nothing from completion port");
        }
    }
}
