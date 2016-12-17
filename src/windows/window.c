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

#include "ocr/window.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ocr/log.h"

struct ocr_window {
    HWND handle;
    bool closed;
};

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    ocr_window_t *window = (ocr_window_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {

    case WM_CLOSE:
        window->closed = true;
        return 0;

    default:
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}


ocr_window_t *ocr_window_create(ocr_pool_t *pool, ocr_window_settings_t *settings)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX window_class = {
        .cbSize = sizeof(window_class),
        .lpfnWndProc = window_proc,
        .hInstance = hInstance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = GetStockObject(BLACK_BRUSH),
        .lpszClassName = "ocr_window",
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW
    };

    if (!RegisterClassEx(&window_class)) {
        OCR_ERROR("failed to register window class");
        return NULL;
    }

    RECT rect = {
        .left = 0,
        .top = 0,
        .right = settings->width,
        .bottom = settings->height
    };
    AdjustWindowRect(&rect, WS_OVERLAPPED, false);

    HWND handle = CreateWindowEx(0, "ocr_window", settings->title, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
                                 rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

    if (!handle) {
        OCR_ERROR("failed to create window");
        return NULL;
    }

    UpdateWindow(handle);
    ShowWindow(handle, SW_SHOW);

    ocr_window_t *window = ocr_pool_alloc(pool, sizeof(*window));
    window->closed = false;
    window->handle = handle;

    SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)window);

    return window;
}


void ocr_window_destroy(ocr_window_t *window)
{
    DestroyWindow(window->handle);
}


void ocr_window_do_events(ocr_window_t *window)
{
    MSG message;
    while (PeekMessage(&message, window->handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}


bool ocr_window_is_closed(ocr_window_t *window)
{
    return window->closed;
}
