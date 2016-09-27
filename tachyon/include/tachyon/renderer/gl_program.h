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

#include "tachyon/core/non_copyable.h"
#include "tachyon/renderer/opengl.h"

namespace tachyon {

class GlFragmentShader;
class GlVertexShader;

class GlProgram : private NonCopyable {
public:

    GlProgram(const char *vertexShaderText, const char *fragmentShaderText);

    GlProgram(const GlVertexShader &vertexShader, const GlFragmentShader &fragmentShader);

    ~GlProgram();

    GLuint id() const {
        return _id;
    }

private:

    GLuint _id;

};

}
