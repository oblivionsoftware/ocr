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

#include <xcb/xcb_icccm.h>

#include "ocr/log.h"


static xcb_screen_t *get_default_screen(xcb_connection_t *connection)
{
    xcb_screen_iterator_t itr = xcb_setup_roots_iterator(xcb_get_setup(connection));
    return itr.data;
}


ocr_window_t *ocr_window_create(ocr_pool_t *pool, ocr_window_settings_t *settings)
{
    Display *display = XOpenDisplay(0);
    if (!display) {
        OCR_ERROR("error opening display");
        return NULL;
    }

    xcb_connection_t *xcb_connection = XGetXCBConnection(display);
    if (!xcb_connection) {
        OCR_ERROR("error getting XCB connection");
        goto out_display;
    }

    XSetEventQueueOwner(display, XCBOwnsEventQueue);

    xcb_screen_t *screen = get_default_screen(xcb_connection);
    xcb_window_t xcb_window = xcb_generate_id(xcb_connection);

    u32 event_mask = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    u32 value_list[] = {event_mask, 0};
    u32 value_mask = XCB_CW_EVENT_MASK;

    xcb_create_window(xcb_connection,
                      XCB_COPY_FROM_PARENT,
                      xcb_window, screen->root,
                      0, 0, settings->width, settings->height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, value_mask, value_list);

    xcb_size_hints_t hints;
    xcb_icccm_size_hints_set_min_size(&hints, settings->width, settings->height);
    xcb_icccm_size_hints_set_max_size(&hints, settings->width, settings->height);

    xcb_icccm_set_wm_size_hints(xcb_connection, xcb_window, XCB_ATOM_WM_NORMAL_HINTS, &hints);

    xcb_map_window(xcb_connection, xcb_window);
    xcb_flush(xcb_connection);

    ocr_window_t *window = ocr_pool_alloc(pool, sizeof(*window));
    window->display = display;
    window->closed = false;
    window->xcb_connection = xcb_connection;
    window->xcb_window = xcb_window;

    return window;

out_display:
    XCloseDisplay(display);

    return NULL;
}


void ocr_window_destroy(ocr_window_t *window)
{
    xcb_destroy_window(window->xcb_connection, window->xcb_window);
    XCloseDisplay(window->display);
}


void ocr_window_do_events(ocr_window_t *window)
{
    xcb_generic_event_t *event = xcb_poll_for_event(window->xcb_connection);
    while (event) {
        switch (event->response_type & ~0x80) {

        case XCB_UNMAP_NOTIFY:
            window->closed = true;
            break;

        }

        free(event);
        event = xcb_poll_for_event(window->xcb_connection);
    }
}


bool ocr_window_is_closed(ocr_window_t *window)
{
    return window->closed;
}
