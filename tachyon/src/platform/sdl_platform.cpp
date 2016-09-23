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

#include "tachyon/platform/sdl_platform.h"

#include "tachyon/core/context.h"
#include "tachyon/core/exception.h"

namespace tachyon {

SdlPlatform::SdlPlatform(const char *title, u32 width, u32 height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        TACHYON_THROW("SDL initialization failed: %s", SDL_GetError());
    }

    u32 flags = SDL_WINDOW_OPENGL;
    _window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    if (!_window) {
        TACHYON_THROW("SDL Window Creation failed: %s", SDL_GetError());
    }
}

SdlPlatform::~SdlPlatform()
{
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void SdlPlatform::run()
{
    _running = true;

    u32 start = SDL_GetTicks();

    Context context;

    while (_running) {
        r32 dt = (SDL_GetTicks() - start) / 1000.0f;
        start = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                _running = false;
                break;
            }
        }

        context.frame(dt);
    }
}

}
