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
#include "ocr/pool.h"
#include "ocr/windows/window.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094

struct ocr_gl_context {
    HDC hdc;
    HGLRC hglrc;
};

static HGLRC (*wglCreateContextAttribsARB)(HDC, HGLRC, const int*);

ocr_gl_context_t *ocr_gl_context_create(ocr_pool_t *pool, ocr_window_t *window)
{
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(pfd),
        .nVersion = 1,
        .dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cDepthBits = 32,
        .iLayerType = PFD_MAIN_PLANE
    };

    HDC hdc = GetDC(window->handle);

    int pixel_format = ChoosePixelFormat(hdc, &pfd);
    if (pixel_format == 0) {
        OCR_ERROR("unable to choose pixel format");
        return NULL;
    }

    if (!SetPixelFormat(hdc, pixel_format, &pfd)) {
        OCR_ERROR("unable to set pixel format");
        return NULL;
    }

    HGLRC temp = wglCreateContext(hdc);
    wglMakeCurrent(hdc, temp);

    wglCreateContextAttribsARB = (void*)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB) {
        OCR_ERROR("unable to create advanced opengl context");

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temp);
        return NULL;
    }

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(temp);
    wglMakeCurrent(hdc, hglrc);

    ocr_gl_context_t *ctx = ocr_pool_alloc(pool, sizeof(*ctx));
    ctx->hdc = hdc;
    ctx->hglrc = hglrc;

    OCR_INFO("initialized OpenGL version: %s", glGetString(GL_VERSION));

    return ctx;
}


void ocr_gl_context_destroy(ocr_gl_context_t *ctx)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(ctx->hglrc);
}


void ocr_gl_context_present(ocr_gl_context_t *ctx)
{
    SwapBuffers(ctx->hdc);
}

