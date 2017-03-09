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

#include "ocr/opengl.h"

#include "ocr/log.h"
#include "ocr/windows/window.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094

namespace ocr {

typedef HGLRC (*PFNWGLCREATECONTEXTATTRIBSARB)(HDC, HGLRC, const int*);
static PFNWGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB;

class GlContext::Impl {
public:

    Impl(const Window &window)
    {
        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFD_MAIN_PLANE;

        _hdc = GetDC(window.impl().handle());

        int pixel_format = ChoosePixelFormat(_hdc, &pfd);
        if (pixel_format == 0) {
            throw std::runtime_error("unable to choose pixel format");
        }

        if (!SetPixelFormat(_hdc, pixel_format, &pfd)) {
            throw std::runtime_error("unable to set pixel format");
        }

        HGLRC temp = wglCreateContext(_hdc);
        wglMakeCurrent(_hdc, temp);

        if (gl3wInit() != 0) {
            throw std::runtime_error("unable to load extensions");
        }

        wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARB>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (!wglCreateContextAttribsARB) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(temp);

            throw std::runtime_error("unable to create advanced opengl context");
        }

        int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 1,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0
        };

        _hglrc = wglCreateContextAttribsARB(_hdc, 0, attribs);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temp);
        wglMakeCurrent(_hdc, _hglrc);

        OCR_INFO("initialized OpenGL version: %s", glGetString(GL_VERSION));
    }

    ~Impl()
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(_hglrc);
    }

    void Impl::present()
    {
        SwapBuffers(_hdc);
    }

private:

    HDC _hdc;
    HGLRC _hglrc;
};

GlContext::GlContext(const Window& window)
    : _impl{std::make_unique<Impl>(window)}
{
}

GlContext::~GlContext()
{
}

void GlContext::present()
{
    _impl->present();
}

}
