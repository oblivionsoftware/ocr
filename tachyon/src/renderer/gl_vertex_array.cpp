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

u32 GlVertexFormat::size() const
{
    u32 size = 0;
    for (auto &element : _elements) {
        size += element.size();
    }

    return size;
}

GlVertexArray::GlVertexArray(const GlVertexFormat &format)
{
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);

    u32 offset = 0;
    for (auto i = 0; i < format.elements().size(); ++i) {
        auto &element = format.elements()[i];

        glVertexAttribPointer(i, element.count(), element.type(), GL_FALSE, format.size(), bufferOffset(offset));
        glEnableVertexAttribArray(i);

        offset += element.size();
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

}
