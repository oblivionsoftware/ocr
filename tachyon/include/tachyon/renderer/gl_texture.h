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

#include "tachyon/core/common.h"
#include "tachyon/core/non_copyable.h"
#include "tachyon/renderer/opengl.h"

namespace tachyon {

class Image;


class GlTexture : private NonCopyable {
public:

    explicit GlTexture(const Image &image);

    GlTexture(GlTexture &&texture);

    ~GlTexture();

    GLuint id() const {
        return _id;
    }

    u32 width() const {
        return _width;
    }

    u32 height() const {
        return _height;
    }

    void bind();

private:

    GLuint _id;

    u32 _width;

    u32 _height;

};

}
