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

static std::unique_ptr<TileMap> _map;

Context::Context(std::unique_ptr<Renderer> renderer)
    : _renderer {std::move(renderer)}
{
    _map.reset(new TileMap("assets/maps/world.tmx", *_renderer));
}

void Context::frame(r32 dt)
{
    auto &commands = _renderer->commandBuffer();
    commands.push<ClearCommand>(ocr_vec4(0.2f, 0.2f, 0.2f, 1.0f));

    _map->render(*_renderer, {0.0f, 0.0f});

    _renderer->present();
}

}
