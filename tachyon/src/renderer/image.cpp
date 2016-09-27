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

#include "tachyon/renderer/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "tachyon/core/exception.h"

namespace tachyon {

Image::Image(const char *path)
{
    int x;
    int y;
    int n;

    std::unique_ptr<u8> data {stbi_load(path, &x, &y, &n, 4)};
    if (!data) {
        TACHYON_THROW("unable to load image from file '%s': %s", path, stbi_failure_reason());
    }

    _width = static_cast<u32>(x);
    _height = static_cast<u32>(y);

    _pixels.resize(x * y * 4);
    memcpy(&_pixels[0], data.get(), _pixels.size());
}

Image::Image(u32 width, u32 height)
    : _width {width},
      _height {height},
      _pixels(width * height * 4)
{
}

}
