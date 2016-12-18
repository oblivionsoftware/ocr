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

#include <ocr/ocr.h>

#include "tachyon/core/math_fwd.h"
#include "tachyon/core/non_copyable.h"

namespace tachyon {

class GlFragmentShader;
class GlVertexShader;

enum class GlStandardUniform {
    ProjectionMatrix,

    Count
};

class GlProgram : private NonCopyable {
public:

    GlProgram(const char *vertexShaderText, const char *fragmentShaderText);

    GlProgram(const GlVertexShader &vertexShader, const GlFragmentShader &fragmentShader);

    ~GlProgram();

    GLuint id() const {
        return _id;
    }

    void bind() {
        glUseProgram(_id);
    }

    void setUniform(GlStandardUniform uniform, mat4 value);

private:

    GLuint _id;

    std::array<GLint, static_cast<size_t>(GlStandardUniform::Count)> _standardUniforms;

};

}
