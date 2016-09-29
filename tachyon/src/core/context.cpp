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

namespace tachyon {

Context::Context(std::unique_ptr<Renderer> renderer)
    : _renderer {std::move(renderer)}
{

    auto texture = _renderer->loadTexture(Image{"assets/textures/tiles.png"});
}

void Context::frame(r32 dt)
{
    static r32 t = 0.0f;
    t += dt;

    auto &commands = _renderer->commandBuffer();
    commands.push<ClearCommand>(std::sinf(t * 0.2f), std::sinf(t * 0.29f), std::sinf(t * 0.17f));

    _renderer->present();
}

}
