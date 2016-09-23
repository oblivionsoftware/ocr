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

#include <SDL2/SDL.h>

#include "tachyon/core/common.h"

namespace tachyon {

/**
 * SDL platform implementation.
 */
class SdlPlatform {
public:

    /**
     * Initializes SDL with the specified window title and dimensions.
     *
     * @param title The window title.
     * @param width The window width.
     * @param height The window height.
     */
    SdlPlatform(const char *title, u32 width, u32 height);

    /**
     * Cleans up SDL.
     */
    ~SdlPlatform();

    /**
     * Runs the platform.
     */
    void run();

private:

    SDL_Window *_window;

    bool _running {false};

};

}
