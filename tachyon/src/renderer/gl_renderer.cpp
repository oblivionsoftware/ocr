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

#include "tachyon/renderer/gl_renderer.h"

#include "tachyon/core/exception.h"
#include "tachyon/renderer/opengl.h"

namespace tachyon {

GlRenderer::GlRenderer(GlContext *context, u32 width, u32 height)
    : _context {context}
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        TACHYON_THROW("glewInit failed: %s", glewGetErrorString(glewError));
    }

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

GlRenderer::~GlRenderer()
{
}

void GlRenderer::flush()
{
}

void GlRenderer::present()
{
    glClear(GL_COLOR_BUFFER_BIT);
    _context->present();
}

}