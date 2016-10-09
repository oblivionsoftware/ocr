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

#include <sstream>

#include "tinyxml2.h"

#include "tachyon/core/exception.h"
#include "tachyon/renderer/image.h"
#include "tachyon/renderer/renderer.h"

namespace tachyon {

TileSet::TileSet(u32 texture, u32 tileWidth, u32 tileHeight, u32 tileCount, u32 columns)
    : _texture {texture},
      _tileWidth {tileWidth},
      _tileHeight {tileHeight},
      _tileCount {tileCount},
      _columns {columns}
{
}

TileSet::TileSet(TileSet &&other)
    : _texture {other._texture},
      _tileWidth {other._tileWidth},
      _tileHeight {other._tileHeight},
      _tileCount {other._tileCount},
      _columns {other._columns}
{
}

TileLayer::TileLayer(u32 width, u32 height)
    : _width {width},
      _height {height}
{
}

TileLayer::TileLayer(TileLayer &&other)
    : _width {other._width},
      _height {other._height},
      _tiles {other._tiles}
{
    other._tiles.clear();
}

void TileLayer::render(Renderer &renderer)
{
}

TileMap::TileMap(const char *path, Renderer &renderer)
{
    using namespace tinyxml2;

    XMLDocument document;
    if (document.LoadFile(path) != 0) {
        TACHYON_THROW("unable to load tile map file '%s'", path);
    }

    auto root {document.RootElement()};
    _width = root->IntAttribute("width");
    _height = root->IntAttribute("height");
    _tileWidth = root->IntAttribute("tilewidth");
    _tileHeight = root->IntAttribute("tileheight");

    {
        auto tileSet {root->FirstChildElement("tileset")};
        while (tileSet) {
            u32 tileWidth = tileSet->IntAttribute("tilewidth");
            u32 tileHeight = tileSet->IntAttribute("tileheight");
            u32 tileCount = tileSet->IntAttribute("tilecount");
            u32 columns = tileSet->IntAttribute("columns");

            auto image = tileSet->FirstChildElement("image");
            auto imagePath = std::string("assets/maps/") + image->Attribute("source");

            u32 texture = renderer.loadTexture(Image{imagePath.c_str()});

            _tileSets.emplace_back(texture, tileWidth, tileHeight, tileCount, columns);

            tileSet = tileSet->NextSiblingElement("tileset");
        }
    }

    {
        auto layer {root->FirstChildElement("layer")};
        while (layer) {
            u32 width = layer->IntAttribute("width");
            u32 height = layer->IntAttribute("height");

            TileLayer tileLayer {width, height};

            auto data = layer->FirstChildElement("data");
            if (data) {
                std::stringstream ss {data->GetText()};

                std::string tile;
                while (std::getline(ss, tile, ',')) {
                    tileLayer.addTile(atoi(tile.c_str()));
                }
            }

            _layers.emplace_back(std::move(tileLayer));

            layer = layer->NextSiblingElement("layer");
        }
    }
}

void TileMap::render(Renderer &renderer)
{
}

}
