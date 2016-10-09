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

class Renderer;

class TileSet : private NonCopyable {
public:

    TileSet(u32 texture, u32 tileWidth, u32 tileHeight, u32 tileCount, u32 columns);

    TileSet(TileSet &&other);

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

    u32 _texture;

    u32 _tileWidth;

    u32 _tileHeight;

    u32 _tileCount;

    u32 _columns;

};

class TileLayer : private NonCopyable {
public:

    TileLayer(u32 width, u32 height);

    TileLayer(TileLayer &&other);

    u32 width() const {
        return _width;
    }

    u32 height() const {
        return _height;
    }

    void addTile(u32 tile) {
        _tiles.push_back(tile);
    }

    void render(Renderer &renderer);

private:

    u32 _width;

    u32 _height;

    std::vector<u32> _tiles;

};

class TileMap : private NonCopyable {
public:

    TileMap(const char *path, Renderer &renderer);

    void render(Renderer &renderer);

private:

    u32 _width;

    u32 _height;

    u32 _tileWidth;

    u32 _tileHeight;

    std::vector<TileSet> _tileSets;

    std::vector<TileLayer> _layers;

};


}
