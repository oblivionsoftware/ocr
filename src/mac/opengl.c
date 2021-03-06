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
#include "ocr/window.h"

struct ocr_gl_context {
    int dummy;
};

ocr_gl_context_t *ocr_gl_context_create(ocr_pool_t *pool, ocr_window_t *window)
{
    ocr_gl_context_t *ctx = ocr_pool_alloc(pool, sizeof(*ctx));
    ctx->dummy = window ? 1 : 0;

    return ctx;
}


void ocr_gl_context_destroy(ocr_gl_context_t *ctx)
{
    OCR_INFO("destroying context: %d", ctx->dummy);
}


void ocr_gl_context_present(ocr_gl_context_t *ctx)
{
    OCR_INFO("presenting context: %d", ctx->dummy);
}

