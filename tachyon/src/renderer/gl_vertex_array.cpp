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

#include "tachyon/renderer/gl_vertex_array.h"

#include "tachyon/core/exception.h"

namespace tachyon {

static inline void *bufferOffset(size_t offset)
{
    return reinterpret_cast<void*>(offset);
}

GlVertexElement::GlVertexElement(u32 count, GLenum type)
    : _count {count},
      _type {type}
{
}

static u32 typeSize(GLenum type)
{
    switch (type) {
    case GL_FLOAT:
        return 4;
    default:
        TACHYON_THROW("unsupported vertex element type: %d", type);
    }
}

u32 GlVertexElement::size() const
{
    return _count * typeSize(_type);
}

GlVertexFormat::GlVertexFormat(std::initializer_list<GlVertexElement> elements)
    : _elements {elements}
{
}

u32 GlVertexFormat::size() const
{
    u32 size = 0;
    for (auto &element : _elements) {
        size += element.size();
    }

    return size;
}

GlVertexArray::GlVertexArray(const GlVertexFormat &format, u32 maxVertexCount, GlBufferUsage usage)
    : _vertexBuffer {GL_ARRAY_BUFFER, format.size() * maxVertexCount, usage},
      _maxVertexCount {maxVertexCount}
{
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);

    _vertexBuffer.bind();

    u32 offset = 0;
    u32 index = 0;

    for (auto &element : format.elements()) {
        glVertexAttribPointer(index, element.count(), element.type(), GL_FALSE, format.size(), bufferOffset(offset));
        glEnableVertexAttribArray(index);

        offset += element.size();
        ++index;
    }
}

GlVertexArray::~GlVertexArray()
{
    glDeleteVertexArrays(1, &_id);
}

void GlVertexArray::bind()
{
    glBindVertexArray(_id);
}

void GlVertexArray::draw(u32 offset, u32 vertexCount)
{
    bind();
    glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}

void GlVertexArray::flush()
{
    if (_vertices) {
        _vertexBuffer.unmap();
        _vertices = nullptr;
    }

    draw(0, _vertexCount);

    _vertexCount = 0;
}

}
