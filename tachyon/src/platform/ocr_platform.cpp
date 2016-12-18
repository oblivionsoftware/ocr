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

#include "tachyon/platform/ocr_platform.h"

#include <memory>

#include <ocr/ocr.h>

#include "tachyon/core/context.h"
#include "tachyon/core/exception.h"
#include "tachyon/renderer/gl_renderer.h"

#if TACHYON_PLATFORM == TACHYON_PLATFORM_WINDOWS
    #include <ocr/windows/window.h>
    #include "tachyon/renderer/dx11_renderer.h"
#endif

namespace tachyon {

/**
 * OpenGL context implementation using OCR.
 */
class OcrGlContext : public GlContext {
public:

    OcrGlContext(ocr_pool_t *pool, ocr_window_t *window);

    ~OcrGlContext();

    virtual void present() override;

private:

    ocr_gl_context_t *_context;

};

OcrPlatform::OcrPlatform(const char *title, RendererType rendererType, u32 width, u32 height)
    : _rendererType {rendererType},
      _width {width},
      _height {height}
{
    _pool = ocr_pool_create(ocr_mb(64), nullptr);

    ocr_window_settings_t settings;
    settings.width = width;
    settings.height = height;
    settings.title = title;

    _window = ocr_window_create(_pool, &settings);
}

OcrPlatform::~OcrPlatform()
{
    ocr_window_destroy(_window);
    ocr_pool_destroy(_pool);
}

static std::unique_ptr<Renderer> createRenderer(ocr_pool_t *pool, ocr_window_t *window, RendererType rendererType, u32 width, u32 height)
{
    switch (rendererType) {

    case RendererType::OpenGL: {
        auto glContext {std::make_unique<OcrGlContext>(pool, window)};
        return std::make_unique<GlRenderer>(std::move(glContext), width, height);
    } break;

#if TACHYON_PLATFORM == TACHYON_PLATFORM_WINDOWS

    case RendererType::DirectX11: {
        return std::make_unique<DX11Renderer>(window->handle, width, height);
    } break;

#endif

    default:
        TACHYON_THROW("unsupported renderer type");
    }
}

void OcrPlatform::run()
{
    _running = true;

    u32 start {0};

    Context context {createRenderer(_pool, _window, _rendererType, _width, _height)};

    while (_running) {
        r32 dt {0.0f};
        start = 0;

        ocr_window_do_events(_window);

        if (ocr_window_is_closed(_window)) {
            _running = false;
        }

        context.frame(dt);
    }
}

OcrGlContext::OcrGlContext(ocr_pool_t *pool, ocr_window_t *window)
{
    _context = ocr_gl_context_create(pool, window);
}

OcrGlContext::~OcrGlContext()
{
    ocr_gl_context_destroy(_context);
}

void OcrGlContext::present()
{
    ocr_gl_context_present(_context);
}

}
