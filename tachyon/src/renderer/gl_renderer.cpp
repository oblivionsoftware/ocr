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
#include "tachyon/renderer/command_buffer.h"
#include "tachyon/renderer/gl_program.h"
#include "tachyon/renderer/gl_shader.h"
#include "tachyon/renderer/opengl.h"

namespace tachyon {

GlRenderer::GlRenderer(std::unique_ptr<GlContext> context, u32 width, u32 height)
    : _context {std::move(context)},
      _commandBuffer {1024 * 1024 * 10},
      _width {width},
      _height {height}
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        TACHYON_THROW("glewInit failed: %s", glewGetErrorString(glewError));
    }

    glViewport(0, 0, _width, _height);
}

void GlRenderer::flush()
{
    CommandIterator itr {_commandBuffer};
    while (itr.next()) {
        switch (itr.type()) {

        case CommandType::Clear: {
            auto cmd = itr.command<ClearCommand>();

            glClearColor(cmd->r, cmd->g, cmd->b, cmd->a);
            glClear(GL_COLOR_BUFFER_BIT);
        } break;

        default:
            TACHYON_THROW("unsupported command: %d", itr.type());
        }
    }

    _commandBuffer.clear();
}

void GlRenderer::present()
{
    flush();
    _context->present();
}

u32 GlRenderer::loadTexture(const Image &image)
{
    _textures.emplace_back(image);

    return static_cast<u32>(_textures.size() - 1);
}

}
