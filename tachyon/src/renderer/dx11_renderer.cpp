
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

#include "tachyon/renderer/dx11_renderer.h"

#include "tachyon/core/exception.h"
#include "tachyon/core/math.h"
#include "tachyon/renderer/command_buffer.h"

namespace tachyon {

struct SpriteVertex {
    vec3 position;
    vec2 texCoords;
    vec4 color;
};

DX11Renderer::DX11Renderer(HWND hwnd, u32 width, u32 height)
    : _commandBuffer {1024 * 1024 * 10},
      _hwnd {hwnd},
      _width {width},
      _height {height}
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;
}

DX11Renderer::~DX11Renderer()
{
}

void DX11Renderer::flush()
{
    CommandIterator itr {_commandBuffer};
    while (itr.next()) {
       switch (itr.type()) {

        case CommandType::Clear: {
        } break;

        case CommandType::DrawSprite: {
        } break;

        default:
            TACHYON_THROW("unsupported command: %d", itr.type());
        }
    }

    _commandBuffer.clear();
}

void DX11Renderer::present()
{
    flush();
}

u32 DX11Renderer::loadTexture(const Image &image)
{
    return 0;
}

}
