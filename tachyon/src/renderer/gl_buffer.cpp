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

#include "tachyon/renderer/gl_buffer.h"

#include <cassert>

#include "tachyon/core/exception.h"

namespace tachyon {

GlBuffer::GlBuffer(GLenum type, u32 capacity, GlBufferUsage usage)
    : _type {type},
      _capacity {capacity}
{
    glGenBuffers(1, &_id);

    glBindBuffer(type, _id);
    glBufferData(type, capacity, nullptr, usage == GlBufferUsage::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

GlBuffer::~GlBuffer()
{
    glDeleteBuffers(1, &_id);
}

void GlBuffer::add(void *data, u32 size)
{
    assert((_offset + size) <= _capacity);

    glBindBuffer(_type, _id);
    glBufferSubData(_type, _offset, size, data);

    _offset += size;
}

void GlBuffer::clear()
{
    _offset = 0;
}

}
