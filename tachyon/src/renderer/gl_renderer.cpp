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
#include "tachyon/core/math.h"
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

uniform sampler2D texture0;

in VertexOutput {
    vec2 texCoords;
    vec4 color;
} fragIn;

void main()
{
    vec4 texColor = texture(texture0, fragIn.texCoords);
    outColor = fragIn.color * texColor;
}
)";

struct SpriteVertex {
    vec3 position;
    vec2 texCoords;
    vec4 color;
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
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    GlVertexFormat spriteFormat {
        {3, GL_FLOAT},
        {2, GL_FLOAT},
        {4, GL_FLOAT}
    };

    _spriteVertexArray = std::make_unique<GlVertexArray>(spriteFormat, 100000, GlBufferUsage::Dynamic);

    _spriteProgram = std::make_unique<GlProgram>(SPRITE_VERTEX_SHADER, SPRITE_FRAGMENT_SHADER);
    _spriteProgram->setUniform(GlStandardUniform::ProjectionMatrix,
                               glm::ortho(0.0f, static_cast<r32>(_width), static_cast<r32>(_height), 0.0f));
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

            glClearColor(cmd->color.r, cmd->color.g, cmd->color.b, cmd->color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

static void drawRect(rect source, rect dest, vec4 color, GlTexture *texture, SpriteVertex *v)
{
    r32 tw = static_cast<r32>(texture->width());
    r32 th = static_cast<r32>(texture->height());

    r32 tl = source.left/ tw;
    r32 tr = source.right / tw;
    r32 tt = source.top / th;
    r32 tb = source.bottom / th;

    v->position = {dest.left, dest.top, 0.0f};
    v->texCoords = {tl, tt};
    v->color = color;

    ++v;
    v->position = {dest.right, dest.bottom, 0.0f};
    v->texCoords = {tr, tb};
    v->color = color;

    ++v;
    v->position = {dest.right, dest.top, 0.0f};
    v->texCoords = {tr, tt};
    v->color = color;

    ++v;
    v->position = {dest.left, dest.top, 0.0f};
    v->texCoords = {tl, tt};
    v->color = color;

    ++v;
    v->position = {dest.right, dest.bottom, 0.0f};
    v->texCoords = {tr, tb};
    v->color = color;

    ++v;
    v->position = {dest.left, dest.bottom, 0.0f};
    v->texCoords = {tl, tb};
    v->color = color;


}

static void drawTile(u32 tileX, u32 tileY, vec2 pos, vec4 color, GlTexture *texture, SpriteVertex *v)
{
    rect dest = {pos.x, pos.x + 32.0f, pos.y, pos.y + 32.0f};
    rect source = {tileX * 32.0f, (tileX * 32.0f) + 32.0f, tileY * 32.0f, (tileY * 32.0f) + 32.0f};

    drawRect(source, dest, color, texture, v);
}

void GlRenderer::present()
{
    flush();

    glUseProgram(_spriteProgram->id());

    auto v = _spriteVertexArray->mapVertices<SpriteVertex>();
    auto *texture = &_textures[0];

    int vertexCount = 0;
    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            drawTile(x % 24, y % 48, {x * 32, y * 32}, {1.0, 1.0f, 1.0f, 1.0f}, texture, v);

            vertexCount += 6;
            v += 6;
        }
    }

    _spriteVertexArray->unmapVertices();

    glUseProgram(_spriteProgram->id());
    texture->bind();
    _spriteVertexArray->draw(0, vertexCount);

    _context->present();
}

u32 GlRenderer::loadTexture(const Image &image)
{
    _textures.emplace_back(image);

    return static_cast<u32>(_textures.size());
}

}
