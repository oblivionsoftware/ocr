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

#include <string.h>

#include "ocr/log.h"

namespace ocr {

class EventLoop::Impl {
public:

    Impl()
    {
        if ((_efd = epoll_create1(0)) == -1) {
            OCR_ERROR("epoll_create1 failed: %s", ::strerror(errno));
            throw std::runtime_error("epoll_create1 failed");
        }
    }

    ~Impl()
    {
        close(_efd);
    }

    void run()
    {
        _running = true;

        struct epoll_event events[64];

        for (int i = 0; i < 10; ++i) {
            int event_count = epoll_wait(_efd, events, 64, 100);
            OCR_INFO("found %d events", event_count);
        }
    }

private:

    int _efd;
    bool _running;
};


EventLoop::EventLoop()
    : _impl{new Impl()}
{
}


EventLoop::~EventLoop()
{
}


void EventLoop::run()
{
    _impl->run();
}

}
