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

#include <memory>

#include "tachyon/core/common.h"
#include "tachyon/core/non_copyable.h"
#include "tachyon/renderer/renderer.h"

namespace tachyon {

class GlContext : private NonCopyable {
public:

    GlContext() = default;

    virtual ~GlContext() = default;

    virtual void present() = 0;

};

class GlRenderer : public Renderer, private NonCopyable {
public:

    GlRenderer(std::unique_ptr<GlContext> context, u32 width, u32 height);

    virtual void present() override;

private:

    std::unique_ptr<GlContext> _context;

    u32 _width;

    u32 _height;

};

}
