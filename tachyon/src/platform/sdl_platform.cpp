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

#include <memory>

#include "SDL_syswm.h"

#include "tachyon/core/context.h"
#include "tachyon/core/exception.h"
#include "tachyon/renderer/dx11_renderer.h"
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

SdlPlatform::SdlPlatform(const char *title, RendererType rendererType, u32 width, u32 height)
    : _rendererType {rendererType},
      _width {width},
      _height {height}
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        TACHYON_THROW("SDL initialization failed: %s", SDL_GetError());
    }

    u32 flags {0};
    if (rendererType == RendererType::OpenGL) {
        flags |= SDL_WINDOW_OPENGL;
    }

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

static std::unique_ptr<Renderer> createRenderer(SDL_Window *window, RendererType rendererType, u32 width, u32 height)
{
    switch (rendererType) {

    case RendererType::OpenGL: {
        auto glContext {std::make_unique<SdlGlContext>(window, 4, 1)};
        return std::make_unique<GlRenderer>(std::move(glContext), width, height);
    } break;

    case RendererType::DirectX11: {
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);

        if (!SDL_GetWindowWMInfo(window, &info)) {
            TACHYON_THROW("SDL_GetWindowWMInfo failed: %s", SDL_GetError());
        }

        return std::make_unique<DX11Renderer>(info.info.win.window, width, height);
    } break;

    default:
        TACHYON_THROW("unsupported renderer type");
    }
}

void SdlPlatform::run()
{
    _running = true;

    u32 start {SDL_GetTicks()};

    Context context {createRenderer(_window, _rendererType, _width, _height)};

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

    SDL_GL_SetSwapInterval(1);
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
