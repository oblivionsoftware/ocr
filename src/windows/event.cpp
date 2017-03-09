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

#include "ocr/log.h"
#include "ocr/windows.h"

namespace ocr {

struct EventLoop::Impl {
    bool running;
    HANDLE completion_port;
};


EventLoop::EventLoop()
    : _impl{std::make_unique<Impl>()}
{
    _impl->completion_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)this, 1);
    if (_impl->completion_port == NULL) {
        throw std::runtime_error("unable to create completion port");
    }
}

EventLoop::~EventLoop()
{
    CloseHandle(_impl->completion_port);
}

void EventLoop::run()
{
    _impl->running = true;

    for (int i = 0; i < 10; ++i) {
        //while (loop->running) {
        DWORD byte_count;
        ULONG_PTR key;
        LPOVERLAPPED overlapped;
        if (GetQueuedCompletionStatus(_impl->completion_port, &byte_count, &key, &overlapped, 100)) {
            OCR_INFO("got completion status: %u", byte_count);
        } else {
            OCR_INFO("nothing from completion port");
        }
    }
}

}
