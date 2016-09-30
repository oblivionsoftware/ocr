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
#include "tachyon/renderer/gl_buffer.h"
#include "tachyon/renderer/gl_program.h"
#include "tachyon/renderer/gl_shader.h"
#include "tachyon/renderer/gl_vertex_array.h"
#include "tachyon/renderer/opengl.h"

namespace tachyon {

const auto SPRITE_VERTEX_SHADER = R"(
#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 color;

out VertexOutput {
    vec2 texCoords;
    vec4 color;
} vertexOut;

uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(position, 1.0f);

    vertexOut.texCoords = texCoords;
    vertexOut.color = color;
}
)";

const auto SPRITE_FRAGMENT_SHADER = R"(
#version 400 core

out vec4 outColor;

uniform sampler2D diffuseTexture;

in VertexOutput {
    vec2 texCoords;
    vec4 color;
} fragIn;

void main()
{
    vec4 texColor = texture(diffuseTexture, fragIn.texCoords);
    outColor = fragIn.color * texColor;
}
)";

struct SpriteVertex {
    float x, y, z;
    float tx, tv;
    float r, g, b, a;
};

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

    GlVertexFormat spriteFormat {
        {3, GL_FLOAT},
        {2, GL_FLOAT},
        {4, GL_FLOAT}
    };

    _spriteVertexArray = std::make_unique<GlVertexArray>(spriteFormat, 4096, GlBufferUsage::Dynamic);
    _spriteProgram = std::make_unique<GlProgram>(SPRITE_VERTEX_SHADER, SPRITE_FRAGMENT_SHADER);
}

GlRenderer::~GlRenderer()
{
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

        case CommandType::DrawSprite: {
            //auto cmd = itr.command<DrawSprite>();
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

    glUseProgram(_spriteProgram->id());

    _context->present();
}

u32 GlRenderer::loadTexture(const Image &image)
{
    _textures.emplace_back(image);

    return static_cast<u32>(_textures.size());
}

}
