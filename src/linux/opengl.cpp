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

#include "ocr/opengl.h"

#include "ocr/linux/window.h"
#include "ocr/log.h"

namespace ocr {

struct GlContext::Impl {
    GLXContext context;
    ::Window x11_window;
    Display *display;
};

GlContext::GlContext(const Window& window)
        : _impl{new Impl()} {

    auto display = window._impl->display;
    auto x11_window = window._impl->window;

    int fb_config_count = 0;
    GLXFBConfig *fb_configs = glXGetFBConfigs(display, 0, &fb_config_count);

    int visual_id = 0;
    GLXFBConfig fb_config = fb_configs[0];
    glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID, &visual_id);

    GLXContext context = glXCreateNewContext(display, fb_config, GLX_RGBA_TYPE, 0, true);

    if (!glXMakeContextCurrent(display, x11_window, x11_window, context)) {
        throw std::runtime_error("unable to make opengl context current");
    }

    if (gl3wInit() != 0) {
        OCR_ERROR("unable to load gl extensions");
        throw std::runtime_error("gl3wInit failed");
    }

    _impl->x11_window = x11_window;
    _impl->display = display;
    _impl->context = context;
}

GlContext::~GlContext() {
    glXDestroyContext(_impl->display, _impl->context);
}


void GlContext::present() {
    glXSwapBuffers(_impl->display, _impl->x11_window);
}

}
