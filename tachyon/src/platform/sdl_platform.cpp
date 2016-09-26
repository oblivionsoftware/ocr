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
#include "tachyon/renderer/gl_renderer.h"

namespace tachyon {

/**
 * OpenGL context implementation using SDL.
 */
class SdlGlContext : public GlContext {
public:

    SdlGlContext(SDL_Window *window, u32 major, u32 minor);

    ~SdlGlContext();

    virtual void present() override;

private:

    SDL_Window *_window;

    SDL_GLContext _context;

};

SdlPlatform::SdlPlatform(const char *title, u32 width, u32 height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        TACHYON_THROW("SDL initialization failed: %s", SDL_GetError());
    }

    u32 flags {SDL_WINDOW_OPENGL};
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

    u32 start {SDL_GetTicks()};

    Context context;
    SdlGlContext glContext {_window, 4, 1};
    GlRenderer glRenderer {&glContext, 1280, 720};

    while (_running) {
        r32 dt {(SDL_GetTicks() - start) / 1000.0f};
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
        glRenderer.present();
    }
}

SdlGlContext::SdlGlContext(SDL_Window *window, u32 major, u32 minor)
    : _window {window}
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    _context = SDL_GL_CreateContext(_window);
    if (!_context) {
        TACHYON_THROW("OpenGL Context creation failed: %s", SDL_GetError());
    }
}

SdlGlContext::~SdlGlContext()
{
    SDL_GL_DeleteContext(_context);
}

void SdlGlContext::present()
{
    SDL_GL_SwapWindow(_window);
}

}