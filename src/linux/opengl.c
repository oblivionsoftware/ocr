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

#include <GL/glx.h>

#include "ocr/linux/window.h"
#include "ocr/log.h"
#include "ocr/pool.h"

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

struct ocr_gl_context {
    GLXContext context;
    ocr_window_t *window;
};

#define gl_load_proc(type, name)                                         \
    (name) = (type)glXGetProcAddress((const GLubyte*)#name)

static ocr_status_t gl_load_extensions(void)
{
    gl_load_proc(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    gl_load_proc(PFNGLUSEPROGRAMPROC, glUseProgram);
    gl_load_proc(PFNGLGENBUFFERSPROC, glGenBuffers);
    gl_load_proc(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    gl_load_proc(PFNGLBINDBUFFERPROC, glBindBuffer);
    gl_load_proc(PFNGLBUFFERDATAPROC, glBufferData);
    gl_load_proc(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
    gl_load_proc(PFNGLMAPBUFFERPROC, glMapBuffer);
    gl_load_proc(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);
    gl_load_proc(PFNGLATTACHSHADERPROC, glAttachShader);
    gl_load_proc(PFNGLLINKPROGRAMPROC, glLinkProgram);
    gl_load_proc(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    gl_load_proc(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    gl_load_proc(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    gl_load_proc(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    gl_load_proc(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i);
    gl_load_proc(PFNGLPROGRAMUNIFORMMATRIX4FVPROC, glProgramUniformMatrix4fv);
    gl_load_proc(PFNGLCREATESHADERPROC, glCreateShader);
    gl_load_proc(PFNGLSHADERSOURCEPROC, glShaderSource);
    gl_load_proc(PFNGLCOMPILESHADERPROC, glCompileShader);
    gl_load_proc(PFNGLGETSHADERIVPROC, glGetShaderiv);
    gl_load_proc(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    gl_load_proc(PFNGLDELETESHADERPROC, glDeleteShader);
    gl_load_proc(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    gl_load_proc(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
    gl_load_proc(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    gl_load_proc(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    gl_load_proc(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);

    return OCR_OK;
}


ocr_gl_context_t *ocr_gl_context_create(ocr_pool_t *pool, ocr_window_t *window)
{
    GLXFBConfig *fb_configs = NULL;
    int fb_config_count = 0;

    fb_configs = glXGetFBConfigs(window->display, 0, &fb_config_count);

    int visual_id = 0;
    GLXFBConfig fb_config = fb_configs[0];
    glXGetFBConfigAttrib(window->display, fb_config, GLX_VISUAL_ID, &visual_id);


    GLXContext context = glXCreateNewContext(window->display, fb_config, GLX_RGBA_TYPE, 0, true);

    glXMakeContextCurrent(window->display, window->window, window->window, context);

    gl_load_extensions();

    ocr_gl_context_t *ctx = ocr_pool_alloc(pool, sizeof(*ctx));
    ctx->window = window;
    ctx->context = context;

    return ctx;
}


void ocr_gl_context_destroy(ocr_gl_context_t *ctx)
{
    ocr_window_t *window = ctx->window;

    glXDestroyContext(window->display, ctx->context);
}


void ocr_gl_context_present(ocr_gl_context_t *ctx)
{
    ocr_window_t *window = ctx->window;

    glXSwapBuffers(window->display, window->window);
}

