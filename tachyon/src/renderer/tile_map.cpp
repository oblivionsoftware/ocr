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

#include "tachyon/renderer/tile_map.h"

#include "tinyxml2.h"

#include "tachyon/core/exception.h"

namespace tachyon {

TileLayer::TileLayer(u32 width, u32 height)
    : _width {width},
      _height {height}
{
}

TileMap::TileMap(const char *path)
{
    using namespace tinyxml2;

    XMLDocument document;
    if (document.LoadFile(path) != 0) {
        TACHYON_THROW("unable to load tile map file '%s'", path);
    }

    XMLElement *root = document.RootElement();
    _width = root->IntAttribute("width");
    _height = root->IntAttribute("height");
    _tileWidth = root->IntAttribute("tilewidth");
    _tileHeight = root->IntAttribute("tileheight");
}

}
