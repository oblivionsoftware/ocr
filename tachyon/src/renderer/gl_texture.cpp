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

#include "tachyon/renderer/gl_texture.h"

#include "tachyon/core/exception.h"
#include "tachyon/renderer/image.h"

namespace tachyon {

GlTexture::GlTexture(const Image &image)
    : _width {image.width()},
      _height {image.height()}
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

GlTexture::GlTexture(GlTexture &&texture)
    : _id {texture._id},
      _width {texture._width},
      _height {texture._height}
{
    texture._id = 0;
}

GlTexture::~GlTexture()
{
    if (_id > 0) {
        glDeleteTextures(1, &_id);
    }
}

void GlTexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _id);
}

}
