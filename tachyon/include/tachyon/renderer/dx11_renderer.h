
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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11_1.h>

#include <memory>
#include <vector>

#include "tachyon/core/common.h"
#include "tachyon/core/non_copyable.h"
#include "tachyon/renderer/command_buffer.h"
#include "tachyon/renderer/renderer.h"

namespace tachyon {

class DX11Renderer : public Renderer, private NonCopyable {
public:

    DX11Renderer(HWND hwnd, u32 width, u32 height);

    ~DX11Renderer();

    virtual void flush() override;

    virtual void present() override;

    virtual CommandBuffer &commandBuffer() override {
        return _commandBuffer;
    }

    virtual u32 loadTexture(const Image &image) override;

private:

    CommandBuffer _commandBuffer;

    HWND _hwnd;

    u32 _width;

    u32 _height;

    ID3D11Device *_device;

    IDXGISwapChain *_swapChain;

    ID3D11RenderTargetView *_renderTargetView;

};

}
