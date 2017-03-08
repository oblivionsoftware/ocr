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


ocr_window_t *ocr_window_create(ocr_pool_t *pool, ocr_window_settings_t *settings)
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        OCR_ERROR("error opening display");
        return NULL;
    }

    GLint attributes[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER,
        None
    };

    XVisualInfo *vi = glXChooseVisual(display, 0, attributes);
    if (vi == NULL) {
        goto out_display;
    }

    Window root = DefaultRootWindow(display);

    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    swa.event_mask = KeyPressMask;

    Window x11_window = XCreateWindow(display, root, 0, 0, settings->width, settings->height,
                                      0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XSizeHints hints = {
        .flags = PMinSize | PMaxSize,
        .min_width = settings->width,
        .min_height = settings->height,
        .max_width = settings->width,
        .max_height = settings->height
    };
    XSetWMNormalHints(display, x11_window, &hints);

    Atom delete_message = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, x11_window, &delete_message, 1);

    XMapWindow(display, x11_window);
    XStoreName(display, x11_window, settings->title);

    ocr_window_t *window = ocr_pool_alloc(pool, sizeof(*window));
    window->display = display;
    window->closed = false;
    window->window = x11_window;
    window->delete_message = delete_message;

    return window;

out_display:
    XCloseDisplay(display);

    return NULL;
}


void ocr_window_destroy(ocr_window_t *window)
{
    XCloseDisplay(window->display);
}


void ocr_window_do_events(ocr_window_t *window)
{
    XEvent event;
    while (XPending(window->display)) {
        XNextEvent(window->display, &event);

        switch (event.type) {

        case ClientMessage:
            if (event.xclient.data.l[0] == window->delete_message) {
                window->closed = true;
            }
            break;

        }
    }

}


bool ocr_window_is_closed(ocr_window_t *window)
{
    return window->closed;
}
