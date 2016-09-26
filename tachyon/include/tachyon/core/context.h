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

#include "tachyon/core/common.h"

namespace tachyon {

class Renderer;

/**
 * The central engine context class.
 */
class Context {
public:

    /**
     * Initializes the engine context.
     *
     * @param renderer The renderer instance.
     */
    explicit Context(Renderer *renderer);

    /**
     * Cleans up engine resources.
     */
    ~Context();

    /**
     * Invoked once per frame to frame the engine.
     *
     * @param dt The amount of elapsed time (in seconds).
     */
    void frame(r32 dt);

private:

    Renderer *_renderer;

};

}
