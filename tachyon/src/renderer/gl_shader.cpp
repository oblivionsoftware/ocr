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

#include "tachyon/renderer/gl_shader.h"

#include "tachyon/core/exception.h"

namespace tachyon {

GlShader::GlShader(GLenum type, const char *text)
{
    _id = glCreateShader(type);
    if (!_id) {
        TACHYON_THROW("glCreateShader failed");
    }

    glShaderSource(_id, 1, (const char**)&text, nullptr);
    glCompileShader(_id);

    GLint status;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        char buffer[512];
        glGetShaderInfoLog(_id, sizeof(buffer), nullptr, buffer);

        glDeleteShader(_id);
        TACHYON_THROW("shader compilation failed: %s", buffer);
    }
}

GlShader::~GlShader()
{
    glDeleteShader(_id);
}

GlVertexShader::GlVertexShader(const char *text)
    : GlShader {GL_VERTEX_SHADER, text}
{
}


GlFragmentShader::GlFragmentShader(const char *text)
    : GlShader {GL_FRAGMENT_SHADER, text}
{
}

}
