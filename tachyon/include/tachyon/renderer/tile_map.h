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
#include "tachyon/core/non_copyable.h"

namespace tachyon {

class TileSet : private NonCopyable {
public:

    u32 tileWidth() const {
        return _tileWidth;
    }

    u32 tileHeight() const {
        return _tileHeight;
    }

    u32 tileCount() const {
        return _tileCount;
    }

    u32 columns() const {
        return _columns;
    }

private:

    u32 _tileWidth;

    u32 _tileHeight;

    u32 _tileCount;

    u32 _columns;

};

class TileLayer : private NonCopyable {
public:

    TileLayer(u32 width, u32 height);

    u32 width() const {
        return _width;
    }

    u32 height() const {
        return _height;
    }

private:

    u32 _width;

    u32 _height;

};

class TileMap : private NonCopyable {
public:

    explicit TileMap(const char *path);

private:

    u32 _width;

    u32 _height;

    u32 _tileWidth;

    u32 _tileHeight;

    std::vector<TileLayer> _layers;

};


}
