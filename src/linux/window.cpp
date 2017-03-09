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

#include "ocr/linux/window.h"

#include <string.h>

#include "ocr/log.h"
#include "ocr/opengl.h"

namespace ocr {

Window::Window(const WindowSettings &settings)
        : _impl{new Impl()} {

    Display *display = XOpenDisplay(NULL);
    if (!display) {
        throw std::runtime_error("error opening display");
    }

    GLint attributes[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER,
        None
    };

    XVisualInfo *vi = glXChooseVisual(display, 0, attributes);
    if (vi == NULL) {
        throw std::runtime_error("failed to choose visual info");
    }

    ::Window root = DefaultRootWindow(display);

    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    swa.event_mask = KeyPressMask;

    ::Window x11_window = XCreateWindow(display, root, 0, 0, settings.width, settings.height,
                               0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XSizeHints hints;
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = settings.width;
    hints.min_height = settings.height;
    hints.max_width = settings.width;
    hints.max_height = settings.height;
    XSetWMNormalHints(display, x11_window, &hints);

    Atom delete_message = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, x11_window, &delete_message, 1);

    XMapWindow(display, x11_window);
    XStoreName(display, x11_window, settings.title);

    _impl->display = display;
    _impl->closed = false;
    _impl->window = x11_window;
    _impl->delete_message = delete_message;
}


Window::~Window() {
    XCloseDisplay(_impl->display);
}


void Window::doEvents() {
    XEvent event;
    while (XPending(_impl->display)) {
        XNextEvent(_impl->display, &event);

        switch (event.type) {

            case ClientMessage:
                if (static_cast<Atom>(event.xclient.data.l[0]) == _impl->delete_message) {
                    _impl->closed = true;
                }
                break;

        }
    }

}

bool Window::isClosed() {
    return _impl->closed;
}

}
