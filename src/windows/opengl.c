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

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLMAPBUFFERPROC glMapBuffer;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
static HGLRC (*wglCreateContextAttribsARB)(HDC, HGLRC, const int*);


struct ocr_gl_context {
    HDC hdc;
    HGLRC hglrc;
};

static void *gl_proc(const char *name)
{
    return (void*)wglGetProcAddress(name);
}

#define gl_load_proc(name)\
    (name) = (void*)wglGetProcAddress(#name)

static ocr_status_t gl_load_extensions(void)
{
    gl_load_proc(glCreateProgram);
    gl_load_proc(glUseProgram);
    gl_load_proc(glGenBuffers);
    gl_load_proc(glDeleteBuffers);
    gl_load_proc(glBindBuffer);
    gl_load_proc(glBufferData);
    gl_load_proc(glBufferSubData);
    gl_load_proc(glMapBuffer);
    gl_load_proc(glUnmapBuffer);
    gl_load_proc(glAttachShader);
    gl_load_proc(glLinkProgram);
    gl_load_proc(glGetProgramiv);
    gl_load_proc(glGetProgramInfoLog);
    gl_load_proc(glDeleteProgram);
    gl_load_proc(glGetUniformLocation);
    gl_load_proc(glProgramUniform1i);
    gl_load_proc(glProgramUniformMatrix4fv);
    gl_load_proc(glCreateShader);
    gl_load_proc(glShaderSource);
    gl_load_proc(glCompileShader);
    gl_load_proc(glGetShaderiv);
    gl_load_proc(glGetShaderInfoLog);
    gl_load_proc(glDeleteShader);
    gl_load_proc(glGenVertexArrays);
    gl_load_proc(glBindVertexArray);
    gl_load_proc(glVertexAttribPointer);
    gl_load_proc(glEnableVertexAttribArray);
    gl_load_proc(glDeleteVertexArrays);
    gl_load_proc(wglCreateContextAttribsARB);

    return OCR_OK;
}


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

    if (OCR_FAILED(gl_load_extensions())) {
        OCR_ERROR("unable to load extensions");
        return NULL;
    }

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

