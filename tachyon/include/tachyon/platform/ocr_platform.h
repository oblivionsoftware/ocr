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

#include <ocr/ocr.h>

#include "tachyon/core/common.h"
#include "tachyon/renderer/renderer.h"

namespace tachyon {

/**
 * OCR platform implementation.
 */
class OcrPlatform {
public:

    /**
     * Initializes OCR with the specified window title and dimensions.
     *
     * @param title The window title.
     * @param rendererType The type of renderer to use.
     * @param width The window width.
     * @param height The window height.
     */
    OcrPlatform(const char *title, RendererType rendererType, u32 width, u32 height);

    /**
     * Cleans up the platform.
     */
    ~OcrPlatform();

    /**
     * Runs the platform.
     */
    void run();

private:

    ocr_pool_t *_pool;

    ocr_window_t *_window;

    ocr_timer_t *_timer;

    RendererType _rendererType;

    u32 _width;

    u32 _height;

    bool _running {false};

};


}
