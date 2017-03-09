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

#pragma once

#include <memory>

#include "ocr/common.h"
#include "ocr/pool.h"

namespace ocr {

struct WindowSettings {
    u32 width;
    u32 height;
    const char *title;
};

class Window {

    friend class GlContext;

public:

    Window(const WindowSettings &settings);

    ~Window();

    void doEvents();

    bool isClosed();

private:

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

}

