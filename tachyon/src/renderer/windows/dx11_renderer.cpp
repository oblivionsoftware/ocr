
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

#pragma comment(lib, "d3d11.lib")

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
    sd.BufferDesc.RefreshRate.Numerator = 144;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                             nullptr, 0, D3D11_SDK_VERSION, &sd, &_swapChain,
                                             &_device, nullptr, &_deviceContext))) {

        TACHYON_THROW("error initializing DirectX11");
    }

    ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer))) {
        TACHYON_THROW("unable to get back buffer");
    }

    if (FAILED(_device->CreateRenderTargetView(backBuffer.get(), nullptr, &_renderTargetView))) {
        TACHYON_THROW("error creating render target view");
    }
    backBuffer->Release();

    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, nullptr);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(viewport));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<r32>(width);
    viewport.Height = static_cast<r32>(height);

    _deviceContext->RSSetViewports(1, &viewport);
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
            auto cmd {itr.command<ClearCommand>()};

            _deviceContext->ClearRenderTargetView(_renderTargetView.get(), glm::value_ptr(cmd->color));
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

    _swapChain->Present(0, 0);
}

u32 DX11Renderer::loadTexture(const Image &image)
{
    return 0;
}

}
