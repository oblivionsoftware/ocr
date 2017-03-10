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

struct ocr_gl_context {
    GLXContext context;
    ocr_window_t *window;
};

ocr_gl_context_t *ocr_gl_context_create(ocr_pool_t *pool, ocr_window_t *window)
{
    int fb_config_count = 0;
    GLXFBConfig *fb_configs = glXGetFBConfigs(window->display, 0, &fb_config_count);

    int visual_id = 0;
    GLXFBConfig fb_config = fb_configs[0];
    glXGetFBConfigAttrib(window->display, fb_config, GLX_VISUAL_ID, &visual_id);

    GLXContext context = glXCreateNewContext(window->display, fb_config, GLX_RGBA_TYPE, 0, true);

    free(fb_configs);

    glXMakeContextCurrent(window->display, window->window, window->window, context);

    if (gl3wInit() != 0) {
        OCR_ERROR("unable to load gl extensions");
        goto out_context;
    }

    ocr_gl_context_t *ctx = ocr_pool_alloc(pool, sizeof(*ctx));
    ctx->window = window;
    ctx->context = context;

    return ctx;

out_context:

    glXDestroyContext(window->display, context);
    return NULL;
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

