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

#include <vector>

#include "tachyon/core/common.h"

namespace tachyon {

class Image {
public:

    /**
     * Loads an image from a file.
     *
     * @param path The path to the file.
     */
    explicit Image(const char *path);

    /**
     * Constructs an empty image of the specified dimensions.
     *
     * @param width The image width.
     * @param height The image height.
     */
    Image(u32 width, u32 height);

    /**
     * Gets the image width.
     *
     * @return The width.
     */
    u32 width() const {
        return _width;
    }

    /**
     * Gets the image height.
     *
     * @return The height.
     */
    u32 height() const {
        return _height;
    }

    /**
     * Gets a writeable pointer to the pixel data.
     *
     * @return The pixels.
     */
    u8 *pixels() {
        return &_pixels[0];
    }

    /**
     * Gets a read-only pointer to the pixel data.
     *
     * @return The pixels.
     */
    const u8 *pixels() const {
        return &_pixels[0];
    }

private:

    u32 _width;

    u32 _height;

    std::vector<u8> _pixels;

};

}
