
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

#include "tachyon/renderer/gl_program.h"

#include "tachyon/core/exception.h"
#include "tachyon/renderer/gl_shader.h"

namespace tachyon {

GlProgram::GlProgram(const char *vertexShaderText, const char *fragmentShaderText)
    : GlProgram {GlVertexShader {vertexShaderText}, GlFragmentShader {fragmentShaderText}}
{
}

GlProgram::GlProgram(const GlVertexShader &vertexShader, const GlFragmentShader &fragmentShader)
{
    _id = glCreateProgram();
    if (!_id) {
        TACHYON_THROW("glCreateProgram failed");
    }

    glAttachShader(_id, vertexShader.id());
    glAttachShader(_id, fragmentShader.id());
    glLinkProgram(_id);

    GLint status;
    glGetProgramiv(_id, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        char buffer[512];
        glGetProgramInfoLog(_id, sizeof(buffer), nullptr, buffer);

        glDeleteProgram(_id);
        TACHYON_THROW("program linking failed: %s", buffer);
    }
}

GlProgram::~GlProgram()
{
    glDeleteProgram(_id);
}

}