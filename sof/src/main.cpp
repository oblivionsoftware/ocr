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

#include <cstdio>
#include <cstdlib>

#include <tachyon/tachyon.h>
#include <tachyon/platform/ocr_platform.h>

#if OCR_PLATFORM == OCR_PLATFORM_WINDOWS
    int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
    int main(int argc, char **argv)
#endif
{
    using namespace tachyon;

    try {
        OcrPlatform platform("Seeds of Fate", RendererType::OpenGL, 1280, 720);
        platform.run();

        return EXIT_SUCCESS;
    } catch (Exception &ex) {
        fprintf(stderr, "unhandled exception: %s\n", ex.what());
        return EXIT_FAILURE;
    }
}
