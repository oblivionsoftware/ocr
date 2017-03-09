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

#include "ocr/common.h"

#include <GL/gl3w.h>

#if OCR_PLATFORM == OCR_PLATFORM_WINDOWS
    #include "ocr/windows.h"
    #include <gl/GL.h>
    #include <gl/GLU.h>

#elif OCR_PLATFORM == OCR_PLATFORM_MAC
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glx.h>
    #include <GL/glu.h>
#endif

namespace ocr {

class Window;

class GlContext {
public:

    GlContext(const Window &window);

    ~GlContext();

    void present();

private:

    class Impl;
    std::unique_ptr<Impl> _impl;

};

}
