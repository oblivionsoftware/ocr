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

#include "ocr/windows/window.h"

#include "ocr/log.h"

namespace ocr {

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    Window::Impl *impl = reinterpret_cast<Window::Impl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg) {

    case WM_CLOSE:
        impl->markClosed();
        return 0;

    default:
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}

Window::Impl::Impl(const WindowSettings &settings)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX window_class;
    ZeroMemory(&window_class, sizeof(window_class));
    window_class.cbSize = sizeof(window_class);
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = hInstance;
    window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    window_class.lpszClassName = "ocr_window";
    window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&window_class)) {
        throw std::runtime_error("failed to register window class");
    }

    RECT rect;
    ZeroMemory(&rect, sizeof(rect));
    rect.right = settings.width;
    rect.bottom = settings.height;

    AdjustWindowRect(&rect, WS_OVERLAPPED, false);

    _handle = CreateWindowEx(0, "ocr_window", settings.title, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
                                 rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

    if (!_handle) {
        throw std::runtime_error("failed to create window");
    }

    UpdateWindow(_handle);
    ShowWindow(_handle, SW_SHOW);

    SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

Window::Impl::~Impl()
{
    DestroyWindow(_handle);
}

void Window::Impl::doEvents()
{
    MSG message;
    while (PeekMessage(&message, _handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

bool Window::Impl::isClosed() const
{
    return _closed;
}

HWND Window::Impl::handle() const
{
    return _handle;
}

void Window::Impl::markClosed()
{
    _closed = true;
}

Window::Window(const WindowSettings &settings)
    : _impl{std::make_unique<Impl>(settings)}
{
}

Window::~Window()
{
}

void Window::doEvents()
{
    _impl->doEvents();
}


bool Window::isClosed() const
{
    return _impl->isClosed();
}

}
