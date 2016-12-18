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

#pragma once

#include <array>
#include <vector>

#include <ocr/ocr.h>

#include "tachyon/core/common.h"
#include "tachyon/core/non_copyable.h"
#include "tachyon/renderer/gl_buffer.h"

namespace tachyon {

class GlVertexElement {
public:

    GlVertexElement(u32 count, GLenum type);

    u32 size() const;

    u32 count() const {
        return _count;
    }

    GLenum type() const {
        return _type;
    }

private:

    u32 _count;

    GLenum _type;

};

class GlVertexFormat {
public:

    GlVertexFormat(std::initializer_list<GlVertexElement> elements);

    u32 size() const;

    const std::vector<GlVertexElement> &elements() const {
        return _elements;
    }

private:

    std::vector<GlVertexElement> _elements;

};

class GlVertexArray : private NonCopyable {
public:

    GlVertexArray(const GlVertexFormat &format, u32 maxVertexCount, GlBufferUsage usage);

    ~GlVertexArray();

    GLuint id() const {
        return _id;
    }

    void bind();

    void draw(u32 offset, u32 vertexCount);

    template <typename T, size_t N>
    void addVertices(const std::array<T, N> &vertices);

    void flush();

private:

    GLuint _id;

    GlBuffer _vertexBuffer;

    u32 _maxVertexCount;

    u32 _vertexCount {0};

    u8 *_vertices {nullptr};

};

template <typename T, size_t N>
void GlVertexArray::addVertices(const std::array<T, N> &vertices) {
    if ((_vertexCount + N) >= _maxVertexCount) {
        flush();
    }

    if (!_vertices) {
        _vertices = reinterpret_cast<u8*>(_vertexBuffer.map());
    }

    size_t size = sizeof(T) * N;

    memcpy(_vertices, &vertices[0], size);
    _vertices += size;
    _vertexCount += N;
}

}
