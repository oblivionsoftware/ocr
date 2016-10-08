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

#include "tachyon/core/context.h"

#include <cmath>

#include "tachyon/core/exception.h"
#include "tachyon/renderer/command_buffer.h"
#include "tachyon/renderer/image.h"
#include "tachyon/renderer/renderer.h"
#include "tachyon/renderer/tile_map.h"

namespace tachyon {

static u32 texture;

static std::unique_ptr<TileMap> _map;

Context::Context(std::unique_ptr<Renderer> renderer)
    : _renderer {std::move(renderer)}
{
    texture = _renderer->loadTexture(Image{"assets/textures/tiles.png"});
    _map.reset(new TileMap("assets/maps/world.tmx"));
}

void Context::frame(r32 dt)
{
    auto &commands = _renderer->commandBuffer();
    commands.push<ClearCommand>(vec4(0.2f, 0.2f, 0.2f, 1.0f));

    const auto ts = 32.0f;

    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            rect source {x * ts, (x * ts) + ts, y * ts, (y * ts) + ts};
            rect dest {x * ts, (x * ts) + ts, y * ts, (y * ts) + ts};

            commands.push<DrawSprite>(texture, source, dest);
        }
    }

    _renderer->present();
}

}
